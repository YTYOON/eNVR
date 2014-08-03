// DlgPTZ.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "DlgPTZ.h"
#include "NVSNetLib.h"
#include "MMSystem.h"
#include "MainFrm.h"

#define  TIMER_ReSTOP									1
#define  TIMER_CheckConnect						2
#define	TIMER_WaitCGI_AUTOFOCUS			3			//for cgi CGAUTOFOCUS stop 1s//
#define	TIMER_WaitCGI_MOVECONT			4			//for cgi CGI_MOVECONT stop 250 ms//
#define	TIMER_WaitCGI_FOCUSCONT		5			//for cgi CGI_FOCUSCONT stop 150 ms//
#define	TIMER_WaitCGI_GET_PATROLNUM	6		//for cgi wait 250ms send CGI_GET_PATROLNUM//
#define	TIMER_WaitCGI_SET_PATROLNUM	7		//for cgi wait 250ms send CGI_SET_PATROLNUM//

#define  TIMER_JoyStick			8



#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#define _CRT_SECURE_NO_DEPRECATE
#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#include <windows.h>
#include <commctrl.h>
#include <basetsd.h>
#include <dinput.h>
#include <dinputd.h>
#include <assert.h>
#include <oleauto.h>
#include <shellapi.h>

#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include "resource.h"




//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
//INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK    EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
BOOL CALLBACK    EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
HRESULT InitDirectInput( HWND hDlg );
VOID FreeDirectInput();
//HRESULT UpdateInputState();


// Stuff to filter out XInput devices
#include <wbemidl.h>
HRESULT SetupForIsXInputDevice();
bool IsXInputDevice( const GUID* pGuidProductFromDirectInput );
void CleanupForIsXInputDevice();


struct XINPUT_DEVICE_NODE
{
	DWORD dwVidPid;
	XINPUT_DEVICE_NODE* pNext;
};

struct DI_ENUM_CONTEXT
{
	DIJOYCONFIG* pPreferredJoyCfg;
	bool bPreferredJoyCfgValid;
};

bool                    g_bFilterOutXinputDevices = false;
XINPUT_DEVICE_NODE*     g_pXInputDeviceList = NULL;




//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

LPDIRECTINPUT8          g_pDI = NULL;
LPDIRECTINPUTDEVICE8    g_pJoystick = NULL;

HWND m_hDlgJoyDebug=NULL;		//Show the recive value



// CDlgPTZ dialog

IMPLEMENT_DYNAMIC(CDlgPTZ, CDialog)

CDlgPTZ::CDlgPTZ(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgPTZ::IDD, pParent),m_pNetsession(NULL),m_pNode(NULL),m_nPTZSpeed(6),m_bCGISent(false), m_nPreInput(0),m_bJoystickZMove(false) //,m_bPatrolEnable(false)
	, m_hPTZCGIThread(NULL), m_ptzCGIType(PTZCGITYPE::PTZ), m_nSliderPos(0),m_nJoyStickSpeed(6),m_pPTZGetInfo(NULL)			//,m_hPatrolThread(NULL), 
	,m_pvk(NULL),m_pDlgJoyDebug(NULL)
{
    m_clrBk = BKCOLOR;
	m_clrFont = FONTCOLOR;

    pDB = NULL;
	m_bPTZAutoScan=0;
	m_ptzAction = P_R;

	//m_pszGreyDib=0;
	m_pMsgIdxStatus=0;
	m_bCanRunCGI=0;
	m_bInitvAttach=0;

	m_nCheckConnect=5;
	m_nCheckAutoType=0;
	m_bGetPatrolNun=0;
	m_bJoystickMove=0;

	m_bLiveViewAutoScan=0;

	m_nOldZoomPos=0;
	m_nOldFocusPos=0;
	m_bWaitCGI_FOCUSCONT=0;
	m_dwtmJoyStickFOCUSC=0;

	m_OldPTZaction=PTZAction::PTZINIT;
	m_nOldJoyStickSpeed=-99;
	//m_bReSendFocus=0;
	m_bJoystickFocus=0;
	m_bJoystickZoom=0;
}

CDlgPTZ::~CDlgPTZ()
{
	DELETEOBJ(m_pPTZGetInfo);
}

void CDlgPTZ::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPTZ)
	DDX_Control(pDX, IDC_STATIC_SLIDER_ZOOM, m_sliderZoom);
	DDX_Control(pDX, IDC_STATIC_SLIDER_FOCUS, m_sliderFocus);
	DDX_Control(pDX, IDC_STATIC_TOGGLE_PARTOL, m_togglePartol);
	DDX_Control(pDX, IDC_STATIC_TOGGLE_AUTO_SCAN, m_toggleAutoScan);
	DDX_Control(pDX, IDC_STATIC_TOGGLE_AUTOFOCUS, m_toggleAutoFocus);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_PTZ_PRESET, m_editPTZPreset);
	DDX_Control(pDX, IDC_BTN_PTZ_PRSET_SET, m_btnPTZPresetSet);
	DDX_Control(pDX, IDC_BTN_PTZ_PRSET_GO, m_btnPTZPresetGo);
}

BEGIN_MESSAGE_MAP(CDlgPTZ, CBaseDialog)
    ON_MESSAGE(WM_BUTTONDISK_PRESS,  OnBitmapPtzPress)
	 ON_MESSAGE(WM_BUTTONDISK_RELEASE,  OnBitmapPtzRelease)
    ON_MESSAGE(WM_BITMAPSLIDER_MOVED,  OnBitmapSliderMoved)
	ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnBitmapSliderMoving)
	ON_BN_CLICKED(IDC_BTN_PTZ_PRSET_SET, &CDlgPTZ::OnBnClickedBtnPtzPrsetSet)
	ON_BN_CLICKED(IDC_BTN_PTZ_PRSET_GO, &CDlgPTZ::OnBnClickedBtnPtzPrsetGo)
	ON_WM_CTLCOLOR()	
	ON_BN_CLICKED(IDC_BTN_PTZ_SETTING, &CDlgPTZ::OnBnClickedPTZSettingBut)
	ON_MESSAGE(MM_JOY1MOVE, OnJoyStickMove)
	ON_MESSAGE(MM_JOY1ZMOVE, OnJoyStickZMove)
	ON_WM_TIMER()
	ON_REGISTERED_MESSAGE(MsgUpdateView, OnUpdateView)
	ON_EN_SETFOCUS(IDC_EDIT_PTZ_PRESET, &CDlgPTZ::OnEnSetfocusEditPtzPreset)
	ON_EN_KILLFOCUS(IDC_EDIT_PTZ_PRESET, &CDlgPTZ::OnEnKillfocusEditPtzPreset)
END_MESSAGE_MAP()


// CDlgPTZ message handlers

BOOL CDlgPTZ::OnInitDialog()
{
	m_hPTZGetInfoThread=0;

    BOOL bRet(false);
    CBaseDialog::OnInitDialog();
    bCreateControler();
	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
	
	if	(!_UseDirectInput)
		bGetJoyStickInfo();

	m_pPTZGetInfo = new CPTZGetInfo;
	m_pPTZGetInfo->Init(this,UM_PTZGetInfo);
	
	//CGS::bGetPatrolSeqDuration(m_vctmapPatrolSeqDurSets);
    return bRet;  // return TRUE unless you set the focus to a control
}
BOOL CDlgPTZ::bCreateControler()
{
    BOOL bRet(false);
	DWORD dwStyle =  WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|SS_NOTIFY ;   
    CRect rect;
    GetClientRect(&rect);
    rect.left = rect.top = 50;
    rect.right = rect.left+109; rect.bottom = rect.top +108;
    m_btnDisk.Create(NULL, dwStyle, rect, this, IDC_BUTTON1);
    //m_btnDisk.LoadBitmaps(IDB_BITMAP_DISK, IDB_BMP_DISK, IDB_BMP_DISK, IDB_BMP_DISK);
    
	m_sliderFocus.SetBitmapChannel( IDB_BITMAP_SLIDER_BG, IDB_BITMAP_SLIDER_BG );
	m_sliderFocus.SetBitmapThumb( IDB_BITMAP_SLIDER_THUMB, IDB_BITMAP_SLIDER_THUMB_PRESS, TRUE );
	m_sliderFocus.DrawFocusRect( FALSE );
	m_sliderFocus.SetRange( -6, 6 );
	m_togglePartol.SetStyle(CBitmapSlider::BOOMERANG);

 	m_sliderZoom.SetBitmapChannel( IDB_BITMAP_SLIDER_BG, IDB_BITMAP_SLIDER_BG );
	m_sliderZoom.SetBitmapThumb( IDB_BITMAP_SLIDER_THUMB, IDB_BITMAP_SLIDER_THUMB_PRESS, TRUE );
	m_sliderZoom.DrawFocusRect( FALSE );
	m_sliderZoom.SetRange( -6, 6 );
	m_togglePartol.SetStyle(CBitmapSlider::BOOMERANG);

 	m_togglePartol.SetBitmapChannel( IDB_BITMAP_TOGGLE_OFF, IDB_BITMAP_TOGGLE_ON );
	m_togglePartol.SetBitmapThumb( IDB_BITMAP_TOGGLE_THUMB, IDB_BITMAP_TOGGLE_THUMB, TRUE );
	m_togglePartol.DrawFocusRect( FALSE );
    m_togglePartol.SetStyle(CBitmapSlider::TOGGLE);
	m_togglePartol.SetRange(-6,6);
	m_togglePartol.SetPos(-6);

 	m_toggleAutoScan.SetBitmapChannel( IDB_BITMAP_TOGGLE_OFF, IDB_BITMAP_TOGGLE_ON );
	m_toggleAutoScan.SetBitmapThumb( IDB_BITMAP_TOGGLE_THUMB, IDB_BITMAP_TOGGLE_THUMB, TRUE );
    m_toggleAutoScan.SetStyle(CBitmapSlider::TOGGLE);
	m_toggleAutoScan.SetRange(-6,6);
	m_toggleAutoScan.SetPos(-6);

 	m_toggleAutoFocus.SetBitmapChannel( IDB_BITMAP_TOGGLE_OFF, IDB_BITMAP_TOGGLE_ON );
	m_toggleAutoFocus.SetBitmapThumb( IDB_BITMAP_TOGGLE_THUMB, IDB_BITMAP_TOGGLE_THUMB, TRUE );
    m_toggleAutoFocus.SetStyle(CBitmapSlider::TOGGLE);
	m_toggleAutoFocus.SetRange(-6,6);
	m_toggleAutoFocus.SetPos(-6);
    bRet = true;
    return bRet;
}



//=============================================================

void CDlgPTZ::vSetNetSession(CNetSession* pNetsession)
{
	//this->m_PtzTab.vSetNetSession(pNetsession);
	if(!pNetsession) return;
	m_pNetsession = pNetsession;
}
 
