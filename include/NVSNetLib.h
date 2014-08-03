#ifndef	_NVSNET_LIB__
#define	_NVSNET_LIB__

#ifndef EXTERN_C
#define EXTERN_C extern "C"
#endif

#ifdef WIN32
	#ifdef _USRDLL
		#define NVSNETLIB_API EXTERN_C __declspec(dllexport)
	#elif _LIB
		#define NVSNETLIB_API EXTERN_C
	#else
		#define NVSNETLIB_API EXTERN_C __declspec(dllimport)
	#endif
#else
	#define NVSNETLIB_API
#endif

#define NETSESSION_ERROR								-1
#define NETSESSION_OK									0



#define NETSESSION_STAT_INIT							0
#define NETSESSION_STAT_CONNECTING						1
#define NETSESSION_STAT_SEND_LOGIN						2
#define NETSESSION_STAT_WAIT_LOGIN						3
#define NETSESSION_STAT_RECV							4
#define NETSESSION_STAT_CLOSE							5
#define NETSESSION_STAT_SLEEP							6



#define NETSESSION_RET_NORMAL							0x00000000		/*    Nothing happended in the library  */

#define NETSESSION_RET_LOGIN_OK_MASK					0x0F000000		
#define NETSESSION_RET_LOGIN_SUCCESS					0x01000000
#define NETSESSION_RET_GET_VIDEO						0x02000000
#define NETSESSION_RET_GET_AUDIO						0x03000000
#define NETSESSION_RET_PARTIAL_LOGIN_SUCCESS			0x04000000
#define NETSESSION_RET_TYPE_CHANGED						0x05000000
#define NETSESSION_RET_GET_ACK							0x06000000
#define NETSESSION_RET_GET_EVENT						0x07000000

#define NETSESSION_RET_GET_EVENT_START					0x08000000
#define NETSESSION_RET_GET_EVENT_STOP					0x09000000

#define NETSESSION_RET_GET_CONTROL						0x0A000000
// in this case the recvData member contains data as the next form.
#ifdef WIN32
#pragma warning(disable : 4200)
#endif
typedef struct SerialData{
#define FCC_SERIALDATA		'IRES'
	unsigned int dwControlType;
#define RS_232			'232P'
#define RS_485			'584P'
	unsigned int 	dwSerialType;		// P232 / P485
	unsigned int 	dwDataSize;
	unsigned int 	dwChannel;		// channel from which the data was
	unsigned char  	bSerialData[];
} SerialData, *pSerialData;
#ifdef WIN32
#pragma warning(default : 4200)
#endif

#define NETSESSION_RET_LOGIN_FAIL_MASK					0xF0000000
#define NETSESSION_RET_LOGIN_FAIL_MAX					0x10000000
#define NETSESSION_RET_LOGIN_FAIL_AUTH					0x20000000
#define NETSESSION_RET_CONNECT_ERROR					0x30000000
#define NETSESSION_RET_CONNECTION_TIMEOUT				0x40000000
#define NETSESSION_RET_NETWORK_ERROR					0x50000000

#define NETSESSION_RET_EVENT_MASK						0x00FF0000
#define NETSESSION_RET_EVENT_MOTION1					0x00010000
#define NETSESSION_RET_EVENT_MOTION2					0x00020000
#define NETSESSION_RET_EVENT_MOTION3					0x00040000
#define NETSESSION_RET_EVENT_MOTION4					0x00080000
#define NETSESSION_RET_EVENT_DINPUT						0x00100000
#define NETSESSION_RET_EVENT_DINPUT2					0x00400000

#define NETSESSION_RET_EVENT_VIDEOSIGNAL				0x00200000

