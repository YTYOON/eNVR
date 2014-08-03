#if !defined(AFX_VIDEO_H__04DD7DE4_48A9_47C5_8DA5_458C0ADFC13C__INCLUDED_)
#define AFX_VIDEO_H__04DD7DE4_48A9_47C5_8DA5_458C0ADFC13C__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Video.h : header file
//
#ifdef NEWNETLIB
#include "NetSession.h"
#else
#include "NVSNetLib.h"
#endif
#include "SendAudio.h"
#include "OSD.h"

#define PANORAMA
#ifdef PANORAMA
#include "PanoSDK.h"
#pragma comment(lib, "Pano360API_003908.lib")
#endif
enum CONTROLIDS{
	ID_TEMP_ID = 4030,
	ID_BTN_SFF,
	ID_BTN_SRW,
	ID_BTN_SSTOP,
	ID_BTN_SPLAY,
	IDC_CAPTIONBAR,
	IDC_BACK,
//	ID_SNAPSHOT,
//	ID_MENU_REMOVE,
//	ID_MENU_PLAY,
	ID_MEMU_SAVETOFILE,
//	ID_MENU_CLOSE,
	IDC_OSD
};
#define IDC_PANORAMA    WM_USER+77
/////////////////////////////////////////////////////////////////////////////
// CVideo window
class DRAW_INFO 
{
public:
	int ox; 
	int oy; 
	int width; 
	int height;
};
typedef unsigned __int8   uint8_t;
struct DECODE_FILE_HEADER
{
    DWORD nHdrSize;
    DWORD version;
    BYTE btStatus[8];
    enum STATUS_ARRAY {RECORD = 0};//btStatus array meaning
};

class CNVRBk;
class CVideo : public CStatic
{
DECLARE_DYNAMIC( CVideo )
 
// Attributes
public:
	static CSendAudio sendAudio;
protected:	
	enum {MSG_LOGO, MSG_DISCONNECTION, MSG_RECONNECTION, MSG_NORMAL};
    CDC m_dcBuf;
	TCHAR m_tzMsg[4][16];
	WORD m_nMsgIdx;
	int m_zDelta;
	HANDLE m_hDecodEvent;
    HANDLE m_hRenderingEvent;
	BYTE m_nConnectType;
    HANDLE m_hStopEvent;
	int  m_nPlayBreak;
	bool m_bRecording;
	BYTE m_nSuspendMode;
	NODEITEM* m_pNode;
	NSAudioInfo* m_pAudioInfo;
	NSVideoInfo* m_pVideoInfo;
	DWORD m_playfps;
	HANDLE	m_hSendAudioThread;
	HANDLE	m_hSendAudioStopEvent;
	HANDLE  m_hAVReceiveThread;
	HANDLE	m_hVideoThread;
    UINT   m_nThreadID;
	LONG m_size;
	POINT m_PickPoint;
	BYTE* m_pImage;
	CNVRBk *m_pbk;
	CRect m_rectVideo; //left, top, width, height
	CRect m_rectDraw; //left, top, width, height
    CRect m_rcThumbnail;//left, top, width, height
    CRect m_rcPosThumbnail; //left, top, right, bottom
	CRect m_rcImg; //left, top, width, height

	CRect m_rectFixRatio;
    float m_fxScale, m_fyScale;
    CRect m_rcCrop;//left, top, right, bottom
public:
protected:
	bool m_bReCalcRect;
	bool m_bOriginalRect, m_bClosePIP;//m_bClosePIP for reflush and erase background
	uint8_t* m_picture_buf;
	BITMAPINFOHEADER m_bih;
    BYTE* m_pszGreyDib;   
    DECODE_FILE_HEADER* m_pStreamStatus;
	bool m_bUpsideDown;
	bool m_bFlip;
	bool m_bFixAspectRatio;
	bool m_bAviFileChangeFlag;
	DRAW_INFO m_DrawInfo;
#ifdef PANORAMA
    PanoramicDisplay * m_pPanorama;
	//bool  m_bPanorama;
    BYTE * m_pPanoramaBuff;
    //vector <bool> m_vtbPanorama;
    bool bFishEyeModal(NODEITEM* pNode);
    afx_msg void OnSetParama360();
    afx_msg void OnSetParama180();
    panoramic_type m_lensType;
    static WNDPROC DisplayWndProc;
	static LRESULT CALLBACK HookProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static VOID PanormaTrackMenuCallback(PanoramicDisplay *Display, HMENU hMenu );
	void vCreatePanoramaDisplay();
	int nPreWidth;
	int nPreHeight;
#endif
// Operations
public:
#ifdef PANORAMA
	bool  m_bPanorama;
#endif

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideo)
	//}}AFX_VIRTUAL

