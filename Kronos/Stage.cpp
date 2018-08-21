#include "stdafx.h"
#include "Stage.h"
#include <QSerialPort>


Stage::Stage(QObject* parent) :
	m_serial(new QSerialPort(parent)),
	connected(false)
{
	const auto infos = QSerialPortInfo::availablePorts();
	for (const QSerialPortInfo &info : infos) {
		this->info = info;
	}
	m_serial->setPort(this->info);
	m_serial->setBaudRate(QSerialPort::Baud9600);
	m_serial->setDataBits(QSerialPort::Data8);
	m_serial->setParity(QSerialPort::NoParity);
	m_serial->setStopBits(QSerialPort::OneStop);
	m_serial->setFlowControl(QSerialPort::NoFlowControl);

	if (m_serial->open(QIODevice::ReadWrite)) {
		connected = true;
		m_serial->write(*(new QByteArray("RES,X,0.1\r", 10)));
		m_serial->write(*(new QByteArray("RES,Y,0.1\r", 10)));
		m_serial->write(*(new QByteArray("COMP 0\r", 7)));
	}
}

char *Stage::send(QByteArray *data) {
	if (connected) {
		char *ret = (char *) calloc(65535, sizeof(char));
		m_serial->write(*data);
		m_serial->waitForReadyRead();
		if (m_serial->read(ret, 65535) < 0) {
			return "ERROR";
		} else {
			return ret;
		}
	} else {
		return "ERROR";
	}
}

char *Stage::MoveAtVelocity(double xspeed, double yspeed) {
	if (connected) {
		QByteArray *data = new QByteArray("VS ", 3);
		data->append(std::to_string(xspeed).c_str());
		data->append(',');
		data->append(std::to_string(yspeed).c_str());
		data->append('\r');
		
		return this->send(data);
	}
	else {
		return "ERROR";
	}
}

char *Stage::MoveLeft(double distance) {
	if (connected) {
		QByteArray *data = new QByteArray("L ", 2);
		data->append(std::to_string(distance).c_str());
		data->append('\r');

		return this->send(data);
	}
	else {
		return "ERROR";
	}
}

char *Stage::MoveRight(double distance) {
	if (connected) {
		QByteArray *data = new QByteArray("R ", 2);
		data->append(std::to_string(distance).c_str());
		data->append('\r');
		return this->send(data);
	}
	else {
		return "ERROR";
	}
}

char *Stage::MoveForward(double distance) {
	if (connected) {
		QByteArray *data = new QByteArray("F ", 2);
		data->append(std::to_string(distance).c_str());
		data->append('\r');
		return this->send(data);
	}
	else {
		return "ERROR";
	}
}

char *Stage::MoveBackward(double distance) {
	if (connected) {
		QByteArray *data = new QByteArray("B ", 2);
		data->append(std::to_string(distance).c_str());
		data->append('\r');
		return this->send(data);
	}
	else {
		return "ERROR";
	}
}

//bool Stage::GetPosition(double *xpos, double *ypos) {
//	if (connected) {
//		QByteArray *cmd = new QByteArray("PS\r", 3);
//		char *ret = this->send(cmd);
//		if (std::string("ERROR").compare(ret) != 0) {
//			std::string retvals = std::string(ret).;
//			size_t delim = retvals.find_first_of(',', 0);
//			if (delim = std::string::npos) {
//				free(ret);
//				ret = this->send(cmd);
//				retvals = std::string(ret);
//				delim = retvals.find_first_of(',', 0);
//			}
//			*xpos = (std::stod(retvals.substr(0, delim)));
//			*ypos = (std::stod(retvals.substr(delim + 1, std::string::npos)));
//			return true;
//		}
//	}
//	return true;
//}

char *Stage::isMoving() {
	if (connected) {
		QByteArray *data = new QByteArray("$,S\r", 4);
		return this->send(data);
	}
	else {
		return "ERROR";
	}
}

bool Stage::ClearCommands() {
	if (connected) {
		m_serial->write(*(new QByteArray("I\r", 2)));
	}
	return true;
}

Stage::~Stage()
{
	if (connected) {
		m_serial->close();
		delete m_serial;
		m_serial = NULL;
	}
}
