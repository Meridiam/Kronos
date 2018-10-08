Imports System
Imports System.Windows.Forms

Public Class frmMain
    Public WinHwnd As IntPtr

    Protected Overrides Sub WndProc(ByRef m As Message)
        If m.Msg = &H401 Then
            FrameInfoLabel.Text = "Brightest pixel: " & BrightestPixel & vbCr & "Frames: " & FrameCount
        Else
            MyBase.WndProc(m)
        End If
    End Sub
    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        WinHwnd = Me.Handle
        CameraSTART()
    End Sub

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        CameraSTOP()
    End Sub
End Class
