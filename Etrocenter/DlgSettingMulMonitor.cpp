// DlgSettingMulMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSettingMulMonitor.h"
#include "DlgMatrixTag.h"
#include "MainFrm.h"

#define	TIMER_INIT		01

// CDlgSettingMulMonitor dialog

IMPLEMENT_DYNAMIC(CDlgSettingMulMonitor, CBaseDialog)

CDlgSettingMulMonitor::CDlgSettingMulMonitor(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSettingMulMonitor::IDD, pParent)
	, m_totMonitor(0),mb_Dirty(false),m_nMainFrameStartMonitorIdx(-1),m_nPlayBackStartMonitorIdx(-1)
{
	m_nMatrixCode=false;
}

CDlgSettingMulMonitor::~CDlgSettingMulMonitor()
{
}

void CDlgSettingMulMonitor::DoDataExchange(CDataExchange* pDX)
{
	//CBaseDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_STARTLIST, m_LCStartList);
	DDX_Control(pDX, IDB_MONITOR1, m_Monitor1);
	DDX_Control(pDX, IDC_BUTMATRIX, m_ButMatrix);
	DDX_Control(pDX, IDC_ST_DESC, m_StMatrix);
	DDX_Control(pDX, IDC_COMBO_EMAP_GROUP, m_combo_emap_group);
	DDX_Control(pDX, IDC_COMBO_GATE_CONTROL, m_combo_GateControl);
}


BEGIN_MESSAGE_MAP(CDlgSettingMulMonitor, CBaseDialog)
//	ON_BN_CLICKED(IDOK, &CDlgSettingMulMonitor::OnBnClickedOk)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTADDSTART, &CDlgSettingMulMonitor::OnBnClickedButaddstart)
//	ON_BN_CLICKED(IDC_BUTMON1, &CDlgSettingMulMonitor::OnBnClickedButmon1)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_RADIOLIVEVIEW, &CDlgSettingMulMonitor::OnBnClickedRadioliveview)
	ON_BN_CLICKED(IDC_RADIOMAIN, &CDlgSettingMulMonitor::OnBnClickedRadiomain)
	ON_WM_CTLCOLOR()
//	ON_BN_SETFOCUS(IDC_BUTMON1, &CDlgSettingMulMonitor::OnBnSetfocusButmon1)
	ON_BN_CLICKED(IDC_BUTMON1, &CDlgSettingMulMonitor::OnBnClickedButmon1)
	ON_BN_CLICKED(IDC_BUTMON2, &CDlgSettingMulMonitor::OnBnClickedButmon2)
	ON_BN_CLICKED(IDC_BUTMON3, &CDlgSettingMulMonitor::OnBnClickedButmon3)
	ON_BN_CLICKED(IDC_BUTMON4, &CDlgSettingMulMonitor::OnBnClickedButmon4)
	ON_BN_CLICKED(IDC_BUTMON5, &CDlgSettingMulMonitor::OnBnClickedButmon5)
	ON_BN_CLICKED(IDC_BUTMON6, &CDlgSettingMulMonitor::OnBnClickedButmon6)	
	//ON_BN_KILLFOCUS(IDC_BUTMON1, &CDlgSettingMulMonitor::OnBnKillfocusButmon1)
	ON_CBN_SELCHANGE(IDC_COMBOGROUP, &CDlgSettingMulMonitor::OnCbnSelchangeCombogroup)
	ON_BN_CLICKED(IDC_BUTMATRIX, &CDlgSettingMulMonitor::OnBnClickedButmatrix)
	ON_BN_CLICKED(IDC_RADIOPLAYBACK, &CDlgSettingMulMonitor::OnBnClickedRadioplayback)
	ON_BN_CLICKED(IDC_RADIOEVENT, &CDlgSettingMulMonitor::OnBnClickedRadioevent)
	ON_BN_CLICKED(IDC_RADIOEMAP, &CDlgSettingMulMonitor::OnBnClickedRadioemap)
	ON_BN_CLICKED(IDC_RADIONONE, &CDlgSettingMulMonitor::OnBnClickedRadionone)
	ON_CBN_SELCHANGE(IDC_COMBO_EMAP_GROUP, &CDlgSettingMulMonitor::OnCbnSelchangeComboEmapGroup)
	ON_BN_CLICKED(IDC_RADIO_ACCESS_CONTROL, &CDlgSettingMulMonitor::OnBnClickedRadioAccessControl)
	ON_BN_CLICKED(IDC_RADIO_GATE_CONTROL, &CDlgSettingMulMonitor::OnBnClickedRadioGateControl)
	ON_CBN_SELCHANGE(IDC_COMBO_GATE_CONTROL, &CDlgSettingMulMonitor::OnCbnSelchangeComboGateControl)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgSettingMulMonitor message handlers

