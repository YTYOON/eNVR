#include "stdafx.h"
#include "DlgAddManageCam.h"
#include "INI.h"
#include "DlgScanNetworkCameras.h"
#include "IPCamTreeView.h"
#include "MainFrm.h"
#include "DlgSettingCamGroup.h"

 
const int MAXBUF = 4096;

// CDlgAddManageCam dialog

IMPLEMENT_DYNAMIC(CDlgAddManageCam, CBaseDialog)

HBRUSH CDlgAddManageCam::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CDlgAddManageCam::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

CDlgAddManageCam::CDlgAddManageCam(int Init, CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAddManageCam::IDD, pParent),m_pBk(NULL)
	, m_nCurHeight(0)
	, m_nScrollPos(0)
	, dirtyFlag(false)
	, initGroupFlag(false)
	, CGIFlag(0)
	, ENCPFlag(false)
	, RecordingFlag(false)
{
	initFlag = Init;
	NODEITEM  Node = {0};
	m_Node = Node;
}

CDlgAddManageCam::CDlgAddManageCam(NODEITEM* Node, int Init, CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAddManageCam::IDD, pParent),m_pBk(NULL)
	, m_nCurHeight(0)
	, m_nScrollPos(0)
	, dirtyFlag(false)
	, initGroupFlag(false)
	, CGIFlag(0)
	, ENCPFlag(false)
	, RecordingFlag(false)
{
	initFlag = Init;
	pNode = Node;
}

CDlgAddManageCam::CDlgAddManageCam(CString TreeNodeText, int Init, CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAddManageCam::IDD, pParent),m_pBk(NULL)
	, m_nCurHeight(0)
	, m_nScrollPos(0)
	, dirtyFlag(false)
	, initGroupFlag(false)
	, CGIFlag(0)
	, ENCPFlag(false)
	, RecordingFlag(false)
{
	initFlag = Init;
	csTreeNodeText = TreeNodeText;
}

CDlgAddManageCam::CDlgAddManageCam(int CameraID, int Init, CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAddManageCam::IDD, pParent),m_pBk(NULL)
	, m_nCurHeight(0)
	, m_nScrollPos(0)
	, dirtyFlag(false)
	, initGroupFlag(false)
	, CGIFlag(0)
	, ENCPFlag(false)
	, RecordingFlag(false)
{
	initFlag = Init;
	iCameraID = CameraID;
}

CDlgAddManageCam::~CDlgAddManageCam()
{
	//if(DBQuery.IsOpen())
	//	DBQuery.Close();
}

void CDlgAddManageCam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAddManageCam, CBaseDialog)

ON_CBN_SELCHANGE(IDC_COMBO_AV_STREAM, &CDlgAddManageCam::OnCbnSelchangeComboAvStream)
ON_BN_CLICKED(IDC_BUTTON_MODIFY_AV_PTZ, &CDlgAddManageCam::OnBnClickedButtonModifyAvPtz)
ON_BN_CLICKED(IDC_BUTTON_SCAN_CAMERA, &CDlgAddManageCam::OnBnClickedButtonScanCamera)
ON_BN_CLICKED(IDC_BUTTON_SAVE_DUPLICATE, &CDlgAddManageCam::OnBnClickedButtonSaveDuplicate)
ON_BN_CLICKED(IDC_BUTTON_DETECT_CAMERA, &CDlgAddManageCam::OnBnClickedButtonDetectCamera)

ON_BN_CLICKED(IDC_CHECK_CAMERA_ENABLE, &CDlgAddManageCam::OnBnClickedCheckCameraEnable)
ON_EN_CHANGE(IDC_EDIT_CAMERA_NAME, &CDlgAddManageCam::OnEnChangeEditCameraName)
ON_BN_CLICKED(IDC_RADIO_DYNAMIC_IP, &CDlgAddManageCam::OnBnClickedRadioDynamicIp)
ON_BN_CLICKED(IDC_RADIO_STATIC_IP, &CDlgAddManageCam::OnBnClickedRadioStaticIp)
ON_BN_CLICKED(IDC_RADIO_PPPOE, &CDlgAddManageCam::OnBnClickedRadioPppoe)
ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_IP_ADDRESS, &CDlgAddManageCam::OnIpnFieldchangedIpaddressIpAddress)
ON_EN_CHANGE(IDC_EDIT_WEB_PORT, &CDlgAddManageCam::OnEnChangeEditWebPort)
ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_SUBNET_MASK, &CDlgAddManageCam::OnIpnFieldchangedIpaddressSubnetMask)
ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_NAME_SERVER, &CDlgAddManageCam::OnIpnFieldchangedIpaddressNameServer)
ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_GATEWAY, &CDlgAddManageCam::OnIpnFieldchangedIpaddressGateway)
ON_EN_CHANGE(IDC_EDIT_USER, &CDlgAddManageCam::OnEnChangeEditUser)
ON_EN_CHANGE(IDC_EDIT_PASSWORD, &CDlgAddManageCam::OnEnChangeEditPassword)
ON_WM_CLOSE()
//ON_BN_CLICKED(IDCANCEL, &CDlgAddManageCam::OnBnClickedCancel)
//ON_BN_CLICKED(IDCANCEL, &CDlgAddManageCam::OnBnClickedCancel)
ON_BN_CLICKED(IDCANCEL, &CDlgAddManageCam::OnBnClickedCancel)
ON_BN_CLICKED(IDOK, &CDlgAddManageCam::OnBnClickedOk)
ON_BN_CLICKED(IDC_BUTTON_RECORDING_DLG, &CDlgAddManageCam::OnBnClickedButtonRecordingDlg)
ON_BN_CLICKED(IDC_BUTTON_GROUP_SETTING, &CDlgAddManageCam::OnBnClickedButtonGroupSetting)
ON_BN_CLICKED(IDC_BUTTON_REMOTE_CAMERA, &CDlgAddManageCam::OnBnClickedButtonRemoteCamera)
ON_BN_CLICKED(IDC_RADIO_RTSP_TCP, &CDlgAddManageCam::OnBnClickedRadioRtspTcp)
ON_BN_CLICKED(IDC_RADIO_RTSP_UDP, &CDlgAddManageCam::OnBnClickedRadioRtspUdp)
//ON_NOTIFY(NM_THEMECHANGED, IDC_COMBO_STREAM_PROTOCOL, &CDlgAddManageCam::OnNMThemeChangedComboStreamProtocol)
ON_CBN_SELCHANGE(IDC_COMBO_STREAM_PROTOCOL, &CDlgAddManageCam::OnCbnSelchangeComboStreamProtocol)
ON_BN_CLICKED(IDC_CHECK_NAS, &CDlgAddManageCam::OnBnClickedCheckNas)
ON_BN_CLICKED(IDC_BTN_NAS, &CDlgAddManageCam::OnBnClickedBtnNas)
ON_BN_CLICKED(IDC_RADIO_RTSP_HTTP, &CDlgAddManageCam::OnBnClickedRadioRtspHttp)
END_MESSAGE_MAP()


// CDlgAddManageCam message handlers

