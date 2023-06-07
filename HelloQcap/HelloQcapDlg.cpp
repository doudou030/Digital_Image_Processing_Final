
// HelloQcapDlg.cpp : implementation file
//


#include "pch.h"
#include "framework.h"
#include "stdafx.h"
#include "HelloQcap.h"
#include "HelloQcapDlg.h"
#include "afxdialogex.h"

#include <time.h>
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CHelloQcapDlg* m_MainDLG = NULL;



VOID Dbg(CHAR* pszStr, ...)
{
	char szData[MAX_PATH] = "[SC] ";

	va_list args;

	va_start(args, pszStr);

	vsnprintf(szData + 5, sizeof(szData) - 1, pszStr, args);

	va_end(args);

	strcat_s(szData, "\n");

	OutputDebugString(szData);
}


//
void display(PVOID pDevice, QDEEP_OBJECT_DETECT_BOUNDING_BOX* DetectBOX, ULONG DetectLen, PVOID pUserData)
{
	CHelloQcapDlg* m_MainDLG = (CHelloQcapDlg*)pUserData;

	for (ULONG i = 1; i <= m_MainDLG->m_nOSDTemp; i++)
	{
		QCAP_SET_OSD_BUFFER(pDevice, i, 0, 0, 0, 0, QCAP_COLORSPACE_TYEP_ARGB32, NULL, 0, 0, 0, 0);
	}	

	static UINT count = 1;

	for (ULONG i = 0; i < DetectLen; i++)
	{
		ULONG    ID = DetectBOX[i].nObjectID;

		ULONG    x = DetectBOX[i].nX;

		ULONG    y = DetectBOX[i].nY;

		ULONG   cx = DetectBOX[i].nWidth;

		ULONG   cy = DetectBOX[i].nHeight;

		HBITMAP hMemoryBitmap = CreateCompatibleBitmap(m_MainDLG->m_hScreenDC, cx, cy);

		BYTE* pMemoryBuffer = (BYTE*)malloc(cx * cy * 4);

		SelectObject(m_MainDLG->m_hMemoryDC, hMemoryBitmap);

		Gdiplus::Graphics graphics(m_MainDLG->m_hMemoryDC);

		Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 255), 6.0f);

		WCHAR* strType[] = { L"ManFace", L"WomanFace" };

		DWORD colorForClass[] = { 0xFF0000FF ,  0xFFFF0000 };

		pen.SetColor(colorForClass[ID]);

		graphics.DrawRectangle(&pen, 0, 0, cx, cy);

		//info for object
		WCHAR	pszText[100] = L"";

		swprintf_s(pszText,  L"%s %d", strType[i], DetectBOX[i].nObjectID);

		//info for object (GDI+)
		Gdiplus::FontFamily o_font_family(L"Arial");

		Gdiplus::Font font(&o_font_family, 20, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

		graphics.DrawString(pszText, -1, &font, Gdiplus::PointF(5, 5), &Gdiplus::SolidBrush(colorForClass[ID]));

		GetBitmapBits(hMemoryBitmap, cx * cy * 4, pMemoryBuffer);

		QCAP_SET_OSD_BUFFER(pDevice, i + 1, x, y, 0, 0, QCAP_COLORSPACE_TYEP_ABGR32, pMemoryBuffer, cx, cy, cx * 4, 255, 0xFFFFFFFF, 0, 2, 1, 22, NULL, QCAP_SEQUENCE_STYLE_FOREMOST);

		if (pMemoryBuffer) {

			free(pMemoryBuffer);

			pMemoryBuffer = NULL;
		}
		if (hMemoryBitmap) {

			DeleteObject(hMemoryBitmap);

			hMemoryBitmap = NULL;
		}

		m_MainDLG->m_nOSDTemp = i + 1;
	}

	count++;
}

