#include "stdafx.h"
#include "EventVideo.h"
#include "DlgEmap.h"
#include "DlgAc.h"
#include "DlgGate.h"
#include "DlgLPR.h"

#define	VIDEOW 200
#define	VIDEOH 150

// CEventVideo dialog

IMPLEMENT_DYNAMIC(CEventVideo, CDialog)

CEventVideo::CEventVideo(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CEventVideo::IDD, pParent),m_black(NULL)
{
	m_black = new CBrush;
	m_black->CreateSolidBrush(RGB(240,240,240));
}

CEventVideo::~CEventVideo(void)
{
}

void CEventVideo::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_STVV, m_videovv);
	DDX_Control(pDX, IDC_DOOR, mbut_door);
	DDX_Control(pDX, IDC_RAUDIO, mbut_raudio);
	DDX_Control(pDX, IDC_SAUDIO, mbut_saudio);
}

BEGIN_MESSAGE_MAP(CEventVideo, CBaseDialog)
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_DOOR, &CEventVideo::OnBnClickedDoor)
	ON_BN_CLICKED(IDC_RAUDIO, &CEventVideo::OnBnClickedRaudio)
	ON_BN_CLICKED(IDC_SAUDIO, &CEventVideo::OnBnClickedSaudio)
	//ON_MESSAGE(ID_CHANGE_SEND_AUDIO_BMP, &CEventVideo::vChangeSendAudioBMP)
END_MESSAGE_MAP()


BOOL CEventVideo::OnInitDialog()
{	
	CBaseDialog::OnInitDialog();

	mb_displayVideo = true;
	mb_door = false;

	if (mc_parent == 'J')	//LPR - JPG
	{
		if (m_title) SetWindowTextW(m_title);
		mbut_door.ShowWindow(SW_HIDE);
		mbut_raudio.ShowWindow(SW_HIDE);
		mbut_saudio.ShowWindow(SW_HIDE);
		return true;
	}

	if (mc_parent == 'A' || mc_parent=='G')
	{
		mbut_door.LoadBitmaps(IDB_KEYB, IDB_KEYR, IDB_KEYB, IDB_KEYB);
		mbut_door.ShowWindow(SW_SHOW);
		mbut_raudio.LoadBitmaps(IDB_BITMAP_AUDIO_OFF);
		mbut_raudio.ShowWindow(SW_SHOW);
		//send audio
		mbut_saudio.LoadBitmaps(IDB_BITMAP_OSD_MIC);
		mbut_saudio.ShowWindow(SW_SHOW);
	}
	
	fnDisplayVideo();
//	if (!mb_showdoor) fnShowDoor(mb_showdoor);
	fnShowDoor();
	if (m_title) SetWindowTextW(m_title);
	return true;  
}

//Display video triggerred by event
void CEventVideo::fnDisplayVideo()
{			
	m_videovv.Create(_T(" "), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0,0,0,0), this ,0);
	m_videovv.m_bDisplayOSD = false;
	m_videovv.vAttach(m_node);	
	m_videovv.MoveWindow(CRect(0, 0,  VIDEOW,   VIDEOH),true);
	m_videovv.ShowWindow(SW_SHOW);
	return;	
}

bool CEventVideo::fnAssignNode(NODEITEM* opNode)
{
	m_node = opNode;
	return true;
}

bool CEventVideo::fnAssignTitle(CString o_title)
{
	m_title = o_title;
	return true;
}

void CEventVideo::OnClose()
{
	m_videovv.vStopPlay();
	m_videovv.vStopRecv();
	m_videovv.DestroyWindow();
	mc_mode = 'C'; //Close

	if (mc_parent =='A')		
		((CDlgAc*)GetParent())->fnAfterVideoClose(m_camidx);		
	else if (mc_parent =='M')
		((CDlgEmap*)GetParent())->fnAfterVideoClose(m_camidx);
	else if (mc_parent =='G')
		((CDlgGate*)GetParent())->fnAfterVideoClose(m_camidx);
	else if (mc_parent =='L')		// License Plate
		((CDlgLPR*)GetParent())->fnAfterVideoClose(m_camidx);

	if (m_black)
	{
		m_black->DeleteObject();
		delete m_black;
	}
	CBaseDialog::OnClose();
}

BOOL CEventVideo::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);

	pDC->FillRect(&rect, m_black);
	return true;
}

void CEventVideo::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect	lrect;
	if (!m_videovv) return;
	m_videovv.GetWindowRect(lrect);
	
	//if (mb_ac)	//Access Control
	if (mc_parent == 'A' || mc_parent == 'G')
		m_videovv.MoveWindow(CRect(0,30,cx, cy), true);
	else //Emap
		m_videovv.MoveWindow(CRect(0,0,cx, cy), true);
}

