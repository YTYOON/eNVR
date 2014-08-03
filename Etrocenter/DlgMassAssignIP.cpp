// DlgAssignIP.cpp : implementation file
//

#include "stdafx.h"
#include "DlgMassAssignIP.h"
#include "IPCamDiscovery.h"
#include "ICMP.h"
#include "INI.h"
//#include "DlgAddManageCam.h"
#include "./TextDesigner/OutlineText.h"

#pragma comment(lib,"Iphlpapi.lib") 

const int MAXBUF = 4096;
const short ELAPSE_TIME = 99;

// CDlgMassAssignIP dialog

#define  PROGRESS_FULLSCALE 100
static int nstatus=1;
const UINT  MsgUpdateListCtrl = RegisterWindowMessage(_T("MsgUpdateListCtrl") );
HANDLE hCheckFailIPAddress = NULL;


IMPLEMENT_DYNAMIC(CDlgMassAssignIP, CBaseDialog)

CDlgMassAssignIP::CDlgMassAssignIP(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgMassAssignIP::IDD, pParent),m_nStepCursor(0),
	m_hWaitIpCamWakeup(NULL), m_hDestory(NULL),m_bAssignIP(false),
	iSolveNode(0),iTotalSolveNode(0),m_bWriteDatabaseFinish(false),
	m_strID(_T("")),m_strPassword(_T("")),m_nProgress_FullScale(PROGRESS_FULLSCALE),
	m_nProgress_CurrentSection(0),m_nProgress_SectionCount(0),m_nTimerCountdown(ELAPSE_TIME),
	m_pBK(NULL)
{
	InitializeCriticalSection (&m_criSec);
	m_hDestory = CreateEvent(NULL, TRUE, FALSE, NULL);
	hAddIPCam2DB = NULL;
    AfxInitRichEdit2();
}

CDlgMassAssignIP::~CDlgMassAssignIP()
{
	DeleteCriticalSection(&m_criSec);
}

void CDlgMassAssignIP::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	//DDX_Control(pDX, IDC_ANIMATE, m_Animate);
    DDX_Control(pDX, IDC_RICHEDIT_GUIDE, m_richEdt);
    

}

BEGIN_MESSAGE_MAP(CDlgMassAssignIP, CBaseDialog)
	ON_BN_CLICKED(IDC_RADIO_STATICIP, &CDlgMassAssignIP::OnBnClickedRadioStaticip)
	ON_BN_CLICKED(IDC_RADIO_DHCP, &CDlgMassAssignIP::OnBnClickedRadioDhcp)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDlgMassAssignIP::OnNMClickList1)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_BTN_NEXT, &CDlgMassAssignIP::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_PREVIOUS, &CDlgMassAssignIP::OnBnClickedBtnPrevious)
	ON_BN_CLICKED(IDOK, &CDlgMassAssignIP::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_ASSIGN_IP, &CDlgMassAssignIP::OnBnClickedCheckAssignIp)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_NETCARD, &CDlgMassAssignIP::OnCbnSelchangeComboNetcard)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MASS_ASSIGN_RESCAN, &CDlgMassAssignIP::OnBnClickedButtonMassAssignRescan)
	ON_WM_CLOSE()
	ON_REGISTERED_MESSAGE(MsgUpdateListCtrl, UpdateListCtrl)
END_MESSAGE_MAP()
	

BEGIN_MESSAGE_MAP(CDlgMassAssignIP::CCheckHeadCtrl, CHeaderCtrl)
	ON_NOTIFY_REFLECT(HDN_ITEMCLICK, OnItemClicked)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CListCtrlColorRow, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW,OnDrawItemColor)
END_MESSAGE_MAP()


void CDlgMassAssignIP::vSetGuideStep(DWORD nStep)
{
    nStep = m_nStepCursor;
    TCHAR  tzSeparate[] = _T("  >>  ");
    const TCHAR* tzGuide1[]= {_T("Select Device"), _T("Add Device"), _T("Finish"),_T( "")};
    const TCHAR* tzGuide2[]= {_T("Select Device"), _T("Pickup IP"), _T("Reset Device"), _T("Add Device"), _T("Finish"), _T("")};

    const TCHAR** tzGuide = m_bAssignIP ? tzGuide2 : tzGuide1;
    CString csText;
    for (int ci =0; _tcslen(tzGuide[ci]) != 0; ++ci)
    {
        csText += tzGuide[ci];
        csText += tzSeparate;
    }
    csText.TrimRight(_T("> "));
    WORD nStart(0), nEnd(0), nTmp(0);
    for (int ci =0; ci <= nStep; ++ci)
    {
        nStart = nTmp;
        nEnd = nTmp +_tcslen(tzGuide[ci]);
        WORD   nSeparate = _tcslen(tzSeparate);
        nTmp = nEnd + nSeparate;
    }
    m_richEdt.SetWindowText(csText);

    CHARFORMAT cf;
    cf.cbSize = sizeof(CHARFORMAT);
    cf.dwMask = CFM_COLOR;
    m_richEdt.SetSel(0, -1);
    m_richEdt.GetSelectionCharFormat(cf); 
    if( cf.dwEffects & CFE_AUTOCOLOR )
    { 
        cf.dwEffects ^= CFE_AUTOCOLOR;
    }        
    m_richEdt.SetSelectionCharFormat(cf);
    cf.crTextColor = RGB(255, 255, 255);
    m_richEdt.SetSel(nStart, nEnd);
    cf.crTextColor = RGB(255,0,0);
    m_richEdt.SetSelectionCharFormat(cf);
    //enforce Redraw window
    Invalidate(true);
    UpdateWindow();
}
BOOL CDlgMassAssignIP::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	m_pBK = CNVRBk::GetBkObject();

	InitialControlText();
	InitialColumn();
	InitialControlStatus();
	InitialListBox();
    vSetGuideStep(0);
  
	 GetProgressCtrl()->SetRange(0, m_nProgress_FullScale);

	 KeepOldCoordinate();
	 MoveDlgItem(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMassAssignIP::KeepOldCoordinate()
{
	GetWindowRect(&m_oldCoordinate.recDlg);

	GetDlgItem(IDC_STATICGROUP)->GetWindowRect(&m_oldCoordinate.recGroup);
	ScreenToClient(&m_oldCoordinate.recGroup);

	GetDlgItem(IDC_CHECK_ASSIGN_IP)->GetWindowRect(&m_oldCoordinate.recChangeIP);
	ScreenToClient(&m_oldCoordinate.recChangeIP);

	GetDlgItem(IDC_RADIO_STATICIP)->GetWindowRect(&m_oldCoordinate.recStatic);
	ScreenToClient(&m_oldCoordinate.recStatic);

	GetDlgItem(IDC_RADIO_DHCP)->GetWindowRect(&m_oldCoordinate.recDHCP);
	ScreenToClient(&m_oldCoordinate.recDHCP);

	GetDlgItem(IDC_STATIC_NETWORK_CARD)->GetWindowRect(&m_oldCoordinate.recNetwork);
	ScreenToClient(&m_oldCoordinate.recNetwork);

	GetDlgItem(IDC_COMBO_NETCARD)->GetWindowRect(&m_oldCoordinate.recCombobox);
	ScreenToClient(&m_oldCoordinate.recCombobox);

	GetDlgItem(IDC_STATIC_MASS_ASSIGN_IP)->GetWindowRect(&m_oldCoordinate.recIPAdress);
	ScreenToClient(&m_oldCoordinate.recIPAdress);

	GetDlgItem(IDC_IPADDRESS_IP)->GetWindowRect(&m_oldCoordinate.recIPAdress_Enter);
	ScreenToClient(&m_oldCoordinate.recIPAdress_Enter);

	GetDlgItem(IDC_STATIC_MASS_ASSIGN_MASK)->GetWindowRect(&m_oldCoordinate.recIPMask);
	ScreenToClient(&m_oldCoordinate.recIPMask);

	GetDlgItem(IDC_IPADDRESS_MASK)->GetWindowRect(&m_oldCoordinate.recIPMask_Enter);
	ScreenToClient(&m_oldCoordinate.recIPMask_Enter);

	GetDlgItem(IDC_STATIC_MASS_ASSIGN_GATEWAY)->GetWindowRect(&m_oldCoordinate.recIPGetway);
	ScreenToClient(&m_oldCoordinate.recIPGetway);

	GetDlgItem(IDC_IPADDRESS_GATEWAY)->GetWindowRect(&m_oldCoordinate.recIPGetway_Enter);
	ScreenToClient(&m_oldCoordinate.recIPGetway_Enter);

	GetDlgItem(IDC_STATIC_MASS_ASSIGN_DNS)->GetWindowRect(&m_oldCoordinate.recIPDNS);
	ScreenToClient(&m_oldCoordinate.recIPDNS);

	GetDlgItem(IDC_IPADDRESS_DNS)->GetWindowRect(&m_oldCoordinate.recIPDNS_Enter);
	ScreenToClient(&m_oldCoordinate.recIPDNS_Enter);

	GetDlgItem(IDC_BUTTON_MASS_ASSIGN_RESCAN)->GetWindowRect(&m_oldCoordinate.recRescan);
	ScreenToClient(&m_oldCoordinate.recRescan);

	GetDlgItem(IDC_LIST1)->GetWindowRect(&m_oldCoordinate.recList);
	ScreenToClient(&m_oldCoordinate.recList);

	GetDlgItem(IDC_STATIC_PERCENT)->GetWindowRect(&m_oldCoordinate.recProgrss);
	ScreenToClient(&m_oldCoordinate.recProgrss);

	GetDlgItem(IDC_PROGRESS1)->GetWindowRect(&m_oldCoordinate.recProgressBar);
	ScreenToClient(&m_oldCoordinate.recProgressBar);

	GetDlgItem(IDC_BTN_PREVIOUS)->GetWindowRect(&m_oldCoordinate.recPrevious);
	ScreenToClient(&m_oldCoordinate.recPrevious);

	GetDlgItem(IDC_BTN_NEXT)->GetWindowRect(&m_oldCoordinate.recNext);
	ScreenToClient(&m_oldCoordinate.recNext);

	GetDlgItem(IDOK)->GetWindowRect(&m_oldCoordinate.recExit);
	ScreenToClient(&m_oldCoordinate.recExit);

	GetDlgItem(IDC_ANIMATE)->GetWindowRect(&m_oldCoordinate.recRun);
	ScreenToClient(&m_oldCoordinate.recRun);
}

void CDlgMassAssignIP::MoveDlgItem(BOOL bExpand)
{
	CRect rec, recTemp, recClient;
	if (!bExpand)
	{
		ShowChangeIPGroup(FALSE);
		
		CountRetangle(m_oldCoordinate.recRescan, m_oldCoordinate.recChangeIP, rec);
		GetDlgItem(IDC_BUTTON_MASS_ASSIGN_RESCAN)->MoveWindow(&rec);

		CountRetangle(m_oldCoordinate.recList, rec, rec);
		GetDlgItem(IDC_LIST1)->MoveWindow(&rec);
		recTemp = rec;
	
		CountRetangle(m_oldCoordinate.recProgrss, recTemp, rec);
		GetDlgItem(IDC_STATIC_PERCENT)->MoveWindow(&rec);
	
		CountRetangle(m_oldCoordinate.recProgressBar, recTemp, rec);
		GetDlgItem(IDC_PROGRESS1)->MoveWindow(&rec);
		recTemp = rec;
		
		CountRetangle(m_oldCoordinate.recRun, recTemp, rec);
		GetDlgItem(IDC_ANIMATE)->MoveWindow(&rec);

		CountRetangle(m_oldCoordinate.recPrevious, recTemp, rec);
		GetDlgItem(IDC_BTN_PREVIOUS)->MoveWindow(&rec);
		
		CountRetangle(m_oldCoordinate.recNext, recTemp, rec);
		GetDlgItem(IDC_BTN_NEXT)->MoveWindow(&rec);
		
		CountRetangle(m_oldCoordinate.recExit, recTemp, rec);
		GetDlgItem(IDOK)->MoveWindow(&rec);
		recTemp = rec;

		GetDlgWindowRect(rec);
 		rec.bottom = rec.bottom - (m_oldCoordinate.recRescan.top - m_oldCoordinate.recChangeIP.bottom);
 		MoveWindow(&rec);
	}
	else
	{
		ShowChangeIPGroup(TRUE);

		GetDlgItem(IDC_STATICGROUP)->MoveWindow(&m_oldCoordinate.recGroup);
		GetDlgItem(IDC_CHECK_ASSIGN_IP)->MoveWindow(&m_oldCoordinate.recChangeIP);
		GetDlgItem(IDC_RADIO_STATICIP)->MoveWindow(&m_oldCoordinate.recStatic);
		GetDlgItem(IDC_RADIO_DHCP)->MoveWindow(&m_oldCoordinate.recDHCP);
		GetDlgItem(IDC_STATIC_NETWORK_CARD)->MoveWindow(&m_oldCoordinate.recNetwork);
		GetDlgItem(IDC_COMBO_NETCARD)->MoveWindow(&m_oldCoordinate.recCombobox);
		GetDlgItem(IDC_STATIC_MASS_ASSIGN_IP)->MoveWindow(&m_oldCoordinate.recIPAdress);
		GetDlgItem(IDC_IPADDRESS_IP)->MoveWindow(&m_oldCoordinate.recIPAdress_Enter);
		GetDlgItem(IDC_STATIC_MASS_ASSIGN_MASK)->MoveWindow(&m_oldCoordinate.recIPMask);
		GetDlgItem(IDC_IPADDRESS_MASK)->MoveWindow(&m_oldCoordinate.recIPMask_Enter);
		GetDlgItem(IDC_STATIC_MASS_ASSIGN_GATEWAY)->MoveWindow(&m_oldCoordinate.recIPGetway);
		GetDlgItem(IDC_IPADDRESS_GATEWAY)->MoveWindow(&m_oldCoordinate.recIPGetway_Enter);
		GetDlgItem(IDC_STATIC_MASS_ASSIGN_DNS)->MoveWindow(&m_oldCoordinate.recIPDNS);
		GetDlgItem(IDC_IPADDRESS_DNS)->MoveWindow(&m_oldCoordinate.recIPDNS_Enter);
		GetDlgItem(IDC_BUTTON_MASS_ASSIGN_RESCAN)->MoveWindow(&m_oldCoordinate.recRescan);
		GetDlgItem(IDC_LIST1)->MoveWindow(&m_oldCoordinate.recList);
		GetDlgItem(IDC_STATIC_PERCENT)->MoveWindow(&m_oldCoordinate.recProgrss);
		GetDlgItem(IDC_PROGRESS1)->MoveWindow(&m_oldCoordinate.recProgressBar);
		GetDlgItem(IDC_ANIMATE)->MoveWindow(&m_oldCoordinate.recRun);
		GetDlgItem(IDC_BTN_PREVIOUS)->MoveWindow(&m_oldCoordinate.recPrevious);
		GetDlgItem(IDC_BTN_NEXT)->MoveWindow(&m_oldCoordinate.recNext);
		GetDlgItem(IDOK)->MoveWindow(&m_oldCoordinate.recExit);

		GetDlgWindowRect(rec);
		MoveWindow(&rec);
	}
    //vSetGuideStep(0);
    CenterWindow();
}

void CDlgMassAssignIP::GetDlgWindowRect(CRect& rec)
{
	GetWindowRect(&rec);
	rec.bottom = rec.top + m_oldCoordinate.recDlg.Height();
}

void CDlgMassAssignIP::ShowChangeIPGroup(BOOL bShow)
{
	GetDlgItem(IDC_STATICGROUP)->ShowWindow(bShow);
	GetDlgItem(IDC_RADIO_STATICIP)->ShowWindow(bShow);
	GetDlgItem(IDC_RADIO_DHCP)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_NETWORK_CARD)->ShowWindow(bShow);
	GetDlgItem(IDC_COMBO_NETCARD)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_MASS_ASSIGN_IP)->ShowWindow(bShow);
	GetDlgItem(IDC_IPADDRESS_IP)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_MASS_ASSIGN_MASK)->ShowWindow(bShow);
	GetDlgItem(IDC_IPADDRESS_MASK)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_MASS_ASSIGN_GATEWAY)->ShowWindow(bShow);
	GetDlgItem(IDC_IPADDRESS_GATEWAY)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_MASS_ASSIGN_DNS)->ShowWindow(bShow);
	GetDlgItem(IDC_IPADDRESS_DNS)->ShowWindow(bShow);
	GetDlgItem(IDC_ANIMATE)->ShowWindow(FALSE);
}

