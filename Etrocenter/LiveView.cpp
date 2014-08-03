// LiveView.cpp : implementation file
//

#include "stdafx.h"
#include "LiveView.h"
#include "math.h"
#include "MainFrm.h"
#include "IntraEvent.h"

#define IDC_SCREEN                      10009 
#define FULLSCREENPOS       5
#define STARTTIMERAFTERSECOND 2
#define ID_AUTOPLAY_TIMER     WM_USER+100001
#define ID_FPS_TIMER     WM_USER+100002
#define ID_EVENT_TIMER  WM_USER+100003
#define ID_GROUP_AUTOPLAY_TIMER  WM_USER+100004
//#define IDC_STATIC_GROUP_NAME	WM_USER+100005
#define ID_GROUP_AUTOPLAY_PAUSE_TIMER  WM_USER+100005
#define EVENTPLAYTIME 1 // event liveview time 1 secs
#define AUTOSCAN_PAUSE_TIMER 5

// CLiveView

IMPLEMENT_DYNAMIC(CLiveView, CStatic)

CLiveView::CLiveView():m_bDragging(false),m_bDisplayOSD(true),m_bOnFlyAway(false),m_nLayout(0),m_bSwap(false)
,m_nCamIndex(0),m_bAutoPlay(false),m_bOneMatrixView(false),m_nOneRectIdx(0),m_nEventRectIdx(0), m_nOriginalRectIdx(0),m_nEventCounter(0)
,m_hThreadEventRec(0),m_bEventEnable(false),m_nAutoPlayTimer(0),m_nEventTimer(0),m_nFPSTimer(0),m_nCurrentPlayGroupId(0),m_bGroupAutoPlay(false)
,m_nGroupAutoPlayTimer(0),m_bGetGroupIdx(true),m_nPreviousGroupNode(-1),m_bPlaySameGroup(false),m_pDlgPause(NULL)
,m_hThreadAutoScanWait(NULL),m_hPauseAutoScan(NULL), m_hResumeAutoScan(NULL)
,m_nAutoScanTime(0),m_bAutoScanPause(false),m_bSetReg(true)
{
	//,m_bOutOfRange(false)
	m_bOutOfRangeIPWall = false;
}

CLiveView::~CLiveView()
{
	if(m_EventRec)
		delete m_EventRec ;
	DELETEOBJ(m_pDlgPause);
}

BEGIN_MESSAGE_MAP(CLiveView, CStatic)
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	//ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(ID_SET_OSD,vSetOSD)
	ON_MESSAGE(ID_CHANGE_SEND_AUDIO_BMP,vChangeSendAudioBMP)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_CREATE()
    ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(MsgUpdateView, OnUpdateView)
END_MESSAGE_MAP()
// CLiveView message handlers

int CLiveView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pbk = CNVRBk::GetBkObject();
	m_EventRec = new CEventReceive();
	UINT nRetEvent(0);
	m_hThreadEventRec = (HANDLE)_beginthreadex(NULL, 0, thdGetEventFromService, (LPVOID)this, 0, &nRetEvent);

	return 0;
}

void CLiveView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC* pDC = &dc;
	CPen Pen( PS_SOLID, BORDERWIDTH, BORDERCOLOR );
    CPen PenFocus ( PS_SOLID, BORDERWIDTH, FOCUSCOLOR );
	CPen PenEvent ( PS_SOLID, BORDERWIDTH, EVENTCOLOR);
	
	if (m_videos.size() > 0 && 	false == m_bOneMatrixView)
	{
        
		for (int ci=0; ci < m_rects.size(); ci++)
		{
            if (m_bEventEnable && m_nEventRectIdx == ci) continue;
            if (nFocusVideo == ci) continue;
			CRect rect = m_rects[ci];
			CPen* pPen = &Pen;;
			//pPen = (nFocusVideo == ci) ? &PenFocus : &Pen;
			CPen *pOldPen =	pDC->SelectObject( pPen );
			rect.DeflateRect(-BORDERWIDTH,-BORDERWIDTH);
			pDC->MoveTo(rect.TopLeft());
			pDC->LineTo(rect.right, rect.top);
			pDC->LineTo(rect.BottomRight());
			pDC->LineTo(rect.left, rect.bottom);
			pDC->LineTo(rect.TopLeft());
			pDC->SelectObject( &pOldPen );		
		}//end for

		//set focus pen
		{
			CRect rect = m_rects[nFocusVideo];
			CPen* pPen;
			pPen =  &PenFocus;
			CPen *pOldPen =	pDC->SelectObject( pPen );
			rect.DeflateRect(-BORDERWIDTH,-BORDERWIDTH);
			pDC->MoveTo(rect.TopLeft());
			pDC->LineTo(rect.right, rect.top);
			pDC->LineTo(rect.BottomRight());
			pDC->LineTo(rect.left, rect.bottom);
			pDC->LineTo(rect.TopLeft());
			pDC->SelectObject( &pOldPen );
		}
		// set event pen  	
		if(m_bEventEnable && m_nEventRectIdx < m_rects.size())
		{
			CRect rect = m_rects[m_nEventRectIdx];
			CPen* pPen;
			pPen =  &PenEvent;
			CPen *pOldPen =	pDC->SelectObject( pPen );
			rect.DeflateRect(-BORDERWIDTH,-BORDERWIDTH);
			pDC->MoveTo(rect.TopLeft());
			pDC->LineTo(rect.right, rect.top);
			pDC->LineTo(rect.BottomRight());
			pDC->LineTo(rect.left, rect.bottom);
			pDC->LineTo(rect.TopLeft());
			pDC->SelectObject( &pOldPen );
		}   
	}

}

void CLiveView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CPoint pt(point);
	ScreenToClient(&pt);
	int ci=0;

	if(!m_bOneMatrixView)
	{
		OnLButtonDown(0,point);
		
		for (ci=0; ci < m_rects.size(); ci++)
		{
			CRect rc = m_rects[ci];
			if (rc.PtInRect(pt))
				break;
		}
	}
	else
	{
		CRect rect;
		GetClientRect(rect);
		if(rect.PtInRect(pt))
		{
			ci = m_nOneRectIdx;				
		}

	}

	if (m_videos.size() > ci)
	{
		CMenu menu;
		menu.LoadMenu(IDR_MENU_VIDEO_SET);
        CGS::SetMenuString(menu.m_hMenu, IDC_SET_VIDEOSET_FLIP, IDS_SET_VIDEOSET_FLIP);
        CGS::SetMenuString(menu.m_hMenu, IDC_SET_VIDEOSET_UPDISEDOWN, IDS_SET_VIDEOSET_UPDISEDOWN);
        CGS::SetMenuString(menu.m_hMenu, IDC_SET_FIX_RATIO, IDS_SET_FIX_RATIO);
        CGS::SetMenuString(menu.m_hMenu, ID_MENU_SNAPSHOT, IDS_SNAPSHOT);
        CGS::SetMenuString(menu.m_hMenu, ID_MENU_EXITFULLSCREEN, IDS_EXITFULLSCREEN);
        CGS::SetMenuString(menu.m_hMenu, IDM_MENU_OSD, IDS_OSD);

		//menu.EnableMenuItem(ID_MENU_EXITFULLSCREEN,MF_DISABLED);
		menu.CheckMenuItem(IDC_SET_VIDEOSET_FLIP, m_videos[ci]->bGetFlip() ? MF_CHECKED : MF_UNCHECKED );
		menu.CheckMenuItem(IDC_SET_VIDEOSET_UPDISEDOWN, m_videos[ci]->bGetUpsideDown() ? MF_CHECKED : MF_UNCHECKED );
		menu.CheckMenuItem(IDC_SET_FIX_RATIO, m_videos[ci]->bGetAspectRation() ? MF_CHECKED : MF_UNCHECKED );
		//menu.CheckMenuItem(ID_PTZ_FREELANCE, m_videos[ci]->bGetPtzFreelance() ? MF_CHECKED : MF_UNCHECKED );
	/*	if(m_bOneMatrixView)
		{
			menu.DeleteMenu(ID_MENU_EXITFULLSCREEN,MF_BYCOMMAND);
		}
		else*/
		{
			menu.CheckMenuItem(ID_MENU_EXITFULLSCREEN, m_pbk->GetMainFrame()->vIsFullScreen() ? MF_CHECKED : MF_UNCHECKED );
		}
		menu.CheckMenuItem(IDM_MENU_OSD,  m_videos[ci]->bGetOnOSD() ? MF_CHECKED : MF_UNCHECKED );
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, m_videos[ci]);
		
	}else
	{
		
		return;
		
	}
}

