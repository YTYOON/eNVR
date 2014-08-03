#pragma once
#include "resource.h"
#include "BtnST.h"

// COSD
#define OSDHEIGHT 26
#define OSDBUTTONWIDTH 26
#define OSDTEXTCOLOR RGB(255,255,255)
#define OSDBKGNDCOLOR RGB(54,54,54)
#define MOREWIDTH 56

enum OSDTYPE {OSDLIVEVIEW=0,OSDPLAYBACK};
enum OSDBUTTONS {CAPTION=0,OSDSNAPSHOT,FIXRATIO,SENDAUDIO,RECVAUDIO,MORE,RECONNECT,DOTRIGGER,INSTANTPLAYBACK,RECVIDEO};
enum OSDCONTROLIDS
{
	IDC_CAPTIONOSD=5100,
	IDC_BITMAP_SNAPSHOT,
	IDC_BITMAP_AUDIO,
	IDC_BITMAP_SEND_AUDIO,
	IDC_BITMAP_FIXRATIO,
	IDC_BITMAP_MORE,
	IDC_BITMAP_RECONNECT,
	IDC_BITMAP_DOTRIGGER,
	IDC_BITMAP_INSTANTPLAYBACK,
	IDC_BITMAP_RECVIDEO,
};

class CStaticCaption : public CStatic
{
	DECLARE_DYNAMIC(CStaticCaption)
public:
	CStaticCaption();
	virtual ~CStaticCaption();
protected:
    //CBrush m_BrushBg;
		
public:
	//{{AFX_MSG(CStaticCaption)
	//afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class COSD : public CStatic
{
	DECLARE_DYNAMIC(COSD)

public:
	COSD();
	virtual ~COSD();
public:
	//OSD Controls
	CStaticCaption * m_pCaptionOSD;	
	CButtonST * m_pSnapShot;
	CButtonST *m_pFixRatio;
	CButtonST *m_pSendAudio;
    CButtonST *m_pRecvAudio;
	CButtonST *m_pReconnect;
	CButtonST *m_pDOTrigger;
	CButtonST *m_pInstantPlayBack;
	CButtonST *m_pMore;
	CStatic *m_pRecVideo;

	HBITMAP		m_hBmpRecOff;
	HBITMAP		m_hBmpRecOn;
	HBITMAP		m_hBmpRecStatus;

protected:
	//vector <int> m_vctnOSDBtnConfig;//m_vctnBtnConfiguration is arranged from left to right
	vector <int> m_vctnOSDBtnConfigAvailable;
	HBRUSH m_hbr;
	CString m_csCameraName;
	OSDTYPE m_osdType;
	CMenu   menu;

private:
    void vDropAllButons();
	void vNewAndRearrangeButton(int nBtnType, CRect &rectButton, CString csCameraName);	

protected:
	DECLARE_MESSAGE_MAP()
public:	
	void vCreateAndRearrangeControl(OSDTYPE osdType, CString csCameraName);
	void vResetOSD();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSnapshot();
	afx_msg void OnMORE();
	afx_msg void OnRecvAudio();
	afx_msg void OnSendAudio();
	afx_msg void OnFixRatio();
	afx_msg void OnRecVideo();
	afx_msg void OnClickMenu(UINT nID);	
    //afx_msg void OnPaint();
};


