// RemoteSrvPort.cpp : implementation file
//

#include "stdafx.h"
#include "RemoteSrvPort.h"
#define SERVICENAME _T("EVRecSvr.exe")
#include <psapi.h>

// CRemoteSrvPort dialog

IMPLEMENT_DYNAMIC(CRemoteSrvPort, CDialog)

CRemoteSrvPort::CRemoteSrvPort(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteSrvPort::IDD, pParent)
{
	m_nHttpPort=8080;
	m_nRTSPPort=554;
	
}

CRemoteSrvPort::~CRemoteSrvPort()
{
}

void CRemoteSrvPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRemoteSrvPort, CDialog)
END_MESSAGE_MAP()


// CRemoteSrvPort message handlers

BOOL CRemoteSrvPort::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRegKey regKey;
    DWORD nHttpPort(8080);
	//TCHAR szRegValue[1024];
	//::ZeroMemory(&szRegValue[0], sizeof(szRegValue));
	const DWORD len = 1024;
	DWORD nActualLen(len);
    CString csForder(CProductDefine::m_tzRegistSubFolder);

	LONG nRet = regKey.Open(HKEY_LOCAL_MACHINE, csForder.GetBuffer());
	if(nRet != ERROR_SUCCESS)
	{
		if( regKey.Create(HKEY_LOCAL_MACHINE, csForder.GetBuffer()) != 0)
		{
			TRACE(_T("Create Registry Fail."));
		}
	}
	
	//nRet = regKey.QueryValue(szRegValue, _T("REMOTESRVPORT"), &nActualLen);
	//if(nRet != ERROR_SUCCESS)
	//{
	//	TRACE(_T("Registry Query Fail!\r\n"));
	//}
	//regKey.Close();
	nRet = regKey.QueryDWORDValue(_T("HTTPPORT"), nHttpPort);
	if(nRet != ERROR_SUCCESS)
	{
        nRet = regKey.SetValue(nHttpPort, _T("HTTPPORT") );
		TRACE(_T("Registry Query Fail!\r\n"));
	}

	regKey.Close();
	BOOL bSaveDef=false;
	if (nHttpPort==0)
	{
		nHttpPort=8080;
		bSaveDef=true;
	}

    CString csHttpPort;
    csHttpPort.Format(_T("%d"), nHttpPort);
	this->SetDlgItemText(IDC_EDIT_SRV_PORT, csHttpPort);
	if (bSaveDef)
	{
		vSetPort2Reg();
	}

	//this->SetDlgItemText(IDC_EDIT_SRV_PORT, szRegValue);

	//::ZeroMemory(&szRegValue[0], sizeof(szRegValue));

	nRet = regKey.Open(HKEY_LOCAL_MACHINE, csForder.GetBuffer());
	if(nRet != ERROR_SUCCESS)
	{
		if( regKey.Create(HKEY_LOCAL_MACHINE, csForder.GetBuffer()) != 0)
		{
			TRACE(_T("Create Registry Fail."));
		}
	}

	DWORD dwRTSPPort;
	CString csRTSPProt;
	nRet = regKey.QueryDWORDValue(_T("RTSPServerPort"), dwRTSPPort);
	//nRet = regKey.QueryValue(szRegValue, _T("RTSPServerPort"), &nActualLen);
	if(nRet != ERROR_SUCCESS)
	{
		TRACE(_T("Registry Query Fail!\r\n"));
		dwRTSPPort = 554;
	}
	regKey.Close();
	
	csRTSPProt.Format(_T("%d"), dwRTSPPort);
	this->SetDlgItemText(IDC_EDIT_RTSPSRV_PORT, csRTSPProt.GetBuffer());


	m_nHttpPort=nHttpPort;
	m_nRTSPPort=(int)dwRTSPPort;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRemoteSrvPort::vSetPort2Reg()
{
	CString csPort;
	GetDlgItemText(IDC_EDIT_SRV_PORT, csPort);

	CRegKey regKey;
	TCHAR szRegValue[1024];
	::ZeroMemory(&szRegValue[0], sizeof(szRegValue));
	const DWORD len = 1024;
	DWORD nActualLen(len);
    CString csForder(CProductDefine::m_tzRegistSubFolder);

	LONG nRet = regKey.Open(HKEY_LOCAL_MACHINE, csForder.GetBuffer());
	if(nRet != ERROR_SUCCESS)
	{
		if( regKey.Create(HKEY_LOCAL_MACHINE, csForder.GetBuffer()) != 0)
		{
			TRACE(_T("Create Registry Fail."));
		}
	}
	DWORD dwHttpPort = _tcstoul(csPort, 0, 10);
	nRet = regKey.SetDWORDValue(_T("HTTPPORT"), dwHttpPort);	
	//nRet = regKey.SetValue( csPort.GetBuffer(), _T("REMOTESRVPORT") );
	if(nRet != ERROR_SUCCESS)
	{
		TRACE(_T("Registry SetValue Fail!\r\n"));
	}
	regKey.Close();

	m_nHttpPort = (int)dwHttpPort;
	//::AfxBeginThread(iRestartRemoteSrv, NULL);
}

