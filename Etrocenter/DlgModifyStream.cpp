// DlgModifyStream.cpp : implementation file
//

#include "stdafx.h"
#include "DlgModifyStream.h"
#include "MainFrm.h"

#define WHITECOLOR RGB(255,255,255)

// CDlgModifyStream dialog

IMPLEMENT_DYNAMIC(CDlgModifyStream, CDialog)

BOOL CDlgModifyStream::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_SilverBrush);

	//return CBaseDialog::OnEraseBkgnd(pDC);
	
	return true;
}

HBRUSH CDlgModifyStream::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
	//RecordDirtyFlag = false;
	return  m_SilverBrush;
}

CDlgModifyStream::CDlgModifyStream(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModifyStream::IDD, pParent)
{
	m_SilverBrush.CreateSolidBrush(GLOBALBKCOLOR);
	m_WhiteBrush.CreateSolidBrush(WHITECOLOR);
}

CDlgModifyStream::~CDlgModifyStream()
{
}

void CDlgModifyStream::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgModifyStream, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgModifyStream message handlers

BOOL CDlgModifyStream::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//vDBConnect();
	/*--Initial ComboBox--*/
	vProtocolInit();
	vResolutionInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgModifyStream::vSetMoStreamInfo(MoStreamInfo o_MoStreamInfo)
{
	//m_MoStreamInfo = o_MoStreamInfo;

	CComboBox* pProtocol = (CComboBox*) GetDlgItem(IDC_COMBO_STREAM_PROTOCOL);
	CComboBox* pResolution = (CComboBox*) GetDlgItem(IDC_COMBO_STREAM_RESOLUTION);

	if(_tcscmp(o_MoStreamInfo.tcVideoType, _T("H264")) == 0)									//Video Type
	{
		((CButton*)(GetDlgItem( IDC_RADIO_H264 )))->SetCheck( 1 );
		((CButton*)(GetDlgItem( IDC_RADIO_MPEG4 )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_MJPEG4 )))->SetCheck( 0 );
	}
	else if(_tcscmp(o_MoStreamInfo.tcVideoType, _T("MPEG4")) == 0)
	{
		((CButton*)(GetDlgItem( IDC_RADIO_H264 )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_MPEG4 )))->SetCheck( 1 );
		((CButton*)(GetDlgItem( IDC_RADIO_MJPEG4 )))->SetCheck( 0 );
	}
	else if(_tcscmp(o_MoStreamInfo.tcVideoType, _T("MJPEG")) == 0)
	{
		((CButton*)(GetDlgItem( IDC_RADIO_H264 )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_MPEG4 )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_MJPEG4 )))->SetCheck( 1 );
	}
	else
	{
		((CButton*)(GetDlgItem( IDC_RADIO_H264 )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_MPEG4 )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_MJPEG4 )))->SetCheck( 0 );
	}

	int nItem = pProtocol->FindStringExact(0, (LPCTSTR)o_MoStreamInfo.tcProtocol);		//Stream Protocol
	if(nItem == -1)
		nItem = pProtocol->FindStringExact(0, _T("Unknown"));
	pProtocol->SetCurSel(nItem);

	nItem = pResolution->FindStringExact(0, (LPCTSTR)o_MoStreamInfo.tcResolution);		//Stream Resolution
	if(nItem == -1 && sizeof(o_MoStreamInfo.tcResolution) != 0)
	{
		CString csResolution;
		csResolution.Format(_T("%s"), o_MoStreamInfo.tcResolution);
		if(csResolution.GetLength() == 0)
		{
			nItem = pResolution->FindStringExact(0, _T("Unknown"));
		}
		else
		{
			pResolution->AddString(csResolution);
			nItem = pResolution->FindStringExact(0, (LPCTSTR)o_MoStreamInfo.tcResolution);
		}
	}
	else if(nItem == -1)
	{
		nItem = pResolution->FindStringExact(0, _T("Unknown"));
	}
	pResolution->SetCurSel(nItem);

	GetDlgItem(IDC_EDIT_STREAM_PORT)->SetWindowTextW((LPCTSTR)o_MoStreamInfo.tcPort);	//Stream Port

	CString csLV_RE;
	csLV_RE.Format(_T("%s"), o_MoStreamInfo.tcLV_RE);
	
//	if(csLV_RE.Find(_T("LV")) != -1)
//		((CButton*)(GetDlgItem( IDC_CHECK_LIVEVIEW )))->SetCheck( 1 );
//	else
//		((CButton*)(GetDlgItem( IDC_CHECK_LIVEVIEW )))->SetCheck( 0 );

