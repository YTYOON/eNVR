#pragma once


// CTreeButton

class CTreeButton : public CBitmapButton
{
	DECLARE_DYNAMIC(CTreeButton)

public:
	CTreeButton();
	virtual ~CTreeButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnPaint();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnBnClicked();
};


