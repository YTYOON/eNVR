// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// MSMToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "MSMToolBar.h"
#include "resource.h"
#include "MSMVisualManager.h"
#include "locale.h"
#include "afxcustomizebutton.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int const IDT_TIMER = 1;
CMFCToolBarImages CMSMToolBar::m_PressedImages;


/////////////////////////////////////////////////////////////////////////////
// CMSMToolBar

IMPLEMENT_DYNCREATE(CMSMToolBar, CMFCToolBar)


CMSMToolBar::CMSMToolBar(): m_logoMeanWidth(0), m_rect(0,0,0,0), m_rectLogo(0,0,0,0)
,m_nAuth(0), m_bLogoClick(false)
//:m_btnClock(IDB_BITMAP_CLOCK+1)//, IDB_BITMAP_CLOCK)
{
	m_hCursor = ::LoadCursor(NULL , IDC_HAND);
    if(m_hCursor)
    {
        //::SetCursor(m_hCursor);
    }
    vGetLogoWidth();
}


CMSMToolBar::~CMSMToolBar()
{
}
void CMSMToolBar::RemoveAllButtons()
{
	m_iButtonCapture = -1;      // nothing captured
	m_iHighlighted = -1;
	m_iSelected = -1;

	while (!m_Buttons.IsEmpty())
	{
		CMFCToolBarButton* pButton = (CMFCToolBarButton*) m_Buttons.RemoveHead();
		ASSERT_VALID(pButton);

		if (pButton != NULL)
		{
			pButton->OnCancelMode();
//memory leak here, but if delete pButton after exit application
//shall assert has exited with code 3 (0x3).
// that because "Use MFC in a Static Library"
			//delete pButton;
            pButton = NULL;
		}
	}

	m_pCustomizeBtn = NULL;

}

BOOL CMSMToolBar::LoadToolBar(UINT uiResID, UINT uiColdResID, UINT uiMenuResID, 
							  BOOL bLocked,
							  UINT uiDisabledResID, UINT uiMenuDisabledResID,
							  UINT uiHotResID, UINT uiPressedResID)
{
	if (!CMFCToolBar::LoadToolBar (uiResID, uiColdResID, uiMenuResID, bLocked, 
		uiDisabledResID, uiMenuDisabledResID, uiHotResID))
	{
		return FALSE;
	}

	if (uiPressedResID != 0)
	{
		m_PressedImages.SetImageSize (m_sizeImage);

 		if (!m_PressedImages.Load (uiPressedResID, AfxGetResourceHandle (), TRUE))
 		{
 			return FALSE;
 		}

		ASSERT (m_Images.GetCount () == m_PressedImages.GetCount ());
	}
////////////////////////

    int nBtnResId = uiResID+1;
    m_btnSlider.m_nID = IDB_BITMAP_CLOCK;
    m_btnSlider.m_bImage = false;
    m_btnSlider.m_bText = false;

	InsertButton(&m_btnSlider,0);//ID_GO_ADDRESS_EDIT
	m_btnSlider.SetRange (0, 100);
    InsertSeparator(1) ;
///////
	//InsertButton(CMFCToolBarButton (nBtnResId, 0),0);//ID_GO_ADDRESS_EDIT
	//m_wndClock.Create(_T("Time date"), dwStyle, rect, this);
	//InsertButton(CMFCToolBarButton (++nBtnResId, 0),0);//ID_GO_ADDRESS_EDIT
	//ReplacButton
	
    m_btnClock.m_nID = IDB_BITMAP_CLOCK+1;
    m_btnClock.m_bImage = false;
    m_btnClock.m_bText = false;
    InsertButton(&m_btnClock, 0);//ID_GO_ADDRESS_EDIT
	//m_btnClock.vSetClockIcon();
    //m_btnClock.SetImage(2);
    vGetLocalTime(m_btnClock.m_strText);
    InsertSeparator(1);

	//RemoveAllButtons();
    //m_btnClock.SetImage
    //SetToolBarBtnText(1, _T("TEST"));
    SetTimer(IDT_TIMER,1000, NULL);
	return true;
}

