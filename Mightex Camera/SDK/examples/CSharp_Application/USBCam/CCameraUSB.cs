using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Xml;
using System.Xml.Serialization;

//*****Open issue remains with bool values always being set to true even after cam initialized.
// Timer doesn't seem to work to show images continually, will need to implement the Mightex Hooker callback
// if we want direct updates to the windows form.

namespace USBCam
{
    [StructLayout(LayoutKind.Explicit)]
    public struct ImageProperty
    {
        [FieldOffset(0)] public int XStart;
        [FieldOffset(4)] public int YStart;
        [FieldOffset(8)] public int GreenGain;
        [FieldOffset(12)] public int BlueGain;
        [FieldOffset(16)] public int RedGain;
        [FieldOffset(20)] public int ExposureTime;
        // Start from SDK V1.4.0.0, we have 3 more fields:
        [FieldOffset(24)] public int TriggerOccurred;
        [FieldOffset(28)] public int Reserved1;
        [FieldOffset(32)] public int Reserved2;
    }

    public delegate void FrameCallbackDelegate( uint FrameType, uint Row, uint Column, 
                                                ref ImageProperty frameProperty, uint BufferPtr );

    public class CCameraUSB
    {
        //Members...
        public enum IMAGES {RAW, BMP, JPG};
        public enum CAM_WORKMODE {VIDEO, EXT_TRIG};
        public enum FRAME_TYPE { RAW, DIB };
        public enum SHARPNESS_LEV { NORMAL, SHARP, SHARPER, SHARPEST };
        public enum RESOLUTION { TINY, LITTLE, SMALL, MOBILE, VGA, SVGA, UVGA, VGALARGE,M_2M,M_3M }
        public const int INFINITE_FRAMES = 0x8888;
        public FrameCallbackDelegate frameDelegate;
        private FUSBCam MightexCam;
        private uint _devHandle = 99;
        private string _camError = "USB Camera Error";
        private IntPtr _pImage = new IntPtr();  //image pointer

        private int _maxX;
        public int MaxX
        {
            get { return _maxX; }
        }
        private int _maxY;
        public int MaxY
        {
            get { return _maxY; }
        }

        private int pixelDepthBits = 24/8;  //24 bit color camera div by 8 bits for 3 bytes total
        public int PixelDepth
        {
            get { return pixelDepthBits; }
        }

        [StructLayout(LayoutKind.Explicit)]
        public struct ImageControl
        {
            //For Image Capture (this is the TImageControl struct in the Mightex lib)
            [XmlElement("Revision")]
            [FieldOffset(0)] public int _revision;       //Reserved for internal use only
            [FieldOffset(4)] public int _resolution;     //indices: 0=32x32, 1=64x64, 2=160x120, 3=320x240, 4=640x480, 5=800x600, 6=1024x768, 7=1280x1024
            [FieldOffset(8)] public int _binMode;        //1 ?No Skip mode, 2 ?2X skip(1:2 decimation)
            [FieldOffset(12)] public int _xStart;        //Upper left hand corner, positive right 
            [FieldOffset(16)] public int _yStart;        //Upper left hand corner, positive down
            [FieldOffset(20)] public int _greenGain;     //Green Gain Value: 0 ?128, the actual gain is GreenGain/8
            [FieldOffset(24)] public int _blueGain;      //Blue Gain Value: 0 ?128, the actual gain is BlueGain/8
            [FieldOffset(28)] public int _redGain;       //Red Gain Value: 0 ?128, the actual gain is RedGain/8
            [FieldOffset(32)] public int _maxExposureTimeIndex;  //Maximum exposure time index: 0=5ms, 1=10ms, 2=100ms, 3=750ms
            [FieldOffset(36)] public int _exposureTime;       //current exposure time in microseconds

            //For Image Render
            //bool is one byte
            // JTZ: for STDCALL version, the struct layout is different from the CDECL version.
            /*
            [FieldOffset(40)] public bool _imageRendorFitWindow; //True if the image always fit video window, otherwise False
            [FieldOffset(41)] public int _gamma;         //Gamma value: 0 ?20, means 0.0 ?2.0
            [FieldOffset(45)] public int _contrast;      //Contrast value: 0 ?100, means 0% -- 100%
            [FieldOffset(49)] public int _bright;        //Brightness : 0 ?100, means 0% -- 100%
            [FieldOffset(53)] public int _sharpLevel;    //SharpLevel index: 0=Normal, 1=Sharp, 2=Sharper, 3=Sharpest
            [FieldOffset(57)] public bool _blkWhtMode;   //true for B&W
            [FieldOffset(58)] public bool _horzMirror;   //true for Horiz
            [FieldOffset(59)] public bool _vertFlip;     //true for vertical flip

            //For Capturing files
            [FieldOffset(60)] public int _catchFrames;   //Number of frames to be captured
            [FieldOffset(64)] public bool _isAvgFrame;   //Save only one frame, but it’s the average of all grabbed frames
            [FieldOffset(65)] public bool _isCatchRaw;   //Save as RAW Data File? 
            [FieldOffset(66)] public bool _isRawGraph;   //Save as JPG or BMP, but not corrected by Gamma, contrast, bright and sharp algorithm
            [FieldOffset(67)] public bool _isCatchJPEG;  //Save as JPEG
            [FieldOffset(68)] public bool _catchIgnoreSkip; //always capture full resolution (but may display with decimation
            */
            [FieldOffset(40)]
            public bool _imageRendorFitWindow; //True if the image always fit video window, otherwise False
            [FieldOffset(44)]
            public int _gamma;         //Gamma value: 0 ?20, means 0.0 ?2.0
            [FieldOffset(48)]
            public int _contrast;      //Contrast value: 0 ?100, means 0% -- 100%
            [FieldOffset(52)]
            public int _bright;        //Brightness : 0 ?100, means 0% -- 100%
            [FieldOffset(56)]
            public int _sharpLevel;    //SharpLevel index: 0=Normal, 1=Sharp, 2=Sharper, 3=Sharpest
            [FieldOffset(60)]
            public bool _blkWhtMode;   //true for B&W
            [FieldOffset(61)]
            public bool _horzMirror;   //true for Horiz
            [FieldOffset(62)]
            public bool _vertFlip;     //true for vertical flip

