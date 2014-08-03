// DlgPTZSetting.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPTZSetting.h"
#include "mycurl.h"
const int MAXBUF = 4096;
#define PATROLDURATIONDEFAULT _T("3")

// CDlgPTZSetting dialog

IMPLEMENT_DYNAMIC(CDlgPTZSetting, CBaseDialog)

HBRUSH CDlgPTZSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CDlgPTZSetting::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

CDlgPTZSetting::CDlgPTZSetting(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgPTZSetting::IDD, pParent),m_pNode(NULL), m_IP(_T("")),m_pInfo(NULL),m_Port(_T(""))
{
	
	
//m_pnfo = new PTZSettingInfo;
	//m_nPatrolSeqDurIdx=0;
	m_nPatrolSeqCount=0;
}

CDlgPTZSetting::~CDlgPTZSetting()
{
	
}

void CDlgPTZSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PTZ_SETTING_PROTOCOL, m_combobox_protocol);
	DDX_Control(pDX, IDC_EDIT_PTZ_SETTING_DEVICEID, m_edit_deviceid);
	DDX_Control(pDX, IDC_COMBO_PTZ_SETTING_BAUD_RATE, m_combobox_baudRate);
	DDX_Control(pDX, IDC_COMBO_PTZ_SETTING_DATA_BITS, m_combobox_data_bits);
	DDX_Control(pDX, IDC_COMBO_PTZ_SETTING_PARITY, m_combobox_parity);
	DDX_Control(pDX, IDC_COMBO_PTZ_SETTING_STOP_BITS, m_combobox_stop_bits);
	DDX_Control(pDX, IDC_COMBO_PTZ_SETTING_PTZ_SPEED, m_combobox_ptzspeed);
	DDX_Control(pDX, IDC_EDIT_PTZ_SETTING_CAMERA_NAME, m_csCameraName);	
	DDX_Control(pDX, IDC_EDIT_PATROL_DURATION, m_editPatrolDuration);
	DDX_Control(pDX, IDC_GROUPBOX_PATROL_SEQ, m_groupbox_patrol_seq);
}