QRETURN on_format_changed_callback(PVOID pDevice , ULONG nVideoInput , ULONG nAudioInput , ULONG nVideoWidth , ULONG nVideoHeight , BOOL bVideoIsInterleaved , double dVideoFrameRate , ULONG nAudioChannels, ULONG nAudioBitsPerSample, ULONG nAudioSampleFrequency, PVOID pUserData)
{
	CHelloQcapDlg *m_MainDLG = (CHelloQcapDlg*)pUserData;

	CString m_strfomatoutput;

	struct tm* local;
	time_t t;
	t = time(NULL);
	local = localtime(&t);

	m_strfomatoutput.Format("input window size \n %d*%d \n Today is \n %d/%d/%d ", nVideoWidth, nVideoHeight, local->tm_year + 1900, local->tm_mon + 1, local-> tm_mday);

	m_MainDLG -> m_inputinfo.SetWindowTextA((LPCSTR)m_strfomatoutput);


	//Dbg("width is %d and height is %d", nVideoWidth, nVideoHeight);

	m_MainDLG->m_VideoFrameRate = dVideoFrameRate;

	m_MainDLG->m_VideoW = nVideoWidth;

	m_MainDLG->m_VideoH = nVideoHeight;

	m_MainDLG->m_AudioChannels = nAudioChannels;
	
	m_MainDLG->m_AudioBitSample = nAudioBitsPerSample;
	
	m_MainDLG->m_AudioFreq = nAudioSampleFrequency;

	return QCAP_RT_OK;
}

//沒訊號的callback
QRETURN on_no_signal_detect_callback(PVOID pDevice , ULONG nVideoInput, ULONG nAudioInput, PVOID pUserData)
{

	CHelloQcapDlg* m_MainDLG = (CHelloQcapDlg*)pUserData;

	CoInitialize(NULL);

	CString m_strfomatoutput;

	m_strfomatoutput.Format("No signal!!!(no)");

	m_MainDLG->m_inputinfo.SetWindowTextA((LPCSTR)m_strfomatoutput);

	QCAP_SET_OSD_PICTURE(m_MainDLG->M_Pdevice, 1, 0, 0, 1920, 1080, "nosignal.jpg", 255, QCAP_SEQUENCE_STYLE_FOREMOST, 0.0);

	return QCAP_RT_OK;
}


//訊號被移除的callback
QRETURN on_no_signal_removed_callback(PVOID pDevice , ULONG nVideoInput , ULONG nAudioInput , PVOID pUserData)
{
	CHelloQcapDlg* m_MainDLG = (CHelloQcapDlg*)pUserData;

	CoInitialize(NULL);

	CString m_strfomatoutput;

	m_strfomatoutput.Format("No signal!!!");

	m_MainDLG->m_inputinfo.SetWindowTextA((LPCSTR)m_strfomatoutput);

	QCAP_SET_OSD_PICTURE(m_MainDLG->M_Pdevice, 1, 0, 0, 1920, 1080, "nosignal.jpg", 255, QCAP_SEQUENCE_STYLE_FOREMOST, 0.2);

	return QCAP_RT_OK;
}

