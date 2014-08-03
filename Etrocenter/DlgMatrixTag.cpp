// DlgMatrixTag.cpp : implementation file
//

#include "stdafx.h"
#include "DlgMatrixTag.h"
#include "resource.h"
#include "MainFrm.h"
#include "DlgAutoScanSetting.h"
#include "DlgMatrixTab.h"

#define TIMER_SETAUTO							01
#define TIMER_DISPLAY_AUTOPLAY	02

int nID[9]={ IDB_BITMAP_FULL, IDB_BITMAP_1X1, IDB_BITMAP_2X2, IDB_BITMAP_1P5, IDB_BITMAP_3X3, IDB_BITMAP_2P8, IDB_BITMAP_1P12, IDB_BITMAP_1P12_2, IDB_BITMAP_4X4};//, IDB_BITMAP_6X6, IDB_BITMAP_7X7
int nIDHot[9]={ IDB_BITMAP_FULLHOT, IDB_BITMAP_1X1HOT, IDB_BITMAP_2X2HOT, IDB_BITMAP_1P5HOT, IDB_BITMAP_3X3HOT, IDB_BITMAP_2P8HOT, IDB_BITMAP_1P12HOT, IDB_BITMAP_1P12_2HOT, IDB_BITMAP_4X4HOT};//, IDB_BITMAP_6X6HOT, IDB_BITMAP_7X7HOT

int nIDAll[15]={ IDB_BITMAP_FULL, IDB_BITMAP_1X1, IDB_BITMAP_2X2, IDB_BITMAP_1P5, IDB_BITMAP_3X3, IDB_BITMAP_2P8, IDB_BITMAP_1P12, IDB_BITMAP_1P12_2, IDB_BITMAP_4X4 , IDB_BITMAP_1P16, IDB_BITMAP_2P18, IDB_BITMAP_2P32, IDB_BITMAP_6X6, IDB_BITMAP_7X7, IDB_BITMAP_8X8};
int nIDHotAll[15]={ IDB_BITMAP_FULLHOT, IDB_BITMAP_1X1HOT, IDB_BITMAP_2X2HOT, IDB_BITMAP_1P5HOT, IDB_BITMAP_3X3HOT, IDB_BITMAP_2P8HOT, IDB_BITMAP_1P12HOT, IDB_BITMAP_1P12_2HOT, IDB_BITMAP_4X4HOT, IDB_BITMAP_1P16HOT, IDB_BITMAP_2P18HOT, IDB_BITMAP_2P32HOT, IDB_BITMAP_6X6HOT, IDB_BITMAP_7X7HOT, IDB_BITMAP_8x8HOT};

int nIDSetting[8]={ IDB_BITMAP_1X1, IDB_BITMAP_2X2, IDB_BITMAP_1P5, IDB_BITMAP_3X3, IDB_BITMAP_2P8, IDB_BITMAP_1P12, IDB_BITMAP_1P12_2, IDB_BITMAP_4X4};
int nIDHotSetting[8]={ IDB_BITMAP_1X1HOT, IDB_BITMAP_2X2HOT, IDB_BITMAP_1P5HOT, IDB_BITMAP_3X3HOT, IDB_BITMAP_2P8HOT, IDB_BITMAP_1P12HOT, IDB_BITMAP_1P12_2HOT, IDB_BITMAP_4X4HOT};

int nIDSettingAll[14]={ IDB_BITMAP_1X1, IDB_BITMAP_2X2, IDB_BITMAP_1P5, IDB_BITMAP_3X3, IDB_BITMAP_2P8, IDB_BITMAP_1P12, IDB_BITMAP_1P12_2, IDB_BITMAP_4X4 , IDB_BITMAP_1P16, IDB_BITMAP_2P18, IDB_BITMAP_2P32, IDB_BITMAP_6X6, IDB_BITMAP_7X7, IDB_BITMAP_8X8};
int nIDHotSettingAll[14]={ IDB_BITMAP_1X1HOT, IDB_BITMAP_2X2HOT, IDB_BITMAP_1P5HOT, IDB_BITMAP_3X3HOT, IDB_BITMAP_2P8HOT, IDB_BITMAP_1P12HOT, IDB_BITMAP_1P12_2HOT, IDB_BITMAP_4X4HOT, IDB_BITMAP_1P16HOT, IDB_BITMAP_2P18HOT, IDB_BITMAP_2P32HOT, IDB_BITMAP_6X6HOT, IDB_BITMAP_7X7HOT, IDB_BITMAP_8x8HOT};