BOOL CDlgPTZ::bGetJoyStickInfo()
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
LRESULT CDlgPTZ::OnJoyStickMove(WPARAM wParam, LPARAM lParam)
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
	//TRACE(_T("m_x = %d; m_y = %d\r\n"),m_x,m_y);
	int nSize =0;

	CONTROLDATA ctrl={0};
	ctrl.channel = 1;
	nSize = 7;

	if((UINT) (wParam & JOY_BUTTON1)) //ZOOM
	{

		ctrl.controlID = PCTL_PTZ_CONT;
		ctrl.payload.ptz_cont.zoom = m_x * -1;
		ctrl.payload.ptz_cont.tilt= 0;
		ctrl.payload.ptz_cont.pan= 0;
		int nSpeed=0;

		// set CGI PTZAction
		m_ptzCGIType = PTZCGITYPE::ZOOM;
		if(m_x < 0 || m_y < 0)
		{
			ptzaction = Z_I;
			//nSpeed = (m_y % 6) -1;
			vJoyStickInputConvert(max((-m_x), (-m_y)));			
		}
		else if (m_x > 0 || m_y > 0)
		{
			ptzaction = Z_O;
			//nSpeed = (m_y % 6) +1;
			vJoyStickInputConvert(max(m_x, (m_y)));
		}

		if ((m_nJoyStickSpeed == m_nOldJoyStickSpeed && m_OldPTZaction == ptzaction) || ptzaction < 0)
		{
			return 0;
		}

		//vPTZMoveCGI(ptzaction, m_nPTZSpeed);		
		m_ptzAction=ptzaction;
		m_nPTZSpeed=m_nJoyStickSpeed;
		m_bJoystickMove=true;
		if(!m_hPTZCGIThread && !(m_x == 0 && m_y == 0))
		{
			UINT nRet(0);
			m_hPTZCGIThread = (HANDLE) _beginthreadex(NULL,0,thdPTZCGI,(LPVOID)this,0,&nRet);
		}
		m_OldPTZaction=ptzaction;
		m_nOldJoyStickSpeed=m_nJoyStickSpeed;

		if (ptzaction==Z_I)
		{
			TRACE(_T("\r\nJoy_Z_I : %d\r\n"),m_nJoyStickSpeed);
		}
		else if (ptzaction==Z_O)
		{
			TRACE(_T("\r\nJoy_Z_O : %d\r\n"),m_nJoyStickSpeed);
		}		
		return 0;
	}else if ((UINT) (wParam & JOY_BUTTON2)) // Focus
	{
		ctrl.controlID = PCTL_FOCUS_CONT;
		ctrl.payload.focus_cont = m_y;
		nSize = 3;
		int nSpeed=0;

		m_ptzCGIType = PTZCGITYPE::FOCUS;
		if (m_pNode->bPTZAutoFocus==1)
		{
			m_toggleAutoFocus.SetPos(-6);
			OnBnClickedAutoFocusBut(0);
			SetTimer(TIMER_WaitCGI_AUTOFOCUS,1000,0);
			return 0;
		}

		if(m_x < 0 || m_y < 0)
		{
			ptzaction = F_I;
			//nSpeed = (m_y % 6) -1;
			vJoyStickInputConvert(max((-m_x), (-m_y)));			
		}
		else if (m_x > 0 || m_y > 0)
		{
			ptzaction = F_O;
			//nSpeed = (m_y % 6) +1;
			vJoyStickInputConvert(max(m_x, (m_y)));
		}

		if ((m_nJoyStickSpeed == m_nOldJoyStickSpeed && m_OldPTZaction == ptzaction) || ptzaction < 0)
		{
			return 0;
		}
	
		//vPTZMoveCGI(ptzaction, nSpeed);
		//vPTZMoveCGI(ptzaction, 0);
		m_ptzAction=ptzaction;
		m_nPTZSpeed=m_nJoyStickSpeed;
		m_bJoystickMove=true;
		if(!m_hPTZCGIThread && !(m_x == 0 && m_y == 0))
		{
			UINT nRet(0);
			m_hPTZCGIThread = (HANDLE) _beginthreadex(NULL,0,thdPTZCGI,(LPVOID)this,0,&nRet);
		}
		m_OldPTZaction=ptzaction;
		m_nOldJoyStickSpeed=m_nJoyStickSpeed;

		if (ptzaction == F_I)
		{
			TRACE(_T("\r\nJoy_F_I : %d\r\n"),m_nJoyStickSpeed);
		}
		else if (F_O)
		{
			TRACE(_T("\r\nJoy_F_O : %d\r\n"),m_nJoyStickSpeed);
		}	
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
			
		if (m_x == 0 && m_y == 0 )	//&& !m_bJoystickZMove) 
		{
			//TRACE(_T("m_x =m_y = 0; return;\r\n"));
			if(m_bCGISent )// send stop cgi only if sent move cgi before
			{
				m_ptzCGIType = PTZCGITYPE::PTZ;
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

		if ((m_nJoyStickSpeed == m_nOldJoyStickSpeed && m_OldPTZaction == ptzaction) || ptzaction < 0)
		{
			return 0;
		}
		
		//vPTZMoveCGI(ptzaction, m_nJoyStickSpeed);
		//TRACE(_T("\r\nm_nPreInput =%d\r\n"),m_nPreInput);

		m_ptzAction=ptzaction;
		m_nPTZSpeed=m_nJoyStickSpeed;
		m_bJoystickMove=true;
		if(!m_hPTZCGIThread)
		{
			UINT nRet(0);
			m_hPTZCGIThread = (HANDLE) _beginthreadex(NULL,0,thdPTZCGI,(LPVOID)this,0,&nRet);
		}
		m_OldPTZaction=ptzaction;
		m_nOldJoyStickSpeed=m_nJoyStickSpeed;
	}
	//::Sleep(100);
	
	return 0;
}
LRESULT CDlgPTZ::OnJoyStickZMove(WPARAM wParam, LPARAM lParam)
{
	return 0;

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
//void CDlgPTZ::vAttach(NODEITEM *pNode, BYTE* pszGreyDib)
void CDlgPTZ::vAttach(NODEITEM *pNode, WORD* pMsgIdx, CVideoThin* pVideoThin)
{
	KillTimer(TIMER_CheckConnect);
	m_pVideoThin = pVideoThin;
	
	m_bCanRunCGI=0;
	m_bInitvAttach=0;
	m_bGetPatrolNun=0;
		
	m_togglePartol.SetPos(-6);
	m_toggleAutoScan.SetPos(-6);
	m_toggleAutoFocus.SetPos(-6);


	//if(pNode == NULL)
	//	return;
	
	//m_pNode = new NODEITEM;
	//::ZeroMemory(m_pNode, sizeof(NODEITEM));

	m_pNode = pNode;	
	if(m_pNode == NULL)
		return;

	BOOL bPTZ=bCheckPTZEnable();
	if (bPTZ==0)
	{
		return;
	}

	if (m_bLiveViewAutoScan)
		return;

	m_pMsgIdxStatus=pMsgIdx;
	if (*m_pMsgIdxStatus != 3)
	{
		//disconnected//
		m_nCheckConnect=5;
		SetTimer(TIMER_CheckConnect,1000,0);		//wait the Video connect for 5 sec//
		return;
	}
	
	/*if (m_pNode->nPatrolSeqDurIdx>0)
	{
		m_nPatrolSeqDurIdx=m_pNode->nPatrolSeqDurIdx-1;
		if (m_vctmapPatrolSeqDurSets.size()>0)
		{
			if (m_nPatrolSeqDurIdx<m_vctmapPatrolSeqDurSets.size())
			{
				if (m_vctmapPatrolSeqDurSets[m_nPatrolSeqDurIdx].size()==0)
				{
					m_pNode->nPatrolSeqDurIdx=0;
					m_nPatrolSeqDurIdx=-1;
				}
			}
			else
			{
				m_pNode->nPatrolSeqDurIdx=0;
				m_nPatrolSeqDurIdx=-1;
			}
		}
		else
		{
			m_pNode->nPatrolSeqDurIdx=0;
			m_nPatrolSeqDurIdx=-1;
		}
	}
	else
	{
		m_pNode->nPatrolSeqDurIdx=0;
		m_nPatrolSeqDurIdx=-1;
	}*/

	//if (m_pNode->nPTZAutorunType)	
	//{
	//	ResetPatrolStatus();
	//}

	if (m_pNode->hPatrolThread == NULL)
	{
		CGS::bGetPatrolSeqDuration(m_mapPatrolSeqDurSets,m_pNode->mac);
		if (m_pVideoThin)
		{
			m_pVideoThin->m_mapPatrolSeqDurSets=m_mapPatrolSeqDurSets;
		}
	}
	else
	{
		m_mapPatrolSeqDurSets=m_pVideoThin->m_mapPatrolSeqDurSets;
	}

	if (m_pNode->bSupportPTZ == 0)
	{
		if(m_pNode->nPTZAutorunType == 1)
			m_togglePartol.SetPos(6);
		else if (m_pNode->nPTZAutorunType == 2)
			m_toggleAutoScan.SetPos(6);
	}

	m_bCanRunCGI=1;
	m_bInitvAttach=1;
	m_pPTZGetInfo->vQueryCgi(CGIType::CGI_GETINFO,m_pNode);

	/*BOOL bOK=0;
	CString csIP, csUser, csPass, csCgiUrl;
	int nWebPort;
	if(m_pNode->nWebPort == 0)
		nWebPort = 80;	
	else nWebPort = m_pNode->nWebPort;*/

	
	
		//if(!m_hPTZGetInfoThread)
		//{
		//	UINT nRet(0);
		//	m_hPTZGetInfoThread = (HANDLE) _beginthreadex(NULL,0,thdPTZGetInfo,(LPVOID)this,0,&nRet);
		//}
	
	
	

	//if (_UseNewPTZ && bPTZ)
	//{
	//	USES_CONVERSION;

	//	TCHAR tzIP[32];
	//	TCHAR tzUser[32];
	//	TCHAR tzPass[32];
	//	
	//	wcscpy(tzIP,   A2W(m_pNode->ip));
	//	wcscpy(tzUser, A2W(m_pNode->szUser));
	//	wcscpy(tzPass, A2W(m_pNode->szPass));				

	//	csIP.Format(_T("%s"),tzIP);
	//	csUser.Format(_T("%s"),tzUser);
	//	csPass.Format(_T("%s"),tzPass);

	//	csCgiUrl.Format(_T("http://%s:%d/config/ptz_autorun.cgi"),csIP,nWebPort);

	//	CString csStatus=mycurl.get(csCgiUrl,csUser,csPass);

	//	for (int nFind=0;nFind<3;nFind++)
	//	{
	//		int nFF=-1;
	//		if (!csStatus.IsEmpty() && csStatus.Find(L"404")>-1)
	//		{
	//			break;
	//		}

	//		if (nFind==0)
	//		{
	//			nFF=csStatus.Find(L"none");
	//		}
	//		else if (nFind==1)
	//		{
	//			nFF=csStatus.Find(L"patrol");
	//		}
	//		else if (nFind==2)
	//		{
	//			nFF=csStatus.Find(L"scan");
	//		}

	//		if(nFF>-1)
	//			bOK=1;

	//		if (bOK)
	//		{
	//			m_pNode->nPTZAutorunType=nFind;
	//			break;
	//		}
	//	}
	//}
	//else
	//{
	//	int a=0;
	//}
	//

	//if (bOK)
	//{
	//	m_pNode->bSupportPTZ=1;
	//	csCgiUrl.Format(_T("http://%s:%d/config/ptz_autofocus.cgi"),csIP,nWebPort);
	//	CString csStatus=mycurl.get(csCgiUrl,csUser,csPass);
	//	if (csStatus.Find(L"Error")==-1)
	//	{
	//		int nFindFocus=csStatus.Find(L"\":\"");
	//		if (nFindFocus>-1)
	//		{
	//			int nMid=csStatus.GetLength()-nFindFocus-7;
	//			csStatus=csStatus.Mid(nFindFocus+3,nMid);
	//			if (csStatus==L"yes")
	//			{
	//				m_pNode->bPTZAutoFocus=1;
	//			}
	//			else
	//			{
	//				m_pNode->bPTZAutoFocus=0;
	//			}
	//		}
	//	}

	//}
	//else
	//{
	//	m_pNode->bSupportPTZ=0;
	//	m_pNode->bPTZAutoFocus=0;		//for old model default 0//
	//}

	//if (m_pNode->nPTZAutorunType==0)
	//{
	//	m_bPatrolEnable=0;
	//	m_bPTZAutoScan=0;
	//	m_togglePartol.SetPos(-6);
	//	m_toggleAutoScan.SetPos(-6);
	//}
	//else if (m_pNode->nPTZAutorunType==1)
	//{
	//	m_bPatrolEnable=1;
	//	m_bPTZAutoScan=0;
	//	m_togglePartol.SetPos(6);
	//	m_toggleAutoScan.SetPos(-6);

	//	OnBnClickedPatrolBut(1);
	//}
	//else if (m_pNode->nPTZAutorunType==2)
	//{
	//	m_bPatrolEnable=0;
	//	m_bPTZAutoScan=1;
	//	m_togglePartol.SetPos(-6);
	//	m_toggleAutoScan.SetPos(6);

	//	OnBnClickedAutoScanBut(1);
	//}

	//m_toggleAutoFocus.EnableWindow(m_pNode->bSupportPTZ);

	//if (m_pNode->bPTZAutoFocus)
	//{
	//	m_toggleAutoFocus.SetPos(6);
	//}
	//else
	//{
	//	m_toggleAutoFocus.SetPos(-6);
	//}
}

void CDlgPTZ::vPTZMoveCGI(PTZAction ptzaction, int nPTZSpeed)
{		
	if(m_pNode == NULL)
		return;	

	if(!bCheckPTZEnable())
		return ;

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

	//BOOL bStop=0;

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
			SetTimer(TIMER_WaitCGI_MOVECONT,250,0);
			m_nOldZoomPos=0; 
			return;

			//bStop=1;
			break;
		case 11:
		case 12://focus
			if (m_bCanRunCGI)
			{
				if (m_nOldFocusPos == nPTZSpeed)
					return;

				m_pPTZGetInfo->vQueryCgi(CGIType::CGI_FOCUSCONT,m_pNode,nPTZSpeed);

				m_bCGISent = true;
				m_nOldFocusPos=m_nSliderPos;

				//csCgiUrl.Format(_T("http://%s:%d/config/focus_cont.cgi?f=%d"),csIP,nWebPort,nPTZSpeed);
				//mycurl.get(csCgiUrl,csUser,csPass);
				//TRACE(_T("PTZSpeed:%d\r\n"),nPTZSpeed);
				//TRACE(_T("%s\r\n"),csCgiUrl);
				//m_bCGISent = true;
			}			
			return;			
		}

		/*---Set PTZ via CGI---*/
		if (m_bCanRunCGI)
		{
			if (m_pNode->bSupportPTZ == 0)
			{
				if (m_pNode->hPatrolThread || m_pNode->hAutoScanThread)
				{
					//To stop thread//
					m_pNode->nPTZAutorunType=0;
					CLOSEHANDLE(m_pNode->hPatrolThread);
					CLOSEHANDLE(m_pNode->hAutoScanThread);

					m_togglePartol.SetPos(-6);
					m_toggleAutoScan.SetPos(-6);
				}
			}
				
			//PTZ	  CGI_MOVECONT//
			if (m_ptzCGIType == PTZCGITYPE::ZOOM && m_nOldZoomPos == nZSpeed)
				return;

			m_pPTZGetInfo->vQueryCgi(CGIType::CGI_MOVECONT,m_pNode, nPSpeed,nTSpeed,nZSpeed);
			m_bCGISent = true;
			if (m_ptzCGIType == PTZCGITYPE::ZOOM)
			{
				m_nOldZoomPos=nZSpeed;
			}
			
			
			//csCgiUrl.Format(_T("http://%s:%d/config/ptz_move_cont.cgi?p=%d&t=%d&z=%d"),csIP,nWebPort,nPSpeed,nTSpeed,nZSpeed);
			//mycurl.get(csCgiUrl,csUser,csPass);
		}

		/*CString csError=mycurl.get(csCgiUrl,csUser,csPass);
		if (bStop)
		{
			BOOL bCheckStop=0;
			int nFindFinish=csError.GetLength();
			int nFindTime=csError.Find(L"p=");
			if (nFindFinish > nFindTime && nFindTime>-1 && nFindFinish>0)
			{
				nFindTime=csError.Find(L"p=",nFindTime+1);
				if (nFindTime>-1)
				{
					bCheckStop=1;
				}
				else
				{
					if (csError.Find(L"p=0")==-1)
					{
						bCheckStop=1;
					}					
				}
			}

			if (bCheckStop)
			{
				SetTimer(TIMER_ReSTOP,50,0);
			}
		}
		*/

		TRACE(_T("PTZSpeed:%d\r\n"),nPTZSpeed);
		TRACE(_T("%s\r\n"),csCgiUrl);
		//m_bCGISent = true;
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
void CDlgPTZ::vPTZMoveVSMP(PTZAction ptzaction, int nPTZSpeed)
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

bool CDlgPTZ::bCheckPTZEnable()
{
	if(m_pNode == NULL && m_pNetsession == NULL)
		return false;

	//CString csMac;
	//if(m_pNode)
 //   csMac.Format(_T("%02X%02X%02X%02X%02X%02X"), m_pNode->mac[0], m_pNode->mac[1], m_pNode->mac[2], m_pNode->mac[3], m_pNode->mac[4], m_pNode->mac[5]);
	//else if(m_pNetsession)
 //   csMac.Format(_T("%02X%02X%02X%02X%02X%02X"), m_mac[0], m_mac[1],  m_mac[2],  m_mac[3],  m_mac[4],  m_mac[5]);

	//CString csPTZEnable;
	//
	//try
	//{	
	//	//CString csSQL(_T("SELECT ptz_support FROM ec_camera WHERE mac_address = '")+ csMac + _T("'"));		//Get Camera ID
	//	CString csSQL;
	//	csSQL.Format(_T("SELECT ptz_support FROM ec_camera WHERE camera_idx = '%d'"), m_pNode->camera_idx);
	//	CRecordset rPTZ(pDB);
	//	rPTZ.Open(CRecordset::forwardOnly, csSQL);
	//	while (!rPTZ.IsEOF()) {
	//		rPTZ.GetFieldValue(_T("ptz_support"), csPTZEnable);
	//		rPTZ.MoveNext();
	//	}
	//}
	//catch(CDBException * p)
	//{
	//	//AfxMessageBox(p->m_strError);
	//	TRACE(p->m_strError);
	//	p->Delete();
	//}

	//
	//if(csPTZEnable==_T("Y"))
	
	//identify node.bSupportPTZ  by lynn
	return m_pNode->bSupportPTZ;
}


BOOL CDlgPTZ::PreTranslateMessage(MSG* pMsg)
{
	static bool bRunPtzfn(false);
	if(m_pNode == NULL && m_pNetsession == NULL)
		return CBaseDialog::PreTranslateMessage(pMsg);

	if(pMsg->message == WM_LBUTTONDOWN)
	{
		//if(GetWindowLong(pMsg->hWnd,GWL_ID) == IDB_MINUS)
		UINT uiIDC = GetWindowLong(pMsg->hwnd,GWL_ID) ;
        bRunPtzfn = true;
		switch(uiIDC)
		{
        
		case IDC_STATIC_SLIDER_ZOOM:
		case IDC_STATIC_SLIDER_FOCUS:
			break;
		default:
            bRunPtzfn = false;
			break;
		}
	}
	else if(bRunPtzfn && (pMsg->message == WM_LBUTTONUP))
	{
        bRunPtzfn = false;
		//vPTZMoveCGI(STOP, m_nPTZSpeed);
		if (m_bCanRunCGI && m_ptzCGIType == PTZCGITYPE::FOCUS)		//&& m_pPTZGetInfo->m_nCgiType == CGIType::CGI_FOCUSCONT)
		{
			KillTimer(TIMER_WaitCGI_AUTOFOCUS);
			m_bWaitCGI_FOCUSCONT=0;
			//m_pPTZGetInfo->vQueryCgi(CGIType::CGI_FOCUSCONT,m_pNode,0);
			SetTimer(TIMER_WaitCGI_FOCUSCONT,150,0);
		}
		else if (m_bCanRunCGI && m_ptzCGIType == PTZCGITYPE::ZOOM)		//m_pPTZGetInfo->m_nCgiType == CGIType::CGI_MOVECONT)
		{
			//Set Stop Z_I , Z_O//
			m_pPTZGetInfo->m_bStopCGI=1;
			vPTZMoveCGI(STOP, m_nPTZSpeed);
			//m_pPTZGetInfo->vQueryCgi(CGIType::CGI_MOVECONT,m_pNode, 0, 0, 0);
		}
	}
	else if(pMsg->message == WM_KEYDOWN)
	{
		int nKey=(int)pMsg->wParam;
		if (nKey==VK_ESCAPE)
		{
			return TRUE;
		}
		else if (nKey==VK_RETURN)
		{
			OnBnClickedBtnPtzPrsetGo();
			return TRUE;
		}
	}
	else if (pMsg->message==UM_PTZGetInfo)
	{///////////////Larry add 102-11-22/////////
			if (pMsg->wParam==PTZGI_Init)
			{
				if (pMsg->lParam==0)
				{
					AfxMessageBox(L"Cannot use cgi to query carmera.");
				}
			}
			if (pMsg->wParam==PTZGI_OK)
			{
				int a=0;
			}
			if (pMsg->wParam==PTZGI_FAIL)
			{
				//Over 5 sec no Response//

				//delete (m_pPTZGetInfo);

				//m_pPTZGetInfo = new CPTZGetInfo;
				//m_pPTZGetInfo->Init(this,UM_PTZGetInfo);
				//AfxMessageBox(L"the network is busy...");
				int a=0;
			}
			else if (pMsg->wParam==PTZGI_vCGI)
			{
				//return vCGI Active//
				if (m_pPTZGetInfo->m_pNode!=m_pNode)
				{
					return false;
				}
				//if (m_pPTZGetInfo->m_bActiven)
				//{
				//		CloseHandle(m_pPTZGetInfo->m_hThread);
				//		delete(m_pPTZGetInfo);
				//}

				if (pMsg->lParam==CGI_GETINFO)
				{
					BOOL bOK=0;
					for (int nFind=0;nFind<3;nFind++)
					{
						int nFF=-1;
						if (!m_pPTZGetInfo->m_csCGIStatus.IsEmpty() && m_pPTZGetInfo->m_csCGIStatus.Find(L"404")>-1)
						{
							break;
						}

						if (nFind==0)
						{
							nFF=m_pPTZGetInfo->m_csCGIStatus.Find(L"none");
						}
						else if (nFind==1)
						{
							nFF=m_pPTZGetInfo->m_csCGIStatus.Find(L"patrol");
						}
						else if (nFind==2)
						{
							nFF=m_pPTZGetInfo->m_csCGIStatus.Find(L"scan");
						}

						if(nFF>-1)
							bOK=1;

						if (bOK)
						{
							m_pNode->nPTZAutorunType=nFind;
							break;
						}
					}

					if (bOK)
					{
						//m_pNode->bSupportPTZ=1;

						//m_pPTZGetInfo->m_csCGIStatus=L"{\"name\":\"patrol\",\"patrol_info\":\"01\",\"preset_info\":\"01\"}";
						//m_pNode->nPTZAutorunType=1;

						int nPatrolNum=m_pPTZGetInfo->m_csCGIStatus.Find(L"preset_info");
						if (nPatrolNum > -1)
						{
							CString csPatrolNum=m_pPTZGetInfo->m_csCGIStatus.Mid(nPatrolNum+14,2);
							//m_pNode->nPatrolNum=_wtoi(csPatrolNum);
						}
						m_pNode->nPatrolNum=1;		//Default:1//


						if (m_bCanRunCGI==1)
						{
							m_pPTZGetInfo->vQueryCgi(CGIType::CGI_GET_AUTOFOCUS,m_pNode);
						}
					}
					else
					{
						//m_pNode->bSupportPTZ=0;
						m_pNode->bPTZAutoFocus=0;		//for old model default 0//
						
						if (m_pNode->nPTZAutorunType == 1)
						{
							m_togglePartol.SetPos(6);
							m_toggleAutoScan.SetPos(-6);
							OnBnClickedPatrolBut(1);
						}
						else if (m_pNode->nPTZAutorunType == 2)
						{
							m_togglePartol.SetPos(-6);
							m_toggleAutoScan.SetPos(6);
							OnBnClickedAutoScanBut(1);
						}
					}

					if (_UseNewPTZ)
					{
						m_bGetPatrolNun=0;
						SetTimer(TIMER_WaitCGI_GET_PATROLNUM,250,0);
					}					
				}
				else if (pMsg->lParam==CGI_GET_AUTOFOCUS)
				{
					CString csStatus=m_pPTZGetInfo->m_csCGIStatus;
					if (csStatus.Find(L"Error")==-1)
					{
						int nFindFocus=csStatus.Find(L"\":\"");
						if (nFindFocus>-1)
						{
							int nMid=csStatus.GetLength()-nFindFocus-7;
							csStatus=csStatus.Mid(nFindFocus+3,nMid);
							if (csStatus==L"yes")
							{
								m_pNode->bPTZAutoFocus=1;
							}
							else
							{
								m_pNode->bPTZAutoFocus=0;
							}
						}
					}

					if (m_pNode->nPTZAutorunType==0)
					{
						//m_bPatrolEnable=0;
						m_bPTZAutoScan=0;
						m_togglePartol.SetPos(-6);
						m_toggleAutoScan.SetPos(-6);
					}
					else if (m_pNode->nPTZAutorunType==1)
					{
						//m_bPatrolEnable=1;
						m_bPTZAutoScan=0;
						m_togglePartol.SetPos(6);
						m_toggleAutoScan.SetPos(-6);

						OnBnClickedPatrolBut(1);
					}
					else if (m_pNode->nPTZAutorunType==2)
					{
						//m_bPatrolEnable=0;
						m_bPTZAutoScan=1;
						m_togglePartol.SetPos(-6);
						m_toggleAutoScan.SetPos(6);

						OnBnClickedAutoScanBut(1);
					}

					m_toggleAutoFocus.EnableWindow(m_pNode->bSupportPTZ);

					if (m_pNode->bPTZAutoFocus)
					{
						m_toggleAutoFocus.SetPos(6);
					}
					else
					{
						m_toggleAutoFocus.SetPos(-6);
					}
				}
				else if (pMsg->lParam==CGI_FOCUSCONT)
				{
					m_bCGISent = true;
				}
				else if (pMsg->lParam==CGI_MOVECONT)
				{
					//m_bCGISent = true;
				}
				else if (pMsg->lParam==CGI_GO_PRESET)
				{
					int a=0;
				}
				else if (pMsg->lParam==CGI_AUTOPATROL || pMsg->lParam==CGI_AUTOSCAN || pMsg->lParam==CGI_SET_AUTOFOCUS
					)	//|| pMsg->lParam & CGI_CHECKSTOP )
				{
						vCheckCGIStatus((int)pMsg->lParam);
				}
				else if (pMsg->lParam == CGI_GET_PATROLNUM)
				{
					vReSetPTZPatrolSetting(m_pPTZGetInfo->m_csCGIStatus);
				}
				else if (pMsg->lParam == CGI_GET_PTZSPEED)
				{
					int nFind=m_pPTZGetInfo->m_csCGIStatus.Find(L"\"}");
					if (!m_pPTZGetInfo->m_csCGIStatus.IsEmpty() && nFind>-1)
					{ 						
						CString csSpeed;
						csSpeed=m_pPTZGetInfo->m_csCGIStatus.Mid(nFind-2, 2);
						if (csSpeed.Left(1) == L"\"")
						{
							csSpeed=m_pPTZGetInfo->m_csCGIStatus.Mid(nFind-1, 1);
						}
						
						int nPTZSpeed=_wtoi(csSpeed);
						if (nPTZSpeed <=1)
						{
							 m_nPTZSpeed=1;
						}
						else if (1 < nPTZSpeed && nPTZSpeed <= 5)
						{
							m_nPTZSpeed=2;
						}
						else if (5 < nPTZSpeed && nPTZSpeed <= 10)
						{
							m_nPTZSpeed=3;
						}
						else if (10 < nPTZSpeed && nPTZSpeed <= 20)
						{
							m_nPTZSpeed=4;
						}
						else if (20 < nPTZSpeed && nPTZSpeed <= 40)
						{
							m_nPTZSpeed=5;
						}
						else if (40 < nPTZSpeed && nPTZSpeed <= 60)
						{
							m_nPTZSpeed=6;
						}
					}
					else
					{
						m_nPTZSpeed=6;			//Default//
					}

					m_bGetPatrolNun=1;
				}
			}
		}
		/////////////////////////////////////////////
	return CBaseDialog::PreTranslateMessage(pMsg);
}

void CDlgPTZ::vGetMACAddress(NODEITEM *p_Node)
{
	if(p_Node == NULL)
	{		
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
	
void CDlgPTZ::vGoToPresetPoint(int nPreset)
{
	if(m_pNode == NULL && m_pNetsession == NULL)
		return ;

	if(!bCheckPTZEnable())
		return;

	if(m_pNode && m_bCanRunCGI==1)
	{
	/*	USES_CONVERSION;
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
		csPass.Format(_T("%s"),tzPass);*/

		/*---Set PTZ Preset via CGI---*/
		//csCgiUrl.Format(_T("http://%s:%d/config/ptz_preset.cgi?name=%d&act=go"),csIP,nWebPort,nPreset);
		//TRACE(csCgiUrl);
		//mycurl.get(csCgiUrl,csUser,csPass);

		m_pPTZGetInfo->vQueryCgi(CGIType::CGI_GO_PRESET, m_pNode, nPreset);
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

void CDlgPTZ::OnBnClickedPatrolBut(bool bIsOn)
{
	if(m_pNode == NULL && m_pNetsession == NULL)
		return ;

	if(!bCheckPTZEnable())
		return;

	if (m_bCanRunCGI==0)
	{
		return;
	}

	if (m_pNode->nPTZAutorunType == 2)
	{
		m_bPTZAutoScan= 0;
		OnBnClickedAutoScanBut(m_bPTZAutoScan);
		m_toggleAutoScan.SetPos(-6);
	}

	if (bIsOn)
		m_pNode->nPTZAutorunType=1;
	else
		m_pNode->nPTZAutorunType=0;



	if (_UseNewPTZ && m_pNode->bSupportPTZ)
	{
		/*USES_CONVERSION;
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
		csPass.Format(_T("%s"),tzPass);*/

		/*---Set PTZ Preset via CGI---*/
		if (bIsOn)
		{
			//csCgiUrl.Format(_T("http://%s:%d/config/ptz_autorun.cgi?name=patrol"),csIP,nWebPort);
			m_pNode->nPTZAutorunType=1;
		}
		else
		{
			//csCgiUrl.Format(_T("http://%s:%d/config/ptz_autorun.cgi?name=none"),csIP,nWebPort);
			m_pNode->nPTZAutorunType=0;
		}

		//TRACE(csCgiUrl);
		//mycurl.get(csCgiUrl,csUser,csPass);
		m_pPTZGetInfo->vQueryCgi(CGIType::CGI_AUTOPATROL, m_pNode, (int)bIsOn);
	}
	else
	{
		if(m_pNode->nPTZAutorunType == 1)
		{
			//if(!CGS::bGetPatrolSeqDuration(m_vctmapPatrolSeqDurSets))
			//{
				/*BOOL bCheckNull=0;
				if (m_vctmapPatrolSeqDurSets.size()>0 && m_nPatrolSeqDurIdx<=m_vctmapPatrolSeqDurSets.size())
				{
					bCheckNull=m_vctmapPatrolSeqDurSets[m_nPatrolSeqDurIdx].size();
				}
				if(bCheckNull==0)*/
				if (m_mapPatrolSeqDurSets.size()==0)
				{
					//m_bPatrolEnable=0;
					m_togglePartol.SetPos(-6);
					AfxMessageBox(_T("There is no patrol point sequence setting, please set it"));
					OnBnClickedPTZSettingBut();
					return;
				}
			//}
				
				if(!m_pNode->hPatrolThread)
				{
					//UINT nRet(0);
					//m_pNode->hPatrolThread = (HANDLE) _beginthreadex(NULL,0,CVideoThin::thdPatrol,(LPVOID)pVideo,0,&nRet);
					m_pVideoThin->vbeginPatrolThread();
				}
				m_togglePartol.SetPos(6);
		}
		else
		{
			m_togglePartol.SetPos(-6);
			CLOSEHANDLE(m_pNode->hPatrolThread);
		}
	}
}

void CDlgPTZ::OnBnClickedAutoScanBut(bool bIsOn)
{
	if(m_pNode == NULL && m_pNetsession == NULL)
		return ;

	if(!bCheckPTZEnable())
		return;

	if (m_bCanRunCGI==0)
	{
		return;
	}
	
	if (m_pNode->nPTZAutorunType == 1)
	{
		//m_bPatrolEnable= 0;
		OnBnClickedPatrolBut(0);
		m_togglePartol.SetPos(-6);
	}

	if (bIsOn)
		m_pNode->nPTZAutorunType=2;
	else
		m_pNode->nPTZAutorunType=0;

	if(m_pNode->bSupportPTZ)
	{
		/*USES_CONVERSION;
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
		csPass.Format(_T("%s"),tzPass);*/

		/*---Set ptz_scan.cgi---*/

		if (bIsOn)
		{
			//csCgiUrl.Format(_T("http://%s:%d/config/ptz_scan.cgi?type=circle&speed=%d&act=show"),csIP,nWebPort,m_nPTZSpeed);
			m_pNode->nPTZAutorunType=2;
		}
		else
		{
			//csCgiUrl.Format(_T("http://%s:%d/config/ptz_scan.cgi"),csIP,nWebPort);
			m_pNode->nPTZAutorunType=0;
		}

		m_pPTZGetInfo->vQueryCgi(CGIType::CGI_AUTOSCAN, m_pNode, m_nPTZSpeed, (int)bIsOn);

		//CString csError=mycurl.get(csCgiUrl,csUser,csPass);
		
		
		//if (!csError.IsEmpty() && csError.Find(L"404")>-1)
		//{
		//	AfxMessageBox(L"This IP camera is not supported.");
		//	m_toggleAutoScan.SetPos(-6);
		//	m_bPTZAutoScan=0;
		//}

	}
	else
	{
		if (bIsOn)
		{
			//m_ptzAction = P_R;
			m_pNode->nPTZAutorunType=2;
			m_toggleAutoScan.SetPos(6);
			
			if(!m_pNode->hAutoScanThread)
			{
				//UINT nRet(0);
				//m_hPTZCGIThread = (HANDLE) _beginthreadex(NULL,0,thdPTZCGI,(LPVOID)this,0,&nRet);
				m_pVideoThin->vbeginAutoScanThread(P_R, m_nPTZSpeed);
			}
		}
		else
		{
			//vPTZMoveCGI(STOP,m_nPTZSpeed);
			m_pVideoThin->vbeginAutoScanThread(STOP);
			m_toggleAutoScan.SetPos(-6);
			m_pNode->nPTZAutorunType=0;
		}
	}

	//else if (m_pNetsession)
	//{
	//	int nSize =0;
	//	CONTROLDATA ctrl={0};
	//	ctrl.channel = 1;
	//	ctrl.controlID = PCTL_PRESET_MOVE;
	//	//ctrl.payload.preset_set = (BYTE)nPreset;
	//	nSize = 5;			
	//	int nRet = m_pNetsession->sendControl(m_pNetsession, FCC_PCONTROL,(char*)&ctrl, nSize);
	//	ETROLOG(_T("OnPreset return :%d\r\n"), nRet);
	//}

}

void CDlgPTZ::OnBnClickedAutoFocusBut(bool bIsOn)
{
	if(m_pNode == NULL && m_pNetsession == NULL)
		return ;

	if(!bCheckPTZEnable())
		return;

	if (m_bCanRunCGI==0)
	{
		return;
	}

	if(m_pNode->bSupportPTZ)
	{
		/*USES_CONVERSION;
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
		csPass.Format(_T("%s"),tzPass);*/

		/*---Set ptz_autofocus.cgi---*/

		if (bIsOn)
		{
			//csCgiUrl.Format(_T("http://%s:%d/config/ptz_autofocus.cgi?autofocus=yes"),csIP,nWebPort,m_nPTZSpeed);
			m_pNode->bPTZAutoFocus=1;
		}
		else
		{
			//csCgiUrl.Format(_T("http://%s:%d/config/ptz_autofocus.cgi?autofocus=no"),csIP,nWebPort);
			m_pNode->bPTZAutoFocus=0;
		}

		m_pPTZGetInfo->vQueryCgi(CGIType::CGI_SET_AUTOFOCUS, m_pNode, (int)bIsOn);
		//CString csError=mycurl.get(csCgiUrl,csUser,csPass);

		//if (!csError.IsEmpty() && csError.Find(L"404")>-1)
		//{
		//	AfxMessageBox(L"This IP camera is not supported.");
		//	m_toggleAutoScan.SetPos(-6);
		//	m_bPTZAutoScan=0;
		//}

	}
	else
	{
		m_pNode->bPTZAutoFocus=bIsOn;
	}

	//else if (m_pNetsession)
	//{
	//	int nSize =0;
	//	CONTROLDATA ctrl={0};
	//	ctrl.channel = 1;
	//	ctrl.controlID = PCTL_PRESET_MOVE;
	//	//ctrl.payload.preset_set = (BYTE)nPreset;
	//	nSize = 5;			
	//	int nRet = m_pNetsession->sendControl(m_pNetsession, FCC_PCONTROL,(char*)&ctrl, nSize);
	//	ETROLOG(_T("OnPreset return :%d\r\n"), nRet);
	//}
}


UINT CDlgPTZ::thdPTZGetInfo(LPVOID lpParam)
{
	CDlgPTZ* pThis = (CDlgPTZ*) lpParam;

	USES_CONVERSION;
	BOOL bOK=0;
	
	CString csIP,csUser,csPass,csCgiUrl,csCGIStatus;
	Curlplus mycurl;

	int nWebPort;
	if(pThis->m_pNode->nWebPort == 0)
		nWebPort = 80;	
	else nWebPort = pThis->m_pNode->nWebPort;

	TCHAR tzIP[32];
	TCHAR tzUser[32];
	TCHAR tzPass[32];

	wcscpy(tzIP,   A2W(pThis->m_pNode->ip));
	wcscpy(tzUser, A2W(pThis->m_pNode->szUser));
	wcscpy(tzPass, A2W(pThis->m_pNode->szPass));				

	csIP.Format(_T("%s"),tzIP);
	csUser.Format(_T("%s"),tzUser);
	csPass.Format(_T("%s"),tzPass);

	TCHAR tzPrefix[16]={};
	if (pThis->m_pNode->bChannel)
	{
		_stprintf(tzPrefix, _T("//%02X%02X%02X%02X%02X%02X"), pThis->m_pNode->mac[0] , pThis->m_pNode->mac[1], pThis->m_pNode->mac[2], pThis->m_pNode->mac[3], pThis->m_pNode->mac[4], pThis->m_pNode->mac[5]);    
	}

	if (pThis->m_nCheckAutoType == CGIType::CGI_AUTOPATROL)
	{		
		csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_autorun.cgi"), csIP, nWebPort, tzPrefix);
		csCGIStatus=mycurl.get(csCgiUrl,csUser,csPass);
		if (csCGIStatus.Find(L"patrol")==-1)
		{
			//pThis->m_bPatrolEnable=0;
			pThis->m_pNode->nPTZAutorunType=0;
			pThis->m_bPTZAutoScan=0;
			pThis->m_togglePartol.SetPos(-6);
			//pThis->m_toggleAutoScan.SetPos(-6);
		}
	}	
	else if(pThis->m_nCheckAutoType == CGIType::CGI_AUTOSCAN)
	{
		csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_autorun.cgi"), csIP, nWebPort, tzPrefix);
		csCGIStatus=mycurl.get(csCgiUrl,csUser,csPass);
		if (csCGIStatus.Find(L"scan")==-1)
		{
			pThis->m_bPTZAutoScan=0;
			pThis->m_pNode->nPTZAutorunType=0;
			//pThis->m_bPatrolEnable=0;
			//pThis->m_togglePartol.SetPos(-6);
			pThis->m_toggleAutoScan.SetPos(-6);
		}
	}
	else	if (pThis->m_nCheckAutoType == CGIType::CGI_SET_AUTOFOCUS)
	{
		csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_autofocus.cgi"), csIP, nWebPort, tzPrefix);
		csCGIStatus=mycurl.get(csCgiUrl,csUser,csPass);
		BOOL bOK=0;			
		if (csCGIStatus.Find(_T("autofocus\":\"yes"))>-1)
			bOK=1;

		pThis->m_pNode->bPTZAutoFocus=bOK;
		if (pThis->m_pNode->bPTZAutoFocus)
		{
			pThis->m_toggleAutoFocus.SetPos(6);
		}
		else
		{
			pThis->m_toggleAutoFocus.SetPos(-6);
		}
	}
	//else if (pThis->m_nCheckAutoType & CGIType::CGI_CHECKSTOP)
	//{
	//	if (pThis->m_nCheckAutoType & CGI_FOCUSCONT)
	//	{//F_I F_O//
	//		int a=0;
	//	}
	//	else if (pThis->m_nCheckAutoType & CGI_MOVECONT)
	//	{//Z_I Z_O//
	//		m_csCgiUrl.Format(_T("http://%s:%d/config/ptz_move_cont.cgi?p=0&t=0&z=0"),m_csIP,nWebPort);
	//		mycurl.get(csCgiUrl,csUser,csPass);
	//	}
	//}
	CLOSEHANDLE(pThis->m_hPTZGetInfoThread);
	return 0;
}

//UINT CDlgPTZ::thdPatrol(LPVOID lpParam)
//{
//	CDlgPTZ* pThis = (CDlgPTZ*) lpParam;
//
//	while(pThis->m_pNode->nPTZAutorunType == 1)
//	{
//		if(pThis->m_mapPatrolSeqDurSets.size()>0)	 //pThis->m_nPatrolSeqDurIdx <pThis->m_vctmapPatrolSeqDurSets.size()
//		{
//			//map <int,pair_INT> tmp;
//			map <int,pair_INT>::iterator it;
//			//tmp = pThis->m_vctmapPatrolSeqDurSets[pThis->m_nPatrolSeqDurIdx];
//
//			for(it =  pThis->m_mapPatrolSeqDurSets.begin(); it != pThis->m_mapPatrolSeqDurSets.end();it++)
//			{
//				if(it->first !=0 && pThis->m_pNode->nPTZAutorunType == 1)
//				{
//					pThis->vGoToPresetPoint(it->second.first);
//				}
//				else
//				{
//					break;
//				}
//				//::Sleep((it->second) *1000); 
//				int nSleepCount=it->second.second*10;
//				while(nSleepCount>0)
//				{
//					if (pThis->m_pNode->nPTZAutorunType != 1)
//					{
//						break;
//					}
//					::Sleep(100);
//					nSleepCount--;
//				}
//
//			}
//		}
//		else
//		{
//			//pThis->m_bPatrolEnable = false;
//			break;
//		}
//	}
//
//	CLOSEHANDLE(pThis->m_pNode->hPatrolThread);
//
//	return 0;
//}


void CDlgPTZ::OnBnClickedPTZSettingBut()
{
	if(m_pNode == NULL && m_pNetsession == NULL)
		return ;

	if(!bCheckPTZEnable())
		return;

	if (m_bCanRunCGI==0)
	{
		return;
	}
	else if (m_pNode->bSupportPTZ ==1 && m_bGetPatrolNun == 0 && _UseNewPTZ)
	{
		//Getting ptz_patrol_info.cgi for waiting a minute...// m_bGetPatrolNun==1 continue
		return;
	}

	CDlgPTZSetting dlgPTZ;
	PTZSettingInfo ptzInfo;
	// init some variables in struct
	//ptzInfo.pvctmapPatrolSeqDurSet =& m_vctmapPatrolSeqDurSets;
	//dlgPTZ.m_nPatrolSeqDurIdx = m_nPatrolSeqDurIdx;	//m_nPatrolSeqDurIdx=-1 =>New//
	//if (m_vctmapPatrolSeqDurSets.size()>m_nPatrolSeqDurIdx && m_vctmapPatrolSeqDurSets.size()>0)
	//{
	//	dlgPTZ.m_mapPatrolSeqDuration = m_vctmapPatrolSeqDurSets[m_nPatrolSeqDurIdx];
	//}	

	dlgPTZ.SetPtzInfo(&ptzInfo);
	dlgPTZ.GetPtzSpeed(m_nPTZSpeed);

	dlgPTZ.m_mapPatrolSeqDuration=m_mapPatrolSeqDurSets;

	if(m_pNode)
	{
		dlgPTZ.vGetNODEITEM(m_pNode);
	}

	else if(m_pNetsession)// && m_IP!=_T("")&& m_Port !=_T(""))
	{
		dlgPTZ.vGetNODEITEM(m_AuthInfo);			
	}
	
	//CGS::bGetPatrolSeqDuration(*ptzInfo.pvctmapPatrolSeqDurSet);
	//return;
	{
		VK vk;
		if (dlgPTZ.DoModal() == IDOK )
		{			
			if(m_pNode)
			{
				/*USES_CONVERSION;
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
				csPass.Format(_T("%s"),tzPass);*/

				////rs485 settings
				//CString csEnable;
				//if(ptzInfo.csProtocol == _T("Disable"))
				//	  csEnable = _T("no");
				//else  csEnable = _T("yes");		

				///*---Set rs485  via CGI---*/
				//csCgiUrl.Format(_T("http://%s:%d/config/rs485.cgi?enable=%s&proto=%s&devid=%s&baudrate=%s&databits=%s&parity=%s&stopbits=%s")
				//				,csIP,nWebPort
				//				,csEnable
				//				,ptzInfo.csProtocol
				//				,ptzInfo.csDeviceID
				//				,ptzInfo.csBraudRate
				//				,ptzInfo.csDataBits
				//				,ptzInfo.csParity
				//				,ptzInfo.csStopBits);

				//mycurl.get(csCgiUrl,csUser,csPass);
				m_nPTZSpeed = _ttoi(ptzInfo.csPTZSpeed);

				m_pPTZGetInfo->vQueryCgi(CGIType::CGI_SET_RS485, m_pNode, &ptzInfo);


				//m_nPatrolSeqDurIdx=dlgPTZ.m_nPatrolSeqDurIdx;
				//m_pNode->nPatrolSeqDurIdx=dlgPTZ.m_nPatrolSeqDurIdx+1;			

				//BOOL bReset=m_bPatrolEnable;
				if ((m_pNode->nPTZAutorunType == 1) && (m_mapPatrolSeqDurSets!=dlgPTZ.m_mapPatrolSeqDuration))
				{
					ResetPatrolStatus();
					m_mapPatrolSeqDurSets=dlgPTZ.m_mapPatrolSeqDuration;
					OnBnClickedPatrolBut(1);
				}
				/*else if (m_mapPatrolSeqDurSets!=dlgPTZ.m_mapPatrolSeqDuration)
				{
					m_mapPatrolSeqDurSets=dlgPTZ.m_mapPatrolSeqDuration;
				}*/
			}
			
			m_mapPatrolSeqDurSets=dlgPTZ.m_mapPatrolSeqDuration;
			if (m_mapPatrolSeqDurSets.size()==0)
			{
				//m_bPatrolEnable=0;
				m_pNode->nPTZAutorunType=0;
				m_togglePartol.SetPos(-6);
			}

			if (_UseNewPTZ)
			{
				SetTimer(TIMER_WaitCGI_SET_PATROLNUM,250,0);
			}
		}
		else
		{
			//if (m_mapPatrolSeqDurSets.size()==0)
			//{
			//	CGS::bGetPatrolSeqDuration(m_mapPatrolSeqDurSets,m_pNode->mac);
			//}		

			//m_pNode->nPatrolSeqDurIdx=dlgPTZ.m_nPatrolSeqDurIdx+1;	

			//int nSet=0;
			//if (m_vctmapPatrolSeqDurSets.size()>0 && m_nPatrolSeqDurIdx<=(m_vctmapPatrolSeqDurSets.size()-1))
			//{
			//	nSet=m_vctmapPatrolSeqDurSets[m_nPatrolSeqDurIdx].size();
			//}
			
			//if (nSet==0)
			if (m_mapPatrolSeqDurSets.size()==0)
			{
				//m_bPatrolEnable=0;
				m_pNode->nPTZAutorunType=0;
				m_togglePartol.SetPos(-6);
			}
			return;
		}

	}
//	if (AfxMessageBox(_T("Do you really want to set PTZ?"), MB_YESNO) == IDNO)
//		return;
}


void CDlgPTZ::ResetPatrolStatus()
{
	if(m_pNode->nPTZAutorunType == 1)
	{
		//m_bPatrolEnable=0;
		for (int n=0;n<40;n++)		//wait 2 sec for thePartol() close//
		{
			Sleep(50);
			
			if (!m_pNode->hPatrolThread)
			{
				break;
			}
		}
		CloseHandle(m_pNode->hPatrolThread);
		Sleep(50);
	}
}

void CDlgPTZ::vJoyStickInputConvert(int nX)
{
	// nX range: 1~127; 0 should not call this function
	//Larry modify to calc interval 5XY to fix Max and Min Value//
	static int nInterval =5;

	if (nX >= 0 && nX < nInterval)
	{
		m_nJoyStickSpeed = 1;
	}
	else if (nX >= nInterval && nX < nInterval*2)
	{
		m_nJoyStickSpeed = 2;
	}
	else if (nX >= nInterval*2 && nX < nInterval*3)
	{
		m_nJoyStickSpeed = 3;
	}
	else if (nX >= nInterval*3 && nX < nInterval*4)
	{
		m_nJoyStickSpeed = 4;
	}
	else if (nX >= nInterval*4 && nX < nInterval*5)
	{
		m_nJoyStickSpeed = 5;
	}
	else
	{
		m_nJoyStickSpeed = 6;
	}

	m_nPreInput = nX;

	/*
	if(m_nPreInput ==0)
	{
		m_nJoyStickSpeed = 1;
		m_nPreInput = nX;
	}
	else
	{
		if(nX >= m_nPreInput)
		{
			m_nJoyStickSpeed++;
			m_nPreInput = nX;
		}
		else
		{
			m_nJoyStickSpeed--;
			m_nPreInput = nX;
		}
	}

	//if(m_nJoyStickSpeed < 1)
	//{
	//	m_nJoyStickSpeed = 1;
	//}

	//if(m_nJoyStickSpeed >6)
	//{
	//	m_nJoyStickSpeed = 6;
	//}
	*/
}

void CDlgPTZ::vSetPresetPoint(int nPreset)
{
	if(m_pNode == NULL && m_pNetsession == NULL)
		return ;

	if(!bCheckPTZEnable())
		return;

	if (m_bCanRunCGI==0)
	{
		return;
	}

	if(m_pNode)
	{
		/*USES_CONVERSION;
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
		csPass.Format(_T("%s"),tzPass);*/

		/*---Set PTZ Preset via CGI---*/
		//csCgiUrl.Format(_T("http://%s:%d/config/ptz_preset.cgi?name=%d&act=add"),csIP,nWebPort,nPreset);
		//mycurl.get(csCgiUrl,csUser,csPass);

		m_pPTZGetInfo->vQueryCgi(CGIType::CGI_SET_PRESET, m_pNode, nPreset);
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
LRESULT CDlgPTZ::OnBitmapPtzPress(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case CButtonDisk::POS_CENTER:
		break;
    case CButtonDisk::POS_UP:
		m_ptzAction = T_U;
		//vPTZMoveCGI(T_U,m_nPTZSpeed);
		break;
    case CButtonDisk::POS_RIGHT:
		m_ptzAction = P_R;
		//vPTZMoveCGI(P_R,m_nPTZSpeed);
		break;
    case CButtonDisk::POS_DOWN:
		m_ptzAction = T_D;
		//vPTZMoveCGI(T_D,m_nPTZSpeed);
		break;
    case CButtonDisk::POS_LEFT:
		m_ptzAction = P_L;
		//vPTZMoveCGI(P_L,m_nPTZSpeed);
        break;
    }
	m_ptzCGIType = PTZCGITYPE::PTZ;

	if(!m_hPTZCGIThread)
	{
		UINT nRet(0);
		m_hPTZCGIThread = (HANDLE) _beginthreadex(NULL,0,thdPTZCGI,(LPVOID)this,0,&nRet);
	}
	return 0;
}
LRESULT CDlgPTZ::OnBitmapPtzRelease(WPARAM wParam, LPARAM lParam)
{
	if (!m_pNode) return 0;
	
    switch (wParam)
    {
    case CButtonDisk::POS_CENTER:
    case CButtonDisk::POS_UP:
    case CButtonDisk::POS_RIGHT:
    case CButtonDisk::POS_DOWN:
    case CButtonDisk::POS_LEFT:
		if (m_pNode->nPTZAutorunType != 0)
		{
			DisablePTZAutoSetting();
		}
		vPTZMoveCGI(STOP, m_nPTZSpeed);
        break;
    }
    return 0;
}

 LRESULT CDlgPTZ::OnBitmapSliderMoved(WPARAM wParam, LPARAM lParam)
 {
	 if(m_pNode == NULL && m_pNetsession == NULL)
		return 0;

	if(!bCheckPTZEnable())
	{
		m_togglePartol.SetPos(-6);
		m_toggleAutoScan.SetPos(-6);
		m_toggleAutoFocus.SetPos(-6);
		return 0;
	}

	bool bChecked(false);
	int nPos(0);

	switch( wParam ) 
    {
    case IDC_STATIC_TOGGLE_PARTOL:
        bChecked = m_togglePartol.GetChecked();
		OnBnClickedPatrolBut(bChecked);
        break;
    case IDC_STATIC_TOGGLE_AUTO_SCAN:
        bChecked = m_toggleAutoScan.GetChecked();
		OnBnClickedAutoScanBut(bChecked);
        break;
    case IDC_STATIC_TOGGLE_AUTOFOCUS:
        bChecked = m_toggleAutoFocus.GetChecked();
		OnBnClickedAutoFocusBut(bChecked);
        break;
    case IDC_STATIC_SLIDER_ZOOM:
        break;
    case IDC_STATIC_SLIDER_FOCUS:
        break;
    default:
        break;
    }
    return 0;

 }
 LRESULT CDlgPTZ::OnBitmapSliderMoving(WPARAM wParam, LPARAM lParam)
 {
	 if(!m_pNode) return 0;
	int nPos(0);
	switch( wParam ) 
    {
    case IDC_STATIC_SLIDER_ZOOM:
		m_nSliderPos = m_sliderZoom.GetPos();
		m_ptzCGIType = PTZCGITYPE::ZOOM;
        break;
    case IDC_STATIC_SLIDER_FOCUS:
        m_nSliderPos = m_sliderFocus.GetPos(); 
		m_ptzCGIType = PTZCGITYPE::FOCUS;

		if (m_pNode->bPTZAutoFocus==1)
		{
			m_toggleAutoFocus.SetPos(-6);
			OnBnClickedAutoFocusBut(0);
			m_bWaitCGI_FOCUSCONT=1;
			SetTimer(TIMER_WaitCGI_AUTOFOCUS,1000,0);
			return 0;
		}
        break;
    default:
         return 0;
    }
	TRACE(_T("nPos:%d\r\n"),m_nSliderPos);

	if(!m_hPTZCGIThread && m_bWaitCGI_FOCUSCONT==0)
	{
		UINT nRet(0);
		m_hPTZCGIThread = (HANDLE) _beginthreadex(NULL,0,thdPTZCGI,(LPVOID)this,0,&nRet);
	}
    return 0;
 }

 void CDlgPTZ::OnBnClickedBtnPtzPrsetSet()
 {	 
	if(!m_pNode) return;
	int nPreset(0);
	CString csPreset;
	m_editPTZPreset.GetWindowTextW(csPreset);
	nPreset = _ttoi(csPreset);
	if( nPreset < 0 || nPreset > 255) return;

	if (m_pNode->nPTZAutorunType != 0)
	{
		DisablePTZAutoSetting();
	}

	vSetPresetPoint(nPreset);
 }

 void CDlgPTZ::OnBnClickedBtnPtzPrsetGo()
 {
	 if(!m_pNode)return;
	int nPreset(0);
	CString csPreset;
	m_editPTZPreset.GetWindowTextW(csPreset);
	nPreset = _ttoi(csPreset);
	if( nPreset < 0 || nPreset > 255) return;

	if (m_pNode->nPTZAutorunType != 0)
	{
		DisablePTZAutoSetting();
	}

	vGoToPresetPoint(nPreset);
 }

 HBRUSH CDlgPTZ::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
 {
	  pDC->SetTextColor(m_clrFont);
	  pDC->SetBkColor(m_clrBk);	
	return  m_SilverBrush;
 }

UINT CDlgPTZ::thdPTZCGI(LPVOID lpParam)
{
	CDlgPTZ* pThis = (CDlgPTZ*) lpParam;
	PTZAction ptzaction;
	int nSpeed(0);	
	
	switch(pThis->m_ptzCGIType)
	{
	case PTZCGITYPE::ZOOM:
		if (pThis->m_bJoystickMove)
		{
			ptzaction = pThis->m_ptzAction;
			nSpeed = pThis->m_nPTZSpeed;
			//nSpeed = abs(pThis->m_nPTZSpeed);

			if (nSpeed == 0)
			{
				CLOSEHANDLE(pThis->m_hPTZCGIThread);	
				return 0;
			}
			break;
		}
		else
		{
			if(pThis->m_nSliderPos==0)
			{
				CLOSEHANDLE(pThis->m_hPTZCGIThread);	
				return 0;
			}	
			if( pThis->m_nSliderPos >0 )
			{
				ptzaction = Z_I;
			}
			else
			{
				ptzaction = Z_O;
			}

			nSpeed = abs(pThis->m_nSliderPos);
			break;
		}
	case PTZCGITYPE::FOCUS:
		if (pThis->m_bJoystickMove)
		{
			ptzaction = pThis->m_ptzAction;
			nSpeed = pThis->m_nPTZSpeed;
			//nSpeed = abs(pThis->m_nPTZSpeed);
			if (nSpeed == 0)
			{
				CLOSEHANDLE(pThis->m_hPTZCGIThread);	
				return 0;
			}			
			break;
		}
		else
		{
			if(pThis->m_nSliderPos==0)
			{
				CLOSEHANDLE(pThis->m_hPTZCGIThread);	
				return 0;
			}	
			if( pThis->m_nSliderPos >0 )
			{
				ptzaction = F_O;
			}
			else
			{
				ptzaction = F_I;
			}	
			//nSpeed = abs(pThis->m_nSliderPos);
			
			nSpeed = pThis->m_nSliderPos;
			break;
		}		
	case PTZCGITYPE::PTZ:
		ptzaction = pThis->m_ptzAction;
		nSpeed = pThis->m_nPTZSpeed;
		break;
	}

	pThis->vPTZMoveCGI(ptzaction, nSpeed);
	CLOSEHANDLE(pThis->m_hPTZCGIThread);
	pThis->m_bJoystickMove=false;
	return 0;
}


void CDlgPTZ::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(!m_pNode) return;
	if (nIDEvent == TIMER_ReSTOP)
	{
		KillTimer(nIDEvent);
		vPTZMoveCGI(STOP, m_nPTZSpeed);
	}
	else if (nIDEvent == TIMER_CheckConnect)
	{
		KillTimer(nIDEvent);

		if (m_nCheckConnect <0 || *m_pMsgIdxStatus == 3)
		{
			if (*m_pMsgIdxStatus == 3 && m_pVideoThin)
			{
				if (m_pVideoThin->m_mapPatrolSeqDurSets.size() == 0)
				{
					CGS::bGetPatrolSeqDuration(m_mapPatrolSeqDurSets,m_pNode->mac);
					if (m_pVideoThin)
					{
						m_pVideoThin->m_mapPatrolSeqDurSets=m_mapPatrolSeqDurSets;
					}
				}
				else
				{
					m_mapPatrolSeqDurSets=m_pVideoThin->m_mapPatrolSeqDurSets;
				}

				if (m_pNode->bSupportPTZ == 0)
				{
					if(m_pNode->nPTZAutorunType == 1)
						m_togglePartol.SetPos(6);
					else if (m_pNode->nPTZAutorunType == 2)
						m_toggleAutoScan.SetPos(6);
				}

				m_bCanRunCGI=1;
				m_bInitvAttach=1;
				m_pPTZGetInfo->vQueryCgi(CGIType::CGI_GETINFO,m_pNode);
			}
			return;
		}
		
		SetTimer(TIMER_CheckConnect,1000,0);
		m_nCheckConnect--;
	}
	else if (nIDEvent == TIMER_WaitCGI_AUTOFOCUS)
	{
		KillTimer(nIDEvent);
		TRACE(_T("nPos:%d\r\n"),m_nSliderPos);
		m_bWaitCGI_FOCUSCONT=0;

		if(!m_hPTZCGIThread)
		{
			UINT nRet(0);
			m_hPTZCGIThread = (HANDLE) _beginthreadex(NULL,0,thdPTZCGI,(LPVOID)this,0,&nRet);
		}
	}
	else if (nIDEvent == TIMER_WaitCGI_MOVECONT)
	{
		//Send PTZ Stop//
		KillTimer(nIDEvent);
		
		if (m_bCanRunCGI)
		{
			m_pPTZGetInfo->vQueryCgi(CGIType::CGI_MOVECONT,m_pNode, 0, 0, 0);
			TRACE(_T("\r\nSend CGI_MOVECONT Stop PTZSpeed:%d\r\n"),m_nPTZSpeed);
		}
		m_bJoystickFocus=0;
		//m_bJoystickZoom=0;
		m_pPTZGetInfo->m_bFOCUSCONTSend=0;
	}
	else if (nIDEvent == TIMER_WaitCGI_FOCUSCONT)
	{
		KillTimer(nIDEvent);

		if (m_bCanRunCGI)
		{
			//Set Stop F_I , F_O//
			m_pPTZGetInfo->m_bStopCGI=1;
			m_pPTZGetInfo->m_bFOCUSCONTSend=0;
			m_pPTZGetInfo->vQueryCgi(CGIType::CGI_FOCUSCONT,m_pNode,0);
			m_nOldFocusPos=0;											  			
			TRACE(_T("\r\nSend CGI_FOCUSCONT Stop\r\n"));
		}
	}
	else if (nIDEvent == TIMER_WaitCGI_GET_PATROLNUM)
	{
		KillTimer(nIDEvent);

		////////////Larry add 102-12-23/////////
		if (m_pNode->bSupportPTZ==1)
		{//Get patrol info from ip camera//
			if (m_bCanRunCGI==1)
			{
				if (m_pPTZGetInfo->m_bActiven==1)
				{
					SetTimer(TIMER_WaitCGI_GET_PATROLNUM,250,0);
					return;
				}

				m_pPTZGetInfo->vQueryCgi(CGIType::CGI_GET_PATROLNUM,m_pNode);		//m_nPatrolNumber
			}
		}
		//////////////////////////////////////////
	}
	else if (nIDEvent == TIMER_WaitCGI_SET_PATROLNUM)
	{
		KillTimer(nIDEvent);

		if (m_pNode->bSupportPTZ==1)
		{
			if (m_bCanRunCGI==1)
			{
				if (m_pPTZGetInfo->m_bActiven==1)
				{
					SetTimer(TIMER_WaitCGI_SET_PATROLNUM,250,0);
					return;
				}

				m_pPTZGetInfo->vQueryCgi(CGIType::CGI_SET_PATROLNUM,m_pNode,m_mapPatrolSeqDurSets);
			}
		}
	}
	else if (nIDEvent == TIMER_JoyStick)
	{
		// Update the input device every timer message
			if( FAILED( UpdateInputState() ) )
			{
				KillTimer(nIDEvent);
		//		//MessageBox( NULL, TEXT( "Error Reading Input State. " ) \
		//		//	TEXT( "The sample will now exit." ), TEXT( "DirectInput Sample" ),
		//		//	MB_ICONERROR | MB_OK );
		//		//EndDialog( hDlg, TRUE );
			}
		//	return TRUE;
	}
	
	CBaseDialog::OnTimer(nIDEvent);
}

void CDlgPTZ::DisablePTZAutoSetting()
{
	if(!m_pNode) return;
	if(m_pNode->bSupportPTZ)
	{
		OnBnClickedPatrolBut(0);
		OnBnClickedAutoScanBut(0);
	}

	m_pNode->nPTZAutorunType=0;
	m_togglePartol.SetPos(-6);
	m_toggleAutoScan.SetPos(-6);	
}
BOOL CDlgPTZ::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	//if(m_bPatrolEnable)
	//{
	//	m_bPatrolEnable=0;
	//	for (int n=0;n<40;n++)	//wait 2 sec for thePartol() close//
	//	{
	//		Sleep(50);
	//		if (!m_hPatrolThread)
	//		{
	//			break;
	//		}
	//	}
	//}

	if (m_pDlgJoyDebug)
	{
		DELETEOBJ(m_pDlgJoyDebug);
	}			
	FreeDirectInput();

	return CBaseDialog::DestroyWindow();
}

void CDlgPTZ::vCheckCGIStatus(int nCheckType)
{
	//To make sure the ip camera  cgi success or fail//

	if (m_pNode==NULL)
	{
		return;
	}
	if (!m_pPTZGetInfo)
	{
		return;
	}

	m_nCheckAutoType=nCheckType;
	if(!m_hPTZGetInfoThread)
	{
		UINT nRet(0);
		m_hPTZGetInfoThread = (HANDLE) _beginthreadex(NULL,0,thdPTZGetInfo,(LPVOID)this,0,&nRet);
	}
}

void CDlgPTZ::vReCheckConnect(BOOL bIsConnect)
{
	if(!m_pNode) return;
	if (m_bInitvAttach == 0)
	{
		return;
	}

	KillTimer(TIMER_CheckConnect);
	if (bIsConnect)
	{
		if (*m_pMsgIdxStatus != 3)
		{
			//disconnected//
			m_nCheckConnect=5;
			SetTimer(TIMER_CheckConnect,1000,0);		//wait the Video connect for 5 sec//
			return;
		}
		
		if (m_pNode->hPatrolThread == NULL)
		{
			CGS::bGetPatrolSeqDuration(m_mapPatrolSeqDurSets,m_pNode->mac);
			if (m_pVideoThin)
			{
				m_pVideoThin->m_mapPatrolSeqDurSets=m_mapPatrolSeqDurSets;
			}
		}
		else
		{
			m_mapPatrolSeqDurSets=m_pVideoThin->m_mapPatrolSeqDurSets;
		}

		if (m_pNode->bSupportPTZ == 0)
		{
			if(m_pNode->nPTZAutorunType == 1)
				m_togglePartol.SetPos(6);
			else if (m_pNode->nPTZAutorunType == 2)
				m_toggleAutoScan.SetPos(6);
		}

		m_bCanRunCGI=1;
		m_pPTZGetInfo->vQueryCgi(CGIType::CGI_GETINFO,m_pNode);
		
		TRACE(_T("\r\nVideoThin Connect\r\n"));
	}
	else
	{
		m_bCanRunCGI=0;
		m_togglePartol.SetPos(-6);
		m_toggleAutoScan.SetPos(-6);
		m_toggleAutoFocus.SetPos(-6);

		TRACE(_T("\r\nVideoThin Disconnect\r\n"));
	}
}

void CDlgPTZ::vReSetPTZPatrolSetting(CString csPatrolSeq)
{
	//csPatrolSeq=L"{\"num\":\"1\",\"name\":\"Patrol_01\",\"repeat\":\"yes\",\"run1_num\":\"1\",\"run1_stay\":\"10\",\"run2_num\":\"2\",\"run2_stay\":\"10\",\"run3_num\":\"3\",\"run3_stay\":\"10\",\"run4_num\":\"4\",\"run4_stay\":\"10\",\"run5_num\":\"5\",\"run5_stay\":\"10\"},";
	m_mapPatrolSeqDurSets.clear();

	int nTotal=csPatrolSeq.GetLength();

	int nRunCount=1;
	CString csRun,csStay;
	csRun.Format(L"run%d_num",nRunCount);

	int nFind=csPatrolSeq.Find(csRun);
	while(nFind > -1)
	{		
		csStay.Format(L"run%d_stay",nRunCount);

		int nRunLen=csRun.GetLength();
		int nStayLen=csStay.GetLength();
		int nRun=1,nStay=3;

		for (int nFindCount=0;nFindCount<2;nFindCount++)
		{
			int nIsSingle=0;
			TCHAR* ps;
			if (nFindCount==0)
			{
				ps=csPatrolSeq.GetBuffer();
				ps +=nFind+nRunLen+3;
			}
			else
			{
				ps=csPatrolSeq.GetBuffer();
				ps +=nFind+nStayLen+3;
			}

			while (*ps!='"')
			{
				ps++;
				nIsSingle++;
			}

			CString csBuf;
			if (nFindCount==0)
			{
				csBuf=csPatrolSeq.Mid(nFind+nRunLen+3,nIsSingle);
				nRun=_wtoi(csBuf);
				nFind=csPatrolSeq.Find(csStay);
				if (nFind==-1)
				{
					break;
				}
			}
			else
			{
				csBuf=csPatrolSeq.Mid(nFind+nStayLen+3,nIsSingle);
				nStay=_wtoi(csBuf);
			}
		}			

		pair_INT p(nRun,nStay);
		m_mapPatrolSeqDurSets.insert(pair<int,pair_INT>(nRunCount,p));


		nRunCount++;
		csRun.Format(L"run%d_num",nRunCount);
		nFind=csPatrolSeq.Find(csRun,nFind+1);
	}

	//103-05-20 to get ptz_speed on web//
	PTZSettingInfo ptzInfo;
	m_pPTZGetInfo->vQueryCgi(CGIType::CGI_GET_PTZSPEED, m_pNode, &ptzInfo);
	/////////////////////////////////////////

	//m_bGetPatrolNun=1;
}

void CDlgPTZ::vSetLiveViewAuto(BOOL bAuto)
{
	m_bLiveViewAutoScan=bAuto;
	if (bAuto)
	{
		KillTimer(TIMER_CheckConnect);
	}
}

LRESULT CDlgPTZ::OnUpdateView(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case UM_SET_LIVEVIEW_AUTOTYPE:
		{
			vSetLiveViewAuto((BOOL)lParam);
		}
		break;

	default:
		break;
	}
	return 0;
}

void CDlgPTZ::OnEnSetfocusEditPtzPreset()
{
	if (m_pvk != NULL)
	{
		DELETEOBJ(m_pvk)
	}
	m_pvk = new VK;

}

void CDlgPTZ::OnEnKillfocusEditPtzPreset()
{
	DELETEOBJ(m_pvk);;
}




//-----------------------------------------------------------------------------
// Name: InitDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
HRESULT InitDirectInput( HWND hDlg )
{
	HRESULT hr;
	// Register with the DirectInput subsystem and get a pointer
	// to a IDirectInput interface we can use.
	// Create a DInput object
	if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION,
		IID_IDirectInput8, ( VOID** )&g_pDI, NULL ) ) )
		return hr;


	if( g_bFilterOutXinputDevices )
		SetupForIsXInputDevice();

	DIJOYCONFIG PreferredJoyCfg = {0};
	DI_ENUM_CONTEXT enumContext;
	enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
	enumContext.bPreferredJoyCfgValid = false;

	IDirectInputJoyConfig8* pJoyConfig = NULL;
	if( FAILED( hr = g_pDI->QueryInterface( IID_IDirectInputJoyConfig8, ( void** )&pJoyConfig ) ) )
		return hr;

	PreferredJoyCfg.dwSize = sizeof( PreferredJoyCfg );
	if( SUCCEEDED( pJoyConfig->GetConfig( 0, &PreferredJoyCfg, DIJC_GUIDINSTANCE ) ) ) // This function is expected to fail if no joystick is attached
		enumContext.bPreferredJoyCfgValid = true;
	SAFE_RELEASE( pJoyConfig );

	// Look for a simple joystick we can use for this sample program.
	if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL,
		EnumJoysticksCallback,
		&enumContext, DIEDFL_ATTACHEDONLY ) ) )
		return hr;

	if( g_bFilterOutXinputDevices )
		CleanupForIsXInputDevice();

	// Make sure we got a joystick
	if( NULL == g_pJoystick )
	{
		//MessageBox( NULL, TEXT( "Joystick not found. The sample will now exit." ),
		//	TEXT( "DirectInput Sample" ),
		//	MB_ICONERROR | MB_OK );
		//EndDialog( hDlg, 0 );
		return S_OK;
	}

	// Set the data format to "simple joystick" - a predefined data format 
	//
	// A data format specifies which controls on a device we are interested in,
	// and how they should be reported. This tells DInput that we will be
	// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
	if( FAILED( hr = g_pJoystick->SetDataFormat( &c_dfDIJoystick2) ) )
		return hr;

	// Set the cooperative level to let DInput know how this device should
	// interact with the system and with other DInput applications.
	if( FAILED( hr = g_pJoystick->SetCooperativeLevel( hDlg, DISCL_EXCLUSIVE |	DISCL_FOREGROUND ) ) )		//( hDlg, DISCL_EXCLUSIVE |	DISCL_FOREGROUND ) ) )
		return hr;

	// Enumerate the joystick objects. The callback function enabled user
	// interface elements for objects that are found, and sets the min/max
	// values property for discovered axes.
	/*if( FAILED( hr = g_pJoystick->EnumObjects( EnumObjectsCallback,
		( VOID* )hDlg, DIDFT_ALL ) ) )
		return hr;*/

	return S_OK;
}