BOOL CDlgSettingMulMonitor::OnInitDialog()
{
	CString ls_name;

	CBaseDialog::OnInitDialog();
	m_ini.setINIFullFileName(_T("\ec.ini"));
	CString csDialogCaption(_T(""));
	csDialogCaption.LoadString(CProductDefine::g_hResStr, IDS_DLG_MULTIMONITOR);
	this->SetWindowTextW(csDialogCaption);
	
	CString csMsg;
	LOADSTRING(csMsg,IDS_HINT_MONITORS);
	SetDlgItemText(IDC_ST_MONITORS,csMsg);

	LOADSTRING(csMsg,IDS_SETTING_MULTIMONITOR_STARTUP);
	GetDlgItem(IDC_STSTARTPROGRAM)->SetWindowTextW(csMsg);	
	
	LOADSTRING(csMsg,IDS_SETTING_MULTIMONITOR_MAIN);
	GetDlgItem(IDC_RADIOMAIN)->SetWindowTextW(csMsg);	
	
	LOADSTRING(csMsg,IDS_ADD_CAM_LIVEVIEW);
	GetDlgItem(IDC_RADIOLIVEVIEW)->SetWindowTextW(csMsg);

	LOADSTRING(csMsg,IDS_HINT_EMAP);
	GetDlgItem(IDC_RADIOEMAP)->SetWindowTextW(csMsg);
	
	LOADSTRING(csMsg,IDS_LIVEVIEW_GROUP);
	SetDlgItemText(IDC_STATIC_LVG,csMsg);

	LOADSTRING(csMsg,IDS_NONE);
	GetDlgItem(IDC_RADIONONE)->SetWindowTextW(csMsg);
	
	LOADSTRING(csMsg,IDS_HINT_PLAYBACK);
	GetDlgItem(IDC_RADIOPLAYBACK)->SetWindowTextW(csMsg);

	LOADSTRING(csMsg,IDS_AC);
	GetDlgItem(IDC_RADIO_ACCESS_CONTROL)->SetWindowTextW(csMsg);

	LOADSTRING(csMsg,IDS_GATE_CONTROL);
	GetDlgItem(IDC_RADIO_GATE_CONTROL)->SetWindowTextW(csMsg);


	m_curMonitor = 0;
	mb_Dirty = false; //Initial dirty check if changed

	m_Primonitor = CMonitors::GetPrimaryMonitor();
	m_Primonitor.GetName(ls_name);

	//Get Total Monitor	 
	//vDBConnect();
	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
	vInitList();

	m_ComboGroup = (CComboBox*)GetDlgItem(IDC_COMBOGROUP);
	// TODO:  Add extra initialization here
	//Populate Liveview group into ComboBox
	vBuildComboValue();

	// populate chain into combo only when ini set to Y
	CString ls_gate = m_ini.getKeyValue(_T("parking_gate"),_T("parking gate"));
	if (ls_gate==_T("Y"))
	{
		GetDlgItem(IDC_RADIO_GATE_CONTROL)->ShowWindow(true);
		m_combo_GateControl.ShowWindow(false);
		GetDlgItem(IDC_STATIC_GATE_CHAIN)->ShowWindow(false);
		//vBuildGateComboValue();
	}
	else //Hide Parking Gate Related fields
	{
		m_combo_GateControl.ShowWindow(false);
		GetDlgItem(IDC_RADIO_GATE_CONTROL)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_GATE_CHAIN)->ShowWindow(false);
	}
	// Show access control radio button only when ini set to Y
	CString csAccess = m_ini.getKeyValue(_T("access_control"),_T("access control"));

	csAccess==_T("Y") ? GetDlgItem(IDC_RADIO_ACCESS_CONTROL)->ShowWindow(true) : GetDlgItem(IDC_RADIO_ACCESS_CONTROL)->ShowWindow(false);


	vSetRadioFocus(0);
	//vMonitorChanged(0);
	OnBnClickedButmon1();

	HideEmapOption();
	
	SetTimer(TIMER_INIT, 300, 0);
	return TRUE;  // return TRUE unless you set the focus to a control

}

void CDlgSettingMulMonitor::HideEmapOption()
{
	if (CNVRBk::GetBkObject()->isNVRType())
	{
		GetDlgItem(IDC_RADIOEMAP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_EMAP_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_EMAP_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_ACCESS_CONTROL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_GATE_CONTROL)->ShowWindow(SW_HIDE);
		GetDlgItem(10407)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_GATE_CONTROL)->ShowWindow(SW_HIDE);
	}
}