BEGIN_MESSAGE_MAP(CDlgPTZSetting, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_PTZ_SETTING_PROTOCOL, &CDlgPTZSetting::OnCbnSelchangeComboPtzSettingProtocol)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CDlgPTZSetting::OnBnClickedOk)	
	ON_BN_CLICKED(IDC_BTN_SET_PATROL, &CDlgPTZSetting::OnBnClickedBtnSetPatrol)
	ON_BN_CLICKED(IDC_BTN_RESET_PATROL, &CDlgPTZSetting::OnBnClickedBtnResetPatrol)
	ON_BN_CLICKED(IDCANCEL, &CDlgPTZSetting::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CDlgPTZSetting::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	// Protocol combobox
	m_combobox_protocol.AddString(_T("Disable"));	
	m_combobox_protocol.AddString(_T("Dynacolor"));
	m_combobox_protocol.AddString(_T("Merit_Lilin"));
	m_combobox_protocol.AddString(_T("PelcoD"));
	m_combobox_protocol.AddString(_T("PelcoP"));
	m_combobox_protocol.AddString(_T("Visca"));
	m_combobox_protocol.AddString(_T("Transparent"));	
	m_combobox_protocol.SetCurSel(0);

	//Device ID
	m_edit_deviceid.SetWindowTextW(_T("0"));

	//Baud Rate
	//m_combobox_baudRate.AddString(_T("1200"));
	//m_combobox_baudRate.AddString(_T("2400"));
	//m_combobox_baudRate.AddString(_T("4800"));
	//m_combobox_baudRate.AddString(_T("9600"));
	//m_combobox_baudRate.AddString(_T("19200"));
	m_combobox_baudRate.SetCurSel(4);

	//Data Bits
	m_combobox_data_bits.AddString(_T("7"));
	m_combobox_data_bits.AddString(_T("8"));
	m_combobox_data_bits.SetCurSel(1);

	//Parity
	m_combobox_parity.AddString(_T("None"));
	m_combobox_parity.AddString(_T("Even"));
	m_combobox_parity.AddString(_T("Odd"));
	m_combobox_parity.SetCurSel(1);

	//Stop bits
	m_combobox_stop_bits.AddString(_T("1"));
	m_combobox_stop_bits.AddString(_T("2"));
	m_combobox_stop_bits.SetCurSel(0);

	//PTZ Speed
	m_combobox_ptzspeed.AddString(_T("1"));
	m_combobox_ptzspeed.AddString(_T("2"));
	m_combobox_ptzspeed.AddString(_T("3"));
	m_combobox_ptzspeed.AddString(_T("4"));
	m_combobox_ptzspeed.AddString(_T("5"));
	m_combobox_ptzspeed.AddString(_T("6"));
	m_combobox_ptzspeed.SetCurSel(m_nPTZSpeed-1);	

	// Patrol Duraion
	m_editPatrolDuration.SetWindowTextW(PATROLDURATIONDEFAULT);

	// Patrol Sequence Group Box
	m_groupbox_patrol_seq.SetWindowTextW(_T("Patrol Sequence"));

	vGetRS485Info();
	vDisplayRS485Info();

	// Display PTZSpeed Combobox
	CString csSpeed;
	m_combobox_protocol.GetWindowText(csSpeed);
	if (m_combobox_protocol.GetCurSel() != -1)
	{
		GetDlgItem(IDC_STATIC_PTZ7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_PTZ_SETTING_PTZ_SPEED)->ShowWindow(SW_SHOW);
	}
	else if(m_combobox_protocol.GetCurSel() == -1)
	{
		GetDlgItem(IDC_STATIC_PTZ7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_PTZ_SETTING_PTZ_SPEED)->ShowWindow(SW_HIDE);
	}

	if(m_pNode->bSupportPTZ)
	{
		GetDlgItem(IDC_STATIC_PTZ7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_PTZ_SETTING_PTZ_SPEED)->ShowWindow(SW_SHOW);
	}

	
	if(m_pNode)
	{
		USES_CONVERSION;
	    TCHAR tzCameraName[128];	
		wcscpy(tzCameraName,   A2W(m_pNode->camera_name));
		CString csCameraName; 

		csCameraName.Format(_T("%s"),tzCameraName);
		if(csCameraName.IsEmpty()) csCameraName = _T("Untitled");
		m_csCameraName.SetWindowTextW(csCameraName);
	}
	if(!m_pNode)
	{
		m_csCameraName.SetWindowTextW(m_csCameraNameString);
	}

	// Get Patrol sequence and duraion info from registry
	//if(CGS::bGetPatrolSeqDuration((*m_pInfo->pvctmapPatrolSeqDurSet)))
	if (m_mapPatrolSeqDuration.size()>0)
	{
		vSetPatrolSeqText();
	}
	else
	{
		m_csPatrolPointSet.Empty();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

	
// CDlgPTZSetting message handlers

void CDlgPTZSetting::OnCbnSelchangeComboPtzSettingProtocol()
{
	// TODO: Add your control notification handler code here
	if (m_combobox_protocol.GetCurSel() !=0)
	{
		GetDlgItem(IDC_STATIC_PTZ7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_PTZ_SETTING_PTZ_SPEED)->ShowWindow(SW_SHOW);
	}
	else if(m_combobox_protocol.GetCurSel() == 0)
	{
		GetDlgItem(IDC_STATIC_PTZ7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_PTZ_SETTING_PTZ_SPEED)->ShowWindow(SW_HIDE);
	}
		
}

void CDlgPTZSetting::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	//GET RS485 info
    vGetPTZSettingInfo();


	//if(m_mapPatrolSeqDuration.size() <= 1)
	//{
	//	AfxMessageBox(_T("Please Set more than 1 patrol points"));
	//	return;
	//}

	if (m_pNode->bSupportPTZ)
	{
		CString csSpeed;
		GetDlgItem(IDC_COMBO_PTZ_SETTING_PTZ_SPEED)->GetWindowText(csSpeed);
		m_nPTZSpeed=_wtoi(csSpeed);
	}

	CGS::bSetPatrolSeqDuration(m_mapPatrolSeqDuration,m_pNode->mac);	
	OnOK();
}

void CDlgPTZSetting::vGetPTZSettingInfo()
{

	if(m_pInfo == NULL)
		return;

	this->GetDlgItemTextW(IDC_COMBO_PTZ_SETTING_PROTOCOL,m_pInfo->csProtocol);
	this->GetDlgItemTextW(IDC_EDIT_PTZ_SETTING_DEVICEID,m_pInfo->csDeviceID);
	this->GetDlgItemTextW(IDC_COMBO_PTZ_SETTING_BAUD_RATE,m_pInfo->csBraudRate);
	this->GetDlgItemTextW(IDC_COMBO_PTZ_SETTING_DATA_BITS,m_pInfo->csDataBits);
	this->GetDlgItemTextW(IDC_COMBO_PTZ_SETTING_PARITY,m_pInfo->csParity);
	this->GetDlgItemTextW(IDC_COMBO_PTZ_SETTING_STOP_BITS,m_pInfo->csStopBits);
	this->GetDlgItemTextW(IDC_COMBO_PTZ_SETTING_PTZ_SPEED,m_pInfo->csPTZSpeed);
	
}

void CDlgPTZSetting::vGetRS485Info()
{
		if(m_pNode)
		{
		USES_CONVERSION;
		TCHAR tzIP[32];
		TCHAR tzUser[32];
		TCHAR tzPass[32];
		wcscpy(tzUser, A2W(m_pNode->szUser));
		wcscpy(tzPass, A2W(m_pNode->szPass));
		wcscpy(tzIP,   A2W(m_pNode->ip));
		CString csIP;
		csIP.Format(_T("%s"),tzIP);
		

		CString csUrl, csBuffer,csUser,csPass;
		csUrl.Format(_T("http://%s:%s@%s:%d/config/rs485.cgi"),tzUser,tzPass, csIP,m_pNode->nWebPort);
		Curlplus CGIScanData;
		csUser.Format(_T("%s"),tzUser);
		csPass.Format(_T("%s"),tzPass);
		csBuffer = CGIScanData.get(csUrl,csUser,csPass);
		
		// old model
		if(csBuffer.Find(_T("{")) ==-1)
		vParseSegment(csBuffer.GetBuffer());
		else
		{//new model
		
			csBuffer.Remove(_T('{'));
			csBuffer.Remove(_T('}'));
			csBuffer.Remove(_T('"'));
			vParseSegmentNew(csBuffer.GetBuffer());
		}

		
		}
		else if (m_ptzAuthInfo.csIP !=NULL && m_ptzAuthInfo.nPort!=0)
		{

			CString csUrl, csBuffer,csUser,csPass;
			csUrl.Format(_T("http://%s:%d/config/rs485.cgi"),
				
				m_ptzAuthInfo.csIP,
				m_ptzAuthInfo.nPort
				);

			Curlplus CGIScanData;
			csUser.Format(_T("%s"), m_ptzAuthInfo.csUser);
			csPass.Format(_T("%s"), m_ptzAuthInfo.csPass);
			csBuffer = CGIScanData.get(csUrl,csUser,csPass);
		    vParseSegment(csBuffer.GetBuffer());
		
		}
}

void CDlgPTZSetting::vParseSegment(TCHAR* tszBuffer)
{	
	
	TCHAR* tszLine;
	int ci = 0;
	tszLine = wcstok(tszBuffer, _T("\r\n"));
	while (tszLine)
	{
		TCHAR* tszName = tszLine;
		TCHAR* tsz = wcschr(tszLine, '=');		
		if (tsz)
		{
			*tsz = '\0';
			tsz++;
			switch(ci)
			{
			case 0:
				m_pInfo->csEnable = tsz;
			break;
			case 1:
				m_pInfo->csProtocol =tsz;
			break;
			case 2:
				m_pInfo->csDeviceID =tsz;
			break;
			case 3:
				m_pInfo->csBraudRate =tsz;
			break;
			case 4:
				m_pInfo->csDataBits =tsz;
			break;
			case 5:
				m_pInfo->csParity =tsz;
			break;
			case 6:
				m_pInfo->csStopBits =tsz;
			break;

			}			
		}
		tszLine = wcstok(NULL, _T("\r\n"));
		ci++;
	}		
	return;
}

void CDlgPTZSetting::vParseSegmentNew(TCHAR* tszBuffer)
{	
	
	TCHAR* tszLine;
	int ci = 1;
	tszLine = wcstok(tszBuffer, _T(","));
	while (tszLine)
	{
		TCHAR* tszName = tszLine;
		TCHAR* tsz = wcschr(tszLine, ':');		
		if (tsz)
		{
			*tsz = '\0';
			tsz++;
			switch(ci)
			{
			case 0:
				m_pInfo->csEnable = tsz;
			break;
			case 1:
				m_pInfo->csProtocol =tsz;
			break;
			case 2:
				m_pInfo->csDeviceID =tsz;
			break;
			case 3:
				m_pInfo->csBraudRate =tsz;
			break;
			case 4:
				m_pInfo->csDataBits =tsz;
			break;
			case 5:
				m_pInfo->csParity =tsz;
			break;
			case 6:
				m_pInfo->csStopBits =tsz;
			break;
			case 7:
				m_pInfo->csComm = tsz;

			}			
		}
		tszLine = wcstok(NULL, _T(","));
		ci++;
	}		
	return;
}

void CDlgPTZSetting::vGetNODEITEM(NODEITEM *pNode)
{

	//memcpy(m_pNode,pNode,sizeof(NODEITEM));
	m_pNode = pNode;

}
void CDlgPTZSetting::vGetNODEITEM(PTZAuthInfo authinfo) // not used
{	memcpy(&m_ptzAuthInfo,&authinfo,sizeof(PTZAuthInfo));
	//m_IP=csIP;
	//m_Port = csPort;
	
	if(m_ptzAuthInfo.csCameraName[0]=='\0') wcscpy(m_ptzAuthInfo.csCameraName , _T("Untitled"));
	m_csCameraNameString.Format(_T("%s"),m_ptzAuthInfo.csCameraName);
	//AfxMessageBox(m_csCameraNameString);
}

void CDlgPTZSetting::vDisplayRS485Info()
{

	int nIndex=0;
	nIndex = m_combobox_protocol.FindStringExact(0,m_pInfo->csProtocol);
	m_combobox_protocol.SetCurSel(nIndex);
	if (nIndex==-1)
	{
		m_combobox_protocol.EnableWindow(FALSE);
	}
	nIndex = 0;

	m_edit_deviceid.SetWindowTextW(m_pInfo->csDeviceID);
	if (m_pInfo->csDeviceID.IsEmpty())
	{
		m_edit_deviceid.EnableWindow(FALSE);
	}
	
	nIndex = m_combobox_baudRate.FindStringExact(0,m_pInfo->csBraudRate);
	m_combobox_baudRate.SetCurSel(nIndex);
	if (nIndex==-1)
	{
		m_combobox_baudRate.EnableWindow(FALSE);
	}
	nIndex = 0;
	
	nIndex = m_combobox_data_bits.FindStringExact(0,m_pInfo->csDataBits);
	m_combobox_data_bits.SetCurSel(nIndex);
	if (nIndex==-1)
	{
		m_combobox_data_bits.EnableWindow(FALSE);
	}
	nIndex = 0;
	
	nIndex = m_combobox_parity.FindStringExact(0,m_pInfo->csParity);
	m_combobox_parity.SetCurSel(nIndex);
	if (nIndex==-1)
	{
		m_combobox_parity.EnableWindow(FALSE);
	}
	nIndex = 0;

	nIndex = m_combobox_stop_bits.FindStringExact(0,m_pInfo->csStopBits);
	m_combobox_stop_bits.SetCurSel(nIndex);
	if (nIndex==-1)
	{
		m_combobox_stop_bits.EnableWindow(FALSE);
	}
	nIndex = 0;
}


void CDlgPTZSetting::OnBnClickedBtnSetPatrol()
{
	CString csPatrol,csDuration,csPatrolSet;
	if (m_csPatrolPointSet.IsEmpty())
	{
		m_mapPatrolSeqDuration.clear();
		m_nPatrolSeqCount=1;
	}

	this->GetDlgItemTextW(IDC_EDIT_PATROL, csPatrol);
	this->GetDlgItemTextW(IDC_EDIT_PATROL_DURATION, csDuration);
	
	if(csPatrol.IsEmpty()) return;

	m_csPatrolPointSet += csPatrol;
	m_csPatrolPointSet += _T("(") + csDuration + _T(")");
	csPatrolSet=csPatrol+ _T("(") + csDuration + _T(")");
	m_csPatrolPointSet+=_T(", ");
	GetDlgItem(IDC_STATIC_PATROL_SHOW)->SetWindowTextW(m_csPatrolPointSet);
	GetDlgItem(IDC_EDIT_PATROL)->SetWindowTextW(_T(""));
	m_editPatrolDuration.SetWindowTextW(_T("3"));
	
	int nPatrol = _ttoi(csPatrol);	
	int nDuration = _ttoi(csDuration);
	pair_INT p(nPatrol,nDuration);	

	if(nPatrol >0)
	{
		m_mapPatrolSeqDuration.insert(pair<int,pair_INT>(m_nPatrolSeqCount,p));
	}
	m_nPatrolSeqCount++;
}

void CDlgPTZSetting::vSetPatrolSeqText(void)
{
	CString csPatrol,csDuration,csPatrolPointSet;
	//for(int i = 0;i<m_pInfo->pvctmapPatrolSeqDurSet->size();i++)
	//{
		map<int,pair_INT>::iterator it;
		//for(it = (*m_pInfo->pvctmapPatrolSeqDurSet)[i].begin(); it!=(*m_pInfo->pvctmapPatrolSeqDurSet)[i].end();it++)
		for(it = m_mapPatrolSeqDuration.begin(); it!=m_mapPatrolSeqDuration.end();it++)
		{
			csPatrol.Format(_T("%d"),it->second.first);
			csDuration.Format(_T("%d"),it->second.second);
			csPatrolPointSet += csPatrol;
			csPatrolPointSet += _T("(") + csDuration + _T(")");	
			csPatrolPointSet+=_T(", ");
		}
	//}
	GetDlgItem(IDC_STATIC_PATROL_SHOW)->SetWindowTextW(csPatrolPointSet);
}

void CDlgPTZSetting::OnBnClickedBtnResetPatrol()
{
	m_csPatrolPointSet.Empty();
	GetDlgItem(IDC_STATIC_PATROL_SHOW)->SetWindowTextW(m_csPatrolPointSet);

	//if (m_nPatrolSeqDurIdx==-1)
	//{
	//	return;
	//}

	int nIdx=0;
	BOOL bDelRegKey=0;
	if (m_mapPatrolSeqDuration.empty()==false)
	{
		m_mapPatrolSeqDuration.clear();

		//m_mapPatrolSeqDuration.clear();
		//for(vector <map <int,pair_INT>>::iterator it=m_pInfo->pvctmapPatrolSeqDurSet->begin(); it!=m_pInfo->pvctmapPatrolSeqDurSet->end();it++)
		//{
		//	if (nIdx==m_nPatrolSeqDurIdx)
		//	{
		//		m_pInfo->pvctmapPatrolSeqDurSet->erase(it);
		//		bDelRegKey=1;
		//		m_pNode->nPatrolSeqDurIdx=0;
		//		break;
		//	}			
		//	nIdx++;
		//}

		//for (int nModify=0;nModify<g_vctpAllNode.size();nModify++)
		//{
		//	//NODEITEM* pNode=;
		//	if (nIdx<g_vctpAllNode[nModify]->nPatrolSeqDurIdx && g_vctpAllNode[nModify]->nPatrolSeqDurIdx!=0)
		//	{				
		//		g_vctpAllNode[nModify]->nPatrolSeqDurIdx--;
		//	}
		//}

		//m_nPatrolSeqDurIdx=FindFreePatrolSeqDurIdx();
	}

	//if (!bDelRegKey)
	//{
	//	nIdx=-1;
	//}
	//CGS::bSetPatrolSeqDuration((*m_pInfo->pvctmapPatrolSeqDurSet),nIdx);

	//CGS::bSetPatrolSeqDuration(*m_pmapPatrolSeqDuration,m_pNode->mac);
}

//int CDlgPTZSetting::FindFreePatrolSeqDurIdx()
//{
//	int nPartolSeqIdx=0;
//	int nNodeTotal=g_vctpAllNode.size();
//		
//	for (int nFind=0;nFind<nNodeTotal;nFind++)
//	{	
//		BOOL bFind=0;
//		for (int nNode=0;nNode<nNodeTotal;nNode++)
//		{
//			if (g_vctpAllNode[nNode]->nPatrolSeqDurIdx==0)
//			{
//				continue;
//			}
//			else if (g_vctpAllNode[nNode]->nPatrolSeqDurIdx==nFind+1)
//			{
//				//int nIdx=g_vctpAllNode[nNode]->nPatrolSeqDurIdx-1;
//				//if(!m_pInfo->pvctmapPatrolSeqDurSet[nIdx].empty())
//				//{
//					nPartolSeqIdx++;
//					bFind=1;
//					break;
//				//}
//			}
//		}
//
//		if (!bFind)
//		{
//			break;
//		}
//	}
//	return nPartolSeqIdx;
//}

void CDlgPTZSetting::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//m_pmapPatrolSeqDuration->clear();

	OnCancel();
}
