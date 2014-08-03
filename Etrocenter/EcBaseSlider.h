#pragma once


// CEcBaseSlider

class CEcBaseSlider : public CSliderCtrl
{
	DECLARE_DYNAMIC(CEcBaseSlider)

public:
	CEcBaseSlider();
	virtual ~CEcBaseSlider();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


