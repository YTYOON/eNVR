// RecordTimeLine.cpp : implementation file
//

#include "stdafx.h"
#include "RecordTimeLine.h"

#define IDC_CAPTIONBAR 1999

// CRecordTimeLine dialog

IMPLEMENT_DYNAMIC(CRecordTimeLine, CDialog)

CRecordTimeLine::CRecordTimeLine(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordTimeLine::IDD, pParent)
{

}

CRecordTimeLine::~CRecordTimeLine()
{
}

void CRecordTimeLine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRecordTimeLine, CDialog)
END_MESSAGE_MAP()


// CRecordTimeLine message handlers

BOOL CRecordTimeLine::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_MAXIMIZE);

	CRect rect;
	GetClientRect(rect);
	rect.DeflateRect(15, 15);
	m_TimeLine.Create(NULL, WS_CHILD|WS_VISIBLE|SS_NOTIFY, rect, this , IDC_CAPTIONBAR);
	
	m_TimeLine.vInitial();
	m_TimeLine.vSetGridRect();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
