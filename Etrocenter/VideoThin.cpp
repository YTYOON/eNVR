// VideoThin.cpp : implementation file
//

#include "stdafx.h"
//#include "EtroExplorer.h"
#include "VideoThin.h"
#include "MainFrm.h"
// CVideoThin

//IMPLEMENT_DYNAMIC(CVideoThin, CVideo)
#define nOSDHeight 26
CVideoThin::CVideoThin():m_hStreamLive(NULL), m_nConxIdx(0), m_hNextChannel(NULL),
m_hPatrolThread(NULL),m_hAutoScanThread(NULL),m_bStopPlay(0),m_bStopRecv(0)
{
	::ZeroMemory(&m_conxInfoEx, sizeof(CONNECTINFOEX));
    if (m_hNextChannel == NULL)
    {
        CSA sa;
        m_hNextChannel = CreateEvent(&sa.m_sa, false, false, NULL);
    }

	m_nptzAction=0;
}

CVideoThin::~CVideoThin()
{
    CLOSEHANDLE(m_hStreamLive);
    CLOSEHANDLE(m_hNextChannel);
    for (int ci =0; ci < m_vtconxInfoEx.size(); ci++)
    {
        delete m_vtconxInfoEx[ci];
    }
    m_vtconxInfoEx.clear();
}


BEGIN_MESSAGE_MAP(CVideoThin, CVideo)
	//{{AFX_MSG_MAP(CVideoThin)
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

void CVideoThin::GetStreamPort(DWORD& nPort, NODEITEM* pNode)
{
	if (pNode->nProtocol == RTPOVERHTTP)
		nPort = pNode->nWebPort;
	else
		nPort = pNode->nAPPort;
}

void CVideoThin::GetStreamUrl(TCHAR *tzPath, NODEITEM* pNode)
{
	wmemset(tzPath, 0x00, 62);
	_tcscpy(tzPath, pNode->stream_url);
}

