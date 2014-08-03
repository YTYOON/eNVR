#pragma once
#include "DataStructure.h"
#include "gsdefine.h"
#include "Jason.h"
//#include "Sqlex.h"


enum Video_Format_Filter
{
	FILTER_FIRST_BYTE	= 0x000000FF,
	FILTER_SECOND_BYTE	= 0x0000FF00,
	FILTER_THIRD_BYTE	= 0x00FF0000,
	FILTER_FOURTH_BYTE	= 0xFF000000
};

enum TreeType
{ 
	AllTree = 0,
	DeviceTree, 
	LayoutTree
};

const UINT  MsgUpdateView = RegisterWindowMessage(_T("MsgUpdateView") );
enum UpdateViewMsg
{
	UM_REFRESH_ALLTREE = 0x00,
	UM_REFRESH_LAYOUTTREE,
	UM_REFRESH_DEVICETREE,
	UM_DELETE_TREENODE,
	UM_FREE_VIDEO,
	UM_SET_VIDEO_FOCUS,
	UM_SET_TREENODE_FOCUS,
	UM_INSERT_TREENODE,
	UM_DELETE_STATION,
	UM_UPDATE_CAMERA_ID,
	UM_UPDATE_DEVICE, 
	UM_SET_LIVEVIEW_AUTOTYPE
};

typedef enum OEM_Product_Company
{
	Invalid_Company = 0,
	Etro_Company,
	XTS_Company,
	CVS_Company,
	GNC_Company,
	NGO_Company
}OEM_Company;
typedef enum OEM_Product_Type
{
	Non_Type,
	CMS_Type,
	NVR_Type
}OEM_Type;

//sensormode-profile_mode-profile number-resolutions

typedef map<CString,vector<vector<CString> > > ResolutionByProfile;
typedef map<CString , ResolutionByProfile > ResolutionBySensor;

typedef map<CString,vector<CString> > fpsByProfile;
typedef map<CString , fpsByProfile > fpsBySensor;


class CMainFrame;
class CNVRBk
{
private:
	bool Initial();
	bool InitialDB();
	bool InitialOEM();
	bool InitialResolution();
	void InitialONVIF();
	void GetResolutionBySensor(const CString& strSec, LPWSTR lpstr);
	void GetFpsBySensor(const CString& strSec, LPWSTR lpstr);
	void GetBitrate(LPWSTR lpstr);
	void GetFramerate(LPWSTR lpstr);
	void GetDefaultValue(const CString& strSec, LPWSTR lpstr, map<int ,CString >& mapDefaultValue);
	void TokenStr(const CString& strSec, const CString& strKey, ResolutionByProfile& mapValue, LPWSTR lpstr);
	void TokenStr(const CString& strSec, const CString& strKey, fpsByProfile& mapValue, LPWSTR lpstr);

	ResolutionBySensor	m_Resolutions;
	fpsBySensor			m_fps;
	map<int ,CString > m_mapBitrateByProfileId;
	map<int ,CString > m_mapFramerateByProfileId;
	vector<NODEITEM>	m_vcOnvif;

public:
	CNVRBk(void);
	~CNVRBk(void);

	/************************************************************************/
	/* Initial NVRBk                                                                     */
	/************************************************************************/
	static CNVRBk *GetBkObject();

	/************************************************************************/
	/* Get DB Data                                                                     */
	/************************************************************************/
	void GetGroup(TreeType byTreeType, vector<Cam_Group>& vcGroup, bool bAutoCreate = true);
	void GetGroup(int nGrpId, Cam_Group& Group);
	void GetGroup(TreeType treeType, const CString& str, Cam_Group& Group);
	void GetGroupCamera(int nGrpId, vector<Cam_Group_Cam>& vcGroupCam);
	void GetGroupCamera(int nGrpId, int nCamId, vector<Cam_Group_Cam>& vcGroupCam);
	void GetGroupCamera(int nCamId, vector<Cam_Group_Cam>& vcGroupCam, bool isIndex);

	void GetAllGroupCamera(vector<Cam_Group_Cam>& vcGroupCam);
	void GetLastGroupCamera(Cam_Group_Cam& grpCam);