void CLiveView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bOneMatrixView) return;

	CPoint pt(point);
	WORD wProtocol;
	NODEITEM *pNode;
    m_bSwap = true;

	ClientToScreen(&pt);
	
	if(m_videos.size() ==0 ) return;

	int ci;
	for (ci=0; ci < m_rects.size(); ci++)
	{
		CRect rc = m_rects[ci];

		ClientToScreen(rc);
		if (rc.PtInRect(pt))
		{
			//g_nFocusRect=ci;
			break;
		}
	}
	// not drawing on Event Video Rect
	if(m_bEventEnable)
	{
		if(ci == m_nEventRectIdx || ci > m_videos.size()-1 || m_videos[ci] == NULL)
		{
			return;
		}		
	}

	if (m_videos.size() > ci)
	{
		DrawVideoFrame(nFocusVideo, ci);
	
	//if(ci > (m_videos.size()-1) ) return;
	// set focus for PTZ
	//	((CIPCamTreeView*)((CMainFrame*)AfxGetMainWnd())->GetTreeView())->GetTreeCtrl()->SelectItem(m_videos[ci]->GetNodeItem()->hTreeItem);
		pNode = m_videos[ci]->GetNodeItem();
		wProtocol = pNode->nProtocol;

	// new video stream service using CGI
		m_pbk->GetMainFrame()->vSetFocusVideoSet(m_videos[ci]->GetNodeItem(),m_videos[ci]->vGetVideoMsgId(), m_videos[ci]);

	}
	
}

void CLiveView::DrawVideoFrame(int oldFocus, int newFocus)
{
	CClientDC dc(this);

	if(newFocus > m_rects.size()-1) 
		newFocus = 0;

	nFocusVideo = newFocus;

	CRect rect = m_rects[oldFocus];
	CPen Pen( PS_SOLID, BORDERWIDTH, BORDERCOLOR );
	CPen PenFocus ( PS_SOLID, BORDERWIDTH, FOCUSCOLOR );
	CPen *pOldPen =	dc.SelectObject( &Pen );
	rect.DeflateRect(-BORDERWIDTH, -BORDERWIDTH);

	//clear//
	dc.MoveTo(rect.TopLeft());
	dc.LineTo(rect.right, rect.top);
	dc.LineTo(rect.BottomRight());
	dc.LineTo(rect.left, rect.bottom);
	dc.LineTo(rect.TopLeft());
	dc.SelectObject( &PenFocus );

	rect = m_rects[newFocus];
	rect.DeflateRect(-BORDERWIDTH, -BORDERWIDTH);
	dc.MoveTo(rect.TopLeft());
	dc.LineTo(rect.right, rect.top);
	dc.LineTo(rect.BottomRight());
	dc.LineTo(rect.left, rect.bottom);
	dc.LineTo(rect.TopLeft());
	dc.SelectObject(&pOldPen);

	if(m_bEventEnable) // redraw event cell
	{
		CRect rect = m_rects[m_nEventRectIdx];
		CPen PenEvent ( PS_SOLID, BORDERWIDTH, EVENTCOLOR);
		CPen *pOldPen =	dc.SelectObject( &PenEvent);
		rect.DeflateRect(-BORDERWIDTH,-BORDERWIDTH);
		dc.MoveTo(rect.TopLeft());
		dc.LineTo(rect.right, rect.top);
		dc.LineTo(rect.BottomRight());
		dc.LineTo(rect.left, rect.bottom);
		dc.LineTo(rect.TopLeft());
		dc.SelectObject( &pOldPen );
		PenEvent.DeleteObject();
	}
	Pen.DeleteObject();
	PenFocus.DeleteObject();
}

void CLiveView::OnSize(UINT nType, int cx, int cy)
{
	if(this == NULL) return;
	
	if(m_bOneMatrixView)
	{
		CRect rect;
		CRect rectVoid(0,0,0,0);
		GetParent()->GetClientRect(rect);
		
		for(int i = 0 ; i <m_videos.size() && i <m_rects.size();i++)
		{
			if(i != m_nOneRectIdx)
			{
				m_videos[i]->vSetStatus(true);
				m_videos[i]->MoveWindow(rectVoid);
			}
		}
		SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW);
		m_videos[m_nOneRectIdx]->MoveWindow(rect);
		::SendMessage(m_videos[m_nOneRectIdx]->m_hWnd, WM_ERASEBKGND, NULL,NULL); 
		return;
	}

	vSetGrids(m_nLayout);
	CRect rc;
	GetParent()->GetClientRect(&rc);
	SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_SHOWWINDOW);

	for (int ci=0; ci < m_videos.size(); ci++)
	{
		if (ci>= m_rects.size()) break;
		if (m_videos[ci])
		{
			m_videos[ci]->MoveWindow(&m_rects[ci]);
			//if(m_videos[ci]->m_bDisplayOSD == true) m_videos[ci]->vRearrangeOSD();
		}
	}	
}