void CDlgMassAssignIP::CountRetangle(const CRect& recOriginal, const CRect& recMove, CRect& recResult)
{
	recResult.left = recOriginal.left;
	recResult.top = recMove.bottom + 5 ;
	recResult.right = recResult.left + recOriginal.Width();
	recResult.bottom = recResult.top + recOriginal.Height();
}

void CDlgMassAssignIP::OnBnClickedRadioStaticip()
{
	if (((CButton*)GetDlgItem(IDC_RADIO_STATICIP))->GetCheck())
	{
		GetDlgItem(IDC_IPADDRESS_IP)->EnableWindow(true);
		GetDlgItem(IDC_IPADDRESS_MASK)->EnableWindow(true);
		GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(true);
		GetDlgItem(IDC_IPADDRESS_DNS)->EnableWindow(true);
		WORD nInx = 0;
		WORD nPickupSize = m_PickUpIP.size();
		for (int ci = 0; ci < m_wndList.GetItemCount(); ci++)
		{
			if (CompareExistDevice(m_wndList.GetItemText(ci, 4)))
			{
				if (m_wndList.GetCheck(ci) && nInx < nPickupSize)
				{	
					m_wndList.SetItemText(ci, 5, m_PickUpIP[nInx]);
					nInx++;
				}
				else
				{
					m_wndList.SetItemText(ci, 5, _T(""));
				}
			}
		 }
	}	
}

void CDlgMassAssignIP::OnBnClickedRadioDhcp()
{
	if (((CButton*)GetDlgItem(IDC_RADIO_DHCP))->GetCheck())
	{
		GetDlgItem(IDC_IPADDRESS_IP)->EnableWindow(false);
		GetDlgItem(IDC_IPADDRESS_MASK)->EnableWindow(false);
		GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(false);
		GetDlgItem(IDC_IPADDRESS_DNS)->EnableWindow(false);
		for (int ci = 0; ci < m_wndList.GetItemCount() ; ci++)
		{
			if (CompareExistDevice(m_wndList.GetItemText(ci, 4)))
			{
				if (m_wndList.GetCheck(ci))
				{	
					m_wndList.SetItemText(ci, 5, _T("DHCP"));
				}
				else
				{
					m_wndList.SetItemText(ci, 5, _T(""));
				}
			}
		 }
	}
}

bool CDlgMassAssignIP::vGetNetWorkInfo(void)
{
	USES_CONVERSION;
	bool bRet = false;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	UINT i;

	/* variables used to print DHCP time info */
	struct tm newtime;
	char buffer[32];
	errno_t error;

	ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof (IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		TRACE(_T("Error allocating memory needed to call GetAdaptersinfo\n"));
		return 1;
	}
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			TRACE(_T("Error allocating memory needed to call GetAdaptersinfo\n"));
			return 1;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		while (pAdapter) {
			TRACE(_T("\tComboIndex: \t%d\n"), pAdapter->ComboIndex);
			TRACE(_T("\tAdapter Name: \t%s\n"), A2W(pAdapter->AdapterName));
			TRACE(_T("\tAdapter Desc: \t%s\n"), A2W(pAdapter->Description));
			TRACE(_T("\tAdapter Addr: \t"));
			for (i = 0; i < pAdapter->AddressLength; i++) {
				if (i == (pAdapter->AddressLength - 1))
					TRACE(_T("%.2X\n"), (int) pAdapter->Address[i]);
				else
					TRACE(_T("%.2X-"), (int) pAdapter->Address[i]);
			}
			TRACE(_T("\tIndex: \t%d\n"), pAdapter->Index);
			TRACE(_T("\tType: \t"));
			switch (pAdapter->Type) {
			case MIB_IF_TYPE_OTHER:
				TRACE(_T("Other\n"));
				break;
			case MIB_IF_TYPE_ETHERNET:
				TRACE(_T("Ethernet\n"));
				break;
			case MIB_IF_TYPE_TOKENRING:
				TRACE(_T("Token Ring\n"));
				break;
			case MIB_IF_TYPE_FDDI:
				TRACE(_T("FDDI\n"));
				break;
			case MIB_IF_TYPE_PPP:
				TRACE(_T("PPP\n"));
				break;
			case MIB_IF_TYPE_LOOPBACK:
				TRACE(_T("Lookback\n"));
				break;
			case MIB_IF_TYPE_SLIP:
				TRACE(_T("Slip\n"));
				break;
			default:
				TRACE(_T("Unknown type %ld\n"), pAdapter->Type);
				break;
			}

			TRACE(_T("\tIP Address: \t%s\n"),A2W(pAdapter->IpAddressList.IpAddress.String));
			GetDlgItem(IDC_IPADDRESS_IP)->SetWindowText(A2W(pAdapter->IpAddressList.IpAddress.String));

			TRACE(_T("\tIP Mask: \t%s\n"), A2W(pAdapter->IpAddressList.IpMask.String));
			GetDlgItem(IDC_IPADDRESS_MASK)->SetWindowText(A2W(pAdapter->IpAddressList.IpMask.String));

			TRACE(_T("\tGateway: \t%s\n"), A2W(pAdapter->GatewayList.IpAddress.String));
			if (GetDlgItem(IDC_IPADDRESS_GATEWAY))
				GetDlgItem(IDC_IPADDRESS_GATEWAY)->SetWindowText(A2W(pAdapter->GatewayList.IpAddress.String));
			TRACE(_T("\t***\n"));

			//Get DNS-IP
			DWORD dwErr;
			ULONG nSize = 0;
			PIP_PER_ADAPTER_INFO pPerAdapterInfo = NULL;
			if( (dwErr = GetPerAdapterInfo(pAdapter->Index, pPerAdapterInfo, &nSize)) == ERROR_BUFFER_OVERFLOW )
			{
				pPerAdapterInfo = (IP_PER_ADAPTER_INFO *) malloc(nSize);
				if ( (dwErr = GetPerAdapterInfo(pAdapter->Index, pPerAdapterInfo, &nSize)) == ERROR_SUCCESS)
				{
					if (pPerAdapterInfo == NULL) {
						TRACE(_T("Error allocating memory needed to call GetAdaptersinfo\n"));
						return 1;
					}					
					GetDlgItem(IDC_IPADDRESS_DNS)->SetWindowText(A2W(pPerAdapterInfo->DnsServerList.IpAddress.String));
				}
			}
			if (pPerAdapterInfo)
			{
				free( pPerAdapterInfo );
			}

			//Get DHCP
			if (pAdapter->DhcpEnabled) {
				TRACE(_T("\tDHCP Enabled: Yes\n"));
				TRACE(_T("\t  DHCP Server: \t%s\n"), A2W(pAdapter->DhcpServer.IpAddress.String));

				TRACE(_T("\t  Lease Obtained: )"));
				// Display local time 
				error = _localtime32_s(&newtime, (__time32_t*) &pAdapter->LeaseObtained);
				if (error)
					TRACE(_T("Invalid Argument to _localtime32_s\n"));
				else {
					// Convert to an ASCII representation 
					error = asctime_s(buffer, 32, &newtime);
					if (error)
						TRACE(_T("Invalid Argument to asctime_s\n"));
					else
						// asctime_s returns the string terminated by \n\0 
						TRACE(_T("%s"), buffer);
				}

				TRACE(_T("\t  Lease Expires:  "));
				error = _localtime32_s(&newtime, (__time32_t*) &pAdapter->LeaseExpires);
				if (error)
					TRACE(_T("Invalid Argument to _localtime32_s\n"));
				else {
					// Convert to an ASCII representation 
					error = asctime_s(buffer, 32, &newtime);
					if (error)
						TRACE(_T("Invalid Argument to asctime_s\n"));
					else
						// asctime_s returns the string terminated by \n\0 
						TRACE(_T("%s"), buffer);
				}
			} else
				TRACE(_T("\tDHCP Enabled: No\n"));

			if (pAdapter->HaveWins) {
				TRACE(_T("\tHave Wins: Yes\n"));
				TRACE(_T("\t  Primary Wins Server:    %s\n"), A2W(pAdapter->PrimaryWinsServer.IpAddress.String));
				TRACE(_T("\t  Secondary Wins Server:  %s\n"), A2W(pAdapter->SecondaryWinsServer.IpAddress.String));
			} else
				TRACE(_T("\tHave Wins: No\n"));

			pAdapter = pAdapter->Next;
			TRACE(_T("\n"));

		}
	} else {
		TRACE(_T("GetAdaptersInfo failed with error: %d\n"), dwRetVal);

	}

	//pAdapter->Next;

	if (pAdapterInfo)
		free(pAdapterInfo);
	bRet = true;
	return bRet;
}