BOOL CDlgAddManageCam::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
	m_pBk = CNVRBk::GetBkObject();
	// TODO:  Add extra initialization here
	CenterWindow();
	HWND hWnd = this->GetSafeHwnd();
	::SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE)|WS_EX_TOPMOST);

	//--Set CStatic Text
	CString csStaticString(_T(""));
	LOADSTRING(csStaticString, IDS_ADD_CAM_CHANNEL_ID);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_CHANNEL_ID);
	((CButton*)GetDlgItem(IDC_STATIC_CHANNEL_ID))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_ENABLED);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_ENABLED);
	((CButton*)GetDlgItem(IDC_STATIC_CAMERA_ENABLE))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_NAME);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_NAME);
	((CButton*)GetDlgItem(IDC_STATIC_CAMERA_NAME))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_LOADING);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_LOADING);
	((CButton*)GetDlgItem(IDC_STATIC_LOADING))->SetWindowTextW(csStaticString);

	//--Network Static Text
	LOADSTRING(csStaticString, IDS_ADD_CAM_NETWORK_GROUP);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_NETWORK_GROUP);
	((CButton*)GetDlgItem(IDC_STATIC_NETWORK_GROUP))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_CONNECTION_TYPE);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_CONNECTION_TYPE);
	((CButton*)GetDlgItem(IDC_STATIC_CONNECTION_TYPE))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_IP);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_IP);
	((CButton*)GetDlgItem(IDC_STATIC_IP_ADDRESS))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_MASK);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_MASK);
	((CButton*)GetDlgItem(IDC_STATIC_SUBNET_MASK))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_DNS);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_DNS);
	((CButton*)GetDlgItem(IDC_STATIC_NAME_SERVER))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_GATEWAY);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_GATEWAY);
	((CButton*)GetDlgItem(IDC_STATIC_GATEWAY))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_PORT);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_PORT);
	((CButton*)GetDlgItem(IDC_STATIC_WEB_PORT))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_MODEL);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_MODEL);
	((CButton*)GetDlgItem(IDC_STATIC_MODEL))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_MAC);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_MAC);
	((CButton*)GetDlgItem(IDC_STATIC_MAC))->SetWindowTextW(csStaticString);

	//--Credentials Static Text
	LOADSTRING(csStaticString, IDS_ADD_CAM_CREDENTIALS_GROUP);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_CREDENTIALS_GROUP);
	((CButton*)GetDlgItem(IDC_STATIC_CREDENTIALS))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_USER);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_USER);
	((CButton*)GetDlgItem(IDC_STATIC_INDIVIDUAL_USER))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_PASSWORD);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_PASSWORD);
	((CButton*)GetDlgItem(IDC_STATIC_INDIVIDUAL_PASSWORD))->SetWindowTextW(csStaticString);

	//--Liveview / Recording Static Text
	LOADSTRING(csStaticString, IDS_ADD_CAM_LIVIEW_RECORDING);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_LIVIEW_RECORDING);
	((CButton*)GetDlgItem(IDC_STATIC_LV_RE))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_LIVEVIEW);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_LIVEVIEW);
	((CButton*)GetDlgItem(IDC_CHECK_LV))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_RECORDING);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_RECORDING);
	((CButton*)GetDlgItem(IDC_CHECK_RE))->SetWindowTextW(csStaticString);

	//--Video Static Text
	LOADSTRING(csStaticString, IDS_ADD_CAM_VIDEO_GROUP);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_VIDEO_GROUP);
	((CButton*)GetDlgItem(IDC_STATIC_AV_GROUP))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_STREAM);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_STREAM);
	((CButton*)GetDlgItem(IDC_STATIC_AV_STREAM))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_VIDEO_CMPR_TYPE);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_VIDEO_CMPR_TYPE);
	((CButton*)GetDlgItem(IDC_STATIC_VIDEO_TYPE))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_RESOLUTION);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_RESOLUTION);
	((CButton*)GetDlgItem(IDC_STATIC_RESOLUTION))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_PROTOCOL);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_PROTOCOL);
	((CButton*)GetDlgItem(IDC_STATIC_PROTOCOL))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_VIDEO_PORT);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_VIDEO_PORT);
	((CButton*)GetDlgItem(IDC_STATIC_PORT))->SetWindowTextW(csStaticString);

	//--PTZ/DI/DO Static Text
	LOADSTRING(csStaticString, IDS_ADD_CAM_PTZ_DIDO_GROUP);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_PTZ_DIDO_GROUP);
	((CButton*)GetDlgItem(IDC_STATIC_PTZ_DIDO_GROUP))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_PTZ);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_PTZ);
	((CButton*)GetDlgItem(IDC_STATIC_PTZ))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_DI_1);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_DI_1);
	((CButton*)GetDlgItem(IDC_STATIC_DI))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_DI_2);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_DI_2);
	((CButton*)GetDlgItem(IDC_STATIC_DI2))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_DO);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_DO);
	((CButton*)GetDlgItem(IDC_STATIC_DO))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_PTZ_PROTOCOL);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_PTZ_PROTOCOL);
	((CButton*)GetDlgItem(IDC_STATIC_PTZ_PROTOCOL))->SetWindowTextW(csStaticString);

	//--Event Static Text
	LOADSTRING(csStaticString, IDS_ADD_CAM_EVENT_GROUP);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_EVENT_GROUP);
	((CButton*)GetDlgItem(IDC_STATIC_EVENT_GROUP))->SetWindowTextW(csStaticString);

	//--Group Static Text
	LOADSTRING(csStaticString, IDS_ADD_CAM_GROUPS_GROUP);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_GROUPS_GROUP);
	((CButton*)GetDlgItem(IDC_STATIC_GROUPS_GROUP))->SetWindowTextW(csStaticString);

	//--Button Static Text
	LOADSTRING(csStaticString, IDS_ADD_CAM_SCAN_CAMS);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_SCAN_CAMS);
	((CButton*)GetDlgItem(IDC_BUTTON_SCAN_CAMERA))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_GET_CAMS);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_GET_CAMS);
	((CButton*)GetDlgItem(IDC_BUTTON_DETECT_CAMERA))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_MODIFY_VIDEO_PTZ);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_MODIFY_VIDEO_PTZ);
	((CButton*)GetDlgItem(IDC_BUTTON_MODIFY_AV_PTZ))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_EVENT_SETTING);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_EVENT_SETTING);
	((CButton*)GetDlgItem(IDC_BUTTON_EVENT_SETTING))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_GROUP_SETTING);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_GROUP_SETTING);
	((CButton*)GetDlgItem(IDC_BUTTON_GROUP_SETTING))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_APPLY);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_APPLY);
	((CButton*)GetDlgItem(IDC_BUTTON_APPLY))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_ADD_CAM_SAVE_DUPLICATE);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_SAVE_DUPLICATE);
	((CButton*)GetDlgItem(IDC_BUTTON_SAVE_DUPLICATE))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_OK);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_OK);
	((CButton*)GetDlgItem(IDOK))->SetWindowTextW(csStaticString);

	LOADSTRING(csStaticString, IDS_CANCEL);
	//csStaticString.LoadStringW(CGS::g_hResStr, IDS_CANCEL);
	((CButton*)GetDlgItem(IDCANCEL))->SetWindowTextW(csStaticString);


	//vDBConnection();
	CString csDialogCaption(_T(""));
	LOADSTRING(csDialogCaption, IDS_DLG_ADD);
	//csDialogCaption.LoadString(CGS::g_hResStr, IDS_DLG_ADD);
	this->SetWindowTextW(csDialogCaption);

	//Hide the save & duplicate button
	GetDlgItem(IDC_BUTTON_SAVE_DUPLICATE)->ShowWindow(0);
	
	//--Initialize Gateway, DNS, Sub NetMask, StreamSum
	vIPCamInfo.csNetmask = _T("");
	vIPCamInfo.csGateway = _T("");
	vIPCamInfo.csDNS = _T("");
	veOrgInfo.push_back(_T(""));
	vIPCamInfo.csStreamSum = _T("1");
	
	//--Forcibly Check LiveView
	//((CButton*)(GetDlgItem( IDC_CHECK_LV )))->EnableWindow(0);

	//--For Modify_IP_Cam
	CString csMac;

	DBIPCamGroupVector.clear();
	//m_pvtIPCamGroup = &g_vtIPCamGroup;

	switch(this->initFlag)
	{
	case ADDCAMERA:
			
			//--Set Channel ID
			GetDlgItem(IDC_EDIT_CHANNEL_ID)->SetWindowText( csGetChannelID() );

			//--Camera Enabled Check
			((CButton*)(GetDlgItem( IDC_CHECK_CAMERA_ENABLE )))->SetCheck( 1 );
			//((CButton*)(GetDlgItem( IDC_CHECK_LV )))->SetCheck( 1 );
			((CButton*)(GetDlgItem( IDC_CHECK_RE )))->SetCheck( 1 );
			

			//--Check Default Group
			vGroupListInit();
			vCheckDefaultGroup();
			//for(int iTemp = 0; iTemp < DBIPCamGroupVector.size(); iTemp++)
			//{
				//vCheckDefaultGroup(DBIPCamGroupVector[iTemp]);
			//vCheckDefaultGroup(g_nRootCameraGroupID);

			//}

			GetDlgItem(IDC_STATIC_RTSPOVER)->ShowWindow(0);
			GetDlgItem(IDC_RADIO_RTSP_UDP)->ShowWindow(0);
			GetDlgItem(IDC_RADIO_RTSP_TCP)->ShowWindow(0);
			GetDlgItem(IDC_RADIO_RTSP_HTTP)->ShowWindow(0);

		break;

	case ADD_NEW_IP_CAM_GROUP:
			
			//--Set Channel ID
			GetDlgItem(IDC_EDIT_CHANNEL_ID)->SetWindowText( csGetChannelID() );

			//--Camera Enabled Check
			((CButton*)(GetDlgItem( IDC_CHECK_CAMERA_ENABLE )))->SetCheck( 1 );
			//((CButton*)(GetDlgItem( IDC_CHECK_LV )))->SetCheck( 1 );
			((CButton*)(GetDlgItem( IDC_CHECK_RE )))->SetCheck( 1 );

			
			


			//--Check Default Group
			vGroupListInit();
			//vCheckDefaultGroup(csTreeNodeText);
			//vCheckDefaultGroup(g_nRootCameraGroupID);

			GetDlgItem(IDC_STATIC_RTSPOVER)->ShowWindow(0);
			GetDlgItem(IDC_RADIO_RTSP_UDP)->ShowWindow(0);
			GetDlgItem(IDC_RADIO_RTSP_TCP)->ShowWindow(0);
			GetDlgItem(IDC_RADIO_RTSP_HTTP)->ShowWindow(0);

		break;

	case ADD_NEW_IP_CAM_NODE:

			//--Set Channel ID
			GetDlgItem(IDC_EDIT_CHANNEL_ID)->SetWindowText( csGetChannelID() );

			//--Camera Enabled Check
			((CButton*)(GetDlgItem( IDC_CHECK_CAMERA_ENABLE )))->SetCheck( 1 );
			//((CButton*)(GetDlgItem( IDC_CHECK_LV )))->SetCheck( 1 );
			((CButton*)(GetDlgItem( IDC_CHECK_RE )))->SetCheck( 1 );

			//--Check Default Group
			vGroupListInit();
			//vCheckDefaultGroup(csTreeNodeText);
			//vCheckDefaultGroup(g_nRootCameraGroupID);

			GetDlgItem(IDC_STATIC_RTSPOVER)->ShowWindow(0);
			GetDlgItem(IDC_RADIO_RTSP_UDP)->ShowWindow(0);
			GetDlgItem(IDC_RADIO_RTSP_TCP)->ShowWindow(0);
			GetDlgItem(IDC_RADIO_RTSP_HTTP)->ShowWindow(0);

		break;

	case MODIFY_IP_CAM:

			//--Disable Button "Scan Netwrok Camera"
			((CButton*)(GetDlgItem( IDC_BUTTON_SCAN_CAMERA )))->EnableWindow(false);

			//--Get IP Cam Data From DB
			vGetIPCamDataFromDB(iCameraID);

			//--Set Data to UI
			GetDlgItem(IDC_EDIT_CHANNEL_ID)->SetWindowText( vIPCamInfo.csCameraIDX );		//Channel ID

			if(vIPCamInfo.csActive == _T("Y"))												//Camera Enable
				((CButton*)(GetDlgItem( IDC_CHECK_CAMERA_ENABLE )))->SetCheck( 1 );
			else
				((CButton*)(GetDlgItem( IDC_CHECK_CAMERA_ENABLE )))->SetCheck( 0 );
			
			GetDlgItem(IDC_EDIT_CAMERA_NAME)->SetWindowText( vIPCamInfo.csCameraName );	//Camera Name

			this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->SetWindowTextW(vIPCamInfo.csIP);	//IP Address

			if(vIPCamInfo.csConnectType.TrimRight() == _T("DHCP"))					//Connection Type
			{
				((CButton*)(GetDlgItem( IDC_RADIO_DYNAMIC_IP )))->SetCheck( 1 );
				this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->EnableWindow(0);
				this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->EnableWindow(0);
				this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->EnableWindow(0);
				this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(0);
			}
			else if(vIPCamInfo.csConnectType.TrimRight() == _T("Static IP"))
			{
				((CButton*)(GetDlgItem( IDC_RADIO_STATIC_IP )))->SetCheck( 1 );
				this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->EnableWindow(1);
				this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->EnableWindow(1);
				this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->EnableWindow(1);
				this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(1);
			}
			else if(vIPCamInfo.csConnectType.TrimRight() == _T("PPPoE"))
			{
				((CButton*)(GetDlgItem( IDC_RADIO_PPPOE )))->SetCheck( 1 );
				this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->EnableWindow(1);
				this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->EnableWindow(1);
				this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->EnableWindow(1);
				this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(1);
			}
			else	//--Connect Type Unknown
			{
				this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->EnableWindow(0);
				this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->EnableWindow(0);
				this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->EnableWindow(0);
				this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(0);
			}

			this->GetDlgItem(IDC_EDIT_WEB_PORT)->SetWindowTextW( vIPCamInfo.csWebPort );	//Set Web Port

			GetDlgItem(IDC_EDIT_MODEL)->SetWindowText( vIPCamInfo.csModel );				//Model

			if(vIPCamInfo.csNetmask == _T(""))
				vIPCamInfo.csNetmask = _T("0.0.0.0");
			this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->SetWindowTextW(vIPCamInfo.csNetmask);	//Set Subnet Mask

			if(vIPCamInfo.csGateway == _T(""))
				vIPCamInfo.csGateway = _T("0.0.0.0");
			this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->SetWindowTextW(vIPCamInfo.csGateway);	//Gateway
			
			if(vIPCamInfo.csDNS == _T(""))
				vIPCamInfo.csDNS = _T("0.0.0.0");
			this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->SetWindowTextW(vIPCamInfo.csDNS);	//Name Server

			csMac = vIPCamInfo.csMac;										//Mac Address
			csMac.Insert(2, _T("-"));
			csMac.Insert(5, _T("-"));
			csMac.Insert(8, _T("-"));
			csMac.Insert(11, _T("-"));
			csMac.Insert(14, _T("-"));
			this->GetDlgItem(IDC_EDIT_MAC)->SetWindowTextW(csMac);			

			//((CButton*)(GetDlgItem( IDC_RADIO_INDIVIDUAL_PASSWORD )))->SetCheck( 1 );				//Individual User & Passwrod

			this->GetDlgItem(IDC_EDIT_USER)->SetWindowTextW(vIPCamInfo.csUser);		//User
			this->GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowTextW(vIPCamInfo.csPWD);	//Password
			
			if(vIPCamInfo.csRS485Enable == _T("Y"))										//PTZ Enalbe
				GetDlgItem(IDC_EDIT_PTZ_ENABLE)->SetWindowTextW(_T("Enable"));
			else
				GetDlgItem(IDC_EDIT_PTZ_ENABLE)->SetWindowTextW(_T("Disable"));

			GetDlgItem(IDC_EDIT_PTZ_PROTOCOL)->SetWindowTextW( vIPCamInfo.csRS485Protocol );		//Set PTZ Protocol		

			if(vIPCamInfo.csRS485In1 == _T("on"))										//Digital Input 1
				GetDlgItem(IDC_EDIT_DI_1)->SetWindowTextW( _T("Normal Open") );
			else
				GetDlgItem(IDC_EDIT_DI_1)->SetWindowTextW( _T("Normal Close") );

			if(vIPCamInfo.csRS485In2 == _T("on"))										//Digital Input 2
				GetDlgItem(IDC_EDIT_DI_2)->SetWindowTextW( _T("Normal Open") );
			else
				GetDlgItem(IDC_EDIT_DI_2)->SetWindowTextW( _T("Normal Close") );

			if(vIPCamInfo.csRS485Out1 == _T("on"))										//Digital Output
				GetDlgItem(IDC_EDIT_DO)->SetWindowTextW( _T("Normal Open") );
			else
				GetDlgItem(IDC_EDIT_DO)->SetWindowTextW( _T("Normal Close") );

			//--Get Stream Data From DB
			GetDlgItem(IDC_BUTTON_MODIFY_AV_PTZ)->EnableWindow(1);
			vGetStreamDataFromDB(iCameraID);	//Get Stream Data
			vIPCamInfo.csStreamSum.Format(_T("%d"), m_veMoStreamInfo.size());
			StreamInit();
			vGetStreamDataFromVector(1);

			if(vStreamInfo.csStreamVideoCompressionType.Find(_T("H264"), 0) != -1)					//Set Video Compression Type
				GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("H264") );
			else if(vStreamInfo.csStreamVideoCompressionType.Find(_T("MPEG4"), 0) != -1)
				GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("MPEG4") );
			else if(vStreamInfo.csStreamVideoCompressionType.Find(_T("MJPEG"), 0) != -1)
				GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("MJPEG") );
			else
				GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("Unknown") );
		
			GetDlgItem(IDC_EDIT_AV_PORT)->SetWindowText( vStreamInfo.csStreamPort );				//Video Port

			GetDlgItem(IDC_EDIT_AV_PROTOCOL)->SetWindowTextW( vStreamInfo.csStreamProtocol );		//Set Protocol

			GetDlgItem(IDC_EDIT_AV_RESOLUTION)->SetWindowTextW( vStreamInfo.csStreamResolution );	//Set Resolution

			//--Stream Liveview and Recording
			for(int iTemp = 0; iTemp < m_veMoStreamInfo.size(); iTemp++)
			{
				vGetStreamDataFromVector(iTemp+1);
				if( vStreamInfo.csLV_RE.Find(_T("LV"), 0) != -1 )
				{
					//((CButton*)GetDlgItem(IDC_CHECK_LV))->SetCheck(1);

					CComboBox* pLVStream =(CComboBox*) GetDlgItem(IDC_COMBO_LV);			//Set LV Stream
					pLVStream->SetCurSel(iTemp);
				}

				if( vStreamInfo.csLV_RE.Find(_T("RE"), 0) != -1 )
				{
					((CButton*)GetDlgItem(IDC_CHECK_RE))->SetCheck(1);
					

					CComboBox* pREStream =(CComboBox*) GetDlgItem(IDC_COMBO_RE);			//Set RE Stream
					pREStream->SetCurSel(iTemp);
				}
			}

			//--Check Default Group
			csMacExit.Format(_T("%d"), iCameraID);
			vGetIPCamGroup( csMacExit );
			if(initGroupFlag == false)
			{
				vGroupListInit();
				initGroupFlag = true;
			}
			
			//for(int iTemp = 0; iTemp < DBIPCamGroupVector.size(); iTemp++)
			//{
				//vCheckDefaultGroup(DBIPCamGroupVector[iTemp]);
			//vCheckDefaultGroup(g_nRootCameraGroupID);

			//}
			//GetDlgItem(IDC_BUTTON_GROUP_SETTING)->EnableWindow(1);

			dirtyFlag = false;

			//--Save Mac, Stream Protocols From DB
			veOrgInfo.clear();
			veOrgInfo.push_back(csMac);
			for(int iTemp = 0; iTemp < ::_ttoi(vIPCamInfo.csStreamSum); iTemp++)
			{	veOrgInfo.push_back(m_veMoStreamInfo[iTemp].tcProtocol);}

			//--Enable Setting Recording, Group Button
			((CButton*)GetDlgItem(IDC_BUTTON_RECORDING_DLG))->EnableWindow(1);
			((CButton*)GetDlgItem(IDC_BUTTON_GROUP_SETTING))->EnableWindow(1);

			vInitStreamProtocol();
			
			vSetStreamPTZUI();

		break;

	case MODIFY_NVR_IP_CAM:

			//--Disable Button "Scan Netwrok Camera" 
			((CButton*)(GetDlgItem( IDC_BUTTON_SCAN_CAMERA )))->EnableWindow(false);
			

			//--Get IP Cam Data From DB
			vGetIPCamDataFromDB(iCameraID);

			//--Set Data to UI
			GetDlgItem(IDC_EDIT_CHANNEL_ID)->SetWindowText( vIPCamInfo.csCameraIDX );		//Channel ID

			if(vIPCamInfo.csActive == _T("Y"))												//Camera Enable
				((CButton*)(GetDlgItem( IDC_CHECK_CAMERA_ENABLE )))->SetCheck( 1 );
			else
				((CButton*)(GetDlgItem( IDC_CHECK_CAMERA_ENABLE )))->SetCheck( 0 );

			GetDlgItem(IDC_EDIT_CAMERA_NAME)->SetWindowText( vIPCamInfo.csCameraName );		//Camera Name
			GetDlgItem(IDC_EDIT_CAMERA_NAME)->EnableWindow(0);

			this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->SetWindowTextW(vIPCamInfo.csIP);	//IP Address
			this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->EnableWindow(0);

			((CButton*)(GetDlgItem( IDC_RADIO_DYNAMIC_IP )))->SetCheck( 1 );				//Connection Type
			this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->EnableWindow(0);
			this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->EnableWindow(0);
			this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->EnableWindow(0);
			this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(0);

			((CButton*)(GetDlgItem( IDC_RADIO_DYNAMIC_IP )))->EnableWindow(0);
			((CButton*)(GetDlgItem( IDC_RADIO_STATIC_IP )))->EnableWindow(0);
			((CButton*)(GetDlgItem( IDC_RADIO_PPPOE )))->EnableWindow(0);

			this->GetDlgItem(IDC_EDIT_WEB_PORT)->SetWindowTextW( vIPCamInfo.csWebPort );	//Set Web Port
			this->GetDlgItem(IDC_EDIT_WEB_PORT)->EnableWindow(0);

			GetDlgItem(IDC_EDIT_MODEL)->SetWindowText( vIPCamInfo.csModel );			//Model
			GetDlgItem(IDC_EDIT_MODEL)->EnableWindow(0);

			if(vIPCamInfo.csNetmask == _T(""))
				vIPCamInfo.csNetmask = _T("0.0.0.0");
			this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->SetWindowTextW(vIPCamInfo.csNetmask);	//Set Subnet Mask

			if(vIPCamInfo.csGateway == _T(""))
				vIPCamInfo.csGateway = _T("0.0.0.0");
			this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->SetWindowTextW(vIPCamInfo.csGateway);	//Gateway
			
			if(vIPCamInfo.csDNS == _T(""))
				vIPCamInfo.csDNS = _T("0.0.0.0");
			this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->SetWindowTextW(vIPCamInfo.csDNS);	//Name Server

			csMac = vIPCamInfo.csMac;										//Mac Address
			csMac.Insert(2, _T("-"));
			csMac.Insert(5, _T("-"));
			csMac.Insert(8, _T("-"));
			csMac.Insert(11, _T("-"));
			csMac.Insert(14, _T("-"));
			this->GetDlgItem(IDC_EDIT_MAC)->SetWindowTextW(csMac);
			this->GetDlgItem(IDC_EDIT_MAC)->EnableWindow(0);

			this->GetDlgItem(IDC_EDIT_USER)->SetWindowTextW(vIPCamInfo.csUser);		//User
			this->GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowTextW(vIPCamInfo.csPWD);	//Password

			//--Disable Button "Get Camera Info."
			((CButton*)(GetDlgItem( IDC_BUTTON_DETECT_CAMERA )))->EnableWindow(false);

			if(vIPCamInfo.csRS485Enable == _T("Y"))										//PTZ Enalbe
				GetDlgItem(IDC_EDIT_PTZ_ENABLE)->SetWindowTextW(_T("Enable"));
			else
				GetDlgItem(IDC_EDIT_PTZ_ENABLE)->SetWindowTextW(_T("Disable"));

			if( vIPCamInfo.csRS485Protocol.GetLength() == 0 )
				GetDlgItem(IDC_EDIT_PTZ_PROTOCOL)->SetWindowTextW(_T("None"));
			else
				GetDlgItem(IDC_EDIT_PTZ_PROTOCOL)->SetWindowTextW( vIPCamInfo.csRS485Protocol );		//Set PTZ Protocol		

			if(vIPCamInfo.csRS485In1 == _T("on"))										//Digital Input 1
				GetDlgItem(IDC_EDIT_DI_1)->SetWindowTextW( _T("Normal Open") );
			else
				GetDlgItem(IDC_EDIT_DI_1)->SetWindowTextW( _T("Normal Close") );

			if(vIPCamInfo.csRS485In2 == _T("on"))										//Digital Input 2
				GetDlgItem(IDC_EDIT_DI_2)->SetWindowTextW( _T("Normal Open") );
			else
				GetDlgItem(IDC_EDIT_DI_2)->SetWindowTextW( _T("Normal Close") );

			if(vIPCamInfo.csRS485Out1 == _T("on"))										//Digital Output
				GetDlgItem(IDC_EDIT_DO)->SetWindowTextW( _T("Normal Open") );
			else
				GetDlgItem(IDC_EDIT_DO)->SetWindowTextW( _T("Normal Close") );

			//--Get Stream Data From DB
			GetDlgItem(IDC_BUTTON_MODIFY_AV_PTZ)->EnableWindow(1);
			vGetStreamDataFromDB(iCameraID);	//Get Stream Data
			vIPCamInfo.csStreamSum.Format(_T("%d"), m_veMoStreamInfo.size());
			StreamInit();
			vGetStreamDataFromVector(1);

			if(vStreamInfo.csStreamVideoCompressionType.Find(_T("H264"), 0) != -1)					//Set Video Compression Type
				GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("H264") );
			else if(vStreamInfo.csStreamVideoCompressionType.Find(_T("MPEG4"), 0) != -1)
				GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("MPEG4") );
			else if(vStreamInfo.csStreamVideoCompressionType.Find(_T("MJPEG"), 0) != -1)
				GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("MJPEG") );
			else
				GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("Unknown") );
		
			GetDlgItem(IDC_EDIT_AV_PORT)->SetWindowText( vStreamInfo.csStreamPort );				//Video Port

			GetDlgItem(IDC_EDIT_AV_PROTOCOL)->SetWindowTextW( vStreamInfo.csStreamProtocol );		//Set Protocol

			if( vStreamInfo.csStreamProtocol.Find(_T("RTSP")) != -1 )
			{
				GetDlgItem(IDC_STATIC_RTSPOVER)->ShowWindow(1);
				GetDlgItem(IDC_RADIO_RTSP_UDP)->ShowWindow(1);
				GetDlgItem(IDC_RADIO_RTSP_TCP)->ShowWindow(1);
				GetDlgItem(IDC_RADIO_RTSP_HTTP)->ShowWindow(1);

				if(vStreamInfo.csStreamProtocol.Find(_T("TCP")) != -1)
				{
					((CButton*)GetDlgItem(IDC_RADIO_RTSP_UDP))->SetCheck(0);
					((CButton*)GetDlgItem(IDC_RADIO_RTSP_TCP))->SetCheck(1);
                    ((CButton*)GetDlgItem(IDC_RADIO_RTSP_HTTP))->SetCheck(0);                    
				}
				else if(vStreamInfo.csStreamProtocol.Find(_T("HTTP")) != -1)
				{
					((CButton*)GetDlgItem(IDC_RADIO_RTSP_UDP))->SetCheck(0);
					((CButton*)GetDlgItem(IDC_RADIO_RTSP_TCP))->SetCheck(0);
                    ((CButton*)GetDlgItem(IDC_RADIO_RTSP_HTTP))->SetCheck(1);                    
				}
				else
				{
					((CButton*)GetDlgItem(IDC_RADIO_RTSP_UDP))->SetCheck(1);
					((CButton*)GetDlgItem(IDC_RADIO_RTSP_TCP))->SetCheck(0);
                    ((CButton*)GetDlgItem(IDC_RADIO_RTSP_HTTP))->SetCheck(0);                    
				}
			}
			else
			{
				GetDlgItem(IDC_STATIC_RTSPOVER)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_RTSP_UDP)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_RTSP_TCP)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_RTSP_HTTP)->ShowWindow(SW_HIDE);
			}

			if( vStreamInfo.csStreamResolution.GetLength() == 0 )
			{
				GetDlgItem(IDC_EDIT_AV_RESOLUTION)->SetWindowTextW(_T("Unknown"));
			}
			else
			{
				GetDlgItem(IDC_EDIT_AV_RESOLUTION)->SetWindowTextW( vStreamInfo.csStreamResolution );	//Set Resolution
			}

			//--Stream Liveview and Recording
			for(int iTemp = 0; iTemp < m_veMoStreamInfo.size(); iTemp++)
			{
				vGetStreamDataFromVector(iTemp+1);
				if( vStreamInfo.csLV_RE.Find(_T("LV"), 0) != -1 )
				{
					//((CButton*)GetDlgItem(IDC_CHECK_LV))->SetCheck(1);

					CComboBox* pLVStream =(CComboBox*) GetDlgItem(IDC_COMBO_LV);			//Set LV Stream
					pLVStream->SetCurSel(iTemp);
				}

				if( vStreamInfo.csLV_RE.Find(_T("RE"), 0) != -1 )
				{
					((CButton*)GetDlgItem(IDC_CHECK_RE))->SetCheck(1);
					

					CComboBox* pREStream =(CComboBox*) GetDlgItem(IDC_COMBO_RE);			//Set RE Stream
					pREStream->SetCurSel(iTemp);
				}
			}
			GetDlgItem(IDC_CHECK_RE)->EnableWindow(0);
			GetDlgItem(IDC_COMBO_RE)->EnableWindow(0);
			//((CButton*)GetDlgItem(IDC_CHECK_LV))->SetCheck(1);

			//------------------Check Default Group
			csMacExit.Format(_T("%d"), iCameraID);
			vGetIPCamGroup( csMacExit );
			if(initGroupFlag == false)
			{
				vGroupListInit();
				initGroupFlag = true;
			}
			
			for(int iTemp = 0; iTemp < DBIPCamGroupVector.size(); iTemp++)
			{
				//vCheckDefaultGroup(DBIPCamGroupVector[iTemp]);
				vCheckDefaultGroup(DBIPCamGroupVector[iTemp].second);
			}
			//GetDlgItem(IDC_BUTTON_GROUP_SETTING)->EnableWindow(1);

			dirtyFlag = false;

			vInitStreamProtocol();
			
			vSetStreamPTZUI();

		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CString CDlgAddManageCam::csGetChannelID()
{
	CString csChannelID;
	int nChannelID = m_pBk->GetLastCameraIdx() + 1;
	csChannelID.Format(_T("%d"), nChannelID);

	return csChannelID;
}