BOOL CLiveView::OnEraseBkgnd(CDC* pDC)
{
	pDC->SetBkColor(GLOBALFONTCOLOR);//BKCOLOR
	CRect rect;
	GetClientRect(rect);
	CBrush bkColor(GLOBALFONTCOLOR);
	pDC->FillRect(&rect,&bkColor);
	
	return CStatic::OnEraseBkgnd(pDC);
}
void CLiveView::vSetGrids(int nGrid)
{
	int i_GridNo=0;

	if (nGrid==22 || nGrid == 12) nGrid=2;

	/*m_grids = nGrid * nGrid;*/

	if (m_rects.size() >0)	m_rects.clear();
	
	RECT rc;
	int ci =0;
	
	CSize sizeTotal;
	
	GetParent()->GetClientRect(&rc);
	
 	sizeTotal.cx = rc.right - rc.left ;
	sizeTotal.cy = rc.bottom - rc.top;

	DWORD nGapx, nGapy, nGapxBig, nGapyBig;
	switch (nGrid)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	{	
			m_nLayout = nGrid; // for OnSize adjustment	
			m_grids = nGrid * nGrid;
			nGapx = sizeTotal.cx / nGrid;
			nGapy = sizeTotal.cy / nGrid;
			for (int cj=0; cj <(nGrid); cj++) {
				for (int ci=0; ci <(nGrid); ci++) {
					RECT rcTmp={nGapx * ci, nGapy * cj, nGapx * (ci +1), nGapy * (cj+1)};
					((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
					m_rects.push_back(rcTmp);
				}
			}
			break;
		}
	case 15://1 big + 5 small
	case 115: // event
		{
			//RECT rcTmp={0,0,0,0};
			m_nLayout = 15; // for OnSize adjustment	
			m_grids = 6;
			nGapx = sizeTotal.cx / 3;
			nGapy = sizeTotal.cy / 3;
			nGapxBig = nGapx * 2;
			nGapyBig = nGapy * 2;

			for (int cj=0; cj < 3; cj++) {
				for (int ci=0; ci < 3; ci++) {
					if (ci==0 && cj==0) {					
						RECT rcTmp={nGapx * ci, nGapyBig * cj, nGapxBig * (ci +1) , nGapyBig * (cj+1) };
						((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
						m_rects.push_back(rcTmp);
					}
					else if((ci==1 && cj==0) || (ci==0 && cj==1) || (ci==1 && cj==1))
						continue;
					else
					{				 
						RECT rcTmp={nGapx * ci, nGapy * cj, nGapx * (ci +1), nGapy * (cj+1)};
						((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
						m_rects.push_back(rcTmp);
					}					
				}
			}
		}
		break;
		case 17: //1 big + 7 small
		{	
			m_nLayout = 17; // for OnSize adjustment	
			m_grids = 8;
			nGapx = sizeTotal.cx / 4;
			nGapy = sizeTotal.cy / 4;
			nGapxBig = nGapx * 3;
			nGapyBig = nGapy * 3;

			for (int cj=0; cj < 4; cj++) {
				for (int ci=0; ci < 4; ci++) {
					if (ci==0 && cj==0) {					
						RECT rcTmp={nGapx * ci, nGapyBig * cj, nGapxBig * (ci +1) , nGapyBig * (cj+1) };
						//((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
						m_rects.push_back(rcTmp);
					}
					else if((ci==1 && cj==0) || (ci==0 && cj==1) || (ci==1 && cj==1) || (ci==0 && cj==2) || (ci==2 && cj==0) ||
						(ci==1 && cj==2) || (ci==2 && cj==1) || (ci==2 && cj==2))
						continue;
					else
					{				 
						RECT rcTmp={nGapx * ci, nGapy * cj, nGapx * (ci +1), nGapy * (cj+1)};
						((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
						m_rects.push_back(rcTmp);
					}					
				}
			}
		}
		break;
		case 28: //2 big + 8 small
		case 128: // event
		{
			m_nLayout = 28; // for OnSize adjustment	
			m_grids = 8;
			nGapx = sizeTotal.cx / 4;
			nGapy = sizeTotal.cy / 4;
			nGapxBig = nGapx * 2;
			nGapyBig = nGapy * 2;

			//Loop into each cells and resize them, i-x; j-y
			for (int cj=0; cj < 4; cj++) 
			{
				for (int ci=0; ci < 4; ci++) 
				{
					if (ci==0 && cj==0) {//Cell 0x0	
						RECT rcTmp={nGapx * ci, nGapyBig * cj, nGapxBig * (ci+1) , nGapyBig * (cj+1) };
						((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
						m_rects.push_back(rcTmp);
					}
					else if  (ci==2 && cj==0) { 	
							RECT rcTmp={nGapx * ci, nGapyBig * cj, nGapxBig * (ci) , nGapyBig * (cj+1) };
						((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
						m_rects.push_back(rcTmp);
					}
					else if((ci==1 && cj==0) || (ci==0 && cj==1) || (ci==1 && cj==1) || (ci==3 && cj==0) || 
						(ci==3 && cj==1) || (ci==2 && cj==1) )
						continue;
					else
					{				 
						RECT rcTmp={nGapx * ci, nGapy * cj, nGapx * (ci +1), nGapy * (cj+1)};
						((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
						m_rects.push_back(rcTmp);
					}					
				}
			}//for (int cj=0; cj < 4; cj++) 
		}
		break;

		case 121: //1 big + 12 small - Big one on the side
		{
			m_nLayout = 121; // for OnSize adjustment	
			m_grids = 13;
			nGapx = sizeTotal.cx / 4;
			nGapy = sizeTotal.cy / 4;
			nGapxBig = nGapx * 2;
			nGapyBig = nGapy * 2;

			//Loop into each cells and resize them, i-x; j-y
			for (int cj=0; cj < 4; cj++) 
			{
				for (int ci=0; ci < 4; ci++) 
				{
					if ( ci==0 && cj==0 ) { //Big Cell  0x0					
						RECT rcTmp={nGapx * ci, nGapyBig * cj, nGapxBig * (ci +1) , nGapyBig * (cj+1) };
						((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
						m_rects.push_back(rcTmp);
					}
					else if((ci==1 && cj==0) || (ci==0 && cj==1) || (ci==1 && cj==1) ) //Skip these cells
						continue;
					else
					{				 
						RECT rcTmp={nGapx * ci, nGapy * cj, nGapx * (ci +1), nGapy * (cj+1)};
						((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
						m_rects.push_back(rcTmp);
					}					
				}
			}//for (int cj=0; cj < 4; cj++) 
		}
		break;

		case 122: //1 big + 12 small - Big one on the middle
		case 1122: // event
		{	
			m_nLayout = 122; // for OnSize adjustment	
			m_grids = 13;
			nGapx = sizeTotal.cx / 4;
			nGapy = sizeTotal.cy / 4;
			nGapxBig = sizeTotal.cx / 4;
			nGapyBig = sizeTotal.cy / 4;

			//Loop into each cells and resize them, i-x; j-y
			for (int cj=0; cj < 4; cj++) 
			{
				for (int ci=0; ci < 4; ci++) 
				{
					if ( ci==1 && cj==1 ) { //Big Cell  0x0					
						RECT rcTmp={nGapx * ci, nGapyBig * cj, nGapxBig * (ci +2) , nGapyBig * (cj+2) };
						((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
						m_rects.push_back(rcTmp);
					}
					else if((ci==2 && cj==1) || (ci==1 && cj==2) || (ci==2 && cj==2) ) //Skip these cells
						continue;
					else
					{				 
						RECT rcTmp={nGapx * ci, nGapy * cj, nGapx * (ci +1), nGapy * (cj+1)};
						((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
						m_rects.push_back(rcTmp);
					}					
				}
			}//for (int cj=0; cj < 4; cj++) 
		}
		break;

		case 116: //1 big + 16 small - Big one on the middle
		case 1116://event
		{
			m_nLayout = 116; // for OnSize adjustment	
			m_grids = 17;
			nGapx = sizeTotal.cx / 5;
			nGapy = sizeTotal.cy / 5;
			nGapxBig = sizeTotal.cx / 5;
			nGapyBig = sizeTotal.cy / 5;

			//Loop into each cells and resize them, i-x; j-y
			for (int cj=0; cj < 5; cj++) 
			{
				for (int ci=0; ci < 5; ci++) 
				{
					if ( ci==1 && cj==1 ) { //Big Cell  0x0					
						RECT rcTmp={nGapx * ci, nGapyBig * cj, nGapxBig * (ci +3) , nGapyBig * (cj+3) };
						((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
						m_rects.push_back(rcTmp);
					}
					else if((ci==2 && cj==1) || (ci==1 && cj==2) || (ci==2 && cj==2) || (ci==3 && cj==2) || 
						(ci==1 && cj==3) || (ci==3 && cj==1) || (ci==2 && cj==3) || (ci==3 && cj==3)  ) //Skip these cells
						continue;
					else
					{				 
						RECT rcTmp={nGapx * ci, nGapy * cj, nGapx * (ci +1), nGapy * (cj+1)};
						((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
						m_rects.push_back(rcTmp);
					}					
				}
			}//for (int cj=0; cj < 4; cj++) 
		}
		break;
			case 218:
			{
					m_nLayout = 218; // for OnSize adjustment	
					m_grids = 20;
					nGapx = sizeTotal.cx / 6;
					nGapy = sizeTotal.cy / 6;
					nGapxBig = nGapx *3;
					nGapyBig = nGapy *3;			

					//Loop into each cells and resize them, i-x; j-y
					for (int cj=0; cj < 6; cj++) 
					{
						for (int ci=0; ci < 6; ci++) 
						{
							if (ci==0 && cj==0) {//Cell 0x0	,first cell big
								RECT rcTmp={nGapx * ci, nGapyBig * cj, nGapxBig * (ci+1) , nGapyBig * (cj+1) };
								((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
								m_rects.push_back(rcTmp);
							}
							else if  (ci== 3 && cj==0) { 	//second big cell
									RECT rcTmp={nGapx * ci, nGapyBig * cj, nGapxBig* 2  , nGapyBig * (cj+1)  };
								((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
								m_rects.push_back(rcTmp);
							}
							else if((ci==1 && cj==0) || (ci==2 && cj==0)|| (ci==4 && cj==0) || (ci==5 && cj==0) || 
								(ci==0 && cj==1) || (ci==1 && cj==1) || (ci==2 && cj==1)|| (ci==3 && cj==1)|| (ci==4 && cj==1)||(ci==5 && cj==1)||
								(ci==0 && cj==2) || (ci==1 && cj==2) || (ci==2 && cj==2)|| (ci==3 && cj==2)|| (ci==4 && cj==2)||(ci==5 && cj==2)
								)// no drawing cells
								continue;
							else
							{			//18 small cells	 
								RECT rcTmp={nGapx * ci, nGapy * cj, nGapx * (ci +1), nGapy * (cj+1)};
								((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
								m_rects.push_back(rcTmp);
							}					
						}
					}//for loop

			}
			break;
		case 232:
			{
					m_nLayout = 232; // for OnSize adjustment	
					m_grids = 34;
					nGapx = sizeTotal.cx / 8;
					nGapy = sizeTotal.cy / 8;
					nGapxBig = nGapx * 4;
					nGapyBig = nGapy * 4;

					//Loop into each cells and resize them, i-x; j-y
					for (int cj=0; cj < 8; cj++) 
					{
						for (int ci=0; ci < 8; ci++) 
						{
							if (ci==0 && cj==0) {//Cell 0x0	
								RECT rcTmp={nGapx * ci, nGapyBig * cj, nGapxBig * (ci+1) , nGapyBig * (cj+1) };
								((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
								m_rects.push_back(rcTmp);
							}
							else if  (ci==4 && cj==0) { 	
									RECT rcTmp={nGapx * ci, nGapyBig * cj, nGapxBig * 2 , nGapyBig * (cj+1) };
								((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
								m_rects.push_back(rcTmp);
							}
							else if((ci==1 && cj==0) || (ci==2 && cj==0)|| (ci==3 && cj==0) || (ci==5 && cj==0) || (ci==6 && cj==0)||(ci==7 && cj==0)||
								(ci==0 && cj==1) || (ci==1 && cj==1) || (ci==2 && cj==1)|| (ci==3 && cj==1)|| (ci==4 && cj==1)||(ci==5 && cj==1)||(ci==6 && cj==1)||(ci==7 && cj==1)||
								(ci==0 && cj==2) || (ci==1 && cj==2) || (ci==2 && cj==2)|| (ci==3 && cj==2)|| (ci==4 && cj==2)||(ci==5 && cj==2)||(ci==6 && cj==2)||(ci==7 && cj==2)||
								(ci==0 && cj==3) || (ci==1 && cj==3) || (ci==2 && cj==3)|| (ci==3 && cj==3)|| (ci==4 && cj==3)||(ci==5 && cj==3)||(ci==6 && cj==3)||(ci==7 && cj==3)
								)// no drawing cells
								continue;
							else
							{				 
								RECT rcTmp={nGapx * ci, nGapy * cj, nGapx * (ci +1), nGapy * (cj+1)};
								((CRect*)&rcTmp)->DeflateRect(DEFLATEWIDTH, DEFLATEWIDTH);
								m_rects.push_back(rcTmp);
							}					
						}
					}//for (int cj=0; cj < 4; cj++) 
			}
			break;

	default: return;
	}
	
	Invalidate(true);

	if (nFocusVideo >= m_videos.size()) 
	{
		nFocusVideo =0;
	}
}
void  CLiveView::vAttach(vector <NODEITEM*> pNode)
{
	if (m_rects.size()==0) return;
	
	double dbDispatch = (double)pNode.size() / (double)m_rects.size();
	m_bOneMatrixView = false;	
	m_bEventEnable = m_pbk->GetMainFrame()->m_bEventLV;
	if (m_bOnFlyAway)	//fix the IP-Wall don't run  Event tab//
	{
		m_bEventEnable=false;
	}

	if(!m_bDragging)
	{
		// clear some data
		//vFreeVideoSet();
		m_vctpNode.clear();

		if (!m_bOnFlyAway)
			*m_bOutOfRange = false;
		else
			m_bOutOfRangeIPWall = false;

		m_vctpGroupAutoScanNode.clear();
		m_vctnGroupAutoPlayIdx.clear();
		KillTimer(m_nAutoPlayTimer);
		KillTimer(m_nEventTimer);
		KillTimer(m_nGroupAutoPlayTimer);
		KillTimer(m_nGroupAutoPauseTimer);
		while(m_nPlayIdxQueue.size() !=0)
		{
			m_nPlayIdxQueue.pop();	
		}	
		m_pbk->GetMainFrame()->SetWindowTextW( CProductDefine::m_tzProductName);		

		// Group Autoplay
		if(m_pbk->GetMainFrame()->m_bGroupAutoPlay && m_pbk->GetMainFrame()->m_bAutoScan)
		{
			vGroupAutoScan();
		}

		else// autoplay and event/ live view
		{
			// clear some group autoplay parameters
			m_bGetGroupIdx = true;
			m_bPlaySameGroup = false;
			m_nPreviousGroupNode = -1;
			m_pbk->GetMainFrame()->vSetGroupAutoScan(false); // set off
			if(!m_bAutoScanPause)
			DELETEOBJ(m_pDlgPause);

			// Set autoplay enable or not 
			bool bAutoPlay = false;
			if(dbDispatch <= 1.0)
			{
				bAutoPlay = false;// more matrix than videos, don't have to autoplay
				m_pbk->GetMainFrame()->m_bAutoScan = false;
			}
			else
			{
				if(!m_bOnFlyAway)
				{
					bAutoPlay = m_pbk->GetMainFrame()->m_bAutoScan; // if not a flyaway dlg , use mainfrm m_bAutoScan setting
				}
				else
				{
					bAutoPlay = m_bAutoPlay; // flyaway dlg, use LiveView m_bAutoPlay setting 
				}
			}

			if(!m_bEventEnable) // Event liveView and autoscan are disjoint
			{ 
				if(!bAutoPlay) // normal liveview
				{			
					for(int i=0;i<pNode.size();i++)
					{					
						if(i > (int)(m_rects.size()-1)) // out of view range
						{
							if (!m_bOnFlyAway)
								*m_bOutOfRange = true;
							else
								m_bOutOfRangeIPWall = true;
							break;
						}				
						vNewVideoSet(pNode[i]);
	       				m_vctpNode.push_back(pNode[i]);									
					}
				}

				if(bAutoPlay) // autoscan
				{
					if (m_pbk->GetMainFrame()->m_bIPWallSelected == FALSE)
						m_pbk->SendMessage(MsgUpdateView, UM_SET_LIVEVIEW_AUTOTYPE, bAutoPlay);

					m_nAutoPlayTimer = SetTimer(ID_AUTOPLAY_TIMER, (m_pbk->GetMainFrame()->m_nAutoScanTime*1000) ,NULL);
					if (_MaskBeta)
						m_pbk->GetMainFrame()->vStartDisplayAutoPlay();

					vector <NODEITEM*> vctNodeTest;
		
					for(int i=0;i<m_rects.size();i++)
					{	
						vctNodeTest = vctDispatchNode(pNode,i);					

						if(vctNodeTest.size()==1)
						{
							vNewVideoSet(vctNodeTest[0]);
						}
						else
						{				
							vNewVideoSet(vctDispatchNode(pNode,i));								
						}
						vctNodeTest.clear();		       		
					}						
				}
			}
			else// event live view
			{			
				m_vctpCurrentGroupNodes.clear();
				vDispatchEventVideo(pNode);										
			}

			/////Larry add 102-11-20 Set autoplay checkbox enable ////
			g_bAutoPlayCheckBox=bAutoPlay;
			////////////////////////////////////////////////////////////////
		}

		/*if(i<m_rects.size()-1)
		{
			DWORD dwStyle = WS_VISIBLE|WS_BORDER;
			m_mediaCtrl.Create(NULL,dwStyle,m_rects[i],this,IDC_MEDIACTRL1,0,0,0);
			m_mediaCtrl.PlayStartMethod(_T("10.1.21.185"),9877,_T("root"),_T("pass"),1);
			m_mediaCtrl.PlayControlMethod(1,1,1);
			m_mediaCtrl.SetOSDMethod(7,_T("fuxker"),1,0,0,0,_T("Arial"),255,0,0,0,0,0,0);
			m_mediaCtrl.MoveWindow(&m_rects[i]);
		}*/
		if(!m_bOnFlyAway)
			m_gs.bSetNodeItem(pNode);
	}
	
	else if(m_bDragging)// dragging
	{				
		if(!m_bEventEnable)
		{
			for(int i=0;i<pNode.size();i++)
			{				
				if((i+m_nCamIndex) > (m_rects.size()-1))
				{					
					if (!m_bOnFlyAway)
						*m_bOutOfRange = true;
					else
						m_bOutOfRangeIPWall = true;
					break;
				}
				vNewVideoSet(pNode[i]);
				m_vctpNode.push_back(pNode[i]);
				
				if (!m_bOnFlyAway)
					*m_bOutOfRange = false;
				else
					m_bOutOfRangeIPWall = false;
			}
		}
		else
		{
			// find a empty cell in m_videos
			for(int i = 0;i<pNode.size();i++)
			{
				for(int j = 0; j < m_videos.size();j++)
				{
					if(m_videos[j] == NULL)
					{
						vector<NODEITEM*> test;
						test.push_back(pNode[i]);
						vNewVideoSet(test,j);
						m_vctpNode.push_back(pNode[i]);
						break;
					}
				}
			}
		}
		if(!m_bOnFlyAway)
		m_gs.bSetNodeItem(m_vctpNode);
		
	}
	m_bSwap = false;
	m_pbk->GetMainFrame()->vSetEventLV();
}

void  CLiveView::vNewVideoSet(NODEITEM* pNode)
{
		if(!pNode) return;
		if(m_nCamIndex > m_rects.size()-1) return;

		BYTE connectType =   NETSESSION_LOGIN_TYPE_VIDEO |NETSESSION_LOGIN_TYPE_EVENT;
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
		WORD nTmpProtocol(0);
		
		CVideoThin* pvideo;
		pvideo = new CVideoThin();
	
		pvideo->Create(_T(" "), dwStyle, m_rects[m_nCamIndex], this ,IDC_SCREEN+m_nCamIndex);
		pvideo->MoveWindow(&m_rects[m_nCamIndex]);
		pvideo->m_nRectIdx = m_nCamIndex;
		pvideo->m_nVideoIdx = m_nCamIndex;
		pvideo->m_bDisplayOSD = m_bDisplayOSD;
		m_videos.push_back(pvideo);
		pvideo->vAttach(pNode); 
		++m_nCamIndex;

		// set the focus video to the first CAM
		if(m_nCamIndex == 1)
		{	
			// stream service using CGI to PTZ control
			if ((m_videos.size() >0) && (m_videos[0]))
			{
				m_pbk->GetMainFrame()->vSetFocusVideoSet(pNode, m_videos[0]->vGetVideoMsgId(), m_videos[0]);
			}
		}

}
void  CLiveView::vNewVideoSet(vector<NODEITEM*> pNode, int nCamIdx) // for event live view. for the event matrix only
{
		if(pNode.empty()) return;
		if(nCamIdx > m_rects.size()-1 || nCamIdx > m_videos.size()-1) return;

		BYTE connectType =   NETSESSION_LOGIN_TYPE_VIDEO |NETSESSION_LOGIN_TYPE_EVENT;
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
		WORD nTmpProtocol(0); 	

		CVideoThin* pvideo;
		pvideo = new CVideoThin();
	
		pvideo->Create(_T(" "), dwStyle, m_rects[nCamIdx], this ,IDC_SCREEN+nCamIdx);
		pvideo->MoveWindow(&m_rects[nCamIdx]);
		pvideo->m_nRectIdx = nCamIdx;
		pvideo->m_nVideoIdx = nCamIdx;
		pvideo->m_bDisplayOSD = m_bDisplayOSD;
		m_videos[nCamIdx] = pvideo;
		((CVideoThin*)pvideo)->vAttach(&pNode); 
		m_nCamIndex++;
		
}
void  CLiveView::vNewVideoSet(NODEITEM* pNode, int nCamIdx) // for event live view. for other cells
{
		if(!pNode) return;
		if(nCamIdx > m_rects.size()-1 || nCamIdx > m_videos.size()-1) return;

		BYTE connectType =   NETSESSION_LOGIN_TYPE_VIDEO |NETSESSION_LOGIN_TYPE_EVENT;
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
		WORD nTmpProtocol(0); 	

		CVideoThin* pvideo;
		pvideo = new CVideoThin();
	
		pvideo->Create(_T(" "), dwStyle, m_rects[nCamIdx], this ,IDC_SCREEN+nCamIdx);
		pvideo->MoveWindow(&m_rects[nCamIdx]);
		pvideo->m_nRectIdx = nCamIdx;
		pvideo->m_nVideoIdx = nCamIdx;
		pvideo->m_bDisplayOSD = m_bDisplayOSD;
		m_videos[nCamIdx] = pvideo;
		((CVideoThin*)pvideo)->vAttach(pNode); 
		m_nCamIndex++;
		
}
void  CLiveView::vNewVideoSet(vector<NODEITEM*> pNode)
{
	if(pNode.size()==0) return;
	if(m_nCamIndex > m_rects.size()-1) return;

		BYTE connectType =   NETSESSION_LOGIN_TYPE_VIDEO |NETSESSION_LOGIN_TYPE_EVENT;
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
		WORD nTmpProtocol(0); 

		CVideoThin* pvideo;
		pvideo = new CVideoThin();
	
		pvideo->Create(_T(" "), dwStyle, m_rects[m_nCamIndex], this ,IDC_SCREEN+m_nCamIndex);
		pvideo->MoveWindow(&m_rects[m_nCamIndex]);
		pvideo->m_nRectIdx = m_nCamIndex;
		pvideo->m_nVideoIdx = m_nCamIndex;
		pvideo->m_bDisplayOSD = m_bDisplayOSD;
		pvideo->m_bAutoPlay = true;
		m_videos.push_back(pvideo);
		((CVideoThin*)pvideo)->vAttach(&pNode); 
		m_nCamIndex++;

		// set the focus video to the first CAM  
		if(m_nCamIndex == 1)
		{	
			// stream service using CGI to PTZ control
			if ((m_videos.size() >0) && (m_videos[0]))
			{
			   m_pbk->GetMainFrame()->vSetFocusVideoSet(pNode[0], m_videos[0]->vGetVideoMsgId());	
			}
		}
}
void  CLiveView::vFreeVideoSet(int nIdx)
{
	BeginWaitCursor();
	m_nCamIndex = 0;
	nFocusVideo =0;
	if (this->m_hWnd) ShowWindow(SW_HIDE);

	if (nIdx != 0)
	{
		if (nIdx < m_videos.size())
		{
			if (m_videos[nIdx])
			{
				m_videos[nIdx]->vStopPlay();
				m_videos[nIdx]->vStopRecv();		
				m_videos[nIdx]->vFreePTZThread();

				::SendMessage(m_videos[nIdx]->m_hWnd, WM_DESTROY, 0, 0);
				delete m_videos[nIdx];
				m_videos[nIdx] = NULL;			
			}		 
		}
		m_videos.clear();
	} 
	else
	{
		for (BYTE ci=0; ci < m_videos.size(); ci++)
		{
			if (m_videos[ci])
			{
				m_videos[ci]->vStopPlay();
				m_videos[ci]->vStopRecv();		
				m_videos[ci]->vFreePTZThread();

				::SendMessage(m_videos[ci]->m_hWnd, WM_DESTROY, 0, 0);
				delete m_videos[ci];
				m_videos[ci] = NULL;			
			}		 
		}
		m_videos.clear();
	}
		
	if (this->m_hWnd) ShowWindow(SW_SHOW);
	EndWaitCursor();

}
LRESULT CLiveView::vSetOSD(WPARAM wParam, LPARAM lParam)
{
	bool bOSD = wParam;
	
	for(int ci=0; ci < m_videos.size(); ci++)
	{
		m_videos[ci]->bResetOSD(wParam);			
	}

	return 0;
}
void CLiveView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPoint pt(point);
	BYTE nTargetRectIdx=0;
	BYTE nSourceRectIdx=0;
	bool bRectInPt = false;

	if(m_bSwap)
	{
		if(nFocusVideo > m_rects.size()-1) 
			nFocusVideo = 0;

		nSourceRectIdx = nFocusVideo;

		// not allow swap event video
		if(m_bEventEnable)
		{
			if(nSourceRectIdx == m_nEventRectIdx)
			{
				for(int j=0;j<m_videos.size();j++)
				{
					if(m_videos[j])
					m_videos[j]->m_bDrag = false;		
				}
				m_bSwap = false;	
				return;
			}
		}

		int ci;
		for (ci=0; ci < m_rects.size(); ci++)
		{
			CRect rc = m_rects[ci];
			if (rc.PtInRect(pt))
			{
				break;
			}
		}		

		// not allow swap event video
		if(m_bEventEnable)
		{
			if(ci == m_nEventRectIdx)
			{
				for(int j=0;j<m_videos.size();j++)
				{
					if(m_videos[j])
					m_videos[j]->m_bDrag = false;		
				}
				m_bSwap = false;	
				return;
			}
			if(ci > (int) m_videos.size()-1 || m_videos[ci] == NULL) return;
		}
	
		if (m_videos.size() > ci)
		{
			nTargetRectIdx = ci;
			DrawVideoFrame(nSourceRectIdx, nTargetRectIdx);
			
			if(nSourceRectIdx != nTargetRectIdx)
			{
                m_videos[nSourceRectIdx]->MoveWindow(&m_rects[nTargetRectIdx]);
                m_videos[nTargetRectIdx]->MoveWindow(&m_rects[nSourceRectIdx]);
			
				// new video stream service using CGI
				m_pbk->GetMainFrame()->vSetFocusVideoSet(m_videos[nSourceRectIdx]->GetNodeItem(), m_videos[nSourceRectIdx]->vGetVideoMsgId(), m_videos[nSourceRectIdx]);

				swap(m_videos[nSourceRectIdx],m_videos[nTargetRectIdx]);

				if(m_bDisplayOSD)
				{
					m_videos[nSourceRectIdx]->vRearrangeOSD();
					m_videos[nTargetRectIdx]->vRearrangeOSD();
				}
			
				m_videos[nSourceRectIdx]->Invalidate(true);
				m_videos[nTargetRectIdx]->Invalidate(true);
			}
							
		}			
				
		for(int j=0;j<m_videos.size();j++)
		{
			if(m_videos[j])
			m_videos[j]->m_bDrag = false;		
		}
		m_bSwap = false;		
	}
}

void CLiveView::SetTreeNodeFocus()
{
// 	NODEITEM *pNode;
// 	((CTaskTreeCtrl*)((CMainFrame*)AfxGetMainWnd())->GetTreeView())->SelectItem(m_videos[nSourceRectIdx]->GetNodeItem()->hTreeItem);
// 	pNode = m_videos[nTargetRectIdx]->GetNodeItem();
}

void CLiveView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(nFlags == MK_LBUTTON )
	{
		m_bSwap &= true;
		if (m_bSwap)
        {
		    for(int j=0;j<m_videos.size();j++)
		    {	
				if(m_videos[j])
			    m_videos[j]->m_bDrag = true;
		    }
        }		
	}
}

BOOL CLiveView::PreTranslateMessage(MSG* pMsg)
{
	int nCounter = 0;
	// TODO: Add your specialized code here and/or call the base class
	if (m_videos.size() > 0)
	{		
		if ( /*( pMsg->message == WM_CONTEXTMENU ) ||*/ ( pMsg->message == WM_MOUSEWHEEL) 
			|| ( pMsg->message == WM_LBUTTONDOWN )  || ( pMsg->message == WM_LBUTTONUP)
			|| ( pMsg->message == WM_MOUSEMOVE ) //|| ( pMsg->message == WM_LBUTTONDBLCLK) 
			)
		{
			CPoint pt;			
			//pt.x = GET_X_LPARAM(pMsg->lParam); 
			//pt.y = GET_Y_LPARAM(pMsg->lParam); 
			pt=AfxGetCurrentMessage()->pt;
			
			if( pMsg->message == WM_MOUSEWHEEL)
			{
				ScreenToClient(&pt);
			}
			for (int ci = 0; ci < m_videos.size() && ci < m_rects.size(); ci++)
			{
				if(m_bOneMatrixView)
				{
					CRect rect;
					GetClientRect(rect);
					if(rect.PtInRect(pt))
					{
						ClientToScreen(&pt);// send screen x-y cordinate to cvideo
						if(m_videos[m_nOneRectIdx])
						{
							::SendMessage(m_videos[m_nOneRectIdx]->m_hWnd, pMsg->message, pMsg->wParam, MAKELPARAM(pt.x,pt.y));	
						}
						break;
					}
					
				}
				else
				{
					//if (m_rects[ci].PtInRect(pt))
					CRect rect;
					rect=m_rects[ci];
					ClientToScreen(rect);

					if (rect.PtInRect(pt))
					{
						//TRACE(_T("LV ptOrgX:%d,ptOrgY:%d\r\n"),pt.x,pt.y);
						//ClientToScreen(&pt);// send screen x-y cordinate to cvideo
						//TRACE(_T("LV ptX:%d,ptY:%d\r\n"),pt.x,pt.y);
						if(m_videos[ci])
						{
							//::SendMessage(m_videos[ci]->m_hWnd, pMsg->message, pMsg->wParam, pMsg->lParam);

							if (pMsg->message == WM_LBUTTONDOWN)
							{							
									if (m_videos[ci]->m_bPanorama)
									{
										CPoint ptPana(pt);
										ScreenToClient(&ptPana);
										OnLButtonDown(1,ptPana);
									}
							}
							else if (pMsg->message == WM_LBUTTONUP)
							{
								////////////////////////Larry add 103-01-24/////////////////////
								if (m_videos[ci]->m_bPanorama && m_videos[ci]->m_bDrag)
								{
									CPoint ptPana(pt);
									ScreenToClient(&ptPana);
									OnLButtonUp(MK_LBUTTON, ptPana);
									return true;
								}
								//////////////////////////////////////////////////////////////////
							}

							::SendMessage(m_videos[ci]->m_hWnd, pMsg->message, pMsg->wParam, MAKELPARAM(pt.x,pt.y));
						}
						break;
					}
				}
				
			}
		}
		if ( pMsg->message == WM_LBUTTONDBLCLK) 
		{
			CPoint pt;
			pt.x = GET_X_LPARAM(pMsg->lParam); 
			pt.y = GET_Y_LPARAM(pMsg->lParam); 

			if(m_nLayout == 1) return CStatic::PreTranslateMessage(pMsg); // if the matrix size is 1, don't have to onematrix view
			
			if(m_bOneMatrixView)
			{
				CRect rect;
				GetClientRect(rect);
				if(rect.PtInRect(pt))
				{
					vOneMatrixView(m_nOneRectIdx);	
				}
				if(m_pDlgPause)
				{
					DELETEOBJ(m_pDlgPause);
					vResumeLiveView();
				}
			}
			else // enable one matrix view
			{
				if(m_pbk->GetMainFrame()->m_bGroupAutoPlay && m_pbk->GetMainFrame()->m_bAutoScan)
				{
					vPauseAutoScan();
				}
				for (int ci = 0; ci < m_videos.size() && ci < m_rects.size(); ci++)
				{
					if (m_rects[ci].PtInRect(pt) )//&& nCounter == 0)
					{						
						vOneMatrixView(ci);	
						break;
						//nCounter++;
					}
				}
			}
		}

		if(pMsg->wParam == VK_ESCAPE)
		{
			if(m_bOneMatrixView)
			{
				vOneMatrixView(0);// disable onematrixview when pressed esc
			}
		}				
	}

	return CStatic::PreTranslateMessage(pMsg);
}
LRESULT  CLiveView::vChangeSendAudioBMP(WPARAM wParam, LPARAM lParam)
{
	int nVideoIdx = wParam;
	COLORREF clBG = GLOBALBKCOLOR;

	if(lParam ==  true)// change to orignal BMP and stop send audio when Autoplay enable 
	{
		for(int ci=0; ci < m_videos.size(); ci++)
		{
			if(m_videos[ci]->m_pOSD->m_pSendAudio)
			{				
			   m_videos[ci]->m_pOSD->m_pSendAudio->SetBitmaps(IDB_BITMAP_OSD_MIC, clBG);
			}
			
		}
	}
	else 
	{

		for(int ci=0; ci < m_videos.size(); ci++)
		{
			if(ci == nVideoIdx)
			{
				if(m_videos[ci]->m_pOSD->m_pSendAudio)
				{
					(m_videos[ci]->m_pOSD->m_pSendAudio)->SetBitmaps(IDB_BITMAP_OSD_MIC_H, clBG);	
					(m_videos[ci]->m_pOSD->m_pSendAudio)->Invalidate();
					
				}
			}
			else
			{							
				if(m_videos[ci]->m_pOSD->m_pSendAudio)
				{				
					m_videos[ci]->m_pOSD->m_pSendAudio->SetBitmaps(IDB_BITMAP_OSD_MIC, clBG);
					m_videos[ci]->m_pOSD->m_pSendAudio->Invalidate();
				}		
			}	
		}
	}
	return 0;
}

vector <NODEITEM*> CLiveView::vctDispatchNode(vector<NODEITEM*> vctpNode, int nMod)
{
	vector<NODEITEM*> vctpNodeTmp;

	for(int i=0;i<vctpNode.size();i++)
	{
		if((i % m_rects.size()) == nMod)
		{
			vctpNodeTmp.push_back(vctpNode[i]);
			m_vctpNode.push_back(vctpNode[i]);
		}
	}	
	return vctpNodeTmp;
}

void CLiveView::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case ID_GROUP_AUTOPLAY_TIMER:
	{
		m_bGetGroupIdx = false;
		if(m_bPlaySameGroup)
		{
			vAttach(m_vctpGroupAutoScanNode);
		}
		else
		{
			vAttach(m_vctpNode);
		}
		break;
	}
	case ID_GROUP_AUTOPLAY_PAUSE_TIMER:
	{
		
		if(m_bAutoScanPause)
		{
			m_nGroupAutoPauseTimer = SetTimer(ID_GROUP_AUTOPLAY_PAUSE_TIMER, (AUTOSCAN_PAUSE_TIMER * 1000) ,NULL);
		}
		else
		{
			KillTimer(m_nGroupAutoPauseTimer);
			vResumeLiveView();
		}

		break;
	}
	case ID_AUTOPLAY_TIMER :
	{		
		for(int i=0;i<m_videos.size();i++)
		{
			if(m_videos[i]->m_bAutoPlay == true)
			{
				((CVideoThin*)m_videos[i])->vSetPlayNextChannel();
			}
		}
		vChangeSendAudioBMP(NULL,true);

		if (_MaskBeta)
			m_pbk->GetMainFrame()->vStartDisplayAutoPlay();
	break;
	}
	case ID_FPS_TIMER:
	{
		if(m_bDisplayOSD)
		{
			for(int i = 0 ;i < m_videos.size(); i++)
			{
				if(m_videos[i])
				{
					//m_videos[i]->vUpdateFPS();
				}
			}
		}
		else
		{
			KillTimer(m_nFPSTimer);
		}
	break;
	}
	case ID_EVENT_TIMER:
	{		
		if(!m_bEventEnable)
		{
			KillTimer(m_nEventTimer);
			break;
		}
		else
		{
			//m_videos[m_nOriginalRectIdx]->MoveWindow(m_rects[m_nOriginalRectIdx]);
			vPlayEventVideoQueue();
			m_nEventCounter--;
			KillTimer(m_nEventTimer);
			break;
		}
	}
	default: 
		break;
	}
}

void CLiveView::vOneMatrixView(int nRectIdx)
{
	m_bOneMatrixView ^= 1;

	if(m_videos[nRectIdx] == NULL /*|| m_videos[m_nOneRectIdx] == NULL*/) return;

	vSetGrids(m_nLayout);

	if(m_bOneMatrixView)
	{
		CRect rect;
		CRect rectVoid(0,0,0,0);
		GetClientRect(rect);
		m_nOneRectIdx = nRectIdx;

		for(int i = 0 ; i <m_videos.size() && i <m_rects.size();i++)
		{
			if(i != nRectIdx && m_videos[i])
			{
				m_videos[i]->vSetStatus(true);
				m_videos[i]->MoveWindow(rectVoid);
			}
		}

		m_videos[nRectIdx]->MoveWindow(rect);
		::SendMessage(m_videos[nRectIdx]->m_hWnd, WM_ERASEBKGND, NULL,NULL); 	
	}

	else
	{		
		for(int i = 0 ; i < m_videos.size() && i <m_rects.size();i++)
		{
			if(i != m_nOneRectIdx && m_videos[i])
			{
				m_videos[i]->vSetStatus(false);
				m_videos[i]->MoveWindow(m_rects[i]);
			}
		}

		m_videos[m_nOneRectIdx]->MoveWindow(m_rects[m_nOneRectIdx]);
		TRACE(_T("m_nOneRectIdx: %d\r\n"),m_nOneRectIdx);
		m_nOneRectIdx = 0;
		TRACE(_T("m_nOneRectIdx: %d\r\n"),m_nOneRectIdx);
	}
}
void CLiveView::vEventCallBack(EventInfo *m_EventInfo, void *lpParam)
{
	CLiveView* pThis = (CLiveView*)lpParam;	
	
	if(!pThis->m_bEventEnable) return;

	CString csEventIP(_T("")), csEventContext(_T("")), csEventStreamID(_T("")), csEventCamIDX(_T(""));
	csEventIP.Format(_T("%s"), m_EventInfo->tcIP);

	switch(m_EventInfo->nEventType)
	{
	case EVENTTYPE::MOTION_DETECTION:
		//csEventContext.Format(_T("%s"), m_EventInfo->tcEventType);
		csEventContext = _T("Motion Detection");
		break;
	case EVENTTYPE::DIGITAL_INPUT:
		csEventContext = _T("Digital Input");
		break;
	case EVENTTYPE::VIDEO_LOST:
		csEventContext = _T("Video Lost");
		break;
	case EVENTTYPE::VIDEO_OPEN:
		csEventContext = _T("Video Open");
		break;
	case EVENTTYPE::REBOOT:
		csEventContext = _T("Reboot");
		break;
	case EVENTTYPE::DIGITAL_OUTPUT:
		csEventContext = _T("Digital Output");
		break;
	case EVENTTYPE::MISSING_OBJECT:
		csEventContext = _T("Missing Object");
		break;
	case EVENTTYPE::EVENT_TRIGGER:
		csEventContext = _T("Event Trigger");
		break;
	default:
		ASSERT(false);
		break;
	}
	csEventCamIDX.Format(_T("%d"), m_EventInfo->nCamIdx);

	if(csEventIP == _T(""))	return;

	pThis->vMatchEventCam(m_EventInfo->nCamIdx);

}
UINT _stdcall CLiveView::thdGetEventFromService(LPVOID lpParam)
{
	CLiveView * pThis = (CLiveView*) lpParam;

	pThis->m_EventRec->vSetCallBack(pThis->vEventCallBack, pThis);
	pThis->m_EventRec->vStartReceive();

	return 0;	
}
void CLiveView::vMatchEventCam(int nCamIdx)
{
		
	if(!m_bEventEnable) return;

	for(int i = 0 ; i < m_vctpCurrentGroupNodes.size();i++)
	{
		if(m_vctpCurrentGroupNodes[i]->camera_idx == nCamIdx && m_nEventRectIdx < (int) m_videos.size()-1 && m_videos[m_nEventRectIdx])
		{
			m_nOriginalRectIdx = i;
			m_nPlayIdxQueue.push(m_nOriginalRectIdx);
			vPlayEventVideoQueue();
			m_nEventCounter++;
			break;
		}
	}
	
}
void CLiveView::vPlayEventVideoQueue()
{
	if(!m_bEventEnable) return;

	if(m_nPlayIdxQueue.size() != 0 && m_nEventRectIdx < (int) m_videos.size()-1 && m_videos[m_nEventRectIdx])
	{
		int nRet = m_nPlayIdxQueue.front();		
		m_videos[m_nEventRectIdx]->vSetPlayNextChannel(nRet);
		m_nEventTimer = SetTimer(ID_EVENT_TIMER, (EVENTPLAYTIME*1000), NULL);
		m_nPlayIdxQueue.pop();
	}
	
}
void CLiveView::vDispatchEventVideo(vector<NODEITEM*> pNode)
{
	if(!m_bEventEnable) return;

	
	m_pbk->GetMainFrame()->GetTreeView()->vGetGroupNodeItems(m_vctpCurrentGroupNodes);
	
	vector <NODEITEM*> vctNodeTest;				
	int nMatrix = m_rects.size();

	if(!m_videos.empty())
	{
		vFreeVideoSet();
	}
	
	// find the rect index for event cell 
	switch(nMatrix)
	{
	case 4:
	case 6:
	case 13: 
	case 10:
	{
		m_nEventRectIdx = 0;
		break;
	}
	/*case 13: 
	{
		m_nEventRectIdx = 5;
		break;
	}*/
	case 17:
	{
		m_nEventRectIdx = 6;
		break; 
	}
	default: break;
	}		
	// push m_videos with null
	for(int i = 0 ; i < m_rects.size();i++)
	{
		m_videos.push_back(NULL);
	}
	// new event videos
	vNewVideoSet(m_vctpCurrentGroupNodes,m_nEventRectIdx);
	// if Event rect idx is 0, loop must add 1 more time to new the last node in vector.
	int nLoopTime(0);
	m_nEventRectIdx == 0 ? nLoopTime = pNode.size()+1: nLoopTime = pNode.size();

	// attach nodeitems 
	for(int i=0;i<nLoopTime;i++)
	{
		if(i > (int)(m_rects.size()-1)) // out of view range
		{
			if (!m_bOnFlyAway)
				*m_bOutOfRange = true;
			else
				m_bOutOfRangeIPWall = true;
			break;
		}
		if(i == m_nEventRectIdx) // bypass the event cell
		{				
			continue;
		}
		else if( i >m_nEventRectIdx) // the matrix cell is exceed than event one , new the previous node 
		{			
			vNewVideoSet(pNode[i-1],i);
			m_vctpNode.push_back(pNode[i-1]);
		}
		else if( i < m_nEventRectIdx)// the matrix cell is preceed than event one , new the node
		{			
			vNewVideoSet(pNode[i],i);
			m_vctpNode.push_back(pNode[i]);
		}		
		
	}	
}
void CLiveView::vGetFitMatrixSize(int nVideoSize)
{
}

void CLiveView::vGroupAutoScan(void)
{
	CString csMsg,csGroupName;

	if(m_bGetGroupIdx)// get autoplayidx from mainfrm
	{
		while(m_nGroupIdxQueue.size() !=0)
		{
			m_nGroupIdxQueue.pop();	
		}

		for(int i = 0;i< m_pbk->GetMainFrame()->m_vctnGroupAutoPlayIdx.size();i++)
		{
			m_nGroupIdxQueue.push(m_pbk->GetMainFrame()->m_vctnGroupAutoPlayIdx[i]);
		}
		CGS::bSetGroupAutoPlayIdx(m_pbk->GetMainFrame()->m_vctnGroupAutoPlayIdx);
	}

	if(m_nGroupIdxQueue.size() == 0) return; // if queue is empty return;

	m_nCurrentPlayGroupId = m_nGroupIdxQueue.front();
	m_pbk->GetMainFrame()->vGetGroupCam(m_nCurrentPlayGroupId,m_vctpGroupAutoScanNode);
	
	if(!m_bPlaySameGroup) 
	{	
		int i(0);			
		for(i;i<m_vctpGroupAutoScanNode.size();i++)
		{					
			if(i > (int)(m_rects.size()-1)) // out of view range
			{
				if (!m_bOnFlyAway)
					*m_bOutOfRange = true;
				else
					m_bOutOfRangeIPWall = true;
				m_nPreviousGroupNode = i;
				m_bPlaySameGroup = true;
				break;
			}	
			else
			{
				vNewVideoSet(m_vctpGroupAutoScanNode[i]);
   				m_vctpNode.push_back(m_vctpGroupAutoScanNode[i]);				
			}
		}
		if(m_nPreviousGroupNode == -1)// no more videos to play, switch to next group
		{
			m_nGroupIdxQueue.pop();
			m_nGroupIdxQueue.push(m_nCurrentPlayGroupId);
		}
	}
	else // there are nodes in group not playing yet
	{	
		int i(0),ci(0);		//i is the counter for GroupNode idx, ci is counter for m_rects
		i = m_nPreviousGroupNode;	
		for( i;i<m_vctpGroupAutoScanNode.size();i++,ci++)
		{					
			if(ci > (int)(m_rects.size()-1)) // out of view range
			{
				if (!m_bOnFlyAway)
					*m_bOutOfRange = true;
				else
					m_bOutOfRangeIPWall = true;
				m_nPreviousGroupNode = i;
				m_bPlaySameGroup = true;
				break;
			}	
			else
			{
				vNewVideoSet(m_vctpGroupAutoScanNode[i]);
   				m_vctpNode.push_back(m_vctpGroupAutoScanNode[i]);				
			}
		}
		if(i == m_vctpGroupAutoScanNode.size() )// all node within a group are played, switch to next group and clear parameters
		{
			m_bPlaySameGroup = false;
			m_nGroupIdxQueue.pop();
			m_nGroupIdxQueue.push(m_nCurrentPlayGroupId);
			m_nPreviousGroupNode = -1;
		}
	}

	m_nGroupAutoPlayTimer = SetTimer(ID_GROUP_AUTOPLAY_TIMER, ((m_pbk->GetMainFrame())->m_nAutoScanTime*1000) ,NULL);
	m_nAutoScanTime = m_pbk->GetMainFrame()->m_nAutoScanTime*1000;
	//UINT nRetEvent(0);
	//m_hThreadAutoScanWait = (HANDLE)_beginthreadex(NULL, 0, thdAutoScanWait, (LPVOID)this, 0, &nRetEvent);
	
	
	// Set mainfrm window text
	m_pbk->GetMainFrame()->vGetGroupName();
	csGroupName = m_pbk->GetMainFrame()->m_vctcsGroupNames[m_nCurrentPlayGroupId-1];
	csMsg.Format(_T("%s --> %s"),  CProductDefine::m_tzProductName, csGroupName);
	if(!m_bOnFlyAway)	
	{
		m_pbk->GetMainFrame()->SetWindowTextW(csMsg);
	}
	else// flyaway dlg title
	{
			GetParent()->SetWindowTextW(csMsg);
	}
		
	//((CMainFrame*)AfxGetMainWnd())->vSetAutoScanCallBack(vAutoScanCallBack,this);
}

void CLiveView::vResumeLiveView(void)
{
	if(m_vctpNode.size()==0) return;

	m_pbk->GetMainFrame()->m_bGroupAutoPlay = m_pbk->GetMainFrame()->m_bAutoScan = true;
	m_bAutoScanPause = false;
	DELETEOBJ(m_pDlgPause);
	m_nLayout = m_strPause.nLayout;
	m_bGetGroupIdx = m_strPause.bGetGroupIdx;
	m_bPlaySameGroup = m_strPause.bPlaySameGroup;
	m_nPreviousGroupNode = m_strPause.nPreviousGroupNode;
	vSetGrids(m_nLayout);
	vAttach(m_vctpNode);
}

void CLiveView::vPauseAutoScan(void)
{	
	memset(&m_strPause,0,sizeof(PAUSEINFO));
	m_strPause.nLayout = m_nLayout;
	m_strPause.bGetGroupIdx =	m_bGetGroupIdx ;
	m_strPause.bPlaySameGroup = m_bPlaySameGroup ;
	m_strPause.nPreviousGroupNode = m_nPreviousGroupNode ;
	m_pbk->GetMainFrame()->m_bGroupAutoScanPause = m_bAutoScanPause = true;

	// create pause dialog
	vCreatePauseDlg();

	if(m_pbk->GetMainFrame()->m_bGroupAutoPlay && m_pbk->GetMainFrame()->m_bAutoScan)
	{
		KillTimer(m_nGroupAutoPlayTimer);
	}
	else if(m_pbk->GetMainFrame()->m_bAutoScan)
	{
		KillTimer(m_nAutoPlayTimer);
	}
	
	m_pbk->GetMainFrame()->m_bGroupAutoPlay = false;
	m_pbk->GetMainFrame()->m_bAutoScan = false;
	m_nGroupAutoPauseTimer = SetTimer(ID_GROUP_AUTOPLAY_PAUSE_TIMER, (AUTOSCAN_PAUSE_TIMER*1000) ,NULL);	
	
}
UINT _stdcall CLiveView::thdAutoScanWait(LPVOID lpParam)
{
	CLiveView *pThis = (CLiveView*) lpParam;
	
	pThis->m_hPauseAutoScan = CreateEvent(NULL, false, false, _T("Pause AutoScan"));
	pThis->m_hResumeAutoScan = CreateEvent(NULL, false, false, _T("Resume AutoScan"));
	
	HANDLE hEvents[2] ={pThis->m_hPauseAutoScan, pThis->m_hResumeAutoScan};
	DWORD dwRet(0),dwWaitTime(0);
	static int nLayout;
	dwWaitTime = pThis->m_nAutoScanTime;
	
	do
	{			
		nLayout = pThis->m_nLayout;
		dwRet = ::WaitForMultipleObjects(2, hEvents, false, dwWaitTime);
		DWORD nError = ::GetLastError();
		switch (dwRet)
		{
		case WAIT_OBJECT_0:// group autoscan timer
		{
			dwWaitTime = INFINITE; 
			break;
		}
		case WAIT_OBJECT_0+1:
		{
			dwWaitTime = pThis->m_nAutoScanTime;
			pThis->vResumeLiveView(); 
			break;
		}
	
		case WAIT_TIMEOUT:
		{
			dwWaitTime = pThis->m_nAutoScanTime;
			//pThis->vChangeGroup();
			break;
		}
		default:break;
		}

	}while(true);

	return 0;
}
void CLiveView::vAutoScanCallBack(void *lpParam)
{
	 CLiveView* pThis = ( CLiveView*) lpParam;
	 pThis->vPauseAutoScan();
}

void CLiveView::vCreatePauseDlg(void)
{
	// create pause dialog
	m_pbk->GetMainFrame()->m_nCurrentPlayGroupId = m_nCurrentPlayGroupId;
	if(!m_pDlgPause)
	{		
		CRect rect;
		GetClientRect(rect);
		m_pDlgPause = new CDlgAutoScanPause ;
		m_pDlgPause->m_pParentHwnd =  this;
		m_pDlgPause->Create(IDD_DLG_AUTOSCAN_PAUSE);		
		//m_pDlgPause->SetWindowPos(NULL,rect.right+100,rect.top,500,100,SWP_SHOWWINDOW);	
		m_pDlgPause->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDlgPause->vSetDisplayGroupName();
	}
}

void CLiveView::OnDestroy()
{
	if(m_bSetReg)
		m_gs.bSetNodeItem(m_vctpNode);

    m_EventRec->vStopEventReceive();
    CStatic::OnDestroy();

    // TODO: Add your message handler code here
}

void CLiveView::SetVideoFocos(NODEITEM *pNode)
{
	if (m_bOneMatrixView)
	{
		return;
	}

	NODEITEM *pvideoNode;
	int nIdx = 0, nCount = m_videos.size();
	for(int nIdx = 0; nIdx < nCount ; nIdx++)
	{
		if (m_videos[nIdx])
		{
			pvideoNode = m_videos[nIdx]->GetNodeItem();
			if ((memcmp(pvideoNode->mac, pNode->mac, sizeof(pNode->mac)) ==0) &&
				(memcmp(pvideoNode->ip, pNode->ip, sizeof(pNode->ip)) ==0))
			{
				DrawVideoFrame(nFocusVideo, nIdx);
			}
		}		
	}
}

void CLiveView::FreeOneVideo(NODEITEM *pNode)
{
	NODEITEM *pvideoNode;

	BeginWaitCursor();
	m_nCamIndex = 0;
	nFocusVideo =0;
	bool bDelete = false;
	if (this->m_hWnd) ShowWindow(SW_HIDE);

	int nIdx = 0, nCount = m_videos.size();
	for(nIdx = 0; nIdx < nCount ; nIdx++)
	{
		pvideoNode = m_videos[nIdx]->GetNodeItem();

		if ((memcmp(pvideoNode->mac, pNode->mac, sizeof(pNode->mac)) == 0) &&
			(memcmp(pvideoNode->ip, pNode->ip, sizeof(pNode->ip)) == 0))
		{
			bDelete = true;
			m_videos[nIdx]->vStopPlay();
			m_videos[nIdx]->vStopRecv();		
			m_videos[nIdx]->vFreePTZThread();

			::SendMessage(m_videos[nIdx]->m_hWnd, WM_DESTROY, 0, 0);
			delete m_videos[nIdx];
			m_videos[nIdx] = NULL;		
			m_videos.erase(m_videos.begin()+nIdx);
			break;
		}
	}

	vector <NODEITEM*>::iterator it = m_vctpNode.begin();
	while(it != m_vctpNode.end())
	{
		if ((memcmp((*it)->mac, pNode->mac, sizeof(pNode->mac)) == 0) &&
			(memcmp((*it)->ip, pNode->ip, sizeof(pNode->ip)) == 0))
		{
			m_vctpNode.erase(it);
			break;
		}
		it++;
	}

	it = m_vctpPreNode.begin();
	while(it != m_vctpPreNode.end())
	{
		if ((memcmp((*it)->mac, pNode->mac, sizeof(pNode->mac)) == 0) &&
			(memcmp((*it)->ip, pNode->ip, sizeof(pNode->ip)) == 0))
		{
			m_vctpPreNode.erase(it);
			break;
		}
		it++;
	}

	it = m_vctpCurrentGroupNodes.begin();
	while(it != m_vctpCurrentGroupNodes.end())
	{
		if ((memcmp((*it)->mac, pNode->mac, sizeof(pNode->mac)) == 0) &&
			(memcmp((*it)->ip, pNode->ip, sizeof(pNode->ip)) == 0))
		{
			m_vctpCurrentGroupNodes.erase(it);
			break;
		}
		it++;
	}

	it = m_vctpGroupAutoScanNode.begin();
	while(it != m_vctpGroupAutoScanNode.end())
	{
		if ((memcmp((*it)->mac, pNode->mac, sizeof(pNode->mac)) == 0) &&
			(memcmp((*it)->ip, pNode->ip, sizeof(pNode->ip)) == 0))
		{
			m_vctpGroupAutoScanNode.erase(it);
			break;
		}
		it++;
	}

	if (bDelete)
	{
		int nIdx = 0, nCount = m_videos.size();
		for(nIdx = 0; nIdx < nCount ; nIdx++)
		{
			m_videos[nIdx]->MoveWindow(m_rects[nIdx]);
		}
	}
	if (this->m_hWnd) ShowWindow(SW_SHOW);
}

LRESULT CLiveView::OnUpdateView(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case UM_FREE_VIDEO:
		{
			NODEITEM *pNode = (NODEITEM*)lParam;
			if (!pNode) 
			{
				vFreeVideoSet();
			}
			else
			{
				FreeOneVideo(pNode);
			}	
		}
		break;
	case UM_SET_VIDEO_FOCUS:
		{
			NODEITEM *pNode = (NODEITEM*)lParam;
			if (pNode)
			{
				SetVideoFocos(pNode);
			}
		}
		break;
	case UM_UPDATE_DEVICE:
		{
			int camid = lParam;
			UpdateDevice(camid);
		}
		break;
	default:
		break;
	}

	return 0;
}

void CLiveView::UpdateDevice(int camid)
{
	NODEITEM *pvideoNode = NULL;
	int nIdx = 0, nCount = m_videos.size();
	for(nIdx = 0; nIdx < nCount ; nIdx++)
	{
		if (m_videos[nIdx])
		{
			pvideoNode = m_videos[nIdx]->GetNodeItem();

			if (pvideoNode->cameraid == camid)
			{
				m_videos[nIdx]->UpdateDevice(camid);
			}
		}
	}
}