void CDlgMassAssignIP::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
/*
	USES_CONVERSION;
	int err;
	int ci;
	CString temp1 = "";
	CString temp2 = "";
	switch( page )
	{
	case IDD_SETTING_PAGE_NETWORK:
		{
			((CSettingPageNetwork*)m_dlgPage[0])->GetSettingValue(&m_netInfo);
			char szSerial[16]={0};
			strncpy(szSerial, m_netInfo.STATICIP.ip,15);
			char szTok[] =".";
			char* szIp[4];
			szIp[0] = strtok(szSerial, szTok);
			szIp[1] = strtok(NULL, szTok);
			szIp[2] = strtok(NULL, szTok);
			szIp[3] = strtok(NULL, szTok);
			for (ci =0; ci < m_pvthENCPItem->size(); ci++)
			{
				m_hENCPItem = (*m_pvthENCPItem)[ci];
				int nSerial = atoi(szIp[3])+ci;
				sprintf(m_netInfo.STATICIP.ip, "%s.%s.%s.%d", szIp[0], szIp[1], szIp[2], nSerial);
				if(int nRet = encp_SetNetworkInfo(m_hENCPItem, (LPCSTR)W2A(m_strAdminID), (LPCSTR)W2A(m_strAdminPass), &m_netInfo) != ENERR_OK )
				{
					return;
				}
			}
		}
		break;
	case IDD_SETTING_PAGE_HOSTNAME:
		((CSettingPageHostname*)m_dlgPage[1])->GetSettingValue(temp1, temp2);
		for (ci =0; ci < m_pvthENCPItem->size(); ci++)
		{
			m_hENCPItem = (*m_pvthENCPItem)[ci];
			err = encp_SetHostName(m_hENCPItem, W2A(m_strAdminID), W2A(m_strAdminPass), W2A(temp1), W2A(temp2));
			if( err != ENERR_OK )
			{
				// Error Handling here...
				CString msg;
				msg.Format(_T("Error %d"),err);
				ETROMSG(msg);
				return;
			}		
		}
		break;
	case IDD_SETTING_PAGE_ACCOUNT:
		((CSettingPageAccount*)m_dlgPage[2])->GetSettingValue(temp1, m_bGuestAllow );
		for (ci =0; ci < m_pvthENCPItem->size(); ci++)
		{
			m_hENCPItem = (*m_pvthENCPItem)[ci];
			if( ::encp_SetAccountInfo(m_hENCPItem, W2A(m_strAdminID), W2A(m_strAdminPass), W2A(temp1), &m_bGuestAllow) != ENERR_OK )
			{
				// Error Handling here...
				return;
			}
		}
		break;
	}
*/
}

bool CDlgMassAssignIP::bGetNextSuitableIP(CString& csIP)
{ 
	TCHAR* tz = csIP.GetBuffer() +csIP.ReverseFind(_T('.'))+1;
	WORD nSuffixIP = _ttol(tz);
	if (nSuffixIP > 0 && ++nSuffixIP < 255)
	{
		csIP.Format(_T("%s.%d"),csIP.Left(csIP.ReverseFind(_T('.'))), nSuffixIP);
		return true;
	}
	else
		return false;
}
struct PINGIP_PARAM
{
	CDlgMassAssignIP* pThis;
	char szIP[16];
};


UINT CDlgMassAssignIP::fnDetectAvailableIP(LPVOID lpParam)
{
	AfxGetApp()->BeginWaitCursor();
	CDlgMassAssignIP* pThis = (CDlgMassAssignIP*) lpParam;
	pThis->EnableWindow(false);

	CString csIP, csPrefix;
	pThis->m_PickUpIP.clear();
	pThis->GetDlgItemText(IDC_IPADDRESS_IP, csPrefix);
	USES_CONVERSION;
	TCHAR tzSpe[] = _T(".");
	TCHAR* tzTok;
	tzTok = _tcstok(csPrefix.GetBuffer(), tzSpe);
	pThis->m_btFinalIP[0] = pThis->m_btStartIP[0] = _ttol(tzTok);
	tzTok = _tcstok(NULL, tzSpe);
	pThis->m_btFinalIP[1] = pThis->m_btStartIP[1] = _ttol(tzTok);
	tzTok = _tcstok(NULL, tzSpe);
	pThis->m_btFinalIP[2] = pThis->m_btStartIP[2] = _ttol(tzTok);
	tzTok = _tcstok(NULL, tzSpe);
	pThis->m_btStartIP[3] = _ttol(tzTok);	
	csIP.Format(_T("%d.%d.%d.%d"), pThis->m_btStartIP[0], pThis->m_btStartIP[1], pThis->m_btStartIP[2], pThis->m_btStartIP[3]);

	WORD nTotalIP = pThis->iGetListCheckedCount();
	pThis->RestartProgress(nTotalIP);

	pThis->m_btFinalIP[3] = pThis->m_btStartIP[3] + nTotalIP;

	HANDLE* hThreads = new HANDLE[nTotalIP];
	PINGIP_PARAM* params = new PINGIP_PARAM[nTotalIP];

	for (int ci = 0; ci < nTotalIP; ci++)
	{
		params[ci].pThis = pThis;
		sprintf(params[ci].szIP, "%d.%d.%d.%d", pThis->m_btStartIP[0], pThis->m_btStartIP[1], pThis->m_btStartIP[2], pThis->m_btStartIP[3]+ci);
		UINT nRet(0);
		hThreads[ci] = (HANDLE)_beginthreadex(NULL, 0, fnPingIP, &params[ci], 0, &nRet);
	}

	DWORD nRet;
	nRet = WaitForMultipleObjects(nTotalIP, hThreads, true, INFINITE);
	int nPickupSize = pThis->m_PickUpIP.size();
	int nInx = 0;
	for (int ci = 0; ci < pThis->m_wndList.GetItemCount(); ci++)
	{
		if (pThis->CompareExistDevice(pThis->m_wndList.GetItemText(ci, 4)))
		{
			if (pThis->m_wndList.GetCheck(ci) && nInx < nPickupSize)
			{	
				pThis->m_wndList.SetItemText(ci, 5, pThis->m_PickUpIP[nInx]);
				nInx++;
			}
			else
			{
				pThis->m_wndList.SetItemText(ci, 5, _T(""));
			}
		}
	 }
	delete [] hThreads;
	delete [] params;

	pThis->RedrawPorgress();
	pThis->GetDlgItem(IDC_BTN_NEXT)->EnableWindow(true);
	pThis->GetDlgItem(IDC_BTN_PREVIOUS)->EnableWindow(true);
	pThis->EnableWindow(true);
	AfxGetApp()->EndWaitCursor();
	return 0;
}
UINT CDlgMassAssignIP::fnPingIP(LPVOID lpParam)
{
	CICMP icmp;	
	CDlgMassAssignIP* pThis = ((PINGIP_PARAM*) lpParam)->pThis;
	char* szIP = ((PINGIP_PARAM*) lpParam)->szIP;
	USES_CONVERSION;
	bool bExistIP = false;
	CString cs;

	do
	{
		icmp.SetConfigure(szIP);
		if (false == (bExistIP = icmp.bPing()))
		{
			CString cs = A2W(szIP);
			EnterCriticalSection(&pThis->m_criSec);
			pThis->m_PickUpIP.push_back(cs);
			LeaveCriticalSection(&pThis->m_criSec);
		}
	}while (  bExistIP && pThis->bGetNextAvailableIP(szIP) );

	//pThis->m_Animate.Close();
	pThis->GetDlgItem(IDC_ANIMATE)->ShowWindow(FALSE);

	pThis->AddProgress_CurrentSection();
	pThis->RedrawPorgress();

	return 0;
}
void CDlgMassAssignIP::OnBnClickedBtnDetectAvailableIp()
{
	if ( ((CButton*)GetDlgItem(IDC_RADIO_STATICIP))->GetCheck() == BST_CHECKED)
	{
		GetDlgItem(IDC_ANIMATE)->ShowWindow(TRUE);
		//m_Animate.Open(IDR_RUN);
		//m_Animate.Play(0,-1,-1);
		AfxBeginThread(fnDetectAvailableIP, ((LPVOID)this));
	}
	else
	{
		OnBnClickedRadioDhcp();
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(true);
		GetDlgItem(IDC_BTN_PREVIOUS)->EnableWindow(true);
		EnableWindow(true);
		AfxGetApp()->EndWaitCursor();
	}
}

bool CDlgMassAssignIP::bGetNextAvailableIP(char* szIP)
{
	bool bRet = false;
	InterlockedExchangeAdd(&m_btFinalIP[3], 1);
	if (m_btFinalIP[3] < 254)
	{
		sprintf(szIP, "%d.%d.%d.%d", m_btFinalIP[0], m_btFinalIP[1], m_btFinalIP[2], m_btFinalIP[3]);
		bRet = true;
	}
	return bRet;
}