#define NETSESSION_RET_VIDEO_CHANED_MASK				0x0000FF00
#define NETSESSION_RET_VIDEO_ENCODING_CHANGED			0x00000100
#define NETSESSION_RET_VIDEO_FRAMEMODE_CHANGED			0x00000200
#define NETSESSION_RET_VIDEO_VBR_CHANGED				0x00000400
#define NETSESSION_RET_VIDEO_BITRATE_CHANGED			0x00000800
#define NETSESSION_RET_VIDEO_WIDTH_CHANGED				0x00001000
#define NETSESSION_RET_VIDEO_HEIGHT_CHANGED				0x00002000
#define NETSESSION_RET_VIDEO_GOPSIZE_CHANGED			0x00004000
#define NETSESSION_RET_VIDEO_FPS_CHANGED				0x00008000

/*  The following flags are duplicated as videos'. */
#define NETSESSION_RET_AUDIO_CHANED_MASK				0x0000FF00
#define NETSESSION_RET_AUDIO_ENCODING_CHANGED			0x00000100
#define NETSESSION_RET_AUDIO_CHANNEL_CHANGED			0x00000200
#define NETSESSION_RET_AUDIO_BPS_CHANGED				0x00000400
#define NETSESSION_RET_AUDIO_SAMPLERATE_CHANGED			0x00000800

//return value for sendAudio
#define NETSESSION_RET_AU_INVALID_ARGUMENT				-1
#define NETSESSION_RET_AU_INVALID_SOCKET				-2
#define NETSESSION_RET_AU_AUDIO_NOT_AVAILABLE			-3
#define NETSESSION_RET_AU_PACKET_IS_TOO_LARGE			-4
#define NETSESSION_RET_AU_NETWORK_ERROR					-5

//return value for sendControl
#define NETSESSION_RET_SC_INVALID_ARGUMENT				-1
#define NETSESSION_RET_SC_INVALID_SOCKET				-2
#define NETSESSION_RET_SC_QUEUE_FULL					-3
#define NETSESSION_RET_SC_PACKET_IS_TOO_LARGE			-4
#define NETSESSION_RET_SC_NETWORK_ERROR					-5

#define NETSESSION_LOGIN_TYPE_VIDEO						0x01
#define NETSESSION_LOGIN_TYPE_AUDIO						0x02
#define NETSESSION_LOGIN_TYPE_AUDIO_PLAYBACK			0x04
#define NETSESSION_LOGIN_TYPE_CONFIG					0x08
#define NETSESSION_LOGIN_TYPE_EVENT						0x10
#define NETSESSION_LOGIN_TYPE_EVENT_VIDEO				0x20

#ifndef ARM4
#pragma pack(1)
#endif

struct ProtoVer
{
	unsigned char version;
	unsigned short id;
	unsigned char type;
};

