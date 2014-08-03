// DlgMatrixEvent.cpp : implementation file
//

#include "stdafx.h"
#include "DlgMatrixEvent.h"
#include "MainFrm.h"
#include "DlgMatrixTab.h"
int nIDEvent[]={IDB_BITMAP_1E3, IDB_BITMAP_1E5, IDB_BITMAP_1E9, IDB_BITMAP_1E12, IDB_BITMAP_1E16};
int nIDHotEvent[]={IDB_BITMAP_1E3_H, IDB_BITMAP_1E5_H, IDB_BITMAP_1E9_H, IDB_BITMAP_1E12_H, IDB_BITMAP_1E16_H};
// CDlgMatrixEvent dialog

IMPLEMENT_DYNAMIC(CDlgMatrixEvent, CDialog)

CDlgMatrixEvent::CDlgMatrixEvent(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatrixEvent::IDD, pParent)
{
	 btnE2x2 = new CButtonST;
	 btnE1P5 = new CButtonST;
	 btnE2P8 = new CButtonST;
	 btnE1P12_2 = new CButtonST;
	 btnE1P16 = new CButtonST;
	   mapTable[IDB_BITMAP_1E3]=12;mapTable[IDB_BITMAP_1E5]=115;mapTable[IDB_BITMAP_1E9]=128;
    mapTable[IDB_BITMAP_1E12]=121;mapTable[IDB_BITMAP_1E16]=1116;	// mapTable[IDB_BITMAP_1E12]=1122;

}

CDlgMatrixEvent::~CDlgMatrixEvent()
{
	delete btnE2x2;
	delete btnE1P5;
	delete btnE2P8;
	delete btnE1P12_2;
	delete btnE1P16;
}

void CDlgMatrixEvent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMatrixEvent, CDialog)
	ON_WM_ERASEBKGND()
	//ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDB_EVEP3, &CDlgMatrixEvent::OnBnClickedE2x2)
	ON_BN_CLICKED(IDB_EVEP5, &CDlgMatrixEvent::OnBnClickedE1P5)
	ON_BN_CLICKED(IDB_EVEP9, &CDlgMatrixEvent::OnBnClickedE2P8)
	ON_BN_CLICKED(IDB_EVEP12, &CDlgMatrixEvent::OnBnClickedE1P12_2)
	ON_BN_CLICKED(IDB_EVEP16, &CDlgMatrixEvent::OnBnClickedE1P16)
    ON_CONTROL_RANGE(BN_CLICKED, IDB_BITMAP_1E3, IDB_BITMAP_1E16, &CDlgMatrixEvent::OnMatrixBnClicked)
END_MESSAGE_MAP()


// CDlgMatrixEvent message handlers

