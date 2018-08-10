#include "stdafx.h"
#include "kronos.h"
#include <QSerialPortInfo>

Kronos::Kronos(QWidget *parent)
	: QWidget(parent),
	connected(false),
	s(NULL),
	unit(new QString("10000")),
	yvelocity(new QString("10000")),
	xvelocity(new QString("10000"))
{
	ui.setupUi(this);
	const auto infos = QSerialPortInfo::availablePorts();
	for (const QSerialPortInfo &info : infos) {
		ui.label->setText(info.portName());
	}
	ui.lineEdit->setText(*unit);
	ui.yvel->setText(*yvelocity);
	ui.xvel->setText(*xvelocity);
	connect(this, SIGNAL(update_draw()), ui.drawwidg, SLOT(on_update_draw()));
}

void Kronos::on_forwardbtn_clicked() {
	if (s != NULL) {
		ui.label->setText(*(new QString(s->MoveForward(unit->toDouble()))));
	}
}

void Kronos::on_rightbtn_clicked() {
	if (s != NULL) {
		ui.label->setText(*(new QString(s->MoveRight(unit->toDouble()))));
	}
}

void Kronos::on_backbtn_clicked() {
	if (s != NULL) {
		ui.label->setText(*(new QString(s->MoveBackward(unit->toDouble()))));
	}
}

void Kronos::on_leftbtn_clicked() {
	if (s != NULL) {
		ui.label->setText(*(new QString(s->MoveLeft(unit->toDouble()))));
	}
}

void Kronos::on_runbtn_clicked() {
	if (s != NULL) {
		ui.label->setText(*(new QString(s->MoveAtVelocity(xvelocity->toDouble(), yvelocity->toDouble()))));
	}
}

void Kronos::on_stopbtn_clicked() {
	if (s != NULL) {
		ui.label->setText(*(new QString(s->ClearCommands())));
	}
}

void Kronos::on_connectbtn_clicked() {
	if (s == NULL) {
		s = new Stage(this);
		ui.label_2->setText(*(new QString("CONNECTED")));
	}
}

void Kronos::on_disconnectbtn_clicked()
{
	delete s;
	s = NULL;
	ui.label_2->setText(*(new QString("DISCONNECTED")));
}

void Kronos::on_lineEdit_returnPressed() {
	unit = new QString(ui.lineEdit->text());
}

void Kronos::on_yvel_returnPressed() {
	yvelocity = new QString(ui.yvel->text());
}

void Kronos::on_xvel_returnPressed() {
	xvelocity = new QString(ui.xvel->text());
}

void Kronos::on_drawbtn_clicked() {
	ui.drawwidg->setEnds(0, 50, 350, 50);
	ui.drawwidg->update();
}
