// AddDevice.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "AddDevice.h"
#include "DlgScanNetworkCameras.h"
#include "ConfigSettingDlg.h"
#include "DlgScanRemoteCam.h"


#define  STATIC_CONNECT		_T("Static IP")
#define  DHCP_CONNECT		_T("DHCP")
#define  PPPoE_CONNECT		_T("PPPoE")
const UINT  MsgFinishAddDevice = RegisterWindowMessage(_T("MsgFinishAddDevice") );

static CString m_strIPPort;
static CString m_strURLPort;
static DWORD m_ipAddress;
static CString m_strURL;
static CString m_strUserName;

// CAddDevice dialog
BOOL g_ThreadExecute = false;

IMPLEMENT_DYNAMIC(CAddDevice, CDialog)

CAddDevice::CAddDevice(int nSelectGrpId, CWnd* pParent /*=NULL*/)
	: CDialog(CAddDevice::IDD, pParent),m_vcNodes(),
	m_DeviceType(ERROR_TYPE),m_isStation(FALSE),m_nSelectGroupId(nSelectGrpId)
	, m_bIP(FALSE)
	, m_strPassword(_T(""))
	, m_pbk(NULL), m_isExist(false),m_isAddSuccess(false)
{

}

CAddDevice::~CAddDevice()
{
}

void CAddDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IP_ADDRESS, m_ipAddress);
	DDX_Text(pDX, IDC_URL_EDIT, m_strURL);
	DDX_Radio(pDX, IDC_IP_RADIO, m_bIP);
	DDX_Text(pDX, IDC_IPPORT_EDIT, m_strIPPort);
	DDX_Text(pDX, IDC_URLPORT_EDIT, m_strURLPort);
	DDX_Text(pDX, IDC_USERNAME_EDIT, m_strUserName);
	DDX_Text(pDX, IDC_PASSWORD_EDIT, m_strPassword);
}


BEGIN_MESSAGE_MAP(CAddDevice, CDialog)
	ON_BN_CLICKED(IDC_SCAN_BUTTON, &CAddDevice::OnBnClickedScanButton)
	ON_BN_CLICKED(IDC_IP_RADIO, &CAddDevice::OnBnClickedIpRadio)
	ON_BN_CLICKED(IDC_URL_RADIO, &CAddDevice::OnBnClickedUrlRadio)
	ON_BN_CLICKED(IDOK, &CAddDevice::OnBnClickedAddButton)
	ON_BN_CLICKED(IDCANCEL, &CAddDevice::OnBnClickedCancel)
	ON_REGISTERED_MESSAGE(MsgFinishAddDevice, OnFinishAddDevice)
END_MESSAGE_MAP()


// CAddDevice message handlers

void CAddDevice::OnBnClickedScanButton()
{
	// TODO: Add your control notification handler code here
	CDlgScanNetworkCameras DlgScan(((CButton*)GetDlgItem(IDC_CHECK_ONVIF))->GetCheck());
	if (DlgScan.DoModal() == IDOK)
	{
		if(DlgScan.GetCheckNodeItem() == NULL)
			return;

		m_isStation = FALSE;
		m_vcNodes.clear();
        NODEITEM node = *DlgScan.GetCheckNodeItem();
		m_vcNodes.push_back(node);

		m_pbk->GetEcParam(_T("MASTERUSER"), m_strUserName);
		UpdateUI();
	}
}

void CAddDevice::UpdateUI() 
{
	USES_CONVERSION;
	NODEITEM node = {0};
	if (!m_isStation && m_vcNodes.size())
		node = m_vcNodes[0];

	m_ipAddress = m_pbk->dwIpStringToDword(A2W(node.ip),true);
	
	if (m_bIP == FALSE)
	{
		m_strIPPort.Format(_T("%d"),node.nWebPort);
	}
	else
	{
		m_strURLPort.Format(_T("%d"),node.nWebPort);
	}

	m_strPassword.Empty();
	UpdateData(false);
}

