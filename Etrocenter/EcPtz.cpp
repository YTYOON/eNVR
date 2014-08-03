 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NVR.h"
#include "EcPtz.h"
#include "Resource.h"
#include "MainFrm.h" 

#define TABID 2317 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define PTZ_THRESHOLD  2
#define NUMOFPRESET 6 // present+1
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEcPtz::CEcPtz(void):m_pNetsession(NULL),m_pNode(NULL),m_nPTZSpeed(6),m_bCGISent(false),m_nPreInput(0),m_bJoystickZMove(false),m_bPatrolEnable(false)
,m_hPatrolThread(NULL),m_nPatrolSeqDurIdx(0)
{
	m_brush.CreateSolidBrush(GLOBALBKCOLOR);
	//vDBConnection();
	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
	bGetJoyStickInfo();
}

CEcPtz::~CEcPtz()
{
	//if (DBQuery.IsOpen())
	//{
	//	DBQuery.Close();
	//}
	
}

BEGIN_MESSAGE_MAP(CEcPtz, CDockablePane)
	//{{AFX_MSG_MAP(CEcPtz)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_MESSAGE(MM_JOY1MOVE, OnJoyStickMove)
	ON_MESSAGE(MM_JOY1ZMOVE, OnJoyStickZMove)
	//ON_CONTROL_RANGE(BN_CLICKED, IDB_PTZTOP, IDB_PTZHOME , &CEcPtz::OnTabPTZButtonClicked)
	//ON_CONTROL_RANGE(WM_LBUTTONUP, IDB_PTZTOP, IDB_PTZHOME , &CEcPtz::OnTabPTZButtonClickedStop)
	//ON_CONTROL_RANGE(BN_CLICKED, IDB_MINUS, IDB_PLUS , &CEcPtz::OnTabZoomButtonClicked)
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
END_MESSAGE_MAP()




void CEcPtz::DoDataExchange(CDataExchange* pDX)
{
	//CDialog::DoDataExchange(pDX);
	////{{AFX_DATA_MAP(CDlgGroupName)
	//DDX_Text(pDX, IDC_EDIT_GROUPNAME, m_csGroupName);
	//DDV_MaxChars(pDX, m_csGroupName, 31);
	//}}AFX_DATA_MAP
}

 

void CEcPtz::vSetNetSession(CNetSession* pNetsession)
{
	//this->m_PtzTab.vSetNetSession(pNetsession);
	if(!pNetsession) return;
	m_pNetsession = pNetsession;
}
 

UINT  CEcPtz::StartTimer (UINT TimerDuration)
{   
        UINT    TimerVal;
        TimerVal = SetTimer (IDT_TIMER, TimerDuration, NULL);  // Starting the Timer
	//	 AfxMessageBox("Unable to obtain timer");
        if (TimerVal == 0)
        {
          //    AfxMessageBox("Unable to obtain timer");
        }
      return TimerVal;
}// end StartTimer



BOOL CEcPtz::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CBrush brush(GLOBALBKCOLOR);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &brush);

	return true;
	//return CSizingControlBarG::OnEraseBkgnd(pDC);
}


HBRUSH CEcPtz::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDockablePane::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

   //  We can use CWnd::GetDlgCtrlID() to perform the most efficient test.
   //int ll = pWnd->GetDlgCtrlID();
   //if (pWnd->GetDlgCtrlID() == 1234 || pWnd->GetDlgCtrlID() == TABID)
   //{
   //   // Set the text color to red
   //   pDC->SetTextColor(RGB(3,7,61));

	  //pDC->SetBkColor(GLOBALBKCOLOR); //SetBkMode(TRANSPARENT);
  
   //   hbr = m_brush;
   //}

   return hbr;
}


