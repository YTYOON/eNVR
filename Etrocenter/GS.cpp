// GS.cpp: implementation of the CGS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GS.h"
//#include "IPCamDiscovery.h"
#include "INI.h"
#include "MainFrm.h"

#define RUNREGSUBFOLDER _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include <list>
using namespace std;

//static CString csLastRecordPath, csLastRecordSort;
//#define RECORDING_BUFFER 10240 //Mega Byte

//vector <KEEP_AUTH_INFO> CGS::m_authinfo;


//TCHAR CGS::m_tzBrand[](BRAND);
//TCHAR CGS::m_tzProductName[](PRODUCTNAME);
//TCHAR CGS::m_tzPlaybackName[](PLAYBACKNAME);
//TCHAR CGS::m_tzPrefix[](PREFIX);
//TCHAR CGS::m_tzRegistSubFolder[](REGISTSUBFOLDER);
CString CGS::csCacheFolder;
//HINSTANCE CGS::g_hResStr = NULL;
LCID CGS::m_nLocaleID = 0;
vector <int> CGS::m_vctnOSDBtnConfig;
CGS::CGS()
{
	//m_authinfo.reserve(256);
}

CGS::~CGS()
{

}
void  CGS::LogEvent(WORD nType, LPCTSTR pFormat, ...)
{
//	::EnterCriticalSection(&myCS);
    TCHAR    chMsg[256];
    HANDLE  hEventSource;
    LPTSTR  lpszStrings[1];
    va_list pArg;
    va_start(pArg, pFormat);
    _vstprintf(chMsg, pFormat, pArg);
    va_end(pArg);
    lpszStrings[0] = chMsg;
    hEventSource = RegisterEventSource(NULL, AfxGetAppName());
    if (hEventSource != NULL)
    {
        ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
        DeregisterEventSource(hEventSource);
		hEventSource = NULL;
    }
//	::LeaveCriticalSection(&myCS);
}
//
//bool CGS::bGetKeepInfo()
//{
//	vGetAppCacheFolder();
//	HKEY hkFolder; 
//	DWORD cbData = 0;
//	DWORD nType = REG_BINARY;
//	bool bResult = false;
//	
//	KEEP_AUTH_INFO* pAuthinfo =NULL; 
//	LONG nRet1 = RegOpenKey(HKEY_CURRENT_USER, REGISTSUBFOLDER, &hkFolder);
//	cbData = 0;//sizeof(KEEP_AUTH_INFO) * 50;
//	LONG nRet2 = RegQueryValueEx(hkFolder, _T("AUTHORIZATION"), NULL, &nType, (BYTE*)pAuthinfo, &cbData);
//	DWORD nApplySize = cbData/sizeof(KEEP_AUTH_INFO);
//	pAuthinfo = new KEEP_AUTH_INFO[nApplySize];
//	memset(pAuthinfo, 0, nApplySize);
//	nRet2 = RegQueryValueEx(hkFolder, _T("AUTHORIZATION"), NULL, &nType, (BYTE*)pAuthinfo, &cbData);
//
//	if ( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS )
//	{
//		m_authinfo.clear();
//		int nSize =  cbData/sizeof(KEEP_AUTH_INFO);
//		for (int ci=0; ci < nSize; ci++)
//		{			
//			try{
//
//				m_authinfo.push_back(pAuthinfo[ci]);
//			}
//			catch(exception* e){
//				delete e;
//				m_authinfo.clear();
//				delete [] pAuthinfo;
//				return false;
//			}		
//		}
//
//		bResult = true;
//	}else
//	{
//		FORMATMESSAGE(nRet2);
//	}
//	try
//	{
//		delete [] pAuthinfo;
//	}catch(exception* e)
//	{
//		m_authinfo.clear();
//		delete e;
//	}
//	RegCloseKey(hkFolder);
//	return bResult;
//}

//bool CGS::bSetKeepInfo()
//{
//	bool bResult = false;
//	HKEY hkFolder;
//	DWORD dwDisposition = REG_CREATED_NEW_KEY;
//	if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_CURRENT_USER, REGISTSUBFOLDER, NULL,  _T("AUTHORIZATION"),
//		                   REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
//	{
//		DWORD nSize = m_authinfo.size();
//		KEEP_AUTH_INFO* pInfo = new KEEP_AUTH_INFO[nSize];
//		memset(pInfo, 0, sizeof(KEEP_AUTH_INFO)* nSize);
//		for (int ci=0; ci < nSize; ci++)
//		{
//			memcpy(&pInfo[ci], &m_authinfo[ci], sizeof(KEEP_AUTH_INFO));
//		}
//		RegSetValueEx(hkFolder, _T("AUTHORIZATION"), NULL, REG_BINARY, (BYTE*) pInfo, nSize * sizeof(KEEP_AUTH_INFO));
//		delete pInfo;
//	}
//	RegCloseKey(hkFolder);
//	return bResult;
//}
bool CGS::bSetPatrolSeqDuration(vector< map <int,int>> vctmapPatrolSeqDuration)
{
	bool bResult = false;
	HKEY hkFolder;
	DWORD dwDisposition = REG_CREATED_NEW_KEY;

	if(vctmapPatrolSeqDuration.size() == 0)// empty vector, clear registry value
	{		
		 DWORD cbData = 0;
		 int nRegSize(0);
		 int *pnSize = NULL;
		 DWORD nType = REG_BINARY;
		 LONG nRet1 = RegOpenKey(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, &hkFolder);
		 LONG nRet2 = RegQueryValueEx(hkFolder, _T("PATROLSEQDURATIONSIZE"), NULL, &nType, (BYTE*)pnSize, &cbData);	
		 DWORD nApplySize = cbData/sizeof(int);
		 if(nApplySize == 0) return false;
		 pnSize = new int[nApplySize];
		 memset(pnSize, 0, nApplySize);
		 nRet2 = RegQueryValueEx(hkFolder, _T("PATROLSEQDURATIONSIZE"), NULL, &nType, (BYTE*)pnSize, &cbData);

		if ( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS )
		{
			int nSize =  cbData/sizeof(int);
			for (int ci=0; ci < nSize; ci++)
			{			
				nRegSize = pnSize[ci];					
			}
		}
		
		CRegKey regKey;
		if( regKey.Open(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder) != ERROR_SUCCESS)
		{
			TRACE(_T("CRegKey Open Error!"));
			return false;
		}

		for(int i = 0 ;i<nRegSize;i++)
		{	
			CString csKeyName;
			csKeyName.Format(_T("PATROLSEQDURATION%d"),i);
			regKey.DeleteValue(csKeyName.GetBuffer());
		}
		regKey.DeleteValue(_T("PATROLSEQDURATIONSIZE"));

		delete [] pnSize;
		regKey.Close();
		return true;

	}

	// store vector size into registry
	if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, NULL,  _T("PATROLSEQDURATIONSIZE"),
		                   REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
	{
		int nSize = vctmapPatrolSeqDuration.size();		
		RegSetValueEx(hkFolder, _T("PATROLSEQDURATIONSIZE"), NULL, REG_BINARY, (BYTE*) &nSize,  sizeof(int));		
	}
	
	//store map value into registry
	for(int i=0;i<vctmapPatrolSeqDuration.size();i++)
	{
		map<int,int>::iterator it;
		map<int,int> tmpMap =  vctmapPatrolSeqDuration[i];
		vector<int> vctTmp;

		for(it = tmpMap.begin();it!= tmpMap.end();it++) // constructing map into array
		{
			if(it->first !=0)
			{
				vctTmp.push_back(it->first);
				vctTmp.push_back(it->second);
			}			
		}
		
		DWORD nSize = vctTmp.size();
		if(nSize == 0) continue;

		int* tmpArray = new int [nSize];

		copy(vctTmp.begin(),vctTmp.end(),tmpArray);
		CString csKeyName;
		csKeyName.Format(_T("PATROLSEQDURATION%d"),i);
	
		if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, NULL, (LPWSTR)(LPCTSTR)(csKeyName) ,
								   REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
		{				
				RegSetValueEx(hkFolder, (LPWSTR)(LPCTSTR)(csKeyName), NULL, REG_BINARY, (BYTE*)tmpArray, nSize * sizeof(int));		
		}
	}

	RegCloseKey(hkFolder);
	return bResult;
}

