#pragma once

class COSFacade
{
public:
    COSFacade(void);
    ~COSFacade(void);
    void ShutDown(){ShutDown(EWX_SHUTDOWN);};
    void Logoff(){ShutDown(EWX_LOGOFF);}; // //  EWX_RESTARTAPPS
    void Restart(){ShutDown(EWX_REBOOT);};
private:
    void ShutDown(int shutdownType);
};
