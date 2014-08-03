#pragma once


// CBaseStatic

class CBaseStatic : public CStatic
{
	DECLARE_DYNAMIC(CBaseStatic)

public:
	CBaseStatic();
	virtual ~CBaseStatic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	// store camreagroupcameraid
	int	m_id;
	int	m_camidx;
	CString	m_lpr;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


