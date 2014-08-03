// OSD.cpp : implementation file
//

#include "stdafx.h"
#include "OSD.h"
#include "Video.h"
#include "MainFrm.h"

const COLORREF GRADIENTCOLOR1 RGB(100,100,100);
const COLORREF GRADIENTCOLOR2 RGB( 0, 0, 0);
//////  CStaticCaption
/////===========================================
IMPLEMENT_DYNAMIC(CStaticCaption, CStatic)
CStaticCaption::CStaticCaption()
{
}
CStaticCaption::~CStaticCaption()
{
}
BEGIN_MESSAGE_MAP(CStaticCaption, CStatic)
//{{AFX_MSG_MAP(CStaticCaption)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CStaticCaption::OnEraseBkgnd(CDC* pDC)
{
	//SetBkColor(pDC->m_hDC, RGB(0,0,0));
	CRect rect;
	GetClientRect(rect);
	pDC->SetTextColor(FONTCOLOR);	
	pDC->SetBkMode(TRANSPARENT);
    GetClientRect(rect);
    CGS::GradientFillRect(pDC, rect, GRADIENTCOLOR1, GRADIENTCOLOR2, GRADIENT_FILL_RECT_V);

    return true;
	//return CStatic::OnEraseBkgnd(pDC);
}

void CStaticCaption::OnPaint()
{
    CPaintDC dc(this); 
    CString cs;
    this->GetWindowText(cs);
    CRect rect;
    GetClientRect(rect);
    dc.ExtTextOut(0,0,ETO_CLIPPED,&rect, cs,NULL);
}

/////////=================================
// COSD

IMPLEMENT_DYNAMIC(COSD, CStatic)

COSD::COSD():m_pCaptionOSD(NULL),m_pSnapShot(NULL),m_pFixRatio(NULL),m_pSendAudio(NULL),m_pRecvAudio(NULL),m_pMore(NULL)
,m_pRecVideo(NULL),m_pReconnect(NULL),m_pDOTrigger(NULL),m_pInstantPlayBack(NULL)
{
	// the osd caption must be included as the first element of osd configuration. 
	if (!_SaveNewOSDItem)
	{
		m_vctnOSDBtnConfigAvailable.push_back(CAPTION);
	}
}

COSD::~COSD()
{	
    ::DeleteObject(m_hbr);
    ::DeleteObject(m_hBmpRecOn);
    ::DeleteObject(m_hBmpRecOff);
    vDropAllButons();
}


BEGIN_MESSAGE_MAP(COSD, CStatic)
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BITMAP_SNAPSHOT,  OnSnapshot)
	ON_BN_CLICKED(IDC_BITMAP_MORE,  OnMORE)
	ON_BN_CLICKED(IDC_BITMAP_AUDIO,  OnRecvAudio)
	ON_BN_CLICKED(IDC_BITMAP_FIXRATIO,  OnFixRatio)
	ON_BN_CLICKED(IDC_BITMAP_SEND_AUDIO,  OnSendAudio)
	//ON_BN_CLICKED(IDC_BITMAP_RECVIDEO,  OnRecVideo)
	ON_COMMAND_RANGE(ID_OSDMENU_SPEAKER,ID_OSDMENU_SNAPSHOT,OnClickMenu)
END_MESSAGE_MAP()

// COSD message handlers

int COSD::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_hbr   =   CreateSolidBrush(OSDBKGNDCOLOR); 
	menu.LoadMenu(IDR_MENU_OSD);
	//bGetOSDConfigFromRegistry();
	

	return 0;
}

