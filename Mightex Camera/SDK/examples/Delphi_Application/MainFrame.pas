unit MainFrame;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Dllinterface, ComCtrls;

const
  TOTAL_FRAME_BYTES : Array [0..9 ] of integer =
    ( (32*32), (64*64), (160*120), (320*240), (640*480), (800*600), (1024*768),
      (1280*1024), (1600*1200), (2048*1536)  );
  FRAME_ROWS : Array [0..9 ] of integer =  ( 32, 64, 120, 240, 480, 600, 768,
      1024, 1200, 1536 );
  FRAME_COLS : Array [0..9 ] of integer =  ( 32, 64, 160, 320, 640, 800, 1024,
      1280, 1600, 2048 );
type
  TForm1 = class(TForm)
    USBInitButton: TButton;
    USBUnInitButton: TButton;
    OpenDeviceButton: TButton;
    ModuleNoButton: TButton;
    ModuleNoLabel: TLabel;
    SerialNoButton: TButton;
    SerialNoLabel: TLabel;
    StartEngineButton: TButton;
    StopEngineButton: TButton;
    ShowPanelButton: TButton;
    HidePanelButton: TButton;
    ShowVideoButton: TButton;
    StartGrabButton: TButton;
    StopGrabButton: TButton;
    GetSettingButton: TButton;
    SetResolutionButton: TButton;
    SetStartButton: TButton;
    SetGainButton: TButton;
    SetExposureTimeButton: TButton;
    GetLastBMPButton: TButton;
    SaveFrameFilesButton: TButton;
    SetGammaButton: TButton;
    SetShowModeButton: TButton;
    GetBufferButton: TButton;
    SetVideoButton: TButton;
    SetExtTriggerButton: TButton;
    WaitingTriggerButton: TButton;
    ResultStatusBar: TStatusBar;
    IOConfigButton: TButton;
    IOOutputButton: TButton;
    InstallFrameHookerButton: TButton;
    Label2: TLabel;
    Label1: TLabel;
    SaveGrabButton: TButton;
    procedure USBInitButtonClick(Sender: TObject);
    procedure USBUnInitButtonClick(Sender: TObject);
    procedure OpenDeviceButtonClick(Sender: TObject);
    procedure ModuleNoButtonClick(Sender: TObject);
    procedure SerialNoButtonClick(Sender: TObject);
    procedure StartEngineButtonClick(Sender: TObject);
    procedure StopEngineButtonClick(Sender: TObject);
    procedure ShowPanelButtonClick(Sender: TObject);
    procedure HidePanelButtonClick(Sender: TObject);
    procedure ShowVideoButtonClick(Sender: TObject);
    procedure StartGrabButtonClick(Sender: TObject);
    procedure StopGrabButtonClick(Sender: TObject);
    procedure GetSettingButtonClick(Sender: TObject);
    procedure SetResolutionButtonClick(Sender: TObject);
    procedure SetStartButtonClick(Sender: TObject);
    procedure SetGainButtonClick(Sender: TObject);
    procedure SetExposureTimeButtonClick(Sender: TObject);
    procedure GetLastBMPButtonClick(Sender: TObject);
    procedure SaveFrameFilesButtonClick(Sender: TObject);
    procedure SetGammaButtonClick(Sender: TObject);
    procedure SetShowModeButtonClick(Sender: TObject);
    procedure GetBufferButtonClick(Sender: TObject);
    procedure SetExtTriggerButtonClick(Sender: TObject);
    procedure SetVideoButtonClick(Sender: TObject);
    procedure WaitingTriggerButtonClick(Sender: TObject);
    procedure IOConfigButtonClick(Sender: TObject);
    procedure IOOutputButtonClick(Sender: TObject);
    procedure InstallFrameHookerButtonClick(Sender: TObject);
    procedure SaveGrabButtonClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  DevHandle : THandle;
  TotalDevices : integer;
  ImageCtl : TImageControl;
  RawBuffer : Array of Byte;
  BMPBuffer : Array of Byte;
  IsColorCamera : Boolean;
  IsImageGrabbed: Boolean;
  HookGrabBmp : TBitmap;

implementation

{$R *.DFM}

procedure DemoCallBack( SeqNo: integer; FileName : PChar ); cdecl;
begin
  // Callback should NOT contain any Blocking functions!!!
  Form1.ResultStatusBar.Panels[0].Text := 'Get ' + IntToStr(SeqNo) + 'th file, Name:' + #10 + #13
                + FileName;
end;

procedure FrameCallBack( FrameType : integer; Row : integer ; Col : integer;
                         Attribute : PImageAttachData; Buffer : PByte ); cdecl;
var
  FrameBytes : integer;
  i,j : integer;
  p : PByteArray;

