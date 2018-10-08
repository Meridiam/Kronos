Attribute VB_Name = "Module1"
Option Explicit
'Dim WinVersion As Integer, SoundAvailable As Integer
'Public VisibleFrame As Frame
Dim FrameCount As Long
Dim ImageBuf() As Byte
 
Public Const RAWDATA_IMAGE = 0
Public Const BMPDATA_IMAGE = 1
Public Const NORMAL_MODE = 0
Public Const TRIGGER_MODE = 1


Public Type TIMAGEATTACHDATA
    XStart As Long
    YStart As Long
    GreenGain As Long
    BlueGain As Long
    RedGain As Long
    ExposureTime As Long
    ' Start from SDK V1.4.0.0, we have 3 more fields:
    TriggerOccurred As Long
    Reserved1 As Long
    Reserved2 As Long
End Type

Public Type IMAGECONTROL
    Revision As Long
    ' For Image Capture
    Resolution As Long
    BinMode As Long
    XStart As Long
    YStart As Long
    GreenGain As Long
    BlueGain As Long
    RedGain As Long
    MaxExposureTimeIndex As Long
    ExposureTime As Long
    ' For Image Rendor
    ImageRendorFitWindow As Byte
    Gamma As Long
    Contrast As Long
    Bright As Long
    SharpLevel As Long
    BWMode As Byte
    HorizontalMirror As Byte
    VerticalFlip As Byte
    ' For Capture Files.
    CatchFrames As Long
    IsAverageFrame As Byte
    IsCatchRAW As Byte
    IsRawGraph As Byte
    IsCatchJPEG As Byte
    CatchIgnoreSkip As Byte
End Type

Declare Function MTUSB_InitDevice Lib "MT_USBCamera_SDK_Stdcall" () As Long
Declare Function MTUSB_UnInitDevice Lib "MT_USBCamera_SDK_Stdcall" () As Long
Declare Function MTUSB_OpenDevice Lib "MT_USBCamera_SDK_Stdcall" (ByVal DeviceID As Long) As Long
Declare Function MTUSB_ShowOpenDeviceDialog Lib "MT_USBCamera_SDK_Stdcall" () As Long
Declare Function MTUSB_GetModuleNo Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByVal ModuleNo As String) As Long
Declare Function MTUSB_GetSerialNo Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByVal SerialNo As String) As Long

Declare Function MTUSB_StartCameraEngine Lib "MT_USBCamera_SDK_Stdcall" (ByVal ParentHandle As Long, ByVal DEV_HANDLE As Long) As Long
Declare Function MTUSB_StopCameraEngine Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long) As Long
Declare Function MTUSB_SetCameraWorkMode Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByVal WorkMode As Long) As Long
' The following two APIs are not recommended...we recommend user to install frame callback for NORMAL and TRIGGER modes
'Declare Function MTUSB_SetExternalParameters Lib "MT_USBCamera_SDK_Stdcall" ( ByVal DevHandle As Long, ByVal AutoLoop As Byte, ByVal IsRawGraph as Byte, ByVal IsJPEG As Byte, ByVal FilePath as String, ByVal FileName As String) as long
'Declare Function MTUSB_WaitingExternalTrigger Lib "MT_USBCamera_SDK_Stdcall" ( DEV_HANDLE DevHandle, bool StartWait, CallBackFunc Aproc );
Declare Function MTUSB_ShowFrameControlPanel Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByVal IsTriggerModeAllow As Byte, _
                                                                             ByVal CloseParent As Byte, ByVal Title As String, ByVal Left As Long, ByVal Top As Long) As Long
Declare Function MTUSB_HideFrameControlPanel Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long) As Long
Declare Function MTUSB_ShowVideoWindow Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByVal Top As Long, ByVal Left As Long, _
                                                                       ByVal Width As Long, ByVal Height As Long) As Long
