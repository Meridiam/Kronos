#pragma once

#include <QtWidgets/QWidget>
#include "ui_kronos.h"
#include "Stage.h"
class Kronos : public QWidget
{
	Q_OBJECT

public:
	Kronos(QWidget *parent = Q_NULLPTR);

private:
	Ui::KronosClass ui;
	bool connected;
	Stage *s;
	QString * unit;
	QString * yvelocity;
	QString * xvelocity;
private slots:
	void on_forwardbtn_clicked();
	void on_rightbtn_clicked();
	void on_backbtn_clicked();
	void on_leftbtn_clicked();
	void on_runbtn_clicked();
	void on_stopbtn_clicked();
	void on_connectbtn_clicked();
	void on_disconnectbtn_clicked();
	void on_lineEdit_returnPressed();
	void on_yvel_returnPressed();
	void on_xvel_returnPressed();
	void on_drawbtn_clicked();
};
