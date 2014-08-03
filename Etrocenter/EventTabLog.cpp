// EventTabLog.cpp : implementation file
//

#include "stdafx.h"
#include "EventTabLog.h"


// CEventTabLog dialog

IMPLEMENT_DYNAMIC(CEventTabLog, CDialog)

CEventTabLog::CEventTabLog(CWnd* pParent /*=NULL*/)
	: CDialog(CEventTabLog::IDD, pParent)
{

}

CEventTabLog::~CEventTabLog()
{
}

void CEventTabLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEventTabLog, CDialog)
END_MESSAGE_MAP()


// CEventTabLog message handlers