            //For Capturing files
            [FieldOffset(64)]
            public int _catchFrames;   //Number of frames to be captured
            [FieldOffset(68)]
            public bool _isAvgFrame;   //Save only one frame, but it’s the average of all grabbed frames
            [FieldOffset(69)]
            public bool _isCatchRaw;   //Save as RAW Data File? 
            [FieldOffset(70)]
            public bool _isRawGraph;   //Save as JPG or BMP, but not corrected by Gamma, contrast, bright and sharp algorithm
            [FieldOffset(71)]
            public bool _isCatchJPEG;  //Save as JPEG
            [FieldOffset(72)]
            public bool _catchIgnoreSkip; //always capture full resolution (but may display with decimation
        }

        private ImageControl _imgControl = new ImageControl();

        //default constructor for testing
        public CCameraUSB( FUSBCam mightexCamera )
        {
            _imgControl._revision = 1;      //Reserved for internal use only
            _imgControl._resolution = 4;
            _imgControl._binMode = 1;
            _imgControl._xStart = 30;
            _imgControl._yStart = 30;
            _imgControl._greenGain = 10;
            _imgControl._blueGain = 11;
            _imgControl._redGain = 12;
            _imgControl._maxExposureTimeIndex = 2;
            _imgControl._exposureTime = 45*1000;

            _imgControl._imageRendorFitWindow = false;
            _imgControl._gamma = 10;
            _imgControl._contrast = 50;
            _imgControl._bright = 50;
            _imgControl._sharpLevel = (int)CCameraUSB.SHARPNESS_LEV.NORMAL;
            _imgControl._blkWhtMode = false;
            _imgControl._horzMirror = true;
            _imgControl._vertFlip = true;

            _imgControl._catchFrames = 1;  //N to infinite
            _imgControl._isAvgFrame = false;
            _imgControl._isCatchRaw = false;
            _imgControl._isRawGraph = false;
            _imgControl._isCatchJPEG = true;  //Save as JPEG
            /* JTZ: it's highly recommended to set the "_catchIgnoreSkip" to false, as setting it to "true" will
             * use considerable more PC resources in camera engine. 
             */
            _imgControl._catchIgnoreSkip = true;

            MightexCam = mightexCamera;
            frameDelegate = new FrameCallbackDelegate(GrabbingFrameCallback);

            //Max x and y will get adjusted with change in res.
            this.SetMaxXY((RESOLUTION)_imgControl._resolution);
        }

        // JTZ: The frame callback.
        public void GrabbingFrameCallback(uint FrameType, uint Row, uint Column,
                                  ref ImageProperty frameProperty, uint BufferPtr)
        {
            uint i, pixelAvg;
            uint frameSize;

            unsafe
            {
                byte *frameptr;

                pixelAvg = 0;
                frameSize = (Row * Column);
                frameptr = (byte *)BufferPtr;
                for ( i=0; i<frameSize; i++ )
                {
                    pixelAvg += *frameptr;
                    frameptr++;
                }
                pixelAvg = pixelAvg / frameSize;
            }
            /*
             * JTZ: As the callback in invoked in a working thread, it's not recommended to do any
             * GUI operation here...instead, the callback can get the image (and the property of it)
             * and notify the main thread (usually the GUI thread) about that....
             */
            MightexCam.SetCallBackMessage(ref frameProperty, pixelAvg);
        }

        public int GetExpTime()
        {
            return _imgControl._exposureTime;
        }

        public int GetXstart()
        {
            return _imgControl._xStart;
        }
        public int GetYstart()
        {
            return _imgControl._yStart;
        }

        public int GetRedGain()
        {
            return _imgControl._redGain;
        }
        public int GetGreenGain()
        {
            return _imgControl._greenGain;
        }
        public int GetBlueGain()
        {
            return _imgControl._blueGain;
        }