//-----------------------------------------------------------------------------
// Enum each PNP device using WMI and check each device ID to see if it contains 
// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it’s an XInput device
// Unfortunately this information can not be found by just using DirectInput.
// Checking against a VID/PID of 0x028E/0x045E won't find 3rd party or future 
// XInput devices.
//
// This function stores the list of xinput devices in a linked list 
// at g_pXInputDeviceList, and IsXInputDevice() searchs that linked list
//-----------------------------------------------------------------------------
HRESULT SetupForIsXInputDevice()
{
	IWbemServices* pIWbemServices = NULL;
	IEnumWbemClassObject* pEnumDevices = NULL;
	IWbemLocator* pIWbemLocator = NULL;
	IWbemClassObject* pDevices[20] = {0};
	BSTR bstrDeviceID = NULL;
	BSTR bstrClassName = NULL;
	BSTR bstrNamespace = NULL;
	DWORD uReturned = 0;
	bool bCleanupCOM = false;
	UINT iDevice = 0;
	VARIANT var;
	HRESULT hr;

	// CoInit if needed
	hr = CoInitialize( NULL );
	bCleanupCOM = SUCCEEDED( hr );

	// Create WMI
	hr = CoCreateInstance( __uuidof( WbemLocator ),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof( IWbemLocator ),
		( LPVOID* )&pIWbemLocator );
	if( FAILED( hr ) || pIWbemLocator == NULL )
		goto LCleanup;

	// Create BSTRs for WMI
	bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" ); if( bstrNamespace == NULL ) goto LCleanup;
	bstrDeviceID = SysAllocString( L"DeviceID" );           if( bstrDeviceID == NULL )  goto LCleanup;
	bstrClassName = SysAllocString( L"Win32_PNPEntity" );    if( bstrClassName == NULL ) goto LCleanup;

	// Connect to WMI 
	hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L,
		0L, NULL, NULL, &pIWbemServices );
	if( FAILED( hr ) || pIWbemServices == NULL )
		goto LCleanup;

	// Switch security level to IMPERSONATE
	CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0 );

	// Get list of Win32_PNPEntity devices
	hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices );
	if( FAILED( hr ) || pEnumDevices == NULL )
		goto LCleanup;

	// Loop over all devices
	for(; ; )
	{
		// Get 20 at a time
		hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
		if( FAILED( hr ) )
			goto LCleanup;
		if( uReturned == 0 )
			break;

		for( iDevice = 0; iDevice < uReturned; iDevice++ )
		{
			// For each device, get its device ID
			hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );
			if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL )
			{
				// Check if the device ID contains "IG_".  If it does, then it’s an XInput device
				// Unfortunately this information can not be found by just using DirectInput 
				if( wcsstr( var.bstrVal, L"IG_" ) )
				{
					// If it does, then get the VID/PID from var.bstrVal
					DWORD dwPid = 0, dwVid = 0;
					WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
					if( strVid && swscanf( strVid, L"VID_%4X", &dwVid ) != 1 )
						dwVid = 0;
					WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
					if( strPid && swscanf( strPid, L"PID_%4X", &dwPid ) != 1 )
						dwPid = 0;

					DWORD dwVidPid = MAKELONG( dwVid, dwPid );

					// Add the VID/PID to a linked list
					XINPUT_DEVICE_NODE* pNewNode = new XINPUT_DEVICE_NODE;
					if( pNewNode )
					{
						pNewNode->dwVidPid = dwVidPid;
						pNewNode->pNext = g_pXInputDeviceList;
						g_pXInputDeviceList = pNewNode;
					}
				}
			}
			SAFE_RELEASE( pDevices[iDevice] );
		}
	}