	void GetCamera(const CString& strMac, ec_Camera& camObj);
	void GetCamera(int nCamId, ec_Camera& camObj, bool isIndex = false);
	void GetCamera(const CString& strIP, const CString& strMac, ec_Camera& camObj);
	void GetCamera(const CString& strMac, vector<ec_Camera>& vccamObj);

	void GetAllCameras(vector<ec_Camera>& vcCam);
	void GetStream(int nCamId, vector<ec_Stream>& vcStream);

	void GetAllCamRecs(vector<ec_Camera_Rec>& vcRec);
	void GetCamRec(int nCamIdx, ec_Camera_Rec& CamRec);

	void GetAllStreamByCamid(map<int, vector<ec_Stream> >& mapStream);
	int GetFirstGroupIdx(TreeType treeType);
	int GetLastGroupIdx();
	void GetLastCameraIdx(int& nCameraid, int& nCamera_idx);
	int GetLastCameraIdx();
	int GetLastDeviceIdx();
	int GetLastCameraId();
	void GetAllCameraId(vector<int>& vcCamId, bool isIdx = false);
	void GetStorageByType(vector<ec_Storage>& vcStorageObj, const CString& strStorageType);
	void GetEcParam(const CString& strParam, CString& strValue);
	bool CheckTreeGroup(TreeType treeType);

	/************************************************************************/
	/* Modify DB Data                                                                      */
	/************************************************************************/
	bool InsertGroups(vector<Cam_Group>& vcGroup);
	bool InsertGroup(const Cam_Group& Group);
	bool InsertCameras(const vector<ec_Camera>& vcCamera);
	bool InsertRecs(const vector<ec_Camera_Rec>& vcRec);
	bool InsertCamera(const ec_Camera& Camera);
	bool InsertGroupCamera(const Cam_Group_Cam& GroupCam);
	bool InsertCamRec(const ec_Camera_Rec& CamRec);
	
	bool DeleteGroupByTreeType(TreeType treeType);
	bool DeleteGroup(int nGrpId);
	bool DeleteGroupCameraByGroup(int nGrpId);
	bool DeleteCamRecTime(int nCamIdx);
	bool DeleteCamRec(int nCamIdx);
	bool DeleteGroupCamera(int nCamId);
	bool DeleteGroupCamera(int nCamId, int nGroupId);
	bool DeleteGroupCameraByGroupCameraId(int nGrpCamId);
	bool DeleteStream(int nCamId);
	bool DeleteCamera(int nCamId);

	bool UpdateGroupId(int nNewGroup, int OldGroup);
	bool UpdateStreamType(const vector<ec_Stream> vcStream);
	bool UpdateGroupCameraGrpId(int OldGroupid, int NewGroupid, int nCamId);
	bool UpdateGroupCamera(const Cam_Group_Cam& GroupCam);
	bool UpdateGroupName(const CString& strNewGrpName, int nGrpId);
	bool UpdateDefaultGroup();
	bool UpdateGroupType(byte byType, int nGrpId);
	bool UpdateCamera(const ec_Camera& Camera, bool bUpdateStream = true);

	/************************************************************************/
	/* Get & Set Member Data                                                                     */
	/************************************************************************/
	TreeType GetTreeType(){return m_TreeType;};
	void SetTreeType(TreeType treeType){m_TreeType=treeType;};
	void SendMessage(UINT uMessage, WPARAM wParam, LPARAM lParam);
	void RegistWnd(CWnd* wnd){m_RegistWnd.push_back(wnd);};
	void UnregistWnd(CWnd* wnd);
	CMainFrame* GetMainFrame(){ return (CMainFrame*)AfxGetMainWnd(); };

	/************************************************************************/
	/* utility function                                                                     */
	/************************************************************************/
	BOOL isNVRType();
	BOOL isATMType(){return m_bATM;};
	OEM_Product_Company GetOEMCompany(){return m_byProductCompany;}
	BOOL isLocalLan(const CString& strAddress, bool& isAddSelf);
	void TransString2Mac(const CString& strMac, BYTE* mac);
	void TransMac2String(BYTE* mac, CString& strMac, bool bDash = false);
	BOOL IsFishEye(const CString& strVideoFormat);
	BOOL IsChannel(const CString& strVideoFormat);
	BYTE GetDevice(const CString& strVideoFormat);
	BYTE GetModelType(const CString& strVideoFormat);
	BYTE GetPanarama(const CString& strVideoFormat);

