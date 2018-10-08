unit DllInterface;

interface

uses windows, graphics;

type
  PByte = ^Byte;
  PWord = ^Word;
  PLong = ^Integer;
  PBitMap = ^TBitMap;

  PImageAttachData = ^TImageAttachData;
  TImageAttachData = packed record
    XStart : integer;
    YStart : integer;
    GreenGain : integer;
    BlueGain : integer;
    RedGain : integer;
    ExposureTime : integer;
    // Start from SDK DLL V1.4.0.0, we have 3 additional fields:
    TriggerOccurred : integer;
    Reserved1 : integer;
    Reserved2 : integer;    
  end;

  PImageControl = ^TImageControl;
  TImageControl = packed record
    Revision : integer;
    // For Image Capture
    Resolution : integer; // 0..4
    BinMode : integer;
    XStart : integer;
    YStart : integer;
    GreenGain : integer;
    BlueGain : integer;
    RedGain : integer;
    MaxExposureTimeIndex : integer;
    ExposureTime : integer;
    // For Image Rendor
    ImageRendorFitWindow : Boolean;
    Gamma : integer;
    Contrast : integer;
    Bright : integer;
    SharpLevel : integer;
    BWMode : Boolean;
    HorizontalMirror : Boolean;
    VerticalFlip : Boolean;
    // For Capture Files.
    CatchFrames : integer;
    IsAverageFrame: Boolean;    
    IsCatchRAW  : Boolean;
    IsRawRGBBmp : Boolean;
    IsCatchJPEG : Boolean;
    CatchIgnoreSkip : Boolean;
  end;
  
TTriggerDoneCallBack = procedure( SeqNo: integer; FileName : PChar ); cdecl;
TGetFrameCallBack = procedure( FrameType: integer; Row : integer; Col : integer;
                               ImageAttribute : PImageAttachData; FramePtr : PByte ); cdecl;

// General functions
function MTUSBInitDevice() : integer; cdecl;
function MTUSBUnInitDevice() :integer; cdecl;
function MTUSBOpenDevice( deviceID : integer ) : THandle ; cdecl;
function MTUSBShowOpenDeviceDialog() : THandle ; cdecl;
function MTUSBGetModuleNo( DeviceHandle : THandle; ModuleNo : PChar ): integer; cdecl;
function MTUSBGetSerialNo( DeviceHandle : THandle; SerialNo : PChar ): integer; cdecl;
function MTUSBStartCameraEngine( ParentHandle : THandle; DeviceHandle : THandle ) : integer ; cdecl;
function MTUSBStopCameraEngine( DeviceHandle : THandle ) : integer ; cdecl;
function MTUSBSetCameraWorkMode( DeviceHandle : THandle ;
                                 WorkMode : Integer ) : integer; cdecl;
function MTUSBSetExternalParameters( DeviceHandle : THandle; AutoLoop : Boolean;  IsRawGraph : Boolean;
                                      IsJPEG : Boolean; FilePath : PChar; FileName : PChar ): integer; cdecl;
function MTUSBWaitingExternalTrigger( DeviceHandle : THandle ; StartWaiting : Boolean ;
                                       TriggerCallBack : TTriggerDoneCallBack ): integer; cdecl;
function MTUSBShowFrameControlPanel( DeviceHandle : THandle ; IsTriggerModeAllow : Boolean ; CloseParent: Boolean; Title : PChar ; 
                                     Left : integer ; Top : integer ) : integer; cdecl;
function MTUSBHideFrameControlPanel( DeviceHandle : THandle ) : integer; cdecl;
function MTUSBShowVideoWindow( DeviceHandle : THandle;
                                Top : integer; Left : integer;
                                Width : integer; Height : integer ) : integer ; cdecl;
function MTUSBStartFrameGrab( DeviceHandle : THandle; TotalFrames : Integer ) : integer ; cdecl;
function MTUSBStopFrameGrab( DeviceHandle : THandle ) : integer ; cdecl;
function MTUSBGetFrameSetting( DeviceHandle : THandle ;
                               SettingPtr : PImageControl ) : integer; cdecl;
function MTUSBSetFrameSetting( DeviceHandle : THandle ;
                               SettingPtr : PImageControl ) : integer; cdecl;
function MTUSBSetResolution( DeviceHandle : THandle ;
                             SettingPtr : PImageControl ):integer; cdecl;
function MTUSBSetStartPosition( DeviceHandle : THandle ;
                                SettingPtr : PImageControl ):integer; cdecl;
function MTUSBSetGain( DeviceHandle : THandle ;
                       SettingPtr : PImageControl ):integer; cdecl;
function MTUSBSetExposureTime( DeviceHandle : THandle ;
                               SettingPtr : PImageControl ):integer; cdecl;
function MTUSBSetGammaValue( DeviceHandle : THandle ;
                              SettingPtr : PImageControl ):integer; cdecl;
function MTUSBSetGammaTable( DeviceHandle : THandle ;
                              GammaTable : PByte ):integer; cdecl;
function MTUSBSetShowMode( DeviceHandle : THandle ;
                            SettingPtr : PImageControl ):integer; cdecl;
function MTUSBSetWhiteBalance( DeviceHandle : THandle ):integer; cdecl;
function MTUSBSetFrameRateLevel( DeviceHandle : THandle; Level : integer ) : integer; cdecl;
function MTUSBSetAutoExposure( DeviceHandle : THandle; AutoExposureOn : Boolean ;
                               ShowAutoExposureMark : Boolean ) : integer; cdecl;
