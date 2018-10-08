//------------------------------------------------------------------------------
// File: IMightex_ClassicSource.h
//
// Desc: DirectShow code - custom interface allowing the user
//       to do some settings.
//
// Copyright (c) Mightex Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef __H_IMightex_ClassicSource__
#define __H_IMightex_ClassicSource__

#ifdef __cplusplus
extern "C" {
#endif
	
	// {2A7D10A2-186F-4e34-8532-B5FBDE4739EE}
	DEFINE_GUID(CLSID_Mightex_ClassicSource, 
		0x2a7d10a2, 0x186f, 0x4e34, 0x85, 0x32, 0xb5, 0xfb, 0xde, 0x47, 0x39, 0xee);
	// {5822540F-0385-461c-8CCF-792D1E8D0518}
	DEFINE_GUID(IID_IMightex_ClassicSource, 
		0x5822540f, 0x385, 0x461c, 0x8c, 0xcf, 0x79, 0x2d, 0x1e, 0x8d, 0x5, 0x18);

	struct CCameraControl;
	struct CCameraGlobalControl;
	
    DECLARE_INTERFACE_(IMightex_ClassicSource, IUnknown)
    {
        STDMETHOD(SetStreamFrameRate) (THIS_
			LONGLONG frameRate       
			) PURE;
        STDMETHOD(SetPhysicalCameraFrameRate) (THIS_
			int frameRateLevel       
			) PURE;
        STDMETHOD(GetPhysicalCameraFrameRate) (THIS_
			LONGLONG &actualFrameRate       
			) PURE;
        STDMETHOD(GetModuleNoSerialNo) (THIS_
			char *ModuleNo, char *SerialNo       
			) PURE;
        STDMETHOD(SetCameraWorkMode) (THIS_
			int WorkMode       
			) PURE;
        STDMETHOD(SetResolution) (THIS_
			int width, int height, int bin       
			) PURE;
        STDMETHOD(SetExposureTime) (THIS_
			int exposureTime       
			) PURE;
        STDMETHOD(SetXYStart) (THIS_
			int xStart, int yStart       
			) PURE;
        STDMETHOD(SetGains) (THIS_
			int redGain, int greenGain, int blueGain       
			) PURE;
        STDMETHOD(SetGamma) (THIS_
			int Gamma, int Contrast, int Bright, int Sharp       
			) PURE;
        STDMETHOD(SetBWMode) (THIS_
			int BWMode, int H_Mirror, int V_Flip       
			) PURE;
        STDMETHOD(Snapshot) (THIS_
			char * TargetFile, BOOL SaveAsJPEG, BOOL AppendDataTime, int SaveFileCount       
			) PURE;
        STDMETHOD(GetCameraControl) (THIS_
			CCameraControl &cameraCtrl, CCameraGlobalControl &cameraGlobalCtl       
			) PURE;
        STDMETHOD(GetLastBMPFrame) (THIS_
			char *FileName       
			) PURE;
        STDMETHOD(GetCurrentFrame) (THIS_
			unsigned char *Buffer       
			) PURE;
        STDMETHOD(SetGPIOConifg) (THIS_
			unsigned char ConfigByte       
			) PURE;
        STDMETHOD(SetGPIOInOut) (THIS_
			unsigned char OutputByte, unsigned char *InputBytePtr       
			) PURE;
    };

#ifdef __cplusplus
}
#endif

#endif // __ICONTRAST__