void CEcPtz::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDockablePane::OnTimer(nIDEvent);
}
BOOL CEcPtz::bGetJoyStickInfo()
{

	JOYINFO joyinfo; 
	UINT wNumDevs, wDeviceID; 
	BOOL bDev1Attached, bDev2Attached; 

	if((wNumDevs = joyGetNumDevs()) == 0) 
		return FALSE; 
	bDev1Attached = joyGetPos(JOYSTICKID1,&joyinfo) != JOYERR_UNPLUGGED; 
	bDev2Attached = wNumDevs == 2 && joyGetPos(JOYSTICKID2,&joyinfo) != 
		JOYERR_UNPLUGGED; 
	if(bDev1Attached || bDev2Attached)   // decide which joystick to use 
		wDeviceID = bDev1Attached ? JOYSTICKID1 : JOYSTICKID2; 
	else 
		return FALSE; 

	while (MMRESULT nJoyRet = joySetCapture(this->m_hWnd, JOYSTICKID1, 0, FALSE)) 
    { 

		{
			ETROLOG(_T("JoySetCaputure fail return code %d\r\n"), nJoyRet);
			break;
		}
    } 

	return true;
}
LRESULT CEcPtz::OnJoyStickMove(WPARAM wParam, LPARAM lParam)
{
//	const int PTZ_THRESHOLD = 1; //5
//	const int PTZ_THRESHOLD_MINUS = -20;
	if ((m_pNetsession==NULL) && (m_pNode==NULL)) return 0;


	// Query DB if PTZ is enabled
	if(!bCheckPTZEnable())
		return 0;
	

	JOYINFOEX joyex;
	ZeroMemory(&joyex, sizeof(JOYINFOEX));
	joyex.dwSize = sizeof(JOYINFOEX);
	joyex.dwFlags = JOY_RETURNALL;
	joyGetPosEx(JOYSTICKID1, &joyex);
	
	 PTZAction ptzaction ;
	
	/*m_nPTZSpeed=6;*/

	if ((joyex.dwButtons  > JOY_BUTTON2) && joyex.dwButtonNumber)
	{
	//	SendMessage(MM_JOY1BUTTONDOWN, joyex.dwButtons, 0);		
		BYTE nPreset = 0;
		int nMask[]={JOY_BUTTON3, JOY_BUTTON4, JOY_BUTTON5, JOY_BUTTON6, JOY_BUTTON7, JOY_BUTTON8, JOY_BUTTON9, JOY_BUTTON10, JOY_BUTTON11, JOY_BUTTON12, JOY_BUTTON13, JOY_BUTTON14, JOY_BUTTON15, JOY_BUTTON16, JOY_BUTTON17, JOY_BUTTON18, JOY_BUTTON19, JOY_BUTTON20, JOY_BUTTON21, JOY_BUTTON22, JOY_BUTTON23, JOY_BUTTON24, JOY_BUTTON25, JOY_BUTTON26, JOY_BUTTON27, JOY_BUTTON28, JOY_BUTTON29, JOY_BUTTON30, JOY_BUTTON31, JOY_BUTTON32 };

		for (int ci=0; ci < sizeof(nMask) / sizeof(int); ci++)
		{
			if (joyex.dwButtons & nMask[ci]) 
			{
				(nPreset = ci+1);
				break;
			}
		}
		if (nPreset)
		{
			int nSize =0;
			CONTROLDATA ctrl={0};
			ctrl.channel = 1;
			ctrl.controlID = PCTL_PRESET_MOVE;
			ctrl.payload.preset_move = (BYTE)nPreset;
			nSize = 5;
			if (m_pNetsession)
			{
				int nRet = m_pNetsession->sendControl(m_pNetsession, FCC_PCONTROL,(char*)&ctrl, nSize);
				ETROLOG(_T("OnJoyStickMove return :%d\r\n"), nRet);
			}
			::Sleep(100);
		}
		return 0;		
	}
	static bool bAlreadyHome = false;
	short stx = LOWORD(lParam);
	short sty = HIWORD(lParam);
 //   char  x = (LOWORD(lParam)/255);
 //   char  y = (HIWORD(lParam) /255);
	int 	x = (LOWORD(lParam) >> 8);
	int     y = (HIWORD(lParam) >> 8);

	x = x - 127;
	y = y - 127;
	if ((x >= -PTZ_THRESHOLD) && (x <= PTZ_THRESHOLD)) x =0;
	if ((y >= -PTZ_THRESHOLD) && (y <= PTZ_THRESHOLD)) y =0;
	if (x > 127) x =127;
	if (y > 127) y =127;
//		TRACE(_T("x = %d; y = %d\r\n"),x,y);

	m_x = x; m_y =y;
	TRACE(_T("m_x = %d; m_y = %d\r\n"),m_x,m_y);
	int nSize =0;

	CONTROLDATA ctrl={0};
	ctrl.channel = 1;
	nSize = 7;

	if((UINT) (wParam & JOY_BUTTON1)) //ZOOM
	{

		ctrl.controlID = PCTL_PTZ_CONT;
		ctrl.payload.ptz_cont.zoom = m_y * -1;
		ctrl.payload.ptz_cont.tilt= 0;
		ctrl.payload.ptz_cont.pan= 0;

		// set CGI PTZAction

		if(m_y < 0)
			ptzaction = Z_I;
		else
			ptzaction = Z_O;

		vPTZMoveCGI(ptzaction, m_nPTZSpeed);

		return 0;

	}else if ((UINT) (wParam & JOY_BUTTON2)) // Focus
	{
		ctrl.controlID = PCTL_FOCUS_CONT;
		ctrl.payload.focus_cont = m_y;
		nSize = 3;
		int nSpeed;

		if(m_y < 0)
		{
			ptzaction = F_I;
			nSpeed = (m_y % 6) -1;
	
		}
		else
		{
			ptzaction = F_O;
			nSpeed = (m_y % 6) +1;
		}
		if(m_x == 0 && m_y == 0)
		{
			nSpeed = 0;
		}

		vPTZMoveCGI(ptzaction, nSpeed);
		vPTZMoveCGI(ptzaction, 0);


		return 0;
	}else
		// Normal move
	{				
		ctrl.controlID = PCTL_PTZ_CONT;
		ctrl.payload.ptz_cont.zoom = 0;
		ctrl.payload.ptz_cont.tilt= m_y * -1;
		ctrl.payload.ptz_cont.pan= m_x;
		if ( (m_y && m_x) == 0 )
			bAlreadyHome = true;
	}

	if (m_pNetsession )
	{
		ETROLOG(_T("-------------->m_x = %d, m_y =%d\n"),m_x, m_y);
		int nRet = m_pNetsession->sendControl(m_pNetsession, FCC_PCONTROL,(char*)&ctrl, nSize);
		//int nRet = m_pNetsession->sendControl(m_pNetsession, FCC_PCONTROL, (char*)&ctrl, 4 + sizeof(ptz));
		ETROLOG(_T("OnJoyStickMove return :%d\r\n"), nRet);
	}
    if (m_pNode)
	{	
		int nX = (m_x);
		int nY = (m_y);

		if (m_x == 0 && m_y == 0  && !m_bJoystickZMove) 
		{
			//TRACE(_T("m_x =m_y = 0; return;\r\n"));
			if(m_bCGISent )// send stop cgi only if sent move cgi before
			{
				vPTZMoveCGI(STOP,m_nPTZSpeed);
			}
			m_bCGISent = false;
			m_nPreInput = 0;
			return 0;
		}

		if(nX < 0 && nY == 0)// pan left
		{
			ptzaction = P_L; 			
			vJoyStickInputConvert(-nX);
		}
		if(nX > 0 && nY == 0) // pan right
		{
			ptzaction = P_R;
			vJoyStickInputConvert(nX);
		}
		if(nX == 0 && nY < 0) // tilt up. NOTICE: nY is up-side down
		{
			ptzaction = T_U;
			vJoyStickInputConvert(-nY);
		}
		if(nX == 0 &&  nY >0) //  tilt down
		{
			ptzaction = T_D;
			vJoyStickInputConvert(nY);
		}
		if(nX > 0 && nY < 0) // pan right tilt up
		{
			ptzaction = PR_TU;
			vJoyStickInputConvert(max(nX, (-nY)));
		}
		if (nX > 0 && nY > 0)// pan right tilt down
		{
			ptzaction = PR_TD;			
			vJoyStickInputConvert(max(nX, (nY)));
		}
		if (nX < 0 && nY > 0)// pan left tilt up
		{
			ptzaction = PL_TD;
		
			vJoyStickInputConvert(max((-nX), nY));
		}
		if (nX < 0 && nY < 0)// pan left tilt down
		{
			ptzaction = PL_TU;
			vJoyStickInputConvert(max((-nX), (-nY)));
		}
		 
		vPTZMoveCGI(ptzaction, m_nPTZSpeed);
		

	}
	//::Sleep(100);
	return 0;
}
LRESULT CEcPtz::OnJoyStickZMove(WPARAM wParam, LPARAM lParam)
{
	if ( !m_pNetsession && !m_pNode ) return 0;
	JOYINFOEX joyex;
	ZeroMemory(&joyex, sizeof(JOYINFOEX));
	joyex.dwSize = sizeof(JOYINFOEX);
	joyex.dwFlags = JOY_RETURNALL;
	joyGetPosEx(JOYSTICKID1, &joyex);
	int zPos = LOWORD(lParam); 

	int z = (LOWORD(lParam) >> 8);
	z = z - 127;
	if ((z >=  -PTZ_THRESHOLD) && (z <= PTZ_THRESHOLD)) z =0;
	if (z > 127) z =127;
	
	if (m_z == z) 
	{		
		return 0;
	}

	m_z = z;

	int nSize =0;
	CONTROLDATA ctrl={0};
	ctrl.channel = 1;
	
	// Set Focus parameters
	/*ctrl.controlID = PCTL_FOCUS_CONT;*/
	/*ctrl.payload.focus_cont = m_z;*/
	/*nSize = 5;*/
		ctrl.controlID = PCTL_PTZ_CONT;
		ctrl.payload.ptz_cont.zoom = (m_z)*-1;
		ctrl.payload.ptz_cont.tilt= 0;
		ctrl.payload.ptz_cont.pan= 0;
	
	nSize = 7;// set ptz must use 7
	int nRet;
	if (m_pNetsession )
	{
		nRet = m_pNetsession->sendControl(m_pNetsession, FCC_PCONTROL,(char*)&ctrl, nSize);
		/*Sleep(100);*/
		ETROLOG(_T("OnJoyStickZMove return :%d\r\n"), nRet);
	}

	//if (m_pNode)
	//{
	//	 PTZAction ptzaction ;
	//	if(m_z == 0)
	//	{
	//		vPTZMoveCGI(STOP,m_nPTZSpeed);
	//		m_nPreInput = 0;
	//		m_bJoystickZMove = false;
	//		return 0;
	//	}

	//	if(m_z < 0) // Zoom In
	//	{
	//		ptzaction = Z_I; 
	//		vJoyStickInputConvert(-m_z);
	//	}
	//	else if(m_z > 0) // Zoom Out
	//	{	
	//		ptzaction = Z_O;
	//		vJoyStickInputConvert(m_z);
	//	}
	//	TRACE(_T("m_z:%d\r\n"),m_z);
	//	vPTZMoveCGI(ptzaction, m_nPTZSpeed);
	//	m_bJoystickZMove = true;
	//}
	
	return 0;
}
void CEcPtz::vAttach(NODEITEM *pNode)
{
	//if(pNode == NULL)
	//	return;
	
	//m_pNode = new NODEITEM;
	//::ZeroMemory(m_pNode, sizeof(NODEITEM));

	//memcpy(m_pNode,pNode,sizeof(NODEITEM));

	m_pNode = pNode;

}

