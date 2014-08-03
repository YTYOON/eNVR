#pragma once
#include "resource.h"
#include "mycurl.h"
#include "MainFrm.h"

// CDlgScanRemoteCam dialog

#define NVR_Etrocenter 0
#define NVR_ISAP 1

struct RemoteNVRInfo	
{

	TCHAR	tcIPAddress[16];
	TCHAR	tcUser[32];
	TCHAR	tcPass[32];
    TCHAR   tcModelName[64];
	int		iHttpPort;
	TCHAR	tcDeviceName[64];
};

class CDlgScanRemoteCam : public CDialog
{
	DECLARE_DYNAMIC(CDlgScanRemoteCam)
private:
	void InsertRemoteDevice(int iRTSPPort);
	void InsertRemoteChannel(int iRTSPPort);
	void InsertOneObject2DB(int iTemp, int iRTSPPort, bool isChannel);
	int GetVideoFormat(int nVideoFormat, bool bIpvStation, bool isChannel);

public:
	CDlgScanRemoteCam(CWnd* pParent = NULL);   // standard constructor
	CDlgScanRemoteCam(CString& strIP,  CString& strPort,  CString& strUser, 
		 CString& strPass, int nSelectGroupId = 0, CWnd* pParent = NULL);
	
	virtual ~CDlgScanRemoteCam();

// Dialog Data
	enum { IDD = IDD_DLG_REMOTE_NVR_CAM_LIST };
	enum RTSP_TYPE{UDP=0, TCP, HTTP, UNKNOWN};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    bool m_bSameLocalLan;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	class CCheckHeadCtrl : public CHeaderCtrl
	{
	public:
		CCheckHeadCtrl(){};
		virtual ~CCheckHeadCtrl(){};

		DECLARE_MESSAGE_MAP()
	protected:
		afx_msg void OnItemClicked(NMHDR* pNMHDR, LRESULT* pResult);
	};


public:
	RemoteNVRInfo m_RemoteNVRInfo;
	CString m_strXmlBuffer;
	CString m_csIP, m_csUser, m_csPass, m_csPort, m_csModelName;
	int m_iGroupID, m_iDBExitCams, m_iTotalCamera;
	vector<int> veCamera2Stream;
	CListCtrl* pScanList;
public:
	afx_msg void OnBnClickedOk();
	int		iGetRTSPPortAndProtocol();
	CImageList	m_checkImgList;
	CCheckHeadCtrl	m_checkHeadCtrl;

	//--InitFlag
	int iInitFlag;
	void vSetInitFlag(int iFlag, bool bCoLocalLan);

	CString m_csStreamProtocol;
	int m_iStreamPort;
	int m_iHttpPort;

//////////////////////////////////////////////////////////////////////////Lynn
private:
	CNVRBk *m_pBk;
	int m_nDeviceID;
	int m_nCameraID;
	vector<NODEITEM> m_vcNodes;
	vector<ec_Camera> m_vcExistCam;

	void Trans_Camera2NODEITEM(const ec_Camera& camObj, NODEITEM& node);
	void InitialListCtrl(CListCtrl* pScanList, int iInitFlag);
	void VerifyCtrlStyle(CListCtrl* pScanList);
	bool isErrorResponse(const CString& csBufferXml);
	bool isExistChannel(const CString& strMac);
	bool isExistStation();
	void UpdateDatabase();
public:
	void GetStream(bool bIpvStation, int iRTSPPort, int nStreamCount, vector<ec_Stream>& vcStreams);
	void InsertCamera(int iTemp, int iRTSPPort, bool isChannel, bool bIpvStation);
	void InsertGroupCamera();
	void InsertCameraRec();
	void GetStringByLanguage(CString& strName, CString& strIP, CString& strMac, CString& strModel, CString& strAllStream, CString& strURL, CString& strVideoFm, CString& strSupportPTZ);
	void SetXMLBuffer(const CString& strXmlBuffer){m_strXmlBuffer = strXmlBuffer;};
	void GetXmlBuffer(CString& csBuffer);
	void GetChannels(vector<NODEITEM>& vcNodes){vcNodes = m_vcNodes;};
};
