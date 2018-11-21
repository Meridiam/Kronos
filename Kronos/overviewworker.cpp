#include "stdafx.h"
#include "overviewworker.h"

overviewworker::overviewworker(QObject *parent)
	: QObject(parent),
	pImageArray(NULL),
	buf_size(640*480*3)
{
	qRegisterMetaType<DEV_HANDLE>("DEV_HANDLE");
}

overviewworker::~overviewworker()
{
}

void overviewworker::overview_init(DEV_HANDLE device_handle) {
	PImageCtl psettings;

	this->dev_handle = device_handle;
	if (MTUSB_StartCameraEngine(NULL, dev_handle) != 1) {
		OutputDebugString(L"ERROR STARTING OVERVIEW CAMERA ENGINE!");
	}
	if (MTUSB_GetFrameSetting(dev_handle, psettings) != 1) {
		OutputDebugString(L"ERROR GETTING FRAME SETTINGS");
	}

	psettings->Resolution = 4;
	psettings->BinMode = 1;
	psettings->ImageRendorFitWindow = false;

	if (MTUSB_SetResolution(dev_handle, psettings) != 1) {
		OutputDebugString(L"ERROR SETTING FRAME SETTINGS");
	}
	if (MTUSB_StartFrameGrab(dev_handle, 0x8888) != 1) {
		OutputDebugString(L"ERROR STARTING FRAME GRAB FOR OVERVIEW!");
	}

	get_newest_img();
}

void overviewworker::alloc_buffer() {
	free_buffer();
	if (!pImageArray)
		pImageArray = (unsigned char *)calloc(sizeof(unsigned char), buf_size);
}

void overviewworker::free_buffer() {
	if (pImageArray) {
		free(pImageArray);
		pImageArray = NULL;
	}
}

void overviewworker::get_newest_img()
{
	
}

void overviewworker::stop_worker()
{
	MTUSB_StopCameraEngine(dev_handle);
}
