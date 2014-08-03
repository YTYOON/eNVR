// SettingPageNetwork.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
//#include "LibENCPTest.h"
#include "NVSENCPLib.h"
#include "SettingPageNetwork.h"
#include "ENCPSettingPage.h"
//#include "Authentication.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingPageNetwork dialog


CSettingPageNetwork::CSettingPageNetwork(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingPageNetwork::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingPageNetwork)
	m_nChoice = 0;
	m_pppDnsType = -1;
	//}}AFX_DATA_INIT
	m_nOldChoice = -1;

	m_bEnableNetwork = 100;
	m_bEnablePPPoE = 100;
	m_BrushDocBar.CreateSolidBrush(BKCOLOR);
}


void CSettingPageNetwork::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingPageNetwork)
	DDX_Control(pDX, IDC_EDIT_SVCNAME, m_editPPPService);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPPPPassword);
	DDX_Control(pDX, IDC_EDIT_MTU, m_editPPPMTU);
	DDX_Control(pDX, IDC_EDIT_ID, m_editPPPID);
	DDX_Control(pDX, IDC_IPADDRESS, m_netIP);
	DDX_Control(pDX, IDC_IP_NS, m_netIPNS);
	DDX_Control(pDX, IDC_IP_NETMASK, m_netIPNM);
	DDX_Control(pDX, IDC_IP_GW, m_netIPGW);
	DDX_Control(pDX, IDC_IP_DNS_ADDR, m_pppDns);
	DDX_Radio(pDX, IDC_RADIO_DHCP, m_nChoice);
	DDX_Radio(pDX, IDC_RADIO_AUTODNS, m_pppDnsType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingPageNetwork, CDialog)
	//{{AFX_MSG_MAP(CSettingPageNetwork)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IP_GW, OnFieldchangedIpGw)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IP_NETMASK, OnFieldchangedIpNetmask)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IP_NS, OnFieldchangedIpNs)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS, OnFieldchangedIpaddress)
	ON_BN_CLICKED(IDC_RADIO_DHCP, OnChoiceChanged)
	ON_BN_CLICKED(IDC_RADIO_AUTODNS, OnRadioDnsType)
	ON_EN_CHANGE(IDC_EDIT_ID, OnChangeEdit)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_RESTORE, OnRestore)
	ON_BN_CLICKED(IDC_RADIO_STATIC, OnChoiceChanged)
	ON_BN_CLICKED(IDC_RADIO_PPPOE, OnChoiceChanged)
	ON_BN_CLICKED(IDC_RADIO_MANUALDNS, OnRadioDnsType)
	ON_EN_CHANGE(IDC_EDIT_MTU, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_SVCNAME, OnChangeEdit)
//	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IP_DNS_ADDR, OnChangeEdit)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingPageNetwork message handlers
void CSettingPageNetwork::ShowNetworkControls(BOOL bShow)
{
	if( bShow ) bShow = SW_SHOW;
	m_netIP.ShowWindow(bShow);
	m_netIPGW.ShowWindow(bShow);
	m_netIPNM.ShowWindow(bShow);
	m_netIPNS.ShowWindow(bShow);

	GetDlgItem(IDC_STATIC_IP)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_NM)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_NS)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_GW)->ShowWindow(bShow);
}

void CSettingPageNetwork::ShowPPPControls(BOOL bShow)
{
	if( bShow ) bShow = SW_SHOW;
	GetDlgItem(IDC_EDIT_ID)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_PASSWORD)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_SVCNAME)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_MTU)->ShowWindow(bShow);
	GetDlgItem(IDC_IP_DNS_ADDR)->ShowWindow(bShow);
	GetDlgItem(IDC_RADIO_AUTODNS)->ShowWindow(bShow);
	GetDlgItem(IDC_RADIO_MANUALDNS)->ShowWindow(bShow);

	GetDlgItem(IDC_STATIC_ID)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_PASS)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_SVCNAME)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_MTU)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_DNS_SERVER)->ShowWindow(bShow);
}

void CSettingPageNetwork::EnableNetworkControls(BOOL bEnable)
{
	if( m_bEnableNetwork == bEnable ) return;
	m_netIP.EnableWindow(bEnable);
	m_netIPGW.EnableWindow(bEnable);
	m_netIPNM.EnableWindow(bEnable);
	m_netIPNS.EnableWindow(bEnable);
	m_bEnableNetwork = bEnable;
}

