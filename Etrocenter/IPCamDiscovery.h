// IPCamDiscovery.h: interface for the CIPCamDiscovery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPCAMDISCOVERY_H__4150DE7C_C8EB_4278_9CC6_5B96A08B2D3A__INCLUDED_)
#define AFX_IPCAMDISCOVERY_H__4150DE7C_C8EB_4278_9CC6_5B96A08B2D3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CIPCamDiscovery : public CObject  
{
public:
	static CRITICAL_SECTION m_criSec;
	static void WINAPI ENCPReceived(HENCPITEM hItem, ENCPCBMSG msg, LPARAM lParam);
	CIPCamDiscovery();
	virtual ~CIPCamDiscovery();
protected:

	static bool bString2ByteArray(CString& cs, BYTE* pByte, TCHAR* wzToken, BYTE nBase = 10);
	static CIPCamDiscovery* pThis;
	static HANDLE hNewIPCamEvent;
	HANDLE hThDiscovery;
	HANDLE hScanRequire;
	HANDLE hExitEvent;
	vector <NODEITEM>::iterator itNode;
public:
	void vResetData();
	void vAddInternetIpcam(NODEITEM* pNode);
	void vSetScanRequire();

	static vector <NODEITEM*> GetNodePtr(){return m_vtNode;};

protected:
	static bool bParseProtocolCapability(NODEITEM* pitem);
	bool bExistItem(NODEITEM* pitem);
	static UINT  GetIpcamBaseInfo(LPVOID lparam);
	static UINT _stdcall DiscoveryThread(LPVOID lpParam);
	void vDiscovery();
private:
	static vector <NODEITEM*> m_vtNode;
};

#endif // !defined(AFX_IPCAMDISCOVERY_H__4150DE7C_C8EB_4278_9CC6_5B96A08B2D3A__INCLUDED_)