void CVideoThin::vAttach(vector <NODEITEM*>* pvtpNode, BYTE connectType)
{
    DWORD nSize = pvtpNode->size();
    if (nSize > 0)
    {   
		SetStopPalay(false);
		SetStopRecv(false);
        for (int ci =0; ci < nSize; ci++)
        {
            m_vtpNode.push_back((*pvtpNode)[ci]);
            NODEITEM* pNode = m_vtpNode[ci];
            CONNECTINFOEX& conxInfoEx = *(new CONNECTINFOEX());
            ::ZeroMemory(&conxInfoEx, sizeof(CONNECTINFOEX));
            strncpy(conxInfoEx.base.ip, pNode->ip, sizeof(pNode->ip));
	        memcpy(conxInfoEx.base.mac, pNode->mac, sizeof(pNode->mac));
	        conxInfoEx.nConnectionType = connectType;
			 GetStreamPort(conxInfoEx.base.nPort, pNode);
	        conxInfoEx.base.nProtocol = pNode->nProtocol;
	        conxInfoEx.nRecvType = RECV_DECODE;
	        conxInfoEx.base.nStreamID = pNode->nStreamID;
	        strncpy(conxInfoEx.base.szUser, pNode->szUser, sizeof(pNode->szUser));
	        strncpy(conxInfoEx.base.szPass, pNode->szPass, sizeof(pNode->szPass));
			GetStreamUrl(conxInfoEx.base.tzPath, pNode);
			conxInfoEx.base.nDeviceType = pNode-> byDeviceType;

            m_vtconxInfoEx.push_back(&conxInfoEx);
        }
        m_pconxInfoEx = m_vtconxInfoEx[0];
        m_nConxIdx = 0;
	    m_pNode = m_vtpNode[0];
	    m_bFlip = m_vtpNode[0]->bFlip;
	    m_bUpsideDown = m_vtpNode[0]->bUpsideDown;
	    m_bRecording = m_vtpNode[0]->bRecording;
		m_csCameraName = m_vtpNode[0]->camera_name;
		
#ifdef PANORAMA
        //Create a panorama display if the first node is a fisheye model              
        m_bPanorama = bFishEyeModal(m_pNode);
		
        if(m_bPanorama)
		{	
			m_pconxInfoEx->base.nColorDepth = CONNECTINFO::BGR24;//m_bPanorama ? CONNECTINFO::BGR24 : CONNECTINFO::BGR32;
	        vCreatePanoramaDisplay();
        }
#endif
	    if( m_hSendAudioStopEvent )ResetEvent(m_hSendAudioStopEvent);

	    if( !m_hAVReceiveThread )
	    {
		    bResetOSD(m_bDisplayOSD);
		    m_hVideoThread = (HANDLE)_beginthreadex(NULL, 0, VideoPlayThread, this, 0, &m_nThreadID );
		    if( m_hVideoThread == NULL ) 
		    {
			    ERRLOG(_T("Error Creating Audio/Video Receive Thread.."));
			    return;
		    }
	    }
    }
	return;
}
void CVideoThin::vAttach(NODEITEM* pNode, BYTE connectType)
{	
	if(pNode == NULL) return;
	SetStopRecv(false);
	SetStopRecv(false);
    m_pconxInfoEx = &m_conxInfoEx; 
	strncpy(m_conxInfoEx.base.ip, pNode->ip, sizeof(pNode->ip));
	memcpy(m_conxInfoEx.base.mac, pNode->mac, sizeof(pNode->mac));
	m_conxInfoEx.nConnectionType = connectType;
	GetStreamPort(m_conxInfoEx.base.nPort, pNode);
	m_conxInfoEx.base.nProtocol = pNode->nProtocol;
	m_conxInfoEx.nRecvType = RECV_DECODE;
	m_conxInfoEx.base.nStreamID = pNode->nStreamID;
	strncpy(m_conxInfoEx.base.szUser, pNode->szUser, sizeof(pNode->szUser));
	strncpy(m_conxInfoEx.base.szPass, pNode->szPass, sizeof(pNode->szPass));
	GetStreamUrl(m_conxInfoEx.base.tzPath, pNode);
	m_conxInfoEx.base.nDeviceType = pNode-> byDeviceType;

	m_pNode = pNode;
	m_bFlip = pNode->bFlip;
	m_bUpsideDown = pNode->bUpsideDown;
	m_bRecording = pNode->bRecording;
	m_csCameraName = pNode->camera_name;
#ifdef PANORAMA
    m_bPanorama = bFishEyeModal(m_pNode);
		
    if(m_bPanorama)
	{	
		m_pconxInfoEx->base.nColorDepth = CONNECTINFO::BGR24;//m_bPanorama ? CONNECTINFO::BGR24 : CONNECTINFO::BGR32;
        vCreatePanoramaDisplay();
    }
#endif
	if( m_hSendAudioStopEvent )ResetEvent(m_hSendAudioStopEvent);
	
	if( !m_hAVReceiveThread )
	{
		bResetOSD(m_bDisplayOSD);
		m_hVideoThread = (HANDLE)_beginthreadex(NULL, 0, VideoPlayThread, this, 0, &m_nThreadID );
		if( m_hVideoThread == NULL ) 
		{
			ERRLOG(_T("Error Creating Audio/Video Receive Thread.."));
			return;
		}
	}
	SwitchRecLight();

	return;
}

