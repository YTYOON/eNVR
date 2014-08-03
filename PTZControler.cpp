// PTZControler.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "PTZControler.h"
#include "MMSystem.h"


// CPTZControler

CPTZControler::CPTZControler(HWND hwnd)
:m_hWnd(hwnd)
{
}

CPTZControler::~CPTZControler()
{
}

BOOL CPTZControler::bGetJoyStickInfo()
{
	JOYINFO joyinfo; 
	UINT wNumDevs, wDeviceID; 
	BOOL bDev1Attached, bDev2Attached; 

	if((wNumDevs = joyGetNumDevs()) == 0) 
		return FALSE; 
	bDev1Attached = joyGetPos(JOYSTICKID1,&joyinfo) != JOYERR_UNPLUGGED; 
	bDev2Attached = wNumDevs == 2 && joyGetPos(JOYSTICKID2,&joyinfo) != 
		JOYERR_UNPLUGGED; 
	if(bDev1Attached || bDev2Attached)   // decide which joystick to use 
		wDeviceID = bDev1Attached ? JOYSTICKID1 : JOYSTICKID2; 
	else 
		return FALSE; 

	while (MMRESULT nJoyRet = joySetCapture(this->m_hWnd, JOYSTICKID1, 0, FALSE)) 
    { 

		{
			ETROLOG(_T("JoySetCaputure fail return code %d\r\n"), nJoyRet);
			break;
		}
    } 

	return true;
}