BOOL CDlgMassAssignIP::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return CBaseDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CDlgMassAssignIP::OnBnClickedBtnMassSet()
{
	UpdateData(true);
	USES_CONVERSION;

	vector <NODEITEM*> Vt = (CIPCamDiscovery::GetNodePtr());
	TCHAR wszTmp[MAX_IP_LENGTH];
	ENCPNETWORK netInfo ={0};
	int nWaitCount = 0;

	GetDlgItemText(IDC_IPADDRESS_MASK, wszTmp, MAX_IP_LENGTH );
	strncpy(netInfo.STATICIP.netmask, W2A(wszTmp), sizeof(netInfo.STATICIP.netmask)); 
	GetDlgItemText(IDC_IPADDRESS_GATEWAY, wszTmp , MAX_IP_LENGTH );
	strncpy(netInfo.STATICIP.gateway, W2A(wszTmp), sizeof(netInfo.STATICIP.gateway)); 
	GetDlgItemText(IDC_IPADDRESS_DNS, wszTmp , MAX_IP_LENGTH );
	strncpy(netInfo.STATICIP.nameserver, W2A(wszTmp), sizeof(netInfo.STATICIP.nameserver)); 

	bool bStaticIP = true;
	if ( ((CButton*)GetDlgItem(IDC_RADIO_STATICIP))->GetCheck() == BST_CHECKED)
	{
		bStaticIP = true;
		netInfo.nettype = EN_STATICIP;
	}else
	{
		bStaticIP = false;
		netInfo.nettype = EN_DHCP;
	}

	RestartProgress(iGetListCheckedCount());
	for (int ci = 0; ci < m_wndList.GetItemCount(); ci++)
	{
		if (m_wndList.GetCheck(ci) && CompareExistDevice(m_wndList.GetItemText(ci, 4)))
		{	
			if (bStaticIP)
			{
				CString cs = m_wndList.GetItemText(ci, 5);
				USES_CONVERSION;
				TCHAR tzSpe[] = _T(".");
				TCHAR* tzTok;
				tzTok = _tcstok(cs.GetBuffer(), tzSpe);
				BYTE nIP[4];
				nIP[0] = _ttol(tzTok);
				tzTok = _tcstok(NULL, tzSpe);
				nIP[1] = _ttol(tzTok);
				tzTok = _tcstok(NULL, tzSpe);
				nIP[2] = _ttol(tzTok);
				tzTok = _tcstok(NULL, tzSpe);
				nIP[3] = _ttol(tzTok);

				sprintf(netInfo.STATICIP.ip, "%d.%d.%d.%d", nIP[0], nIP[1], nIP[2], nIP[3]);
			}		

			CStringA strA = W2A(m_strID);
			strlen(Vt[ci]->szUser) ? NULL : strcpy(Vt[ci]->szUser, strA);

			strA = W2A(m_strPassword);
 			strlen(Vt[ci]->szPass) ? NULL : strcpy(Vt[ci]->szPass, strA);

			int nRet = encp_SetNetworkInfo(Vt[ci]->hEncp, Vt[ci]->szUser, Vt[ci]->szPass, &netInfo);
			AddProgress_CurrentSection();
			RedrawPorgress();

			if (nRet == ENERR_OK)
			{
				nWaitCount++;
				m_wndList.SetItemText(ci, 6, _T("Waiting for IPCAM Reboot"));
				NODEITEM* pNodeitem =(NODEITEM*) Vt[ci];		
				pNodeitem->nStatus = RESETTING;
			}
			else
			{	 
				m_wndList.SetItemText(ci, 6, _T("Assign IP Fail"));
			}
		}
	 }
	UINT ret(0);
	RestartProgress(nWaitCount);
	RedrawPorgress();

	GetDlgItem(IDC_ANIMATE)->ShowWindow(TRUE);
	//if (m_Animate.Open(IDR_RUN))
	//{
	//	m_Animate.Play(0, (UINT)-1, (UINT)-1);
	//}
	m_hWaitIpCamWakeup =  (HANDLE)_beginthreadex( NULL, 0, fnWaitIpCamWakeUp, (LPVOID)this, 0, (UINT*)&ret );

}
UINT CDlgMassAssignIP::fnWaitIpCamWakeUp(LPVOID lpParam)
{
	CDlgMassAssignIP* pThis = (CDlgMassAssignIP*)lpParam;
	HANDLE hNewIPCamEvent = CreateEvent(NULL, TRUE, FALSE, NEW_IPCAM_AVAILABLE);
	vector <NODEITEM*> Vt = (CIPCamDiscovery::GetNodePtr());
	WORD nPollingCount = 0;
	int m_nTimer = 0;
	BOOL	bClose = FALSE;
    pThis->m_nTimerCountdown = ELAPSE_TIME;
    pThis->SetTimer(1, 1000, NULL);
	for(int iTemp = 0; iTemp < pThis->m_wndList.GetItemCount(); iTemp++)
	{
		if(pThis->m_wndList.GetCheck(iTemp) && pThis->CompareExistDevice(pThis->m_wndList.GetItemText(iTemp, 4)))
		{
			pThis->iTotalSolveNode++;
		}
	}
	
	DWORD nRetryTime(0);
	do
	{
		HANDLE hObjects[] = {hNewIPCamEvent, pThis->m_hDestory};
		int nDebug = sizeof(hObjects)/sizeof(HANDLE);
		DWORD nRet = ::WaitForMultipleObjects(sizeof(hObjects)/sizeof(HANDLE), hObjects, false, 10000);
		if (nRet == WAIT_OBJECT_0 || nRet == WAIT_TIMEOUT)
		{
			if (nRet == WAIT_TIMEOUT)
			{
				++nRetryTime;
				if (nRetryTime > 2)// ¤j©ó2¤ÀÄÁ broadcast again
				{
					HANDLE hScanRequire = CreateEvent(NULL, FALSE, TRUE, SCAN_REQUIRE);
					::SetEvent(hScanRequire);
					::CloseHandle(hScanRequire);
				}
			}
			nPollingCount = 0;
			for (int ci = 0; ci < pThis->m_wndList.GetItemCount(); ci++)
			{
				if (pThis->m_wndList.GetItemText(ci, 6) == _T("Waiting for IPCAM Reboot"))
				{
					++nPollingCount;
					for (int cj = 0; cj < Vt.size(); cj++)
					{
						CString csMac;
						pThis->vGetMacAddress( Vt[cj], csMac);
						
						if (csMac == pThis->m_wndList.GetItemText(ci, 4) && Vt[cj]->nStatus == LIVE)
						{
							if (pThis->GetCGI(Vt[cj], cj))
							{
								if (!bClose)
								{
									//pThis->m_Animate.Close();
									pThis->GetDlgItem(IDC_ANIMATE)->ShowWindow(FALSE);
								}
								--nPollingCount;
								pThis->m_wndList.SetItemText(ci, 6, _T("Insert To DB OK"));
								pThis->AddProgress_CurrentSection();
								pThis->RedrawPorgress();
							}
							else
							{
								if (!bClose)
								{
									//pThis->m_Animate.Close();
									pThis->GetDlgItem(IDC_ANIMATE)->ShowWindow(FALSE);
								}
								pThis->m_wndList.SetItemText(ci, 6, _T("Insert To DB Fail."));
								pThis->AddProgress_CurrentSection();
								pThis->RedrawPorgress();
							}
						}
					}
				}
			}
		}
		else
		{
			pThis->m_nStepCursor = ADDTOTABLE;
            pThis->vSetGuideStep(pThis->m_nStepCursor);
            break;
			//return 0;
		}
	}while (nPollingCount != 0);
	
	if (nPollingCount == 0)
	{
		HANDLE hThread;
		hThread = (HANDLE)_beginthreadex( NULL, 0, vSaveCGIData2DB, (LPVOID)pThis, 0, NULL );
		WaitForSingleObject (hThread, INFINITE);

		CListBox* pList = (CListBox*) pThis->GetDlgItem(IDC_LIST_STATUS);
		pList->SetCurSel(3);
		pThis->GetDlgItem(IDC_BTN_NEXT)->EnableWindow(true);
		pThis->m_nStepCursor = ADDTOTABLE;
        pThis->vSetGuideStep(pThis->m_nStepCursor);

	}
    CGS::SetTransparent(pThis->m_hWnd, 255);
    pThis->KillTimer(1);
    pThis->Invalidate(true);
    pThis->UpdateWindow();
    //pThis->RedrawWindow();
	return 0;
}
void CDlgMassAssignIP::vSetListCtrlCheckBoxHeader(void)
{
	CHeaderCtrl* pHeadCtrl = m_wndList.GetHeaderCtrl();
	ASSERT(pHeadCtrl->GetSafeHwnd());

	VERIFY( m_checkHeadCtrl.SubclassWindow(pHeadCtrl->GetSafeHwnd()) );
	VERIFY( m_checkImgList.Create(IDB_CHECKBOXES, 16, 3, RGB(255,0,255)) );
	int i = m_checkImgList.GetImageCount();
	m_checkHeadCtrl.SetImageList(&m_checkImgList);
	
	HDITEM hdItem;
	hdItem.mask = HDI_IMAGE | HDI_FORMAT;
	VERIFY( m_checkHeadCtrl.GetItem(0, &hdItem) );
	hdItem.iImage = 2;
	hdItem.fmt |= HDF_IMAGE;
	
	VERIFY( m_checkHeadCtrl.SetItem(0, &hdItem) );

}

LRESULT CDlgMassAssignIP::UpdateListCtrl(WPARAM wParam,LPARAM lParam)
{
	THREAD_INFO *pMessage=(THREAD_INFO*)lParam;

	if(pMessage->bResult)
	{
		m_wndList.SetItemText(pMessage->nNodeIdx, 6, _T("Insert To DB OK"));
		m_wndList.SetItemData(pMessage->nNodeIdx, RGB(0,0,0));
		vUpdatevcFailIPAddress(Modify_Remove, pMessage);
	}
	else
	{
		m_wndList.SetItemText(pMessage->nNodeIdx, 6, _T("Insert To DB Fail"));
		m_wndList.SetItemData(pMessage->nNodeIdx, RGB(255,0,0));
		vUpdatevcFailIPAddress(Modify_Insert, pMessage);
	}

	return 0;
}

void CDlgMassAssignIP::vUpdatevcFailIPAddress(ModifyFailIPArray ModifyType,  THREAD_INFO *pThreadinfo)
{
	vector<THREAD_INFO>::iterator it = m_vcFailIPAddress.begin();
	CString strOld, strNew;
	BOOL bFind = FALSE;

	vGetMacAddress( pThreadinfo->pNodeItem, strNew);

	while(it != m_vcFailIPAddress.end())
	{
		vGetMacAddress( it->pNodeItem, strOld);
		if(strOld.Compare(strNew) == 0)
		{
			bFind = TRUE;
			if (ModifyType == Modify_Remove)
			{
				//if (TryEnterCriticalSection(&m_criSec) != 0)
				{
					m_vcFailIPAddress.erase(it);
					//LeaveCriticalSection(&m_criSec);
				}
				break;
			} 
		}

		if(it != m_vcFailIPAddress.end())
			it++;
	}

	if (!bFind && ModifyType == Modify_Insert)
	{
		if (TryEnterCriticalSection(&m_criSec) != 0)
		{
			m_vcFailIPAddress.push_back(*pThreadinfo);
			LeaveCriticalSection(&m_criSec);
		}
	}
}

UINT CDlgMassAssignIP::ConnectOneIPCam(LPVOID lpParam)
{
	USES_CONVERSION;
	THREAD_INFO Thread = *(THREAD_INFO*)lpParam;

	if (Thread.pMassAssignIP && !Thread.bFinish)
	{
		if(Thread.pMassAssignIP->GetCGI(Thread.pNodeItem, Thread.nThreadIdx))
		{
			Thread.bFinish = true;
			Thread.bResult = true;
			Thread.pMassAssignIP->vSetMultiThreadInfo(Thread);
			
			Thread.pMassAssignIP->AddProgress_CurrentSection();
			Thread.pMassAssignIP->RedrawPorgress();
			::SendMessage(Thread.pMassAssignIP->m_hWnd,MsgUpdateListCtrl,0, (LPARAM)&Thread);
		}
		else
		{
			Thread.bFinish = true;
			Thread.bResult = false;

			Thread.pMassAssignIP->vSetMultiThreadInfo(Thread);
			::SendMessage(Thread.pMassAssignIP->m_hWnd,MsgUpdateListCtrl,0, (LPARAM)&Thread);
		}
	}

	return 0;
}

bool CDlgMassAssignIP::CompareExistDevice(const CString& csMac)
{
	CString str = csMac;
	bool bResult = true; 
	int nIdx = 0, nCount = m_vcExistDevice.size();
	str.Remove(_T('-'));
	for (nIdx = 0 ; nIdx < nCount ; nIdx++)
	{
		if (m_vcExistDevice[nIdx].csMac == str)
		{
			bResult = false;
			break;
		}
	}
	return bResult;
}

void CDlgMassAssignIP::vInsertThreadInfo2Array()
{
	CString csMac;
	int nThreadIdx = 0;
	vector <NODEITEM*> Vt = (CIPCamDiscovery::GetNodePtr());
	int nCount = m_wndList.GetItemCount();
	for (int ci = 0; ci < nCount; ci++)
	{
		if (m_wndList.GetCheck(ci) && CompareExistDevice(m_wndList.GetItemText(ci, 4)))
		{
			int nPollingCount = Vt.size();
			for (int cj = 0; cj < nPollingCount; cj++)
			{
				vGetMacAddress(Vt[cj], csMac);
				if (csMac == m_wndList.GetItemText(ci, 4) && Vt[cj]->nStatus == LIVE)
				{
					THREAD_INFO strThreadInfo;
					strThreadInfo.nNodeIdx = ci;
					strThreadInfo.pNodeItem = Vt[cj];
					strThreadInfo.pMassAssignIP = this;
					strThreadInfo.nThreadIdx = nThreadIdx++;
					m_vcMultiThreadInfo.push_back(strThreadInfo);

					map<CString, CJason> mapjason;
					m_vcJason.push_back(mapjason);
				}
			}
		}
	}
}

UINT CDlgMassAssignIP::ConnectThread(LPVOID lpParam)
{
	CDlgMassAssignIP* pThis = (CDlgMassAssignIP*) lpParam;
	vector<HANDLE>	vcMultiThreadHand;
	hCheckFailIPAddress = CreateEvent(NULL, false, false, _T("CheckFailIPAddress"));
	
	pThis->RestartProgress( pThis->iGetListCheckedCount() );
	
	while(!pThis->m_bWriteDatabaseFinish)
	{
		ResetEvent(hCheckFailIPAddress);
		vcMultiThreadHand.clear();
		pThis->vCreateMultiThread(vcMultiThreadHand);
		pThis->vWaitMultithreadNSaveData(vcMultiThreadHand);
		WaitForSingleObject (hCheckFailIPAddress, INFINITE);
		pThis->vCheckFailIPAddress();
	}
	
	::CloseHandle(hCheckFailIPAddress);

	pThis->GetDlgItem(IDC_BTN_NEXT)->EnableWindow(true);
	pThis->SetGlobalEvent();
	return 0;
}

void CDlgMassAssignIP::SetGlobalEvent()
{
	HANDLE hCameraRecording;
	CSA csa;

	//--Insert DB Complete
	hAddIPCam2DB = NULL;
	HANDLE hINSERTDB = CreateEvent(NULL, false, false, _T("INSERT_DB_COMPLETE"));
	SetEvent(hINSERTDB);
	CloseHandle(hINSERTDB);

	GetNVRBk()->NotifyEventSvr();
	GetNVRBk()->NotifyRecordSvr();
	GetNVRBk()->NotifyHttpSvr();

	//--Send Event to Refresh Event Server
	//CSA csa;
	HANDLE hEventAction = CreateEvent(&csa.m_sa, false, false, ETROCENTER_ACTION_REFRESH);
	if (hEventAction == NULL)
	{
		TRACE(_T("Error code :%d"), ::GetLastError());
	}
	SetEvent( hEventAction );
	CloseHandle(hEventAction);
}