#pragma optimize("", off)
#pragma optimize("", on) 
LONG CVideoThin::VideoPlayThreadRunPack()
{
    CSA sa;
    bool bNextChannel = false;
    HANDLE hStreamLive = CreateEvent(&sa.m_sa, TRUE, FALSE, STREAM_SERVER_PROCESS_LIVE);
    HANDLE hPreWait[]={m_hStopEvent, hStreamLive};
    DWORD nSize = sizeof(hPreWait)/sizeof(HANDLE);
    DWORD nWaitRet = ::WaitForMultipleObjects(nSize, hPreWait, false, INFINITE);
    CLOSEHANDLE(hStreamLive);
    switch( nWaitRet )
    {
    case WAIT_OBJECT_0:
        return -1;
        break;
    case WAIT_FAILED:
        ERRLOG(_T("VideoPlayThreadRunPack WaitForMultipleObjects fail\r\nError Code:%d"), ::GetLastError());
        return -1;
        break;
    } 
    m_pconxInfoEx->nThreadID = m_nThreadID;
    m_pconxInfoEx->nProcessID = GetCurrentProcessId();// OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());//::GetCurrentProcess();
    m_pconxInfoEx->nStatus &=  0xFFFFFFFE;
	if (false == bConnectStreamingSvr(m_pconxInfoEx)) return false;
    CDC* pdc = GetDC();
    if (!pdc) 
    {
        return 0;
    }
	CString csDecodEvent;
	csDecodEvent.Format(DECODE_EVENT_NAME,m_pNode->mac[0],m_pNode->mac[1],m_pNode->mac[2],m_pNode->mac[3],m_pNode->mac[4],m_pNode->mac[5], m_pconxInfoEx->base.nPort, m_pNode->nStreamID);
	m_hDecodEvent = CreateEvent(&sa.m_sa, false, false, csDecodEvent);
    HANDLE handles[] = { m_hDecodEvent, m_hStopEvent, m_hNextChannel};
    int nWaitSize = sizeof(handles)/sizeof(HANDLE);
    while (nWaitRet = ::WaitForMultipleObjects(nWaitSize, handles, false, 30000))
	{
		if ( nWaitRet == (WAIT_OBJECT_0+1) || GetStopRecv())
			break;
		if (m_nMsgIdx == MSG_LOGO)
		{
			m_nMsgIdx = MSG_DISCONNECTION;
            CRect rectTmp;
            GetWindowRect(&rectTmp);
	        GetParent()->ScreenToClient(&rectTmp);
	        GetParent()->InvalidateRect(&rectTmp, true);	
			DrawText(pdc->m_hDC);
			break;
		}
        if ( nWaitRet == (WAIT_OBJECT_0+2))
        {
            bNextChannel = true;
            ::ResetEvent(m_hNextChannel);
            break;
        }
        if ( nWaitRet == STATUS_TIMEOUT )
        {
			DrawText(pdc->m_hDC);
			break;
        }       
	}
	
	if (nWaitRet == WAIT_OBJECT_0)
	{
		CString csMappingName;
		csMappingName.Format(MAPPING_FILE_NAME, m_pNode->mac[0], m_pNode->mac[1], m_pNode->mac[2], m_pNode->mac[3], m_pNode->mac[4], m_pNode->mac[5], m_pconxInfoEx->base.nPort,  m_pNode->nStreamID);
		HANDLE hMappingFile(NULL);
		hMappingFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, csMappingName);

		bool bReconnect = false;  
		if ((NULL != hMappingFile) && (GetStopRecv() != true) && (bReconnect != true))
		{
			BITMAPINFOHEADER* pbih;
            m_pStreamStatus = (DECODE_FILE_HEADER*)::MapViewOfFile(hMappingFile, FILE_MAP_WRITE, 0, 0, 0);
            pbih = (BITMAPINFOHEADER*)((char*)m_pStreamStatus + sizeof(DECODE_FILE_HEADER));
			memcpy(&m_bih, pbih, sizeof(BITMAPINFOHEADER));
			DWORD nSize = WIDTHBYTES( m_bih.biWidth * m_bih.biBitCount) * abs(m_bih.biHeight);
			m_rcImg.left = m_rcImg.top = 0;
			m_rcImg.right = m_bih.biWidth;
			m_rcImg.bottom = m_bih.biHeight*-1;
			m_picture_buf = (uint8_t*)pbih +sizeof(BITMAPINFOHEADER);
            bResetDrawRect();

			///////////Larry add 103-01-22///////////
			if (m_hVideoThread)
			{
				if (m_pNode->nPTZAutorunType == 1 && m_pNode->bSupportPTZ == 0)
				{
					if (m_mapPatrolSeqDurSets.size() == 0)
					{
						CGS::bGetPatrolSeqDuration(m_mapPatrolSeqDurSets,m_pNode->mac);
					}
					vbeginPatrolThread();
				}
				else if (m_pNode->nPTZAutorunType == 2 && m_pNode->bSupportPTZ == 0)
				{
					vbeginAutoScanThread(CDlgPTZ::P_R, m_pNode->nPTZSpeed);
				}
			}

            if (m_pszGreyDib)
            {
                DELETEARRAY(m_pszGreyDib);
            }
#ifdef PANORAMA
			if(m_bPanorama && m_pPanorama)// first time or the resolution of video changes, change camera width and height and setlicense
			{
				if(nPreWidth !=  abs(m_bih.biWidth) || nPreHeight != abs(m_bih.biHeight))
				{
					nPreWidth = abs(m_bih.biWidth); 
					nPreHeight = abs(m_bih.biHeight);
					m_pPanorama->ChangeCamera(abs(m_bih.biWidth),abs(m_bih.biHeight));	
					char chMac[18] ={};
					char szIp[16] ={};
					DWORD nWebPort = m_pNode->nWebPort;
					strcpy(szIp, m_pNode->ip); 
					if (m_pNode->byDeviceType == IPCAMERA || m_pNode->byDeviceType == ONVIF)
					{
						sprintf(chMac, "%02X:%02X:%02X:%02X:%02X:%02X", m_pNode->mac[0], m_pNode->mac[1], m_pNode->mac[2], m_pNode->mac[3], m_pNode->mac[4], m_pNode->mac[5]);
					}else
					{
						//Lynn modified for surveillance show at 2013/10/25
						sprintf(chMac, "%02X:%02X:%02X:%02X:%02X:%02X",0x00,0x19,0x6c,0xff,0x00,0x01);
						if (m_pNode->byDeviceType == IPVSTATION || m_pNode->byDeviceType == eNVR)
						{
							nWebPort = 8080;
							HKEY hkFolder; 
							DWORD cbData = sizeof(DWORD);
							DWORD nType = REG_DWORD;
							LONG nRet1 = RegOpenKey(HKEY_LOCAL_MACHINE, REGISTSUBFOLDER, &hkFolder);	
							LONG nRet4 = RegQueryValueEx(hkFolder, _T("HTTPPORT"), NULL, &nType, (LPBYTE)&nWebPort, &cbData);
							if (nWebPort == 0)
							{
								nWebPort=8080;
							}
							strcpy(szIp, "127.0.0.1");
						}
					}
					bool bRet = m_pPanorama->SetLicenseCode(chMac, "", szIp, nWebPort, m_pNode->szUser, m_pNode->szPass);
                     m_pPanorama->SetMirror(true);
					if(m_pOSD && m_bDisplayOSD)
					{
						m_pOSD->Invalidate();//invalidate or OSD will disappear when panorama display 
					}
				}

				int nWidth(0), nHeight(0);
				m_pPanoramaBuff = m_pPanorama->GetDisplaySourceFrameBuffer (&nWidth, &nHeight);

			}
#endif
			while ( !GetStopRecv() && !bReconnect && !bNextChannel)
			{
               nWaitRet = ::WaitForMultipleObjects(nWaitSize, handles, false, 10000);
				switch (nWaitRet)
				{
				    case WAIT_OBJECT_0:
						{
							m_bReCalcRect ?	bResetDrawRect() : NULL;
							m_nMsgIdx = MSG_NORMAL;
							++m_nFPS;
							SwitchRecLight();
							DrawPicture2(pdc->m_hDC, m_picture_buf);
						}
						break;
                    case WAIT_OBJECT_0+1:
						{
							//SwitchRecLight(true);
						}	
						break;
                    case WAIT_OBJECT_0+2:
						{
							::ResetEvent(m_hNextChannel);
							bNextChannel = true;
						}
						break;
                    case WAIT_FAILED:
						{
							SwitchRecLight(true);
							bReconnect = true;
						}
						break;
				    case WAIT_TIMEOUT:
				    default:
                      if ((m_pconxInfoEx->nStatus & CONNECTINFOEX::PAUSE) != CONNECTINFOEX::PAUSE)
					    {
							USES_CONVERSION;
							ETROLOG(_T("Wait for Render Event Time out\t ip %s"), A2W(this->m_pNode->ip));
							bReconnect = true;
							GreyPicture(&m_bih, m_picture_buf);
							m_nMsgIdx = MSG_RECONNECTION;
							DrawText(pdc->m_hDC);
							SwitchRecLight(true);
					    }
					    break;
				}
			}
	        m_picture_buf = NULL;
	        UnmapViewOfFile(pbih);
		}
		CLOSEHANDLE(hMappingFile);	
    }

	m_pStreamStatus = NULL;
	CLOSEHANDLE(m_hDecodEvent);
    if (bNextChannel)
	{
	    vShiftNextChannel();
    }
	
    if (GetStopRecv())
    {
        bDisConnectStreamingSvr(m_pconxInfoEx);
    }
	ReleaseDC(pdc);
	
	return 0;
}


