// DlgSettingRecording.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSettingRecording.h"
#include "MainFrm.h"

#define WHITECOLOR RGB(255,255,255)


#define	TIMER_UPDB_ec_storage		1


// CDlgSettingRecording dialog

IMPLEMENT_DYNAMIC(CDlgSettingRecording, CDialog)

CDlgSettingRecording::CDlgSettingRecording(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSettingRecording::IDD, pParent), DirtyFlag(false), bShowFlag(false), bMon(false), bTue(false), bWed(false), bThu(false), bFri(false), bSat(false), bSun(false), m_bBufferFlag(false), m_iInitFlag(0),m_DlgRecRecord(NULL)
{
	m_SilverBrush.CreateSolidBrush(GLOBALBKCOLOR);
	m_WhiteBrush.CreateSolidBrush(WHITECOLOR);	

	::ZeroMemory(&byFromTimeLine, sizeof(byFromTimeLine));
	::ZeroMemory(&bySQLByte, sizeof(bySQLByte));
	::ZeroMemory(&byConvertByte, sizeof(byConvertByte));

//	vDBConnect();
}

CDlgSettingRecording::~CDlgSettingRecording()
{
	//delete m_CRecordTimeLine;
	//delete m_DlgRecRecord;
	//DBQuery.Close();
}

void CDlgSettingRecording::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PATH, pPathList);
	DDX_Control(pDX, IDC_TREE_CAMERA_LIST, pRECCameraTreeCtrl);
	DDX_Control(pDX, IDC_EDIT_RECORDING_CAMERA_NAME, pRECCameraNameEdit);
	//DDX_Control(pDX, IDC_TAB_RECORDING, pRecordingTab);
	//DDX_LBString(pDX, IDC_LIST_PATH, csStoragePath);
	//DDV_MaxChars(pDX, csStoragePath, 20);
}


BEGIN_MESSAGE_MAP(CDlgSettingRecording, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PATH, &CDlgSettingRecording::OnBnClickedButtonAddPath)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_PATH, &CDlgSettingRecording::OnBnClickedButtonDeletePath)
	ON_BN_CLICKED(IDOK, &CDlgSettingRecording::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_RECORDING, &CDlgSettingRecording::OnTcnSelchangeTabRecording)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CAMERA_LIST, &CDlgSettingRecording::OnTvnSelchangedTreeCameraList)
	ON_BN_CLICKED(IDC_BUTTON_DUPLICATE, &CDlgSettingRecording::OnBnClickedButtonDuplicate)
	ON_BN_CLICKED(IDC_BUTTON_RECORDING_SAVE, &CDlgSettingRecording::OnBnClickedButtonRecordingSave)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_PATH, &CDlgSettingRecording::OnBnClickedButtonModifyPath)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
//	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PATH, &CDlgSettingRecording::OnNMDblclkListPath)
ON_WM_LBUTTONDBLCLK()
ON_NOTIFY(NM_DBLCLK, IDC_LIST_PATH, &CDlgSettingRecording::OnNMDblclkListPath)
ON_NOTIFY(NM_CLICK, IDC_LIST_PATH, &CDlgSettingRecording::OnNMClickListPath)
ON_EN_KILLFOCUS(IDC_EDIT_PATH_LIST, &CDlgSettingRecording::OnEnKillfocusEditPathList)
ON_BN_CLICKED(IDCANCEL, &CDlgSettingRecording::OnBnClickedCancel)
ON_BN_CLICKED(IDC_RADIO_CONTINUOUS, &CDlgSettingRecording::OnBnClickedRadioContinuous)
ON_BN_CLICKED(IDC_RADIO_EVENT, &CDlgSettingRecording::OnBnClickedRadioEvent)
ON_BN_CLICKED(IDC_RADIO_SCHEDULING, &CDlgSettingRecording::OnBnClickedRadioScheduling)
ON_BN_CLICKED(IDC_BTN_MON, &CDlgSettingRecording::OnBnClickedBtnMon)
ON_BN_CLICKED(IDC_BTN_TUE, &CDlgSettingRecording::OnBnClickedBtnTue)
ON_BN_CLICKED(IDC_BTN_WED, &CDlgSettingRecording::OnBnClickedBtnWed)
ON_BN_CLICKED(IDC_BTN_THU, &CDlgSettingRecording::OnBnClickedBtnThu)
ON_BN_CLICKED(IDC_BTN_FRI, &CDlgSettingRecording::OnBnClickedBtnFri)
ON_BN_CLICKED(IDC_BTN_SAT, &CDlgSettingRecording::OnBnClickedBtnSat)
ON_BN_CLICKED(IDC_BTN_SUN, &CDlgSettingRecording::OnBnClickedBtnSun)
ON_BN_CLICKED(IDC_RADIO_SCHEDULE_EVENT, &CDlgSettingRecording::OnBnClickedRadioScheduleEvent)
ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgSettingRecording message handlers

void CDlgSettingRecording::OnBnClickedButtonAddPath()
{
	// TODO: Add your control notification handler code here
	LPITEMIDLIST pFolderRoot = NULL;
	LPITEMIDLIST pFolderSelected = NULL;
	BROWSEINFO bPath = {0};
	CString csPath, csSelectDialogTitle;

	LOADSTRING(csSelectDialogTitle, IDS_SELECT_FOLDER);

	bPath.hwndOwner = this->m_hWnd;
	bPath.pidlRoot = pFolderRoot;
	//bPath.pszDisplayName = szDisplayName;
	//bPath.lpszTitle = _T("Select a folder");
	bPath.lpszTitle = csSelectDialogTitle;
	bPath.ulFlags = BIF_NEWDIALOGSTYLE;
	bPath.lpfn = NULL;
	bPath.lParam = 0;

	pFolderSelected = SHBrowseForFolder(&bPath);

	//if root is set up.
	/*if(pFolderRoot)
    {
        CoTaskMemFree(pFolderRoot);
    }*/

	if(pFolderSelected)
	{
		SHGetPathFromIDList(pFolderSelected, csPath.GetBuffer(1024));
	}

	IMalloc *pmal =0;
	if(SHGetMalloc(&pmal) == S_OK)
	{
		pmal->Free(pFolderSelected);
		pmal->Release();
		DirtyFlag = true;
	}
	
	/*--Check the Path--*/
	CFileFind finder;
	if(csPath == _T(""))
		return;
	else if(csPath.GetLength() > 130)	
	{
		AfxMessageBox(IDS_PATH_TOOLONG);
		//AfxMessageBox(_T("The Path is too long.\r\nPlease Check the Path."));
		return;
	}
	else if(!finder.FindFile(csPath))
	{
		int nRet;
		nRet = GetFileAttributes(csPath);
		if (FILE_ATTRIBUTE_DIRECTORY == (GetFileAttributes(csPath) & FILE_ATTRIBUTE_DIRECTORY))
		{	
			AfxMessageBox(IDS_PATH_ROOT);
			//AfxMessageBox(_T("The Path cannot be Root.\r\nPlease Check the Path."));
			return;
		}
	}

	if (g_csaSavePath.GetSize() > 0)
	{
		int nTotal=g_csaSavePath.GetSize();
		for (int n=0; n<nTotal; n++)
		{
			CString csSavePath=g_csaSavePath.GetAt(n);
			if (csSavePath == csPath)
			{
				CString csMsgRepeat;
				LOADSTRING(csMsgRepeat, IDS_PATH_REPEAT);
				AfxMessageBox(csMsgRepeat);
				return;
			}
		}
	}
	
	CString csItemID;
	csItemID.Format(_T("%d"), pPathList.GetItemCount() + 1);
	int nItem = pPathList.InsertItem(0, csItemID);
	pPathList.SetItemText(nItem, 1, csPath);
	pPathList.SetItemText(nItem, 2, _T("50"));

	DirtyFlag = true;
}

void CDlgSettingRecording::OnBnClickedButtonDeletePath()
{
	if(pPathList.GetItemCount() == 1)
	{
		CString csMsg;
		LOADSTRING(csMsg, IDS_RECORD_PATH_ERROR_2);
		AfxMessageBox(csMsg);
		return;
	}

	if(pPathList.GetSelectionMark() != -1)
	{
		pPathList.DeleteItem(pPathList.GetSelectionMark());

		for(int iTemp = 1; iTemp <= pPathList.GetItemCount(); iTemp++)
		{
			CString csPathID;
			csPathID.Format(_T("%d"), iTemp);
			pPathList.SetItemText((iTemp - 1), 0, csPathID); 
		}
	}

	DirtyFlag = true;
}

void CDlgSettingRecording::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(m_iInitFlag == NEW_CAMERA)
	{
		if( ((CButton*)this->GetDlgItem(IDC_RADIO_CONTINUOUS))->GetCheck())
		{
			m_TimeLine.vGetEventByte(byFromTimeLine);
			bTimeArrayConvert2Storage(byFromTimeLine, 168, bySQLByte, 42);
			
			bySQLByte[0] = 0xFF;
		}
		else if(((CButton*)this->GetDlgItem(IDC_RADIO_SCHEDULE_EVENT))->GetCheck())
		{
			m_TimeLine.vGetEventByte(byFromTimeLine);
			bTimeArrayConvert2Storage(byFromTimeLine, 168, bySQLByte, 42);

			bySQLByte[0] = 0x00;
		}

		OnOK();
		return;
	}


	OnBnClickedButtonRecordingSave();

	if(!m_bBufferFlag)	return;
	//{
	//	bBufferFlag = false;
	//	return;
	//}

	DirtyFlag = false;
	//m_DlgRecRecord->RecordDirtyFlag = false;
	//m_DlgRecSchedule->ScheduleDirtyFlag = false;
	/*CSA csa;
	HANDLE hCameraRecording = CreateEvent(&csa.m_sa, false, false, RECORDING_STATUS_CHANGE);
	if (hCameraRecording == NULL)
	{
		TRACE(_T("Error code :%d"), ::GetLastError());
	}
	SetEvent( hCameraRecording );
	CloseHandle(hCameraRecording);*/
	OnOK();
}