IMPLEMENT_DYNAMIC(CMyCheckBox, CButton)
CMyCheckBox::CMyCheckBox()
:m_BrushBg( BKCOLOR ), m_bCheckedLiveview(false),m_bCheckedIPwall(false)
{
	 m_nMyCBoxID=MyCBox_LiveView;
}
CMyCheckBox::~CMyCheckBox()
{
}
BEGIN_MESSAGE_MAP(CMyCheckBox, CButton)
//{{AFX_MSG_MAP(CMyCheckBox)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
    //ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


HBRUSH CMyCheckBox::CtlColor(CDC* pDC, UINT nCtlColor)
{
	//SetBkColor(pDC->m_hDC, RGB(0,0,0));
	/*pDC->SetTextColor(FONTCOLOR);	
	pDC->SetBkMode(TRANSPARENT);
	return  m_BrushBg;*/
	if (nCtlColor == CTLCOLOR_STATIC)
    {
	   // pDC->SetBkColor(BKCOLOR);
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(FONTCOLOR);
       //return (HBRUSH)GetStockObject( NULL_BRUSH ) ;
         /* SetBkMode(hdc,TRANSPARENT);
          SetTextColor(hdc,RGB(0,255,0));*/
          //return (LRESULT)hBrush;
    }
	return  m_BrushBg;
}

BOOL CMyCheckBox::OnEraseBkgnd(CDC* pDC)
{
	//SetBkColor(pDC->m_hDC, RGB(0,0,0));
	//CRect rect;
	//GetClientRect(rect);
	//pDC->FillRect(&rect, &m_BrushBg);
	pDC->SetTextColor(FONTCOLOR);	
	//pDC->SetBkMode(TRANSPARENT);
	return CButton::OnEraseBkgnd(pDC);
}

void CMyCheckBox::OnPaint()
{
    CPaintDC dc(this); 
}



void CMyCheckBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CRect rect =  lpDrawItemStruct->rcItem;
    CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
    
    //GetClientRect(rect);
	//CBrush bkColor(BKGNDCOLOR);
	//pDC->FillRect(&rect,&m_BrushBg);

    pDC->SetTextColor(FONTCOLOR);	
    int nSaveDC=pDC->SaveDC();
    UINT state = lpDrawItemStruct->itemState;
    POINT pt ;
    TCHAR strText[MAX_PATH + 1];
    ::GetWindowText(m_hWnd, strText, MAX_PATH);


    if (state & ODS_FOCUS)
    {
    }else
    {
    }
    if (state & ODS_SELECTED || state & ODS_DEFAULT){
    }


    CRect BoxRect;
    BoxRect=rect;
    BoxRect.right=BoxRect.left+13;
    //DWORD n = GetCheck();
    //SetCheck(BST_UNCHECKED);
    //n = GetCheck();
    DWORD nCheckStyle = 0;

	if (m_nMyCBoxID==MyCBox_LiveView)
	{
		if (m_bCheckedLiveview)  
			nCheckStyle =  DFCS_CHECKED;
	}
	else if (m_nMyCBoxID==MyCBox_IPWall)
	{
		if (m_bCheckedIPwall)  
			nCheckStyle =  DFCS_CHECKED;
	}  

    //pDC->DrawFrameControl(BoxRect, DFC_BUTTON, DFCS_BUTTONCHECK|(GetCheck()? DFCS_CHECKED: 0));  
    pDC->DrawFrameControl(BoxRect, DFC_BUTTON, DFCS_BUTTONCHECK|nCheckStyle);  
    


    if (strText!=NULL)
    {
        CFont* hFont = GetFont();
        CFont* hOldFont = pDC->SelectObject(hFont);
        CSize szExtent = pDC->GetTextExtent(strText, lstrlen(strText));
        CPoint pt( rect.CenterPoint().x - szExtent.cx / 2+4, rect.CenterPoint().y - szExtent.cy / 2);
        //if (state & ODS_SELECTED) 
          //pt.Offset(1, 1);
        int nMode = pDC->SetBkMode(TRANSPARENT);
        //if (state & ODS_DISABLED)
            //pDC->DrawState(pt, szExtent, strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
        //else
          pDC->DrawState(pt, szExtent, strText, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
        pDC->SelectObject(hOldFont);
        pDC->SetBkMode(nMode);
    }

    pDC->RestoreDC(nSaveDC);
}


void CMyCheckBox::PreSubclassWindow()
{
    SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW|WS_VISIBLE);
    CButton::PreSubclassWindow();
}

void CMyCheckBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bChecked=0;
	if (m_nMyCBoxID==MyCBox_LiveView)
	{
		m_bCheckedLiveview ^= 1;
		 SetCheck(m_bCheckedLiveview ? BST_CHECKED : BST_UNCHECKED);
		 bChecked=m_bCheckedLiveview;
	}
	else if (m_nMyCBoxID==MyCBox_IPWall)
	{
		m_bCheckedIPwall ^= 1;
		 SetCheck(m_bCheckedIPwall ? BST_CHECKED : BST_UNCHECKED);
		 bChecked=m_bCheckedIPwall;
	}
    
   
    ((CMainFrame*)AfxGetMainWnd())->m_bAutoScan  = bChecked;
    ((CMainFrame*)AfxGetMainWnd())->vSetAutoScan(bChecked,m_nMyCBoxID);
    CButton::OnLButtonDown(nFlags, point);
}

// CDlgMatrixTag dialog

IMPLEMENT_DYNAMIC(CDlgMatrixTag, CDialog)

CDlgMatrixTag::CDlgMatrixTag(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatrixTag::IDD, pParent),m_bSetStyle(false)//,m_bAutoScan(false)
{
	btnAutoScanSetting = new CButtonST;
	btnAutoScanSettingGroup = new CButtonST;
	mapTable[IDB_BITMAP_1X1]=1;mapTable[IDB_BITMAP_2X2]=2;mapTable[IDB_BITMAP_3X3]=3;
    mapTable[IDB_BITMAP_4X4]=4;mapTable[IDB_BITMAP_6X6]=6;mapTable[IDB_BITMAP_7X7]=7;
    mapTable[IDB_BITMAP_1P5]=15;mapTable[IDB_BITMAP_2P8]=28;
    mapTable[IDB_BITMAP_1P12]=121;mapTable[IDB_BITMAP_1P12_2]=122;

	mapTable[IDB_BITMAP_1P16]=116;mapTable[IDB_BITMAP_2P18]=218;mapTable[IDB_BITMAP_8X8]=8;mapTable[IDB_BITMAP_2P32]=232;


	m_nAutoScanTime=10;
	m_nAutoScanCount=10;
	m_rcDispAutoText.SetRectEmpty();
}

CDlgMatrixTag::~CDlgMatrixTag()
{
	delete btnAutoScanSetting;
	delete btnAutoScanSettingGroup;

	for (int ci = 0; ci < m_vtBtnMatrixs.size(); ci++)
	{
	    delete m_vtBtnMatrixs[ci];
     m_vtBtnMatrixs[ci] = NULL;
	}
}

void CDlgMatrixTag::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_AUTOPLAY, m_ButtonAutoScan);
	DDX_Control(pDX, IDC_BTN_AUTOSCAN_SETTING, m_btnAutioScanSetting);
}