LCleanup:
	if( bstrNamespace )
		SysFreeString( bstrNamespace );
	if( bstrDeviceID )
		SysFreeString( bstrDeviceID );
	if( bstrClassName )
		SysFreeString( bstrClassName );
	for( iDevice = 0; iDevice < 20; iDevice++ )
		SAFE_RELEASE( pDevices[iDevice] );
	SAFE_RELEASE( pEnumDevices );
	SAFE_RELEASE( pIWbemLocator );
	SAFE_RELEASE( pIWbemServices );

	return hr;
}


//-----------------------------------------------------------------------------
// Returns true if the DirectInput device is also an XInput device.
// Call SetupForIsXInputDevice() before, and CleanupForIsXInputDevice() after
//-----------------------------------------------------------------------------
bool IsXInputDevice( const GUID* pGuidProductFromDirectInput )
{
	// Check each xinput device to see if this device's vid/pid matches
	XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
	while( pNode )
	{
		if( pNode->dwVidPid == pGuidProductFromDirectInput->Data1 )
			return true;
		pNode = pNode->pNext;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Cleanup needed for IsXInputDevice()
//-----------------------------------------------------------------------------
void CleanupForIsXInputDevice()
{
	// Cleanup linked list
	XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
	while( pNode )
	{
		XINPUT_DEVICE_NODE* pDelete = pNode;
		pNode = pNode->pNext;
		SAFE_DELETE( pDelete );
	}
}



//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
									VOID* pContext )
{
	DI_ENUM_CONTEXT* pEnumContext = ( DI_ENUM_CONTEXT* )pContext;
	HRESULT hr;

	if( g_bFilterOutXinputDevices && IsXInputDevice( &pdidInstance->guidProduct ) )
		return DIENUM_CONTINUE;

	// Skip anything other than the perferred joystick device as defined by the control panel.  
	// Instead you could store all the enumerated joysticks and let the user pick.
	if( pEnumContext->bPreferredJoyCfgValid &&
		!IsEqualGUID( pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance ) )
		return DIENUM_CONTINUE;

	// Obtain an interface to the enumerated joystick.
	hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick, NULL );

	// If it failed, then we can't use this joystick. (Maybe the user unplugged
	// it while we were in the middle of enumerating it.)
	if( FAILED( hr ) )
		return DIENUM_CONTINUE;

	// Stop enumeration. Note: we're just taking the first joystick we get. You
	// could store all the enumerated joysticks and let the user pick.
	return DIENUM_STOP;
}