BOOL CDlgSettingRecording::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
	m_pbk = CNVRBk::GetBkObject();

	CString csDialogCaption(_T(""));
	LOADSTRING(csDialogCaption, IDS_DLG_SETTING_RECORDING);
	//csDialogCaption.LoadString(CGS::g_hResStr, IDS_DLG_SETTING_RECORDING);
	this->SetWindowTextW(csDialogCaption);

    SETITEMTEXT(IDC_STATIC_GROUP_PATH, IDS_STATIC_GROUP_PATH);
    SETITEMTEXT(IDC_GROUP_CAMERA_LIST, IDS_GROUP_CAMERA_LIST);    
    SETITEMTEXT(IDC_BTN_SUN, IDS_BTN_SUN);
    SETITEMTEXT(IDC_BTN_MON, IDS_BTN_MON);
    SETITEMTEXT(IDC_BTN_TUE, IDS_BTN_TUE);
    SETITEMTEXT(IDC_BTN_WED, IDS_BTN_WED);
    SETITEMTEXT(IDC_BTN_THU, IDS_BTN_THU);
    SETITEMTEXT(IDC_BTN_FRI, IDS_BTN_FRI);
    SETITEMTEXT(IDC_BTN_SAT, IDS_BTN_SAT);
    SETITEMTEXT(IDC_STATIC_REC_TYPE, IDS_STATIC_REC_TYPE);
    SETITEMTEXT(IDC_RADIO_CONTINUOUS, IDS_RADIO_CONTINUOUS);
    SETITEMTEXT(IDC_RADIO_SCHEDULE_EVENT, IDS_RADIO_SCHEDULE_EVENT);
    SETITEMTEXT(IDC_RADIO_SCHEDULING, IDS_RADIO_SCHEDULING);
    SETITEMTEXT(IDC_RADIO_EVENT, IDS_RADIO_EVENT);
    SETITEMTEXT(IDC_BUTTON_DUPLICATE, IDS_BUTTON_DUPLICATE);
	SETITEMTEXT(IDC_STATIC_CURSOR_COLOR, IDS_TRIGGER_TYPE);

	//--------------Initial Bitmap Button-----------------*/
	m_cbRecPathDelete.SubclassDlgItem(IDC_BUTTON_DELETE_PATH, this);
	m_cbRecPathAdd.SubclassDlgItem(IDC_BUTTON_ADD_PATH, this);
	m_cbRecPathEdit.SubclassDlgItem(IDC_BUTTON_MODIFY_PATH, this);

	m_cbRecContinue.SubclassDlgItem(IDC_BUTTON_CONTINUE_RECT, this);
	m_cbRecEvent.SubclassDlgItem(IDC_BUTTON_EVENT_RECT, this);

	m_cbRecPathDelete.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL);
	m_cbRecPathAdd.LoadBitmaps(IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD);
	m_cbRecPathEdit.LoadBitmaps(IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT);

	m_cbRecContinue.LoadBitmaps(IDB_CONTINUE_TYPE, IDB_CONTINUE_TYPE, IDB_CONTINUE_TYPE, IDB_CONTINUE_TYPE);
	m_cbRecEvent.LoadBitmaps(IDB_EVENT_TYPE, IDB_EVENT_TYPE, IDB_EVENT_TYPE, IDB_EVENT_TYPE);

	//--Disable Button Duplicate--*/
	((CButton*)(GetDlgItem( IDC_BUTTON_DUPLICATE )))->EnableWindow( 0 );

	LV_COLUMN ListColumn;
	
	//-------------Initial Path List Box------------------------*/

	WCHAR * PathListTitles[3];// = {_T("Priority"), _T("Path"), _T("Buffer Size(GB)")};
    CString csColTitle;
    LOADSTRING(csColTitle, IDS_PATH_LIST_TITLES);
	PathListTitles[0] = wcstok(csColTitle.GetBuffer(), _T(";\r\n"));
    PathListTitles[1] = wcstok(NULL, _T(";\r\n"));
    PathListTitles[2] = wcstok(NULL, _T(";\r\n"));
	for(int iTemp = 0; iTemp < 3; iTemp++)
	{
		ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		ListColumn.fmt = LVCFMT_LEFT;
		ListColumn.iSubItem = iTemp;

		if(iTemp == 1 )
		{ListColumn.cx = 550;}
		else if(iTemp == 2)
		{ListColumn.cx = 100;}
		else
		{ListColumn.cx = 60;}

		ListColumn.pszText = PathListTitles[iTemp];
		pPathList.InsertColumn(iTemp, &ListColumn);
	}

	DWORD dwStyle = pPathList.GetStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	pPathList.SetExtendedStyle(dwStyle);

	//-----------Get Path Information  From DB-------*/
	vGetPathInfo();

	//--Initial Time Line
	//m_GroupBox.SubclassDlgItem(IDC_STATIC_SCHEDULE, this);
	CRect rect;
	//m_GroupBox.GetClientRect(&rect);
	this->GetDlgItem(IDC_STATIC_SCHEDULE)->GetClientRect(&rect);
	//this->GetParent()->GetClientRect(&rect);

	rect.DeflateRect(410, 335, -320, -300);
	m_TimeLine.Create(NULL, WS_CHILD|WS_VISIBLE|SS_NOTIFY, rect, this);
	m_TimeLine.vSetGridRect();
	m_TimeLine.vDisable();

	//vShowSchedule(false);

	//------------Initial Camera / Recording Tree Control-------------------*/
	vInitRecCameraTreeCtrl();

	//--Not Dirty yet--*/
	DirtyFlag = false;
	//m_DlgRecRecord->RecordDirtyFlag = false;
	//m_DlgRecSchedule->ScheduleDirtyFlag = false;

	//--Microhelp on buttons--*/
	m_ToolTip= new CToolTipCtrl();
	m_ToolTip->Create(this);
	CString ls_st;
	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_DEL_STORE_LOC);
	LOADSTRING(ls_st, IDS_TOOL_DEL_STORE_LOC);
	m_ToolTip->AddTool(&m_cbRecPathDelete, ls_st);

	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_ADD_STORE_LOC);
	LOADSTRING(ls_st, IDS_TOOL_ADD_STORE_LOC);
	m_ToolTip->AddTool(&m_cbRecPathAdd, ls_st);

	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_MOD_STORE_LOC);
	LOADSTRING(ls_st, IDS_TOOL_MOD_STORE_LOC);
	m_ToolTip->AddTool(&m_cbRecPathEdit, ls_st);

	/*m_ToolTip->AddTool(&m_cbRecPathDelete,_T("Delete Store Location"));
	m_ToolTip->AddTool(&m_cbRecPathAdd,_T("Add New Store Location"));
	m_ToolTip->AddTool(&m_cbRecPathEdit,_T("Modify Store Location"));*/
	m_ToolTip->Activate(TRUE);

	GetDlgItem(IDC_RADIO_CONTINUOUS)->EnableWindow(0);
	GetDlgItem(IDC_RADIO_SCHEDULE_EVENT)->EnableWindow(0);
	GetDlgItem(IDC_RADIO_SCHEDULING)->EnableWindow(0);
	GetDlgItem(IDC_RADIO_EVENT)->EnableWindow(0);

	vGetCurrentRecordingPath();	

	switch(m_iInitFlag)
	{
	case RECORDING_DEFAULT:
		
		break;
	case NEW_CAMERA:
		pRECCameraTreeCtrl.EnableWindow(0);
		((CButton*)GetDlgItem(IDC_BUTTON_RECORDING_SAVE))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_BUTTON_DUPLICATE))->EnableWindow(0);

		m_TimeLine.vEnable();
		GetDlgItem(IDC_RADIO_CONTINUOUS)->EnableWindow(1);
		GetDlgItem(IDC_RADIO_SCHEDULE_EVENT)->EnableWindow(1);
		GetDlgItem(IDC_RADIO_SCHEDULING)->EnableWindow(1);
		GetDlgItem(IDC_RADIO_EVENT)->EnableWindow(1);

		((CButton*)GetDlgItem(IDC_RADIO_CONTINUOUS))->SetCheck(1);

		BYTE byContinuousByte[43];
		::ZeroMemory(&byContinuousByte, sizeof(byContinuousByte));
		for(int iTemp = 1; iTemp <= 21; iTemp++)
		{
			byContinuousByte[iTemp] = 0xFF;
		}
		m_TimeLine.vFillScheduleAndEventRect(byContinuousByte);

		break;
	case MODIFY_CAMERA:
		
		vShowCameraInfo(iCameraIDX, m_IP);
		break;
	default:
		ASSERT(false);
		break;
	};
    pRECCameraTreeCtrl.SetFocus();

	//--Show the first camera recording schedule
	//--Get Tree Control Selected Item
	HTREEITEM hSelect = pRECCameraTreeCtrl.GetSelectedItem();

	//--Check if it's a iSAP node
	CString csItemText = pRECCameraTreeCtrl.GetItemText(hSelect);
	if(csItemText.Find(_T("Channel")) != -1 || csItemText.Find(_T("NVR")) != -1)
	{
		m_TimeLine.vReset();
		m_TimeLine.vDisable();

		GetDlgItem(IDC_RADIO_CONTINUOUS)->EnableWindow(0);
		GetDlgItem(IDC_RADIO_SCHEDULE_EVENT)->EnableWindow(0);
		GetDlgItem(IDC_RADIO_SCHEDULING)->EnableWindow(0);
		GetDlgItem(IDC_RADIO_EVENT)->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_BUTTON_RECORDING_SAVE))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_BUTTON_DUPLICATE))->EnableWindow(0);
		return true;
	}

	/*--Check Button Duplicate Should Enable or not--*/
	if(hSelect == NULL)
		((CButton*)(GetDlgItem( IDC_BUTTON_DUPLICATE )))->EnableWindow( 0 );
	else
		((CButton*)(GetDlgItem( IDC_BUTTON_DUPLICATE )))->EnableWindow( 1 );
	
	/*--Make Sure that hSelectRoot is a Root TreeItem--*/
	//HTREEITEM hSelectRoot = hGetCameraSelectRoot(hSelect);

	/*--Check hSelectRoot has Child Item or not. If not, Disable Button Duplicate--*/
	//if(pRECCameraTreeCtrl.GetChildItem(hSelectRoot) == NULL)
	//	((CButton*)(GetDlgItem( IDC_BUTTON_DUPLICATE )))->EnableWindow( 0 );
	
	/*--Get Item's Index Value in Tree Control--*/
	HTREEITEM hRoot = pRECCameraTreeCtrl.GetRootItem();
	iCameraTreeIDX = 0;
	while(hSelect != hRoot)
	{
		iCameraTreeIDX++;
		hRoot = pRECCameraTreeCtrl.GetNextSiblingItem(hRoot);
	}

	if(veRecCameraList.size() == 0)	return 0;
	iCameraIDX = _ttoi(veRecCameraList[iCameraTreeIDX].csCameraIDX);	//iCameraIDX is CameraIDX in DB

	//veRecCameraList[iCameraTreeIDX].csCameraIDX.ReleaseBuffer();

	//--Set Camera Name in Edit Control
	GetDlgItem(IDC_EDIT_RECORDING_CAMERA_NAME)->SetWindowText( veRecCameraList[iCameraTreeIDX].csIPnName );

	//--Reset TimeLine
	m_TimeLine.vReset();

	//--Check ecac_acmera
	if( bExistECAC(iCameraIDX) )
	{
		m_TimeLine.vGetEventByte(byFromTimeLine);
		bTimeArrayConvert2Storage(byFromTimeLine, 168, bySQLByte, 42);

		m_TimeLine.vSetChangeColorFlag(true);
		m_TimeLine.vFillAllRect();
		m_TimeLine.vDisable();

		((CButton*)GetDlgItem(IDC_RADIO_CONTINUOUS))->SetCheck(1);
		GetDlgItem(IDC_RADIO_CONTINUOUS)->EnableWindow(1);
		GetDlgItem(IDC_RADIO_SCHEDULE_EVENT)->EnableWindow(0);
		GetDlgItem(IDC_RADIO_SCHEDULING)->EnableWindow(0);
		GetDlgItem(IDC_RADIO_EVENT)->EnableWindow(0);
		return true;
	}

	//Get Type and Binary Recording Time
	bGetRecoringTypeAndBinaryTime(iCameraTreeIDX);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgSettingRecording::OnTcnSelchangeTabRecording(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	
	
	//CRect crRecordingTab;
	//pRecordingTab.GetClientRect(&crRecordingTab);

	//if(pRecordingTab.GetCurSel() == 0)
	//{
	//	m_CRecordTimeLine->ShowWindow(SW_HIDE);
	//	//m_DlgRecSchedule->ShowWindow(SW_HIDE);
	//	m_DlgRecRecord->ShowWindow(SW_SHOW);
	//}
	//else if(pRecordingTab.GetCurSel() == 1)
	//{
	//	m_CRecordTimeLine->ShowWindow(SW_SHOW);
	//	//m_DlgRecSchedule->ShowWindow(SW_SHOW);
	//	m_DlgRecRecord->ShowWindow(SW_HIDE);
	//}


	*pResult = 0;
}

