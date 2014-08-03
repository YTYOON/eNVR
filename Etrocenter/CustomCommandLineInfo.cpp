#include "stdafx.h"
#include "CustomCommandLineInfo.h"

CCustomCommandLineInfo::CCustomCommandLineInfo(void)
{
//	m_bExport = m_bOpen = m_bWhatever = FALSE;
}

CCustomCommandLineInfo::~CCustomCommandLineInfo(void)
{
}


//  //for convenience maintain 3 variables to indicate the param passed. 
//  BOOL m_bExport;       //for /e
//  BOOL m_bOpen;         //for /o
//  BOOL m_bWhatever;     //for /whatever
// 
//  //public methods for checking these.
//public:
//  BOOL IsExport() { return m_bExport; };
//  BOOL IsOpen() { return m_bOpen; };
//  BOOL IsWhatever() { return m_bWhatever; };


//void CCustomCommandLineInfo::ParseParam(/*LPCTSTR pszParam*/ const TCHAR* pszParam, bool bFlag, bool bLast)
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


//void CCustomCommandLineInfo::ParseParam(LPCTSTR pszParam, bool bFlag, bool bLast)
//{
// 
//  if(0 == _tcscmp(pszParam, _T("/o")))
//    {
//      m_bOpen = TRUE;
//    } 
//    else if(0 == _tcscmp(pszParam, _T("/e")))
//    {
//      m_bExport = TRUE;
//    }
//    else if(0 == _tcscmp(pszParam, _T("/whatever")))
//    {
//      m_bWhatever = TRUE;
//    }
//}
	//if(0 == strcmp((char*)pszParam, _T("/o")))
	//AfxMessageBox(_T("pszParm"));
	/*if(0 == _tcscmp(pszParam, _T("/o")))
    {
      m_bOpen = TRUE;
    } 
    else if(0 == _tcscmp(pszParam, _T("/e")))
    {
      m_bExport = TRUE;
    }
    else if(0 == _tcscmp(pszParam, _T("/whatever")))
    {
      m_bWhatever = TRUE;
    }
}*/

 //void ParseParam( /*LPCSTR lpszParam*/const TCHAR* lpszParam, BOOL bFlag, BOOL bLast ) 
 //   { 
 //      USES_CONVERSION;  
 //                if( bFlag && strcmp( T2CA(lpszParam), "PC" )==0 )  
 //       { 
 //           bRunningPC=TRUE; 
 //       } 
 //       else 
 //       { 
 //           bRunningPC=FALSE; 
 //           CCommandLineInfo::ParseParam( lpszParam, bFlag, bLast ); // call base class 
 //       } 
 //   } 

