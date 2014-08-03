#include <Windows.h>

struct CPUInfo{
	
	int nType;
	int nCPUInfo[4];

};

class CGetSystemInfo
{
public:
	CGetSystemInfo(void);
	~CGetSystemInfo(void);

protected:
	//--CPU Info
	void GetCpuID(CPUInfo& m_CPUInfo);
	void CheckMode();

	CPUInfo m_CPUInfo;
	CPUInfo m_RegularCPUInfo[5];

	//--Memory Info
	MEMORYSTATUSEX m_statex;
	void vGetGlobalMemoryStatusEx(MEMORYSTATUSEX m_statex);
	int m_iDIV, m_iWIDTH;

	//--OS Version Info
	DWORD dwVersion, dwMajorVersion, dwMinorVersion, dwBuild;
	void vGetOSVersionInfo();

    CString getVersionMark();
    bool bEmbeddedWin7();
public:
	//--CPU Info
	//CString csGetCPUID();
	bool bCheckAuthorization();
	
	//--Memory Info
	void		vSetDIV(int iDIV = 1024);
	void		vSetWIDTH(int iWIDTH = 7);
	DWORD		dwGetMemoryLoad();
	DWORDLONG	ullGetTotalPhys();
	DWORDLONG	ullGetAvailPhys();
	DWORDLONG	ullGetTotalPageFile();
	DWORDLONG	ullGetAvailPageFile();
	DWORDLONG	ullGetTotalVirtual();
	DWORDLONG	ullGetAvailVirtual();
	DWORDLONG	ullGetAvailExtendedVirtual();

	//--OS Version Info
	DWORD	dwGetMajorVersion();
	DWORD	dwGetMinorVersion();
	DWORD	dwGetBuildNumber();
};