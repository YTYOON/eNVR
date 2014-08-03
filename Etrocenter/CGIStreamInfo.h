
#pragma pack(push,1)
/*
struct NSVideoInfo {
	#define FCC_MP4S 'S4PM'
	#define FCC_DX50 '05XD'
	#define FCC_MJPG 'GPJM'
	#define FCC_MPG2 '2GPM'
	#define FCC_MPG1 '1GPM'
	#define FCC_H263 '362H'
	#define FCC_H264 '462H'
	unsigned int encodingType; // video compressor type.
	#define VIFM_IONLY 0x01
	#define VIFM_IPFRAME 0x02
	#define VIFM_IPB 0x03
	unsigned char frameMode; // how to encode video stream.
	unsigned int fps; // 1 ~ 30
	unsigned char vbr; // if 0, use the bitrate member for const bitrate else over zero, various bitrate.
	// Bitrate setting -- a unit : 1000 Kbps
	#define BITRATE_4M 4000
	#define BITRATE_3M 3000
	#define BITRATE_2M 2000
	#define BITRATE_1_5M 1500
	#define BITRATE_1M 1000
	#define BITRATE_750K 750
	#define BITRATE_500K 500
	#define BITRATE_384K 384
	#define BITRATE_256K 256
	#define BITRATE_128K 128
	#define BITRATE_64K 64
	#define BITRATE_32K 32
	unsigned int bitrate; // bitrate in kilo bit per second
	unsigned short videoWidth; // frame width
	unsigned short videoHeight; // frame height
	#define VIVS_MOTION 0x06
	#define VIVS_DIN 0x10
	#define VIVS_VSIGNAL 0x40
	unsigned char videoStatus1; // video signal
	#define VIVS_DIN1 0x01
	#define VIVS_DIN2 0x02
	unsigned char videoStatus2; //for Digital Input event
	#define VIVS_MOTION_AREA1 0x01
	#define VIVS_MOTION_AREA2 0x02
	#define VIVS_MOTION_AREA3 0x04
	unsigned char videoStatus3;
	unsigned char videoStatus4;
	#define VIVT_NTSC 0x00
	#define VIVT_PAL 0x01
	#define VIVT_SECAM 0x02
	unsigned char videoType; // video output type
	unsigned char groupSize; // group of picture size
};

struct NSAudioInfo {
	#define FCC_IMAACPCM 'MCPI'
	#define FCC_MSADPCM 'MCPM'
	#define FCC_PCM 'MCPN'
	unsigned int encodingType; // "IPCM" IMA ACPCM, "MPCM" MSADPCM, "NPCM" PCM
	#define AUDIO_MONO 0x01
	#define AUDIO_STEREO 0x02
	unsigned char channel; // Stereo(0x02), Mono(0x01)
	unsigned char bitPerSample; // 8 bit, 16 bit
	unsigned int samplingRate; // 8000, 11025, 16000, 22050, 44100, 48000
};
struct NSServerInfo {
	unsigned char cameraModel[4];
	unsigned char reserved;
	unsigned char videoCount;
	unsigned char audioCount;
	unsigned char serverName[32];
	unsigned char channelName[32];
	unsigned char channelNumber;
	struct NSVideoInfo videoInfo;
	struct NSAudioInfo audioInfo;
};
*/
typedef struct _VSM_Stream
{
	int id;
	int size;
	int status;
	int preserved;
	int fps;
	unsigned int sequence;
	unsigned int sec;
	unsigned int usec;
	unsigned char frameType;
	struct NSServerInfo serverInfo;
	unsigned int dataSize;
	char recvData;
}VSM_Stream, PVSM_Stream;
#pragma pack(pop)