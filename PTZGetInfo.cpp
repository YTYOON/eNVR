// PTZGetInfo.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "PTZGetInfo.h"

int nCgiPTZSpeed[7]={0, 1, 5, 10, 20, 40, 60};		//103-05-20 new ptz speed spec//
// CPTZGetInfo

IMPLEMENT_DYNCREATE(CPTZGetInfo, CWinThread)

CPTZGetInfo::CPTZGetInfo()
{
	m_bActiven=0;
	m_nCgiType=0;
	m_pNode=0;
	m_nPTZSpeed=0;

	m_nPSpeed=0;
	m_nTSpeed=0;
	m_nZSpeed=0;
	m_nPreset=0;
	m_bAutoPatrol=0;
	m_bAutoScan=0;
	m_bAutoFocus=0;

	m_bStopCGI=0;
	m_nPatrolNum=1;

	m_bFOCUSCONTSend=0;
	m_bRunJoyFocus=0;
	m_bRunJoyZoom=0;
}

CPTZGetInfo::~CPTZGetInfo()
{
}

BOOL CPTZGetInfo::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CPTZGetInfo::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPTZGetInfo, CWinThread)
END_MESSAGE_MAP()


// CPTZGetInfo message handlers

BOOL CPTZGetInfo::Init(CWnd* pWnd,UINT nMsgID)
{
	m_pWnd=pWnd;
	m_nMsgID=nMsgID;
	BOOL bOK=CreateThread();
	return PostThreadMessage(m_nMsgID,PTZGI_Init,bOK);
}

BOOL CPTZGetInfo::_Init()
{
	SetThreadPriority(THREAD_PRIORITY_HIGHEST);
	return true;
}

BOOL CPTZGetInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	HRESULT	hr;
	if (pMsg->message==m_nMsgID)
	{
		if (pMsg->wParam==PTZGI_Init)
		{
			hr=_Init();
			m_pWnd->PostMessage(m_nMsgID,PTZGI_Init,(LPARAM)hr);
		}
		else if (pMsg->wParam==PTZGI_vCGI)
		{
			SetTimerQueryCgi();
		}
		else if (pMsg->wParam==PTZGI_vCGI_START)
		{
			_vQueryCgi();
			m_pWnd->PostMessage(m_nMsgID,PTZGI_vCGI,m_nCgiType);
		}

	}

	return CWinThread::PreTranslateMessage(pMsg);
}


BOOL CPTZGetInfo::vQueryCgi(DWORD nCgiType, NODEITEM* pNode, int nPSpeed, int nTSpeed, int nZSpeed )
{
	if (pNode==NULL)
	{
		return false;
	}
	if (m_bActiven)
	{
		return false;
	}

	m_csCGIStatus.Empty();

	m_nCgiType=nCgiType;
	m_pNode=pNode;
	m_nPTZSpeed=nPSpeed;
	m_nPSpeed=nPSpeed;
	m_nTSpeed=nTSpeed;
	m_nZSpeed=nZSpeed;

	if (nCgiType==CGI_GO_PRESET || nCgiType==CGI_SET_PRESET)
	{
		m_nPreset=nPSpeed;
	}
	else if (nCgiType==CGI_AUTOPATROL)
	{
		m_bAutoPatrol=(BOOL)nPSpeed;
	}
	else if (nCgiType==CGI_AUTOSCAN)
	{
		m_bAutoScan=(BOOL)nTSpeed;
	}
	else if (nCgiType==CGI_SET_AUTOFOCUS)
	{
		m_bAutoFocus=(BOOL)nPSpeed;
	}
	else if (nCgiType==CGI_GET_PATROLNUM)
	{
		m_nPatrolNum=m_pNode->nPatrolNum;
	}
	
	m_bActiven=1;
	return PostThreadMessage(m_nMsgID,PTZGI_vCGI,0);
}

