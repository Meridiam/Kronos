#pragma once

#include <QWidget>
#include "ui_imagedisplay.h"

class ImageDisplay : public QWidget
{
	Q_OBJECT

public:
	ImageDisplay(QWidget *parent = Q_NULLPTR);
	ImageDisplay(QWidget * parent, Qt::WindowFlags f);
	Ui::ImageDisplay GetUI();
	~ImageDisplay();

private slots:
	void changeLabel(QString *s);

private:
	Ui::ImageDisplay ui;
};