BEGIN_MESSAGE_MAP(CDlgMatrixTag, CDialog)
    
    //ON_BN_CLICKED(IDC_CHECK_AUTOPLAY, &CDlgMatrixTag::OnBnAutoScan)	
	ON_BN_CLICKED(IDB_CLOSE, &CDlgMatrixTag::OnBnClickedClose)
	ON_CONTROL_RANGE(BN_CLICKED, IDB_BITMAP_FULL, IDB_BITMAP_1P5, &CDlgMatrixTag::OnMatrixBnClicked)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
    //ON_WM_LBUTTONUP()
 	ON_WM_DESTROY()
    ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BTN_AUTOSCAN_SETTING, &CDlgMatrixTag::OnBnClickedBtnAutoscanSetting)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CDlgMatrixTag::OnMatrixBnClicked(UINT uid)
{	
	 CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
     pMain->m_OrgLayoutIdx = mapTable[uid];
	   if (uid == IDB_BITMAP_FULL) 
		{
			//pMain->RunFullScreen();
			pMain->m_bOnFullScreen ^= 1;
			if (!m_bSetStyle)
				pMain->OnFullScreen(true);
			//return;
		}else
	   {
		    if (!m_bSetStyle)
		   {
			   BOOL bChecked=0;
			   if (m_ButtonAutoScan.m_nMyCBoxID==MyCBox_LiveView)
			   {
				   bChecked=m_ButtonAutoScan.m_bCheckedLiveview;

			  	   CNVRBk::GetBkObject()->SendMessage(MsgUpdateView, UM_SET_LIVEVIEW_AUTOTYPE, bChecked);
			   }
			   else if (m_ButtonAutoScan.m_nMyCBoxID==MyCBox_IPWall)
			   {
				   bChecked=m_ButtonAutoScan.m_bCheckedIPwall;
			   }
			   ((CMainFrame*)AfxGetMainWnd())->m_bAutoScan  = bChecked;
			   ((CMainFrame*)AfxGetMainWnd())->vSetAutoScan(bChecked,m_ButtonAutoScan.m_nMyCBoxID);

			   if (pMain->m_bIPWallSelected == true)
			   {
				   pMain->vGetIpcamTreeCtrl()->vFlyaway(mapTable[uid]);
			   }
			   else
			   {
				   CNVRBk::GetBkObject()->SendMessage(MsgUpdateView,UM_FREE_VIDEO,NULL);
				   pMain->vGetIpcamTreeCtrl()->vLiveView();
				   vSetMatrixBtnHot(uid);

				   ((DlgMatrixTab*)(GetParent()->GetParent()))->vRestoreBtnState(0);// 0 stands for RESTORE_BTN_TYPE::LIVEVIEWBTN
				   if (((DlgMatrixTab*)GetParent()->GetParent())->bDialogStyle())
				   {
					   ::PostMessage((GetParent()->GetParent())->m_hWnd,WM_CLOSE, NULL, NULL);
				   }
			   }
		   }
			else
			{
				vSetMatrixBtnHot(uid);
				OnOK();
			}
		}	  

	   //////////Larry add 102-11-20///////////
	   if (_MaskBeta)
	   {  
		   if (!m_bSetStyle)
		   {
			   if (!g_bAutoPlayCheckBox)
			   {
				   if (!m_rcDispAutoText.IsRectEmpty())
				   {
					   CClientDC dc(this);
					   dc.FillSolidRect(m_rcDispAutoText,BKCOLOR);
					   KillTimer(TIMER_DISPLAY_AUTOPLAY);
				   }
			   }
		   }			
	   }
	   ////////////////////////////////////////
}
// CDlgMatrixTag message handlers
//void CDlgMatrixTag::OnBnAutoScan()
//{
//	m_bAutoScan ^= 1;
//	((CButton*)GetDlgItem(IDC_CHECK_AUTOPLAY))->SetCheck(m_bAutoScan);
//  
//}
void CDlgMatrixTag::OnBnAutoScanSetting()
{
    CGS::SetTransparent(this->GetParent()->GetParent()->m_hWnd,120);
	CDlgAutoScanSetting dlg;
    //ReleaseCapture();
	dlg.DoModal();
    CGS::SetTransparent(this->GetParent()->GetParent()->m_hWnd,255);
	//SetCapture();
}

