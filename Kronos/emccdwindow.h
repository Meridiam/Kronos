#pragma once

#include <QOpenGLWidget>
#include <QPoint>
#include "ui_emccdwindow.h"
#include "emccdworker.h"

class EMCCDWindow : public QOpenGLWidget
{
	Q_OBJECT

public:
	EMCCDWindow(QWidget *parent = Q_NULLPTR);
	//int AllocateBuffers();
	//void FreeBuffers();
	//void GetTheImages();
	//bool AcquireImageData();
	//bool DrawLines(); 
	~EMCCDWindow();
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void mousePressEvent(QMouseEvent * event);
	QPoint * mapPointToGrid(QPoint pt);
	int pixelCoordToGridCoord(int pv);
	void mouseReleaseEvent(QMouseEvent * event);
	QPoint * getOrigin();
	QPoint * getEnd();
	void initializeGL() override;
	long pixelValToNanometerVal(int gridval);
	void setScale(int nanometersperpixel);
	void getOriginNanometerCoords(long * x, long * y);
	void getEndNanometerCoords(long * x, long * y);

private:
	Ui::EMCCDWindow ui;
	QPoint * start;
	QPoint * end;
	QColor * guideColor;
	int pixelspermicron;
	int nanometersperpixel;
	//char * aBuffer;
	//int gblX;
	//int gblY;
	//int minT;
	//int maxT;
	//int camTemp;
	//float camExposure;
	//float camAccumTime;
	//float camKineticTime;
	int imgSize;
	long * pImageArray;
	//LARGE_INTEGER giStart;
	//LARGE_INTEGER giEnd;
	//int numloops;
	long maxVal;
	long minVal;
	EMCCDWorker * updater;
	QThread * thread;

private slots:
	void init_disp(int targetTemp, float exposureTime, char * dir);
	void receive_image(long * imgcpy, int minVal, int maxVal);

signals:
	//void temp_changed(int newTemp);
	void start_worker(int targetTemp, float exposureTime, char * dir);
	void request_img();
};