void CDlgAddManageCam::vGroupListInit()
{
	//------------- Group List Initial
	CListCtrl * pGroupList = (CListCtrl *)this->GetDlgItem(IDC_LIST_GROUP);

	//pGroupList->SetExtendedStyle(pGroupList->GetStyle()|LVS_EX_CHECKBOXES);
	
	LONG lStyle;
	lStyle = ::GetWindowLong(pGroupList->m_hWnd, GWL_STYLE);
	lStyle&=~LVS_TYPEMASK;
	lStyle|=LVS_REPORT;
	::SetWindowLong(pGroupList->m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = pGroupList->GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |=LVS_EX_CHECKBOXES;
	pGroupList->SetExtendedStyle(dwStyle);

	if(initGroupFlag == false)
	{

		LV_COLUMN ListColumn;
		WCHAR * ListTitles[2] = {_T(""), _T("Group Description")};
		for(int i = 0; i < 2; i++)
		{
			ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			ListColumn.fmt = LVCFMT_LEFT;
			ListColumn.iSubItem = i;

			if(i == 0 )
			{ListColumn.cx = 30;}
			else
			{ListColumn.cx = 280;}
			
			ListColumn.pszText = ListTitles[i];
			pGroupList->InsertColumn(i, &ListColumn);
		}

		pair<int , CString> pairGroup;
		veGroupList.clear();
		vector<Cam_Group> vcGroup;
		m_pBk->GetGroup(DeviceTree, vcGroup);

		for(int iTemp = 0; iTemp < vcGroup.size(); iTemp++)
		{
			pairGroup.first = vcGroup[iTemp].cameragroupid;
			pairGroup.second = vcGroup[iTemp].camera_group_desc;

			int nItem = pGroupList->GetItemCount();
			pGroupList->InsertItem(nItem, _T(""), 0);
			pGroupList->SetItemText(nItem, 1, vcGroup[iTemp].camera_group_desc);

			veGroupList.push_back(pairGroup);
		}

		initGroupFlag = true;
	}
}

void CDlgAddManageCam::vCheckDefaultGroup(CString m_csTreeNodeText)
{
	CListCtrl * pGroupList = (CListCtrl *)this->GetDlgItem(IDC_LIST_GROUP);

	for(int iTemp = 0; iTemp < pGroupList->GetItemCount(); iTemp++)
	{
		if(pGroupList->GetItemText(iTemp, 1) == m_csTreeNodeText)
			pGroupList->SetCheck(iTemp, 1);
		//else
		//	pGroupList->SetCheck(iTemp, 0);
	}
}

void CDlgAddManageCam::vCheckDefaultGroup(int nRootGroupID)
{
	//DBIPCamGroupVector// right menu from IPTree//
	CListCtrl * pGroupList = (CListCtrl *)this->GetDlgItem(IDC_LIST_GROUP);
	
	if (nRootGroupID >0 && veGroupList.size() >0)
	{
		for(int iTemp = 0; iTemp < veGroupList.size(); iTemp++)
		{
			if(veGroupList[iTemp].first == nRootGroupID)
				pGroupList->SetCheck(iTemp, 1);

			if (initFlag == MODIFY_IP_CAM || initFlag == MODIFY_NVR_IP_CAM)
			{
				for(int iModify = 0; iModify < DBIPCamGroupVector.size(); iModify++)
				{
					if (veGroupList[iTemp].first == DBIPCamGroupVector[iModify].first)
					{
						pGroupList->SetCheck(iTemp, 1);
						break;
					}
				}				
			}
		}
	}	
}


void CDlgAddManageCam::vGetIPCamDataFromDB(int iCameraID)
{
	ec_Camera camObj;
	CString csNetMask1, csNetMask2, csNetMask3, csNetMask4;
	m_pBk->GetCamera(iCameraID, camObj);

	vIPCamInfo.csCameraIDX.Format(_T("%d"), camObj.camera_idx);
	vIPCamInfo.csCameraName = camObj.cameraname;
	vIPCamInfo.csConnectType = camObj.connect_type_code;
	vIPCamInfo.csIP = camObj.ipaddress;
	vIPCamInfo.csWebPort.Format(_T("%d"), camObj.httpport);
	vIPCamInfo.csGateway = camObj.gateway;
	vIPCamInfo.csDNS = camObj.name_server;
	vIPCamInfo.csMac = camObj.mac_address;
	vIPCamInfo.csUser = camObj.username;
	vIPCamInfo.csPWD = camObj.password;
	vIPCamInfo.csModel = camObj.model_code;
	vIPCamInfo.csRS485Enable = camObj.ptz_support;
	vIPCamInfo.csRS485Protocol = camObj.ptz_protocol;
	vIPCamInfo.csRS485In1 = camObj.digital_in1;
	vIPCamInfo.csRS485In2 = camObj.digital_in2;
	vIPCamInfo.csRS485Out1 = camObj.digital_out;
	vIPCamInfo.csSpeaker = camObj.speaker_support;
	vIPCamInfo.csMicrophone = camObj.mic_support;
	vIPCamInfo.csNetmask = camObj.subnet_mask1 + "." + camObj.subnet_mask2 + "." + camObj.subnet_mask3 + "." + camObj.subnet_mask4;
	vIPCamInfo.csStreamSum.Format(_T("%d"),camObj.vcStream.size());
	vIPCamInfo.csActive = camObj.active_;
	vIPCamInfo.csStreamUrl = camObj.stream_url;
}

void CDlgAddManageCam::vGetStreamDataFromDB(int iCameraID)
{
	CString csLV_RE, csStreamPort, csStreamProtocol, csStreamVideoCompressionType, csStreamResolution;
	
	if(vIPCamInfo.csStreamSum == _T("0"))
		vIPCamInfo.csStreamSum = _T("1");

	vector<ec_Stream> vcStream;
	m_pBk->GetStream(iCameraID, vcStream);
	CString str;

	int nIdx = 0, nCount = vcStream.size();
	for (nIdx=0; nIdx < nCount; nIdx++)
	{
		str.Format(_T("%d"), vcStream[nIdx].stream_port);
		_tcscpy(m_MoStreamInfo.tcPort, str);
		_tcscpy(m_MoStreamInfo.tcProtocol, vcStream[nIdx].stream_protocol);
		_tcscpy(m_MoStreamInfo.tcResolution, vcStream[nIdx].video_res);
		_tcscpy(m_MoStreamInfo.tcVideoType, vcStream[nIdx].video_cmpr_type);
		_tcscpy(m_MoStreamInfo.tcLV_RE, vcStream[nIdx].stream_type);

		m_veMoStreamInfo.push_back(m_MoStreamInfo);
	}
}

void CDlgAddManageCam::vGetStreamDataFromVector(int iStreamNum)
{
	if(m_veMoStreamInfo.size() == 0)
	{
		vStreamInfo.csLV_RE =_T("");
		vStreamInfo.csStreamPort =_T("Unknown");
		vStreamInfo.csStreamProtocol =_T("Unknown");
		vStreamInfo.csStreamResolution =_T("Unknown");
		vStreamInfo.csStreamVideoCompressionType =_T("Unknown");
		return;
	}

	vStreamInfo.csLV_RE.Format(_T("%s"), (LPCTSTR)m_veMoStreamInfo[iStreamNum-1].tcLV_RE);
	vStreamInfo.csStreamPort.Format(_T("%s"), (LPCTSTR)m_veMoStreamInfo[iStreamNum-1].tcPort);
	vStreamInfo.csStreamProtocol.Format(_T("%s"), (LPCTSTR)m_veMoStreamInfo[iStreamNum-1].tcProtocol);
	vStreamInfo.csStreamResolution.Format(_T("%s"), (LPCTSTR)m_veMoStreamInfo[iStreamNum-1].tcResolution);
	vStreamInfo.csStreamVideoCompressionType.Format(_T("%s"), (LPCTSTR)m_veMoStreamInfo[iStreamNum-1].tcVideoType);
}

void CDlgAddManageCam::StreamInit()
{
	if(::_ttoi(vIPCamInfo.csStreamSum) <= 0)	vIPCamInfo.csStreamSum = _T("1");

	//--Stream ComboBox
	CComboBox* pAVStream =(CComboBox*) GetDlgItem(IDC_COMBO_AV_STREAM);

	pAVStream->ResetContent();
	ASSERT(pAVStream->GetCount() == 0);

	for(int iTemp = 1; iTemp <= ::_ttoi(vIPCamInfo.csStreamSum); iTemp++)
	{
		CString csStream;
		csStream.Format(_T("Stream %d"), iTemp);
		pAVStream->AddString(csStream);
		pAVStream->SetCurSel(0);
	}

	//--Liveview ComboBox
	CComboBox* pLVStream =(CComboBox*) GetDlgItem(IDC_COMBO_LV);
	
	pLVStream->ResetContent();
	ASSERT(pLVStream->GetCount() == 0);

	for(int iTemp = 1; iTemp <= ::_ttoi(vIPCamInfo.csStreamSum); iTemp++)
	{
		CString csStream;
		csStream.Format(_T("Stream %d"), iTemp);
		pLVStream->AddString(csStream);
		pLVStream->SetCurSel(0);
	}

	//--Recording ComboBox
	CComboBox* pREStream =(CComboBox*) GetDlgItem(IDC_COMBO_RE);
	
	pREStream->ResetContent();
	ASSERT(pREStream->GetCount() == 0);

	for(int iTemp = 1; iTemp <= ::_ttoi(vIPCamInfo.csStreamSum); iTemp++)
	{
		CString csStream;
		csStream.Format(_T("Stream %d"), iTemp);
		pREStream->AddString(csStream);
		pREStream->SetCurSel(0);
	}

	if(::_ttoi(vIPCamInfo.csStreamSum) == 0)
	{
		GetDlgItem(IDC_BUTTON_MODIFY_AV_PTZ)->EnableWindow(0);
	}
}

void CDlgAddManageCam::vGetMoPTZInfo()
{
	//--PTC Info
	_tcscpy(m_MoPTZInfo.tcEnable , _T(""));
	_tcscpy(m_MoPTZInfo.tcProtocol , _T(""));
	_tcscpy(m_MoPTZInfo.tcDI1 , _T(""));
	_tcscpy(m_MoPTZInfo.tcDI2 , _T(""));
	_tcscpy(m_MoPTZInfo.tcDO , _T(""));

	CString csPTZEnable, csPTZProtocol, csPTZDI_1, csPTZDI_2, csPTZDO;

	GetDlgItem(IDC_EDIT_PTZ_ENABLE)->GetWindowTextW(csPTZEnable);
	_tcscpy(m_MoPTZInfo.tcEnable , csPTZEnable);

	GetDlgItem(IDC_EDIT_PTZ_PROTOCOL)->GetWindowTextW(csPTZProtocol);
	_tcscpy(m_MoPTZInfo.tcProtocol , csPTZProtocol);

	GetDlgItem(IDC_EDIT_DI_1)->GetWindowTextW(csPTZDI_1);
	_tcscpy(m_MoPTZInfo.tcDI1 , csPTZDI_1);

	GetDlgItem(IDC_EDIT_DI_2)->GetWindowTextW(csPTZDI_2);
	_tcscpy(m_MoPTZInfo.tcDI2 , csPTZDI_2);

	GetDlgItem(IDC_EDIT_DO)->GetWindowTextW(csPTZDO);
	_tcscpy(m_MoPTZInfo.tcDO , csPTZDO);
}
void CDlgAddManageCam::OnCbnSelchangeComboAvStream()
{
	// TODO: Add your control notification handler code here
	CComboBox* pAVStream =(CComboBox*) GetDlgItem(IDC_COMBO_AV_STREAM);
	int SelIndex = pAVStream->GetCurSel() + 1;

	this->vGetStreamDataFromVector(SelIndex);

	if(vStreamInfo.csStreamVideoCompressionType.Find(_T("H264"), 0) != -1)					//Set Video Compression Type
		GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("H264") );
	else if(vStreamInfo.csStreamVideoCompressionType.Find(_T("MPEG4"), 0) != -1)
		GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("MPEG4") );
	else if(vStreamInfo.csStreamVideoCompressionType.Find(_T("MJPEG"), 0) != -1)
		GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("MJPEG") );

	GetDlgItem(IDC_EDIT_AV_PORT)->SetWindowText( vStreamInfo.csStreamPort );				//Video Port

	//GetDlgItem(IDC_EDIT_AV_PROTOCOL)->SetWindowTextW( vStreamInfo.csStreamProtocol );		//Set Protocol

	OnCbnSelchangeComboStreamProtocol();

	/*CComboBox* pStreamProtocol = (CComboBox*) GetDlgItem(IDC_COMBO_STREAM_PROTOCOL);
	int iCurSel = pStreamProtocol->GetCurSel();
	CString csCurSel;
	pStreamProtocol->GetLBText(iCurSel, csCurSel);

	if(csCurSel.Find(_T("RTSP")) == -1)
	{
		GetDlgItem(IDC_STATIC_RTSPOVER)->ShowWindow(0);
		GetDlgItem(IDC_RADIO_RTSP_UDP)->ShowWindow(0);
		GetDlgItem(IDC_RADIO_RTSP_TCP)->ShowWindow(0);
	}
	else
	{
		GetDlgItem(IDC_STATIC_RTSPOVER)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_RTSP_UDP)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_RTSP_TCP)->ShowWindow(1);
		if(vStreamInfo.csStreamProtocol.Find(_T("RTPTCP")) != -1)
		{
			((CButton*)GetDlgItem(IDC_RADIO_RTSP_UDP))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_RTSP_TCP))->SetCheck(1);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_RADIO_RTSP_UDP))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_RTSP_TCP))->SetCheck(0);
		}
	}*/

	//GetDlgItem(IDC_EDIT_AV_RESOLUTION)->SetWindowTextW( vStreamInfo.csStreamResolution );	//Set Resolution

	if( vStreamInfo.csStreamResolution.GetLength() == 0 )
	{
		GetDlgItem(IDC_EDIT_AV_RESOLUTION)->SetWindowTextW(_T("Unknown"));
	}
	else
	{
		GetDlgItem(IDC_EDIT_AV_RESOLUTION)->SetWindowTextW( vStreamInfo.csStreamResolution );	//Set Resolution
	}

}

void CDlgAddManageCam::OnBnClickedButtonModifyAvPtz()
{
	// TODO: Add your control notification handler code here
	CDlgModifyStreamPTZ * m_CDlgModifyStreamPTZ = new CDlgModifyStreamPTZ();

	vGetMoPTZInfo();

	m_CDlgModifyStreamPTZ->vSetStreamPTZInfo(m_MoPTZInfo, m_veMoStreamInfo);

	if(m_CDlgModifyStreamPTZ->DoModal() == IDOK)
	{
		memcpy(&m_MoPTZInfo, &m_CDlgModifyStreamPTZ->m_MoPTZInfo, sizeof(m_MoPTZInfo));
		m_veMoStreamInfo.assign(m_CDlgModifyStreamPTZ->m_veMoStreamInfo.begin(), m_CDlgModifyStreamPTZ->m_veMoStreamInfo.end());

		//vDirtyFlagChanged();
	}
	else
	{
		delete m_CDlgModifyStreamPTZ;
		return;
	}

	//--Set PTZ UI
	GetDlgItem(IDC_EDIT_PTZ_ENABLE)->SetWindowText( m_MoPTZInfo.tcEnable );			//	PTZ
	GetDlgItem(IDC_EDIT_PTZ_PROTOCOL)->SetWindowText( m_MoPTZInfo.tcProtocol );
	GetDlgItem(IDC_EDIT_DI_1)->SetWindowText( m_MoPTZInfo.tcDI1 );
	GetDlgItem(IDC_EDIT_DI_2)->SetWindowText( m_MoPTZInfo.tcDI1 );
	GetDlgItem(IDC_EDIT_DO)->SetWindowText( m_MoPTZInfo.tcDO );
	
	//--Set Stream UI
	vGetStreamDataFromVector(1);

	int nIndex;
	CComboBox* pAVStream =(CComboBox*) GetDlgItem(IDC_COMBO_AV_STREAM);						//Set AV Stream
	nIndex = pAVStream->FindStringExact(0, _T("Stream 1"));
	if(nIndex == -1)
	{nIndex = pAVStream->FindStringExact(0, _T("Unknown"));}
	pAVStream->SetCurSel(nIndex);

	if(vStreamInfo.csStreamVideoCompressionType.Find(_T("H264"), 0) != -1)					//Set Video Compression Type
		GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("H264") );
	else if(vStreamInfo.csStreamVideoCompressionType.Find(_T("MPEG4"), 0) != -1)
		GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("MPEG4") );
	else if(vStreamInfo.csStreamVideoCompressionType.Find(_T("MJPEG"), 0) != -1)
		GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("MJPEG") );

	GetDlgItem(IDC_EDIT_AV_PORT)->SetWindowText( vStreamInfo.csStreamPort );				//Video Port

	GetDlgItem(IDC_EDIT_AV_PROTOCOL)->SetWindowTextW( vStreamInfo.csStreamProtocol );		//Set Protocol

	GetDlgItem(IDC_EDIT_AV_RESOLUTION)->SetWindowTextW( vStreamInfo.csStreamResolution );	//Set Resolution

	delete m_CDlgModifyStreamPTZ;
}

void CDlgAddManageCam::OnBnClickedButtonScanCamera()
{
	// TODO: Add your control notification handler code here
	
	CDlgScanNetworkCameras DlgScan;
	if (DlgScan.DoModal() == IDOK)
	{
		//if(DlgScan.pCheckNode == NULL)
		//	return;

		initFlag = 0;
		//DlgScan.vZeroNodeItem();
		//memcpy(&m_Node, DlgScan.pCheckNode, sizeof(m_Node));
		BeginWaitCursor();
		GetDlgItem(IDC_STATIC_LOADING)->ShowWindow(SW_SHOW);//Data Loading
		//UpdateData(false);
		this->UpdateWindow();
	}
	else
		return;

	//--Clean Container in this Dialog
	ec_Camera camObj;
	ScanMap.clear();
	m_veMoStreamInfo.clear();
	CString strMac;
	strMac.Format(_T("%02X%02X%02X%02X%02X%02X"), m_Node.mac[0], m_Node.mac[1], m_Node.mac[2], m_Node.mac[3], m_Node.mac[4], m_Node.mac[5]);
	m_pBk->GetCamera(strMac, camObj);

	if(!camObj.isempty())
	{
		csMacExit.Format(_T("%d"), camObj.camera_idx);
		GetDlgItem(IDC_EDIT_CHANNEL_ID)->SetWindowText( csMacExit );	
	}

	//--Get Info From ENCP
	USES_CONVERSION;
	CString csIP = A2W(m_Node.ip);
	vGetInfoFromENCP(csIP);
	
	//--Get Info From CGI
	GetDlgItem(IDC_EDIT_USER)->SetWindowText( vIPCamInfo.csUser );
	GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText( vIPCamInfo.csPWD );

	if(vIPCamInfo.csUser == _T("") || vIPCamInfo.csPWD == _T("") )
		vSetCredentials();													//Set User n Password

	int iGetIPCamType = this->bGetIPCamType(csIP);
	if(iGetIPCamType == OLD_TYPE)
	{	
		this->csThreadIP = csIP;
		::AfxBeginThread(vGetCGIData, (LPVOID)this);
		
		HANDLE hCGIThread = ::CreateEvent(NULL, false, false, _T("Global\\GET_CGI_DATA"));
		::WaitForSingleObject(hCGIThread, INFINITE);
		::CloseHandle(hCGIThread);

		//vGetCGIData(csIP);
		vGetIPCamDataFromMap(0);
		vGetStreamDataFromMap(0);

	}
	else if(iGetIPCamType == NEW_TYPE)
	{
		this->csThreadIP = csIP;
		::AfxBeginThread(vGetCGIDataNew, (LPVOID)this);

		HANDLE hCGIThread = ::CreateEvent(NULL, false, false, _T("Global\\GET_NEW_CGI_DATA"));
		::WaitForSingleObject(hCGIThread, INFINITE);
		::CloseHandle(hCGIThread);

		//vGetCGIDataNew(csIP);
		vGetIPCamDataFromMap(1);
		vGetStreamDataFromMap(1);
	}else if(iGetIPCamType == UNAUTHORIZED || iGetIPCamType == ERROR_TYPE)
	{
		//Do Nothing.
	}

	/*if(::_ttoi(vIPCamInfo.csStreamSum) < 2 )
	{
		vIPCamInfo.csStreamSum.Format(_T("%d"), m_veMoStreamInfo.size());
	}*/

	/*-----------Set UI-----------*/
	CIPAddressCtrl *IPAddress = (CIPAddressCtrl*) GetDlgItem(IDC_IPADDRESS_IP_ADDRESS);	//Set IP Address
	DWORD dwAddress= ntohl( inet_addr(m_Node.ip));
	IPAddress->SetAddress(dwAddress);

	CString csCameraName;
	csCameraName.Format(_T("%s - %s"), A2W(m_Node.ip), A2W(m_Node.modelname));
	if(m_Node.nType == 1)																//Set Connection Type
	{
		((CButton*)(GetDlgItem( IDC_RADIO_DYNAMIC_IP )))->SetCheck( 1 );
		((CButton*)(GetDlgItem( IDC_RADIO_STATIC_IP )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_PPPOE )))->SetCheck( 0 );
		csCameraName.Format(_T("%s"), A2W(m_Node.modelname));
		this->GetDlgItem(IDC_EDIT_CAMERA_NAME)->SetWindowTextW(csCameraName);					//Set Camera Name
		this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->EnableWindow(0);
		this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->EnableWindow(0);
		this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->EnableWindow(0);
		this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(0);
	}
	else if(m_Node.nType == 0)
	{
		((CButton*)(GetDlgItem( IDC_RADIO_DYNAMIC_IP )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_STATIC_IP )))->SetCheck( 1 );
		((CButton*)(GetDlgItem( IDC_RADIO_PPPOE )))->SetCheck( 0 );
		this->GetDlgItem(IDC_EDIT_CAMERA_NAME)->SetWindowTextW(csCameraName);					//Set Camera Name
		this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->EnableWindow(1);
		this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->EnableWindow(1);
		this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->EnableWindow(1);
		this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(1);
	}
	else if (m_Node.nType == 2)
	{
		((CButton*)(GetDlgItem( IDC_RADIO_DYNAMIC_IP )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_STATIC_IP )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_PPPOE )))->SetCheck( 1 );
		this->GetDlgItem(IDC_EDIT_CAMERA_NAME)->SetWindowTextW(csCameraName);					//Set Camera Name
		this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->EnableWindow(1);
		this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->EnableWindow(1);
		this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->EnableWindow(1);
		this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(1);
	}

	this->GetDlgItem(IDC_EDIT_MODEL)->SetWindowTextW(vIPCamInfo.csModel);				//Set Model

	if(vIPCamInfo.csNetmask == _T(""))
		vIPCamInfo.csNetmask = _T("0.0.0.0");
	this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->SetWindowTextW(vIPCamInfo.csNetmask);				//Set Subnet Mask

	if(vIPCamInfo.csDNS == _T(""))
		vIPCamInfo.csDNS = _T("0.0.0.0");
	this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->SetWindowTextW(vIPCamInfo.csDNS);				//Set Name Server

	if(vIPCamInfo.csGateway == _T(""))
		vIPCamInfo.csGateway = _T("0.0.0.0");
	this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->SetWindowTextW(vIPCamInfo.csGateway);				//Set Gateway

	this->GetDlgItem(IDC_EDIT_MAC)->SetWindowTextW(vIPCamInfo.csMac);						//Set Mac Address

	this->GetDlgItem(IDC_EDIT_WEB_PORT)->SetWindowTextW(vIPCamInfo.csWebPort);				//Web Port

	//--Initial Stream Protocol ComboBox
	vInitStreamProtocol();

	StreamInit();

	vSetStreamPTZUI();


	/*-----------Display Event Info-------*/
	vDisplayEventInfo();

	GetDlgItem(IDC_STATIC_LOADING)->ShowWindow(SW_HIDE);//Data Loading
	//UpdateData(false);
	this->UpdateWindow();
	EndWaitCursor();

	//Not dirty
	this->dirtyFlag = false;
	
	//--Save Mac and Stream Protocols
	veOrgInfo.clear();
	veOrgInfo.push_back(vIPCamInfo.csMac);
	for(int iTemp = 0; iTemp < ::_ttoi(vIPCamInfo.csStreamSum); iTemp++)
	{
		veOrgInfo.push_back(m_veMoStreamInfo[iTemp].tcProtocol);
	}

	//--Check Recording Text Box
	if( ((CButton*)GetDlgItem(IDC_COMBO_RE))->GetWindowTextLengthW() > 0)
	{
		//((CButton*)GetDlgItem(IDC_BUTTON_RECORDING_DLG))->EnableWindow(1);
	}

	//--Enable NAS 
	if(iGetIPCamType == NEW_TYPE)
	{
		(CButton*)GetDlgItem(IDC_CHECK_NAS)->EnableWindow(1);
	}
	
}

