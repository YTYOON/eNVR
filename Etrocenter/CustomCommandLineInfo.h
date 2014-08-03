#pragma once
#include "afxwin.h"


class CCustomCommandLineInfo : public CCommandLineInfo
{
	public:
		BOOL JustTerminate;

	void ParseParam( LPCSTR lpszParam, BOOL bFlag, BOOL bLast )
	{
		AfxMessageBox(_T("export"));
		if( bFlag && strcmp( lpszParam, "exit" )==0 ) // handle "/exit"
			JustTerminate=1;
		else
		{
			JustTerminate=0;
			CCommandLineInfo::ParseParam( lpszParam, bFlag, bLast );
		}
	}
	CCustomCommandLineInfo();
	virtual ~CCustomCommandLineInfo();
};



//class CCustomCommandLineInfo : 	public CCommandLineInfo
//{
//public:
//	CCustomCommandLineInfo(void);
//	~CCustomCommandLineInfo(void);
//	//virtual void ParseParam( const TCHAR* pszParam, bool bFlag, bool bLast);
//	//virtual void ParseParam(LPCTSTR pszParam, bool bFlag, bool bLast);
//
//
//
////for convenience maintain 3 variables to indicate the param passed. 
//  BOOL m_bExport;       //for /e
//  BOOL m_bOpen;         //for /o
//  BOOL m_bWhatever;     //for /whatever
// 
// 
//
//  //public methods for checking these.
//public:
//  BOOL IsExport() { return m_bExport; };
//  BOOL IsOpen() { return m_bOpen; };
//  BOOL IsWhatever() { return m_bWhatever; };
//
// virtual void CCustomCommandLineInfo::ParseParam(/*LPCTSTR pszParam*/ const TCHAR* pszParam, bool bFlag, bool bLast)
//{
//	 USES_CONVERSION;   	
//    if( strcmp( T2CA(pszParam), "/e" )==0 )  
//    { 
//		AfxMessageBox(_T("export"));
//		 m_bExport = true;
//       // bRunningPC=TRUE; 
//    } 
//    else 
//    { 
//		AfxMessageBox(_T("No export"));
//         m_bExport = false;
//        //CCommandLineInfo::ParseParam( pszParam, bFlag, bLast ); // call base class 
//    } 
//}
//
// 
//};
//

 