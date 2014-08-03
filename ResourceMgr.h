#pragma once

// CResourceMgr command target

class CResourceMgr : public CObject
{
public:
	static HINSTANCE g_hResStr;
	static DWORD m_nLocaleID;
public:
	CResourceMgr();
	virtual ~CResourceMgr();
	void vQuerynGetDLL();
	bool CResourceMgr::bSetLocaleID(LCID lcid);

private:
	CString csLCIDtoDLL(LCID lcid);
	bool bGetLocaleID();



};


