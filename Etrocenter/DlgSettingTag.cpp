// DlgSettingTag.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSettingTag.h"

#define	ACCESSW 305
#define	ACCESSH 260

// CDlgSettingTag dialog

IMPLEMENT_DYNAMIC(CDlgSettingTag, CDialog)


CDlgSettingTag::CDlgSettingTag(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSettingTag::IDD, pParent)
{
	btnclose = new CButtonST;
	btnaddcam = new CButtonST;
	btnip = new CButtonST;
	btnevent = NULL;//new CBitmapButton;
	btncamgroup = new CButtonST;
	btnemap = new CButtonST;
	btnrecording = new CButtonST;
	btnmulmon = new CButtonST;
	btnsystem = new CButtonST;
	btnftp = new CButtonST;
	btnac = new CButtonST;
	btnpgate = new CButtonST;
	btneventaction = new CButtonST;
	btnnas = new CButtonST;
	pstMulti = new CStatic;
}

CDlgSettingTag::~CDlgSettingTag()
{
	delete btnclose;
	delete btnaddcam;
	delete btnip;
	delete btncamgroup;
	delete btnemap;
	delete btnrecording;
	delete btnmulmon;
	delete btnsystem;
	delete btnftp;
	delete btnac;
	delete btnpgate;
	delete btneventaction;
	delete btnnas;
	delete pstMulti;
	m_rectButton.clear();
}

void CDlgSettingTag::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSettingTag, CDialog)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDB_CLOSE, &CDlgSettingTag::OnBnClickedClose)
	ON_BN_CLICKED(IDB_CAMERA, &CDlgSettingTag::OnBnClickedAddCam)
	ON_BN_CLICKED(IDB_IP, &CDlgSettingTag::OnBnClickedIp)
	//ON_BN_CLICKED(IDB_EVENT, &CDlgSettingTag::OnBnClickedEvent)
	ON_BN_CLICKED(IDB_CAMGROUP, &CDlgSettingTag::OnBnClickedCamgroup)
	ON_BN_CLICKED(IDB_RECORDING, &CDlgSettingTag::OnBnClickedRecording)
	ON_BN_CLICKED(IDB_SYSTEM, &CDlgSettingTag::OnBnClickedSystem)
	ON_BN_CLICKED(IDB_MULMON, &CDlgSettingTag::OnBnClickedMulmon)
	ON_BN_CLICKED(IDB_ACCESSD, &CDlgSettingTag::OnBnClickedAccess)
	ON_BN_CLICKED(IDB_GATE, &CDlgSettingTag::OnBnClickedPGate)
	ON_BN_CLICKED(IDB_EVENT, &CDlgSettingTag::OnBnClickedEventAction)
	ON_BN_CLICKED(IDB_NAS, &CDlgSettingTag::OnBnClickedNAS)

	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