BEGIN_MESSAGE_MAP(CMSMToolBar, CMFCToolBar)
	ON_WM_CREATE()
	ON_WM_SYSCOLORCHANGE()
    ON_WM_TIMER()
    ON_WM_DESTROY()
    ON_WM_SETCURSOR()
    ON_WM_LBUTTONUP()
    ON_WM_SIZE()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSMToolBar message handlers

int CMSMToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMFCToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetPaneStyle((GetPaneStyle () & 
		~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT))
		| CBRS_SIZE_DYNAMIC);
	SetBorders ();
	
	SetGrayDisabledButtons (FALSE);

    LOGFONT lf;                        // Used to create the CFont.
    memset(&lf, 0, sizeof(LOGFONT));   // Clear out structure.
    lf.lfHeight = 20;                  // Request a 20-pixel-high font
    _tcscpy(lf.lfFaceName, _T("Arial"));    //    with face name "Arial".
    static CFont m_font;
    m_font.CreateFontIndirect(&lf);    // Create the font.
    SetFont(&m_font);

   //GetDlgItem(IDC_TEXT1)->SetFont(&m_font);

	return 0;
}

BOOL CMSMToolBar::DrawButton(CDC* pDC, CMFCToolBarButton* pButton,
							CMFCToolBarImages* pImages,
							BOOL bHighlighted, BOOL bDrawDisabledImages)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	CMFCToolBarImages* pNewImages = pImages;

	CAfxDrawState ds;

	if (!m_bMenuMode && (pButton->m_nStyle & (TBBS_PRESSED)) && 
		m_PressedImages.GetCount () > 0)
	{
		pNewImages = &m_PressedImages;
	
		pNewImages->SetTransparentColor (afxGlobalData.clrBtnFace);

		pNewImages->PrepareDrawImage (ds, GetImageSize (), FALSE);
	}

	if (!CMFCToolBar::DrawButton (pDC, pButton, pNewImages, bHighlighted, 
			bDrawDisabledImages))
	{
		return FALSE;
	}

	if (pNewImages != pImages)
	{
		pNewImages->EndDrawImage (ds);
	}

	return TRUE;
}


void CMSMToolBar::OnSysColorChange ()
{
	CMFCToolBar::OnSysColorChange ();

	m_PressedImages.OnSysColorChange ();
}

 void CMSMToolBar::vGetLocalTime(CString& str)
{

	const int STR_SZ = 1024;
	TCHAR szBuf[STR_SZ] = { 0 };
	TCHAR szLocale[STR_SZ] = { 0 };

	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGLANGUAGE, szBuf, STR_SZ);
	_tcscpy(szLocale, szBuf);
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGCOUNTRY, szBuf, STR_SZ);
	if (_tcsclen(szBuf) != 0){
		_tcscat(szLocale, _T("_"));
		_tcscat(szLocale, szBuf);
	}
	::GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, szBuf, STR_SZ);
	if (_tcsclen(szBuf) != 0){
		_tcscat(szLocale, _T("."));
		_tcscat(szLocale, szBuf);
	}

	{
		_tsetlocale(LC_ALL, szLocale); // e.g. szLocale = "English_United States.1252
	
		TCHAR strDate[STR_SZ];
		TCHAR strTime[STR_SZ];
		//TCHAR str[STR_SZ];

		time_t lt;
		time (&lt);
		tm* timeptr = localtime (&lt);
		_tcsftime(strDate, STR_SZ, _T("%x"), timeptr); // Date representation for current locale
		_tcsftime(strTime, STR_SZ, _T("%X"), timeptr); // Time representation for current locale
        str.Format(_T("%s   %s"), strDate,  strTime);

	}
    return;
}

