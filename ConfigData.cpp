#include "stdafx.h"
#include "ConfigData.h"
#include "NVRBk.h"

HANDLE g_hCGIHandle = NULL;

CConfigData::CConfigData(void):m_nodeIdx(0),m_isCGIReady(false)
{
	m_pBK = CNVRBk::GetBkObject();
}

CConfigData::~CConfigData(void)
{
	CloseCgiRequest();
}

void CConfigData::Initial(vector<NODEITEM>& vcNodeItem, UpdateCgiFun pFun, void* vParam)
{
	m_UpdateCgiFun = pFun;
	m_callbackParam = vParam;

	SetNodeitem(vcNodeItem);
	if (isNeedCGIInfo())
	{
		g_hCGIHandle = (HANDLE)::_beginthreadex(NULL, 0, GetCGIInfoThread, (LPVOID)this, 0, NULL);
	}
}

bool CConfigData::isNeedCGIInfo()
{
	NODEITEM node = {0};
	GetModifyNodeItem(node);
	bool bResult = true;

	if (node.byDeviceType == eNVR || node.byDeviceType == IPVSTATION ||
		node.byDeviceType == ONVIF || node.byModelType != MODEL_1180)
	{
		bResult = false;
		m_isCGIReady = true;
	}
	return bResult;
}

void CConfigData::SetVideoCGI(map<CString, CString>& mapCtrlValue)
{
	USES_CONVERSION;
	map<CString , CJason> mapJason;
	CString str;
	NODEITEM node = {0};
	ec_Camera camObj;
	ec_Stream streamObj;
	Cam_Group_Cam grpCam;

	GetModifyCamObj(camObj);
	GetModifyNodeItem(node);
	GetCGI(mapJason);

	str.Format(_T("%d"), node.nWebPort);
	CJason jason = m_pBK->SetVideo(mapCtrlValue,A2W(node.ip), str, A2W(node.szUser), A2W(node.szPass),mapJason[_T("video")],_T(""));
	if (!jason.isError())
	{
		CString stream_protocol = camObj.vcStream.size()?camObj.vcStream[0].stream_protocol:_T("RTSP");
		short stream_port = camObj.vcStream.size()?camObj.vcStream[0].stream_port:554;
		int nCount = jason.GetCountByKey(_T("profile_id"));
		camObj.vcStream.clear();
		for(int nIdx = 0; nIdx < nCount; nIdx++)
		{
			streamObj.clear();
			if(nIdx == 0)
				streamObj.stream_type = _T("RE");
			streamObj.stream_name.Format(_T("Stream %d"), nIdx+1);
			m_pBK->GetResAndCMPR(jason, nIdx, streamObj.video_res, streamObj.video_cmpr_type);
			streamObj.stream_protocol = stream_protocol;
			streamObj.stream_port = stream_port;
			camObj.vcStream.push_back(streamObj);
		}

		GetModifyGrpCam(grpCam);
		if (grpCam.streamid > camObj.vcStream.size())
		{
			grpCam.streamid = 1;
		}
		SetModifyGrpCam(grpCam);
		SetModifyCamObj(camObj);
	}
};

void CConfigData::SetStreamCGI(map<CString, CString>& mapCtrlValue)
{
	USES_CONVERSION;
	CString str;
	NODEITEM node = {0};
	map<CString , CJason> mapJason;
	ec_Camera camObj;
	CString strValue;

	GetModifyNodeItem(node);
	GetCGI(mapJason);
	GetModifyCamObj(camObj);

	str.Format(_T("%d"), node.nWebPort);
	CJason jason = m_pBK->SetStream(mapCtrlValue,A2W(node.ip), str, A2W(node.szUser), A2W(node.szPass),mapJason[_T("stream")],_T(""));
	if (jason.isError())
		return;

	jason.GetValuebyKey(_T("stream_name"),strValue);
	camObj.stream_url = strValue;

	jason.GetValuebyKey(_T("rtsp_port"),strValue);
	int nIdx = 0, nCount = camObj.vcStream.size();
	for(int nIdx = 0; nIdx < nCount; nIdx++)
	{
		camObj.vcStream[nIdx].stream_port = _ttoi(strValue);
	}

// 	m_pBK->UpdateCamera(camObj,true);
// 	m_pBK->SendMessage(MsgUpdateView, UM_REFRESH_ALLTREE, 0);
	SetModifyCamObj(camObj);
}