void CAddDevice::GetKeyinAddress(CString& strAddress, CString& strPort)
{
	if (m_bIP == FALSE)
	{
		strAddress = m_pbk->strIpDwordToString(m_ipAddress);
		strPort = m_strIPPort;
	}
	else
	{
		strAddress = m_strURL; 
		strPort = m_strURLPort;
	}
}

bool CAddDevice::IsStation(CString& strBuffer)
{
	CString strAddress, strPort;
	bool bResult = false;
	GetKeyinAddress(strAddress, strPort);
	bResult = m_pbk->isStation(strBuffer, strAddress, strPort, m_strUserName, m_strPassword);
	return bResult;
}

bool CAddDevice::AddStation(const CString& strBuffer)
{
	bool bAddSelf = false;
	bool bResult = false;
	vector<NODEITEM> vcNodes;
	CString strAddress, strPort;
	GetKeyinAddress(strAddress, strPort);
	CDlgScanRemoteCam dlg(strAddress, strPort, m_strUserName, m_strPassword, m_nSelectGroupId);
	dlg.SetXMLBuffer(strBuffer);
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetChannels(vcNodes);
		m_vcNodes = vcNodes;
		bResult = true;
	}
	return bResult;
}

void CAddDevice::OnBnClickedAddButton()
{
	// TODO: Add your control notification handler code here
	if (!g_ThreadExecute)
	{
		UpdateData(true);
		::AfxBeginThread(AddDeviceThread, (LPVOID)this);
        GetDlgItem(IDOK)->EnableWindow(false);
	}
	else
	{
		DisplayError(SERCHING_DEVICE);
	}
}

void CAddDevice::CallConfigDlg()
{
	if ((m_isAddSuccess || m_isStation) && 
		!m_isExist)
	{
		if (!m_isStation)
		{
			CConfigSettingDlg dlg(m_vcNodes);
			dlg.DoModal();
			if (m_isExist && m_vcNodes.size())
			{
				m_pbk->SendMessage(MsgUpdateView,UM_UPDATE_DEVICE,m_vcNodes[0].cameraid);

			}
		}
		OnOK();
	}
}

void CAddDevice::GetCameraName(CString& strName)
{
	USES_CONVERSION;

	if(!m_isStation)
	{
		m_mapJason[_T("info")].GetValuebyKey(_T("model"),strName);
		if (strName.IsEmpty())
		{
			if (m_vcNodes.size())
			{
				strName = A2W(m_vcNodes[0].modelname);
			}
		}
	}
}

void CAddDevice::GetConnectTypeCode(CString& strConnectTypeCode)
{
	CString str;
	m_mapJason[_T("network")].GetValuebyKey(_T("dhcp"),str);

	if (str.Find(_T("on")) != -1)
	{
		strConnectTypeCode = DHCP_CONNECT;
	}
	else
	{
		strConnectTypeCode = STATIC_CONNECT;
	}
}

void CAddDevice::GetBrandCode(CString& strBrand_code)
{
	USES_CONVERSION;

	NODEITEM node = {0};
	if(!m_isStation && m_vcNodes.size())
	{
		node = m_vcNodes[0];
		strBrand_code = A2W(node.sn);
	}
	else
	{
		if (m_DeviceType == Model_1180)
		{
			CString str;
			m_mapJason[_T("info")].GetValuebyKey(_T("sn"),strBrand_code);
		}
	}
}

void CAddDevice::GetDigitalStatus(CJason& jason, const CString& strKey, CString& strValue)
{
	jason.GetValuebyKey(strKey,strValue);
	if(strValue.Find(_T("on")) != -1)	
		strValue = _T("Y");
	else
		strValue = _T("N");
}

