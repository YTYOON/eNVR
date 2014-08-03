#include "stdafx.h"
#include "GetCpuID.h"
    #include <intrin.h>

CGetSystemInfo::CGetSystemInfo()
{
	::ZeroMemory(&m_CPUInfo, sizeof(m_CPUInfo));
	GetCpuID(m_CPUInfo);
	CheckMode();

	vGetGlobalMemoryStatusEx(m_statex);
	vSetWIDTH();
	vSetDIV();

	vGetOSVersionInfo();
}

CGetSystemInfo::~CGetSystemInfo()
{

}

//CString CGetSystemInfo::csGetCPUID()
//{
//	return this->m_csLocalCpuID;
//}

void CGetSystemInfo::GetCpuID(CPUInfo &m_CPUInfo)
{
	BOOL bException = FALSE;
    BYTE szCpu[32]  = { 0 };        
    UINT  uCpuID     = 0U;
    char szBuf[256] = {0};
    UINT s1 = 0U;
    UINT s2 = 0U;
    //int CPUInfo[4]={};
    //int InfoType(0);
    //__cpuid(CPUInfo, InfoType);
	__cpuid(m_CPUInfo.nCPUInfo, m_CPUInfo.nType);

#if defined(Debug)

	CString csAfxMessage;
	csAfxMessage.Format(_T("InfoType %d"), InfoType);
	::AfxMessageBox(csAfxMessage);
	csAfxMessage.Format(_T("CPUInfo[0] = 0x%x"), CPUInfo[0]);
	::AfxMessageBox(csAfxMessage);
	csAfxMessage.Format(_T("CPUInfo[1] = 0x%x"), CPUInfo[1]);
	::AfxMessageBox(csAfxMessage);
	csAfxMessage.Format(_T("CPUInfo[2] = 0x%x"), CPUInfo[2]);
	::AfxMessageBox(csAfxMessage);
	csAfxMessage.Format(_T("CPUInfo[3] = 0x%x"), CPUInfo[3]);
	::AfxMessageBox(csAfxMessage);

#endif

	return;
}

void CGetSystemInfo::CheckMode()
{
#if defined(NVR16)
	//--
#elif defined(NVR24)
#elif defined(NVR32)
	m_RegularCPUInfo[0].nType = 0;
	m_RegularCPUInfo[0].nCPUInfo[0] = 13;
	m_RegularCPUInfo[0].nCPUInfo[1] = 1970169159;
	m_RegularCPUInfo[0].nCPUInfo[2] = 1818588270;
	m_RegularCPUInfo[0].nCPUInfo[3] = 1231384169;
#elif defined(NVR64)
	//--
#else
	m_RegularCPUInfo[0].nType = 0;
	m_RegularCPUInfo[0].nCPUInfo[0] = 13;
	m_RegularCPUInfo[0].nCPUInfo[1] = 1970169159;
	m_RegularCPUInfo[0].nCPUInfo[2] = 1818588270;
	m_RegularCPUInfo[0].nCPUInfo[3] = 1231384169;
#endif
	return;
}

bool CGetSystemInfo::bCheckAuthorization()
{
	/*for(int iTemp = 0; iTemp < 5; iTemp++)
	{
		if(m_RegularCPUInfo[0].nType == m_CPUInfo.nType  && m_RegularCPUInfo[0].nCPUInfo[0].)
		if(m_CPUInfo.nType == )
	}*/
    //bEmbeddedWin7();
    //getVersionMark();
    bool bRet(false);
	for(int iTemp = 0; iTemp < 5; iTemp++)
	{
		if(m_RegularCPUInfo[iTemp].nType == m_CPUInfo.nType &&  
            m_RegularCPUInfo[iTemp].nCPUInfo[0] == m_CPUInfo.nCPUInfo[0] && m_RegularCPUInfo[iTemp].nCPUInfo[1] == m_CPUInfo.nCPUInfo[1] && m_RegularCPUInfo[iTemp].nCPUInfo[2] == m_CPUInfo.nCPUInfo[2] && m_RegularCPUInfo[iTemp].nCPUInfo[3] == m_CPUInfo.nCPUInfo[3])
		{
            if (bEmbeddedWin7())
            {
			    bRet = true;
            }
            break;
		}
	}
	return bRet;
}

void CGetSystemInfo::vGetGlobalMemoryStatusEx(MEMORYSTATUSEX m_statex)
{
	m_statex.dwLength = sizeof(m_statex);

	GlobalMemoryStatusEx(&m_statex);

	return;
}

DWORD CGetSystemInfo::dwGetMemoryLoad()
{
	return m_statex.dwMemoryLoad;
}

DWORDLONG CGetSystemInfo::ullGetTotalPhys()
{
	return m_statex.ullTotalPhys;
}

DWORDLONG CGetSystemInfo::ullGetAvailPhys()
{
	return m_statex.ullAvailPhys;
}

DWORDLONG CGetSystemInfo::ullGetTotalPageFile()
{
	return m_statex.ullTotalPageFile;
}

DWORDLONG CGetSystemInfo::ullGetAvailPageFile()
{
	return m_statex.ullAvailPageFile;
}

DWORDLONG CGetSystemInfo::ullGetTotalVirtual()
{
	return m_statex.ullTotalVirtual;
}

DWORDLONG CGetSystemInfo::ullGetAvailVirtual()
{
	return m_statex.ullAvailVirtual;
}

DWORDLONG CGetSystemInfo::ullGetAvailExtendedVirtual()
{
	return m_statex.ullAvailExtendedVirtual;
}

