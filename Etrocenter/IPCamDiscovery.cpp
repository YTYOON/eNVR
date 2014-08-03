// IPCamDiscovery.cpp: implementation of the CIPCamDiscovery class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NVR.h"
#include "IPCamDiscovery.h"
#include "CGIVar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HANDLE CIPCamDiscovery::hNewIPCamEvent =NULL;
CIPCamDiscovery* CIPCamDiscovery::pThis;
vector <NODEITEM*> CIPCamDiscovery::m_vtNode;
CRITICAL_SECTION CIPCamDiscovery::m_criSec;

CIPCamDiscovery::CIPCamDiscovery()
:hThDiscovery(NULL)
{
	m_vtNode.reserve(256);
	hNewIPCamEvent = CreateEvent(NULL, TRUE, FALSE, NEW_IPCAM_AVAILABLE);
	hScanRequire =   CreateEvent(NULL, FALSE, TRUE, SCAN_REQUIRE);
	hExitEvent = 	 CreateEvent(NULL, TRUE, FALSE, IPCAM_EXPLORER_EXIT);
	if (&m_criSec && m_criSec.LockCount == 0)
	{
		InitializeCriticalSection (&m_criSec);
		vDiscovery();
	}
}

CIPCamDiscovery::~CIPCamDiscovery()
{
	encp_Close();
	vResetData();
	if (hNewIPCamEvent)
	{
		CloseHandle(hNewIPCamEvent);
		hNewIPCamEvent = NULL;
	}
	CloseHandle(hExitEvent);
	CloseHandle(hScanRequire);

	if (hThDiscovery)
	{
		if ((WaitForSingleObject(hThDiscovery,INFINITE) == 0))
		{
			CloseHandle( hThDiscovery );
			hThDiscovery = NULL;
		}
	}
	
}

void CIPCamDiscovery::vDiscovery()
{
	if ( hThDiscovery == NULL)
	{
		pThis = this;
		UINT nRet(0);
		hThDiscovery = (HANDLE)_beginthreadex(NULL, 0, DiscoveryThread, this, 0, &nRet);
	}
}

UINT CIPCamDiscovery::DiscoveryThread(LPVOID lpParam)
{
	CIPCamDiscovery* pThis = (CIPCamDiscovery*)lpParam;
	ENCPRESULT err;
	CString msg;
	ENCPINITSTRUCT init;
	int ret;

	init.lpfnErrorCallback = NULL;
	init.lpfnReceiveCallback = ENCPReceived;
	init.nSocketType = ENCP_UDP;
	ret = encp_Initialise( &init );

	if( ret != ENERR_OK )
	{
		AfxMessageBox(IDS_INI_ENCP_ERR);
		encp_Close();
		return 0;
	}
	HANDLE hObjects[]={pThis->hScanRequire, pThis->hExitEvent};
	DWORD dwRet;
	while (WAIT_OBJECT_0 == (dwRet = ::WaitForMultipleObjects (2, hObjects, false, INFINITE) ))
	{
		pThis->vResetData();
		encp_UseVersion(4);
		err = encp_Search();
		Sleep(100);
		encp_UseVersion(2);
		err = encp_Search();
		if ( err )
		{
			SDKERRMSG(err);
			msg.Format(_T("%d"),GetLastError());
			switch( err )
			{
			case ENERR_FAIL:			// General Error
				break;
			case ENERR_NOTIMPL:			// This feature is not implemented
				break;
			case ENERR_CALL_INIT:		// encp_Initialise() has not called.
				break;
			case ENERR_INVALIDARG:		// One or more arguments are invalid
				break;
			case ENERR_OUTOFMEMORY:		// Ran out of memory
				break;
			case ENERR_SOCKETERROR:		// unable to create socket
				break;
			case ENERR_NOCLASSID:		// No such interface supported
				break;
			case ENERR_HANDLE:
				break;
			} 
		}
	}
	ETROLOG(_T("CIPCamDiscovery WaitForMultipleObjects return %d\r\n"), dwRet);
	ASSERT(dwRet != WAIT_FAILED);
	return 0;
}