BOOL CDlgMatrixTag::CreateMatrixs()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
    //int nBtnWidth(48+10), nBtnHeight(70), nWidthGap(10), nHightGap(10);
    int nBtnWidth(39), nBtnHeight(41), nWidthGap(10), nHightGap(10);
    int nLeftMargin(10), nTopMargin(40);
	 
    int nButtonSize=0;
	if (!m_bSetStyle)
	{
		if (CNVRBk::GetBkObject()->isNVRType())
			nButtonSize=sizeof(nID)/ sizeof(int);
		else
			nButtonSize=sizeof(nIDAll)/ sizeof(int);
	}
	else
	{
		if (CNVRBk::GetBkObject()->isNVRType())
			nButtonSize= sizeof(nIDSetting)/ sizeof(int);
		else
			nButtonSize=sizeof(nIDSettingAll)/ sizeof(int);
	}

    CRect rect;
    GetClientRect(rect);
	
	if (rect.IsRectEmpty())
	{
		return false;
	}


    int nColumn = (rect.Width() - nLeftMargin*2) / (nBtnWidth +nWidthGap);
	//int nRow = (rect.Height() - nTopMargin) / nColumn + (((rect.Height() - nTopMargin) % nColumn ) ?  1 : 0);
	int nRow=	  210	/  (nBtnHeight + nHightGap);				  //CMainFrame::CreateMatrixPane() is set 210
	if (!m_bSetStyle)
	{
		nTopMargin=60;
		nColumn++;
	}
  

    DWORD dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;//BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE |WS_CHILD|WS_VISIBLE|BS_OWNERDRAW ;
	COLORREF clBG = RGB(0,0,0);
    int nHoveOver = (int)BTNST_AUTO_GRAY;

	m_btnAutioScanSetting.SetBitmaps(IDB_EMAPSETU, clBG ,nHoveOver);
    CRect rect2;
	CRect rtAutoSetting;
	m_ButtonAutoScan.GetWindowRect(rtAutoSetting);
	ScreenToClient(rtAutoSetting);
    m_btnAutioScanSetting.GetClientRect(rect2);
    //ScreenToClient(rect2);
    m_btnAutioScanSetting.MoveWindow(rtAutoSetting.right+5, 10, 24,24);

    
    for (int cj = 0; cj < nRow; cj++)
    {    
        for (int ci = 0; ci < nColumn; ci++)
        {
            if ((cj* nColumn + ci) < nButtonSize)
            {
                int nX0 = nLeftMargin + (nBtnWidth +nWidthGap)* ci;
				if (!m_bSetStyle && cj == 0)
					nX0 -=  (nBtnWidth +nWidthGap);
                int nX1 = nX0 +nBtnWidth;
                int nY0 = nTopMargin + (nBtnHeight +nHightGap)* cj;
                int nY1 = nY0 +nBtnHeight;
                CButtonST* pBtn = new CButtonST;

				if (cj == 0 && ci == 0 && !m_bSetStyle)
				{
					//if (CNVRBk::GetBkObject()->isNVRType())
					//{
					//		if (nID[0] == IDB_BITMAP_FULL)
					//		{
					//			nX0 = nLeftMargin;
					//			nX1 = nX0 +nBtnWidth;
					//			nY0 = rtAutoSetting.top;
					//			nY1 = nY0 +nBtnHeight;
					//		}
					//}
					//else
					//{
						if (nIDAll[0] == IDB_BITMAP_FULL)
						{
							nX0 = nLeftMargin;
							nX1 = nX0 +nBtnWidth;
							nY0 = rtAutoSetting.top-5;
							nY1 = nY0 +nBtnHeight;
						}
					//}
				}
				
				
				if (!m_bSetStyle)
				{	
					if (CNVRBk::GetBkObject()->isNVRType())
					{
						pBtn->Create(NULL, dwStyle, CRect(nX0, nY0, nX1, nY1), this, nID[nColumn* cj +ci]);              
						if(mapTable[nID[nColumn* cj +ci]] == pMain->m_OrgLayoutIdx)
						{
							pBtn->SetBitmaps(nIDHot[nColumn* cj +ci], clBG);
							g_uIDBtnMatrix = nID[nColumn* cj +ci];
						}
						else
						{
							pBtn->SetBitmaps(nIDHot[nColumn* cj +ci], clBG ,nHoveOver,  nID[nColumn* cj +ci]);
						}
					}
					else
					{
						pBtn->Create(NULL, dwStyle, CRect(nX0, nY0, nX1, nY1), this, nIDAll[nColumn* cj +ci]);              
						if(mapTable[nIDAll[nColumn* cj +ci]] == pMain->m_OrgLayoutIdx)
						{
							pBtn->SetBitmaps(nIDHotAll[nColumn* cj +ci], clBG);
							g_uIDBtnMatrix = nIDAll[nColumn* cj +ci];
						}
						else
						{
							pBtn->SetBitmaps(nIDHotAll[nColumn* cj +ci], clBG ,nHoveOver,  nIDAll[nColumn* cj +ci]);
						}
					}
					
				}
				else
				{
					if (CNVRBk::GetBkObject()->isNVRType())
					{
						pBtn->Create(NULL, dwStyle, CRect(nX0, nY0, nX1, nY1), this, nIDSetting[nColumn* cj +ci]);              
						if(mapTable[nIDSetting[nColumn* cj +ci]] == pMain->m_OrgLayoutIdx)
						{
							pBtn->SetBitmaps(nIDHotSetting[nColumn* cj +ci], clBG);
							//g_uIDBtnMatrix = nID[nColumn* cj +ci];
						}
						else
						{
							pBtn->SetBitmaps(nIDHotSetting[nColumn* cj +ci], clBG ,nHoveOver,  nIDHotSetting[nColumn* cj +ci]);
						}
					}
					else
					{
						pBtn->Create(NULL, dwStyle, CRect(nX0, nY0, nX1, nY1), this, nIDSettingAll[nColumn* cj +ci]);              
						if(mapTable[nIDSettingAll[nColumn* cj +ci]] == pMain->m_OrgLayoutIdx)
						{
							pBtn->SetBitmaps(nIDHotSettingAll[nColumn* cj +ci], clBG);
							//g_uIDBtnMatrix = nID[nColumn* cj +ci];
						}
						else
						{
							pBtn->SetBitmaps(nIDHotSettingAll[nColumn* cj +ci], clBG ,nHoveOver,  nIDHotSettingAll[nColumn* cj +ci]);
						}
					}					
				}


				m_vtBtnMatrixs.push_back(pBtn);
			}
        }
    }
    
    return true;    
}
BOOL CDlgMatrixTag::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (m_bSetStyle)
	{
		m_btnAutioScanSetting.ShowWindow(SW_HIDE);
		m_ButtonAutoScan.ShowWindow(SW_HIDE);
	}

    //if (((DlgMatrixTab*)GetParent()->GetParent())->bDialogStyle())
    //{
    //    m_Brushbk.CreateSolidBrush(GLOBALBKCOLOR);
    //}
    //else
    {
        m_Brushbk.CreateSolidBrush(BKCOLOR);
    }
	int i_x;
	int i_y;
	
	CRect rect;
	GetParent()->GetClientRect(rect);
	if(m_bSetStyle)
	{
		long nStyle;
		nStyle = GetWindowLong(this->m_hWnd,GWL_STYLE);
		nStyle &= ~(WS_CHILD);
		nStyle &= (WS_POPUP) ;
		SetWindowLong(this->m_hWnd,GWL_STYLE, nStyle);
		POINT pos;
		GetCursorPos(&pos);
		i_x=pos.x;
		i_y=pos.y;
		SetParent(NULL);
	}
	else
	{
		i_x=rect.Width();
		i_y=rect.Height();
	}
	int i_h=70, i_w=48, i_gap=30, i_sw=10, i_dw=410, i_y1=50 ;

	if (!m_bSetStyle)
		SetWindowPos(NULL, i_x-i_dw, i_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	else
		SetWindowPos(NULL, i_x-i_dw, i_y, 380, 180, SWP_SHOWWINDOW);

    CreateMatrixs();

	if (!m_bSetStyle)
		SetTimer(TIMER_SETAUTO,500,0);		//Wait for LiveView.cpp  vAttach() finish//
	else
		vSetMatrixBtnHot(vMartixCodeToBitmapID(m_lRet));

	return TRUE;  
	
	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMatrixTag::OnBnClickedClose()
{
	EndDialog(-1);
	POSTPARENT(-1);
	//::PostMessage((GetParent()->GetParent())->m_hWnd,ID_MATRIX_SELECTED,(WPARAM)-1,NULL);
}

BOOL CDlgMatrixTag::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_Brushbk);
    //::PostMessage(GetDlgItem(IDC_CHECK_AUTOPLAY)->m_hWnd, 
	return true;//CDialog::OnEraseBkgnd(pDC);
}