bool CGS::bGetPatrolSeqDuration(vector< map <int,int>> &vctmapPatrolSeqDuration)
{
	HKEY hkFolder; 
	DWORD cbData = 0;
	DWORD nType = REG_BINARY;
	bool bResult = false;
	int nRegSize(0);

	int *pnSize = NULL;
    LONG nRet1 = RegOpenKey(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, &hkFolder);
	cbData = 0;
	LONG nRet2 = RegQueryValueEx(hkFolder, _T("PATROLSEQDURATIONSIZE"), NULL, &nType, (BYTE*)pnSize, &cbData);	
	
	DWORD nApplySize = cbData/sizeof(int);

	if(nApplySize == 0) return false;

	pnSize = new int[nApplySize];
	memset(pnSize, 0, nApplySize);
	nRet2 = RegQueryValueEx(hkFolder, _T("PATROLSEQDURATIONSIZE"), NULL, &nType, (BYTE*)pnSize, &cbData);

	if ( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS )
	{
		int nSize =  cbData/sizeof(int);
		for (int ci=0; ci < nSize; ci++)
		{			
			nRegSize = pnSize[ci];					
		}
	}
	if( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS && nRegSize != 0)
	{
		vctmapPatrolSeqDuration.clear();
		for(int i = 0 ;i<nRegSize;i++)
		{	
			CString csKeyName;
			csKeyName.Format(_T("PATROLSEQDURATION%d"),i);
			int *pnSeqDuration = NULL;
			cbData = 0;
			LONG nRet3 = RegQueryValueEx(hkFolder, (LPWSTR)(LPCTSTR)(csKeyName), NULL, &nType, (BYTE*)pnSeqDuration, &cbData);
			DWORD nApplySize = cbData/sizeof(int);
			pnSeqDuration = new int[nApplySize];
			memset(pnSeqDuration, 0, nApplySize);
			if( nRet3 == ERROR_SUCCESS)
			{
				nRet3 = RegQueryValueEx(hkFolder, (LPWSTR)(LPCTSTR)(csKeyName), NULL, &nType, (BYTE*)pnSeqDuration, &cbData);
				if( nRet3 == ERROR_SUCCESS)
				{
					map<int,int> mapTmp;
					for(int j=0;j<cbData/sizeof(int);j+=2)
					{					
						mapTmp.insert(pair<int,int>(pnSeqDuration[j],pnSeqDuration[j+1]));
					}
					vctmapPatrolSeqDuration.push_back(mapTmp);
				}
			}
			delete [] pnSeqDuration;
		}
		bResult = true;
	}
	else
	{
		FORMATMESSAGE(nRet2);
	}
	
	RegCloseKey(hkFolder);

	delete [] pnSize;
	
	return bResult;
}


bool CGS::bSetPatrolSeqDuration(vector< map <int,pair_INT>> vctmapPatrolSeqDuration,int nDelKeySN)
{
	bool bResult = false;
	HKEY hkFolder;
	DWORD dwDisposition = REG_CREATED_NEW_KEY;

	if(vctmapPatrolSeqDuration.size() == 0)// empty vector, clear registry value
	{		
		DWORD cbData = 0;
		int nRegSize(0);
		int *pnSize = NULL;
		DWORD nType = REG_BINARY;
		LONG nRet1 = RegOpenKey(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, &hkFolder);
		LONG nRet2 = RegQueryValueEx(hkFolder, _T("PATROLSEQDURATIONSIZE"), NULL, &nType, (BYTE*)pnSize, &cbData);	
		DWORD nApplySize = cbData/sizeof(int);
		if(nApplySize == 0) return false;
		pnSize = new int[nApplySize];
		memset(pnSize, 0, nApplySize);
		nRet2 = RegQueryValueEx(hkFolder, _T("PATROLSEQDURATIONSIZE"), NULL, &nType, (BYTE*)pnSize, &cbData);

		if ( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS )
		{
			int nSize =  cbData/sizeof(int);
			for (int ci=0; ci < nSize; ci++)
			{			
				nRegSize = pnSize[ci];					
			}
		}

		CRegKey regKey;
		if( regKey.Open(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder) != ERROR_SUCCESS)
		{
			TRACE(_T("CRegKey Open Error!"));
			return false;
		}

		for(int i = 0 ;i<nRegSize;i++)
		{	
			CString csKeyName;
			csKeyName.Format(_T("PATROLSEQDURATION%d"),i);
			regKey.DeleteValue(csKeyName.GetBuffer());
		}
		regKey.DeleteValue(_T("PATROLSEQDURATIONSIZE"));

		delete [] pnSize;
		regKey.Close();
		return true;

	}

	if (nDelKeySN>-1)
	{
		CRegKey regDelKey;
		if( regDelKey.Open(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder) == ERROR_SUCCESS)
		{
			CString csDelKey;
			csDelKey.Format(_T("PATROLSEQDURATION%d"),nDelKeySN);
			regDelKey.DeleteValue(csDelKey.GetBuffer());			
		}
		regDelKey.Close();
	}

	// store vector size into registry
	if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, NULL,  _T("PATROLSEQDURATIONSIZE"),
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
	{
		int nSize = vctmapPatrolSeqDuration.size();		
		RegSetValueEx(hkFolder, _T("PATROLSEQDURATIONSIZE"), NULL, REG_BINARY, (BYTE*) &nSize,  sizeof(int));		
	}

	//store map value into registry
	for(int i=0;i<vctmapPatrolSeqDuration.size();i++)
	{
		map<int,pair_INT>::iterator it;
		map<int,pair_INT> tmpMap =  vctmapPatrolSeqDuration[i];
		vector<int> vctTmp;

		for(it = tmpMap.begin();it!= tmpMap.end();it++) // constructing map into array
		{
			if(it->first !=0 )
			{
				vctTmp.push_back(it->second.first);
				vctTmp.push_back(it->second.second);
			}			
		}

		DWORD nSize = vctTmp.size();
		if(nSize == 0) continue;

		int* tmpArray = new int [nSize];

		copy(vctTmp.begin(),vctTmp.end(),tmpArray);
		CString csKeyName;
		csKeyName.Format(_T("PATROLSEQDURATION%d"),i);

		if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, NULL, (LPWSTR)(LPCTSTR)(csKeyName) ,
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
		{				
			RegSetValueEx(hkFolder, (LPWSTR)(LPCTSTR)(csKeyName), NULL, REG_BINARY, (BYTE*)tmpArray, nSize * sizeof(int));		
		}
	}

	RegCloseKey(hkFolder);
	return bResult;
}

bool CGS::bGetPatrolSeqDuration(vector< map <int,pair_INT>> &vctmapPatrolSeqDuration)
{
	HKEY hkFolder; 
	DWORD cbData = 0;
	DWORD nType = REG_BINARY;
	bool bResult = false;
	int nRegSize(0);

	int *pnSize = NULL;
	LONG nRet1 = RegOpenKey(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, &hkFolder);
	cbData = 0;
	LONG nRet2 = RegQueryValueEx(hkFolder, _T("PATROLSEQDURATIONSIZE"), NULL, &nType, (BYTE*)pnSize, &cbData);	

	DWORD nApplySize = cbData/sizeof(int);

	if(nApplySize == 0) return false;

	pnSize = new int[nApplySize];
	memset(pnSize, 0, nApplySize);
	nRet2 = RegQueryValueEx(hkFolder, _T("PATROLSEQDURATIONSIZE"), NULL, &nType, (BYTE*)pnSize, &cbData);

	if ( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS )
	{
		int nSize =  cbData/sizeof(int);
		for (int ci=0; ci < nSize; ci++)
		{			
			nRegSize = pnSize[ci];					
		}
	}
	if( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS && nRegSize != 0)
	{
		vctmapPatrolSeqDuration.clear();
		for(int i = 0 ;i<nRegSize;i++)
		{	
			CString csKeyName;
			csKeyName.Format(_T("PATROLSEQDURATION%d"),i);
			int *pnSeqDuration = NULL;
			cbData = 0;
			LONG nRet3 = RegQueryValueEx(hkFolder, (LPWSTR)(LPCTSTR)(csKeyName), NULL, &nType, (BYTE*)pnSeqDuration, &cbData);
			DWORD nApplySize = cbData/sizeof(int);
			pnSeqDuration = new int[nApplySize];
			memset(pnSeqDuration, 0, nApplySize);
			if( nRet3 == ERROR_SUCCESS)
			{
				nRet3 = RegQueryValueEx(hkFolder, (LPWSTR)(LPCTSTR)(csKeyName), NULL, &nType, (BYTE*)pnSeqDuration, &cbData);
				if( nRet3 == ERROR_SUCCESS)
				{
					map<int,pair_INT> mapTmp;
					int nIdx=0;
					for(int j=0;j<cbData/sizeof(int);j+=2)
					{
						nIdx++;
						pair_INT p(pnSeqDuration[j],pnSeqDuration[j+1]);
						mapTmp.insert(pair<int,pair_INT>(nIdx,p));
					}
					vctmapPatrolSeqDuration.push_back(mapTmp);
				}
			}
			delete [] pnSeqDuration;
		}
		bResult = true;
	}
	else
	{
		FORMATMESSAGE(nRet2);
	}

	RegCloseKey(hkFolder);

	delete [] pnSize;

	return bResult;
}

