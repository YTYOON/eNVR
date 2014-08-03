// ResourceMgr.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "ResourceMgr.h"
#include "ProductDefine.h"

HINSTANCE CResourceMgr::g_hResStr;

DWORD CResourceMgr::m_nLocaleID;


// CResourceMgr

CResourceMgr::CResourceMgr()
{
}

CResourceMgr::~CResourceMgr()
{
}

void CResourceMgr::vQuerynGetDLL()
{
	if(bGetLocaleID() == false) // initial
	{
		LCID lcid  = GetSystemDefaultLCID();
		m_nLocaleID = lcid;
		bSetLocaleID(m_nLocaleID);	
	}
	
	if (m_nLocaleID == ENUS_LCID)
		return;
	
	CString csDLLName = csLCIDtoDLL(m_nLocaleID);
	if(csDLLName.IsEmpty()) // no supported dll, use default	
		return;
    TRACE(_T("Resource Dll Name: %s\r\n"), csDLLName);
	g_hResStr = ::LoadLibrary(csDLLName);
    if (g_hResStr == NULL)
    {
        TRACE(_T("LoadLibrary Fail Error Code: %d\r\n"), GetLastError());
    }
	bSetLocaleID(m_nLocaleID);
}
CString CResourceMgr::csLCIDtoDLL(LCID lcid)
{
	
	WORD LangID = LANGIDFROMLCID(lcid);
	BYTE nPrimID,nSubID;

	/*nPrimID = PRIMARYLANGID( LangID );
	nSubID  = SUBLANGID( LangID );*/

	nPrimID = LOBYTE( LangID );
	nSubID  = HIBYTE( LangID );
	
	// get app path
	CString csFindFile, csTargetName(_T("")),csAppPath,csLingualName(_T("")), csFind,csDLL,csPrim(_T(""));
	TCHAR apppath[MAX_PATH]={0};
    GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
    csAppPath.Format(_T("%s"),apppath);
    int nPos = csAppPath.ReverseFind('\\');
    csAppPath = csAppPath.Left(nPos + 1); 
	
	csDLL.Format(_T("Lingual*.dll"));
	//csFindFile.Format(_T("Lingual*%02X.dll"), nPrimID);
	csTargetName.Format(_T("Lingual%02X%02X.dll"), nSubID, nPrimID);	

	csFind = csAppPath + csTargetName;
	
	CFileFind finder;

	if(finder.FindFile(csFind) == true)
	{
		TRACE(_T("%s\r\n"),csTargetName);
		return csTargetName;
	}
	else
	{
		bool bFound = false;
		csFind.Empty();
		csFind = csAppPath + csDLL;
		csPrim.Format(_T("%X"),nPrimID);
		if (finder.FindFile(csFind) == true) 
		{ 
			while (finder.FindNextFileW())
			{
				csLingualName = finder.GetFileName();
				if ( csLingualName.Find(csPrim) > 0 )
				{
					bFound = true;
	    			break;
				}
			}
		}
		TRACE(_T("%s\r\n"),csLingualName);
		
		if(!bFound) 
		{
			csLingualName.Empty();
		}
		return csLingualName;
	}
}
bool CResourceMgr::bSetLocaleID(LCID lcid)
{	
	bool bResult = false;
	HKEY hkFolder;
	DWORD dwDisposition = REG_CREATED_NEW_KEY;
	int nRet;

	if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, NULL,  _T("LocaleID"),
		                   REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
	{		
		int ret = RegSetValueEx(hkFolder, _T("LocaleID"), NULL, REG_DWORD, (BYTE*) &lcid, sizeof(DWORD));
		m_nLocaleID = lcid;
		nRet = ::GetLastError();
	}

	RegCloseKey(hkFolder);
	return bResult;
}

bool CResourceMgr::bGetLocaleID()
{
	HKEY hkFolder; 
	LCID lcid = 0;
	DWORD cbData = 0;
	DWORD nType = REG_DWORD;
	bool bResult = false;
	
	LONG nRet1 = RegOpenKey(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, &hkFolder);
	cbData = 0;
	LONG nRet2 = RegQueryValueEx(hkFolder, _T("LocaleID"), NULL, &nType, (BYTE*)&lcid, &cbData);
	if ((cbData % sizeof (LCID)))
	{
		RegDeleteValue(hkFolder, _T("LocaleID"));
		return false;
	}
	DWORD nApplySize = cbData/sizeof(LCID);
	nRet2 = RegQueryValueEx(hkFolder, _T("LocaleID"), NULL, &nType, (BYTE*)&lcid, &cbData);
	if ( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS )
	{
		m_nLocaleID = lcid;
		bResult = true;
	}
	else
	{
		FORMATMESSAGE(nRet2);
	}
	RegCloseKey(hkFolder);
	return bResult;
}

