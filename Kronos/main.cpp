#include "stdafx.h"
#include "kronos.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Kronos w;
	w.show();
	return a.exec();
}
