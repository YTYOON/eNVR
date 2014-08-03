#pragma once

#include "mycurl.h"
#include "DlgPtzSetting.h"
#include "gsdefine.h"

//////////define PTZGetInfo message type/////////
enum MSGType {PTZGI_Init=0,PTZGI_vCGI,PTZGI_vCGI_START,PTZGI_OK,PTZGI_FAIL};
/////////////////////////////////////////////////////

//////////define PTZGetInfo cgi type/////////
enum CGIType {CGI_GETINFO=0,CGI_GET_AUTOFOCUS,CGI_FOCUSCONT,CGI_MOVECONT,CGI_GO_PRESET,CGI_AUTOPATROL,CGI_AUTOSCAN,CGI_SET_AUTOFOCUS,CGI_SET_PRESET,CGI_SET_RS485,CGI_CHECKSTOP,CGI_SET_PATROLNUM,CGI_GET_PATROLNUM, CGI_GET_PTZSPEED};
///////////////////////////////////////////////
																							   

// CPTZGetInfo

class CPTZGetInfo : public CWinThread
{
	DECLARE_DYNCREATE(CPTZGetInfo)

//protected:
	CPTZGetInfo();           // protected constructor used by dynamic creation
	virtual ~CPTZGetInfo();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	BOOL Init(CWnd* pWnd,UINT nMsgID);
	BOOL _Init();
	BOOL vQueryCgi(DWORD nCgiType, NODEITEM* pNode, int nPSpeed=0, int nTSpeed=0, int nZSpeed=0);
	BOOL vQueryCgi(DWORD nCgiType, NODEITEM* pNode, PTZSettingInfo* ptzInfo);
	BOOL vQueryCgi(DWORD nCgiType, NODEITEM* pNode, map <int,pair_INT> mapPatrolSeq);
	void _vQueryCgi();
	void SetTimerQueryCgi();

public:
	Curlplus mycurl;

	CWnd*	m_pWnd;
	UINT		m_nMsgID;

	NODEITEM* m_pNode;
	BOOL				m_bActiven;

	CString		m_csIP, m_csUser,m_csPass, m_csCgiUrl;
	DWORD		m_nCgiType;
	CString		m_csCGIStatus;
	int				m_nPTZSpeed;
	int				m_nPSpeed;
	int				m_nTSpeed;
	int				m_nZSpeed;
	int				m_nPreset;
	BOOL			m_bAutoPatrol;
	BOOL			m_bAutoScan;
	BOOL			m_bAutoFocus;
	CString		m_csRS485Enable;
	//PTZSettingInfo	m_ptzInfo;
	CString		m_csProtocol;
	CString		m_csDeviceID;
	CString		m_csBraudRate;
	CString		m_csDataBits;
	CString		m_csParity;
	CString		m_csStopBits;
	CString		m_csPTZSpeed;
	CString		m_csComm;

	BOOL			m_bStopCGI;
	int				m_nPatrolNum;
	CString		m_csPresetPoint;

	BOOL			m_bFOCUSCONTSend;

	BOOL			m_bRunJoyFocus;
	BOOL			m_bRunJoyZoom;


protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