void CAddDevice::GetVideoFormat(CJason& jason, CString& video_format)
{
	CString strFish;
	CString strLens;

	byte byFirst = 0;
	byte byThide = m_DeviceType;

	jason.GetValuebyKey(_T("System"), _T("mcp_system_fisheye"), strFish);
	jason.GetValuebyKey(_T("Lens"), _T("mcp_lens_option"), strLens);

	if (strFish.Find(_T("yes")) != -1 )
	{
		byFirst += FISHEYE;
	}
	if (strLens.Find(_T("26")) != -1)
	{
		byFirst += PANORAMA_180;
	}
	if (strLens.Find(_T("25")) != -1)
	{
		byFirst += PANORAMA_360;
	}

	DWORD dwVideoFormat = m_pbk->PackVideoFormat(0,byThide,0,byFirst);
	video_format.Format(_T("%d"),dwVideoFormat);
}

void CAddDevice::GetNetMask(CJason& jason, ec_Camera& camObj)
{
	CString csNetmask;
	jason.GetValuebyKey(_T("netmask"),csNetmask);
	LPCTSTR lpszFullString = csNetmask.GetBuffer();
	AfxExtractSubString(camObj.subnet_mask1, lpszFullString, 0, '.');
	AfxExtractSubString(camObj.subnet_mask2, lpszFullString, 1, '.');
	AfxExtractSubString(camObj.subnet_mask3, lpszFullString, 2, '.');
	AfxExtractSubString(camObj.subnet_mask4, lpszFullString, 3, '.');
	csNetmask.ReleaseBuffer();
}

void CAddDevice::GetPTZSupport(CString& ptz_support)
{
	if (m_DeviceType == Model_1150)
	{
		CString str;
		m_mapJason[_T("rs485")].GetValuebyKey(_T("enable"), str);
		if (str == _T("yes"))
			ptz_support = _T("Y");
		else
			ptz_support = _T("N");
	}
	else if (m_DeviceType == Model_1180)
	{
		CString str1, str2;
		m_mapJason[_T("mcp")].GetValuebyKey(_T("System"),_T("mcp_system_speeddome"), str1);
		m_mapJason[_T("mcp")].GetValuebyKey(_T("Lens"),_T("mcp_lens_motorized"), str2);

		if (str1.Find(_T("yes")) != -1 || str2.Find(_T("yes")) != -1)
			ptz_support = _T("Y");
		else
			ptz_support = _T("N");
	}
}

void CAddDevice::GetStreamUrl(CString& stream_url)
{
	if (m_DeviceType == Model_1150)
	{
		//m_mapJason[_T("stream")].GetValuebyKey(_T("stream_url"), stream_url);

	}
	else if (m_DeviceType == Model_1180)
	{
		m_mapJason[_T("stream")].GetValuebyKey(_T("stream_name"), stream_url);
	}
}

void CAddDevice::GetCameraObj(ec_Camera& camObj)
{
	USES_CONVERSION;
	CString strAddress, strPort, str;
	GetKeyinAddress(strAddress, strPort);
	
	camObj.camera_idx = m_pbk->GetLastCameraIdx() + 1;
	camObj.ipaddress = strAddress;
	camObj.httpport = _ttoi(strPort);
	camObj.username = m_strUserName;
	camObj.password = m_strPassword;
	camObj.active_ = _T("Y");
	m_mapJason[_T("network")].GetValuebyKey(_T("gateway"), camObj.gateway);
	m_mapJason[_T("network")].GetValuebyKey(_T("dns1"), camObj.name_server);
	m_mapJason[_T("info")].GetValuebyKey(_T("macaddr"), camObj.mac_address);
	camObj.mac_address.Replace(_T(":"),_T(""));
	m_mapJason[_T("rs485")].GetValuebyKey(_T("proto"),camObj.ptz_protocol);
	m_mapJason[_T("info")].GetValuebyKey(_T("model"),camObj.model_code);
	
	GetCameraName(camObj.cameraname);
	GetConnectTypeCode(camObj.connect_type_code);
	GetBrandCode(camObj.brand_code);
	GetDigitalStatus(m_mapJason[_T("io")], _T("in1"), camObj.digital_in1);
	GetDigitalStatus(m_mapJason[_T("io")], _T("in2"), camObj.digital_in2);
	GetDigitalStatus(m_mapJason[_T("io")], _T("out1"), camObj.digital_out);
	GetVideoFormat(m_mapJason[_T("mcp")], camObj.video_format);
	GetNetMask(m_mapJason[_T("network")], camObj);
	GetPTZSupport(camObj.ptz_support);
	GetStreamUrl(camObj.stream_url);

	if (m_isONVIF)
	{
		DWORD dwVideoFormat = m_pbk->PackVideoFormat(0,0,ONVIF,0);
		NODEITEM node = m_vcNodes[0];
		m_pbk->TransMac2String(node.mac,camObj.mac_address);
		camObj.stream_url.Format(_T("%s"),node.stream_url);
		camObj.video_format.Format(_T("%d"),dwVideoFormat);
		camObj.model_code = A2W(node.modelname);
	}
}

