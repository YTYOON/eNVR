//#include "BarPtz.h"

//#pragma once

// BaseRadioButton

class CBaseRadioButton : public CButton
{
	DECLARE_DYNAMIC(CBaseRadioButton)

public:
	CBaseRadioButton();
	virtual ~CBaseRadioButton();

	
public:
	//virtual afx_msg void OnBnClicked();
	//virtual void SetColor(const COLORREF newColor);

 
public:
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	BOOL		m_bIsSelected;		// Is button pressed?
	int			m_Index;

	//CCameraBar*	m_Layout;
	COLORREF	m_BkColor;

	//CCameraBar* GetLayout(){return &m_Layout;};
	//CIPCamTreeView* GetTreeView(){return &m_wndTree;};

//protected:
//	CCameraBar* m_Layout;

protected:
	DECLARE_MESSAGE_MAP()
};


