#include "stdafx.h"
#include "emccdwindow.h"

EMCCDWindow::EMCCDWindow(QWidget *parent)
	: QOpenGLWidget(parent),
	guideColor(new QColor(211, 211, 211, 75)),
	start(NULL),
	end(NULL),
	pImageArray(NULL),
	updater(NULL),
	thread(NULL),
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

	if (pImageArray) {
		QColor * c = new QColor(0, 0, 0, 0);
		int i = 0;
		int d;
		for (int y = 0; y < 320; y += 10) {
			for (int x = 0; x < 640; x += 10) {
				d = ((pImageArray[i] - minVal) / (maxVal - minVal)) * 255;
				if (!(d == 0)) {
					delete c;
					c = new QColor(d, d, d);
					p.setPen(*c);
					p.fillRect(x, y, 10, 10, *c);
				}
				i++;
			}
		}
		free(pImageArray);
		pImageArray = NULL;
	}

	// Draw lines to bound selection
	p.setPen(Qt::red);
	p.drawLine(startx, topy, startx, boty);
	p.drawLine(leftx, starty, rightx, starty);
	p.drawLine(endx, topy, endx, boty);
	p.drawLine(leftx, endy, rightx, endy);
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

void EMCCDWindow::init_disp(int targetTemp, float exposureTime, char *dir) {
	if (!thread) {
		thread = new QThread(this);
		updater = new EMCCDWorker(0);
		updater->moveToThread(thread);

		connect(thread, SIGNAL(destroyed()), updater, SLOT(deleteLater()));
		connect(updater, SIGNAL(temp_changed(int)), this->parent(), SLOT(display_newtemp(int)));
		connect(updater, SIGNAL(new_image(long *, int, int)), this, SLOT(receive_image(long *, int, int)));
		connect(this, SIGNAL(start_worker(int, float, char *)), updater, SLOT(EMCCD_Init(int, float, char *)));
		connect(this->parent(), SIGNAL(abort_acq()), updater, SLOT(StopWorker()));
		connect(this, SIGNAL(request_img()), updater, SLOT(GetTheImages()));

		thread->start();

		// Must use a signal instead of calling init as method to avoid blocking
		emit start_worker(targetTemp, exposureTime, dir);
	}
}

void EMCCDWindow::receive_image(long * imgcpy, int minVal, int maxVal) {
	pImageArray = imgcpy;
	this->minVal = minVal;
	this->maxVal = maxVal;
	this->update();

	// request next img once current img is drawn
	// uses step-by-step methodology to avoid blocking img retrieval loop in worker thread
	emit request_img();
}

EMCCDWindow::~EMCCDWindow() {
	thread->quit();
	thread->wait();
}

