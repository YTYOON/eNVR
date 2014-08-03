#pragma once
#include "Video.h"
#include "GS.h"
#include "VideoThin.h"
#include "EventReceive.h"
#include "DlgAutoScanPause.h"

#define BORDERWIDTH 2
#define DEFLATEWIDTH (BORDERWIDTH*2)
#define BORDERCOLOR RGB(60,60,60)
//#define BKGNDCOLOR RGB(0,0,0)
#define FOCUSCOLOR RGB(190,0,0)
#define EVENTCOLOR RGB(0,0,190)
// CLiveView
struct PAUSEINFO
{
	int  nLayout;
	bool bGetGroupIdx;
	bool bPlaySameGroup;
	int  nPreviousGroupNode;
};

class CNVRBk;
class CLiveView : public CStatic
{
	DECLARE_DYNAMIC(CLiveView)

public:
	CLiveView();
	virtual ~CLiveView();

public://  play video functions
	void SetRegFlag(bool bflag){m_bSetReg = bflag;};
	void vSetGrids(int nGrid);
	void vAttach(vector <NODEITEM*> pNode);
	void vNewVideoSet(NODEITEM* pNode);	
	void vNewVideoSet(vector<NODEITEM*> pNode);
	void vNewVideoSet(vector<NODEITEM*> pNode, int nCamIdx);
	void vNewVideoSet(NODEITEM* pNode, int nCamIdx);
	void vFreeVideoSet(int nIdx = 0);
	void vOneMatrixView(int nRectIdx);
	void vMatchEventCam(int nCamIdx);
	void vPlayEventVideoQueue();
	void vDispatchEventVideo(vector<NODEITEM*> pNode);
	void vGetFitMatrixSize(int nVideoSize);
	void vGroupAutoScan(void);	
	void vResumeLiveView(void);	
	void vPauseAutoScan(void);
	static void vEventCallBack(EventInfo *m_EventInfo, void *lpParam);
	static void vAutoScanCallBack(void *lpParam);
	static UINT _stdcall thdGetEventFromService(LPVOID lpParam);
	static UINT _stdcall thdAutoScanWait(LPVOID lpParam);
	vector <NODEITEM*> vctDispatchNode(vector <NODEITEM*> vctpNode,int nDivider);

public: // member variables
	bool m_bSetReg;
	BYTE nFocusVideo;
	int	m_grids;
	long	m_GlobalScreenW;
	int		m_nCamIndex;
	int     m_nLayout;
	int     m_nPreLayout;
	int		m_nOneRectIdx;
	int m_nEventRectIdx;
	int m_nOriginalRectIdx;
	int m_nEventCounter;
	int m_nCurrentPlayGroupId;
	int m_nPreviousGroupNode;
	int m_nAutoScanTime;
	DWORD m_nAutoPlayTimer;
	DWORD m_nEventTimer;
	DWORD m_nFPSTimer;
	DWORD m_nGroupAutoPlayTimer;
	DWORD m_nGroupAutoPauseTimer;
	vector <NODEITEM*> m_vctpNode;
	vector <NODEITEM*> m_vctpPreNode;
	vector <NODEITEM*> m_vctpCurrentGroupNodes;
	vector <NODEITEM*> m_vctpGroupAutoScanNode;
	vector <CVideoThin*> m_videos;
	vector <CRect> m_rects;
	vector <int> m_vctnGroupAutoPlayIdx;
	queue <int> m_nPlayIdxQueue;
	queue <int> m_nGroupIdxQueue;
	CRect   m_rect;	
	bool m_bDragging;
	bool m_bDisplayOSD;
	bool m_bOnFlyAway;
	bool m_bSwap;
	bool*	m_bOutOfRange;					//for LiveView and Event//
	bool	m_bOutOfRangeIPWall;		//for IP Wall//
	bool m_bAutoPlay;
	bool m_bOneMatrixView;
	bool m_bEventEnable;
	bool m_bGroupAutoPlay;
	bool m_bGetGroupIdx;
	bool m_bPlaySameGroup;
	bool m_bAutoScanPause;
	CGS m_gs;
	EventInfo m_EventInfo;	
	CEventReceive* m_EventRec;
	HANDLE m_hThreadEventRec;	
	HANDLE m_hThreadAutoScanWait;
	HANDLE m_hPauseAutoScan;
	HANDLE m_hResumeAutoScan;
	CDlgAutoScanPause *m_pDlgPause;
	PAUSEINFO m_strPause;
	CNVRBk *m_pbk;
private:
	void SetTreeNodeFocus();
	void SetVideoFocos(NODEITEM *pNode);
	void DrawVideoFrame(int oldFocus, int newFocus);
	void FreeOneVideo(NODEITEM *pNode);
	void UpdateDevice(int camid);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT vSetOSD(WPARAM wParam, LPARAM lParam);
	LRESULT vChangeSendAudioBMP(WPARAM wParam, LPARAM lParam);	
	LRESULT OnUpdateView(WPARAM,LPARAM);
	void vCreatePauseDlg(void);   
};


