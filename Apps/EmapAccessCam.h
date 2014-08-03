 #pragma once

#define	CAMSIZEW 38
#define	CAMSIZEH 23
#define	ACCESSW 24
#define	ACCESSH 24

typedef struct STR_CAM
{
		int	camidx;
		CString cskey;
		NMHDR hdr;
		int delta;
		int	xdiff;
		int	ydiff;
		CPoint*	cpoint;
	} STR_CAM;

// CEmapAccessCam

class CEmapAccessCam : public CBitmapButton
{
	DECLARE_DYNAMIC(CEmapAccessCam)

public:
	CEmapAccessCam();
	virtual ~CEmapAccessCam();

//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	int	m_camidx;

		public:
		bool mb_butDown;
		bool mb_dragging;
		bool mb_dirty;
		bool	mb_ac;
		
		CString m_title;

		int	m_accameraid;
		int	m_mapx, m_mapy, m_idx, m_camgroupid;
		int	m_movex, m_movey;
		int	m_nX, m_nY;
		char	mc_mode;
		bool mbTimerOn;

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClicked();
	afx_msg void OnBnDoubleclicked();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);

 
};


