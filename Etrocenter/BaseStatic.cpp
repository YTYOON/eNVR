// BaseStatic.cpp : implementation file
//

#include "stdafx.h"
#include "BaseStatic.h"


// CBaseStatic

IMPLEMENT_DYNAMIC(CBaseStatic, CStatic)

CBaseStatic::CBaseStatic()
: m_id(0)
{

}

CBaseStatic::~CBaseStatic()
{
}


BEGIN_MESSAGE_MAP(CBaseStatic, CStatic)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CBaseStatic message handlers



BOOL CBaseStatic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CStatic::OnEraseBkgnd(pDC);
}
