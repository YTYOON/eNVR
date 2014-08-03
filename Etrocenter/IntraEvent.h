#pragma once
enum EVENTTYPE 
{
	// Inter-event
	MOTION_DETECTION =0, DIGITAL_INPUT=1, VIDEO_LOST =2, VIDEO_OPEN =3, REBOOT =4, DIGITAL_OUTPUT =5, MISSING_OBJECT = 6, EVENT_TRIGGER =7, TRANSACTION = 8,
	// Intra-event
	DISK_FULL = 64, LOG_IN = 65, LOG_OUT = 66, DISCONNECT = 67, RECORDING_FAIL = 68 
};
/*
// original
// CIntraEvent command target
enum EVENTTYPE 
{    MOTION_DETECTION =0, DIGITAL_INPUT=1, VIDEO_LOST =2, VIDEO_OPEN =3, REBOOT =4, DIGITAL_OUTPUT =5,
     MISSING_OBJECT = 6, EVENT_TRIGGER =7, TRANSACTION = 8, DISK_FULL = 9, RECORDING_FAIL = 10, LOG_IN = 11, LOG_OUT = 12, DISCONNECT = 13
};
*/
struct INTERNAL_EVENTINFO
{
    union EXTRA
    {
        BYTE  mac[6];
        TCHAR msg[64];
    };    
    BOOL   _nStatus;
    DWORD  _eventtype;
    time_t _occurtTime;
    EXTRA _extra;
};

class CIntraEvent : public CObject
{
public:
	CIntraEvent();
	virtual ~CIntraEvent();
    static bool g_bRecrodingStatus;
	static bool bSendInternalEvent(int nErrorCode){INTERNAL_EVENTINFO rec_info ={}; rec_info._eventtype = nErrorCode; rec_info._occurtTime = CTime::GetCurrentTime().GetTime(); return bSendInternalEvent(rec_info);};
    static bool bSendInternalEventEx(BYTE* szMac, int nErrorCode){
		INTERNAL_EVENTINFO rec_info ={}; 
		memcpy(rec_info._extra.mac, szMac, sizeof(rec_info._extra.mac));
		rec_info._eventtype = nErrorCode; 
		rec_info._occurtTime = CTime::GetCurrentTime().GetTime(); 
		return bSendInternalEvent(rec_info);
	};
    static bool bTriggerRecordingStatus(BYTE* szMac, bool bStatus){
        INTERNAL_EVENTINFO rec_info ={}; 
        memcpy(rec_info._extra.mac, szMac, sizeof(rec_info._extra.mac));
        rec_info._nStatus = bStatus;
        rec_info._eventtype = EVENTTYPE::RECORDING_FAIL; 
        rec_info._occurtTime = CTime::GetCurrentTime().GetTime(); 
        return bSendInternalEvent(rec_info);
    };
    static bool bTriggerDeleteRecordingFileEvent(CString& cs)
    {
        INTERNAL_EVENTINFO rec_info ={}; 
        _tcsncpy(rec_info._extra.msg, cs.GetBuffer(), sizeof(rec_info._extra.msg)/sizeof(TCHAR)-1);
        rec_info._eventtype = EVENTTYPE::DISK_FULL; 
        rec_info._occurtTime = CTime::GetCurrentTime().GetTime(); 
        return bSendInternalEvent(rec_info);
    }
    static bool bSendInternalEvent(INTERNAL_EVENTINFO& Rec_info)
    {
        bool fSuccess(false);
        int nRetry(100);
        while (--nRetry)
        {
            LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\INTERNAL_EVENT_SERVICE_CONNECTION");
            CSA sa;
            HANDLE hEventRecPipe = CreateFile(
               lpszPipename,
               GENERIC_READ |  // read and write access 
               GENERIC_WRITE, 
               0,              // no sharing 
               NULL,           // default security attributes
               OPEN_EXISTING,  // opens existing pipe 
               0,              // default attributes 
               NULL);

            if(hEventRecPipe == INVALID_HANDLE_VALUE)
            {
				TRACE(_T("[bSendInternalEvent]: CreateFile Error code :%d\r\n"), GetLastError());
                continue;
            }

            DWORD cbWritten = 0;

            fSuccess = WriteFile(hEventRecPipe, 
                 (LPVOID)&Rec_info, 
                 sizeof(INTERNAL_EVENTINFO), 
                 &cbWritten, 
                 NULL);

            if(!fSuccess)
            {
                printf("Write file Error: %d", GetLastError());
            }
            else
            {
                FlushFileBuffers(hEventRecPipe);
                ::CloseHandle(hEventRecPipe);
                break;
            }
        }
        return fSuccess;
    }

};