bool CGS::bSetPatrolSeqDuration(map <int,pair_INT> mapPatrolSeqDuration,BYTE mac[6])
{
	bool bResult = false;
	HKEY hkFolder;
	DWORD dwDisposition = REG_CREATED_NEW_KEY;
	CString csSubFolder=CProductDefine::m_tzRegistSubFolder;
	csSubFolder+=L"PATROLSEQDURATION\\";

	CString csMac;
	csMac.Format(_T("%02X%02X%02X%02X%02X%02X"),mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

	if(mapPatrolSeqDuration.size() == 0)// empty mapPatrolSeqDuration, create registry value
	{		
		LONG nRet1 = RegOpenKey(HKEY_CURRENT_USER, csSubFolder, &hkFolder);
		if (nRet1==ERROR_SUCCESS)
		{
			//if(ERROR_SUCCESS ==RegCreateKeyEx(HKEY_CURRENT_USER, csSubFolder, NULL, csMac.GetBuffer(),
			//	REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
			//{
			//	int nSize = mapPatrolSeqDuration.size();
			//	RegSetValueEx(hkFolder, csMac, NULL, REG_BINARY, (BYTE*) &nSize,  sizeof(int));
			//}
			
			CRegKey RKDelKey;
			LONG nDel = RKDelKey.Open(HKEY_CURRENT_USER, csSubFolder, KEY_ALL_ACCESS);
			if (nDel==ERROR_SUCCESS)
			{
				nDel = RKDelKey.DeleteValue(csMac);
			}			
			RKDelKey.Close();
		}
		RegCloseKey(hkFolder);
		return bResult;
	}

	//store map data value into registry
	map<int,pair_INT>::iterator it;
	map<int,pair_INT> tmpMap = mapPatrolSeqDuration;
	vector<int> vctTmp;

	for(it = tmpMap.begin();it!= tmpMap.end();it++) // constructing map into array
	{
		if (it->first!=0)
		{
			vctTmp.push_back(it->second.first);
			vctTmp.push_back(it->second.second);
		}		
	}

	DWORD nSize = vctTmp.size();
	int* tmpArray = new int [nSize];
	copy(vctTmp.begin(),vctTmp.end(),tmpArray);

	if(ERROR_SUCCESS ==RegCreateKeyEx(HKEY_CURRENT_USER, csSubFolder, NULL, csMac.GetBuffer(),
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
	{				
		RegSetValueEx(hkFolder, csMac, NULL, REG_BINARY, (BYTE*)tmpArray, nSize * sizeof(int));
	}

	bResult=true;
	RegCloseKey(hkFolder);
	return bResult;
}

bool CGS::bGetPatrolSeqDuration(map <int,pair_INT> &mapPatrolSeqDuration,BYTE mac[6])
{
	HKEY hkFolder; 
	DWORD cbData = 0;
	DWORD nType = REG_BINARY;
	bool bResult = false;
	//int nRegSize(0);
	mapPatrolSeqDuration.clear();
	CString csSubFolder=CProductDefine::m_tzRegistSubFolder;
	csSubFolder+=L"PATROLSEQDURATION\\";

	CString csMac;
	csMac.Format(_T("%02X%02X%02X%02X%02X%02X"),mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

	LONG nRet1 = RegOpenKey(HKEY_CURRENT_USER, csSubFolder, &hkFolder);
	int *pnSize = NULL;
	cbData = 0;
	LONG nRet2 = RegQueryValueEx(hkFolder, csMac, NULL, &nType, (BYTE*)pnSize, &cbData);

	DWORD nApplySize = cbData/sizeof(int);

	if(nApplySize == 0) return false;

	pnSize = new int[nApplySize];
	memset(pnSize, 0, nApplySize);
	nRet2 = RegQueryValueEx(hkFolder, csMac, NULL, &nType, (BYTE*)pnSize, &cbData);

	/*if ( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS )
	{
		int nSize =  cbData/sizeof(int);
		for (int ci=0; ci < nSize; ci++)
		{			
			nRegSize = pnSize[ci];					
		}
	}*/

	int nIdx=0;
	for(int j=0;j<cbData/sizeof(int);j+=2)
	{
		nIdx++;
		pair_INT p(pnSize[j],pnSize[j+1]);
		mapPatrolSeqDuration.insert(pair<int,pair_INT>(nIdx,p));		
	}
	
	bResult=true;
	RegCloseKey(hkFolder);
	delete [] pnSize;
	return bResult;
}

bool CGS::bSetGroupAutoPlayIdx(vector<int> vctnGroupIdx)
{
	bool bResult = false;
	HKEY hkFolder;
	DWORD dwDisposition = REG_CREATED_NEW_KEY;
	
	if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, NULL,  _T("GROUPAUTOSCANIDX"),
		                   REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
	{
		DWORD nSize = vctnGroupIdx.size();
		int * pnIdx = new int[nSize];

		for(int i=0;i<vctnGroupIdx.size();i++)
		{
			pnIdx[i] = (vctnGroupIdx[i]);
		}
		
		RegSetValueEx(hkFolder, _T("GROUPAUTOSCANIDX"), NULL, REG_BINARY, (BYTE*) pnIdx, nSize * sizeof(int));
		delete [] pnIdx;
	}

	RegCloseKey(hkFolder);
	return bResult;
}
bool CGS::bGetGroupAutoScanIdx(vector<int> & vctnGroupIdx)
{
	HKEY hkFolder; 
	DWORD cbData = 0;
	DWORD nType = REG_BINARY;
	bool bResult = false;
	
	int* pnIdx = NULL; 
    LONG nRet1 = RegOpenKey(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, &hkFolder);
	cbData = 0;
	LONG nRet2 = RegQueryValueEx(hkFolder, _T("GROUPAUTOSCANIDX"), NULL, &nType, (BYTE*)pnIdx, &cbData);
	if ((cbData % sizeof (int)))
	{
		RegDeleteValue(hkFolder, _T("GROUPAUTOSCANIDX"));
		return false;
	}

	DWORD nApplySize = cbData/sizeof(int);
	pnIdx = new int[nApplySize];
	memset(pnIdx, 0, nApplySize);
	nRet2 = RegQueryValueEx(hkFolder, _T("GROUPAUTOSCANIDX"), NULL, &nType, (BYTE*)pnIdx, &cbData);

	if ( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS )
	{
		vctnGroupIdx.clear();
		int nSize =  cbData/sizeof(int);
		for (int ci=0; ci < nSize; ci++)
		{			
			try{

				vctnGroupIdx.push_back((pnIdx[ci]));
			}
			catch(exception* e){
				delete e;
				vctnGroupIdx.clear();
				delete [] pnIdx;
				return false;
			}		
		}

		bResult = true;
	}else
	{
		FORMATMESSAGE(nRet2);
	}
	try
	{
		delete [] pnIdx;
	}catch(exception* e)
	{
		vctnGroupIdx.clear();
		delete e;
	}
	RegCloseKey(hkFolder);
	return bResult;
}

bool CGS::bSetNodeItem(vector <NODEITEM*> vctpNode)
{
	bool bResult = false;
	HKEY hkFolder;
	DWORD dwDisposition = REG_CREATED_NEW_KEY;

	if(!m_vctNode.empty()) m_vctNode.clear();

	if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, NULL,  _T("NODEITEM"),
		                   REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
	{
		DWORD nSize = vctpNode.size();
		NODEITEM * pNode = new NODEITEM[nSize];
		for(int i=0;i<vctpNode.size();i++)
		{
			vctpNode[i]->hPatrolThread=NULL;
			vctpNode[i]->hAutoScanThread=NULL;
			m_vctNode.push_back(*(vctpNode[i]));
			pNode[i] = (*(vctpNode[i]));
		}
		
		RegSetValueEx(hkFolder, _T("NODEITEM"), NULL, REG_BINARY, (BYTE*) pNode, nSize * sizeof(NODEITEM));
		delete [] pNode;
	}

	RegCloseKey(hkFolder);
	return bResult;
}

bool CGS::bGetNodeItem()
{
	//vGetAppCacheFolder();
	HKEY hkFolder; 
	DWORD cbData = 0;
	DWORD nType = REG_BINARY;
	bool bResult = false;
	
	NODEITEM *pNode = NULL; 
    LONG nRet1 = RegOpenKey(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, &hkFolder);
	cbData = 0;//sizeof(KEEP_AUTH_INFO) * 50;
	LONG nRet2 = RegQueryValueEx(hkFolder, _T("NODEITEM"), NULL, &nType, (BYTE*)pNode, &cbData);
	if ((cbData % sizeof (NODEITEM)))
	{
		RegDeleteValue(hkFolder, _T("NODEITEM"));
		return false;
	}

	DWORD nApplySize = cbData/sizeof(NODEITEM);
	pNode = new NODEITEM[nApplySize];
	memset(pNode, 0, nApplySize);
	nRet2 = RegQueryValueEx(hkFolder, _T("NODEITEM"), NULL, &nType, (BYTE*)pNode, &cbData);

	if ( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS )
	{
		m_vctNode.clear();
		int nSize =  cbData/sizeof(NODEITEM);
		USES_CONVERSION;

		for (int ci=0; ci < nSize; ci++)
		{			
			try{

				pNode[ci].hPatrolThread=NULL;
				pNode[ci].hAutoScanThread=NULL;

				CString csIP;
				csIP.Format( _T("%s"), A2W(pNode[ci].ip) );
				if (!csIP.IsEmpty() && _wtoi(csIP) > 0)
				{
					m_vctNode.push_back((pNode[ci]));
				}				
			}
			catch(exception* e){
				delete e;
				m_vctNode.clear();
				delete [] pNode;
				return false;
			}		
		}

		bResult = true;
	}else
	{
		FORMATMESSAGE(nRet2);
	}
	try
	{
		delete [] pNode;
	}catch(exception* e)
	{
		m_vctNode.clear();
		delete e;
	}
	RegCloseKey(hkFolder);
	return bResult;

}

bool CGS::bSetRun(bool bEnable)
{	
	HKEY hkFolder;
	DWORD dwDisposition = REG_CREATED_NEW_KEY;
	if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, RUNREGSUBFOLDER, NULL, CProductDefine::m_tzProductName,
		                   REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
	{
		if(bEnable)
		{
			TCHAR apppath[MAX_PATH]={0};
			GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
			const BYTE* lpData = (const BYTE*)(LPCTSTR) apppath;
			RegSetValueEx(hkFolder, CProductDefine::m_tzProductName, NULL, REG_SZ, (BYTE*)apppath ,  (_tcslen(apppath)+1)*2 );
		}
		else
		{
			LONG nRet = RegDeleteValue(hkFolder, CProductDefine::m_tzProductName);
			FORMATMESSAGE(nRet);
		}
	}
	RegCloseKey(hkFolder);
	return true;
}

bool CGS::bGetRun()
{
	HKEY hkFolder; 
	bool bResult = true;
	DWORD cbData = 0;
	DWORD nType = REG_BINARY;
	 TCHAR* execPath =NULL; 

	LONG nRet1 = RegOpenKey(HKEY_LOCAL_MACHINE, RUNREGSUBFOLDER, &hkFolder);
	LONG nRet2 = RegQueryValueEx(hkFolder, CProductDefine::m_tzProductName, NULL, &nType, (BYTE*)execPath, &cbData);
	if ( nRet1 == ERROR_SUCCESS && nRet2 == ERROR_SUCCESS )
	{		
		bResult = true;

	}else
	{
		FORMATMESSAGE(nRet1);
		bResult = false;
	}
	
	RegCloseKey(hkFolder);
	return bResult;

}
//void CGS::vTraceItem(NODEITEM *pItem)
//{
//	USES_CONVERSION;
//	vector <NODEITEM*>& Vt = *(CIPCamDiscovery::GetNodePtr());
//	if (&Vt)
//	{
//		int nSize = Vt.size();
//		for (int ci=0; ci < nSize;  ci++)
//		{
//			TRACE(_T("*** NODE: %d, %s"), ci, A2W(Vt[ci]->ip));
//		}
//	}
//	TRACE(_T("*** NODE: %s"), A2W(pItem->ip));
//}

bool CGS::bSaveGroupName(CString* pcsGroup, BYTE nSize)
{
	bool bResult = false;
	HKEY hkFolder;
	int ci;
	DWORD dwDisposition = REG_CREATED_NEW_KEY;
	if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, NULL,  _T("GroupName"),
		                   REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
	{
		DWORD nBufferSize =0;
		for (ci=0; ci < nSize; ci++)
		{
			nBufferSize = nBufferSize + pcsGroup[ci].GetLength() + 1;
		}
		TCHAR* wchBuffer = new TCHAR[nBufferSize]();
		DWORD nIndex =0;
		for (ci =0; ci < nSize; ci++)
		{
			wcsncpy( &wchBuffer[nIndex], pcsGroup[ci].LockBuffer(), pcsGroup[ci].GetLength());
			nIndex = nIndex + pcsGroup[ci].GetLength() + 1;
		}
		RegSetValueEx(hkFolder, _T("GroupName"), NULL, REG_BINARY, (BYTE*) wchBuffer, nIndex * sizeof(TCHAR));
		delete [] wchBuffer;
	}
	return bResult;
}

bool CGS::bGetGroupName(CString csGroup[], BYTE* nSize)
{
	HKEY hkFolder; 
	DWORD cbData = 0;
	DWORD nType = REG_BINARY;
	bool bResult = false;
	int ci=0;
	
	TCHAR* pBuffer =NULL; 
	LONG nRet1 = RegOpenKey(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, &hkFolder);
	cbData = 0;
	LONG nRet2 = RegQueryValueEx(hkFolder, _T("GroupName"), NULL, &nType, (BYTE*)pBuffer, &cbData);
	DWORD nApplySize = cbData/ sizeof(TCHAR);
	pBuffer = new TCHAR[nApplySize]();
	nRet2 = RegQueryValueEx(hkFolder, _T("GroupName"), NULL, &nType, (BYTE*)pBuffer, &cbData);

	if ( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS )
	{
		int nIndex = 0;
		while (nIndex < nApplySize && ci < *nSize)
		{
			csGroup[ci] = &pBuffer[nIndex];
			nIndex = nIndex + csGroup[ci].GetLength() +1;
			ci++;
		}
		*nSize = ci;
		bResult = true;
	}else
	{
		FORMATMESSAGE(nRet2);
	}
	try
	{
		delete [] pBuffer;
	}catch(exception* e)
	{
		delete e;
	}
	RegCloseKey(hkFolder);
	return bResult;
}

void CGS::GradientFillRect( CDC *pDC, CRect &rect, COLORREF col_from, COLORREF col_to, DWORD dwMode )
{
	TRIVERTEX        vert[2];
	GRADIENT_RECT    mesh;

	vert[0].x      = rect.left;
	vert[0].y      = rect.top;
	vert[0].Alpha  = 0x0000;
	vert[0].Blue   = GetBValue(col_from) << 8;
	vert[0].Green  = GetGValue(col_from) << 8;
	vert[0].Red    = GetRValue(col_from) << 8;

	vert[1].x      = rect.right;
	vert[1].y      = rect.bottom; 
	vert[1].Alpha  = 0x0000;
	vert[1].Blue   = GetBValue(col_to) << 8;
	vert[1].Green  = GetGValue(col_to) << 8;
	vert[1].Red    = GetRValue(col_to) << 8;

	mesh.UpperLeft  = 0;
	mesh.LowerRight = 1;
#if _MSC_VER >= 1300  // only VS7 and above has GradientFill as a pDC member
	pDC->GradientFill( vert, 2, &mesh, 1, dwMode );
#else
	GradientFill( pDC->m_hDC, vert, 2, &mesh, 1, dwMode );
#endif
}


BOOL CGS::bCreateHierarchyDir(LPCTSTR lpPathName)
{
	BOOL bRet = false;
	CString csTmp = lpPathName;
	CFileFind finder;
	vector <int> nIdxs;
	nIdxs.push_back(csTmp.GetLength());
	while (!finder.FindFile(csTmp))
	{
		int n = csTmp.ReverseFind(_T('\\'));
		if (n >0)
		{
			nIdxs.push_back(n);
			csTmp = csTmp.Left(n);
		}else
		{
            break;
		}
	}
	for (int ci = nIdxs.size()-2; ci >=0; ci--)
	{
		TCHAR tzDir[MAX_PATH]={0};
		_tcsncpy_s(tzDir, lpPathName, nIdxs[ci]);
		bRet = CreateDirectory(tzDir, NULL);
		if (bRet == FALSE)
		{
			int nErr = ::GetLastError();
			if ( nErr == ERROR_ALREADY_EXISTS)
			{
				continue;
			}
			return false;
		}
	}
	return bRet;
}
CString CGS::csGetAppStoreFolder()
{
    TCHAR szPath[MAX_PATH];
	CString csFolder;
    if(!SUCCEEDED(::SHGetFolderPath(
        NULL,       // use the desktop for the owner window
        CSIDL_APPDATA,     // current CSIDL to look up
        NULL,       // use the token of the logged in user
        SHGFP_TYPE_CURRENT, // get the current value
        szPath)))   // the buffer to hold the path
	{
		return CString("");
	}
	csFolder.Format(_T("%s\\%s"),szPath, CProductDefine::m_tzBrand);
	WIN32_FIND_DATA FindFileData = {0};
    HANDLE hFind = ::FindFirstFile(csFolder.LockBuffer(), &FindFileData);
    if(INVALID_HANDLE_VALUE == hFind)
	{
		if (!SUCCEEDED (::_wmkdir( csFolder.LockBuffer() )))
			return CString("");
	}
	csFolder += _T("\\Cache");
    hFind = ::FindFirstFile(csFolder.LockBuffer(), &FindFileData);
    if(INVALID_HANDLE_VALUE == hFind)
	{
		if (!SUCCEEDED (::_wmkdir( csFolder.LockBuffer() )))
			return CString("");
	}
	FindClose( hFind );
	return csFolder;
}

void CGS::vGetAppCacheFolder()
{
    TCHAR szPath[MAX_PATH];
    if(!SUCCEEDED(::SHGetFolderPath(
        NULL,       // use the desktop for the owner window
        CSIDL_MYPICTURES,     // current CSIDL to look up
        NULL,       // use the token of the logged in user
        SHGFP_TYPE_CURRENT, // get the current value
        szPath)))   // the buffer to hold the path
	{
		return;
	}
	csCacheFolder.Format(_T("%s\\%s"),szPath, CProductDefine::m_tzBrand);
	WIN32_FIND_DATA FindFileData = {0};
    HANDLE hFind = ::FindFirstFile(csCacheFolder.LockBuffer(), &FindFileData);
    if(INVALID_HANDLE_VALUE == hFind)
	{
		if (!SUCCEEDED (::_wmkdir( csCacheFolder.LockBuffer() )))
			return;
	}
	csCacheFolder += _T("\\Cache");
    hFind = ::FindFirstFile(csCacheFolder.LockBuffer(), &FindFileData);
    if(INVALID_HANDLE_VALUE == hFind)
	{
		if (!SUCCEEDED (::_wmkdir( csCacheFolder.LockBuffer() )))
			return;
	}
	FindClose( hFind );
	return;

}

void CGS::PrintOutLogFile(CString csLog)
{

}

bool CGS::bCleanRegistry()
{
	bool bResult = false;
	HKEY hkFolder;
	int nRet;
	//m_authinfo.clear();
	DWORD dwDisposition = REG_CREATED_NEW_KEY;
	if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, NULL,  _T("AUTHORIZATION"),
		                   REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
	{
		nRet = RegDeleteValue(hkFolder,  _T("AUTHORIZATION"));
		FORMATMESSAGE(nRet);
	}
	RegCloseKey(hkFolder);
	return bResult = nRet == ERROR_SUCCESS ? true : false;
}


bool CGS::bGetRecordFolder(CString &csFolder)
{
	/*
	unsigned __int64 i64FreeBytesToCaller=0, i64TotalBytes=0, i64FreeBytes=0;
	CString csRecordingSort;
	CString csPath, csSortOrder, csPathCount, csNextPath;

#ifndef _DEBUG
	CIniReader m_ini;
	m_ini.setINIFullFileName(_T("\ec.ini"));
#else
	CIniReader m_ini(ECINI);
#endif

	CString cpDns = m_ini.getKeyValue(_T("DSN"),_T("database")); 
	CString cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
	CString cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
 
	CString ls_connect;
	ls_connect.Format(_T("ODBC;UID=%s;PWD=%s"), cpUid, cpPwd);
	CDatabase m_DB;
	m_DB.Open((LPCTSTR)cpDns, FALSE, FALSE, ls_connect );

	try
	{
		//--Get Store Path Count
		CRecordset rPathCount(&m_DB);
		rPathCount.Open(CRecordset::forwardOnly, _T("SELECT COUNT(DISTINCT store_location) AS path_count FROM ec_storage;"));
		while (!rPathCount.IsEOF()) { 
			rPathCount.GetFieldValue(_T("path_count"), csPathCount);
			csPathCount.TrimRight();
			rPathCount.MoveNext();
		}
		rPathCount.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	if(csLastRecordPath.GetLength() == 0)
	{
		try{
				//--Get First Recording Path
				CRecordset rPath(&m_DB);
				rPath.Open(CRecordset::forwardOnly, _T("SELECT * FROM ec_storage ORDER BY sort_order"));
				while (!rPath.IsEOF()) { 
					rPath.GetFieldValue(_T("store_location"), csPath);
					rPath.GetFieldValue(_T("sort_order"), csSortOrder);

					csPath.TrimRight();
					csSortOrder.TrimRight();

					//--GetDiskFreeSpace
					GetDiskFreeSpaceEx( (LPCTSTR)csPath, (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes );

					if(i64FreeBytesToCaller/(1024*1024) > RECORDING_BUFFER)
					{
						csFolder = csPath;
						break;
					}
					rPath.MoveNext();
				}
				rPath.Close();
			}
			catch(CDBException * p)
			{
				AfxMessageBox(p->m_strError);
				p->Delete();
			}
	
		//--Mark the Last Recording Path
		csFolder = csPath;
		csLastRecordPath = csFolder;
		csLastRecordSort = csSortOrder;

		//--Get Next Recording Path
		csRecordingSort = csSortOrder;
		int iPathCount = ::_ttoi(csPathCount);
		int iSortOrder = ::_ttoi(csSortOrder);
		int iRecordSort = ::_ttoi(csRecordingSort);
		iRecordSort = iRecordSort + 1;

		while(true)
		{
			if(iRecordSort > iPathCount)
			{iRecordSort = 1;}

			if(iRecordSort == iSortOrder) 
			{	
				csNextPath = csGetStoragePath(iRecordSort, &m_DB);
				break;
			}

			csNextPath = csGetStoragePath(iRecordSort, &m_DB);

			if(csNextPath.GetAt(0) != csPath.GetAt(0))
			{break;}

			iRecordSort++;
		}

		//--Check Next Path is enough to Record or not
		GetDiskFreeSpaceEx( (LPCTSTR)csNextPath, (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes );
		if(i64FreeBytesToCaller/(1024*1024) <= RECORDING_BUFFER)
		{
			vDeleteRecordFile(csNextPath);	
		}
	}
	else
	{
		//--Check the Last Recording Path
		GetDiskFreeSpaceEx( (LPCTSTR)csLastRecordPath, (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes );
		if(i64FreeBytesToCaller/(1024*1024) <= RECORDING_BUFFER)
		{
			//--Get Next Recording Path
			int iPathCount = ::_ttoi(csPathCount);
			int iSortOrder = ::_ttoi(csLastRecordSort);
			int iRecordSort = ::_ttoi(csLastRecordSort);
			
			iRecordSort++;
			while(true)
			{
				if(iRecordSort > iPathCount)
				{iRecordSort = 1;}

				if(iRecordSort == iSortOrder) 
				{	
					csNextPath = csGetStoragePath(iRecordSort, &m_DB);
					break;
				}

				csNextPath = csGetStoragePath(iRecordSort, &m_DB);

				if(csNextPath.GetAt(0) != csPath.GetAt(0))
				{break;}

				iRecordSort++;
			}
			
			csFolder = csNextPath;
			csLastRecordPath = csFolder;

			CString csStaticTemp;
			csStaticTemp.Format(_T("%d"), iRecordSort);
			csLastRecordSort = csStaticTemp;

			//--Get Next Recording Path
			iSortOrder = iRecordSort;

			iRecordSort++;
			while(true)
			{
				if(iRecordSort > iPathCount)
				{iRecordSort = 1;}

				if(iRecordSort == iSortOrder) 
				{	
					csNextPath = csGetStoragePath(iRecordSort, &m_DB);
					break;
				}

				csNextPath = csGetStoragePath(iRecordSort, &m_DB);

				if(csNextPath.GetAt(0) != csPath.GetAt(0))
				{break;}

				iRecordSort++;
			}

			//--Check Next Path is enough to Record or not
			GetDiskFreeSpaceEx( (LPCTSTR)csNextPath, (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes );
			if(i64FreeBytesToCaller/(1024*1024) <= RECORDING_BUFFER)
			{
				vDeleteRecordFile(csNextPath);	
			}
		}
		else
		{
			csFolder = csLastRecordPath;
			
			//--Get Next Recording Path
			int iPathCount = ::_ttoi(csPathCount);
			int iSortOrder = ::_ttoi(csLastRecordSort);
			int iRecordSort = ::_ttoi(csLastRecordSort);

			iRecordSort++;
			while(true)
			{
				if(iRecordSort > iPathCount)
				{iRecordSort = 1;}

				if(iRecordSort == iSortOrder) 
				{	
					csNextPath = csGetStoragePath(iRecordSort, &m_DB);
					break;
				}

				csNextPath = csGetStoragePath(iRecordSort, &m_DB);

				if(csNextPath.GetAt(0) != csPath.GetAt(0))
				{break;}

				iRecordSort++;
			}

			//--Check Next Path is enough to Record or not
			GetDiskFreeSpaceEx( (LPCTSTR)csNextPath, (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes );
			if(i64FreeBytesToCaller/(1024*1024) <= RECORDING_BUFFER)
			{
				vDeleteRecordFile(csNextPath);	
			}
		}
	}
	
	csFolder = _T("C:\\Recording"); 
	CString DEFAULT_RECORDING_FOLDER = csFolder;
	CString csRoot;
	HKEY hkFolder; 
	DWORD cbData = 0;
	DWORD nType = REG_BINARY;
	
	int ci=0;
	CFileFind finder;
	TCHAR* pBuffer =NULL; 
	LONG nRet1 = RegOpenKey(HKEY_CURRENT_USER, CGS::m_tzRegistSubFolder, &hkFolder);
	cbData = 0;
	LONG nRet2 = RegQueryValueEx(hkFolder, _T("RecordingFolder"), NULL, &nType, (BYTE*)pBuffer, &cbData);
	DWORD nApplySize = cbData/ sizeof(TCHAR);
	pBuffer = new TCHAR[nApplySize];
	memset(pBuffer, 0, nApplySize);
	nRet2 = RegQueryValueEx(hkFolder, _T("RecordingFolder"), NULL, &nType, (BYTE*)pBuffer, &cbData);
	if ( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS )
	{
		csRoot = pBuffer;
	}else
	{
		csRoot = DEFAULT_RECORDING_FOLDER;
	}
	do
	{
		if (!finder.FindFile(csRoot))
		{
			TRACE(csRoot);
			int nRet;
			nRet = GetFileAttributes(csRoot);
			if (FILE_ATTRIBUTE_DIRECTORY != (GetFileAttributes(csRoot) & FILE_ATTRIBUTE_DIRECTORY))
			{	
				if (CreateDirectory(csRoot, NULL) == false)
				{
					break;
				}
			}
		}
		SYSTEMTIME hosttime;
		GetLocalTime(&hosttime);
		csFolder.Format(_T("%s\\%04d"), csRoot, hosttime.wYear);
		if (!finder.FindFile(csFolder))
		{
			TRACE(csFolder);
			if (CreateDirectory(csFolder, NULL) == false)
				break;	
		}
		csFolder.Format(_T("%s\\%04d\\%02d"), csRoot, hosttime.wYear,  hosttime.wMonth);
		if (!finder.FindFile(csFolder))
		{
			TRACE(csFolder);
			if (CreateDirectory(csFolder, NULL) == false)
				break;	
		}
		csFolder.Format(_T("%s\\%04d\\%02d\\%02d"), csRoot, hosttime.wYear, hosttime.wMonth, hosttime.wDay);
		if (!finder.FindFile(csFolder))
		{
			TRACE(csFolder);
			if (CreateDirectory(csFolder, NULL) == false) 
				break;	
		}
		bResult = true;
	}while (false);
	try
	{
		delete [] pBuffer;
	}catch(exception* e)
	{
		delete e;
	}
	RegCloseKey(hkFolder);
	//m_DB.Close();*/
	bool bResult = false;
	return bResult;
}

 
bool CGS::vSetProtocolValue( CString op_value)
{

	bool bResult = false;
	HKEY hkFolder;
	DWORD dwDisposition = REG_CREATED_NEW_KEY;
	if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, NULL,  _T("PROTOCOL"),
		                   REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
	{
	 
		const BYTE* lpData = (const BYTE*)(LPCTSTR)op_value;

		RegSetValueEx(hkFolder, _T("PROTOCOL"), NULL, REG_SZ, (BYTE*)lpData, sizeof(op_value) * 2);  

		//delete pInfo;
	}

	RegCloseKey(hkFolder);
	return bResult;	 
}


bool CGS::vGetProtocol(CString &o_Protocol)
{
	const TCHAR DEFAULT_PROTOCOL[] =_T("VSMP");
	HKEY hkFolder; 
	DWORD cbData = 0;
	DWORD nType = REG_BINARY;
	bool bResult = false;
	int ci=0;
	
	TCHAR* pBuffer =NULL; 
	LONG nRet1 = RegOpenKey(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, &hkFolder);
	cbData = 0;
	LONG nRet2 = RegQueryValueEx(hkFolder, _T("Protocol"), NULL, &nType, (BYTE*)pBuffer, &cbData);
	DWORD nApplySize = cbData/ sizeof(TCHAR);
	pBuffer = new TCHAR[nApplySize]();
	nRet2 = RegQueryValueEx(hkFolder, _T("Protocol"), NULL, &nType, (BYTE*)pBuffer, &cbData);

	if ( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS )
		o_Protocol = pBuffer;

	RegCloseKey(hkFolder);
	return bResult;

}
#define MSGID _T("[&~`]")
void CGS::ETROINFO(const TCHAR* szFile, const TCHAR* szFunction, long nLine, const TCHAR* pszFmt, ...)
{
	va_list ptr; va_start(ptr, pszFmt);
	CString csMsg;
	csMsg.Format(_T("%s0x%X %s %s %d ::"), MSGID, GetCurrentThreadId(), szFile, szFunction, nLine);

	csMsg.AppendFormatV(pszFmt, ptr);
    csMsg.Append(_T("\r\n"));
	OutputDebugString(csMsg);
	va_end(ptr);
}
void CGS::vQuerynGetDLL()
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
    CProductDefine::g_hResStr = ::LoadLibrary(csDLLName);
    if ( CProductDefine::g_hResStr == NULL)
    {
        TRACE(_T("LoadLibrary Fail Error Code: %d\r\n"), GetLastError());
    }
	bSetLocaleID(m_nLocaleID);
}
CString CGS::csLCIDtoDLL(LCID lcid)
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
bool CGS::bSetLocaleID(LCID lcid)
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

bool CGS::bGetLocaleID()
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
bool CGS::bGetOSDConfigFromRegistry(void)
{
	HKEY hkFolder; 
	DWORD cbData = 0;
	DWORD nType = REG_BINARY;
	bool bResult = false;
	
	int* pnIdx = NULL; 
    LONG nRet1 = RegOpenKey(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, &hkFolder);
	cbData = 0;
	LONG nRet2 = RegQueryValueEx(hkFolder, _T("OSDCONFIG"), NULL, &nType, (BYTE*)pnIdx, &cbData);
	if ((cbData % sizeof (int)))
	{
		RegDeleteValue(hkFolder, _T("OSDCONFIG"));
		return false;
	}

	DWORD nApplySize = cbData/sizeof(int);
	pnIdx = new int[nApplySize];
	memset(pnIdx, 0, nApplySize);
	nRet2 = RegQueryValueEx(hkFolder, _T("OSDCONFIG"), NULL, &nType, (BYTE*)pnIdx, &cbData);

	if ( nRet1 == ERROR_SUCCESS && nRet2  == ERROR_SUCCESS )
	{
		m_vctnOSDBtnConfig.clear();
		int nSize =  cbData/sizeof(int);
		for (int ci=0; ci < nSize; ci++)
		{		
			m_vctnOSDBtnConfig.push_back((pnIdx[ci]));			
		}

		bResult = true;
	}
	else
	{
		//TRACE(nRet2);
	}
	RegCloseKey(hkFolder);
	return bResult;	
}

bool CGS::bSetOSDConfigToRegistry(void)
{
	bool bResult = false;
	HKEY hkFolder;
	DWORD dwDisposition = REG_CREATED_NEW_KEY;

	if(m_vctnOSDBtnConfig.size() == 0 ) return bResult;
	
	if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_CURRENT_USER, CProductDefine::m_tzRegistSubFolder, NULL,  _T("OSDCONFIG"),
		                   REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
	{
		DWORD nSize = m_vctnOSDBtnConfig.size();
		int * pnIdx = new int[nSize];

		for(int i=0;i<m_vctnOSDBtnConfig.size();i++)
		{
			pnIdx[i] = (m_vctnOSDBtnConfig[i]);
		}
		
		RegSetValueEx(hkFolder, _T("OSDCONFIG"), NULL, REG_BINARY, (BYTE*) pnIdx, nSize * sizeof(int));
		delete [] pnIdx;
		bResult = true;
	}

	RegCloseKey(hkFolder);
	return bResult;
}


/*====================================================
	CCrypto
======================================================*/

CCrypto::CCrypto() : m_hCryptProv(NULL), m_hKey(NULL), m_hHash(NULL)
{
	//	Create the Crypt context.
	//if(!::CryptAcquireContext(&m_hCryptProv, NULL, NULL, PROV_RSA_FULL, 0)) 
	if(!::CryptAcquireContext(&m_hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))	//Fix For Vista/Win7 by Hank
		return;

	//	Create an empty hash object.
	if(!::CryptCreateHash(m_hCryptProv, CALG_MD5, 0, 0, &m_hHash)) 
		return;

	//	Memory files are opened automatically on construction, we don't
	//	explcitly call open.
}

//	Destructor, frees Crypto stuff.
CCrypto::~CCrypto()
{
	//	Close the file.
	m_file.Close();

	// Clean up.
	if(m_hHash)
		::CryptDestroyHash(m_hHash);

	if(m_hKey)
		::CryptDestroyKey(m_hKey);

	if(m_hCryptProv)
		::CryptReleaseContext(m_hCryptProv, 0);
}

//	Derive a key from a password.
bool CCrypto::DeriveKey(CString strPassword)
{
	//	Return failure if we don't have a context or hash.
	if(m_hCryptProv == NULL || m_hHash == NULL)
		return false;

	//	If we already have a hash, trash it.
	if(m_hHash)
	{
		CryptDestroyHash(m_hHash);
		m_hHash = NULL;
		if(!CryptCreateHash(m_hCryptProv, CALG_MD5, 0, 0, &m_hHash)) 
			return false;
	}

	//	If we already have a key, destroy it.
	if(m_hKey)
	{
		::CryptDestroyKey(m_hKey);
		m_hKey = NULL;
	}

	//	Hash the password. This will have a different result in UNICODE mode, as it
	//	will hash the UNICODE string (this is by design, allowing for UNICODE passwords, but
	//	it's important to be aware of this behaviour.
	if(!CryptHashData(m_hHash, (const BYTE*)strPassword.GetString(), strPassword.GetLength() * sizeof(TCHAR), 0)) 
		return false;
	
	//	Create a session key based on the hash of the password.
	if(!CryptDeriveKey(m_hCryptProv, CALG_RC2, m_hHash, CRYPT_EXPORTABLE, &m_hKey))
		return false;

	//	And we're done.
	return true;
}

bool CCrypto::Encrypt(const CObject& serializable, CByteArray& arData)
{
	//	Return failure if we don't have a context or key.
	if(m_hCryptProv == NULL || m_hKey == NULL)
		return false;

	//	Return failure if the object is not serializable.
	if(serializable.IsSerializable() == FALSE)
		return false;

	//	Before we write to the file, trash it.
	m_file.SetLength(0);

	//	Create a storing archive from the memory file.
	CArchive ar(&m_file, CArchive::store);

	//	We know that serialzing an object will not change it's data, as we can
	//	safely use a const cast here.

	//	Write the data to the memory file.
	const_cast<CObject&>(serializable).Serialize(ar);
	
	//	Close the archive, flushing the write.
	ar.Close();

	//	Encrypt the contents of the memory file and store the result in the array.
	return InternalEncrypt(arData);
}

bool CCrypto::Decrypt(const CByteArray& arData, CObject& serializable)
{
	//	Return failure if we don't have a context or key.
	if(m_hCryptProv == NULL || m_hKey == NULL)
		return false;

	//	Return failure if the object is not serializable.
	if(serializable.IsSerializable() == FALSE)
		return false;

	//	Decrypt the contents of the array to the memory file.
	if(InternalDecrypt(arData) == false)
		return false;

	//	Create a reading archive from the memory file.
	CArchive ar(&m_file, CArchive::load);

	//	Read the data from the memory file.
	serializable.Serialize(ar);
	
	//	Close the archive.
	ar.Close();

	//	And we're done.
	return true;
}

bool CCrypto::Encrypt(const CString& str, CByteArray& arData)
{
	//	Return failure if we don't have a context or key.
	if(m_hCryptProv == NULL || m_hKey == NULL)
		return false;

	//	Before we write to the file, trash it.
	m_file.SetLength(0);

	//	Create a storing archive from the memory file.
	CArchive ar(&m_file, CArchive::store);

	//	Write the string to the memory file.
	ar << str;

	//	Close the archive, flushing the write.
	ar.Close();

	//	Encrypt the contents of the memory file and store the result in the array.
	return InternalEncrypt(arData);
}

bool CCrypto::Decrypt(const CByteArray& arData, CString& str)
{
	//	Return failure if we don't have a context or key.
	if(m_hCryptProv == NULL || m_hKey == NULL)
		return false;

	//	Decrypt the contents of the array to the memory file.
	if(InternalDecrypt(arData) == false)
		return false;

	//	Create a reading archive from the memory file.
	CArchive ar(&m_file, CArchive::load);

	//	Read the data from the memory file.
	ar >> str;
	
	//	Close the archive.
	ar.Close();

	//	And we're done.
	return true;
}

bool CCrypto::InternalEncrypt(CByteArray& arDestination)
{
	//	Get the length of the data in memory. Increase the capacity to handle the size of the encrypted data.
	ULONGLONG uLength = m_file.GetLength();
	ULONGLONG uCapacity = uLength * 2;
	m_file.SetLength(uCapacity);

	//	Acquire direct access to the memory.
	BYTE* pData = m_file.Detach();

	//	We need a DWORD to tell encrypt how much data we're encrypting.
	DWORD dwDataLength = static_cast<DWORD>(uLength);

	//	Now encrypt the memory file.
	if(!::CryptEncrypt(m_hKey, NULL, TRUE, 0, pData, &dwDataLength, static_cast<DWORD>(uCapacity)))
	{
		//	Free the memory we release from the memory file.
		delete [] pData;

		return false;
	}	

	//	Assign all of the data we have encrypted to the byte array- make sure anything 
	//	already in the array is trashed first.
	arDestination.RemoveAll();
	arDestination.SetSize(static_cast<INT_PTR>(dwDataLength));
	memcpy(arDestination.GetData(), pData, dwDataLength);

	//	Free the memory we release from the memory file.
	delete [] pData;

	return true;
}

bool CCrypto::InternalDecrypt(const CByteArray& arSource)
{
	//	Trash the file.
	m_file.SetLength(0);

	//	Write the contents of the byte array to the memory file.
	m_file.Write(arSource.GetData(), static_cast<UINT>(arSource.GetCount()));
	m_file.Flush();

	//	Acquire direct access to the memory file buffer.
	BYTE* pData = m_file.Detach();

	//	We need a DWORD to tell decrpyt how much data we're encrypting.
	DWORD dwDataLength = static_cast<DWORD>(arSource.GetCount());
	DWORD dwOldDataLength = dwDataLength;

	//	Now decrypt the data.
	if(!::CryptDecrypt(m_hKey, NULL, TRUE, 0, pData, &dwDataLength))
	{
		//	Free the memory we release from the memory file.
		delete [] pData;

		return false;
	}

	//	Set the length of the data file, write the decrypted data to it.
	m_file.SetLength(dwDataLength);
	m_file.Write(pData, dwDataLength);
	m_file.Flush();
	m_file.SeekToBegin();

	//	Free the memory we release from the memory file.
	delete [] pData;
	return true;
}

void CCrypto::strtohex(const char *str, unsigned char *hex)
{
	int i,len = strlen(str);
	char* t;
	unsigned char* x;

	for(i=0,t=(char *)str,x=hex;i<len;i+=2,x++,t+=2)
	{
		if(*t >= '0' && *t <= '9')
		{
			*x = ((*t & 0x0f) << 4);
		}
		else
		{
			char h = 0x0a + tolower(*t) - 'a';
			*x = (h << 4) ;
		}
		if(*(t+1) >= '0' && *(t+1) <= '9')
		{
			*x |= (*(t+1) & 0x0f);
		}
		else
		{
			char l = 0x0a + tolower(*(t+1)) - 'a';
			*x += l;
		}
	}
}

CString CCrypto::DecryptPassword(CString csEncrypt)
{
	CString csDecrypt;
	CByteArray cbArray2;

	LPCSTR lpSTR;
	USES_CONVERSION;
	lpSTR = W2A(csEncrypt.LockBuffer());
	csEncrypt.UnlockBuffer();

	unsigned char hexdata[32];

	memset(hexdata, 0, sizeof(unsigned char)*32);

	strtohex(lpSTR, hexdata);

	cbArray2.SetSize(csEncrypt.GetLength()/2);

	for(int iTemp = 0; iTemp < (csEncrypt.GetLength()/2); iTemp++)
	{
		cbArray2[iTemp] = (BYTE)hexdata[iTemp];
	}

	Decrypt(cbArray2, csDecrypt);

	return csDecrypt;
}
BOOL CGS::SetMenuString (HMENU hMenu, UINT uItem, int nID) 
{ 
    CString cs;
    LOADSTRING(cs, nID);
    if (cs.GetLength() == 0) return false;
    BOOL bByPosition = false;
    MENUITEMINFO miiGet = {sizeof (MENUITEMINFO), MIIM_TYPE}; 
    if (:: GetMenuItemInfo (hMenu, uItem, bByPosition, &miiGet) == false) return false; 

    MENUITEMINFO mii = { sizeof (MENUITEMINFO), MIIM_TYPE, MFT_STRING | ((miiGet.fType && MFT_RADIOCHECK)? MFT_RADIOCHECK: 0)}; 
    mii.dwTypeData = (LPTSTR) cs.GetBuffer(); 
    return:: SetMenuItemInfo (hMenu, uItem, bByPosition, &mii); 
}
//void CGS::vDeleteRecordFile(CString m_csNextPath)
//{
//	list<CString> listAVIFiles;	//List for AVI Files
//	list<CString> listDirFiles;	//List for Directory
//
//	CTime timeTemp, timeCurrent;
//	CTimeSpan spanTime;
//	CString csNextPath = m_csNextPath;
//	CFileFind finder;
//	unsigned __int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;
//
//	bool bFileExit = finder.FindFile(csNextPath + _T("\\*.*"));
//
//	while(bFileExit)
//	{
//		bFileExit = finder.FindNextFileW();
//		
//		if(finder.GetFileName() == _T(".") || finder.GetFileName() == _T(".."))
//			continue;
//
//		if(finder.IsDirectory())
//		{
//			listDirFiles.push_back(finder.GetFilePath());
//		}
//		else
//		{
//			listAVIFiles.push_back(finder.GetFilePath());
//		}
//	}
//
//	listDirFiles.sort();
//	listAVIFiles.sort();
//
//	list <CString>::iterator i;
//
//	for(i = listDirFiles.begin(); i != listDirFiles.end(); i++)
//	{
//		CString csDirectory = (LPCTSTR)*i;
//		vDeleteRecordFile(csDirectory);
//	}
//
//	for(i = listAVIFiles.begin(); i != listAVIFiles.end(); i++)
//	{
//		CString csFileName = (LPCTSTR)*i;
//
//		GetDiskFreeSpaceEx( (LPCTSTR)csNextPath, (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes );
//		if(i64FreeBytesToCaller/(1024*1024) > RECORDING_BUFFER)
//		{
//			CString csFreeSpace;
//			csFreeSpace.Format(_T("%d"), (i64FreeBytesToCaller/(1024*1024)));
//			//AfxMessageBox(csFreeSpace);					
//			return;
//		}
//
//		int iTemp = ((CString)*i).Find(_T(".avi"));
//		if(iTemp == -1)continue;
//
//		try
//		{
//			CFile::Remove(csFileName);
//		}
//		catch (CFileException* pe)
//		{
//			if( pe->m_cause == CFileException::sharingViolation)
//				ERRLOG(_T("SHARE.EXE was not loaded, or a shared region was locked.\n"));
//			pe->Delete();
//		}
//		
//	}
//
//}

//CString CGS::csGetStoragePath(int iRecordSort, CDatabase *m_DB)
//{
//	CString csNextPath;
//	try
//	{
//		CString SQL_Next_Path;
//		SQL_Next_Path.Format(_T("SELECT store_location FROM ec_storage WHERE sort_order = %d"), iRecordSort);
//
//		CRecordset rNextPath(m_DB);
//		//CRecordset rNextPath(m_DB);
//		rNextPath.Open(CRecordset::forwardOnly, SQL_Next_Path);
//		while(!rNextPath.IsEOF()) { 
//			rNextPath.GetFieldValue(_T("store_location"), csNextPath);
//			csNextPath.TrimRight();
//			rNextPath.MoveNext();
//		}
//		rNextPath.Close();
//	}
//	catch(CDBException * p)
//	{
//		AfxMessageBox(p->m_strError);
//	}
//
//	return csNextPath;
//}
void CGS::SetTransparent(HWND hwnd,UINT alpha)  
{  
	typedef     BOOL     (FAR PASCAL*LAYERFUNC)(HWND,COLORREF,BYTE,DWORD);  
	LAYERFUNC SetLayer;  
	HMODULE   hmod=LoadLibrary(_T("user32.dll"));  
	if(hmod   !=   NULL)  
	{   
		SetLayer=(LAYERFUNC)GetProcAddress(hmod,"SetLayeredWindowAttributes");  
		if(SetLayer!=NULL)  
		{  
			SetWindowLong(hwnd,GWL_EXSTYLE,GetWindowLong(hwnd,GWL_EXSTYLE)|WS_EX_LAYERED);//|0x80000L);  
			SetLayer(hwnd,0,alpha,LWA_ALPHA);  
		}  
		FreeLibrary(hmod);  
	}  
}
//void CGS::SetTransparent(HWND hwnd, COLORREF clrMask)  
//{  
//	typedef     BOOL     (FAR PASCAL*LAYERFUNC)(HWND,COLORREF,BYTE,DWORD);  
//	LAYERFUNC SetLayer;  
//	HMODULE   hmod=LoadLibrary(_T("user32.dll"));  
//	if(hmod   !=   NULL)  
//	{   
//		SetLayer=(LAYERFUNC)GetProcAddress(hmod,"SetLayeredWindowAttributes");  
//		if(SetLayer!=NULL)  
//		{  
//			SetWindowLong(hwnd,GWL_EXSTYLE,GetWindowLong(hwnd,GWL_EXSTYLE)|0x80000L);  
//			SetLayer(hwnd, clrMask, 0, LWA_COLORKEY);  
//		}  
//		FreeLibrary(hmod);  
//	}  
//}