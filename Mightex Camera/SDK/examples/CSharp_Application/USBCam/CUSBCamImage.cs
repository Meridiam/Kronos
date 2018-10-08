using System;
using System.Collections.Generic;
using System.Text;

namespace USBCam
{
    //might want to make this class static sense only one Mightex camera can be active at a time
    public class CUSBCamImage
    {
        //[ StructLayout( LayoutKind.Sequential, CharSet=CharSet.Ansi ) ]

        //For Image Capture (this is the TImageControl struct in the Mightex lib)
        public int _revision;      //Reserved for internal use only
        public int _resolution;    //indices: 0=32x32, 1=64x64, 2=160x120, 3=320x240, 4=640x480, 5=800x600, 6=1024x768, 7=1280x1024
        public int _binMode;       //1 – No Skip mode, 2 – 2X skip(1:2 decimation)
        public int _xStart;        //Upper left hand corner, positive right 
        public int _yStart;        //Upper left hand corner, positive down
        public int _greenGain;     //Green Gain Value: 0 – 128, the actual gain is GreenGain/8
        public int _blueGain;      //Blue Gain Value: 0 – 128, the actual gain is BlueGain/8
        public int _redGain;       //Red Gain Value: 0 – 128, the actual gain is RedGain/8
        public int _maxExposureTimeIndex;  //Maximum exposure time index: 0=5ms, 1=10ms, 2=100ms, 3=750ms
        public int _exposureTime;       //current exposure time in microseconds

        //For Image Render
        public bool _imageRendorFitWindow; //True if the image always fit video window, otherwise False
        public int _gamma;         //Gamma value: 0 – 20, means 0.0 – 2.0
        public int _contrast;      //Contrast value: 0 – 100, means 0% -- 100%
        public int _bright;        //Brightness : 0 – 100, means 0% -- 100%
        public int _sharpLevel;    //SharpLevel index: 0=Normal, 1=Sharp, 2=Sharper, 3=Sharpest
        public bool _blkWhtMode;   //true for B&W
        public bool _horzMirror;   //true for Horiz
        public bool _vertFlip;     //true for vertical flip

        //For Capturing files
        public int _catchFrames;   //Number of frames to be captured
        public bool _isAvgFrame;   //Save only one frame, but it’s the average of all grabbed frames
        public bool _isCatchRaw;   //Save as RAW Data File? 
        public bool _isRawGraph;    //Save as JPG or BMP, but not corrected by Gamma, contrast, bright and sharp algorithm
        public bool _isCatchJPEG;  //Save as JPEG
        public bool _catchIgnoreSkip; //always capture full resolution (but may display with decimation

        //default constructor
        public CUSBCamImage()
        {
        //_revision;      //Reserved for internal use only
            _resolution = 4;    
            _binMode = 1;       
            _xStart = 0;         
            _yStart = 0;        
            _greenGain = 50;     
            _blueGain = 50;      
            _redGain =50;       
            _maxExposureTimeIndex = 2;  
            _exposureTime = 15;
 
            _imageRendorFitWindow = true; 
            _gamma = 10;         
            _contrast = 50;      
            _bright = 50;        
            _sharpLevel = 0;    
            _blkWhtMode = false;   
            _horzMirror = true;   
            _vertFlip = true;     

            _catchFrames = 1;  //N to infinite
            _isAvgFrame = false;   
            _isCatchRaw = false;   
            _isRawGraph = false;    
            _isCatchJPEG = true;  //Save as JPEG
            _catchIgnoreSkip = true; 
        }
    }
}