        public bool GetBlkWht()
        {
            return _imgControl._blkWhtMode;
        }
        public bool GetHorzMirr()
        {
            return _imgControl._horzMirror;
        }
        public bool GetFlipVert()
        {
            return _imgControl._vertFlip;
        }
        //Thin wrapper for calling into MT_USBCamera_SDK_Stdcall.dll
        //Interops are in the USBCam.Designer.cs (partial class) file

        /// <summary>
        /// Call this function first, this function communicates with device driver to reserve resources
        /// </summary>
        /// <returns>number of cameras on USB 2.0 chan</returns>
        public int InitDevice()
        {
            int numCam = MtxInitDevice();

            if (numCam < 0)
            {
                MessageBox.Show("Error trying to initialize camera resources. No cameras found on USB 2.0 bus.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return numCam;
        }
        
        //Call this function before the app terminates, it releases all resources
        public void UnInitDevice()
        {
            if (MtxUnInitDevice() < 0)
            {
                MessageBox.Show("Error trying to uninitialize camera.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        //Opens device, deviceID is a zero base index, so if InitDevice returns 2 for example, there devices at 0 and 1.
        //Note: The current device driver only supports ONE opened device, opening another device will close the previous opened device automatically
        //returns: device handle, i.e. devHandle
        public void OpenDevice(int deviceID)
        {
            _devHandle = MtxOpenDevice(deviceID);
        }

        /// <summary>
        /// Function used to get the module number on a particular USB channel
        /// </summary>
        /// <returns></returns>
        public string GetModuleNo()
        {
            string moduleNumber = "Unknown";
            //char moduleNo = 'X';
            StringBuilder rtnStr = new StringBuilder();
            
            if (MtxGetModuleNo(_devHandle, rtnStr) < 0)
            {
                MessageBox.Show("Error trying to retrieve camera module number.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                moduleNumber = rtnStr.ToString();
            }
            return moduleNumber;
        }

        /// <summary>
        /// Function used to get the serial number on a particular USB channel
        /// </summary>
        /// <returns></returns>
        public string GetSerialNo()
        {
            string serialNumber = "Unknown2";
            StringBuilder rtnStr = new StringBuilder();
            if (MtxGetSerialNo(_devHandle, rtnStr) < 0)
            {
                MessageBox.Show("Error trying to retrieve camera serial number.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                serialNumber = rtnStr.ToString();
            }
            return serialNumber;
        }

        //Camera has multithread engine internally, which is responsible for all the frame grabbing, raw data to RGB data conversion…etc. functions. 
        //User MUST start this engine for all the following camera related operations
        //ParentHandle ?The window handle of the main form of user’s application, as the engine relies on
        //Windows Message Queue, it needs a parent window handle
        public void StartCameraEngine(IntPtr parentHandle)
        {
            if (MtxStartCameraEngine(parentHandle, _devHandle) < 0)
            {
                MessageBox.Show("Error trying to start camera engine.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// Stops the started camera engine.
        /// </summary>
        public void StopCameraEngine()
        {
            if (MtxStopCameraEngine(_devHandle) < 0)
            {
                MessageBox.Show("Error trying to stop camera engine.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// Sets camera to either "video" mode - continuously deliver frames to PC or
        /// "external trigger" mode - camera waits for external trigger to capture 1 frame
        /// </summary>
        /// <param name="mode"></param>
        public void SetCameraWorkMode(CAM_WORKMODE mode)
        {
            int WorkMode = (int)mode;
            if (MtxSetCameraWorkMode(_devHandle, WorkMode) < 0)
            {
                MessageBox.Show("Error trying to set camera work mode.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        //While the camera is in “External Trigger?Mode, invoking this function set the parameters for external trigger mode.
        public void SetExternalParameters(bool autoLoop, bool IsRawGraph, bool IsJPEG, ref char FilePath, ref char FileName)
        {
            if(MtxSetExternalParameters(_devHandle, autoLoop, IsRawGraph, IsJPEG, ref FilePath, ref FileName) < 0)
            {
                MessageBox.Show("Error trying to set external parameters for use with external trigger.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        //While the camera is in “External Trigger?Mode, invoking this function starting the waiting for external signal.
        //private void WaitingExternalTrigger(DEV_HANDLE devHandle, bool StartWait, CallBackFunc Aproc)
        //{
        //}

        /// <summary>
        /// Hides (makes invisible) the control panel which will be displayed once the camera engine starts
        /// </summary>
        ///
        /* JTZ: I add this method for user's debug purpose...use might show the control panel...so all the
         * settings (e.g. exposure time...etc.) are visible on this panel...user might hide it in his  formal
         * application.
         */
        public void ShowFrameControlPanel( uint Left, uint Top)
        {
            String myTitle = "Debug Only";

            if (MtxShowFrameControlPanel(_devHandle, true, true, myTitle, Left, Top) < 0)
            {
                MessageBox.Show("Error trying to show camera control panel.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// Hides (makes invisible) the control panel which will be displayed once the camera engine starts
        /// </summary>
        public void HideFrameControlPanel()
        {
            if (MtxHideFrameControlPanel(_devHandle) < 0)
            {
                MessageBox.Show("Error trying to hide camera control panel.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// Show video window
        /// </summary>
        public void ShowVideoWindow(int top, int left, int width, int height)
        {
            if (MtxShowVideoWindow(_devHandle, top, left, width, height) < 0)
            {
                MessageBox.Show("Error trying to display video window. Invalid device or engine not started.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// Starts frame grabbing after camera resources prepared. After call, user should see images in video window
        /// </summary>
        /// <param name="totalFrames"></param>
        public void StartFrameGrab(int totalFrames)
        {
            // Install frame call back.
            MtxInstallFrameHooker(_devHandle, false, 0, frameDelegate);
            if (MtxStartFrameGrab(_devHandle, totalFrames) < 0)
            {
                MessageBox.Show("Error trying to start frame grabbing images.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// Stops frame grabbing, call if totalFrames set to INFINITE_FRAMES
        /// </summary>
        public void StopFrameGrab()
        {
            // Install frame call back.
            MtxInstallFrameHooker(_devHandle, false, 0, null );
            if (MtxStopFrameGrab(_devHandle) < 0)
            {
                MessageBox.Show("Error trying to stop frame grabbing images.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        //SettingPtr is a pointer to TImageControl struct
        unsafe public void GetCamFrameSettings()
        {
            fixed (ImageControl* structPtr = &_imgControl)
            {
                //if (MtxGetFrameSetting(_devHandle, ref _imgControl) < 0)
                if (MtxGetFrameSetting(_devHandle, (uint)structPtr) < 0)
                {
                    MessageBox.Show("Error trying to get camera frame (image) settings.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        /// <summary>
        /// Set all image control parameters by current settings, effect is immediate
        /// </summary>
        unsafe public void SetFrameSetting()
        {
            /*
             * JTZ: I don't know why...but this works...it might be related to the "movable" object,
             * while _imgControl is taken as "ref" parameter to call the API, it becomes a "movable"
             * object...But why "bool" only ???
             */
            fixed ( ImageControl * structPtr = &_imgControl)
            {
                //if (MtxSetFrameSetting(_devHandle, ref _imgControl ) < 0)
                if (MtxSetFrameSetting(_devHandle, (uint)structPtr) < 0)
                {
                    MessageBox.Show("Error trying to set default camera frame settings.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        //Note: only three elements _resolution, _binMode and _imageRendorFitWindow are used by this function, all others are ignored.
        /// <summary>
        /// Set the resolution (including capture and render)
        /// </summary>
        /// <param name="resolution"></param>
        /// <param name="binMode"></param>
        /// <param name="imgRendorFitWindow"></param>
        unsafe public void SetResolution(RESOLUTION resolution, int binMode, bool imgRendorFitWindow)
        {
            _imgControl._resolution = (int)resolution;
            _imgControl._binMode = binMode;
            _imgControl._imageRendorFitWindow = imgRendorFitWindow;

            this.SetMaxXY(resolution);

            fixed (ImageControl* structPtr = &_imgControl)
            {
                //if (MtxSetFrameSetting(_devHandle, ref _imgControl ) < 0)
                if (MtxSetResolution(_devHandle, (uint)structPtr) < 0)
                {
                    MessageBox.Show("Error trying to set camera resolution, bin mode, and image rendor fitting.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            /*
            if (MtxSetResolution(_devHandle, ref _imgControl) < 0)
            {
                MessageBox.Show("Error trying to set camera resolution, bin mode, and image rendor fitting.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            */
        }

        private void SetMaxXY(RESOLUTION res)
        {
            //Set maximum X and Y start positions based on resolution
            switch (res)
            {
                case RESOLUTION.TINY:
                    _maxX = 32;
                    _maxY = 32;
                    break;
                case RESOLUTION.LITTLE:
                    _maxX = 64;
                    _maxY = 64;
                    break;
                case RESOLUTION.SMALL:
                    _maxX = 160;
                    _maxY = 120;
                    break;
                case RESOLUTION.MOBILE:
                    _maxX = 320;
                    _maxY = 240;
                    break;
                case RESOLUTION.VGA:
                    _maxX = 640;
                    _maxY = 480;
                    break;
                case RESOLUTION.SVGA:
                    _maxX = 800;
                    _maxY = 600;
                    break;
                case RESOLUTION.UVGA:
                    _maxX = 1024;
                    _maxY = 768;
                    break;
                case RESOLUTION.VGALARGE:
                    _maxX = 1280;
                    _maxY = 1024;
                    break;
                case RESOLUTION.M_2M:
                    _maxX = 1600;
                    _maxY = 1200;
                    break;
                case RESOLUTION.M_3M:
                    _maxX = 2048;
                    _maxY = 1536;
                    break;
                default:
                    MessageBox.Show("Resolution not defined, unable to set maximum X and Y start positions.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    break;
            }
        }

        //Note: only two elements _xStart and _yStart are used by this function, all others are ignored.
        /// <summary>
        /// Set the start position of ROI
        /// </summary>
        /// <param name="Xstart"></param>
        /// <param name="Ystart"></param>
        unsafe public void SetStartPosition(int Xstart, int Ystart)
        {
            _imgControl._xStart = Xstart;
            _imgControl._yStart = Ystart;

            fixed (ImageControl* structPtr = &_imgControl)
            {
                //if(MtxSetStartPosition(_devHandle, ref _imgControl) < 0)
                if (MtxSetStartPosition(_devHandle, (uint)structPtr) < 0)
                {
                    MessageBox.Show("Error trying to set camera X and Y start positions.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        //Note: only three elements _greenGain, _blueGain and _redGain are used by this function, all others are ignored.
        /// <summary>
        /// Set RGB Gain parameters
        /// </summary>
        /// <param name="redGain"></param>
        /// <param name="greenGain"></param>
        /// <param name="blueGain"></param>
        unsafe public void SetGain(int redGain, int greenGain, int blueGain)
        {
            _imgControl._redGain = redGain;
            _imgControl._greenGain = greenGain;
            _imgControl._blueGain = blueGain;

            fixed (ImageControl* structPtr = &_imgControl)
            {
                //if (MtxSetGain(_devHandle, ref _imgControl) < 0)
                if (MtxSetGain(_devHandle, (uint)structPtr) < 0)
                {
                    MessageBox.Show("Error trying to set camera RGB gains.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        //Note: only two elements _maxExposureTimeIndex and _exposureTime are used by this function, all others are ignored.
        /// <summary>
        /// Set camera exposure parameters
        /// </summary>
        /// <param name="maxExpTimeIndex"></param>
        /// <param name="expTime"></param>
        unsafe public void SetExposureTime(int maxExpTimeIndex, int expTime)
        {
            _imgControl._maxExposureTimeIndex = maxExpTimeIndex;
            _imgControl._exposureTime = expTime*1000; //convert milli to microseconds

            fixed (ImageControl* structPtr = &_imgControl)
            {
                //if (MtxSetExposureTime(_devHandle, ref _imgControl) < 0)
                if (MtxSetExposureTime(_devHandle, (uint)structPtr) < 0)
                {
                    MessageBox.Show("Error trying to set camera exposure settings.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        //Set the Gamma, Contrast and Brightness parameters
        //Note: only four elements _gamma, _contrast, _bright and _sharpLevel are used by this function, all others are ignored.
        unsafe public void SetGammaValue(int gamma, int contrast, int brightness, SHARPNESS_LEV sharpLev)
        {
            _imgControl._gamma = gamma;
            _imgControl._contrast = contrast;
            _imgControl._bright = brightness;
            _imgControl._sharpLevel = (int) sharpLev;

            fixed (ImageControl* structPtr = &_imgControl)
            {
                //if (MtxSetGammaValue(_devHandle, ref _imgControl) < 0)
                if (MtxSetGammaValue(_devHandle, (uint)structPtr) < 0)
                {
                    MessageBox.Show("Error trying to set camera gamma, contrast, brightness, and sharpness levels.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
        
        //Note: only three elements _blkWhtmode, _horzMirror and _vertFlip are used by this function, all others are ignored.
        /// <summary>
        /// Set the BWMode, HorizontalMirror and VerticalFlip parameters
        /// </summary>
        /// <param name="bwMode"></param>
        /// <param name="horzMirror"></param>
        /// <param name="vertFlip"></param>
        unsafe public void SetShowMode(bool bwMode, bool horzMirror, bool vertFlip)
        {
            _imgControl._blkWhtMode = bwMode;
            _imgControl._horzMirror = horzMirror;
            _imgControl._vertFlip = vertFlip;

            fixed (ImageControl* structPtr = &_imgControl)
            {
                if (MtxSetShowMode(_devHandle, (uint)structPtr) < 0)
                //if (MtxSetShowMode(_devHandle, ref _imgControl) < 0)
                {
                    MessageBox.Show("Error trying to set camera B&W moode, horizontal mirror, and vertical flip settings.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        /// <summary>
        /// Automatically set white balance, set proper exposure time and put white paper in front of camera
        /// </summary>
        public void SetWhiteBalance()
        {
            try
            {
                if (MtxSetWhiteBalance(_devHandle) < 0)
                {
                    MessageBox.Show("Error trying to set white balance automatically.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            catch (DivideByZeroException)
            {
                MessageBox.Show("Image too dark. Set white balance by placing white paper in FOV with adequate lighting.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        
        //Set the current frame grabbing rate.
        //rate level must be from 0 (lowest) to 10 (highest)
        public void SetFrameRateLevel(int RateLevel)
        {
            if (RateLevel < 0 || RateLevel > 10)
            {
                MessageBox.Show("Invalid frame rate level, must be 0 through 10.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else if (MtxSetFrameRateLevel(_devHandle, RateLevel) < 0)
            {
                MessageBox.Show("Error trying to set camera frame rate level.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        //returns:  -1 If the function fails 
        //          -2 If camera is not grabbing frames
        //          -3 Camera is grabbing frames but is unplugged from USB bus
        //          1 The call succeeds
        /// <summary>
        /// Get the current frame grabbing rate.
        /// </summary>
        public void GetCurrentFrameRate()
        {
            if(MtxGetCurrentFrameRate(_devHandle) < 0)
            {
                MessageBox.Show("Error trying to get current frame rate.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        //Set the auto exposure and auto exposure mark
        public void SetAutoExposure(bool AutoExposureOn, bool ShowExposureMark)
        {
            if (MtxSetAutoExposure(_devHandle, AutoExposureOn, ShowExposureMark) < 0)
            {
                MessageBox.Show("Error trying to set auto exposure and mark.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
       
        //Get the bitmap format frame of the last captured frame.
        //While the frame grabbing is running OR it’s stopped, 
        //we can always get the last (for the time this function is invoking) frame of the Video window in Bitmap format. Note that this function may mainly be used in situation of user stop the video as 
        //the frame is exactly the user’s interesting.
        public void GetLastBMPFrame(string FileName)
        {
            StringBuilder FN = new StringBuilder();
            FN.Insert(0, FileName);

            if(MtxGetLastBMPFrame(_devHandle, FN) < 0)
            {
                MessageBox.Show("Error trying to get last BMP frame.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        
        //call this function to get a frame.
        //FrameType 0=raw data
        //          1=24 bit bitmap data, device independent bitmap
        //public void GetCurrentFrame(FRAME_TYPE FrameType, IntPtr Buffer)
        public IntPtr GetCurrentFrame(FRAME_TYPE FrameType)
        {
            //if(_pImage.
            int FrameTyp = (int)FrameType;
            if (MtxGetCurrentFrame(_devHandle, FrameTyp, _pImage) < 0)
            {
                MessageBox.Show("Error trying to get current frame buffer. Frame grabber running?", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return _pImage;           
        }

        public void AllocImageMem(RESOLUTION res)
        {
            this.SetMaxXY(res);
            _pImage = Marshal.AllocHGlobal(pixelDepthBits * _maxX * _maxY);
        }

        public void FreeImageMem()
        {
            if (_pImage != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(_pImage);
                _pImage = IntPtr.Zero;
            }
        }
        //Call this function to save one or more frames to files.
        /// <summary>
        /// 
        /// </summary>
        /// <param name="numFrames">number of frames to be captured</param>
        /// <param name="isAvgFrame">average multiple frames if true</param>
        /// <param name="isCatchRaw">save as raw data file if true</param>
        /// <param name="isRawGraph">save as raw but not corrected</param>
        /// <param name="isCatchJPEG">save as JPEG</param>
        /// <param name="FilePath">file path</param>
        /// <param name="FileName">file name</param>
        public void SaveFramesToFiles(int numFrames, bool isAvgFrame, bool isCatchRaw, bool isRawGraph, bool isCatchJPEG, string FilePath, string FileName)
        {
            StringBuilder FP = new StringBuilder();
            FP.Insert(0, FilePath);
            StringBuilder FN = new StringBuilder();
            FN.Insert(0, FileName);

            _imgControl._catchFrames = numFrames;   //Number of frames to be captured
            _imgControl._isAvgFrame = isAvgFrame;   //Save only one frame, but it’s the average of all grabbed frames
            _imgControl._isCatchRaw = isCatchRaw;   //Save as RAW Data File? 
            _imgControl._isRawGraph = isRawGraph;    //Save as JPG or BMP, but not corrected by Gamma, contrast, bright and sharp algorithm
            _imgControl._isCatchJPEG = isCatchJPEG;  //Save as JPEG
            _imgControl._catchIgnoreSkip = true;

            if (MtxSaveFramesToFiles(_devHandle, ref _imgControl, FP, FN) < 0)
            {
                MessageBox.Show("Error trying to save camera frames to file.", _camError, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        #region Mightex interop functions for accessing MT_USBCamera_SDK_Stdcall.dll and MtUsblib.dll files
  
        //Call this function first, this function communicates with device driver to reserve resources
        //When the system uses NTFS use WINNT, for FAT32 use WINDOWS
        /*
         * JTZ: Note that I assume we put MT_USBCamera_SDK_Stdcall.dll and MtUsblib.dll in windows\system32.
         */

        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_InitDevice", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxInitDevice();


        //Call this function before the app terminates, it releases all resources
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_UnInitDevice", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxUnInitDevice();

        //Opens device, deviceID is a zero base index, so if InitDevice returns 2 for example, there devices at 0 and 1.
        //Note: The current device driver only supports ONE opened device, opening another device will close the previous opened device automatically
        //returns: device handle
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_OpenDevice", CallingConvention = CallingConvention.StdCall)]
        private static extern uint MtxOpenDevice(int deviceID);

        //Function used to get the device module number on a particular USB chan
        //For an opened device, user might get its Module Number by invoking this function.

        //The module number and serial number are what appear if one calls the
        //SDK_HANDLE_API MTUSB_ShowOpenDeviceDialog() method.
        //returns:  -1 If the function fails (e.g. invalid device handle), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_GetModuleNo", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxGetModuleNo(uint devHandle, StringBuilder moduleNo);

        //Function used to get the serial module number on a particular USB chan
        //reserve 16 char for character ref
        //returns:  -1 If the function fails (e.g. invalid device handle), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_GetSerialNo", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxGetSerialNo(uint devHandle, StringBuilder serialNo);

        //Camera has multithread engine internally, which is responsible for all the frame grabbing, raw data to RGB data conversion…etc. functions. 
        //User MUST start this engine for all the following camera related operations
        //ParentHandle ?The window handle of the main form of user’s application, as the engine relies on
        //Windows Message Queue, it needs a parent window handle
        //returns:  -1 If the function fails (e.g. invalid device handle), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_StartCameraEngine", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxStartCameraEngine(IntPtr parentHandle, uint devHandle);

        //Stops the started camera engine.
        //returns:  -1 If the function fails (e.g. invalid device handle or if the engine is NOT started), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_StopCameraEngine", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxStopCameraEngine(uint devHandle);

        //Sets camera to either "video" mode - continuously deliver frames to PC or
        //"external trigger" mode - camera waits for external trigger to capture 1 frame
        //returns:  -1 If the function fails (e.g. invalid device handle), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_SetCameraWorkMode", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxSetCameraWorkMode(uint devHandle, int WorkMode);

        //While the camera is in “External Trigger?Mode, invoking this function set the parameters for external trigger mode.
        //returns:  -1 if the function fails (e.g. invalid device handle or camera is NOT in External Trigger Mode or it’s during waiting for external trigger ), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_SetExternalParameters", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxSetExternalParameters(uint devHandle, bool autoLoop, bool IsRawGraph, bool IsJPEG, ref char FilePath, ref char FileName);

        //While the camera is in “External Trigger?Mode, invoking this function starting the waiting for external signal.
        //private void WaitingExternalTrigger(DEV_HANDLE devHandle, bool StartWait, CallBackFunc Aproc)
        //{
        //}
        //Showes (makes visible) the control panel which will be displayed once the camera engine starts
        //returns:  -1 If the function fails (e.g. invalid device handle or if the engine is NOT started yet), 1 if the call succeeds.
        /*
         * JTZ: I add this API for user debug purpose. 
         */
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_ShowFrameControlPanel", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxShowFrameControlPanel(uint DevHandle, bool IsTriggerModeAllow, bool IsCloseParent,
                                    string PanelTitle, uint Left, uint Top );

        //Hides (makes invisible) the control panel which will be displayed once the camera engine starts
        //returns:  -1 If the function fails (e.g. invalid device handle or if the engine is NOT started yet), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_HideFrameControlPanel", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxHideFrameControlPanel(uint DevHandle);

        //Shows video window in separate dialog 
        //returns:  -1 If the function fails (e.g. invalid device handle or if the engine is NOT started yet), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_ShowVideoWindow", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxShowVideoWindow(uint DevHandle, int top, int left, int width, int height);

        //Starts frame grabbing after camera resources prepared.
        //After call, user should see images in video window
        //returns:  -1 If the function fails (e.g. invalid device handle or if the engine is NOT started yet), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_StartFrameGrab", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxStartFrameGrab(uint devHandle, int totalFrames);

        //Stops frame grabbing, call if totalFrames set to INFINITE_FRAMES
        //returns:  -1 If the function fails (e.g. invalid device handle or if the engine is NOT started yet), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_StopFrameGrab", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxStopFrameGrab(uint devHandle);

        //SettingPtr is a pointer to TImageControl struct
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_GetFrameSetting", CallingConvention = CallingConvention.StdCall)]
        //private static extern int MtxGetFrameSetting(uint devHandle, ref ImageControl Setting);
        private static extern int MtxGetFrameSetting(uint devHandle, uint Setting);

        //Set all parameters by invoking, effect is immediate
        //returns:  -1 If the function fails (e.g. invalid device handle), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_SetFrameSetting",CallingConvention = CallingConvention.StdCall)]
        //private static extern int MtxSetFrameSetting(uint DevHandle, ref ImageControl Setting);
        /* 
         * JTZ: The second parameter actually holds the address of _imgControl.
         */
        private static extern int MtxSetFrameSetting(uint DevHandle, uint Setting);

        //Set the resolution (including capture and render)
        //returns:  -1 If the function fails (e.g. invalid device handle), 1 if the call succeeds.
        //Note: only three elements _resolution, _binMode and _imageRendorFitWindow are used by this function, all others are ignored.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_SetResolution", CallingConvention = CallingConvention.StdCall)]
        //private static extern int MtxSetResolution(uint DevHandle, ref ImageControl Setting);
        private static extern int MtxSetResolution(uint DevHandle, uint Setting);

        //Set the start position of ROI
        //returns:  -1 If the function fails (e.g. invalid device handle), 1 if the call succeeds.
        //Note: only two elements _xStart and _yStart are used by this function, all others are ignored.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_SetStartPosition", CallingConvention = CallingConvention.StdCall)]
        //private static extern int MtxSetStartPosition(uint DevHandle, ref ImageControl Setting);
        private static extern int MtxSetStartPosition(uint DevHandle, uint Setting);

        //Set RGB Gains parameters
        //returns:  -1 If the function fails (e.g. invalid device handle), 1 if the call succeeds.
        //Note: only three elements _greenGain, _blueGain and _redGain are used by this function, all others are ignored.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_SetGain", CallingConvention = CallingConvention.StdCall)]
        //private static extern int MtxSetGain(uint DevHandle, ref ImageControl Setting);
        private static extern int MtxSetGain(uint DevHandle, uint Setting);

        //Set exposure parameters
        //returns:  -1 If the function fails (e.g. invalid device handle), 1 if the call succeeds.
        //Note: only two elements _maxExposureTimeIndex and _exposureTime are used by this function, all others are ignored.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_SetExposureTime", CallingConvention = CallingConvention.StdCall)]
        //private static extern int MtxSetExposureTime(uint DevHandle, ref ImageControl Setting);
        private static extern int MtxSetExposureTime(uint DevHandle, uint Setting);

        //Set the Gamma, Contrast and Brightness parameters
        //returns:  -1 If the function fails (e.g. invalid device handle), 1 if the call succeeds.
        //Note: only four elements _gamma? _contrast? _bright and _sharpLevel are used by this function, all others are ignored.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_SetGammaValue", CallingConvention = CallingConvention.StdCall)]
        //private static extern int MtxSetGammaValue(uint DevHandle, ref ImageControl Setting);
        private static extern int MtxSetGammaValue(uint DevHandle, uint Setting);

        //Set the BWMode, HorizontalMirror and VerticalFlip parameters
        //returns:  -1 If the function fails (e.g. invalid device handle), 1 if the call succeeds.
        //Note: only three elements _blkWhtmode, _horzMirror and _vertFlip are used by this function, all others are ignored.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_SetShowMode", CallingConvention = CallingConvention.StdCall)]
        //private static extern int MtxSetShowMode(uint DevHandle, ref ImageControl Setting);
        private static extern int MtxSetShowMode(uint DevHandle, uint Setting);

        //Automatically set white balance, set proper exposure time and put white paper in front of camera
        //returns:  -1 If the function fails (e.g. invalid device handle), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_SetWhiteBalance", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxSetWhiteBalance(uint DevHandle);

        //Set the current frame grabbing rate.
        //returns:  -1 If the function fails (e.g. invalid device handle), 1 if the call succeeds.
        //rate level must be from 0 (lowest) to 10 (highest)
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_SetFrameRateLevel", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxSetFrameRateLevel(uint DevHandle, int RateLevel);

        //Get the current frame grabbing rate.
        //returns:  -1 If the function fails 
        //          -2 If camera is not grabbing frames
        //          -3 Camera is grabbing frames but is unplugged from USB bus
        //          1 The call succeeds
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_GetCurrentFrameRate", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxGetCurrentFrameRate(uint DevHandle);

        //Set the auto exposure and auto exposure mark
        //returns:  -1 If the function fails (e.g. invalid device handle or if the engine is NOT started yet), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_SetAutoExposure", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxSetAutoExposure(uint DevHanlde, bool AutoExposureOn, bool ShowExposureMark);

        //Get the bitmap format frame of the last captured frame.
        //returns:  -1 If the function fails (e.g. invalid device handle)
        //1 if the call succeeds.
        //While the frame grabbing is running OR it’s stopped, 
        //we can always get the last (for the time this function is invoking) frame of the Video window in Bitmap format. Note that this function may mainly be used in situation of user stop the video as 
        //the frame is exactly the user’s interesting.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_GetLastBMPFrame", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxGetLastBMPFrame(uint DevHandle, StringBuilder FileName);

        //call this function to get a frame.
        //FrameType 0=raw data
        //          1=24 bit bitmap data(DIB)
        //returns: -1 If the function fails (e.g. invalid device handle or the frame grabbing is NOT running), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_GetCurrentFrame", CallingConvention = CallingConvention.StdCall)]
        //private static extern int MtxGetCurrentFrame(uint DevHandle, int FrameType, ref byte Buffer);
        private static extern int MtxGetCurrentFrame(uint DevHandle, int FrameType, IntPtr Buff);

        //Call this function to save one or more frames to files.
        //returns: -1 If the function fails (e.g. invalid device handle or the frame grabbing is NOT running), 1 if the call succeeds.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_SaveFramesToFiles", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxSaveFramesToFiles(uint DevHandle, ref ImageControl Setting, StringBuilder FilePath, StringBuilder FileName);

        // JTZ: we allow user to install a callback for each grabbed frame.
        [DllImport("MT_USBCamera_SDK_Stdcall.dll", EntryPoint = "MTUSB_InstallFrameHooker", CallingConvention = CallingConvention.StdCall)]
        private static extern int MtxInstallFrameHooker(uint DevHandle, bool IsMaxFrameRate, uint FrameType, Delegate FrameCallBack);

        #endregion
    }

}