void CDlgAddManageCam::vGetCGIData(CString csIP)
{
	map<CString, sVERSETS>::iterator pos;
	map<CString, sSETS>::iterator pos2;

	ScanMap[_T("config/stream_info.cgi")];

	for(pos = ScanMap.begin(); pos != ScanMap.end(); pos++)
	{
		CString csUrl, csBuffer;
		Curlplus CGIScanData;
		
		csUrl.Format(_T("http://%s:%s/%s"), csIP, vIPCamInfo.csWebPort, pos->first);	
		csBuffer = CGIScanData.get(csUrl, vIPCamInfo.csUser, vIPCamInfo.csPWD);

		if(csBuffer == _T(""))
		{
			CString csMsg(_T(""));
			LOADSTRING(csMsg, IDS_ADD_CAM_NOT_FOUND);
			//csMsg.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_NOT_FOUND);
			AfxMessageBox(csMsg);
			//AfxMessageBox(_T("IP Camera Not Found."));
			return;
		}

		char *buffer = new char[csBuffer.GetLength() + 1];
		wcstombs_s(NULL, buffer, csBuffer.GetLength() + 1, csBuffer, _TRUNCATE);
	
		TCHAR tzBuffer[MAXBUF];
		MultiByteToWideChar(CP_ACP, 0, buffer, -1, tzBuffer, MAXBUF);
		vParseSegment(tzBuffer, pos);
		delete buffer;
	}
		
	/*--------Get Stream Number----------*/
	vIPCamInfo.csStreamSum = ScanMap[_T("config/stream_info.cgi")].aSet[_T("vprofilenum")].csKey;

	for(int iTemp = 1; iTemp <= ::_ttoi(vIPCamInfo.csStreamSum); iTemp++)
	{
		CString csVideo;
		csVideo.Format(_T("config/video.cgi?profileid=%d"), iTemp);
		ScanMap[csVideo];
	}

	ScanMap[_T("config/rs485.cgi")];
    ScanMap[_T("config/mcp.cgi?mcp=System")];		
	ScanMap[_T("config/io.cgi")];
	ScanMap[_T("config/network.cgi")];
	ScanMap[_T("config/event_info.cgi")];
	ScanMap[_T("common/info.cgi")];
	ScanMap[_T("config/stream.cgi")];

	for(pos = ScanMap.begin(); pos != ScanMap.end(); pos++)
	{
		CString csUrl, csBuffer;
		Curlplus CGIScanData;

		csUrl.Format(_T("http://%s:%s/%s"), csIP, vIPCamInfo.csWebPort, pos->first);
		csBuffer = CGIScanData.get(csUrl, vIPCamInfo.csUser, vIPCamInfo.csPWD);
		
		char *buffer = new char[csBuffer.GetLength() + 1];
		wcstombs_s(NULL, buffer, csBuffer.GetLength() + 1, csBuffer, _TRUNCATE);
	
		TCHAR tzBuffer[MAXBUF];
		MultiByteToWideChar(CP_ACP, 0, buffer, -1, tzBuffer, MAXBUF);
		vParseSegment(tzBuffer, pos);
		delete buffer;

		/*-------------GET Event Info-------------*/

		if(pos->first == _T("config/event_info.cgi"))
		{
			vCheckEvent(pos,csIP);
		}
	}

	vGetIPCamDataFromMap(0);	//Old Type IP Cam
}

void CDlgAddManageCam::vParseSegment(TCHAR *tszBuffer, map<CString,sVERSETS>::iterator pos)
{
	TCHAR* tszLine;
	tszLine = wcstok(tszBuffer, _T("\r\n"));
	while (tszLine)
	{
		TCHAR* tszName = tszLine;
		TCHAR* tsz = wcschr(tszLine, '=');		
		if (tsz)
		{
			*tsz = '\0';
			tsz++;
			pos->second.aSet[tszName] = sSETS(tsz, tszName);
		}
		tszLine = wcstok(NULL, _T("\r\n"));
	}		
	return;
}

void CDlgAddManageCam::vCheckEvent(map<CString,sVERSETS>::iterator pos, CString csIP)
{
	map<CString, sVERSETS>::iterator pos2;

	CString nNumOfevent;

	if(ScanMap[_T("config/event_info.cgi")].aSet[_T("num")].csKey == _T("0"))
		return;
	else
	{
		CString csEventNames;
		TCHAR * tszEventNames;
		TCHAR * tszLine;
		CString csUrl;
		Curlplus CGIScanData;
		CString csEventInfo;
	
		csEventNames = ScanMap[_T("config/event_info.cgi")].aSet[_T("name")].csKey;
		
		tszEventNames = csEventNames.GetBuffer();
		
		vector <int> iEventNumber;

		tszLine = wcstok(tszEventNames,_T(","));
		if(tszLine)
		iEventNumber.push_back(_wtoi(tszLine));
		else
			iEventNumber.push_back(1);
		while (tszLine)
		{
			
			tszLine = wcstok(NULL, _T(","));
			if(tszLine)
			iEventNumber.push_back(_wtoi(tszLine));
		}	

		for(int i = 0 ; i<iEventNumber.size(); i++)
		{
			int nEventNumber;

			nEventNumber = iEventNumber[i];

			CString csSaveEventInfo;

			csSaveEventInfo.Format(_T("%s/event/name=%d"),csIP,nEventNumber);

			EventInfoMap[csSaveEventInfo];

			csUrl.Format(_T("http://%s/config/event.cgi?name=%d"),csIP,nEventNumber);
		
			csEventInfo = CGIScanData.get(csUrl);

			char *buffer = new char[csEventInfo.GetLength() + 1];
			wcstombs_s(NULL, buffer, csEventInfo.GetLength() + 1,csEventInfo, _TRUNCATE);
	
			TCHAR tzBuffer[MAXBUF];
			MultiByteToWideChar(CP_ACP, 0, buffer, -1, tzBuffer, MAXBUF);

			pos2 = EventInfoMap.find(csSaveEventInfo);
			vParseSegment(tzBuffer, pos2);
			delete buffer;
			
		}
	}
}

void CDlgAddManageCam::vGetIPCamDataFromMap(int CGIFlag)
{
	if(CGIFlag == 0)
	{
		vIPCamInfo.csRS485Enable = ScanMap[_T("config/rs485.cgi")].aSet[_T("enable")].csKey;
		vIPCamInfo.csRS485Protocol = ScanMap[_T("config/rs485.cgi")].aSet[_T("proto")].csKey;
		vIPCamInfo.csRS485In1 = ScanMap[_T("config/io.cgi")].aSet[_T("in1")].csKey;
		vIPCamInfo.csRS485In2 = ScanMap[_T("config/io.cgi")].aSet[_T("in2")].csKey;
		vIPCamInfo.csRS485Out1 = ScanMap[_T("config/io.cgi")].aSet[_T("out1")].csKey;
		vIPCamInfo.csNetmask = ScanMap[_T("config/network.cgi")].aSet[_T("netmask")].csKey;
		vIPCamInfo.csGateway = ScanMap[_T("config/network.cgi")].aSet[_T("gateway")].csKey;
		vIPCamInfo.csDNS = ScanMap[_T("config/network.cgi")].aSet[_T("dns1")].csKey;
		vIPCamInfo.csStreamUrl = ScanMap[_T("config/stream.cgi")].aSet[_T("stream_url")].csKey;
		vIPCamInfo.csStreamSum = ScanMap[_T("config/stream_info.cgi")].aSet[_T("vprofilenum")].csKey;
		vIPCamInfo.csMac = ScanMap[_T("common/info.cgi")].aSet[_T("macaddr")].csKey;
		vIPCamInfo.csMac.Replace(_T(":"), _T("-"));
	}
	else if(CGIFlag == 1)
	{
		vIPCamInfo.csRS485Enable = _T("no");
		vIPCamInfo.csRS485Enable = ScanMap[_T("config/rs485.cgi")].aSet[_T("enable")].csKey;
		vIPCamInfo.csRS485Enable = ScanMap[_T("config/mcp.cgi?mcp=System")].aSet[_T("mcp_system_speeddome")].csKey;	
		vIPCamInfo.csRS485Protocol = ScanMap[_T("config/rs485.cgi")].aSet[_T("proto")].csKey;		
		vIPCamInfo.csRS485In1 = ScanMap[_T("config/io.cgi")].aSet[_T("in1")].csKey;
		vIPCamInfo.csRS485In2 = ScanMap[_T("config/io.cgi")].aSet[_T("in2")].csKey;
		vIPCamInfo.csRS485Out1 = ScanMap[_T("config/io.cgi")].aSet[_T("out1")].csKey;
		vIPCamInfo.csNetmask = ScanMap[_T("config/network.cgi")].aSet[_T("netmask")].csKey;
		vIPCamInfo.csGateway = ScanMap[_T("config/network.cgi")].aSet[_T("gateway")].csKey;
		vIPCamInfo.csDNS = ScanMap[_T("config/network.cgi")].aSet[_T("dns1")].csKey;
		vIPCamInfo.csStreamUrl = ScanMap[_T("config/stream.cgi")].aSet[_T("stream_name")].csKey;
		vIPCamInfo.csMac = ScanMap[_T("config/info.cgi")].aSet[_T("macaddr")].csKey.MakeUpper();
		vIPCamInfo.csMac.Replace(_T(":"), _T("-"));
	}
}

void CDlgAddManageCam::vSetCredentials()
{
	m_pBk->GetEcParam(_T("MASTERUSER"), vIPCamInfo.csUser);
	m_pBk->GetEcParam(_T("MASTERPASSWORD"), vIPCamInfo.csPWD);

	GetDlgItem(IDC_EDIT_USER)->SetWindowText( vIPCamInfo.csUser );
	GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText( vIPCamInfo.csPWD );
}

void CDlgAddManageCam::vDisplayEventInfo()
{
	map<CString, sVERSETS>::iterator posEvent1;
	map<CString, sSETS>::iterator posEvent2;
	CString csDisplayEventInfoType , csDisplayEventInfoAction,csDisplayEventInfo;
	int nEventInfoIndex=1;

	CListBox* m_listBoxEvent = (CListBox*) GetDlgItem(IDC_LIST_ADDMANCAM_EVENT1);
	m_listBoxEvent->ResetContent();
	csDisplayEventInfo=_T("");

	for(posEvent1 = EventInfoMap.begin(); posEvent1 != EventInfoMap.end(); posEvent1++)
	{
		for(posEvent2 = posEvent1->second.aSet.begin(); posEvent2 != posEvent1->second.aSet.end(); posEvent2++)
		{
			if(posEvent2->first == _T("event"))
			{
				if(posEvent2->second.csKey == _T("none"))
					csDisplayEventInfoType =  _T("");
				else 
					 csDisplayEventInfoType = posEvent2->second.csKey ;

			}

		    if(posEvent2->first == _T("action"))
			{
				if(posEvent2->second.csKey == _T("none"))
					csDisplayEventInfoAction = _T("");
				else
					 csDisplayEventInfoAction =  posEvent2->second.csKey;

			}
		}
				
		if( csDisplayEventInfoAction !=_T("") || csDisplayEventInfoType!=_T(""))
		{
		csDisplayEventInfo.Format(_T( "(%d) %s / %s"), nEventInfoIndex ,csDisplayEventInfoType,  csDisplayEventInfoAction);
		m_listBoxEvent->AddString(csDisplayEventInfo);
		nEventInfoIndex++;
		}

	}
}

void CDlgAddManageCam::vGetCGIDataNew(CString csIP)
{
	map<CString, sVERSETS>::iterator pos;
	map<CString, sSETS>::iterator pos2;

	/*---Get Stream, rs485, io, network.cgi----------*/

	ScanMap[_T("config/stream.cgi")];
	ScanMap[_T("config/rs485.cgi")];
    ScanMap[_T("config/mcp.cgi?mcp=System")];	
	ScanMap[_T("config/io.cgi")];
	ScanMap[_T("config/network.cgi")];
	ScanMap[_T("config/audio.cgi")];
	ScanMap[_T("config/info.cgi")];
	ScanMap[_T("config/stream.cgi")];
	ScanMap[_T("config/video.cgi")];

	for(pos = ScanMap.begin(); pos != ScanMap.end(); pos++)
	{
		CString csUrl, csBuffer;
		Curlplus CGIScanData;

		csUrl.Format(_T("http://%s:%s/%s"), csIP, vIPCamInfo.csWebPort, pos->first);

		csBuffer = CGIScanData.get(csUrl, vIPCamInfo.csUser, vIPCamInfo.csPWD);
		if((csBuffer.Find(_T("Denied")) != -1) || (csBuffer.Find(_T("Error")) != -1))
		{
			TRACE(_T("Access Denied"));
			return;	
		}
		else if( csBuffer == _T("") )
		{
			TRACE(_T("Network Error"));
			return;
		}
		else if(csBuffer.Find(_T("401")) != -1 && csBuffer.Find(_T("Unauthorized")) != -1)
		{
			TRACE(_T("401:Unauthorized"));
			return;
		}
			csBuffer.Remove(_T('{'));
			csBuffer.Remove(_T('"'));
			csBuffer.Remove(_T('}'));
			csBuffer.Remove(_T(' '));
		
		char *buffer = new char[csBuffer.GetLength() + 1];
		wcstombs_s(NULL, buffer, csBuffer.GetLength() + 1, csBuffer, _TRUNCATE);
	
		TCHAR tzBuffer[MAXBUF] = {0};
		MultiByteToWideChar(CP_ACP, 0, buffer, -1, tzBuffer, MAXBUF);
		
		if(pos->first == _T("config/video.cgi"))
		{
			vIPCamInfo.csStreamSum.Format(_T("%d"), ParseSegmentVideoCgi(tzBuffer, pos));
			vParseSegmentNew(tzBuffer, pos);
		}
		else
		{
			vParseSegmentNew(tzBuffer, pos);
		}

		delete buffer;
	}

	vGetIPCamDataFromMap(1);	//New Type IP Cam
}

void CDlgAddManageCam::vParseSegmentNew(TCHAR *tszBuffer, map<CString,sVERSETS>::iterator pos)
{
	TCHAR* tszLine;
	tszLine = wcstok(tszBuffer, _T(", "));

	while (tszLine)
	{
		TCHAR* tszName = tszLine;
		TCHAR* tsz = wcschr(tszLine, ':');	
		if (tsz)
		{
			*tsz = '\0';
			tsz++;
			pos->second.aSet[tszName] = sSETS(tsz, tszName, true);
			pos->second.flag = true;
		}

		tszLine = wcstok(NULL, _T(", "));
	}	
	
	return;
}

int CDlgAddManageCam::ParseSegmentVideoCgi(TCHAR *tszBuffer, map<CString,sVERSETS>::iterator pos)
{
	TCHAR* tszProfile = NULL; 
	TCHAR* tszBufferTmp = NULL;
	int cNumOfProfile = 0;;

	//cNumOfProfile = -1;

	tszBufferTmp = new TCHAR[_tcslen(tszBuffer)+1]();

	memcpy(tszBufferTmp, tszBuffer, _tcslen(tszBuffer)*sizeof(TCHAR));
	
	CString csTestBuffer;
	csTestBuffer.Format(_T("%s"), tszBuffer);

	if(csTestBuffer.Find(_T("401")) != -1 && csTestBuffer.Find(_T("Unauthorized")) != -1)
	{
		TRACE(_T("401:Unauthorized"));
		return 2;
	}

	csTestBuffer.Remove(_T('['));
	csTestBuffer.Remove(_T(']'));
	
	TCHAR *tszParse = new TCHAR[(csTestBuffer.GetLength()+1)*sizeof(TCHAR)]();
	::_tcscpy(tszParse, csTestBuffer.GetBuffer());

	//int iLength = csTestBuffer.GetLength();
	int iPos = 0;
	int iFind = 0;
	while(iPos != -1)
	{
		//cNumOfProfile++;
		CString csTemp = csTestBuffer.Tokenize(_T(","), iPos);

		if(csTemp.Find(_T("resolution")) != -1)
		{
			cNumOfProfile++;
			CString csScanMapID, csaSetID, csResolution;
			csScanMapID.Format(_T("profile_id_%d"), cNumOfProfile);
			::AfxExtractSubString(ScanMap[csScanMapID].aSet[_T("resolution")].csKey, csTemp.GetBuffer(), 2, ':'); 
		}

		if(csTemp.Find(_T("codec")) != -1)
		{
			CString csScanMapID, csaSetID, csCodec;
			csScanMapID.Format(_T("profile_id_%d"), cNumOfProfile);
			::AfxExtractSubString(ScanMap[csScanMapID].aSet[_T("codec")].csKey, csTemp.GetBuffer(), 1, ':'); 
		}	
	}
	
	delete tszBufferTmp, tszParse;

	return cNumOfProfile;
}

