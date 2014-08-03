// GS.h: interface for the CGS class.
//
//////////////////////////////////////////////////////////////////////
#include "Wincrypt.h"
#if !defined(AFX_GS_H__A04814DF_57D9_4DE6_BDEB_5DB1D510632E__INCLUDED_)
#define AFX_GS_H__A04814DF_57D9_4DE6_BDEB_5DB1D510632E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define ERRLOG(FORMAT, ...) CGS::LogEvent(EVENTLOG_ERROR_TYPE, FORMAT, __VA_ARGS__)
#define WARNLOG(FORMAT, ...) CGS::LogEvent(EVENTLOG_WARNING_TYPE, (FORMAT), __VA_ARGS__)

class CSA
{
public:
	SECURITY_ATTRIBUTES m_sa;
	SECURITY_DESCRIPTOR m_sd;
	CSA(){
		::ZeroMemory(&m_sa, sizeof(m_sa));
		::ZeroMemory(&m_sd, sizeof(m_sd));
		m_sa.nLength = sizeof (SECURITY_ATTRIBUTES);
		m_sa.lpSecurityDescriptor = &m_sd;
		m_sa.bInheritHandle = TRUE;
		if (!InitializeSecurityDescriptor (&m_sd, SECURITY_DESCRIPTOR_REVISION)) {ASSERT(FALSE); return;}
		if (!SetSecurityDescriptorDacl (&m_sd, TRUE, NULL, FALSE)){ASSERT(FALSE); return; }	
	};
};
class CGS  
{
public:
    //static HINSTANCE g_hResStr;
    //static HINSTANCE g_hResOEMStr;
    //static TCHAR m_tzBrand[32];
    //static TCHAR m_tzProductName[32];
    //static TCHAR m_tzPlaybackName[32];
    //static TCHAR m_tzPrefix[32];
    //static TCHAR m_tzRegistSubFolder[MAX_PATH];


public:
	//static void ERRLOG(WORD nType, LPCTSTR pFormat, ...){
	//	LogEvent(EVENTLOG_ERROR_TYPE, pFormat, __VA_ARGS__ );
	//};
	//static void WARNLOG(WORD nType, LPCTSTR pFormat, ...){LogEvent(EVENTLOG_WARNING_TYPE, pFormat, __VA_ARGS__ );};
    static BOOL bCreateHierarchyDir(LPCTSTR lpPathName);
    static BOOL SetMenuString (HMENU hMenu, UINT uItem, int nID); 
	static void LogEvent(WORD nType, LPCTSTR pFormat, ...);
	static void ETROINFO(const TCHAR* szFile, const TCHAR* szFunction, long nLine, const TCHAR* pszFmt, ...);
	static bool bGetRecordFolder(CString& csFolder);
	bool bCleanRegistry();
	CString csGetCacheFolder() {if (csCacheFolder.GetLength() ==0) {vGetAppCacheFolder();}  return csCacheFolder;};
	static CString csCacheFolder;
	static void PrintOutLogFile(CString csLog);
	static CString csGetDatabaseFolder(){return csGetAppStoreFolder()+_T("\\Database");};
	static void GradientFillRect( CDC *pDC, CRect &rect, COLORREF col_from, COLORREF col_to,  DWORD dwMode =GRADIENT_FILL_RECT_H);
    static void SetTransparent(HWND hwnd, UINT alpha);
    //static void SetTransparent(HWND hwnd, COLORREF clrMask);
	
	/*--Delete Record File--*/
	//static void vDeleteRecordFile(CString csNextPath);
	/*--Get Next Storage Path--*/
	//static CString csGetStoragePath(int iRecordSort, CDatabase *m_DB);
	//static CString csGetStoragePath(CString csRecordingSort);

	bool bGetGroupName(CString csGroup[], BYTE* nSize);
	bool bSaveGroupName(CString* pcsGroup, BYTE nSize);
	//static void vTraceItem(NODEITEM* pItem);

