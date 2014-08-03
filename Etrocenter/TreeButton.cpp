// TreeButton.cpp : ��@��
//

#include "stdafx.h"
#include "TreeButton.h"



#define TREEBKCOLOR RGB(255,255,250)

// CTreeButton

IMPLEMENT_DYNAMIC(CTreeButton, CBitmapButton)

CTreeButton::CTreeButton()
{

}

CTreeButton::~CTreeButton()
{
}


BEGIN_MESSAGE_MAP(CTreeButton, CButton)
	//ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_CONTROL_REFLECT(BN_CLICKED, &CTreeButton::OnBnClicked)
	//ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()


// CTreeButton �T���B�z�`��


//void CTreeButton::OnSize(UINT nType, int cx, int cy)
//{
//	CButton::OnSize(nType, cx, cy);
//
//	// TODO: �b���[�J�z���T���B�z�`���{���X
//}

int CTreeButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �b���[�J�S�O�إߪ��{���X

	return 0;
}

void CTreeButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �b���[�J�z���T���B�z�`���{���X
	// ���n�I�s�Ϥ��T���� CBitmapButton::OnPaint()
}

BOOL CTreeButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �b���[�J�z���T���B�z�`���{���X�M (��) �I�s�w�]��

	//return CBitmapButton::OnEraseBkgnd(pDC);
	char* ls_test="RGB(235,235,27)";
	unsigned int red;
	unsigned int green;
	unsigned int blue;
	//const CString ls_tmp=is_bkColor;
	sscanf(ls_test, "RGB(%u,%u,%u)", &red, &green, &blue );


	pDC->SetBkColor(RGB(red,green,blue));
	CBrush brush(BKCOLOR);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &brush);
	return true;


}

void CTreeButton::OnBnClicked()
{
	return;
	// TODO: �b���[�J����i���B�z�`���{���X
}

//HBRUSH CTreeButton::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
//{
//	// TODO:  �b���ܧ� DC �������ݩ�
//
//	// TODO:  �p�G�����өI�s���B�z�`���A�h�Ǧ^�D NULL ����
//	  HBRUSH brush;
//            brush = CreateSolidBrush(RGB(210,0,0));
//            return brush;
//	//return NULL;
//}