/*----------------------------------------------------------------------------*/
struct NSVideoInfo {
#define FCC_MP4S		'S4PM'
#define FCC_DX50		'05XD'
#define FCC_MJPG		'GPJM'
#define FCC_MPG2		'2GPM'
#define FCC_MPG1		'1GPM'
#define FCC_H263		'362H'
#define FCC_H264		'462H'
	unsigned int 	encodingType;		// video compressor type.
#define VIFM_IONLY		0x01
#define VIFM_IPFRAME	0x02
#define VIFM_IPB		0x03
	unsigned char  	frameMode;			// how to encode video stream.
	unsigned int 	fps;				// 1 ~ 30
	unsigned char  	vbr;				// if 0, use the bitrate member for const bitrate else over zero, various bitrate.
// Bitrate setting -- a unit : 1000 Kbps
#define BITRATE_4M			4000
#define BITRATE_3M			3000
#define BITRATE_2M			2000
#define BITRATE_1_5M		1500
#define BITRATE_1M			1000
#define BITRATE_750K		750
#define BITRATE_500K		500
#define BITRATE_384K		384
#define BITRATE_256K		256
#define BITRATE_128K		128
#define BITRATE_64K			64
#define BITRATE_32K			32
	unsigned int 	bitrate;			// bitrate in kilo bit per second
	unsigned short 	videoWidth;			// frame width
	unsigned short 	videoHeight;		// frame height
#define VIVS_MOTION			0x06
#define VIVS_DIN			0x10
#define	VIVS_VSIGNAL		0x40
	unsigned char 	videoStatus1;		// video signal
#define VIVS_DIN1			0x01
#define VIVS_DIN2			0x02
	unsigned char 	videoStatus2;		//for Digital Input event
#define VIVS_MOTION_AREA1	0x01
#define VIVS_MOTION_AREA2	0x02
#define VIVS_MOTION_AREA3	0x04
	unsigned char 	videoStatus3;
	unsigned char 	videoStatus4;
#define	VIVT_NTSC			0x00
#define VIVT_PAL			0x01
#define VIVT_SECAM			0x02
	unsigned char	videoType;			// video output type
	unsigned char	groupSize;			// group of picture size
#ifndef ARM4
};
#else
} __attribute__ ((packed));
#endif
/*----------------------------------------------------------------------------*/
#define FCC_MP3				'_3PM'
struct NSAudioInfo {
#define FCC_IMAACPCM		'MCPI'
#define FCC_MSADPCM			'MCPM'
#define FCC_PCM				'MCPN'
	unsigned int 	encodingType;		// "IPCM" IMA ACPCM, "MPCM" MS ADPCM, "NPCM" PCM
#define AUDIO_MONO			0x01
#define AUDIO_STEREO		0x02
	unsigned char  	channel;			// Stereo(0x02), Mono(0x01)
	unsigned char  	bitPerSample;		// 8 bit, 16 bit
	unsigned int 	samplingRate;		// 8000, 11025, 16000, 22050, 44100, 48000
#ifndef ARM4
};
#else
} __attribute__ ((packed));
#endif
/*----------------------------------------------------------------------------*/
struct NSServerInfo {
	unsigned char 		cameraModel[4];
	unsigned char		reserved;
	unsigned char  		videoCount;
	unsigned char  		audioCount;
	unsigned char  		serverName[32];
	unsigned char  		channelName[32];
	unsigned char  		channelNumber;

	struct NSVideoInfo	videoInfo;
	struct NSAudioInfo	audioInfo;
#ifndef ARM4
};
#else
} __attribute__ ((packed));
#endif

#ifndef ARM4
#pragma pack()
#endif

/*----------------------------------------------------------------------------*/
typedef struct CNetSession {
	int					status;

	int					fps;

	unsigned int 		sequence;
	unsigned int 		sec;
	unsigned int 		usec;
	unsigned char  		frameType;

	unsigned int		dataSize;
	char				*recvData;

	char				*winPosition;

	int					isVideo;
	int					isAudio;
	int					isPlayBackAudio;
	int					isConfig;
	
	int					isEvent;
	int					isEventVideo;
	
	struct NSServerInfo	serverInfo;

	int					(*close)(void *_this);
	int					(*release)(void *_this);
	int					(*sendAudio)(void *_this, char *_buf, unsigned int _size);

	// See below for details...
	int					(*sendControl)(void *_this, unsigned int _type, char *_buf, unsigned int _size);
	int					(*doStateMachine)(void *_this);

	// New functions
	int					(*sendAudio2)(void *_this, char *_buf, unsigned int _size, unsigned int _audioType);
} CNetSession, *pCNetSession;

/**************************************************************************************
						_type values for sendControl
***************************************************************************************/

/* This is for rebooting video server */
#define FCC_RESTARTSYSTEM	'SYSR'

#define FCC_STOPEVENTVIDEO	'NEVE'

	/* these are the _type */
#define FCC_PCONTROL		'LTCP'
/* See below for more information */
#define FCC_CH_BIT			'TIBV'
// _buf : 4-byte-double-word-type bitrate value.
#define FCC_CH_RESOL		'SERV'
// _buf : 4-byte-double-word-type resolution value.
// Requested resolution type
	#define	RESOL_MODE_D1			0x01	// PAL : 720x576,	NTSC : 720x480
	#define	RESOL_MODE_4CIF			0x02	// PAL : 704x576,	NTSC : 704x480
	#define	RESOL_MODE_VGA			0x03	// PAL : 640x480,	NTSC : 640x480
	#define	RESOL_MODE_CIF			0x04	// PAL : 352x288,	NTSC : 352x240
	#define	RESOL_MODE_QVGA			0x05	// PAL : 320x240,	NTSC : 320x240
	#define	RESOL_MODE_QCIF			0x06	// PAL : 176x144,	NTSC : 176x120
	#define	RESOL_MODE_QQVGA		0x07	// PAL : 160x112,	NTSC : 160x112
