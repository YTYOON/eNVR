#ifndef __IPSSENDAUDIO_H___
#define __IPSSENDAUDIO_H___

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C		extern "C"
#else
#define EXTERN_C		extern
#endif
#endif

#ifndef USE_IPSSENDAUDIO_LIB
	#ifdef IPSSENDAUDIO_EXPORTS
		#define IPSSENDAUDIO_API			EXTERN_C _declspec(dllexport)
	#else
		#define IPSSENDAUDIO_API			EXTERN_C _declspec(dllimport)
	#endif
#else
#define IPSSENDAUDIO_API			EXTERN_C
#endif

#define	 WM_IPSSENDAUDIO_NOTIFY			WM_USER+2005

typedef	 BOOL (*IPSSEND_CALLBACK_PROC)( DWORD msg, WPARAM wParam, LPARAM lParam );


IPSSENDAUDIO_API	DWORD IPSSEND_Inititalize( IPSSEND_CALLBACK_PROC lpfnCallback );
IPSSENDAUDIO_API	DWORD IPSSEND_Connect( LPCSTR szServerIP, USHORT nPort, LPCSTR szID, LPCSTR szPass, LPCSTR szRelayAlias );
IPSSENDAUDIO_API	DWORD IPSSEND_Start();
IPSSENDAUDIO_API	DWORD IPSSEND_Stop();
IPSSENDAUDIO_API	BOOL IPSSEND_IsRunning();
IPSSENDAUDIO_API	DWORD IPSSEND_Close( );
IPSSENDAUDIO_API DWORD IPSSEND_SetBufferingSize( DWORD dwcbByte );
IPSSENDAUDIO_API DWORD IPSSEND_GetBufferingSize( );

#define IPS_ENCODETYPE_MP3			0
#define IPS_ENCODETYPE_ADPCM		1
#define IPS_ENCODETYPE_PCM_MULAW	2
IPSSENDAUDIO_API DWORD IPSSEND_SetEncodeType(int nType);
IPSSENDAUDIO_API DWORD IPSSEND_GetEncodeType();


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

IPSSENDAUDIO_API	NVSVersion IPSSEND_GetVersion( );
IPSSENDAUDIO_API	NVSVersion IPSSEND_GetNVSNetLibVersion( );


#endif