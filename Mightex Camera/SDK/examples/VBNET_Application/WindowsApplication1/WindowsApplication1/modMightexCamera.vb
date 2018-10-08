Imports System
Imports System.Text
Imports System.Runtime.InteropServices

Module modMightexCamera

    Delegate Sub MyFrameHookerDelegate(ByVal FrameType As Integer, ByVal row As Integer, ByVal Column As Integer, ByRef lpImageAttribute As TIMAGEATTACHDATA, ByRef Buffer As Byte)

    Public FrameCount As Integer
    Public Average As Integer
    Public BrightestPixel As Byte
    Public ImageBuf() As Byte
    Dim DeviceHandle As Integer
    Dim WindowHandle As IntPtr

    Dim FrameHooker1 As MyFrameHookerDelegate
    Dim FrameHooker2 As MyFrameHookerDelegate

    Public Const RAWDATA_IMAGE = 0
    Public Const BMPDATA_IMAGE = 1
    Public Const NORMAL_MODE = 0
    Public Const TRIGGER_MODE = 1

    <StructLayout(LayoutKind.Explicit)> _
    Public Structure TIMAGEATTACHDATA
        <FieldOffset(0)> Public XStart As Integer
        <FieldOffset(4)> Public YStart As Integer
        <FieldOffset(8)> Public GreenGain As Integer
        <FieldOffset(12)> Public BlueGain As Integer
        <FieldOffset(16)> Public RedGain As Integer
        <FieldOffset(20)> Public ExposureTime As Integer
        ' Start from SDK V1.4.0.0, we have 3 more fields:
        <FieldOffset(24)> Public TriggerOccurred As Integer
        <FieldOffset(28)> Public Reserved1 As Integer
        <FieldOffset(32)> Public Reserved2 As Integer
    End Structure
    <StructLayout(LayoutKind.Explicit)> _
    Public Structure IMAGECONTROL
        <FieldOffset(0)> Public Revision As Integer
        ' For Image Capture
        <FieldOffset(4)> Public Resolution As Integer
        <FieldOffset(8)> Public BinMode As Integer
        <FieldOffset(12)> Public XStart As Integer
        <FieldOffset(16)> Public YStart As Integer
        <FieldOffset(20)> Public GreenGain As Integer
        <FieldOffset(24)> Public BlueGain As Integer
        <FieldOffset(28)> Public RedGain As Integer
        <FieldOffset(32)> Public MaxExposureTimeIndex As Integer
        <FieldOffset(36)> Public ExposureTime As Integer
        ' For Image Rendor
        <FieldOffset(40)> Public ImageRendorFitWindow As Byte
        <FieldOffset(44)> Public Gamma As Integer
        <FieldOffset(48)> Public Contrast As Integer
        <FieldOffset(52)> Public Bright As Integer
        <FieldOffset(56)> Public SharpLevel As Integer
        <FieldOffset(60)> Public BWMode As Byte
        <FieldOffset(61)> Public HorizontalMirror As Byte
        <FieldOffset(62)> Public VerticalFlip As Byte
        ' For Capture Files.
        <FieldOffset(64)> Public CatchFrames As Integer
        <FieldOffset(68)> Public IsAverageFrame As Byte
        <FieldOffset(69)> Public IsCatchRAW As Byte
        <FieldOffset(70)> Public IsRawGraph As Byte
        <FieldOffset(71)> Public IsCatchJPEG As Byte
        <FieldOffset(72)> Public CatchIgnoreSkip As Byte
    End Structure

    <DllImport("MT_USBCamera_SDK_Stdcall.dll", CallingConvention:=CallingConvention.StdCall)> _
