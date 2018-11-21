#pragma once

#include <QOpenGLWidget>
#include "overviewworker.h"
#include "ui_overviewwindow.h"

class overviewwindow : public QOpenGLWidget
{
	Q_OBJECT
public:
	overviewwindow(QWidget *parent = Q_NULLPTR);
	~overviewwindow();
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	QPoint * mapPointToGrid(QPoint pt);
	int pixelCoordToGridCoord(int pv);
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
	Ui::overviewwindow ui;
	QPoint * start;
	QPoint * end;
	QPoint * laser;
	QColor * guideColor;
	int pixelspermicron;
	int nanometersperpixel;
	bool laserselected;
	int imgSize;
	long * pImageArray;
	long maxVal;
	long minVal;
	overviewworker * updater;
	QThread * thread;

private slots:
	void overview_init_disp();
	void overview_receive_image(long * imgcpy, int minVal, int maxVal);

signals:
	//void temp_changed(int newTemp);
	void overview_start_worker(DEV_HANDLE device_handle);
	void overview_request_img();
};
