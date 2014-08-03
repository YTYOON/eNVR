// DlgAppTag.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAppTag.h"

// CDlgAppTag dialog

IMPLEMENT_DYNAMIC(CDlgAppTag, CDialog)

CDlgAppTag::CDlgAppTag(CWnd* pParent)
	: CDialog(CDlgAppTag::IDD, pParent)
{
	btnclose = new CButtonST;
	btnemap  = new CButtonST;
	btnpback = new CButtonST;
	btnac	 = new CButtonST;
	btngate	 = new CButtonST;
	btnlpr = new CButtonST;
}

CDlgAppTag::~CDlgAppTag()
{
	delete btnclose ;
	delete btnemap;
	delete btnpback;
	delete btnac;
	delete btnlpr;

	m_rectButton.clear();
}

void CDlgAppTag::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAppTag, CDialog)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDB_CLOSE, &CDlgAppTag::OnBnClickedClose)
	ON_BN_CLICKED(IDB_EMAP, &CDlgAppTag::OnBnClickedEmap)
	ON_BN_CLICKED(IDB_PLAYBACK, &CDlgAppTag::OnBnClickedPlayback)
	ON_BN_CLICKED(IDB_ACCESSD, &CDlgAppTag::OnBnClickedAc)
	ON_BN_CLICKED(IDB_GATE, &CDlgAppTag::OnBnClickedGate)
	ON_BN_CLICKED(IDB_LPR, &CDlgAppTag::OnBnClickedLpr)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgAppTag message handlers

BOOL CDlgAppTag::OnInitDialog()
{
	//CDialog::OnInitDialog();
	m_vtRValue.clear();
	m_vtRValue.push_back(-1);
 
    CString csHint;
	POINT pos;
	GetCursorPos(&pos);
// here is your coordinates
	int i_x=pos.x;
	int i_y=pos.y;
	int i_w=48, i_h=70, i_gap=30, i_sw=10, i_dw=200, i_pgap=18;//, i_neww=260;

	//Check ini file for access control enabled
	m_ini.setINIFullFileName(_T("\\ec.ini"));
	CString csAC = m_ini.getKeyValue(_T("access_control"),_T("access control"));
	if (csAC.MakeUpper() == _T("Y"))
			mb_AC = true;
	else
			mb_AC = false;	

	csAC = m_ini.getKeyValue(_T("parking_gate"),_T("parking gate"));
	if (csAC.MakeUpper() == _T("Y"))
			mb_gate = true;
	else
			mb_gate = false;	

	csAC = m_ini.getKeyValue(_T("lpr"),_T("lpr"));
	if (csAC.MakeUpper() == _T("Y"))
			mb_lpr = true;
	else
			mb_lpr = false;	
	
	//SetWindowPos(NULL, i_x-i_dw, i_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	SetWindowPos(NULL, i_x, i_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	//Expand the dlg if Access Control
	if (mb_AC) i_dw +=65;
	if (mb_gate) i_dw += 65;
	if (mb_lpr) i_dw += 75;

	//MoveWindow( i_x-i_dw - i_pgap, i_y, i_dw, 120, true);
    MoveWindow( i_x, i_y, i_dw +i_pgap, 120, true);


 
	// TODO:  Add extra initialization here
    COLORREF clBG = MASKCOLOR;
    int nHoveOver = (int)BTNST_AUTO_GRAY;
	DWORD dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;
 
	CRect rc;
	GetClientRect(rc);

	btnclose->Create(NULL, dwStyle, CRect(rc.Width()-17, 1, rc.Width()-1, 17), this, IDB_CLOSE);
	btnclose->SetBitmaps(IDB_CLOSE, clBG ,nHoveOver);
	m_rectButton.push_back(CRect(rc.Width()-17, 1, rc.Width()-1, 17));

	btnemap->Create(NULL, dwStyle, CRect(i_sw, i_sw, i_sw+ i_w +10, i_sw+i_h +10), this, IDB_EMAP);
	btnemap->SetBitmaps(IDB_EMAP, clBG ,nHoveOver);
    LOADSTRING(csHint, IDS_HINT_EMAP);
    btnemap->SetWindowText(csHint);
    btnemap->SetAlign(CButtonST::ST_ALIGN_VERT);
	m_rectButton.push_back(CRect(i_sw, i_sw, i_sw+ i_w, i_sw+i_h));
	m_vtRValue.push_back(EMAPID);

	i_x = i_sw+ i_w + i_gap;
	btnpback->Create(NULL, dwStyle, CRect(i_x, i_sw, i_x+i_w +10, i_sw+i_h +10), this, IDB_PLAYBACK);
	btnpback->SetBitmaps(IDB_PLAYBACK, clBG ,nHoveOver);
    LOADSTRING(csHint, IDS_HINT_PLAYBACK);
    btnpback->SetWindowText(csHint);
    btnpback->SetAlign(CButtonST::ST_ALIGN_VERT);

	m_rectButton.push_back(CRect(i_x, i_sw, i_x+i_w, i_sw+i_h));
	m_vtRValue.push_back(PLAYBACKID);

	if (mb_AC)
	{
		i_x = i_x + i_w + i_gap;
		btnac->Create(NULL, dwStyle, CRect(i_x, i_sw, i_x+i_w +10, i_sw+ i_h +10), this, IDB_ACCESSD);
		btnac->SetBitmaps(IDB_ACCESSD, clBG ,nHoveOver);
        LOADSTRING(csHint, IDS_HINT_ACCESS_CONTROL);
        btnac->SetWindowText(csHint);
        btnac->SetAlign(CButtonST::ST_ALIGN_VERT);
		m_rectButton.push_back(CRect(i_x, i_sw, i_x+i_w, i_sw+i_h));
		m_vtRValue.push_back(ACID);
	}

	if (mb_gate)
	{
		i_x = i_x + i_w + i_gap;
		btngate->Create(NULL, dwStyle, CRect(i_x, i_sw, i_x+i_w +10, i_sw+ i_h +10), this, IDB_GATE);
		btngate->SetBitmaps(IDB_GATE, clBG ,nHoveOver);
        LOADSTRING(csHint, IDS_HINT_GATE);
        btngate->SetWindowText(csHint);
        btngate->SetAlign(CButtonST::ST_ALIGN_VERT);
		m_rectButton.push_back(CRect(i_x, i_sw, i_x+i_w, i_sw+i_h));
		m_vtRValue.push_back(GATEID);
	}

	if (mb_lpr)
	{
		i_x = i_x + i_w + i_gap;
		btnlpr->Create(NULL, dwStyle, CRect(i_x, i_sw, i_x+i_w +10, i_sw+ i_h +10), this, IDB_LPR);
		btnlpr->SetBitmaps(IDB_LPR, clBG ,nHoveOver);
        LOADSTRING(csHint, IDS_HINT_LPR);
        btnlpr->SetWindowText(csHint);
        btnlpr->SetAlign(CButtonST::ST_ALIGN_VERT);
		m_rectButton.push_back(CRect(i_x, i_sw, i_x+i_w, i_sw+i_h));
		m_vtRValue.push_back(LPRID);
	}
    SetCapture();
	return TRUE;  
	
	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgAppTag::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CBrush brush(GLOBALBKCOLOR);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &brush);

	return true;//CDialog::OnEraseBkgnd(pDC);
}

