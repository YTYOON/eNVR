#pragma once
#include "stdafx.h"
#include "resource.h"
#include "TimeLine.h"

// CRecordTimeLine dialog

class CRecordTimeLine : public CDialog
{
	DECLARE_DYNAMIC(CRecordTimeLine)

public:
	CRecordTimeLine(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRecordTimeLine();

// Dialog Data
	enum { IDD = IDD_DLG_TIMELINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	HICON m_hIcon;

	CTimeLine m_TimeLine;
	virtual BOOL OnInitDialog();
};