bool CVideoThin::bConnectStreamingSvr(CONNECTINFOEX* pconxInfoEx)
{
    HANDLE hpipe(INVALID_HANDLE_VALUE);
    WORD nRetryTime = 100;
    TCHAR sz[256] = {0};
	lstrcat(sz, TEXT("\\\\.\\pipe\\STREAMING_SERVER_CONNECTION"));
    CSA sa;
	while (--nRetryTime && (hpipe == INVALID_HANDLE_VALUE))
	{
        hpipe = CreateFile(sz, GENERIC_WRITE , 0, &sa.m_sa, OPEN_EXISTING , 0, NULL);
		if (hpipe != INVALID_HANDLE_VALUE) 
		{
			DWORD cbRead = 0;
			DWORD dwNumBytes(0);
			WriteFile(hpipe, pconxInfoEx, sizeof(CONNECTINFOEX), &dwNumBytes, NULL);
			FlushFileBuffers(hpipe);
		    CloseHandle(hpipe);
			break;
		}else
		{
            ETROLOG(_T("CreateFile STREAMING_SERVER_CONNECTION FAIL ErrorCode: %d\r\n"),::GetLastError());
            if (WaitNamedPipe(sz, 20) != 0)
            {
                continue;
            }
			//::Sleep(100);
		}
		
		if (GetStopRecv())
		{
			return false;
		}	
    }
    if (nRetryTime == 0)
    {
        ERRLOG(_T("CreateFile STREAMING_SERVER_CONNECTION Fail"));
        return false;
    }
    return true;
}
bool CVideoThin::bDisConnectStreamingSvr(CONNECTINFOEX* pconxInfoEx)
{
    TCHAR sz[256] = {0};
    _tcscpy(sz, TEXT("\\\\.\\pipe\\STREAMING_SERVER_DISCONNECTION"));
    HANDLE hpipe(INVALID_HANDLE_VALUE);
    CSA sa;
    int nRetryTime = 100;
    while (--nRetryTime && (hpipe == INVALID_HANDLE_VALUE))
    {
        hpipe = CreateFile(sz, GENERIC_WRITE , 0, NULL, OPEN_EXISTING, 0, &sa.m_sa);
	    if (hpipe != INVALID_HANDLE_VALUE) 
	    {
		    DWORD cbRead = 0;
		    DWORD dwNumBytes(0);
		    WriteFile(hpipe, pconxInfoEx, sizeof(CONNECTINFOEX), &dwNumBytes, NULL);
		    FlushFileBuffers(hpipe);
		    CloseHandle(hpipe);
		    break;
	    }else
	    {
		    if (ERROR_FILE_NOT_FOUND == ::GetLastError())
		    {
			    break;
		    }
	    }
	    ::Sleep(10);
    }
    ASSERT(nRetryTime);
    return nRetryTime;
}
void CVideoThin::vShiftNextChannel()//int nShiftID)
{
    int nPresentIdx = m_nConxIdx;
   
    m_pconxInfoEx->nStatus |= CONNECTINFOEX::PAUSE;
    bConnectStreamingSvr(m_pconxInfoEx);

    m_pconxInfoEx = m_vtconxInfoEx[nPresentIdx];
    m_pNode = m_vtpNode[nPresentIdx];
    if(m_pOSD->m_pCaptionOSD)
    {
	   m_pOSD->m_pCaptionOSD->SetWindowTextW(m_pNode->wcAlias);
       m_pOSD->m_pCaptionOSD->UpdateWindow();
    }
	//if current node is not a fisheye model, delete panorama
	m_bPanorama = bFishEyeModal(m_pNode);
	//m_pconxInfoEx->base.nColorDepth = CONNECTINFO::BGR24;//m_bPanorama ? CONNECTINFO::BGR24 : CONNECTINFO::BGR32;
	m_bPanorama ? m_pconxInfoEx->base.nColorDepth = CONNECTINFO::BGR24 : m_pconxInfoEx->base.nColorDepth = CONNECTINFO::BGR32;

	if( m_bPanorama )
	{		
		
		if(m_pPanorama)
		{
			CRect rectPano;
			rectPano = m_rectVideo;
			rectPano.top += nOSDHeight;       
			m_pPanorama->MoveWindow(rectPano.left,rectPano.top, rectPano.Width(), rectPano.Height());
		}
		else
		{
			vCreatePanoramaDisplay();	
		}
	}
	else
	{	
		if(m_pPanorama)
		{
			m_pPanorama->MoveWindow(0,0);
		}
	}	
}
void CVideoThin::vSetRecvAudio()
{
    m_bSetAudio ^= 1;
    if (m_bSetAudio)
    {
		if(m_bAudioStart)
		{
			m_bSetAudio = false;
			return;
		}
        if (m_pVideoAudio)
        {
            m_pVideoAudio->vSetRecvAudio();
        }
        m_pVideoAudio = this;
		if (m_pOSD && m_pOSD->m_pRecvAudio)
		{
			m_pOSD->m_pRecvAudio->SetBitmaps(IDB_BITMAP_AUDIO_OFF_H,GLOBALBKCOLOR);
			m_pOSD->m_pRecvAudio->Invalidate(false);
			m_pOSD->m_pRecvAudio->UpdateWindow();
		}
    }else
    {
        m_pVideoAudio = NULL;
        if (m_pOSD && m_pOSD->m_pRecvAudio)
		{
			m_pOSD->m_pRecvAudio->SetBitmaps(IDB_BITMAP_AUDIO_OFF,GLOBALBKCOLOR);
			m_pOSD->m_pRecvAudio->Invalidate(false);
			m_pOSD->m_pRecvAudio->UpdateWindow();
		}
    }
    if (m_bSetAudio)
    {
        m_pconxInfoEx->nStatus = m_pconxInfoEx->nStatus | CONNECTINFOEX::AUDIO_ON;
    }else
    {
         m_pconxInfoEx->nStatus = m_pconxInfoEx->nStatus & 0xFFFFFFFD;
    }
    bConnectStreamingSvr(m_pconxInfoEx);
}
void CVideoThin::vSetRecVideo()
{
	int a=0;
	/*m_bSetAudio ^= 1;
	if (m_bSetAudio)
	{
		if(m_bAudioStart)
		{
			m_bSetAudio = false;
			return;
		}
		if (m_pVideoAudio)
		{
			m_pVideoAudio->vSetRecvAudio();
		}
		m_pVideoAudio = this;
		if (m_pOSD->m_pRecvAudio)
		{
			m_pOSD->m_pRecvAudio->SetBitmaps(IDB_BITMAP_AUDIO_OFF_H,GLOBALBKCOLOR);
			m_pOSD->m_pRecvAudio->Invalidate(false);
			m_pOSD->m_pRecvAudio->UpdateWindow();
		}
	}else
	{
		m_pVideoAudio = NULL;
		if (m_pOSD->m_pRecvAudio)
		{
			m_pOSD->m_pRecvAudio->SetBitmaps(IDB_BITMAP_AUDIO_OFF,GLOBALBKCOLOR);
			m_pOSD->m_pRecvAudio->Invalidate(false);
			m_pOSD->m_pRecvAudio->UpdateWindow();
		}
	}
	if (m_bSetAudio)
	{
		m_pconxInfoEx->nStatus = m_pconxInfoEx->nStatus | CONNECTINFOEX::AUDIO_ON;
	}else
	{
		m_pconxInfoEx->nStatus = m_pconxInfoEx->nStatus & 0xFFFFFFFD;
	}
	bConnectStreamingSvr(m_pconxInfoEx);*/
}