//	if(csLV_RE.Find(_T("RE")) != -1)
//		((CButton*)(GetDlgItem( IDC_CHECK_STREAM_RECORDING )))->SetCheck( 1 );
//	else
//		((CButton*)(GetDlgItem( IDC_CHECK_STREAM_RECORDING )))->SetCheck( 0 );
}

void CDlgModifyStream::vProtocolInit()
{
	CComboBox* pProtocol = (CComboBox*) GetDlgItem(IDC_COMBO_STREAM_PROTOCOL);

	try
	{
		CString csProtocol;
		CRecordset rProtocol(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
		rProtocol.Open(CRecordset::forwardOnly, _T("SELECT stream_protocol_desc FROM ec_hstream_protocol ORDER BY sort_order ASC"));
		while (!rProtocol.IsEOF()) { 
			rProtocol.GetFieldValue(_T("stream_protocol_desc"), csProtocol);
			
			csProtocol.TrimRight();
			pProtocol->AddString(csProtocol);
			pProtocol->SetCurSel(0);

			rProtocol.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CDlgModifyStream::vResolutionInit()
{
	CComboBox* pResolution = (CComboBox*) GetDlgItem(IDC_COMBO_STREAM_RESOLUTION);

	try
	{
		CString csResolution;
		CRecordset rResolution(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
		rResolution.Open(CRecordset::forwardOnly, _T("SELECT resolution_desc FROM ec_hresolution ORDER BY sort_order ASC"));
		while (!rResolution.IsEOF()) { 
			rResolution.GetFieldValue(_T("resolution_desc"), csResolution);
			
			csResolution.TrimRight();
			pResolution->AddString(csResolution);
			pResolution->SetCurSel(0);

			rResolution.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

//bool CDlgModifyStream::vDBConnect(void)
//{
//	CIniReader m_ini(ECINI);
//	CString cpDns = m_ini.getKeyValue(_T("DSN"),_T("database"));
//	CString cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
//	CString cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
// 
//	CString ls_connect;
//	ls_connect.Format(_T("ODBC;UID=%s;PWD=%s"), cpUid, cpPwd);
//	return (((CMainFrame*)AfxGetMainWnd())->m_DB).Open((LPCTSTR)cpDns, FALSE, FALSE, ls_connect );
//}

MoStreamInfo CDlgModifyStream::mGetMoStreamInfo()
{
	_tcscpy(m_MoStreamInfo.tcLV_RE ,_T(""));
	_tcscpy(m_MoStreamInfo.tcPort ,_T(""));
	_tcscpy(m_MoStreamInfo.tcProtocol ,_T(""));
	_tcscpy(m_MoStreamInfo.tcResolution ,_T(""));
	_tcscpy(m_MoStreamInfo.tcVideoType ,_T(""));

	if(((CButton*)(GetDlgItem( IDC_RADIO_H264 )))->GetCheck() == 1)
		_tcscpy(m_MoStreamInfo.tcVideoType ,_T("H264"));
	else if(((CButton*)(GetDlgItem( IDC_RADIO_MPEG4 )))->GetCheck() == 1)
		_tcscpy(m_MoStreamInfo.tcVideoType ,_T("MPEG4"));
	else if(((CButton*)(GetDlgItem( IDC_RADIO_MJPEG4 )))->GetCheck() == 1)
		_tcscpy(m_MoStreamInfo.tcVideoType ,_T("MJPEG"));
	else
		_tcscpy(m_MoStreamInfo.tcVideoType ,_T(""));

	CComboBox* pProtocol = (CComboBox*) GetDlgItem(IDC_COMBO_STREAM_PROTOCOL);
	int nIndex = pProtocol->GetCurSel();
	pProtocol->GetLBText(nIndex, (LPTSTR)(LPCTSTR)m_MoStreamInfo.tcProtocol);

	CComboBox* pResolution = (CComboBox*) GetDlgItem(IDC_COMBO_STREAM_RESOLUTION);
	nIndex = pResolution->GetCurSel();
	pResolution->GetLBText(nIndex, (LPTSTR)(LPCTSTR)m_MoStreamInfo.tcResolution);

	CString csTemp;
	GetDlgItem(IDC_EDIT_STREAM_PORT)->GetWindowTextW(csTemp);
	_tcscpy(m_MoStreamInfo.tcPort ,csTemp);

//	CString csLV_RE;

//	if(((CButton*)(GetDlgItem( IDC_CHECK_LIVEVIEW )))->GetCheck() == 1)
//		csLV_RE = csLV_RE + _T("LV");

//	if(((CButton*)(GetDlgItem( IDC_CHECK_STREAM_RECORDING )))->GetCheck() == 1)
//		csLV_RE = csLV_RE + _T("RE");

//	_tcscpy(m_MoStreamInfo.tcLV_RE ,csLV_RE);

	return m_MoStreamInfo;
}