void CDlgAddManageCam::vParseSegmentVideoCgi2(TCHAR *tszBuffer, int cNumOfProfile)
{
	TCHAR* tszLine;

	CString csProfileName;

	csProfileName.Format(_T("config/video.cgi?profileid=%d"),cNumOfProfile);

	tszLine = wcstok(tszBuffer, _T(", "));

	while (tszLine)
	{
		TCHAR* tszName = tszLine;
		TCHAR* tsz = wcschr(tszLine, ':');	
		if (tsz)
		{
			*tsz = '\0';
			tsz++;
			ScanMap[csProfileName].aSet[tszName] = sSETS(tsz, tszName, true);
			ScanMap[csProfileName].flag = true;
		}

		tszLine = wcstok(NULL, _T(", "));
	}	

	return;
}

void CDlgAddManageCam::vCheckDefaultGroup()
{
	CString  csFirstGroup(_T(""));
	bool bDefaultGroupExist = false;
	pair <int ,CString> pair_groupidFirst, pair_groupid;

	vector<Cam_Group> vcGroup;
	m_pBk->GetGroup(DeviceTree, vcGroup);

	int nIdx = 0, nCount = vcGroup.size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		//pair <int ,CString> pair_groupid(_wtoi(csCameraGroupID),csGroupDesc);

		pair_groupid.first = vcGroup[nIdx].cameragroupid;
		pair_groupid.second = vcGroup[nIdx].camera_group_desc;

		if(csFirstGroup.GetLength() == 0)
		{
			csFirstGroup = vcGroup[nIdx].camera_group_desc;
			pair_groupidFirst.first = vcGroup[nIdx].cameragroupid;
			pair_groupidFirst.second = vcGroup[nIdx].camera_group_desc;
		}

		if(vcGroup[nIdx].default_ == _T("Y"))
		{
			bDefaultGroupExist = true;
			DBIPCamGroupVector.push_back(pair_groupid);
		}
	}

	//--The first Group must be default.
	if(!bDefaultGroupExist)
	{
		//DBIPCamGroupVector.push_back(csFirstGroup);
		DBIPCamGroupVector.push_back(pair_groupidFirst);
	}
}

void CDlgAddManageCam::vSetStreamPTZUI()
{
	vGetStreamDataFromVector(1);

	int nIndex;
	CComboBox* pAVStream =(CComboBox*) GetDlgItem(IDC_COMBO_AV_STREAM);			//Set AV Stream
	nIndex = pAVStream->FindStringExact(0, _T("Stream 1"));
	if(nIndex == -1)
	{nIndex = pAVStream->FindStringExact(0, _T("Unknown"));}
	pAVStream->SetCurSel(nIndex);
	
	if(vStreamInfo.csStreamVideoCompressionType.Find(_T("H264"), 0) != -1)					//Set Video Compression Type
		GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("H264") );
	else if(vStreamInfo.csStreamVideoCompressionType.Find(_T("MPEG4"), 0) != -1)
		GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("MPEG4") );
	else if(vStreamInfo.csStreamVideoCompressionType.Find(_T("MJPEG"), 0) != -1)
		GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("MJPEG") );
	else if(vStreamInfo.csStreamVideoCompressionType.Find(_T("Unknown"), 0) != -1)
		GetDlgItem(IDC_EDIT_AV_TYPE)->SetWindowText( _T("Unknown") );

	if(vStreamInfo.csStreamPort == _T(""))	vStreamInfo.csStreamPort = _T("Unknown");	//Set Video Port
	GetDlgItem(IDC_EDIT_AV_PORT)->SetWindowText( vStreamInfo.csStreamPort );
	
	CComboBox* pStreamProtocol = (CComboBox*) GetDlgItem(IDC_COMBO_STREAM_PROTOCOL);
	for(int iTemp = 0; iTemp < pStreamProtocol->GetCount(); iTemp++)
	{
		//int iCurSel = pStreamProtocol->GetCurSel();
		CString csCurSel;
		pStreamProtocol->GetLBText(iTemp, csCurSel);

		if(csCurSel == vStreamInfo.csStreamProtocol)
		{
			pStreamProtocol->SetCurSel(iTemp);

			break;
		}
		else if(vStreamInfo.csStreamProtocol == _T("RTPTCP") && csCurSel == _T("RTSP"))
		{
			pStreamProtocol->SetCurSel(iTemp);

			break;
		}
		else if(vStreamInfo.csStreamProtocol == _T("RTPHTTP") && csCurSel == _T("RTSP"))
		{
			pStreamProtocol->SetCurSel(iTemp);
			break;
		}
		else if(vStreamInfo.csStreamProtocol == _T("RTSP") && csCurSel == _T("RTSP"))
		{
			pStreamProtocol->SetCurSel(iTemp);
			break;
		}
	}

	if(vStreamInfo.csStreamProtocol == _T(""))	vStreamInfo.csStreamProtocol = _T("Unknown");	//Set Protocol
	GetDlgItem(IDC_EDIT_AV_PROTOCOL)->SetWindowTextW(vStreamInfo.csStreamProtocol);		

	if( vStreamInfo.csStreamResolution.GetLength() == 0 )
	{
		GetDlgItem(IDC_EDIT_AV_RESOLUTION)->SetWindowTextW(_T("Unknown"));
	}
	else
	{
		GetDlgItem(IDC_EDIT_AV_RESOLUTION)->SetWindowTextW( vStreamInfo.csStreamResolution );	//Set Resolution
	}

	if(vIPCamInfo.csRS485Enable == _T("yes") || vIPCamInfo.csRS485Enable == _T("Y"))		//Set PTZ Enable
		GetDlgItem(IDC_EDIT_PTZ_ENABLE)->SetWindowTextW(_T("Enable"));
	else if(vIPCamInfo.csRS485Enable == _T("no") || vIPCamInfo.csRS485Enable == _T("N") || vIPCamInfo.csRS485Enable == _T(""))
		GetDlgItem(IDC_EDIT_PTZ_ENABLE)->SetWindowTextW(_T("Disable"));

	if(vIPCamInfo.csRS485Protocol == _T(""))
		GetDlgItem(IDC_EDIT_PTZ_PROTOCOL)->SetWindowTextW(_T("Unknown"));
	else
		GetDlgItem(IDC_EDIT_PTZ_PROTOCOL)->SetWindowTextW( vIPCamInfo.csRS485Protocol );		//Set PTZ Protocol

	if(vIPCamInfo.csRS485In1 == _T("on") || vIPCamInfo.csRS485In1 == _T("Y"))				//Set Digital Input 1
		GetDlgItem(IDC_EDIT_DI_1)->SetWindowTextW( _T("Normal Open") );
	else if(vIPCamInfo.csRS485In1 == _T("off") || vIPCamInfo.csRS485In1 == _T("N"))	
		GetDlgItem(IDC_EDIT_DI_1)->SetWindowTextW( _T("Normal Close") );
	else
		GetDlgItem(IDC_EDIT_DI_1)->SetWindowTextW( _T("Normal Close") );

	if(vIPCamInfo.csRS485In2 == _T("on") || vIPCamInfo.csRS485In2 == _T("Y"))				//Set Digital Input 2
		GetDlgItem(IDC_EDIT_DI_2)->SetWindowTextW( _T("Normal Open") );
	else if(vIPCamInfo.csRS485In2 == _T("off") || vIPCamInfo.csRS485In2 == _T("N"))	
		GetDlgItem(IDC_EDIT_DI_2)->SetWindowTextW( _T("Normal Close") );
	else
		GetDlgItem(IDC_EDIT_DI_2)->SetWindowTextW( _T("Normal Close") );

	if(vIPCamInfo.csRS485Out1 == _T("on") || vIPCamInfo.csRS485Out1 == _T("Y"))				//Set Digital Output 
		GetDlgItem(IDC_EDIT_DO)->SetWindowTextW( _T("Normal Open") );
	else if(vIPCamInfo.csRS485Out1 == _T("off") || vIPCamInfo.csRS485Out1 == _T("N"))	
		GetDlgItem(IDC_EDIT_DO)->SetWindowTextW( _T("Normal Close") );
	else
		GetDlgItem(IDC_EDIT_DO)->SetWindowTextW( _T("Normal Close") );

	GetDlgItem(IDC_BUTTON_MODIFY_AV_PTZ)->EnableWindow(1);

	//--RTP over UDP/TCP Radio Button
	if(initFlag == 3 || initFlag == 4)
	{
        //modify ipcam
    }else
    {
        CString csTemp;
		int iCurSel = pStreamProtocol->GetCurSel();
		pStreamProtocol->GetLBText(iCurSel, csTemp);
    }
	
    if(vStreamInfo.csStreamProtocol.Find(_T("RTSP")) != -1)
	{
		GetDlgItem(IDC_STATIC_RTSPOVER)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_RTSP_UDP)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_RTSP_TCP)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_RTSP_HTTP)->ShowWindow(1);
		((CButton*)GetDlgItem(IDC_RADIO_RTSP_UDP))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_RTSP_TCP))->SetCheck(0);
        ((CButton*)GetDlgItem(IDC_RADIO_RTSP_HTTP))->SetCheck(0);
	}
	else if(vStreamInfo.csStreamProtocol.Find(_T("RTPTCP")) != -1)
	{
		GetDlgItem(IDC_STATIC_RTSPOVER)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_RTSP_UDP)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_RTSP_TCP)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_RTSP_HTTP)->ShowWindow(1);
		((CButton*)GetDlgItem(IDC_RADIO_RTSP_UDP))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_RTSP_TCP))->SetCheck(1);
        ((CButton*)GetDlgItem(IDC_RADIO_RTSP_HTTP))->SetCheck(0);
	}
	else if(vStreamInfo.csStreamProtocol.Find(_T("RTPHTTP")) != -1)
	{
		GetDlgItem(IDC_STATIC_RTSPOVER)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_RTSP_UDP)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_RTSP_TCP)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_RTSP_HTTP)->ShowWindow(1);
		((CButton*)GetDlgItem(IDC_RADIO_RTSP_UDP))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_RTSP_TCP))->SetCheck(0);
        ((CButton*)GetDlgItem(IDC_RADIO_RTSP_HTTP))->SetCheck(1);
	}
	else
	{
		GetDlgItem(IDC_STATIC_RTSPOVER)->ShowWindow(0);
		GetDlgItem(IDC_RADIO_RTSP_UDP)->ShowWindow(0);
		GetDlgItem(IDC_RADIO_RTSP_TCP)->ShowWindow(0);
		GetDlgItem(IDC_RADIO_RTSP_HTTP)->ShowWindow(0);
		((CButton*)GetDlgItem(IDC_RADIO_RTSP_UDP))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_RTSP_TCP))->SetCheck(0);
        ((CButton*)GetDlgItem(IDC_RADIO_RTSP_HTTP))->SetCheck(0);
	}

}

CString CDlgAddManageCam::csGetResolutionDesc(CString csScan)
{
	if(csScan == _T("720x480"))
		csScan = _T("D1(NTSC)");
	else if(csScan == _T("352x240"))
		csScan = _T("CIF(NTSC)");
	else if(csScan == _T("176x120"))
		csScan = _T("QCIF(NTSC)");
	else if(csScan == _T("720x576"))
		csScan = _T("D1(PAL)");
	else if(csScan == _T("352x288"))
		csScan = _T("CIF(PAL)");
	else if(csScan == _T("176x144"))
		csScan = _T("QCIF(PAL)");
	else if(csScan == _T("1280x1024"))
		csScan = _T("SXGA");
	else if(csScan == _T("640x480"))
		csScan = _T("VGA");
	else if(csScan == _T("320x240"))
		csScan = _T("QVGA");
	else if(csScan == _T("160x120"))
		csScan = _T("QQVGA(NTSC)");
	else if(csScan == _T("160x128"))
		csScan = _T("QQVGA(PAL)");
	else if(csScan == _T("1920x1080"))
		csScan = _T("FULL HD");
	else if(csScan == _T(""))
		csScan = _T("Unknown");

	return csScan;
}

void CDlgAddManageCam::vGetStreamDataFromMap(int CGIFlag)
{
	if(::_ttoi(vIPCamInfo.csStreamSum) > 0)
	{
		m_veMoStreamInfo.clear();
		CString csStreamResolution, csStreamVideoCompressionType, csLV_RE, csStreamProtocol, csStreamPort;
		if(CGIFlag == 0)
		{
			for(int iTemp = 1; iTemp <= ::_ttoi(vIPCamInfo.csStreamSum); iTemp++)
			{
				CString csStreamNum;
				csStreamNum.Format(_T("config/video.cgi?profileid=%d"), iTemp);
				csStreamResolution = csGetResolutionDesc(ScanMap[csStreamNum].aSet[_T("resolution")].csKey);
				csStreamVideoCompressionType = ScanMap[csStreamNum].aSet[_T("codec")].csKey;
				if(csStreamVideoCompressionType == _T(""))
					csStreamVideoCompressionType = _T("Unknown");
				csLV_RE = _T("");
				csStreamPort = ScanMap[csStreamNum].aSet[_T("port")].csKey;
				csStreamProtocol = ScanMap[csStreamNum].aSet[_T("format")].csKey;

				if(csStreamProtocol == _T("RTP"))
					csStreamProtocol = _T("RTSP");
	
				if(csStreamProtocol == _T("VSM"))
					csStreamProtocol = _T("TCP");

				_tcscpy(m_MoStreamInfo.tcProtocol, csStreamProtocol);
				_tcscpy(m_MoStreamInfo.tcPort, csStreamPort);
				_tcscpy(m_MoStreamInfo.tcResolution, csStreamResolution);
				_tcscpy(m_MoStreamInfo.tcVideoType, csStreamVideoCompressionType);
				_tcscpy(m_MoStreamInfo.tcLV_RE, csLV_RE);
				m_veMoStreamInfo.push_back(m_MoStreamInfo);
			}
		}
		else if(CGIFlag == 1)
		{
			for(int iTemp = 1; iTemp <= ::_ttoi(vIPCamInfo.csStreamSum); iTemp++)
			{
				CString csStreamNum;
				csStreamNum.Format(_T("profile_id_%d"), iTemp);
				csLV_RE = _T("");
				
				CString csStreamResolution = csGetResolutionDesc(ScanMap[csStreamNum].aSet[_T("resolution")].csKey);

				csStreamVideoCompressionType = ScanMap[csStreamNum].aSet[_T("codec")].csKey;
				if(csStreamVideoCompressionType == _T(""))
					csStreamVideoCompressionType = _T("Unknown");
				csStreamPort = ScanMap[_T("config/stream.cgi")].aSet[_T("rtsp_port")].csKey;
				csStreamProtocol = _T("RTSP");

				_tcscpy(m_MoStreamInfo.tcProtocol, csStreamProtocol);
				_tcscpy(m_MoStreamInfo.tcPort, csStreamPort);
				_tcscpy(m_MoStreamInfo.tcResolution, csStreamResolution);
				_tcscpy(m_MoStreamInfo.tcVideoType, csStreamVideoCompressionType);
				_tcscpy(m_MoStreamInfo.tcLV_RE, csLV_RE);
				m_veMoStreamInfo.push_back(m_MoStreamInfo);
			}
		}
	}
}

void CDlgAddManageCam::OnBnClickedButtonSaveDuplicate()
{
	// TODO: Add your control notification handler code here

	//--Clear m_veMoStreamInfo
	for(int iTemp = 0; iTemp < m_veMoStreamInfo.size(); iTemp++)
	{
		_tcscpy(m_veMoStreamInfo[iTemp].tcLV_RE, _T(""));
	}
	
	CComboBox* pLVStream =(CComboBox*) GetDlgItem(IDC_COMBO_LV);
	int SelIndex = pLVStream->GetCurSel();
	CString csLV;

	for(int iTemp = 0; iTemp < m_veMoStreamInfo.size(); iTemp++)
	{
		csLV.Format(_T("%s"), m_veMoStreamInfo[SelIndex].tcLV_RE);
		csLV = csLV + _T("LV");
		_tcscpy(m_veMoStreamInfo[SelIndex].tcLV_RE, csLV);
	}

	if( ((CButton*)GetDlgItem(IDC_CHECK_RE))->GetCheck() )
	{
		CComboBox* pLVStream =(CComboBox*) GetDlgItem(IDC_COMBO_RE);
		int SelIndex = pLVStream->GetCurSel();
		CString csRE;

		for(int iTemp = 0; iTemp < m_veMoStreamInfo.size(); iTemp++)
		{
			if(iTemp == SelIndex)
			{
				csRE.Format(_T("%s"), m_veMoStreamInfo[SelIndex].tcLV_RE);
				csRE = csRE + _T("RE");
				_tcscpy(m_veMoStreamInfo[SelIndex].tcLV_RE, csRE);
			}
			else
			{
				csRE.Format(_T("%s"), m_veMoStreamInfo[iTemp].tcLV_RE);
				csRE.Replace(_T("RE"), _T(""));
				_tcscpy(m_veMoStreamInfo[iTemp].tcLV_RE, csRE);
			}
		}
	}
	else
	{
		for(int iTemp = 0; iTemp < m_veMoStreamInfo.size(); iTemp++)
		{
			CString csRE(_T(""));
			csRE.Format(_T("%s"), m_veMoStreamInfo[iTemp].tcLV_RE);
			csRE.Replace(_T("RE"), _T(""));
			_tcscpy(m_veMoStreamInfo[iTemp].tcLV_RE, csRE);
		}
	}

	//----------Check Exist in DB or not

	ec_Camera camObj;
	vIPCamInfo.csMac.Replace(_T("-"), _T(""));
	m_pBk->GetCamera(vIPCamInfo.csMac, camObj);

	if (camObj.isempty())
	{
		csMacExit.Empty();
	}

	if(csMacExit.GetLength() == 0)
	{
		vInsertIPCam();
		vSaveGroupList();
		//vSaveLV_REStream();

		//bSaveNewRecordingPath();
		if(RecordingFlag == false)
		{
			vSetDefaultRecordType();
		}
	}
	else
	{
		vUpdateIPCam();
		vSaveGroupList();
		//vSaveLV_REStream();
	}

	//TreeCtrl Refresh
	CMainFrame * m_CMainFrame = (CMainFrame*)(::AfxGetApp()->GetMainWnd());
//	CIPCamTreeView *pTreeView = (CIPCamTreeView *)m_CMainFrame->GetTreeView();
    CNVRBk::GetBkObject()->SendMessage(MsgUpdateView, UM_REFRESH_DEVICETREE, 0);

	//--notify recording service
	CSA csa;
	HANDLE hCameraRecording = CreateEvent(&csa.m_sa, false, false, RECORDING_STATUS_CHANGE);
	if (hCameraRecording == NULL)
	{
		TRACE(_T("Error code :%d"), ::GetLastError());
	}
	SetEvent( hCameraRecording );
	CloseHandle(hCameraRecording);

	//--Get Start the New IP Cam
	HANDLE hRestartEvent = ::CreateEvent(NULL, false, false, _T("Global\\ETROCENTER_EVENT_RESTART"));
	if(hRestartEvent == NULL)
	{
		TRACE(_T("Create Restart Event Error code :%d"), ::GetLastError());
	}
	SetEvent( hRestartEvent );
	CloseHandle(hRestartEvent);

	//--Send Event to Refresh Event Server
	//CSA csa;
	HANDLE hEventAction = CreateEvent(&csa.m_sa, false, false, _T("Global\\ETROCENTER_ACTION_REFRESH"));
	if (hEventAction == NULL)
	{
		TRACE(_T("Error code :%d"), ::GetLastError());
	}
	SetEvent( hEventAction );
	CloseHandle(hEventAction);
}

