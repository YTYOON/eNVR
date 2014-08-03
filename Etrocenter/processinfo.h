#include "stdafx.h"

#ifndef __PROCESSINFO_H__
#define __PROCESSINFO_H__


#include <pdh.h>
#include <Iphlpapi.h>
#include <string>
#include <hash_map>
/////////////////////
#include   <windows.h>   
#include   <conio.h>   
#include   <stdio.h>   
///////////////////

using namespace std;
using namespace stdext;

#pragma comment(lib, "pdh.lib")
#pragma comment(lib, "Iphlpapi.lib")

#define PROCESSOR   "\\Processor(%s)\\%s"
#define PROCESS     "\\Process(%s)\\%s"

#define BYTE_RECV_SEC       "Bytes Received/sec"
#define BYTE_SENT_SEC       "Bytes Sent/sec"
#define BYTE_TOTAL_SEC      "Bytes Total/sec"

#define PACKET_RECV_SEC     "Packets Received/sec"
#define PACKET_SENT_SEC     "Packets Sent/sec"
#define PACKET_TOTAL_SEC    "Packets/sec"
/////////////////////////////////////

    
#define   SystemBasicInformation   0   
#define   SystemPerformanceInformation   2   
#define   SystemTimeInformation   3   
    
#define   Li2Double(x)   ((double)((x).HighPart)   *   4.294967296E9   +   (double)((x).LowPart)) 

typedef   struct   
{   
  DWORD   dwUnknown1;   
  ULONG   uKeMaximumIncrement;   
  ULONG   uPageSize;   
  ULONG   uMmNumberOfPhysicalPages;   
  ULONG   uMmLowestPhysicalPage;   
  ULONG   uMmHighestPhysicalPage;   
  ULONG   uAllocationGranularity;   
  PVOID   pLowestUserAddress;   
  PVOID   pMmHighestUserAddress;   
  ULONG   uKeActiveProcessors;   
  BYTE   bKeNumberProcessors;   
  BYTE   bUnknown2;   
  WORD   wUnknown3;   
}   SYSTEM_BASIC_INFORMATION;   


    
typedef   struct   
{   
  LARGE_INTEGER   liIdleTime;   
  DWORD   dwSpare[76];   
}   SYSTEM_PERFORMANCE_INFORMATION;   
    
typedef   struct   
{   
  LARGE_INTEGER   liKeBootTime;   
  LARGE_INTEGER   liKeSystemTime;   
  LARGE_INTEGER   liExpTimeZoneBias;   
  ULONG   uCurrentTimeZoneId;   
  DWORD   dwReserved;   
}   SYSTEM_TIME_INFORMATION;   
    
typedef   LONG   (WINAPI   *PROCNTQSI)(UINT,PVOID,ULONG,PULONG);   
     
////////////////////////////////////////

typedef hash_map<string, HCOUNTER*> CHCOUNTERMap;

class CProcessInfo
{
public:
    CProcessInfo();
    ~CProcessInfo();
 
    bool Init();
 
    bool ReSetCollectQueryData();

 
    bool AddTotalCPUCounter();
    DWORD GetTotalCPUValue();
 
    bool AddProcCPUCounter(const char * pszProc);
    DWORD GetProcCPUValue(const char * pszProc);

 
    bool AddElapsedTimeCounter(const char * pszProc);
    DWORD GetElapsedTimeValue(const char * pszProc);

 
    bool AddMemoryCounter(const char * pszProc);
    DWORD GetMemoryValue(const char * pszProc);

 
    bool AddNetCounter(const char * pszIP, const char *pszDesc);
    DWORD GetNetValue(const char * pszIP, const char *pszDesc);
    
 
    bool AddCounter(const char * pszDesc);
    DWORD GetCounterValue(const char * pszDesc);

 
    int GetProcNumber()             { return m_nProcNumber; }
   
    long GetTotalPhysMemory()       { return m_nMemory; }
 
    long GetAvailPhysMemory();

protected:
    
    const char* _GetAdaptersInfoByIP(const char* pszIP);
    HCOUNTER *_FindHCounter(const char * pszDesc);

    bool _AddCounter(const char * pszDesc);
    DWORD _GetCounterValue(const char * pszDesc);

private:
    int             m_nProcNumber;           
    long            m_nMemory;              
    int             m_nCntrs;
    HQUERY          m_hQuery;
    HCOUNTER*       m_pca;
    PDH_STATUS      m_pdhStatus; 

    PIP_ADAPTER_INFO    m_pAdInfo;

    CHCOUNTERMap    m_oHCountMap;

};

#endif
