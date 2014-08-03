#include "stdafx.h"
#include "processInfo.h"

CProcessInfo::CProcessInfo()
:m_nProcNumber(0),m_nMemory(0),m_pAdInfo(NULL)
{
    m_oHCountMap.clear();
}

CProcessInfo::~CProcessInfo()
{
    if (NULL != m_pAdInfo)
    {
        delete [] m_pAdInfo;
        m_pAdInfo = NULL;
    }

    ReSetCollectQueryData();
    PdhCloseQuery(m_hQuery);
}

bool CProcessInfo::Init()
{
    SYSTEM_INFO sInfo;
    GetSystemInfo(&sInfo);

    MEMORYSTATUS memst;
    GlobalMemoryStatus(&memst);

    m_nProcNumber = sInfo.dwNumberOfProcessors;
    m_nMemory = (long)memst.dwTotalPhys;

    return (ERROR_SUCCESS == PdhOpenQuery(0, 0, &m_hQuery)); 
}

long CProcessInfo::GetAvailPhysMemory()
{
    MEMORYSTATUS memst;
    GlobalMemoryStatus(&memst);
    return (long)memst.dwAvailPhys;
}

bool CProcessInfo::ReSetCollectQueryData()
{
    CHCOUNTERMap::iterator it = m_oHCountMap.begin();
    while (it != m_oHCountMap.end())
    {
        if (ERROR_SUCCESS != PdhRemoveCounter((HCOUNTER)(*(it->second))))
        {
            int nErr = GetLastError();
        }
        if (GlobalFree(it->second) != NULL)
        {
            int nErr = GetLastError();
        }
        it->second = NULL;
        it = m_oHCountMap.erase(it);
    }

    return true;
}

bool CProcessInfo::AddTotalCPUCounter()
{
    char szCPUDesc[512];
    szCPUDesc[0] = '\0';

    sprintf_s(szCPUDesc, "\\Processor(_Total)\\%s Processor Time", "%");

    return _AddCounter(szCPUDesc);
}

DWORD CProcessInfo::GetTotalCPUValue()
{
    char szCPUDesc[512];
    szCPUDesc[0] = '\0';

    sprintf_s(szCPUDesc, "\\Processor(_Total)\\%s Processor Time", "%");

    _GetCounterValue(szCPUDesc);
    Sleep(500);

    return _GetCounterValue(szCPUDesc);
}

bool CProcessInfo::AddProcCPUCounter(const char * pszProc)
{
    char szCPUDesc[512];
    szCPUDesc[0] = '\0';

    sprintf_s(szCPUDesc, "\\Process(%s)\\%s Processor Time", pszProc, "%");

    return _AddCounter(szCPUDesc);
}

DWORD CProcessInfo::GetProcCPUValue(const char * pszProc)
{
    char szCPUDesc[512];
    szCPUDesc[0] = '\0';

    sprintf_s(szCPUDesc, "\\Process(%s)\\%s Processor Time", pszProc, "%");

    _GetCounterValue(szCPUDesc);
    Sleep(500);

    return _GetCounterValue(szCPUDesc);
}

bool CProcessInfo::AddElapsedTimeCounter(const char * pszProc)
{
    char szElapsedTimeDesc[512];
    szElapsedTimeDesc[0] = '\0';

    sprintf_s(szElapsedTimeDesc, "\\Process(%s)\\Elapsed Time", pszProc);

    return _AddCounter(szElapsedTimeDesc);
}

DWORD CProcessInfo::GetElapsedTimeValue(const char * pszProc)
{
    char szElapsedTimeDesc[512];
    szElapsedTimeDesc[0] = '\0';

    sprintf_s(szElapsedTimeDesc, "\\Process(%s)\\Elapsed Time", pszProc);

    return _GetCounterValue(szElapsedTimeDesc);
}

