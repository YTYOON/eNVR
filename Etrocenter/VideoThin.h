#pragma once

#include "Video.h"
#include "mycurl.h"



// CVideoThin

class CVideoThin : public CVideo
{
protected:
    virtual void vSetRecvAudio();
	virtual void vSetRecVideo();
    HANDLE m_hNextChannel;
	CONNECTINFOEX m_conxInfoEx;
   
    vector <CONNECTINFOEX*> m_vtconxInfoEx;
    HANDLE m_hStreamLive;
    vector <NODEITEM*> m_vtpNode;
    WORD m_nConxIdx;
//    WORD m_nNodeIdx;

public:
	 CONNECTINFOEX* m_pconxInfoEx;
//	DECLARE_DYNAMIC(CVideoThin)
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoSet)
	//}}AFX_VIRTUAL

public:
	virtual void vAttach(vector <NODEITEM*>* pNode, BYTE connectType =NETSESSION_LOGIN_TYPE_VIDEO | NETSESSION_LOGIN_TYPE_AUDIO);
	virtual void vAttach(NODEITEM* pNode, BYTE connectType = NETSESSION_LOGIN_TYPE_VIDEO | NETSESSION_LOGIN_TYPE_AUDIO);
    void vSetPlayNextChannel(int nChannel = -1){
        if (nChannel < 0)
        {
            if (++m_nConxIdx > m_vtconxInfoEx.size()-1)
            {
                m_nConxIdx = 0;
            }        
        }else
        {
            m_nConxIdx = nChannel;
        }
        BOOL bRet = SetEvent(m_hNextChannel);
        TRACE(_T("SetEvent return %d"), bRet);
    }
	CVideoThin();
	virtual ~CVideoThin();

protected:
	DECLARE_MESSAGE_MAP()
//	static UINT __stdcall AVReceiveThread(LPVOID lpParam);
	virtual LONG VideoPlayThreadRunPack();
    bool bDisConnectStreamingSvr(CONNECTINFOEX* pconxInfoEx);
    bool bConnectStreamingSvr(CONNECTINFOEX* pconxInfoEx);
	void GetStreamUrl(TCHAR *tzPath, NODEITEM* pNode);
	void GetStreamPort(DWORD& nPort, NODEITEM* pNode);
	virtual void ReconnectStream();
	void SwitchRecLight(bool bTurnoff = false);
	
public:
    void vShiftNextChannel();
	bool bFillUpAviHeadInfo();
	void vSetStatus(bool bStatus);

	/////////////////////DON add 103-01-21//////////////////////////
	static UINT __stdcall CVideoThin::thdPatrol(LPVOID lpParam);
	static UINT __stdcall CVideoThin::thdAutoScan(LPVOID lpParam);
	map <int,pair_INT> m_mapPatrolSeqDurSets;
	void CVideoThin::vGoToPresetPoint(int nPreset);
	//CPTZGetInfo*	m_pPTZGetInfo;
	Curlplus mycurl;
	void CVideoThin::vbeginPatrolThread();
	void CVideoThin::vbeginAutoScanThread(int ptzAction, int nPTZSpeed=0);
	int	m_nptzAction;
	void vFreePTZThread();

	HANDLE m_hPatrolThread;
	HANDLE m_hAutoScanThread;
	////////////////////////////////////////////////////////////////////
	
	virtual void SetStopPalay(BOOL bRes);
	virtual void SetStopRecv(BOOL bRes);

	BOOL GetStopPalay(){return m_bStopPlay;};
	BOOL GetStopRecv(){return m_bStopRecv;};;
private:
	BOOL  m_bStopPlay;
	BOOL  m_bStopRecv;
};