//Get values from ec_monitor and build into structure
void CDlgSettingMulMonitor::vInitList(void)
{
	USES_CONVERSION;
	CString ls_primary, ls_app, ls_target, ls_matrix;
	CDBVariant varValue, varGroupid;
	int  ci=0, ll_groupid, ll_monitorid;
	
	STRMONITOR lMon={0};	 
 
	//Get Group List from ec_camera_group
	CRecordset m_rs(pDB);
	m_rs.Open(CRecordset::dynaset,_T("select * from ec_monitor order by monitorid"));
	int ll_monitorIdx=0; 

	while (!m_rs.IsEOF()) { 	
		
		m_rs.GetFieldValue(_T("monitorid"), varValue, SQL_C_SSHORT);
		ll_monitorid = varValue.m_iVal;
		lMon.monitorid = ll_monitorid;
		

		m_rs.GetFieldValue(_T("primary_"), ls_primary);
		lMon.primary_ = ls_primary.Trim();		
		
		
		m_rs.GetFieldValue(_T("cameragroupid"), varGroupid, SQL_C_SSHORT);
		lMon.camgroupid = varGroupid.m_iVal;
		

 		m_rs.GetFieldValue(_T("startup_app"), ls_app);
		lMon.startupapp = ls_app.Trim();
		
		
		m_rs.GetFieldValue(_T("startup_target"), ls_target.Trim());
		lMon.target = ls_target.Trim();
		

		m_rs.GetFieldValue(_T("matrixcode"), ls_matrix);
		lMon.matrixcode = ls_matrix.Trim();

		ma_Monitor.push_back(lMon);
		ci++;
		ll_monitorIdx++;
		m_rs.MoveNext();  		
	} 

	
	

	//Insert Rows into ec_monitor if total monitor > rows in ec_monitor
	CString ls_sql;
	
	try 
	{
	 while (m_totMonitor >ci)
	 {
 		ls_sql.Format(_T("INSERT INTO ec_monitor (monitorid, primary_, startup_app) VALUES (%d , '%s', '%s')"), ci+1, _T("N"), _T(""));				
		pDB->ExecuteSQL(ls_sql);
		lMon.monitorid = ci+1;
		lMon.startupapp = _T("NULL");
		ma_Monitor.push_back(lMon);

		ci++;
	 }
	} 
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
		return ;
	}


	
	
}


//int CDlgSettingMulMonitor::vDBConnect(void)
//{
//	//Database m_DB - 
//	CIniReader m_ini(ECINI);
//	//char* cpDns = m_ini.getKeyValue(_T("DSN"),_T("database"));
//	//char* cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
//	//char* cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
// 
//	CString cpDns = m_ini.getKeyValue(_T("DSN"),_T("database")); 
//	CString cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
//	CString cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
//
//	CString ls_connect;
//	ls_connect.Format(_T("ODBC;UID=%s;PWD=%s"), cpUid, cpPwd);
//	bool lb_ret = m_DB.Open((LPCTSTR)cpDns, FALSE, FALSE, ls_connect );
//	if (!lb_ret) return -1;
//	return 0;
//}


int CDlgSettingMulMonitor::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	int ll_x=350, ll_y = 100, ll_width=220, ll_h=225 ;
	int ll_x1 = 40, ll_y1=25, ll_y2= 40, ll_w1=72, ll_h1=72;

	////Start List
	//DWORD ldwStyle = WS_VISIBLE |LVS_SMALLICON | WS_BORDER | LVS_OWNERDRAWFIXED |LVS_EX_HEADERDRAGDROP;
	//m_LCStartList.Create(ldwStyle, CRect(ll_x, ll_y, ll_x+ ll_width, ll_y+ll_h), this, IDC_STARTLIST);
	//m_LCStartList.SetBkColor(GLOBALTREEBKCOLOR);


	DWORD ldwStyle = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW;
	//m_Arrow.Create(NULL, ldwStyle, CRect(ll_x - 60, ll_y+50, ll_x-12, ll_y+98), this, IDC_BUTADDSTART);
	//m_Arrow.LoadBitmaps(IDB_ARROW_LEFT, IDB_ARROW_LEFT_SEL, IDB_ARROW_LEFT_SEL, IDB_ARROW_LEFT);//, IDB_EMAP,IDB_EMAP,IDB_EMAP);

	//Create Matrix button & Bitmaps
	m_ButMatrix.Create(NULL, ldwStyle, CRect(395, 155, 427 , 187), this, IDC_BUTMATRIX);
	m_ButMatrix.LoadBitmaps(IDB_MATRIX32, IDB_MATRIX32, IDB_MATRIX32, IDB_MATRIX32);

	//Create Matrix Description
	m_StMatrix.Create(_T(""), ldwStyle,  CRect(440, 165, 732 , 185), this, IDC_ST_DESC);


	m_totMonitor = CMonitors::GetMonitorCount();
	//m_totMonitor = 3;
	if (m_totMonitor>6)
		AfxMessageBox(IDS_MAX_MONITOR);
		//AfxMessageBox(_T("EtroCenter only support up to 6 monitors in this version"));

	m_Monitor1.Create(NULL, ldwStyle, CRect(ll_x1, ll_y2, ll_x1+ll_w1, ll_y2+ll_h1), this, IDC_BUTMON1);
	m_Monitor1.LoadBitmaps(IDB_MONITOR1, IDB_MONITOR1_SEL, IDB_MONITOR1_SEL, IDB_MONITOR1);

	if (m_totMonitor==1) m_Monitor1.SetState(true);

	if (m_totMonitor>=2)
	{
		ll_x1 = ll_x1 + 80;
		m_Monitor2.Create(NULL, ldwStyle, CRect(ll_x1, ll_y2, ll_x1+ll_w1, ll_y2 + ll_h1), this, IDC_BUTMON2);
		m_Monitor2.LoadBitmaps(IDB_MONITOR2, IDB_MONITOR2_SEL, IDB_MONITOR2_SEL, IDB_MONITOR2);
	}

	if (m_totMonitor>=3)
	{
		ll_x1 = ll_x1 + 80;
		m_Monitor3.Create(NULL, ldwStyle, CRect(ll_x1, ll_y2, ll_x1+ll_w1, ll_y2 + ll_h1), this, IDC_BUTMON3);
		m_Monitor3.LoadBitmaps(IDB_MONITOR3, IDB_MONITOR3_SEL, IDB_MONITOR3_SEL, IDB_MONITOR3);
	}

	if (m_totMonitor>=4)
	{
		ll_x1 = ll_x1 + 80;
		m_Monitor4.Create(NULL, ldwStyle, CRect(ll_x1, ll_y2, ll_x1+ll_w1, ll_y2 + ll_h1), this, IDC_BUTMON4);
		m_Monitor4.LoadBitmaps(IDB_MONITOR4, IDB_MONITOR4_SEL, IDB_MONITOR4_SEL, IDB_MONITOR4);
	}

	if (m_totMonitor>=5)
	{
		ll_x1 = ll_x1 + 80;
		m_Monitor5.Create(NULL, ldwStyle, CRect(ll_x1, ll_y2, ll_x1+ll_w1, ll_y2 + ll_h1), this, IDC_BUTMON5);
		m_Monitor5.LoadBitmaps(IDB_MONITOR5, IDB_MONITOR5_SEL, IDB_MONITOR5_SEL, IDB_MONITOR5);
	}

	if (m_totMonitor>=6)
	{
		ll_x1 = ll_x1 + 80;
		m_Monitor6.Create(NULL, ldwStyle, CRect(ll_x1, ll_y2, ll_x1+ll_w1, ll_y2 + ll_h1), this, IDC_BUTMON6);
		m_Monitor6.LoadBitmaps(IDB_MONITOR6, IDB_MONITOR6_SEL, IDB_MONITOR6_SEL, IDB_MONITOR6);
	}
	// TODO:  Add your specialized creation code here	
	return 0;
}