void CEventVideo::OnBnClickedDoor()
{
 	if (mc_parent == 'A')		
		((CDlgAc*)GetParent())->fnOpenDoor(m_camidx);		
	else if (mc_parent == 'G')
		((CDlgGate*)GetParent())->fnOpenDoor(m_camidx);		
}

//For receive audio
void CEventVideo::OnBnClickedRaudio()
{
	if (m_videovv.m_bSetAudio)
			mbut_raudio.LoadBitmaps(IDB_BITMAP_AUDIO_OFF);
	else
			mbut_raudio.LoadBitmaps(IDB_BITMAP_AUDIO_OFF_H);
	
	m_videovv.OnRecvAudio();
}

void CEventVideo::OnBnClickedSaudio()
{
	if (!m_videovv.m_bAudioStart)
		mbut_saudio.LoadBitmaps(IDB_BITMAP_OSD_MIC_H);
	else
		mbut_saudio.LoadBitmaps(IDB_BITMAP_OSD_MIC);
	
	m_videovv.OnSendAudio();
}


LRESULT  CEventVideo::vChangeSendAudioBMP(WPARAM wParam, LPARAM lParam)
{
		//int nVideoIdx = wParam;	
		//m_videovv.m_pSendAudio->LoadBitmaps(IDB_BITMAP_OSD_SEND_AUDIO, IDB_BITMAP_OSD_SEND_AUDIO, IDB_BITMAP_OSD_SEND_AUDIO, IDB_BITMAP_OSD_SEND_AUDIO);
		//m_videos[ci]->m_pSendAudio->LoadBitmaps(IDB_BITMAP_OSD_SEND_AUDIO, IDB_BITMAP_OSD_SEND_AUDIO, IDB_BITMAP_OSD_SEND_AUDIO, IDB_BITMAP_OSD_SEND_AUDIO);


	
	//if(lParam ==  1)// change to orignal BMP and stop send audio when Autoplay enable 
	//{
	//	for(int ci=0; ci < m_videos.size(); ci++)
	//	{
	//		if(m_videos[ci]->m_pSendAudio)
	//		{				
	//		   m_videos[ci]->m_pSendAudio->LoadBitmaps(IDB_BITMAP_OSD_SEND_AUDIO, IDB_BITMAP_OSD_SEND_AUDIO, IDB_BITMAP_OSD_SEND_AUDIO, IDB_BITMAP_OSD_SEND_AUDIO);
	//		}

	//		//m_videos[ci]->sendAudio.vStopSendAudio();
	//	}
	//}
	//else 
	//{

	//	for(int ci=0; ci < m_videos.size(); ci++)
	//	{
	//		if(ci == nVideoIdx)
	//		{
	//			if(m_videos[ci]->m_pSendAudio)
	//			{
	//				m_videos[ci]->m_pSendAudio->LoadBitmaps(IDB_BITMAP_OSD_SEND_AUDIO_USE, IDB_BITMAP_OSD_SEND_AUDIO_USE,IDB_BITMAP_OSD_SEND_AUDIO_USE, IDB_BITMAP_OSD_SEND_AUDIO_USE);	
	//				m_videos[ci]->m_pSendAudio->Invalidate();
	//				
	//			}
	//		}
	//		else
	//		{							
	//			if(m_videos[ci]->m_pSendAudio)
	//			{				
	//				m_videos[ci]->m_pSendAudio->LoadBitmaps(IDB_BITMAP_OSD_SEND_AUDIO, IDB_BITMAP_OSD_SEND_AUDIO, IDB_BITMAP_OSD_SEND_AUDIO, IDB_BITMAP_OSD_SEND_AUDIO);
	//				m_videos[ci]->m_pSendAudio->Invalidate();
	//			}		
	//		}	
	//	}
	//}
	return 0;
}


void CEventVideo::fnShowDoor()
{
	if (mb_door)
		mbut_door.ShowWindow(SW_SHOW);
	else
		mbut_door.ShowWindow(SW_HIDE);
/*	try{
		bool	lb_control=false;
		CString	ls_control, ls_col;
		if (pDB)
		{
			CString ls_sql;
			if (mc_parent=='G')		//Gate
			{
				ls_col = _T("gate_control");
				ls_sql.Format(_T("SELECT gate_control FROM ecgt_camera WHERE camera_idx=%d"), m_camidx);
			}
 			else if (mc_parent=='A')	//AC
			{
				ls_col = _T("ac_control");
				ls_sql.Format(_T("SELECT ac_control FROM ecac_camera WHERE camera_idx=%d "), m_camidx);
			}
			else
				return;

			TRACE(ls_sql);
			CRecordset m_rs(pDB);	 
			m_rs.Open(CRecordset::dynamic, ls_sql);
			while(!m_rs.IsEOF() ) 
			{
					m_rs.GetFieldValue(ls_col, ls_control);
					if (ls_control==_T("Y")) 
							lb_control = true;
					break;
			}
			
			mbut_door.ShowWindow(lb_control);
		}
	}
		catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
		*/
}
