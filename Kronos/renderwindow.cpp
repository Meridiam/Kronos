#include "stdafx.h"
#include "renderwindow.h"

RenderWindow::RenderWindow(QWidget *parent)
	: QOpenGLWidget(parent),
	guideColor(new QColor(211, 211, 211, 75)),
	start(NULL),
	end(NULL),
	img(NULL),
//	pixelspermicron will always be ceil(actual pixels per micron) 
//	because actual pixels per micron could be floating point value, 
//	which is obviously impossible to render on a screen.
	pixelspermicron(16)
{
	ui.setupUi(this);
}

void RenderWindow::initializeGL() 
{
	// initialize state upon loading UI
	if (start == NULL) {
		start = mapPointToGrid(rect().center());
		end = mapPointToGrid(rect().center());
		img = new QImage("C:\\Users\\Scarcelli Lab\\Downloads\\picture1.png");
	}
}

void RenderWindow::paintGL()
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

	p.drawImage(rect().topLeft(), *img);

	// Draw guide lines based on number of pixels per "micron"
	p.setPen(*guideColor);

	int dist = 0;
	while (dist < rightx) {
		p.drawLine((int) dist, boty, dist, topy);
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

void RenderWindow::resizeGL(int w, int h) {}

void RenderWindow::setImage(const char * const uri) {
	delete img;
	img = new QImage(uri);
}

void RenderWindow::mousePressEvent(QMouseEvent * event) {
	delete start;
	start = new QPoint(pixelCoordToGridCoord(event->x()), pixelCoordToGridCoord(event->y()));
}

// return pointer to a new qpoint on the grid that is closest to the arg pt
QPoint * RenderWindow::mapPointToGrid(QPoint pt) {
	return new QPoint(pixelCoordToGridCoord(pt.x()), pixelCoordToGridCoord(pt.y()));
}

// convert pixel coordinate value to grid coodinate value by rounding to the
// nearest grid value
int RenderWindow::pixelCoordToGridCoord(int pv) {
	if (((((float)pv) / ((float)pixelspermicron)) - (pv/pixelspermicron)) >= .5)
		return ((pv / pixelspermicron) + 1) * pixelspermicron;
	else
		return (pv / pixelspermicron) * pixelspermicron;
}

void RenderWindow::mouseReleaseEvent(QMouseEvent * event) {
	delete end;
	end = new QPoint(pixelCoordToGridCoord(event->x()), pixelCoordToGridCoord(event->y()));
	this->update();
}

QPoint * RenderWindow::getOrigin() {
	return this->start;
}

QPoint * RenderWindow::getEnd() {
	return this->end;
}

RenderWindow::~RenderWindow() {}