void CDlgAddManageCam::vInsertIPCam()
{
	USES_CONVERSION;

	ec_Camera camObj;
	vGetUIData();

	DWORD video_format = (m_Node.byDeviceType <<8) + (m_Node.bFishEye? TRUE:FALSE);
	camObj.camera_idx = atoi(W2A(vIPCamInfo.csCameraIDX));
	camObj.cameraname = vIPCamInfo.csCameraName;
	camObj.connect_type_code = vIPCamInfo.csConnectType; 
	camObj.ipaddress = vIPCamInfo.csIP;
	camObj.httpport = _ttoi(vIPCamInfo.csWebPort);
	camObj.gateway = vIPCamInfo.csGateway;
	camObj.name_server = vIPCamInfo.csDNS;
	camObj.mac_address = vIPCamInfo.csMac.MakeUpper();
	camObj.username = vIPCamInfo.csUser;
	camObj.password = vIPCamInfo.csPWD;
	camObj.brand_code = vIPCamInfo.csSn;
	camObj.model_code = vIPCamInfo.csModel;
	camObj.ptz_support = vIPCamInfo.csRS485Enable;
	camObj.ptz_protocol = vIPCamInfo.csRS485Protocol;
	camObj.digital_in1 = vIPCamInfo.csRS485In1;
	camObj.digital_in2 = vIPCamInfo.csRS485In2;
	camObj.digital_out = vIPCamInfo.csRS485Out1;
	camObj.video_format.Format(_T("%d"), video_format);
	camObj.subnet_mask1 = vIPCamInfo.csNetmask1;
	camObj.subnet_mask2 = vIPCamInfo.csNetmask2;
	camObj.subnet_mask3 = vIPCamInfo.csNetmask3;
	camObj.subnet_mask4 = vIPCamInfo.csNetmask4;
	camObj.active_ = vIPCamInfo.csActive;
	camObj.stream_url = vIPCamInfo.csStreamUrl;

	GetStream(camObj.vcStream);
	m_pBk->InsertCamera(camObj);

	return;
}

void CDlgAddManageCam::vUpdateIPCam()
{
	vGetUIData();

	ec_Camera camObj;
	m_pBk->GetCamera(vIPCamInfo.csMac, camObj);

	camObj.cameraname = vIPCamInfo.csCameraName;
	camObj.active_ = vIPCamInfo.csActive;
	camObj.connect_type_code = vIPCamInfo.csConnectType; 
	camObj.ipaddress = vIPCamInfo.csIP;
	camObj.model_code = vIPCamInfo.csModel;
	camObj.subnet_mask1 = vIPCamInfo.csNetmask1;
	camObj.subnet_mask2 = vIPCamInfo.csNetmask2;
	camObj.subnet_mask3 = vIPCamInfo.csNetmask3;
	camObj.subnet_mask4 = vIPCamInfo.csNetmask4;
	camObj.gateway = vIPCamInfo.csGateway;
	camObj.name_server = vIPCamInfo.csDNS;
	camObj.mac_address = vIPCamInfo.csMac.MakeUpper();
	camObj.username = vIPCamInfo.csUser;
	camObj.password = vIPCamInfo.csPWD;
	camObj.ptz_support = vIPCamInfo.csRS485Enable;
	camObj.ptz_protocol = vIPCamInfo.csRS485Protocol;
	camObj.digital_in1 = vIPCamInfo.csRS485In1;
	camObj.digital_in2 = vIPCamInfo.csRS485In2;
	camObj.digital_out = vIPCamInfo.csRS485Out1;
	camObj.httpport = _ttoi(vIPCamInfo.csWebPort);
	camObj.stream_url = vIPCamInfo.csStreamUrl;
	camObj.brand_code = vIPCamInfo.csSn;
	m_pBk->UpdateCamera(camObj);
}

void CDlgAddManageCam::vSaveGroupList()
{
	Cam_Group_Cam groupCamObj;
	DBIPCamGroupVector.clear();
	m_pBk->DeleteGroupCamera(::_ttoi(csMacExit));

	CListCtrl * pGroupList = (CListCtrl *)this->GetDlgItem(IDC_LIST_GROUP);				//Group List
	int iGroupID;
	for(int iTemp = 0, iGroupID = pGroupList->GetItemCount(); iTemp < pGroupList->GetItemCount(); iTemp++, iGroupID--)
	{
		groupCamObj.clear();
		if( pGroupList->GetCheck(iTemp) == 1 )
		{
			CString csGroupDesc = pGroupList->GetItemText(iTemp, 1);
			CString csGroupID;
			//csGroupID.Format(_T("%d"), g_vtIPCamGroup[iTemp].first);

			//pair <int , CString> pair_Insert(g_vtIPCamGroup[iTemp].first , csGroupDesc);
			//DBIPCamGroupVector.push_back(pair_Insert);

			groupCamObj.cameragroupid = ::_ttoi(csGroupID);
			groupCamObj.cameraid = m_pBk->GetLastCameraId();
			groupCamObj.camera_idx = ::_ttoi(vIPCamInfo.csCameraIDX);
			groupCamObj.streamid = 1;
			m_pBk->InsertGroupCamera(groupCamObj);
		}
	}
}

void CDlgAddManageCam::GetStream(vector<ec_Stream>& vcStream)
{
	USES_CONVERSION;

	ec_Stream streamObj;
	int nCount = _ttoi(vIPCamInfo.csStreamSum);
	for(int nIdx = 0; nIdx < nCount; nIdx++)
	{
		streamObj.clear();
		if(m_veMoStreamInfo.size() > 0)
		{
			vGetStreamDataFromVector(nIdx+1);
				
			streamObj.stream_name.Format(_T("Stream %d"), nIdx+1);
			streamObj.stream_type = vStreamInfo.csLV_RE;
			streamObj.stream_port = _ttoi(vStreamInfo.csStreamPort);
			streamObj.stream_protocol = vStreamInfo.csStreamProtocol;
			streamObj.video_cmpr_type = vStreamInfo.csStreamVideoCompressionType;
			streamObj.video_res = vStreamInfo.csStreamResolution;

			if(streamObj.stream_protocol == _T("RTSP(RTP-TCP)"))
			{
				streamObj.stream_protocol = _T("RTPTCP");
			}
			vcStream.push_back(streamObj);
		}
	}
}

void CDlgAddManageCam::vGetUIData()
{
	this->GetDlgItem(IDC_EDIT_CHANNEL_ID)->GetWindowTextW(vIPCamInfo.csCameraIDX);		//Channel ID

	this->GetDlgItem(IDC_EDIT_CAMERA_NAME)->GetWindowTextW(vIPCamInfo.csCameraName);	//Camera Name
	
	if( ((CButton*)(GetDlgItem( IDC_RADIO_DYNAMIC_IP )))->GetCheck() == 1 )				//Connection Type
		vIPCamInfo.csConnectType = _T("DHCP");
	else if( ((CButton*)(GetDlgItem( IDC_RADIO_STATIC_IP )))->GetCheck() == 1 )
		vIPCamInfo.csConnectType = _T("Static IP");
	else if( ((CButton*)(GetDlgItem( IDC_RADIO_PPPOE )))->GetCheck() == 1 )
		vIPCamInfo.csConnectType = _T("PPPoE");
	else
		vIPCamInfo.csConnectType = _T("");

	this->GetDlgItem(IDC_EDIT_WEB_PORT)->GetWindowTextW(vIPCamInfo.csWebPort);			//Web Port

	BYTE addr[4];																		//IP Address
	USES_CONVERSION;
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP_ADDRESS))->GetAddress(addr[0], addr[1], addr[2], addr[3]);	
	vIPCamInfo.csIP.Format(_T("%d.%d.%d.%d"), addr[0], addr[1], addr[2], addr[3]);

	this->GetDlgItem(IDC_EDIT_MODEL)->GetWindowTextW(vIPCamInfo.csModel);				//Model

	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_SUBNET_MASK))->GetAddress(addr[0], addr[1], addr[2], addr[3]);	//Subnet Mask
	vIPCamInfo.csNetmask.Format(_T("%d.%d.%d.%d"), addr[0], addr[1], addr[2], addr[3]);
	vIPCamInfo.csNetmask1.Format(_T("%d"), addr[0]);
	vIPCamInfo.csNetmask2.Format(_T("%d"), addr[1]);
	vIPCamInfo.csNetmask3.Format(_T("%d"), addr[2]);
	vIPCamInfo.csNetmask4.Format(_T("%d"), addr[3]);

	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_NAME_SERVER))->GetAddress(addr[0], addr[1], addr[2], addr[3]);	//Name Server
	vIPCamInfo.csDNS.Format(_T("%d.%d.%d.%d"), addr[0], addr[1], addr[2], addr[3]);

	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_GATEWAY))->GetAddress(addr[0], addr[1], addr[2], addr[3]);	//Gateway
	vIPCamInfo.csGateway.Format(_T("%d.%d.%d.%d"), addr[0], addr[1], addr[2], addr[3]);

	this->GetDlgItem(IDC_EDIT_MAC)->GetWindowTextW(vIPCamInfo.csMac);											//Mac Address
	vIPCamInfo.csMac.Remove('-');

	this->GetDlgItem(IDC_EDIT_USER)->GetWindowTextW(vIPCamInfo.csUser);							//User Name & Password
	this->GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowTextW(vIPCamInfo.csPWD);

	if( ((CButton*)(GetDlgItem( IDC_CHECK_CAMERA_ENABLE )))->GetCheck() == 1 )				//Camera Enabled
		vIPCamInfo.csActive = _T("Y");
	else if( ((CButton*)(GetDlgItem( IDC_CHECK_CAMERA_ENABLE )))->GetCheck() == 0 )
		vIPCamInfo.csActive = _T("N");

	CString csTemp;
	GetDlgItem(IDC_EDIT_PTZ_ENABLE)->GetWindowTextW(csTemp);								//PTZ Enable
	if(csTemp == _T("Enable"))
		vIPCamInfo.csRS485Enable = _T("Y");
	else if(csTemp == _T("Disable"))
		vIPCamInfo.csRS485Enable = _T("N");

	GetDlgItem(IDC_EDIT_DI_1)->GetWindowTextW(csTemp);
	if(csTemp == _T("Normal Open"))															//PTZ DI 1
		vIPCamInfo.csRS485In1 = _T("Y");
	else if(csTemp == _T("Normal Close"))
		vIPCamInfo.csRS485In1 = _T("N");
	
	GetDlgItem(IDC_EDIT_DI_2)->GetWindowTextW(csTemp);
	if(csTemp == _T("Normal Open"))															//PTZ DI 2
		vIPCamInfo.csRS485In2 = _T("Y");
	else if(csTemp == _T("Normal Close"))
		vIPCamInfo.csRS485In2 = _T("N");

	GetDlgItem(IDC_EDIT_DO)->GetWindowTextW(csTemp);
	if(csTemp == _T("Normal Open"))															//PTZ D0
		vIPCamInfo.csRS485Out1 = _T("Y");
	else if(csTemp == _T("Normal Close"))
		vIPCamInfo.csRS485Out1 = _T("N");

	GetDlgItem(IDC_EDIT_PTZ_PROTOCOL)->GetWindowTextW(vIPCamInfo.csRS485Protocol);			//PTZ Protocol
	
	//s_ScanList.csStreamValue.Format(_T("%d"),iStreamNum);									//StreamNum
}

void CDlgAddManageCam::vGetIPCamGroup(CString csCameraID)
{
	pair <int , CString> pair_groupID;
	vector<Cam_Group> vcGroup;
	vector<Cam_Group_Cam> vcGroupCam;
	m_pBk->GetGroup(DeviceTree, vcGroup);

	int nIdx = 0, nCount = vcGroup.size();
	for (nIdx = 0 ; nIdx < nCount; nIdx++)
	{
		vcGroupCam.clear();
		m_pBk->GetGroupCamera(vcGroup[nIdx].cameragroupid, _ttoi(csCameraID), vcGroupCam);

		if (vcGroupCam.size())
		{
			pair_groupID.first = vcGroup[nIdx].cameragroupid;
			pair_groupID.second = vcGroup[nIdx].camera_group_desc;
			DBIPCamGroupVector.push_back(pair_groupID);
		}
	}
}
void CDlgAddManageCam::OnBnClickedButtonDetectCamera()
{
	// TODO: Add your control notification handler code here
	ScanMap.clear();
	//initFlag = 0;
	m_veMoStreamInfo.clear();

	GetDlgItem(IDC_STATIC_LOADING)->ShowWindow(SW_SHOW);//Data Loading
	this->UpdateWindow();
	UpdateData(false);
	BeginWaitCursor();

	this->GetDlgItem(IDC_EDIT_WEB_PORT)->GetWindowTextW(vIPCamInfo.csWebPort);			//Web Port

	BYTE addr[4] = {0};																		//IP Address
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP_ADDRESS))->GetAddress(addr[0], addr[1], addr[2], addr[3]);	
	vIPCamInfo.csIP.Format(_T("%d.%d.%d.%d"), addr[0], addr[1], addr[2], addr[3]);
												
	this->GetDlgItem(IDC_EDIT_USER)->GetWindowTextW(vIPCamInfo.csUser);							//User Name & Password
	this->GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowTextW(vIPCamInfo.csPWD);

	vGetInfoFromENCP(vIPCamInfo.csIP);

	int iGetIPCamType = this->bGetIPCamType(vIPCamInfo.csIP);
	if(iGetIPCamType == OLD_TYPE)
	{
		vGetCGIData(vIPCamInfo.csIP);
		vGetIPCamDataFromMap(0);
		vGetStreamDataFromMap(0);

	}else if(iGetIPCamType == NEW_TYPE)
	{
		vGetCGIDataNew(vIPCamInfo.csIP);
		vGetIPCamDataFromMap(1);
		vGetStreamDataFromMap(1);

	}else if(iGetIPCamType == UNAUTHORIZED || iGetIPCamType == ERROR_TYPE)
	{
		GetDlgItem(IDC_STATIC_LOADING)->ShowWindow(SW_HIDE);//Data Loading
		UpdateData(false);
		EndWaitCursor();
		return;
	}

	if(::_ttoi(vIPCamInfo.csStreamSum) < 2 )
	{
		vIPCamInfo.csStreamSum.Format(_T("%d"), m_veMoStreamInfo.size());
	}

	//-----------Set UI
	this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->SetWindowTextW(vIPCamInfo.csIP);	//Set IP Address

	//USES_CONVERSION;
	CString csCameraName;
	csCameraName.Format(_T("%s - %s"), vIPCamInfo.csIP, vIPCamInfo.csModel);
	if(vIPCamInfo.csConnectType == _T("1"))											//Set Connection Type
	{
		((CButton*)(GetDlgItem( IDC_RADIO_DYNAMIC_IP )))->SetCheck( 1 );
		((CButton*)(GetDlgItem( IDC_RADIO_STATIC_IP )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_PPPOE )))->SetCheck( 0 );
		csCameraName.Format(_T("%s"), vIPCamInfo.csModel);
		this->GetDlgItem(IDC_EDIT_CAMERA_NAME)->SetWindowTextW(csCameraName);		//Set Camera Name
	}
	else if(vIPCamInfo.csConnectType == _T("0"))
	{
		((CButton*)(GetDlgItem( IDC_RADIO_DYNAMIC_IP )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_STATIC_IP )))->SetCheck( 1 );
		((CButton*)(GetDlgItem( IDC_RADIO_PPPOE )))->SetCheck( 0 );
		this->GetDlgItem(IDC_EDIT_CAMERA_NAME)->SetWindowTextW(csCameraName);		//Set Camera Name
	}
	else if (vIPCamInfo.csConnectType == _T("2"))
	{
		((CButton*)(GetDlgItem( IDC_RADIO_DYNAMIC_IP )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_STATIC_IP )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_PPPOE )))->SetCheck( 1 );
		this->GetDlgItem(IDC_EDIT_CAMERA_NAME)->SetWindowTextW(csCameraName);		//Set Camera Name
	}

	this->GetDlgItem(IDC_EDIT_MODEL)->SetWindowTextW(vIPCamInfo.csModel);				//Set Model

	if(vIPCamInfo.csNetmask == _T(""))
		vIPCamInfo.csNetmask = _T("0.0.0.0");
	this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->SetWindowTextW(vIPCamInfo.csNetmask);	//Set Subnet Mask

	if(vIPCamInfo.csDNS == _T(""))
		vIPCamInfo.csDNS = _T("0.0.0.0");
	this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->SetWindowTextW(vIPCamInfo.csDNS);	//Set Name Server
	
	if(vIPCamInfo.csGateway == _T(""))
		vIPCamInfo.csGateway = _T("0.0.0.0");
	this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->SetWindowTextW(vIPCamInfo.csGateway);	//Set Gateway

	if(vIPCamInfo.csMac.GetLength() < 5 || vIPCamInfo.csMac.Find(_T("-")) == -1 )
	{
		CString csIP2MAC;
		csIP2MAC.Format(_T("%03d-%03d-%03d-%03d"), addr[0], addr[1], addr[2], addr[3]);
		vIPCamInfo.csMac = csIP2MAC;
	}

	this->GetDlgItem(IDC_EDIT_MAC)->SetWindowTextW(vIPCamInfo.csMac);						//Set Mac Address

	this->GetDlgItem(IDC_EDIT_WEB_PORT)->SetWindowTextW(vIPCamInfo.csWebPort);				//Web Port

	vInitStreamProtocol();

	StreamInit();

	vSetStreamPTZUI();

	//-----------Display Event Info
	vDisplayEventInfo();

	//--Check Default Group
	//GetDlgItem(IDC_BUTTON_GROUP_SETTING)->EnableWindow(1);
	if(initGroupFlag == false)
	{
		vGroupListInit();
		initGroupFlag = true;
	}
	vCheckDefaultGroup();
	for(int iTemp = 0; iTemp < DBIPCamGroupVector.size(); iTemp++)
	{
		//vCheckDefaultGroup(DBIPCamGroupVector[iTemp]);
		vCheckDefaultGroup(DBIPCamGroupVector[iTemp].second);
	}

	//--Check Recording Text Box
	if( ((CButton*)GetDlgItem(IDC_COMBO_RE))->GetWindowTextLengthW() > 0)
	{
		//((CButton*)GetDlgItem(IDC_BUTTON_RECORDING_DLG))->EnableWindow(1);
		if(initFlag != ADDCAMERA)
			((CButton*)GetDlgItem(IDC_BUTTON_RECORDING_DLG))->EnableWindow(1);
	}

	GetDlgItem(IDC_STATIC_LOADING)->ShowWindow(SW_HIDE);//Data Loading
    UpdateData();
	EndWaitCursor();

	this->dirtyFlag = false;
    BringWindowToTop();

	//--Enable NAS
	if(iGetIPCamType == NEW_TYPE)
	{
		(CButton*)GetDlgItem(IDC_CHECK_NAS)->EnableWindow(1);
	}
}

void CDlgAddManageCam::vDirtyFlagChanged()
{
	dirtyFlag = true;
}
void CDlgAddManageCam::OnBnClickedCheckCameraEnable()
{
	// TODO: Add your control notification handler code here
	dirtyFlag = true;
}

void CDlgAddManageCam::OnEnChangeEditCameraName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBaseDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	dirtyFlag = true;
}

void CDlgAddManageCam::OnBnClickedRadioDynamicIp()
{
	// TODO: Add your control notification handler code here
	dirtyFlag = true;
	this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->EnableWindow(0);
	this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->EnableWindow(0);
	this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->EnableWindow(0);
	this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(0);
}

void CDlgAddManageCam::OnBnClickedRadioStaticIp()
{
	// TODO: Add your control notification handler code here
	dirtyFlag = true;
	this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->EnableWindow(1);
	this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->EnableWindow(1);
	this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->EnableWindow(1);
	this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(1);
}

void CDlgAddManageCam::OnBnClickedRadioPppoe()
{
	// TODO: Add your control notification handler code here
	dirtyFlag = true;
	this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->EnableWindow(1);
	this->GetDlgItem(IDC_IPADDRESS_SUBNET_MASK)->EnableWindow(1);
	this->GetDlgItem(IDC_IPADDRESS_NAME_SERVER)->EnableWindow(1);
	this->GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(1);
}