void CEcPtz::vPTZMoveCGI(PTZAction ptzaction, int nPTZSpeed)
{	
	
	if(m_pNode == NULL)
		return;	

	USES_CONVERSION;
	TCHAR tzIP[32];
	TCHAR tzUser[32];
	TCHAR tzPass[32];
	int nWebPort;
	wcscpy(tzIP,   A2W(m_pNode->ip));
	wcscpy(tzUser, A2W(m_pNode->szUser));
	wcscpy(tzPass, A2W(m_pNode->szPass));

	if(m_pNode->nWebPort == 0)
		nWebPort = 80;
	
	else nWebPort = m_pNode->nWebPort; 

	CString csIP, csUser, csPass, csCgiUrl,csChannel;

	csIP.Format(_T("%s"),tzIP);
	csUser.Format(_T("%s"),tzUser);
	csPass.Format(_T("%s"),tzPass);
	int nPSpeed= 0, nTSpeed= 0, nZSpeed= 0;
	bool bEtro = true;

	if(m_pNode->byDeviceType == ISAP)
	{
		bEtro = false;
		csChannel.Format(_T("%d"), m_pNode->mac[5]);
	}

	if(bEtro)
	{
		/*----set P,T,Z ----*/
		switch(ptzaction)
		{
		case 0:
			nPSpeed -= nPTZSpeed;
			break;
		case 1:
			nPSpeed += nPTZSpeed;
			break;
		case 2:
			nTSpeed += nPTZSpeed;
			break;
		case 3:
			nTSpeed -= nPTZSpeed;
			break;
		case 4:
			nZSpeed += nPTZSpeed;
			break;
		case 5:
			nZSpeed -= nPTZSpeed;
			break;
		case 6:
			nPSpeed -= nPTZSpeed;
			nTSpeed += nPTZSpeed;
			break;
		case 7:
			nPSpeed -= nPTZSpeed;
			nTSpeed -= nPTZSpeed;
			break;
		case 8:
			nPSpeed += nPTZSpeed;
			nTSpeed += nPTZSpeed;
			break;
		case 9:
			nPSpeed += nPTZSpeed;
			nTSpeed -= nPTZSpeed;
			break;
		case 10:
			nPSpeed=nTSpeed=nZSpeed=0;
			break;
		case 11:
		case 12://focus
			csCgiUrl.Format(_T("http://%s:%d/config/focus_cont.cgi?f=%d"),csIP,nWebPort,nPTZSpeed);
			mycurl.get(csCgiUrl,csUser,csPass);
			TRACE(_T("PTZSpeed:%d\r\n"),m_nPTZSpeed);
			TRACE(_T("%s\r\n"),csCgiUrl);
			m_bCGISent = true;
			return;			
		}

		/*---Set PTZ via CGI---*/
		csCgiUrl.Format(_T("http://%s:%d/config/ptz_move_cont.cgi?p=%d&t=%d&z=%d"),csIP,nWebPort,nPSpeed,nTSpeed,nZSpeed);
		mycurl.get(csCgiUrl,csUser,csPass);
		TRACE(_T("PTZSpeed:%d\r\n"),m_nPTZSpeed);
		TRACE(_T("%s\r\n"),csCgiUrl);
		m_bCGISent = true;
	}
	else // isap 
	{
		CString csCmd(_T(""));
		nPTZSpeed -= 1 ; // Speed range 0~5;	

		/*----set P,T,Z ----*/
		switch(ptzaction)
		{
		case 0:
			csCmd.Format(_T("/ptz cmdMoveLeft=%d,0"),nPTZSpeed);
			break;
		case 1:
			csCmd.Format(_T("/ptzcmdMoveRight=%d,0"),nPTZSpeed);
			break;
		case 2:
			csCmd.Format(_T("/ptzcmdMoveUp=0,%d"),nPTZSpeed);
			break;
		case 3:
			csCmd.Format(_T("/ptzcmdMoveDown=0,%d"),nPTZSpeed);
			break;
		case 4:
		case 5:
			break;
		case 6:
			csCmd.Format(_T("/ptzcmdMoveUpLeft=%d,%d"),nPTZSpeed,nPTZSpeed);
			break;
		case 7:
			csCmd.Format(_T("/ptzcmdMoveDownLeft=%d,%d"),nPTZSpeed,nPTZSpeed);
			break;
		case 8:
			csCmd.Format(_T("/ptzcmdMoveUpRight=%d,%d"),nPTZSpeed,nPTZSpeed);
			break;
		case 9:
			csCmd.Format(_T("/ptzcmdMoveDownRight=%d,%d"),nPTZSpeed,nPTZSpeed);
			break;
		case 10:
			//csCmd.Format(_T("/ptzcmdMoveStop"));
			break;
		}
	
		if(csCmd.IsEmpty()) return;

		/*---Set PTZ via CGI---*/
		csCgiUrl.Format(_T("http://%s:%d/airvideo/ptz?channel=channel%s"),csIP,nWebPort,csChannel);
		CString csConn;
		csConn.Format(_T("/airvideo/ptz?channel=channel%s"),csChannel);
		TRACE(_T("PTZ URL:%s\r\n"),csCgiUrl);
		//mycurl.post(csCgiUrl,csUser,csPass,csConn);
		mycurl.post(csCgiUrl,csUser,csPass,csCmd);	
		
	}
}
void CEcPtz::vPTZMoveVSMP(PTZAction ptzaction, int nPTZSpeed)
{
	CONTROLDATA ctrl={0};
	ctrl.channel = 1;
	ctrl.controlID = PCTL_PTZ_CONT;
	int nSize;
	nSize = 7 ;
	

	switch(ptzaction)
	{
	case 0:
		ctrl.payload.ptz_cont.zoom = 0;
		ctrl.payload.ptz_cont.tilt= 0;
		ctrl.payload.ptz_cont.pan= -(nPTZSpeed);
		break;
	case 1:
		ctrl.payload.ptz_cont.zoom = 0;
		ctrl.payload.ptz_cont.tilt= 0;
		ctrl.payload.ptz_cont.pan= nPTZSpeed;
		break;
	case 2:
		ctrl.payload.ptz_cont.zoom = 0;
		ctrl.payload.ptz_cont.tilt= nPTZSpeed;
		ctrl.payload.ptz_cont.pan= 0;
		break;
	case 3:
		ctrl.payload.ptz_cont.zoom = 0;
		ctrl.payload.ptz_cont.tilt= -(nPTZSpeed);
		ctrl.payload.ptz_cont.pan= 0;
		break;
	case 4:
		ctrl.payload.ptz_cont.zoom = (nPTZSpeed);;
		ctrl.payload.ptz_cont.tilt= 0;
		ctrl.payload.ptz_cont.pan= 0;
		break;
	case 5:
		ctrl.payload.ptz_cont.zoom = -(nPTZSpeed);
		ctrl.payload.ptz_cont.tilt= 0;
		ctrl.payload.ptz_cont.pan= 0;
		break;
    case 6:
		ctrl.payload.ptz_cont.zoom = 0;
		ctrl.payload.ptz_cont.tilt= (nPTZSpeed);
		ctrl.payload.ptz_cont.pan= -(nPTZSpeed);
		break;
	case 7:
		ctrl.payload.ptz_cont.zoom = 0;
		ctrl.payload.ptz_cont.tilt= -(nPTZSpeed);
		ctrl.payload.ptz_cont.pan= -(nPTZSpeed);
		break;
	case 8:
		ctrl.payload.ptz_cont.zoom = 0;
		ctrl.payload.ptz_cont.tilt= (nPTZSpeed);
		ctrl.payload.ptz_cont.pan= -(nPTZSpeed);
		break;
	case 9:
		ctrl.payload.ptz_cont.zoom = 0;
		ctrl.payload.ptz_cont.tilt= -(nPTZSpeed);
		ctrl.payload.ptz_cont.pan= (nPTZSpeed);
		break;
	case 10:
		ctrl.payload.ptz_cont.zoom = 0;
		ctrl.payload.ptz_cont.tilt= 0;
		ctrl.payload.ptz_cont.pan= 0;
		break;

	}

	
	int nRet = m_pNetsession->sendControl(m_pNetsession, FCC_PCONTROL,(char*)&ctrl, nSize);
				ETROLOG(_T("OnJoyStickMove return :%d\r\n"), nRet);



}