void COSD::vCreateAndRearrangeControl (OSDTYPE osdType, CString csCameraName)
{
	CRect rectButton;	
	CRect rectParent;

	LONG nBtnWidth = OSDBUTTONWIDTH;
	LONG nBtnHeight = OSDHEIGHT;

	GetClientRect(&rectParent);	

	m_osdType = osdType;
	m_csCameraName = csCameraName;

	// CONSTRUCT AVAILABLE BUTTON CONFIG BY TYPE
	if( (m_vctnOSDBtnConfigAvailable.size() == 1 && _SaveNewOSDItem==0) || _SaveNewOSDItem)
	{		
		switch(osdType)
		{
		case OSDLIVEVIEW:
			m_vctnOSDBtnConfigAvailable.push_back(RECVIDEO);
			if (_SaveNewOSDItem)
			{
				m_vctnOSDBtnConfigAvailable.push_back(CAPTION);
			}
			m_vctnOSDBtnConfigAvailable.push_back(RECVAUDIO);
			m_vctnOSDBtnConfigAvailable.push_back(SENDAUDIO);
			m_vctnOSDBtnConfigAvailable.push_back(FIXRATIO);
			m_vctnOSDBtnConfigAvailable.push_back(OSDSNAPSHOT);
			break;
		case OSDPLAYBACK:
			if (_SaveNewOSDItem)
			{
				m_vctnOSDBtnConfigAvailable.push_back(CAPTION);
			}
			m_vctnOSDBtnConfigAvailable.push_back(OSDSNAPSHOT);
			m_vctnOSDBtnConfigAvailable.push_back(RECVAUDIO);
			m_vctnOSDBtnConfigAvailable.push_back(FIXRATIO);
			break;
		}

		m_vctnOSDBtnConfigAvailable.push_back(MORE); // The more button is always the last 		
	}

	if( CGS::m_vctnOSDBtnConfig.size() == 0)
	{
		CGS::m_vctnOSDBtnConfig = m_vctnOSDBtnConfigAvailable;
	}

	unsigned int nBtnConfigSize =  CGS::m_vctnOSDBtnConfig.size()-1;

	for(int i = 0;i < CGS::m_vctnOSDBtnConfig.size() ;i++)
	{
		rectButton.top = rectParent.top;
		rectButton.bottom = rectParent.top + OSDHEIGHT;

		if(CGS::m_vctnOSDBtnConfig[i] == RECVIDEO)
		{			
			rectButton.left = rectParent.left;
			rectButton.right = rectParent.left+nBtnWidth;
		}
		else if(CGS::m_vctnOSDBtnConfig[i] == CAPTION)
		{			
			rectButton.left = rectParent.left+nBtnWidth;
			rectButton.right = rectParent.right - ((nBtnWidth)* (nBtnConfigSize-i-1)) -MOREWIDTH ;
		}
		else if(CGS::m_vctnOSDBtnConfig[i] == MORE)
		{
			rectButton.left = rectParent.right-MOREWIDTH;
			rectButton.right = rectParent.right;
		}
		else
		{
			rectButton.left = rectButton.right;
			rectButton.right = rectButton.left+nBtnWidth;
		}

		vNewAndRearrangeButton(CGS::m_vctnOSDBtnConfig[i], rectButton, csCameraName);
	}	

}