	//bool bSetKeepInfo();
	//bool bGetKeepInfo();
	//static vector <KEEP_AUTH_INFO> m_authinfo;
	//vector <KEEP_AUTH_INFO>& GetAuthInfo(){return m_authinfo;};
	CGS();
	virtual ~CGS();

protected:
	//static CRITICAL_SECTION	criticalLog;
	static CString csGetAppStoreFolder();
	void vGetAppCacheFolder();
	/*--Last Recording Path--*/
	//static CString csLastRecordingPath;
	
public:
	// Set the register key
	bool vSetProtocolValue(CString op_value);
	bool vGetProtocol(CString& o_Protocol);

	// Save the previous played nodeitem to the registry
	bool bSetNodeItem(vector <NODEITEM*> vctpNode);
	bool bGetNodeItem();

	vector <NODEITEM> m_vctNode;
	bool bSetRun(bool bEnable);
	bool bGetRun();
	static void vQuerynGetDLL();
	static LCID m_nLocaleID;
	static bool bSetLocaleID(LCID lcid);
	static bool bGetLocaleID();
	static CString  CGS::csLCIDtoDLL(LCID lcid);
	static bool bSetGroupAutoPlayIdx(vector<int> vctnGroupIdx);
	static bool bGetGroupAutoScanIdx(vector<int> & vctnGroupIdx);
	static bool bSetPatrolSeqDuration(vector< map <int,int>> vctmapPatrolSeqDuration);
	static bool bGetPatrolSeqDuration(vector< map <int,int>> &vctmapPatrolSeqDuration);
	static bool bSetPatrolSeqDuration(vector< map <int,pair_INT>> vctmapPatrolSeqDuration,int nDelKeySN=-1);
	static bool bGetPatrolSeqDuration(vector< map <int,pair_INT>> &vctmapPatrolSeqDuration);
	static bool bSetPatrolSeqDuration(map <int,pair_INT> mapPatrolSeqDuration,BYTE mac[6]);
	static bool bGetPatrolSeqDuration(map <int,pair_INT> &mapPatrolSeqDuration,BYTE mac[6]);
	static bool bGetOSDConfigFromRegistry(void);
	static bool bSetOSDConfigToRegistry(void);
	static vector<int> m_vctnOSDBtnConfig;

};

/*========================================
			CCrypto
=========================================*/

class CCrypto
	{
	public:

		//	All of the Cryptography API initialisation is done in the 
		//	constructor, so constructing this object is expensive. I recommend having
		//	only one instance only, and keep it in your application class.
		CCrypto();
		virtual ~CCrypto();

		//	These functions are essential to using the crypto object- you must
		//	have a key from some source or other.

		//	Derive a key from a password.
		virtual bool DeriveKey(CString strPassword);

		//	These functions handle encryption and decryption.

		virtual bool Encrypt(const CObject& serializable, CByteArray& arData);
		virtual bool Decrypt(const CByteArray& arData, CObject& serializable);

		virtual bool Encrypt(const CString& str, CByteArray& arData);
		virtual bool Decrypt(const CByteArray& arData, CString& str);
        CString DecryptPassword(CString csEncrypt);
        void strtohex(const char* str, unsigned char* hex);

	protected:

		//	Encrypt the contents of the memory file and store in the passed array.
		virtual bool InternalEncrypt(CByteArray& arDestination);

		//	Decrypt the contents of the passed array and store in the memory file.
		virtual bool InternalDecrypt(const CByteArray& arSource);

		//	Handle to the cryptography provider.
		HCRYPTPROV m_hCryptProv;
	
		//	Handle to the cryptography key.
		HCRYPTKEY m_hKey;

		//	Handle to the hash object.
		HCRYPTHASH m_hHash;

		//	Internally, the encryption and decryption of data is done with
		//	a CMemFile intermediate.
		CMemFile m_file;
	};

#endif // !defined(AFX_GS_H__A04814DF_57D9_4DE6_BDEB_5DB1D510632E__INCLUDED_)
//
//class DBConnection
//{
//public:
//	DBConnection();
//	~DBConnection();
//
//	CDatabase m_DB;
//};