HBRUSH CDlgMatrixTag::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_STATIC)
    {
	    pDC->SetBkColor(BKCOLOR);
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(FONTCOLOR);
       //return (HBRUSH)GetStockObject( NULL_BRUSH ) ;
         /* SetBkMode(hdc,TRANSPARENT);
          SetTextColor(hdc,RGB(0,255,0));*/
          //return (LRESULT)hBrush;
    }
	return  m_Brushbk;
}


void CDlgMatrixTag::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	//((CMainFrame*)AfxGetMainWnd())->m_bAutoScan  = m_bAutoScan;
	//((CMainFrame*)AfxGetMainWnd())->vSetAutoScan(m_bAutoScan);
}

void CDlgMatrixTag::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    //CDC *pDc = CDC::FromHandle(lpDrawItemStruct->hDC);
    //pDc->SetTextColor(OSDTEXTCOLOR);
    CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDlgMatrixTag::OnBnClickedBtnAutoscanSetting()
{
	CGS::SetTransparent(this->GetParent()->GetParent()->m_hWnd,120);
	CDlgAutoScanSetting dlg(this);
    //ReleaseCapture();
	dlg.DoModal();
    CGS::SetTransparent(this->GetParent()->GetParent()->m_hWnd,255);
	//SetCapture();
}
void CDlgMatrixTag::vSetMatrixBtnHot(UINT nId)// if nId is not a valid id, this function restore btn state to original
{
	if (m_vtBtnMatrixs.size() == 0 || nId == 0)
		return;

	 int nButtonSize =0;
	 if (!m_bSetStyle)
	 {
		 if (CNVRBk::GetBkObject()->isNVRType())
			nButtonSize=sizeof(nID)/ sizeof(int);
		 else
			 nButtonSize=sizeof(nIDAll)/ sizeof(int);	 	
	 }
	 else
	 {
		 if (CNVRBk::GetBkObject()->isNVRType())
			 nButtonSize=sizeof(nIDSetting)/ sizeof(int);
		 else
			 nButtonSize=sizeof(nIDSettingAll)/ sizeof(int);
	 }

	 COLORREF clBG = RGB(0,0,0);
    int nHoveOver = (int)BTNST_AUTO_GRAY;
	
	 // restore all to original bitmap
	 for(int i = 0; i < m_vtBtnMatrixs.size();i++)
	 {
		if (!m_bSetStyle)
		{
			if (CNVRBk::GetBkObject()->isNVRType())
				m_vtBtnMatrixs[i]->SetBitmaps(nIDHot[i], clBG ,nHoveOver,  nID[i]);
			else
				m_vtBtnMatrixs[i]->SetBitmaps(nIDHotAll[i], clBG ,nHoveOver,  nIDAll[i]);
		}
		else
		{
			if (CNVRBk::GetBkObject()->isNVRType())
				m_vtBtnMatrixs[i]->SetBitmaps(nIDHotSetting[i], clBG ,nHoveOver,  nIDSetting[i]);
			else
				m_vtBtnMatrixs[i]->SetBitmaps(nIDHotSettingAll[i], clBG ,nHoveOver,  nIDHotSettingAll[i]);
		}
	 }

	 // set selected matrix button to hot
	 for(int i =0;i<nButtonSize;i++)
	 {
		 if (!m_bSetStyle)
		 {
			 if (CNVRBk::GetBkObject()->isNVRType())
			 {
				 if(nID[i] == nId)
				 {
					 m_vtBtnMatrixs[i]->SetBitmaps(nIDHot[i], clBG);
					 g_uIDBtnMatrix=nId;				 
				 }
			 }
			 else
			 {
				 if(nIDAll[i] == nId)
				 {
					 m_vtBtnMatrixs[i]->SetBitmaps(nIDHotAll[i], clBG);
					 g_uIDBtnMatrix=nId;				 
				 }
			 }
		 }
		 else
		 {
			  if (CNVRBk::GetBkObject()->isNVRType())
			  {
				  if(nIDSetting[i] == nId)
				  {
					  m_vtBtnMatrixs[i]->SetBitmaps(nIDHotSetting[i], clBG);
					  m_lRet=vBitmapIDToMartixCode(nId)	;
					  return;
				  }
			  }
			  else
			  {
				  if(nIDSettingAll[i] == nId)
				  {
					  m_vtBtnMatrixs[i]->SetBitmaps(nIDHotSettingAll[i], clBG);
					  m_lRet=vBitmapIDToMartixCode(nId)	;
					  return;
				  }
			  }
			 
		 }
	 }
}
void CDlgMatrixTag::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent==TIMER_SETAUTO)
	{
		KillTimer(nIDEvent);

		if (g_bAutoPlayCheckBox)
		{
			m_ButtonAutoScan.m_bCheckedLiveview=1;
		}

		CRect rcWnd;
		GetClientRect(&rcWnd);

		m_rcDispAutoText=rcWnd;
		m_rcDispAutoText.left=rcWnd.left+200;
		m_rcDispAutoText.right=m_rcDispAutoText.left+20;
		m_rcDispAutoText.top=rcWnd.top+110;
		m_rcDispAutoText.bottom=m_rcDispAutoText.top+20;
	}
	else if (nIDEvent==TIMER_DISPLAY_AUTOPLAY)
	{
		//KillTimer(nIDEvent);

		CClientDC dc(this);
		dc.SetTextColor(FONTCOLOR);
		dc.FillSolidRect(m_rcDispAutoText,BKCOLOR);

		m_nAutoScanCount--;
		CString csText;
		csText.Format(L"%d",m_nAutoScanCount);

		if(m_rcDispAutoText.IsRectEmpty())
			return;

		if(m_nAutoScanCount>0)
		{
			dc.DrawText(csText,m_rcDispAutoText,DT_LEFT);
		}
		else
		{
			KillTimer(TIMER_DISPLAY_AUTOPLAY);
			return;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgMatrixTag::vSetMyCBoxID(UINT nMyCBoxID)
{
	m_ButtonAutoScan.m_nMyCBoxID=nMyCBoxID;
}

void CDlgMatrixTag::vStartDisplayAutoPlay(int nAutoPlayTime)
{
	KillTimer(TIMER_DISPLAY_AUTOPLAY);

	m_nAutoScanTime=nAutoPlayTime;
	m_nAutoScanCount=nAutoPlayTime;

	SetTimer(TIMER_DISPLAY_AUTOPLAY,1000,0);
}

int CDlgMatrixTag::vBitmapIDToMartixCode(int nBitmapID)
{
	int nMartixCode=0;
	//int nID[9]={IDB_BITMAP_FULL, IDB_BITMAP_1X1, IDB_BITMAP_2X2, IDB_BITMAP_3X3, IDB_BITMAP_4X4, IDB_BITMAP_1P5, IDB_BITMAP_2P8, IDB_BITMAP_1P12, IDB_BITMAP_1P12_2};//, IDB_BITMAP_6X6, IDB_BITMAP_7X7
	//int nIDAll[15]={IDB_BITMAP_FULL, IDB_BITMAP_1X1, IDB_BITMAP_2X2, IDB_BITMAP_3X3, IDB_BITMAP_4X4, IDB_BITMAP_1P5, IDB_BITMAP_2P8, IDB_BITMAP_1P12, IDB_BITMAP_1P12_2, IDB_BITMAP_1P16, IDB_BITMAP_6X6, IDB_BITMAP_2P18, IDB_BITMAP_7X7, IDB_BITMAP_8X8, IDB_BITMAP_2P32};
	switch(nBitmapID)
	{
	case IDB_BITMAP_1X1:
		nMartixCode = 1;	//_T("1 X 1");
		break;
	case IDB_BITMAP_2X2:
		nMartixCode =22;	// _T("2 X 2");
		break;
	case IDB_BITMAP_3X3:
		nMartixCode =3;	// _T("3 X 3");
		break;
	case IDB_BITMAP_4X4:
		nMartixCode =4;	// _T("4 X 4");
		break;
	case IDB_BITMAP_6X6:
		nMartixCode =6;	// _T("6 X 6");
		break;
	case IDB_BITMAP_7X7:
		nMartixCode =7;	// _T("7 X 7");
		break;
	case IDB_BITMAP_8X8:
		nMartixCode =8;	// _T("8 X 8");
		break;
	case IDB_BITMAP_1P5:
		nMartixCode =15;	// _T("1 + 5");
		break;
	case IDB_BITMAP_2P8:
		nMartixCode =28;	// _T("2 + 8");
		break;
	case IDB_BITMAP_1P12:
		nMartixCode =121;	// _T("1 + 12");
		break;
	case IDB_BITMAP_1P12_2:
		nMartixCode =122;	// _T("1 + 12");
		break;
	case IDB_BITMAP_1P16:
		nMartixCode =116;	// _T("1 + 16");
		break;
	case IDB_BITMAP_2P18:
		nMartixCode =218;	// _T("2 + 18");
		break;
	case IDB_BITMAP_2P32:
		nMartixCode =232;	// _T("2 + 32");
		break;
	}

	return nMartixCode;
}

int CDlgMatrixTag::vMartixCodeToBitmapID(int nMartixCode)
{
	int nBitmapID=0;
	//int nID[9]={IDB_BITMAP_FULL, IDB_BITMAP_1X1, IDB_BITMAP_2X2, IDB_BITMAP_3X3, IDB_BITMAP_4X4, IDB_BITMAP_1P5, IDB_BITMAP_2P8, IDB_BITMAP_1P12, IDB_BITMAP_1P12_2};//, IDB_BITMAP_6X6, IDB_BITMAP_7X7
	switch(nMartixCode)
	{
	case 1:
		nBitmapID = IDB_BITMAP_1X1;	//_T("1 X 1");
		break;
	case 2:
		nBitmapID =IDB_BITMAP_2X2;	// _T("2 X 2");
		break;
	case 3:
		nBitmapID =IDB_BITMAP_3X3;	// _T("3 X 3");
		break;
	case 4:
		nBitmapID =IDB_BITMAP_4X4;	// _T("4 X 4");
		break;
	case 6:
		nBitmapID =IDB_BITMAP_6X6;	// _T("6 X 6");
		break;
	case 7:
		nBitmapID =IDB_BITMAP_7X7;	// _T("7 X 7");
		break;
	case 8:
		nBitmapID =IDB_BITMAP_8X8;	// _T("8 X 8");
		break;
	case 15:
		nBitmapID =IDB_BITMAP_1P5;	// _T("1 + 5");
		break;
	case 28:
		nBitmapID =IDB_BITMAP_2P8;	// _T("2 + 8");
		break;
	case 121:
		nBitmapID =IDB_BITMAP_1P12;	// _T("1 + 12");
		break;
	case 122:
		nBitmapID =IDB_BITMAP_1P12_2;	// _T("1 + 12");
		break;
	case 116:
		nBitmapID =IDB_BITMAP_1P16;	// _T("1 + 16");
		break;
	case 218:
		nBitmapID =IDB_BITMAP_2P18;	// _T("2 + 18");
		break;
	case 232:
		nBitmapID =IDB_BITMAP_2P32;	// _T("2 + 32");
		break;
	}

	return nBitmapID;
}

void CDlgMatrixTag::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bSetStyle)
	{
		OnBnClickedClose();
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgMatrixTag::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_bSetStyle)
	{
		CDialog::OnCancel();
	}
}