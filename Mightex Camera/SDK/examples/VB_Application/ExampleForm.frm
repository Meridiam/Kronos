VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   8505
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   8175
   LinkTopic       =   "Form1"
   ScaleHeight     =   8505
   ScaleWidth      =   8175
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command23 
      Caption         =   "Set Window Unmovable"
      Height          =   375
      Left            =   5760
      TabIndex        =   26
      Top             =   2640
      Width           =   2295
   End
   Begin VB.TextBox Text1 
      Height          =   1455
      Left            =   240
      MultiLine       =   -1  'True
      TabIndex        =   25
      Text            =   "ExampleForm.frx":0000
      Top             =   6120
      Width           =   7695
   End
   Begin VB.CommandButton Command22 
      Caption         =   "UnInstall Frame Hooker"
      Height          =   375
      Left            =   5880
      TabIndex        =   24
      Top             =   5400
      Width           =   2175
   End
   Begin VB.CommandButton Command21 
      Caption         =   "Install Frame Hooker"
      Height          =   375
      Left            =   5880
      TabIndex        =   22
      Top             =   4920
      Width           =   2175
   End
   Begin VB.CommandButton Command20 
      Caption         =   "Set Output"
      Height          =   375
      Left            =   2040
      TabIndex        =   21
      Top             =   5640
      Width           =   1695
   End
   Begin VB.CommandButton Command19 
      Caption         =   "Set IO Config"
      Height          =   375
      Left            =   120
      TabIndex        =   20
      Top             =   5640
      Width           =   1695
   End
   Begin VB.CommandButton Command18 
      Caption         =   "Set To Trigger Mode"
      Height          =   375
      Left            =   2040
      TabIndex        =   19
      Top             =   4920
      Width           =   1695
   End
   Begin VB.CommandButton Command17 
      Caption         =   "Set to Video Mode"
      Height          =   375
      Left            =   120
      TabIndex        =   18
      Top             =   4920
      Width           =   1695
   End
   Begin VB.CommandButton Command16 
      Caption         =   "Save Image To Files"
      Height          =   375
      Left            =   120
      TabIndex        =   17
      Top             =   4320
      Width           =   1695
   End
   Begin VB.CommandButton Command15 
      Caption         =   "Set Exposure Time"
      Height          =   375
      Left            =   3720
      TabIndex        =   16
      Top             =   3360
      Width           =   1815
   End
   Begin VB.CommandButton Command14 
      Caption         =   "Set Gains"
      Height          =   375
      Left            =   2040
      TabIndex        =   15
      Top             =   3360
      Width           =   1455
   End
   Begin VB.CommandButton Command13 
      Caption         =   "Set Resolution"
      Height          =   375
      Left            =   120
      TabIndex        =   14
      Top             =   3360
      Width           =   1695
   End
   Begin VB.CommandButton Command12 
      Caption         =   "Stop Grab"
      Height          =   375
      Left            =   4080
      TabIndex        =   13
      Top             =   2640
      Width           =   1575
   End
   Begin VB.CommandButton Command11 
      Caption         =   "Start Grab"
      Height          =   375
      Left            =   2160
      TabIndex        =   12
      Top             =   2640
      Width           =   1695
   End
   Begin VB.CommandButton Command10 
      Caption         =   "Show Video Window"
      Height          =   375
      Left            =   5760
      TabIndex        =   11
      Top             =   2160
      Width           =   1815
   End
   Begin VB.CommandButton Command9 
      Caption         =   "Hide Control Panel"
      Height          =   375
      Left            =   4080
      TabIndex        =   10
      Top             =   2160
      Width           =   1575
   End
   Begin VB.CommandButton Command8 
      Caption         =   "Show Control Panel"
      Height          =   375
      Left            =   2160
      TabIndex        =   9
      Top             =   2160
      Width           =   1695
   End
   Begin VB.CommandButton Command7 
      Caption         =   "Stop Camera Engine"
      Height          =   375
      Left            =   4440
      TabIndex        =   8
      Top             =   120
      Width           =   1815
   End
   Begin VB.CommandButton Command6 
      Caption         =   "Start Camera Engine"
      Height          =   375
      Left            =   120
      TabIndex        =   7
      Top             =   2160
      Width           =   1695
   End
   Begin VB.CommandButton Command5 
      Caption         =   "Get Serial No."
      Height          =   375
      Left            =   120
      TabIndex        =   5
      Top             =   1560
      Width           =   1575
   End
   Begin VB.CommandButton Command4 
      Caption         =   "Get Module No."
      Height          =   375
      Left            =   120
      TabIndex        =   3
      Top             =   1080
      Width           =   1575
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Uninitialize Devices"
      Height          =   375
      Left            =   6360
      TabIndex        =   2
      Top             =   120
      Width           =   1695
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Open Device"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   600
      Width           =   1575
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Initialize Devices"
      Height          =   375
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   1575
   End
   Begin VB.Label StatusLabel 
      Height          =   375
      Left            =   240
      TabIndex        =   23
      Top             =   8040
      Width           =   7815
   End
   Begin VB.Label SerialNoLabel 
      Caption         =   "Serial No.:"
      Height          =   375
      Left            =   1920
      TabIndex        =   6
      Top             =   1560
      Width           =   2175
   End
   Begin VB.Label ModuleNoLabel 
      Caption         =   "Module No.:"
      Height          =   255
      Left            =   1920
      TabIndex        =   4
      Top             =   1200
      Width           =   1935
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim DeviceHandle As Long