void CDlgAddManageCam::OnIpnFieldchangedIpaddressIpAddress(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	CString csWebPort, csUser, csPWD;

	this->GetDlgItem(IDC_EDIT_WEB_PORT)->GetWindowTextW(csWebPort);			//Web Port

	this->GetDlgItem(IDC_EDIT_USER)->GetWindowTextW(csUser);							//User Name & Password
	this->GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowTextW(csPWD);

	if(csWebPort.GetLength() > 0 && csUser.GetLength() > 0 && csPWD.GetLength() > 0)
		GetDlgItem(IDC_BUTTON_DETECT_CAMERA)->EnableWindow(1);

	dirtyFlag = true;
	*pResult = 0;
}

void CDlgAddManageCam::OnEnChangeEditWebPort()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBaseDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString csIP, csUser, csPWD;

	BYTE addr[4];																		//IP Address
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP_ADDRESS))->GetAddress(addr[0], addr[1], addr[2], addr[3]);	
	csIP.Format(_T("%d.%d.%d.%d"), addr[0], addr[1], addr[2], addr[3]);

	this->GetDlgItem(IDC_EDIT_USER)->GetWindowTextW(csUser);							//User Name & Password
	this->GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowTextW(csPWD);

	if(csIP.GetLength() > 0 && csUser.GetLength() > 0 && csPWD.GetLength() > 0)
		GetDlgItem(IDC_BUTTON_DETECT_CAMERA)->EnableWindow(1);

	dirtyFlag = true;
}

void CDlgAddManageCam::OnIpnFieldchangedIpaddressSubnetMask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	dirtyFlag = true;
	*pResult = 0;
}

void CDlgAddManageCam::OnIpnFieldchangedIpaddressNameServer(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	dirtyFlag = true;
	*pResult = 0;
}

void CDlgAddManageCam::OnIpnFieldchangedIpaddressGateway(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	dirtyFlag = true;
	*pResult = 0;
}

void CDlgAddManageCam::OnEnChangeEditUser()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBaseDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString csWebPort, csIP, csPWD;

	this->GetDlgItem(IDC_EDIT_WEB_PORT)->GetWindowTextW(csWebPort);			//Web Port

	BYTE addr[4];																		//IP Address
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP_ADDRESS))->GetAddress(addr[0], addr[1], addr[2], addr[3]);	
	csIP.Format(_T("%d.%d.%d.%d"), addr[0], addr[1], addr[2], addr[3]);

	//User Name & Password
	this->GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowTextW(csPWD);

	if(csIP.GetLength() > 0 && csWebPort.GetLength() > 0 && csPWD.GetLength() > 0)
		GetDlgItem(IDC_BUTTON_DETECT_CAMERA)->EnableWindow(1);

	dirtyFlag = true;
}

void CDlgAddManageCam::OnEnChangeEditPassword()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBaseDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString csWebPort, csIP, csUser, csPWD;

	this->GetDlgItem(IDC_EDIT_WEB_PORT)->GetWindowTextW(csWebPort);			//Web Port

	BYTE addr[4];																		//IP Address
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP_ADDRESS))->GetAddress(addr[0], addr[1], addr[2], addr[3]);	
	csIP.Format(_T("%d.%d.%d.%d"), addr[0], addr[1], addr[2], addr[3]);

	this->GetDlgItem(IDC_EDIT_USER)->GetWindowTextW(csUser);							//User Name & Password

	if(csIP.GetLength() > 0 && csWebPort.GetLength() > 0 && csUser.GetLength() > 0)
		GetDlgItem(IDC_BUTTON_DETECT_CAMERA)->EnableWindow(1);

	dirtyFlag = true;
}

void CDlgAddManageCam::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	//Check MAC exit or not
	if (vIPCamInfo.csMac.GetLength() == 0)	dirtyFlag = false;

	if(dirtyFlag == true)
	{
		int iRes = AfxMessageBox(IDS_SAVE_DATA,MB_YESNO | MB_ICONQUESTION);
		//int iRes = AfxMessageBox(_T("Data Already Modified, Would you want save Data ?"),MB_YESNO | MB_ICONQUESTION);
		if(iRes == IDYES)
		{
			OnBnClickedButtonSaveDuplicate();
		}
		else if(iRes == IDNO)
		{
			CDialog::OnCancel();
			return;
		}
	}

	CDialog::OnCancel();

}

BOOL CDlgAddManageCam::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
		case WM_KEYDOWN:
		{
			switch( pMsg->wParam )
			{
				case VK_ESCAPE:
						if (dirtyFlag == true )
						{
							int ll_ret = AfxMessageBox(IDS_SAVE_DATA, MB_YESNO|MB_ICONQUESTION);		
							//CString ls_confirm = _T("Data had been changed, do you want to save the changes ?");
							//int ll_ret = AfxMessageBox(ls_confirm, MB_YESNO|MB_ICONQUESTION);		
							if (ll_ret==IDYES) 	
							{				
								OnBnClickedButtonSaveDuplicate();
							}
							else
							{
								CDialog::OnCancel();
								return true;
							}
							::DispatchMessage( pMsg );
						}
				break;
			} //switch
		} // Case WM_KEYDOWN:
	}

	return CBaseDialog::PreTranslateMessage(pMsg);
}

void CDlgAddManageCam::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnClose();
}

void CDlgAddManageCam::OnBnClickedOk()
{
	this->GetDlgItem(IDC_IPADDRESS_IP_ADDRESS)->GetWindowTextW(vIPCamInfo.csIP);	//IP Address
	this->GetDlgItem(IDC_EDIT_WEB_PORT)->GetWindowTextW( vIPCamInfo.csWebPort );	//Set Web Port
	this->GetDlgItem(IDC_EDIT_USER)->GetWindowTextW(vIPCamInfo.csUser);							//User
	this->GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowTextW(vIPCamInfo.csPWD);			//Password
	this->GetDlgItem(IDC_EDIT_MAC)->GetWindowTextW(vIPCamInfo.csMac);							//Mac Address

	//--SET NAS 
	if( ((CButton*)GetDlgItem(IDC_CHECK_NAS))->GetCheck() )
	{
		vGetNASInfoFromCamera();

		TCHAR tzTmp[MAX_PATH], tzServer[MAX_PATH], tzPath[MAX_PATH];
		_tcscpy(tzTmp, m_NASInfo.tcServerAddr);
		_tcscpy(tzServer, _tcstok(tzTmp, _T("\\")));
		_tcscpy(tzPath, _tcstok(NULL, _T("\\")));

		CCrypto m_CCrypto;
		m_CCrypto.DeriveKey(_T("Encrypt!@#$%^&*()_+-~`"));
		CString csDecrypt = m_CCrypto.DecryptPassword(m_NASInfo.tcServerPass);
		CString csServerType;
		csServerType.Format(_T("%s"), m_NASInfo.tcServerType);
		if(csServerType.Find(_T("Samba")) != -1 )	csServerType = _T("SMB");

		CString csUrl[3];
		csUrl[0].Format(_T("http://%s/config/nas.cgi?nas=yes&protocol=%s&addr=%s&folder=%%2F%s&pass=%s&user=%s"), vIPCamInfo.csIP, csServerType, tzServer, tzPath, csDecrypt, m_NASInfo.tcServerUser);	//-- %%2F -> /
		csUrl[1].Format(_T("http://%s/config/record_cont.cgi?crec_max_fsize=100&crec_storage=nas&crec_sched_rec=on&crec_sched_wday=1,1,1,1,1,1,1&crec_sched_stm=0000&crec_sched_etm=2359"), vIPCamInfo.csIP);
		csUrl[2].Format(_T("http://%s/config/video.cgi?framerate_p1=%s&ratecontrol_p1=%s&bitrate_p1=%s&resolution_p1=%s&codec_p1=%s&mode_p1=%s&record_p1=on&tvout_p1=%s&roistartx_p1=%s&roistarty_p1=%s&quality_p1=%s&contrec_p1=%s&sensor_mode=%s"), 
			vIPCamInfo.csIP, 
			mapNAS[_T("config/video.cgi?profile_id=1")].aSet[_T("framerate")].csKey, 
			mapNAS[_T("config/video.cgi?profile_id=1")].aSet[_T("ratecontrol")].csKey, 
			mapNAS[_T("config/video.cgi?profile_id=1")].aSet[_T("bitrate")].csKey, 
			mapNAS[_T("config/video.cgi?profile_id=1")].aSet[_T("resolution")].csKey, 
			mapNAS[_T("config/video.cgi?profile_id=1")].aSet[_T("codec")].csKey, 
			mapNAS[_T("config/video.cgi?profile_id=1")].aSet[_T("mode")].csKey, 
			mapNAS[_T("config/video.cgi?profile_id=1")].aSet[_T("tvout")].csKey, 
			mapNAS[_T("config/video.cgi?profile_id=1")].aSet[_T("roistartx")].csKey, 
			mapNAS[_T("config/video.cgi?profile_id=1")].aSet[_T("roistarty")].csKey, 
			mapNAS[_T("config/video.cgi?profile_id=1")].aSet[_T("quality")].csKey, 
			mapNAS[_T("config/video.cgi?profile_id=1")].aSet[_T("contrec")].csKey, 
			mapNAS[_T("config/video.cgi")].aSet[_T("sensor_mode")].csKey);

		Curlplus CGIScanData;
		for(int iTemp = 0; iTemp <= 2; iTemp++)
		{
			CString csReturn = CGIScanData.get(csUrl[iTemp], vIPCamInfo.csUser, vIPCamInfo.csPWD);
			//AfxMessageBox(_T("pause"));
		}

	}

	// TODO: Add your control notification handler code here
	//--Check one of IP, web port, username, password is blank or not.
	if(vIPCamInfo.csIP == _T("0.0.0.0") || vIPCamInfo.csWebPort == _T("") || vIPCamInfo.csUser == _T("") || vIPCamInfo.csPWD == _T(""))
	{
		OnBnClickedButtonDetectCamera();
		if(vIPCamInfo.csIP == _T("0.0.0.0") || vIPCamInfo.csWebPort == _T("") || vIPCamInfo.csUser == _T("") || vIPCamInfo.csPWD == _T(""))
		{
			CString csMsg(_T(""));
			//csMsg.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_DATA_ERROR);
			LOADSTRING(csMsg, IDS_ADD_CAM_DATA_ERROR);
			//AfxMessageBox(_T("Data Error. Please check:\r\n1.IP Address.\r\n2.Web Port.\r\n3.Username.\r\n4.Password."));
			AfxMessageBox(csMsg);
			return;
		}
	}

	if (vIPCamInfo.csMac.GetLength() == 0)
	{
		CString csMsg(_T(""));
		//csMsg.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_CAMERA_NOT_FOUND);
		LOADSTRING(csMsg, IDS_ADD_CAM_CAMERA_NOT_FOUND);
		AfxMessageBox(csMsg);
		//AfxMessageBox(_T("IP Camera not found.\r\nPlease check this camera exist first."));
		return;
	}

	//Lynn modified at 2013/10/22
	vUpdateNodeItem();

	OnBnClickedButtonSaveDuplicate();
	
	
	vGetPathBufferFromDB();

	map<CString, int>::iterator iPos = mapPathBuffer.begin();
	for(; iPos != mapPathBuffer.end(); iPos++)
	{
		if(!bCheckPathBuffer(iPos->first, iPos->second))
		{
			CString csMsg1, csMSG;
			LOADSTRING(csMsg1, IDS_SET_RECORDING_ERROR_1);
			csMSG.Format(_T("%s%s, Are you sure to continue ?"), csMsg1.GetBuffer(), iPos->first);
			int iRet = AfxMessageBox(csMSG, MB_YESNO);

			if(iRet == IDYES)
			{
				break;
			}
			else if(iRet == IDNO)
			{
				return;
			}
		}
	}

	CDialog::OnOK();
}

//Lynn modified at 2013/10/22
void CDlgAddManageCam::vUpdateNodeItem()
{
	//Lynn modified for surveillance show at 2013/10/25 
	//m_Node.dyDeviceType.enFishEye = false;

	map<CString, sVERSETS>::iterator it = ScanMap.find(_T("config/mcp.cgi?mcp=System"));
	if (it != ScanMap.end())
	{
		map<CString, sSETS>::iterator its = it->second.aSet.find(_T("mcp_system_fisheye"));
		if (its != it->second.aSet.end())
		{
			//Lynn modified for surveillance show at 2013/10/25 
			if (its->second.csKey == _T("yes"))
			{
// 				m_Node.bFishEye = TRUE;
// 				CIPCamDiscovery::SetNodePtr(m_Node);
			}
		}
	}
}

void CDlgAddManageCam::vGetInfoFromENCP(CString csIP)
{
	USES_CONVERSION;
	//m_pvtNode = CIPCamDiscovery::GetNodePtr();

	for(int iTemp = 0; iTemp < m_pvtNode->size(); iTemp++)
	{
		NODEITEM* vpNode = (*this->m_pvtNode)[iTemp];
		CString csNodeIP;
		csNodeIP.Format(_T("%s"), A2W(vpNode->ip));
		if(csNodeIP == csIP)
		{
			vIPCamInfo.csMac.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"), vpNode->mac[0], vpNode->mac[1], vpNode->mac[2], vpNode->mac[3], vpNode->mac[4], vpNode->mac[5]);
			vIPCamInfo.csModel.Format(_T("%s"), A2W(vpNode->modelname));
			vIPCamInfo.csWebPort.Format(_T("%d"), vpNode->nWebPort);
            //vIPCamInfo.csStreamSum = _T("2");
			vIPCamInfo.csIP = csNodeIP;
			vIPCamInfo.csConnectType.Format(_T("%d"), vpNode->nType);
            vIPCamInfo.csSn = A2W(vpNode->sn);
			
			CString csStreamProtocol, csStream1Port, csStream2Port;
			int iStreamProtocol;
			/*if((vpNode->nProtocol) == NODEITEM::VSMP)
				csStreamProtocol = _T("TCP");
			else if((vpNode->nProtocol) == NODEITEM::CGI)
				csStreamProtocol = _T("CGI");
			else if((vpNode->nProtocol) == NODEITEM::RTSP)
				csStreamProtocol = _T("RTSP");
			else if((vpNode->nProtocol) == NODEITEM::RTPOVERTCP)
				csStreamProtocol = _T("RTPTCP");
			else if((vpNode->nProtocol) == NODEITEM::RTPOVERHTTP)
				csStreamProtocol = _T("RTPHTTP");
			else if((vpNode->nProtocol) == NODEITEM::AVIFILE)
				csStreamProtocol = _T("AVIFILE");*/

			if(vpNode->nAPPort > -1)
			{
				csStream1Port.Format(_T("%d"), vpNode->nAPPort);
				_tcscpy(m_MoStreamInfo.tcPort, csStream1Port);
				_tcscpy(m_MoStreamInfo.tcProtocol, csStreamProtocol);
				_tcscpy(m_MoStreamInfo.tcLV_RE, _T(""));
				_tcscpy(m_MoStreamInfo.tcResolution, _T("Unknown"));
				_tcscpy(m_MoStreamInfo.tcVideoType, _T("Unknown"));
				
				m_veMoStreamInfo.push_back(m_MoStreamInfo);
			}
			 
			if(vpNode->nAP2Port > -1)
			{
				csStream2Port.Format(_T("%d"), vpNode->nAP2Port);
				_tcscpy(m_MoStreamInfo.tcPort, csStream2Port);
				_tcscpy(m_MoStreamInfo.tcProtocol, csStreamProtocol);
				_tcscpy(m_MoStreamInfo.tcLV_RE, _T(""));
				_tcscpy(m_MoStreamInfo.tcResolution, _T("Unknown"));
				_tcscpy(m_MoStreamInfo.tcVideoType, _T("Unknown"));
				
				m_veMoStreamInfo.push_back(m_MoStreamInfo);
			}

			ENCPFlag = true;
			pNode = vpNode;
		}
	}
}

void CDlgAddManageCam::vSetDefaultRecordType()
{
	ec_Camera camObj;
	ec_Camera_Rec camRecObj;

	m_pBk->GetCamera(_ttoi(csMacExit), camObj);
	camRecObj.camera_idx = camObj.camera_idx;
	camRecObj.recording_type = _T("0");
	m_pBk->InsertCamRec(camRecObj);
}

int CDlgAddManageCam::bGetIPCamType(CString csIP)
{
	Curlplus CGIScanData;
	CString csOldUrl, csOldBuffer, csNewUrl, csNewBuffer;
	csOldUrl.Format(_T("http://%s:%s/config/stream_info.cgi"), csIP, vIPCamInfo.csWebPort);	
	csOldBuffer = CGIScanData.get(csOldUrl, vIPCamInfo.csUser, vIPCamInfo.csPWD);

	csNewUrl.Format(_T("http://%s:%s/config/stream.cgi"), csIP, vIPCamInfo.csWebPort);
	csNewBuffer = CGIScanData.get(csNewUrl, vIPCamInfo.csUser, vIPCamInfo.csPWD);

	if(csOldBuffer.Find(_T("vprofilenum")) != -1 && csNewBuffer.Find(_T("rtsp_port")) == -1)
		return OLD_TYPE;
	else if(csOldBuffer.Find(_T("vprofilenum")) == -1 && csNewBuffer.Find(_T("rtsp_port")) != -1)
		return NEW_TYPE;
	else if(csOldBuffer.Find(_T("401")) != -1 && csNewBuffer.Find(_T("401")) != -1)
	{
		CString csMsg(_T(""));
		//csMsg.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_UNAUTHORIZED);
		LOADSTRING(csMsg, IDS_ADD_CAM_UNAUTHORIZED);
		AfxMessageBox(csMsg);
		//AfxMessageBox(_T("Unauthorized.Please Check the Username & Password and click Button \"Get Camera Info\""));
		return UNAUTHORIZED;
	}
	else if(csOldBuffer == _T("") && csNewBuffer == _T(""))
	{
		CString csMsg(_T(""));
		//csMsg.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_NOT_FOUND);
		LOADSTRING(csMsg, IDS_ADD_CAM_NOT_FOUND);
		AfxMessageBox(csMsg);
		//AfxMessageBox(_T("IP Camera Not Found."));
		return ERROR_TYPE;
	}
	else
		return ERROR_TYPE;
}