Function MTUSB_GetModuleNo(ByVal DevHandle As Integer, ByVal ModuleNo As StringBuilder) As Integer
    End Function


    'Function MTUSB_GetModuleNo(ByVal DevHandle As Integer, ByVal ModuleNo As StringBuilder) As Integer
    Declare Function MTUSB_InitDevice Lib "MT_USBCamera_SDK_Stdcall" () As Integer
    Declare Function MTUSB_UnInitDevice Lib "MT_USBCamera_SDK_Stdcall" () As Integer
    Declare Function MTUSB_OpenDevice Lib "MT_USBCamera_SDK_Stdcall" (ByVal DeviceID As Integer) As Integer
    Declare Function MTUSB_ShowOpenDeviceDialog Lib "MT_USBCamera_SDK_Stdcall" () As Integer

    Declare Function MTUSB_GetSerialNo Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByVal SerialNo As StringBuilder) As Integer

    Declare Function MTUSB_StartCameraEngine Lib "MT_USBCamera_SDK_Stdcall" (ByVal ParentHandle As Integer, ByVal DEV_HANDLE As Integer) As Integer
    Declare Function MTUSB_StopCameraEngine Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer) As Integer
    Declare Function MTUSB_SetCameraWorkMode Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByVal WorkMode As Integer) As Integer
    ' The following two APIs are not recommended...we recommend user to install frame callback for NORMAL and TRIGGER modes
    'Declare Function MTUSB_SetExternalParameters Lib " MT_USBCamera_SDK_Stdcall" ( ByVal DevHandle As Integer, ByVal AutoLoop As Byte, ByVal IsRawGraph as Byte, ByVal IsJPEG As Byte, ByVal FilePath as String, ByVal FileName As String) as long
    'Declare Function MTUSB_WaitingExternalTrigger Lib " MT_USBCamera_SDK_Stdcall" ( DEV_HANDLE DevHandle, bool StartWait, CallBackFunc Aproc );
    Declare Function MTUSB_ShowFrameControlPanel Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByVal IsTriggerModeAllow As Byte, _
                                                                                 ByVal CloseParent As Byte, ByVal Title As String, ByVal Left As Integer, ByVal Top As Integer) As Integer
    Declare Function MTUSB_HideFrameControlPanel Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer) As Integer
    Declare Function MTUSB_ShowVideoWindow Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByVal Top As Integer, ByVal Left As Integer, _
                                                                           ByVal Width As Integer, ByVal Height As Integer) As Integer
    Declare Function MTUSB_StartFrameGrab Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByVal TotalFrames As Integer) As Integer
    Declare Function MTUSB_StopFrameGrab Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer) As Integer
    Declare Function MTUSB_GetFrameSetting Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByRef lpImageCtl As IMAGECONTROL) As Integer
    Declare Function MTUSB_SetFrameSetting Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByRef lpImageCtl As IMAGECONTROL) As Integer
    'Declare Function MTUSB_SetFrameSetting Lib " MT_USBCamera_SDK_Stdcall" ( DEV_HANDLE DevHandle, PImageCtl SettingPtr);
    Declare Function MTUSB_SetResolution Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByRef lpImageCtl As IMAGECONTROL) As Integer
    Declare Function MTUSB_SetStartPosition Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByRef lpImageCtl As IMAGECONTROL) As Integer
    Declare Function MTUSB_SetGain Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByRef lpImageCtl As IMAGECONTROL) As Integer
    Declare Function MTUSB_SetExposureTime Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByRef lpImageCtl As IMAGECONTROL) As Integer
    Declare Function MTUSB_SetGammaValue Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByRef lpImageCtl As IMAGECONTROL) As Integer
    'Declare Function MTUSB_SetGammaTable Lib " MT_USBCamera_SDK_Stdcall" ( DEV_HANDLE DevHandle, unsigned char *GammaTable );
    Declare Function MTUSB_SetShowMode Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByRef lpImageCtl As IMAGECONTROL) As Integer
    Declare Function MTUSB_SetWhiteBalance Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer) As Integer
    Declare Function MTUSB_SetFrameRateLevel Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByVal RateLevel As Integer) As Integer
    Declare Function MTUSB_SetAutoExposure Lib "MT_USBCamera_SDK_Stdcall" (ByVal DEV_HANDLE As Integer, ByVal AutoExposureOn As Byte, ByVal ShowExposureMark As Byte) As Integer
    'Declare Function MTUSB_GetCurrentFrameRate Lib " MT_USBCamera_SDK_Stdcall" ( DEV_HANDLE DevHandle );
    Declare Function MTUSB_GetLastBMPFrame Lib "MT_USBCamera_SDK_Stdcall" (ByVal DEV_HANDLE As Integer, ByVal FileName As String) As Integer
    'Declare Function MTUSB_GetCurrentFrame Lib " MT_USBCamera_SDK_Stdcall" ( DEV_HANDLE DevHandle, int FrameType, unsigned char *Buffer );
    'Declare Function MTUSB_InstallFrameHooker Lib " MT_USBCamera_SDK_Stdcall" (ByVal DEV_HANDLE As Integer, ByVal IsMaxFrameRate As Byte, ByVal FrameType As Integer, ByVal FrameHooker As Integer) As Integer
    Declare Function MTUSB_InstallFrameHooker Lib "MT_USBCamera_SDK_Stdcall" (ByVal DEV_HANDLE As Integer, ByVal IsMaxFrameRate As Byte, ByVal FrameType As Integer, ByVal FrameHooker As MyFrameHookerDelegate) As Integer
    Declare Function MTUSB_SaveFramesToFiles Lib "MT_USBCamera_SDK_Stdcall" (ByVal DevHandle As Integer, ByRef lpImageCtl As IMAGECONTROL, ByVal FilePath As String, ByVal FileName As String) As Integer
    Declare Function MTUSB_SetLEDBrightness Lib "MT_USBCamera_SDK_Stdcall" (ByVal DEV_HANDLE As Integer, ByVal LEDChannel As Byte, ByVal Brightness As Byte) As Integer
    Declare Function MTUSB_SetGPIOConifg Lib "MT_USBCamera_SDK_Stdcall" (ByVal DEV_HANDLE As Integer, ByVal ConfigByte As Byte) As Integer
    Declare Function MTUSB_SetGPIOInOut Lib "MT_USBCamera_SDK_Stdcall" (ByVal DEV_HANDLE As Integer, ByVal OutputByte As Byte, ByRef InputBytePtr As Byte) As Integer
    Declare Function MTUSB_SetWindowsMovable Lib "MT_USBCamera_SDK_Stdcall" (ByVal IsControlPanelMovable As Byte, ByVal IsVideoWindowMovable As Byte) As Integer




    Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (ByRef Destination As Byte, ByRef Source As Byte, ByVal Length As Integer)
    Declare Function PostMessage Lib "user32" Alias "PostMessageA" (ByVal hwnd As IntPtr, ByVal wMsg As Integer, ByVal wParam As Integer, ByVal lParam As Integer) As IntPtr

    '
    'Declare Function GetWindowsDirectory Lib "kernel32" Alias "GetWindowsDirectoryA" (ByVal lpBuffer As String, ByVal nSize As Integer) As Integer
    'Declare Sub GetSystemInfo Lib "kernel32" (lpSystemInfo As SystemInfo)
    'Declare Sub GlobalMemoryStatus Lib "kernel32" (lpBuffer As MEMORYSTATUS)
    'Declare Function GetVersionEx Lib "kernel32" Alias "GetVersionExA" (ByRef lpVersionInformation As OSVERSIONINFO) As Integer
    'Declare Function GetSystemMetrics Lib "User32" (ByVal nIndex As Integer) As Integer
    'Declare Function GetDeviceCaps Lib "GDI32" (ByVal hDC As Integer, ByVal nIndex As Integer) As Integer
    'Declare Function TrackPopupMenu Lib "User32" (ByVal hMenu As Integer, ByVal wFlags As Integer, ByVal X As Integer, ByVal Y As Integer, ByVal nReserved As Integer, ByVal hWnd As Integer, lpReserved As Any) As Integer
    'Declare Function GetMenu Lib "User32" (ByVal hWnd As Integer) As Integer
    'Declare Function GetSubMenu Lib "User32" (ByVal hMenu As Integer, ByVal nPos As Integer) As Integer
    'Declare Function GetDesktopWindow Lib "User32" () As Integer
    'Declare Function GetDC Lib "User32" (ByVal hWnd As Integer) As Integer
    'Declare Function ReleaseDC Lib "User32" (ByVal hWnd As Integer, ByVal hDC As Integer) As Integer
    'Declare Function BitBlt Lib "GDI32" (ByVal hDestDC As Integer, ByVal X As Integer, ByVal Y As Integer, ByVal nWidth As Integer, ByVal nHeight As Integer, ByVal hSrcDC As Integer, ByVal XSrc As Integer, ByVal YSrc As Integer, ByVal dwRop As Integer) As Integer
    'Declare Sub SetWindowPos Lib "User32" (ByVal hWnd As Integer, ByVal hWndInsertAfter As Integer, ByVal X As Integer, ByVal Y As Integer, ByVal cx As Integer, ByVal cy As Integer, ByVal wFlags As Integer)
    'Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, lpKeyName As Any, ByVal lpDefault As String, ByVal lpRetunedString As String, ByVal nSize As Integer, ByVal lpFileName As String) As Integer
    'Declare Function GetProfileString Lib "kernel32" Alias "GetProfileStringA" (ByVal lpAppName As String, lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Integer) As Integer
    'Declare Function waveOutGetNumDevs Lib "winmm" () As Integer
    'Declare Function GetSystemDirectory Lib "kernel32" Alias "GetSystemDirectoryA" (ByVal lpBuffer As String, ByVal nSize As Integer) As Integer
    'Declare Function sndPlaySound Lib "winmm" Alias "sndPlaySoundA" (ByVal lpszSoundName As String, ByVal uFlags As Integer) As Integer


    Sub NoFrameHooker(ByVal FrameType As Integer, ByVal row As Integer, ByVal Column As Integer, ByRef lpImageAttribute As TIMAGEATTACHDATA, ByRef Buffer As Byte)
        'Nothing
    End Sub

    Sub MyFrameHooker(ByVal FrameType As Integer, ByVal row As Integer, ByVal Column As Integer, ByRef lpImageAttribute As TIMAGEATTACHDATA, ByRef Buffer As Byte)
        'Dim MyStr As String
        Dim i As Integer
        Dim TotalPixel As Integer
        Dim tmpBrightestPixel As Byte
        Dim M As New Message

        tmpBrightestPixel = 0

        FrameCount = FrameCount + 1
        ReDim ImageBuf(Column * row)

        ' We copy the image data to ImageBuf()...Note that this callback is invoked in a higher
        ' priority thread... so this callback should not be blocked or do any GUI operations
        ' (Although this example does a simple GUI showing for demo purpose)...we recommend user to put the
        ' Image data and attributes in buffer and post a message to main thread for further
        ' processing.
        CopyMemory(ImageBuf(0), Buffer, (Column * row))
        TotalPixel = 0
        For i = 0 To (Column * row - 1)
            TotalPixel = TotalPixel + ImageBuf(i)
            If ImageBuf(i) > tmpBrightestPixel Then tmpBrightestPixel = ImageBuf(i)
        Next
        Average = TotalPixel / (Column * row)

        BrightestPixel = tmpBrightestPixel
        'MyStr = "Frames: " & FrameCount & "   Resolution: " & Column & "x" & row & " Gain (" & lpImageAttribute.RedGain _
        '& "," & lpImageAttribute.GreenGain & "," & lpImageAttribute.BlueGain & ") PixelValue: " & Average
        'Form1.StatusLabel.Text = MyStr

        'frmMain.FrameInfoLabel.Text = "Brightest pixel: " & BrightestPixel & vbCr & "Average pixel: " & Average & vbCr & "Frames: " & FrameCount
        M.Msg = &H401   'User defined message
        M.HWnd = WindowHandle
        M.WParam = BrightestPixel
        M.LParam = FrameCount
        PostMessage(M.HWnd, M.Msg, M.WParam, M.LParam)
        'DoEvents
    End Sub



    '----- FUNCTIONS FOR STARTING AND STOPPING CAMERA -----'

    Public Sub CameraSTART()
        InitializeDevices()
        OpenDevice()
        GetModuleNo()
        GetSerialNo()
        StartCameraEngine()
        SetFrameSettings()
        InstallFrameHooker()
        StartGrab()
    End Sub

    Public Sub CameraSTOP()
        'Stops the camera
        'UninstallFrameHooker()
        StopGrab()
        StopCameraEngine()
        UninitDevices()
        MsgBox("Camera Stopped")
    End Sub



    Private Sub InitializeDevices()
        Dim TotalDevices As Integer
        Dim MyStr As String

        WindowHandle = frmMain.WinHwnd  'It's not changed...we keep it here.
        TotalDevices = MTUSB_InitDevice
        ' Important: We should have two delegate to reference the callbacks...otherwise
        ' GC will collect the callbacks back.
        FrameHooker1 = AddressOf MyFrameHooker
        FrameHooker2 = AddressOf NoFrameHooker
        If TotalDevices <> 1 Then
            MyStr = ("There are " & TotalDevices & " devices.")
            MsgBox(MyStr)
        End If
    End Sub

    Private Sub OpenDevice()
        DeviceHandle = MTUSB_OpenDevice(0)
    End Sub


    Private Sub GetModuleNo()
        Dim ModuleName As New StringBuilder(32)
        Dim Result As Integer
        'Dim block As IntPtr
        'ModuleName = Space(32)
        Result = MTUSB_GetModuleNo(DeviceHandle, ModuleName)
        'block = Marshal.AllocHGlobal(32)
        'Result = MTUSB_GetModuleNo(DeviceHandle, block.ToInt32)
        'Result = MTUSB_GetModuleNo(DeviceHandle, &H12345678)
        frmMain.ModuleNoLabel.Text = "ModuleNo.: " & ModuleName.ToString
        'Marshal.FreeHGlobal(block)
    End Sub

    Private Sub GetSerialNo()
        Dim SerialName As New StringBuilder(32)
        'SerialName = Space(32)
        MTUSB_GetSerialNo(DeviceHandle, SerialName)
        frmMain.SerialNoLabel.Text = "SerialNo.: " & SerialName.ToString
    End Sub

    Private Sub StartCameraEngine()
        MTUSB_StartCameraEngine(frmMain.Handle, DeviceHandle) 'frmMain.hWnd????
    End Sub

    Private Sub SetFrameSettings()
        Dim MyImageCtl As IMAGECONTROL
        Dim Result As Integer
        Result = MTUSB_GetFrameSetting(DeviceHandle, MyImageCtl)

        MyImageCtl.Resolution = 4
        '0 – 32 x 32
        '1 – 64 x 64
        '2 – 160 x 120
        '3 – 320 x 240
        '4 – 640 x 480
        '5 – 800 x 600
        '6 – 1024 x 768
        '7 – 1280 x 1024

        MyImageCtl.BinMode = 2  ' 2:1 mode

        MyImageCtl.XStart = 488 ' ((1280 / 2) / 2) - (320 / 2)
        MyImageCtl.YStart = 392 ' ((1024 / 2) - (240 / 2)) / 2

        MyImageCtl.RedGain = 127
        MyImageCtl.GreenGain = 127
        MyImageCtl.BlueGain = 127

        MyImageCtl.MaxExposureTimeIndex = 0
        MyImageCtl.ExposureTime = 1000 'microseconds


        MTUSB_SetFrameSetting(DeviceHandle, MyImageCtl)

        MTUSB_SetFrameRateLevel(DeviceHandle, 10)
    End Sub


    Private Sub InstallFrameHooker()
        MTUSB_InstallFrameHooker(DeviceHandle, 1, RAWDATA_IMAGE, FrameHooker1)
    End Sub

    Private Sub StartGrab()
        MTUSB_StartFrameGrab(DeviceHandle, 2000000)
    End Sub

    Private Sub UninstallFrameHooker()
        MTUSB_InstallFrameHooker(DeviceHandle, 1, RAWDATA_IMAGE, FrameHooker2)
    End Sub

    Private Sub StopGrab()
        MTUSB_StopFrameGrab(DeviceHandle)
    End Sub

    Private Sub StopCameraEngine()
        MTUSB_StopCameraEngine(DeviceHandle)
    End Sub

    Private Sub UninitDevices()
        MTUSB_UnInitDevice()
    End Sub
End Module
