#ifndef _CPUUSAGE_H
#define _CPUUSAGE_H

#include <windows.h>

class CCpuUsage
{
public:
	CCpuUsage();
	virtual ~CCpuUsage();

// Methods
	int GetCpuUsage();
	inline int GetCpuUsage(LPCTSTR pProcessName);
	inline int GetCpuUsage(DWORD dwProcessID);

	BOOL EnablePerformaceCounters(BOOL bEnable = TRUE);

// Attributes
private:
	bool			m_bFirstTime;
	LONGLONG		m_lnOldValue ;
	LARGE_INTEGER	m_OldPerfTime100nSec;
};


#endif