QRETURN on_process_preview_video_buffer(PVOID pDevice, double dSampleTime, BYTE* pFrameBuffer, ULONG nFrameBufferLen, PVOID pUserData)
{
	CHelloQcapDlg* m_MainDLG = (CHelloQcapDlg*)pUserData;

	if (nFrameBufferLen > 0)
	{
		m_MainDLG->m_pObjectSize = 1000;

		QDEEP_SET_VIDEO_OBJECT_DETECT_UNCOMPRESSION_BUFFER(m_MainDLG->m_pDetector, QCAP_COLORSPACE_TYPE_YUY2, m_MainDLG->m_VideoW, m_MainDLG->m_VideoH, pFrameBuffer, nFrameBufferLen, m_MainDLG->m_pObjectList, &m_MainDLG ->m_pObjectSize);

		display(pDevice, m_MainDLG->m_pObjectList, m_MainDLG->m_pObjectSize, m_MainDLG);
	}

	if ((m_MainDLG->ISSTARTRTSP == true) && pFrameBuffer > 0)
	{
		QCAP_SET_VIDEO_BROADCAST_SERVER_UNCOMPRESSION_BUFFER(m_MainDLG->m_pRTSPSERVER, 0, QCAP_COLORSPACE_TYPE_YUY2, m_MainDLG->m_VideoW, m_MainDLG->m_VideoH, pFrameBuffer, nFrameBufferLen);
	}

	if ((m_MainDLG->ISSTARTRTMP == true) && pFrameBuffer > 0)
	{
		QCAP_SET_VIDEO_BROADCAST_SERVER_UNCOMPRESSION_BUFFER(m_MainDLG->m_pRTMPSERVER, 0, QCAP_COLORSPACE_TYPE_YUY2, m_MainDLG->m_VideoW, m_MainDLG->m_VideoH, pFrameBuffer, nFrameBufferLen);
	}

	struct tm* local;
	time_t t;
	t = time(NULL);
	local = localtime(&t);

	char* now_time;
	CString latest_time;
	ULONG BoundaryWidth, BoundaryHeight;

	latest_time.Format("%d/%d/%d  %d:%d:%d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);

	now_time = (LPSTR)(LPCTSTR)latest_time;

	QCAP_GET_OSD_TEXT_BOUNDARY(pDevice, 0,
		now_time, "Arial",
		QCAP_FONT_STYLE_BOLD, 72,
		&BoundaryWidth,
		&BoundaryHeight);

	QCAP_SET_OSD_TEXT(pDevice, 1, 1350, 10, BoundaryWidth, BoundaryHeight, now_time, "Arial", QCAP_FONT_STYLE_BOLD, 36, 0xFFFFFF00, 0X00000000, 255, 0, 0, QCAP_SEQUENCE_STYLE_FOREMOST, 0);


	return QCAP_RT_OK;
}

QRETURN video_preview_callback(PVOID pDevice , double dSampleTime , BYTE* pFrameBuffer , ULONG nFrameBufferLen ,PVOID pUserData )
{
	CHelloQcapDlg* m_MainDLG = (CHelloQcapDlg*)pUserData;
	
	if ((m_MainDLG->ISSTARTRTSP == true) && pFrameBuffer > 0)
	{
		QCAP_SET_VIDEO_BROADCAST_SERVER_UNCOMPRESSION_BUFFER(m_MainDLG->m_pRTSPSERVER, 0, QCAP_COLORSPACE_TYPE_YUY2, m_MainDLG->m_VideoW, m_MainDLG->m_VideoH, pFrameBuffer, nFrameBufferLen);
	}
	
	if ((m_MainDLG->ISSTARTRTMP == true) && pFrameBuffer > 0)
	{
		QCAP_SET_VIDEO_BROADCAST_SERVER_UNCOMPRESSION_BUFFER(m_MainDLG->m_pRTMPSERVER, 0, QCAP_COLORSPACE_TYPE_YUY2, m_MainDLG->m_VideoW, m_MainDLG->m_VideoH, pFrameBuffer, nFrameBufferLen);
	}

	return QCAP_RT_OK;
}



QRETURN audio_preview_callback(PVOID pDevice, double dSampleTime, BYTE* pFrameBuffer, ULONG nFrameBufferLen, PVOID pUserData)
{
	CHelloQcapDlg* m_MainDLG = (CHelloQcapDlg*)pUserData;

	if ((m_MainDLG->ISSTARTRTSP == true) && pFrameBuffer > 0)
	{
		QCAP_SET_AUDIO_BROADCAST_SERVER_UNCOMPRESSION_BUFFER(m_MainDLG->m_pRTSPSERVER, 0, pFrameBuffer, nFrameBufferLen);
	}
	
	if ((m_MainDLG->ISSTARTRTMP == true) && pFrameBuffer > 0)
	{
		QCAP_SET_AUDIO_BROADCAST_SERVER_UNCOMPRESSION_BUFFER(m_MainDLG->m_pRTMPSERVER, 0,  pFrameBuffer, nFrameBufferLen);
	}

	return QCAP_RT_OK;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHelloQcapDlg dialog
CHelloQcapDlg::CHelloQcapDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_HELLOQCAP_DIALOG, pParent)
	, m_input(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHelloQcapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_WINDOW, m_PreviewWindow);
	DDX_Control(pDX, IDC_STATIC_INFO, m_inputinfo);
	DDX_Control(pDX, IDC_BOTTON_1, m_button1);
	DDX_Control(pDX, IDC_BOTTON_2, m_button2);
	DDX_Control(pDX, IDC_BOTTON_3, m_button3);
	DDX_Control(pDX, IDC_BOTTON_4, m_button4);
	DDX_Control(pDX, IDC_BOTTON_5, m_button5);
	DDX_Control(pDX, IDC_BOTTON_6, m_button6);
	DDX_Control(pDX, IDC_BOTTON_7, m_button7);
	DDX_Control(pDX, IDC_BOTTON_8, m_button8);


	DDX_Control(pDX, IDC_COMBO1, v_combobox1);
	DDX_Text(pDX, IDC_EDIT1, m_input);
	DDX_Control(pDX, IDC_COMBO2, m_combox2);
	DDX_Control(pDX, IDC_COMBO3, m_combox3);
}

