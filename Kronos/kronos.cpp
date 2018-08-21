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
	ui.xSteps->setText(*(new QString("10")));
	ui.yLines->setText(*(new QString("10")));
	QObject::connect(this, SIGNAL(initialize_emccd(int, int, char *)), ui.emccdDisp, SLOT(init_disp(int, int, char *)));
	QObject::connect(ui.emccdDisp, SIGNAL(temp_changed(int)), this, SLOT(display_newtemp(int)));
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
 * TODO: Test cmosMoveBtn
 * 
 * Also remove change scale button and replace with change
 * scale list of hardcoded scale values
 *
 * ALSO: CLEAN UP ALL THE CODE BEYOND THIS POINT
 */
void Kronos::on_cmosMoveBtn_clicked() {
	long originNx;
	long originNy;
	long laserNx;
	long laserNy;

	long xdist;
	long ydist;

	if (s != NULL) {
		ui.cmosDisp->getOriginNanometerCoords(&originNx, &originNy);
		ui.cmosDisp->getLaserNanometerCoords(&laserNx, &laserNy);

		xdist = originNx - laserNx;
		ydist = originNy - laserNy;

		QByteArray * data = new QByteArray("B ");
		data->append(std::to_string(ydist / 100L).c_str(), std::to_string(ydist / 100L).length());
		data->append('\r');
		ui.label->setText(*(new QString(s->send(data))));

		delete data;

		data = new QByteArray("L ");
		data->append(std::to_string(xdist / 100L).c_str(), std::to_string(xdist / 100L).length());
		data->append('\r');
		ui.label->setText(*(new QString(s->send(data))));
	}
}

void Kronos::on_acquireBtn_clicked() {
	long originNx;
	long originNy;
	long endNx;
	long endNy;
	long xdist;
	long ydist;
	long xstep;
	long ystep;

	if (s != NULL) {
		ui.cmosDisp->getOriginNanometerCoords(&originNx, &originNy);
		ui.cmosDisp->getEndNanometerCoords(&endNx, &endNy);

		xdist = originNx - endNx;
		ydist = originNy - endNy;

		// round up to nearest micron & zero frac part
		if (xdist % 1000L > 0) {
			xdist += 1000L;
			xdist /= 1000L;
			xdist *= 1000L;
		}

		if (ydist % 1000L > 0) {
			ydist += 1000L;
			ydist /= 1000L;
			ydist *= 1000L;
		}

		ui.xWidth->setText(*(new QString(xdist)));
		ui.yHeight->setText(*(new QString(ydist)));

		xstep = xdist / ui.xSteps->text().toLong();
		ystep = ydist / ui.yLines->text().toLong();

		int steps = ui.xSteps->text().toInt();
		int lines = ui.yLines->text().toInt();

		QByteArray * data;
		char * moving = s->isMoving();

		for (int i = 0; i < lines; i++) {
			for (int j = 0; j < steps; i++) {
				while (std::stoi(moving) != 0) {
					delete moving;
					moving = s->isMoving();
				}

				data = new QByteArray("R ");
				data->append(std::to_string(xstep / 100L).c_str(), std::to_string(xstep / 100L).length());
				data->append('\r');
				s->send(data);
				delete data;
			}
			moving = s->isMoving();
			while (std::stoi(moving) != 0) {
				delete moving;
				moving = s->isMoving();
			}

			data = new QByteArray("L ");
			data->append(std::to_string(xdist).c_str(), std::to_string(xdist).length());
			data->append('\r');
			s->send(data);
			delete data;

			moving = s->isMoving();
			while (std::stoi(moving) != 0) {
				delete moving;
				moving = s->isMoving();
			}

			data = new QByteArray("F ");
			data->append(std::to_string(xstep / 100L).c_str(), std::to_string(xstep / 100L).length());
			data->append('\r');
			s->send(data);
			delete data;
		}
	}
}

int Kronos::nanometerToMicron(long nm, int * frac) {
	*frac = ((nm % 1000) - (nm % 10)) / 10;
	return (nm - *frac) / 1000;
}

void Kronos::on_initBtn_clicked() {
	char * a = (char *) calloc(sizeof(char), ui.dir->text().length());
	strcpy(a, ui.dir->text().toStdString().c_str());
	emit initialize_emccd(ui.emccdTemp->text().toInt(), ui.emccdExposure->text().toInt(), a);
}

void Kronos::display_newtemp(int newTemp) {
	ui.emccdTempDisp->setText(*(new QString(newTemp)));
	this->update();
}