void CConfigData::SetCameraInfo(map<CString, CString>& mapCtrlValue)
{
	NODEITEM node = {0};
	ec_Camera camObj;
	vector<Cam_Group_Cam> vcGrpCams;
	vector<Cam_Group> vcGrps;
	Cam_Group_Cam grpCam;

	int nIdx = 0, nCount = 0;
	GetModifyNodeItem(node);
	GetModifyCamObj(camObj);
	GetGroups(vcGrps);
	GetModifyGrpCam(grpCam);

	camObj.cameraname = mapCtrlValue[_T("camera_name")];
	m_pBK->UpdateCamera(camObj,false);

	int nGrpId = 0;
	nCount = vcGrps.size();
	for ( nIdx = 0; nIdx < nCount; nIdx++ )
	{
		if(mapCtrlValue[_T("group")].Find(vcGrps[nIdx].camera_group_desc) != -1)
		{
			nGrpId = vcGrps[nIdx].cameragroupid;
			break;
		}
	}
	grpCam.cameragroupid = nGrpId;
	//m_pBK->UpdateGroupCamera(grpCam);

	SetModifyCamObj(camObj);
	SetModifyGrpCam(grpCam);
}

void CConfigData::SetProtocol(const CString& strProtocol)
{
	NODEITEM node = {0};
	GetModifyNodeItem(node);
	CString str;
	ec_Camera camObj;
	m_pBK->GetCamera(node.cameraid,camObj);

	if (strProtocol.Find(_T("UDP")) != -1)
	{
		str = _T("RTSP");
	}
	else if (strProtocol.Find(_T("TCP")) != -1)
	{
		str = _T("RTPTCP");
	}
	else if (strProtocol.Find(_T("HTTP")) != -1)
	{
		str = _T("RTPHTTP");
	}

	int nIdx = 0, nCount = camObj.vcStream.size();
	for(int nIdx = 0; nIdx < nCount; nIdx++)
	{
		camObj.vcStream[nIdx].stream_protocol = str; 
	}

	//m_pBK->UpdateCamera(camObj,true);
	SetModifyCamObj(camObj);
}

void CConfigData::SetStreamInfo(map<CString, CString>& mapCtrlValue)
{
	map<CString, CString>::iterator it;
	ec_Camera camobj;
	Cam_Group_Cam grpCam;
	GetModifyCamObj(camobj);
	GetModifyGrpCam(grpCam);

	CString strCtrl, strValue;
	int nRecStreamId = 0, nLiveStreamId = 0, nIdx = 0, nCount = 0 ;

	nCount = camobj.vcStream.size();
	for(int nIdx = 0; nIdx < nCount ;  nIdx++)
	{
		camobj.vcStream[nIdx].stream_type.Empty();
	}

	it = mapCtrlValue.begin();
	while(it != mapCtrlValue.end())
	{
		if (it->first.Find(_T("livestream")) != -1)
		{
			grpCam.streamid =_ttoi(it->second.Right(1));
		}
		else if (it->first.Find(_T("recording")) != -1)
		{
			nRecStreamId =_ttoi(it->second.Right(1));
			if ((it->second.Left(2) != _T("No")))
			{
				if(camobj.vcStream.size() >= nRecStreamId)
					camobj.vcStream[nRecStreamId-1].stream_type += _T("RE");
			}		
		}

		it++;
	}
	SetModifyCamObj(camobj);
	SetModifyGrpCam(grpCam);
}

void CConfigData::InitialDefaultValue()
{
	m_pBK->GetResolutionArray(m_Resolutions);
	m_pBK->GetDefaultBitrate(m_mapBitrateByProfileId);
	m_pBK->GetDefaultFramerate(m_mapFramerateByProfileId);
	m_pBK->GetFpsArray(m_fps);
}

