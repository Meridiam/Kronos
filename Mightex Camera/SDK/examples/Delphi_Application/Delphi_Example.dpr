program Delphi_Example;

uses
  Forms,
  MainFrame in 'MainFrame.pas' {Form1},
  DllInterface in 'DllInterface.pas';

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
