// CGIVar.h: interface for the CGIVar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGIVAR_H__094195FF_C849_4924_B81B_58CD7747DC02__INCLUDED_)
#define AFX_CGIVAR_H__094195FF_C849_4924_B81B_58CD7747DC02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
struct SETS
{
	enum TYPE {STRING=0, CHECK, INTERGER};
	TYPE nType;
	CString csKey;
	bool    bCheck;
	int     nInt;
	CString csCaption;
	SETS(TCHAR* A, TCHAR* B){csKey = A; csCaption = B;};
	SETS(){};
};

struct VERSETS
{
	map <CString, SETS> aSet;
//	map <CString, VERSETS>* pSub;
};
#include <afxinet.h>
//#include ".\PropertyViewLib\IPropertyHost.h"
class CGISession : public CInternetSession
{
public:
	CGISession( LPCTSTR pstrAgent = NULL, DWORD dwContext = 1,
		DWORD dwAccessType = PRE_CONFIG_INTERNET_ACCESS, LPCTSTR pstrProxyName = NULL,
		LPCTSTR pstrProxyBypass = NULL, DWORD dwFlags = 0 );
protected:
	virtual void OnStatusCallback(DWORD dwContext, DWORD dwInternalStatus,
		LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	void LogInternetException(LPVOID pParam, CInternetException* pe);
};
class CGIVar// : public IPropertyHost  
{
public:
	bool bfnTurnLed(NODEITEM *pNode, bool bOn);
	bool bfnReboot(NODEITEM* pNode);

	bool bUpdateFirmware(CString csFile, NODEITEM* pNode);
	void vAttach(NODEITEM* pNode);
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	virtual bool IsPropertyEnabled( const void* pProperty );
	//virtual void GetProperties( EPropList& PropList );
	bool bIsDirty() {return m_bDirty;};
	void vSetDirty(bool bDirty) {m_bDirty = bDirty;};


	CGIVar();
	virtual ~CGIVar();
protected:
	map <CString, VERSETS> varmap;
	HANDLE hThreadRobort;
	CString csTarget;
	static UINT _stdcall CreateCgiRobor(LPVOID lpParam);
	bool m_bStaticIP;
	inline void* pLookupProperty(TCHAR* tszCgi, TCHAR* tszName);
	CString csGetAppCacheFolder();
	//inline void AddPropCombo(EPropList& PropList, TCHAR* tszCgi, TCHAR* tszName, const TCHAR* tszCombSrc);
	//inline void AddPropCombo(EPropList& PropList, TCHAR* tszCgi, TCHAR* tszName, TCHAR* tszSrcCgi, TCHAR* tszSrcName);
	//inline void AddPropInt(EPropList& PropList, TCHAR* tszCgi, TCHAR* tszName, bool bWrite = true);
	//inline void AddPropString(EPropList& PropList, TCHAR* tszCgi, TCHAR* tszName, bool bWrite = true);
	//inline void AddPropCheck(EPropList& PropList, TCHAR* tszCgi, TCHAR* tszName, const TCHAR* tszTitle = _T(""), const TCHAR* tszOn =_T("on"));
	void vParseSegment(TCHAR* tszBuffer, map <CString, VERSETS>::iterator pos);

	CString csMakeTempFilename();
	HANDLE hDestory;
	HANDLE hSectionClosed;
	bool m_bDirty;

public:
	int	 vCheckVideoStatus(NODEITEM *pNode);
};

#endif // !defined(AFX_CGIVAR_H__094195FF_C849_4924_B81B_58CD7747DC02__INCLUDED_)