void CDlgMassAssignIP::vCreateMultiThread(vector<HANDLE>& vcMultiThreadHand)
{
	THREAD_INFO strThreadInfo;
	vector<THREAD_INFO> vcMultiThreadInfo;
	HANDLE hThread;

	vcMultiThreadInfo = m_vcMultiThreadInfo;
	int nCount = vcMultiThreadInfo.size();

	for (int nIdx = 0; nIdx < nCount; nIdx++)
	{
		strThreadInfo = vcMultiThreadInfo[nIdx];
		hThread = (HANDLE)_beginthreadex( NULL, 0, ConnectOneIPCam, (void*)&strThreadInfo, 0, NULL );
		vcMultiThreadHand.push_back(hThread);
		::Sleep(100);
	}
}

void CDlgMassAssignIP::vWaitMultithreadNSaveData(vector<HANDLE>& vcMultiThreadHand)
{
	USES_CONVERSION;
	int nIdx = 0;
	int numThreads = vcMultiThreadHand.size();
	HANDLE *threads = new HANDLE[numThreads];
	for (nIdx = 0; nIdx < numThreads; nIdx++)
	{
		threads[nIdx] = vcMultiThreadHand[nIdx];
	}
	WaitForMultipleObjects(vcMultiThreadHand.size(), threads, TRUE, numThreads*4000);

	for (nIdx = 0; nIdx < numThreads; nIdx++)
	{
		TerminateThread(vcMultiThreadHand[nIdx], 0);
	}
	delete[] threads;

	THREAD_INFO threadinfo;
	for (nIdx = 0; nIdx < numThreads; nIdx++)
	{
		vGetMultiThreadInfo(threadinfo, nIdx);
		if (threadinfo.bFinish == false)
		{
			threadinfo.bFinish = true;
			vSetMultiThreadInfo(threadinfo);
			::SendMessage(this->m_hWnd,MsgUpdateListCtrl,0, (LPARAM)&threadinfo);
		}
	}

	HANDLE hThread;
	hThread = (HANDLE)_beginthreadex( NULL, 0, vSaveCGIData2DB, (LPVOID)this, 0, NULL );
	WaitForSingleObject (hThread, INFINITE);
	SetEvent(hCheckFailIPAddress);
}

bool CDlgMassAssignIP::GetCGI(NODEITEM* pNode,  int nJasonMapIdx)
{
	USES_CONVERSION;
	bool bRetryConnect;
	bool bRet = false;
	CString strWebPort;
	strWebPort.Format(_T("%d"),pNode->nWebPort);

	MODELTYPE mod_type = iGetIPCamType(A2W(pNode->ip), strWebPort);

	pNode->byModelType = mod_type;
	if(mod_type == MODEL_1150)
	{
		if(!bGetOldCGIData(A2W(pNode->ip), strWebPort, nJasonMapIdx))
		{
			bRet = false;
		}
		else
		{
			bRet = true;
		}
	}	
	else if(mod_type == MODEL_1180)
	{
		if(!bGetNewCGIData(A2W(pNode->ip), strWebPort, nJasonMapIdx))
		{
			bRet = false;
		}
		else
		{
			bRet = true;
		}
	}

	if (bRet == true)
	{
		vSetSuccessNode(pNode, nJasonMapIdx, mod_type);
	}

	return bRet;
}

void CDlgMassAssignIP::GetStreamProtocol(int nStreamIdx, map<CString,CJason>& mapJason, int iIPCamType, CString& stream_protocol)
{
	if (iIPCamType == MODEL_1180)
	{
		stream_protocol = _T("RTSP");
	} 
	else
	{
		CString str,strValue;
		str.Format(_T("vprofileproto%d"),nStreamIdx+1);
		mapJason[_T("stream_info")].GetValuebyKey(str,strValue);

		if (strValue.Find(_T("RTP")) != -1)
		{
			stream_protocol = _T("RTSP");
		}
		else if (strValue.Find(_T("VSM")) != -1)
		{
			stream_protocol = _T("TCP");
		}
	}
}

void CDlgMassAssignIP::GetCameraStream(NODEITEM* pNode, int iIPCamType, int nJasonMapIdx, vector<ec_Stream>& vcStream)
{
	ec_Stream streamObj;
	map<CString,CJason> mapJason;
	mapJason = GetJasonMap(nJasonMapIdx);
	
	int nTotalStream = 0; 
	if(iIPCamType == MODEL_1150)
	{
		CString streamCount;
		mapJason[_T("stream_info")].GetValuebyKey(_T("vprofilenum"),streamCount);
		nTotalStream = ::_ttoi(streamCount);
	}
	else
	{
		nTotalStream = mapJason[_T("video")].GetCountByKey(_T("profile_id"));
	}
	 
	for(int nIdx = 0; nIdx < nTotalStream; nIdx++)
	{
		streamObj.clear();
		if(nIdx == 0)
			streamObj.stream_type = _T("RE");

		GetResAndCMPR(nIdx, iIPCamType, mapJason, streamObj.video_res, streamObj.video_cmpr_type);
		GetStreamProtocol(nIdx, mapJason, iIPCamType, streamObj.stream_protocol);
		GetStreamPort(nIdx, iIPCamType, pNode, mapJason, streamObj.stream_port);
		streamObj.stream_name.Format(_T("Stream %d"), nIdx+1);
		vcStream.push_back(streamObj);
	}
}

void CDlgMassAssignIP::GetResAndCMPR(int nStreamIdx, int iIPCamType, map<CString,CJason>& mapJason, CString& video_res, CString& video_cmpr_type)
{
	CString str(_T(""));
	if (iIPCamType == MODEL_1180)
	{
		GetNVRBk()->GetResAndCMPR(mapJason[_T("video")], nStreamIdx, video_res, video_cmpr_type);
	} 
	else
	{
		vector<CString> vcStr;
		mapJason[_T("stream_info")].GetValuebyKey(_T("resolutions"), str);
		mapJason[_T("stream_info")].PaserValue(str, vcStr);
		if (nStreamIdx < vcStr.size())
		{
			video_res = GetNVRBk()->GetResolutionDesc(vcStr[nStreamIdx]);
		}
		str.Format(_T("vprofile%d"), nStreamIdx+1);
		mapJason[_T("stream_info")].GetValuebyKey(str, video_cmpr_type);
		if(video_cmpr_type == _T(""))
			video_cmpr_type = _T("Unknown");
	}
}

void CDlgMassAssignIP::GetStreamPort(int nStreamIdx, int iIPCamType, NODEITEM* pNode, map<CString,CJason>& mapJason, short& stream_port)
{
	CString str;
	if (iIPCamType == MODEL_1180)
	{
		mapJason[_T("stream")].GetValuebyKey(_T("rtsp_port"),str);
		stream_port = _ttoi(str);
	} 
	else
	{
		CString str,strValue;
		str.Format(_T("video_profile%d"),nStreamIdx+1);
		mapJason[str].GetValuebyKey(_T("port"),strValue);
		stream_port = _ttoi(strValue);
	}
}

void CDlgMassAssignIP::GetConnectType(int nType, CString& connect_type_code)
{
	if(nType == 0)
		connect_type_code = _T("Static IP");
	else if(nType == 1)
		connect_type_code = _T("DHCP");
	else if(nType == 2)
		connect_type_code = _T("PPPoE");
}

void CDlgMassAssignIP::GetVideoFormat(NODEITEM* pNode, CJason& jason, CString& video_format)
{
	CString strfish,strlens;
	DWORD dwVideoFormat = 0;
	byte byFirst = 0, byThird = 0;
	jason.GetValuebyKey(_T("System"), _T("mcp_system_fisheye"), strfish);
	jason.GetValuebyKey(_T("Lens"), _T("mcp_system_fisheye"), strlens);

	if (strfish.Find(_T("yes")) != -1 )
	{
		byFirst += FISHEYE;
	}

	if (strlens.Find(_T("26")) != -1 )
	{
		byFirst += PANORAMA_180;
	}

	if (strlens.Find(_T("25")) != -1 )
	{
		byFirst += PANORAMA_360;
	}

	byThird = pNode->byModelType;
	dwVideoFormat = GetNVRBk()->PackVideoFormat(0,byThird,0,byFirst);
	video_format.Format(_T("%d"),dwVideoFormat);
}

void CDlgMassAssignIP::GetDigitalStatus(CJason& jason, const CString& strKey, CString& strValue)
{
	jason.GetValuebyKey(strKey,strValue);
	if(strValue.Find(_T("on")) != -1)	
		strValue = _T("Y");
	else
		strValue = _T("N");
}

void CDlgMassAssignIP::GetNetMask(CJason& jason, ec_Camera& camObj)
{
	CString csNetmask;
	jason.GetValuebyKey(_T("netmask"),csNetmask);
	LPCTSTR lpszFullString = csNetmask.GetBuffer();
	AfxExtractSubString(camObj.subnet_mask1, lpszFullString, 0, '.');
	AfxExtractSubString(camObj.subnet_mask2, lpszFullString, 1, '.');
	AfxExtractSubString(camObj.subnet_mask3, lpszFullString, 2, '.');
	AfxExtractSubString(camObj.subnet_mask4, lpszFullString, 3, '.');
	csNetmask.ReleaseBuffer();
}

void CDlgMassAssignIP::GetPTZSupport(int iIPCamType, map<CString,CJason>& mapJason, CString& ptz_support)
{
	if (iIPCamType = MODEL_1180)
	{
		CString str1, str2;
		mapJason[_T("mcp")].GetValuebyKey(_T("System"),_T("mcp_system_speeddome"), str1);
		mapJason[_T("mcp")].GetValuebyKey(_T("Lens"),_T("mcp_lens_motorized"), str2);

		if (str1.Find(_T("yes")) != -1 || str2.Find(_T("yes")) != -1)
			ptz_support = _T("Y");
		else
			ptz_support = _T("N");
	} 
	else
	{
		mapJason[_T("rs485")].GetValuebyKey(_T("enable"), ptz_support);
		if(ptz_support.Find(_T("yes")) != -1)	
			ptz_support = _T("Y");
		else
			ptz_support = _T("N");
	}
}

void CDlgMassAssignIP::GetStreamUrl(int iIPCamType, map<CString,CJason>& mapJason, CString& stream_url)
{
	if (iIPCamType = MODEL_1180)
	{
		mapJason[_T("stream")].GetValuebyKey(_T("stream_name"), stream_url);
	}
	else
	{
		mapJason[_T("stream")].GetValuebyKey(_T("stream_url"), stream_url);
	}
}

void CDlgMassAssignIP::InsertCamera(NODEITEM* pNode, int iIPCamType, int nJasonMapIdx, int nCameraIdx, const vector<ec_Stream>& vcStream)
{
	USES_CONVERSION;
	ec_Camera camObj;
	map<CString,CJason> mapJason = GetJasonMap(nJasonMapIdx);

	mapJason[_T("rs485")].GetValuebyKey(_T("proto"),camObj.ptz_protocol);
	mapJason[_T("network")].GetValuebyKey(_T("dns1"), camObj.name_server);
	mapJason[_T("network")].GetValuebyKey(_T("gateway"),camObj.gateway);
	
	GetConnectType(pNode->nType, camObj.connect_type_code);
	GetDigitalStatus(mapJason[_T("io")], _T("in1"), camObj.digital_in1);
	GetDigitalStatus(mapJason[_T("io")], _T("in2"), camObj.digital_in2);
	GetDigitalStatus(mapJason[_T("io")], _T("out1"), camObj.digital_out);
	GetVideoFormat(pNode, mapJason[_T("mcp")], camObj.video_format);
	GetNetMask(mapJason[_T("network")], camObj);
	GetPTZSupport(iIPCamType, mapJason, camObj.ptz_support);
	GetStreamUrl(iIPCamType, mapJason, camObj.stream_url);
	vGetMacAddress(pNode, camObj.mac_address, false);

	camObj.camera_idx = nCameraIdx; 
	camObj.cameraname = A2W(pNode->modelname); 
	camObj.ipaddress = A2W(pNode->ip);
	camObj.username = m_strID;
	camObj.password = m_strPassword; 
	camObj.active_ = _T("Y"); 
	camObj.httpport = pNode->nWebPort; 
	camObj.model_code.Format(_T("%s"), A2W(pNode->modelname));
	camObj.brand_code = A2W(pNode->sn); 
	camObj.vcStream = vcStream;

	if (GetNVRBk())
		GetNVRBk()->InsertCamera(camObj);
}

void CDlgMassAssignIP::InsertCamRec(int nCameraIdx)
{
	ec_Camera_Rec camRec;
	camRec.clear();
	camRec.camera_idx = nCameraIdx;
	camRec.recording_type = _T("0");

	if (GetNVRBk())
		GetNVRBk()->InsertCamRec(camRec);
}

