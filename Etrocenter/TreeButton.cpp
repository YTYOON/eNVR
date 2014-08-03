// TreeButton.cpp : 實作檔
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


// CTreeButton 訊息處理常式


//void CTreeButton::OnSize(UINT nType, int cx, int cy)
//{
//	CButton::OnSize(nType, cx, cy);
//
//	// TODO: 在此加入您的訊息處理常式程式碼
//}

int CTreeButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此加入特別建立的程式碼

	return 0;
}

void CTreeButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此加入您的訊息處理常式程式碼
	// 不要呼叫圖片訊息的 CBitmapButton::OnPaint()
}

BOOL CTreeButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

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
	// TODO: 在此加入控制項告知處理常式程式碼
}

//HBRUSH CTreeButton::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
//{
//	// TODO:  在此變更 DC 的任何屬性
//
//	// TODO:  如果不應該呼叫父處理常式，則傳回非 NULL 筆刷
//	  HBRUSH brush;
//            brush = CreateSolidBrush(RGB(210,0,0));
//            return brush;
//	//return NULL;
//}