BEGIN_MESSAGE_MAP(CHelloQcapDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BOTTON_1, &CHelloQcapDlg::OnBnClickedBotton1)
	ON_BN_CLICKED(IDC_BOTTON_2, &CHelloQcapDlg::OnBnClickedBotton2)
	ON_BN_CLICKED(IDC_BOTTON_3, &CHelloQcapDlg::OnBnClickedBotton3)
	ON_BN_CLICKED(IDC_BOTTON_4, &CHelloQcapDlg::OnBnClickedBotton4)
	ON_BN_CLICKED(IDC_BOTTON_5, &CHelloQcapDlg::OnBnClickedBotton5)
	ON_BN_CLICKED(IDC_BOTTON_6, &CHelloQcapDlg::OnBnClickedBotton6)
	ON_BN_CLICKED(IDC_BOTTON_7, &CHelloQcapDlg::OnBnClickedBotton7)
	ON_BN_CLICKED(IDC_BOTTON_8, &CHelloQcapDlg::OnBnClickedBotton8)

	
	ON_CBN_SELCHANGE(IDC_COMBO1, &CHelloQcapDlg::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_EDIT1, &CHelloQcapDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CHelloQcapDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CHelloQcapDlg message handlers

BOOL CHelloQcapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	
	v_combobox1.AddString(_T("RTMP"));//0
	v_combobox1.AddString(_T("RTSP"));//1
	v_combobox1.SetCurSel(0);

	m_combox2.AddString(_T("1920 * 1080"));//3
	m_combox2.AddString(_T("1600 * 900"));//2
	m_combox2.AddString(_T("1280 * 720"));//1
	m_combox2.AddString(_T("1024 * 576"));//0
	m_combox2.SetCurSel(3);

	m_combox3.AddString(_T("60"));//2
	m_combox3.AddString(_T("50"));//1
	m_combox3.AddString(_T("30"));//0
	m_combox3.SetCurSel(2);

	int choose_stream = v_combobox1.GetCurSel();
	int choose_resolution = m_combox2.GetCurSel();
	int choose_frame = m_combox3.GetCurSel();

	//以下是QCAP及QDEEP

	CoInitialize(NULL);

	M_Pdevice = NULL;
	
	m_pRTSPSERVER = NULL;

	m_pRTMPSERVER = NULL;

	m_VideoW = 0;

	m_VideoH = 0;

	m_AudioChannels = 0;

	m_AudioBitSample = 0;

	m_AudioFreq = 0;

	ISSTARTRTSP = false;

	/// 0513 DEEP

	m_nFrameSkip = 1;

	m_nOSDTemp = 0;

	m_pDetector = NULL;

	m_pObjectSize = 1000;

	m_pObjectList = new QDEEP_OBJECT_DETECT_BOUNDING_BOX[m_pObjectSize];

	m_hScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);

	m_hMemoryDC = CreateCompatibleDC(m_hScreenDC);

	QCAP_CREATE("UB3300 USB", 0, m_PreviewWindow.m_hWnd, &M_Pdevice, TRUE);

	/////////////沒訊號/////////////
	if (M_Pdevice == 0) 
	{ 

	QCAP_REGISTER_NO_SIGNAL_DETECTED_CALLBACK(M_Pdevice, on_no_signal_detect_callback, this);

	}
	/////////////沒訊號/////////////

	QDEEP_CREATE_OBJECT_DETECT(QDEEP_GPU_TYPE_INTEL_CPU, 0, QDEEP_OBJECT_DETECT_CONFIG_MODEL_FACE_HEAD_BODY, "QDEEP.OD.FACE.HEAD.BODY.CFG", &m_pDetector, QDEEP_OBJECT_DETECT_FLAG_TRAJECTORY_TRACKING);

	QDEEP_START_OBJECT_DETECT(m_pDetector);

	QCAP_REGISTER_SIGNAL_REMOVED_CALLBACK(M_Pdevice, on_no_signal_removed_callback, this);

	QCAP_REGISTER_FORMAT_CHANGED_CALLBACK(M_Pdevice, on_format_changed_callback, this);

	//on_process_preview_video_buffer;video_preview_callback 前面有影像偵測

	QCAP_REGISTER_VIDEO_PREVIEW_CALLBACK(M_Pdevice, on_process_preview_video_buffer, this);

	//on_process_preview_audio_buffer;audio_preview_callback

	QCAP_REGISTER_AUDIO_PREVIEW_CALLBACK(M_Pdevice, audio_preview_callback, this);

	QCAP_RUN(M_Pdevice);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHelloQcapDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHelloQcapDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHelloQcapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//關掉時需要全部關掉
void CHelloQcapDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here\

	if (M_Pdevice != NULL)
	{
		QCAP_STOP(M_Pdevice);

		QCAP_DESTROY(M_Pdevice);

		M_Pdevice = NULL;
	}

	if (ISSTARTRTSP != true)
	{
		return;
	}

	if (m_pObjectList != NULL)
	{
		delete[] m_pObjectList;

		m_pObjectList = NULL;
	}

	if (m_pDetector != NULL)
	{
		QDEEP_STOP_OBJECT_DETECT(m_pDetector);

		QDEEP_DESTROY_OBJECT_DETECT(m_pDetector);

		m_pDetector = NULL;
	}

	if (m_hMemoryDC)
	{
		DeleteDC(m_hMemoryDC);

		m_hMemoryDC = NULL;
	}

	if (m_hScreenDC)
	{
		DeleteDC(m_hScreenDC);

		m_hScreenDC = NULL;
	}

	QCAP_STOP_BROADCAST_SERVER(m_pRTSPSERVER);

	QCAP_DESTROY_BROADCAST_SERVER(m_pRTSPSERVER);

	QCAP_STOP_BROADCAST_SERVER(m_pRTMPSERVER);

	QCAP_DESTROY_BROADCAST_SERVER(m_pRTMPSERVER);

	m_pRTSPSERVER = NULL;

	m_pRTMPSERVER = NULL;

	ISSTARTRTSP = false;

	ISSTARTRTMP = false;

	CoUninitialize();
	
}

void CHelloQcapDlg::OnBnClickedBotton1()
{
	// TODO: Add your control notification handler code here
	if (!M_Pdevice) {
		return;
	}
	QCAP_SNAPSHOT_BMP(M_Pdevice, "snapshot.bmp", 1);
}


void CHelloQcapDlg::OnBnClickedBotton2()
{
	// TODO: Add your control notification handler code here
	if (!M_Pdevice) {
		return;
	}
	QCAP_SNAPSHOT_JPG(M_Pdevice, "snapshot.jpg", 1);
}


void CHelloQcapDlg::OnBnClickedBotton3()
{
	// TODO: Add your control notification handler code here
	if (!M_Pdevice) return;
	
	ULONG m_gop = 0;

	if (m_VideoIsInterLeaved == false) 
	{
		m_gop = (ULONG)m_VideoFrameRate / 2;
	}
	else
	{
		m_gop = (ULONG)m_VideoFrameRate / 4;
	}

	QCAP_SET_VIDEO_RECORD_PROPERTY(M_Pdevice, 0, QCAP_ENCODER_TYPE_SOFTWARE, QCAP_ENCODER_FORMAT_H264, QCAP_RECORD_MODE_CBR, 8000, 12 * 1024 * 1024, m_gop, 0, 0, QCAP_DOWNSCALE_MODE_OFF);

	QCAP_SET_AUDIO_RECORD_PROPERTY(M_Pdevice, 0, QCAP_ENCODER_TYPE_SOFTWARE, QCAP_ENCODER_FORMAT_AAC);

	QCAP_START_RECORD(M_Pdevice, 0, "record.mp4");
}


void CHelloQcapDlg::OnBnClickedBotton4()
{
	// TODO: Add your control notification handler code here

	if (!M_Pdevice) {
		return;
	}
	QCAP_STOP_RECORD(M_Pdevice, 0);
}


void CHelloQcapDlg::OnBnClickedBotton5()
{
	// TODO: Add your control notification handler code here
	if (!M_Pdevice) {
		return;
	}
	
	UpdateData(TRUE);
}

//播放影片廣告AD.mp4
void CHelloQcapDlg::OnBnClickedBotton6()
{
	// TODO: Add your control notification handler code here
	if (!M_Pdevice) {
		return;
	}
	QCAP_SET_OSD_PICTURE(M_Pdevice, 1, 1300, 600, 534, 360, "AD.gif", 255, QCAP_SEQUENCE_STYLE_FOREMOST, 10);
}


