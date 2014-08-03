#pragma once

// CEVDatabase command target

class CEVDatabase : public CDatabase
{
public:
	CEVDatabase();
	virtual ~CEVDatabase();
protected:
	bool bDBConnect(void);
	bool bDectecDBReady(void);
};