void CDlgSettingMulMonitor::OnBnClickedButaddstart()
{
	//AfxMessageBox(_T("click me"));
	// TODO: Add your control notification handler code here
}

void CDlgSettingMulMonitor::vArrangeMonitor(void)
{
	//m_totMonitor is the total monitor

}

BOOL CDlgSettingMulMonitor::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

void CDlgSettingMulMonitor::OnBnClickedRadioliveview()
{
	vResetMainPlaybackMonitorIdx();
	mb_Dirty = true;
	((CComboBox*)GetDlgItem(IDC_COMBOGROUP))->EnableWindow(true);
	m_combo_emap_group.EnableWindow(false);
	vUpdateStruct(LIVEVIEW);	 

	//Enable button
	(CButton*)GetDlgItem(IDC_BUTMATRIX)->EnableWindow(1);
	//ma_Monitor[m_curMonitor].startupapp[9] ='\0';


	//Get right Matrix Code
	CString ls_desc(ma_Monitor[m_curMonitor].matrixcode);
	m_StMatrix.SetWindowTextW(_T("          "));
	m_StMatrix.SetWindowTextW(csGetMatrixCode(_ttoi(ls_desc)));

	vResetMonitorFocus();
}

void CDlgSettingMulMonitor::vBuildComboValue(void)
{
	int ci=0;
	CString	ls_desc;

	CDBVariant	varID;

	m_ComboGroup = (CComboBox*)GetDlgItem(IDC_COMBOGROUP);
	CRecordset m_rsGroup(pDB);
	m_rsGroup.Open(CRecordset::dynaset,_T("SELECT cameragroupid, camera_group_desc, module_code, sort_order FROM ec_camera_group WHERE module_code='LIVEVIEW' ORDER BY sort_order"));
	//int ll_rows = m_rsGroup.GetRecordCount();

	m_rsGroup.MoveFirst();

	while(!m_rsGroup.IsEOF() ) 
	{ 
		m_rsGroup.GetFieldValue(_T("cameragroupid"), varID, SQL_C_SSHORT);

		m_rsGroup.GetFieldValue(_T("camera_group_desc"), ls_desc);
 		m_ComboGroup->InsertString(ci, ls_desc);
		m_combo_emap_group.InsertString(ci, ls_desc);
		m_vctGroupId.push_back(varID.m_iVal);
		ci++;
		m_rsGroup.MoveNext();
		
	}
	m_rsGroup.Close();
 
}
void CDlgSettingMulMonitor::vBuildGateComboValue(void)
{
	int ci=0;
	CString	ls_desc;
	CDBVariant	varID;
	CRecordset m_rsGroup(pDB);
	m_rsGroup.Open(CRecordset::dynaset,_T("SELECT chainid, chain_desc FROM ecgt_chain"));
	//int ll_rows = m_rsGroup.GetRecordCount();
	//m_rsGroup.MoveFirst();

	while(!m_rsGroup.IsEOF() ) 
	{ 
		m_rsGroup.GetFieldValue(_T("chainid"), varID, SQL_C_SSHORT);
		m_rsGroup.GetFieldValue(_T("chain_desc"), ls_desc);
 		m_combo_GateControl.InsertString(ci, ls_desc);
		m_vctChainId.push_back(varID.m_iVal);
		ci++;
		m_rsGroup.MoveNext();
		
	}
	m_rsGroup.Close();
}
void CDlgSettingMulMonitor::OnBnClickedRadiomain()
{
	m_nMainFrameStartMonitorIdx = m_curMonitor ;
	// clear current monitor config
	ma_Monitor[m_curMonitor].startupapp.Empty();
	if(m_curMonitor == m_nPlayBackStartMonitorIdx)
	{
		m_nPlayBackStartMonitorIdx = -1;
	}
	vRadioClick(MAINCONSOLE);
	
}

