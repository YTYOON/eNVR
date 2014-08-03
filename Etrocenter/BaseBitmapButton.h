#pragma once

//#include "EcBaseTab.h"
// CBaseBitmapButton

class CBaseBitmapButton : public CBitmapButton
{
	DECLARE_DYNAMIC(CBaseBitmapButton)
	int m_index;


public:
	CBaseBitmapButton();
	virtual ~CBaseBitmapButton();
	//CDlgMatrixTag	lDlg;

public:
	void SetIndex(int o_index);
	int GetIndex();


protected:
	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnBnClicked();



	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