// Implementation

public:
// Construction
	CVideo();
	virtual ~CVideo();
	void UpdateDevice(int camid);
	void MoveWindow(LPCRECT lpRect, BOOL bRepaint = true);
	void SetSaveAviFile(bool bSave){m_bRecording = bSave;};
	virtual void vAttach(NODEITEM* pNode, BYTE connectType =NETSESSION_LOGIN_TYPE_VIDEO) = NULL;
	virtual void vStopPlay();
	virtual void vStopRecv();
	virtual CNetSession* GetNetSession(){return NULL;};
	void vPulseAviFileChange(){m_bAviFileChangeFlag = true;};
	bool bGetFlip() {return m_bFlip;};
	bool bGetUpsideDown() {return m_bUpsideDown;};
	bool bGetAspectRation(){return m_bFixAspectRatio;};
	bool bGetPtzFreelance(){return m_bPtzFreelance;};
	bool bGetOnOSD(){return m_bDisplayOSD;};
	NODEITEM* GetNodeItem() {return m_pNode;};
	WORD* vGetVideoMsgId(){return &m_nMsgIdx;};		//Larry add 102-11-25//
	afx_msg void OnMenuExitfullscreen();
	afx_msg void OnMenuOSD();
	afx_msg void OnSetFixRatio();
    virtual void vSetRecvAudio()=NULL;
	virtual void vSetRecVideo()=NULL;
    bool m_bSetAudio;

	// Generated message map functions
protected:
	virtual void ReconnectStream() = 0;
    void PanoramaUpdateNewFrame( BYTE *image);
    void vTextOutLine(CDC* pdc, TCHAR* tzCaption, WORD nFontSize, CRect* pRc);
	void GreyPicture(BITMAPINFOHEADER* pbih, BYTE *image);
    static CVideo* m_pVideoAudio;
    enum {CLICK_NONE =0, MOVE_THUMBNAIL = 1};
    int m_nClickMode;
	bool m_bPtzFreelance;
	bool bMouseDown;
	CPoint m_CursorPos;
	void vHintInfo();
	//inline LONG nfnAudioPack();

	static UINT __stdcall VideoPlayThread(LPVOID lpParam);
	virtual LONG VideoPlayThreadRunPack()=NULL;
	//{{AFX_MSG(CVideo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	afx_msg void OnSetVideosetUpdisedown();
	afx_msg void OnSetVideosetFlip();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPtzFreelance();
	afx_msg void OnClicked();
//	afx_msg LRESULT OnMouseWheelNotify(WPARAM wp, LPARAM lp);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//	virtual AVFrame* alloc_avpicture(PixelFormat pix_fmt, int width, int height);
	inline virtual void DrawPicture(HDC hDC, BYTE *image);
	inline virtual void DrawPicture2(HDC hDC, BYTE *image);
	inline virtual void DrawPicture3(HDC hDC, BYTE *image);

	virtual void DrawText(HDC hDC);
	void vSetVideosetFlip(bool bSet) {m_bFlip = bSet;};
	void vGetUpsideDown(bool bSet) {m_bUpsideDown = bSet;};
	void vReleaseAVObject();
//	bool VideoPlayThreadRunInit();
	static LONG CVideo::AppExceptionPolicy(LPEXCEPTION_POINTERS pExceptPtrs);
	static void LogException(LPEXCEPTION_POINTERS pExceptPtrs);

public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSnapshot();
	afx_msg void OnSendAudio();
    afx_msg void OnRecvAudio(){vSetRecvAudio();};
	afx_msg void OnRecVideo(){vSetRecVideo();};
	// OSD FUNCTIONS
	bool bCreateOSD();
	bool bResetOSD(bool bOSD);
	void vRearrangeOSD();
	bool m_bDisplayOSD;
	//
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_nRectIdx;
	bool m_bDrag;
	bool m_bAudioStart;
	int m_nVideoIdx;
	bool m_bAutoPlay;
	volatile int m_nFPS;
	CString m_csCameraName;
	CString m_csNameFPS;

protected:
	virtual bool bResetDrawRect(void);

public:
	HBRUSH m_hbr;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CRect m_rectSendAudio;
	bool m_bLButtonUp;
	bool m_bPreSetAudio;
	COSD* m_pOSD;
	virtual void SetStopPalay(BOOL bRes)=0;
	virtual void SetStopRecv(BOOL bRes)=0;

	void SetStopRecvStream(BOOL bRes){m_bStopRecvStream = bRes;};
	BOOL GetStopRecvStream(){return m_bStopRecvStream;};
private:
	BOOL m_bStopRecvStream;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEO_H__04DD7DE4_48A9_47C5_8DA5_458C0ADFC13C__INCLUDED_)