HBRUSH CDlgSettingMulMonitor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return m_SilverBrush;
}

 

void CDlgSettingMulMonitor::vSetRadioFocus(int nIndex)
{
	CString ls_app;
	char*	lcApp;
	bool	lb_group=false;
	int		ll_groupid=0;

	//uncheck all radios	
	//((CButton*) GetDlgItem(IDC_RADIOMAIN)->SetCheck(BST_UNCHECKED); 		
	((CButton*) GetDlgItem(IDC_RADIOLIVEVIEW))->SetCheck(BST_UNCHECKED); 		
	((CButton*) GetDlgItem(IDC_RADIOEMAP))->SetCheck(BST_UNCHECKED); 
	//((CButton*) GetDlgItem(IDC_RADIOPLAYBACK))->SetCheck(BST_UNCHECKED); 
	((CButton*) GetDlgItem(IDC_RADIOEVENT))->SetCheck(BST_UNCHECKED); 	
	((CButton*) GetDlgItem(IDC_RADIONONE))->SetCheck(BST_UNCHECKED); 
	((CButton*) GetDlgItem(IDC_RADIO_ACCESS_CONTROL))->SetCheck(BST_UNCHECKED); 
	((CButton*) GetDlgItem(IDC_RADIO_GATE_CONTROL))->SetCheck(BST_UNCHECKED); 
	
	m_StMatrix.SetWindowTextW(_T("          ")) ;


	if (ma_Monitor[nIndex].startupapp.Trim()== LIVEVIEW)
	//if (strncmp(ma_Monitor[o_index].startupapp, LIVEVIEW, 8)==0)
	{
		((CButton*) GetDlgItem(IDC_RADIOLIVEVIEW))->SetCheck(BST_CHECKED);
		(CButton*)GetDlgItem(IDC_BUTMATRIX)->EnableWindow(1);
		lb_group=true;
		int ll_sel=0;

		//Select the right group in the combobox
		ll_groupid = ma_Monitor[nIndex].camgroupid;
		for (int i=0; i<m_vctGroupId.size(); i++)
		{
			if (m_vctGroupId[i] == ll_groupid)
			{
				ll_sel = i;
				break;
			}
		}

		m_ComboGroup->SetCurSel(ll_sel);
		m_ComboGroup->EnableWindow(true);
		//Get right Matrix Code
		CString ls_desc(ma_Monitor[nIndex].matrixcode);
		m_StMatrix.SetWindowTextW(_T("          ")) ;
		m_StMatrix.SetWindowTextW(csGetMatrixCode(_ttoi(ls_desc))) ;
		m_nMatrixCode=_ttoi(ls_desc);
	}
	else if (ma_Monitor[nIndex].startupapp.Trim()== EMAP)
	//else if (strncmp(ma_Monitor[o_index].startupapp, EMAP, 4)==0) //Emap
	{
		int ll_sel(0);
		int ll_groupid = ma_Monitor[nIndex].camgroupid;
		for (int i=0; i<m_vctGroupId.size(); i++)
		{
			if (m_vctGroupId[i] == ll_groupid)
			{
				ll_sel = i;
				break;
			}
		}
		m_combo_emap_group.SetCurSel(ll_sel);
		m_combo_emap_group.EnableWindow(true);
		((CButton*) GetDlgItem(IDC_RADIOEMAP))->SetCheck(BST_CHECKED); 
	}
	else if (ma_Monitor[nIndex].startupapp.Trim()==PLAYBACK)
	{
		OnBnClickedRadioplayback();
	}
	else if (ma_Monitor[nIndex].startupapp.Trim()== EVENTM)
	{
		
	}
	else if (ma_Monitor[nIndex].startupapp.Trim()== MAINCONSOLE)
	{
		OnBnClickedRadiomain();
	}
	else if (ma_Monitor[nIndex].startupapp.Trim()== ACCESSCONTROL)
	{
		((CButton*) GetDlgItem(IDC_RADIO_ACCESS_CONTROL))->SetCheck(BST_CHECKED); 
	}
	else if (ma_Monitor[nIndex].startupapp.Trim()== GATECONTROL)
	{
		int ll_sel(0);
		int ll_groupid = ma_Monitor[nIndex].camgroupid;
		for (int i=0; i<m_vctChainId.size(); i++)
		{
			if (m_vctChainId[i] == ll_groupid)
			{
				ll_sel = i;
				break;
			}
		}
		m_combo_GateControl.SetCurSel(ll_sel);
		m_combo_GateControl.EnableWindow(true);
		((CButton*) GetDlgItem(IDC_RADIO_GATE_CONTROL))->SetCheck(BST_CHECKED); 
	}
	else
	{
		((CButton*) GetDlgItem(IDC_RADIONONE))->SetCheck(BST_CHECKED); 
	}

	if(m_nMainFrameStartMonitorIdx == nIndex)// Check main radio if monitor idx hits
	{
		((CButton*) GetDlgItem(IDC_RADIOMAIN))->SetCheck(BST_CHECKED);
		((CButton*) GetDlgItem(IDC_RADIONONE))->SetCheck(BST_UNCHECKED);
	}

	if(m_nPlayBackStartMonitorIdx == nIndex)// Check playback radio if monitor idx hits
	{
		((CButton*) GetDlgItem(IDC_RADIOPLAYBACK))->SetCheck(BST_CHECKED);
		((CButton*) GetDlgItem(IDC_RADIONONE))->SetCheck(BST_UNCHECKED);
	}

	((CComboBox*)GetDlgItem(IDC_COMBOGROUP))->EnableWindow(lb_group);
}
void CDlgSettingMulMonitor::OnBnClickedButmon1()
{
	vMonitorChanged(0);
}