void CAddDevice::GetResAndCMPR(int nStreamIdx, CString& video_res, CString& video_cmpr_type)
{
	CString str(_T(""));
	if (m_DeviceType == Model_1180)
	{
		m_pbk->GetResAndCMPR(m_mapJason[_T("video")], nStreamIdx, video_res, video_cmpr_type);
	} 
	else
	{
		vector<CString> vcStr;
		m_mapJason[_T("stream_info")].GetValuebyKey(_T("resolutions"), str);
		m_mapJason[_T("stream_info")].PaserValue(str, vcStr);
		if (nStreamIdx < vcStr.size())
		{
			video_res = m_pbk->GetResolutionDesc(vcStr[nStreamIdx]);
		}
		str.Format(_T("vprofile%d"), nStreamIdx+1);
		m_mapJason[_T("stream_info")].GetValuebyKey(str, video_cmpr_type);
		if(video_cmpr_type == _T(""))
			video_cmpr_type = _T("Unknown");
	}
}

void CAddDevice::GetStreamProtocol(int nStreamIdx, CString& stream_protocol)
{
	if (m_DeviceType == Model_1180)
	{
		CString strAddress, strPort;
		GetKeyinAddress(strAddress, strPort);
		bool bAddSelf;
		bool bResult = m_pbk->isLocalLan(strAddress, bAddSelf);

		if (bResult)
			stream_protocol = _T("RTSP");
		else
			stream_protocol = _T("RTPTCP");
	} 
	else
	{
		CString str,strValue;
		str.Format(_T("vprofileproto%d"),nStreamIdx+1);
		m_mapJason[_T("stream_info")].GetValuebyKey(str,strValue);

		if (strValue.Find(_T("RTP")) != -1)
		{
			stream_protocol = _T("RTSP");
		}
		else if (strValue.Find(_T("VSM")) != -1)
		{
			stream_protocol = _T("TCP");
		}
	}
}

void CAddDevice::GetStreamPort(int nStreamIdx, short& stream_port)
{
	CString str;
	if (m_DeviceType == Model_1180)
	{
		m_mapJason[_T("stream")].GetValuebyKey(_T("rtsp_port"),str);
		stream_port = _ttoi(str);
	} 
	else
	{
		CString str,strValue;
		str.Format(_T("video_profile%d"),nStreamIdx+1);
		m_mapJason[str].GetValuebyKey(_T("port"),strValue);
		stream_port = _ttoi(strValue);
	}
}

void CAddDevice::GetVideoStream(ec_Camera& camObj)
{
	CString str;
	int nCount = 0;
	vector<ec_Stream> vcStream;

	if (m_isONVIF)
	{
		nCount = 1;
	}
	else
	{
		if (m_DeviceType == Model_1150)
		{
			m_mapJason[_T("stream_info")].GetValuebyKey(_T("vprofilenum"), str);
			nCount = _ttoi(str);
		}
		else
		{
			nCount = m_mapJason[_T("video")].GetCountByKey(_T("profile_id"));
		}
	}
	
	ec_Stream streamObj;
	for(int nIdx = 0; nIdx < nCount; nIdx++)
	{
		streamObj.clear();
		if(nIdx == 0)
			streamObj.stream_type = _T("RE");

		streamObj.stream_name.Format(_T("Stream %d"), nIdx+1);
		GetResAndCMPR(nIdx, streamObj.video_res, streamObj.video_cmpr_type);
		GetStreamProtocol(nIdx, streamObj.stream_protocol);
		GetStreamPort(nIdx, streamObj.stream_port);
		
		if (m_isONVIF)
		{
			streamObj.stream_protocol = _T("RTSP");
			GetONVIFStreamPort(camObj.stream_url,streamObj.stream_port);
		}

		vcStream.push_back(streamObj);
	}
	camObj.vcStream = vcStream;
}