void CDlgMassAssignIP::InsertGroupCamera(int nCamId, int nCameraIdx)
{
	Cam_Group_Cam GroupCam;
	if (GetNVRBk())
		GroupCam.cameragroupid = GetNVRBk()->GetFirstGroupIdx(DeviceTree);
	GroupCam.cameraid = nCamId;
	GroupCam.camera_idx = nCameraIdx;
	GroupCam.streamid = 1;
	GroupCam.numb_ = 0;

	if (GetNVRBk())
		GetNVRBk()->InsertGroupCamera(GroupCam);	
}

UINT CDlgMassAssignIP::vSaveCGIData2DB(LPVOID lpParam)
{
	CDlgMassAssignIP* pThis = (CDlgMassAssignIP*)lpParam;

	vector<SuccessNode> vcSuccessNode;
	pThis->vGetSuccessNode(vcSuccessNode);
	int nCount = vcSuccessNode.size();
	WORD nCameraIdx = 0;
	int nCamId = 0, nJasonMapIdx = 0, iIPCamType = 0;
	vector<ec_Stream> vcStream;

	for (int nIdx = 0; nIdx < nCount ; nIdx++)
	{
		vcStream.clear();

		nCameraIdx = pThis->GetNVRBk()->GetEmptyCameraIdx();
		NODEITEM* pNode = vcSuccessNode[nIdx].pNode; 
		nJasonMapIdx = vcSuccessNode[nIdx].nJasonMapIdx; 
		iIPCamType = pNode->byModelType;
		pThis->GetCameraStream(pNode,iIPCamType,nJasonMapIdx,vcStream);
		pThis->InsertCamera(pNode, iIPCamType, nJasonMapIdx, nCameraIdx, vcStream);

		if (pThis->GetNVRBk())
			nCamId = pThis->GetNVRBk()->GetLastCameraId();

		if (pThis->GetNVRBk())
			pThis->GetNVRBk()->DeleteCamRec(nCameraIdx);
		
		if (pThis->GetNVRBk())
			pThis->GetNVRBk()->DeleteGroupCamera(nCamId);

		pThis->InsertCamRec(nCameraIdx);
		pThis->InsertGroupCamera(nCamId, nCameraIdx);
	}

	return 0;
}

bool CDlgMassAssignIP::bGetAlreadyDefinedIPCamMac(vector <IPCAM_BASE_INFO>& vtcsMacs)
{
	vtcsMacs.clear();
	vector<ec_Camera> vcCam;
	IPCAM_BASE_INFO csBaseInfo;

	if (GetNVRBk())
		GetNVRBk()->GetAllCameras(vcCam);
	int nIdx = 0, nCount = vcCam.size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		csBaseInfo.csMac = vcCam[nIdx].mac_address.MakeUpper();
		csBaseInfo.csUser = vcCam[nIdx].username;
		csBaseInfo.csPassword = vcCam[nIdx].password;
		vtcsMacs.push_back(csBaseInfo);
	}

	return true;
}

void CDlgMassAssignIP::vDoStatus(WORD nStatus)
{
	int m_nTimer=0;
	DWORD dwRet=0;
	UINT nRet(0);
	CMainFrame* pCMainFrame = (CMainFrame*)AfxGetMainWnd();
	CProgressCtrl * m_CProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	CListBox* pList2 = (CListBox*) GetDlgItem(IDC_LIST1);
	WORD nAssignIPCase[] ={NOT_ANY_ACTION_YET, PRESET_IP, SETIP, ADDTOTABLE, DONE};
	WORD nSaveToSaverCasse[] ={NOT_ANY_ACTION_YET, ADDTOTABLE, DONE};

	if (m_bAssignIP == false)
	{
		nStatus = nSaveToSaverCasse[nStatus];
	}


	switch(nStatus)
	{
	case NOT_ANY_ACTION_YET:
        m_CProgressCtrl->SetPos( 0 );
		GetDlgItem(IDC_BTN_PREVIOUS)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_STATICIP)->EnableWindow(true);
		GetDlgItem(IDC_RADIO_DHCP)->EnableWindow(true);
		GetDlgItem(IDOK)->EnableWindow(true);
		if (((CButton*)GetDlgItem(IDC_RADIO_STATICIP))->GetCheck())
		{
			GetDlgItem(IDC_IPADDRESS_IP)->EnableWindow(true);
			GetDlgItem(IDC_IPADDRESS_MASK)->EnableWindow(true);
			GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(true);
			GetDlgItem(IDC_IPADDRESS_DNS)->EnableWindow(true);
		}
		for (int ci =0; ci < m_wndList.GetItemCount(); ci++)
		{
			m_wndList.SetItemText(ci, 5, _T(""));
		}
		break;
	case PRESET_IP:
		BeginWaitCursor();
		GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(true);
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(false);
		GetDlgItem(IDOK)->EnableWindow(false);

		OnBnClickedBtnDetectAvailableIp();

		GetDlgItem(IDC_RADIO_STATICIP)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_DHCP)->EnableWindow(false);
		GetDlgItem(IDC_IPADDRESS_IP)->EnableWindow(false);
		GetDlgItem(IDC_IPADDRESS_MASK)->EnableWindow(false);
		GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(false);
		GetDlgItem(IDC_IPADDRESS_DNS)->EnableWindow(false);
		break;
	case SETIP:
		{
			CString cs;
			LOADSTRING(cs, IDS_ASSIGN_IP_WARNING);
			if( IDYES == AfxMessageBox(cs, MB_YESNO) )
			{
				CDlgAuthorize dlg(IDD_DLG_AUTHORIZE, NULL, NULL, this);
				CString strID, strpassword;
				if (dlg.DoModal() == IDOK)
				{
					dlg.vGetIDnPassword(strID, strpassword);
					vSetAuthorize(strID, strpassword);

					OnBnClickedBtnMassSet();
					GetDlgItem(IDC_BTN_PREVIOUS)->EnableWindow(false);
					GetDlgItem(IDC_BTN_NEXT)->EnableWindow(false);
					GetDlgItem(IDOK)->EnableWindow(false);
                    CGS::SetTransparent(this->m_hWnd, 200);
				}
				else
				{
					m_nStepCursor--;
                    vSetGuideStep(m_nStepCursor);
				}
			}
		}
		break;
	case ADDTOTABLE:
		{
			CString strID, strpassword;
			CDlgAuthorize dlg(IDD_DLG_AUTHORIZE, NULL, NULL, this);
			if (dlg.DoModal() == IDOK)
			{
				dlg.vGetIDnPassword(strID, strpassword);
				vSetAuthorize(strID, strpassword);
				if( !pCMainFrame->bCheckCameraCountWithMassAssignIP( iGetListCheckedCount() ) )
				{
					AfxMessageBox(_T("Over limitation of product, Please remove camera(s) on the list."));
					--m_nStepCursor;
                    vSetGuideStep(m_nStepCursor);
					break;
				}

				if (bCheckBuffer(nStatus))
				{
					pList2->EnableWindow(false);
					GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(true);
					GetDlgItem(IDOK)->EnableWindow(false);
					
					GetDlgItem(IDC_BTN_NEXT)->EnableWindow(false);
					GetDlgItem(IDC_BTN_PREVIOUS)->EnableWindow(false);
					vInsertThreadInfo2Array();
					hAddIPCam2DB = (HANDLE)::_beginthreadex(NULL, 0, ConnectThread, (LPVOID)this, 0, &nRet);
					::CloseHandle(hAddIPCam2DB);
				}
			}
			else
			{
				m_nStepCursor--;
                vSetGuideStep(m_nStepCursor);
			}
		}
		break;
	case DONE:
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(false);
		GetDlgItem(IDOK)->EnableWindow(true);
		pList2->EnableWindow(true);
		break;
	}
	CListBox* pList = (CListBox*) GetDlgItem(IDC_LIST_STATUS);
	pList->SetCurSel(nStatus);
    GetDlgItem(IDC_CHECK_ASSIGN_IP)->EnableWindow(m_nStepCursor < SETIP);
}

BOOL CDlgMassAssignIP::bCheckBuffer(WORD& nStatus)
{
	BOOL bResult = TRUE;

	map<CString, int>::iterator iPos;
	vGetPathBufferFromDB();
	for(iPos = mapPathBuffer.begin(); iPos != mapPathBuffer.end(); iPos++)
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
				nStatus = DONE;
				GetDlgItem(IDC_BTN_NEXT)->EnableWindow(false);
				GetDlgItem(IDOK)->EnableWindow(true);
				bResult = FALSE;
			}
		}
	}

	return bResult;
}

void CDlgMassAssignIP::OnBnClickedBtnNext()
{
	(m_nStepCursor < DONE) ? ++m_nStepCursor : NULL;
    vSetGuideStep(m_nStepCursor);
	vDoStatus(m_nStepCursor);	
}

void CDlgMassAssignIP::OnBnClickedBtnPrevious()
{
	(m_nStepCursor > NOT_ANY_ACTION_YET) ? --m_nStepCursor : NULL;
    vSetGuideStep(m_nStepCursor);
	vDoStatus(m_nStepCursor);	
}

void CDlgMassAssignIP::OnBnClickedOk()
{
	if(m_nStepCursor != 0)
	{
		CMainFrame * m_CMainFrame = (CMainFrame*)(::AfxGetApp()->GetMainWnd());
		CTaskTreeCtrl *pCTaskTreeCtrl = (CTaskTreeCtrl *)m_CMainFrame->GetTreeView();
		CNVRBk::GetBkObject()->SendMessage(MsgUpdateView, UM_REFRESH_DEVICETREE, 0);
	}

	DWORD nRet = SignalObjectAndWait(m_hDestory, m_hWaitIpCamWakeup, 60000, false);

	if(hAddIPCam2DB != NULL)
	{
		::AfxBeginThread(fnWaitForInsertDB, (LPVOID)this);
	}
	else	
	{
		OnOK();
	}
}

void CDlgMassAssignIP::OnBnClickedCheckAssignIp()
{
	m_bAssignIP ^=1;
	GetDlgItem(IDC_RADIO_STATICIP)->EnableWindow(m_bAssignIP);
	GetDlgItem(IDC_RADIO_DHCP)->EnableWindow(m_bAssignIP);
	GetDlgItem(IDC_IPADDRESS_IP)->EnableWindow(m_bAssignIP);
	GetDlgItem(IDC_IPADDRESS_MASK)->EnableWindow(m_bAssignIP);
	GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(m_bAssignIP);
	GetDlgItem(IDC_IPADDRESS_DNS)->EnableWindow(m_bAssignIP);

	MoveDlgItem(m_bAssignIP);
    m_nStepCursor = 0;
    vSetGuideStep(m_nStepCursor);
}

int CDlgMassAssignIP::iGetCameraIDX()
{
	int iChannelID = 1;

	if (GetNVRBk())
		iChannelID = GetNVRBk()->GetEmptyCameraIdx();

	return iChannelID;
}

BOOL CDlgMassAssignIP::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgMassAssignIP::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// TODO:  Change any attributes of the DC here
	UINT id = pWnd->GetDlgCtrlID();
	if (id == IDC_ANIMATE)
		return (HBRUSH)GetStockObject(NULL_BRUSH);

	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CDlgMassAssignIP::OnCbnSelchangeComboNetcard()
{
	// TODO: Add your control notification handler code here
	CComboBox* pNetCardCombo =(CComboBox*) GetDlgItem(IDC_COMBO_NETCARD);
	int iNetCard = pNetCardCombo->GetCurSel();

	//IP Address
	GetDlgItem(IDC_IPADDRESS_IP)->SetWindowText(NCI.GetNetCardIPAddress(iNetCard+1));

	//Subnet Mask
	GetDlgItem(IDC_IPADDRESS_MASK)->SetWindowText(NCI.GetNetCardSubnetMask(iNetCard+1));

	//Gateway
	GetDlgItem(IDC_IPADDRESS_GATEWAY)->SetWindowText(NCI.GetNetCardGateWay(iNetCard+1));

	//DNS Server
	GetDlgItem(IDC_IPADDRESS_DNS)->SetWindowText(NCI.GetNetCardDNS(iNetCard+1));
}
void vTextOutLine(CDC& dc, TCHAR* tzCaption, WORD nFontSize, CRect* pRc)
{
#define OUTLINE1 Color(50,50,50)
#define OUTLINE2 Color(255, 128, 128, 128)
#define OUTLINSHADOW Gdiplus::Color(128,30,30,30)


	using namespace Gdiplus;
	using namespace TextDesigner;

	Graphics graphics(dc.GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	OutlineText m_OutlineText;
	m_OutlineText.TextOutline(
		OUTLINE1, 
		OUTLINE2, 
		5);

	m_OutlineText.EnableShadow(true);
	//Rem to SetNullShadow() to release memory if a previous shadow has been set.
	m_OutlineText.SetNullShadow();
	m_OutlineText.Shadow(
		OUTLINSHADOW, 5, 
		Gdiplus::Point(4,4));

	FontFamily fontFamily(L"Arial");


    WORD nPosX = (pRc->Width() - nFontSize/2 * _tcslen(tzCaption))/2;
    WORD nPosY = (pRc->Height() - nFontSize )/2;

	StringFormat strFormat;
	m_OutlineText.DrawString(&graphics,&fontFamily, 
		FontStyleBold, nFontSize, tzCaption, 
		Gdiplus::Point(nPosX, nPosY), &strFormat);

}

void CDlgMassAssignIP::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CProgressCtrl * m_CProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	
	if(nIDEvent == 0)
	{
		nstatus++;
		(nstatus == 120) ? nstatus = 0: NULL;
		m_CProgressCtrl->SetPos(nstatus);
	}
    else if (nIDEvent == 1)
    {
        const  WORD nFontSize(256);
        if (m_nTimerCountdown <=0) m_nTimerCountdown = ELAPSE_TIME;
        CString cs;
        cs.Format(_T("%d"), m_nTimerCountdown--);
        CRect rect;
        GetClientRect(&rect);
        Invalidate(false);
        UpdateWindow();
        CDC* pDc = this->GetDC();
        vTextOutLine(*pDc, cs.GetBuffer(), nFontSize, &rect);
        ReleaseDC(pDc);
    }

	CBaseDialog::OnTimer(nIDEvent);
}

