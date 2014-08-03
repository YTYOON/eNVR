// DlgRecDuplicate.cpp : implementation file
//

#include "stdafx.h"
#include "DlgRecDuplicate.h"
#include "MainFrm.h"

#define WHITECOLOR RGB(255,255,255)

// CDlgRecDuplicate dialog

IMPLEMENT_DYNAMIC(CDlgRecDuplicate, CDialog)

BOOL CDlgRecDuplicate::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_SilverBrush);

	//return CBaseDialog::OnEraseBkgnd(pDC);
	return true;
}

HBRUSH CDlgRecDuplicate::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

CDlgRecDuplicate::CDlgRecDuplicate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRecDuplicate::IDD, pParent)
{
	m_SilverBrush.CreateSolidBrush(GLOBALBKCOLOR);
	m_WhiteBrush.CreateSolidBrush(WHITECOLOR);
}

CDlgRecDuplicate::~CDlgRecDuplicate()
{
	vDBClose();
}

void CDlgRecDuplicate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DUPLICATE_REC_TYPE, pRecTypeEdit);
	DDX_Control(pDX, IDC_EDIT_DUPLICATE_SCHEDULE, pRecScheduleEdit);
	//DDX_Control(pDX, IDC_LIST_DUPLICATE_CAMERA, pDuplicateCameraList);
}


BEGIN_MESSAGE_MAP(CDlgRecDuplicate, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_DUPLICATE_CAMERA, &CDlgRecDuplicate::OnLbnSelchangeListDuplicateCamera)
	ON_BN_CLICKED(IDOK, &CDlgRecDuplicate::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgRecDuplicate message handlers

BOOL CDlgRecDuplicate::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  Add extra initialization here
	m_pbk = CNVRBk::GetBkObject();

	pDuplicateCameraList = ((CCheckListBox*)(this->GetDlgItem(IDC_LIST_DUPLICATE_CAMERA)));
	//pDuplicateCameraList->SetCheckStyle( BS_3STATE );//shell be crash on 64bits OS

	CString csDialogCaption(_T(""));
	LOADSTRING(csDialogCaption,IDS_DLG_SETTING_RECORDING_DUPLICATE);
	//csDialogCaption.LoadString(CGS::g_hResStr, IDS_DLG_SETTING_RECORDING_DUPLICATE);
	this->SetWindowTextW(csDialogCaption);

	GetDlgItem(IDC_EDIT_DUPLICATE_REC_TYPE)->SetWindowTextW( m_csCameraName );
	GetDlgItem(IDC_EDIT_DUPLICATE_SCHEDULE)->SetWindowTextW( m_csRecType );

	//vDBConnect();
	//Assign the Mainfrm DB pointer 
	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
	vGetAllCameraName();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//void CDlgRecDuplicate::vDBConnect()
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

void CDlgRecDuplicate::vDBClose()
{
	//DBQuery.Close();
}

void CDlgRecDuplicate::vGetAllCameraName()
{
	pDuplicateCameraList->ResetContent();

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
			CString csIPnName;
			csIPnName.Format(_T("( %d ) %s - %s"), vcCams[nIdx].camera_idx, vcCams[nIdx].ipaddress, vcCams[nIdx].cameraname);
			//m_RecCameraList.iTreePos = nIdx+1;

			//CString csCameraName = vcCams[nIdx].cameraname;
			//m_RecCameraList.csCameraIDX.Format(_T("%d"),vcCams[nIdx].camera_idx);
			//::ZeroMemory(&m_RecCameraList.byteTimeArray, sizeof(m_RecCameraList.byteTimeArray));
			//veRecCameraList.push_back(m_RecCameraList);
																													   
			int nItem=0;
			if(csIPnName == m_csCameraName)
			{
				//pDuplicateCameraList->SetCheck(nItem, 1);
				//pDuplicateCameraList->Enable(nItem, 0);
				continue;
			}
			else
			{
				nItem= pDuplicateCameraList->AddString(csIPnName);
				pDuplicateCameraList->SetCheck(nItem, 0);
			}

			if(csIPnName.Find(_T("ISAP")) != -1)
			{
				pDuplicateCameraList->Enable(nItem, 0);
			}
		}
	}

	//try
	//{
	//	//CString SQL_CameraName(_T("SELECT cameraname FROM ec_camera ORDER BY camera_idx ASC"));

	//	CString csCameraName;
	//	CRecordset rCamName(pDB);
	//	rCamName.Open(CRecordset::forwardOnly, SQL_CameraName);
	//	while (!rCamName.IsEOF()) { 
	//		rCamName.GetFieldValue(_T("cameraname"), csCameraName);
	//		csCameraName.TrimRight();

	//		int nItem = pDuplicateCameraList->AddString(csCameraName);
	//		if(csCameraName == m_csCameraName)
	//		{
	//			pDuplicateCameraList->SetCheck(nItem, 1);
	//			pDuplicateCameraList->Enable(nItem, 0);
	//		}
	//		else
	//			pDuplicateCameraList->SetCheck(nItem, 0);

	//		if(csCameraName.Find(_T("ISAP")) != -1)
	//		{
	//			pDuplicateCameraList->Enable(nItem, 0);
	//		}

	//		rCamName.MoveNext();
	//	}
	//}
	//catch(CDBException * p)
	//{
	//	AfxMessageBox(p->m_strError);
	//	p->Delete();
	//}

}
void CDlgRecDuplicate::OnLbnSelchangeListDuplicateCamera()
{
	// TODO: Add your control notification handler code here
	int nItem = pDuplicateCameraList->GetCurSel();
	if(pDuplicateCameraList->GetCheck(nItem) == 0)
		pDuplicateCameraList->SetCheck(nItem, 1 );
	else if(pDuplicateCameraList->GetCheck(nItem) == 1)
		pDuplicateCameraList->SetCheck(nItem, 0 );
}

void CDlgRecDuplicate::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	for(int iTemp = 0; iTemp < pDuplicateCameraList->GetCount(); iTemp++)
	{
		if(pDuplicateCameraList->GetSel(iTemp) == 1)
		{
			CString csCameraName;
			pDuplicateCameraList->GetText(iTemp, csCameraName);
			veDupCamereName.push_back(csCameraName);
		}
	}

	OnOK();
}

void CDlgRecDuplicate::vSetRecInfo(CString csCameraName)
{
	m_csCameraName = csCameraName;
}

void CDlgRecDuplicate::vSetRecType(CString csRecordingType)
{
	m_csRecType = csRecordingType;
}