bool CAddDevice::isExistDevice(ec_Camera& camObj)
{
	bool bResult = false;

	vector<ec_Camera> vccamObj;
	m_pbk->GetCamera(camObj.mac_address,vccamObj);

	int nIdx = 0, nCount = vccamObj.size();
	for(int nIdx = 0; nIdx < nCount ; nIdx++)
	{
		if (m_pbk->GetDevice(vccamObj[nIdx].video_format) == IPCAMERA || 
			m_pbk->GetDevice(vccamObj[nIdx].video_format) == ONVIF)
		{
			bResult = true;
			camObj = vccamObj[nIdx];
			int nReturn = AfxMessageBox(_T("This device already exist!"));
			m_isExist = true;
			break;
		}
	}

	return bResult;
}

bool CAddDevice::GetCGI()
{
	if (m_isONVIF)
		return true;

	CString strAddress, strPort;
	GetKeyinAddress(strAddress, strPort);
	if (m_DeviceType == Model_1180)
	{
		CJason jStream = m_pbk->GetStream(strAddress, strPort, m_strUserName, m_strPassword);
		if(jStream.isError())
			return false;

		CJason jRs485 = m_pbk->GetRS485(strAddress, strPort, m_strUserName, m_strPassword);
		if(jRs485.isError())
			return false;

		CJason jMcp = m_pbk->GetMcp(strAddress, strPort, m_strUserName, m_strPassword);
		if(jMcp.isError())
			return false;

		CJason jIo = m_pbk->GetIO(strAddress, strPort, m_strUserName, m_strPassword);
		if(jIo.isError())
			return false;

		CJason jNetwork = m_pbk->GetNetwork(strAddress, strPort, m_strUserName, m_strPassword);
		if(jNetwork.isError())
			return false;

		CJason jInfo = m_pbk->GetInfo(strAddress, strPort, m_strUserName, m_strPassword);
		if(jInfo.isError())
			return false;

		CJason jVideo = m_pbk->GetVideo(strAddress, strPort, m_strUserName, m_strPassword);
		if(jVideo.isError())
			return false;

		CJason jAudio = m_pbk->GetAudio(strAddress, strPort, m_strUserName, m_strPassword);
		if(jAudio.isError())
			return false;

		m_mapJason[_T("stream")] = jStream;
		m_mapJason[_T("rs485")] = jRs485;
		m_mapJason[_T("mcp")] = jMcp;
		m_mapJason[_T("io")] = jIo;
		m_mapJason[_T("network")] = jNetwork;
		m_mapJason[_T("info")] = jInfo;
		m_mapJason[_T("video")] = jVideo;
		m_mapJason[_T("audio")] = jAudio;
	} 
	else if (m_DeviceType == Model_1150)
	{
		CJason jStream_Info = m_pbk->GetStream_Info(strAddress, strPort, m_strUserName, m_strPassword);
		if(jStream_Info.isError())
			return false;

		CJason jVideo = m_pbk->GetVideo(strAddress, strPort, m_strUserName, m_strPassword);
		if(jVideo.isError())
			return false;

		/*CJason jStream = m_pbk->GetStream(strAddress, strPort, m_strUserName, m_strPassword);
		if(jStream.isError())
			return false;*/

		CJason jRs485 = m_pbk->GetRS485(strAddress, strPort, m_strUserName, m_strPassword);
		if(jRs485.isError())
			return false;

		CJason jIo = m_pbk->GetIO(strAddress, strPort, m_strUserName, m_strPassword);
		if(jIo.isError())
			return false;

		CJason jNetwork = m_pbk->GetNetwork(strAddress, strPort, m_strUserName, m_strPassword);
		if(jNetwork.isError())
			return false;

		CJason jInfo = m_pbk->GetCommonInfo(strAddress, strPort, m_strUserName, m_strPassword);
		if(jInfo.isError())
			return false;

		CString str;
		CJason jprofile;
		jStream_Info.GetValuebyKey(_T("vprofilenum"),str);
		int nCount = _ttoi(str);

		for (int nidx = 0; nidx < nCount; nidx++)
		{
			jprofile = m_pbk->GetVideo_Profile(nidx+1, strAddress, strPort, m_strUserName, m_strPassword);
			if(jprofile.isError())
				return false;
			else
			{
				str.Format(_T("video_profile%d"),nidx+1);
				m_mapJason[str] = jprofile;
			}
		}

		m_mapJason[_T("stream_info")] = jStream_Info;
		m_mapJason[_T("video")] = jVideo;
		//m_mapJason[_T("stream")] = jStream;
		m_mapJason[_T("rs485")] = jRs485;
		m_mapJason[_T("io")] = jIo;
		m_mapJason[_T("network")] = jNetwork;
		m_mapJason[_T("info")] = jInfo;
	}

	return true;
}

