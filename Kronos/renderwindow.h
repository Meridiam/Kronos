#pragma once

#include <QOpenGLWidget>
#include <QPoint>
#include "ui_renderwindow.h"

class RenderWindow : public QOpenGLWidget
{
	Q_OBJECT

public:
	RenderWindow(QWidget *parent = Q_NULLPTR);
	~RenderWindow();
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void setImage(const char * const uri);
	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	QPoint * mapPointToGrid(QPoint pt);
	void mouseReleaseEvent(QMouseEvent * event);
	QPoint * getOrigin();
	QPoint * getEnd();
	void initializeGL() override;
	long pixelValToNanometerVal(int gridval);
	void setScale(int nanometersperpixel);
	void getLaserNanometerCoords(long * x, long * y);
	void getOriginNanometerCoords(long * x, long * y);
	void getEndNanometerCoords(long * x, long * y);

private:
	Ui::RenderWindow ui;
	QImage * img;
	QPoint * start;
	QPoint * end;
	QPoint * laser;
	QColor * guideColor;
	int pixelspermicron;
	int nanometersperpixel;
	bool laserselected;
	bool rendergrid;
	int pixelCoordToGridCoord(int pv);
};
