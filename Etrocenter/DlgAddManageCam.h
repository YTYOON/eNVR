#pragma once
#include "resource.h"
#include "atltypes.h"
#include "afxwin.h"
#include "BaseDialog.h"
#include "afxcmn.h"
#include "DlgModifyStreamPTZ.h"
#include "DlgSettingRecording.h"
#include "DlgAddRemoteNVR.h"
#include "DlgNAS.h"

#define ADDCAMERA 0
#define ADD_NEW_IP_CAM_GROUP 1
#define ADD_NEW_IP_CAM_NODE 2
#define MODIFY_IP_CAM 3
#define MODIFY_NVR_IP_CAM 4

#define UNAUTHORIZED 12
#define OLD_TYPE 10
#define NEW_TYPE 11
#define ERROR_TYPE 13

//struct NASInfo{
//
//	enum NASTYPE	{SAMBA=0, NFS=1};
//	DWORD	dwServerType;
//	TCHAR	tcServerAddr[130];
//	TCHAR	tcServerUser[100];
//	TCHAR	tcServerPass[100];
//};

struct StreamInfo
{
	CString csStreamVideoCompressionType;
	CString csStreamProtocol;
	CString csStreamResolution;
	CString csStreamPort;
	CString csLV_RE;
};

struct IPCamInfo
{
	CString csMicrophone;
	CString csSpeaker;
	CString csStreamSum;

	CString csRS485Enable;
	CString csRS485Protocol;
	CString csRS485In1;
	CString csRS485In2;
	CString csRS485Out1;

	CString csCameraIDX;
	CString csNetmask;
	CString csGateway;
	CString csDNS;
	CString csWebPort;
	CString csCameraName;
	CString csConnectType;
	CString csIP;
	CString csMac;
	CString csUser;
	CString csPWD;
	CString csModel;
	CString csActive;
	CString csStreamUrl;
    CString csSn;

	CString csNetmask1;
	CString csNetmask2;
	CString csNetmask3;
	CString csNetmask4;
};

struct sSETS
{
	enum TYPE {STRING=0, CHECK, INTERGER};
	TYPE nType;
	CString csKey;
	bool    bCheck;
	int     nInt;
	CString csCaption;
	bool IntFlag;//if NewValue = int, IntFlag = false;
	sSETS(TCHAR* A, TCHAR* B, bool C){csKey = A; csCaption = B; IntFlag = C;};
	sSETS(TCHAR* A, TCHAR* B){csKey = A; csCaption = B;};
    sSETS(){};
	
};

struct sVERSETS
{
	map <CString, sSETS> aSet;
	bool flag;//if changed, flag = false;
//	map <CString, VERSETS>* pSub;
};

class CNVRBk;
class CDlgAddManageCam : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgAddManageCam)