bool CAddDevice::isONVIF()
{	
	CString str;
	bool bResult = false;
	if (m_vcNodes.size() && !m_isStation)
	{
		if (m_vcNodes[0].byDeviceType == ONVIF)
		{
			str = m_pbk->get_ONVIF_RTSP(m_vcNodes[0].stream_url, m_strUserName, m_strPassword);
			GetONVIFStreamURL(str, m_vcNodes[0].stream_url);
			bResult = true;
		}
	}
	else
	{
		if (((CButton*)GetDlgItem(IDC_CHECK_ONVIF))->GetCheck())
		{
			bResult = CheckONVIF();
		}
	}
	return bResult;
}

bool CAddDevice::CheckONVIF()
{
	USES_CONVERSION;
	bool bResult = false;
	vector<NODEITEM> vcONVIFnode;
	vector<NODEITEM*> vcENCPnode;
	m_pbk->GetONVIFNodes(vcONVIFnode);
	vcENCPnode = CIPCamDiscovery::GetNodePtr();
	vector<CString> vcStrIP;
	vector<CString>::iterator it;
	CString str;
	int nIdx = 0, nCount = vcONVIFnode.size();
	for(nIdx = 0; nIdx < nCount; nIdx++)
	{
		str.Empty();
		str = A2W(vcONVIFnode[nIdx].ip);
		vcStrIP.push_back(str);
	}

	nCount = vcENCPnode.size();
	for(nIdx = 0; nIdx < nCount; nIdx++)
	{
		str.Empty();
		str = A2W(vcENCPnode[nIdx]->ip);
		it = find(vcStrIP.begin(),vcStrIP.end(),str);
		if (it != vcStrIP.end())
		{
			int nIdx = it-vcStrIP.begin();
			vcStrIP.erase(it);
			vcONVIFnode.erase(vcONVIFnode.begin()+nIdx);
		}
	}

	CString strAddress, strPort;
	GetKeyinAddress(strAddress, strPort);

	nCount = vcONVIFnode.size();
	for(nIdx = 0; nIdx < nCount; nIdx++)
	{
		str.Empty();
		str = A2W(vcONVIFnode[nIdx].ip);
		if (str == strAddress)
		{
			str = m_pbk->get_ONVIF_RTSP(vcONVIFnode[nIdx].stream_url, m_strUserName, m_strPassword);
			GetONVIFStreamURL(str, vcONVIFnode[nIdx].stream_url);
			m_vcNodes.push_back(vcONVIFnode[nIdx]);
			bResult = true;
			break;
		}
	}
	return bResult;
}

