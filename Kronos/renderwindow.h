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
	QPoint * mapPointToGrid(QPoint pt);
	void mouseReleaseEvent(QMouseEvent * event);
	QPoint * getOrigin();
	QPoint * getEnd();
	void initializeGL() override;

private:
	Ui::RenderWindow ui;
	QImage * img;
	QPoint * start;
	QPoint * end;
	QColor * guideColor;
	int pixelspermicron;
	int pixelCoordToGridCoord(int pv);
};