void CMSMToolBar::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent != IDT_TIMER) return;
	UpdateData();
	CString csTime;
    vGetLocalTime(csTime);
        //vGetLocalTime(m_btnClock.m_strText);
	
	//this->Invalidate(true); //cause gdi leak, use updatewindow(), javan
    UpdateWindow();
    m_btnClock.SetWindowText( csTime);
	
    //m_btnClock.m_strText = csTime;
    m_btnSlider.UpdateCpuUsage();

	
	
    CMFCToolBar::OnTimer(nIDEvent);
}

void CMSMToolBar::OnDestroy()
{
    KillTimer(IDT_TIMER);
    RemoveAllButtons();
    CMFCToolBar::OnDestroy();

    // TODO: Add your message handler code here
}

BOOL CMSMToolBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    BOOL bRet(true);
    CPoint pt;
    ::GetCursorPos(&pt);
    ScreenToClient(&pt);
    if (m_rectLogo.PtInRect(pt)) 
    {
        ::SetCursor(m_hCursor);
    }
    else
    {
        bRet = CMFCToolBar::OnSetCursor(pWnd, nHitTest, message);
    }
    return bRet; 
}

void CMSMToolBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_Buttons.IsEmpty() && m_rectLogo.IsRectEmpty() == 0)
	{
		if (m_rectLogo.PtInRect(point) && m_bLogoClick)
		{
			((CNVRApp*)AfxGetApp())->OnAppAbout();
		}
	}
	CMFCToolBar::OnLButtonUp(nFlags, point);
}

void CMSMToolBar::OnSize(UINT nType, int cx, int cy)
{
    if (GetSafeHwnd() != NULL)
    {
        GetClientRect(&m_rect);
        CSize sizeFixedLayout = CalcFixedLayout(false, true);
        m_rectLogo = m_rect;
        m_rectLogo.left = max(m_rect.right - m_logoMeanWidth, sizeFixedLayout.cx);
        m_rectLogo.left = min(m_rectLogo.left, m_rectLogo.right);
    }
    CMFCToolBar::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
}


void CMSMToolBar::vGetLogoWidth()
{
#define LOGO_BMP_WIDTH 402
	CImage image;	
    HMODULE hResOem = ::LoadLibrary(_T("Manufacturer.DLL"));
    m_logoMeanWidth  = LOGO_BMP_WIDTH;
    if (hResOem)
    {
        HANDLE hBitmap = (HBITMAP)::LoadImage(hResOem, MAKEINTRESOURCE(IDB_BITMAP_NEW_LOGO), IMAGE_BITMAP, 0, 0, 0);
        if (hBitmap == NULL) return;
	    image.LoadFromResource(hResOem, IDB_BITMAP_NEW_LOGO);  
        m_logoMeanWidth = image.GetWidth();
        FreeLibrary(hResOem);
    }
    return;
}

