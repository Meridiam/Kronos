//------------------------------------------------------------------------------
// File: videoShow.h
//
// Desc: DirectShow code - Video privewing and capturing.
//
// Copyright (c) Mightex Corporation.  All rights reserved.
//------------------------------------------------------------------------------

#pragma once

#ifndef __Mightex_VideoShow_DEFINED
#define __Mightex_VideoShow_DEFINED

#include <atlbase.h>
#include <dshow.h>
#include <dshowutil.h>
#include <streams.h>

enum eDEVICETYPE {MT9M001_M, MT9M001_C, MT9V032_M, MT9V032_C, MT9T001, MT9P001};

const MAX_RESOLUTIONS_V032 = 4;  
const MAX_RESOLUTIONS_M001 = 7;
const MAX_RESOLUTIONS_T001 = 9;
const MAX_EXPOSURETIME[]={5, 10, 100, 750};
const DEFAULT_WIDTH = 640;
const DEFAULT_HEIGHT = 480;

/********************************************** 
 * 
 *  struct declarations
 * 
 **********************************************/

struct IMightex_ClassicSource;

static struct { int width; int height; }
s_vidFrameSize[] = 
{
	{  32,   32 }, 
	{  64 ,  64 },
	{  160,  120}, 
	{  320,  240}, 
	{  640,  480}, 
	{  800,  600}, 
	{ 1024,  768}, 
	{ 1280, 1024}, 
	{ 1600, 1200}, 
	{ 2048, 1536}, 
	{ 2592, 1944},
};

struct CGrabFrameToFileCtl
{
	char TargetFile[256];
	BOOL SavetoFileNeeded;
	BOOL SaveAsJPEG;
	BOOL AppendDataTime;
	BOOL SwitchSkipModeNeeded;
	int SaveFileCount;
	int SavedCount;
};

struct CCameraControl
{
	char ModuleNo[32];
	char SerialNo[32];
	eDEVICETYPE DeviceType;
	int deviceID;
	int workMode;
	int bin;
	int width;
	int height;
	int MAX_RESOLUTION;
	int exposureTime;
	int xStart;
	int	yStart;
	int redGain;
	int greenGain;
	int blueGain;
	LONGLONG streamFrameRate;
	LONGLONG actualFrameRate;
};

struct CCameraGlobalControl
{
	int Gamma;
	int Contrast;
	int Brightness;
	int Sharpness;
	int BWMode;
	int H_Mirror;
	int V_Flip;
};

/********************************************** 
 * 
 *  Class declarations
 * 
 **********************************************/

class CMightex_VideoShow
{
	IVideoWindow  * g_pVW;
	IMediaControl * g_pMC;
	IGraphBuilder * g_pGraph;
	//IMediaEventEx * g_pME;
	//ICaptureGraphBuilder2 * g_pCapture;
	//IReferenceClock * g_pRC;
	//IMediaFilter * g_pMF;
    IBaseFilter * pMightex_ClassicSourceFilter;
	IMightex_ClassicSource * mIMightex_ClassicSource;
	IBaseFilter * pVideoRendererFilter;
	IBaseFilter * pSmartTee;
	IBaseFilter * pCompressor;
	IBaseFilter * pAviDest;
	IConfigInterleaving * pConfigInterleaving;
	//IConfigAviMux * pConfigAviMux;
    //IBaseFilter * pACap;
	IBaseFilter * pFileWriter;
    IFileSinkFilter * pFS;
	IAMStreamConfig * pSC;
	CComPtr<IPin> pSmartTeeOut0;
	CComPtr<IPin> pSmartTeeOut1;
	CComPtr<IPin> pSmartTeeIn;
	CComPtr<IPin> pVideoRendererFilterIn;
	CComPtr<IPin> pMightex_ClassicSourceFilterOut;

	HWND old_ghApp;
	BOOL isPreviewing;
	BOOL isCapturing;
	BOOL isCompressing;
	CGrabFrameToFileCtl saveFileCtl;
	CCameraControl cameraCtrl;
	CCameraGlobalControl cameraGlobalCtl;
	int compressorNum[256];
	
	HRESULT InitCameraControl();
	HRESULT GetInterfaces(void);
	void GetCompressor(int n, IBaseFilter ** ppCompressor);
	HRESULT ConnectFilters(CComPtr<IGraphBuilder> graph_, CComPtr<IBaseFilter> pUpFilter, CComPtr<IBaseFilter> pDownFilter);
	HRESULT ResizeVideoWindow(HWND ghApp);
	HRESULT SetupVideoWindow(HWND ghApp);
	
public:

    CMightex_VideoShow();
    ~CMightex_VideoShow();

	HRESULT Init();
	HRESULT AddCompressorsToList(CStringList &m_CompressorList);
	HRESULT Preview(HWND ghApp = NULL);
	HRESULT StopPreview();
	HRESULT Capture(WCHAR * targetFile = NULL, int compressorNum = -2);
	HRESULT StopCapture();
	HRESULT Snapshot(char * TargetFile, BOOL SavetoFileNeeded, BOOL SaveAsJPEG, BOOL AppendDataTime, BOOL SwitchSkipModeNeeded, int SaveFileCount, int SavedCount);
	BOOL GetPreviewState()	{ return isPreviewing; }
	BOOL GetCaptureState()	{ return isCapturing; }
	HRESULT GetCameraControl(CCameraControl &cameraCtrl);
	HRESULT SetStreamFrameRate(LONGLONG frameRate);
	HRESULT SetCameraWorkMode(int WorkMode);
	HRESULT SetResolution(int width, int height, int bin);
	HRESULT SetExposureTime(int exposureTime);
	HRESULT SetGains(int redGain, int greenGain, int blueGain);
	HRESULT SetXYStart(int xStart, int yStart);
	HRESULT OpenPropertyPages(HWND ghApp);		
	HRESULT SetWindowCaption();
	HRESULT SetGPIOConifg(unsigned char ConfigByte);		
	HRESULT SetGPIOInOut(unsigned char OutputByte, unsigned char *InputBytePtr);
};

#endif