void CDlgSettingMulMonitor::OnBnClickedButmon2(){

	vMonitorChanged(1);
}

void CDlgSettingMulMonitor::OnBnClickedButmon3()
{
	vMonitorChanged(2);
}

void CDlgSettingMulMonitor::OnBnClickedButmon4()
{	
	vMonitorChanged(3);
}
void CDlgSettingMulMonitor::OnBnClickedButmon5()
{
	vMonitorChanged(4);
}
void CDlgSettingMulMonitor::OnBnClickedButmon6()
{
	vMonitorChanged(5);
}

void CDlgSettingMulMonitor::vMonitorChanged(int nMonitor)
{	
	m_curMonitor = nMonitor;
	// set radio main checked status 
	m_curMonitor == m_nMainFrameStartMonitorIdx  ?	((CButton*) GetDlgItem(IDC_RADIOMAIN))->SetCheck(BST_CHECKED):((CButton*) GetDlgItem(IDC_RADIOMAIN))->SetCheck(BST_UNCHECKED);		
	// set radio playback checked status
	m_curMonitor == m_nPlayBackStartMonitorIdx ? ((CButton*) GetDlgItem(IDC_RADIOPLAYBACK))->SetCheck(BST_CHECKED):((CButton*) GetDlgItem(IDC_RADIOPLAYBACK))->SetCheck(BST_UNCHECKED);	
	
	vSetRadioFocus(nMonitor);
}

void CDlgSettingMulMonitor::OnCbnSelchangeCombogroup()
{	
	mb_Dirty = true;
	vResetMonitorFocus();
	//On group selection changed, need to get the proper group
	int li_groupid=0;
	int nCurSel = m_ComboGroup->GetCurSel();
	if(nCurSel>m_vctGroupId.size()-1) nCurSel = 0;
	li_groupid = m_vctGroupId[nCurSel];
	ma_Monitor[m_curMonitor].camgroupid = li_groupid;
	ma_Monitor[m_curMonitor].dirty_ = _T("Y");
}

void CDlgSettingMulMonitor::OnOK()
{
	if(m_nMainFrameStartMonitorIdx != -1 && m_nMainFrameStartMonitorIdx < ma_Monitor.size())
	{
			ma_Monitor[m_nMainFrameStartMonitorIdx].dirty_ = _T("Y");
			ma_Monitor[m_nMainFrameStartMonitorIdx].startupapp = MAINCONSOLE;	
			ma_Monitor[m_nMainFrameStartMonitorIdx].camgroupid = -1;
			ma_Monitor[m_nMainFrameStartMonitorIdx].matrixcode = _T("");
	}
	if(m_nPlayBackStartMonitorIdx != -1 && m_nPlayBackStartMonitorIdx < ma_Monitor.size())
	{
			ma_Monitor[m_nPlayBackStartMonitorIdx].dirty_ = _T("Y");
			ma_Monitor[m_nPlayBackStartMonitorIdx].startupapp = PLAYBACK;	
			ma_Monitor[m_nPlayBackStartMonitorIdx].camgroupid = -1;
			ma_Monitor[m_nPlayBackStartMonitorIdx].matrixcode = _T("");
	}
	if (mb_Dirty)
	{
		vSaveDB();
	}

	CBaseDialog::OnOK();
}

void CDlgSettingMulMonitor::OnBnClickedButmatrix()
{
	char*	lpc;	
	
	USES_CONVERSION;	
	CDlgMatrixTag *lMatrixDlg = new CDlgMatrixTag;
	lMatrixDlg->m_bSetStyle = true;
	lMatrixDlg->m_lRet = m_nMatrixCode;
	
	//Open Matrix Dialog
	int lret = lMatrixDlg->DoModal();

	vResetMonitorFocus();
	if (lret==2) return; //Escape key

 	
	if (lMatrixDlg->m_lRet>0 && lret!=2) //Click on one of the Matrix
	{
		mb_Dirty = true;
		
		CString csMatrix,csAutoScan;

		csAutoScan = _T("N");
		if(((CMainFrame*)AfxGetMainWnd())->m_bAutoScan)
		{
			csAutoScan = _T("Y");
		}
		//csMatrix.Format(_T("%d"),lret);
		ma_Monitor[m_curMonitor].dirty_ = _T("Y");
		//ma_Monitor[m_curMonitor].matrixcode = csMatrix;
		ma_Monitor[m_curMonitor].primary_ = csAutoScan; // used for autoscan
		m_StMatrix.SetWindowTextW(_T("                             "));
		m_StMatrix.SetWindowTextW(csGetMatrixCode(lMatrixDlg->m_lRet));
		m_nMatrixCode=lMatrixDlg->m_lRet;

		csMatrix.Format(L"%d", m_nMatrixCode);
		ma_Monitor[m_curMonitor].matrixcode=csMatrix;
	}
	
}