void CAddDevice::GetONVIFStreamPort(const CString& str, short& nPort)
{
	CString cs_rtsp = str;
	wchar_t *wstr = NULL;
	wchar_t str_rtsp[128];
	wcscpy(str_rtsp, cs_rtsp.GetBuffer());
	wstr = wcstok(str_rtsp,_T(":"));
	wstr = wcstok(NULL,_T(":"));
	if(wstr)
	{
		wstr = wcstok(NULL,_T("/"));
	}

	if (wstr)
		nPort = _ttoi(wstr);
	else
		nPort = 554;
	cs_rtsp.ReleaseBuffer();
}

void CAddDevice::AddDevice()
{
	ec_Camera camObj;
	CString strAddress, strPort;
	CString str;
	bool isCGIReady = false;
	
	GetKeyinAddress(strAddress, strPort);

	m_DeviceType = m_pbk->GetDeviceModelType(strAddress, strPort, m_strUserName, m_strPassword);
	m_isONVIF = isONVIF();
	if (m_isONVIF)
	{
		m_DeviceType = UN_KNOW;
	}
	isCGIReady = GetCGI();
	GetCameraObj(camObj);
	GetVideoStream(camObj);

	if (m_DeviceType != ERROR_TYPE && isCGIReady)
	{
		m_isAddSuccess = true;
	}
	if (m_isONVIF)
	{
		if (camObj.stream_url.IsEmpty())
		{
			m_isAddSuccess = false;
		}
	}
	InsertDevice2DB(camObj);
}

void CAddDevice::InsertDevice2DB(ec_Camera& camObj)
{
	if(m_isAddSuccess)
	{
		vector<Cam_Group> vcGroup;
		Cam_Group_Cam groupCamObj;
		if (!isExistDevice(camObj))
		{
			m_pbk->GetGroup(DeviceTree, vcGroup);
			if (vcGroup.size())
			{
				ec_Camera_Rec recObj;
				recObj.camera_idx = camObj.camera_idx;
				recObj.recording_type = _T("0");
				m_pbk->InsertCamRec(recObj);

				m_pbk->InsertCamera(camObj);
				camObj.cameraid = m_pbk->GetLastCameraId();

				if(!m_nSelectGroupId)
					m_nSelectGroupId = vcGroup[0].cameragroupid;

				groupCamObj.cameragroupid = m_nSelectGroupId;

				groupCamObj.cameraid = m_pbk->GetLastCameraId();
				groupCamObj.camera_idx = m_pbk->GetLastCameraIdx();
				groupCamObj.numb_ = 0;
				groupCamObj.streamid = 1;
				m_pbk->InsertGroupCamera(groupCamObj);
			}
		}
		else
		{
			vector<Cam_Group_Cam> vcGrpCams;
			m_pbk->GetGroupCamera(camObj.cameraid,vcGrpCams,false);
			m_nSelectGroupId = vcGrpCams[0].cameragroupid;
		}
		NODEITEM node = {0};
		Trans_Camera2NODEITEM(camObj, node);
		node.nGroup = m_nSelectGroupId;
		m_vcNodes.clear();
		m_vcNodes.push_back(node);
	}
	else
	{
		DisplayError();
	}
}

void CAddDevice::DisplayError(ErrorMsg msg)
{
	CString csMsg;
	switch(msg)
	{
	case NOT_FOUND_DEVICE:
		{
			LOADSTRING(csMsg, IDS_ADD_CAM_NOT_FOUND);
		}
		break;
	case SERCHING_DEVICE:
		{
			csMsg = _T("Serching device, please wait!");
		}
		break;
	}

	AfxMessageBox(csMsg);
}

void CAddDevice::OnBnClickedIpRadio()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	UpdateRadio();
}

void CAddDevice::OnBnClickedUrlRadio()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	UpdateRadio();
}

