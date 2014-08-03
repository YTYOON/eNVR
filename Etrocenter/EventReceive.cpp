#include "StdAfx.h"
#include "EventReceive.h"

vector<CEventReceive::MyCallBack> CEventReceive::m_veCallBack;
vector<void*> CEventReceive::m_vtlParam;
enum EVENTID {EVENTSERVER=0, EVSTREAM, ECLIVEVIEW};
DWORD CEventReceive::m_ConnectCounter(0);
HANDLE CEventReceive::m_hDaemon(NULL);
DWORD CEventReceive::m_nTotalID(-1);
//HANDLE CEventReceive::m_hEventPipe(NULL);
bool CEventReceive::m_bStopDaemon(false);
HANDLE CEventReceive::m_hStopReceDaemon(NULL);
#ifndef CLOSEHANDLE
#define CLOSEHANDLE(h) {if (h) {CloseHandle(h); h = NULL;}}
#endif
CEventReceive::CEventReceive(void)
{
}

CEventReceive::~CEventReceive(void)
{
	TRACE(_T("*******~CEventReceive()."));
    vStopEventReceive(); 
}

void CEventReceive::vSetCallBack(void *fnParam, void *lpParam)
{
	MyCallBack fnMyCallBack = (MyCallBack)fnParam;
    m_vtlParam.push_back((void*)lpParam);
	m_veCallBack.push_back(fnMyCallBack);
    ++m_ConnectCounter;
    m_vtID.push_back(++m_nTotalID);
}

void CEventReceive::vStartReceive()
{
    if (m_hDaemon == NULL)
    {
        UINT nThreadId(0);
        m_hDaemon = (HANDLE)::_beginthreadex(NULL, 0, fnReceiveEventDaemon, (LPVOID)NULL, 0, &nThreadId);
    }
}

UINT CEventReceive::fnReceiveEventDaemon(LPVOID lpParam)
{
#ifdef _DEBUG
static int iEventCount = 0;
//#define PRINTLOG { printf("Count = ##### %d, Camera Index:%d, EventType: %d, EventStatus: %d, IP: %s\n", ++iEventCount, eventInfo.nCamIdx, eventInfo.nEventType, eventInfo.nStatus, W2A(eventInfo.tcIP));}
#define PRINTLOG
#else
#define PRINTLOG
#endif
    USES_CONVERSION;
	bool		bStartReceive;
    EventInfo eventInfo;
	bStartReceive = true;
	LPTSTR	lpszPipename = TEXT("\\\\.\\pipe\\EVENT_SERVICE_CONNECTION");
	DWORD cbRead = 0;
    CSA sa;
    //CEventReceive* pThis = (CEventReceive*) lpParam;
    m_hStopReceDaemon = CreateEvent(&sa.m_sa, true, false, NULL);
    HANDLE hEventServerStartup = ::CreateEvent(&sa.m_sa, false, false, EVENT_SERVER_START_UP);

    HANDLE handles[] ={hEventServerStartup, m_hStopReceDaemon};
    HANDLE hEventPipe(NULL);
	//--Check System Event Not Exist
    //while(!pThis->m_bStopDaemon)
    while(!m_bStopDaemon)
	{
		DWORD nRet = ::WaitForMultipleObjects (sizeof(handles)/sizeof(HANDLE), handles, false, INFINITE);
		if (nRet == WAIT_OBJECT_0 +1) break; //pThis->m_hStopReceDaemon
_CREATE_PIPE:
        while(!m_bStopDaemon)
	    {
            CLOSEHANDLE(hEventPipe);
		    hEventPipe = CreateFile(
			    lpszPipename,   // pipe name 
			    GENERIC_READ |  // read and write access 
			    GENERIC_WRITE, 
			    0,              // no sharing 
			    NULL,           // default security attributes
			    OPEN_EXISTING,  // opens existing pipe 
			    0,              // default attributes 
			    NULL);          // no template file 

		    if(hEventPipe != INVALID_HANDLE_VALUE)	
            {
                break;
            }
			TRACE(_T("[fnReceiveEventDaemon]: CreateFile Error code :%d\r\n"), GetLastError());
            Sleep(1);
	    }
	    while(!m_bStopDaemon)
        {
		    ::ZeroMemory(&eventInfo, sizeof(EventInfo));
		    bool fSuccess = ReadFile(
			            hEventPipe,		// pipe handle
			            (EventInfo*)&eventInfo,		// buffer to receive reply 
			            sizeof(EventInfo),		// size of buffer	
			            &cbRead,		// number of bytes read
			            NULL);			// not overlapped 
            if (!fSuccess)
            {
                DWORD nErr = GetLastError();
                TRACE(_T("******ReadFile Error code :%d\r\n"), nErr);
                if (nErr == ERROR_BROKEN_PIPE) 
				{
					Sleep(100);
					goto _CREATE_PIPE;
					//break;
				}
                if (nErr == ERROR_PIPE_NOT_CONNECTED) break; //eventserver alread shut down
                if (hEventPipe == NULL) break;
                ASSERT(false);
                Sleep(1);
            }else if(!m_bStopDaemon)
            {
                PRINTLOG;
				TRACE(_T("==============> get evnet happen.\n"));
	            for(int iTemp=0; iTemp < m_veCallBack.size(); iTemp++)
	            {
                    if (m_vtlParam[iTemp] != NULL)
                    {
		                m_veCallBack[iTemp](&eventInfo, m_vtlParam[iTemp]);
                    }
	            }	            
            }
	    }
    }
	TRACE(_T("******* Thread21-exit."));
    CLOSEHANDLE(hEventServerStartup);
    CLOSEHANDLE(hEventPipe);    
    return 0;
}
void CEventReceive::vStopEventReceive(void)
{
    for (int ci =0; ci < m_vtID.size(); ++ci)//m_nId == -1 not vSetCallBack yet
    {
        --m_ConnectCounter;
        if (m_ConnectCounter <=0)
        {
            m_bStopDaemon = true;
            SetEvent(m_hStopReceDaemon);
            //CancelSynchronousIo(m_hDaemon); //not support for windows xp
            CSA sa;
            HANDLE hClientStop = CreateEvent(&sa.m_sa, false, false, (LPCWSTR)EVENT_CLIENT_LOGOUT);
			SetEvent(hClientStop);
            //CloseHandle(m_hEventPipe);  m_hEventPipe = NULL;
            DWORD nRet = ::WaitForSingleObject(m_hDaemon, 1);//shall be dead lock with call back sendmessage          
            if (nRet != WAIT_OBJECT_0)
            {
                MSG message;
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}
            m_veCallBack[m_vtID[ci]]=NULL;
            m_vtlParam[m_vtID[ci]]= NULL;
            m_veCallBack.clear();
            m_vtlParam.clear();
            CLOSEHANDLE(m_hStopReceDaemon);
            CLOSEHANDLE(m_hDaemon);
        }else
        {
            m_veCallBack[m_vtID[ci]]=NULL;
            m_vtlParam[m_vtID[ci]]= NULL;
        }
    }
    m_vtID.clear();
}