void CConfigData::SetNodeitem(const vector<NODEITEM>& vcNode)
{
	m_vcOldNode = vcNode; 
	m_vcModifyNode = vcNode;
	int nIdx=0 , nCount=vcNode.size();
	ec_Camera camObj;
	vector<Cam_Group_Cam> vcGroupCam;
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		camObj.clear();
		vcGroupCam.clear();
		m_pBK->GetCamera(vcNode[nIdx].cameraid, camObj);
		m_pBK->GetGroupCamera(vcNode[nIdx].cameraid, vcGroupCam, false);


		m_vcOldCamObj.push_back(camObj);
		if (vcGroupCam.size())
			m_vcOldGrpCam.push_back(vcGroupCam[0]);
	}

	m_vcModifyGrpCam = m_vcOldGrpCam;
	m_vcModifyCamObj = m_vcOldCamObj;
	m_pBK->GetGroup(m_pBK->GetTreeType(), m_vcGroup, false);
}

bool CConfigData::GetCGIInfo()
{
	USES_CONVERSION;
	NODEITEM node = {0};
	GetModifyNodeItem(node);
	CString strPort;
	strPort.Format(_T("%d"),node.nWebPort);
	CString strMac(_T(""));
	CJason jason;
	map<CString , CJason> mapJason;


	if (node.byDeviceType != IPCAMERA && node.byDeviceType != ONVIF)
	{
		m_pBK->TransMac2String(node.mac, strMac);
	}

	jason = m_pBK->GetNetwork(A2W(node.ip),strPort,A2W(node.szUser),A2W(node.szPass));
	if(jason.isError()) return false;
	mapJason[_T("network")] = jason;

	jason = m_pBK->GetStream(A2W(node.ip),strPort,A2W(node.szUser),A2W(node.szPass));
	if(jason.isError()) return false;
	mapJason[_T("stream")] = jason;

	if (node.byModelType == MODEL_1150)
	{
		jason = m_pBK->GetStream_Info(A2W(node.ip),strPort,A2W(node.szUser),A2W(node.szPass));
		if(jason.isError()) return false;
		mapJason[_T("stream_info")] = jason;
	}
	else if (node.byModelType == MODEL_1180)
	{
		InitialDefaultValue();
		jason = m_pBK->GetVideo(A2W(node.ip),strPort,A2W(node.szUser),A2W(node.szPass));
		if(jason.isError()) return false;
		mapJason[_T("video")] = jason;

		jason = m_pBK->GetMcp_Video(A2W(node.ip),strPort,A2W(node.szUser),A2W(node.szPass));
		if(jason.isError()) return false;
		mapJason[_T("mcp_video")] = jason;

		jason = m_pBK->GetVideo_Info(A2W(node.ip),strPort,A2W(node.szUser),A2W(node.szPass));
		if(jason.isError()) return false;
		mapJason[_T("video_info")] = jason;

		jason = m_pBK->GetSensor(A2W(node.ip),strPort,A2W(node.szUser),A2W(node.szPass));
		if(jason.isError()) return false;
		mapJason[_T("sensor")] = jason;

		jason = m_pBK->GetCGI(_T("mcp.cgi?mcp=TvOutput"), A2W(node.ip),strPort,A2W(node.szUser),A2W(node.szPass));
		if(jason.isError()) return false;
		mapJason[_T("mcp_tvout")] = jason;

	}

	m_vcmapJason.push_back(mapJason);
	m_isCGIReady = true;

	return true;
}

void CConfigData::SaveAllData()
{
	ec_Camera camObj;
	Cam_Group_Cam grpCam;
	GetModifyCamObj(camObj);
	GetModifyGrpCam(grpCam);

	m_pBK->UpdateCamera(camObj,true);
	m_pBK->UpdateGroupCamera(grpCam);

	m_pBK->NotifyRecordSvr();
}

UINT CConfigData::GetCGIInfoThread (LPVOID lpParam)
{
	CConfigData *pthis = (CConfigData*)lpParam;
	pthis->GetCGIInfo();
	
	if (pthis->m_isCGIReady && pthis->m_callbackParam) 
	{
		pthis->m_UpdateCgiFun(pthis->m_callbackParam);
	}
 	else
 	{
// 		CString csMsg;
// 		LOADSTRING(csMsg, IDS_ADD_CAM_NOT_FOUND);
// 		AfxMessageBox(csMsg);
 	}
	return 0;
}

void CConfigData::CloseCgiRequest()
{
	if (g_hCGIHandle)
	{
		::CloseHandle(g_hCGIHandle);
		g_hCGIHandle = NULL;
	}
	
	if (g_hCGIHandle)
	{
		TerminateThread(g_hCGIHandle, 0);
		g_hCGIHandle = NULL;
	}
}