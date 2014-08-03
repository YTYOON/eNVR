// EVDatabase.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "EVDatabase.h"
#include "ini.h"

// CEVDatabase

CEVDatabase::CEVDatabase()
{
}

CEVDatabase::~CEVDatabase()
{
}

bool CEVDatabase::bDectecDBReady(void)
{
	bool bRet(false);
	int nRetry = 10;
	CIniReader iniReader;
	while (--nRetry > 0)
	{
		try{
			
			CString cpDns = iniReader.getKeyValue(_T("DSN"),_T("database"));
			CString cpUid = iniReader.getKeyValue(_T("UID"),_T("database"));
			CString cpPwd = iniReader.getKeyValue(_T("PWD"),_T("database"));
            CString csDbName = iniReader.getKeyValue(_T("DBNAME"),_T("database"));
			CString csDBOpenString;
			csDBOpenString.Format(_T("DSN=%s;UID=%s;PWD=%s;LANGUAGE=us_english"), cpDns, cpUid, cpPwd);
			BOOL bRet = OpenEx(csDBOpenString, CDatabase::noOdbcDialog);
            if (bRet)
            {
                int nRetryDatabaseStartup(4);
                while (--nRetryDatabaseStartup > 0)
                {
                    CString csQuery; 
                    csQuery.Format(_T("SELECT state_desc FROM sys.databases WHERE name = '%s'"), csDbName);
                    CString csTmp;
		            CRecordset recordSet(this);		
                    CDBVariant var;
		            recordSet.Open(CRecordset::snapshot, csQuery);
		            recordSet.GetFieldValue(_T("state_desc"), csTmp);
                    bRet = (csTmp == "ONLINE") ? true : false;
                    if (bRet)
			            return bRet;
                    else
                        Sleep(1000);
                }
            }
		}
		catch(CDBException * p)
		{
			p->Delete();
			HANDLE hWait = ::CreateEvent(NULL,FALSE,FALSE,STREAM_SERVER_LAUNCH);
			DWORD hResult;
			hResult = WaitForSingleObject(hWait, 1000);
			if(hResult == WAIT_TIMEOUT && nRetry == 1)
			{
				CString csMSG;
				LOADSTRING(csMSG, IDS_DB_INIT_FAIL);
				AfxMessageBox(csMSG);
				PostQuitMessage(0);
			}else
            {
                Sleep(1000);
            }
			CLOSEHANDLE(hWait);
		}
	}
	return bRet;

}

bool CEVDatabase::bDBConnect(void)
{
	bool bRet(false);
	int nRetry = 10;
	CIniReader iniReader;
	while (--nRetry > 0)
	{
		try{
			
			CString cpDns = iniReader.getKeyValue(_T("DSN"),_T("database"));
			CString cpUid = iniReader.getKeyValue(_T("UID"),_T("database"));
			CString cpPwd = iniReader.getKeyValue(_T("PWD"),_T("database"));
            CString csDbName = iniReader.getKeyValue(_T("DBNAME"),_T("database"));
			CString csDBOpenString;
			csDBOpenString.Format(_T("DSN=%s;UID=%s;PWD=%s;LANGUAGE=us_english"), cpDns, cpUid, cpPwd);
			bRet = OpenEx(csDBOpenString, CDatabase::noOdbcDialog);
		}
		catch(CDBException * p)
		{
			p->Delete();
			CString csMSG;
			LOADSTRING(csMSG, IDS_DB_INIT_FAIL);
			AfxMessageBox(csMSG);
		}
	}
	return bRet;
}