public:
	CDlgAddManageCam(NODEITEM* Node, int Init, CWnd* pParent = NULL);   // standard constructor
	CDlgAddManageCam(CString TreeNodeText, int Init, CWnd* pParent = NULL);	//Add New IP Cam from Group
	CDlgAddManageCam(int Init, CWnd* pParent = NULL);
	CDlgAddManageCam(int CameraID, int Init, CWnd* pParent = NULL);
	virtual ~CDlgAddManageCam();

	// Dialog Data
	enum { IDD = IDD_DLGADDMANAGECAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	int m_nCurHeight;
	int m_nScrollPos;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	virtual BOOL OnInitDialog();

public:
	/*--Flag--*/
	int initFlag;
	bool dirtyFlag;
	bool initGroupFlag;
	int CGIFlag; //0 Old Type, 1 New Type
	bool ENCPFlag;	//Find the ITEMNODE by ENCP if ENCPFlag is true
	bool RecordingFlag; // 0 Default

	/*--Parameter--*/
	NODEITEM* pNode;
	CString csTreeNodeText;
	int iCameraID;
	CString csDSN, csDBUser, csDBPass, csODBC;	//For Database
	map <CString, sVERSETS> ScanMap;
	IPCamInfo vIPCamInfo;
	StreamInfo vStreamInfo;
	//CDatabase DBQuery;
	CDatabase* pDB;
	NODEITEM m_Node;
	vector<pair <int,CString>> DBIPCamGroupVector;	//IP Cam Group from Table 'ec_camera_group_camera' 
	vector<pair <int,CString>> *m_pvtIPCamGroup;

	CString csMacExit;	//'cameraidx' in DB 
	CString csMacExit_cameraid;	//'cameraid' in DB 
	CString csThreadIP;


	BYTE m_SQLBytes[43];

	/*--Struct for Modify Stream and PTZ--*/
	//CDlgModifyStreamPTZ *m_CDlgModifyStreamPTZ;
	MoPTZInfo m_MoPTZInfo;
	MoStreamInfo m_MoStreamInfo;
	vector<MoStreamInfo> m_veMoStreamInfo;
	void vGetMoPTZInfo();
	vector<CString> veOrgInfo; //Save Stream Protocols in DB, [0] is Mac.
	vector<pair <int , CString>> veGroupList;

	/*--Parameter for Getting Camera Info from ENCP--*/
	vector <NODEITEM*>* m_pvtNode;
	void vGetInfoFromENCP(CString csIP);

	/*--Work Function--*/
	//void vDBConnection();
	CString csGetChannelID();
	void vGroupListInit();
	void vCheckDefaultGroup();
	void vCheckDefaultGroup(CString m_csTreeNodeText);

	void vCheckDefaultGroup(int nRootGroupID);		//Larry add 102-12-16//

	void vGetIPCamDataFromDB(int iCameraID);
	void vGetStreamDataFromDB(int iCameraID);
	void StreamInit();
	void vGetStreamDataFromVector(int iStreamNum);
	void vGetCGIData(CString csIP);
	static UINT vGetCGIData (LPVOID lpParam);

	void vParseSegment(TCHAR* tszBuffer, map <CString, sVERSETS>::iterator pos);
	void vGetIPCamDataFromMap(int CGIFlag);
	void vSetCredentials();
	void vGetCGIDataNew(CString csIP);
	static UINT vGetCGIDataNew(LPVOID lpParam);

	void vParseSegmentNew(TCHAR* tszBuffer, map <CString, sVERSETS>::iterator pos);
	int ParseSegmentVideoCgi(TCHAR* tszBuffer, map <CString, sVERSETS>::iterator pos);
	void vParseSegmentVideoCgi2(TCHAR* tszBuffer, int cNumOfProfile);
	void vGetStreamDataFromMap(int CGIFlag);
	void vSetStreamPTZUI();
	CString csGetResolutionDesc(CString csScan);
	void vInsertIPCam();
	void vUpdateIPCam();
	void vSaveGroupList();
	//void vSaveLV_REStream();
	void vGetUIData();
	void vGetIPCamGroup(CString csCameraID);
	void vDirtyFlagChanged();
	void vSetDefaultRecordType();
	bool bSaveNewRecordingPath(CString csCameraIDX);

	//Lynn modified at 2013/10/22
	void	vUpdateNodeItem();

	//--Check IP Cam Model Type
	int bGetIPCamType(CString csIP);	//True->Old, False->New

	/*------Check IPCAM event-----*/
	void vCheckEvent(map <CString, sVERSETS>::iterator pos, CString csIP);
	void vDisplayEventInfo();
	map<CString, sVERSETS> EventInfoMap;
	

	//--Initial Stream Protocol ComboBox
	void vInitStreamProtocol();

	//--Check Storage Buffer
	bool bCheckPathBuffer( CString csPath, int iBuffer);
	map<CString, int>	mapPathBuffer;
	void vGetPathBufferFromDB();

	afx_msg void OnCbnSelchangeComboAvStream();
	afx_msg void OnBnClickedButtonModifyAvPtz();
	afx_msg void OnBnClickedButtonScanCamera();
//	afx_msg void OnBnClickedButtonDetectCamera();
	afx_msg void OnBnClickedButtonSaveDuplicate();
	afx_msg void OnBnClickedButtonDetectCamera();
	afx_msg void OnBnClickedCheckCameraEnable();
	afx_msg void OnEnChangeEditCameraName();
	afx_msg void OnBnClickedRadioDynamicIp();
	afx_msg void OnBnClickedRadioStaticIp();
	afx_msg void OnBnClickedRadioPppoe();
	afx_msg void OnIpnFieldchangedIpaddressIpAddress(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditWebPort();
	afx_msg void OnIpnFieldchangedIpaddressSubnetMask(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIpnFieldchangedIpaddressNameServer(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIpnFieldchangedIpaddressGateway(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditUser();
	afx_msg void OnEnChangeEditPassword();
protected:
	CNVRBk *m_pBk;
	void GetStream(vector<ec_Stream>& vcStream);
//	virtual void OnCancel();
public:
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnBnClickedCancel();
//	afx_msg void OnBnClickedCancel();
protected:
//	virtual void OnCancel();
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonRecordingDlg();
	afx_msg void OnBnClickedButtonGroupSetting();
	afx_msg void OnBnClickedButtonRemoteCamera();
	afx_msg void OnBnClickedRadioRtspTcp();
	afx_msg void OnBnClickedRadioRtspUdp();
//	afx_msg void OnNMThemeChangedComboStreamProtocol(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboStreamProtocol();
	afx_msg void OnBnClickedCheckNas();

	//--NAS Info
	NASInfo m_NASInfo;
	afx_msg void OnBnClickedBtnNas();
	map<CString, sVERSETS> mapNAS;
	void vGetNASInfoFromCamera();
	afx_msg void OnBnClickedRadioRtspHttp();
};