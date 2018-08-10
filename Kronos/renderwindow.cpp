#include "stdafx.h"
#include "renderwindow.h"

RenderWindow::RenderWindow(QWidget *parent)
	: QOpenGLWidget(parent)
{
	ui.setupUi(this);
}
void RenderWindow::initializeGL() 
{
	start = new QPoint(rect().topLeft());
	end = new QPoint(rect().bottomRight());
	img = new QImage("C:\\Users\\Scarcelli Lab\\Downloads\\picture1.png");
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
	p.setPen(Qt::gray);

	int microns = 0;
	int totalmicronsx = floor(rect().topRight().x() * .107);
	while (microns < totalmicronsx) {
		p.drawLine((int) round(microns / .107), boty, (int) round(microns / .107), topy);
		microns += 1;
	}

	microns = 0;
	int totalmicronsy = floor(rect().bottomRight().y() * .107);
	while (microns < totalmicronsy) {
		p.drawLine(leftx, (int)round(microns / .107), rightx, (int)round(microns / .107));
		microns += 1;
	}
	p.setPen(Qt::red);
	p.drawLine(startx, topy, startx, boty);
	p.drawLine(leftx, starty, rightx, starty);
	p.drawLine(endx, topy, endx, boty);
	p.drawLine(leftx, endy, rightx, endy);
	p.drawEllipse(*end, 4, 4);
	p.setPen(Qt::green);
	p.drawEllipse(*start, 4, 4);
	p.drawPoint(*start);
}

void RenderWindow::resizeGL(int w, int h)
{
}

void RenderWindow::setImage(const char * const uri) {
	delete img;
	img = new QImage(uri);
}

void RenderWindow::mousePressEvent(QMouseEvent * event) {
	delete start;
	start = new QPoint(nearestMicronValue(event->x(), 0.107), nearestMicronValue(event->y(), 0.107));
}

int RenderWindow::nearestMicronValue(int pv, double fac) {
	double micronval = ((double) pv) * fac;
	double nearest_int_micronval = round(micronval);
	double ideal_pixelval = nearest_int_micronval / fac;
	return (int) ceil(ideal_pixelval);
}

void RenderWindow::mouseReleaseEvent(QMouseEvent * event) {
	delete end;
	end = new QPoint(nearestMicronValue(event->x(), 0.107), nearestMicronValue(event->y(), 0.107));
	this->update();
}

void RenderWindow::setEnds(int sx, int sy, int ex, int ey) {
	delete start;
	delete end;
	start = new QPoint(sx, sy);
	end = new QPoint(ex, ey);
}

RenderWindow::~RenderWindow()
{
}