bool CProcessInfo::AddMemoryCounter(const char * pszProc)
{
    char szMemoryDesc[512];
    szMemoryDesc[0] = '\0';

    sprintf_s(szMemoryDesc, "\\Process(%s)\\Working Set", pszProc);

    return _AddCounter(szMemoryDesc);
}

DWORD CProcessInfo::GetMemoryValue(const char * pszProc)
{
    char szMemoryDesc[512];
    szMemoryDesc[0] = '\0';

    sprintf_s(szMemoryDesc, "\\Process(%s)\\Working Set", pszProc);

    return _GetCounterValue(szMemoryDesc);
}

bool CProcessInfo::AddNetCounter(const char * pszIP, const char *pszDesc)
{
    char szNetDesc[1024];
    szNetDesc[0] = '\0';

    sprintf_s(szNetDesc, "\\NetWork Interface(%s)\\%s", _GetAdaptersInfoByIP(pszIP), pszDesc);

    return _AddCounter(szNetDesc);
}

DWORD CProcessInfo::GetNetValue(const char * pszIP, const char *pszDesc)
{
    char szNetDesc[1024];
    szNetDesc[0] = '\0';

    sprintf_s(szNetDesc, "\\NetWork Interface(%s)\\%s", _GetAdaptersInfoByIP(pszIP), pszDesc);

    return _GetCounterValue(szNetDesc);
}

bool CProcessInfo::AddCounter(const char * pszDesc)
{
    return _AddCounter(pszDesc);
}

DWORD CProcessInfo::GetCounterValue(const char * pszDesc)
{
    return _GetCounterValue(pszDesc);
}

DWORD CProcessInfo::_GetCounterValue(const char * pszDesc)
{
    if(ERROR_SUCCESS == PdhCollectQueryData(m_hQuery))
    {
        HCOUNTER *pCounter = _FindHCounter(pszDesc);
        if (pCounter != NULL)
        {
            PDH_FMT_COUNTERVALUE szFmtValue={0};
            DWORD dwCtrType;
            if(ERROR_SUCCESS == PdhGetFormattedCounterValue(*pCounter, 
                                                            PDH_FMT_LONG, // PDH_FMT_DOUBLE, 
                                                            &dwCtrType, 
                                                            &szFmtValue))
            {
                return szFmtValue.longValue;
            }
        }
    }

    return -1;
}

bool CProcessInfo::_AddCounter(const char * pszDesc)
{
	CString csPszDesc;
	csPszDesc.Format(_T("%s"), csPszDesc);
	LPCTSTR lp;
	lp = csPszDesc;

    HCOUNTER* pca = (HCOUNTER *)GlobalAlloc(GPTR, (sizeof(HCOUNTER))); 
    m_oHCountMap[pszDesc] = pca;
    return (ERROR_SUCCESS == PdhAddCounter(m_hQuery, lp, 0, pca));
}

HCOUNTER *CProcessInfo::_FindHCounter(const char * pszDesc)
{
    CHCOUNTERMap::iterator it = m_oHCountMap.find(pszDesc);
    if (it == m_oHCountMap.end())
    {
        return NULL;
    }
    return it->second;
}

const char* CProcessInfo::_GetAdaptersInfoByIP(const char* pszIP)
{
    if (NULL != m_pAdInfo)
    {
        delete [] m_pAdInfo;
        m_pAdInfo = NULL;
    }

    ULONG    buflen;
    PIP_ADAPTER_INFO    pAdInfo_c = NULL;

    buflen = 0;
    GetAdaptersInfo(m_pAdInfo, &buflen);
    m_pAdInfo = (struct _IP_ADAPTER_INFO *)new UCHAR[buflen+1];

    if (GetAdaptersInfo(m_pAdInfo, &buflen) == ERROR_SUCCESS)
    {
        do
        {
            if (0 == strcmp(pszIP, m_pAdInfo->IpAddressList.IpAddress.String))
            {
                return m_pAdInfo->Description;
            }

        } while ((m_pAdInfo->Next != NULL) && (m_pAdInfo = m_pAdInfo->Next));
    }

    return NULL;
}