void COSD::vNewAndRearrangeButton(int nBtnType, CRect &rectButton, CString csCameraName)
{
	DWORD dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;
	COLORREF clBG = GLOBALBKCOLOR;
    int nHoveOver = (int)BTNST_AUTO_GRAY;

	CString csSnapshot;
	CString csSpeaker;
	CString csFix;
	CString csAudio;
	//CString csRecVideo;

	//csRecVideo.LoadStringW(CProductDefine::g_hResStr, IDS_OSD_TOOLTIP_RECVIDEO);
    csSpeaker.LoadStringW(CProductDefine::g_hResStr, IDS_OSD_SPEAKER);			
	csSnapshot.LoadStringW(CProductDefine::g_hResStr, IDS_OSD_TOOLTIP_SNAPSHOT);
	csFix.LoadStringW(CProductDefine::g_hResStr, IDS_OSD_TOOLTIP_FIX_RATIO);
	csAudio.LoadStringW(CProductDefine::g_hResStr, IDS_OSD_TOOLTIP_SEND_AUDIO);

	switch(nBtnType)
	{
	case CAPTION:
		if(!m_pCaptionOSD)
		{
			m_pCaptionOSD = new CStaticCaption();
			m_pCaptionOSD->Create(NULL, WS_CHILD | WS_VISIBLE, rectButton, this, IDC_CAPTIONOSD );
		}
		m_pCaptionOSD->SetWindowText(csCameraName);
		m_pCaptionOSD->SetWindowPos(NULL, rectButton.left, rectButton.top, rectButton.right-rectButton.left, rectButton.bottom-rectButton.top, SWP_SHOWWINDOW);
		break;
	case OSDSNAPSHOT:
		if(!m_pSnapShot)
		{
			m_pSnapShot = new CButtonST();
			m_pSnapShot->Create(NULL, dwStyle, rectButton, this, IDC_BITMAP_SNAPSHOT );
			m_pSnapShot->SetBitmaps(IDB_BITMAP_OSD_SNAPSHOT_H,clBG,IDB_BITMAP_OSD_SNAPSHOT);
			m_pSnapShot->SetTooltipText(csSnapshot);
		}	
		m_pSnapShot->SetWindowPos(NULL, rectButton.left, rectButton.top, rectButton.right-rectButton.left,rectButton.bottom - rectButton.top, SWP_SHOWWINDOW);
		break;
	case FIXRATIO:
		if(!m_pFixRatio)
		{
			m_pFixRatio = new CButtonST();
			m_pFixRatio->Create(NULL,dwStyle, rectButton, this, IDC_BITMAP_FIXRATIO);
			m_pFixRatio->SetBitmaps(IDB_BITMAP_OSD_FIXRATIO_H,clBG,IDB_BITMAP_OSD_FIXRATIO);
			m_pFixRatio->SetTooltipText(csFix);
		}		
		m_pFixRatio->SetWindowPos(NULL, rectButton.left, rectButton.top, rectButton.right-rectButton.left,rectButton.bottom - rectButton.top, SWP_SHOWWINDOW);
		break; 
	case SENDAUDIO:
		if(!m_pSendAudio)
		{
			m_pSendAudio = new CButtonST();
			m_pSendAudio->Create(NULL, dwStyle, rectButton, this, IDC_BITMAP_SEND_AUDIO );
			m_pSendAudio->SetBitmaps(IDB_BITMAP_OSD_MIC_H,clBG,IDB_BITMAP_OSD_MIC);
			m_pSendAudio->SetTooltipText(csAudio);
		}		
		m_pSendAudio->SetWindowPos(NULL, rectButton.left, rectButton.top, rectButton.right-rectButton.left,rectButton.bottom - rectButton.top, SWP_SHOWWINDOW);

		break;
	case RECVAUDIO:
		if(!m_pRecvAudio)
		{
			m_pRecvAudio = new CButtonST();
			m_pRecvAudio->Create(NULL, dwStyle, rectButton, this, IDC_BITMAP_AUDIO );
			m_pRecvAudio->SetBitmaps(IDB_BITMAP_AUDIO_OFF_H,clBG,IDB_BITMAP_AUDIO_OFF);
			m_pRecvAudio->SetTooltipText(csSpeaker);
		}
		m_pRecvAudio->SetWindowPos(NULL, rectButton.left, rectButton.top, rectButton.right-rectButton.left,rectButton.bottom - rectButton.top, SWP_SHOWWINDOW);
		break;
	case MORE:
		if(!m_pMore)
		{
			m_pMore = new CButtonST();
			m_pMore->Create(NULL, dwStyle, rectButton, this, IDC_BITMAP_MORE);
			m_pMore->SetBitmaps(IDB_BITMAP_MORE_H,clBG,IDB_BITMAP_MORE);
		}
		m_pMore->SetWindowPos(NULL, rectButton.left, rectButton.top, rectButton.right-rectButton.left,rectButton.bottom - rectButton.top, SWP_SHOWWINDOW);
		break;
	case RECONNECT:
		if(!m_pReconnect)
		{
			m_pReconnect = new CButtonST();
			m_pReconnect->Create(NULL, dwStyle, rectButton, this, IDC_BITMAP_RECONNECT);
			//m_pReconnect->SetBitmaps(IDB_BITMAP_MORE_H,clBG,IDB_BITMAP_MORE);
		}
		m_pReconnect->SetWindowPos(NULL, rectButton.left, rectButton.top, rectButton.right-rectButton.left,rectButton.bottom - rectButton.top, SWP_SHOWWINDOW);
		break;
	case DOTRIGGER:
		if(!m_pDOTrigger)
		{
			m_pDOTrigger = new CButtonST();
			m_pDOTrigger->Create(NULL, dwStyle, rectButton, this, IDC_BITMAP_DOTRIGGER);
			//m_pReconnect->SetBitmaps(IDB_BITMAP_MORE_H,clBG,IDB_BITMAP_MORE);
		}
		m_pDOTrigger->SetWindowPos(NULL, rectButton.left, rectButton.top, rectButton.right-rectButton.left,rectButton.bottom - rectButton.top, SWP_SHOWWINDOW);
		break;
	case INSTANTPLAYBACK:
		if(!m_pInstantPlayBack)
		{
			m_pInstantPlayBack = new CButtonST();
			m_pInstantPlayBack->Create(NULL, dwStyle, rectButton, this, IDC_BITMAP_INSTANTPLAYBACK);
			//m_pReconnect->SetBitmaps(IDB_BITMAP_MORE_H,clBG,IDB_BITMAP_MORE);
		}
		m_pInstantPlayBack->SetWindowPos(NULL, rectButton.left, rectButton.top, rectButton.right-rectButton.left,rectButton.bottom - rectButton.top, SWP_SHOWWINDOW);
		break;
	case RECVIDEO:
		if(!m_pRecVideo)
		{
			m_pRecVideo = new CStatic();
			m_pRecVideo->Create(NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_CENTERIMAGE, rectButton, this, IDC_BITMAP_RECVIDEO);
			m_hBmpRecOff=LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_BAR_REC_OFF));
			m_hBmpRecOn=LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_BAR_REC_ON));
			m_pRecVideo->SetBitmap(m_hBmpRecOff);
			m_hBmpRecStatus=m_hBmpRecOff;
		}
		m_pRecVideo->SetWindowPos(NULL, rectButton.left, rectButton.top, OSDBUTTONWIDTH,OSDHEIGHT, SWP_SHOWWINDOW);
		break;
	default :
		break;
	}	

	Invalidate(true);
	GetParent()->Invalidate();
}

