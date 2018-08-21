#include "stdafx.h"
#include "emccdwindow.h"

EMCCDWindow::EMCCDWindow(QWidget *parent)
	: QOpenGLWidget(parent),
	guideColor(new QColor(211, 211, 211, 75)),
	start(NULL),
	end(NULL),
	gblX(0),
	gblY(0),
	//	pixelspermicron will always be ceil(actual pixels per micron) 
	//	because actual pixels per micron could be floating point value, 
	//	which is obviously impossible to render on a screen.
	pixelspermicron(10),
	nanometersperpixel(107)
{
	ui.setupUi(this);
}

void EMCCDWindow::initializeGL()
{
	// initialize state upon loading UI
	if (start == NULL) {
		start = mapPointToGrid(rect().center());
		end = mapPointToGrid(rect().center());
	}
}

void EMCCDWindow::paintGL()
{
	QPainter p(this);
	int startx = start->x();
	int starty = start->y();
	int endx = end->x();
	int endy = end->y();
	int leftx = rect().topLeft().x();
	int topy = rect().topLeft().y();
	int rightx = rect().topRight().x();
	int boty = rect().bottomLeft().y();

	QColor * c = new QColor(0, 0, 0, 0);
	int i = 0;
	int d;
	for (int y = 0; y < gblY; y++) {
		for (int x = 0; x < gblX; x++) {
			delete c;
			d = ((pImageArray[i] - minVal) / (maxVal - minVal)) * 255;
			c = new QColor(d, d, d);
			p.setPen(*c);
			p.drawPoint(x, y);
			i++;
		}
	}

	// Draw guide lines based on number of pixels per "micron"
	p.setPen(*guideColor);

	int dist = 0;
	while (dist < rightx) {
		p.drawLine((int)dist, boty, dist, topy);
		dist += pixelspermicron;
	}

	dist = 0;
	while (dist < boty) {
		p.drawLine(leftx, dist, rightx, dist);
		dist += pixelspermicron;
	}

	// Draw lines to bound selection
	p.setPen(Qt::red);
	p.drawLine(startx, topy, startx, boty);
	p.drawLine(leftx, starty, rightx, starty);
	p.drawLine(endx, topy, endx, boty);
	p.drawLine(leftx, endy, rightx, endy);

	// Draw end indicator
	p.drawEllipse(*end, 4, 4);

	// Draw origin indicator
	p.setPen(Qt::green);
	p.drawEllipse(*start, 4, 4);
	p.drawPoint(*start);
}

void EMCCDWindow::resizeGL(int w, int h) {}

void EMCCDWindow::mousePressEvent(QMouseEvent * event) {
	delete start;
	start = new QPoint(pixelCoordToGridCoord(event->x()), pixelCoordToGridCoord(event->y()));
}

// return pointer to a new qpoint on the grid that is closest to the arg pt
QPoint * EMCCDWindow::mapPointToGrid(QPoint pt) {
	return new QPoint(pixelCoordToGridCoord(pt.x()), pixelCoordToGridCoord(pt.y()));
}

// convert pixel coordinate value to grid coodinate value by rounding to the
// nearest grid value
int EMCCDWindow::pixelCoordToGridCoord(int pv) {
	if (((((float)pv) / ((float)pixelspermicron)) - (pv / pixelspermicron)) >= .5)
		return ((pv / pixelspermicron) + 1) * pixelspermicron;
	else
		return (pv / pixelspermicron) * pixelspermicron;
}

void EMCCDWindow::mouseReleaseEvent(QMouseEvent * event) {
	delete end;
	end = new QPoint(pixelCoordToGridCoord(event->x()), pixelCoordToGridCoord(event->y()));
	this->update();
}

QPoint * EMCCDWindow::getOrigin() {
	return this->start;
}

QPoint * EMCCDWindow::getEnd() {
	return this->end;
}

long EMCCDWindow::pixelValToNanometerVal(int px) {
	return (((long)px) * ((long)nanometersperpixel));
}

void EMCCDWindow::setScale(int nanometersperpixel) {
	this->pixelspermicron = ceil(1000 / nanometersperpixel);
	this->nanometersperpixel = nanometersperpixel;

	// Reset position of start and end cursors
	delete start;
	delete end;
	start = mapPointToGrid(rect().center());
	end = mapPointToGrid(rect().center());

	// Trigger render update
	this->update();
}