begin
  // it will be invoked every time we get a frame.  don't block this function
  // data should be copied to other buffer and then quickly return.
  // As it's invoked in a working thread, it's NOT recommended to do any UI
  // related operations. User might notify UI thread to do the following by
  // using Windows Thread Sync mechanisms.
  if Attribute.TriggerOccurred = 1 then
    Form1.ResultStatusBar.Panels[0].Text := ' Trigger Occurred!'
  else
    Form1.ResultStatusBar.Panels[0].Text := ' No Trigger!';

  if  IsImageGrabbed = False then
    begin
      if FrameType = 0 then // Raw Data
        begin
          HookGrabBmp := TBitmap.Create;
          HookGrabBmp.Height := Row;
          HookGrabBmp.Width := Col;
          HookGrabBmp.PixelFormat := pf24Bit;
          for i:=0 to HookGrabBmp.Height-1 do
            begin
              p := HookGrabBmp.scanline[i];
              for j:=0 to  HookGrabBmp.Width-1 do
                begin
                  //p[3*j]  := Buffer[i*HookGrabBmp.Width + j];
                  p[3*j]  := PByte(Dword(Buffer) + i*HookGrabBmp.Width + j )^;
                  p[3*j+1]:= p[3*j];
                  p[3*j+2]:= p[3*j];
                end;
            end;
        end
      else // Color
        begin
          HookGrabBmp := TBitmap.Create;
          HookGrabBmp.Height := Row;
          HookGrabBmp.Width := Col;
          HookGrabBmp.PixelFormat := pf24Bit;
          for i:=0 to HookGrabBmp.Height-1 do
            begin
              p := HookGrabBmp.scanline[i];
              for j:=0 to HookGrabBmp.Width-1 do
                begin
                  if IsColorCamera then
                    begin
                      //p[3*j]  := Buffer[i*HookGrabBmp.Width*3 + 3*j ];
                      //p[3*j+1]:= Buffer[i*HookGrabBmp.Width*3 + 3*j + 1 ];
                      //p[3*j+2]:= Buffer[i*HookGrabBmp.Width*3 + 3*j + 2 ];
                      p[3*j]  := PByte(Dword(Buffer) + i*HookGrabBmp.Width*3 + j*3 )^;
                      p[3*j+1]:= PByte(Dword(Buffer) + i*HookGrabBmp.Width*3 + j*3 + 1 )^;
                      p[3*j+2]:= PByte(Dword(Buffer) + i*HookGrabBmp.Width*3 + j*3 + 2 )^;
                    end
                  else // BW Camera
                    begin
                      //p[3*j]  := Buffer[i*HookGrabBmp.Width + j];
                      p[3*j]  := PByte(Dword(Buffer) + i*HookGrabBmp.Width + j )^;
                      p[3*j+1]:= p[3*j];
                      p[3*j+2]:= p[3*j];
                    end;
                end;
            end;
        end;
      (*
      FrameBytes := Row * Col * 3;
      try
        try
          SetLength( BMPBuffer, FrameBytes );
          for i:=0 to FrameBytes-1 do
            begin
              BMPBuffer[i] := Buffer^;
              Inc( Buffer );
            end;
        except
          on Exception do;
        end;
      finally
        SetLength( BMPBuffer, 0 );
      end;
      *)
      IsImageGrabbed := True;
    end;
end;

procedure TForm1.USBInitButtonClick(Sender: TObject);
begin
  TotalDevices := MTUSBInitDevice();
  ShowMessage( 'There are ' + IntToStr(TotalDevices) + ' Devices!' );
end;

procedure TForm1.USBUnInitButtonClick(Sender: TObject);
begin
  MTUSBUnInitDevice();
end;

procedure TForm1.OpenDeviceButtonClick(Sender: TObject);
var
  i: integer;
begin
  //label1.Caption := DateTimeToStr(Now);
  //for i:=0 to 3999 do
    DevHandle := MTUSBOpenDevice(0);
  //label2.Caption := DateTimeToStr(Now);
  if DevHandle = $FFFFFFFF then
    ShowMessage( 'Device Open Fail!' );
end;

procedure TForm1.ModuleNoButtonClick(Sender: TObject);
var
  ModuleNo : Array[0..31] of Char;
  Position : integer;
begin
  IsColorCamera := True;
  if MTUSBGetModuleNo( DevHandle, ModuleNo ) = 1 then
    begin
      ModuleNoLabel.Caption := String(ModuleNo);
      Position := Pos( 'B0', ModuleNo );
      if Position <> 0 then
        begin
          IsColorCamera := False;
        end
    end;
end;