BOOL CDlgSettingTag::OnInitDialog()
{
    CString csHint;
	POINT pos;
	GetCursorPos(&pos);
 
	int i_x=pos.x;
	int i_y=pos.y;
	int i_w=48, i_h=90, i_gap=45, i_sw=20, i_dw=310, i_pgap=18 ;

	//Check ini file for access control enabled
	m_ini.setINIFullFileName(_T("\ec.ini"));
	CString csAC = m_ini.getKeyValue(_T("access_control"),_T("access control"));
	if (csAC.MakeUpper() == _T("Y"))
			mb_AC = true;
	else
			mb_AC = false;

	CString csGate = m_ini.getKeyValue(_T("parking_gate"),_T("parking gate"));
	if (csGate.MakeUpper() == _T("Y"))
			mb_gate = true;
	else
			mb_gate = false;


	//SetWindowPos(NULL, i_x-i_dw, i_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	SetWindowPos(NULL, i_x, i_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	//Expand the dlg if Access Control
	int	llw = 0, llh=0;
//	if (mb_AC || mb_gate)
//	{
		llw= ACCESSW;
		llh = ACCESSH;
//	}
	
	if (llw>0)
		MoveWindow( i_x , i_y, llw, llh, true);
		//MoveWindow( i_x -i_dw - i_pgap  , i_y, llw, llh, true);

    COLORREF clBG = MASKCOLOR;
    int nHoveOver = (int)BTNST_AUTO_GRAY;
	DWORD dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;

	CRect rc;
	GetClientRect(rc);

	//Close Bitmap
	btnclose->Create(NULL, dwStyle, CRect(rc.Width()-17, 1, rc.Width()-1, 17), this, IDB_CLOSE);
    btnclose->SetBitmaps(IDB_CLOSE, clBG ,nHoveOver);
	m_rectButton.push_back(CRect(rc.Width()-17, 1, rc.Width()-1, 17));

	//Assign IP - 1st Row
	btnip->Create(NULL, dwStyle, CRect(i_sw, i_sw, i_sw+ i_w+10, i_sw+i_h), this,IDB_IP);
	btnip->SetBitmaps(IDB_IP, clBG ,nHoveOver);
    LOADSTRING(csHint, IDS_HINT_ASSIGN_IP);
    btnip->SetWindowText(csHint);
    btnip->SetAlign(CButtonST::ST_ALIGN_VERT);
	m_rectButton.push_back(CRect(i_sw, i_sw, i_sw+ i_w, i_sw+i_h));

	//Add Camera
	i_x = i_sw+ i_w + i_gap;
	btnaddcam->Create(NULL, dwStyle, CRect(i_x, i_sw, i_x+i_w+10, i_sw+i_h), this, IDB_CAMERA);
	btnaddcam->SetBitmaps(IDB_CAMERA, clBG ,nHoveOver);
    LOADSTRING(csHint, IDS_HINT_ADD_CAMERA);
    btnaddcam->SetWindowText(csHint);
    btnaddcam->SetAlign(CButtonST::ST_ALIGN_VERT);
	m_rectButton.push_back(CRect(i_x, i_sw, i_x+i_w, i_sw+i_h+20));

	//Camera Group
	i_x = i_x+ i_w + i_gap;
	btncamgroup->Create(NULL, dwStyle,CRect(i_x, i_sw, i_x+i_w+10, i_sw+i_h), this, IDB_CAMGROUP);
	btncamgroup->SetBitmaps(IDB_CAMGROUP, clBG ,nHoveOver);
    LOADSTRING(csHint, IDS_HINT_GROUP_SETTING);
    btncamgroup->SetWindowText(csHint);
    btncamgroup->SetAlign(CButtonST::ST_ALIGN_VERT);
	m_rectButton.push_back(CRect(i_x, i_sw, i_x+i_w, i_sw+i_h));

	//2nd Row
	i_x = i_x+ i_w + i_gap;
	int i_newy = i_sw+i_gap+i_w+15;

	btnrecording->Create(NULL, dwStyle, CRect(i_sw, i_newy,  i_sw+ i_w+3+10, i_newy+i_h), this, IDB_RECORDING);
	btnrecording->SetBitmaps(IDB_RECORDING, clBG ,nHoveOver);
    LOADSTRING(csHint, IDS_HINT_Recording);
    btnrecording->SetWindowText(csHint);
    btnrecording->SetAlign(CButtonST::ST_ALIGN_VERT);
	m_rectButton.push_back(CRect(i_sw, i_newy,  i_sw+ i_w+3, i_newy+i_h));

	//Multiple Monitors
	i_x = i_sw+ i_w + i_gap;
	btnmulmon->Create(NULL, dwStyle, CRect(i_x, i_newy,  i_x+ i_w+10, i_newy+i_h), this, IDB_MULMON);
	btnmulmon->SetBitmaps(IDB_MULMON, clBG ,nHoveOver);
    LOADSTRING(csHint, IDS_HINT_MONITORS);
    btnmulmon->SetWindowText(csHint);
    btnmulmon->SetAlign(CButtonST::ST_ALIGN_VERT);
	m_rectButton.push_back(CRect(i_x, i_newy,  i_x+ i_w, i_newy+i_h));
 
	////System
	//i_x = i_x + i_w + i_gap;
	//btnsystem->Create(NULL, dwStyle, CRect(i_x, i_newy,  i_x+ i_w+10, i_newy+i_h), this, IDB_SYSTEM);
	//btnsystem->SetBitmaps(IDB_SYSTEM, clBG ,nHoveOver);
 //   LOADSTRING(csHint, IDS_HINT_SYSTEM);
 //   btnsystem->SetWindowText(csHint);
 //   btnsystem->SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_rectButton.push_back(	CRect(i_x, i_newy,  i_x+ i_w, i_newy+i_h));
    
    SetCapture();

	////Access Control	 - 3rd Line
	//i_newy += i_h + 15;
	//int ll_g2w = 0;
	//if (mb_AC)
	//{
	//	btnac->Create(NULL, dwStyle, CRect(i_sw, i_newy,  i_sw+ i_w+10, i_newy+i_h), this, IDB_ACCESSD);
	//	btnac->SetBitmaps(IDB_ACCESSD, clBG ,nHoveOver);
	//	LOADSTRING(csHint, IDS_HINT_ACCESS_CONTROL);
	//	btnac->SetWindowText(csHint);
	//	btnac->SetAlign(CButtonST::ST_ALIGN_VERT);
	//	m_rectButton.push_back(	CRect(i_x, i_newy,  i_x+ i_w+10, i_newy+i_h));
	//	ll_g2w += i_w + i_gap;
	//}

	////Parking Gate Control
	//if (mb_gate)
	//{
	//	if (!mb_AC)
	//		i_x = i_sw;
	//	else
	//		i_x = i_sw+ i_w + i_gap;

	//	btnpgate->Create(NULL, dwStyle, CRect(i_x, i_newy,  i_x+ i_w+10, i_newy+i_h), this, IDB_GATE);
	//	btnpgate->SetBitmaps(IDB_GATE, clBG ,nHoveOver);
	//	LOADSTRING(csHint, IDS_HINT_GATE);
	//	btnpgate->SetWindowText(csHint);
	//	btnpgate->SetAlign(CButtonST::ST_ALIGN_VERT);
	//	m_rectButton.push_back(	CRect(i_x, i_newy,  i_x+ i_w+10, i_newy+i_h));
	//	ll_g2w += i_w + i_gap;
	//}
	//else if (mb_AC)
	//	i_x = i_sw;

	////Modify Groupbox 2
	//if (ll_g2w >0)
	//{
	//		GetDlgItem(IDC_GROUPBOX2)->ShowWindow(true);
	//		GetDlgItem(IDC_GROUPBOX2)->MoveWindow(i_sw - 10, i_newy - 10, ll_g2w, i_h + 15);
	//}

	////--Setting Event Action
	////i_x = i_x - i_w - i_gap;
	//if (!mb_AC && !mb_gate)
	//	i_x = i_sw;
	//else
	//	i_x = i_x + i_w + i_gap;

	//btneventaction->Create(NULL, dwStyle, CRect(i_x, i_newy,  i_x+ i_w+10, i_newy+i_h), this, IDB_EVENT);
	//btneventaction->SetBitmaps(IDB_EVENT, clBG ,nHoveOver);
	//LOADSTRING(csHint, IDS_HINT_EVENT_TRIGGER_ACTION);
	//btneventaction->SetWindowText(csHint);
	//btneventaction->SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_rectButton.push_back(	CRect(i_x, i_newy,  i_x+ i_w+10, i_newy+i_h));
/*
	//--Setting NAS
	i_newy += i_h + 15;
	btnnas->Create(NULL, dwStyle, CRect(i_sw, i_newy, i_sw+ i_w+10, i_newy+i_h), this, IDB_NAS);
	btnnas->SetBitmaps(IDB_NAS, clBG ,nHoveOver);
	LOADSTRING(csHint, IDS_HINT_NAS);
	btnnas->SetWindowText(csHint);
	btnnas->SetAlign(CButtonST::ST_ALIGN_VERT);
	m_rectButton.push_back(CRect(i_sw, i_newy, i_sw+ i_w+10, i_newy+i_h));
*/
	return TRUE;  
	
	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDlgSettingTag::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
 
	CBrush brush(GLOBALBKCOLOR);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &brush);
	 
	return true;//CDialog::OnEraseBkgnd(pDC);
}