void CEcPtz::OnTabPTZButtonClicked(UINT nCmdID)
{
	
	if(m_pNode == NULL && m_pNetsession == NULL)
		return;

	// Query DB if PTZ is enabled
	if(!bCheckPTZEnable())
		return;	

	/*int nPTZSpeed = 6;*/
	
	// set PTZ function by CGI
	if(m_pNode)
	{	
		
	switch(nCmdID)
	{
		case IDB_PTZTOP:			
			vPTZMoveCGI(T_U,m_nPTZSpeed);
			break;

		case IDB_PTZBOT:
			vPTZMoveCGI(T_D,m_nPTZSpeed);
			break;

		case IDB_PTZRGT:
			vPTZMoveCGI(P_R,m_nPTZSpeed);
			break;

		case IDB_PTZLFT:
			vPTZMoveCGI(P_L,m_nPTZSpeed);
			break;

		case IDB_PTZTRGT:
			vPTZMoveCGI(PR_TU,m_nPTZSpeed);
			break;

		case IDB_PTZTLFT:
			vPTZMoveCGI(PL_TU,m_nPTZSpeed);
			break;

		case IDB_PTZBRGT:
			vPTZMoveCGI(PR_TD,m_nPTZSpeed);
			break;

		case IDB_PTZBLFT:
			vPTZMoveCGI(PL_TD,m_nPTZSpeed);
			break;
		case IDB_PTZHOME:		                 
			break;
	}
	
		
	// set PTZ function by VSMP
			
	}

	else if (m_pNetsession)
	{
		switch(nCmdID)
		{
		case IDB_PTZTOP:			
			vPTZMoveVSMP(T_U,m_nPTZSpeed);
			break;

		case IDB_PTZBOT:
			vPTZMoveVSMP(T_D,m_nPTZSpeed);
			break;

		case IDB_PTZRGT:
			vPTZMoveVSMP(P_R,m_nPTZSpeed);
			break;

		case IDB_PTZLFT:
			vPTZMoveVSMP(P_L,m_nPTZSpeed);
			break;

		case IDB_PTZTRGT:
			vPTZMoveVSMP(PR_TU,m_nPTZSpeed);
			break;

		case IDB_PTZTLFT:
			vPTZMoveVSMP(PL_TU,m_nPTZSpeed);
			break;

		case IDB_PTZBRGT:
			vPTZMoveVSMP(PR_TD,m_nPTZSpeed);
			break;

		case IDB_PTZBLFT:
			vPTZMoveVSMP(PL_TD,m_nPTZSpeed);
			break;
		case IDB_PTZHOME:                     
			break;
	}
	

	}

	//if(BST_UNCHECKED==((CButton*)GetDlgItem(nCmdID))->GetCheck())
	//	OnTabPTZButtonClickedStop();



}