void CIPCamDiscovery::ENCPReceived(HENCPITEM hItem, ENCPCBMSG msg, LPARAM lParam)
{
	bool bPickUp = false;
	NODEITEM templatenodeitem={0};
	templatenodeitem.nProtocol = VSMP;
	
	//	Based on Registry Setting
	// 1: Default Recording; 0: Default NOT Recording
	templatenodeitem.bRecording = 1; 

	
	NODEITEM* pitem = new NODEITEM(templatenodeitem);

	ENCPNETWORK nInfo={0};
	ENCPNODEINFO NodeInfo={0};
	ENCPPORTINFO PortInfo={0};
	ENCPFWINFO FWInfo={0};
	ENCPRESULT result;
	CString mmsg;

	//memset(&PortInfo,0,sizeof(PortInfo));
	switch( msg )
	{
	case ENM_ENCPSEARCH:
		result = encp_GetNodeInfo(hItem, &NodeInfo);
		if( result != ENERR_OK )
		{
			switch( result )
			{
			case ENERR_FAIL:				// general error
				break;
			case ENERR_NOTIMPL:			// This feature is not implemented
				break;
			case ENERR_CALL_INIT:		// encp_Initialise() has not called.
				break;
			case ENERR_INVALIDARG:		// One or more arguments are invalid
				break;
			case ENERR_OUTOFMEMORY:		// Ran out of memory
				break;
			case ENERR_SOCKETERROR:		// unable to create socket
				break;
			case ENERR_NOCLASSID:		// No such interface supported
				break;
			case ENERR_HANDLE:
				break;
			}
			// Do something with those errors
			return;
		}

		result = encp_GetPortInfo(hItem, &PortInfo);
		strncpy( pitem->ip, NodeInfo.ip, sizeof(pitem->ip) );
		memcpy( pitem->mac, &NodeInfo.mac, sizeof(pitem->mac) );
		strncpy( pitem->hostname, NodeInfo.hostName, sizeof(pitem->hostname)-1 );
		strncpy( pitem->modelname, NodeInfo.modelName, sizeof(pitem->modelname)-1 );
		pitem->nAPPort = PortInfo.av_port ;
		pitem->nWebPort = PortInfo.web_port ;
		pitem->nAP2Port = PortInfo.av2_port ;
		pitem->nAVType = PortInfo.av_type ;
		pitem->nAV2Type = PortInfo.av2_type ;
		pitem->bAV = PortInfo.av_enable;
		pitem->bAV2 = PortInfo.av2_enable;
		bParseProtocolCapability(pitem);

		result = encp_GetGuestPermission(hItem, &pitem->bAccepGuest);
		if( result != ENERR_OK )
		{
			switch( result )
			{
			case ENERR_FAIL:				// general error
				break;
			case ENERR_NOTIMPL:			// This feature is not implemented
				break;
			case ENERR_CALL_INIT:		// encp_Initialise() has not called.
				break;
			case ENERR_INVALIDARG:		// One or more arguments are invalid
				break;
			case ENERR_OUTOFMEMORY:		// Ran out of memory
				break;
			case ENERR_SOCKETERROR:		// unable to create socket
				break;
			case ENERR_NOCLASSID:		// No such interface supported
				break;
			case ENERR_HANDLE:
				break;
			}

			// Do something with those errors
			return;
		}

		result = encp_IsAudioEnabled(hItem, &pitem->bAudio);
		if( result != ENERR_OK )
		{
			switch( result )
			{
			case ENERR_FAIL:				// general error
				break;
			case ENERR_NOTIMPL:			// This feature is not implemented
				break;
			case ENERR_CALL_INIT:		// encp_Initialise() has not called.
				break;
			case ENERR_INVALIDARG:		// One or more arguments are invalid
				break;
			case ENERR_OUTOFMEMORY:		// Ran out of memory
				break;
			case ENERR_SOCKETERROR:		// unable to create socket
				break;
			case ENERR_NOCLASSID:		// No such interface supported
				break;
			case ENERR_HANDLE:
				break;
			}

			// Do something with those errors
			return;
		}

		result = encp_GetNetworkInfo(hItem, &nInfo);
		if( result != ENERR_OK )
		{
			switch( result )
			{
			case ENERR_FAIL:				// general error
				break;
			case ENERR_NOTIMPL:			// This feature is not implemented
				break;
			case ENERR_CALL_INIT:		// encp_Initialise() has not called.
				break;
			case ENERR_INVALIDARG:		// One or more arguments are invalid
				break;
			case ENERR_OUTOFMEMORY:		// Ran out of memory
				break;
			case ENERR_SOCKETERROR:		// unable to create socket
				break;
			case ENERR_NOCLASSID:		// No such interface supported
				break;
			case ENERR_HANDLE:
				break;
			}
			break;
		}
		pitem->nType = nInfo.nettype;

		result = encp_GetFWInfo(hItem, &FWInfo);
		memcpy(pitem->firmwarever, FWInfo.version, sizeof(pitem->firmwarever));
		memcpy(pitem->sn,FWInfo.sn,sizeof(FWInfo.sn));
		pitem->nStatus = LIVE;
		pitem->hEncp = hItem;
		if (false == pThis->bExistItem(pitem))
		{
			EnterCriticalSection(&m_criSec);
			pThis->m_vtNode.push_back(pitem);
			LeaveCriticalSection(&m_criSec);
//			memset(pitem, 0, sizeof(NODEITEM));
			PulseEvent(hNewIPCamEvent);
			::Sleep(10);
			PulseEvent(hNewIPCamEvent);
			bPickUp = true;
		}
		break;

	case ENM_TYPE_MISMATCH:
	case ENM_AUTHFAIL:
	case ENM_SETFAIL:
	case ENM_SETSUCCESS:
	case ENM_UNKNOWN_ERROR:
	default:
/*
		if( m_dlgSetting.IsWindowVisible() )
		{
			while( m_dlgSetting.IsMessageReceived() ) Sleep(10);
			m_dlgSetting.OnENCPItemReceived(hItem, msg, lParam);
		}
*/
		break;
	}
	if (false == bPickUp)
		delete pitem;
}