void CDlgSettingTag::OnBnClickedClose()
{
	EndDialog(-1);
}

void CDlgSettingTag::OnBnClickedAddCam()
{
	EndDialog(1);	//Add New Camera
}

void CDlgSettingTag::OnBnClickedIp()
{
	EndDialog(ASSIGNIPID);
}

void CDlgSettingTag::OnBnClickedEvent()
{
	EndDialog(3);
}
 
void CDlgSettingTag::OnBnClickedCamgroup()
{
	EndDialog(4);
}

void CDlgSettingTag::OnBnClickedRecording()
{
	EndDialog(5);
}

void CDlgSettingTag::OnBnClickedMulmon()
{
	EndDialog(6);
}

void CDlgSettingTag::OnBnClickedSystem()
{
	EndDialog(SYSTEMID);
}

void CDlgSettingTag::OnBnClickedAccess()
{
	EndDialog(ACCESSID);
}

void CDlgSettingTag::OnBnClickedPGate()
{
	EndDialog(PGATEID);
}

void CDlgSettingTag::OnBnClickedEventAction()
{
	EndDialog(EVENTACTION);
}

void CDlgSettingTag::OnBnClickedNAS()
{
	EndDialog(NAS);
}

void CDlgSettingTag::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(rect);
	int nReturnValue[]={-1, ASSIGNIPID, 1, 4,/* 3,*/ 5, 6, SYSTEMID, ACCESSID };
	
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
    }
    ReleaseCapture();
	EndDialog( nHitInx );

	
	
}