void EMCCDWindow::getOriginNanometerCoords(long * x, long * y) {
	*x = pixelValToNanometerVal(start->x());
	*y = pixelValToNanometerVal(start->y());
}

void EMCCDWindow::getEndNanometerCoords(long * x, long * y) {
	*x = pixelValToNanometerVal(end->x());
	*y = pixelValToNanometerVal(end->y());
}

void EMCCDWindow::init_disp(int targetTemp, int exposureTime, char * dir) {
	numloops = 100;

	aBuffer = dir;
	Initialize(aBuffer);
	GetDetector(&gblX, &gblY);
	GetTemperatureRange(&minT, &maxT);
	SetTemperature(targetTemp);
	CoolerON();

	int prevTemp;
	GetTemperature(&camTemp);
	while (camTemp != targetTemp) {
		prevTemp = camTemp;
		GetTemperature(&camTemp);
		//OutputDebugString(L"I MADE IT!\n");

		if (camTemp != prevTemp) {
			emit temp_changed(camTemp);
		}
	}

	SetAcquisitionMode(5);
	SetReadMode(4);
	SetExposureTime(exposureTime);
	SetVSAmplitude(1);
	// Set Horizontal Speed to max
	int STemp = 0;
	int HSnumber = 0;
	int ADnumber = 0;
	int nAD = 0;
	int index = 0;
	float speed = 0.0f;
	GetNumberADChannels(&nAD);
	for (int iAD = 0; iAD < nAD; iAD++) {
		GetNumberHSSpeeds(iAD, 0, &index);
		for (int iSpeed = 0; iSpeed < index; iSpeed++) {
			GetHSSpeed(iAD, 0, iSpeed, &speed);
			if (speed > STemp) {
				STemp = speed;
				HSnumber = iSpeed;
				ADnumber = iAD;
			}
		}
	}

	SetADChannel(ADnumber);
	SetHSSpeed(0, HSnumber);
	SetVSSpeed(.5);
	SetFrameTransferMode(0);
	SetShutter(1, 1, 0, 0);
	SetTriggerMode(10);
	SetImage(1, 1, 1, gblX, 1, gblY);
	GetAcquisitionTimings(&camExposure, &camAccumTime, &camKineticTime);

	imgSize = AllocateBuffers();

	int test = GetTickCount();
	int test2;
	do {
		test2 = GetTickCount() - test;
	} while (test2 < 2000);

	StartAcquisition();

	GetTheImages();

	CoolerOFF();
}

int EMCCDWindow::AllocateBuffers() {
	int 	size;
	FreeBuffers();
	size = gblX * gblY;  // Needs to hold full image
  // only allocate if necessary
	if (!pImageArray)
		pImageArray = (long *) calloc(sizeof(long), size);

	return size;
}

void EMCCDWindow::FreeBuffers() {
	if (pImageArray) {
		free(pImageArray);
		pImageArray = NULL;
	}
}

void EMCCDWindow::GetTheImages() {

	for (int i = 0; i < numloops; i++) {

		SendSoftwareTrigger();

		WaitForAcquisition();
		if (AcquireImageData() == FALSE) {
			i = numloops;
		}
	}

	AbortAcquisition();

}

bool EMCCDWindow::AcquireImageData() {
	int errVal=GetNewData(pImageArray, imgSize);
	if (errVal != DRV_SUCCESS) {
		return FALSE;
	}

	if (!DrawLines()) {
		return FALSE;
	}

	return TRUE;
}

bool EMCCDWindow::DrawLines() {
	int 			i;
	BOOL 			bRetValue = TRUE;
	maxVal = 1;
	minVal = 65536;

	if (pImageArray != NULL) {

		// Find max value and scale data to fill rect
		for (i = 0; i < imgSize; i++) {
			if (pImageArray[i] > maxVal)
				maxVal = pImageArray[i];
			if (pImageArray[i] < minVal)
				minVal = pImageArray[i];
		}

		if (maxVal == minVal)
			return FALSE;

		this->update();
	}
	else
		bRetValue = FALSE;
	return bRetValue;
}

EMCCDWindow::~EMCCDWindow() {}
