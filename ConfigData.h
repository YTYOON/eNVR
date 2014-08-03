#pragma once

typedef void(*UpdateCgiFun)(void*);

class CConfigData
{
private:
	bool m_isCGIReady;
	vector<NODEITEM> m_vcOldNode;
	vector<ec_Camera> m_vcOldCamObj;
	vector<Cam_Group_Cam> m_vcOldGrpCam;
	vector<NODEITEM> m_vcModifyNode;
	vector<ec_Camera> m_vcModifyCamObj;
	vector<Cam_Group_Cam> m_vcModifyGrpCam;
	vector<map<CString , CJason> > m_vcmapJason;
	vector<Cam_Group> m_vcGroup;
	int m_nodeIdx;
	CNVRBk *m_pBK;
	ResolutionBySensor m_Resolutions;
	map<int ,CString > m_mapBitrateByProfileId;
	map<int ,CString > m_mapFramerateByProfileId;
	fpsBySensor m_fps;
	void* m_callbackParam;

public:
	UpdateCgiFun m_UpdateCgiFun;
	CConfigData(void);
	~CConfigData(void);

	void Initial(vector<NODEITEM>& vcNodeItem, UpdateCgiFun pFun, void* vParam);
	void GetOldNodeItem(NODEITEM& pNode){if(m_vcOldNode.size()>m_nodeIdx)pNode = m_vcOldNode[m_nodeIdx];};
	void GetOldCamObj(ec_Camera& CamObj){if(m_vcOldCamObj.size()>m_nodeIdx)CamObj = m_vcOldCamObj[m_nodeIdx];};
	void GetOldGrpCam(Cam_Group_Cam& GrpCam){if(m_vcOldGrpCam.size()>m_nodeIdx)GrpCam = m_vcOldGrpCam[m_nodeIdx];};
	void GetModifyNodeItem(NODEITEM& pNode){if(m_vcModifyNode.size()>m_nodeIdx) pNode = m_vcModifyNode[m_nodeIdx];};
	void GetModifyCamObj(ec_Camera& CamObj){if(m_vcModifyCamObj.size()>m_nodeIdx)CamObj = m_vcModifyCamObj[m_nodeIdx];};
	void GetModifyGrpCam(Cam_Group_Cam& GrpCam){if(m_vcModifyGrpCam.size()>m_nodeIdx)GrpCam = m_vcModifyGrpCam[m_nodeIdx];};

	void SetModifyNodeItem(NODEITEM& pNode){if(m_vcModifyNode.size()>m_nodeIdx)m_vcModifyNode[m_nodeIdx] = pNode;};
	void SetModifyCamObj(ec_Camera& CamObj){if(m_vcModifyCamObj.size()>m_nodeIdx)m_vcModifyCamObj[m_nodeIdx] = CamObj;};
	void SetModifyGrpCam(Cam_Group_Cam& GrpCam){if(m_vcModifyGrpCam.size()>m_nodeIdx)m_vcModifyGrpCam[m_nodeIdx]=GrpCam;};

	void GetCGI(map<CString , CJason>& mapJason){if(m_vcmapJason.size()>m_nodeIdx)mapJason = m_vcmapJason[m_nodeIdx];};
	void GetGroups(vector<Cam_Group>& vcGroup){vcGroup = m_vcGroup;};
	void SetNodeIdx(int nodeIdx){m_nodeIdx = nodeIdx;}

	void GetFpsArray(fpsBySensor& fps){fps = m_fps;};
	void GetResolutionArray(ResolutionBySensor& Resolutions){Resolutions = m_Resolutions;}; 
	void GetDefaultFramerate(map<int ,CString >& mapFramerateByProfileId){mapFramerateByProfileId = m_mapFramerateByProfileId;};
	void GetDefaultBitrate(map<int ,CString >& mapBitrateByProfileId){mapBitrateByProfileId = m_mapBitrateByProfileId;};

	void SetVideoCGI(map<CString, CString>& mapCtrlValue);
	void SetStreamCGI(map<CString, CString>& mapCtrlValue);
	void SetCameraInfo(map<CString, CString>& mapCtrlValue);
	void SetProtocol(const CString& strProtocol);
	void SetStreamInfo(map<CString, CString>& mapCtrlValue);
	void SaveAllData();
	void CloseCgiRequest();
	
private:
	void InitialDefaultValue();
	void SetNodeitem(const vector<NODEITEM>& vcNode);
	bool GetCGIInfo();
	bool isNeedCGIInfo();
	static UINT _stdcall GetCGIInfoThread (LPVOID lpParam);
};