void CHelloQcapDlg::OnBnClickedBotton7()
{
	// TODO: Add your control notification handler code here

	char* pick_key;
	int pick_frame, pick_resolution;
	double latest_frame;
	char SERVER_TYPE;

	pick_key = (char *)(LPCTSTR)m_input.GetString();
	SERVER_TYPE = v_combobox1.GetCurSel();
	pick_resolution = m_combox2.GetCurSel();
	pick_frame = m_combox3.GetCurSel();
	
	if (pick_resolution == 0)
	{
		m_VideoW = 1024;
		m_VideoH = 576;
	}
	else if (pick_resolution == 1) 
	{
		m_VideoW = 1280;
		m_VideoH = 720;
	}
	else if (pick_resolution == 2)
	{
		m_VideoW = 1600;
		m_VideoH = 900;
	}
	else if (pick_resolution == 3)
	{
		m_VideoW = 1920;
		m_VideoH = 1080;
	}

	if (pick_frame == 0) latest_frame = 30;
	else if (pick_frame == 1) latest_frame = 50;
	else if (pick_frame == 2) latest_frame = 60;

	if (!M_Pdevice)
	{
		return;
	}

	ULONG m_gop = 0;

	DOUBLE m_FPS = latest_frame;

	if (m_VideoIsInterLeaved == false)
	{
		m_gop = (ULONG)m_VideoFrameRate / 2;
	}
	else
	{
		m_gop = (ULONG)m_VideoFrameRate / 4;
	}
	
	if (SERVER_TYPE == 1)
	{
		//RTSP串流方式
		QCAP_CREATE_BROADCAST_RTSP_SERVER(0, 1, &m_pRTSPSERVER, "root", "root", 8556, 8080, 0, 0, 0);

		QCAP_SET_VIDEO_BROADCAST_SERVER_PROPERTY(m_pRTSPSERVER, 0, QCAP_ENCODER_TYPE_SOFTWARE, QCAP_ENCODER_FORMAT_H264,
			QCAP_COLORSPACE_TYPE_YUY2, m_VideoW, m_VideoH, m_FPS, QCAP_RECORD_MODE_CBR, 8000, 6 * 1024 * 1024, m_gop, 0, 0, NULL, 0, 1, QCAP_BROADCAST_FLAG_FULL);

		QCAP_SET_AUDIO_BROADCAST_SERVER_PROPERTY(m_pRTSPSERVER, 0, QCAP_ENCODER_TYPE_SOFTWARE, QCAP_ENCODER_FORMAT_AAC_ADTS, m_AudioChannels, m_AudioBitSample, m_AudioFreq);

		QCAP_START_BROADCAST_SERVER(m_pRTSPSERVER);

		ISSTARTRTSP = true;
	}
	else if (SERVER_TYPE == 0)
	{
		//RTMP串流方式
		//rtmp://a.rtmp.youtube.com/live2/zx8q-0448-7mpp-gmrh-2smt
	
		QCAP_CREATE_BROADCAST_RTMP_WEB_PORTAL_SERVER(0, pick_key/*自己的網址+金鑰*/, &m_pRTMPSERVER, NULL, NULL, 10000);

		QCAP_SET_VIDEO_BROADCAST_SERVER_PROPERTY(m_pRTMPSERVER, 0, QCAP_ENCODER_TYPE_SOFTWARE, QCAP_ENCODER_FORMAT_H264, QCAP_COLORSPACE_TYPE_YV12,
			m_VideoW, m_VideoH, m_FPS, QCAP_RECORD_MODE_CBR, 8000, 4 * 1024 * 1024, 30, 0, 0, NULL, FALSE, FALSE, QCAP_BROADCAST_FLAG_FULL);

		QCAP_SET_AUDIO_BROADCAST_SERVER_PROPERTY(m_pRTMPSERVER, 0, QCAP_ENCODER_TYPE_SOFTWARE, QCAP_ENCODER_FORMAT_AAC_ADTS, 2, 16, 48000);

		QCAP_START_BROADCAST_SERVER(m_pRTMPSERVER, NULL);

		ISSTARTRTMP = true;
	}
}


void CHelloQcapDlg::OnBnClickedBotton8()
{
	// TODO: Add your control notification handler code here

	if (!M_Pdevice) {
		return;
	}
	QCAP_STOP_BROADCAST_SERVER(m_pRTSPSERVER);

	QCAP_DESTROY_BROADCAST_SERVER(m_pRTSPSERVER);

	QCAP_STOP_BROADCAST_SERVER(m_pRTMPSERVER);

	QCAP_DESTROY_BROADCAST_SERVER(m_pRTMPSERVER);

	m_pRTSPSERVER = NULL;

	m_pRTMPSERVER = NULL;

	ISSTARTRTSP = false;

	ISSTARTRTMP = false;
}


void CHelloQcapDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}


void CHelloQcapDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CHelloQcapDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