void CSettingPageNetwork::EnablePPPControls(BOOL bEnable)
{
	if( m_bEnablePPPoE == bEnable ) return;

	UpdateData();
	GetDlgItem(IDC_EDIT_ID)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SVCNAME)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_MTU)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_AUTODNS)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_MANUALDNS)->EnableWindow(bEnable);
	GetDlgItem(IDC_IP_DNS_ADDR)->EnableWindow(m_pppDnsType);

	OnRadioDnsType();

	m_bEnablePPPoE = bEnable;
}

void CSettingPageNetwork::OnChoiceChanged() 
{
	UpdateData();
	if( m_nOldChoice == m_nChoice ) return;
	switch( m_nChoice )
	{
	case 0:
		switch(m_nOldChoice)
		{
		case 2:		// PPPOE
			EnablePPPControls(FALSE);
			break;
		case -1:	// INITIAL
			ShowPPPControls(FALSE);
			ShowNetworkControls();
		case 1:		// STATIC IP
			EnableNetworkControls(FALSE);
		}
		break;
	case 1:
		ShowPPPControls(FALSE);
		ShowNetworkControls();
		EnableNetworkControls();
		break;
	case 2:
		ShowPPPControls();
		ShowNetworkControls(FALSE);
		EnablePPPControls();
		break;
	}
	m_nOldChoice = m_nChoice;
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(TRUE);
}

BOOL CSettingPageNetwork::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LoadNetworkInfo();

	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(FALSE);

	//SetClassLong( m_hWnd, GCL_HBRBACKGROUND, (LONG)m_BrushDocBar.GetSafeHandle());	
	SetClassLongPtr( m_hWnd, GCL_HBRBACKGROUND, (LONG)m_BrushDocBar.GetSafeHandle());	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingPageNetwork::OnRadioDnsType() 
{
	UpdateData();
	GetDlgItem(IDC_IP_DNS_ADDR)->EnableWindow(m_pppDnsType);
}

void CSettingPageNetwork::OnChangeEdit() 
{
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(TRUE);
}

void CSettingPageNetwork::OnFieldchangedIpGw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(TRUE);
	
	*pResult = 0;
}

void CSettingPageNetwork::OnFieldchangedIpNetmask(NMHDR* pNMHDR, LRESULT* pResult) 
{
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(TRUE);
	
	*pResult = 0;
}

void CSettingPageNetwork::OnFieldchangedIpNs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(TRUE);
	
	*pResult = 0;
}

void CSettingPageNetwork::OnFieldchangedIpaddress(NMHDR* pNMHDR, LRESULT* pResult) 
{
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(TRUE);

	*pResult = 0;
}

void CSettingPageNetwork::OnApply() 
{
	CEncpSettingPage * pParent = (CEncpSettingPage*)GetParent();

	UpdateData();
	if( pParent->ShowAuthDialog() == IDOK )
	{
		GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
		pParent->OnApply(IDD_SETTING_PAGE_NETWORK);
	}
}

void CSettingPageNetwork::OnRestore() 
{
	LoadNetworkInfo();

	((CEncpSettingPage*)GetParent())->OnRestore(IDD_SETTING_PAGE_HOSTNAME);
	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(FALSE);
}