	DWORD PackVideoFormat(byte FourthByte, byte ThirdByte, byte SecondByte, byte FirstByte);
	void UnpackVideoFormat(const CString& strVideoFormat, byte& FourthByte, byte& ThirdByte, byte& SecondByte, byte& FirstByte);
	
	//bForward=true->127.0.0.1 bForward=false->1.0.0.127
	DWORD dwIpStringToDword(const CString& csIp, bool bForward = false);
	CString strIpDwordToString(const DWORD& dwIp);
	
	void GetResolutionArray(ResolutionBySensor& Resolutions){Resolutions = m_Resolutions;};
	void GetFpsArray(fpsBySensor& fps){fps = m_fps;};
	void GetDefaultFramerate(map<int ,CString >& mapFramerateByProfileId){mapFramerateByProfileId = m_mapFramerateByProfileId;};
	void GetDefaultBitrate(map<int ,CString >& mapBitrateByProfileId){mapBitrateByProfileId = m_mapBitrateByProfileId;};

	CString GetResolutionDesc(const CString& csScan);
	void GetResAndCMPR(CJason& jVideo, int nStreamIdx, CString& video_res, CString& video_cmpr_type);
	PROTOCOL GetStreamProtocol(const ec_Camera& camObj, int nStreamIdx);

	void NotifyRecordSvr();
	void NotifyEventSvr();
	void NotifyHttpSvr();
	
	/************************************************************************/
	/*  ONVIF function                                                                    */
	/************************************************************************/
	void scan_ONVIF();
	CString get_ONVIF_RTSP(CString url, CString csUser, CString csPWD);
	void GetONVIFNodes(vector<NODEITEM>& vcOnvif){vcOnvif = m_vcOnvif;};

	/************************************************************************/
	/* CGI function                                                                     */
	/************************************************************************/
	BOOL isStation(CString& strBuffer, const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword);
	MODELTYPE GetDeviceModelType(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword);
	
	CJason GetCGI(const CString& strCGI, const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));

	CJason GetSensor(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetNetwork(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetInfo(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetRS485(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetIO(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetMcp(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetMcp_System(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetMcp_Lens(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetMcp_Video(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetStream(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetVideo(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetStream_Info(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetAudio(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetCommonInfo(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetVideo_Info(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	CJason GetVideo_Profile(int nProfileId, const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac=_T(""));
	
	CJason SetVideo(map<CString,CString>& mapValue, const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CJason& jason, const CString& strMac=_T(""));
	CJason SetNetwork(map<CString,CString>& mapValue, const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CJason& jason, const CString& strMac=_T(""));
	CJason SetStream(map<CString,CString>& mapValue, const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CJason& jason, const CString& strMac=_T(""));

private:
	CDatabase m_DB;
	TreeType m_TreeType;
	bool m_bOpenDatabase;
	vector<CWnd*> m_RegistWnd;
	OEM_Product_Company		m_byProductCompany;
	OEM_Product_Type		m_byProductType;
	bool	m_bATM;
	//CSqlEx	m_pSqlEx;

	bool UpdateStreams(const vector<ec_Stream>& vcStream, int nCameraId);
	bool InsertStreams(const vector<ec_Stream>& vcStream, int nCameraId);
	void GetGroupObj(CRecordset& RecordObj, Cam_Group& GroupObj);
	void GetCameraGroupCameraObj(CRecordset& RecordObj, Cam_Group_Cam& GroupCamObj);
	void GetCameraObj(CRecordset& RecordObj, ec_Camera& CamObj);
	void GetStorageObj(CRecordset& RecordObj, ec_Storage& StorageObj);
	void GetStreamObj(CRecordset& RecordObj, ec_Stream& StreamObj);
	void GetRecordingObj(CRecordset& RecordObj, ec_Camera_Rec& RecObj);
	CString CombineURL(const CString& strCGI, const CString& strAddress, const CString& strPort, const CString& strMac=_T(""));
};