void CDlgAppTag::OnBnClickedClose()
{
	EndDialog(-1);
}

void CDlgAppTag::OnBnClickedEmap()
{
	EndDialog(EMAPID);
}


void CDlgAppTag::OnBnClickedPlayback()
{
	EndDialog(PLAYBACKID);
}

void CDlgAppTag::OnBnClickedLpr()
{
	//Open License Plate Application here...
	EndDialog(LPRID);
	
}

void CDlgAppTag::OnBnClickedAc()
{
	EndDialog(ACID);	
}

void CDlgAppTag::OnBnClickedGate()
{
	EndDialog(GATEID);	
}

void CDlgAppTag::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CRect rect;
	GetClientRect(rect);

	/*
	int nReturnValue[]={-1, EMAPID, PLAYBACKID, ACID, GATEID, LPRID};
	
	int nHitInx = -1;
    if (rect.PtInRect(point))
    {
        for(int i=0; i<m_rectButton.size(); i++)
	    {
		    if(m_rectButton[i].PtInRect(point))
		    {
                ASSERT( i < sizeof(nReturnValue)/sizeof(int));
                nHitInx = nReturnValue[i];
			    break;
		    }
	    }   	   
    }*/

	int nHitInx = -1;
	if (rect.PtInRect(point))
	{
		for(int i=0; i<m_rectButton.size(); i++)
		{
			if(m_rectButton[i].PtInRect(point))
			{
				//ASSERT( i < sizeof(pRValue)/sizeof(int));
				nHitInx = m_vtRValue[i];
				break;
			}		  
		}
	}
    ReleaseCapture();
	EndDialog( nHitInx );
}
