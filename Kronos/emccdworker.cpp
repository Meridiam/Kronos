#include "stdafx.h"
#include "emccdworker.h"

EMCCDWorker::EMCCDWorker(QObject *parent)
	: QObject(parent),
	pImageArray(NULL),
	aBuffer(NULL),
	done(false)
{
}

void EMCCDWorker::EMCCD_Init(int targetTemp, float exposureTime, char * dir) {
	numloops = 10000;

#ifdef DEBUGGING_MODE
	OutputDebugString(L"SIGNAL RECEIVED!\n");
#endif

	aBuffer = dir;
	Initialize(aBuffer);
	GetTemperatureRange(&minT, &maxT);
	GetDetector(&gblX, &gblY);
	SetTemperature(targetTemp);
	CoolerON();

#ifdef DEBUGGING_MODE
	OutputDebugString(L"COOLING!\n");
#endif

	GetTemperature(&camTemp);
	emit temp_changed(camTemp);
	while (camTemp > targetTemp) {
		GetTemperature(&camTemp);
		emit temp_changed(camTemp);
	}

	if (SetAcquisitionMode(5) != DRV_SUCCESS) {
		OutputDebugString(L"SETACQMODE ERROR!\n");
	}
	if (DRV_SUCCESS != SetReadMode(4)) {
		OutputDebugString(L"SETREADMODE ERROR!\n");
	}
	if (DRV_SUCCESS != SetExposureTime(exposureTime)) {
		OutputDebugString(L"SETEXPOSURE ERROR!\n");
	}
	if (DRV_SUCCESS != SetVSAmplitude(1)) {
		OutputDebugString(L"SETVSAMP ERROR!\n");
	}
	// Set Horizontal Speed to max
	int STemp = 0;
	int HSnumber = 0;
	int ADnumber = 0;
	int nAD = 0;
	int index = 0;
	float speed = 0.0f;
	if (DRV_SUCCESS != GetNumberADChannels(&nAD)) {
		OutputDebugString(L"GETNUMAD ERROR!\n");
	}
	for (int iAD = 0; iAD < nAD; iAD++) {
		if (DRV_SUCCESS != GetNumberHSSpeeds(iAD, 0, &index)) {
			OutputDebugString(L"GETNUMHSSPEEDS ERROR!\n");
		}
		for (int iSpeed = 0; iSpeed < index; iSpeed++) {
			if (DRV_SUCCESS != GetHSSpeed(iAD, 0, iSpeed, &speed)) {
				OutputDebugString(L"GETHSSPEED ERROR!\n");
			}
			if (speed > STemp) {
				STemp = speed;
				HSnumber = iSpeed;
				ADnumber = iAD;
			}
		}
	}

	if (DRV_SUCCESS != SetADChannel(ADnumber)) {
		OutputDebugString(L"SETADCHANNEL ERROR!\n");
	}
	if (DRV_SUCCESS != SetHSSpeed(0, HSnumber)) {
		OutputDebugString(L"SETHSSPEED ERROR!\n");
	}
	if (DRV_SUCCESS != SetVSSpeed(.5)) {
		OutputDebugString(L"SETVSSPEED ERROR!\n");
	}
	if (DRV_SUCCESS != SetFrameTransferMode(0)) {
		OutputDebugString(L"SETFRAMETRANSFER ERROR!\n");
	}
	if (DRV_SUCCESS != SetShutter(1, 1, 0, 0)) {
		OutputDebugString(L"SETSHUTTER ERROR!\n");
	}
	if (DRV_SUCCESS != SetTriggerMode(10)) {
		OutputDebugString(L"SETTRIGGERMODE ERROR!\n");
	}
	if (DRV_SUCCESS != SetImage(1, 1, 110, 173, 237, 268)) {
		OutputDebugString(L"SETIMAGE ERROR!\n");
	}
	if (DRV_SUCCESS != GetAcquisitionTimings(&camExposure, &camAccumTime, &camKineticTime)) {
		OutputDebugString(L"GET ACQ TIMINGS ERROR!\n");
	}

	imgSize = AllocateBuffers();

	int test = GetTickCount();
	int test2;
	do {
		test2 = GetTickCount() - test;
	} while (test2 < 2000);

	if (DRV_SUCCESS != StartAcquisition()) {
		OutputDebugString(L"ACQUISITION ERROR!\n");
	}

	GetTheImages();
}

