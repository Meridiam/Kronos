#include "stdafx.h"
#include "overviewwindow.h"

overviewwindow::overviewwindow(QWidget *parent)
	: QOpenGLWidget(parent),
	guideColor(new QColor(211, 211, 211, 75)),
	start(NULL),
	end(NULL),
	laser(NULL),
	pImageArray(NULL),
	updater(NULL),
	thread(NULL),
	laserselected(false),
	//	pixelspermicron will always be ceil(actual pixels per micron) 
	//	because actual pixels per micron could be floating point value, 
	//	which is obviously impossible to render on a screen.
	pixelspermicron(10),
	nanometersperpixel(107)
{
	ui.setupUi(this);
	qRegisterMetaType<DEV_HANDLE>("DEV_HANDLE");
}

void overviewwindow::initializeGL()
{
	// initialize state upon loading UI
	if (start == NULL) {
		laser = mapPointToGrid(rect().center());
		start = mapPointToGrid(rect().center());
		end = mapPointToGrid(rect().center());
	}
}

void overviewwindow::paintGL()
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

	p.setPen(Qt::blue);
	p.drawEllipse(*laser, 4, 4);
	p.drawPoint(*laser);

}

void overviewwindow::resizeGL(int w, int h) {}

void overviewwindow::mousePressEvent(QMouseEvent * event) {
	delete start;
	start = new QPoint(pixelCoordToGridCoord(event->x()), pixelCoordToGridCoord(event->y()));
}

void overviewwindow::mouseMoveEvent(QMouseEvent * event) {
	if (laserselected) {
		delete laser;
		laser = new QPoint(event->pos());

		// draw laser
		this->update();
	}
}

// return pointer to a new qpoint on the grid that is closest to the arg pt
QPoint * overviewwindow::mapPointToGrid(QPoint pt) {
	return new QPoint(pixelCoordToGridCoord(pt.x()), pixelCoordToGridCoord(pt.y()));
}

// convert pixel coordinate value to grid coodinate value by rounding to the
// nearest grid value
int overviewwindow::pixelCoordToGridCoord(int pv) {
	if (((((float)pv) / ((float)pixelspermicron)) - (pv / pixelspermicron)) >= .5)
		return ((pv / pixelspermicron) + 1) * pixelspermicron;
	else
		return (pv / pixelspermicron) * pixelspermicron;
}

void overviewwindow::mouseReleaseEvent(QMouseEvent * event) {
	delete end;
	end = new QPoint(pixelCoordToGridCoord(event->x()), pixelCoordToGridCoord(event->y()));
	this->update();
}

QPoint * overviewwindow::getOrigin() {
	return this->start;
}

QPoint * overviewwindow::getEnd() {
	return this->end;
}

long overviewwindow::pixelValToNanometerVal(int px) {
	return (((long)px) * ((long)nanometersperpixel));
}

void overviewwindow::setScale(int nanometersperpixel) {
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

void overviewwindow::getLaserNanometerCoords(long * x, long * y) {
	*x = pixelValToNanometerVal(laser->x());
	*y = pixelValToNanometerVal(laser->y());
}

void overviewwindow::getOriginNanometerCoords(long * x, long * y) {
	*x = pixelValToNanometerVal(start->x());
	*y = pixelValToNanometerVal(start->y());
}

void overviewwindow::getEndNanometerCoords(long * x, long * y) {
	*x = pixelValToNanometerVal(end->x());
	*y = pixelValToNanometerVal(end->y());
}

void overviewwindow::overview_init_disp() {
	if (!thread) {
		MTUSB_InitDevice();
		DEV_HANDLE h = MTUSB_ShowOpenDeviceDialog();

		thread = new QThread(this);
		updater = new overviewworker(0);
		updater->moveToThread(thread);

		connect(updater, SIGNAL(overview_new_image(long *, int, int)), this, SLOT(overview_receive_image(long *, int, int)));
		connect(this, SIGNAL(overview_start_worker(DEV_HANDLE)), updater, SLOT(overview_init(DEV_HANDLE)));
		connect(this->parent(), SIGNAL(overview_abort_acq()), updater, SLOT(stop_worker()));
		connect(this, SIGNAL(overview_request_img()), updater, SLOT(get_newest_img()));

		thread->start();

		// Must use a signal instead of calling init as method to avoid blocking
		emit overview_start_worker(h);
	}
}

void overviewwindow::overview_receive_image(long * imgcpy, int minVal, int maxVal) {
	pImageArray = imgcpy;
	this->minVal = minVal;
	this->maxVal = maxVal;
	this->update();

	// request next img once current img is drawn
	// uses step-by-step methodology to avoid blocking img retrieval loop in worker thread
	emit overview_request_img();
}

overviewwindow::~overviewwindow() {
	if (thread != NULL) {
		thread->quit();
		thread->wait();
	}
}