void CVideoThin::vSetStatus(bool bStatus)
{
	if(bStatus)
	{
		 m_pconxInfoEx->nStatus |= CONNECTINFOEX::PAUSE;
	}
	else
	{ 
		m_pconxInfoEx->nStatus &= 0xFFFFFFFE;
	}
	bConnectStreamingSvr(m_pconxInfoEx);
}

UINT CVideoThin::thdPatrol(LPVOID lpParam)
{
	CVideoThin* pThis = (CVideoThin*) lpParam;	
	
	while(pThis->GetStopPalay() == false)		//pThis->m_pNode->nPTZAutorunType == 1//
	{
		BOOL bExit=0;
		if(pThis->m_mapPatrolSeqDurSets.size()>0)	 //pThis->m_nPatrolSeqDurIdx <pThis->m_vctmapPatrolSeqDurSets.size()
		{
			map <int,pair_INT>::iterator it;

			for(it =  pThis->m_mapPatrolSeqDurSets.begin(); it != pThis->m_mapPatrolSeqDurSets.end();it++)
			{
				if (pThis->GetStopPalay())
				{
					bExit=1;
					break;
				}

				if(pThis->m_nMsgIdx == MSG_NORMAL)
				{
					if(it->first !=0 && pThis->m_pNode->nPTZAutorunType == 1)
					{
						pThis->vGoToPresetPoint(it->second.first);
						if (pThis->GetStopPalay())
						{
							bExit=1;
							break;
						}
					}
					else
					{
						break;
					}
				}
				else
				{							
					if (pThis->GetStopPalay())
					{
						bExit=1;
					}
					break;
				}

				int nSleepCount=it->second.second*10;

				while(nSleepCount>0 && pThis->m_pNode && pThis->GetStopPalay() == false)
				{
					if (pThis->m_nMsgIdx != MSG_NORMAL )
					{
						if (pThis->GetStopPalay() == false)
						{
							if (pThis->m_pNode->nPTZAutorunType != 1 )
							{
								break;
							}
						}
						else
						{
							bExit=1;
							break;
						}
					}					
					::Sleep(100);
					nSleepCount--;
				}


				if (bExit)
				{
					break;
				}
			}
		}
		else
		{
			//pThis->m_bPatrolEnable = false;
			break;
		}
	}

	if (pThis->m_hVideoThread != NULL)
	{
		CLOSEHANDLE(pThis->m_pNode->hPatrolThread);
	}
	return 0;
}

