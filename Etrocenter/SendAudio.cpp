#include "stdafx.h"
#include "SendAudio.h"
#include "mycurl.h"

CSendAudio::CSendAudio(void):m_pNode(NULL),m_bAudioStart(false)
{
}

CSendAudio::~CSendAudio(void)
{

}

bool  CSendAudio::bDetectWaveIn()
{
	int nDevices = 	waveInGetNumDevs();

	if(nDevices ==0)
		return false;
	else
		return true;
}

bool CSendAudio::bStartSendAudio(NODEITEM* pNode)
{
	if (m_pNode != NULL)
	{
		this->vStopSendAudio();
	}

	int nDevices = 	waveInGetNumDevs();

	m_pNode = pNode;

	USES_CONVERSION;
		
	// check the machine is new or old
	Curlplus CGIScanData;
	CString csOldUrl, csOldBuffer, csNewUrl, csNewBuffer,csUser,csPass;

	csUser.Format(_T("%s"),A2W(m_pNode->szUser));
	csPass.Format(_T("%s"),A2W(m_pNode->szPass));

	csOldUrl.Format(_T("http://%s:%d/config/stream_info.cgi"), A2W(m_pNode->ip), m_pNode->nWebPort);	
	csOldBuffer = CGIScanData.get(csOldUrl,csUser, csPass);

	
	csNewUrl.Format(_T("http://%s:%d/config/stream.cgi"), A2W(m_pNode->ip), m_pNode->nWebPort);
	csNewBuffer = CGIScanData.get(csNewUrl,csUser, csPass);

	if(csOldBuffer.Find(_T("vprofilenum")) != -1 && csNewBuffer.Find(_T("rtsp_port")) == -1)
	{

		HRESULT hrEncode=  IPSSEND_SetEncodeType(IPS_ENCODETYPE_ADPCM);
		if( FAILED(hrEncode))
		{
			AfxMessageBox(_T("Set Encode error"));
			vStopSendAudio();
			return false;
		}
	}
		
	else if(csOldBuffer.Find(_T("vprofilenum")) == -1 && csNewBuffer.Find(_T("rtsp_port")) != -1)
	{

		HRESULT hrEncode=  IPSSEND_SetEncodeType(IPS_ENCODETYPE_PCM_MULAW);
		if( FAILED(hrEncode))
		{
			AfxMessageBox(_T("Set Encode error"));
			vStopSendAudio();
			return false;
		}
	}
	

	// IPS_ENCODETYPE_ADPCM// for old machine
	//IPS_ENCODETYPE_PCM_MULAW// for new machine

	HRESULT hrInit = IPSSEND_Inititalize( NULL );
	if( FAILED(hrInit) )
	{
		AfxMessageBox(_T("Inititalize error!"));
		vStopSendAudio();
		return false;
	}

	/*char szHostName[32];
	USHORT nPort;
	char szUserID[32];
	char szUserPass[32];
	char szRelayAlias[32]={""};

	strcpy(szHostName,m_pNode->ip);
	strcpy(szUserID,m_pNode->szUser);
	strcpy(szUserPass,m_pNode->szPass);*/


	HRESULT hrConnect =  IPSSEND_Connect( m_pNode->ip, 1852/*nPort*/,m_pNode->szUser , m_pNode->szPass, NULL );
	if( FAILED(hrConnect) )
	{
		AfxMessageBox(_T("Connect error"));
		vStopSendAudio();
		return false; 
	}
 
	
	
	HRESULT hrStart =  IPSSEND_Start();
	
	return true;


}
void CSendAudio::vStopSendAudio()
{
		
	if(IPSSEND_IsRunning())
	{
		HRESULT hrStop =  IPSSEND_Stop();

		if( FAILED(hrStop) )
		{
			AfxMessageBox(_T("Stop error"));
		}

		HRESULT hrClose =  IPSSEND_Close();
		if( FAILED(hrClose) )
		{
			AfxMessageBox(_T("Close error"));
		}
	}
	else
	{
		HRESULT hrClose =  IPSSEND_Close();
	}

		m_pNode = NULL;
}