#pragma once
#include "afx.h"

class CEventManager :
	public CObject
{
public:
	CEventManager(void);
	~CEventManager(void);

	CDatabase*	pDB;
 
	struct EVENTINFO
	{
		int	EventId;
		int	camera_idx;
		char	event_source_code[10];
		char	event_type_code[10];
		int		target_camera_idx;
		int		CameraIdx;	
		int		StreamId;
		int		AlertLevel;
		int		TargetCamIdx;
		int		PTZPoint;
		int	    Interval;
		int		nWebPort;
		CString csEventName;
		CString csEventSource;
		CString csEventType;
		CString csEventAction;
		CString csAlarmLevel;
		CString csLifeCycle;
		CString csDuration;
		CString csDurationFrom;
		CString csDurationTo;
		CString	cActive;
		CString csIP;
		CString csUsername;
		CString csPass;
		CString csModelName;
		CString csParam;
	};

	void fnInit(void);
	bool fnLoadEvent(int oCamIdx);
	bool fnLoadEvent(void);
	

private:
	EVENTINFO* m_eventInfo;
	vector <EVENTINFO*> mvt_eventInfo;
};