void CVideoThin::vGoToPresetPoint(int nPreset)
{
	if(m_pNode == NULL)
		return ;

	if (m_nMsgIdx == MSG_NORMAL)	////enum {MSG_LOGO, MSG_DISCONNECTION, MSG_RECONNECTION, MSG_NORMAL};////
	{
		USES_CONVERSION;
		TCHAR tzIP[32];
		TCHAR tzUser[32];
		TCHAR tzPass[32];
		int nWebPort;
		wcscpy(tzIP,   A2W(m_pNode->ip));
		wcscpy(tzUser, A2W(m_pNode->szUser));
		wcscpy(tzPass, A2W(m_pNode->szPass));

		if(m_pNode->nWebPort == 0)
			nWebPort = 80;

		else nWebPort = m_pNode->nWebPort; 

		CString csIP, csUser, csPass, csCgiUrl;
		csIP.Format(_T("%s"),tzIP);
		csUser.Format(_T("%s"),tzUser);
		csPass.Format(_T("%s"),tzPass);

		/*---Set PTZ Preset via CGI---*/
		csCgiUrl.Format(_T("http://%s:%d/config/ptz_preset.cgi?name=%d&act=go"),csIP,nWebPort,nPreset);
		TRACE(csCgiUrl);
		mycurl.get(csCgiUrl, csUser, csPass);
	}
}