Private Sub Command1_Click()
    Dim TotalDevices As Long
    Dim MyStr As String

    TotalDevices = MTUSB_InitDevice
    MyStr = ("There are " & TotalDevices & " devices.")
    MsgBox (MyStr)

End Sub

Private Sub Command10_Click()
    MTUSB_ShowVideoWindow DeviceHandle, 200, 300, 320, 240
End Sub

Private Sub Command11_Click()
    MTUSB_StartFrameGrab DeviceHandle, 10000
End Sub

Private Sub Command12_Click()
    MTUSB_StopFrameGrab DeviceHandle
End Sub

Private Sub Command13_Click()
    Dim MyImageCtl As IMAGECONTROL
    MTUSB_GetFrameSetting DeviceHandle, MyImageCtl
    MyImageCtl.BinMode = 1
    MyImageCtl.Resolution = 7
    MTUSB_SetResolution DeviceHandle, MyImageCtl
End Sub

Private Sub Command14_Click()
    Dim MyImageCtl As IMAGECONTROL
    MTUSB_GetFrameSetting DeviceHandle, MyImageCtl
    MyImageCtl.RedGain = 16
    MyImageCtl.GreenGain = 16
    MyImageCtl.BlueGain = 8
    MTUSB_SetGain DeviceHandle, MyImageCtl
End Sub

Private Sub Command15_Click()
    Dim MyImageCtl As IMAGECONTROL
    MTUSB_GetFrameSetting DeviceHandle, MyImageCtl
    MyImageCtl.MaxExposureTimeIndex = 2
    MyImageCtl.ExposureTime = 20000 '20ms
    MTUSB_SetExposureTime DeviceHandle, MyImageCtl
End Sub

Private Sub Command16_Click()
    Dim MyImageCtl As IMAGECONTROL
    MTUSB_GetFrameSetting DeviceHandle, MyImageCtl
    MyImageCtl.IsCatchJPEG = 0
    MyImageCtl.IsCatchRAW = 0
    MyImageCtl.CatchFrames = 1
    MTUSB_SaveFramesToFiles DeviceHandle, MyImageCtl, "C:", "VB_Image"
End Sub

Private Sub Command17_Click()
    MTUSB_SetCameraWorkMode DeviceHandle, NORMAL_MODE
End Sub

Private Sub Command18_Click()
    MTUSB_SetCameraWorkMode DeviceHandle, TRIGGER_MODE
End Sub

Private Sub Command19_Click()
    MTUSB_SetGPIOConifg DeviceHandle, &H3  'Pin1&2 as Output, 3&4 as Input
End Sub

Private Sub Command2_Click()
    DeviceHandle = MTUSB_OpenDevice(0)
End Sub

Private Sub Command20_Click()
    Dim InputState As Byte
    MTUSB_SetGPIOInOut DeviceHandle, &H1, InputState   ' High for Pin1, Low for Pin2
End Sub

Private Sub Command21_Click()
    MTUSB_InstallFrameHooker DeviceHandle, 1, RAWDATA_IMAGE, AddressOf MyFrameHooker
End Sub

Private Sub Command22_Click()
    MTUSB_InstallFrameHooker DeviceHandle, 1, RAWDATA_IMAGE, 0
End Sub

Private Sub Command23_Click()
    MTUSB_SetWindowsMovable 0, 0
End Sub

Private Sub Command3_Click()
    MTUSB_UnInitDevice
End Sub

Private Sub Command4_Click()
    Dim ModuleName As String
    ModuleName = Space(32)
    MTUSB_GetModuleNo DeviceHandle, ModuleName
    ModuleNoLabel.Caption = "ModuleNo.: " & ModuleName
    
End Sub

Private Sub Command5_Click()
    Dim SerialName As String
    SerialName = Space(32)
    MTUSB_GetSerialNo DeviceHandle, SerialName
    SerialNoLabel.Caption = "SerialNo.: " & SerialName
End Sub

Private Sub Command6_Click()
    MTUSB_StartCameraEngine hWnd, DeviceHandle
End Sub

Private Sub Command7_Click()
    MTUSB_StopCameraEngine DeviceHandle
End Sub

Private Sub Command8_Click()
    MTUSB_ShowFrameControlPanel DeviceHandle, 1, 0, "VB_Example", 100, 100
End Sub

Private Sub Command9_Click()
    MTUSB_HideFrameControlPanel DeviceHandle
End Sub

Private Sub Form_Load()
    FrameCount = 0
End Sub