MODELTYPE CDlgMassAssignIP::iGetIPCamType(CString csIP, CString csWebPort)
{
	MODELTYPE mode = m_pBK->GetDeviceModelType(csIP, csWebPort, m_strID, m_strPassword);
	return mode;
}

bool CDlgMassAssignIP::bGetOldCGIData(CString csIP, CString csWebPort, int nJasonMapIdx)
{
	if (m_vcJason.size() > nJasonMapIdx)
	{
		map<CString, CJason> mapJason;
		CJason jStream_Info = m_pBK->GetStream_Info(csIP, csWebPort, m_strID, m_strPassword);
		if(jStream_Info.isError())
			return false;

		CJason jVideo = m_pBK->GetVideo(csIP, csWebPort, m_strID, m_strPassword);
		if(jVideo.isError())
			return false;

// 		CJason jStream = m_pBK->GetStream(csIP, csWebPort, m_strID, m_strPassword);
// 		if(jStream.isError())
// 			return false;

		CJason jRs485 = m_pBK->GetRS485(csIP, csWebPort, m_strID, m_strPassword);
		if(jRs485.isError())
			return false;

		CJason jIo = m_pBK->GetIO(csIP, csWebPort, m_strID, m_strPassword);
		if(jIo.isError())
			return false;

		CJason jNetwork = m_pBK->GetNetwork(csIP, csWebPort, m_strID, m_strPassword);
		if(jNetwork.isError())
			return false;

		CString str;
		CJason jprofile;
		jStream_Info.GetValuebyKey(_T("vprofilenum"),str);
		int nCount = _ttoi(str);

		for (int nidx = 0; nidx < nCount; nidx++)
		{
			jprofile = m_pBK->GetVideo_Profile(nidx+1, csIP, csWebPort, m_strID, m_strPassword);
			if(jprofile.isError())
				return false;
			else
			{
				str.Format(_T("video_profile%d"),nidx+1);
				mapJason[str] = jprofile;
			}
		}

		mapJason[_T("stream_info")] = jStream_Info;
		mapJason[_T("video")] = jVideo;
		//mapJason[_T("stream")] = jStream;
		mapJason[_T("rs485")] = jRs485;
		mapJason[_T("io")] = jIo;
		mapJason[_T("network")] = jNetwork;

		if (TryEnterCriticalSection(&m_criSec) != 0)
		{
			if (m_vcJason.size() > nJasonMapIdx)
			{
				m_vcJason[nJasonMapIdx] = mapJason;
			}
			LeaveCriticalSection(&m_criSec);
		}
	}
	return true;
}

bool CDlgMassAssignIP::bGetNewCGIData(CString csIP, CString csWebPort, int nJasonMapIdx)
{
	if (m_vcJason.size() > nJasonMapIdx)
	{
		map<CString, CJason> mapJason;
		CJason jStream = m_pBK->GetStream(csIP, csWebPort, m_strID, m_strPassword);
		if(jStream.isError())
			return false;

		CJason jRs485 = m_pBK->GetRS485(csIP, csWebPort, m_strID, m_strPassword);
		if(jRs485.isError())
			return false;

		CJason jMcp = m_pBK->GetMcp(csIP, csWebPort, m_strID, m_strPassword);
		if(jMcp.isError())
			return false;

		CJason jIo = m_pBK->GetIO(csIP, csWebPort, m_strID, m_strPassword);
		if(jIo.isError())
			return false;

		CJason jNetwork = m_pBK->GetNetwork(csIP, csWebPort, m_strID, m_strPassword);
		if(jNetwork.isError())
			return false;

		CJason jInfo = m_pBK->GetInfo(csIP, csWebPort, m_strID, m_strPassword);
		if(jInfo.isError())
			return false;

		CJason jVideo = m_pBK->GetVideo(csIP, csWebPort, m_strID, m_strPassword);
		if(jVideo.isError())
			return false;

		CJason jAudio = m_pBK->GetAudio(csIP, csWebPort, m_strID, m_strPassword);
		if(jAudio.isError())
			return false;

		mapJason[_T("stream")] = jStream;
		mapJason[_T("rs485")] = jRs485;
		mapJason[_T("mcp")] = jMcp;
		mapJason[_T("io")] = jIo;
		mapJason[_T("network")] = jNetwork;
		mapJason[_T("info")] = jInfo;
		mapJason[_T("video")] = jVideo;
		mapJason[_T("audio")] = jAudio;

		if (TryEnterCriticalSection(&m_criSec) != 0)
		{
			if (m_vcJason.size() > nJasonMapIdx)
			{
				m_vcJason[nJasonMapIdx] = mapJason;
			}
			LeaveCriticalSection(&m_criSec);
		}
	}
	return true;
}

void CDlgMassAssignIP::OnBnClickedButtonMassAssignRescan()
{
	// TODO: Add your control notification handler code here
	USES_CONVERSION;
	m_wndList.DeleteAllItems();
	
	vector <NODEITEM*> Vt = (CIPCamDiscovery::GetNodePtr());

	for(int iTemp = 0; iTemp < Vt.size(); iTemp++)
	{
		Vt[iTemp] = NULL;
	}
	Vt.clear();

 	HANDLE hScanRequire = CreateEvent(NULL, FALSE, TRUE, SCAN_REQUIRE);
 	::SetEvent(hScanRequire);
 	::CloseHandle(hScanRequire);
 
 	::Sleep(1000);

	Vt = (CIPCamDiscovery::GetNodePtr());
	
	int ci;
	bGetAlreadyDefinedIPCamMac(m_vcExistDevice);
	int nRow ;
	CString csTmp, csMac, csUIMac;
	for (ci =0,nRow = 0; ci < Vt.size(); ci++)
	{
		bool bCheck = true;

		vGetMacAddress(Vt[ci], csMac, FALSE);
		vGetMacAddress(Vt[ci], csUIMac, TRUE);

		for (int cj =0; cj < m_vcExistDevice.size(); cj++)
		{
			if (m_vcExistDevice[cj].csMac == csMac)
			{
				strncpy(Vt[ci]->szUser, W2A(m_vcExistDevice[cj].csUser), sizeof(Vt[ci]->szUser));
				strncpy(Vt[ci]->szPass, W2A(m_vcExistDevice[cj].csPassword), sizeof(Vt[ci]->szPass));
				bCheck = false;
				break;
			}
		}	

		csTmp.Format(_T("%d"), nRow+1);
		m_wndList.InsertItem(nRow, csTmp);
		m_wndList.SetItemText(nRow, 4, csUIMac);
		m_wndList.SetItemText(nRow, 1, A2W(Vt[ci]->modelname));
		switch (Vt[ci]->nType)
		{
		case EN_STATICIP:
			csTmp = _T("Static IP");
			break;
		case EN_DHCP:
			csTmp = _T("DHCP");
			break;
		case EN_PPPOE:
			csTmp = _T("PPPOE");		
			break;
		}
		m_wndList.SetItemText(nRow, 2, csTmp);
		m_wndList.SetItemText(nRow, 3, A2W(Vt[ci]->ip));

		m_wndList.SetCheck(nRow, bCheck);
		nRow++;
	}
	((CButton*)GetDlgItem(IDC_RADIO_STATICIP))->SetCheck(true);
	GetDlgItem(IDC_IPADDRESS_IP)->SetWindowText(NCI.GetNetCardIPAddress(1));
	GetDlgItem(IDC_IPADDRESS_MASK)->SetWindowText(NCI.GetNetCardSubnetMask(1));
	GetDlgItem(IDC_IPADDRESS_GATEWAY)->SetWindowText(NCI.GetNetCardGateWay(1));
	GetDlgItem(IDC_IPADDRESS_DNS)->SetWindowText(NCI.GetNetCardDNS(1));
}

UINT CDlgMassAssignIP::fnWaitForInsertDB(LPVOID lpParam)
{
	CDlgMassAssignIP* pThis = (CDlgMassAssignIP*)lpParam;

	HANDLE hINSERTDB = CreateEvent(NULL, false, false, _T("INSERT_DB_COMPLETE"));
	WaitForSingleObject(hINSERTDB, INFINITE);
	CloseHandle(hINSERTDB);
	pThis->OnOK();

	return 0;
}
void CDlgMassAssignIP::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(m_nStepCursor == PRESET_IP || m_nStepCursor == SETIP || m_nStepCursor == ADDTOTABLE)	return;

	CBaseDialog::OnClose();
}

int CDlgMassAssignIP::iGetListCheckedCount()
{
	int iCount = 0;
	for(int iTemp = 0; iTemp < m_wndList.GetItemCount(); iTemp++)
	{
		if ( m_wndList.GetCheck(iTemp) && CompareExistDevice(m_wndList.GetItemText(iTemp, 4)))
		{	
			iCount++;	
		}
	}

	return iCount;
}