void CVideoThin::vbeginPatrolThread()
{
	CLOSEHANDLE(m_pNode->hAutoScanThread);

	UINT nRet(0);
	if (!m_pNode->hPatrolThread)
	{
		m_pNode->hPatrolThread = (HANDLE) _beginthreadex(NULL,0,thdPatrol,(LPVOID)this,0,&nRet);
	}	
}

void CVideoThin::vbeginAutoScanThread(int ptzAction, int nPTZSpeed)
{
	CLOSEHANDLE(m_pNode->hPatrolThread);

	UINT nRet(0);
	m_pNode->nPTZSpeed=nPTZSpeed;
	m_nptzAction=ptzAction;

	if (ptzAction == CDlgPTZ::PTZAction::STOP)
	{
		m_pNode->nPTZAutorunType=0;		
	}

	if (!m_pNode->hAutoScanThread)
	{
		m_pNode->hAutoScanThread = (HANDLE) _beginthreadex(NULL,0,thdAutoScan,(LPVOID)this,0,&nRet);
	}	
}

UINT CVideoThin::thdAutoScan(LPVOID lpParam)
{
	CVideoThin* pThis = (CVideoThin*) lpParam;
	int nSpeed(0);
	int nA=1;
	while(nA==1) //pThis->m_pNode->nPTZAutorunType == 2 && pThis->m_hVideoThread)
	{
		USES_CONVERSION;
		TCHAR tzIP[32];
		TCHAR tzUser[32];
		TCHAR tzPass[32];
		int nWebPort;
		wcscpy(tzIP,   A2W(pThis->m_pNode->ip));
		wcscpy(tzUser, A2W(pThis->m_pNode->szUser));
		wcscpy(tzPass, A2W(pThis->m_pNode->szPass));

		if(pThis->m_pNode->nWebPort == 0)
			nWebPort = 80;	
		else nWebPort = pThis->m_pNode->nWebPort; 

		CString csIP, csUser, csPass, csCgiUrl,csChannel;

		csIP.Format(_T("%s"),tzIP);
		csUser.Format(_T("%s"),tzUser);
		csPass.Format(_T("%s"),tzPass);
		int nPSpeed= 0, nTSpeed= 0, nZSpeed= 0;
		int nPTZSpeed=pThis->m_pNode->nPTZSpeed;
		bool bEtro = true;

		if (pThis->m_nptzAction == CDlgPTZ::PTZAction::STOP)
		{
			nPSpeed=nTSpeed=nZSpeed= 0;
		}

		if(pThis->m_pNode->byDeviceType == ISAP)
		{
			bEtro = false;
			csChannel.Format(_T("%d"), pThis->m_pNode->mac[5]);
		}

		if(bEtro)
		{
			nPSpeed += nPTZSpeed;

			/*---Set PTZ via CGI---*/			
			if (pThis->m_pNode->bSupportPTZ == 0 && pThis->m_nMsgIdx == MSG_NORMAL)
			{
				csCgiUrl.Format(_T("http://%s:%d/config/ptz_move_cont.cgi?p=%d&t=%d&z=%d"),csIP,nWebPort,nPSpeed,nTSpeed,nZSpeed);
				pThis->mycurl.get(csCgiUrl,csUser,csPass);
				TRACE(_T("%s\r\n"),csCgiUrl);
				TRACE(_T("PTZSpeed:%d\r\n"),nPTZSpeed);
			}
			else
				break;
		}
		else // isap 
		{
			CString csCmd(_T(""));
			nPTZSpeed -= 1 ; // Speed range 0~5;	

			/*----set P,T,Z ----*/
			csCmd.Format(_T("/ptzcmdMoveRight=%d,0"),nPTZSpeed);

			/*---Set PTZ via CGI---*/
			csCgiUrl.Format(_T("http://%s:%d/airvideo/ptz?channel=channel%s"),csIP,nWebPort,csChannel);
			CString csConn;
			csConn.Format(_T("/airvideo/ptz?channel=channel%s"),csChannel);
			TRACE(_T("PTZ URL:%s\r\n"),csCgiUrl);
			//mycurl.post(csCgiUrl,csUser,csPass,csConn);
			if (pThis->m_pNode->bSupportPTZ == 0 && pThis->m_nMsgIdx == MSG_NORMAL)
				pThis->mycurl.post(csCgiUrl,csUser,csPass,csCmd);		
			else
				break;
		}
		nA++;
	}	

	if (pThis->m_hVideoThread != NULL)
	{
		CLOSEHANDLE(pThis->m_pNode->hAutoScanThread);
	}
	return 0;
}

