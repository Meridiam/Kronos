object Form1: TForm1
  Left = 257
  Top = 116
  Width = 480
  Height = 529
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object ModuleNoLabel: TLabel
    Left = 136
    Top = 80
    Width = 75
    Height = 13
    Caption = 'ModuleNoLabel'
  end
  object SerialNoLabel: TLabel
    Left = 136
    Top = 112
    Width = 66
    Height = 13
    Caption = 'SerialNoLabel'
  end
  object Label2: TLabel
    Left = 280
    Top = 232
    Width = 32
    Height = 13
    Caption = 'Label2'
  end
  object Label1: TLabel
    Left = 280
    Top = 216
    Width = 32
    Height = 13
    Caption = 'Label1'
  end
  object USBInitButton: TButton
    Left = 8
    Top = 8
    Width = 113
    Height = 25
    Caption = 'Initialize Devices'
    TabOrder = 0
    OnClick = USBInitButtonClick
  end
  object USBUnInitButton: TButton
    Left = 352
    Top = 8
    Width = 113
    Height = 25
    Caption = 'UnInitialize Devices'
    TabOrder = 1
    OnClick = USBUnInitButtonClick
  end
  object OpenDeviceButton: TButton
    Left = 8
    Top = 40
    Width = 113
    Height = 25
    Caption = 'Open Device'
    TabOrder = 2
    OnClick = OpenDeviceButtonClick
  end
  object ModuleNoButton: TButton
    Left = 8
    Top = 72
    Width = 113
    Height = 25
    Caption = 'Get Module No.'
    TabOrder = 3
    OnClick = ModuleNoButtonClick
  end
  object SerialNoButton: TButton
    Left = 8
    Top = 104
    Width = 113
    Height = 25
    Caption = 'Get Serial No.'
    TabOrder = 4
    OnClick = SerialNoButtonClick
  end
  object StartEngineButton: TButton
    Left = 8
    Top = 136
    Width = 113
    Height = 25
    Caption = 'Start Camera Engine'
    TabOrder = 5
    OnClick = StartEngineButtonClick
  end
  object StopEngineButton: TButton
    Left = 240
    Top = 8
    Width = 107
    Height = 25
    Caption = 'Stop Camera Engine'
    TabOrder = 6
    OnClick = StopEngineButtonClick
  end
  object ShowPanelButton: TButton
    Left = 8
    Top = 168
    Width = 113
    Height = 25
    Caption = 'Show Panel'
    TabOrder = 7
    OnClick = ShowPanelButtonClick
  end
  object HidePanelButton: TButton
    Left = 128
    Top = 168
    Width = 113
    Height = 25
    Caption = 'Hide Panel'
    TabOrder = 8
    OnClick = HidePanelButtonClick
  end
  object ShowVideoButton: TButton
    Left = 248
    Top = 168
    Width = 105
    Height = 25
    Caption = 'Show Video'
    TabOrder = 9
    OnClick = ShowVideoButtonClick
  end
  object StartGrabButton: TButton
    Left = 8
    Top = 200
    Width = 113
    Height = 25
    Caption = 'Start Frame Grab'
    TabOrder = 10
    OnClick = StartGrabButtonClick
  end
  object StopGrabButton: TButton
    Left = 128
    Top = 200
    Width = 113
    Height = 25
    Caption = 'Stop Frame Grab'
    TabOrder = 11
    OnClick = StopGrabButtonClick
  end
  object GetSettingButton: TButton
    Left = 8
    Top = 256
    Width = 113
    Height = 25
    Caption = 'Get Settings'
    TabOrder = 12
    OnClick = GetSettingButtonClick
  end
  object SetResolutionButton: TButton
    Left = 8
    Top = 288
    Width = 113
    Height = 25
    Caption = 'Set Resolution'
    TabOrder = 13
    OnClick = SetResolutionButtonClick
  end
  object SetStartButton: TButton
    Left = 128
    Top = 288
    Width = 113
    Height = 25
    Caption = 'Set Start Postion'
    TabOrder = 14
    OnClick = SetStartButtonClick
  end
  object SetGainButton: TButton
    Left = 248
    Top = 288
    Width = 105
    Height = 25
    Caption = 'Set Gains'
    TabOrder = 15
    OnClick = SetGainButtonClick
  end
  object SetExposureTimeButton: TButton
    Left = 360
    Top = 288
    Width = 105
    Height = 25
    Caption = 'Set Exposure'
    TabOrder = 16
    OnClick = SetExposureTimeButtonClick
  end
  object GetLastBMPButton: TButton
    Left = 8
    Top = 352
    Width = 113
    Height = 25
    Caption = 'Get Last BMP'
    TabOrder = 17
    OnClick = GetLastBMPButtonClick
  end
  object SaveFrameFilesButton: TButton
    Left = 128
    Top = 352
    Width = 113
    Height = 25
    Caption = 'Save Frame Files'
    TabOrder = 18
    OnClick = SaveFrameFilesButtonClick
  end
  object SetGammaButton: TButton
    Left = 8
    Top = 320
    Width = 113
    Height = 25
    Caption = 'Set Gamma Value'
    TabOrder = 19
    OnClick = SetGammaButtonClick
  end
  object SetShowModeButton: TButton
    Left = 128
    Top = 320
    Width = 113
    Height = 25
    Caption = 'Set Show Mode'
    TabOrder = 20
    OnClick = SetShowModeButtonClick
  end
  object GetBufferButton: TButton
    Left = 248
    Top = 352
    Width = 105
    Height = 25
    Caption = 'Get Frame Buffer'
    TabOrder = 21
    OnClick = GetBufferButtonClick
  end
  object SetVideoButton: TButton
    Left = 360
    Top = 440
    Width = 105
    Height = 25
    Caption = 'Set Video Mode'
    TabOrder = 22
    OnClick = SetVideoButtonClick
  end
  object SetExtTriggerButton: TButton
    Left = 8
    Top = 400
    Width = 113
    Height = 25
    Caption = 'Set Trigger Mode'
    TabOrder = 23
    OnClick = SetExtTriggerButtonClick
  end
  object WaitingTriggerButton: TButton
    Left = 128
    Top = 400
    Width = 113
    Height = 25
    Caption = 'Waiting Trigger...'
    TabOrder = 24
    OnClick = WaitingTriggerButtonClick
  end
  object ResultStatusBar: TStatusBar
    Left = 0
    Top = 468
    Width = 472
    Height = 27
    Panels = <
      item
        Width = 300
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object IOConfigButton: TButton
    Left = 8
    Top = 440
    Width = 113
    Height = 25
    Caption = 'Set IO Config'
    TabOrder = 26
    OnClick = IOConfigButtonClick
  end
  object IOOutputButton: TButton
    Left = 128
    Top = 440
    Width = 113
    Height = 25
    Caption = 'Set Output'
    TabOrder = 27
    OnClick = IOOutputButtonClick
  end
  object InstallFrameHookerButton: TButton
    Left = 360
    Top = 352
    Width = 105
    Height = 25
    Caption = 'Install Frame Hooker'
    TabOrder = 28
    OnClick = InstallFrameHookerButtonClick
  end
  object SaveGrabButton: TButton
    Left = 360
    Top = 384
    Width = 105
    Height = 25
    Caption = 'Save Grab File'
    TabOrder = 29
    OnClick = SaveGrabButtonClick
  end
end