#define FCC_CH_FRAMEMODE	'MRFV'  
// _buf : 4-byte-double-word-type capture-frame-mode value.
//	#define VIFM_IONLY		0x01
//	#define VIFM_IPFRAME	0x02
//	#define VIFM_IPB		0x03
#define FCC_SERVERFPS		'SPFS'
// _buf : 4-byte-double-word-type server fps.
#define FCC_DIGITALOUT		'TUOD'
// _buf : 4-byte-double-word-type digital-out.
#define FCC_GET_FRAMEMODE	'MFGV'
// _buf : 4-byte-double-word-type get-frame-mode value.
//	#define VIFM_IONLY		0x01
//	#define VIFM_IPFRAME	0x02
//	#define VIFM_IPB		0x03
#define FCC_CH_VIDEO_GROUP	'SRGV'
// _buf : 4-byte-double-word-type group-size value.
// 1 ~ 30
#define FCC_CH_ENCODINGTYPE	'CNEV'
// _buf : 4-byte-double-FOURCC value.

#define FCC_WINPOSSET		'TESW'
	typedef struct _tagWINPOSSET
	{
		unsigned char	x1;	// x coordinator of channel #1
		unsigned char	y1;	// y coordinator of channel #1
		unsigned char	w1;	// width size of channel #1
		unsigned char	h1;	// height size of channel #1
		unsigned char	x2;	// x coordinator of channel #2
		unsigned char	y2;	// y coordinator of channel #2
		unsigned char	w2;	// width size of channel #2
		unsigned char	h2;	// height size of channel #2
		unsigned char	x3;	// x coordinator of channel #3
		unsigned char	y3;	// y coordinator of channel #3
		unsigned char	w3;	// width size of channel #3
		unsigned char	h3;	// height size of channel #3
		unsigned char	x4;	// x coordinator of channel #4
		unsigned char	y4;	// y coordinator of channel #4
		unsigned char	w4;	// width size of channel #4
		unsigned char	h4;	// height size of channel #4
		unsigned char	C1;	// do not use. reserved.
		unsigned char	C2;	// do not use. reserved.
		unsigned char	C3;	// do not use. reserved.
		unsigned char	C4;	// do not use. reserved.
		unsigned char	pip;	// picture-in-picture setting
	}WINPOSSET;

#define FCC_SIMPLEWINPOSSET		'TSWS'
	typedef struct _tagSIMPLEWINPOSSET
	{
	#define SIMWPOS_CH1					1		// ch #1 only
	#define SIMWPOS_CH2					2		// ch #2 only
	#define SIMWPOS_CH3					3		// ch #3 only
	#define SIMWPOS_CH4					4		// ch #4 only
	#define SIMWPOS_QUAD				5		// All channels at the same time.
	#define SIMWPOS_PIP					6		// if this is set, the PIPOption must be set too.
		unsigned char nType;		// refer to the upper values...
		struct
		{
			unsigned char nHostChannel;		// The host channel number that is shown in a full screen
			unsigned char nEmbChannel;		// The embeded channel number to the host channel.
	#define SIMWPOS_EMBPOS_TOPLEFT		1
	#define SIMWPOS_EMBPOS_TOPRIGHT		2
	#define SIMWPOS_EMBPOS_BOTTOMRIGHT	3
	#define SIMWPOS_EMBPOS_BOTTOMLEFT	4
			unsigned char nEmbLocation;		// The position of the embeded channel.
	#define SIMWPOS_EMBSCALE_HALF		1
	#define SIMWPOS_EMBSCALE_THIRD		2
	#define SIMWPOS_EMBSCALE_QUARTER	3
			unsigned char nEmbScale;			// How big is the embeded channel?
		}PIPOption;
	}SIMPLEWINPOSSET;