//void EMCCDWindow::init_disp(int targetTemp, float exposureTime, char * dir) {
//	numloops = 6000;
//
//#ifdef DEBUGGING_MODE
//	OutputDebugString(L"SIGNAL RECEIVED!\n");
//#endif
//
//	aBuffer = dir;
//	Initialize(aBuffer);
//	GetTemperatureRange(&minT, &maxT);
//	GetDetector(&gblX, &gblY);
//	SetTemperature(targetTemp);
//	CoolerON();
//
//#ifdef DEBUGGING_MODE
//	OutputDebugString(L"COOLING!\n");
//#endif
//
//	GetTemperature(&camTemp);
//	emit temp_changed(camTemp);
//	qApp->processEvents();
//	while (camTemp > targetTemp) {
//		GetTemperature(&camTemp);
//		emit temp_changed(camTemp);
//	}
//
//	if (SetAcquisitionMode(5) != DRV_SUCCESS) {
//		OutputDebugString(L"SETACQMODE ERROR!\n");
//	}
//	if (DRV_SUCCESS != SetReadMode(4)) {
//		OutputDebugString(L"SETREADMODE ERROR!\n");
//	}
//	if (DRV_SUCCESS != SetExposureTime(exposureTime)) {
//		OutputDebugString(L"SETEXPOSURE ERROR!\n");
//	}
//	if (DRV_SUCCESS != SetVSAmplitude(1)) {
//		OutputDebugString(L"SETVSAMP ERROR!\n");
//	}
//	// Set Horizontal Speed to max
//	int STemp = 0;
//	int HSnumber = 0;
//	int ADnumber = 0;
//	int nAD = 0;
//	int index = 0;
//	float speed = 0.0f;
//	if (DRV_SUCCESS != GetNumberADChannels(&nAD)) {
//		OutputDebugString(L"GETNUMAD ERROR!\n");
//	}
//	for (int iAD = 0; iAD < nAD; iAD++) {
//		if (DRV_SUCCESS != GetNumberHSSpeeds(iAD, 0, &index)) {
//			OutputDebugString(L"GETNUMHSSPEEDS ERROR!\n");
//		}
//		for (int iSpeed = 0; iSpeed < index; iSpeed++) {
//			if (DRV_SUCCESS != GetHSSpeed(iAD, 0, iSpeed, &speed)) {
//				OutputDebugString(L"GETHSSPEED ERROR!\n");
//			}
//			if (speed > STemp) {
//				STemp = speed;
//				HSnumber = iSpeed;
//				ADnumber = iAD;
//			}
//		}
//	}
//
//	if (DRV_SUCCESS != SetADChannel(ADnumber)) {
//		OutputDebugString(L"SETADCHANNEL ERROR!\n");
//	}
//	if (DRV_SUCCESS != SetHSSpeed(0, HSnumber)) {
//		OutputDebugString(L"SETHSSPEED ERROR!\n");
//	}
//	if (DRV_SUCCESS != SetVSSpeed(.5)) {
//		OutputDebugString(L"SETVSSPEED ERROR!\n");
//	}
//	if (DRV_SUCCESS != SetFrameTransferMode(0)) {
//		OutputDebugString(L"SETFRAMETRANSFER ERROR!\n");
//	}
//	if (DRV_SUCCESS != SetShutter(1, 1, 0, 0)) {
//		OutputDebugString(L"SETSHUTTER ERROR!\n");
//	}
//	if (DRV_SUCCESS != SetTriggerMode(10)) {
//		OutputDebugString(L"SETTRIGGERMODE ERROR!\n");
//	}
//	if (DRV_SUCCESS != SetImage(1, 1, 110, 173, 237, 268)) {
//		OutputDebugString(L"SETIMAGE ERROR!\n");
//	}
//	if (DRV_SUCCESS != GetAcquisitionTimings(&camExposure, &camAccumTime, &camKineticTime)) {
//		OutputDebugString(L"GET ACQ TIMINGS ERROR!\n");
//	}
//
//	imgSize = AllocateBuffers();
//
//	int test = GetTickCount();
//	int test2;
//	do {
//		test2 = GetTickCount() - test;
//	} while (test2 < 2000);
//
//	if (DRV_SUCCESS != StartAcquisition()) {
//		OutputDebugString(L"ACQUISITION ERROR!\n");
//	}
//
//	GetTheImages();
//
//	if (DRV_SUCCESS != CoolerOFF()) {
//		OutputDebugString(L"COOLEROFF ERROR!\n");
//	}
//
//	while (camTemp < -20) {
//		GetTemperature(&camTemp);
//		emit temp_changed(camTemp);
//	}
//
//	if (DRV_SUCCESS != ShutDown()) {
//		OutputDebugString(L"CAMERA SHUTDOWN ERROR!\n");
//	}
//
//#ifdef DEBUGGING_MODE
//	OutputDebugString(L"DONE!\n");
//#endif
//}
//
//int EMCCDWindow::AllocateBuffers() {
//	int 	size;
//	FreeBuffers();
//	size = 32 * 64;  // Needs to hold partial image
//  // only allocate if necessary
//	if (!pImageArray)
//		pImageArray = (long *) calloc(sizeof(long), size);
//
//	return size;
//}
//
//void EMCCDWindow::FreeBuffers() {
//	if (pImageArray) {
//		free(pImageArray);
//		pImageArray = NULL;
//	}
//}
//
//void EMCCDWindow::GetTheImages() {
//
//	for (int i = 0; i < numloops; i++) {
//
//#ifdef DEBUGGING_MODE
//		OutputDebugString(L"IMAGING!\n");
//#endif
//
//		if (DRV_SUCCESS != SendSoftwareTrigger()) {
//			OutputDebugString(L"SENDTRIGGER ERROR!\n");
//		}
//
//		if (DRV_SUCCESS != WaitForAcquisition()) {
//			OutputDebugString(L"WAITFORACQ ERROR!\n");
//		}
//		if (AcquireImageData() == FALSE) {
//			i = numloops;
//		}
//	}
//
//	AbortAcquisition();
//
//}
//
//bool EMCCDWindow::AcquireImageData() {
//	int errVal=GetNewData(pImageArray, imgSize);
//	if (errVal != DRV_SUCCESS) {
//		OutputDebugString(L"GETNEWIMG ERROR!\n");
//		return FALSE;
//	}
//
//	if (!DrawLines()) {
//		return FALSE;
//	}
//
//	return TRUE;
//}
//
//bool EMCCDWindow::DrawLines() {
//	int 			i;
//	BOOL 			bRetValue = TRUE;
//	maxVal = 1;
//	minVal = 65536;
//
//	if (pImageArray != NULL) {
//
//		// Find max value and scale data to fill rect
//		for (i = 0; i < imgSize; i++) {
//			if (pImageArray[i] > maxVal)
//				maxVal = pImageArray[i];
//			if (pImageArray[i] < minVal)
//				minVal = pImageArray[i];
//		}
//
//		if (maxVal == minVal)
//			return FALSE;
//
//#ifdef DEBUGGING_MODE
//		OutputDebugString(L"DRAWING!\n");
//#endif
//
//		this->update();
//	}
//	else
//		bRetValue = FALSE;
//	return bRetValue;
//}