void CGetSystemInfo::vSetWIDTH(int iWIDTH)
{
	m_iWIDTH = iWIDTH;

	return;
}

void CGetSystemInfo::vSetDIV(int iDIV)
{
	m_iDIV = iDIV;

	return;
}

void CGetSystemInfo::vGetOSVersionInfo()
{
	dwVersion = ::GetVersion();

	return;
}

DWORD CGetSystemInfo::dwGetMajorVersion()
{
	dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));

	return dwMajorVersion;
}

DWORD CGetSystemInfo::dwGetMinorVersion()
{
	dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	return dwMinorVersion;
}

DWORD CGetSystemInfo::dwGetBuildNumber()
{
	if(dwVersion < 0x80000000)
		dwBuild = (DWORD)(HIWORD(dwVersion));
	else
		dwBuild = 0;

	return dwBuild;
}

CString CGetSystemInfo::getVersionMark()
{

    OSVERSIONINFOEX os; 
    os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX); 
    if(GetVersionEx((OSVERSIONINFO *)&os)){
        CString cs;
        cs.Format(_T("os.dwBuildNumber :%d, os.dwMajorVersion :%d, os.dwMinorVersion :%d, os.dwPlatformId :%d, os.szCSDVersion :%s, os.wProductType :%d, os.wServicePackMajor :%d, os.wServicePackMinor :%d, os.wSuiteMask %d"),os.dwBuildNumber, os.dwMajorVersion, os.dwMinorVersion, os.dwPlatformId, os.szCSDVersion, os.wProductType, os.wServicePackMajor, os.wServicePackMinor, os.wSuiteMask);
        AfxMessageBox(cs);

        CString vmark;

        switch(os.dwMajorVersion){                //先判斷操作系統版本 
            case 5: 
                switch(os.dwMinorVersion){ 
                    case 0:                  //Windows 2000 
                        if(os.wSuiteMask==VER_SUITE_ENTERPRISE) 
                            vmark=_T("Advanced Server"); 
                        break; 
                    case 1:                  //Windows XP 
                        if(os.wSuiteMask==VER_SUITE_EMBEDDEDNT) 
                            vmark=_T("Embedded"); 
                        else if(os.wSuiteMask==VER_SUITE_PERSONAL) 
                            vmark=_T("Home Edition"); 
                        else 
                            vmark=_T("Professional"); 
                        break; 
                    case 2: 
                        if(GetSystemMetrics(SM_SERVERR2)==0 && 

                        os.wSuiteMask==VER_SUITE_BLADE)  //Windows Server 2003 
                            vmark=_T("Web Edition"); 
                        else if(GetSystemMetrics(SM_SERVERR2)==0 && 
                        os.wSuiteMask==VER_SUITE_COMPUTE_SERVER) 
                            vmark=_T("Compute Cluster Edition"); 
                        else if(GetSystemMetrics(SM_SERVERR2)==0 && 
                        os.wSuiteMask==VER_SUITE_STORAGE_SERVER) 
                            vmark=_T("Storage Server"); 
                        else if(GetSystemMetrics(SM_SERVERR2)==0 && 
                        os.wSuiteMask==VER_SUITE_DATACENTER) 
                            vmark=_T("Datacenter Edition"); 
                        else if(GetSystemMetrics(SM_SERVERR2)==0 && 
                        os.wSuiteMask==VER_SUITE_ENTERPRISE) 
                            vmark=_T("Enterprise Edition"); 
                        else if(GetSystemMetrics(SM_SERVERR2)!=0 && 
                           os.wSuiteMask==VER_SUITE_STORAGE_SERVER) 
                            vmark=_T("Storage Server"); 
                        break; 
                } 
                break; 
            case 6: 
                switch(os.dwMinorVersion){ 
                    case 0: 
                        if(os.wProductType!=VER_NT_WORKSTATION && 
                        os.wSuiteMask==VER_SUITE_DATACENTER)  /* Windows Server 2008*/ 
                            vmark=_T("Datacenter Server"); 
                        else if(os.wProductType!=VER_NT_WORKSTATION && 
                        os.wSuiteMask==VER_SUITE_ENTERPRISE) 
                            vmark=_T("Enterprise"); 
                        else if(os.wProductType==VER_NT_WORKSTATION && 
                           os.wSuiteMask==VER_SUITE_PERSONAL)  //Windows Vista

                            vmark=_T("Home"); 
                        break; 
                } 
                break; 
            default: 
                vmark=_T(""); 
        } 
        return vmark; 
    } 
    else 
        return _T("");

}
bool CGetSystemInfo::bEmbeddedWin7()
{
    bool bRet(false);
    OSVERSIONINFOEX os; 
    os.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX); 
    if(GetVersionEx((OSVERSIONINFO *)&os))
    { 
        /*CString cs;
        cs.Format(_T("os.dwMinorVersion: %d os.wSuiteMask: %d VER_SUITE_EMBEDDEDNT: %d (os.wSuiteMask & VER_SUITE_EMBEDDEDNT) :%d"), os.dwMinorVersion, os.wSuiteMask, VER_SUITE_EMBEDDEDNT, (os.wSuiteMask & VER_SUITE_EMBEDDEDNT));
        AfxMessageBox(cs);*/
        if ((os.dwMajorVersion == 6)  && ((os.wSuiteMask & VER_SUITE_EMBEDDEDNT) == VER_SUITE_EMBEDDEDNT))
            bRet = true;
    }
    return bRet;
}