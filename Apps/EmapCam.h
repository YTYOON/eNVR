#pragma once
#define	CAMSIZEW 38
#define	CAMSIZEH 23
#define	ACCESSW 24
#define	ACCESSH 24


class CEmapCam:public CBitmapButton
{
public:

	CEmapCam(void);
	~CEmapCam(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClicked();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);


	public:
		bool mb_butDown;
		bool mb_dragging;
		bool mb_dirty;
		bool	mb_ac;
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		CString m_title;

		int	m_accameraid;
		int	m_mapx, m_mapy, m_idx, m_camgroupid;
		int	m_movex, m_movey;
		int	m_nX, m_nY;
		char	mc_mode;
		bool mbTimerOn;

		afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

		afx_msg void OnBnDoubleclicked();
};