//void CDlgSettingRecording::vDBConnect()
//{
//	CIniReader s_ini(_T(".\\ec.ini"));
//	csDSN.Format(_T("%s"), s_ini.getKeyValue(_T("DSN"),_T("database")));
//	csDBUser.Format(_T("%s"), s_ini.getKeyValue(_T("UID"),_T("database")));
//	csDBPass.Format(_T("%s"), s_ini.getKeyValue(_T("PWD"),_T("database")));
//	csODBC = _T("ODBC;DSN=") + csDSN + _T(";UID=") + csDBUser + _T(";PWD=") + csDBPass;
//
//	if( DBQuery.Open((csDSN), FALSE, FALSE, csODBC) )
//	{/*AfxMessageBox(_T("Connection OK"));*/}
//}

CString CDlgSettingRecording::csSQLRecording(CString csCameraIDX, CString csRecordingFlag)
{
	//vDBConnect();
	try
	{
		CString SQL_Recording_Type;
		SQL_Recording_Type.Format(_T("SELECT * FROM ec_camera_recording WHERE camera_idx = '%s'"), csCameraIDX);

		CRecordset rRecType(&((CMainFrame*)AfxGetMainWnd())->m_DB);
		rRecType.Open(CRecordset::forwardOnly, SQL_Recording_Type);
		while (!rRecType.IsEOF()) { 
			rRecType.GetFieldValue(_T("recording_type"), csRecordingFlag);
			csRecordingFlag.TrimRight();
			rRecType.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}


	return csRecordingFlag;
}

CString CDlgSettingRecording::csSQLRecordingID(CString csCameraIDX, CString csRecordingID)
{
	try
	{
		CString SQL_Recording_ID;
		SQL_Recording_ID.Format(_T("SELECT * FROM ec_camera_recording WHERE camera_idx = '%s'"), csCameraIDX);

		CRecordset rRecID(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
		rRecID.Open(CRecordset::forwardOnly, SQL_Recording_ID);
		while (!rRecID.IsEOF()) { 
			rRecID.GetFieldValue(_T("camera_recordingid"), csRecordingID);
			csRecordingID.TrimRight();
			rRecID.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return csRecordingID;
}

int CDlgSettingRecording::iSQLScheduleNum(CString csRecordingID, int iScheduleNum)
{
	try
	{
		CString SQL_Schedule_Count, csScheduleNum;
		SQL_Schedule_Count.Format(_T("SELECT COUNT(*) AS Schedule_Count FROM ec_camera_recording_time WHERE camera_recordingid = '%s'"), csRecordingID);

		CRecordset rRecNum(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
		rRecNum.Open(CRecordset::forwardOnly, SQL_Schedule_Count);
		while (!rRecNum.IsEOF()) { 
			rRecNum.GetFieldValue(_T("Schedule_Count"), csScheduleNum);
			csScheduleNum.TrimRight();
			rRecNum.MoveNext();
		}

		iScheduleNum = _ttoi(csScheduleNum.GetBuffer(0));
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return iScheduleNum;
}

void CDlgSettingRecording::vSQLInsertScheduleTime(CString csRecordingID, HTREEITEM hScheduleParent)
{
	try
	{
		CString SQL_Schedule_Time;
		SQL_Schedule_Time.Format(_T("SELECT day_type, CONVERT(VARCHAR(10), recording_date, 110) AS recording_date, CONVERT(VARCHAR(8), time_from, 108) AS time_from, CONVERT(VARCHAR(8), time_to, 108) AS time_to FROM ec_camera_recording_time WHERE camera_recordingid = '%s'"), csRecordingID);

		CString csDayType, csRecordingDate, csTimeFrom, csTimeTo, csScheduleTime;
		CRecordset rRecTime(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
		rRecTime.Open(CRecordset::forwardOnly, SQL_Schedule_Time);
		while (!rRecTime.IsEOF()) { 
			rRecTime.GetFieldValue(_T("day_type"), csDayType);
			rRecTime.GetFieldValue(_T("recording_date"), csRecordingDate);
			rRecTime.GetFieldValue(_T("time_from"), csTimeFrom);
			rRecTime.GetFieldValue(_T("time_to"), csTimeTo);
			csDayType.TrimRight();
			csRecordingDate.TrimRight();
			csTimeFrom.TrimRight();
			csTimeTo.TrimRight();

			csTimeFrom = csTimeFrom.Left(2);
			csTimeTo = csTimeTo.Left(2);

			if(csDayType == _T("1"))
			{
				csScheduleTime.Format(_T("Everyday:%s-%s"), csTimeFrom, csTimeTo);
				pRECCameraTreeCtrl.InsertItem(csScheduleTime, hScheduleParent);
			}
			else if(csDayType == _T("2"))
			{
				csScheduleTime.Format(_T("Sunday:%s-%s"), csTimeFrom, csTimeTo);
				pRECCameraTreeCtrl.InsertItem(csScheduleTime, hScheduleParent);
			}
			else if(csDayType == _T("3"))
			{
				csScheduleTime.Format(_T("Monday:%s-%s"), csTimeFrom, csTimeTo);
				pRECCameraTreeCtrl.InsertItem(csScheduleTime, hScheduleParent);
			}
			else if(csDayType == _T("4"))
			{
				csScheduleTime.Format(_T("Tuesday:%s-%s"), csTimeFrom, csTimeTo);
				pRECCameraTreeCtrl.InsertItem(csScheduleTime, hScheduleParent);
			}
			else if(csDayType == _T("5"))
			{
				csScheduleTime.Format(_T("Wednesday:%s-%s"), csTimeFrom, csTimeTo);
				pRECCameraTreeCtrl.InsertItem(csScheduleTime, hScheduleParent);
			}
			else if(csDayType == _T("6"))
			{
				csScheduleTime.Format(_T("Thursday:%s-%s"), csTimeFrom, csTimeTo);
				pRECCameraTreeCtrl.InsertItem(csScheduleTime, hScheduleParent);
			}
			else if(csDayType == _T("7"))
			{
				csScheduleTime.Format(_T("Friday:%s-%s"), csTimeFrom, csTimeTo);
				pRECCameraTreeCtrl.InsertItem(csScheduleTime, hScheduleParent);
			}
			else if(csDayType == _T("8"))
			{
				csScheduleTime.Format(_T("Saturday:%s-%s"), csTimeFrom, csTimeTo);
				pRECCameraTreeCtrl.InsertItem(csScheduleTime, hScheduleParent);
			}
			else if(csDayType == _T("9"))
			{
				csScheduleTime.Format(_T("Specific Day:%s, %s-%s"), csRecordingDate, csTimeFrom, csTimeTo);
				pRECCameraTreeCtrl.InsertItem(csScheduleTime, hScheduleParent);
			}
			else
				continue;

			rRecTime.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CDlgSettingRecording::vSQLRecCameraList()
{
	vector<ec_Camera> vcCams;
	m_pbk->GetAllCameras(vcCams);
	bool bCamera = false;

	int nIdx = 0, nCoun = vcCams.size();
	for (nIdx = 0; nIdx < nCoun; nIdx++)
	{
		bCamera = false;
		if (m_pbk->GetDevice(vcCams[nIdx].video_format) == eNVR || 
			m_pbk->GetDevice(vcCams[nIdx].video_format) == IPVSTATION)
		{
// 			if (m_pbk->IsChannel(vcCams[nIdx].video_format))
// 			{
// 				bCamera = true;
// 			}
			continue;
		}
		else
		{
			bCamera = true;
		}
		if (bCamera)
		{
			m_RecCameraList.csIPnName.Format(_T("( %d ) %s - %s"), vcCams[nIdx].camera_idx, vcCams[nIdx].ipaddress, vcCams[nIdx].cameraname);
			m_RecCameraList.iTreePos = nIdx+1;

			m_RecCameraList.csCameraName = vcCams[nIdx].cameraname;
			m_RecCameraList.csCameraIDX.Format(_T("%d"),vcCams[nIdx].camera_idx);
			::ZeroMemory(&m_RecCameraList.byteTimeArray, sizeof(m_RecCameraList.byteTimeArray));
			veRecCameraList.push_back(m_RecCameraList);
		}
	}

	/*try
	{
		int iCameraID = 1;
		CString csIP, csCameraName, csCameraIDX;
		CString csSQL_Camera(_T("SELECT * FROM ec_camera WHERE active_ = 'Y' OR active_ = NULL"));
		CRecordset rCamera(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
		rCamera.Open(CRecordset::forwardOnly, csSQL_Camera);
		while (!rCamera.IsEOF()) { 
			rCamera.GetFieldValue(_T("camera_idx"), csCameraIDX);
			rCamera.GetFieldValue(_T("cameraname"), csCameraName);
			rCamera.GetFieldValue(_T("ipaddress"), csIP);

			csCameraName.TrimRight();
			csIP.TrimRight();
			csCameraIDX.TrimRight();
			m_RecCameraList.csIPnName.Format(_T("( %s ) %s - %s"), csCameraIDX, csIP, csCameraName);
			m_RecCameraList.iTreePos = iCameraID;

			m_RecCameraList.csCameraName = csCameraName;
			m_RecCameraList.csCameraIDX = csCameraIDX;
			::ZeroMemory(&m_RecCameraList.byteTimeArray, sizeof(m_RecCameraList.byteTimeArray));
			veRecCameraList.push_back(m_RecCameraList);

			iCameraID++;
			rCamera.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}*/
}
void CDlgSettingRecording::OnTvnSelchangedTreeCameraList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	/*--Get Tree Control Selected Item--*/
	HTREEITEM hSelect = pRECCameraTreeCtrl.GetSelectedItem();

	//--Check if it's a iSAP node
	CString csItemText = pRECCameraTreeCtrl.GetItemText(hSelect);
	if(csItemText.Find(_T("Channel")) != -1 || csItemText.Find(_T("NVR")) != -1)
	{
		m_TimeLine.vReset();
		m_TimeLine.vDisable();

		GetDlgItem(IDC_RADIO_CONTINUOUS)->EnableWindow(0);
		GetDlgItem(IDC_RADIO_SCHEDULE_EVENT)->EnableWindow(0);
		GetDlgItem(IDC_RADIO_SCHEDULING)->EnableWindow(0);
		GetDlgItem(IDC_RADIO_EVENT)->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_BUTTON_RECORDING_SAVE))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_BUTTON_DUPLICATE))->EnableWindow(0);
		return;
	}

	/*--Check Button Duplicate Should Enable or not--*/
	if(hSelect == NULL)
		((CButton*)(GetDlgItem( IDC_BUTTON_DUPLICATE )))->EnableWindow( 0 );
	else
		((CButton*)(GetDlgItem( IDC_BUTTON_DUPLICATE )))->EnableWindow( 1 );
	
	/*--Make Sure that hSelectRoot is a Root TreeItem--*/
	//HTREEITEM hSelectRoot = hGetCameraSelectRoot(hSelect);

	/*--Check hSelectRoot has Child Item or not. If not, Disable Button Duplicate--*/
	//if(pRECCameraTreeCtrl.GetChildItem(hSelectRoot) == NULL)
	//	((CButton*)(GetDlgItem( IDC_BUTTON_DUPLICATE )))->EnableWindow( 0 );
	
	/*--Get Item's Index Value in Tree Control--*/
	HTREEITEM hRoot = pRECCameraTreeCtrl.GetRootItem();
	iCameraTreeIDX = 0;
	while(hSelect != hRoot)
	{
		iCameraTreeIDX++;
		hRoot = pRECCameraTreeCtrl.GetNextSiblingItem(hRoot);
	}
	iCameraIDX = _ttoi(veRecCameraList[iCameraTreeIDX].csCameraIDX);	//iCameraIDX is CameraIDX in DB
	//veRecCameraList[iCameraTreeIDX].csCameraIDX.ReleaseBuffer();

	//--Set Camera Name in Edit Control
	GetDlgItem(IDC_EDIT_RECORDING_CAMERA_NAME)->SetWindowText( veRecCameraList[iCameraTreeIDX].csIPnName );

	//--Reset TimeLine
	m_TimeLine.vReset();

	//--Check ecac_acmera
	if( bExistECAC(iCameraIDX) )
	{
		m_TimeLine.vGetEventByte(byFromTimeLine);
		bTimeArrayConvert2Storage(byFromTimeLine, 168, bySQLByte, 42);

		m_TimeLine.vSetChangeColorFlag(true);
		m_TimeLine.vFillAllRect();
		m_TimeLine.vDisable();

		((CButton*)GetDlgItem(IDC_RADIO_CONTINUOUS))->SetCheck(1);
		GetDlgItem(IDC_RADIO_CONTINUOUS)->EnableWindow(1);
		GetDlgItem(IDC_RADIO_SCHEDULE_EVENT)->EnableWindow(0);
		GetDlgItem(IDC_RADIO_SCHEDULING)->EnableWindow(0);
		GetDlgItem(IDC_RADIO_EVENT)->EnableWindow(0);
		return;
	}

	//Get Type and Binary Recording Time
	bGetRecoringTypeAndBinaryTime(iCameraTreeIDX);



	*pResult = 0;
}

HTREEITEM CDlgSettingRecording::hGetCameraSelectRoot(HTREEITEM hSelect)
{
	HTREEITEM hParent1 = pRECCameraTreeCtrl.GetParentItem(hSelect);
	if(hParent1 == NULL)
		return hSelect;
	HTREEITEM hParent2 = pRECCameraTreeCtrl.GetParentItem(hParent1);
	if(hParent2 == NULL)
		return hParent1;
	HTREEITEM hParent3 = pRECCameraTreeCtrl.GetParentItem(hParent2);
	if(hParent3 == NULL)
		return hParent2;

	return hParent3;
}
void CDlgSettingRecording::OnBnClickedButtonDuplicate()
{
	// TODO: Add your control notification handler code here
    CDlgRecDuplicate m_CDlgRecDuplicate(this);

	/*--Get Tree Control Selected Item--*/
	HTREEITEM hSelect = pRECCameraTreeCtrl.GetSelectedItem();
	
	/*--Make Sure that it is a Root Item--*/
	HTREEITEM hSelectRoot = hGetCameraSelectRoot(hSelect);

	//HTREEITEM hRecType = pRECCameraTreeCtrl.GetChildItem(hSelectRoot);
	//HTREEITEM hRecSchedule = pRECCameraTreeCtrl.GetNextItem(hRecType, TVGN_NEXT);

	/*--Get Recording Type on Tree Control--*/
	//CString csRecType = pRECCameraTreeCtrl.GetItemText(hRecType);
	//CString csSchedule = pRECCameraTreeCtrl.GetItemText(hRecSchedule);

	m_CDlgRecDuplicate.vSetRecInfo(veRecCameraList[iCameraTreeIDX].csIPnName);

	if(((CButton*)this->GetDlgItem(IDC_RADIO_CONTINUOUS))->GetCheck())
	{
		m_CDlgRecDuplicate.vSetRecType(_T("Continuous Recording"));
	}
	else if(((CButton*)this->GetDlgItem(IDC_RADIO_SCHEDULE_EVENT))->GetCheck())
	{
		m_CDlgRecDuplicate.vSetRecType(_T("Scheduling & Event Recording"));
	}

	if(m_CDlgRecDuplicate.DoModal() == IDOK)
	{
		/*--Put Checked Camera Name into a Vector and Get it Back--*/
		this->m_veDuplicateCamera.assign(m_CDlgRecDuplicate.veDupCamereName.begin(), m_CDlgRecDuplicate.veDupCamereName.end());
		m_CDlgRecDuplicate.vDBClose();
		DirtyFlag = true;
	}
	else
	{
		m_CDlgRecDuplicate.vDBClose();
		return;
	}

	/*--Save Recording Data to DB--*/

	vDeleteStoragePath();
	vSaveStoragePath();
	
	BYTE byFromTimeLine[168] = {0};
	BYTE bySQLByte[43] = {0};

	m_TimeLine.vGetEventByte(byFromTimeLine);
	bTimeArrayConvert2Storage(byFromTimeLine, 168, bySQLByte, 42);

	CString csRecordingType (_T(""));
	if( ((CButton*)this->GetDlgItem(IDC_RADIO_CONTINUOUS))->GetCheck())
	{
		//iRecordingType = RECORDING_TYPE::CONTINOUS;
		csRecordingType = _T("0");
		bySQLByte[0] = 0xFF;
	}
	else if(((CButton*)this->GetDlgItem(IDC_RADIO_SCHEDULE_EVENT))->GetCheck())
	{
		//iRecordingType = RECORDING_TYPE::SCHEDULING;
		csRecordingType = _T("1");
	}	
	else
		ASSERT(false);

	int iVectorCameraIDX;
	for(int iTemp = 0; iTemp < m_veDuplicateCamera.size(); iTemp++)
	{
		/*--Get iCameraIDX from DB--*/
		iVectorCameraIDX = iGetCameraIDXVector(m_veDuplicateCamera[iTemp]);
		
		bDeleteCameraRecordingInfo(iVectorCameraIDX);
		bSaveCameraBinaryTime(iVectorCameraIDX, bySQLByte, csRecordingType);
	}

	/*--Refresh Camera Tree Control--*/
	pRECCameraTreeCtrl.DeleteAllItems();
	veRecCameraList.clear();
	vInitRecCameraTreeCtrl();

	GetDlgItem(IDC_RADIO_CONTINUOUS)->EnableWindow(0);
	GetDlgItem(IDC_RADIO_SCHEDULE_EVENT)->EnableWindow(0);
	GetDlgItem(IDC_RADIO_SCHEDULING)->EnableWindow(0);
	GetDlgItem(IDC_RADIO_EVENT)->EnableWindow(0);
}

void CDlgSettingRecording::vSaveRecordingInfo(int iCameraIDX)
{
	/*--Get Recording Type--*/
	/*CString csRecType;
	if(((CButton*)(m_DlgRecRecord->GetDlgItem(IDC_RADIO_RECORDING_RECORD_EVENT)))->GetCheck() == 1)
		csRecType = _T("Event");
	else if(((CButton*)(m_DlgRecRecord->GetDlgItem(IDC_RADIO_RECORDING_RECORD_CONTINUOUS)))->GetCheck() == 1)
		csRecType = _T("Continuous");
	else if(((CButton*)(m_DlgRecRecord->GetDlgItem(IDC_RADIO_RECORDING_RECORD_SCHEDULING)))->GetCheck() == 1)
		csRecType = _T("Scheduling");
	else
		csRecType = _T("");*/

	CString csRecType;
	if(((CButton*)(GetDlgItem( IDC_RADIO_EVENT )))->GetCheck() == 1)
		csRecType = _T("Event");
	else if(((CButton*)(GetDlgItem( IDC_RADIO_CONTINUOUS )))->GetCheck() == 1)
		csRecType = _T("Continuous");
	else if(((CButton*)(GetDlgItem( IDC_RADIO_SCHEDULING )))->GetCheck() == 1)
		csRecType = _T("Scheduling");
	else
		csRecType = _T("");

	try
	{
		//--Save Recording Type and Info to DB
		/*--Save Recording Type to DB--*/
		CString SQLInsertRecType;
		SQLInsertRecType.Format(_T("INSERT INTO ec_camera_recording(camera_idx, recording_type) VALUES ('%d', '%s')"), iCameraIDX, csRecType);
		(((CMainFrame*)AfxGetMainWnd())->m_DB).ExecuteSQL(SQLInsertRecType);
		
		/*--Get Recording ID From DB--*/
		//CString csCameraIDX, csRecordingID;
		//csCameraIDX.Format(_T("%d"), iCameraIDX);
		//csRecordingID = csSQLRecordingID(csCameraIDX, csRecordingID);

		/*--Call m_DlgRecSchedule to Save Recording Time--*/
		//m_DlgRecSchedule->vSaveRecTime(csRecordingID);
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CDlgSettingRecording::OnBnClickedButtonRecordingSave()
{
	// TODO: Add your control notification handler code here
	/*BYTE byFromTimeLine[168] = {0};.
	BYTE bySQLByte[43] = {0};
	BYTE byConvertByte[337] = {0};*/
	
	CString csPath, csBuffer;
	for(int iTemp = 0; iTemp < pPathList.GetItemCount(); iTemp++)
	{
		csPath = pPathList.GetItemText(iTemp, 1);

		CString csPathWithStar;
		csPathWithStar = csPath + _T("\\*.*");

		CFileFind m_CFileFind;
		if (!m_CFileFind.FindFile(csPathWithStar.GetBuffer()))
        {
            if (CGS::bCreateHierarchyDir(csPathWithStar.GetBuffer()) == 0)
            {
                CString csMsg;
                csMsg.Format(_T("Can't create recording path %s"), csPathWithStar.GetBuffer());
			    AfxMessageBox(csMsg);
			    m_bBufferFlag = false;
				m_CFileFind.Close();
                return;
            }else
            {
                m_CFileFind.FindFile(csPathWithStar.GetBuffer());
            }
        }
		m_CFileFind.FindNextFileW();

		CString csRoot = m_CFileFind.GetRoot();
		if(csRoot.GetLength() <= 3)	
		{
			CString csMsg;
			LOADSTRING(csMsg, IDS_RECORD_PATH_ERROR);
			AfxMessageBox(csMsg);
			m_bBufferFlag = false;
			m_CFileFind.Close();
			return;
		}
		
		csBuffer = pPathList.GetItemText(iTemp, 2);
		if( !bCheckPathBuffer(csPath, ::_ttoi(csBuffer)) )
		{
			CString csMSG(_T("")), csMsg1(_T("")), csMsg2(_T(""));
			LOADSTRING(csMsg1, IDS_SET_RECORDING_ERROR_1);
			LOADSTRING(csMsg2, IDS_SET_RECORDING_ERROR_2);
			//csMsg1.LoadStringW(CGS::g_hResStr, IDS_SET_RECORDING_ERROR_1);
			//csMsg2.LoadStringW(CGS::g_hResStr, IDS_SET_RECORDING_ERROR_2);

			csMSG.Format(_T("%s %s%s"), csMsg1.GetBuffer(), csPath.GetBuffer(), csMsg2.GetBuffer());
			int iRet = AfxMessageBox(csMSG, MB_YESNO);

			if(iRet == IDYES)
			{
				m_bBufferFlag = true;
				//return;

			}
			else if(iRet == IDNO)
			{
				//OnClose();
				m_bBufferFlag = false;
				m_CFileFind.Close();
				return;
			}
		}
		m_CFileFind.Close();
	}

	m_bBufferFlag = true;
	vDeleteStoragePath();
	vSaveStoragePath();

	HTREEITEM hSelect = pRECCameraTreeCtrl.GetSelectedItem();
	HTREEITEM hRoot = pRECCameraTreeCtrl.GetRootItem();
	iCameraTreeIDX = 0;
	while(hSelect != hRoot)
	{
		iCameraTreeIDX++;
		hRoot = pRECCameraTreeCtrl.GetNextSiblingItem(hRoot);
	}

	if(iCameraTreeIDX >= pRECCameraTreeCtrl.GetCount() && iCameraTreeIDX != 0)
	{
		CString csMsg(_T(""));
		LOADSTRING(csMsg, IDS_CAM_SELECT);
		//AfxMessageBox(csMsg);
		return;
	}

	if(veRecCameraList.size() != 0)
	{
		::ZeroMemory(&veRecCameraList[iCameraTreeIDX].byteTimeArray, sizeof(veRecCameraList[iCameraTreeIDX].byteTimeArray));

		m_TimeLine.vGetEventByte(byFromTimeLine);
		bTimeArrayConvert2Storage(byFromTimeLine, 168, veRecCameraList[iCameraTreeIDX].byteTimeArray, 42);

		//int iRecordingType  = 0;
		CString csRecordingType (_T(""));
		if( ((CButton*)this->GetDlgItem(IDC_RADIO_CONTINUOUS))->GetCheck())
		{
			//iRecordingType = RECORDING_TYPE::CONTINOUS;
			csRecordingType = _T("0");
			//bySQLByte[0] = 0xFF;
			veRecCameraList[iCameraTreeIDX].byteTimeArray[0] = 0xFF;

			bDeleteCameraRecordingInfo(iCameraIDX);
			bSaveCameraBinaryTime(iCameraIDX, veRecCameraList[iCameraTreeIDX].byteTimeArray, csRecordingType);
		}
		else if(((CButton*)this->GetDlgItem(IDC_RADIO_SCHEDULE_EVENT))->GetCheck())
		{
			//iRecordingType = RECORDING_TYPE::SCHEDULING;
			csRecordingType = _T("1");
			//bySQLByte[0] = 0x00;
			veRecCameraList[iCameraTreeIDX].byteTimeArray[0] = 0x00;

			bDeleteCameraRecordingInfo(iCameraIDX);
			bSaveCameraBinaryTime(iCameraIDX, veRecCameraList[iCameraTreeIDX].byteTimeArray, csRecordingType);
		}
	}

	bMon = false;
	bTue = false;
	bWed = false;
	bThu = false;
	bFri = false;
	bSat = false;
	bSun = false;

	DirtyFlag = false;

	CSA csa;
	HANDLE hCameraRecording = CreateEvent(&csa.m_sa, false, false, RECORDING_STATUS_CHANGE);
	if (hCameraRecording == NULL)
	{
		TRACE(_T("Error code :%d"), ::GetLastError());
	}
	SetEvent( hCameraRecording );
	CloseHandle(hCameraRecording);
}

void CDlgSettingRecording::vSaveStoragePath()
{
	g_csaSavePath.RemoveAll();

	for(int iTemp = 0; iTemp < pPathList.GetItemCount(); iTemp++)
	{
		CString csStoragePath = pPathList.GetItemText(iTemp, 1);
		g_csaSavePath.Add(csStoragePath);

		CString csStoragePriority = pPathList.GetItemText(iTemp, 0);
		CString csBufferSize = pPathList.GetItemText(iTemp, 2);
		try
		{
			CString SQLInsertStoragePath;
			SQLInsertStoragePath.Format(_T("INSERT INTO ec_storage(store_location, storage_type, buffer_size, sort_order) VALUES ('%s', 'RECORD','%s', '%s')"), csStoragePath, csBufferSize, csStoragePriority);
			(((CMainFrame*)AfxGetMainWnd())->m_DB).ExecuteSQL(SQLInsertStoragePath);
		}
		catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
	}
}

void CDlgSettingRecording::vDeleteStoragePath()
{
	try
	{
		CString SQLDeletePath;
		SQLDeletePath.Format(_T("DELETE FROM ec_storage WHERE storage_type='RECORD'"));
		(((CMainFrame*)AfxGetMainWnd())->m_DB).ExecuteSQL(SQLDeletePath);
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CDlgSettingRecording::vDeleteRecordingInfo(int iCameraIDX)
{
	try
	{
		/*--Get Recording ID From DB--*/
		CString csCameraIDX, csRecordingID;
		csCameraIDX.Format(_T("%d"), iCameraIDX);
		csRecordingID = csSQLRecordingID(csCameraIDX, csRecordingID);

		CString SQLDeleteRecType;
		SQLDeleteRecType.Format(_T("DELETE FROM ec_camera_recording WHERE camera_idx = '%d' "), iCameraIDX);
		(((CMainFrame*)AfxGetMainWnd())->m_DB).ExecuteSQL(SQLDeleteRecType);

		CString SQLDeleteRecTime;
		SQLDeleteRecTime.Format(_T("DELETE FROM ec_camera_recording_time WHERE camera_recordingid = '%s' "), csRecordingID);
		(((CMainFrame*)AfxGetMainWnd())->m_DB).ExecuteSQL(SQLDeleteRecTime);
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

int CDlgSettingRecording::iGetCameraIDXVector(CString csCameraName)
{
	int iCameraIDXVector;
	for(int iTemp = 0; iTemp < veRecCameraList.size(); iTemp++)
	{
		if(veRecCameraList[iTemp].csIPnName == csCameraName)
		{
			iCameraIDXVector = _ttoi(veRecCameraList[iTemp].csCameraIDX.GetBuffer(0));
			return iCameraIDXVector;
		}
	}
}

void CDlgSettingRecording::vInitRecCameraTreeCtrl()
{
	pRECCameraTreeCtrl.ModifyStyle(0, TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES);
	pRECCameraTreeCtrl.DeleteAllItems();

	HTREEITEM hRoot, hRecParent, hScheduleParent;

	vSQLRecCameraList();
	
	for(int iTemp = 0, iListCount = 1; iTemp < veRecCameraList.size(); iTemp++, iListCount++)
	{
		/*-------Set Root(IP - CameraName) Item--------*/
		hRoot = pRECCameraTreeCtrl.InsertItem(veRecCameraList[iTemp].csIPnName);

		///*------Set Recording Parent Item--------*/
		//CString csRecFlag, csRecID;
		//csRecID = csSQLRecordingID(veRecCameraList[iTemp].csCameraIDX, csRecID);	//Get Recording ID From DB
		//csRecFlag = csSQLRecording(veRecCameraList[iTemp].csCameraIDX, csRecFlag);	//Get Recording Type From DB

		//if(csRecFlag != _T(""))
		//{
		//	csRecFlag = _T("Recording / ") + csRecFlag;
		//	hRecParent =  pRECCameraTreeCtrl.InsertItem(csRecFlag, hRoot);
		//}

		//pRECCameraTreeCtrl.InsertItem(_T("Schedule & Event"), hRoot);

		///*------Set Schedule Parent Item------*/
		//int iScheduleNum = 0;
		//iScheduleNum = iSQLScheduleNum(csRecID, iScheduleNum);						//Get Schedule Number From DB

		//if(iScheduleNum > 0)
		//{
		//	CString csScheduleFlag;
		//	csScheduleFlag.Format(_T("Schedule / (%d)"), iScheduleNum);
		//	hScheduleParent = pRECCameraTreeCtrl.InsertItem(csScheduleFlag, hRoot);
		//	vSQLInsertScheduleTime(csRecID, hScheduleParent);						//Set Schedule Time From DB
		//}
	}
}

void CDlgSettingRecording::vGetPathInfo()
{
	try
	{
		int iPathID = 1;
		CString csSQL_Path(_T("SELECT * FROM ec_storage WHERE storage_type='RECORD'"));
		CString csPath, csPathID, csBufferSize;
		CRecordset rPath(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
		rPath.Open(CRecordset::forwardOnly, csSQL_Path);
		while (!rPath.IsEOF()) { 
			rPath.GetFieldValue(_T("store_location"), csPath);
			rPath.GetFieldValue(_T("buffer_size"), csBufferSize);

			csPathID.Format(_T("%d"), iPathID);
			csBufferSize.TrimRight();
			int nItem = pPathList.InsertItem(0, csPathID);
			pPathList.SetItemText(nItem, 1, csPath);
			pPathList.SetItemText(nItem, 2, csBufferSize);
			iPathID++;

			rPath.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}
void CDlgSettingRecording::OnBnClickedButtonModifyPath()
{
	// TODO: Add your control notification handler code here
	LPITEMIDLIST pFolderRoot = NULL;
	LPITEMIDLIST pFolderSelected = NULL;
	BROWSEINFO bPath = {0};
	CString csPath;

	bPath.hwndOwner = this->m_hWnd;
	bPath.pidlRoot = pFolderRoot;
	//bPath.pszDisplayName = szDisplayName;
	bPath.lpszTitle = _T("Select a folder");
	bPath.ulFlags = BIF_NEWDIALOGSTYLE;
	bPath.lpfn = NULL;
	bPath.lParam = 0;

	pFolderSelected = SHBrowseForFolder(&bPath);

	//if root is set up.
	/*if(pFolderRoot)
    {
        CoTaskMemFree(pFolderRoot);
    }*/

	if(pFolderSelected)
	{
		SHGetPathFromIDList(pFolderSelected, csPath.GetBuffer(1024));
	}

	IMalloc *pmal =0;
	if(SHGetMalloc(&pmal) == S_OK)
	{
		pmal->Free(pFolderSelected);
		pmal->Release();
		DirtyFlag = true;
	}
	
	if(csPath == _T(""))
		return;

	if (g_csaSavePath.GetSize() > 0)
	{
		int nTotal=g_csaSavePath.GetSize();
		for (int n=0; n<nTotal; n++)
		{
			CString csSavePath=g_csaSavePath.GetAt(n);
			if (csSavePath == csPath)
			{
				CString csMsgRepeat;
				LOADSTRING(csMsgRepeat, IDS_PATH_REPEAT);
				AfxMessageBox(csMsgRepeat);
				return;
			}
		}
	}

	pPathList.SetItemText(pPathList.GetSelectionMark(), 1, csPath);
}

BOOL CDlgSettingRecording::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_SilverBrush);

	//return CBaseDialog::OnEraseBkgnd(pDC);
	return true;
}

HBRUSH CDlgSettingRecording::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	//return hbr;
	if( nCtlColor == CTLCOLOR_EDIT )//|| nCtlColor == CTLCOLOR_MSGBOX)
	{
		pDC->SetBkColor(WHITECOLOR);
		return m_WhiteBrush;
	}
	
	pDC->SetBkColor(GLOBALBKCOLOR);	
	return  m_SilverBrush;
}

//void CDlgSettingRecording::OnDestroy()
//{
//	CDialog::OnDestroy();
//
//	// TODO: Add your message handler code here
//	if(DirtyFlag == true || m_DlgRecRecord->RecordDirtyFlag == true || m_DlgRecSchedule->ScheduleDirtyFlag == true)
//	{
//		int iRes = AfxMessageBox(_T("Data Already Modified, Would you want save Data ?\r\nyes save data; no restore data;"),MB_YESNOCANCEL);
//		if(iRes == IDOK)
//		{
//			OnBnClickedButtonRecordingSave();
//		}
//		else if(iRes == IDCANCEL || iRes == IDNO)
//			return;
//	}
//}

void CDlgSettingRecording::OnClose()
{
	CDialog::OnCancel();
}


void CDlgSettingRecording::OnNMDblclkListPath(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	LVHITTESTINFO info;
    info.pt = pNMItemActivate->ptAction;

    if(pPathList.SubItemHitTest(&info) != -1)
    {
        hitRow = info.iItem;
        hitCol = info.iSubItem;
		
		if (hitCol != 2)
		{
			hitRow=-1;
			return;
		}

        if(m_edit.m_hWnd == NULL)
        {
            RECT rect;
            rect.left = 0;
            rect.top = 0;
            rect.bottom = 15;
            rect.right = 200;
            m_edit.Create(WS_CHILD | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL | ES_WANTRETURN | ES_MULTILINE, rect, this, IDC_EDIT_PATH_LIST);
            m_edit.SetFont(this->GetFont(), FALSE);
        }
        CRect rect;
        pPathList.GetSubItemRect(info.iItem, info.iSubItem, LVIR_BOUNDS, rect);
        rect.top += 52;
        rect.left += 13;
        rect.right += 13;
		if(info.iSubItem == 0)
		{rect.right = 73;}
        rect.bottom += 52;

        m_edit.SetWindowText(pPathList.GetItemText(info.iItem, info.iSubItem));
        m_edit.MoveWindow(&rect, TRUE);
        m_edit.ShowWindow(1);
        m_edit.SetFocus();
		this->bShowFlag = true;
		this->DirtyFlag = true;
	}

	*pResult = 0;
}

void CDlgSettingRecording::OnNMClickListPath(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	if(m_edit.m_hWnd != NULL)
	{
        m_edit.ShowWindow(0);
        if(hitRow != -1)
        {
            CString text;
            m_edit.GetWindowText(text);
            pPathList.SetItemText(hitRow, hitCol, text);
        }
    }
    hitCol = hitRow = -1;

	*pResult = 0;
}

void CDlgSettingRecording::OnEnKillfocusEditPathList()
{
	// TODO: Add your control notification handler code here
	if(m_edit.m_hWnd != NULL)
	{
        m_edit.ShowWindow(0);
        if(hitRow != -1)
        {
            CString text;
            m_edit.GetWindowText(text);
            pPathList.SetItemText(hitRow, hitCol, text);
        }
    }
    hitCol = hitRow = -1;
}

BOOL CDlgSettingRecording::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_ToolTip != NULL)
       m_ToolTip->RelayEvent(pMsg);

	switch ( pMsg->message )
	{
		case WM_KEYDOWN:
		{
			switch( pMsg->wParam )
			{
				case VK_ESCAPE:
						//if(DirtyFlag == true || m_DlgRecRecord->RecordDirtyFlag == true || m_DlgRecSchedule->ScheduleDirtyFlag == true)
						bool bCheck(false);
						if(m_DlgRecRecord)
						{
							if(m_DlgRecRecord->RecordDirtyFlag == true) 
								bCheck = true;
						}
						if(DirtyFlag == true || bCheck == true )
						{
							//CString ls_confirm = _T("Data had been changed, do you want to save changes ?");
							int ll_ret = AfxMessageBox(IDS_SAVE_DATA, MB_YESNO|MB_ICONQUESTION);		
							if (ll_ret==IDYES) 	
							{				
								OnBnClickedButtonRecordingSave();
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

	return CDialog::PreTranslateMessage(pMsg);
}
void CDlgSettingRecording::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//if(DirtyFlag == true || m_DlgRecRecord->RecordDirtyFlag == true || m_DlgRecSchedule->ScheduleDirtyFlag == true)
	//{
	//	int iRes = AfxMessageBox(_T("Data Already Modified, Would you want to save Data ?"),MB_YESNO);
	//	if(iRes == IDOK)
	//	{
	//		OnBnClickedButtonRecordingSave();
	//	}
	//	else if(iRes == IDCANCEL || iRes == IDNO)
	//	{
	//		CDialog::OnCancel();
	//		return;
	//	}
	//}

	OnCancel();
}

void CDlgSettingRecording::OnBnClickedRadioContinuous()
{
	// TODO: Add your control notification handler code here
	DirtyFlag = true;
	//vShowSchedule(false);
	m_TimeLine.vGetEventByte(byFromTimeLine);
	bTimeArrayConvert2Storage(byFromTimeLine, 168, bySQLByte, 42);

	m_TimeLine.vSetChangeColorFlag(true);
	m_TimeLine.vFillAllRect();
	m_TimeLine.vDisable();

	GetDlgItem(IDC_RADIO_SCHEDULING)->EnableWindow(0);
	GetDlgItem(IDC_RADIO_EVENT)->EnableWindow(0);
	//pRecordingTab.ShowWindow(0);
}

void CDlgSettingRecording::OnBnClickedRadioEvent()
{
	// TODO: Add your control notification handler code here
	DirtyFlag = true;
	//vShowSchedule(true);
	m_TimeLine.vSetChangeColorFlag(false);

	bMon = false;
	bTue = false;
	bWed = false;
	bThu = false;
	bFri = false;
	bSat = false;
	bSun = false;
	//pRecordingTab.ShowWindow(0);
}

void CDlgSettingRecording::OnBnClickedRadioScheduling()
{
	// TODO: Add your control notification handler code here
	DirtyFlag = true;
	//vShowSchedule(true);
	m_TimeLine.vSetChangeColorFlag(true);

	bMon = false;
	bTue = false;
	bWed = false;
	bThu = false;
	bFri = false;
	bSat = false;
	bSun = false;
	//pRecordingTab.ShowWindow(1);
}

CString CDlgSettingRecording::csGetRecordingType(int m_iCameraIDX)
{
	CString csRecordingFlag;
	try
	{
		CString SQL_Recording_Type;
		SQL_Recording_Type.Format(_T("SELECT recording_type FROM ec_camera_recording WHERE camera_idx = '%d'"), m_iCameraIDX);

		CRecordset rRecType(pDB);
		rRecType.Open(CRecordset::forwardOnly, SQL_Recording_Type);
		while (!rRecType.IsEOF()) { 
			rRecType.GetFieldValue(_T("recording_type"), csRecordingFlag);
			csRecordingFlag.TrimRight();
			rRecType.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	//RecordDirtyFlag = false;

	return csRecordingFlag;
}

void CDlgSettingRecording::vShowSchedule(bool bShow)
{
	m_TimeLine.ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_HOUR)->ShowWindow(bShow);
	GetDlgItem(IDC_BTN_MON)->ShowWindow(bShow);
	GetDlgItem(IDC_BTN_TUE)->ShowWindow(bShow);
	GetDlgItem(IDC_BTN_WED)->ShowWindow(bShow);
	GetDlgItem(IDC_BTN_THU)->ShowWindow(bShow);
	GetDlgItem(IDC_BTN_FRI)->ShowWindow(bShow);
	GetDlgItem(IDC_BTN_SAT)->ShowWindow(bShow);
	GetDlgItem(IDC_BTN_SUN)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_SCHEDULE)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_CURSOR_COLOR)->ShowWindow(bShow);
	GetDlgItem(IDC_RADIO_SCHEDULING)->ShowWindow(bShow);
	GetDlgItem(IDC_RADIO_EVENT)->ShowWindow(bShow);

	if(!bShow)	Invalidate();

	/*HANDLE hwnd = (HANDLE)this->GetSafeHwnd();
	::UpdateWindow((HWND)hwnd);*/
}
void CDlgSettingRecording::OnBnClickedBtnMon()
{
	// TODO: Add your control notification handler code here
	bMon = !bMon;
	m_TimeLine.vFillDayRect(2, bMon);
}

void CDlgSettingRecording::OnBnClickedBtnTue()
{
	// TODO: Add your control notification handler code here
	bTue = !bTue;
	m_TimeLine.vFillDayRect(3, bTue);
}

void CDlgSettingRecording::OnBnClickedBtnWed()
{
	// TODO: Add your control notification handler code here
	bWed = !bWed;
	m_TimeLine.vFillDayRect(4, bWed);
}

void CDlgSettingRecording::OnBnClickedBtnThu()
{
	// TODO: Add your control notification handler code here
	bThu = !bThu;
	m_TimeLine.vFillDayRect(5, bThu);
}

void CDlgSettingRecording::OnBnClickedBtnFri()
{
	// TODO: Add your control notification handler code here
	bFri = !bFri;
	m_TimeLine.vFillDayRect(6, bFri);
}

void CDlgSettingRecording::OnBnClickedBtnSat()
{
	// TODO: Add your control notification handler code here
	bSat = !bSat;
	m_TimeLine.vFillDayRect(7, bSat);
}

void CDlgSettingRecording::OnBnClickedBtnSun()
{
	// TODO: Add your control notification handler code here
	bSun = !bSun;
	m_TimeLine.vFillDayRect(1, bSun);
}

bool CDlgSettingRecording::bTimeArrayConvert2Storage(BYTE *btSrc, DWORD nsrcSize, BYTE *btDesc, DWORD nDescSize)
{
	bitset <7*24*2> BitVector;

	for(int iTemp = 0; iTemp < nsrcSize; iTemp++)
	{
		switch(btSrc[iTemp])
		{
		case '0':
			BitVector[iTemp] = 0x00;
			BitVector[iTemp + 168] = 0x00;
			break;
		case '1':
			BitVector[iTemp] = 0x01;
			BitVector[iTemp + 168] = 0x00;
			break;
		case '2':
			BitVector[iTemp] = 0x00;
			BitVector[iTemp + 168] = 0x01;
			break;
		default:
			ASSERT(false);
			break;
		}
	}

	for(int iTemp = 0; iTemp < 7*24*2; iTemp++)
	{
		int iByte = iTemp/8 +1;
		int iBit = 7-(iTemp%8);

		if(BitVector[iTemp])
		{
			btDesc[iByte] |= 1 << iBit;
		}else
		{
			btDesc[iByte] |= 0 << iBit;
		}
	}


	/*bitset <7*24*2> bitvec;
	for (int ci =0; ci < 7; ci++)
    {
        for (int cj= 0; cj < 6; cj++)
        {
            for (int ck =0; ck <4; ck++)
            {
                WORD nDescIdx =  ci*24*2+ cj*4*2 + ck*2;
                WORD nSrcIdx  =  ci*24+ cj*4;

                bitvec[nDescIdx +0] = btSrc[nSrcIdx] & 0X01;
                bitvec[nDescIdx +1] = btSrc[nSrcIdx] & 0X02;
            }
        }
    }
    memcpy(btDesc, &bitvec, 7*24*2/8);*/
    return true;
}

bool CDlgSettingRecording::bStorageConvert2TimeArray(BYTE *btSrc, DWORD nSrcSize, BYTE *btDesc, DWORD nDescSize)
{
	int iDescCount = 0;
	for(int iTemp = 1; iTemp < nSrcSize; iTemp++)
	{
		for(int iBits = 0; iBits < 8; iBits++)
		{
			unsigned int bitmask = 1 << iBits;
			if(btSrc[iTemp] & bitmask)
			{
				btDesc[iDescCount] = '1';
				iDescCount++;
			}
			else
			{
				btDesc[iDescCount] = '0';
				iDescCount++;
			}
		}
	}

	return true;
}

//bool CDlgSettingRecording::bGetRecoringTypeAndBinaryTime(int iCameraIDX)
//{
//	USES_CONVERSION;
//
//	CString csTime(_T("")), csRecordingType(_T(""));
//	BYTE SQLByte[337];
//	//char cTime[44] = {0};
//
//	try
//	{
//		CString csSQL(_T(""));
//		//csSQL.Format(_T("SELECT CONVERT(varchar(43), recording_time, 0) AS BinTime, recording_type FROM ec_camera_recording WHERE camera_idx = %d"), iCameraIDX);
//		csSQL.Format(_T("SELECT recording_time, recording_type FROM ec_camera_recording WHERE camera_idx = %d"), iCameraIDX);
//
//		CRecordset rRecTypeTime(pDB);
//		rRecTypeTime.Open(CRecordset::forwardOnly, csSQL);
//		while(!rRecTypeTime.IsEOF()){
//
//			rRecTypeTime.GetFieldValue(_T("recording_time"), csTime);
//			rRecTypeTime.GetFieldValue(_T("recording_type"), csRecordingType);
//			//csBinTime.TrimRight();
//			//csRecordingType.TrimRight();
//			rRecTypeTime.MoveNext();
//		}
//	}
//	catch(CDBException * p)
//	{
//		AfxMessageBox(p->m_strError);
//		p->Delete();
//	}
//
//	BYTE byTime[43] = {0};
//	for(int iTemp = 0; iTemp < 43; iTemp++)
//	{
//		CString csHex(_T(""));
//		csHex.Format(_T("0x%s"), csTime.Mid(iTemp*2, 2));
//		byTime[iTemp] = ::_tcstol(csHex, NULL, 16);
//	}
//
//
//	//char* cTime = W2A(csTime);
//	//::wcstombs(cTime, csTime, wcslen(csTime));
//
//	if(csRecordingType == _T("Continuous"))
//	{
//		((CButton*)this->GetDlgItem(IDC_RADIO_CONTINUOUS))->SetCheck(1);
//		((CButton*)this->GetDlgItem(IDC_RADIO_SCHEDULING))->SetCheck(0);
//		((CButton*)this->GetDlgItem(IDC_RADIO_EVENT))->SetCheck(0);
//		
//		bStorageConvert2TimeArray(byTime, 43, SQLByte, 336);
//		m_TimeLine.vFillScheduleAndEventRect(SQLByte);
//
//		//m_TimeLine.vSetChangeColorFlag(true);
//		//m_TimeLine.vFillAllRect();
//		//this->UpdateWindow();
//
//	}else if(csRecordingType == _T("Scheduling"))
//	{
//		((CButton*)this->GetDlgItem(IDC_RADIO_CONTINUOUS))->SetCheck(0);
//		((CButton*)this->GetDlgItem(IDC_RADIO_SCHEDULING))->SetCheck(1);
//		((CButton*)this->GetDlgItem(IDC_RADIO_EVENT))->SetCheck(0);
//
//		bStorageConvert2TimeArray(byTime, 43, SQLByte, 336);
//		m_TimeLine.vFillScheduleAndEventRect(SQLByte);
//
//	}else if(csRecordingType == _T("Event"))
//	{
//		((CButton*)this->GetDlgItem(IDC_RADIO_CONTINUOUS))->SetCheck(0);
//		((CButton*)this->GetDlgItem(IDC_RADIO_SCHEDULING))->SetCheck(0);
//		((CButton*)this->GetDlgItem(IDC_RADIO_EVENT))->SetCheck(1);
//
//		bStorageConvert2TimeArray(byTime, 43, SQLByte, 336);
//		m_TimeLine.vFillScheduleAndEventRect(SQLByte);
//	}
//	else 
//		ASSERT(false);
//
//	return true;
//}

bool CDlgSettingRecording::bDeleteCameraRecordingInfo(int iCameraIDX)
{
	try
	{
		CString csSQL(_T(""));
		csSQL.Format(_T("DELETE FROM ec_camera_recording WHERE camera_idx = %d"), iCameraIDX);
		pDB->ExecuteSQL(csSQL);
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return true;
}

bool CDlgSettingRecording::bGetRecoringTypeAndBinaryTime(int iCameraTreeIDX)
{
    SQLLEN cb;
	SQLRETURN rc;
    //BYTE byRecordTime[43]={0};
    SQLHSTMT hstmt;
    TCHAR tzType[16] = {0};
    if (SQL_SUCCESS == (rc = ::SQLAllocHandle(SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt)))
	{
        CString csSQL;
        csSQL.Format(_T("SELECT recording_time, recording_type FROM ec_camera_recording WHERE camera_idx = %d"), iCameraIDX);
        if (SQL_SUCCESS == (rc = ::SQLPrepare(hstmt, csSQL.GetBuffer(), SQL_NTS)))
		{
			if (SQL_SUCCESS == (rc = ::SQLExecute(hstmt)))
			{
				//SDWORD cb(0);
				SQLLEN cb(0);
				SQLBindCol(hstmt, 1, SQL_C_BINARY, &veRecCameraList[iCameraTreeIDX].byteTimeArray, sizeof(veRecCameraList[iCameraTreeIDX].byteTimeArray), &cb);
				SQLBindCol(hstmt, 2, SQL_C_WCHAR, &tzType, sizeof(tzType), &cb);
				rc = SQLFetch(hstmt);
			}
		}
	}
	if (hstmt)
	{
		::SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	}
   
	
	switch(_ttoi(tzType))
	{
	case RECORDING_TYPE::CONTINOUS:

		GetDlgItem(IDC_RADIO_CONTINUOUS)->EnableWindow(1);
		GetDlgItem(IDC_RADIO_SCHEDULE_EVENT)->EnableWindow(1);
		((CButton*)this->GetDlgItem(IDC_RADIO_CONTINUOUS))->SetCheck(1);
		((CButton*)this->GetDlgItem(IDC_RADIO_SCHEDULE_EVENT))->SetCheck(0);

		//vShowSchedule(false);
		m_TimeLine.vReset();
		m_TimeLine.vFillAllRect();
		m_TimeLine.vDisable();
		m_TimeLine.m_iArrayFirstByte = CTimeLine::STATUS::CONTINUOUS;
		GetDlgItem(IDC_RADIO_SCHEDULING)->EnableWindow(0);
		GetDlgItem(IDC_RADIO_EVENT)->EnableWindow(0);
		break;
	case RECORDING_TYPE::SCHEDULING_EVENT_TRIGGER:

		GetDlgItem(IDC_RADIO_CONTINUOUS)->EnableWindow(1);
		GetDlgItem(IDC_RADIO_SCHEDULE_EVENT)->EnableWindow(1);
		((CButton*)this->GetDlgItem(IDC_RADIO_CONTINUOUS))->SetCheck(0);
		((CButton*)this->GetDlgItem(IDC_RADIO_SCHEDULE_EVENT))->SetCheck(1);

		//vShowSchedule(true);
		((CButton*)this->GetDlgItem(IDC_RADIO_SCHEDULING))->SetCheck(1);
		((CButton*)this->GetDlgItem(IDC_RADIO_EVENT))->SetCheck(0);
		m_TimeLine.vSetChangeColorFlag(true);
		m_TimeLine.vReset();
		m_TimeLine.vFillScheduleAndEventRect(veRecCameraList[iCameraTreeIDX].byteTimeArray);
		m_TimeLine.vEnable();
		m_TimeLine.m_iArrayFirstByte =  CTimeLine::STATUS::EVENT;
		GetDlgItem(IDC_RADIO_SCHEDULING)->EnableWindow(1);
		GetDlgItem(IDC_RADIO_EVENT)->EnableWindow(1);
		break;
	default:

		ASSERT(false);
		break;
	}

	return ((SQL_SUCCESS == rc) || (SQL_SUCCESS_WITH_INFO == rc));
}

bool CDlgSettingRecording::bSaveCameraBinaryTime(int iCameraIDX, BYTE* bySQLByte, CString csRecordingType)
{
    SQLRETURN rc(-1);
    SQLINTEGER cbPhotoParam;
    SQLHSTMT hstmt;

    if (SQL_SUCCESS == (rc = ::SQLAllocHandle(SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt)))
    {
        CString csSQL;
        csSQL.Format(_T("INSERT INTO ec_camera_recording(camera_idx, recording_type, recording_time) VALUES ('%d', '%s', ? )"), iCameraIDX, csRecordingType);
        rc = SQLPrepare(hstmt, csSQL.GetBuffer(), SQL_NTS);
        if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) 
        {
           // SQLINTEGER nSize(43);
			SQLLEN nSize(43);
	        SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT,	SQL_C_BINARY, SQL_BINARY, 0, 0, (SQLPOINTER) bySQLByte, 0, &nSize);
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
void CDlgSettingRecording::OnBnClickedRadioScheduleEvent()
{
	// TODO: Add your control notification handler code here
	//vShowSchedule(true);
	((CButton*)this->GetDlgItem(IDC_RADIO_SCHEDULING))->SetCheck(1);
	((CButton*)this->GetDlgItem(IDC_RADIO_EVENT))->SetCheck(0);	

	m_TimeLine.vReset();
	m_TimeLine.vSetChangeColorFlag(true);
	m_TimeLine.vEnable();

	GetDlgItem(IDC_RADIO_SCHEDULING)->EnableWindow(1);
	GetDlgItem(IDC_RADIO_EVENT)->EnableWindow(1);

	if(m_iInitFlag == NEW_CAMERA || m_iInitFlag == MODIFY_CAMERA)	return;

	HTREEITEM hSelect = pRECCameraTreeCtrl.GetSelectedItem();
	HTREEITEM hRoot = pRECCameraTreeCtrl.GetRootItem();
	iCameraTreeIDX = 0;
	while(hSelect != hRoot)
	{
		iCameraTreeIDX++;
		hRoot = pRECCameraTreeCtrl.GetNextSiblingItem(hRoot);
	}

	if(iCameraTreeIDX >= pRECCameraTreeCtrl.GetCount() && iCameraTreeIDX != 0)
	{
		CString csMsg(_T(""));
		LOADSTRING(csMsg, IDS_CAM_SELECT);
		AfxMessageBox(csMsg);
		return;
	}

	m_TimeLine.vFillScheduleAndEventRect(veRecCameraList[iCameraTreeIDX].byteTimeArray);
}

void CDlgSettingRecording::vGetCurrentRecordingPath()
{
	CRegKey regKey;
	TCHAR szCurrentPath[1024]={0};
	const DWORD len = 1024;
	DWORD nActualLen(len);
    CString csForder(CProductDefine::m_tzRegistSubFolder);


    LONG nRet = regKey.Open(HKEY_LOCAL_MACHINE, csForder.GetBuffer());
	if(nRet != ERROR_SUCCESS)
	{
		if( regKey.Create(HKEY_LOCAL_MACHINE, csForder.GetBuffer()) != 0)
		{
			TRACE(_T("Create Registry Fail."));
			return; 
		}
	}

	nRet = regKey.QueryValue(szCurrentPath, _T("Recording Path"), &nActualLen);
	if(nRet != ERROR_SUCCESS)
	{
		TRACE(_T("Registry Query Fail!\r\n"));
		return;
	}

    CString csCurrentPath, csCurrentPath2;
    LOADSTRING(csCurrentPath, IDS_CURRENT_RECORDING_PATH);
	csCurrentPath2.Format(_T("%s %s"), csCurrentPath, szCurrentPath);
    SetDlgItemText(IDC_STATIC_CURRENT_PATH, csCurrentPath2);


	//////////Larry add 102-12-20///////////////
	int nCheckCount=pPathList.GetItemCount();
	BOOL bFindDef=0;
	int nItem=0;
	CString csDef;
	csDef.Format(_T("%s"),szCurrentPath);

	while(nCheckCount>0)
	{
		CString csPath=pPathList.GetItemText(nItem, 1);
		if (csPath == csDef)
		{
			bFindDef=1;
			break;
		}
		nCheckCount--;
		nItem++;
	}

	if (!bFindDef)
	{
		//Add item and DB update//
		int nLast=pPathList.GetItemCount();
		CString csPathID;
		csPathID.Format(_T("%d"),nLast+1);
		pPathList.InsertItem(nLast, csPathID);		
		pPathList.SetItemText(nLast, 1, csDef);
		pPathList.SetItemText(nLast, 2, _T("50"));

		SetTimer(TIMER_UPDB_ec_storage,200,0);
	}
	//////////////////////////////////////////////
}

bool CDlgSettingRecording::bCheckPathBuffer(CString csPath, int iBuffer)
{
	unsigned __int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;

	GetDiskFreeSpaceEx( (LPCTSTR)csPath, (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes );
	
	if(i64FreeBytesToCaller/(1024*1024*1024) > iBuffer)	return true;
	else	return false;
}

void CDlgSettingRecording::vSetInitFlag(int iInitFlag, CString csIP, int iCameraIdx)
{
	m_iInitFlag = iInitFlag;
	m_IP = csIP;
	iCameraIDX = iCameraIdx;
}

void CDlgSettingRecording::vShowCameraInfo(int iCameraidx, CString csIP)
{
	for(int iTemp = 0; iTemp < veRecCameraList.size(); iTemp++)
	{
		if( iCameraidx == ::_ttoi(veRecCameraList[iTemp].csCameraIDX) && veRecCameraList[iTemp].csIPnName.Find(csIP) != -1 )
		{
			HTREEITEM hRoot = pRECCameraTreeCtrl.GetRootItem();
			iCameraTreeIDX = 0;
			while(iTemp != iCameraTreeIDX)
			{
				iCameraTreeIDX++;
				hRoot = pRECCameraTreeCtrl.GetNextSiblingItem(hRoot);
			}
            pRECCameraTreeCtrl.ModifyStyle(0, TVS_SHOWSELALWAYS);
			pRECCameraTreeCtrl.SelectItem(hRoot);

			if(((CButton*)this->GetDlgItem(IDC_RADIO_CONTINUOUS))->GetCheck())
			{
				BYTE byContinuousByte[43];
				::ZeroMemory(&byContinuousByte, sizeof(byContinuousByte));
				for(int iTemp = 1; iTemp <= 21; iTemp++)
				{
					byContinuousByte[iTemp] = 0xFF;
				}
				m_TimeLine.vFillScheduleAndEventRect(byContinuousByte);
			}
            
            break;
		}
	}
}

bool CDlgSettingRecording::bExistECAC(int iCameraIdx)
{
	long lRecordCount = 0;
	try
	{	
		CString csSQL;
		csSQL.Format(_T("SELECT * FROM ecac_camera WHERE camera_idx = %d"), iCameraIdx);
		CRecordset rECAC(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
		rECAC.Open(CRecordset::forwardOnly, csSQL);

		lRecordCount = rECAC.GetRecordCount();

		rECAC.Close();

		/*int iCameraID = 1;
		CString csIP, csCameraName, csCameraIDX;
		CString csSQL_Camera(_T("SELECT * FROM ecac_camera WHERE active_ = 'Y' OR active_ = NULL"));
		CRecordset rCamera(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
		rCamera.Open(CRecordset::forwardOnly, csSQL_Camera);
		while (!rCamera.IsEOF()) { 
			rCamera.GetFieldValue(_T("camera_idx"), csCameraIDX);
			rCamera.GetFieldValue(_T("cameraname"), csCameraName);
			rCamera.GetFieldValue(_T("ipaddress"), csIP);

			csCameraName.TrimRight();
			csIP.TrimRight();
			csCameraIDX.TrimRight();
			m_RecCameraList.csIPnName.Format(_T("( %d ) %s - %s"), iCameraID, csIP, csCameraName);

			m_RecCameraList.csCameraName = csCameraName;
			m_RecCameraList.csCameraIDX = csCameraIDX;
			::ZeroMemory(&m_RecCameraList.byteTimeArray, sizeof(m_RecCameraList.byteTimeArray));
			veRecCameraList.push_back(m_RecCameraList);

			iCameraID++;
			rCamera.MoveNext();
		}*/
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	if(lRecordCount > 0) return true;
	else return false;
}
void CDlgSettingRecording::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == TIMER_UPDB_ec_storage)
	{
		KillTimer(nIDEvent);

		vDeleteStoragePath();
		vSaveStoragePath();
	}

	CDialog::OnTimer(nIDEvent);
}