bool CDlgMassAssignIP::bCheckPathBuffer(CString csPath, int iBuffer)
{
	unsigned __int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;

	GetDiskFreeSpaceEx( (LPCTSTR)csPath, (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes );
	
	if(i64FreeBytesToCaller/(1024*1024*1024) > iBuffer)	return true;
	else	return false;
}

void CDlgMassAssignIP::vGetPathBufferFromDB()
{
	vector<ec_Storage> vcStorage;
	mapPathBuffer.clear();

	if (GetNVRBk())
		GetNVRBk()->GetStorageByType(vcStorage, _T("RECORD"));
	int nIdx = 0, nCount = vcStorage.size();
	for (nIdx = 0; nIdx < nCount ; nIdx++)
	{
		mapPathBuffer.insert(::pair<CString, int>(vcStorage[nIdx].store_location, ::_ttoi(vcStorage[nIdx].buffer_size.GetBuffer())));
		vcStorage[nIdx].buffer_size.ReleaseBuffer();
	}
	return;
}

void CDlgMassAssignIP::vCheckFailIPAddress()
{
	USES_CONVERSION;

	if (m_vcFailIPAddress.size())
	{
		THREAD_INFO threadinfo;
		vector<THREAD_INFO>::iterator it = m_vcFailIPAddress.begin();
		CString strMsg, strMac;
		CString strID(_T("")), strpassword(_T(""));
		m_vcMultiThreadInfo.clear();
		m_vcJason.clear();
		m_vcSuccessNode.clear();

		while(it != m_vcFailIPAddress.end())
		{
			vGetMacAddress(it->pNodeItem, strMac);
			strMsg.Format(_T("Invalid User or Password, Please try again!\n\nModel Name:%s\nMac:%s\nIP:%s"), A2W(it->pNodeItem->modelname),strMac,A2W(it->pNodeItem->ip));

			CDlgAuthorize dlg(IDD_DLG_AUTHORIZE_MSG, &strMsg, &m_strPassword, this);
			if (dlg.DoModal() == IDOK)
			{
				dlg.vGetIDnPassword(strID, strpassword);

				vSetAuthorize(strID, strpassword);
				threadinfo = *it;
				threadinfo.bFinish = false;
				threadinfo.nThreadIdx = 0;
				threadinfo.bResult = false;
				m_vcMultiThreadInfo.push_back(threadinfo);

				map<CString, CJason> mapjason;
				m_vcJason.push_back(mapjason);
				break;
			}
			else
			{
				threadinfo = *it;
				vUpdatevcFailIPAddress(Modify_Remove, &threadinfo);
				it = m_vcFailIPAddress.begin();

				AddProgress_CurrentSection();
				RedrawPorgress();

				continue;
			}
			if(it != m_vcFailIPAddress.end())
				it++;
		}

		if (it == m_vcFailIPAddress.end())
		{
			m_bWriteDatabaseFinish = TRUE;
		}
	}
	else
	{
		m_bWriteDatabaseFinish = TRUE;
	}
}

void CDlgMassAssignIP::InitialControlText()
{
	CString csDlgCaption;
	LOADSTRING(csDlgCaption, IDS_DLG_MASS_ASSIGN_IP);
	this->SetWindowTextW(csDlgCaption);

	SETITEMTEXT(IDC_STATIC_HINT, IDS_MASS_ASSIGN_HINT);
	SETITEMTEXT(IDC_BUTTON_MASS_ASSIGN_RESCAN, IDS_MASS_ASSIGN_BTN_RESCAN);
	SETITEMTEXT(IDC_CHECK_ASSIGN_IP, IDS_MASS_ASSIGN_CHANGE_CHECK);
	SETITEMTEXT(IDC_STATIC_MASS_ASSIGN_GROUP, IDS_MASS_ASSIGN_NETWORK_GROUP);
	SETITEMTEXT(IDC_STATIC_NETWORK_CARD, IDS_MASS_ASSIGN_NETWORK_CARD);
	SETITEMTEXT(IDC_STATIC_MASS_ASSIGN_IP, IDS_MASS_ASSIGN_IP);
	SETITEMTEXT(IDC_STATIC_MASS_ASSIGN_MASK, IDS_MASS_ASSIGN_MASK);
	SETITEMTEXT(IDC_STATIC_MASS_ASSIGN_GATEWAY, IDS_MASS_ASSIGN_GATEWAY);
	SETITEMTEXT(IDC_STATIC_MASS_ASSIGN_DNS, IDS_MASS_ASSIGN_DNS);
	SETITEMTEXT(IDC_STATIC_MASS_ASSIGN_STATUS_HINT, IDS_MASS_ASSIGN_STATUS_HINT);
	SETITEMTEXT(IDC_BTN_PREVIOUS, IDS_MASS_ASSIGN_PREVIOUS);
	SETITEMTEXT(IDC_BTN_NEXT, IDS_MASS_ASSIGN_NEXT);
	SETITEMTEXT(IDOK, IDS_MASS_ASSIGN_OK);
	SETITEMTEXT(IDC_STATIC_PERCENT, IDS_MASS_ASSIGN_PROCESSING); 
	SETITEMTEXT(IDC_STATIC_HINT, IDS_MASS_ASSIGN_HINT);
	SETITEMTEXT(IDC_BUTTON_MASS_ASSIGN_RESCAN, IDS_MASS_ASSIGN_BTN_RESCAN);	
}

void CDlgMassAssignIP::InitialColumn()
{
	USES_CONVERSION;
	m_wndList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	const int nColumns = 3;
	const int nRows = 50;
	m_wndList.InsertColumn(0, _T("#"), LVCFMT_LEFT, 40);

	CString csListTitles;
	LOADSTRING(csListTitles, IDS_MASS_ASSIGN_LIST_TITLES);
	int nColumn =0;
	CString tzTitle[6] ={};
	CClientDC dc(this);
	CSize cs;

	for(nColumn = 0; nColumn < 6; nColumn++)
	{
		::AfxExtractSubString(tzTitle[nColumn], csListTitles, nColumn, ';');
		m_wndList.InsertColumn(nColumn+1, tzTitle[nColumn], LVCFMT_LEFT, 60 + nColumn * 10);

		if(nColumn == 0)
		{
			cs = dc.GetTextExtent(_T("EV8782Q-BDD"));
			m_wndList.SetColumnWidth(nColumn+1, cs.cx);
		}
		else if(nColumn == 2)
		{
			cs = dc.GetTextExtent(_T("255.255.255.255"));
			m_wndList.SetColumnWidth(nColumn+1, cs.cx);
		}
		else if(nColumn == 3)
		{
			cs = dc.GetTextExtent(_T("00-00-00-00-00-00-00"));
			m_wndList.SetColumnWidth(nColumn+1, cs.cx);
		}
		else if(nColumn == 5)
		{
			cs = dc.GetTextExtent(_T("Insert To DB OK"));
			m_wndList.SetColumnWidth(nColumn+1, cs.cx);
		}
		else
		{
			cs = dc.GetTextExtent(tzTitle[nColumn]);
			m_wndList.SetColumnWidth(nColumn+1, cs.cx);
		}
	}
	vSetListCtrlCheckBoxHeader();

	vector <NODEITEM*> Vt = (CIPCamDiscovery::GetNodePtr());
	int ci;
	bGetAlreadyDefinedIPCamMac(m_vcExistDevice);
	int nRow ;
	for (ci =0,nRow = 0; ci < Vt.size(); ci++)
	{
		CString csTmp, csMac, csUIMac;
		bool bCheck = true;
		vGetMacAddress(Vt[ci], csMac, FALSE);
		vGetMacAddress(Vt[ci], csUIMac, TRUE);

		for (int cj =0; cj < m_vcExistDevice.size(); cj++)
		{
			if (m_vcExistDevice[cj].csMac == csMac)
			{
				strncpy(Vt[ci]->szUser, W2A(m_vcExistDevice[cj].csUser), sizeof(Vt[ci]->szUser));
				strncpy(Vt[ci]->szPass, W2A(m_vcExistDevice[cj].csPassword), sizeof(Vt[ci]->szPass));
				bCheck = false;
				break;
			}
		}	

		csTmp.Format(_T("%d"), nRow+1);
		m_wndList.InsertItem(nRow, csTmp);
		m_wndList.SetItemText(nRow, 4, csUIMac);
		m_wndList.SetItemText(nRow, 1, A2W(Vt[ci]->modelname));
		switch (Vt[ci]->nType)
		{
		case EN_STATICIP:
			csTmp = _T("Static IP");
			break;
		case EN_DHCP:
			csTmp = _T("DHCP");
			break;
		case EN_PPPOE:
			csTmp = _T("PPPOE");		
			break;
		}
		m_wndList.SetItemText(nRow, 2, csTmp);
		m_wndList.SetItemText(nRow, 3, A2W(Vt[ci]->ip));

		m_wndList.SetCheck(nRow, bCheck);
		nRow++;
	}
}

void CDlgMassAssignIP::InitialControlStatus()
{
	CString str;
	int iTemp = 0;

	((CButton*)GetDlgItem(IDC_CHECK_ASSIGN_IP))->SetCheck(m_bAssignIP);
	GetDlgItem(IDC_RADIO_STATICIP)->EnableWindow(m_bAssignIP);
	GetDlgItem(IDC_RADIO_DHCP)->EnableWindow(m_bAssignIP);
	GetDlgItem(IDC_IPADDRESS_IP)->EnableWindow(m_bAssignIP);
	GetDlgItem(IDC_IPADDRESS_MASK)->EnableWindow(m_bAssignIP);
	GetDlgItem(IDC_IPADDRESS_GATEWAY)->EnableWindow(m_bAssignIP);
	GetDlgItem(IDC_IPADDRESS_DNS)->EnableWindow(m_bAssignIP);

	((CButton*)GetDlgItem(IDC_RADIO_STATICIP))->SetCheck(true);
	GetDlgItem(IDC_IPADDRESS_IP)->SetWindowText(NCI.GetNetCardIPAddress(1));
	GetDlgItem(IDC_IPADDRESS_MASK)->SetWindowText(NCI.GetNetCardSubnetMask(1));
	GetDlgItem(IDC_IPADDRESS_GATEWAY)->SetWindowText(NCI.GetNetCardGateWay(1));
	GetDlgItem(IDC_IPADDRESS_DNS)->SetWindowText(NCI.GetNetCardDNS(1));

	GetDlgItem(IDC_BTN_PREVIOUS)->EnableWindow(false);

	int iNetCardNum = NCI.GetNetCardNum();
	CComboBox* pNetCardCombo =(CComboBox*) GetDlgItem(IDC_COMBO_NETCARD);
	for(iTemp = 0; iTemp < iNetCardNum; iTemp++)
	{
		str = NCI.GetNetCardDeviceName(iTemp+1);
		pNetCardCombo->InsertString(iTemp, str);
	}

	for(iTemp = 0; iTemp < iNetCardNum; iTemp++)
	{
		GetDlgItem(IDC_IPADDRESS_IP)->GetWindowTextW(str);
		if(str == NCI.GetNetCardIPAddress(iTemp+1) )
		{
			pNetCardCombo->SetCurSel(iTemp);
		}
	}
}

void CDlgMassAssignIP::InitialListBox()
{
	CListBox* pList = (CListBox*) GetDlgItem(IDC_LIST_STATUS);
	CString csListTitle;
	LOADSTRING(csListTitle, IDS_ASSIGN_IP_1);
	pList->InsertString(0, csListTitle);

	LOADSTRING(csListTitle, IDS_ASSIGN_IP_2);
	pList->InsertString(1, csListTitle);

	LOADSTRING(csListTitle, IDS_ASSIGN_IP_3);
	pList->InsertString(2, csListTitle);

	LOADSTRING(csListTitle, IDS_ASSIGN_IP_4);
	pList->InsertString(3, csListTitle);

	LOADSTRING(csListTitle, IDS_ASSIGN_IP_5);
	pList->InsertString(4, csListTitle);
	pList->SetCurSel(0);
	pList->EnableWindow(false);
}

void CDlgMassAssignIP::RestartProgress( int SectionCount )
{ 
	GetProgressCtrl()->SetPos(0); 
	m_nProgress_CurrentSection = 0; 
	m_nProgress_SectionCount = 0;
	m_nProgress_SectionCount = SectionCount;
}

void CDlgMassAssignIP::RedrawPorgress()
{
	if (0 < m_nProgress_SectionCount)
	{
		int perSectionScale = m_nProgress_FullScale / m_nProgress_SectionCount;
		int nPercent = 0;

		if (m_nProgress_CurrentSection < m_nProgress_SectionCount)
		{
			nPercent = perSectionScale*m_nProgress_CurrentSection;
		}
		else
		{
			nPercent = 100;
		}
		GetProgressCtrl()->SetPos(nPercent);
		CString csPercent;
		csPercent.Format(_T("%d%%"), nPercent );
		GetDlgItem(IDC_STATIC_PERCENT)->SetWindowTextW(csPercent);
	}	
}

void CDlgMassAssignIP::vGetMacAddress(NODEITEM* pNode, CString& str, BOOL bDash)
{
	if (pNode)
	{
		if (bDash)
		{
			str.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),
				pNode->mac[0],
				pNode->mac[1],
				pNode->mac[2],
				pNode->mac[3],
				pNode->mac[4],
				pNode->mac[5]);	
		}
		else
		{
			str.Format(_T("%02X%02X%02X%02X%02X%02X"),
				pNode->mac[0],
				pNode->mac[1],
				pNode->mac[2],
				pNode->mac[3],
				pNode->mac[4],
				pNode->mac[5]);	
		}
	}
}

/************************************************************************/
/*          CDlgAuthorize dialog                                                     */
/************************************************************************/
IMPLEMENT_DYNAMIC(CDlgAuthorize, CDialog)

CDlgAuthorize::CDlgAuthorize(DWORD dwDlgID, CString* pStrMsg, CString* pStrPaw, CWnd* pParent )
: CDialog(dwDlgID, pParent),m_strModelName(_T("")), m_strMacAddress(_T("")), m_strIPAddress(_T(""))
, m_strID(_T("root")), m_strPassword(_T("")), m_strMsg(_T(""))
{
	if (pStrMsg)
	{
		m_strMsg = *pStrMsg;
	}

	if (pStrPaw)
	{
		m_strPassword = *pStrPaw;
	}
}

CDlgAuthorize::~CDlgAuthorize()
{
}

void CDlgAuthorize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
}


BEGIN_MESSAGE_MAP(CDlgAuthorize, CDialog)
ON_WM_ACTIVATE()
END_MESSAGE_MAP()

void CDlgAuthorize::vGetIDnPassword( CString& strID, CString& strPassword )
{
	strID = m_strID;
	strPassword = m_strPassword;
}

BOOL CDlgAuthorize::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE; 
}

void CDlgAuthorize::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	CRect rec, dlgrec;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_MSG);
	if (!m_strMsg.IsEmpty() && pWnd)
	{
		pWnd->SetWindowText(m_strMsg);
	}
	// TODO: Add your message handler code here
}
