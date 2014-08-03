// BaseBitmapButton.cpp : implementation file
//

#include "StdAfx.h"
#include "BaseBitmapButton.h"
#include "EcBaseTab.h" 
#include "DlgMatrixTag.h"
#include "IPCamTreeView.h"
#include "SystemInfo.h"
#include "NVR.h"

// CBaseBitmapButton

IMPLEMENT_DYNAMIC(CBaseBitmapButton, CBitmapButton)

CBaseBitmapButton::CBaseBitmapButton()
{

}

CBaseBitmapButton::~CBaseBitmapButton()
{
}


BEGIN_MESSAGE_MAP(CBaseBitmapButton, CBitmapButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CBaseBitmapButton::OnBnClicked)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CBaseBitmapButton message handlers

void CBaseBitmapButton::SetIndex(int o_index)
{
	m_index = o_index;
}

int CBaseBitmapButton::GetIndex()
{
	return m_index;
}

void CBaseBitmapButton::OnBnClicked()
{
 
	CEcBaseTab*		pEcBaseTab = (CEcBaseTab*)GetParent();
	//EndDialog(3);
    if (m_index==200)
	{
		CSystemInfo* lp_parent = (CSystemInfo*)GetParent();
		if (lp_parent)
			lp_parent->vOpenHelp();
	}
}

void CBaseBitmapButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	this->GetClientRect(rect);
	
	if(rect.PtInRect(point))
	{
		::SetCursor(LoadCursor(NULL,IDC_HAND));
	}

	CBitmapButton::OnMouseMove(nFlags, point);
}