bool CIPCamDiscovery::bExistItem(NODEITEM *pitem)
{
	int nCount = m_vtNode.size();
	for (int ci=0; ci < nCount; ci++)
	{
		if( 0 == memcmp( &(m_vtNode[ci]->mac[0]), pitem->mac, sizeof(m_vtNode[ci]->mac) ))
		{
			if (m_vtNode[ci]->nStatus == RESETTING)
			{
				HANDLE hPauseScanIPCam  = OpenEvent(SYNCHRONIZE, FALSE, PAUSE_IPCAM_DISCOVERY);
				if (WAIT_OBJECT_0 == ::WaitForSingleObject( hPauseScanIPCam, 0))
				{
					CloseHandle(hPauseScanIPCam);
					return true;
				}
				CloseHandle(hPauseScanIPCam);
				NODEITEM* pNode = m_vtNode[ci];
				pitem->hTreeItem = m_vtNode[ci]->hTreeItem;
				memcpy(pitem->szUser, m_vtNode[ci]->szUser, sizeof(m_vtNode[ci]->szUser));
				memcpy(pitem->szPass, m_vtNode[ci]->szPass, sizeof(m_vtNode[ci]->szPass));
				pitem->nGroup =  m_vtNode[ci]->nGroup;
				memcpy(m_vtNode[ci], pitem, sizeof(NODEITEM));
				memset(pitem, 0, sizeof(NODEITEM));
				NODEITEM* pItem = m_vtNode[ci];
#ifndef FACTORY
				delete ((CGIVar*) pItem->pIPCamCGIInfo);
				m_vtNode[ci]->pIPCamCGIInfo = new CGIVar;//CIPCamCGIInfo;
				((CGIVar*) m_vtNode[ci]->pIPCamCGIInfo)->vAttach(m_vtNode[ci]);//->ip, pNode->nWebPort, pNode->szUser, pNode->szPass);
#endif
				((CTaskTreeCtrl*)((CMainFrame*)AfxGetMainWnd())->GetTreeView())->vUpdateTreeItem(m_vtNode[ci]);
//				((CTaskTreeCtrl*)((CMainFrame*)AfxGetMainWnd())->				UpdateData(FALSE);
			}
			return true;
		}
	}
	return false;
}

void CIPCamDiscovery::vSetScanRequire()
{
	SetEvent(hScanRequire);
}

