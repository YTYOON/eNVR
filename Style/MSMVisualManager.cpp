// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// MSMVisualManager.cpp : implementation file
//

#include "stdafx.h"
#include "MSMVisualManager.h"
#include "afxtagmanager.h"
#include "MSMCaptionBar.h"
#include "MSMToolBar.h"
#include "resource.h"
#include "resource2.h"
#define AFX_RT_STYLE_XML _T("STYLE_XML")
#define TOOLBAR_WIDTH 1060
const CMFCVisualManagerOffice2007::Style c_StyleDefault = CMFCVisualManagerOffice2007::Office2007_ObsidianBlack;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WP_MINBUTTON		15
#define WP_MAXBUTTON		17
#define WP_CLOSEBUTTON		18
#define WP_RESTOREBUTTON	21

#define CBS_NORMAL			1
#define CBS_HOT				2
#define CBS_PUSHED			3
#define	CBS_DISABLED		4

IMPLEMENT_DYNCREATE(CMSMVisualManager, CMFCVisualManagerOffice2007)

/////////////////////////////////////////////////////////////////////////////
// CMSMVisualManager

BOOL _StretchBitmap (CDC* pDC, const CRect& rectClient, CBitmap& bmp)
{
	ASSERT_VALID (pDC);

	if (bmp.GetSafeHandle () != NULL)
	{
		BITMAP bm;
		ZeroMemory (&bm, sizeof (BITMAP));

		bmp.GetBitmap (&bm);
		
		CDC dc;
		dc.CreateCompatibleDC (pDC);

		// Select the bitmap into the in-memory DC
		CBitmap* pOldBitmap = dc.SelectObject (&bmp);

		int nOldStretchBltMode = pDC->SetStretchBltMode (COLORONCOLOR);

		pDC->StretchBlt(0, 0, rectClient.Width (), rectClient.Height (), &dc, 
						0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);


		dc.SelectObject (pOldBitmap);

		pDC->SetStretchBltMode (nOldStretchBltMode);

		return true;
	}

	return false;
}


void _DrawReflectedGradient (CDC* pDC, COLORREF clrS, COLORREF clrM, COLORREF clrF,
							 const CRect& rect, BOOL bHorz)
{
	ASSERT_VALID (pDC);

	CDrawingManager dm (*pDC);

	long x1 = rect.left;
	long x2 = rect.right;
	long y1 = rect.top;
	long y2 = rect.bottom;

	if (bHorz)
	{
		CRect rt (x1, y1, x2, (y1 + y2) / 2);
		dm.FillGradient (rt, clrM, clrS, TRUE);
		rt.top = rt.bottom;
		rt.bottom = y2;
		dm.FillGradient (rt, clrF, clrM, TRUE);
	}
	else
	{
		CRect rt (x1, y1, (x1 + x2) / 2, y2);
		dm.FillGradient (rt, clrS, clrM, FALSE);
		rt.left  = rt.right;
		rt.right = x2;
		dm.FillGradient (rt, clrM, clrF, FALSE);
	}
}

void CMSMVisualManager::vDrawLogo(CDC& dc, CRect& clientRect)
{
	CImage image;	
	//image.LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_NEW_LOGO);
    HMODULE hResOem = ::LoadLibrary(_T("Manufacturer.DLL"));
    if (hResOem)
    {
        /*bool bRet = ::FindResource(hResOem, MAKEINTRESOURCE(IDB_BITMAP_NEW_LOGO), IMAGE_BITMAP);
        if (bRet == false) return;*/
        HANDLE hBitmap = (HBITMAP)::LoadImage(hResOem, MAKEINTRESOURCE(IDB_BITMAP_NEW_LOGO), IMAGE_BITMAP, 0, 0, 0);
        if (hBitmap == NULL) return;
        //CLOSEHANDLE(hBitmap);
	    image.LoadFromResource(hResOem, IDB_BITMAP_NEW_LOGO);
        //if (image.) return;
        //if (image.m_hBitmap == NULL) return;
        FreeLibrary(hResOem);
    }else
    {
        return;
    }
    //if (clientRect.Width() < (TOOLBAR_WIDTH +image.GetWidth())) return;
	//HRESULT hResult = image.Load(_T("C:\\LOGO.BMP"));
		//HICON hResultIcon = (HICON)image.Load(".\\images\\History.ico");
	/*    if (hResult == S_OK)
		{
			HBITMAP hbmp = image.Detach();
			//bitmap.Attach(hbmp);
			m_ButtonBit.Attach(hbmp);
		}
	*/ 
	//CDC MemDC;
	//MemDC.CreateCompatibleDC(&dc);
	  
	/*    MemDC.SelectObject(m_ButtonBit);
		CBitmap *pOldBmp = MemDC.SelectObject(&m_ButtonBit);
	*/
	CRect rt(0,0,image.GetWidth(),image.GetHeight());
	
	HBITMAP hBmp = (HBITMAP) GetCurrentObject(dc.m_hDC, OBJ_BITMAP);
	
	BITMAP bmp;
	
	::GetObject(hBmp, sizeof(bmp), &bmp);
	DWORD nWidth = bmp.bmWidth;
    image.Draw(dc, nWidth- image.GetWidth()+1, m_CaptionButtonSize.cy);
	//BOOL bRet =dc.BitBlt(500, 0, rt.Width(), rt.Height(), &MemDC, 0, 0, SRCCOPY);
	return;
}
CMSMVisualManager::CMSMVisualManager()
{
    //CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
    //CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
	////CDockingManager::SetDockingMode(DT_SMART);
	////RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);
	SetMenuShadowDepth  (0);

	m_MemoryFilled = false;
	m_MemorySize   = CSize (0, 0);

	m_CaptionButtonSize = CSize (0, 0);

	m_brMenuBarButtonHighlighted.CreateSolidBrush (RGB ( 57, 134, 206));
	m_clrMenuBarButtonBorderHighlighted = RGB ( 57, 134, 206);

	m_clrMenuBarButtonTextRegular       = RGB (255, 255, 255);
	m_clrMenuBarButtonTextPressed       = RGB (255, 255, 255);
	m_clrMenuBarButtonTextHighlighted   = RGB (255, 255, 255);

	m_clrToolBarButtonTextRegular       = RGB (255, 255, 255);
	m_clrToolBarButtonTextPressed       = RGB (255, 223, 127);
	m_clrToolBarButtonTextHighlighted   = RGB (255, 223, 127);

	m_clrCategorieBarButtonTextRegular	= RGB (232, 243, 253);
	m_clrCategorieBarButtonTextPressed	= RGB (255, 223, 127);
	m_clrCategorieBarButtonTextHighlighted
										= RGB (255, 223, 127);

	m_clrToolBarSeparatorS              = RGB (35, 35, 35);
	m_clrToolBarSeparatorM              = RGB (100, 100, 100);
	m_clrToolBarSeparatorF              = RGB ( 0, 0, 0);

	m_clrComboBoxBorder					= RGB ( 78, 122, 171);

	m_clrComboBoxBtnRegularF			= RGB (208, 220, 252);
	m_clrComboBoxBtnRegularL			= RGB (182, 205, 251);
	m_clrComboBoxBtnRegularBorder		= RGB (177, 197, 245);
	m_clrComboBoxBtnHighlightedF		= RGB (226, 244, 254);
	m_clrComboBoxBtnHighlightedL		= RGB (203, 219, 252);
	m_clrComboBoxBtnHighlightedBorder	= RGB (140, 165, 225);
	m_clrComboBoxBtnPressedF			= RGB (122, 153, 242);
	m_clrComboBoxBtnPressedL			= RGB (159, 178, 235);
	m_clrComboBoxBtnPressedBorder		= RGB (181, 198, 242);

	m_clrCategorieBarButtonBorder       = RGB ( 53,  94, 159);
	m_clrCategorieBarButtonBorderM      = RGB (191, 209, 231);
	m_clrCategorieBarButtonLine         = RGB (123, 165, 210);
	m_clrCategorieBarButtonF            = RGB ( 77, 127, 193);
	m_clrCategorieBarButtonL            = RGB ( 40,  74, 144);

	m_brLinksBarBack.CreateSolidBrush (RGB ( 49,  85, 153));

	m_brLinksBarButtonHighlighted.CreateSolidBrush (RGB ( 38,  69, 135));
	m_clrLinksBarButtonBorderHighlighted= RGB ( 38,  69, 135);
    CMFCVisualManagerOffice2007::SetStyle(c_StyleDefault);
}

BOOL CMSMVisualManager::LoadMSMCaptionButtonsIcons (LPCTSTR lpszID)
{
	CBitmap bmp;

	m_CaptionButtonSize = CSize (0, 0);

	if (bmp.LoadBitmap (lpszID))
	{
		BITMAP bm;
		ZeroMemory (&bm, sizeof (BITMAP));

		bmp.GetBitmap (&bm);

		if (m_CaptionButtonIconst.GetSafeHandle() != NULL && 
			m_CaptionButtonIconst.GetImageCount () > 0)
		{
			m_CaptionButtonIconst.DeleteImageList ();
		}

		UINT nFlags = ILC_MASK;

		switch (bm.bmBitsPixel)
		{
		case 4 :
			nFlags |= ILC_COLOR4;
			break;
		case 8 :
			nFlags |= ILC_COLOR8;
			break;
		case 16:
			nFlags |= ILC_COLOR16;
			break;
		case 24:
			nFlags |= ILC_COLOR24;
			break;
		case 32:
			nFlags |= ILC_COLOR32;
			break;
		default:
			ASSERT (FALSE);
		}

		m_CaptionButtonSize.cx = bm.bmWidth / 12;
		m_CaptionButtonSize.cy = bm.bmHeight;

		m_CaptionButtonIconst.Create (m_CaptionButtonSize.cx, 
			m_CaptionButtonSize.cy, nFlags, 0, 3);
		m_CaptionButtonIconst.Add (&bmp, RGB (255, 0, 255));

		return true;
	}

	return false;
}

void CMSMVisualManager::OnUpdateSystemColors ()
{
    //CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);

	//CMFCVisualManagerOffice2003::OnUpdateSystemColors();
	PreUpdateSystemColors();

	m_clrMenuLight						= RGB (210, 232, 253);
	m_brMenuLight.DeleteObject ();
	m_brMenuLight.CreateSolidBrush (m_clrMenuLight);

	m_clrHighlight						= RGB (0, 0, 0);
	m_brHighlight.DeleteObject ();
	m_brHighlight.CreateSolidBrush (m_clrHighlight);

	m_clrHighlightDn					= RGB (0, 0, 0);
	m_brHighlightDn.DeleteObject ();
	m_brHighlightDn.CreateSolidBrush (m_clrHighlightDn);

	m_clrHighlightChecked				= RGB (152, 192, 235);
	m_brHighlightChecked.DeleteObject ();
	m_brHighlightChecked.CreateSolidBrush (m_clrHighlightChecked);

	m_clrMenuItemBorder 				= RGB (152, 192, 235);

	m_clrMenuBorder 					= RGB (128, 128, 128);
}

///////////////////////////////////////////////////////////