UINT CRemoteSrvPort::iRestartRemoteSrv(LPVOID lpParam)
{
	TCHAR szParm[MAX_PATH];
    _stprintf(szParm, _T("EVRemoteSrv.exe -k"));
    STARTUPINFO ssi;
    PROCESS_INFORMATION ppi;
    ZeroMemory( &ssi, sizeof(ssi) );
    ssi.cb = sizeof(ssi);
	CreateProcess(NULL, szParm, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &ssi, &ppi ) ;

	DWORD dwRemoteSrv = GetRemoteSrvID();
	HANDLE hRemoteSrv = ::OpenProcess(PROCESS_ALL_ACCESS, true, dwRemoteSrv);
	
	::WaitForSingleObject(hRemoteSrv, INFINITE);

	_stprintf(szParm, _T("EVRemoteSrv.exe -s"));
	ZeroMemory( &ssi, sizeof(ssi) );
	ssi.cb = sizeof(ssi);
	CreateProcess(NULL, szParm, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &ssi, &ppi ) ;

	return 0;
}

DWORD CRemoteSrvPort::GetRemoteSrvID()
{
	// Get the list of process identifiers.
    DWORD aProcesses[4096], cbNeeded=0, cProcesses=0;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses)/sizeof(DWORD), &cbNeeded ) )
    {
        return 0;
    }

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for ( i = 0; i < cProcesses; i++ )
    {
        if( aProcesses[i] != 0 )
        {
            //PrintProcessNameAndID( aProcesses[i]);
			TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

			 HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, aProcesses[i] );

			// Get the process name.
			if (NULL != hProcess )
			{
				HMODULE hMod;
				DWORD cbNeeded;

				if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
				{
					GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR) );
				}
			}

			// Print the process name and identifier.
			TCHAR szCenter[MAX_PATH] = SERVICENAME;

			if(::wcscmp(szProcessName, szCenter) == 0)
			{
				//LogEvent(_T("%s (PID: %u)\n"), szProcessName, aProcesses[i]);
				return aProcesses[i];
			}
	
			// Release the handle to the process.
			CloseHandle( hProcess );
        }
    }

	return 0;
}

void CRemoteSrvPort::vSetRTSPPort2Reg()
{
	CString csPort;
	GetDlgItemText(IDC_EDIT_RTSPSRV_PORT, csPort);

	CRegKey regKey;
	TCHAR szRegValue[1024];
	::ZeroMemory(&szRegValue[0], sizeof(szRegValue));
	const DWORD len = 1024;
	DWORD nActualLen(len);
    CString csForder(CProductDefine::m_tzRegistSubFolder);

	LONG nRet = regKey.Open(HKEY_LOCAL_MACHINE, csForder.GetBuffer());
	if(nRet != ERROR_SUCCESS)
	{
		if( regKey.Create(HKEY_LOCAL_MACHINE, csForder.GetBuffer()) != 0)
		{
			TRACE(_T("Create Registry Fail."));
		}
	}

	DWORD dwRTSPProt = _tcstoul(csPort, 0, 10);
	nRet = regKey.SetDWORDValue(_T("RTSPServerPort"), dwRTSPProt);
	//nRet = regKey.SetValue( csPort.GetBuffer(), _T("REMOTESRVPORT") );
	if(nRet != ERROR_SUCCESS)
	{
		TRACE(_T("Registry SetValue Fail!\r\n"));
	}
	regKey.Close();

	m_nRTSPPort = (int)dwRTSPProt;
	//::AfxBeginThread(iRestartRemoteSrv, NULL);
}