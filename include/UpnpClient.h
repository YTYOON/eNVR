// UpnpClient.h : main header file for the UpnpClient DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "mycurl.h"

// CUpnpClientApp
// See UpnpClient.cpp for the implementation of this class
//
#define	bzero(a, b)					memset(a, 0, b)
#define	socklen_t						int

//#define	UPNP_UDP_PACKET_SIZE			1536
#define	UPNP_UDP_PACKET_SIZE			10240

#define	UPNP_CAM_PORT				3702	// discovery responded port

#define	UPNP_MULTICAST_IP			"239.255.255.250"

//#define	UPNP_PORT					1900
#define	UPNP_PORT					3702

#define	UPNP_RESPONSE_HEADER		"HTTP/1.1 200 OK\r\n"

#define	UPNP_LOCATION				"LOCATION: "

/////////////////////////////////////////////////////

//#define	DISCOVERY_KEY_MAC			"Address"
#define	DISCOVERY_KEY_MAC			"Address>urn:uuid:"

#define	DISCOVERY_KEY_FUNCTION		"NetworkVideoTransmitter"

#define	DISCOVERY_KEY_MODEL			"hardware/"

#define	DISCOVERY_KEY_SERVICE		"XAddrs>"

size_t	g_iLenKEY_MAC, g_iLenKEY_Function, g_iLenKEY_Model, g_iLenKEY_Service;



//var
SOCKADDR_IN		g_saAdvertisement;
size_t			g_iLenResponseHeader;

#define	TEST_SONY					1

typedef	union _IPv4
{
	BYTE							a_b [4];
	DWORD							dw;
	struct in_addr						in;
} IPv4, *PIPv4;
// Find result
typedef struct _DEV_CAMERA
{
	IPv4								ip;
	WORD							wPort;
	char								uuid[64];		// null string means no uuid info.
	char								Name[128];
	char								Xml[128];
	//char								MAC[13];
	BYTE							MAC[6];	// destination mac address
	union
	{
		char							Manufacturer[128];
		DWORD						dwId;
	};
} DEV_CAMERA, *PDEV_CAMERA;

typedef struct _DEVICE_LIST
{
	DWORD							dwFound;
	DEV_CAMERA						Camera [1];
} DEVICE_LIST, *PDEVICE_LIST;

//const	char			szDiscovery[] =	"M-SEARCH * HTTP/1.1\r\nHost:239.255.255.250:1900\r\nST:upnp:rootdevice\r\nMan:\"ssdp:discover\"\r\nMX:3\r\n\r\n";

const	char			szDiscovery_1[] =	"<?xml version=\"1.0\" encoding=\"utf-8\"?><Envelope xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" xmlns=\"http://www.w3.org/2003/05/soap-envelope\"><Header><wsa:MessageID xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">uuid:e72322fb-6512-4d1b-99b1-";
const	char			szDiscovery_2[] =	"</wsa:MessageID><wsa:To xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">urn:schemas-xmlsoap-org:ws:2005:04:discovery</wsa:To><wsa:Action xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe</wsa:Action></Header><Body><Probe xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\"><Types>tds:Device</Types><Scopes /></Probe></Body></Envelope>";

const	char			szDiscovery_3[] =	"<?xml version=\"1.0\" encoding=\"utf-8\"?><Envelope xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns=\"http://www.w3.org/2003/05/soap-envelope\"><Header><wsa:MessageID xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">uuid:4a4e5e49-62eb-49de-95b1-";
const	char			szDiscovery_4[] =	"</wsa:MessageID><wsa:To xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">urn:schemas-xmlsoap-org:ws:2005:04:discovery</wsa:To><wsa:Action xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe</wsa:Action></Header><Body><Probe xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\"><Types>dn:NetworkVideoTransmitter</Types><Scopes /></Probe></Body></Envelope>";

//PTZ
typedef enum _PTZ_MOVE
{
	MOVE_UP_LEFT,
	MOVE_UP,
	MOVE_UP_RIGHT,
	MOVE_LEFT,
	MOVE_HOME,
	MOVE_RIGHT,
	MOVE_DOWN_LEFT,
	MOVE_DOWN,
	MOVE_DOWN_RIGHT,
	MOVE_ZOOM_INCREASE,
	MOVE_ZOOM_DECREASE,
	MOVE_FOCUS_INCREASE,
	MOVE_FOCUS_DECREASE,
	MOVE_FOCUS_AUTO,
	MOVE_SCAN_AUTO,
	MOVE_PATROL_AUTO,
} PTZ_MOVE; 

extern "C" __declspec(dllexport) PDEVICE_LIST ONVIF_Discovery(int);
//extern "C" __declspec(dllexport) int ONVIF_Discovery(int);

extern "C" __declspec(dllexport) void ONVIF_FreeListBuffer(void);

//extern "C" __declspec(dllexport) int UpnpSendDescription(int);
extern "C" __declspec(dllexport) CString ONVIF_GET_RTSP(CString url, CString csUser, CString csPWD);
extern "C" __declspec(dllexport) CString ONVIF_GET_PTZ_Capability(CString url, CString csUser, CString csPWD);
extern "C" __declspec(dllexport) void ONVIF_SET_PTZ(CString url, CString csUser, CString csPWD, PTZ_MOVE enumMove);

class CUpnpClientApp : public CWinApp
{
public:
	CUpnpClientApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
