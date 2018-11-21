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

private:
	Ui::EMCCDWindow ui;
	QPoint * start;
	QPoint * end;
	QColor * guideColor;
	int pixelspercamerapixel;
	int imgSize;
	long * pImageArray;
	long maxVal;
	long minVal;
	EMCCDWorker * updater;
	QThread * thread;

private slots:
	void emccd_init_disp(int targetTemp, float exposureTime, char * dir);
	void emccd_receive_image(long * imgcpy, int minVal, int maxVal);

signals:
	//void temp_changed(int newTemp);
	void emccd_start_worker(int targetTemp, float exposureTime, char * dir);
	void emccd_request_img();
};