function MTUSBGetCurrentFrameRate( DeviceHandle : THandle ) : integer; cdecl;
function MTUSBGetLastBMPFrame( DeviceHandle : THandle ; FileName : PChar ) :integer ; cdecl;
function MTUSBGetLastBMPObject( DeviceHandle : THandle ; NewBmp : TBitmap ) : integer ; cdecl;
function MTUSBGetCurrentFrame( DeviceHandle : THandle ;
                               FrameType : integer ; Buffer : PBYTE ) : integer; cdecl;
function MTUSBInstallFrameHooker( DeviceHandle : THandle ; IsMaxFrameRate: Boolean;
                            FrameType : integer ; FrameHooker : TGetFrameCallBack  ) : integer; cdecl;
function MTUSBSaveFramesToFiles( DeviceHandle : THandle ; SettingPtr : PImageControl;
                                 FilePath : PChar; FileName : PChar ) : integer; cdecl;
function MTUSBSetLEDBrightness( DeviceHandle : THandle ; LEDChannel : Byte ;
                                 Brightness : Byte ) : integer; cdecl;
function MTUSBSetGPIOConifg( DeviceHandle : THandle ; ConfigByte : Byte ) : integer; cdecl;
function MTUSBSetGPIOInOut( DeviceHandle : THandle ; OutputByte : Byte;
                             InputBytePtr : PByte ) : integer; cdecl;


implementation

function MTUSBInitDevice; external 'MT_USBCamera_SDK.dll' name 'MTUSB_InitDevice';
function MTUSBUnInitDevice; external 'MT_USBCamera_SDK.dll' name 'MTUSB_UnInitDevice';
function MTUSBOpenDevice; external 'MT_USBCamera_SDK.dll' name 'MTUSB_OpenDevice';
function MTUSBShowOpenDeviceDialog; external 'MT_USBCamera_SDK.dll' name 'MTUSB_ShowOpenDeviceDialog';
function MTUSBGetModuleNo; external 'MT_USBCamera_SDK.dll' name 'MTUSB_GetModuleNo';
function MTUSBGetSerialNo; external 'MT_USBCamera_SDK.dll' name 'MTUSB_GetSerialNo';
function MTUSBStartCameraEngine; external 'MT_USBCamera_SDK.dll' name 'MTUSB_StartCameraEngine';
function MTUSBStopCameraEngine; external 'MT_USBCamera_SDK.dll' name 'MTUSB_StopCameraEngine';
function MTUSBSetCameraWorkMode; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetCameraWorkMode';
function MTUSBSetExternalParameters; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetExternalParameters';
function MTUSBWaitingExternalTrigger; external 'MT_USBCamera_SDK.dll' name 'MTUSB_WaitingExternalTrigger';
function MTUSBShowFrameControlPanel; external 'MT_USBCamera_SDK.dll' name 'MTUSB_ShowFrameControlPanel';
function MTUSBHideFrameControlPanel; external 'MT_USBCamera_SDK.dll' name 'MTUSB_HideFrameControlPanel';
function MTUSBShowVideoWindow; external 'MT_USBCamera_SDK.dll' name 'MTUSB_ShowVideoWindow';
function MTUSBStartFrameGrab; external 'MT_USBCamera_SDK.dll' name 'MTUSB_StartFrameGrab';
function MTUSBStopFrameGrab; external 'MT_USBCamera_SDK.dll' name 'MTUSB_StopFrameGrab';
function MTUSBGetFrameSetting; external 'MT_USBCamera_SDK.dll' name 'MTUSB_GetFrameSetting';
function MTUSBSetFrameSetting; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetFrameSetting';
function MTUSBSetResolution; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetResolution';
function MTUSBSetStartPosition; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetStartPosition';
function MTUSBSetGain; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetGain';
function MTUSBSetExposureTime; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetExposureTime';
function MTUSBSetGammaValue; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetGammaValue';
function MTUSBSetGammaTable; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetGammaTable';
function MTUSBSetShowMode; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetShowMode';
function MTUSBSetWhiteBalance; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetWhiteBalance';
function MTUSBSetFrameRateLevel; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetFrameRateLevel';
function MTUSBSetAutoExposure; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetAutoExposure';
function MTUSBGetCurrentFrameRate; external 'MT_USBCamera_SDK.dll' name 'MTUSB_GetCurrentFrameRate'
function MTUSBGetLastBMPFrame; external 'MT_USBCamera_SDK.dll' name 'MTUSB_GetLastBMPFrame';
function MTUSBGetLastBMPObject; external 'MT_USBCamera_SDK.dll' name 'MTUSB_GetLastBMPObject';
function MTUSBGetCurrentFrame; external 'MT_USBCamera_SDK.dll' name 'MTUSB_GetCurrentFrame';
function MTUSBInstallFrameHooker; external 'MT_USBCamera_SDK.dll' name 'MTUSB_InstallFrameHooker';
function MTUSBSaveFramesToFiles; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SaveFramesToFiles';
function MTUSBSetLEDBrightness; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetLEDBrightness';
function MTUSBSetGPIOConifg; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetGPIOConifg';
function MTUSBSetGPIOInOut; external 'MT_USBCamera_SDK.dll' name 'MTUSB_SetGPIOInOut';

end.

