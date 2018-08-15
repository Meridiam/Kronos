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
	connect(this, SIGNAL(update_draw()), ui.cmosDisp, SLOT(on_update_draw()));
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

void Kronos::on_toggleScaleBtn_clicked() {
	ui.cmosDisp->setScale(65);
}

/*
 * TODO: Test cmosMoveBtn and consider changing Stage move
 * methods to take two args: whole dist & fractional dist
 * 
 * Also remove change scale button and replace with change
 * scale list of hardcoded scale values
 *
 * Also consider if subtraction of fractions really can be
 * done by subtracting whole parts then fractional parts
 */
void Kronos::on_cmosMoveBtn_clicked() {
	long originNx;
	long originNy;
	long laserNx;
	long laserNy;
	int originx;
	int originxfrac;
	int originy;
	int originyfrac;
	int laserx;
	int laserxfrac;
	int lasery;
	int laseryfrac;
	int xdist;
	int xdistfrac;
	int ydist;
	int ydistfrac;

	ui.cmosDisp->getEndNanometerCoords(&originNx, &originNy);
	ui.cmosDisp->getLaserNanometerCoords(&laserNx, &laserNy);

	originx = nanometerToMicron(originNx, &originxfrac);
	originy = nanometerToMicron(originNy, &originyfrac);
	laserx = nanometerToMicron(laserNx, &laserxfrac);
	lasery = nanometerToMicron(laserNy, &laseryfrac);

	xdist = originx - laserx;
	xdistfrac = originxfrac - laserxfrac;
	ydist = originy - lasery;
	ydistfrac = originyfrac - laseryfrac;

	QByteArray * data = new QByteArray("B ");
	data->append(std::to_string(ydist).c_str(), std::to_string(ydist).length());
	data->append('.');
	data->append(std::to_string(abs(ydistfrac)).c_str(), std::to_string(abs(ydistfrac)).length());
	data->append('\r');
	s->send(data);

	delete data;

	data = new QByteArray("L ");
	data->append(std::to_string(xdist).c_str(), std::to_string(xdist).length());
	data->append('.');
	data->append(std::to_string(abs(xdistfrac)).c_str(), std::to_string(abs(xdistfrac)).length());
	data->append('\r');
	s->send(data);
}

int Kronos::nanometerToMicron(long nm, int * frac) {
	*frac = ((nm % 1000) - (nm % 10)) / 10;
	return (nm - *frac) / 1000;
}