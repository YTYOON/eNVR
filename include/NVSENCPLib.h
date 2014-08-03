#ifndef __NVSLIBENCP_H____
#define __NVSLIBENCP_H____


#ifndef EXTERN_C
#define EXTERN_C extern "C"
#endif

//#define USE_LIB_IN_DLL
#ifdef WIN32
  #ifdef USE_LIB_IN_DLL

    #ifdef NVSENCPLIB_EXPORTS
      #define NVSENCPLIB_API EXTERN_C __declspec(dllexport)
    #else
      #define NVSENCPLIB_API EXTERN_C __declspec(dllimport)
    #endif

  #else

    #ifdef NVSENCPLIB_EXPORTS
      #error "This project is dll-based project..."
    #endif

    #define NVSENCPLIB_API EXTERN_C
  #endif
#else
  #define NVSENCPLIB_API
#endif



#define     HOSTNAME_LENGTH				32
#define     DOMAIN_LENGTH				64
#define     MODEL_LENGTH				64

#define		MAX_IP_LENGTH				16
#define		MAX_PPPOE_ID_LEN			16
#define		MAX_PPPOE_PASS_LEN			32
#define		MAX_PPPOE_SERVICE_LEN		32

#ifndef OUT
#define OUT						// Output parameter
#endif
#ifndef IN
#define IN						// Input parameter
#endif
#ifndef CALLBACK
#define CALLBACK
#endif
#ifndef BYTE
typedef unsigned char BYTE;
#endif
#ifndef BOOL
typedef int BOOL;
#endif
#ifndef LPCSTR
        #define LPCSTR const char*
#endif
#ifndef LPARAM
        #define LPARAM long
#endif

typedef enum _ENCPRESULT
{
	ENERR_OK = 0,
	ENERR_FAIL,				// general error
	ENERR_NOTIMPL,			// This feature is not implemented
	ENERR_CALL_INIT,		// encp_Initialise() has not called.
	ENERR_INVALIDARG,		// One or more arguments are invalid
	ENERR_OUTOFMEMORY,		// Ran out of memory
	ENERR_SOCKETERROR,		// unable to create socket
	ENERR_NOCLASSID,		// No such class id supported
	ENERR_HANDLE,
	ENERR_NOTSUPPORT
}ENCPRESULT;

typedef enum _ENCPCBMSG
{
	ENM_ENCPSEARCH,
	ENM_TYPE_MISMATCH,
	ENM_AUTHFAIL,
	ENM_SETFAIL,
	ENM_SETSUCCESS,
	ENM_UNKNOWN_ERROR,
	ENM_PERMDENY
}ENCPCBMSG;



typedef void*	HENCPITEM;

//
// related to Callback procedure function
// 
//#ifdef USE_LIB_IN_DLL
typedef void (CALLBACK *ENCPERRORCALLBACKPROC) ( ENCPRESULT, LPCSTR );
typedef void (CALLBACK *ENCPRECEIVECALLBACKPROC) ( HENCPITEM, ENCPCBMSG, LPARAM );
//#else
//typedef void (*ENCPERRORCALLBACKPROC) ( ENCPRESULT, LPCSTR );
//typedef void (*ENCPRECEIVECALLBACKPROC) ( HENCPITEM, ENCPCBMSG, LPARAM);
//#endif



typedef struct  _tagENCPINITSTRUCT
{
	ENCPRECEIVECALLBACKPROC lpfnReceiveCallback;
	ENCPERRORCALLBACKPROC lpfnErrorCallback;

#define ENCP_UDP		2			// SOCK_DGRAM
#define ENCP_RAW		3			// SOCK_RAW
	int				nSocketType;
}ENCPINITSTRUCT, *PENCPINITSTRUCT;


//
// struct ENCPNETWORK is used when retrieve or write network settings from or to the video server.
// 

typedef struct _tagENCPNETWORK
{
#define EN_STATICIP		0
#define EN_DHCP			1
#define EN_PPPOE		2
	int nettype;								// Network Type => Static IP, DHCP, PPPoE

	// Static IP Information
	struct
	{
		char ip[MAX_IP_LENGTH];					// Camera Server's static IP Address
		char netmask[MAX_IP_LENGTH];			// Camera Server's static Subnet mask Address
		char gateway[MAX_IP_LENGTH];			// Camera Server's static Gateway Address
		char nameserver[MAX_IP_LENGTH];			// Camera Server's static Name Server Address
	}STATICIP;

	// PPPoE Information
	// when these struct is filled with all 0, video server doens't support PPPoE.
	struct
	{
		char id[MAX_PPPOE_ID_LEN];
		char password[MAX_PPPOE_PASS_LEN];
		char service[MAX_PPPOE_SERVICE_LEN];
		unsigned int 	mtu;
#define DNS_AUTOMATIC		0
#define DNS_MANUAL			1
		unsigned char 	dns_server_type;	// DNS_DYNAMIC(0), DNS_STATIC(1)
		char nameserver[MAX_IP_LENGTH];
	}PPPOE;
}ENCPNETWORK, *PENCPNETWORK;