void CEcPtz::vOnZoomScroll(PTZAction ptzaction,int nPTZSpeed)
{
	if(m_pNode == NULL && m_pNetsession == NULL)
		return;

	if(!bCheckPTZEnable())
		return ;

	if(m_pNode)
	vPTZMoveCGI(ptzaction,nPTZSpeed);
	
	else if(m_pNetsession)
		vPTZMoveVSMP(ptzaction,nPTZSpeed);
}
void CEcPtz::OnTabZoomButtonClicked(UINT nCmdID)
{
	// return if no video stream selected
	if(m_pNode == NULL && m_pNetsession == NULL)

		return;

	// check if PTZ enable

	if(!bCheckPTZEnable())
		return;
	
	
	/* m_nPTZSpeed = 6;*/

	if(m_pNode)
	{
	switch(nCmdID)
	{
		case IDB_MINUS:
			vPTZMoveCGI(Z_O,m_nPTZSpeed);
			break;

		case IDB_PLUS:
			vPTZMoveCGI(Z_I,m_nPTZSpeed);
			break;

	}
	}
	
	else if(m_pNetsession)
	{

		switch(nCmdID)
	{
		case IDB_MINUS:
			vPTZMoveVSMP(Z_O,m_nPTZSpeed);
			break;

		case IDB_PLUS:
			vPTZMoveVSMP(Z_I,m_nPTZSpeed);
			break;

	}


	}

}
void CEcPtz::OnTabPTZButtonClickedStop()
{
	// return if no video stream selected
	if(m_pNode == NULL && m_pNetsession == NULL)

		return;

	// check if PTZ enable

	if(!bCheckPTZEnable())
		return;
	

	/*int nPTZSpeed = 6;*/
	if(m_pNode)
	{
		vPTZMoveCGI(STOP, m_nPTZSpeed);

	}

	else if(m_pNetsession)
	{
		vPTZMoveVSMP(STOP,m_nPTZSpeed);
	}

}
void CEcPtz::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CSizingControlBarG::OnLButtonUp(nFlags, point);
	//AfxMessageBox(_T("hi"));
}

