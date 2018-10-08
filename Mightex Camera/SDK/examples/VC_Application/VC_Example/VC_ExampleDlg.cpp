// VC_ExampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VC_Example.h"
#include "VC_ExampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// JTZ: Timer for GPIO
#define GPIO_TIMER_ID	1
#define GPIO_TIMER_MS   250

// JTZ: image queue
#define QUEUE_SIZE 2
unsigned char * ImageQueuePtr[QUEUE_SIZE];
int QueueHead, QueueTail, QueueSize;
CRITICAL_SECTION QueueLock;

bool IsGPIOConfigDone;
bool IsSetOutput;
unsigned char OutputByte, InputByte;
int TimerCount;

int FrameCount;
int FrameTimeStamp[200];
extern CVC_ExampleApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVC_ExampleDlg dialog

CVC_ExampleDlg::CVC_ExampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVC_ExampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVC_ExampleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVC_ExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVC_ExampleDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVC_ExampleDlg, CDialog)
	//{{AFX_MSG_MAP(CVC_ExampleDlg)
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_FRAMECALLBACK, OnFrameCallBackMessage)
	ON_MESSAGE(WM_DEVICECALLBACK, OnDeviceCallBackMessage)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DEVICEINIT_BUTTON, OnDeviceinitButton)
	ON_BN_CLICKED(IDC_DEVICEOPEN_BUTTON, OnDeviceopenButton)
	ON_BN_CLICKED(IDC_GetModule_BUTTON, OnGetModuleBUTTON)
	ON_BN_CLICKED(IDC_GetSerialNo_BUTTON, OnGetSerialNoBUTTON)
	ON_BN_CLICKED(IDC_StartEngine_BUTTON, OnStartEngineBUTTON)
	ON_BN_CLICKED(IDC_ShowPanel_BUTTON, OnShowPanelBUTTON)
	ON_BN_CLICKED(IDC_HidePanel_BUTTON, OnHidePanelBUTTON)
	ON_BN_CLICKED(IDC_ShowVideo_BUTTON, OnShowVideoBUTTON)
	ON_BN_CLICKED(IDC_StartGrab_BUTTON, OnStartGrabBUTTON)
	ON_BN_CLICKED(IDC_StopGrab_BUTTON, OnStopGrabBUTTON)
	ON_BN_CLICKED(IDC_GetSetting_BUTTON, OnGetSettingBUTTON)
	ON_BN_CLICKED(IDC_StopEngine_BUTTON, OnStopEngineBUTTON)
	ON_BN_CLICKED(IDC_UninitailDevice_BUTTON, OnUninitailDeviceBUTTON)
	ON_BN_CLICKED(IDC_SetResolution_BUTTON, OnSetResolutionBUTTON)
	ON_BN_CLICKED(IDC_SetStart_BUTTON, OnSetStartBUTTON)
	ON_BN_CLICKED(IDC_SetGains_BUTTON, OnSetGainsBUTTON)
	ON_BN_CLICKED(IDC_SetExposure_BUTTON, OnSetExposureBUTTON)
	ON_BN_CLICKED(IDC_SetGamma_BUTTON, OnSetGammaBUTTON)
	ON_BN_CLICKED(IDC_SetShow_BUTTON, OnSetShowBUTTON)
	ON_BN_CLICKED(IDC_GetLastBMP_BUTTON, OnGetLastBMPBUTTON)
	ON_BN_CLICKED(IDC_SaveFiles_BUTTON, OnSaveFilesBUTTON)
	ON_BN_CLICKED(IDC_GetBuffer_BUTTON, OnGetBufferBUTTON)
	ON_BN_CLICKED(IDC_SETTRIGGER_BUTTON, OnSettriggerButton)
	ON_BN_CLICKED(IDC_WAITTRIGGER_BUTTON, OnWaittriggerButton)
	ON_BN_CLICKED(IDC_SETVIDEO_BUTTON, OnSetvideoButton)
	ON_BN_CLICKED(IDC_IOCONFIG_BUTTON, OnIoconfigButton)
	ON_BN_CLICKED(IDC_IOOUTPUT_BUTTON, OnIooutputButton)
	ON_BN_CLICKED(IDC_FRAMECALLBACK_BUTTON, OnFramecallbackButton)
	ON_BN_CLICKED(IDC_FRAMECALLBACK_BUTTON2, OnFramecallbackButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnDisplayVideoWindow)
	ON_BN_CLICKED(IDC_BUTTON2, OnHideVideoWindow)
	ON_BN_CLICKED(IDC_DEVICECALLBACK_BUTTON3, OnDevicecallbackButton3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVC_ExampleDlg message handlers
void DemoCallBack( int ImageSequenceNo, char *FileName )
{
	// Note: Don't block this function...as it's called by Camera Engine!!!
	((CVC_ExampleDlg *)theApp.m_pMainWnd)->SetCallBackStatus( 1, FileName );
}

void FrameCallBack( int FrameType, int Row, int Col, 
				    TImageAttachData* Attributes, unsigned char *Frameptr )
{
	unsigned char * srcPtr, * destPtr;
	int i, size;

	//PostMessage( theApp.m_pMainWnd->m_hWnd,WM_FRAMECALLBACK, 0, (LPARAM)Attributes->Reserved1 );
	//SendMessage( theApp.m_pMainWnd->m_hWnd,WM_FRAMECALLBACK, 0, (LPARAM)Attributes->Reserved1 );
	//((CVC_ExampleDlg *)theApp.m_pMainWnd)->SetCallBackStatus( 2,  charbuf );
	// From Firmware V1.1.3, we have frame time stamp in Reserved1, it's
	// 0 -- 65535ms (round back)
	FrameTimeStamp[FrameCount++] = Attributes->Reserved1;
	if ( FrameCount == 200 )
	{
		FrameCount = 0; // For set a breakpoint here to observe the time stamp.
	}

	/*
	if ( Attributes->TriggerOccurred )
		Sleep(0);
	else
		Sleep(10);
	*/
	// JTZ: put the image frame into Queue, note that it might ignore the new frame
	// if the queue is full...which implies that the main GUI thread is slower than
	// it in image processing.
	if ( QueueSize < QUEUE_SIZE )
	{
		// We assume FrameType = 1 (BMP).
		size = Row * Col * 3;
		srcPtr = Frameptr;
		destPtr = ImageQueuePtr[QueueHead];
		memcpy( destPtr, srcPtr, size);
		/*
		for ( i=0; i<size; i++ )
		{
			*destPtr++ = *srcPtr++;		
		}
		*/
		EnterCriticalSection( &QueueLock );
		QueueSize++;
		QueueHead = (QueueHead+1) % QUEUE_SIZE;
		LeaveCriticalSection( &QueueLock );
		// Notify the main GUI thread
		PostMessage( theApp.m_pMainWnd->m_hWnd,WM_FRAMECALLBACK, 0, (LPARAM)Attributes->Reserved1 );
	}
}

void DeviceNotifactionCallBack( int ErrorNo )
{
	PostMessage( theApp.m_pMainWnd->m_hWnd,WM_DEVICECALLBACK, 0, 0 );		
}

void CVC_ExampleDlg::SetCallBackStatus( int SeqNo, char *ShowMessage )
{
	if (SeqNo == 1)
		SetDlgItemText( IDC_CALLBACK_STATIC, CString( ShowMessage ) );
	else
		SetDlgItemText( IDC_CALLBACK2_STATIC, CString( ShowMessage ) );
}

BOOL CVC_ExampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVC_ExampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CVC_ExampleDlg::OnFrameCallBackMessage(WPARAM wParam, LPARAM lParam)
{
	int i, size;
	unsigned int AveragePixelValue;
	unsigned char *srcPtr;
	char charbuf[32];

	//sprintf( charbuf, "%d", (int)lParam );
	//((CVC_ExampleDlg *)theApp.m_pMainWnd)->SetCallBackStatus( 2,  charbuf );
	/*
	if ( lParam % 2 == 0 ) 
	{
		// New Gains
		ImageCtl.GreenGain = 32; // 4.0
		ImageCtl.BlueGain = 12; // 1.5
		ImageCtl.RedGain = 16; // 2
		MTUSB_SetGain( DevHandle, &ImageCtl );
		// New ET
		ImageCtl.MaxExposureTimeIndex = 2;  // 100ms
		ImageCtl.ExposureTime = 15000;      // 15000us
		MTUSB_SetExposureTime( DevHandle, &ImageCtl );			
	}
	else
	{
		// New Gains
		ImageCtl.GreenGain = 24; // 4.0
		ImageCtl.BlueGain = 9; // 1.5
		ImageCtl.RedGain = 12; // 2
		MTUSB_SetGain( DevHandle, &ImageCtl );	
		// New ET
		ImageCtl.MaxExposureTimeIndex = 2;  // 100ms
		ImageCtl.ExposureTime = 25000;      // 7000us
		MTUSB_SetExposureTime( DevHandle, &ImageCtl );	
	}
	*/
	// memory allocation for DIB, from Boris, this is NOt recommended
	/*
     unsigned char * Ptr = new unsigned char[2048*1536*3+256];
     if (Ptr)
     {
          if (MTUSB_GetCurrentFrame(DevHandle, 1, Ptr)==-1) 
			MessageBeep(MB_ICONASTERISK); //Always returns False
          delete [] Ptr;
     }
	*/
	if ( QueueSize > 0 ) // we have frames in Queue, processing it.
	{
		AveragePixelValue = 0;
		size = 2048*1536*3; // we hard code it, user might get proper Row/Column
		                    // according to the setting resolution.
		srcPtr = ImageQueuePtr[QueueTail];
		for (i=0;i<size;i++)
			AveragePixelValue += *srcPtr++;
		AveragePixelValue = AveragePixelValue / size; // Get Average.
		EnterCriticalSection( &QueueLock );
		QueueSize--;
		QueueTail = (QueueTail+1) % QUEUE_SIZE;
		LeaveCriticalSection( &QueueLock );

		sprintf( charbuf, "%d", AveragePixelValue );
		((CVC_ExampleDlg *)theApp.m_pMainWnd)->SetCallBackStatus( 3,  charbuf );
	}
}

void CVC_ExampleDlg::OnDeviceCallBackMessage(WPARAM wParam, LPARAM lParam)
{
	OnStopEngineBUTTON();
	OnUninitailDeviceBUTTON();

	OnDeviceinitButton();
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVC_ExampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CVC_ExampleDlg::OnTimer(UINT nIDEvent) 
{
    if ( (nIDEvent == GPIO_TIMER_ID) && ( IsGPIOConfigDone ) )
	{
		TimerCount++;
		if ( IsSetOutput )
		{
			OutputByte = TimerCount & 0x0f;
			MTUSB_SetGPIOInOut( DevHandle, OutputByte, &InputByte );
			IsSetOutput = false;
		}
		else
		{
			MTUSB_SetGPIOInOut( DevHandle, OutputByte, &InputByte );
			IsSetOutput = true;
			if ( (InputByte&0x0F) != (OutputByte&0x0F) ) // Only last 4 bits are used.
				MessageBeep(MB_ICONASTERISK);  // Beep!!!
		}
	}
	CDialog::OnTimer(nIDEvent);
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVC_ExampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVC_ExampleDlg::OnOK() 
{
	// TODO: Add extra validation here
	// Clean Up
	CDialog::OnOK();
}

void CVC_ExampleDlg::OnDeviceinitButton() 
{
	int i,TotalDevices;
	CString MyStr;

	// TODO: Add your control notification handler code here
	TotalDevices = MTUSB_InitDevice();
	MyStr.Format( "There are %d devices.", TotalDevices );
	MessageBox( MyStr );

	SetTimer( GPIO_TIMER_ID, GPIO_TIMER_MS, NULL );
	IsGPIOConfigDone = false;
	IsSetOutput = true;

	InitializeCriticalSection( &QueueLock );
	// JTZ: We might create a Image Queue here
	if ( TotalDevices >= 1 )
	{
		for ( i=0; i<QUEUE_SIZE; i++)
		{
			ImageQueuePtr[i] = new unsigned char[2048*1536*3+256];
			// We assume we have enough resources, no error check here.
		}
		QueueHead = QueueTail = QueueSize = 0;
	}
	else
	{
		for ( i=0; i<QUEUE_SIZE; i++)
		{
			ImageQueuePtr[i] = NULL;
		}			
	}
}


void CVC_ExampleDlg::OnUninitailDeviceBUTTON() 
{
	int i;
	// TODO: Add your control notification handler code here
	MTUSB_UnInitDevice();
	KillTimer( GPIO_TIMER_ID );
	DeleteCriticalSection( &QueueLock );
	for ( i=0; i<QUEUE_SIZE; i++)
	{
		if ( ImageQueuePtr[i] )
			delete ImageQueuePtr[i];
	}
}

void CVC_ExampleDlg::OnDeviceopenButton() 
{
	// TODO: Add your control notification handler code here
	DevHandle = MTUSB_OpenDevice(0);
	if ( DevHandle == 0xFFFFFFFF)
	{
		MessageBox( "Device Open Fail!" );	
	}
}

void CVC_ExampleDlg::OnGetModuleBUTTON() 
{
	char ModuleNo[32];

	// TODO: Add your control notification handler code here
	if ( MTUSB_GetModuleNo( DevHandle, ModuleNo ) == 1 )
	{
		SetDlgItemText(IDC_ModuleNo_STATIC, CString( ModuleNo ) );
	}
}

void CVC_ExampleDlg::OnGetSerialNoBUTTON() 
{
	char SerialNo[32];

	// TODO: Add your control notification handler code here
	if ( MTUSB_GetSerialNo( DevHandle, SerialNo ) == 1 )
	{
		SetDlgItemText(IDC_SerialNo_STATIC, CString( SerialNo ) );
	}
	
}

void CVC_ExampleDlg::OnStartEngineBUTTON() 
{
	// TODO: Add your control notification handler code here
	MTUSB_StartCameraEngine( m_hWnd, DevHandle );
	MTUSB_SetGPIOConifg( DevHandle, 0x0F ); // Pin1, 2, 3 and 4 as Output.
	IsGPIOConfigDone = true; // All output.
}

void CVC_ExampleDlg::OnStopEngineBUTTON() 
{
	// TODO: Add your control notification handler code here
	MTUSB_StopCameraEngine( DevHandle );
	// Don't let the Timer run...as engine was stoped already.
	IsGPIOConfigDone = false; 
}

void CVC_ExampleDlg::OnShowPanelBUTTON() 
{
	// TODO: Add your control notification handler code here
	MTUSB_ShowFrameControlPanel( DevHandle, TRUE, FALSE, "VC++ Example", 8, 8 );
}

void CVC_ExampleDlg::OnHidePanelBUTTON() 
{
	// TODO: Add your control notification handler code here
	MTUSB_HideFrameControlPanel( DevHandle );
}

void CVC_ExampleDlg::OnShowVideoBUTTON() 
{
	// TODO: Add your control notification handler code here
	MTUSB_ShowVideoWindow( DevHandle, 100, 100 , 300, 400 );
}

void CVC_ExampleDlg::OnStartGrabBUTTON() 
{
	// TODO: Add your control notification handler code here
	MTUSB_StartFrameGrab( DevHandle, 0x8888 );
}

void CVC_ExampleDlg::OnStopGrabBUTTON() 
{
	// TODO: Add your control notification handler code here
	MTUSB_StopFrameGrab( DevHandle );
}

void CVC_ExampleDlg::OnGetSettingBUTTON() 
{
	// TODO: Add your control notification handler code here
	MTUSB_GetFrameSetting( DevHandle, &ImageCtl );
}

void CVC_ExampleDlg::OnSetResolutionBUTTON() 
{
	// TODO: Add your control notification handler code here
	ImageCtl.Resolution = 9; // 2048x1536
	ImageCtl.BinMode = 1; // Not 1:2 Decimation Mode
	ImageCtl.ImageRendorFitWindow = TRUE;
	MTUSB_SetResolution( DevHandle, &ImageCtl );		
}

void CVC_ExampleDlg::OnSetStartBUTTON() 
{
	// TODO: Add your control notification handler code here
	ImageCtl.XStart = 64;
	ImageCtl.YStart = 64;
	MTUSB_SetStartPosition( DevHandle, &ImageCtl );
}

void CVC_ExampleDlg::OnSetGainsBUTTON() 
{
	// TODO: Add your control notification handler code here
	ImageCtl.GreenGain = 32; // 4.0
	ImageCtl.BlueGain = 12; // 1.5
	ImageCtl.RedGain = 16; // 2
	MTUSB_SetGain( DevHandle, &ImageCtl );	
}

void CVC_ExampleDlg::OnSetExposureBUTTON() 
{
	// TODO: Add your control notification handler code here
	ImageCtl.MaxExposureTimeIndex = 2;  // 100ms
	ImageCtl.ExposureTime = 15000;      // 15000us
	MTUSB_SetExposureTime( DevHandle, &ImageCtl );	
}

void CVC_ExampleDlg::OnSetGammaBUTTON() 
{
	// TODO: Add your control notification handler code here
	ImageCtl.Gamma = 12; // 1.2
	ImageCtl.Contrast = 58; // 58%
	ImageCtl.Bright = 66; // 66%
	ImageCtl.SharpLevel = 1; // Sharp
	MTUSB_SetGammaValue( DevHandle, &ImageCtl );
}

void CVC_ExampleDlg::OnSetShowBUTTON() 
{
	// TODO: Add your control notification handler code here
	ImageCtl.BWMode = TRUE;
	ImageCtl.HorizontalMirror = FALSE;
	ImageCtl.VerticalFlip = TRUE;
	MTUSB_SetShowMode( DevHandle, &ImageCtl );	
}

void CVC_ExampleDlg::OnGetLastBMPBUTTON() 
{
	// TODO: Add your control notification handler code here
	MTUSB_GetLastBMPFrame( DevHandle, "c:\\Example.bmp" );	
}

void CVC_ExampleDlg::OnSaveFilesBUTTON() 
{
	// TODO: Add your control notification handler code here
	ImageCtl.IsCatchRAW = FALSE;
	ImageCtl.IsCatchJPEG = TRUE;
	ImageCtl.IsRawGraph = FALSE;
	ImageCtl.CatchIgnoreSkip = TRUE;
	ImageCtl.CatchFrames = 3;
	ImageCtl.IsAverageFrame = FALSE;
	MTUSB_SaveFramesToFiles( DevHandle, &ImageCtl, "c:", "image" );	
}

void CVC_ExampleDlg::OnGetBufferBUTTON() 
{
	unsigned char *BytePtr;
	unsigned char *PropertyPtr;
	TImageAttachData ImageProperty;
	
	// TODO: Add your control notification handler code here
	MTUSB_GetFrameSetting( DevHandle, &ImageCtl );

	BytePtr = (unsigned char *)malloc( 0x400000 );
	PropertyPtr = (unsigned char *)&ImageProperty.XStart;
	//if ( MTUSB_GetCurrentFrame( DevHandle, 0, BytePtr ) != 1 )
	if ( MTUSB_GetCurrentFrame2( DevHandle, 0, BytePtr, PropertyPtr) != 1 )
	{
		MessageBox( "Get Current Frame Error!", "Error", MB_OK );
	}

	//MTUSB_GetCurrentFrame( DevHandle, 1, BytePtr );
	free( (void *)BytePtr );
}

void CVC_ExampleDlg::OnSettriggerButton() 
{
	// TODO: Add your control notification handler code here
	MTUSB_SetCameraWorkMode( DevHandle, 1 );
}

void CVC_ExampleDlg::OnWaittriggerButton() 
{
	// TODO: Add your control notification handler code here
    
    MTUSB_SetExternalParameters( DevHandle, TRUE, TRUE, TRUE, "c:", "DoNotSaveFile" );  
	MTUSB_WaitingExternalTrigger( DevHandle, TRUE,  DemoCallBack );
	// Using  MTUSB_WaitingExternalTrigger( DevHandle, TRUE,  NULL )
	// to start waiting, but without install a callback....this might
	// be needed when user installed a framecallback already...and
	// don't want to save the image to a file.
	// MTUSB_WaitingExternalTrigger( DevHandle, TRUE,  NULL );

	// Using MTUSB_WaitingExternalTrigger( DevHandle, FALSE, NULL );
	// to notify the engine to abort the waiting of external trigger.
}

void CVC_ExampleDlg::OnSetvideoButton() 
{
	// TODO: Add your control notification handler code here
	MTUSB_SetCameraWorkMode( DevHandle, 0 );
}

void CVC_ExampleDlg::OnIoconfigButton() 
{
	// TODO: Add your control notification handler code here
	//MTUSB_SetGPIOConifg( DevHandle, 0x03 ); // Pin1, 2 as Output, Pin3, 4 as input
	MTUSB_SetGPIOConifg( DevHandle, 0x0F ); // Pin1, 2, 3 and 4 as Output.
}

void CVC_ExampleDlg::OnIooutputButton() 
{
	unsigned char RcvByte;

	// TODO: Add your control notification handler code here
	MTUSB_SetGPIOInOut( DevHandle, 0x01, &RcvByte );   // High for Pin1, Low for Pin2
	//SetDlgItemText( IDC_CALLBACK_STATIC, CString( RcvByte ) );
}

void CVC_ExampleDlg::OnFramecallbackButton() 
{
	// TODO: Add your control notification handler code here
	FrameCount = 0;
	MTUSB_GetFrameSetting( DevHandle, &ImageCtl );
	MTUSB_InstallFrameHooker( DevHandle, FALSE, 1, FrameCallBack );
}

void CVC_ExampleDlg::OnFramecallbackButton2() 
{
	// TODO: Add your control notification handler code here
	MTUSB_InstallFrameHooker( DevHandle, FALSE, 1, NULL );	
}



void CVC_ExampleDlg::OnDisplayVideoWindow() 
{
	// TODO: Add your control notification handler code here
	MTUSB_DisplayVideoWindow( 1 );
}

void CVC_ExampleDlg::OnHideVideoWindow() 
{
	// TODO: Add your control notification handler code here
	MTUSB_DisplayVideoWindow( 0 );
}

void CVC_ExampleDlg::OnDevicecallbackButton3() 
{
	// TODO: Add your control notification handler code here
	MTUSB_InstallDeviceHooker( DeviceNotifactionCallBack );	
}