CString CDlgSettingMulMonitor::csGetMatrixCode(int lret)
{
	CString csMatrix;
	switch(lret)
	{
	case 1:
		csMatrix = _T("1 X 1");
		break;
	case 22:
		csMatrix = _T("2 X 2");
		break;
	case 3:
		csMatrix = _T("3 X 3");
		break;
	case 4:
		csMatrix = _T("4 X 4");
		break;
	case 6:
		csMatrix = _T("6 X 6");
		break;
	case 7:
		csMatrix = _T("7 X 7");
		break;
	case 8:
		csMatrix = _T("8 X 8");
		break;
	case 15:
		csMatrix = _T("1 + 5");
		break;
	case 28:
		csMatrix = _T("2 + 8");
		break;
	case 121:
		csMatrix = _T("1 + 12");
		break;
	case 122:
		csMatrix = _T("1 + 12");
		break;
	case 116:
		csMatrix = _T("1 + 16");
		break;
	case 218:
		csMatrix = _T("2 + 18");
		break;
	case 232:
		csMatrix = _T("2 + 32");
		break;
	}
	return csMatrix;
	

}

bool CDlgSettingMulMonitor::vSaveDB(void)
{

	char* lpPrimary;
	CString ls_sql,  ls_matrix;
	 USES_CONVERSION;

	//Save the modified into DB
	 for (int i=0; i<ma_Monitor.size(); i++)
	{
		//if (strncmp(ma_Monitor[i].dirty_, YES, 1)!=0) continue;
		if (ma_Monitor[i].dirty_!=_T("Y")) continue;
		CString ls_primary(ma_Monitor[i].primary_);
		CString ls_app(ma_Monitor[i].startupapp);

		try {
			ls_sql.Format(_T("UPDATE ec_monitor SET primary_='%s', cameragroupid=%d,  startup_app='%s', matrixcode='%s'  WHERE monitorid = %d"),  ls_primary , ma_Monitor[i].camgroupid, ls_app, CString(ma_Monitor[i].matrixcode), ma_Monitor[i].monitorid);						
			TRACE(ls_sql);
			pDB->ExecuteSQL(ls_sql);
		}
		catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
			return false;
		}	
	}
	return true;
}

void CDlgSettingMulMonitor::OnBnClickedRadioplayback()
{	
	m_nPlayBackStartMonitorIdx = m_curMonitor;
	// clear current monitor config
	ma_Monitor[m_curMonitor].startupapp.Empty();
	if(m_nMainFrameStartMonitorIdx == m_curMonitor)
	{
		m_nMainFrameStartMonitorIdx = -1;
	}
	vRadioClick(PLAYBACK);
}

void CDlgSettingMulMonitor::OnBnClickedRadioevent()
{
	vResetMainPlaybackMonitorIdx();
	vUpdateStruct(EVENTM); 
	vRadioClick(EVENTM);
}

void CDlgSettingMulMonitor::OnBnClickedRadioemap()
{
	vResetMainPlaybackMonitorIdx();
	vUpdateStruct( EMAP);
	vRadioClick(EMAP);
	m_combo_emap_group.EnableWindow(true);
}


void CDlgSettingMulMonitor::vRadioClick(CString o_Mode)
{
	int ll_size;
	CString lsNull=_T("         ");
	const CString lcs;
	char* lpcMode;
	char* lpcNull;

	vResetMonitorFocus();
	mb_Dirty = true;	
	o_Mode.GetBuffer();
	ll_size =  o_Mode.GetLength();
	lpcMode = (char*) o_Mode.GetBuffer(ll_size) ;
	lpcNull = (char*) lsNull.GetBuffer(10) ;
	 
 	m_ComboGroup->EnableWindow(false);
	m_combo_emap_group.EnableWindow(false);
	m_combo_GateControl.EnableWindow(false);

	ma_Monitor[m_curMonitor].dirty_=YES;
	//ma_Monitor[m_curMonitor].matrixcode=_T("");	
	
	//Disable Button
	(CButton*)GetDlgItem(IDC_BUTMATRIX)->EnableWindow(0);

	m_StMatrix.SetWindowTextW(lsNull) ;
}

