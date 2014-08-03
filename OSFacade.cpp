#include "stdafx.h"
#include "OSFacade.h"

COSFacade::COSFacade(void)
{
}

COSFacade::~COSFacade(void)
{
}

void COSFacade::ShutDown(int shutdownType)
{
   DWORD dwVersion = GetVersion();   
   if (dwVersion < 0x80000000) // Windows NT  
   {   
        HANDLE hToken;     
        TOKEN_PRIVILEGES tkp;   
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))    
            return;     
        LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);    
        tkp.PrivilegeCount = 1; // one privilege to set          
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;     
        AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);     
        if (!ExitWindowsEx(shutdownType | EWX_FORCE, 0))     
            return;     
   	}  
   	else  
    {   
        if (!ExitWindowsEx(shutdownType | EWX_FORCE, 0))    
            return;     
   	}     
}
