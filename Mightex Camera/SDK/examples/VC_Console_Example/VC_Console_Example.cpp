// VC_Console_Example.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <assert.h>
#include <conio.h>

#include <iostream>


#include "MT_USBCamera_SDK.h"

#define  GRAB_FRAME_FOREVER 0x8888


using namespace std;

int DeviceHandle;
TImageControl SettingCtl;
char showbuf[1024];

void CameraFaultCallBack( int ErrorType )
{
	assert(FALSE);

	// Note: It's recommended to stop the engine and close the application
	MTUSB_StopCameraEngine(DeviceHandle);
	MTUSB_UnInitDevice();
}

void FrameCallBack( int FrameType, int Row, int Col, 
                    TImageAttachData* Attributes, unsigned char *BytePtr )
{
	int i, j, PixelSum;
	unsigned char *sPtr;

	PixelSum = 0;
	sPtr = BytePtr;
	for ( i=0; i<Row; i++)
	{
		for ( j=0; j<Col; j++)
		{
			PixelSum = PixelSum + (int)*sPtr++;
		}
	}
	PixelSum = PixelSum / (Col*Row);
	sprintf( showbuf, "%dx%d, %d, ==> %d \n", Col, Row, Attributes->Reserved1, PixelSum );
	cout << showbuf;
	//cout << Attributes->Reserved1 << ":" << cout.flush();
}

BOOL StopCamera(void)
{
	MTUSB_StopFrameGrab(DeviceHandle);
	MTUSB_StopCameraEngine(DeviceHandle);
	MTUSB_UnInitDevice();
	return TRUE;
}

int main(int argc, char**argv)
{
	int ret;
	MSG   msg; 
//	HWND hw=NULL;
//	DWORD err;
//	CDialog cdia;

//	hw = CreateWindowEx(WS_EX_LEFT,"Message",NULL,WS_CHILD,CW_USEDEFAULT,0,CW_USEDEFAULT,0,HWND_MESSAGE,NULL,NULL,NULL);
//	if (hw==NULL)
//		err = GetLastError();

	ret = MTUSB_InitDevice();
	assert(ret==1);  // One camera only

	DeviceHandle = MTUSB_OpenDevice(0); // Open the camera.

	ret = MTUSB_InstallDeviceHooker( CameraFaultCallBack );

	ret = MTUSB_StartCameraEngine(NULL, DeviceHandle);
	assert(ret == 1);

	MTUSB_SetCameraWorkMode( DeviceHandle, 0); // Continuous mode
	MTUSB_GetFrameSetting( DeviceHandle, &SettingCtl);

	SettingCtl.Resolution = 7; // 1280x1024
	SettingCtl.BinMode = 1;
	SettingCtl.ImageRendorFitWindow = true;
	if (-1==MTUSB_SetResolution(DeviceHandle,&SettingCtl))
	{
		StopCamera();
		return FALSE;
	}
	SettingCtl.RedGain = 16;
	SettingCtl.GreenGain = 16;
	SettingCtl.BlueGain = 16;
	if ( -1==MTUSB_SetGain(DeviceHandle, &SettingCtl) )
	{
		StopCamera();
		return FALSE;
	}
	// User might set other parameters here.

	ret = MTUSB_StartFrameGrab(DeviceHandle, GRAB_FRAME_FOREVER);
	assert(ret==1);

    // 0=RAW, 1=BMP 
	// RAWDATA for B013 camera
	ret = MTUSB_InstallFrameHooker( DeviceHandle, true, RAWDATA_IMAGE, FrameCallBack );
	//ret = BUFUSB_InstallFrameHooker( 1, FrameCallBack );
	//ret = BUFUSB_InstallFrameHooker( 0, FrameCallBack2 );
	assert(ret==1);

	int quit = 0;
	while (!quit)
	{
		if (!_kbhit())
		{
			Sleep(20);
			
			// The following is to let camera engine to be active..it needs message loop.
			if(GetMessage(&msg,NULL,NULL,NULL))   
			{   
				//if(msg.message == WM_TIMER)   
				{   
					TranslateMessage(&msg);   
					DispatchMessage(&msg);   
				}   
			}
		}
		else
		{
			char ch;

			ch = _getch();

			switch(toupper(ch))
			{
			case 'Q':
				quit=1;
				break;
			default:
				break;
			}

		}
	}

	StopCamera();

//	if (hw)
//	{
//		err = DestroyWindow(hw);
//		if (err=0)
//			err = GetLastError();
//	}

	return 0;
}