typedef union _tagMAC {
	BYTE cam_mac[6]; // destination mac address char type array
	struct{
		BYTE m1;
		BYTE m2;
		BYTE m3;
		BYTE m4;
		BYTE m5; 
		BYTE m6;
#ifndef ARM4
	}S_mac;
} 
#else
	} __attribute__ ((packed)) S_mac;
} __attribute__ ((packed))
#endif
MAC;

typedef struct _tagENCPNODEINFO
{
	MAC		mac;
	char ip[MAX_IP_LENGTH];
	char modelName[MODEL_LENGTH];
	char hostName[HOSTNAME_LENGTH];
	char reserved[32];
	char domainName[DOMAIN_LENGTH];
#ifndef ARM4
}
#else
} __attribute__ ((packed))
#endif
ENCPNODEINFO, *PENCPNODEINFO;

typedef struct _tagENCPPORTINFO
{
	unsigned short av_port;
	unsigned short web_port;
	unsigned short av2_port;
	unsigned short config_port;
	unsigned char av_type;
	unsigned char av2_type;
	unsigned char av_enable;
	unsigned char av2_enable;
#ifndef ARM4
}
#else
} __attribute__ ((packed))
#endif
ENCPPORTINFO, *PENCPPORTINFO;

typedef struct _tagENCPFWINFO
{
	unsigned char version[4];
	char sn[16];
#ifndef ARM4
}
#else
} __attribute__ ((packed))
#endif
ENCPFWINFO, *PENCPFWINFO;

#ifndef _NVS_VERSION_STRUCTURE_
#define _NVS_VERSION_STRUCTURE_
// Call to get the version of NVSENCPLib,
typedef struct NVSVersion {
	int major;
	int	minor;
	int release;
	int build;
} NVSVersion, *pNVSVersion;
#endif




NVSENCPLIB_API	ENCPRESULT encp_Initialise( IN PENCPINITSTRUCT init );
NVSENCPLIB_API	ENCPRESULT encp_Close( );
NVSENCPLIB_API	ENCPRESULT encp_Search( );

NVSENCPLIB_API	ENCPRESULT encp_GetNodeInfo( IN HENCPITEM hItem, OUT PENCPNODEINFO pNodeInfo );
NVSENCPLIB_API	ENCPRESULT encp_GetNetworkInfo( IN HENCPITEM hItem, OUT PENCPNETWORK pNetInfo );
NVSENCPLIB_API	ENCPRESULT encp_SetNetworkInfo( IN HENCPITEM hItem, IN LPCSTR lpszAuthID, IN LPCSTR lpszAuthPass, IN PENCPNETWORK pNetInfo );
NVSENCPLIB_API	ENCPRESULT encp_SetHostName( IN HENCPITEM hItem, IN LPCSTR lpszAuthID, IN LPCSTR lpszAuthPass,
										 IN LPCSTR lpszHostName, IN LPCSTR lpszDomainName );

NVSENCPLIB_API	ENCPRESULT encp_IsAudioEnabled( IN HENCPITEM hItem, OUT BOOL * pEnable );

NVSENCPLIB_API	ENCPRESULT encp_GetGuestPermission( IN HENCPITEM hItem, OUT BOOL * pPermission );
NVSENCPLIB_API	ENCPRESULT encp_SetAccountInfo( IN HENCPITEM hItem, IN LPCSTR lpszAuthID, IN LPCSTR lpszAuthPass,
										   IN LPCSTR lpszNewPassword, IN BOOL * pNewGuestPermission );

// Only support for Version 2.x
NVSENCPLIB_API ENCPRESULT encp_UseVersion(int);
NVSENCPLIB_API ENCPRESULT encp_GetPortInfo(HENCPITEM, PENCPPORTINFO);
NVSENCPLIB_API ENCPRESULT encp_GetFWInfo(HENCPITEM, PENCPFWINFO);
NVSENCPLIB_API ENCPRESULT encp_Initialise2(IN PENCPINITSTRUCT init, char *ip);

NVSENCPLIB_API	NVSVersion encp_GetVersion();

#endif

