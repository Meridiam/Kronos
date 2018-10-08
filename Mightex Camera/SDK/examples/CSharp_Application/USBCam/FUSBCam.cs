using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace USBCam
{
    public partial class FUSBCam : Form
    {
        //private ArrayList _myLights;
        enum IMAGEEXT { JPEG = 1, BMP, PNG, TIFF };  //corresponds to file filters in Save dialog box
        private string _camConfError = "Camera Configuration Error";
        private CCameraUSB myUSBCam; // = new CCameraUSB( this );
        private int _numCams;
        private bool _camEngineStarted = false;
        private bool _startedContFG = false;
        private int _oldRedGain;
        private int _oldGreenGain;
        private int _oldBlueGain;
        private int _callbackCount;

        public ImageProperty CurrentImageProperty;
        public uint PixelAverage;


        public FUSBCam()
        {
            InitializeComponent();

            myUSBCam = new CCameraUSB( this );
            _callbackCount = 0;
            
            _numCams = myUSBCam.InitDevice();
            //Populate camera combo box
            if (_numCams > 0)
            {
                string cam = "";
                for (int i = 0; i < _numCams; i++)
                {
                    cam = String.Format("Camera {0}", i + 1);
                    CBCameraSelect.Items.Add(cam);
                }
                //CBCameraSelect.SelectedIndex = 0;
                myUSBCam.AllocImageMem(CCameraUSB.RESOLUTION.M_3M); // Maximum memory space
                //CBResolution.SelectedIndex = (int)CCameraUSB.RESOLUTION.VGA;
                NUpDownExposureTime.Value = myUSBCam.GetExpTime()/1000;

                //ROI
                NUpDownX.Value = myUSBCam.GetXstart();
                NUpDownY.Value = myUSBCam.GetYstart();

                //Gains
                NUpDownRed.Value = myUSBCam.GetRedGain();
                NUpDownGreen.Value = myUSBCam.GetGreenGain();
                NUpDownBlue.Value = myUSBCam.GetBlueGain();

                //Set light controls
                //_myLights = lights;
                //double intensity1 = ((CLight)_myLights[0]).IntensityLevel;
                //((CLight)_myLights[0]).SetIntensity(intensity1);
                //SBLightIntensity1.Value = System.Convert.ToInt32(intensity1);
                //TBLightIntensity1.Text = intensity1.ToString("F1");
                //myUSBCam.AllocImageMem((CCameraUSB.RESOLUTION)CBResolution.SelectedIndex);
                
            }
        }

        private void BCancel_Click(object sender, EventArgs e)
        {
            //Cleanup Resources called by FUSBCam_FormClosed
            Close();
        }

        private void BOK_Click(object sender, EventArgs e)
        {
            //Cleanup Resources called by FUSBCam_FormClosed
            Close();
        }

        private void CleanUpResources()
        {
            myUSBCam.FreeImageMem();
            if (this._camEngineStarted)
            {
                //Free IntPtr memory in myUSBCam object
                //myUSBCam.FreeImageMem();
                myUSBCam.StopCameraEngine();
            }

            //release camera resources
            this.myUSBCam.UnInitDevice();
        }

        private void CBCameraSelect_SelectedIndexChanged(object sender, EventArgs e)
        {
            //Open device and get module and serial numbers
            myUSBCam.OpenDevice(CBCameraSelect.SelectedIndex);
            TBModuleNo.Text = myUSBCam.GetModuleNo();
            TBSerialNo.Text = myUSBCam.GetSerialNo();
            
            //Start the camera engine
            myUSBCam.StartCameraEngine(this.Handle);
            _camEngineStarted = true;
            System.Threading.Thread.Sleep(200);

            //Set default camera settings
            Trace.Flush();
            Trace.WriteLine("Calling SetFrameSetting() from FUSBCam Form....");
            Trace.Indent();
            /*
             * JTZ: Show the control panel for debug purpose only.
             */
            myUSBCam.ShowFrameControlPanel(100, 100);
            myUSBCam.SetFrameSetting();  //image is black?????
            myUSBCam.GetCamFrameSettings(); 

            Trace.Unindent();
            Trace.WriteLine("Returned from SetFrameSetting(), end trace.");

            //Black and White and Orientation
            bool blkWht = myUSBCam.GetBlkWht();
            bool horzMirr = myUSBCam.GetHorzMirr();
            bool flipVert = myUSBCam.GetFlipVert();
            ////Set Orientation & B&W
            //myUSBCam.SetShowMode(blkWht, horzMirr, flipVert);

            //if (blkWht)
            //    CBBlackWhite.CheckState = CheckState.Checked;
            //if (horzMirr)
            //    CBHorzMirr.CheckState = CheckState.Checked;
            //if (flipVert)
            //    CBFlipVert.CheckState = CheckState.Checked;
            //myUSBCam.SetExposureTime(2, 15);

            CBResolution.SelectedIndex = (int)CCameraUSB.RESOLUTION.VGA;
        }

        private void CBShowVideoWindow_CheckedChanged(object sender, EventArgs e)
        {
            //Used for development
            //if (CBShowVideoWindow.Checked)
            //{
            //    myUSBCam.ShowVideoWindow(100, 400, 680, 520);
            //}
        }

        private void BContFG_Click(object sender, EventArgs e)
        {
            if (!_startedContFG)
            {
                myUSBCam.StartFrameGrab(CCameraUSB.INFINITE_FRAMES);
                _startedContFG = true;
                BContFG.Text = "Stop Cont FG";
                //myUSBCam.SetResolution((CCameraUSB.RESOLUTION)CBResolution.SelectedIndex, 1, false);
                //timerContFG.Enabled = true;
                //System.Threading.Thread.Sleep(200);
            }
            else
            {
                myUSBCam.StopFrameGrab();
                _startedContFG = false;
                BContFG.Text = "Start Cont FG";
            }
        }

        private void CBResolution_SelectedIndexChanged(object sender, EventArgs e)
        {
            //if (_startedContFG)
            {
                myUSBCam.SetResolution((CCameraUSB.RESOLUTION)CBResolution.SelectedIndex, 1, false);
                //myUSBCam.FreeImageMem();
                //myUSBCam.AllocImageMem((CCameraUSB.RESOLUTION)CBResolution.SelectedIndex);
            }
        }

        private void CBBlackWhite_CheckedChanged(object sender, EventArgs e)
        {
            //bool blkWht = myUSBCam.GetBlkWht();
            bool horzMirr = myUSBCam.GetHorzMirr();
            bool flipVert = myUSBCam.GetFlipVert();

            if (CBBlackWhite.Checked)
            {
                myUSBCam.SetShowMode(true, horzMirr, flipVert);
            }
            else
            {
                myUSBCam.SetShowMode(false, horzMirr, flipVert);
            }
        }

        private void CBShowAutoExpRegion_CheckedChanged(object sender, EventArgs e)
        {
            if (CBShowAutoExpRegion.Checked)
            {
                myUSBCam.SetAutoExposure(false, true);
            }
            else
            {
                myUSBCam.SetAutoExposure(false, false);
            }
        }

        private void NUpDownExposureTime_ValueChanged(object sender, EventArgs e)
        {
            int newExpTime = System.Convert.ToInt32(NUpDownExposureTime.Value);
            if (_camEngineStarted)
            {
                myUSBCam.SetExposureTime(2, newExpTime);
            }
        }

        private void NUpDownRed_ValueChanged(object sender, EventArgs e)
        {
            int newRedGain = System.Convert.ToInt32(NUpDownRed.Value);
            if (_camEngineStarted)
            {
                myUSBCam.SetGain(newRedGain, myUSBCam.GetGreenGain(), myUSBCam.GetBlueGain());
            }
        }

        private void NUpDownGreen_ValueChanged(object sender, EventArgs e)
        {
            int newGreenGain = System.Convert.ToInt32(NUpDownGreen.Value);
            if (_camEngineStarted)
            {
                myUSBCam.SetGain(myUSBCam.GetRedGain(), newGreenGain, myUSBCam.GetBlueGain());
            }
        }

        private void NUpDownBlue_ValueChanged(object sender, EventArgs e)
        {
            int newBlueGain = System.Convert.ToInt32(NUpDownBlue.Value);
            if(_camEngineStarted)
            {
                myUSBCam.SetGain(myUSBCam.GetRedGain(), myUSBCam.GetGreenGain(), newBlueGain);
            }
        }

        private void CBAutoWhiteBalance_CheckedChanged(object sender, EventArgs e)
        {
            if (CBAutoWhiteBalance.Checked)
            {
                //set old values for gains
                _oldRedGain = System.Convert.ToInt32(NUpDownRed.Value);
                _oldGreenGain = System.Convert.ToInt32(NUpDownGreen.Value);
                _oldBlueGain = System.Convert.ToInt32(NUpDownBlue.Value);

                //Perform auto white balance
                myUSBCam.SetWhiteBalance();

                //Retrieve new gain settings
                myUSBCam.GetCamFrameSettings();
                NUpDownRed.Value = myUSBCam.GetRedGain();
                NUpDownGreen.Value = myUSBCam.GetGreenGain();
                NUpDownBlue.Value = myUSBCam.GetBlueGain();
            }
            else
            {
                NUpDownRed.Value = _oldRedGain;
                NUpDownGreen.Value = _oldGreenGain;
                NUpDownBlue.Value = _oldBlueGain;
            }
        }

        private void NUpDownX_ValueChanged(object sender, EventArgs e)
        {
            int xPos = System.Convert.ToInt32(NUpDownX.Value);
            int yPos = System.Convert.ToInt32(NUpDownY.Value);

            //Set max x and y based on resolution
            NUpDownX.Maximum = myUSBCam.MaxX;
            NUpDownY.Maximum = myUSBCam.MaxY;

            if (_camEngineStarted)
            {
                myUSBCam.SetStartPosition(xPos, yPos);
            }
        }

        private void NUpDownY_ValueChanged(object sender, EventArgs e)
        {
            int xPos = System.Convert.ToInt32(NUpDownX.Value);
            int yPos = System.Convert.ToInt32(NUpDownY.Value);

            if (_camEngineStarted)
            {
                myUSBCam.SetStartPosition(xPos, yPos);
            }
        }

        private void BSnapshot_Click(object sender, EventArgs e)
        {
            //Used for development
            //if (PBImage.Image != null)
            //{
            //    PBImage.Image.Dispose();
               
            //}
            //myUSBCam.SetResolution((CCameraUSB.RESOLUTION)CBResolution.SelectedIndex, 1, false);
            //myUSBCam.StartFrameGrab(1);
            //myUSBCam.GetLastBMPFrame(@"d:\RSX Images\lastframe.bmp");

            //Bitmap lastBMP = new Bitmap(@"d:\RSX Images\lastframe.bmp");
            //PBImage.Image = lastBMP;
            //System.Drawing.Imaging.PixelFormat pf = PBImage.Image.PixelFormat;
        }

        private void BSaveImage_Click(object sender, EventArgs e)
        {
            //Save Form Dialog box...
            DialogResult saveDlgResult = SFDsaveImage.ShowDialog(this);
            if (saveDlgResult == DialogResult.Cancel)
                return;

            string fileName = SFDsaveImage.FileName;
            fileName = fileName.ToLower();
            IMAGEEXT fileExtEnum = (IMAGEEXT)SFDsaveImage.FilterIndex;

            //Extension already exist?
            if(fileName.Contains("."))
                fileName = fileName.Remove(fileName.Length-4);
            
            //Convert image to appropriate format
            try
            {
                string fileExt = "";
                Bitmap locImage = new Bitmap(PBImage.Image);
                switch (fileExtEnum)
                {
                    case IMAGEEXT.JPEG:
                        fileExt = ".jpg";
                        fileName += fileExt;
                        locImage.Save(fileName, System.Drawing.Imaging.ImageFormat.Jpeg);
                        break;
                    case IMAGEEXT.BMP:
                        fileExt = ".bmp";
                        fileName += fileExt;
                        locImage.Save(fileName, System.Drawing.Imaging.ImageFormat.Bmp);
                        break;
                    case IMAGEEXT.PNG:
                        fileExt = ".png";
                        fileName += fileExt;
                        locImage.Save(fileName, System.Drawing.Imaging.ImageFormat.Png);
                        break;
                    case IMAGEEXT.TIFF:
                        fileExt = ".tif";
                        fileName += fileExt;
                        locImage.Save(fileName, System.Drawing.Imaging.ImageFormat.Tiff);
                        break;
                    default:
                        locImage.Save(fileName);  //unknown extension index
                        break;
                }
            }
            catch (NullReferenceException)
            {
                MessageBox.Show("No image available to save.", _camConfError, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
        }

        private void BGetLastFrame_Click(object sender, EventArgs e)
        {
            //Used for development
            //myUSBCam.GetLastBMPFrame(@"d:\RSX Images\lastframe.bmp");
        }

        private void timerContFG_Tick(object sender, EventArgs e)
        {
            if (_startedContFG)
                BGetCurrentFrame_Click(sender, e);   
        }

        private void BGetCurrentFrame_Click(object sender, EventArgs e)
        {
            if (CBCameraSelect.Text == "")
            {
                MessageBox.Show("Please select camera and try again.", _camConfError, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            if (!_startedContFG)
            {
                BContFG_Click(sender, e);
                System.Threading.Thread.Sleep(300);
            }
            
            if (_startedContFG)
            { 
                //Allocate IntPtr memory on global heap
                //myUSBCam.AllocImageMem((CCameraUSB.RESOLUTION)CBResolution.SelectedIndex);

                IntPtr recentImg = myUSBCam.GetCurrentFrame(CCameraUSB.FRAME_TYPE.DIB);

                Bitmap myBMP = new Bitmap(myUSBCam.MaxX, myUSBCam.MaxY, myUSBCam.PixelDepth * myUSBCam.MaxX, 
                    System.Drawing.Imaging.PixelFormat.Format24bppRgb, recentImg);

                //Bitmap showBMP = new Bitmap(myBMP);
                //PBImage.Image = showBMP;
                PBImage.Image = myBMP;

                //myUSBCam.FreeImageMem();
            }
        }

        private void CBHorzMirr_CheckedChanged(object sender, EventArgs e)
        {
        //*****Open issue remains with bool values always being set to true even after cam initialized.
            //bool blkWht = myUSBCam.GetBlkWht();
            ////bool horzMirr = myUSBCam.GetHorzMirr();
            //bool flipVert = myUSBCam.GetFlipVert();

            //if (CBHorzMirr.Checked)
            //{
            //    myUSBCam.SetShowMode(blkWht, true, flipVert);
            //}
            //else
            //{
            //    myUSBCam.SetShowMode(blkWht, false, flipVert);
            //}
        }

        private void CBFlipVert_CheckedChanged(object sender, EventArgs e)
        {
        //*****Open issue remains with bool values always being set to true even after cam initialized.
            //bool blkWht = myUSBCam.GetBlkWht();
            //bool horzMirr = myUSBCam.GetHorzMirr();
            ////bool flipVert = myUSBCam.GetFlipVert();

            //if (CBFlipVert.Checked)
            //{
            //    myUSBCam.SetShowMode(blkWht, horzMirr, true);
            //}
            //else
            //{
            //    myUSBCam.SetShowMode(blkWht, horzMirr, false);
            //}
        }

        private void CBContSnap_CheckedChanged(object sender, EventArgs e)
        {
            if (CBContSnap.Checked)
                timerContFG.Enabled = true;
            else
                timerContFG.Enabled = false;
        }

        private void SBLightIntensity1_Scroll(object sender, EventArgs e)
        {
            //Get value and set light intensity
            //((CLight)_myLights[0]).SetIntensity(SBLightIntensity1.Value);
            //TBLightIntensity1.Text = SBLightIntensity1.Value.ToString("F1");
        }

        private void BLiveImage_Click(object sender, EventArgs e)
        {
            //yUSBCam.ShowVideoWindow(100, 400, 680, 520);
            timerContFG.Enabled = true;
        }

        private void BSaveSettings_Click(object sender, EventArgs e)
        {
            //Save current camera and LED light settings to xml file
        }

        private void FUSBCam_FormClosed(object sender, FormClosedEventArgs e)
        {
            CleanUpResources();
        }

        public void SetCallBackMessage( ref ImageProperty imageProp, uint pixelAvg )
        {
            CurrentImageProperty  = imageProp;
            PixelAverage = pixelAvg;
            _callbackCount++;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if ( _callbackCount > 0 )
            {
                CallBackMessageLabel.Text = "Frame Count: " + _callbackCount + " Exposure Time;  " + CurrentImageProperty.ExposureTime + " Pixel Average: " +  PixelAverage; // ShowMessage
            }
        }

     }
}