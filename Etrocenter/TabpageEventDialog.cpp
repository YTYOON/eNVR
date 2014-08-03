// TabpageEventDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TabpageEventDialog.h"


// CTabpageEventDialog dialog

IMPLEMENT_DYNAMIC(CTabpageEventDialog, CDialog)

CTabpageEventDialog::CTabpageEventDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTabpageEventDialog::IDD, pParent)
{

}

CTabpageEventDialog::~CTabpageEventDialog()
{
}

void CTabpageEventDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabpageEventDialog, CDialog)
END_MESSAGE_MAP()


// CTabpageEventDialog message handlers