void CMSMVisualManager::PreUpdateSystemColors()
{
	CleanUp();

	CMFCVisualManagerOffice2003::OnUpdateSystemColors();

	if (!afxGlobalData.bIsOSAlphaBlendingSupport || afxGlobalData.IsHighContrastMode() || afxGlobalData.m_nBitsPerPixel <= 8)
	{
		return;
	}

	m_nMenuBorderSize = 1;

	HINSTANCE hinstResOld = NULL;

	if (m_hinstRes == NULL)
	{
		SetStyle(c_StyleDefault);
	}

	if (m_hinstRes != NULL)
	{
		hinstResOld = AfxGetResourceHandle();
		AfxSetResourceHandle(m_hinstRes);
	}

	CTagManager tm;
//IDR_EVSTYLE_XML
	if (!tm.LoadFromResource(IDR_EVSTYLE_XML, _T("STYLE_XML")))
	{
#if !defined _AFXDLL
		TRACE(_T("\r\nImportant: to enable the Office 2007 look in static link,\r\n"));
		TRACE(_T("include afxribbon.rc from the RC file in your project.\r\n\r\n"));
		ASSERT(FALSE);
#endif
		if (hinstResOld != NULL)
		{
			AfxSetResourceHandle(hinstResOld);
		}

		return;
	}

	{
		CString strStyle;
		tm.ExcludeTag(_T("STYLE"), strStyle);
		tm.SetBuffer(strStyle);
	}

	CString strItem;

	m_nType = 20;

	if (!tm.IsEmpty())
	{
		int nVersion = 0;

		if (tm.ExcludeTag(_T("VERSION"), strItem))
		{
			CTagManager tmItem(strItem);

			tmItem.ReadInt(_T("NUMBER"), nVersion);

			if (nVersion == 2007)
			{
				tmItem.ReadInt(_T("TYPE"), m_nType);

				if (m_nType < 10)
				{
					m_nType *= 10;
				}

				m_bLoaded = TRUE;
			}

			if (m_bLoaded)
			{
				if (tmItem.ExcludeTag(_T("ID_PREFIX"), strItem))
				{
					strItem.Trim();
					m_strStylePrefix = strItem;
				}
			}
		}
	}

	if (!m_bLoaded)
	{
		if (hinstResOld != NULL)
		{
			::AfxSetResourceHandle(hinstResOld);
		}

		return;
	}

	// globals
	if (tm.ExcludeTag(_T("GLOBALS"), strItem))
	{
		CTagManager tmItem(strItem);

		tmItem.ReadColor(_T("BarText"), afxGlobalData.clrBarText);

		if (tmItem.ReadColor(_T("BarFace"), afxGlobalData.clrBarFace))
		{
			afxGlobalData.brBarFace.DeleteObject();
			afxGlobalData.brBarFace.CreateSolidBrush(afxGlobalData.clrBarFace);
			m_clrMenuShadowBase = afxGlobalData.clrBarFace;
		}
		if (tmItem.ReadColor(_T("ActiveCaption"), afxGlobalData.clrActiveCaption))
	    //afxGlobalData.clrActiveCaption = RGB(70,70,70);
		{
			afxGlobalData.clrInactiveCaption     = afxGlobalData.clrActiveCaption;
			afxGlobalData.brActiveCaption.DeleteObject();
			afxGlobalData.brActiveCaption.CreateSolidBrush(afxGlobalData.clrActiveCaption);
		}
		if (tmItem.ReadColor(_T("CaptionText"), afxGlobalData.clrCaptionText))
		{
			afxGlobalData.clrInactiveCaptionText = afxGlobalData.clrCaptionText;
		}

		tmItem.ReadColor(_T("InactiveCaption"), afxGlobalData.clrInactiveCaption);
		afxGlobalData.brInactiveCaption.DeleteObject();
		afxGlobalData.brInactiveCaption.CreateSolidBrush(afxGlobalData.clrInactiveCaption);
		tmItem.ReadColor(_T("InactiveCaptionText"), afxGlobalData.clrInactiveCaptionText);

		tmItem.ReadColor(_T("BarShadow"), afxGlobalData.clrBarShadow);
		tmItem.ReadColor(_T("BarDkShadow"), afxGlobalData.clrBarDkShadow);
		tmItem.ReadColor(_T("BarLight"), afxGlobalData.clrBarLight);

		COLORREF clrFloatToolBarBorder;
		tmItem.ReadColor(_T("FloatToolBarBorder"), clrFloatToolBarBorder);
		m_brFloatToolBarBorder.DeleteObject();
		m_brFloatToolBarBorder.CreateSolidBrush(clrFloatToolBarBorder);

		tmItem.ReadColor(_T("HighlightGradientDark"), m_clrHighlightGradientDark);
		tmItem.ReadColor(_T("HighlightGradientLight"), m_clrHighlightGradientLight);

		m_clrHighlightDnGradientDark = m_clrHighlightGradientLight;
		m_clrHighlightDnGradientLight = m_clrHighlightGradientDark;
		tmItem.ReadColor(_T("HighlightDnGradientDark"), m_clrHighlightDnGradientDark);
		tmItem.ReadColor(_T("HighlightDnGradientLight"), m_clrHighlightDnGradientLight);

		m_clrHighlightCheckedGradientDark = m_clrHighlightDnGradientLight;
		m_clrHighlightCheckedGradientLight = m_clrHighlightDnGradientDark;
		tmItem.ReadColor(_T("HighlightCheckedGradientDark"), m_clrHighlightCheckedGradientDark);
		tmItem.ReadColor(_T("HighlightCheckedGradientLight"), m_clrHighlightCheckedGradientLight);

		tmItem.ReadColor(_T("PressedButtonBorder"), m_clrPressedButtonBorder);

		COLORREF clrHB = afxGlobalData.clrHilite;
		COLORREF clrHT = afxGlobalData.clrTextHilite;
		if (tmItem.ReadColor(_T("Highlight"), clrHB) && tmItem.ReadColor(_T("HighlightText"), clrHT))
		{
			afxGlobalData.clrHilite = clrHB;

			afxGlobalData.brHilite.DeleteObject();
			afxGlobalData.brHilite.CreateSolidBrush(clrHB);

			afxGlobalData.clrTextHilite = clrHT;
		}

		tmItem.ReadColor(_T("MenuShadowColor"), m_clrMenuShadowBase);

		// ToolTipParams
		m_bToolTipParams = tmItem.ReadToolTipInfo(_T("TOOLTIP"), m_ToolTipParams);
	}

	// mainwnd
	if (tm.ExcludeTag(_T("MAINWND"), strItem))
	{
		CTagManager tmItem(strItem);

		// caption
		CString strCaption;
		if (tmItem.ExcludeTag(_T("CAPTION"), strCaption))
		{
			CTagManager tmCaption(strCaption);

			NONCLIENTMETRICS ncm;
			if (afxGlobalData.GetNonClientMetrics (ncm))
			{
				tmCaption.ReadFont(_T("FONT"), ncm.lfCaptionFont);
				m_AppCaptionFont.DeleteObject();
				m_AppCaptionFont.CreateFontIndirect(&ncm.lfCaptionFont);
			}

			tmCaption.ReadColor(_T("ActiveStart"), m_clrAppCaptionActiveStart);
			tmCaption.ReadColor(_T("ActiveFinish"), m_clrAppCaptionActiveFinish);
			tmCaption.ReadColor(_T("InactiveStart"), m_clrAppCaptionInactiveStart);
			tmCaption.ReadColor(_T("InactiveFinish"), m_clrAppCaptionInactiveFinish);
			tmCaption.ReadColor(_T("ActiveText"), m_clrAppCaptionActiveText);
			tmCaption.ReadColor(_T("InactiveText"), m_clrAppCaptionInactiveText);
			tmCaption.ReadColor(_T("ActiveTitleText"), m_clrAppCaptionActiveTitleText);
			tmCaption.ReadColor(_T("InactiveTitleText"), m_clrAppCaptionInactiveTitleText);

			tmCaption.ReadBool(_T("TextCenter"), m_bNcTextCenter);

			tmCaption.ReadControlRenderer(_T("BORDER"), m_ctrlMainBorderCaption, MakeResourceID(_T("IDB_OFFICE2007_MAINBORDER_CAPTION")));

			m_szNcBtnSize[0] = CSize(::GetSystemMetrics(SM_CXSIZE), ::GetSystemMetrics(SM_CYSIZE));
			m_szNcBtnSize[1] = CSize(::GetSystemMetrics(SM_CXSMSIZE), ::GetSystemMetrics(SM_CYSMSIZE));

			// buttons
			CString strButtons;
			if (tmCaption.ExcludeTag(_T("BUTTONS"), strButtons))
			{
				CTagManager tmButtons(strButtons);

				for (int i = 0; i < 2; i++)
				{
					CString str;
					CString suffix;
					if (i == 1)
					{
						suffix = _T("_S");
					}
					if (tmButtons.ExcludeTag(i == 0 ? _T("NORMAL") : _T("SMALL"), str))
					{
						CTagManager tmBtn(str);

						tmBtn.ReadSize(_T("ConstSize"), m_szNcBtnSize[i]);

						CSize sizeIcon(0, 0);
						if (tmBtn.ReadSize(_T("IconSize"), sizeIcon))
						{
							m_SysBtnClose[i].Clear();
							m_SysBtnClose[i].SetPreMultiplyAutoCheck(TRUE);
							m_SysBtnClose[i].SetImageSize(sizeIcon);
							m_SysBtnClose[i].LoadStr(MakeResourceID(_T("IDB_OFFICE2007_SYS_BTN_CLOSE") + suffix));

							m_SysBtnRestore[i].Clear();
							m_SysBtnRestore[i].SetPreMultiplyAutoCheck(TRUE);
							m_SysBtnRestore[i].SetImageSize(sizeIcon);
							m_SysBtnRestore[i].LoadStr(MakeResourceID(_T("IDB_OFFICE2007_SYS_BTN_RESTORE") + suffix));

							m_SysBtnMaximize[i].Clear();
							m_SysBtnMaximize[i].SetPreMultiplyAutoCheck(TRUE);
							m_SysBtnMaximize[i].SetImageSize(sizeIcon);
							m_SysBtnMaximize[i].LoadStr(MakeResourceID(_T("IDB_OFFICE2007_SYS_BTN_MAXIMIZE") + suffix));

							m_SysBtnMinimize[i].Clear();
							m_SysBtnMinimize[i].SetPreMultiplyAutoCheck(TRUE);
							m_SysBtnMinimize[i].SetImageSize(sizeIcon);
							m_SysBtnMinimize[i].LoadStr(MakeResourceID(_T("IDB_OFFICE2007_SYS_BTN_MINIMIZE") + suffix));
						}

						CTagManager::ParseControlRenderer(tmBtn.GetBuffer(), m_SysBtnBack[i], MakeResourceID(_T("IDB_OFFICE2007_SYS_BTN_BACK")));
					}
				}
			}
		}

		// border
		tmItem.ReadControlRenderer(_T("BORDER"), m_ctrlMainBorder, MakeResourceID(_T("IDB_OFFICE2007_MAINBORDER")));
		tmItem.ReadControlRenderer(_T("BORDER_MDICHILD"), m_ctrlMDIChildBorder, MakeResourceID(_T("IDB_OFFICE2007_MDICHILDBORDER")));

		if (tmItem.ReadColor(_T("MainClientArea"), m_clrMainClientArea))
		{
			m_brMainClientArea.DeleteObject();
			m_brMainClientArea.CreateSolidBrush(m_clrMainClientArea);
		}
	}

	// menu
	if (tm.ExcludeTag(_T("MENU"), strItem))
	{
		CTagManager tmItem(strItem);

		if (tmItem.ReadColor(_T("Light"), m_clrMenuLight))
		{
			m_brMenuLight.DeleteObject();
			m_brMenuLight.CreateSolidBrush(m_clrMenuLight);
		}

		m_clrMenuRarelyUsed = CLR_DEFAULT;
		tmItem.ReadColor(_T("Rarely"), m_clrMenuRarelyUsed);

		tmItem.ReadColor(_T("Border"), m_clrMenuBorder);

		if (tmItem.ReadColor(_T("Separator1"), m_clrSeparator1))
		{
			m_penSeparator.DeleteObject();
			m_penSeparator.CreatePen(PS_SOLID, 1, m_clrSeparator1);
		}

		if (tmItem.ReadColor(_T("Separator2"), m_clrSeparator2))
		{
			m_penSeparator2.DeleteObject();
			m_penSeparator2.CreatePen(PS_SOLID, 1, m_clrSeparator2);
		}

		COLORREF clrGroupBack = (COLORREF)-1;
		if (tmItem.ReadColor(_T("GroupBackground"), clrGroupBack))
		{
			m_brGroupBackground.DeleteObject();
			m_brGroupBackground.CreateSolidBrush(clrGroupBack);
		}

		tmItem.ReadColor(_T("GroupText"), m_clrGroupText);

		if (tmItem.ReadColor(_T("ItemBorder"), m_clrMenuItemBorder))
		{
			m_penMenuItemBorder.DeleteObject();
			m_penMenuItemBorder.CreatePen(PS_SOLID, 1, m_clrMenuItemBorder);
		}

		tmItem.ReadInt(_T("BorderSize"), m_nMenuBorderSize);

		tmItem.ReadControlRenderer(_T("ItemBack"), m_ctrlMenuItemBack, MakeResourceID(_T("IDB_OFFICE2007_MENU_ITEM_BACK")));
		tmItem.ReadToolBarImages(_T("ItemCheck"), m_MenuItemMarkerC, MakeResourceID(_T("IDB_OFFICE2007_MENU_ITEM_MARKER_C")));
		tmItem.ReadToolBarImages(_T("ItemRadio"), m_MenuItemMarkerR, MakeResourceID(_T("IDB_OFFICE2007_MENU_ITEM_MARKER_R")));
		tmItem.ReadControlRenderer(_T("ItemShowAll"), m_ctrlMenuItemShowAll, MakeResourceID(_T("IDB_OFFICE2007_MENU_ITEM_SHOWALL")));
		tmItem.ReadControlRenderer(_T("Highlighted"), m_ctrlMenuHighlighted[0], MakeResourceID(_T("IDB_OFFICE2007_MENU_BTN")));
		tmItem.ReadControlRenderer(_T("HighlightedDisabled"), m_ctrlMenuHighlighted[1], MakeResourceID(_T("IDB_OFFICE2007_MENU_BTN_DISABLED")));
		tmItem.ReadControlRenderer(_T("ButtonBorder"), m_ctrlMenuButtonBorder, MakeResourceID(_T("IDB_OFFICE2007_MENU_BTN_VERT_SEPARATOR")));
		tmItem.ReadControlRenderer(_T("ScrollBtn_T"), m_ctrlMenuScrollBtn[0], MakeResourceID(_T("IDB_OFFICE2007_MENU_BTN_SCROLL_T")));
		tmItem.ReadControlRenderer(_T("ScrollBtn_B"), m_ctrlMenuScrollBtn[1], MakeResourceID(_T("IDB_OFFICE2007_MENU_BTN_SCROLL_B")));

		tmItem.ReadColor(_T("TextNormal"), m_clrMenuText);
		tmItem.ReadColor(_T("TextHighlighted"), m_clrMenuTextHighlighted);
		tmItem.ReadColor(_T("TextDisabled"), m_clrMenuTextDisabled);

		COLORREF clrImages = m_clrMenuText;

		CString strColors;
		if (tmItem.ExcludeTag(_T("COLORS"), strColors))
		{
			CTagManager tmColors(strColors);

			tmColors.ReadColor(_T("Black"), clrImages);
			CMenuImages::SetColor(CMenuImages::ImageBlack, clrImages);

			tmColors.ReadColor(_T("Black2"), clrImages);
			CMenuImages::SetColor(CMenuImages::ImageBlack2, clrImages);

			struct XColors
			{
				CMenuImages::IMAGE_STATE state;
				LPCTSTR name;
			};
			XColors colors[4] =
			{
				{CMenuImages::ImageGray, _T("Gray")},
				{CMenuImages::ImageLtGray, _T("LtGray")},
				{CMenuImages::ImageWhite, _T("White")},
				{CMenuImages::ImageDkGray, _T("DkGray")}
			};

			for (int ic = 0; ic < 4; ic++)
			{
				if (tmColors.ReadColor(colors[ic].name, clrImages))
				{
					CMenuImages::SetColor(colors[ic].state, clrImages);
				}
			}
		}
		else
		{
			tmItem.ReadColor(_T("ImagesColor"), clrImages);
			CMenuImages::SetColor(CMenuImages::ImageBlack, clrImages);
			CMenuImages::SetColor(CMenuImages::ImageBlack2, clrImages);
		}
	}

	// bars
	if (tm.ExcludeTag(_T("BARS"), strItem))
	{
		CTagManager tmItem(strItem);

		CString strBar;
		if (tmItem.ExcludeTag(_T("DEFAULT"), strBar))
		{
			CTagManager tmBar(strBar);

			if (tmBar.ReadColor(_T("Bkgnd"), m_clrBarBkgnd))
			{
				m_brBarBkgnd.DeleteObject();
				m_brBarBkgnd.CreateSolidBrush(m_clrBarBkgnd);
			}

			tmBar.ReadColor(_T("GradientLight"), m_clrBarGradientLight);
			m_clrBarGradientDark = m_clrBarGradientLight;
			tmBar.ReadColor(_T("GradientDark"), m_clrBarGradientDark);
		}

		if (tmItem.ExcludeTag(_T("TOOLBAR"), strBar))
		{
			CTagManager tmBar(strBar);

			m_clrToolBarGradientLight = m_clrBarGradientLight;
			m_clrToolBarGradientDark  = m_clrBarGradientDark;

			m_clrToolbarDisabled = CDrawingManager::SmartMixColors(m_clrToolBarGradientDark, m_clrToolBarGradientLight);

			tmBar.ReadColor(_T("GradientLight"), m_clrToolBarGradientLight);
			tmBar.ReadColor(_T("GradientDark"), m_clrToolBarGradientDark);

			m_clrToolBarGradientVertLight = m_clrToolBarGradientLight;
			m_clrToolBarGradientVertDark  = m_clrToolBarGradientDark;

			tmBar.ReadColor(_T("GradientVertLight"), m_clrToolBarGradientVertLight);
			tmBar.ReadColor(_T("GradientVertDark"), m_clrToolBarGradientVertDark);

			tmBar.ReadColor(_T("CustomizeButtonGradientLight"), m_clrCustomizeButtonGradientLight);
			tmBar.ReadColor(_T("CustomizeButtonGradientDark"), m_clrCustomizeButtonGradientDark);

			tmBar.ReadToolBarImages(_T("GRIPPER"), m_ToolBarGripper, MakeResourceID(_T("IDB_OFFICE2007_GRIPPER")));
			tmBar.ReadToolBarImages(_T("TEAR"), m_ToolBarTear, MakeResourceID(_T("IDB_OFFICE2007_TEAR")));

			tmBar.ReadControlRenderer(_T("BUTTON"), m_ctrlToolBarBtn, MakeResourceID(_T("IDB_OFFICE2007_TOOLBAR_BTN")));
			tmBar.ReadControlRenderer(_T("BORDER"), m_ctrlToolBarBorder, MakeResourceID(_T("IDB_OFFICE2007_TOOLBAR_BORDER")));

			m_clrToolBarBtnText = afxGlobalData.clrBarText;
			m_clrToolBarBtnTextHighlighted = m_clrToolBarBtnText;
			tmBar.ReadColor(_T("TextNormal"), m_clrToolBarBtnText);
			tmBar.ReadColor(_T("TextHighlighted"), m_clrToolBarBtnTextHighlighted);
			tmBar.ReadColor(_T("TextDisabled"), m_clrToolBarBtnTextDisabled);

			if (tmBar.ReadColor(_T("BottomLineColor"), m_clrToolBarBottomLine))
			{
				m_penBottomLine.DeleteObject();
				m_penBottomLine.CreatePen(PS_SOLID, 1, m_clrToolBarBottomLine);
			}

			m_penSeparatorDark.DeleteObject();
			m_penSeparatorDark.CreatePen(PS_SOLID, 1, CDrawingManager::PixelAlpha(m_clrToolBarBottomLine, RGB(255, 255, 255), 95));

			m_penSeparatorLight.DeleteObject();
			m_penSeparatorLight.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		}

		if (tmItem.ExcludeTag(_T("MENUBAR"), strBar))
		{
			CTagManager tmBar(strBar);

			m_clrMenuBarGradientLight = m_clrToolBarGradientLight;
			m_clrMenuBarGradientDark  = m_clrToolBarGradientDark;

			tmBar.ReadColor(_T("GradientLight"), m_clrMenuBarGradientLight);
			tmBar.ReadColor(_T("GradientDark"), m_clrMenuBarGradientDark);

			m_clrMenuBarGradientVertLight = m_clrMenuBarGradientLight;
			m_clrMenuBarGradientVertDark  = m_clrMenuBarGradientDark;

			tmBar.ReadColor(_T("GradientVertLight"), m_clrMenuBarGradientVertLight);
			tmBar.ReadColor(_T("GradientVertDark"), m_clrMenuBarGradientVertDark);

			m_clrMenuBarBtnText            = m_clrToolBarBtnText;
			m_clrMenuBarBtnTextHighlighted = m_clrToolBarBtnTextHighlighted;
			m_clrMenuBarBtnTextDisabled    = m_clrToolBarBtnTextDisabled;
			tmBar.ReadColor(_T("TextNormal"), m_clrMenuBarBtnText);
			tmBar.ReadColor(_T("TextHighlighted"), m_clrMenuBarBtnTextHighlighted);
			tmBar.ReadColor(_T("TextDisabled"), m_clrMenuBarBtnTextDisabled);

			tmBar.ReadControlRenderer(_T("BUTTON"), m_ctrlMenuBarBtn, MakeResourceID(_T("IDB_OFFICE2007_MENUBAR_BTN")));
		}

		if (tmItem.ExcludeTag(_T("POPUPBAR"), strBar))
		{
			CTagManager tmBar(strBar);
			tmBar.ReadControlRenderer(_T("BORDER"), m_ctrlPopupBorder, MakeResourceID(_T("IDB_OFFICE2007_POPUPMENU_BORDER")));

			CString strResize;
			if (tmBar.ExcludeTag(_T("RESIZEBAR"), strResize))
			{
				CTagManager tmResize(strResize);
				tmResize.ReadControlRenderer(_T("BACK"), m_ctrlPopupResizeBar, MakeResourceID(_T("IDB_OFFICE2007_POPUPMENU_RESIZEBAR")));
				tmResize.ReadToolBarImages(_T("ICON_HV"), m_PopupResizeBar_HV, MakeResourceID(_T("IDB_OFFICE2007_POPUPMENU_RESIZEBAR_ICON_HV")));
				tmResize.ReadToolBarImages(_T("ICON_HVT"), m_PopupResizeBar_HVT, MakeResourceID(_T("IDB_OFFICE2007_POPUPMENU_RESIZEBAR_ICON_HVT")));
				tmResize.ReadToolBarImages(_T("ICON_V"), m_PopupResizeBar_V, MakeResourceID(_T("IDB_OFFICE2007_POPUPMENU_RESIZEBAR_ICON_V")));
			}
		}

		if (tmItem.ExcludeTag(_T("STATUSBAR"), strBar))
		{
			CTagManager tmBar(strBar);

			tmBar.ReadControlRenderer(_T("BACK"), m_ctrlStatusBarBack, MakeResourceID(_T("IDB_OFFICE2007_STATUSBAR_BACK")));
			tmBar.ReadControlRenderer(_T("BACK_EXT"), m_ctrlStatusBarBack_Ext, MakeResourceID(_T("IDB_OFFICE2007_STATUSBAR_BACK_EXT")));

			tmBar.ReadToolBarImages(_T("PANEBORDER"), m_StatusBarPaneBorder, MakeResourceID(_T("IDB_OFFICE2007_STATUSBAR_PANEBORDER")));
			tmBar.ReadToolBarImages(_T("SIZEBOX"), m_StatusBarSizeBox, MakeResourceID(_T("IDB_OFFICE2007_STATUSBAR_SIZEBOX")));

			m_clrStatusBarText = m_clrMenuBarBtnText;
			m_clrStatusBarTextDisabled = m_clrMenuBarBtnTextDisabled;
			m_clrExtenedStatusBarTextDisabled = m_clrMenuBarBtnTextDisabled;

			tmBar.ReadColor(_T("TextNormal"), m_clrStatusBarText);
			tmBar.ReadColor(_T("TextDisabled"), m_clrStatusBarTextDisabled);
			tmBar.ReadColor(_T("TextExtendedDisabled"), m_clrExtenedStatusBarTextDisabled);
		}

		if (tmItem.ExcludeTag(_T("CAPTIONBAR"), strBar))
		{
			CTagManager tmBar(strBar);

			tmBar.ReadColor(_T("GradientLight"), m_clrCaptionBarGradientLight);
			tmBar.ReadColor(_T("GradientDark"), m_clrCaptionBarGradientDark);
			tmBar.ReadColor(_T("TextNormal"), m_clrCaptionBarText);
		}
	}

	if (m_clrMenuRarelyUsed == CLR_DEFAULT)
	{
		m_clrMenuRarelyUsed = m_clrBarBkgnd;
	}

	m_brMenuRarelyUsed.DeleteObject();
	m_brMenuRarelyUsed.CreateSolidBrush(m_clrMenuRarelyUsed);

	m_clrEditBorder            = afxGlobalData.clrWindow;
	m_clrEditBorderDisabled    = afxGlobalData.clrBtnShadow;
	m_clrEditBorderHighlighted = m_clrMenuItemBorder;
	m_clrEditSelection         = afxGlobalData.clrHilite;

	// edit
	if (tm.ExcludeTag(_T("EDIT"), strItem))
	{
		CTagManager tmItem(strItem);

		tmItem.ReadColor(_T("BorderNormal"), m_clrEditBorder);
		tmItem.ReadColor(_T("BorderHighlighted"), m_clrEditBorderHighlighted);
		tmItem.ReadColor(_T("BorderDisabled"), m_clrEditBorderDisabled);
		tmItem.ReadColor(_T("Selection"), m_clrEditSelection);
	}

	m_clrComboBorder               = afxGlobalData.clrWindow;
	m_clrComboBorderDisabled       = afxGlobalData.clrBtnShadow;
	m_clrComboBorderHighlighted    = m_clrMenuItemBorder;
	m_clrComboBorderPressed        = m_clrComboBorderHighlighted;
	m_clrComboBtnBorder            = m_clrComboBorder;
	m_clrComboBtnBorderHighlighted = m_clrComboBorderHighlighted;
	m_clrComboBtnBorderPressed     = m_clrComboBorderHighlighted;
	m_clrComboSelection            = afxGlobalData.clrHilite;
	m_clrComboBtnStart             = m_clrToolBarGradientDark;
	m_clrComboBtnFinish            = m_clrToolBarGradientLight;
	m_clrComboBtnDisabledStart     = afxGlobalData.clrBtnFace;
	m_clrComboBtnDisabledFinish    = m_clrComboBtnDisabledStart;
	m_clrComboBtnHighlightedStart  = m_clrHighlightGradientDark;
	m_clrComboBtnHighlightedFinish = m_clrHighlightGradientLight;
	m_clrComboBtnPressedStart      = m_clrHighlightDnGradientDark;
	m_clrComboBtnPressedFinish     = m_clrHighlightDnGradientLight;

	// combobox
	if (tm.ExcludeTag(_T("COMBO"), strItem))
	{
		CTagManager tmItem(strItem);

		tmItem.ReadColor(_T("BorderNormal"), m_clrComboBorder);
		tmItem.ReadColor(_T("BorderHighlighted"), m_clrComboBorderHighlighted);
		tmItem.ReadColor(_T("BorderDisabled"), m_clrComboBorderDisabled);

		m_clrComboBorderPressed = m_clrComboBorderHighlighted;
		tmItem.ReadColor(_T("BorderPressed"), m_clrComboBorderPressed);

		tmItem.ReadColor(_T("Selection"), m_clrComboSelection);

		CString strButton;
		if (tmItem.ExcludeTag(_T("BUTTON"), strButton))
		{
			CTagManager tmButton(strButton);

			tmButton.ReadColor(_T("GradientStartNormal"), m_clrComboBtnStart);
			tmButton.ReadColor(_T("GradientFinishNormal"), m_clrComboBtnFinish);
			tmButton.ReadColor(_T("BtnBorderNormal"), m_clrComboBtnBorder);

			if (!tmButton.ReadControlRenderer(_T("IMAGE"), m_ctrlComboBoxBtn, MakeResourceID(_T("IDB_OFFICE2007_COMBOBOX_BTN"))))
			{
				tmButton.ReadColor(_T("GradientStartHighlighted"), m_clrComboBtnHighlightedStart);
				tmButton.ReadColor(_T("GradientFinishHighlighted"), m_clrComboBtnHighlightedFinish);
				tmButton.ReadColor(_T("GradientStartDisabled"), m_clrComboBtnDisabledStart);
				tmButton.ReadColor(_T("GradientFinishDisabled"), m_clrComboBtnDisabledFinish);
				tmButton.ReadColor(_T("GradientStartPressed"), m_clrComboBtnPressedStart);
				tmButton.ReadColor(_T("GradientFinishPressed"), m_clrComboBtnPressedFinish);

				tmButton.ReadColor(_T("BtnBorderHighlighted"), m_clrComboBtnBorderHighlighted);
				tmButton.ReadColor(_T("BtnBorderDisabled"), m_clrComboBtnBorderDisabled);

				m_clrComboBtnBorderPressed = m_clrComboBtnBorderHighlighted;
				tmButton.ReadColor(_T("BtnBorderPressed"), m_clrComboBtnBorderPressed);
			}
		}
	}

	m_clrRibbonEditBorder            = m_clrEditBorder;
	m_clrRibbonEditBorderDisabled    = m_clrEditBorderDisabled;
	m_clrRibbonEditBorderHighlighted = m_clrEditBorderHighlighted;
	m_clrRibbonEditBorderPressed     = m_clrRibbonEditBorderHighlighted;
	m_clrRibbonEditSelection         = m_clrEditSelection;

	m_clrRibbonComboBtnBorder            = m_clrComboBtnBorder;
	m_clrRibbonComboBtnBorderHighlighted = m_clrComboBtnBorderHighlighted;
	m_clrRibbonComboBtnBorderPressed     = m_clrComboBtnBorderPressed;
	m_clrRibbonComboBtnStart             = m_clrComboBtnStart;
	m_clrRibbonComboBtnFinish            = m_clrComboBtnFinish;
	m_clrRibbonComboBtnDisabledStart     = m_clrComboBtnDisabledStart;
	m_clrRibbonComboBtnDisabledFinish    = m_clrComboBtnDisabledFinish;
	m_clrRibbonComboBtnHighlightedStart  = m_clrComboBtnHighlightedStart;
	m_clrRibbonComboBtnHighlightedFinish = m_clrComboBtnHighlightedFinish;
	m_clrRibbonComboBtnPressedStart      = m_clrComboBtnPressedStart;
	m_clrRibbonComboBtnPressedFinish     = m_clrComboBtnPressedFinish;

	// task pane
	m_clrTaskPaneGradientDark       = m_clrBarGradientLight;
	m_clrTaskPaneGradientLight      = m_clrTaskPaneGradientDark;

	if (tm.ExcludeTag(_T("TASK"), strItem))
	{
		CTagManager tmItem(strItem);

		tmItem.ReadColor(_T("GradientDark"), m_clrTaskPaneGradientDark);
		tmItem.ReadColor(_T("GradientLight"), m_clrTaskPaneGradientLight);

		CString strGroup;
		if (tmItem.ExcludeTag(_T("GROUP"), strGroup))
		{
			CTagManager tmGroup(strGroup);

			CString strState;
			if (tmGroup.ExcludeTag(_T("NORMAL"), strState))
			{
				CTagManager tmState(strState);

				CString str;

				if (tmState.ExcludeTag(_T("CAPTION"), str))
				{
					CTagManager tmCaption(str);

					tmCaption.ReadColor(_T("DarkNormal"), m_clrTaskPaneGroupCaptionDark);
					tmCaption.ReadColor(_T("LightNormal"), m_clrTaskPaneGroupCaptionLight);
					tmCaption.ReadColor(_T("DarkHighlighted"), m_clrTaskPaneGroupCaptionHighDark);
					tmCaption.ReadColor(_T("LightHighlighted"), m_clrTaskPaneGroupCaptionHighLight);
					tmCaption.ReadColor(_T("TextNormal"), m_clrTaskPaneGroupCaptionText);
					tmCaption.ReadColor(_T("TextHighlighted"), m_clrTaskPaneGroupCaptionTextHigh);
				}

				if (tmState.ExcludeTag(_T("AREA"), str))
				{
					CTagManager tmArea(str);

					tmArea.ReadColor(_T("DarkNormal"), m_clrTaskPaneGroupAreaDark);
					tmArea.ReadColor(_T("LightNormal"), m_clrTaskPaneGroupAreaLight);
				}
			}

			if (tmGroup.ExcludeTag(_T("SPECIAL"), strState))
			{
				CTagManager tmState(strState);

				CString str;
				if (tmState.ExcludeTag(_T("CAPTION"), str))
				{
					CTagManager tmCaption(str);

					tmCaption.ReadColor(_T("DarkNormal"), m_clrTaskPaneGroupCaptionSpecDark);
					tmCaption.ReadColor(_T("LightNormal"), m_clrTaskPaneGroupCaptionSpecLight);
					tmCaption.ReadColor(_T("DarkHighlighted"), m_clrTaskPaneGroupCaptionHighSpecDark);
					tmCaption.ReadColor(_T("LightHighlighted"), m_clrTaskPaneGroupCaptionHighSpecLight);
					tmCaption.ReadColor(_T("TextNormal"), m_clrTaskPaneGroupCaptionTextSpec);
					tmCaption.ReadColor(_T("TextHighlighted"), m_clrTaskPaneGroupCaptionTextHighSpec);
				}

				if (tmState.ExcludeTag(_T("AREA"), str))
				{
					CTagManager tmArea(str);

					tmArea.ReadColor(_T("DarkNormal"), m_clrTaskPaneGroupAreaSpecDark);
					tmArea.ReadColor(_T("LightNormal"), m_clrTaskPaneGroupAreaSpecLight);
				}
			}

			if (tmGroup.ReadColor(_T("BORDER"), m_clrTaskPaneGroupBorder))
			{
				m_penTaskPaneGroupBorder.DeleteObject();
				m_penTaskPaneGroupBorder.CreatePen(PS_SOLID, 1, m_clrTaskPaneGroupBorder);
			}
		}

		tmItem.ReadControlRenderer(_T("SCROLL_BUTTON"), m_ctrlTaskScrollBtn, MakeResourceID(_T("IDB_OFFICE2007_TASKPANE_SCROLL_BTN")));
	}

	if (tm.ExcludeTag(_T("TABS"), strItem))
	{
		CTagManager tmItem(strItem);

		tmItem.ReadColor(_T("TextColorActive"), m_clrTabTextActive);
		tmItem.ReadColor(_T("TextColorInactive"), m_clrTabTextInactive);

		CString strTab;
		if (tmItem.ExcludeTag(_T("3D"), strTab))
		{
			CTagManager tmTab(strTab);

			CString strBtn;
			if (tmTab.ExcludeTag(_T("BUTTON"), strBtn))
			{
				CMFCControlRendererInfo params(MakeResourceID(_T("IDB_OFFICE2007_TAB_3D")), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0));
				if (CTagManager::ParseControlRendererInfo(strBtn, params))
				{
					m_ctrlTab3D[0].Create(params);
					m_ctrlTab3D[1].Create(params, TRUE);
				}
			}
		}

		if (tmItem.ExcludeTag(_T("FLAT"), strTab))
		{
			CTagManager tmTab(strTab);

			CString strBtn;
			if (tmTab.ExcludeTag(_T("BUTTON"), strBtn))
			{
				CMFCControlRendererInfo params(MakeResourceID(_T("IDB_OFFICE2007_TAB_FLAT")), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0));
				if (CTagManager::ParseControlRendererInfo(strBtn, params))
				{
					m_ctrlTabFlat[0].Create(params);
					m_ctrlTabFlat[1].Create(params, TRUE);
				}
			}

			tmTab.ReadColor(_T("Black"), m_clrTabFlatBlack);
			tmTab.ReadColor(_T("Highlight"), m_clrTabFlatHighlight);

			COLORREF clr;
			if (tmTab.ReadColor(_T("BorderInnerNormal"), clr))
			{
				m_penTabFlatInner[0].DeleteObject();
				m_penTabFlatInner[0].CreatePen(PS_SOLID, 1, clr);
			}
			if (tmTab.ReadColor(_T("BorderInnerActive"), clr))
			{
				m_penTabFlatInner[1].DeleteObject();
				m_penTabFlatInner[1].CreatePen(PS_SOLID, 1, clr);
			}
			if (tmTab.ReadColor(_T("BorderOuterNormal"), clr))
			{
				m_penTabFlatOuter[0].DeleteObject();
				m_penTabFlatOuter[0].CreatePen(PS_SOLID, 1, clr);
			}
			if (tmTab.ReadColor(_T("BorderOuterActive"), clr))
			{
				m_penTabFlatOuter[1].DeleteObject();
				m_penTabFlatOuter[1].CreatePen(PS_SOLID, 1, clr);
			}
		}
	}

	if (tm.ExcludeTag(_T("HEADER"), strItem))
	{
		CTagManager tmItem(strItem);

		tmItem.ReadColor(_T("NormalStart"), m_clrHeaderNormalStart);
		tmItem.ReadColor(_T("NormalFinish"), m_clrHeaderNormalFinish);
		tmItem.ReadColor(_T("NormalBorder"), m_clrHeaderNormalBorder);
		tmItem.ReadColor(_T("HighlightedStart"), m_clrHeaderHighlightedStart);
		tmItem.ReadColor(_T("HighlightedFinish"), m_clrHeaderHighlightedFinish);
		tmItem.ReadColor(_T("HighlightedBorder"), m_clrHeaderHighlightedBorder);
		tmItem.ReadColor(_T("PressedStart"), m_clrHeaderPressedStart);
		tmItem.ReadColor(_T("PressedFinish"), m_clrHeaderPressedFinish);
		tmItem.ReadColor(_T("PressedBorder"), m_clrHeaderPressedBorder);
	}

	m_clrRibbonCategoryText                = m_clrMenuBarBtnText;
	m_clrRibbonCategoryTextHighlighted     = m_clrMenuBarBtnTextHighlighted;
	m_clrRibbonCategoryTextDisabled		   = m_clrMenuBarBtnTextDisabled;
	m_clrRibbonPanelText                   = m_clrToolBarBtnText;
	m_clrRibbonPanelTextHighlighted        = m_clrToolBarBtnTextHighlighted;
	m_clrRibbonPanelCaptionText            = m_clrRibbonPanelText;
	m_clrRibbonPanelCaptionTextHighlighted = m_clrRibbonPanelTextHighlighted;

	m_clrRibbonEdit                        = afxGlobalData.clrBarLight;
	m_clrRibbonEditHighlighted             = afxGlobalData.clrWindow;
	m_clrRibbonEditPressed                 = m_clrRibbonEditHighlighted;
	m_clrRibbonEditDisabled                = afxGlobalData.clrBtnFace;

	if (tm.ExcludeTag(_T("RIBBON"), strItem))
	{
		CTagManager tmItem(strItem);

		CString str;

		if (tmItem.ExcludeTag(_T("CATEGORY"), str))
		{
			CTagManager tmCategory(str);
			tmCategory.ReadControlRenderer(_T("BACK"), m_ctrlRibbonCategoryBack, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CATEGORY_BACK")));

			CString strTab;
			if (tmCategory.ExcludeTag(_T("TAB"), strTab))
			{
				CTagManager tmTab(strTab);
				tmTab.ReadControlRenderer(_T("BUTTON"), m_ctrlRibbonCategoryTab, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CATEGORY_TAB")));

				tmTab.ReadColor(_T("TextNormal"), m_clrRibbonCategoryText);
				tmTab.ReadColor(_T("TextHighlighted"), m_clrRibbonCategoryTextHighlighted);
				tmTab.ReadColor(_T("TextDisabled"), m_clrRibbonCategoryTextDisabled);
			}

			tmCategory.ReadControlRenderer(_T("TAB_SEPARATOR"), m_ctrlRibbonCategoryTabSep, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CATEGORY_TAB_SEP")));

			tmCategory.ReadControlRenderer(_T("BUTTON_PAGE_L"), m_ctrlRibbonCategoryBtnPage[0], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_PAGE_L")));
			tmCategory.ReadControlRenderer(_T("BUTTON_PAGE_R"), m_ctrlRibbonCategoryBtnPage[1], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_PAGE_R")));
		}

		if (tmItem.ExcludeTag(_T("PANEL"), str))
		{
			CTagManager tmPanel(str);

			{
				CString strBack;
				if (tmPanel.ExcludeTag(_T("BACK"), strBack))
				{
					CTagManager tmBack(strBack);

					tmBack.ReadControlRenderer(_T("TOP"), m_ctrlRibbonPanelBack_T, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_PANEL_BACK_T")));
					tmBack.ReadControlRenderer(_T("BOTTOM"), m_ctrlRibbonPanelBack_B, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_PANEL_BACK_B")));
				}
			}

			{
				CString strCaption;
				if (tmPanel.ExcludeTag(_T("CAPTION"), strCaption))
				{
					CTagManager tmCaption(strCaption);

					tmCaption.ReadControlRenderer(_T("LAUNCH_BTN"), m_ctrlRibbonBtnLaunch, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_LAUNCH")));
					tmCaption.ReadToolBarImages(_T("LAUNCH_ICON"), m_RibbonBtnLaunchIcon, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_LAUNCH_ICON")));
					tmCaption.ReadColor(_T("TextNormal"), m_clrRibbonPanelCaptionText);
					tmCaption.ReadColor(_T("TextHighlighted"), m_clrRibbonPanelCaptionTextHighlighted);
				}
			}

			tmPanel.ReadToolBarImages(_T("SEPARATOR"), m_RibbonPanelSeparator, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_PANEL_SEPARATOR")));
			tmPanel.ReadControlRenderer(_T("QAT"), m_ctrlRibbonPanelQAT, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_PANEL_QAT")));

			{
				CString strButtons;
				if (tmPanel.ExcludeTag(_T("BUTTONS"), strButtons))
				{
					CTagManager tmButtons(strButtons);

					tmButtons.ReadControlRenderer(_T("BUTTON_GROUP_F"), m_ctrlRibbonBtnGroup_F, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_GROUP_F")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUP_M"), m_ctrlRibbonBtnGroup_M, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_GROUP_M")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUP_L"), m_ctrlRibbonBtnGroup_L, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_GROUP_L")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUP_S"), m_ctrlRibbonBtnGroup_S, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_GROUP_S")));

					tmButtons.ReadControlRenderer(_T("BUTTON_GROUPMENU_F_C"), m_ctrlRibbonBtnGroupMenu_F[0], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_F_C")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUPMENU_F_M"), m_ctrlRibbonBtnGroupMenu_F[1], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_F_M")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUPMENU_M_C"), m_ctrlRibbonBtnGroupMenu_M[0], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_M_C")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUPMENU_M_M"), m_ctrlRibbonBtnGroupMenu_M[1], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_M_M")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUPMENU_L_C"), m_ctrlRibbonBtnGroupMenu_L[0], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_L_C")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUPMENU_L_M"), m_ctrlRibbonBtnGroupMenu_L[1], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_L_M")));

					tmButtons.ReadControlRenderer(_T("BUTTON_NORMAL_S"), m_ctrlRibbonBtn[0], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_NORMAL_S")));
					tmButtons.ReadControlRenderer(_T("BUTTON_NORMAL_B"), m_ctrlRibbonBtn[1], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_NORMAL_B")));

					tmButtons.ReadControlRenderer(_T("BUTTON_DEFAULT"), m_ctrlRibbonBtnDefault, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_DEFAULT")));
					tmButtons.ReadControlRenderer(_T("BUTTON_DEFAULT_ICON"), m_ctrlRibbonBtnDefaultIcon, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_DEFAULT_ICON")));
					tmButtons.ReadToolBarImages(_T("BUTTON_DEFAULT_IMAGE"), m_RibbonBtnDefaultImage, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_DEFAULT_IMAGE")));
					tmButtons.ReadControlRenderer(_T("BUTTON_DEFAULT_QAT"), m_ctrlRibbonBtnDefaultQAT, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_DEFAULT_QAT")));

					if (!m_ctrlRibbonBtnDefaultQAT.IsValid())
					{
						tmButtons.ReadControlRenderer(_T("BUTTON_DEFAULT_QAT_ICON"), m_ctrlRibbonBtnDefaultQATIcon, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_DEFAULT_QAT_ICON")));
					}

					tmButtons.ReadControlRenderer(_T("BUTTON_MENU_H_C"), m_ctrlRibbonBtnMenuH[0], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_MENU_H_C")));
					tmButtons.ReadControlRenderer(_T("BUTTON_MENU_H_M"), m_ctrlRibbonBtnMenuH[1], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_MENU_H_M")));
					tmButtons.ReadControlRenderer(_T("BUTTON_MENU_V_C"), m_ctrlRibbonBtnMenuV[0], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_MENU_V_C")));
					tmButtons.ReadControlRenderer(_T("BUTTON_MENU_V_M"), m_ctrlRibbonBtnMenuV[1], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_MENU_V_M")));
					tmButtons.ReadControlRenderer(_T("BUTTON_CHECK"), m_ctrlRibbonBtnCheck, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_CHECK")));

					tmButtons.ReadControlRenderer(_T("BUTTON_PNL_T"), m_ctrlRibbonBtnPalette[0], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_PALETTE_T")));
					tmButtons.ReadControlRenderer(_T("BUTTON_PNL_M"), m_ctrlRibbonBtnPalette[1], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_PALETTE_M")));
					tmButtons.ReadControlRenderer(_T("BUTTON_PNL_B"), m_ctrlRibbonBtnPalette[2], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_PALETTE_B")));
				}
			}

			{
				CString strEdit;
				if (tmPanel.ExcludeTag(_T("EDIT"), strEdit))
				{
					CTagManager tmEdit(strEdit);

					tmEdit.ReadColor(_T("Normal"), m_clrRibbonEdit);
					tmEdit.ReadColor(_T("Highlighted"), m_clrRibbonEditHighlighted);
					tmEdit.ReadColor(_T("Disabled"), m_clrRibbonEditDisabled);
					tmEdit.ReadColor(_T("Pressed"), m_clrRibbonEditPressed);

					tmEdit.ReadColor(_T("BorderNormal"), m_clrRibbonEditBorder);
					tmEdit.ReadColor(_T("BorderHighlighted"), m_clrRibbonEditBorderHighlighted);
					tmEdit.ReadColor(_T("BorderDisabled"), m_clrRibbonEditBorderDisabled);
					tmEdit.ReadColor(_T("BorderPressed"), m_clrRibbonEditBorderPressed);
					tmEdit.ReadColor(_T("Selection"), m_clrRibbonEditSelection);

					CString strButton;
					if (tmEdit.ExcludeTag(_T("BUTTON"), strButton))
					{
						CTagManager tmButton(strButton);

						tmButton.ReadColor(_T("GradientStartNormal"), m_clrRibbonComboBtnStart);
						tmButton.ReadColor(_T("GradientFinishNormal"), m_clrRibbonComboBtnFinish);
						tmButton.ReadColor(_T("BtnBorderNormal"), m_clrRibbonComboBtnBorder);

						if (!tmButton.ReadControlRenderer(_T("IMAGE"), m_ctrlRibbonComboBoxBtn, MakeResourceID(_T("IDB_OFFICE2007_COMBOBOX_BTN"))))
						{
							tmButton.ReadColor(_T("GradientStartHighlighted"), m_clrRibbonComboBtnHighlightedStart);
							tmButton.ReadColor(_T("GradientFinishHighlighted"), m_clrRibbonComboBtnHighlightedFinish);
							tmButton.ReadColor(_T("GradientStartDisabled"), m_clrRibbonComboBtnDisabledStart);
							tmButton.ReadColor(_T("GradientFinishDisabled"), m_clrRibbonComboBtnDisabledFinish);
							tmButton.ReadColor(_T("GradientStartPressed"), m_clrRibbonComboBtnPressedStart);
							tmButton.ReadColor(_T("GradientFinishPressed"), m_clrRibbonComboBtnPressedFinish);

							tmButton.ReadColor(_T("BtnBorderHighlighted"), m_clrRibbonComboBtnBorderHighlighted);
							tmButton.ReadColor(_T("BtnBorderDisabled"), m_clrRibbonComboBtnBorderDisabled);

							m_clrRibbonComboBtnBorderPressed = m_clrRibbonComboBtnBorderHighlighted;
							tmButton.ReadColor(_T("BtnBorderPressed"), m_clrRibbonComboBtnBorderPressed);
						}
					}
				}
			}

			tmPanel.ReadColor(_T("TextNormal"), m_clrRibbonPanelText);
			tmPanel.ReadColor(_T("TextHighlighted"), m_clrRibbonPanelTextHighlighted);
		}

		if (tmItem.ExcludeTag(_T("CONTEXT"), str))
		{
			CTagManager tmContext(str);

			CString strCategory;
			if (tmContext.ExcludeTag(_T("CATEGORY"), strCategory))
			{
				CTagManager tmCategory(strCategory);

				CMFCControlRendererInfo prBack;
				CMFCControlRendererInfo prCaption;
				CMFCControlRendererInfo prTab;
				CMFCControlRendererInfo prDefault;
				COLORREF clrText = m_clrRibbonCategoryText;
				COLORREF clrTextHighlighted = m_clrRibbonCategoryTextHighlighted;
				COLORREF clrCaptionText = clrText;

				tmCategory.ReadControlRendererInfo(_T("BACK"), prBack);

				CString strTab;
				if (tmCategory.ExcludeTag(_T("TAB"), strTab))
				{
					CTagManager tmTab(strTab);

					tmTab.ReadControlRendererInfo(_T("BUTTON"), prTab);
					tmTab.ReadColor(_T("TextNormal"), clrText);
					tmTab.ReadColor(_T("TextHighlighted"), clrTextHighlighted);
				}

				CString strCaption;
				if (tmCategory.ExcludeTag(_T("CAPTION"), strCaption))
				{
					CTagManager tmCaption(strCaption);

					tmCaption.ReadControlRendererInfo(_T("BACK"), prCaption);
					tmCaption.ReadColor(_T("TextNormal"), clrCaptionText);
				}

				tmCategory.ReadControlRendererInfo(_T("BUTTON_DEFAULT"), prDefault);

				CString strID[AFX_RIBBON_CATEGORY_COLOR_COUNT] =
				{
					MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CONTEXT_R_")),
					MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CONTEXT_O_")),
					MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CONTEXT_Y_")),
					MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CONTEXT_G_")),
					MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CONTEXT_B_")),
					MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CONTEXT_I_")),
					MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CONTEXT_V_"))
				};

				for (int i = 0; i < AFX_RIBBON_CATEGORY_COLOR_COUNT; i++)
				{
					CMFCRibbonContextCategory& cat = m_ctrlRibbonContextCategory[i];

					prDefault.m_strBmpResID = strID[i] + _T("BTN_DEFAULT");
					prTab.m_strBmpResID     = strID[i] + _T("CATEGORY_TAB");
					prCaption.m_strBmpResID = strID[i] + _T("CATEGORY_CAPTION");
					prBack.m_strBmpResID    = strID[i] + _T("CATEGORY_BACK");

					cat.m_ctrlBtnDefault.Create(prDefault);
					cat.m_ctrlCaption.Create(prCaption);
					cat.m_ctrlTab.Create(prTab);
					cat.m_ctrlBack.Create(prBack);
					cat.m_clrText            = clrText;
					cat.m_clrTextHighlighted = clrTextHighlighted;
					cat.m_clrCaptionText     = clrCaptionText;
				}
			}

			CString strPanel;
			if (tmContext.ExcludeTag(_T("PANEL"), strPanel))
			{
				CTagManager tmPanel(strPanel);

				CString strBack;
				if (tmPanel.ExcludeTag(_T("BACK"), strBack))
				{
					CTagManager tmBack(strBack);

					tmBack.ReadControlRenderer(_T("TOP"), m_ctrlRibbonContextPanelBack_T, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CONTEXT_PANEL_BACK_T")));
					tmBack.ReadControlRenderer(_T("BOTTOM"), m_ctrlRibbonContextPanelBack_B, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CONTEXT_PANEL_BACK_B")));
				}

				CString strCaption;
				if (tmPanel.ExcludeTag(_T("CAPTION"), strCaption))
				{
					CTagManager tmCaption(strCaption);

					tmCaption.ReadColor(_T("TextNormal"), m_clrRibbonContextPanelCaptionText);
					tmCaption.ReadColor(_T("TextHighlighted"), m_clrRibbonContextPanelCaptionTextHighlighted);
				}

				tmPanel.ReadColor(_T("TextNormal"), m_clrRibbonContextPanelText);
				tmPanel.ReadColor(_T("TextHighlighted"), m_clrRibbonContextPanelTextHighlighted);
			}

			tmContext.ReadControlRenderer(_T("SEPARATOR"), m_ctrlRibbonContextSeparator, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CONTEXT_SEPARATOR")));
		}

		tmItem.ReadControlRenderer(_T("MAIN_BUTTON"), m_RibbonBtnMain, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_MAIN")));

		if (tmItem.ExcludeTag(_T("MAIN"), str))
		{
			CTagManager tmMain(str);
			tmMain.ReadControlRenderer(_T("BACK"), m_ctrlRibbonMainPanel, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_PANEL_MAIN")));
			tmMain.ReadControlRenderer(_T("BORDER"), m_ctrlRibbonMainPanelBorder, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_PANEL_MAIN_BORDER")));
			tmMain.ReadControlRenderer(_T("BUTTON"), m_ctrlRibbonBtnMainPanel, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_PANEL_MAIN")));
		}

		if (tmItem.ExcludeTag(_T("CAPTION"), str))
		{
			CTagManager tmCaption(str);
			tmCaption.ReadControlRenderer(_T("QA"), m_ctrlRibbonCaptionQA, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CAPTION_QA")));
			tmCaption.ReadControlRenderer(_T("QA_GLASS"), m_ctrlRibbonCaptionQA_Glass, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_CAPTION_QA_GLASS")));
		}

		if (tmItem.ExcludeTag(_T("STATUS"), str))
		{
			CTagManager tmStatus(str);
			tmStatus.ReadControlRenderer(_T("PANE_BUTTON"), m_ctrlRibbonBtnStatusPane, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_STATUS_PANE")));

			CString strSlider;
			if (tmStatus.ExcludeTag(_T("SLIDER"), strSlider))
			{
				CTagManager tmSlider(strSlider);

				tmSlider.ReadControlRenderer(_T("THUMB"), m_ctrlRibbonSliderThumb, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_SLIDER_THUMB")));
				tmSlider.ReadControlRenderer(_T("PLUS"), m_ctrlRibbonSliderBtnPlus, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_SLIDER_BTN_PLUS")));
				tmSlider.ReadControlRenderer(_T("MINUS"), m_ctrlRibbonSliderBtnMinus, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_SLIDER_BTN_MINUS")));
			}

			CString strProgress;
			if (tmStatus.ExcludeTag(_T("PROGRESS"), strProgress))
			{
				CTagManager tmProgress(strProgress);

				tmProgress.ReadControlRenderer(_T("BACK"), m_ctrlRibbonProgressBack, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_PROGRESS_BACK")));
				tmProgress.ReadControlRenderer(_T("NORMAL"), m_ctrlRibbonProgressNormal, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_PROGRESS_NORMAL")));
				tmProgress.ReadControlRenderer(_T("NORMAL_EXT"), m_ctrlRibbonProgressNormalExt, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_PROGRESS_NORMAL_EXT")));
				tmProgress.ReadControlRenderer(_T("INFINITY"), m_ctrlRibbonProgressInfinity, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_PROGRESS_INFINITY")));
			}
		}

		if (tmItem.ExcludeTag(_T("BORDERS"), str))
		{
			CTagManager tmBorders(str);

			tmBorders.ReadControlRenderer(_T("QAT"), m_ctrlRibbonBorder_QAT, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BORDER_QAT")));
			tmBorders.ReadControlRenderer(_T("FLOATY"), m_ctrlRibbonBorder_Floaty, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BORDER_FLOATY")));
		}

		if (tmItem.ExcludeTag(_T("KEYTIP"), str))
		{
			CTagManager tmKeyTip(str);

			tmKeyTip.ReadControlRenderer(_T("BACK"), m_ctrlRibbonKeyTip, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_KEYTIP_BACK")));
			tmKeyTip.ReadColor(_T("TextNormal"), m_clrRibbonKeyTipTextNormal);

			BOOL bSystem = FALSE;
			if (m_clrRibbonKeyTipTextNormal == (COLORREF)(-1))
			{
				if (m_bToolTipParams && m_ToolTipParams.m_clrText != (COLORREF)(-1))
				{
					m_clrRibbonKeyTipTextNormal = m_ToolTipParams.m_clrText;
				}
				else
				{
					bSystem = TRUE;
					m_clrRibbonKeyTipTextNormal = ::GetSysColor(COLOR_INFOTEXT);
				}
			}

			tmKeyTip.ReadColor(_T("TextDisabled"), m_clrRibbonKeyTipTextDisabled);

			if (m_clrRibbonKeyTipTextDisabled == (COLORREF)(-1))
			{
				if (bSystem)
				{
					m_clrRibbonKeyTipTextDisabled = afxGlobalData.clrGrayedText;
				}
				else
				{
					m_clrRibbonKeyTipTextDisabled = CDrawingManager::PixelAlpha(
						m_clrRibbonKeyTipTextNormal, afxGlobalData.clrWindow, 50);
				}
			}
		}

		if (tmItem.ExcludeTag(_T("HYPERLINK"), str))
		{
			CTagManager tmHyperlink(str);

			tmHyperlink.ReadColor(_T("Inactive"), m_clrRibbonHyperlinkInactive);
			tmHyperlink.ReadColor(_T("Active"), m_clrRibbonHyperlinkActive);
			tmHyperlink.ReadColor(_T("StatusbarInactive"), m_clrRibbonStatusbarHyperlinkInactive);
			tmHyperlink.ReadColor(_T("StatusbarActive"), m_clrRibbonStatusbarHyperlinkActive);
		}
	}

	m_clrOutlookCaptionTextNormal   = m_clrCaptionBarText;
	m_clrOutlookPageTextNormal      = m_clrOutlookCaptionTextNormal;
	m_clrOutlookPageTextHighlighted = m_clrOutlookPageTextNormal;
	m_clrOutlookPageTextPressed     = m_clrOutlookPageTextNormal;

	if (tm.ExcludeTag(_T("OUTLOOK"), strItem))
	{
		CTagManager tmItem(strItem);

		CString str;
		if (tmItem.ExcludeTag(_T("CAPTION"), str))
		{
			CTagManager tmCaption(str);

			tmCaption.ReadColor(_T("TextNormal"), m_clrOutlookCaptionTextNormal);
		}

		if (tmItem.ExcludeTag(_T("PAGEBUTTON"), str))
		{
			CTagManager tmPage(str);

			tmPage.ReadControlRenderer(_T("BACK"), m_ctrlOutlookWndPageBtn, MakeResourceID(_T("IDB_OFFICE2007_OUTLOOK_BTN_PAGE")));

			tmPage.ReadColor(_T("TextNormal"), m_clrOutlookPageTextNormal);
			tmPage.ReadColor(_T("TextHighlighted"), m_clrOutlookPageTextHighlighted);
			tmPage.ReadColor(_T("TextPressed"), m_clrOutlookPageTextPressed);
		}

		if (tmItem.ExcludeTag(_T("BAR"), str))
		{
			CTagManager tmBar(str);

			tmBar.ReadControlRenderer(_T("BACK"), m_ctrlOutlookWndBar, MakeResourceID(_T("IDB_OFFICE2007_OUTLOOK_BAR_BACK")));
		}
	}

	// Popup Window:
	m_clrPopupGradientLight = m_clrBarGradientLight;
	m_clrPopupGradientDark = m_clrBarGradientDark;

	if (tm.ExcludeTag(_T("POPUP"), strItem))
	{
		CTagManager tmItem(strItem);

		tmItem.ReadColor(_T("GradientFillLight"), m_clrPopupGradientLight);
		tmItem.ReadColor(_T("GradientFillDark"), m_clrPopupGradientDark);
	}

	if (hinstResOld != NULL)
	{
		AfxSetResourceHandle(hinstResOld);
	}
}


///////////////////////////////////////////////////////
CMSMVisualManager::~CMSMVisualManager()
{
}

void CMSMVisualManager::SetupMemoryBitmapSize (CDC* pDC, const CSize& size)
{
	if (pDC == NULL && size == CSize (0, 0))
	{
		return;
	}

	if (m_MemoryDC.GetSafeHdc () != NULL)
	{
		if (pDC->GetDeviceCaps (BITSPIXEL) != m_MemoryDC.GetDeviceCaps (BITSPIXEL))
		{
			m_MemoryBitmap.DeleteObject ();
			m_MemoryDC.DeleteDC ();
		}
	}

	if (m_MemoryDC.GetSafeHdc () == NULL)
	{
		m_MemoryDC.CreateCompatibleDC (pDC);
	}

	if (m_MemorySize != size || m_MemoryBitmap.GetSafeHandle () == NULL)
	{
		m_MemorySize = size;

		if (m_MemoryBitmap.GetSafeHandle () != NULL)
		{
			m_MemoryBitmap.DeleteObject ();
		}

		m_MemoryBitmap.CreateCompatibleBitmap (pDC, m_MemorySize.cx, m_MemorySize.cy);
		m_MemoryDC.SelectObject (m_MemoryBitmap);

		m_MemoryFilled = false;
	}

	if (m_MSMFrameBitmap.GetSafeHandle () != NULL)
	{
		m_MemoryFilled = _StretchBitmap (&m_MemoryDC, CRect (CPoint (0, 0), m_MemorySize), m_MSMFrameBitmap);
	}
}

void CMSMVisualManager::OnFillBarBackground (CDC* pDC, CBasePane* pBar,
		CRect rectClient, CRect rectClip, BOOL /*bNCArea*/)
{
	//vDrawLogo(*pDC);
	//return;
	if (m_MSMFrameBitmap.GetSafeHandle () == NULL)
	{
		m_MSMFrameBitmap.LoadBitmap (_T("IDB_MSM_FRAME_BKGND"));

		if (!m_MemoryFilled)
		{
			m_MemoryFilled = 
				_StretchBitmap (&m_MemoryDC, CRect (CPoint (0, 0), m_MemorySize), m_MSMFrameBitmap);
			//vDrawLogo(m_MemoryDC);
		}
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);

	if (rectClip.IsRectEmpty ())
	{
		rectClip = rectClient;
	}

	CRuntimeClass* pBarClass = pBar->GetRuntimeClass ();

	if (pBarClass == NULL || pBarClass->IsDerivedFrom (RUNTIME_CLASS (CMFCPopupMenuBar)))
	{
		pDC->FillRect (rectClip, &m_brMenuLight);

		return;
	}

	BOOL bDefault = true;

	if (pBarClass != NULL &&
		(pBarClass->IsDerivedFrom (RUNTIME_CLASS (CMSMCaptionBar)) 
        ||
		 //pBarClass->IsDerivedFrom (RUNTIME_CLASS (CMSMMenuBar)) ||
		 pBarClass->IsDerivedFrom (RUNTIME_CLASS (CMSMToolBar))
		))
	{
		//if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CMSMLinksBar)))
		//{
		//	pDC->FillRect (rectClip, &m_brLinksBarBack);
		//	return;
		//}

		if (!pBar->IsFloating ())
		{
			CRect rt (rectClient);

			CWnd* pWnd = pBar->GetTopLevelFrame ();
			if (pWnd != NULL)
			{
				CRect rtW;
				pBar->GetWindowRect (rtW);
				pWnd->ScreenToClient (rtW);

				rt.OffsetRect (rtW.TopLeft ());
			}

			int nOldStretchBltMode = pDC->SetStretchBltMode (COLORONCOLOR);

			pDC->BitBlt(rectClient.left, rectClient.top,
						rectClient.Width (), rectClient.Height (), 
						&m_MemoryDC, 
						rt.left, rt.top, 
						SRCCOPY);
            if (rectClient.Width() > TOOLBAR_WIDTH)
			    vDrawLogo(m_MemoryDC, rectClient);
			pDC->SetStretchBltMode (nOldStretchBltMode);

			bDefault = false;

			//if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CMSMCategoryBar)))
			//{
			//	CDrawingManager dm (*pDC);

			//	CRect rt (rectClient);
			//	rt.top += 3;

			//	rt.bottom = rt.top + 1;
			//	dm.HighlightRect (rt, 15, (COLORREF)-1, 0, RGB (255, 255, 255));

			//	rt.top++;
			//	rt.bottom++;
			//	dm.HighlightRect (rt, 40, (COLORREF)-1, 0, RGB (255, 255, 255));

			//	rt.top++;
			//	rt.bottom = rectClient.bottom - 1;
			//	dm.HighlightRect (rt, 30, (COLORREF)-1, 0, RGB (255, 255, 255));

			//	rt.top = rt.bottom;
			//	rt.bottom++;
			//	dm.HighlightRect (rt, 10, (COLORREF)-1, 0, RGB (255, 255, 255));
			//}
		}
	}
	if (bDefault)
	{
		CMFCVisualManagerOffice2007::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
	}
}

void CMSMVisualManager::MSMDrawCaptionButton (CDC* pDC, CRect rect, 
										   AFX_BUTTON_STATE state, UINT id)
{
	int nState = CBS_NORMAL;

	int nPart = 0;

	if (m_CaptionButtonIconst.GetSafeHandle () != NULL &&
		m_CaptionButtonIconst.GetImageCount () >= 12)
	{
		long index = 0;

		switch (id)
		{
		case SC_CLOSE:
			index = 9;
			break;
		case SC_MINIMIZE:
			index = 0;
			break;
		case SC_MAXIMIZE:
			index = 3;
			break;
		case SC_RESTORE:
			index = 6;
			break;
		default:
			ASSERT (FALSE);
		}

		switch (state)
		{
		case ButtonsIsHighlighted:
			index += 1;
			break;
		case ButtonsIsPressed:
			index += 2;
			break;
		}

		m_CaptionButtonIconst.Draw (pDC, index, rect.TopLeft (), ILD_NORMAL);

		return;
	}

	if (m_pfDrawThemeBackground == NULL || m_hThemeWindow == NULL)
	{
		switch (state)
		{
		case ButtonsIsRegular:
		case ButtonsIsHighlighted:
			nState = 0;
			break;
		case ButtonsIsPressed:
			nState = DFCS_PUSHED;
			break;
		}

		switch (id)
		{
		case SC_CLOSE:
			nPart = DFCS_CAPTIONCLOSE;
			break;
		case SC_MINIMIZE:
			nPart = DFCS_CAPTIONMIN;
			break;
		case SC_MAXIMIZE:
			nPart = DFCS_CAPTIONMAX;
			break;
		case SC_RESTORE:
			nPart = DFCS_CAPTIONRESTORE;
			break;
		default:
			ASSERT (FALSE);
		}

		pDC->DrawFrameControl (rect, DFC_CAPTION, nPart | nState);
		return;
	}

	switch (state)
	{
	case ButtonsIsRegular:
		nState = CBS_NORMAL;
		break;
	case ButtonsIsPressed:
		nState = CBS_PUSHED;
		break;
	case ButtonsIsHighlighted:
		nState = CBS_HOT;
		break;
	}

	switch (id)
	{
	case SC_CLOSE:
		nPart = WP_CLOSEBUTTON;
		break;
	case SC_MINIMIZE:
		nPart = WP_MINBUTTON;
		break;
	case SC_MAXIMIZE:
		nPart = WP_MAXBUTTON;
		break;
	case SC_RESTORE:
		nPart = WP_RESTOREBUTTON;
		break;
	default:
		ASSERT (FALSE);
	}

	(*m_pfDrawThemeBackground) (m_hThemeWindow, pDC->GetSafeHdc(), nPart,
		nState, &rect, 0);
}

COLORREF CMSMVisualManager::GetToolbarButtonTextColor (CMFCToolBarButton* pButton,
												CMFCVisualManager::AFX_BUTTON_STATE state)
{
	COLORREF clr = COLORREF (-1);

	CMFCToolBarMenuButton* pMenuButton = 
		DYNAMIC_DOWNCAST (CMFCToolBarMenuButton, pButton);

	BOOL bIsMenuButton = pMenuButton != NULL;

	BOOL bIsPopupMenu = bIsMenuButton &&
		pMenuButton->GetParentWnd () != NULL &&
		pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CMFCPopupMenuBar));

	if (bIsMenuButton && !bIsPopupMenu)
	{
		switch (state)
		{
		case ButtonsIsRegular:
			clr = m_clrMenuBarButtonTextRegular;
			break;
		case ButtonsIsPressed:
			clr = m_clrMenuBarButtonTextPressed;
			break;
		case ButtonsIsHighlighted:
			clr = m_clrMenuBarButtonTextHighlighted;
			break;
		}
	}
	else
	{
		//if (pButton->IsKindOf (RUNTIME_CLASS (CMSMCategoryBarButton)))
		//{
		//	switch (state)
		//	{
		//	case ButtonsIsRegular:
		//		clr = m_clrCategorieBarButtonTextRegular;
		//		break;
		//	case ButtonsIsPressed:
		//		clr = m_clrCategorieBarButtonTextPressed;
		//		break;
		//	case ButtonsIsHighlighted:
		//		clr = m_clrCategorieBarButtonTextHighlighted;
		//		break;
		//	}
		//}
		//else
		{
			switch (state)
			{
			case ButtonsIsRegular:
				clr = m_clrToolBarButtonTextRegular;
				break;
			case ButtonsIsPressed:
				clr = m_clrToolBarButtonTextPressed;
				break;
			case ButtonsIsHighlighted:
				clr = m_clrToolBarButtonTextHighlighted;
				break;
			}
		}
	}

	if (clr == COLORREF (-1))
	{
		clr = CMFCVisualManagerOffice2007::GetToolbarButtonTextColor (pButton, state);
	}

	return clr;
}

void CMSMVisualManager::OnFillHighlightedArea (CDC* pDC, CRect rect, 
		CBrush* pBrush, CMFCToolBarButton* pButton)
{
	if (pButton == NULL)
	{
		CMFCVisualManagerOffice2007::OnFillHighlightedArea (pDC, rect,
												pBrush, pButton);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pBrush);

	CMFCToolBarMenuButton* pMenuButton = 
		DYNAMIC_DOWNCAST (CMFCToolBarMenuButton, pButton);

	BOOL bIsMenuButton = pMenuButton != NULL;

	BOOL bIsPopupMenu = bIsMenuButton &&
		pMenuButton->GetParentWnd () != NULL &&
		pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CMFCPopupMenuBar));
	
	CBrush* pBr = pBrush;

	bIsMenuButton = bIsMenuButton && 
		pMenuButton->GetParentWnd () != NULL &&
		pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CMFCMenuBar));

	BOOL bDefault = true;

	if (bIsMenuButton || bIsPopupMenu)
	{
		if (!bIsPopupMenu)
		{
			pBr = &m_brMenuBarButtonHighlighted;
		}
	}
	//else if (pButton->IsKindOf (RUNTIME_CLASS (CMSMLinksBarButton)))
	//{
	//	pBr = &m_brLinksBarButtonHighlighted;
	//	rect.InflateRect (0, 0, 1, 1);
	//}
	else
	{
		bDefault = false;
	}

	if (bDefault)
	{
		CMFCVisualManagerOfficeXP::OnFillHighlightedArea (pDC, rect, pBr, pButton);
		return;
	}
	//else if (pButton->IsKindOf (RUNTIME_CLASS (CMSMCategoryBarButton)))
	//{
	//	if ((pButton->m_nStyle & TBBS_CHECKED) != 0)
	//	{
	//		CDrawingManager dm(*pDC);
	//
	//		dm.FillGradient (rect,
	//			m_clrCategorieBarButtonL, m_clrCategorieBarButtonF, TRUE);
	//	}
	//}
	else
	{
		if (pMenuButton != NULL &&
			pMenuButton->GetParentWnd () != NULL &&
			pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CMSMToolBar)))
		{
			rect.InflateRect (0, 0, 1, 1);

			CDrawingManager dm (*pDC);

			dm.HighlightRect (rect, 10, (COLORREF)-1, 0, RGB (255, 255, 255));
		}
		else // other buttons
		{
			// TODO: paint toolbar buttons
			//CMFCVisualManagerOffice2003::OnFillHighlightedArea (pDC, rect, pBr, pButton);
		}
	}
}

void CMSMVisualManager::OnDrawMenuBorder (CDC* pDC, CMFCPopupMenu* /*pMenu*/, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->Draw3dRect (rect, m_clrMenuBorder, m_clrMenuBorder);

	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (rect, m_clrMenuLight, m_clrMenuLight);

	CRect rectLeft (1, 1, 2, rect.bottom - 1);
	pDC->FillRect (rectLeft, &m_brBarBkgnd);

}

void CMSMVisualManager::OnDrawButtonBorder (CDC* pDC,
		CMFCToolBarButton* pButton, CRect rect, AFX_BUTTON_STATE state)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (state != ButtonsIsPressed && state != ButtonsIsHighlighted)
	{
		ASSERT ((pButton->m_nStyle & TBBS_CHECKED) == 0);
		return;
	}

	CMFCToolBarMenuButton* pMenuButton = 
		DYNAMIC_DOWNCAST (CMFCToolBarMenuButton, pButton);

	BOOL bIsMenuButton = pMenuButton != NULL;

	BOOL bIsPopupMenu = bIsMenuButton &&
		pMenuButton->GetParentWnd () != NULL &&
		pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CMFCPopupMenuBar));

	bIsMenuButton = bIsMenuButton &&
		pMenuButton->GetParentWnd () != NULL &&
		pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CMFCMenuBar));

	if (bIsMenuButton || bIsPopupMenu)
	{
		COLORREF clrBorder = m_clrMenuItemBorder;

		switch (state)
		{
		case ButtonsIsPressed:
		case ButtonsIsHighlighted:
			clrBorder = bIsPopupMenu 
				? m_clrMenuItemBorder
				: m_clrMenuBarButtonBorderHighlighted;
			break;
		}


		if (!bIsPopupMenu || state != ButtonsIsHighlighted)
		{
			rect.right--;
			rect.bottom--;
		}

		if (bIsMenuButton && !bIsPopupMenu && 
			pMenuButton->IsDroppedDown ())
		{
			CMFCPopupMenu* pPopupMenu= pMenuButton->GetPopupMenu ();
			if (pPopupMenu != NULL && 
				(pPopupMenu->IsWindowVisible () || pPopupMenu->IsShown()))
			{
				ExtendMenuButton (pMenuButton, rect);

				BOOL bRTL = pPopupMenu->GetExStyle() & WS_EX_LAYOUTRTL;

				if (m_bShdowDroppedDownMenuButton && !bRTL && 
					CMFCMenuBar::IsMenuShadows () &&
					!CMFCToolBar::IsCustomizeMode () &&
					afxGlobalData.m_nBitsPerPixel > 8 &&
					!afxGlobalData.m_bIsBlackHighContrast &&
					!pPopupMenu->IsRightAlign ())
				{
					CDrawingManager dm (*pDC);

					dm.DrawShadow (rect, m_nMenuShadowDepth, 100, 75, NULL, NULL,
						m_clrMenuShadowBase);
				}
			}
		}

		if (state == ButtonsIsPressed || state == ButtonsIsHighlighted)
		{
			if (bIsPopupMenu && (pButton->m_nStyle & TBBS_CHECKED))
			{
				rect.bottom ++;
			}

			pDC->Draw3dRect (rect, clrBorder, clrBorder);
		}
	}
	//else if (pButton->IsKindOf (RUNTIME_CLASS (CMSMCategoryBarButton)))
	//{
	//	if ((pButton->m_nStyle & TBBS_CHECKED) != 0)
	//	{
	//		_DrawReflectedGradient (pDC, m_clrCategorieBarButtonBorder, 
	//			m_clrCategorieBarButtonBorderM, m_clrCategorieBarButtonBorder, 
	//			CRect (rect.left, rect.top, rect.right, rect.top + 1), false);

	//		CDrawingManager dm(*pDC);

	//		dm.FillGradient (CRect (rect.left, rect.top, rect.left + 1, rect.bottom),
	//			m_clrCategorieBarButtonL, m_clrCategorieBarButtonBorder, TRUE);

	//		dm.FillGradient (CRect (rect.right - 1, rect.top, rect.right, rect.bottom),
	//			m_clrCategorieBarButtonL, m_clrCategorieBarButtonBorder, TRUE);

	//		{
	//			CPen pen (PS_SOLID, 0, m_clrCategorieBarButtonLine);

	//			CPen* pOldPen = pDC->SelectObject (&pen);

	//			pDC->MoveTo (rect.left + 1, rect.top + 1);
	//			pDC->LineTo (rect.right - 1, rect.top + 1);

	//			pDC->SelectObject (pOldPen);
	//		}

	//		{
	//			CPen pen (PS_SOLID, 0, m_clrCategorieBarButtonL);

	//			CPen* pOldPen = pDC->SelectObject (&pen);

	//			pDC->MoveTo (rect.left + 1, rect.bottom - 1);
	//			pDC->LineTo (rect.right - 1, rect.bottom - 1);

	//			pDC->SelectObject (pOldPen);
	//		}
	//	}
	//}
	//else if (pButton->IsKindOf (RUNTIME_CLASS (CMSMLinksBarButton)))
	//{
	//	if (state == ButtonsIsPressed || state == ButtonsIsHighlighted)
	//	{
	//		pDC->Draw3dRect (rect, m_clrLinksBarButtonBorderHighlighted, m_clrLinksBarButtonBorderHighlighted);
	//	}
	//}
	else
	{
		if (pMenuButton != NULL &&
			pMenuButton->GetParentWnd () != NULL &&
			pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CMSMToolBar)))
		{
			CRect rt (rect);

			CDrawingManager dm (*pDC);

			rt.bottom = rt.top + 1;
			dm.HighlightRect (rt, 15, (COLORREF)-1, 0, RGB (255, 255, 255));

			rt.bottom = rect.bottom;
			rt.top    = rt.bottom - 1;
			dm.HighlightRect (rt, 15, (COLORREF)-1, 0, RGB (255, 255, 255));

			rt.top    = rect.top    + 1;
			rt.bottom = rect.bottom - 1;
			rt.right  = rt.left + 1;
			dm.HighlightRect (rt, 15, (COLORREF)-1, 0, RGB (255, 255, 255));

			rt.right  = rect.right;
			rt.left   = rt.right - 1;
			dm.HighlightRect (rt, 15, (COLORREF)-1, 0, RGB (255, 255, 255));
		}
		else
		{
			// TODO: paint toolbar buttons
			//CMFCVisualManagerOffice2003::OnDrawButtonBorder (pDC, pButton, rect, state);
		}
	}
}

void CMSMVisualManager::OnHighlightMenuItem (CDC*pDC, CMFCToolBarMenuButton* pButton,
											CRect rect, COLORREF& clrText)
{
	CMFCToolBarMenuButton* pMenuButton = 
		DYNAMIC_DOWNCAST (CMFCToolBarMenuButton, pButton);

	BOOL bIsMenuButton = pMenuButton != NULL;

	BOOL bIsPopupMenu = bIsMenuButton &&
		pMenuButton->GetParentWnd () != NULL &&
		pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CMFCPopupMenuBar));

	if (bIsPopupMenu && rect.Width() > pButton->Rect ().Width () / 2)
	{
		rect.right--;
	}

	CMFCVisualManagerOfficeXP::OnHighlightMenuItem (pDC, pButton, rect, clrText);
}

void CMSMVisualManager::OnDrawSeparator (CDC* pDC, CBasePane* pBar,
										 CRect rect, BOOL bHorz)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);

	if (pBar->IsDialogControl () || 
		pBar->IsKindOf (RUNTIME_CLASS (CMFCPopupMenuBar)) || 
		pBar->IsKindOf (RUNTIME_CLASS (CMFCColorBar)))
	{
		CMFCVisualManagerOffice2007::OnDrawSeparator (pDC, pBar, rect, bHorz);
		return;
	}


	CRect rectSeparator = rect;

	int x1, x2;
	int y1, y2;

	if (bHorz)
	{
		x1 = x2 = (rect.left + rect.right) / 2;
		y1 = rect.top;
		y2 = rect.bottom - 1;
	}
	else
	{
		y1 = y2 = (rect.top + rect.bottom) / 2;
		x1 = rect.left;
		x2 = rect.right;

		if (pBar->IsKindOf (RUNTIME_CLASS (CMFCPopupMenuBar)) &&
			!pBar->IsKindOf (RUNTIME_CLASS (CMFCColorBar)))
		{
			
			x1 = rect.left + CMFCToolBar::GetMenuImageSize ().cx + 
						GetMenuImageMargin () + 1;

			CRect rectBar;
			pBar->GetClientRect (rectBar);

			if (rectBar.right - x2 < 50) // Last item in row
			{
				x2 = rectBar.right;
			}

			if (((CMFCPopupMenuBar*) pBar)->m_bDisableSideBarInXPMode)
			{
				x1 = 0;
			}

			//---------------------------------
			//	Maybe Quick Customize separator
			//---------------------------------
			if (pBar->IsKindOf (RUNTIME_CLASS (CMFCPopupMenuBar)))
			{
				CWnd* pWnd = pBar->GetParent();
				if (pWnd != NULL && pWnd->IsKindOf (RUNTIME_CLASS (CMFCPopupMenu)))
				{
					CMFCPopupMenu* pMenu = (CMFCPopupMenu*)pWnd;
					if (pMenu->IsCustomizePane())
					{
						x1 = rect.left + 2*CMFCToolBar::GetMenuImageSize ().cx + 
								3*GetMenuImageMargin () + 2;
					}
				}
			}
		}
	}

	if (x1 == x2)
	{
		x2++;
	}

	if (y1 == y2)
	{
		y2++;
	}

	_DrawReflectedGradient (pDC, m_clrToolBarSeparatorS, 
		m_clrToolBarSeparatorM, m_clrToolBarSeparatorF, CRect (x1, y1, x2, y2), !!bHorz);
}

void CMSMVisualManager::OnDrawComboBorder (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CMFCToolBarComboBoxButton* pButton)
{
	if (afxGlobalData.m_nBitsPerPixel <= 8 || afxGlobalData.m_bIsBlackHighContrast)
	{
		CMFCVisualManagerOffice2007::OnDrawComboBorder (pDC, rect,
												bDisabled,
												bIsDropped,
												bIsHighlighted,
												pButton);
		return;
	}

	rect.DeflateRect (1, 1);

	COLORREF colorBorder = bDisabled ? afxGlobalData.clrBtnShadow : m_clrComboBoxBorder;
	pDC->Draw3dRect (&rect, colorBorder, colorBorder);
}

void CMSMVisualManager::OnDrawComboDropButton (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CMFCToolBarComboBoxButton* pButton)
{
	if (afxGlobalData.m_nBitsPerPixel <= 8 || afxGlobalData.m_bIsBlackHighContrast)
	{
		CMFCVisualManagerOfficeXP::OnDrawComboDropButton (pDC, rect,
												bDisabled, bIsDropped,
												bIsHighlighted, pButton);
		return;
	}

	ASSERT_VALID(pDC);
	ASSERT_VALID (this);

	CPoint pointTriangle (
		rect.left + (rect.Width () - CMenuImages::Size ().cx) / 2 + 1,
		rect.top + (rect.Height () - CMenuImages::Size ().cy) / 2 + 1);

	if (!bDisabled)
	{
		COLORREF clr1 = m_clrComboBoxBtnRegularF;
		COLORREF clr2 = m_clrComboBoxBtnRegularL;
		COLORREF clrB = m_clrComboBoxBtnRegularBorder;

		if (bIsHighlighted)
		{
			clr1 = m_clrComboBoxBtnHighlightedF;
			clr2 = m_clrComboBoxBtnHighlightedL;
			clrB = m_clrComboBoxBtnHighlightedBorder;
		}
		else if (bIsDropped)
		{
			clr1 = m_clrComboBoxBtnPressedF;
			clr2 = m_clrComboBoxBtnPressedL;
			clrB = m_clrComboBoxBtnPressedBorder;
		}

		CDrawingManager dm (*pDC);
		dm.FillGradient (rect, clr2, clr1, TRUE);

		pDC->Draw3dRect (rect, clrB, clrB);
	}

	CMenuImages::Draw (pDC, CMenuImages::IdArrowDown, pointTriangle,
		bDisabled ? CMenuImages::ImageGray : (bIsDropped && bIsHighlighted) ? 
			CMenuImages::ImageWhite : CMenuImages::ImageBlack);
}

void CMSMVisualManager::OnFillTasksPaneBackground(CDC* pDC, CRect rectWorkArea)
{
	CDrawingManager dm (*pDC);
	dm.FillGradient (rectWorkArea, RGB (0, 0, 0), RGB (35, 35, 35), FALSE);

	//CPen penLeft (PS_SOLID, 1, RGB (188, 218, 247));
	CPen penLeft (PS_SOLID, 1, RGB (0, 0, 0));
	CPen* pOldPen = pDC->SelectObject (&penLeft);

	pDC->MoveTo (rectWorkArea.right - 1, rectWorkArea.top);
	pDC->LineTo (rectWorkArea.right - 1, rectWorkArea.bottom);

	pDC->SelectObject (pOldPen);
}

void CMSMVisualManager::OnDrawTask(CDC* pDC, CMFCTasksPaneTask* pTask, 
									   CImageList* pIcons, BOOL bIsHighlighted, BOOL bIsSelected)
{
	if (pTask != NULL)
	{
		pTask->m_clrText = pTask->m_clrTextHot = RGB(255,0,0);//RGB (16, 37, 127);
	}

	CMFCVisualManagerOffice2007::OnDrawTask(pDC, pTask, pIcons, bIsHighlighted, bIsSelected);
}
