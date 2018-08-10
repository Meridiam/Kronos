#include "stdafx.h"
#include "imagedisplay.h"

ImageDisplay::ImageDisplay(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

ImageDisplay::ImageDisplay(QWidget *parent, Qt::WindowFlags f) 
	: QWidget(parent, f)
{
	ui.setupUi(this);
}

void ImageDisplay::changeLabel(QString *s) {
	ui.hilabel->setText(*s);
}

Ui::ImageDisplay ImageDisplay::GetUI() {
	return this->ui;
}

ImageDisplay::~ImageDisplay()
{
}