void EMCCDWorker::StopWorker() {
	OutputDebugString(L"ABORT SIGNAL RECEIVED!!!!!");
	done = true;
	/* TODO: THINK OF WAY TO INTERRUPT ACQ WITHOUT PROCESSEVENTS & REMOVE PROCESSEVENTS IN GENERAL, QTIMER PERHAPS?*/
}

int EMCCDWorker::AllocateBuffers() {
	int 	size;
	FreeBuffers();
	size = 32 * 64;  // Needs to hold partial image
  // only allocate if necessary
	if (!pImageArray)
		pImageArray = (long *)calloc(sizeof(long), size);

	return size;
}

void EMCCDWorker::FreeBuffers() {
	if (pImageArray) {
		free(pImageArray);
		pImageArray = NULL;
	}
}

void EMCCDWorker::GetTheImages() {
	if (!done) {

#ifdef DEBUGGING_MODE
		OutputDebugString(L"IMAGING!\n");
#endif

		if (DRV_SUCCESS != SendSoftwareTrigger()) {
			OutputDebugString(L"SENDTRIGGER ERROR!\n");
		}

		if (DRV_SUCCESS != WaitForAcquisition()) {
			OutputDebugString(L"WAITFORACQ ERROR!\n");
		}

		if (AcquireImageData() == FALSE) {
#ifdef DEBUGGING_MODE
			OutputDebugString(L"ACQ ERROR!\n");
#endif
			AbortAcquisition();

			if (DRV_SUCCESS != CoolerOFF()) {
				OutputDebugString(L"COOLEROFF ERROR!\n");
			}

			while (camTemp < -20) {
				GetTemperature(&camTemp);
				emit temp_changed(camTemp);
			}

			if (DRV_SUCCESS != ShutDown()) {
				OutputDebugString(L"CAMERA SHUTDOWN ERROR!\n");
			}

#ifdef DEBUGGING_MODE
			OutputDebugString(L"DONE!\n");
#endif
		}
	}
	else {
		AbortAcquisition();

		if (DRV_SUCCESS != CoolerOFF()) {
			OutputDebugString(L"COOLEROFF ERROR!\n");
		}

		while (camTemp < -20) {
			GetTemperature(&camTemp);
			emit temp_changed(camTemp);
		}

		if (DRV_SUCCESS != ShutDown()) {
			OutputDebugString(L"CAMERA SHUTDOWN ERROR!\n");
		}

#ifdef DEBUGGING_MODE
		OutputDebugString(L"DONE!\n");
#endif
	}

}

bool EMCCDWorker::AcquireImageData() {
	int errVal = GetNewData(pImageArray, imgSize);
	if (errVal != DRV_SUCCESS) {
		OutputDebugString(L"GETNEWIMG ERROR!\n");
		return FALSE;
	}

	if (!DrawLines()) {
		return FALSE;
	}

	return TRUE;
}

bool EMCCDWorker::DrawLines() {
	int 			i;
	BOOL 			bRetValue = TRUE;
	maxVal = 1;
	minVal = 65536;

	if (pImageArray != NULL) {

		// Find max value and scale data to fill rect
		for (i = 0; i < imgSize; i++) {
			if (pImageArray[i] > maxVal)
				maxVal = pImageArray[i];
			if (pImageArray[i] < minVal)
				minVal = pImageArray[i];
		}

		if (maxVal == minVal)
			return FALSE;

#ifdef DEBUGGING_MODE
		OutputDebugString(L"DRAWING!\n");
#endif

		long * imgcpy = (long *) calloc(sizeof(long), imgSize);
		memcpy(imgcpy, pImageArray, sizeof(long) * imgSize);
		emit new_image(imgcpy, minVal, maxVal);
	}
	else
		bRetValue = FALSE;
	return bRetValue;
}

EMCCDWorker::~EMCCDWorker()
{
}