#define FCC_AVONOFF			'PYTS'
	#define STYP_VIDEO		0x00000001
	#define STYP_AUDIO		0x00000002
	#define STYP_SENDAUDIO	0x00000004
// _buf : 4-byte-double-word-type value.
//        0x01 : Video, 0x02 : Sampling Audio, 0x04 : Playback Audio.


/*
	When Control id is
	#define FCC_PCONTROL		'LTCP'

	The control ids are here...

	The Daemon Control IDs are used to control the Pan-Tilt, Zoom, Focus and so forth of the specified camera source.

	* grammar *
	pNetSession->sendControl(
		pNetSesson,				// pNetSession is a return value of the NETSESSION_Init() function. and this pointer must not point NULL.
		FCC_PCONTROL,			// this indicates the content to send is a daemon-control.
		_buf,					// pointer to a buffer that keeps control data.
		size					// the size of the _buf. refer to the control id for the size.
	);

*/

	typedef struct _tagCONTROLDATA
	{
		unsigned short channel;
		unsigned short controlID;
		union 
		{
			unsigned char brighness;
			unsigned char contrast;
			unsigned char saturation;
			unsigned char sharpness;
			unsigned char whitebalance;
			unsigned char blc;
			unsigned char zoom;
			unsigned char focus;
			unsigned char exposure;
			unsigned char iris;
			unsigned char pan;
			unsigned char tilt;
			unsigned char light;
			unsigned char preset_set;
			unsigned char preset_move;
			unsigned char preset_clear;
			char		  focus_cont;
			struct
			{
				char pan;
				char tilt;
				char zoom;
			}ptz_cont;
			unsigned char transparent_data[1024];
		}payload;
	}CONTROLDATA;


	#define PCTL_BRIGHTNESS				0x1001		// 
	#define PCTL_CONTRAST				0x1002		// 
	#define PCTL_SATURATION				0x1003		// 
	#define PCTL_SHARPNESS				0x1004		// 
	#define PCTL_WHITEBALANCE			0x1005		// 
	#define PCTL_BLC					0x1006		// backlight compensation
	#define PCTL_ZOOM					0x1007		// 
	#define PCTL_FOCUS					0x1008		// 
	#define PCTL_EXPOSURE				0x1009		// 
	#define PCTL_IRIS					0x100A		// 
	#define PCTL_PAN					0x100B		// 
	#define PCTL_TILT					0x100C		// 
	#define PCTL_LIGHT					0x1011		// light on/off
	#define PCTL_PRESET_SET				0x1020		// 
	#define PCTL_PRESET_MOVE			0x1021		// 
	#define PCTL_PRESET_CLEAR			0x1022		// 
	#define PCTL_FOCUS_CONT				0x2001		// 
	#define PCTL_PTZ_CONT				0x2002		// 
		
	#define PCTL_RESETDOME				0xFFFF
	#define PCTL_TRANSPARENT			0x7000		// Set the control-daemon to transparent mode...
	#define PCTL_RS232					0x7001





#ifndef _NVS_VERSION_STRUCTURE_
#define _NVS_VERSION_STRUCTURE_
// Call to get the version of NVSNetLib,
typedef struct NVSVersion {
	int major;
	int	minor;
	int release;
	int build;
} NVSVersion, *pNVSVersion;
#endif



/*----------------------------------------------------------------------------*/
NVSNETLIB_API void * NETSESSION_Init(
	char *_address, 
	unsigned short _port, 
	char *_url,
	char *_id,
	char *_passwd,
	unsigned char _connectType
	);

NVSNETLIB_API NVSVersion NETSESSION_GetVersion();
NVSNETLIB_API unsigned int NETSESSION_GetServerVer(CNetSession* _sess);
	

#endif