void CVideoThin::vFreePTZThread()
{
	SetStopRecv(true);
	CLOSEHANDLE(m_pNode->hPatrolThread);
	CLOSEHANDLE(m_pNode->hAutoScanThread);

	DWORD dwResult;
	if (m_pNode->hPatrolThread)
	{
		dwResult = ::WaitForSingleObject(m_pNode->hPatrolThread, 10000);
		if (dwResult == WAIT_TIMEOUT) 
		{
			::TerminateThread(m_pNode->hPatrolThread, -1);
		}
	}
}

void CVideoThin::ReconnectStream()
{
	if (m_pNode->nStreamID != m_pconxInfoEx->base.nStreamID ||
		m_pNode->nProtocol != m_pconxInfoEx->base.nProtocol)
	{
		vStopPlay();
		vStopRecv();
		vAttach(m_pNode);
	}
}

void CVideoThin::SwitchRecLight(bool bTurnoff)
{
	if (m_pOSD && m_pOSD->m_pRecVideo)
	{
		if (m_pNode->bRecording && 
			m_pStreamStatus &&
			m_pStreamStatus->btStatus[0]==1)
		{
			if (bTurnoff != true)
			{
				if (m_pOSD->m_hBmpRecStatus != m_pOSD->m_hBmpRecOn)
				{
					m_pOSD->m_pRecVideo->SetBitmap(m_pOSD->m_hBmpRecOn);
					m_pOSD->m_hBmpRecStatus = m_pOSD->m_hBmpRecOn;
				}
			}
			else
			{
				if (m_pOSD->m_hBmpRecStatus != m_pOSD->m_hBmpRecOff)
				{
					m_pOSD->m_pRecVideo->SetBitmap(m_pOSD->m_hBmpRecOff);
					m_pOSD->m_hBmpRecStatus = m_pOSD->m_hBmpRecOff;
				}
			}
		}
		else
		{
			if (m_pOSD->m_hBmpRecStatus != m_pOSD->m_hBmpRecOff)
			{
				m_pOSD->m_pRecVideo->SetBitmap(m_pOSD->m_hBmpRecOff);
				m_pOSD->m_hBmpRecStatus = m_pOSD->m_hBmpRecOff;
			}
		}
	}
}

void CVideoThin::SetStopPalay(BOOL bRes)
{
	m_bStopPlay = bRes;
}

void CVideoThin::SetStopRecv(BOOL bRes)
{
	m_bStopRecv = bRes;
}