procedure TForm1.SerialNoButtonClick(Sender: TObject);
var
  SerialNo : Array[0..31] of Char;
begin
  if MTUSBGetSerialNo( DevHandle, SerialNo ) = 1 then
    SerialNoLabel.Caption := String(SerialNo);
end;

procedure TForm1.StartEngineButtonClick(Sender: TObject);
begin
  MTUSBStartCameraEngine( Handle, DevHandle );
end;

procedure TForm1.StopEngineButtonClick(Sender: TObject);
begin
  MTUSBStopCameraEngine( DevHandle );
end;

procedure TForm1.ShowPanelButtonClick(Sender: TObject);
begin
  MTUSBShowFrameControlPanel( DevHandle, True, False, 'Delphi Example', 8, 8 );
end;

procedure TForm1.HidePanelButtonClick(Sender: TObject);
begin
  MTUSBHideFrameControlPanel( DevHandle );
end;

procedure TForm1.ShowVideoButtonClick(Sender: TObject);
begin
  MTUSBShowVideoWindow( DevHandle, 100, 100 , 300, 400 );
end;

procedure TForm1.StartGrabButtonClick(Sender: TObject);
begin
  MTUSBStartFrameGrab( DevHandle, $8888 );
end;

procedure TForm1.StopGrabButtonClick(Sender: TObject);
begin
  MTUSBStopFrameGrab( DevHandle );
end;

procedure TForm1.GetSettingButtonClick(Sender: TObject);
begin
  MTUSBGetFrameSetting( DevHandle, @ImageCtl );
end;

procedure TForm1.SetResolutionButtonClick(Sender: TObject);
begin
  ImageCtl.Resolution := 6; // 1024x768
  ImageCtl.BinMode := 2;
  ImageCtl.ImageRendorFitWindow := True;
  MTUSBSetResolution( DevHandle, @ImageCtl );
end;

procedure TForm1.SetStartButtonClick(Sender: TObject);
begin
  ImageCtl.XStart := 64;
  ImageCtl.YStart := 64;
  MTUSBSetStartPosition( DevHandle, @ImageCtl );
end;

procedure TForm1.SetGainButtonClick(Sender: TObject);
begin
  ImageCtl.GreenGain := 32; // 4.0
  ImageCtl.BlueGain := 12; // 1.5
  ImageCtl.RedGain := 16; // 2
  MTUSBSetGain( DevHandle, @ImageCtl );
end;

procedure TForm1.SetExposureTimeButtonClick(Sender: TObject);
begin
  ImageCtl.MaxExposureTimeIndex := 2;  // 100ms
  ImageCtl.ExposureTime := 15000;      // 15000us
  MTUSBSetExposureTime( DevHandle, @ImageCtl );
end;

procedure TForm1.SetGammaButtonClick(Sender: TObject);
begin
  ImageCtl.Gamma := 12; // 1.2
  ImageCtl.Contrast := 58; // 58%
  ImageCtl.Bright := 66; // 66%
  ImageCtl.SharpLevel := 1; // Sharp
  MTUSBSetGammaValue( DevHandle, @ImageCtl );
end;

procedure TForm1.SetShowModeButtonClick(Sender: TObject);
begin
  ImageCtl.BWMode := True;
  ImageCtl.HorizontalMirror := False;
  ImageCtl.VerticalFlip := True;
  MTUSBSetShowMode( DevHandle, @ImageCtl );
end;

procedure TForm1.GetLastBMPButtonClick(Sender: TObject);
var
  i: integer;
begin
  //label1.Caption := DateTimeToStr(Now);
  //for i:=1 to 100 do
  //  MTUSBGetLastBMPFrame( DevHandle, PChar('c:\Example' + IntToStr(i)+ '.bmp') );
  MTUSBGetLastBMPFrame( DevHandle, 'c:\Example.bmp' );
  //label2.Caption := DateTimeToStr(Now);
end;

procedure TForm1.SaveFrameFilesButtonClick(Sender: TObject);
begin
  ImageCtl.IsCatchRAW := False;
  ImageCtl.IsCatchJPEG := False;
  //ImageCtl.IsCatchJPEG := True;
  //ImageCtl.IsRawRGBBmp := True;
  //ImageCtl.CatchIgnoreSkip := True;
  ImageCtl.CatchFrames := 1;
  MTUSBSaveFramesToFiles( DevHandle, @ImageCtl, 'c:', 'image' );
end;

procedure TForm1.GetBufferButtonClick(Sender: TObject);
var
  FrameBytes : integer;
  BytePtr : PByte;
  SampleBmp: TBitmap;
  i,j : integer;
  p : PByteArray;
