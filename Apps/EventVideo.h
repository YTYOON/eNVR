#pragma once
#include "Resource.h"
#include "BaseDialog.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "PictureCtrl.h"
#include "VideoThin.h"
#include "afxext.h"


class CEventVideo : public CBaseDialog
{
		DECLARE_DYNAMIC(CEventVideo)

// Dialog Data
	enum { IDD = IDD_DLGEMAPVIEW1 };

public:
	CEventVideo(CWnd* pParent = NULL);   // standard constructor
	~CEventVideo(void);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

	public:
		virtual BOOL OnInitDialog();

		//bool	mb_ac;
		char	mc_parent;
		bool	mb_displayVideo; 
		bool	mb_door;	//display door icon or not

		CString	m_title;
		int			m_camidx;
		char		mc_mode;
		CBrush* 	m_black;

		void fnDisplayVideo();

		CVideoThin* mp_Video;

		bool fnCamWithinGroup(int oGroupid, int oCamidx);
		bool fnAssignNode(NODEITEM* opNode);
		bool fnAssignTitle(CString o_title);
		bool	mb_showdoor;

		CVideoThin	m_videovv;
		NODEITEM*	m_node;
		afx_msg void OnClose();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		
		CBitmapButton mbut_door;
		CBitmapButton mbut_saudio, mbut_raudio;		

		afx_msg void OnBnClickedDoor();
		afx_msg void OnBnClickedRaudio();
		afx_msg void OnBnClickedSaudio();

		LRESULT vChangeSendAudioBMP(WPARAM wParam, LPARAM lParam);			
		// show-hide the door button
		void fnShowDoor();
};
