#pragma once
#include <QSerialPort>
#include <QSerialPortInfo>

class Stage
{
public:
	Stage(QObject * parent, QString * portName);
	char * send(QByteArray * data);
	char * MoveAtVelocity(double xspeed, double yspeed);
	char * MoveLeft(double distance);
	char * MoveRight(double distance);
	char * MoveForward(double distance);
	char * MoveBackward(double distance);
	char * isMoving();
	//bool GetPosition(double * xpos, double * ypos);
	bool ClearCommands();
	~Stage();
private:
	QSerialPort * m_serial;
	bool connected;
	QSerialPortInfo info;
};