BOOL CDlgMatrixEvent::OnInitDialog()
{
	CDialog::OnInitDialog();
    	
    //if (((DlgMatrixTab*)GetParent()->GetParent())->bDialogStyle())
    //{
    //    m_Brushbk.CreateSolidBrush(GLOBALBKCOLOR);
    //}
    //else
    {
        m_Brushbk.CreateSolidBrush(BKCOLOR);
    }

	CRect rect;
	GetParent()->GetClientRect(rect);
// here is your coordinates
	/*int i_x=pos.x;
	int i_y=pos.y;*/
	int i_x=rect.Width();
	int i_y=rect.Height();
	int i_h=70, i_w=48, i_gap=30, i_sw=10, i_dw=410, i_y1=50 ;

	SetWindowPos(NULL, i_x-i_dw, i_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    CreateMatrixs();

	//// TODO:  Add extra initialization here
	//DWORD dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;
 //   COLORREF clBG = MASKCOLOR;
 //   int nHoveOver = (int)BTNST_AUTO_GRAY;

	//CRect rc;
	//GetClientRect(rc);

	//btnE2x2->Create(NULL, dwStyle, CRect(i_sw, i_y1, i_sw+ i_w+10, i_y1+i_h +10), this, IDB_EVEP3);
	//btnE2x2->SetBitmaps(IDB_EVEP3, clBG ,nHoveOver);
	//m_rectButton.push_back(CRect(i_sw, i_y1, i_sw+ i_w, i_y1+i_h +10));

	//i_x = i_sw+ i_w + i_gap;
	//btnE1P5->Create(NULL, dwStyle, CRect(i_x, i_y1, i_x+i_w+10, i_y1+i_h +10), this, IDB_EVEP5);
	//btnE1P5->SetBitmaps(IDB_EVEP5, clBG ,nHoveOver);
	//m_rectButton.push_back( CRect(i_x, i_y1, i_x+i_w+10, i_y1+i_h +10));

	//i_x = i_x+ i_w + i_gap;
	//btnE2P8->Create(NULL, dwStyle,CRect(i_x, i_y1, i_x+i_w+10, i_y1+i_h +10), this, IDB_EVEP9);
	//btnE2P8->SetBitmaps(IDB_EVEP9, clBG ,nHoveOver);
	//m_rectButton.push_back(CRect(i_x, i_y1, i_x+i_w, i_y1+i_h +10));

	//i_x = i_x+ i_w + i_gap;
	//btnE1P12_2->Create(NULL, dwStyle,CRect(i_x, i_y1, i_x+i_w+10, i_y1+i_h +10), this, IDB_EVEP12);
	//btnE1P12_2->SetBitmaps(IDB_EVEP12, clBG ,nHoveOver);
	//m_rectButton.push_back(CRect(i_x, i_y1, i_x+i_w, i_y1+i_h +10));

	//i_x = i_x+ i_w + i_gap;
	//btnE1P16->Create(NULL, dwStyle,CRect(i_x, i_y1, i_x+i_w+10, i_y1+i_h +10), this, IDB_EVEP16);
	//btnE1P16->SetBitmaps(IDB_EVEP16, clBG ,nHoveOver);
	//m_rectButton.push_back(CRect(i_x, i_y1, i_x+i_w, i_y1+i_h +10));


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CDlgMatrixEvent::CreateMatrixs()
{
	 CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
    //int nBtnWidth(48+10), nBtnHeight(70), nWidthGap(10), nHightGap(10);
    int nBtnWidth(39), nBtnHeight(41), nWidthGap(10), nHightGap(10);
    int nLeftMargin(10), nTopMargin(40);
    
    int nButtonSize = sizeof(nIDEvent)/ sizeof(int);
    CRect rect;
    GetClientRect(rect);
    int nColumn = (rect.Width() - nLeftMargin*2) / (nBtnWidth +nWidthGap);
    int nRow = (rect.Height() - nTopMargin) / nColumn + (((rect.Height() - nTopMargin) % nColumn ) ?  1 : 0);
  

    DWORD dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;//BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE |WS_CHILD|WS_VISIBLE|BS_OWNERDRAW ;
	COLORREF clBG = RGB(0,0,0);
    int nHoveOver = (int)BTNST_AUTO_GRAY;
	UINT nSelectID(0);
    
    for (int cj = 0; cj < nRow; cj++)
    {    
        for (int ci = 0; ci < nColumn; ci++)
        {
            if ((cj* nColumn + ci) < nButtonSize)
            {
                int nX0 = nLeftMargin + (nBtnWidth +nWidthGap)* ci;
                int nX1 = nX0 +nBtnWidth;
                int nY0 = nTopMargin + (nBtnHeight +nHightGap)* cj;
                int nY1 = nY0 +nBtnHeight;
                CButtonST* pBtn = new CButtonST;
	            pBtn->Create(NULL, dwStyle, CRect(nX0, nY0, nX1, nY1), this, nIDEvent[nColumn* cj +ci]);
                if(mapTable[nIDEvent[nColumn* cj +ci]] == pMain->m_OrgLayoutIdx)
				{
					pBtn->SetBitmaps(nIDHotEvent[nColumn* cj +ci], clBG);
					g_uIDBtnMatrix = nIDEvent[nColumn* cj +ci];
				}
				else
				{
					pBtn->SetBitmaps(nIDHotEvent[nColumn* cj +ci], clBG ,nHoveOver,  nIDEvent[nColumn* cj +ci]);
				}
	            m_vtBtnMatrixs.push_back(pBtn);
	        }
        }
    }
    
    return true;    
}

void CDlgMatrixEvent::OnMatrixBnClicked(UINT uid)
{
    CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();  
    pMain->m_OrgLayoutIdx = mapTable[uid];  
    pMain->vGetIpcamTreeCtrl()->vLiveView();
	vSetMatrixBtnHot(uid);		
	((DlgMatrixTab*)(GetParent()->GetParent()))->vRestoreBtnState(1);//1 stands for RESTORE_BTN_TYPE::EVENTBTN
    if (((DlgMatrixTab*)GetParent()->GetParent())->bDialogStyle())
    {
       ::PostMessage((GetParent()->GetParent())->m_hWnd,WM_CLOSE, NULL, NULL);
    }
}
BOOL CDlgMatrixEvent::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_Brushbk);

	return true;//CDialog::OnEraseBkgnd(pDC);
}


void CDlgMatrixEvent::OnBnClickedE2x2()
{
	EndDialog(12);
	::PostMessage((GetParent()->GetParent())->m_hWnd,ID_MATRIX_SELECTED,(WPARAM)12,NULL);
}
void CDlgMatrixEvent::OnBnClickedE1P5()
{
	EndDialog(115);
	::PostMessage((GetParent()->GetParent())->m_hWnd,ID_MATRIX_SELECTED,(WPARAM)115,NULL);
}
void CDlgMatrixEvent::OnBnClickedE2P8()
{
	EndDialog(128);
	::PostMessage((GetParent()->GetParent())->m_hWnd,ID_MATRIX_SELECTED,(WPARAM)128,NULL);
}
void CDlgMatrixEvent::OnBnClickedE1P12_2()
{
	EndDialog(1122);
	::PostMessage((GetParent()->GetParent())->m_hWnd,ID_MATRIX_SELECTED,(WPARAM)1122,NULL);
}
void CDlgMatrixEvent::OnBnClickedE1P16()
{
	EndDialog(1116);
	::PostMessage((GetParent()->GetParent())->m_hWnd,ID_MATRIX_SELECTED,(WPARAM)1116,NULL);
}

void CDlgMatrixEvent::vSetMatrixBtnHot(UINT nId)
{
	 int nButtonSize = sizeof(nIDEvent)/ sizeof(int);
	 COLORREF clBG = RGB(0,0,0);
    int nHoveOver = (int)BTNST_AUTO_GRAY;
	
	 // restore all to original bitmap
	 for(int i = 0; i < m_vtBtnMatrixs.size();i++)
	 {
		 m_vtBtnMatrixs[i]->SetBitmaps(nIDHotEvent[i], clBG ,nHoveOver,  nIDEvent[i]);
	 }

	 // set selected matrix button to hot
	 for(int i =0;i<nButtonSize;i++)
	 {
		if(nIDEvent[i] == nId)
		{
			m_vtBtnMatrixs[i]->SetBitmaps(nIDHotEvent[i], clBG);
			g_uIDBtnMatrix=nId;
			break;
		}
	 }
}
void CDlgMatrixEvent::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	//CDialog::OnCancel();
}