BOOL CPTZGetInfo::vQueryCgi(DWORD nCgiType, NODEITEM* pNode, PTZSettingInfo* ptzInfo)
{
	if (pNode==NULL)
	{
		return false;
	}
	if (m_bActiven)
	{
		return false;
	}

	m_csCGIStatus.Empty();
	m_csRS485Enable.Empty();
	m_csProtocol.Empty();
	m_csDeviceID.Empty();
	m_csBraudRate.Empty();
	m_csDataBits.Empty();
	m_csParity.Empty();
	m_csStopBits.Empty();
	m_csPTZSpeed.Empty();
	m_csComm.Empty();

	m_nCgiType=nCgiType;
	m_pNode=pNode;
	
	//memcpy(&m_ptzInfo,&ptzInfo,sizeof(PTZSettingInfo));

	if(ptzInfo->csProtocol == _T("Disable"))
	{
		m_csRS485Enable = _T("no");
		ptzInfo->csProtocol=_T("none");
	}
	else  
		m_csRS485Enable = _T("yes");		

	
	m_csProtocol=ptzInfo->csProtocol;
	m_csDeviceID=ptzInfo->csDeviceID;
	m_csBraudRate=ptzInfo->csBraudRate;
	m_csDataBits=ptzInfo->csDataBits;
	m_csParity=ptzInfo->csParity;
	m_csStopBits=ptzInfo->csStopBits;
	m_csPTZSpeed=ptzInfo->csPTZSpeed;
	m_csComm=ptzInfo->csComm;

	m_nPTZSpeed=_wtoi(m_csPTZSpeed);

	m_bActiven=1;
	return PostThreadMessage(m_nMsgID,PTZGI_vCGI,0);
}

BOOL CPTZGetInfo::vQueryCgi(DWORD nCgiType, NODEITEM* pNode, map <int,pair_INT> mapPatrolSeq)
{
	if (pNode==NULL)
	{
		return false;
	}
	if (m_bActiven)
	{
		return false;
	}

	m_nCgiType=nCgiType;
	m_pNode=pNode;

	if (nCgiType==CGI_SET_PATROLNUM)
	{
		m_csPresetPoint.Empty();
		if (mapPatrolSeq.size()>0)
		{
			map<int,pair_INT>::iterator it;
			for(it = mapPatrolSeq.begin();it!= mapPatrolSeq.end();it++) // constructing map into array
			{
				if (!m_csPresetPoint.IsEmpty())
				{
					m_csPresetPoint+=L"&";
				}

				CString csNum;
				if (it->first!=0)
				{					
					csNum.Format(_T("run%d_num=%d&run%d_stay=%d"), it->first, it->second.first, it->first, it->second.second);
				}
				m_csPresetPoint+=csNum;				
			}

		}
		
	}

	m_bActiven=1;
	return PostThreadMessage(m_nMsgID,PTZGI_vCGI,0);
}