//-----------------------------------------------------------------------------
// Name: EnumObjectsCallback()
// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
//       joystick. This function enables user interface elements for objects
//       that are found to exist, and scales axes min/max values.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
								  VOID* pContext )
{
	HWND hDlg = ( HWND )pContext;

	static int nSliderCount = 0;  // Number of returned slider controls
	static int nPOVCount = 0;     // Number of returned POV controls

	// For axes that are returned, set the DIPROP_RANGE property for the
	// enumerated axis in order to scale min/max values.
	if( pdidoi->dwType & DIDFT_AXIS )
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof( DIPROPRANGE );
		diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
		diprg.lMin = -1000;
		diprg.lMax = +1000;

		// Set the range for the axis
		if( FAILED( g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
			return DIENUM_STOP;
	}


	//// Set the UI to reflect what objects the joystick supports
	if( pdidoi->guidType == GUID_XAxis )
	{
		EnableWindow( GetDlgItem( hDlg, IDC_X_AXIS ), TRUE );
		EnableWindow( GetDlgItem( hDlg, IDC_X_AXIS_TEXT ), TRUE );
	}
	if( pdidoi->guidType == GUID_YAxis )
	{
		EnableWindow( GetDlgItem( hDlg, IDC_Y_AXIS ), TRUE );
		EnableWindow( GetDlgItem( hDlg, IDC_Y_AXIS_TEXT ), TRUE );
	}
	if( pdidoi->guidType == GUID_ZAxis )
	{
		EnableWindow( GetDlgItem( hDlg, IDC_Z_AXIS ), TRUE );
		EnableWindow( GetDlgItem( hDlg, IDC_Z_AXIS_TEXT ), TRUE );
	}
	if( pdidoi->guidType == GUID_RxAxis )
	{
		EnableWindow( GetDlgItem( hDlg, IDC_X_ROT ), TRUE );
		EnableWindow( GetDlgItem( hDlg, IDC_X_ROT_TEXT ), TRUE );
	}
	if( pdidoi->guidType == GUID_RyAxis )
	{
		EnableWindow( GetDlgItem( hDlg, IDC_Y_ROT ), TRUE );
		EnableWindow( GetDlgItem( hDlg, IDC_Y_ROT_TEXT ), TRUE );
	}
	if( pdidoi->guidType == GUID_RzAxis )
	{
		EnableWindow( GetDlgItem( hDlg, IDC_Z_ROT ), TRUE );
		EnableWindow( GetDlgItem( hDlg, IDC_Z_ROT_TEXT ), TRUE );
	}
	if( pdidoi->guidType == GUID_Slider )
	{
		switch( nSliderCount++ )
		{
		case 0 :
			EnableWindow( GetDlgItem( hDlg, IDC_SLIDER0 ), TRUE );
			EnableWindow( GetDlgItem( hDlg, IDC_SLIDER0_TEXT ), TRUE );
			break;

		case 1 :
			EnableWindow( GetDlgItem( hDlg, IDC_SLIDER1 ), TRUE );
			EnableWindow( GetDlgItem( hDlg, IDC_SLIDER1_TEXT ), TRUE );
			break;
		}
	}
	if( pdidoi->guidType == GUID_POV )
	{
		switch( nPOVCount++ )
		{
		case 0 :
			EnableWindow( GetDlgItem( hDlg, IDC_POV0 ), TRUE );
			EnableWindow( GetDlgItem( hDlg, IDC_POV0_TEXT ), TRUE );
			break;

		case 1 :
			EnableWindow( GetDlgItem( hDlg, IDC_POV1 ), TRUE );
			EnableWindow( GetDlgItem( hDlg, IDC_POV1_TEXT ), TRUE );
			break;

		case 2 :
			EnableWindow( GetDlgItem( hDlg, IDC_POV2 ), TRUE );
			EnableWindow( GetDlgItem( hDlg, IDC_POV2_TEXT ), TRUE );
			break;

		case 3 :
			EnableWindow( GetDlgItem( hDlg, IDC_POV3 ), TRUE );
			EnableWindow( GetDlgItem( hDlg, IDC_POV3_TEXT ), TRUE );
			break;
		}
	}

	return DIENUM_CONTINUE;
}



//-----------------------------------------------------------------------------
// Name: FreeDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
VOID FreeDirectInput()
{
	// Unacquire the device one last time just in case 
	// the app tried to exit while the device is still acquired.
	if( g_pJoystick )
		g_pJoystick->Unacquire();

	// Release any DirectInput objects.
	SAFE_RELEASE( g_pJoystick );
	SAFE_RELEASE( g_pDI );
}

void CDlgPTZ::InitJoyStick(HWND hDlg)
{
	m_hJoyDlg=	hDlg;
	InitCommonControls();

	WCHAR* strCmdLine;
	int nNumArgs;
	LPWSTR* pstrArgList = CommandLineToArgvW( GetCommandLineW(), &nNumArgs );
	for( int iArg = 1; iArg < nNumArgs; iArg++ )
	{
		strCmdLine = pstrArgList[iArg];

		// Handle flag args
		if( *strCmdLine == L'/' || *strCmdLine == L'-' )
		{
			strCmdLine++;

			int nArgLen = ( int )wcslen( L"noxinput" );
			if( _wcsnicmp( strCmdLine, L"noxinput", nArgLen ) == 0 && strCmdLine[nArgLen] == 0 )
			{
				g_bFilterOutXinputDevices = true;
				continue;
			}
		}
	}
	LocalFree( pstrArgList );

	// Display the main dialog box.
	m_pDlgJoyDebug = new CDlgJoyStick;
	m_pDlgJoyDebug->Create(IDD_JOYST_IMM, this);
//#ifdef _DEBUG
//	m_pDlgJoyDebug->ShowWindow(SW_SHOW);
//#else if
	m_pDlgJoyDebug->ShowWindow(SW_HIDE);
//#endif
	m_hDlgJoyDebug = m_pDlgJoyDebug->GetSafeHwnd();

	if( FAILED( InitDirectInput(m_hJoyDlg ) ) )
	{
		//MessageBox(TEXT( L"Error Initializing DirectInput" ), TEXT( L"DirectInput Sample" ), MB_ICONERROR | MB_OK );
		//EndDialog( hDlg, 0 );

		MessageBox(L"Error Initializing DirectInput");
	}
	else
	{
//#ifdef _DEBUG
		HRESULT hr;
		if (g_pJoystick)
		{
			if( FAILED( hr = g_pJoystick->EnumObjects( EnumObjectsCallback,
				( VOID* )(m_pDlgJoyDebug->GetSafeHwnd()), DIDFT_ALL ) ) )
				TRACE(L"Error To Set Enable window ");
		}																				 		
//#endif
		// Set a timer to go off 30 times a second. At every timer message
		// the input device will be read
		if (g_pJoystick)
			SetTimer( TIMER_JoyStick, 1000 / 30, NULL );
	}
}

//-----------------------------------------------------------------------------
// Name: UpdateInputState()
// Desc: Get the input device's state and display it.
//-----------------------------------------------------------------------------
HRESULT CDlgPTZ::UpdateInputState()
{
	if ((m_pNetsession==NULL) && (m_pNode==NULL)) return 0;

	// Query DB if PTZ is enabled
	if(!bCheckPTZEnable())
		return 0;

	
	HRESULT hr;
	TCHAR strText[512] = {0}; // Device state text
	DIJOYSTATE2 js;           // DInput joystick state 

	if( NULL == g_pJoystick )
		return S_OK;

	// Poll the device to read the current state
	hr = g_pJoystick->Poll();
	if( FAILED( hr ) )
	{
		// DInput is telling us that the input stream has been
		// interrupted. We aren't tracking any state between polls, so
		// we don't have any special reset that needs to be done. We
		// just re-acquire and try again.
		hr = g_pJoystick->Acquire();
		while( hr == DIERR_INPUTLOST )
			hr = g_pJoystick->Acquire();

		// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
		// may occur when the app is minimized or in the process of 
		// switching, so just try again later 
		return S_OK;
	}

	// Get the input's device state
	if( FAILED( hr = g_pJoystick->GetDeviceState( sizeof( DIJOYSTATE2), &js ) ) )
		return hr; // The device should have been acquired during the Poll()

	PTZAction ptzaction(PTZINIT);
	JOYINFOEX joyex;
	ZeroMemory(&joyex, sizeof(JOYINFOEX));
	joyex.dwSize = sizeof(JOYINFOEX);
	joyex.dwFlags = JOY_RETURNALL;
	joyGetPosEx(JOYSTICKID1, &joyex);

	//if ((joyex.dwButtons  > JOY_BUTTON2) && joyex.dwButtonNumber)
	//{
	//	//	SendMessage(MM_JOY1BUTTONDOWN, joyex.dwButtons, 0);		
	//	BYTE nPreset = 0;
	//	int nMask[]={JOY_BUTTON3, JOY_BUTTON4, JOY_BUTTON5, JOY_BUTTON6, JOY_BUTTON7, JOY_BUTTON8, JOY_BUTTON9, JOY_BUTTON10, JOY_BUTTON11, JOY_BUTTON12, JOY_BUTTON13, JOY_BUTTON14, JOY_BUTTON15, JOY_BUTTON16, JOY_BUTTON17, JOY_BUTTON18, JOY_BUTTON19, JOY_BUTTON20, JOY_BUTTON21, JOY_BUTTON22, JOY_BUTTON23, JOY_BUTTON24, JOY_BUTTON25, JOY_BUTTON26, JOY_BUTTON27, JOY_BUTTON28, JOY_BUTTON29, JOY_BUTTON30, JOY_BUTTON31, JOY_BUTTON32 };

	//	for (int ci=0; ci < sizeof(nMask) / sizeof(int); ci++)
	//	{
	//		if (joyex.dwButtons & nMask[ci]) 
	//		{
	//			(nPreset = ci+1);
	//			break;
	//		}
	//	}
	//	if (nPreset)
	//	{
	//		int nSize =0;
	//		CONTROLDATA ctrl={0};
	//		ctrl.channel = 1;
	//		ctrl.controlID = PCTL_PRESET_MOVE;
	//		ctrl.payload.preset_move = (BYTE)nPreset;
	//		nSize = 5;
	//		if (m_pNetsession)
	//		{
	//			int nRet = m_pNetsession->sendControl(m_pNetsession, FCC_PCONTROL,(char*)&ctrl, nSize);
	//			ETROLOG(_T("OnJoyStickMove return :%d\r\n"), nRet);
	//		}
	//		::Sleep(100);
	//	}
	//	return 0;		
	//}

	//// Display joystick state to dialog
//#ifdef _DEBUG
	//// Axes
	StringCchPrintf( strText, 512, TEXT( "%ld" ), js.lX );
	(m_pDlgJoyDebug->GetDlgItem(IDC_X_AXIS))->SetWindowText(strText);
	//SetWindowText( GetDlgItem( m_hDlgJoyDebug, IDC_X_AXIS ), strText );
	
	StringCchPrintf( strText, 512, TEXT( "%ld" ), js.lY );
	(m_pDlgJoyDebug->GetDlgItem(IDC_Y_AXIS))->SetWindowText(strText);
	//SetWindowText( GetDlgItem( m_hDlgJoyDebug, IDC_Y_AXIS ), strText );

	StringCchPrintf( strText, 512, TEXT( "%ld" ), js.lZ );
	(m_pDlgJoyDebug->GetDlgItem(IDC_Z_AXIS))->SetWindowText(strText);
	//SetWindowText( GetDlgItem( m_hDlgJoyDebug, IDC_Z_AXIS ), strText );

	StringCchPrintf( strText, 512, TEXT( "%ld" ), js.lRx );
	(m_pDlgJoyDebug->GetDlgItem(IDC_X_ROT))->SetWindowText(strText);
	//SetWindowText( GetDlgItem( m_hDlgJoyDebug, IDC_X_ROT ), strText );

	StringCchPrintf( strText, 512, TEXT( "%ld" ), js.lRy );
	(m_pDlgJoyDebug->GetDlgItem(IDC_Y_ROT))->SetWindowText(strText);
	//SetWindowText( GetDlgItem( m_hDlgJoyDebug, IDC_Y_ROT ), strText );

	StringCchPrintf( strText, 512, TEXT( "%ld" ), js.lRz );
	(m_pDlgJoyDebug->GetDlgItem(IDC_Z_ROT))->SetWindowText(strText);
	//SetWindowText( GetDlgItem( m_hDlgJoyDebug, IDC_Z_ROT ), strText );

	//// Slider controls
	StringCchPrintf( strText, 512, TEXT( "%ld" ), js.rglSlider[0] );
	(m_pDlgJoyDebug->GetDlgItem(IDC_SLIDER0))->SetWindowText(strText);
	//SetWindowText( GetDlgItem( m_hDlgJoyDebug, IDC_SLIDER0 ), strText );

	StringCchPrintf( strText, 512, TEXT( "%ld" ), js.rglSlider[1] );
	(m_pDlgJoyDebug->GetDlgItem(IDC_SLIDER1))->SetWindowText(strText);
	//SetWindowText( GetDlgItem( m_hDlgJoyDebug, IDC_SLIDER1 ), strText );


	//// Points of view
	StringCchPrintf( strText, 512, TEXT( "%ld" ), js.rgdwPOV[0] );
	(m_pDlgJoyDebug->GetDlgItem(IDC_POV0))->SetWindowText(strText);
	//SetWindowText( GetDlgItem( m_hDlgJoyDebug, IDC_POV0 ), strText );

	StringCchPrintf( strText, 512, TEXT( "%ld" ), js.rgdwPOV[1] );
	(m_pDlgJoyDebug->GetDlgItem(IDC_POV1))->SetWindowText(strText);
	//SetWindowText( GetDlgItem( m_hDlgJoyDebug, IDC_POV1 ), strText );

	StringCchPrintf( strText, 512, TEXT( "%ld" ), js.rgdwPOV[2] );
	(m_pDlgJoyDebug->GetDlgItem(IDC_POV2))->SetWindowText(strText);
	//SetWindowText( GetDlgItem( m_hDlgJoyDebug, IDC_POV2 ), strText );

	StringCchPrintf( strText, 512, TEXT( "%ld" ), js.rgdwPOV[3] );
	(m_pDlgJoyDebug->GetDlgItem(IDC_POV3))->SetWindowText(strText);
	//SetWindowText( GetDlgItem( m_hDlgJoyDebug, IDC_POV3 ), strText );



	////// Fill up text with which buttons are pressed
	StringCchCopy( strText, 512, TEXT( "" ) );
	for(	int i=0; i < 32; i++ )
	{
		if( js.rgbButtons[i] & 0x80 )
		{
			TCHAR sz[128];
			StringCchPrintf( sz, 128, TEXT( "%02d " ), i );
			StringCchCat( strText, 512, sz );
		}
	}
	(m_pDlgJoyDebug->GetDlgItem(IDC_BUTTONS))->SetWindowText(strText);
	//SetWindowText( GetDlgItem( m_hDlgJoyDebug, IDC_BUTTONS ), strText );
//#endif


	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof( DIPROPRANGE );
	diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = DIPH_BYID; // Specify the enumerated axis
	g_pJoystick->GetProperty(DIPROP_RANGE, &diprg.diph);
	int a=0;

	bool bAlreadyHome = false;
	int 	x = js.lX / 166;		//1000~-1000	div 6//
	int	y = js.lY / 166;

	
	m_x = x; m_y =y;
	//TRACE(_T("m_x = %d; m_y = %d\r\n"),m_x,m_y);
	int nSize =0;

	CONTROLDATA ctrl={0};
	ctrl.channel = 1;
	nSize = 7;

	BOOL bSendStop=false;

	//if (m_pPTZGetInfo->m_bStopCGI)
	//	return 0;

	if((UINT) (joyex.dwButtons & JOY_BUTTON1)) //ZOOM
	{
		ctrl.controlID = PCTL_PTZ_CONT;
		ctrl.payload.ptz_cont.zoom = m_x * -1;
		ctrl.payload.ptz_cont.tilt= 0;
		ctrl.payload.ptz_cont.pan= 0;
		int nSpeed=0;

		// set CGI PTZAction
		m_ptzCGIType = PTZCGITYPE::ZOOM;
		if	(!(m_x == 0 && m_y ==0 ))
		{
			if(m_x <= 0 && m_y <=0)				//(-,-)//
			{
				ptzaction = Z_O;
				//nSpeed = (m_y % 6) -1;
				//TRACE (L"\r\n (-,-) \r\n");
			}
			else if (m_x >= 0 && m_y >= 0)		//(+,+)//
			{
				ptzaction = Z_I;
				//nSpeed = (m_y % 6) +1;
				//TRACE (L"\r\n (+,+) \r\n");
			}
			else																//(+,-) or (-,+)//
			{
				if (m_x > m_y)										//(+,-)// 
				{
					if (abs(m_y) > abs(m_x))
					{
						ptzaction = Z_O;
					}
					else
					{
						ptzaction = Z_I;
					}
					//TRACE (L"\r\n (+,-) \r\n");
				}
				else														   //(-,+)//
				{
					if (abs(m_x) > abs(m_y))
					{
						ptzaction = Z_O;
					}
					else
					{
						ptzaction = Z_I;
					}
					//TRACE (L"\r\n (-,+) \r\n");
				}
			}	
		}
		else
		{
				if ((m_pPTZGetInfo->m_bRunJoyZoom && (m_OldPTZaction == PTZAction::Z_I || m_OldPTZaction == PTZAction::Z_O)))		//PTZCGITYPE::ZOOM
				{
					m_ptzCGIType = PTZCGITYPE::PTZ;
					vPTZMoveCGI(STOP,m_nPTZSpeed);
					TRACE(_T("\r\n JoyStick Send CGI_MOVECONT Stop ZOOM keydown (0,0) PTZSpeed:%d\r\n"),m_nPTZSpeed);
					m_OldPTZaction = ptzaction;
					return 0;
				}

				return 0;			 
		}


		if (ptzaction == Z_I)
		{
			vJoyStickInputConvert2(m_x, m_y);
			//TRACE (L"\r\n F_I \r\n");
		}
		else if (ptzaction == Z_O)
		{
			vJoyStickInputConvert2(m_x, m_y);			
			//TRACE (L"\r\n F_O \r\n");
		}


		if ((m_nJoyStickSpeed == m_nOldJoyStickSpeed && m_OldPTZaction == ptzaction) || ptzaction < 0)
		{
			return 0;
		}

		//vPTZMoveCGI(ptzaction, m_nPTZSpeed);		
		m_ptzAction=ptzaction;
		m_nPTZSpeed=m_nJoyStickSpeed;
		m_bJoystickMove=true;
		if(!m_hPTZCGIThread && !(m_x == 0 && m_y == 0))
		{
			UINT nRet(0);
			m_bJoystickZoom=m_pPTZGetInfo->m_bRunJoyZoom=true;
			m_hPTZCGIThread = (HANDLE) _beginthreadex(NULL,0,thdPTZCGI,(LPVOID)this,0,&nRet);
		}
		m_OldPTZaction=ptzaction;
		m_nOldJoyStickSpeed=m_nJoyStickSpeed;

		CString csZoomSpeed;
		if (ptzaction==Z_I)
		{
			TRACE(_T("\r\nJoy_Z_I : %d\r\n"),m_nJoyStickSpeed);
			csZoomSpeed.Format(L"P: 0 , T: 0 , Z: %d ", m_nJoyStickSpeed);
		}
		else if (ptzaction==Z_O)
		{
			TRACE(_T("\r\nJoy_Z_O : -%d\r\n"),m_nJoyStickSpeed);
			csZoomSpeed.Format(L"P: 0 , T: 0 , Z: -%d ", m_nJoyStickSpeed);
		}
		(m_pDlgJoyDebug->GetDlgItem(IDC_PTZ_SPEED))->SetWindowText(csZoomSpeed);
		return 0;
	}
	else if ((UINT) (joyex.dwButtons & JOY_BUTTON2)) // Focus
	{
		DWORD dwTime=GetTickCount();
		if (m_pPTZGetInfo->m_bActiven)
		{
			m_dwtmJoyStickFOCUSC=dwTime;
			//TRACE(L"\r\n return ptz has Activen \r\n");
			return 0;
		}

		if (m_dwtmJoyStickFOCUSC == 0)
		{
			m_dwtmJoyStickFOCUSC=dwTime;
		}

		if ((dwTime-m_dwtmJoyStickFOCUSC)< 1000 )
		{
			//TRACE(L"\r\n return time delay 1000ms \r\n");
			return 0;
		}
		
		ctrl.controlID = PCTL_FOCUS_CONT;
		ctrl.payload.focus_cont = m_y;
		nSize = 3;
		int nSpeed=0;

		m_ptzCGIType = PTZCGITYPE::FOCUS;
		if (m_pNode->bPTZAutoFocus==1)
		{
			m_toggleAutoFocus.SetPos(-6);
			OnBnClickedAutoFocusBut(0);
			m_bWaitCGI_FOCUSCONT=1;
			SetTimer(TIMER_WaitCGI_AUTOFOCUS,1000,0);
			return 0;
		}

		if	(!(m_x == 0 && m_y ==0 ))
		{
			if(m_x <= 0 && m_y <=0)				//(-,-)//
			{
				ptzaction = F_I;
				//nSpeed = (m_y % 6) -1;
				//TRACE (L"\r\n (-,-) \r\n");
			}
			else if (m_x >= 0 && m_y >= 0)		//(+,+)//
			{
				ptzaction = F_O;
				//nSpeed = (m_y % 6) +1;
				//TRACE (L"\r\n (+,+) \r\n");
			}
			else																//(+,-) or (-,+)//
			{
				if (m_x > m_y)										//(+,-)// 
				{
					if (abs(m_y) > abs(m_x))
					{
						ptzaction = F_I;
					}
					else
					{
						ptzaction = F_O;
					}
					//TRACE (L"\r\n (+,-) \r\n");
				}
				else														   //(-,+)//
				{
					if (abs(m_x) > abs(m_y))
					{
						ptzaction = F_I;
					}
					else
					{
						ptzaction = F_O;
					}
					//TRACE (L"\r\n (-,+) \r\n");
				}
			}	
		}

		else
		{
			if (m_pPTZGetInfo->m_bRunJoyFocus && (m_OldPTZaction == PTZAction::F_I || m_OldPTZaction == PTZAction::F_O))
			{
				m_pPTZGetInfo->m_bFOCUSCONTSend=0;
				KillTimer(TIMER_WaitCGI_AUTOFOCUS);
				m_bWaitCGI_FOCUSCONT=0;
				SetTimer(TIMER_WaitCGI_FOCUSCONT,150,0);
				TRACE(_T("\r\n JoyStick Send CGI_FOCUSCONT Stop FOCUS keydown (0,0) \r\n"));
				m_OldPTZaction=ptzaction;
				return 0;
			}
			return 0;		
		}

		if (ptzaction == F_I)
		{
			vJoyStickInputConvert2(m_x, m_y);
			m_nJoyStickSpeed = -m_nJoyStickSpeed;
			//TRACE (L"\r\n F_I \r\n");
		}
		else if (ptzaction == F_O)
		{
			vJoyStickInputConvert2(m_x, m_y);
			//TRACE (L"\r\n F_O \r\n");
		}


		if ((m_nJoyStickSpeed == m_nOldJoyStickSpeed && m_OldPTZaction == ptzaction) || (ptzaction < 0))
		{
			//m_dwtmJoyStickFOCUSC=dwTime;
			//TRACE(L"\r\n return the same ptzaction \r\n");
			return 0;
		}

		if (ptzaction == m_OldPTZaction && m_OldPTZaction > -1)
		{
			m_pPTZGetInfo->m_bFOCUSCONTSend^=1;
		}
		else
			m_pPTZGetInfo->m_bFOCUSCONTSend=0;


		
		//vPTZMoveCGI(ptzaction, nSpeed);
		//vPTZMoveCGI(ptzaction, 0);
		m_ptzAction=ptzaction;
		m_nPTZSpeed=m_nJoyStickSpeed;
		m_bJoystickMove=true;

		
		if(!m_hPTZCGIThread && m_bWaitCGI_FOCUSCONT== 0 && !(m_x == 0 && m_y == 0))
		{
			UINT nRet(0);
			m_bJoystickFocus=m_pPTZGetInfo->m_bRunJoyFocus=true;
			m_hPTZCGIThread = (HANDLE) _beginthreadex(NULL,0,thdPTZCGI,(LPVOID)this,0,&nRet);
			m_OldPTZaction=ptzaction;
			m_nOldJoyStickSpeed=m_nJoyStickSpeed;
			m_dwtmJoyStickFOCUSC=dwTime;

			CString csFocusSpeed;
			csFocusSpeed.Format(L"%d", m_nJoyStickSpeed);
			(m_pDlgJoyDebug->GetDlgItem(IDC_FOCUS_SPEED))->SetWindowText(csFocusSpeed);
			if (ptzaction == F_I)
			{
				TRACE(_T("\r\nJoy_F_I : %d\r\n"),m_nJoyStickSpeed);
			}
			else if (F_O)
			{
				TRACE(_T("\r\nJoy_F_O : %d\r\n"),m_nJoyStickSpeed);
			}
		}
			
		return 0;
	}
	else
		// Normal move
	{
		ctrl.controlID = PCTL_PTZ_CONT;
		ctrl.payload.ptz_cont.zoom = 0;
		ctrl.payload.ptz_cont.tilt= m_y * -1;
		ctrl.payload.ptz_cont.pan= m_x;
		if (m_y == 0 && m_x == 0)
			bAlreadyHome = true;
		else
			bAlreadyHome = false;

		if (m_pPTZGetInfo->m_bRunJoyFocus || m_pPTZGetInfo->m_bRunJoyZoom)
		{
			bSendStop=true;		//the button0 or button1 is keyup//
		}
		else if (bAlreadyHome)
		{
			m_bJoystickFocus=0;
			if (m_bJoystickZoom && m_pPTZGetInfo->m_bRunJoyZoom == FALSE)
			{
				m_bJoystickZoom=0;
			}						
		}
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

		if ((m_x == 0 && m_y == 0) || bSendStop)
		{
			//TRACE(_T("m_x =m_y = 0; return;\r\n"));
			if((m_bCGISent && m_OldPTZaction > PTZAction::PTZINIT) || bSendStop)// send stop cgi only if sent move cgi before
			{ 
				if ((m_ptzCGIType != PTZCGITYPE::FOCUS && bSendStop == FALSE) || 
					(m_pPTZGetInfo->m_bRunJoyZoom && bSendStop == TRUE && (m_OldPTZaction == PTZAction::Z_I || m_OldPTZaction == PTZAction::Z_O)))		//&& m_ptzCGIType != PTZCGITYPE::ZOOM
				{
					m_ptzCGIType = PTZCGITYPE::PTZ;
					vPTZMoveCGI(STOP,m_nPTZSpeed);
					//if (m_bCanRunCGI)
					//{
					//	m_nPTZSpeed=0;
					//	m_pPTZGetInfo->vQueryCgi(CGIType::CGI_MOVECONT,m_pNode, 0, 0, 0);
						TRACE(_T("\r\n JoyStick Send CGI_MOVECONT Stop PTZSpeed:%d\r\n"),m_nPTZSpeed);
					//}
					//m_bJoystickFocus=0;
					//m_pPTZGetInfo->m_bFOCUSCONTSend=0;
				}
				else if ((m_ptzCGIType == PTZCGITYPE::FOCUS && bSendStop == FALSE) || 
					(m_pPTZGetInfo->m_bRunJoyFocus && bSendStop == TRUE && (m_OldPTZaction == PTZAction::F_I || m_OldPTZaction == PTZAction::F_O)))
				{
					KillTimer(TIMER_WaitCGI_AUTOFOCUS);
					m_bWaitCGI_FOCUSCONT=0;
					////m_pPTZGetInfo->vQueryCgi(CGIType::CGI_FOCUSCONT,m_pNode,0);
					SetTimer(TIMER_WaitCGI_FOCUSCONT,150,0);

					//if (m_bCanRunCGI || m_pPTZGetInfo->m_bRunJoyFocus)
					//{
					//	//Set Stop F_I , F_O//
					//	m_pPTZGetInfo->m_bStopCGI=1;
					//	m_pPTZGetInfo->vQueryCgi(CGIType::CGI_FOCUSCONT,m_pNode,0);
						TRACE(_T("\r\n JoyStick Send CGI_FOCUSCONT Stop \r\n"));
					//}
				}

				m_OldPTZaction=ptzaction;
			}			
			m_bCGISent = false;
			m_nPreInput = 0;
			return 0;
		}

		if (m_bJoystickFocus || m_bJoystickZoom)
			return 0;


		CString csPTZSpeed;
		if(nX < 0 && nY == 0)// pan left
		{
			ptzaction = P_L;
			vJoyStickInputConvert2(nX);
			csPTZSpeed.Format(L"P: -%d , T: 0 , Z: 0 ", m_nJoyStickSpeed);
		}
		if(nX > 0 && nY == 0) // pan right
		{
			ptzaction = P_R;
			vJoyStickInputConvert2(nX);
			csPTZSpeed.Format(L"P: %d , T: 0 , Z: 0 ", m_nJoyStickSpeed);
		}
		if(nX == 0 && nY < 0) // tilt up. NOTICE: nY is up-side down
		{
			ptzaction = T_U;
			vJoyStickInputConvert2(0, nY);
			csPTZSpeed.Format(L"P: 0 , T: %d , Z: 0 ", m_nJoyStickSpeed);
		}
		if(nX == 0 &&  nY >0) //  tilt down
		{
			ptzaction = T_D;
			vJoyStickInputConvert2(0, nY);
			csPTZSpeed.Format(L"P: 0 , T: -%d , Z: 0 ", m_nJoyStickSpeed);
		}
		if(nX > 0 && nY < 0) // pan right tilt up
		{
			ptzaction = PR_TU;
			vJoyStickInputConvert2(nX, nY);
			csPTZSpeed.Format(L"P: %d , T: %d , Z: 0 ", m_nJoyStickSpeed, m_nJoyStickSpeed);
		}
		if (nX > 0 && nY > 0)// pan right tilt down
		{
			ptzaction = PR_TD;			
			vJoyStickInputConvert2(nX, nY);
			csPTZSpeed.Format(L"P: %d , T: -%d , Z: 0 ", m_nJoyStickSpeed, m_nJoyStickSpeed);
		}
		if (nX < 0 && nY > 0)// pan left tilt down
		{
			ptzaction = PL_TD;		
			vJoyStickInputConvert2(nX, nY);
			csPTZSpeed.Format(L"P: -%d , T: -%d , Z: 0 ", m_nJoyStickSpeed, m_nJoyStickSpeed);
		}
		if (nX < 0 && nY < 0)// pan left tilt up
		{
			ptzaction = PL_TU;
			vJoyStickInputConvert2(nX, nY);
			csPTZSpeed.Format(L"P: -%d , T: %d , Z: 0 ", m_nJoyStickSpeed, m_nJoyStickSpeed);
		}

		if ((m_nJoyStickSpeed == m_nOldJoyStickSpeed && m_OldPTZaction == ptzaction) || ptzaction < 0)
		{
			return 0;
		}

		//vPTZMoveCGI(ptzaction, m_nJoyStickSpeed);
		//TRACE(_T("\r\nm_nPreInput =%d\r\n"),m_nPreInput);
		TRACE (L"\r\n JoyStickSpeed: %d \r\n", m_nJoyStickSpeed);

		if (m_OldPTZaction != PTZAction::F_I && m_OldPTZaction != PTZAction::F_O && m_OldPTZaction >-1)
		{
			m_ptzCGIType =	 PTZCGITYPE::PTZ;
		}

		m_ptzAction=ptzaction;
		m_nPTZSpeed=m_nJoyStickSpeed;
		m_bJoystickMove=true;
		if(!m_hPTZCGIThread)
		{
			UINT nRet(0);
			m_hPTZCGIThread = (HANDLE) _beginthreadex(NULL,0,thdPTZCGI,(LPVOID)this,0,&nRet);
		}
		m_OldPTZaction=ptzaction;
		m_nOldJoyStickSpeed=m_nJoyStickSpeed;

		(m_pDlgJoyDebug->GetDlgItem(IDC_PTZ_SPEED))->SetWindowText(csPTZSpeed);
	}

	return S_OK;
}

void CDlgPTZ::vJoyStickInputConvert2(int nX, int nY)
{
	nX=abs(nX);
	nY=abs(nY);
	int nXY=max(nX, nY);
	if (nXY==0)
		nXY=0;

	m_nJoyStickSpeed=nXY;
}
