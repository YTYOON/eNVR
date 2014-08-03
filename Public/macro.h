#define LOADOEMSTRING(RES, ID, TZ) \
{ \
    CString csTmp; \
    csTmp.LoadString(RES,ID); \
    if (csTmp.GetLength() >0) \
        ::ZeroMemory(TZ, sizeof(TZ));\
        _tcsncpy(TZ, csTmp.GetBuffer(), min(sizeof(TZ)-1, csTmp.GetLength())); \
}

//#define ETROLOG 
#ifdef _DEBUG
#define ETROLOG TRACE  
#else
//#define ETROLOG
#define ETROLOG(FORMAT, ...)  { CGS::ETROINFO( _T(__FILE__),  _T(__FUNCTION__), __LINE__, FORMAT, __VA_ARGS__); }
#endif
#define ETROMSG AfxMessageBox
#define DELETEOBJ( OBJ ) {if (OBJ) {delete OBJ, OBJ = NULL;} }
#define DELETEARRAY( OBJ ) {if (OBJ) {delete [] OBJ, OBJ = NULL;} }
#define CLOSEHANDLE( handle ) if ( (handle) ) {::CloseHandle((handle)); (handle) = NULL;}
#define   WIDTHBYTES(bits)         (((bits)   +   31)   /   32   *   4) 
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

#define SDKERRMSG( rc ) if ( rc != ERROR_SUCCESS ){\
	LPVOID lpMsgBuf = NULL;\
	int nErrCode = ::GetLastError();\
	FormatMessage(\
	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,\
	NULL, nErrCode,\
	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),\
	(LPTSTR) &lpMsgBuf, 0, NULL);\
	ETROLOG(_T("Error Code:%d\tError Message:%s"), nErrCode, lpMsgBuf);}

#define FORMATMESSAGE( nErrCode ) if ( nErrCode != ERROR_SUCCESS ){\
	LPVOID lpMsgBuf = NULL;\
	FormatMessage(\
	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,\
	NULL, nErrCode,\
	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),\
	(LPTSTR) &lpMsgBuf, 0, NULL);\
	ETROLOG(_T("Error Code:%d\tError Message:%s"), nErrCode, lpMsgBuf);}

#define LOADOEM(STRING, ID) if (false == STRING.LoadString(CGS::g_hResOEMStr, ID)){ STRING.LoadString(ID);}
#define LOADSTRING(STRING, ID) if (false == STRING.LoadString(CResourceMgr::g_hResStr, ID)){ STRING.LoadString(ID);}
//#define AFXMESSAGEBOX(ID,  ...) {CString cs; LOADSTRING(cs,ID); AfxMessageBox(cs, , __VA_ARGS__);}
#define SETITEMTEXT(idc, ids){CString cs; LOADSTRING(cs, ids); SetDlgItemText(idc, cs);}
#define POSTPARENT(nRet) if(!m_bSetStyle && (GetParent()->GetParent())->m_hWnd) {::PostMessage((GetParent()->GetParent())->m_hWnd,ID_MATRIX_SELECTED,(WPARAM)nRet,NULL);} 