void COSD::vDropAllButons()
{
    DELETEOBJ(m_pSnapShot);
	DELETEOBJ(m_pCaptionOSD);
	DELETEOBJ(m_pRecvAudio);
    DELETEOBJ(m_pFixRatio);    
	DELETEOBJ(m_pMore);
	DELETEOBJ(m_pSendAudio);	
	DELETEOBJ(m_pRecVideo);
    DELETEOBJ(m_pReconnect);
	DELETEOBJ(m_pDOTrigger);
	DELETEOBJ(m_pInstantPlayBack);
}
void COSD::vResetOSD()
{
    vDropAllButons();
	vCreateAndRearrangeControl(m_osdType,m_csCameraName);		
}

HBRUSH COSD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CStatic::OnCtlColor(pDC, pWnd, nCtlColor);
    //return (HBRUSH)NULL_BRUSH;

	// TODO:  Change any attributes of the DC here
	if(nCtlColor == CTLCOLOR_STATIC) //
	{	
		pDC->SetTextColor(OSDTEXTCOLOR);
		pDC->SetBkColor(OSDBKGNDCOLOR);
	}

	// TODO:  Return a different brush if the default is not desired
	return m_hbr;
}

void COSD::OnSnapshot()
{
	((CVideo*)GetParent())->OnSnapshot();
}
void COSD::OnRecvAudio()
{
	((CVideo*)GetParent())->OnRecvAudio();

}
void COSD::OnSendAudio()
{
	((CVideo*)GetParent())->OnSendAudio();

}
void COSD::OnFixRatio()
{
	((CVideo*)GetParent())->OnSetFixRatio();
}

void COSD::OnRecVideo()
{
	((CVideo*)GetParent())->OnRecVideo();
}

void COSD::OnMORE()
{
	CRect rect;
	GetClientRect(rect);
	
	POINT point;
	GetCursorPos(&point);
    CGS::SetMenuString(menu.m_hMenu, ID_OSDMENU_SPEAKER, IDS_OSD_SPEAKER);
    CGS::SetMenuString(menu.m_hMenu, ID_OSDMENU_MIC, IDS_OSD_TOOLTIP_SEND_AUDIO);
    CGS::SetMenuString(menu.m_hMenu, ID_OSDMENU_FIXRATIO, IDS_SET_FIX_RATIO);
    CGS::SetMenuString(menu.m_hMenu, ID_OSDMENU_SNAPSHOT, IDS_SNAPSHOT);
	//CGS::SetMenuString(menu.m_hMenu, ID_OSDMENU_REC,IDS_OSD_TOOLTIP_RECVIDEO);

	// prepare available menu list
	for(int i = 0;i < m_vctnOSDBtnConfigAvailable.size() ;i++)
	{
			switch(m_vctnOSDBtnConfigAvailable[i] )
			{
				case OSDSNAPSHOT:
					menu.EnableMenuItem(ID_OSDMENU_SNAPSHOT, MF_ENABLED);					
					break;
				case FIXRATIO:
					menu.EnableMenuItem(ID_OSDMENU_FIXRATIO, MF_ENABLED);					
					break; 
				case SENDAUDIO:
					menu.EnableMenuItem(ID_OSDMENU_MIC, MF_ENABLED);					
						break;
				case RECVAUDIO:
					menu.EnableMenuItem(ID_OSDMENU_SPEAKER, MF_ENABLED);					
						break;
				//case RECVIDEO:
				//	menu.EnableMenuItem(ID_OSDMENU_REC,MF_ENABLED);
				//	break;
				default :
						break;
			}
	}
	
	// set current displayed button checked state

	for(int i = 0;i < CGS::m_vctnOSDBtnConfig.size() ;i++)
	{
			switch(CGS::m_vctnOSDBtnConfig[i] )
			{
				case OSDSNAPSHOT:					
					menu.CheckMenuItem(ID_OSDMENU_SNAPSHOT,  MF_CHECKED );
					break;
				case FIXRATIO:					
					menu.CheckMenuItem(ID_OSDMENU_FIXRATIO,  MF_CHECKED );
						break; 
				case SENDAUDIO:					
					menu.CheckMenuItem(ID_OSDMENU_MIC,  MF_CHECKED );
						break;
				case RECVAUDIO:					
					menu.CheckMenuItem(ID_OSDMENU_SPEAKER,  MF_CHECKED );
						break;
				//case RECVIDEO:
				//	menu.CheckMenuItem(ID_OSDMENU_REC, MF_CHECKED);
				//	break;
				default :
						break;
			}
	}	
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}


