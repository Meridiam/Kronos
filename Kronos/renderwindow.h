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
	void mouseReleaseEvent(QMouseEvent * event);
	void setEnds(int sx, int sy, int ex, int ey);
	void initializeGL() override;

private:
	Ui::RenderWindow ui;
	QImage * img;
	QPoint * start;
	QPoint * end;
	int nearestMicronValue(int pv, double fac);
};
