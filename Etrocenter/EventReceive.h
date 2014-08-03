#pragma once
#include <afxmt.h>
#include "stdafx.h"
#include "Strsafe.h"
#include "IntraEvent.h"
#include "GlobalDef.h"
#include <vector>

using namespace std;

enum EVENT_STATUS {OFF=0, ON};

struct EventInfo
{
	HANDLE hPipe;
	TCHAR tcIP[16];
	TCHAR sd[16];
	DWORD nEventType;
	DWORD nStream;
	DWORD nCamIdx;
	time_t time;
	int   nStatus;
	TCHAR tcCardNum[20];
	TCHAR tcMAC[16];
	bool input1;
	bool input2;
	bool md1;
	bool md2;
	bool md3;
	bool vsignal1;
	bool tamper;
};
/*/
struct EventInfo
{
	HANDLE hPipe;
	TCHAR tcIP[16];
	DWORD nEventType;
	DWORD nStream;
	DWORD nCamIdx;
	int   nStatus;
    time_t time;
	TCHAR tcCardNum[20];
};
*/
class CEventReceive
{
	typedef void( *MyCallBack)(EventInfo* t_EventInfo, void* lpParam);

public:
	CEventReceive(void);
	~CEventReceive(void);
	void vSetCallBack(void* fnParam, void* lpParam);
	void vStartReceive();
    static  UINT _stdcall fnReceiveEventDaemon(LPVOID lpParam);
    static DWORD m_ConnectCounter;
    static HANDLE m_hDaemon;
	
	static vector<void*> m_vtlParam;
	static vector<MyCallBack> m_veCallBack;
protected:
    vector <DWORD> m_vtID;
    static bool m_bStopDaemon;
    static HANDLE m_hStopReceDaemon;
    static DWORD m_nTotalID;
    //static HANDLE m_hEventPipe;

public:
    void vStopEventReceive(void);
};