UINT CDlgAddManageCam::vGetCGIData(LPVOID lpParam)
{
	CDlgAddManageCam* pThis = (CDlgAddManageCam*)lpParam;

	pThis->ScanMap[_T("config/stream_info.cgi")];

	map<CString, sVERSETS>::iterator pos = pThis->ScanMap.begin();
	map<CString, sSETS>::iterator pos2;

	for(; pos != pThis->ScanMap.end(); pos++)
	{
		CString csUrl, csBuffer;
		Curlplus CGIScanData;
		
		csUrl.Format(_T("http://%s:%s/%s"), pThis->csThreadIP, pThis->vIPCamInfo.csWebPort, pos->first);	
		csBuffer = CGIScanData.get(csUrl, pThis->vIPCamInfo.csUser, pThis->vIPCamInfo.csPWD);

		if(csBuffer == _T(""))
		{
			CString csMsg(_T(""));
			//csMsg.LoadStringW(CGS::g_hResStr, IDS_ADD_CAM_NOT_FOUND);
			LOADSTRING(csMsg, IDS_ADD_CAM_NOT_FOUND);
			AfxMessageBox(csMsg);
		//AfxMessageBox(_T("IP Camera Not Found."));
			return 0;
		}

		char *buffer = new char[csBuffer.GetLength() + 1];
		wcstombs_s(NULL, buffer, csBuffer.GetLength() + 1, csBuffer, _TRUNCATE);
	
		TCHAR tzBuffer[MAXBUF];
		MultiByteToWideChar(CP_ACP, 0, buffer, -1, tzBuffer, MAXBUF);
		pThis->vParseSegment(tzBuffer, pos);
		delete buffer;
	}
		
	/*--------Get Stream Number----------*/
	pThis->vIPCamInfo.csStreamSum = pThis->ScanMap[_T("config/stream_info.cgi")].aSet[_T("vprofilenum")].csKey;

	for(int iTemp = 1; iTemp <= ::_ttoi(pThis->vIPCamInfo.csStreamSum); iTemp++)
	{
		CString csVideo;
		csVideo.Format(_T("config/video.cgi?profileid=%d"), iTemp);
		pThis->ScanMap[csVideo];
	}

	pThis->ScanMap[_T("config/rs485.cgi")];
    pThis->ScanMap[_T("config/mcp.cgi?mcp=System")];	
	pThis->ScanMap[_T("config/io.cgi")];
	pThis->ScanMap[_T("config/network.cgi")];
	pThis->ScanMap[_T("config/event_info.cgi")];
	pThis->ScanMap[_T("common/info.cgi")];
	pThis->ScanMap[_T("config/stream.cgi")];

	for(pos = pThis->ScanMap.begin(); pos != pThis->ScanMap.end(); pos++)
	{
		CString csUrl, csBuffer;
		Curlplus CGIScanData;

		csUrl.Format(_T("http://%s:%s/%s"), pThis->csThreadIP, pThis->vIPCamInfo.csWebPort, pos->first);
		csBuffer = CGIScanData.get(csUrl, pThis->vIPCamInfo.csUser, pThis->vIPCamInfo.csPWD);
		
		char *buffer = new char[csBuffer.GetLength() + 1];
		wcstombs_s(NULL, buffer, csBuffer.GetLength() + 1, csBuffer, _TRUNCATE);
	
		TCHAR tzBuffer[MAXBUF];
		MultiByteToWideChar(CP_ACP, 0, buffer, -1, tzBuffer, MAXBUF);
		pThis->vParseSegment(tzBuffer, pos);
		delete buffer;

		/*-------------GET Event Info-------------*/

		if(pos->first == _T("config/event_info.cgi"))
		{
			pThis->vCheckEvent(pos, pThis->csThreadIP);
		}
	}

	pThis->vGetIPCamDataFromMap(0);	//Old Type IP Cam
	
	HANDLE hCGIThread = CreateEvent(NULL, false, false, _T("Global\\GET_CGI_DATA"));
	if (hCGIThread == NULL)
	{
		TRACE(_T("Error code :%d"), ::GetLastError());
	}
	SetEvent( hCGIThread );

	return 0;
}

UINT CDlgAddManageCam::vGetCGIDataNew(LPVOID lpParam)
{
	CDlgAddManageCam* pThis = (CDlgAddManageCam*)lpParam;

	map<CString, sVERSETS>::iterator pos;
	map<CString, sSETS>::iterator pos2;

	/*---Get Stream, rs485, io, network.cgi----------*/

	pThis->ScanMap[_T("config/stream.cgi")];
	pThis->ScanMap[_T("config/rs485.cgi")];
    pThis->ScanMap[_T("config/mcp.cgi?mcp=System")];	
	pThis->ScanMap[_T("config/io.cgi")];
	pThis->ScanMap[_T("config/network.cgi")];
	pThis->ScanMap[_T("config/audio.cgi")];
	pThis->ScanMap[_T("config/info.cgi")];
	pThis->ScanMap[_T("config/stream.cgi")];

	for(pos = pThis->ScanMap.begin(); pos != pThis->ScanMap.end(); pos++)
	{
		CString csUrl, csBuffer;
		Curlplus CGIScanData;

		csUrl.Format(_T("http://%s:%s/%s"), pThis->csThreadIP, pThis->vIPCamInfo.csWebPort, pos->first);

		csBuffer = CGIScanData.get(csUrl, pThis->vIPCamInfo.csUser, pThis->vIPCamInfo.csPWD);
		if((csBuffer.Find(_T("Denied")) != -1) || (csBuffer.Find(_T("Error")) != -1))
		{
			//AfxMessageBox(_T("Access Denied"));
			TRACE(_T("Access Denied"));
			return 0;	
		}
		else if( csBuffer == _T("") )
		{
			//AfxMessageBox(_T("Network Error"));
			TRACE(_T("Network Error"));
			return 0;
		}
		else if(csBuffer.Find(_T("401")) != -1 && csBuffer.Find(_T("Unauthorized")) != -1)
		{
			TRACE(_T("401:Unauthorized"));
			return 0;
		}
			csBuffer.Remove(_T('{'));
			csBuffer.Remove(_T('"'));
			csBuffer.Remove(_T('}'));
			csBuffer.Remove(_T(' '));
		
		char *buffer = new char[csBuffer.GetLength() + 1];
		wcstombs_s(NULL, buffer, csBuffer.GetLength() + 1, csBuffer, _TRUNCATE);
	
		TCHAR tzBuffer[MAXBUF];
		MultiByteToWideChar(CP_ACP, 0, buffer, -1, tzBuffer, MAXBUF);
		pThis->vParseSegmentNew(tzBuffer, pos);
		delete buffer;
	}
		
	/*--------Get Stream Number && video.cgi----------*/
	    CString csUrl, csBuffer;
		csUrl.Format(_T("http://%s:%s/config/video.cgi"), pThis->csThreadIP, pThis->vIPCamInfo.csWebPort);
		Curlplus CGIScanData;

		csBuffer = CGIScanData.get(csUrl, pThis->vIPCamInfo.csUser, pThis->vIPCamInfo.csPWD);
		
		csBuffer.Remove(_T('{'));
		csBuffer.Remove(_T('"'));
		csBuffer.Remove(_T('}'));
		
		char *buffer = new char[csBuffer.GetLength() + 1];
		wcstombs_s(NULL, buffer, csBuffer.GetLength() + 1, csBuffer, _TRUNCATE);
	
		TCHAR tzBuffer[MAXBUF];
		MultiByteToWideChar(CP_ACP, 0, buffer, -1, tzBuffer, MAXBUF);

		pThis->vIPCamInfo.csStreamSum.Format(_T("%d"), pThis->ParseSegmentVideoCgi(tzBuffer, pos));
		delete buffer;

	pThis->vGetIPCamDataFromMap(1);	//New Type IP Cam

	HANDLE hCGIThread = CreateEvent(NULL, false, false, _T("Global\\GET_NEW_CGI_DATA"));
	if (hCGIThread == NULL)
	{
		TRACE(_T("Error code :%d"), ::GetLastError());
	}
	SetEvent( hCGIThread );

	return 0;
}

void CDlgAddManageCam::OnBnClickedButtonRecordingDlg()
{
	// TODO: Add your control notification handler code here
	RecordingFlag = true;

	CString csIP, csCameraIDX;
	GetDlgItemText(IDC_IPADDRESS_IP_ADDRESS, csIP);
	GetDlgItemText(IDC_EDIT_CHANNEL_ID, csCameraIDX);

	::ZeroMemory(&m_SQLBytes, sizeof(m_SQLBytes));
	CDlgSettingRecording pCDlgSettingRecording;
	if( (initFlag == 0 && csMacExit.GetLength() == 0) || (initFlag == 1 && csMacExit.GetLength() == 0) || (initFlag == 2 && csMacExit.GetLength() == 0))
	{
		pCDlgSettingRecording.vSetInitFlag(NEW_CAMERA, csIP, ::_ttoi(csCameraIDX));
		if(pCDlgSettingRecording.DoModal() == IDOK)
		{
			::memcpy(&m_SQLBytes, &pCDlgSettingRecording.bySQLByte, sizeof(m_SQLBytes));
		}
	}
	else if(initFlag == 3 || csMacExit.GetLength() != 0)
	{
		pCDlgSettingRecording.vSetInitFlag(MODIFY_CAMERA, csIP, ::_ttoi(csCameraIDX));
		pCDlgSettingRecording.DoModal();
	}
	else
	{
		pCDlgSettingRecording.vSetInitFlag(NEW_CAMERA, csIP, ::_ttoi(csCameraIDX));
		pCDlgSettingRecording.DoModal();
	}

	bSaveNewRecordingPath(csCameraIDX);
}

void CDlgAddManageCam::OnBnClickedButtonGroupSetting()
{
	// TODO: Add your control notification handler code here
	CDlgSettingCamGroup pCDlgSettingCamGroup;
	pCDlgSettingCamGroup.DoModal();
}

bool CDlgAddManageCam::bSaveNewRecordingPath(CString csCameraIDX)
{
	SQLRETURN rc(-1);
    SQLINTEGER cbPhotoParam;
    SQLHSTMT hstmt;
	int iRecordingType = 0;

	if( m_SQLBytes[0] == 0x00)	iRecordingType = 1;
	else	iRecordingType = 0;

    if (SQL_SUCCESS == (rc = ::SQLAllocHandle(SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt)))
    {
        CString csSQL;
		csSQL.Format(_T("INSERT INTO ec_camera_recording(camera_idx, recording_type, recording_time) VALUES ('%s', '%d', ? )"), csCameraIDX, iRecordingType);
        rc = SQLPrepare(hstmt, csSQL.GetBuffer(), SQL_NTS);
        if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
        {
            //SQLINTEGER nSize(43);
			SQLLEN nSize(43);
	        SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT,	SQL_C_BINARY, SQL_BINARY, 0, 0, (SQLPOINTER) m_SQLBytes, 0, &nSize);
	        cbPhotoParam = SQL_LEN_DATA_AT_EXEC(0);         
            rc = SQLExecute(hstmt);	
            if (rc == SQL_ERROR)
            {
                SQLTCHAR      SqlState[6]={0}; 
                SQLTCHAR  Msg[SQL_MAX_MESSAGE_LENGTH]={0};  
                SQLINTEGER    NativeError; 
                SQLSMALLINT   MsgLen;             
                SQLRETURN     nRet; 
                nRet = SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, 1, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen); 
                AfxMessageBox(Msg);
            }
        }
    }
	if (hstmt)
	{
		::SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	}
	return ((SQL_SUCCESS == rc) || (SQL_SUCCESS_WITH_INFO == rc));
}

void CDlgAddManageCam::OnBnClickedButtonRemoteCamera()
{
	// TODO: Add your control notification handler code here
	CDlgAddRemoteNVR m_RemoteNVR;
	if( m_RemoteNVR.DoModal() == IDOK)
	{
		//AfxMessageBox(_T("OK!"));
	}
}

void CDlgAddManageCam::OnBnClickedRadioRtspTcp()
{
	// TODO: Add your control notification handler code here
	CComboBox* pAVStream =(CComboBox*) GetDlgItem(IDC_COMBO_AV_STREAM);
	int iCurSel = pAVStream->GetCurSel();

	CString csProtocol(_T("RTPTCP"));
	_tcscpy(m_veMoStreamInfo[iCurSel].tcProtocol, csProtocol);
	
}

void CDlgAddManageCam::OnBnClickedRadioRtspUdp()
{
	// TODO: Add your control notification handler code here
	CComboBox* pAVStream =(CComboBox*) GetDlgItem(IDC_COMBO_AV_STREAM);
	int iCurSel = pAVStream->GetCurSel();

	CString csProtocol(_T("RTSP"));
	_tcscpy(m_veMoStreamInfo[iCurSel].tcProtocol, csProtocol);
}

void CDlgAddManageCam::vInitStreamProtocol()
{
	CComboBox* pStreamProtocol = (CComboBox*) GetDlgItem(IDC_COMBO_STREAM_PROTOCOL);
	pStreamProtocol->ResetContent();
	ASSERT(pStreamProtocol->GetCount() == 0);

	TCHAR* pszProtocol[5] = {_T("CGI"), _T("TCP"), _T("RTSP"), _T("Unknown")};

	for(int iTemp = 0; iTemp <= 3; iTemp++)
	{
		pStreamProtocol->AddString(pszProtocol[iTemp]);
	}

	pStreamProtocol->SetCurSel(0);
}
//void CDlgAddManageCam::OnNMThemeChangedComboStreamProtocol(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// This feature requires Windows XP or greater.
//	// The symbol _WIN32_WINNT must be >= 0x0501.
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

void CDlgAddManageCam::OnCbnSelchangeComboStreamProtocol()
{
	// TODO: Add your control notification handler code here

	CComboBox* pStreamProtocol = (CComboBox*) GetDlgItem(IDC_COMBO_STREAM_PROTOCOL);
	int iCurSel = pStreamProtocol->GetCurSel();
	CString csCurSel;
	pStreamProtocol->GetLBText(iCurSel, csCurSel);

	if(csCurSel.Find(_T("RTSP")) == -1)
	{
		GetDlgItem(IDC_STATIC_RTSPOVER)->ShowWindow(0);
		GetDlgItem(IDC_RADIO_RTSP_UDP)->ShowWindow(0);
		GetDlgItem(IDC_RADIO_RTSP_TCP)->ShowWindow(0);
		GetDlgItem(IDC_RADIO_RTSP_HTTP)->ShowWindow(0);
	}
	else
	{
		GetDlgItem(IDC_STATIC_RTSPOVER)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_RTSP_UDP)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_RTSP_TCP)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_RTSP_HTTP)->ShowWindow(1);
        if (vStreamInfo.csStreamProtocol.Find(_T("RTPTCP")) != -1)
		{
			((CButton*)GetDlgItem(IDC_RADIO_RTSP_UDP))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_RTSP_TCP))->SetCheck(1);
            ((CButton*)GetDlgItem(IDC_RADIO_RTSP_HTTP))->SetCheck(0);    
		}
		else if (vStreamInfo.csStreamProtocol.Find(_T("RTPHTTP")) != -1)
		{
			((CButton*)GetDlgItem(IDC_RADIO_RTSP_UDP))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_RTSP_TCP))->SetCheck(0);
            ((CButton*)GetDlgItem(IDC_RADIO_RTSP_HTTP))->SetCheck(1);    
		}
        else
		{
			((CButton*)GetDlgItem(IDC_RADIO_RTSP_UDP))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_RTSP_TCP))->SetCheck(0);
            ((CButton*)GetDlgItem(IDC_RADIO_RTSP_HTTP))->SetCheck(0);            
		}
	}

	CComboBox* pAVStream =(CComboBox*) GetDlgItem(IDC_COMBO_AV_STREAM);
	int iAVStream = pAVStream->GetCurSel();
	_tcscpy(m_veMoStreamInfo[iAVStream].tcProtocol, csCurSel);
	

}

void CDlgAddManageCam::OnBnClickedCheckNas()
{
	// TODO: Add your control notification handler code here
	if(((CButton*)GetDlgItem(IDC_CHECK_NAS))->GetCheck())
	{
		(CButton*)GetDlgItem(IDC_BTN_NAS)->EnableWindow(1);

		//--Select NAS Path
		CString csTopNAS;
		vector<ec_Storage> vcStorageObj;
		m_pBk->GetStorageByType(vcStorageObj, _T("NFS"));
		m_pBk->GetStorageByType(vcStorageObj, _T("Samba"));

		int nIdx= 0, nCount = vcStorageObj.size();
		for (nIdx=0 ; nIdx < nCount; nIdx++)
		{
			::_tcscpy(m_NASInfo.tcServerType, vcStorageObj[nIdx].storage_type.GetBuffer());
			::_tcscpy(m_NASInfo.tcServerAddr, vcStorageObj[nIdx].store_location.GetBuffer());
			::_tcscpy(m_NASInfo.tcServerUser, vcStorageObj[nIdx].server_user.GetBuffer());
			::_tcscpy(m_NASInfo.tcServerPass, vcStorageObj[nIdx].server_password.GetBuffer());
		}

		if( vcStorageObj.size() == 0 )
		{
			CDlgNAS m_CDlgNAS;
			if(m_CDlgNAS.DoModal() == IDOK)
			{
				m_NASInfo = m_CDlgNAS.m_NASInfo;
				this->SetDlgItemText(IDC_EDIT_NAS_PATH, m_NASInfo.tcServerAddr);
			}
		}
		else
		{
			this->SetDlgItemText(IDC_EDIT_NAS_PATH, m_NASInfo.tcServerAddr);
		}
	}
	else
	{
		(CButton*)GetDlgItem(IDC_BTN_NAS)->EnableWindow(0);
		this->SetDlgItemText(IDC_EDIT_NAS_PATH, _T(""));
		::ZeroMemory(&m_NASInfo, sizeof(NASInfo));
	}
}

void CDlgAddManageCam::OnBnClickedBtnNas()
{
	// TODO: Add your control notification handler code here
	CDlgNAS m_CDlgNAS;
	if(m_CDlgNAS.DoModal() == IDOK)
	{
		m_NASInfo = m_CDlgNAS.m_NASInfo;
		this->SetDlgItemText(IDC_EDIT_NAS_PATH, m_NASInfo.tcServerAddr);
	}

}

void CDlgAddManageCam::vGetNASInfoFromCamera()
{
	map<CString, sVERSETS>::iterator pos;
	map<CString, sSETS>::iterator pos2;

	mapNAS[_T("config/video.cgi")];
	mapNAS[_T("config/video.cgi?profile_id=1")];

	
	//--Get NAS Info From New Type
	for(pos = mapNAS.begin(); pos != mapNAS.end(); pos++)
	{
		CString csUrl, csBuffer;
		Curlplus CGIScanData;

		csUrl.Format(_T("http://%s:%s/%s"), vIPCamInfo.csIP, vIPCamInfo.csWebPort, pos->first);
		csBuffer = CGIScanData.get(csUrl, vIPCamInfo.csUser, vIPCamInfo.csPWD);
		
		if((csBuffer.Find(_T("Denied")) != -1) || (csBuffer.Find(_T("Error")) != -1))
		{
			//AfxMessageBox(_T("Access Denied"));
			TRACE(_T("Access Denied"));
			continue;
		}
		else if( csBuffer == _T("") )
		{
			//AfxMessageBox(_T("Network Error"));
			TRACE(_T("Network Error"));
			continue;
		}
		else if(csBuffer.Find(_T("401")) != -1 && csBuffer.Find(_T("Unauthorized")) != -1)
		{
			TRACE(_T("401:Unauthorized"));
			continue;
		}

			csBuffer.Remove(_T('{'));
			csBuffer.Remove(_T('"'));
			csBuffer.Remove(_T('}'));
			csBuffer.Remove(_T(' '));
		
		char *buffer = new char[csBuffer.GetLength() + 1];
		wcstombs_s(NULL, buffer, csBuffer.GetLength() + 1, csBuffer, _TRUNCATE);
	
		TCHAR tzBuffer[MAXBUF];
		MultiByteToWideChar(CP_ACP, 0, buffer, -1, tzBuffer, MAXBUF);
		vParseSegmentNew(tzBuffer, pos);
		delete buffer;
	}
	
}

bool CDlgAddManageCam::bCheckPathBuffer(CString csPath, int iBuffer)
{
	unsigned __int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;

	GetDiskFreeSpaceEx( (LPCTSTR)csPath, (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes );
	
	if(i64FreeBytesToCaller/(1024*1024*1024) > iBuffer)	return true;
	else	return false;
}

void CDlgAddManageCam::vGetPathBufferFromDB()
{
	mapPathBuffer.clear();

	vector<ec_Storage> vcStorage;
	m_pBk->GetStorageByType(vcStorage, _T("RECORD"));

	int nIdx = 0, ncCount = vcStorage.size();
	for (nIdx=0 ; nIdx < ncCount; nIdx++)
	{
		mapPathBuffer.insert(::pair<CString, int>(vcStorage[nIdx].store_location, ::_ttoi(vcStorage[nIdx].buffer_size.GetBuffer())));
	}
	return;
}
void CDlgAddManageCam::OnBnClickedRadioRtspHttp()
{
	// TODO: Add your control notification handler code here
	CComboBox* pAVStream =(CComboBox*) GetDlgItem(IDC_COMBO_AV_STREAM);
	int iCurSel = pAVStream->GetCurSel();

	CString csProtocol(_T("RTPHTTP"));
	_tcscpy(m_veMoStreamInfo[iCurSel].tcProtocol, csProtocol);
}