bool CEcPtz::bCheckPTZEnable()
{

	if(m_pNode == NULL && m_pNetsession == NULL)
		return false;

	CString csMac;

	if(m_pNode)
    csMac.Format(_T("%02X%02X%02X%02X%02X%02X"), m_pNode->mac[0], m_pNode->mac[1], m_pNode->mac[2], m_pNode->mac[3], m_pNode->mac[4], m_pNode->mac[5]);
	else if(m_pNetsession)
    csMac.Format(_T("%02X%02X%02X%02X%02X%02X"), m_mac[0], m_mac[1],  m_mac[2],  m_mac[3],  m_mac[4],  m_mac[5]);

	CString csPTZEnable;
	
	try
	{	
		
		CString csSQL(_T("SELECT ptz_support FROM ec_camera WHERE mac_address = '")+ csMac + _T("'"));		//Get Camera ID
		CRecordset rPTZ(pDB);
		rPTZ.Open(CRecordset::forwardOnly, csSQL);
		while (!rPTZ.IsEOF()) {
			rPTZ.GetFieldValue(_T("ptz_support"), csPTZEnable);
			rPTZ.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		//AfxMessageBox(p->m_strError);
		p->Delete();
	}

	
	if(csPTZEnable==_T("Y"))
		return true;
	else return false;

}


BOOL CEcPtz::PreTranslateMessage(MSG* pMsg)
{
    static bool bRunPtzfn(false);
	if(m_pNode == NULL && m_pNetsession == NULL)
		return CDockablePane::PreTranslateMessage(pMsg);

	if(pMsg->message == WM_LBUTTONDOWN)
	{
		//if(GetWindowLong(pMsg->hWnd,GWL_ID) == IDB_MINUS)
		UINT uiIDC = GetWindowLong(pMsg->hwnd,GWL_ID) ;
        bRunPtzfn = true;
		switch(uiIDC)
		{
		case IDB_MINUS:
		case IDB_PLUS:
			OnTabZoomButtonClicked(uiIDC);
			break;
		case IDB_PTZTOP:			
		case IDB_PTZBOT:
		case IDB_PTZRGT:
		case IDB_PTZLFT:			
		case IDB_PTZTRGT:
		case IDB_PTZTLFT:
		case IDB_PTZBRGT:
		case IDB_PTZBLFT:
            OnTabPTZButtonClicked(uiIDC);			
			break;
		case IDB_PTZHOME:		                 
        default:
            bRunPtzfn = false;
		}
	}else if(bRunPtzfn && (pMsg->message == WM_LBUTTONUP))
	{
        bRunPtzfn = false;
		OnTabPTZButtonClickedStop();
	}

	return CDockablePane::PreTranslateMessage(pMsg);
}

void CEcPtz::vGetMACAddress(NODEITEM *p_Node)
{
	if(p_Node == NULL)
	{
		/*m_IP.Empty();
		m_Port.Empty();
		m_CameraName.Empty();*/
		ZeroMemory(&m_AuthInfo,sizeof(PTZAuthInfo)) ;		
		return;
	}

	memcpy(&m_mac, p_Node->mac,sizeof(m_mac));
	USES_CONVERSION;
	wcscpy(m_AuthInfo.csUser, A2W(p_Node->szUser));
	wcscpy(m_AuthInfo.csPass , A2W(p_Node->szPass));
	wcscpy(m_AuthInfo.csIP,   A2W(p_Node->ip));
	wcscpy(m_AuthInfo.csCameraName,   A2W(p_Node->camera_name));
	m_AuthInfo.nPort = p_Node->nWebPort;	
}
	
void CEcPtz::OnCbnSelchangeComboPTZPreset(int nPreset)
{
	if(m_pNode == NULL && m_pNetsession == NULL)
		return ;

	if(!bCheckPTZEnable())
		return;

	if(m_pNode)
	{
		USES_CONVERSION;
		TCHAR tzIP[32];
		TCHAR tzUser[32];
		TCHAR tzPass[32];
		int nWebPort;
		wcscpy(tzIP,   A2W(m_pNode->ip));
		wcscpy(tzUser, A2W(m_pNode->szUser));
		wcscpy(tzPass, A2W(m_pNode->szPass));

		if(m_pNode->nWebPort == 0)
			nWebPort = 80;
		
		else nWebPort = m_pNode->nWebPort; 

		CString csIP, csUser, csPass, csCgiUrl;
		csIP.Format(_T("%s"),tzIP);
		csUser.Format(_T("%s"),tzUser);
		csPass.Format(_T("%s"),tzPass);

		/*---Set PTZ Preset via CGI---*/
		csCgiUrl.Format(_T("http://%s:%d/config/ptz_preset.cgi?name=%d&act=go"),csIP,nWebPort,nPreset);
		TRACE(csCgiUrl);
		mycurl.get(csCgiUrl,csUser,csPass);
	}

	else if (m_pNetsession)
	{
		int nSize =0;
		CONTROLDATA ctrl={0};
		ctrl.channel = 1;
		ctrl.controlID = PCTL_PRESET_MOVE;
		ctrl.payload.preset_move = (BYTE)nPreset;
		nSize = 5;			
		int nRet = m_pNetsession->sendControl(m_pNetsession, FCC_PCONTROL,(char*)&ctrl, nSize);
		ETROLOG(_T("OnPreset return :%d\r\n"), nRet);
	}
}

void CEcPtz::OnBnClickedPatrolBut()
{
	if(m_pNode == NULL && m_pNetsession == NULL)
		return ;

	if(!bCheckPTZEnable())
		return;	

	if(!CGS::bGetPatrolSeqDuration(m_vctmapPatrolSeqDurSets))
	{
		if(m_vctmapPatrolSeqDurSets.size() == 0)
		{
			AfxMessageBox(_T("There is no patrol point sequence setting, please set it"));
			OnBnClickedPTZSettingBut();
			return;
		}
	}

	m_bPatrolEnable ^=1 ;

	m_bPatrolEnable == true && m_vctmapPatrolSeqDurSets.size() != 0 ? m_PtzTab.pPatrol->SetColor(RGB(220,0,0)) :  m_PtzTab.pPatrol->SetColor(RGB(154,181,210));

	if(!m_hPatrolThread)
	{
		UINT nRet(0);
		m_hPatrolThread = (HANDLE) _beginthreadex(NULL,0,thdPatrol,(LPVOID)this,0,&nRet);
	}
}
UINT CEcPtz::thdPatrol(LPVOID lpParam)
{
	CEcPtz* pThis = (CEcPtz*) lpParam;

	while(pThis->m_bPatrolEnable)
	{
		if( pThis->m_nPatrolSeqDurIdx < pThis->m_vctmapPatrolSeqDurSets.size())
		{
			map <int,pair_INT> tmp;
			map <int,pair_INT>::iterator it;
			tmp = pThis->m_vctmapPatrolSeqDurSets[pThis->m_nPatrolSeqDurIdx];
			for(it =  tmp.begin(); it !=  tmp.end();it++)
			{
				if(it->first !=0)
				{
					pThis->OnCbnSelchangeComboPTZPreset(it->second.first);
				}
				::Sleep((it->second.second) *1000); 
			}
		}
		else
		{
			pThis->m_bPatrolEnable = false;
			break;
		}
	}

	CLOSEHANDLE(pThis->m_hPatrolThread);

	return 0;
}

void CEcPtz::OnBnClickedPTZSettingBut()
{
	if(m_pNode == NULL && m_pNetsession == NULL)
		return ;

	if(!bCheckPTZEnable())
		return;

	CDlgPTZSetting dlgPTZ;
	PTZSettingInfo ptzInfo;
	// init some variables in struct
	ptzInfo.pvctmapPatrolSeqDurSet = & m_vctmapPatrolSeqDurSets;
	//dlgPTZ.m_nPatrolSeqDurIdx=m_pNode->nPatrolSeqDurIdx;
	dlgPTZ.SetPtzInfo(&ptzInfo);
	dlgPTZ.GetPtzSpeed(m_nPTZSpeed);

	if(m_pNode)
	{
		dlgPTZ.vGetNODEITEM(m_pNode);
	}

	else if(m_pNetsession)// && m_IP!=_T("")&& m_Port !=_T(""))
	{
		dlgPTZ.vGetNODEITEM(m_AuthInfo);			
	}

	if (dlgPTZ.DoModal() == IDOK )
	{			
		if(m_pNode)
		{
			USES_CONVERSION;
			TCHAR tzIP[32];
			TCHAR tzUser[32];
			TCHAR tzPass[32];
			int nWebPort;
			wcscpy(tzIP,   A2W(m_pNode->ip));
			wcscpy(tzUser, A2W(m_pNode->szUser));
			wcscpy(tzPass, A2W(m_pNode->szPass));

			if(m_pNode->nWebPort == 0)
				nWebPort = 80;
			
			else nWebPort = m_pNode->nWebPort; 

			CString csIP, csUser, csPass, csCgiUrl;
			csIP.Format(_T("%s"),tzIP);
			csUser.Format(_T("%s"),tzUser);
			csPass.Format(_T("%s"),tzPass);

			//rs485 settings
			CString csEnable;
			if(ptzInfo.csProtocol == _T("Disable"))
				  csEnable = _T("no");
			else  csEnable = _T("yes");		
			
			/*if(ptzInfo.vctnPatrolPoint.size()!=0)
			{
				for(int i=0; i<ptzInfo.vctnPatrolPoint.size();i++)
				{
					m_vctnPatrolPoints.push_back(ptzInfo.vctnPatrolPoint[i]);
				}
			}*/

			/*---Set rs485  via CGI---*/
			csCgiUrl.Format(_T("http://%s:%d/config/rs485.cgi?enable=%s&proto=%s&devid=%s&baudrate=%s&databits=%s&parity=%s&stopbits=%s")
							,csIP,nWebPort
							,csEnable
							,ptzInfo.csProtocol
							,ptzInfo.csDeviceID
							,ptzInfo.csBraudRate
							,ptzInfo.csDataBits
							,ptzInfo.csParity
							,ptzInfo.csStopBits);

			mycurl.get(csCgiUrl,csUser,csPass);
			m_nPTZSpeed = _ttoi(ptzInfo.csPTZSpeed);		
		}
		// not using netsession now
		//if(m_pNetsession)
		//{
		//	//rs485 settings
		//	CString csEnable;
		//	if(ptzInfo.csProtocol == _T("Disable"))
		//		  csEnable = _T("no");
		//	else  csEnable = _T("yes");
		//
		//if(ptzInfo.bPresetPointSet == true && ptzInfo.nPresetPoint !=0 )// set preset point
		//{
		//	vSetPresetPoint(ptzInfo.nPresetPoint);
		//}
		//
		//if(ptzInfo.vctnPatrolPoint.size()!=0)
		//{
		//	for(int i=0; i<ptzInfo.vctnPatrolPoint.size();i++)
		//	{
		//		m_vctnPatrolPoints.push_back(ptzInfo.vctnPatrolPoint[i]);
		//	}
		//}
		//	/*---Set rs485  via CGI---*/
		//	CString csCgiUrl;
		//	csCgiUrl.Format(_T("http://%s:%d/config/rs485.cgi?enable=%s&proto=%s&devid=%s&baudrate=%s&databits=%s&parity=%s&stopbits=%s")
		//					,m_AuthInfo.csIP,m_AuthInfo.nPort
		//					,csEnable
		//					,ptzInfo.csProtocol
		//					,ptzInfo.csDeviceID
		//					,ptzInfo.csBraudRate
		//					,ptzInfo.csDataBits
		//					,ptzInfo.csParity
		//					,ptzInfo.csStopBits);

		//	mycurl.get(csCgiUrl,m_AuthInfo.csUser,m_AuthInfo.csPass);
		//	m_nPTZSpeed = _ttoi(ptzInfo.csPTZSpeed);
		//}
	}
	else return;



//	if (AfxMessageBox(_T("Do you really want to set PTZ?"), MB_YESNO) == IDNO)
//		return;

}
void CEcPtz::vJoyStickInputConvert(int nX)
{
	// nX range: 1~127; 0 should not call this function
	if(m_nPreInput == 0)
	{
		m_nPTZSpeed = 1;
		m_nPreInput = nX;
	}

	else
	{
		if(nX >= m_nPreInput)
		{
			m_nPTZSpeed++;
			m_nPreInput = nX;
		}
		else
		{
			m_nPTZSpeed--;
			m_nPreInput = nX;
		}
	}
	if(m_nPTZSpeed < 1)
	{
		m_nPTZSpeed = 1;
	}
	if(m_nPTZSpeed >6)
	{
		m_nPTZSpeed = 6;
	}
}
void CEcPtz::vSetPresetPoint(int nPreset)
{
	if(m_pNode == NULL && m_pNetsession == NULL)
		return ;

	if(!bCheckPTZEnable())
		return;

	if(m_pNode)
	{
		USES_CONVERSION;
		TCHAR tzIP[32];
		TCHAR tzUser[32];
		TCHAR tzPass[32];
		int nWebPort;
		wcscpy(tzIP,   A2W(m_pNode->ip));
		wcscpy(tzUser, A2W(m_pNode->szUser));
		wcscpy(tzPass, A2W(m_pNode->szPass));

		if(m_pNode->nWebPort == 0)
			nWebPort = 80;
		
		else nWebPort = m_pNode->nWebPort; 

		CString csIP, csUser, csPass, csCgiUrl;
		csIP.Format(_T("%s"),tzIP);
		csUser.Format(_T("%s"),tzUser);
		csPass.Format(_T("%s"),tzPass);

		/*---Set PTZ Preset via CGI---*/
		csCgiUrl.Format(_T("http://%s:%d/config/ptz_preset.cgi?name=%d&act=add"),csIP,nWebPort,nPreset);
		mycurl.get(csCgiUrl,csUser,csPass);

	}

	else if (m_pNetsession)
	{
		int nSize =0;
		CONTROLDATA ctrl={0};
		ctrl.channel = 1;
		ctrl.controlID = PCTL_PRESET_MOVE;
		ctrl.payload.preset_set = (BYTE)nPreset;
		nSize = 5;			
		int nRet = m_pNetsession->sendControl(m_pNetsession, FCC_PCONTROL,(char*)&ctrl, nSize);
		ETROLOG(_T("OnPreset return :%d\r\n"), nRet);
	}
	
}



int CEcPtz::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP ;
	 
	CRect rect= CRect(0,0,400,400);
	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
	if (pDB == NULL)
	{
		//AfxMessageBox(IDS_PTZ_DB_ERR);
		//AfxMessageBox(_T("CEcPtz - OnCreate: Can't get proper Database from MainFrame"));
	}

	m_PtzTab.Create(TCS_TABS | TCS_FIXEDWIDTH | WS_CHILD | WS_VISIBLE, rect, this, TABID);
	bGetJoyStickInfo();

	return 0;
}