void CPTZGetInfo::_vQueryCgi()
{
	/*if (pNode!=m_pNode && m_bActiven==1)
	{
		CWnd* pWnd=m_pWnd;
		UINT nMsgID=m_nMsgID;
		SuspendThread();

		m_pWnd=pWnd;
		m_nMsgID=nMsgID;
		BOOL bOK=CreateThread();
	}*/
	BOOL bStop=0;

	if (m_bActiven)
	{
		USES_CONVERSION;
		BOOL bOK=0;
		
		int nWebPort;
		if(m_pNode->nWebPort == 0)
			nWebPort = 80;	
		else nWebPort = m_pNode->nWebPort;

		TCHAR tzIP[32];
		TCHAR tzUser[32];
		TCHAR tzPass[32];

		wcscpy(tzIP,   A2W(m_pNode->ip));
		wcscpy(tzUser, A2W(m_pNode->szUser));
		wcscpy(tzPass, A2W(m_pNode->szPass));				

		m_csIP.Format(_T("%s"),tzIP);
		m_csUser.Format(_T("%s"),tzUser);
		m_csPass.Format(_T("%s"),tzPass);
		TCHAR tzPrefix[16]={};
		if (m_pNode->bChannel)
		{
			_stprintf(tzPrefix, _T("//%02X%02X%02X%02X%02X%02X"), m_pNode->mac[0] , m_pNode->mac[1], m_pNode->mac[2], m_pNode->mac[3], m_pNode->mac[4], m_pNode->mac[5]);    
		}
		
		if (m_nCgiType==CGI_GETINFO)
		{
			m_bRunJoyFocus=false;
			m_bRunJoyZoom=false;
			m_bFOCUSCONTSend=0;
			m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_autorun.cgi"), m_csIP, nWebPort, tzPrefix);
			TRACE(_T("\r\nCGI_GETINFO\r\n"));
		}
		else if (m_nCgiType==CGI_GET_AUTOFOCUS)
		{
			m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_autofocus.cgi"), m_csIP, nWebPort, tzPrefix);
			TRACE(_T("\r\nCGI_GET_AUTOFOCUS\r\n"));
		}
		else if (m_nCgiType==CGI_FOCUSCONT)
		{
			m_csCgiUrl.Format(_T("http://%s:%d%s/config/focus_cont.cgi?f=%d"), m_csIP, nWebPort, tzPrefix, m_nPTZSpeed);
			if (m_nPTZSpeed==0)
			{
				//m_bRunJoyFocus=false;
				//m_nCgiType |= CGI_CHECKSTOP;
				bStop=1;
				TRACE(_T("\r\n CGI_FOCUS_STOP \r\n"));
			}
			else
			{
				TRACE(_T("\r\n CGI_FOCUSCONT \r\n"));
			}
		}
		else if (m_nCgiType==CGI_MOVECONT)
		{
			int nPSpeed=0, nTSpeed=0, nZSpeed=0;
			if (m_pNode->byModelType == MODELTYPE::MODEL_1180)
			{
				nPSpeed=abs(m_nPSpeed);
				nTSpeed=abs(m_nTSpeed);
				nZSpeed=abs(m_nZSpeed);

				nPSpeed=nCgiPTZSpeed[nPSpeed];
				nTSpeed=nCgiPTZSpeed[nTSpeed];
				nZSpeed=nCgiPTZSpeed[nZSpeed];

				if (m_nPSpeed<0)
				{
					nPSpeed=-nPSpeed;
				}
				if (m_nTSpeed<0)
				{
					nTSpeed=-nTSpeed;
				}
				if (m_nZSpeed<0)
				{
					nZSpeed=-nZSpeed;
				}
			}			

			if (m_pNode->byModelType == MODELTYPE::MODEL_1180)
				m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_move_cont.cgi?pp=%d&tt=%d&zz=%d"), m_csIP, nWebPort, tzPrefix, nPSpeed, nTSpeed, nZSpeed);
			else
				m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_move_cont.cgi?p=%d&t=%d&z=%d"), m_csIP, nWebPort, tzPrefix, m_nPSpeed, m_nTSpeed, m_nZSpeed);

			if (m_nPTZSpeed==0 && m_nTSpeed==0 && m_nZSpeed==0)
			{
				//m_bRunJoyZoom=false;
				//m_nCgiType |= CGI_CHECKSTOP;
				bStop=1;
				TRACE(_T("\r\nCGI_CHECKSTOP\r\n"));
			}
			else
			{			
				TRACE(_T("\r\nCGI_MOVECONT\r\n"));
			}
		}
		else if (m_nCgiType==CGI_GO_PRESET)
		{
			m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_preset.cgi?name=%d&act=go"), m_csIP, nWebPort, tzPrefix, m_nPreset);
			TRACE(_T("\r\nCGI_GO_PRESET\r\n"));
		}
		else if (m_nCgiType==CGI_AUTOPATROL)
		{
			if (m_bAutoPatrol)
				m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_autorun.cgi?name=patrol&num=%d"), m_csIP, nWebPort, tzPrefix, m_nPatrolNum);
			else
				m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_autorun.cgi?name=none"), m_csIP, nWebPort, tzPrefix);
			TRACE(_T("\r\nCGI_AUTOPATROL\r\n"));
		}
		else if (m_nCgiType==CGI_AUTOSCAN)
		{
			if (m_bAutoScan)
				m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_autorun.cgi?name=scan"), m_csIP, nWebPort, tzPrefix);
				//m_csCgiUrl.Format(_T("http://%s:%d/config/ptz_scan.cgi?type=circle&speed=%d&act=show"),m_csIP,nWebPort,m_nPTZSpeed);
			else
				m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_autorun.cgi?name=none"), m_csIP, nWebPort, tzPrefix);
				//m_csCgiUrl.Format(_T("http://%s:%d/config/ptz_scan.cgi"),m_csIP,nWebPort);
			TRACE(_T("\r\nCGI_AUTOSCAN\r\n"));
		}
		else if (m_nCgiType==CGI_SET_AUTOFOCUS)
		{
			if (m_bAutoFocus)
				m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_autofocus.cgi?autofocus=yes"), m_csIP, nWebPort, tzPrefix);
			else
				m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_autofocus.cgi?autofocus=no"), m_csIP, nWebPort, tzPrefix);
			TRACE(_T("\r\nCGI_SET_AUTOFOCUS\r\n"));
		}
		else if (m_nCgiType==CGI_SET_PRESET)
		{
			m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_preset.cgi?name=%d&act=add"), m_csIP, nWebPort, tzPrefix, m_nPreset);
			TRACE(_T("\r\nCGI_SET_PRESET\r\n"));
		}
		else if (m_nCgiType==CGI_SET_RS485)
		{
			//m_csCgiUrl.Format(_T("http://%s:%d/config/rs485.cgi?enable=%s&proto=%s&devid=%s&baudrate=%s&databits=%s&parity=%s&stopbits=%s")		//,m_csRS485Enable//
			m_csCgiUrl.Format(_T("http://%s:%d%s/config/rs485.cgi?proto=%s&devid=%s&baudrate=%s&databits=%s&parity=%s&stopbits=%s")
				, m_csIP,nWebPort
				, tzPrefix
				, m_csProtocol
				, m_csDeviceID
				, m_csBraudRate
				, m_csDataBits
				, m_csParity
				, m_csStopBits);
			TRACE(_T("\r\nCGI_SET_RS485\r\n"));
		}
		else if (m_nCgiType == CGI_GET_PATROLNUM)
		{
			m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_patrol_info.cgi?num=%d"), m_csIP, nWebPort, tzPrefix, m_nPatrolNum);
			TRACE(_T("\r\nCGI_GET_PATROLNUM\r\n"));
		}
		else if (m_nCgiType == CGI_SET_PATROLNUM)
		{
			m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_patrol_info.cgi?num=%d&act=update&%s"), m_csIP, nWebPort, tzPrefix, m_nPatrolNum, m_csPresetPoint);
			TRACE(_T("\r\nCGI_SET_PATROLNUM\r\n"));
		}

		else if (m_nCgiType == CGI_GET_PTZSPEED)
		{
			m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_speed.cgi"), m_csIP, nWebPort, tzPrefix);
			m_csCGIStatus=mycurl.get(m_csCgiUrl,m_csUser,m_csPass);
			TRACE(_T("\r\CGI_GET_PTZSPEED : %s\r\n"), m_csCGIStatus);
		}



		if (!m_bStopCGI || bStop==1)
		{
			m_csCGIStatus=mycurl.get(m_csCgiUrl,m_csUser,m_csPass);
			if (m_nCgiType == CGI_SET_RS485 && m_pNode->byModelType==MODELTYPE::MODEL_1180)
			{
				int nPTZSpeed=0;
				nPTZSpeed=nCgiPTZSpeed[m_nPTZSpeed];
				m_csCgiUrl.Format(_T("http://%s:%d%s/config/ptz_speed.cgi?s=%d"), m_csIP, nWebPort, tzPrefix, nPTZSpeed);
				m_csCGIStatus=mycurl.get(m_csCgiUrl,m_csUser,m_csPass);
				TRACE(_T("\r\nCGI_SET_PTZSPEED : %d\r\n"), nPTZSpeed);
			}


			int nReSend(0);
			while (m_bFOCUSCONTSend && m_nCgiType==CGI_FOCUSCONT && !bStop && nReSend==0)
			{
				Sleep(250);
				if (bStop || m_bStopCGI)
					break;

				m_csCGIStatus=mycurl.get(m_csCgiUrl,m_csUser,m_csPass);
				CString csMsg;
				csMsg=L"\r\n ReSend Focus   "+m_csCGIStatus+L"  \r\n";
				nReSend++;
				TRACE(csMsg);
			}

			if ( bStop==1 )
			{
				CString csReStop;
				csReStop.Format(_T("\r\nCGI_STOP -> 1\r\n"));
				TRACE(csReStop);

				for (int nStop=0;nStop<2;nStop++)
				{
					Sleep(20);
					m_csCGIStatus=mycurl.get(m_csCgiUrl,m_csUser,m_csPass);
					csReStop.Format(_T("\r\nCGI_STOP -> %d\r\n"),nStop+2);
					TRACE(csReStop);
				}

				m_bRunJoyFocus=false;
				m_bRunJoyZoom=false;
			}
		}		
		
		m_bActiven=0;
		m_csCgiUrl.Empty();
		if (bStop)
		{
			m_bStopCGI=0;
		}
		TRACE(m_csCGIStatus);
	}
}

void::CPTZGetInfo::SetTimerQueryCgi()
{
	PostThreadMessage(m_nMsgID,PTZGI_vCGI_START,0);
	//Wait for cgi from http 5 sec//
	//for (int i=0;i<25;i++)
	//{
	//	Sleep(200);
	//}

	//if (m_bActiven==1)
	//{
	//	m_pWnd->PostMessage(m_nMsgID,PTZGI_FAIL,m_nCgiType);
		//SuspendThread();
	//}
};