BOOL CMSMToolBar::SetButtons(const UINT* lpIDArray, int nIDCount, BOOL bRemapImages)
{
	ASSERT_VALID(this);
	ASSERT(nIDCount >= 1);  // must be at least one of them
	ENSURE(lpIDArray == NULL || AfxIsValidAddress(lpIDArray, sizeof(UINT) * nIDCount, FALSE));

	// Save customize button:
	CMFCCustomizeButton* pCustomizeBtn = NULL;
	if (m_pCustomizeBtn != NULL)
	{
		ASSERT_VALID(m_pCustomizeBtn);
		ASSERT(m_pCustomizeBtn == m_Buttons.GetTail()); // Should be last

		CRuntimeClass* pRTC = m_pCustomizeBtn->GetRuntimeClass();
		pCustomizeBtn = DYNAMIC_DOWNCAST(CMFCCustomizeButton, pRTC->CreateObject());

		ASSERT_VALID(pCustomizeBtn);
		pCustomizeBtn->CopyFrom(*m_pCustomizeBtn);
	}

	RemoveAllButtons();

	while (!m_OrigButtons.IsEmpty())
	{
		delete m_OrigButtons.RemoveHead();
	}

	if (lpIDArray == NULL)
	{
		while (nIDCount-- > 0)
		{
			InsertSeparator();
		}

		return TRUE;
	}

	int iImage = m_iImagesOffset;

	// Go through them adding buttons:
	for (int i = 0; i < nIDCount; i ++)
	{
		int iCmd = *lpIDArray ++;

		m_OrigButtons.AddTail(new CMFCToolBarButton(iCmd, -1));

		if (iCmd == 0) // Separator
		{
			InsertSeparator();
		}
		else if (bRemapImages)
		{
			if (isNeedInsertBtn(iCmd))
			{
				CMFCToolBarButton btn;
				btn.m_nID = iCmd;
				btn.m_bImage = true;
				btn.m_bText = false;
				btn.SetImage(iImage);

				if (InsertButton(btn) >= 0 && !m_bLocked)
				{
					m_DefaultImages.SetAt(iCmd, iImage);
				}
			}
			
			iImage ++;
		}
		else
		{
			if (m_DefaultImages.Lookup(iCmd, iImage))
			{
				if (isNeedInsertBtn(iCmd))
				{
					CMFCToolBarButton btn;
					btn.m_nID = iCmd;
					btn.m_bImage = true;
					btn.m_bText = false;
					btn.SetImage(iImage);
					InsertButton(btn);
				}
				
			}
		}
	}

	// Restore customize button:
	if (pCustomizeBtn != NULL)
	{
		InsertButton(pCustomizeBtn);
		m_pCustomizeBtn = pCustomizeBtn;
	}

	if (GetSafeHwnd() != NULL)
	{
		// Allow to produce some user actions:

		OnReset();

		CWnd* pParentFrame = (m_pDockSite == NULL) ? GetParent() : m_pDockSite;
		if (pParentFrame != NULL)
		{
			pParentFrame->SendMessage(AFX_WM_RESETTOOLBAR, (WPARAM) m_uiOriginalResID);

			while (!m_OrigResetButtons.IsEmpty())
			{
				delete m_OrigResetButtons.RemoveHead();
			}

			// Store Buttons state after OnToolbarReset
			int i = 0;
			for (POSITION pos = m_Buttons.GetHeadPosition(); pos != NULL; i++)
			{
				CMFCToolBarButton* pButton = (CMFCToolBarButton*) m_Buttons.GetNext(pos);

				if (pButton != NULL && pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarButton)))
				{
					CRuntimeClass* pRTC = pButton->GetRuntimeClass();
					CMFCToolBarButton* pBtn = (CMFCToolBarButton*)pRTC->CreateObject();
					pBtn->CopyFrom(*pButton);
					m_OrigResetButtons.AddTail(pBtn);

				}
			}
		}
	}

	return TRUE;
}

bool CMSMToolBar::isNeedInsertBtn(int iCmd)
{
	bool bResult = true;

	if (m_nAuth == LV_SET && iCmd == ID_TOOLBAR_SYSTEM)  
	{
		bResult = false;
	}
	else if (m_nAuth == LV_APP && iCmd == ID_TOOLBAR_CPU)
	{
		bResult = false;
	}
	else if (m_nAuth == LV_ONLY && (iCmd == ID_TOOLBAR_CPU || iCmd == ID_TOOLBAR_SYSTEM))
	{
		bResult = false;
	}

	return bResult;
}
void CMSMToolBar::OnLButtonDown(UINT nFlags, CPoint point)
{
		if (m_rectLogo.IsRectEmpty() == 0)
        {
			if (m_rectLogo.PtInRect(point))
			{
				m_bLogoClick = true;
			}else
            {
                m_bLogoClick = false;
            }
		}
        CMFCToolBar::OnLButtonDown(nFlags, point);
}