void CSettingPageNetwork::GetSettingValue(PENCPNETWORK pNetInfo)
{
	UpdateData();
	USES_CONVERSION;
	if( pNetInfo == NULL ) return;
	switch( GetCheckedRadioButton(IDC_RADIO_DHCP, IDC_RADIO_PPPOE) )
	{
	case 0:	return;
	case IDC_RADIO_DHCP:
		pNetInfo->nettype = EN_DHCP;
		break;
	case IDC_RADIO_STATIC:
		pNetInfo->nettype = EN_STATICIP;
		break;
	case IDC_RADIO_PPPOE:
		pNetInfo->nettype = EN_PPPOE;
		break;
	}
	TCHAR wszTmp[MAX_IP_LENGTH];
	m_netIP.GetWindowText(wszTmp, MAX_IP_LENGTH );
	strncpy(pNetInfo->STATICIP.ip, W2A(wszTmp), sizeof(pNetInfo->STATICIP.ip)); 
	m_netIPGW.GetWindowText(wszTmp, MAX_IP_LENGTH );
	strncpy(pNetInfo->STATICIP.gateway, W2A(wszTmp), sizeof(pNetInfo->STATICIP.ip)); 
	m_netIPNM.GetWindowText(wszTmp , MAX_IP_LENGTH );
	strncpy(pNetInfo->STATICIP.netmask, W2A(wszTmp), sizeof(pNetInfo->STATICIP.ip)); 
	m_netIPNS.GetWindowText(wszTmp , MAX_IP_LENGTH );
	strncpy(pNetInfo->STATICIP.nameserver, W2A(wszTmp), sizeof(pNetInfo->STATICIP.ip)); 
	switch( GetCheckedRadioButton(IDC_RADIO_AUTODNS, IDC_RADIO_MANUALDNS) )
	{
	case IDC_RADIO_MANUALDNS:
		pNetInfo->PPPOE.dns_server_type = DNS_MANUAL;
		break;
	default:
		pNetInfo->PPPOE.dns_server_type = DNS_AUTOMATIC;
		break;
	}
	GetDlgItemText(IDC_EDIT_ID, A2W(pNetInfo->PPPOE.id), MAX_PPPOE_ID_LEN);
	pNetInfo->PPPOE.mtu = GetDlgItemInt(IDC_EDIT_MTU);
	m_pppDns.GetWindowText(wszTmp, MAX_IP_LENGTH);
	strncpy(pNetInfo->PPPOE.nameserver, W2A(wszTmp), sizeof(pNetInfo->STATICIP.ip)); 
	GetDlgItemText(IDC_EDIT_PASSWORD, A2W(pNetInfo->PPPOE.password), MAX_PPPOE_PASS_LEN);
	GetDlgItemText(IDC_EDIT_SVCNAME, A2W(pNetInfo->PPPOE.service), MAX_PPPOE_SERVICE_LEN);
}

void CSettingPageNetwork::LoadNetworkInfo()
{
	USES_CONVERSION;
	PENCPNETWORK pNetInfo = &((CEncpSettingPage*)GetParent())->m_netInfo;
	switch( pNetInfo->nettype )
	{
	case EN_STATICIP:
		CheckRadioButton(IDC_RADIO_DHCP, IDC_RADIO_PPPOE, IDC_RADIO_STATIC);
		break;
	case EN_DHCP:
		CheckRadioButton(IDC_RADIO_DHCP, IDC_RADIO_PPPOE, IDC_RADIO_DHCP);
		break;
	case EN_PPPOE:
		CheckRadioButton(IDC_RADIO_DHCP, IDC_RADIO_PPPOE, IDC_RADIO_PPPOE);
		break;
	}
	OnChoiceChanged();

	if (strlen(pNetInfo->STATICIP.ip) == 0)
	{
		SetDlgItemText(IDC_STATIC_IP, _T("IP Addr. Start:"));
	}
	m_netIP.SetWindowText( A2W(pNetInfo->STATICIP.ip) );
	m_netIPGW.SetWindowText( A2W(pNetInfo->STATICIP.gateway) );
	m_netIPNS.SetWindowText( A2W(pNetInfo->STATICIP.nameserver) );
	m_netIPNM.SetWindowText( A2W(pNetInfo->STATICIP.netmask) );

	m_editPPPID.SetWindowText( A2W(pNetInfo->PPPOE.id) );
	m_editPPPPassword.SetWindowText( A2W(pNetInfo->PPPOE.password ));
	m_editPPPService.SetWindowText( A2W(pNetInfo->PPPOE.service) );
	SetDlgItemInt(IDC_EDIT_MTU, pNetInfo->PPPOE.mtu);
	switch( pNetInfo->PPPOE.dns_server_type )
	{
	case DNS_AUTOMATIC:
		CheckRadioButton(IDC_RADIO_AUTODNS, IDC_RADIO_MANUALDNS, IDC_RADIO_AUTODNS);
		break;
	case DNS_MANUAL:
		CheckRadioButton(IDC_RADIO_AUTODNS, IDC_RADIO_MANUALDNS, IDC_RADIO_MANUALDNS);
		break;
	}
	CString cs = A2W(pNetInfo->PPPOE.nameserver);
	m_pppDns.SetWindowText( cs );
	OnRadioDnsType();
}

void CSettingPageNetwork::OnClose() 
{
	((CEncpSettingPage*)GetParent())->Close(IDCANCEL);
}
BOOL CSettingPageNetwork::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);	
	pDC->SetBkColor(BKCOLOR);
	CRect rect;
	GetClientRect(rect);
	CBrush brush(BKCOLOR);
	pDC->FillRect(&rect, &brush);

	return true;
}