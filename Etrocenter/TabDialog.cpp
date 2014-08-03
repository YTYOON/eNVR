// TabDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TabDialog.h"


// CTabDialog dialog

IMPLEMENT_DYNAMIC(CTabDialog, CDialog)

CTabDialog::CTabDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTabDialog::IDD, pParent)
{

}

CTabDialog::~CTabDialog()
{
}

void CTabDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(CTabDialog, CDialog)
END_MESSAGE_MAP()


// CTabDialog message handlers