void CDlgSettingMulMonitor::vResetMonitorFocus(void)
{
	switch (m_curMonitor)
	{
		case 0:
			(CBitmapButton*)GetDlgItem(IDC_BUTMON1)->SetFocus();
			break;

		case 1:
			(CBitmapButton*)GetDlgItem(IDC_BUTMON2)->SetFocus();
			break;

		case 2:
			(CBitmapButton*)GetDlgItem(IDC_BUTMON3)->SetFocus();
			break;

		case 3:
			(CBitmapButton*)GetDlgItem(IDC_BUTMON4)->SetFocus();
			break;

		case 4:
			(CBitmapButton*)GetDlgItem(IDC_BUTMON5)->SetFocus();
			break;

		case 5:
			(CBitmapButton*)GetDlgItem(IDC_BUTMON6)->SetFocus();
			break;
	}
}

bool CDlgSettingMulMonitor::vUpdateStruct(CString osMode)
{
	if (m_curMonitor<0) return false;

	ma_Monitor[m_curMonitor].dirty_ = _T("Y");
	ma_Monitor[m_curMonitor].startupapp = osMode;
	
	if (osMode!=LIVEVIEW && osMode != EMAP && osMode != GATECONTROL )
	{
		ma_Monitor[m_curMonitor].camgroupid = -1;
		//ma_Monitor[m_curMonitor].matrixcode = _T("");
	}
	else if(osMode == LIVEVIEW)
	{
		// To prevent the group id would be -1 if the group combo box not been selected before save 
		int li_groupid=0;
		int nCurSel = m_ComboGroup->GetCurSel();
		if(nCurSel > (m_vctGroupId.size()-1) ) nCurSel = 0;
		li_groupid = m_vctGroupId[nCurSel];
		ma_Monitor[m_curMonitor].camgroupid = li_groupid;
	}
	else if(osMode == EMAP)
	{
		// To prevent the group id would be -1 if the group combo box not been selected before save 
		int li_groupid=0;
		int nCurSel = m_combo_emap_group.GetCurSel();
		if(nCurSel > (m_vctGroupId.size()-1) ) nCurSel = 0;
		li_groupid = m_vctGroupId[nCurSel];
		ma_Monitor[m_curMonitor].camgroupid = li_groupid;
	}
	else if(osMode == GATECONTROL)
	{
		// To prevent the group id would be -1 if the group combo box not been selected before save 
		int li_groupid=0;
		int nCurSel = m_combo_GateControl.GetCurSel();
		if(nCurSel > (m_vctGroupId.size()-1) ) nCurSel = 0;
		li_groupid = m_vctGroupId[nCurSel];
		ma_Monitor[m_curMonitor].camgroupid = li_groupid;
	}
		

	return true;
}

void CDlgSettingMulMonitor::OnBnClickedRadionone()
{
	vResetMainPlaybackMonitorIdx();
	mb_Dirty = true;
	vResetMonitorFocus();
	vUpdateStruct(NONE);
	vRadioClick(NONE);
	//m_ComboGroup->EnableWindow(false);
	//m_combo_emap_group.EnableWindow(false);
}	

void CDlgSettingMulMonitor::OnCbnSelchangeComboEmapGroup()
{
	mb_Dirty = true;
	vResetMonitorFocus();
	//On group selection changed, need to get the proper group
	int li_groupid=0;
	int nCurSel = m_combo_emap_group.GetCurSel();
	if(nCurSel>m_vctGroupId.size()-1) nCurSel = 0;
	li_groupid = m_vctGroupId[nCurSel];
	ma_Monitor[m_curMonitor].camgroupid = li_groupid;
	ma_Monitor[m_curMonitor].dirty_ = _T("Y");

}

void CDlgSettingMulMonitor::OnBnClickedRadioAccessControl()
{
	vResetMainPlaybackMonitorIdx();
	vUpdateStruct(ACCESSCONTROL); 
	vRadioClick(ACCESSCONTROL);
}

void CDlgSettingMulMonitor::OnBnClickedRadioGateControl()
{
	vResetMainPlaybackMonitorIdx();
	vUpdateStruct(GATECONTROL); 
	vRadioClick(GATECONTROL);
	m_combo_GateControl.EnableWindow(true);
}


void CDlgSettingMulMonitor::OnCbnSelchangeComboGateControl()
{
	mb_Dirty = true;
	vResetMonitorFocus();
	//On group selection changed, need to get the proper group
	int nChainId(0);
	int nCurSel = m_combo_emap_group.GetCurSel();
	if(nCurSel > m_vctChainId.size()-1) nCurSel = 0;
	nChainId = m_vctChainId[nCurSel];
	ma_Monitor[m_curMonitor].camgroupid = nChainId;
	ma_Monitor[m_curMonitor].dirty_ = _T("Y");
}

void CDlgSettingMulMonitor::vResetMainPlaybackMonitorIdx(void)
{
	if(m_curMonitor == m_nPlayBackStartMonitorIdx)
	{
		m_nPlayBackStartMonitorIdx = -1;
	}
	if(m_nMainFrameStartMonitorIdx == m_curMonitor)
	{
		m_nMainFrameStartMonitorIdx = -1;
	}
}

void CDlgSettingMulMonitor::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TIMER_INIT)
	{
		KillTimer(nIDEvent);

		vResetMonitorFocus();
	}

	CBaseDialog::OnTimer(nIDEvent);
}
