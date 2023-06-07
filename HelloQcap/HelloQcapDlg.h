
// HelloQcapDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CHelloQcapDlg dialog
class CHelloQcapDlg : public CDialog
{
// Construction
public:
	CHelloQcapDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HELLOQCAP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();

	CStatic m_PreviewWindow;
	
	CStatic m_inputinfo;
	CButton m_button1;
	CButton m_button2;
	CButton m_button3;
	CButton m_button4;
	CButton m_button5;
	CButton m_button6;
	CButton m_button7;
	CButton m_button8;

	afx_msg void OnBnClickedBotton1();
	afx_msg void OnBnClickedBotton2();
	afx_msg void OnBnClickedBotton3();
	afx_msg void OnBnClickedBotton4();
	afx_msg void OnBnClickedBotton5();
	afx_msg void OnBnClickedBotton6();
	afx_msg void OnBnClickedBotton7();
	afx_msg void OnBnClickedBotton8();

	/// /////////////////////////////////////////////////
	/////QCAP
	PVOID M_Pdevice;

	PVOID m_pRTSPSERVER;

	PVOID m_pRTMPSERVER;
	
	ULONG m_VideoW;

	ULONG m_VideoH;

	DOUBLE  m_VideoFrameRate;

	ULONG m_AudioChannels, m_AudioBitSample, m_AudioFreq;

	bool  m_VideoIsInterLeaved;

	bool ISSTARTRTSP = false;
	
	bool ISSTARTRTMP = false;
	/// /////////////////////////////////////////////////
	//20220513 QDEEP

	HDC m_hScreenDC;

	HDC	m_hMemoryDC;

	ULONG m_nOSDTemp;

	PVOID m_pDetector;

	QDEEP_OBJECT_DETECT_BOUNDING_BOX* m_pObjectList;

	ULONG m_pObjectSize;

	ULONG m_nFrameSkip;

	
	
	
	CComboBox v_combobox1;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	// ºô§}
	CString m_input;

	CComboBox m_combox2;
	CComboBox m_combox3;
};
