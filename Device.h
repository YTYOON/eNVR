#pragma once

// CDevice command target

class CDevice : public CObject
{
public:
	CDevice();
	virtual ~CDevice();
    void operator =( const CDevice& device);
    bool operator==( const CDevice& device)
	{return !(memcmp(device.mac, mac, sizeof(mac)) && memcmp(device.macParent, macParent, sizeof(macParent)));}
public:
    BYTE            macParent[6];
	BYTE			mac[6];	// destination mac address
	INT				nType;	// ENCP_DHCP, ENCP_STATIC, ENCP_PPPOE
	INT				cameraid;
	INT				camera_idx;
	char			camera_name[128];
	char			ip[16];	// ip address on the net
	char			hostname[HOSTNAME_LENGTH];	// host name
	char			modelname[MODEL_LENGTH];	// model name
	BOOL			bAudio;			// is Audio functional?
	BOOL			bAccepGuest;	// is guest account allowed
	WORD            nAPPort;
	WORD            nAP2Port;
	WORD            nWebPort;
	WORD            nAVType;
	WORD            nAV2Type;
	BOOL            bAV;
	BOOL            bAV2;
	BYTE            firmwarever[4];
	char            sn[17];
	BYTE            nGroup;
	char            szUser[MAX_USER_NAME];
	char            szPass[MAX_PASS_WORD];
	TCHAR           wcAlias[32];
	WORD            nGrid;
	HANDLE          hGrowupThread;
	BOOL            bChecked;
	void*           pIPCamCGIInfo;
	void*           hEncp;
	enum ipcamstatus {NOTEXIST=0, LIVE=1, RESETTING=2};
	ipcamstatus     nStatus;
	bool            bFlip;
	bool            bUpsideDown;
	bool            bRecording;
	HTREEITEM       hTreeItem;
    enum PROTOCOL   {VSMP=1, CGI=2, RTSP=4, RTPOVERTCP=8, RTPOVERHTTP=16, AVIFILE=32, CGI_ISAP=64};
	WORD            nProtocol;
	WORD            nProtocolCapability;
	WORD            nStreamID;
	DWORD			nENVR;	//	NVR Brand
	enum NVR_BRAND	{LOCAL=0, REMOTE_ETROCENTER=1, ISAP=2};
	TCHAR			stream_url[64];			

	//Lynn modified for surveillance show at 2013/10/25 
	BYTE        bFishEye;
	BYTE		dyDeviceType;
	BYTE        szReverse1;
	BYTE        szReverse2;

	////////////////////////////////////////Larry add 102-11-08//////////////////////////////////////////
	int			nPTZAutorunType;	//GET /config/ptz_autorun.cgi //name=none (0),	patrol (1), scan (2)//
	BOOL		bPTZAutoFocus;
	BOOL		bSupportPTZ;
	int			nPatrolNum;		//ptz_patrol_info.cgi#num=1//
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//char			stream_url[50];
	//WORD			nLiveviewID;
	//__tagNODEITEM(){ memset(&nType, 0, sizeof(__tagNODEITEM)); }
	

};