Declare Function MTUSB_StartFrameGrab Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByVal TotalFrames As Long) As Long
Declare Function MTUSB_StopFrameGrab Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long) As Long
Declare Function MTUSB_GetFrameSetting Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByRef lpImageCtl As IMAGECONTROL) As Long
'Declare Function MTUSB_SetFrameSetting Lib "MT_USBCamera_SDK_Stdcall" ( DEV_HANDLE DevHandle, PImageCtl SettingPtr);
Declare Function MTUSB_SetResolution Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByRef lpImageCtl As IMAGECONTROL) As Long
Declare Function MTUSB_SetStartPosition Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByRef lpImageCtl As IMAGECONTROL) As Long
Declare Function MTUSB_SetGain Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByRef lpImageCtl As IMAGECONTROL) As Long
Declare Function MTUSB_SetExposureTime Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByRef lpImageCtl As IMAGECONTROL) As Long
Declare Function MTUSB_SetGammaValue Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByRef lpImageCtl As IMAGECONTROL) As Long
'Declare Function MTUSB_SetGammaTable Lib "MT_USBCamera_SDK_Stdcall" ( DEV_HANDLE DevHandle, unsigned char *GammaTable );
Declare Function MTUSB_SetShowMode Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByRef lpImageCtl As IMAGECONTROL) As Long
Declare Function MTUSB_SetWhiteBalance Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long) As Long
Declare Function MTUSB_SetFrameRateLevel Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByVal RateLevel As Long) As Long
Declare Function MTUSB_SetAutoExposure Lib "MT_USBCamera_SDK_Stdcall" (ByVal DEV_HANDLE As Long, ByVal AutoExposureOn As Byte, ByVal ShowExposureMark As Byte) As Long
'Declare Function MTUSB_GetCurrentFrameRate Lib "MT_USBCamera_SDK_Stdcall" ( DEV_HANDLE DevHandle );
Declare Function MTUSB_GetLastBMPFrame Lib "MT_USBCamera_SDK_Stdcall" (ByVal DEV_HANDLE As Long, ByVal FileName As String) As Long
'Declare Function MTUSB_GetCurrentFrame Lib "MT_USBCamera_SDK_Stdcall" ( DEV_HANDLE DevHandle, int FrameType, unsigned char *Buffer );
Declare Function MTUSB_InstallFrameHooker Lib "MT_USBCamera_SDK_Stdcall" (ByVal DEV_HANDLE As Long, ByVal IsMaxFrameRate As Byte, ByVal FrameType As Long, ByVal FrameHooker As Long) As Long
Declare Function MTUSB_SaveFramesToFiles Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Long, ByRef lpImageCtl As IMAGECONTROL, ByVal FilePath As String, ByVal FileName As String) As Long
Declare Function MTUSB_SetLEDBrightness Lib "MT_USBCamera_SDK_Stdcall" (ByVal DEV_HANDLE As Long, ByVal LEDChannel As Byte, ByVal Brightness As Byte) As Long
Declare Function MTUSB_SetGPIOConifg Lib "MT_USBCamera_SDK_Stdcall" (ByVal DEV_HANDLE As Long, ByVal ConfigByte As Byte) As Long
Declare Function MTUSB_SetGPIOInOut Lib "MT_USBCamera_SDK_Stdcall" (ByVal DEV_HANDLE As Long, ByVal OutputByte As Byte, ByRef InputBytePtr As Byte) As Long
Declare Function MTUSB_SetWindowsMovable Lib "MT_USBCamera_SDK_Stdcall" (ByVal IsControlPanelMovable As Byte, ByVal IsVideoWindowMovable As Byte) As Long


Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)
'
'Declare Function GetWindowsDirectory Lib "kernel32" Alias "GetWindowsDirectoryA" (ByVal lpBuffer As String, ByVal nSize As Long) As Long
'Declare Sub GetSystemInfo Lib "kernel32" (lpSystemInfo As SystemInfo)
'Declare Sub GlobalMemoryStatus Lib "kernel32" (lpBuffer As MEMORYSTATUS)
'Declare Function GetVersionEx Lib "kernel32" Alias "GetVersionExA" (ByRef lpVersionInformation As OSVERSIONINFO) As Long
'Declare Function GetSystemMetrics Lib "User32" (ByVal nIndex As Long) As Long
'Declare Function GetDeviceCaps Lib "GDI32" (ByVal hDC As Long, ByVal nIndex As Long) As Long
'Declare Function TrackPopupMenu Lib "User32" (ByVal hMenu As Long, ByVal wFlags As Long, ByVal X As Long, ByVal Y As Long, ByVal nReserved As Long, ByVal hWnd As Long, lpReserved As Any) As Long
'Declare Function GetMenu Lib "User32" (ByVal hWnd As Long) As Long
'Declare Function GetSubMenu Lib "User32" (ByVal hMenu As Long, ByVal nPos As Long) As Long
'Declare Function GetDesktopWindow Lib "User32" () As Long
'Declare Function GetDC Lib "User32" (ByVal hWnd As Long) As Long
'Declare Function ReleaseDC Lib "User32" (ByVal hWnd As Long, ByVal hDC As Long) As Long
'Declare Function BitBlt Lib "GDI32" (ByVal hDestDC As Long, ByVal X As Long, ByVal Y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal XSrc As Long, ByVal YSrc As Long, ByVal dwRop As Long) As Long
'Declare Sub SetWindowPos Lib "User32" (ByVal hWnd As Long, ByVal hWndInsertAfter As Long, ByVal X As Long, ByVal Y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long)
'Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, lpKeyName As Any, ByVal lpDefault As String, ByVal lpRetunedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long
'Declare Function GetProfileString Lib "kernel32" Alias "GetProfileStringA" (ByVal lpAppName As String, lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Long) As Long
'Declare Function waveOutGetNumDevs Lib "winmm" () As Long
'Declare Function GetSystemDirectory Lib "kernel32" Alias "GetSystemDirectoryA" (ByVal lpBuffer As String, ByVal nSize As Long) As Long
'Declare Function sndPlaySound Lib "winmm" Alias "sndPlaySoundA" (ByVal lpszSoundName As String, ByVal uFlags As Long) As Long

Sub MyFrameHooker(ByVal FrameType As Long, ByVal row As Long, ByVal Column As Long, ByRef lpImageAttribute As TIMAGEATTACHDATA, ByRef Buffer As Byte)
    Dim MyStr As String
    Dim i As Long
    Dim TotalPixel As Long
    Dim Average As Long
    

    FrameCount = FrameCount + 1
    ReDim ImageBuf(Column * row)
       
    ' We copy the image data to ImageBuf()...Note that this callback is invoked in a higher
    ' priority thread... so this callback should not be blocked or do any GUI operations
    ' (Although this example does a simple GUI showing for demo purpose)...we recommend user to put the
    ' Image data and attributes in buffer and post a message to main thread for further
    ' processing.
    CopyMemory ImageBuf(0), Buffer, (Column * row)
    TotalPixel = 0
    For i = 0 To (Column * row - 1)
        TotalPixel = TotalPixel + ImageBuf(i)
    Next
    Average = TotalPixel / (Column * row)
        
    MyStr = "Frames: " & FrameCount & "   Resolution: " & Column & "x" & row & " Gain (" & lpImageAttribute.RedGain _
     & "," & lpImageAttribute.GreenGain & "," & lpImageAttribute.BlueGain & ") PixelValue: " & Average
    Form1.StatusLabel.Caption = MyStr
    
End Sub