void CAddDevice::UpdateRadio()
{
	if (m_bIP == FALSE)
	{
		GetDlgItem(IDC_URL_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_URLPORT_EDIT)->EnableWindow(FALSE);

		GetDlgItem(IDC_IPPORT_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_IP_ADDRESS)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_IPPORT_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IP_ADDRESS)->EnableWindow(FALSE);

		GetDlgItem(IDC_URL_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_URLPORT_EDIT)->EnableWindow(TRUE);
	}
}
BOOL CAddDevice::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_pbk = CNVRBk::GetBkObject();

	UpdateData(false);
	if (m_ipAddress==0)
		((CIPAddressCtrl*)GetDlgItem(IDC_IP_ADDRESS))->SetWindowText(_T(""));

	if(!m_pbk->isNVRType())
		GetDlgItem(IDC_CHECK_ONVIF)->ShowWindow(SW_HIDE);

	UpdateRadio();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAddDevice::Trans_Camera2NODEITEM(const ec_Camera& camObj, NODEITEM& node)
{
	USES_CONVERSION;
	CString str;

	str = camObj.stream_url;
	_tcscpy(node.stream_url, str.GetBuffer());	
	node.nStreamID = 1;
	node.cameraid = camObj.cameraid;
	node.camera_idx = camObj.camera_idx;

	str = camObj.cameraname;
	strncpy(node.camera_name , W2A(str), camObj.cameraname.GetLength());
	_tcscpy(node.wcAlias, str.GetBuffer(32));	

	str = camObj.ipaddress;
	strncpy(node.ip , W2A(str), camObj.ipaddress.GetLength());
	m_pbk->TransString2Mac(camObj.mac_address, node.mac);
	node.nWebPort = camObj.httpport;

	str = camObj.username;
	strncpy(node.szUser, W2A(str), camObj.username.GetLength());

	str = camObj.password;
	strncpy(node.szPass, W2A(str), camObj.password.GetLength());
	node.byDeviceType = m_pbk->GetDevice(camObj.video_format);
	node.byModelType = m_pbk->GetModelType(camObj.video_format);
// 	vDispatchvideo_format2NodeItem(camObj.video_format, &node);
// 	node.nENVR = GetNVRBrand(node);

	str = camObj.model_code;
	strcpy(node.modelname, W2A(str));
	if (camObj.vcStream.size())
	{
		node.nAPPort = camObj.vcStream[0].stream_port;
	}

	node.nGroup = m_nSelectGroupId;
	node.nProtocol = m_pbk->GetStreamProtocol(camObj,0);
}

void CAddDevice::GetONVIFStreamURL(const CString& strRTSP, TCHAR *tzPath)
{
	wmemset(tzPath, 0x00, 64);
	CString str = strRTSP, strURL;
	int nIdx = str.ReverseFind(_T('/'))+1;
	strURL = str.Right(str.GetLength()-nIdx);
// 	nIdx = strURL.Find(_T("."));
// 	strURL = strURL.Left(nIdx);
	wcscpy(tzPath,strURL.GetBuffer());
	strURL.ReleaseBuffer();
}

UINT CAddDevice::AddDeviceThread (LPVOID lpParam)
{
	CAddDevice *pthis = (CAddDevice*)lpParam;
	
	g_ThreadExecute = true;
	if (pthis->m_pbk->isNVRType())
	{
		pthis->AddDevice();
	}
	else
	{
		CString strBuffer;
		if (pthis->IsStation(strBuffer))
		{
			if (!pthis->AddStation(strBuffer))
			{
				//return 0;
			}
			else
			{
				pthis->m_isStation = TRUE;
			}
		}
		else
		{
			pthis->AddDevice();
		}
	}

	g_ThreadExecute = false;
	pthis->SendMessage(MsgFinishAddDevice,NULL,NULL);
	return 0;
}

void CAddDevice::OnBnClickedCancel()
{
 	if (g_ThreadExecute)
 	{
		DisplayError(SERCHING_DEVICE);
		return;
 	}
 	else
 	{
 		OnCancel();
 	}
}

LRESULT CAddDevice::OnFinishAddDevice(WPARAM,LPARAM)
{
    GetDlgItem(IDOK)->EnableWindow(true);
	CallConfigDlg();
	return 0;
}