void CIPCamDiscovery::vAddInternetIpcam(NODEITEM *pParamNode)
{
	NODEITEM* pNode = new NODEITEM;
	memcpy(pNode, pParamNode, sizeof(NODEITEM));
	pNode->nProtocol = VSMP;
	EnterCriticalSection(&m_criSec);
	pThis->m_vtNode.push_back(pNode);
	LeaveCriticalSection(&m_criSec);
	CWinThread* pThread = AfxBeginThread(GetIpcamBaseInfo, pNode);//(LPVOID )pNode);
}
UINT CIPCamDiscovery::GetIpcamBaseInfo(LPVOID lparam)
{
	USES_CONVERSION;
	NODEITEM *pNode =(NODEITEM *)lparam;

	CInternetSession session; // can't get status callbacks for OpenURL
	CStdioFile* pFile1 = NULL; // could call ReadString to get 1 line
	static int nTotalRange = 0;
	static int nPos = 0;
	static int nTotalEntry = 0;
	WORD nEvent(0);
	TCHAR tzIP[32];
	TCHAR tzUser[32];
	TCHAR tzPass[32];
	int ci=0;

	_tcscpy(tzIP,   A2W(pNode->ip));
	_tcscpy(tzUser, A2W(pNode->szUser));
	_tcscpy(tzPass, A2W(pNode->szPass));
	CGS gs;
	UINT nBytesRead = 0;
	CString csUrl;

	bool bExist = false;
	TRACE(_T("TOTAL RANGE:%d\r\n"),nTotalRange);
	ci = -1;
	CString csFile;
	map <CString, VERSETS>::iterator pos;

	const int MAXBUF = 4096;
	char* buffer = new char[MAXBUF];
	memset(buffer, 0, MAXBUF);
	map <CString, CString> var;
	TCHAR tzBuffer[MAXBUF];
	TCHAR* tzSeg;	

	csUrl.Format( _T("http://%s:%s@%s:%d/%s"), tzUser, tzPass, tzIP, pNode->nWebPort, _T("common/info.cgi"));
	TRACE(_T("%s\r\n"),csUrl);
	try {
		pFile1 = session.OpenURL(csUrl, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_KEEP_CONNECTION);
	}
	catch(CInternetException* e) {
		int nError = e->m_dwError;
		e->Delete();
		if (nError == 12031)
		{
			goto exit;
		}
	}
	if (!pFile1) goto exit;
	nBytesRead = pFile1->Read(buffer, MAXBUF - 1);

	if (strncmp(buffer, "Error: 401", 10) == 0)
		goto exit;

	MultiByteToWideChar(CP_ACP, 0, buffer, -1, tzBuffer, MAXBUF);
//	pThis->vParseSegment(tzBuffer, pos);
	tzSeg = _tcstok(tzBuffer, _T("\r\n"));
	while (tzSeg)
	{
		nEvent = ::WaitForSingleObject(pThis->hExitEvent, 0);
		if (nEvent != WAIT_TIMEOUT)
		{
			if (nEvent == WAIT_OBJECT_0)
			{
				ETROLOG(_T("WAIT_OBJECT_0\t\n"));
			}else
			{
				SDKERRMSG(nEvent);
			}
			break;
		}
		TCHAR* tz = wcschr(tzSeg, '=');
		if (tz)
		{
			*tz = '\0';
			var[tzSeg] = tz+1;
			TRACE(_T("%s ---> %s\r\n"),tzSeg, var[tzSeg]);
		}
		tzSeg = _tcstok(NULL, _T("\r\n"));
	}	//
	pNode->nStatus = LIVE;
	strncpy(pNode->hostname, W2A(var[_T("name")].GetBuffer()),   var[_T("name")].GetLength());
	strncpy(pNode->modelname, W2A(var[_T("model")].GetBuffer()), var[_T("model")].GetLength());
	pNode->bAudio = var[_T("speaker")].Compare(_T("yes")) ? false : true;

//	bString2ByteArray(var["macaddr"], pNode->mac, _T(":\r\n"), 16);
	bString2ByteArray(var[_T("version")], pNode->firmwarever, _T(".\r\n"), 10);
//114.32.31.167:8080,1850		
	//if (false == pThis->bExistItem(pitem))
	{
		PulseEvent(hNewIPCamEvent);
	}

exit:
	delete buffer;
	return 0;
	
}
bool CIPCamDiscovery::bString2ByteArray(CString& cs, BYTE* pByte, TCHAR* wzToken, BYTE nBase)
{	
	TCHAR* tz1, *tz2;
	bool bRet = false;
	tz1 = tz2 = _tcstok(cs.LockBuffer(), wzToken);
	if (tz1 == NULL)
		return bRet;
	int ci=0;
	do{
		tz2 = _tcstok(NULL, wzToken);
		if (tz2 == 0) bRet = true;
		int nTmp = _tcstoul(tz1, &tz2, nBase);
		TRACE(_T("%s %d %x\r\n"), tz1, nTmp, nTmp);
		pByte[ci++] = nTmp;
		tz1 = tz2+1;
	}while (bRet == false);

	return bRet;
}

void CIPCamDiscovery::vResetData()
{
//	encp_Close();
	EnterCriticalSection(&m_criSec);
	for (int ci=0; ci < m_vtNode.size(); ci++)
	{
		NODEITEM* pNode = m_vtNode[ci];
		delete pNode;
		m_vtNode[ci] = NULL;
	}
	m_vtNode.clear();
	TRACE(_T("%d\r\n"),m_vtNode.size());
	LeaveCriticalSection(&m_criSec);
}

bool CIPCamDiscovery::bParseProtocolCapability(NODEITEM* pitem )
{
	if (pitem == NULL)
	{
		ASSERT(pitem);
		return false;
	}
	//WORD nDefaultProtocol(VSMP);
	//switch (pitem->nAPPort)
	switch (pitem->nAVType)
	{
	case 2://
		pitem->nProtocol = RTSP;
		break;
	case 1:
	default:
		pitem->nProtocol = VSMP;
		break;
	}
	WORD nCapability = 0;
	
	switch (pitem->modelname[2])
	{
	case '3':
	case '6':
//		nDefaultProtocol = VSMP;
		nCapability |= VSMP;
		break;
	case '8':
//		nDefaultProtocol = RTSP;
		nCapability |= RTSP;
		nCapability |= RTPOVERTCP;
		break;
	default:
		break;
	};
	switch (pitem->modelname[4])
	{
	case '3':
		break;
	default:
		nCapability |= RTSP;
//		nCapability |= CGI;
		break;
	}
	pitem->nProtocolCapability = nCapability;
	return true;
}
