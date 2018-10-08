#pragma once

#include <QObject>

class EMCCDWorker : public QObject
{
	Q_OBJECT

public:
	EMCCDWorker(QObject *parent);
	int AllocateBuffers();
	void FreeBuffers();
	bool AcquireImageData();
	bool DrawLines();
	~EMCCDWorker();
private:
	char * aBuffer;
	int gblX;
	int gblY;
	int minT;
	int maxT;
	bool done;
	int camTemp;
	float camExposure;
	float camAccumTime;
	float camKineticTime;
	int imgSize;
	long * pImageArray;
	LARGE_INTEGER giStart;
	LARGE_INTEGER giEnd;
	long maxVal;
	long minVal;
private slots:
	void EMCCD_Init(int targetTemp, float exposureTime, char * dir);
	void GetTheImages();
	void StopWorker();
signals:
	void temp_changed(int newTemp);
	void new_image(long * img, int minVal, int maxVal);
};
