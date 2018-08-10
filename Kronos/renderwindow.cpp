#include "stdafx.h"
#include "renderwindow.h"

RenderWindow::RenderWindow(QWidget *parent)
	: QOpenGLWidget(parent)
{
	ui.setupUi(this);
}
void RenderWindow::initializeGL() 
{
	start = rect().topLeft();
	end = rect().bottomRight();
}

void RenderWindow::paintGL()
{
	QPainter p(this);
	p.setPen(Qt::red);
	p.drawImage(*(new QPoint(0, 0)), *(new QImage("C:\\Users\\Scarcelli Lab\\Downloads\\picture1.png")));
	p.drawLine(start, end);
}

void RenderWindow::resizeGL(int w, int h)
{
}

void RenderWindow::setEnds(int sx, int sy, int ex, int ey) {
	start = *(new QPoint(sx, sy));
	end = *(new QPoint(ex, ey));
}

RenderWindow::~RenderWindow()
{
}