begin
  MTUSBGetFrameSetting( DevHandle, @ImageCtl );
  FrameBytes := TOTAL_FRAME_BYTES[ ImageCtl.Resolution ] div
                ( ImageCtl.BinMode * ImageCtl.BinMode );
  SetLength( RawBuffer, FrameBytes );
  BytePtr := @RawBuffer[0];
  MTUSBGetCurrentFrame( DevHandle, 0, BytePtr );
  SampleBmp := TBitmap.Create;
  SampleBmp.Height := FRAME_ROWS[ImageCtl.Resolution ] div ImageCtl.BinMode;
  SampleBmp.Width := FRAME_COLS[ImageCtl.Resolution ] div ImageCtl.BinMode;
  SampleBmp.PixelFormat := pf24Bit;
  for i:=0 to SampleBmp.Height-1 do
    begin
      p := SampleBmp.scanline[i];
      for j:=0 to  SampleBmp.Width-1 do
        begin
          p[3*j]  := RawBuffer[i*SampleBmp.Width + j];
          p[3*j+1]:= p[3*j];
          p[3*j+2]:= p[3*j];
        end;
    end;
  SampleBmp.SaveToFile( 'CurrentRawData.bmp' );
  SampleBmp.Free;
  SetLength( RawBuffer, 0 );

  FrameBytes := FrameBytes * 3;
  SetLength( BMPBuffer, FrameBytes );
  BytePtr := @BMPBuffer[0];
  MTUSBGetCurrentFrame( DevHandle, 1, BytePtr );
  SampleBmp := TBitmap.Create;
  SampleBmp.Height := FRAME_ROWS[ImageCtl.Resolution ] div ImageCtl.BinMode;
  SampleBmp.Width := FRAME_COLS[ImageCtl.Resolution ] div ImageCtl.BinMode;
  SampleBmp.PixelFormat := pf24Bit;
  for i:=0 to SampleBmp.Height-1 do
    begin
      p := SampleBmp.scanline[i];
      for j:=0 to SampleBmp.Width-1 do
        begin
          if IsColorCamera then
            begin
              p[3*j]  := BMPBuffer[i*SampleBmp.Width*3 + 3*j ];
              p[3*j+1]:= BMPBuffer[i*SampleBmp.Width*3 + 3*j + 1 ];
              p[3*j+2]:= BMPBuffer[i*SampleBmp.Width*3 + 3*j + 2 ];
            end
          else // BW Camera
            begin
              p[3*j]  := BMPBuffer[i*SampleBmp.Width + j];
              p[3*j+1]:= p[3*j];
              p[3*j+2]:= p[3*j];
            end;
        end;
    end;
  SampleBmp.SaveToFile( 'CurrentBmpData.bmp' );
  SampleBmp.Free;
  SetLength( BMPBuffer, 0 );
end;

procedure TForm1.InstallFrameHookerButtonClick(Sender: TObject);
begin
  IsImageGrabbed := False;
  //MTUSBInstallFrameHooker( DevHandle, False, 0, FrameCallBack ); // Raw Data
  MTUSBInstallFrameHooker( DevHandle, False, 1, FrameCallBack ); // RGB Data
end;

procedure TForm1.SetExtTriggerButtonClick(Sender: TObject);
begin
  MTUSBSetCameraWorkMode( DevHandle, 1 );
end;

procedure TForm1.SetVideoButtonClick(Sender: TObject);
begin
  MTUSBSetCameraWorkMode( DevHandle, 0 );
end;

procedure TForm1.WaitingTriggerButtonClick(Sender: TObject);
var
  ret : integer;
begin
  ret := MTUSBSetExternalParameters( DevHandle, False, True, True,
                                     'c:', 'catchimg' );
  ret := MTUSBWaitingExternalTrigger( DevHandle, True, DemoCallBack );
  {*
    For Abort the the waiting.... user may call
       MTUSBWaitingExternalTrigger( DevHandle, False, nil );
  *}
end;

procedure TForm1.IOConfigButtonClick(Sender: TObject);
var
  ret : integer;
begin
  ret := MTUSBSetGPIOConifg( DevHandle, $3 ); // Pin1, 2 as Output, 3, 4 as Input
end;

procedure TForm1.IOOutputButtonClick(Sender: TObject);
var
  ret : integer;
  RcvByte : Byte;
begin
  ret := MTUSBSetGPIOInOut( DevHandle, $1, @RcvByte );   // High for Pin1, Low for Pin2
  ResultStatusBar.Panels[1].Text := IntToStr( RcvByte );
end;

procedure TForm1.SaveGrabButtonClick(Sender: TObject);
begin
  if IsImageGrabbed = True then
    begin
      HookGrabBmp.SaveToFile( 'HookGrab.Bmp' );
    end;
end;

end.