void COSD::OnClickMenu(UINT nID)
{
	
	int nDeleteItem(0), nAddItem(0);
	UINT nState(0);
	
	switch(nID)
	{
	case ID_OSDMENU_FIXRATIO:
		nState  = menu.GetMenuState(ID_OSDMENU_FIXRATIO,MF_BYCOMMAND);
		if(nState == MF_CHECKED)
		{ 
			nDeleteItem = FIXRATIO;
			menu.CheckMenuItem(ID_OSDMENU_FIXRATIO,  MF_UNCHECKED );
		}
		else
		{
			nAddItem = FIXRATIO;
			menu.CheckMenuItem(ID_OSDMENU_FIXRATIO,  MF_CHECKED );
		}	
		break;
	case ID_OSDMENU_SNAPSHOT:
		nState  = menu.GetMenuState(ID_OSDMENU_SNAPSHOT,MF_BYCOMMAND);
		if(nState == MF_CHECKED )
		{ 
			nDeleteItem = OSDSNAPSHOT ;
			menu.CheckMenuItem(ID_OSDMENU_SNAPSHOT,  MF_UNCHECKED );	
		}
		else
		{
			nAddItem = OSDSNAPSHOT;
			menu.CheckMenuItem(ID_OSDMENU_SNAPSHOT,  MF_CHECKED ) ;	
		}			
		break;
	case ID_OSDMENU_MIC:
		nState  = menu.GetMenuState(ID_OSDMENU_MIC,MF_BYCOMMAND);
		if(nState == MF_CHECKED)
		{
			nDeleteItem = SENDAUDIO ; 
			menu.CheckMenuItem(ID_OSDMENU_MIC,  MF_UNCHECKED ) ;
		}
		else
		{
			 nAddItem = SENDAUDIO;	
			 menu.CheckMenuItem(ID_OSDMENU_MIC,  MF_CHECKED ) ;	
		}
		break;
	case ID_OSDMENU_SPEAKER:		
		 nState  = menu.GetMenuState(ID_OSDMENU_SPEAKER , MF_BYCOMMAND);
		if(nState == MF_CHECKED)
		{
			nDeleteItem = RECVAUDIO ;
			menu.CheckMenuItem(ID_OSDMENU_SPEAKER,  MF_UNCHECKED ) ;	
		}
		else
		{
			nAddItem = RECVAUDIO;
		    menu.CheckMenuItem(ID_OSDMENU_SPEAKER,  MF_CHECKED ) ;	
		}
		break;
	case ID_OSDMENU_REC:
		/*nState  = menu.GetMenuState(ID_OSDMENU_REC,MF_BYCOMMAND);
		if(nState == MF_CHECKED)
		{
			nDeleteItem = SENDAUDIO ; 
			menu.CheckMenuItem(ID_OSDMENU_REC,  MF_UNCHECKED ) ;
		}
		else
		{
			nAddItem = SENDAUDIO;	
			menu.CheckMenuItem(ID_OSDMENU_REC,  MF_CHECKED ) ;	
		}
		break;*/
	default: break;

	}
	
	// delete btn from cofig
	if(nDeleteItem !=0)
	{
		for(int i = 0;i< CGS::m_vctnOSDBtnConfig.size() ;i++)
		{
			if(CGS::m_vctnOSDBtnConfig[i] == nDeleteItem)
			{
				CGS::m_vctnOSDBtnConfig.erase(CGS::m_vctnOSDBtnConfig.begin()+i);
			}
		}
	}
	// add btn to cofig
	if(nAddItem !=0)
	{	
		std::vector<int>::iterator it;
		it = CGS::m_vctnOSDBtnConfig.begin();
		CGS::m_vctnOSDBtnConfig.insert(it+2,nAddItem);
	}			

	// Save and Reset osd
	((CMainFrame*)AfxGetMainWnd())->vSaveAndResetOSD();
}



