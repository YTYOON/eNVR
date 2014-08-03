// DlgScanRemoteCam.cpp : implementation file
//

#include "stdafx.h"
#include "DlgScanRemoteCam.h"
#include "tinystr.h"
#include "tinyxml.h"

// CDlgScanRemoteCam dialog

IMPLEMENT_DYNAMIC(CDlgScanRemoteCam, CDialog)

CDlgScanRemoteCam::CDlgScanRemoteCam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgScanRemoteCam::IDD, pParent), iInitFlag(0), m_bSameLocalLan(false),m_pBk(NULL),
	m_nDeviceID(0),m_nCameraID(0),m_strXmlBuffer(_T("")),m_vcNodes()
{

}

CDlgScanRemoteCam::CDlgScanRemoteCam(CString& strIP,  CString& strPort,  CString& strUser, CString& strPass,  
									 int nSelectGroupId, CWnd* pParent)

: CDialog(CDlgScanRemoteCam::IDD, pParent),iInitFlag(0),m_bSameLocalLan(false),m_pBk(NULL),m_nDeviceID(0),
m_nCameraID(0),m_strXmlBuffer(_T("")),m_vcNodes(),m_iGroupID(nSelectGroupId),
m_csIP(strIP),m_csUser(strUser),m_csPass(strPass),m_csPort(strPort)
{
	strIP.ReleaseBuffer();
	strUser.ReleaseBuffer();
	strPass.ReleaseBuffer();
}

CDlgScanRemoteCam::~CDlgScanRemoteCam()
{
}

void CDlgScanRemoteCam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgScanRemoteCam, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgScanRemoteCam::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDlgScanRemoteCam::CCheckHeadCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CCheckHeadCtrl)
	ON_NOTIFY_REFLECT(HDN_ITEMCLICK, OnItemClicked)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CDlgScanRemoteCam message handlers
void CDlgScanRemoteCam::GetStringByLanguage(CString& strName, CString& strIP, CString& strMac, CString& strModel, CString& strAllStream, CString& strURL, CString& strVideoFm, CString& strSupportPTZ)
{
	LOADSTRING(strName, IDS_ADD_CAM_NAME);	
	LOADSTRING(strIP, IDS_ADD_CAM_IP);	
	LOADSTRING(strMac, IDS_ADD_CAM_MAC);	
	LOADSTRING(strModel, IDS_ADD_CAM_MODEL);	
	LOADSTRING(strAllStream, IDS_TOTAL_STREAM);	
	LOADSTRING(strURL, IDS_STREAM_URL);	
	LOADSTRING(strVideoFm, IDS_VIDEO_FORMAT);	
	LOADSTRING(strSupportPTZ, IDS_PTZ_ENABLE);	
}

void CDlgScanRemoteCam::GetXmlBuffer(CString& csBuffer)
{
	if (m_strXmlBuffer.GetLength() > 0)
	{
		csBuffer = m_strXmlBuffer;
	}
	else
	{
		CString csURL;
		Curlplus CGIScanData;
		csURL.Format(_T("http://%s:%d/config/AddCamera.cgi"), m_csIP, m_csPort);
		csBuffer = CGIScanData.RemoteGet(csURL, m_csUser, m_csPass);
	}
}

void CDlgScanRemoteCam::InitialListCtrl(CListCtrl* pScanList, int iInitFlag)
{
	vector<DWORD> vcWidth;
	vector<CString> vcTitles;

	CString strName, strIP, strMac, strModel, strAllStream, strURL, strVideoFm, strSupportPTZ;
	GetStringByLanguage(strName, strIP, strMac, strModel, strAllStream, strURL, strVideoFm, strSupportPTZ);

	if(iInitFlag == NVR_Etrocenter)
	{
		vcTitles.push_back(_T(""));
		vcTitles.push_back(strName);
		vcTitles.push_back(strIP);
		vcTitles.push_back(strMac);
		vcTitles.push_back(strModel);
		vcTitles.push_back(strAllStream);
		vcTitles.push_back(strURL);
		vcTitles.push_back(strVideoFm);
		vcTitles.push_back(strSupportPTZ);
		vcWidth.push_back(50);
		vcWidth.push_back(90);
		vcWidth.push_back(90);
		vcWidth.push_back(90);
		vcWidth.push_back(90);
		vcWidth.push_back(90);
		vcWidth.push_back(90);
		vcWidth.push_back(0);
		vcWidth.push_back(0);
	}
	else if(iInitFlag == NVR_ISAP)
	{
		vcTitles.push_back(_T(""));
		vcTitles.push_back(strName);
		vcTitles.push_back(strIP);
		vcTitles.push_back(strMac);
		vcTitles.push_back(strModel);
		vcTitles.push_back(strAllStream);
		vcTitles.push_back(strURL);
		vcWidth.push_back(20);
		vcWidth.push_back(90);
		vcWidth.push_back(90);
		vcWidth.push_back(90);
		vcWidth.push_back(90);
		vcWidth.push_back(90);
		vcWidth.push_back(90);
	}
	LV_COLUMN ListColumn;
	ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	ListColumn.fmt = LVCFMT_LEFT;	

	int nCount = vcWidth.size();
	for(int i = 0; i < nCount; i++)
	{
		ListColumn.iSubItem = i;
		ListColumn.cx = vcWidth[i];
		ListColumn.pszText = (LPWSTR)(LPCTSTR)vcTitles[i];
		pScanList->InsertColumn(i, &ListColumn);
	}
}

void CDlgScanRemoteCam::VerifyCtrlStyle(CListCtrl* pScanList)
{
	DWORD dwStyle = pScanList->GetStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES;
	pScanList->SetExtendedStyle(dwStyle);

	//--Set List Control Image check box
	CHeaderCtrl* pHeadCtrl = pScanList->GetHeaderCtrl();
	ASSERT(pHeadCtrl->GetSafeHwnd());

	VERIFY( m_checkHeadCtrl.SubclassWindow(pHeadCtrl->GetSafeHwnd()) );
	VERIFY( m_checkImgList.Create(IDB_CHECKBOXES, 16, 3, RGB(255,0,255)) );
	int i = m_checkImgList.GetImageCount();
	m_checkHeadCtrl.SetImageList(&m_checkImgList);

	HDITEM hdItem;
	hdItem.mask = HDI_IMAGE | HDI_FORMAT;
	VERIFY( m_checkHeadCtrl.GetItem(0, &hdItem) );
	hdItem.iImage = 1;
	hdItem.fmt |= HDF_IMAGE;

	VERIFY( m_checkHeadCtrl.SetItem(0, &hdItem) );
}

bool CDlgScanRemoteCam::isErrorResponse(const CString& csBufferXml)
{
	USES_CONVERSION;
	bool bResult = false;
	CString csBuffer = csBufferXml;
	const char* pCheck = T2A(csBuffer.GetBuffer());

	TiXmlDocument xml_error;
	xml_error.Parse(pCheck);
	TiXmlElement* p_CheckRoot = xml_error.FirstChildElement();
	CString csRoot;
	csRoot.Format(_T("%s"), A2W(p_CheckRoot->Value()));

	//--Check if error or not.
	if( csRoot.Compare(_T("AddCameraInfo")) != 0)
	{
		TiXmlElement* p_CheckChild = p_CheckRoot->FirstChildElement();
		p_CheckChild = p_CheckChild ? p_CheckChild->FirstChildElement() : NULL;
		p_CheckChild = p_CheckChild ? p_CheckChild->FirstChildElement() : NULL;
		p_CheckChild = p_CheckChild ? p_CheckChild->NextSiblingElement() : NULL;
		if (!p_CheckChild) 
		{
			OnOK();
			return true;
		}
		CString csErrorMSG;
		csErrorMSG.Format(_T("%s"), A2W(p_CheckChild->GetText()) );

		if(csErrorMSG.Compare(_T("HTTP Error: 401 Unauthorized")) == 0);
		{
			CString csString;
			//csString.LoadString(CGS::g_hResStr, IDS_SCAN_REMOTE_ERROR_2);
			LOADSTRING(csString, IDS_SCAN_REMOTE_ERROR_2);
			AfxMessageBox(csString);
			OnOK();
			return true;
		}
	}

	return bResult;
}

BOOL CDlgScanRemoteCam::OnInitDialog()
{
	bool bAddSelf = false;
	CDialog::OnInitDialog();
	m_pBk = CNVRBk::GetBkObject();
	m_bSameLocalLan = m_pBk->isLocalLan(m_csIP, bAddSelf);
	if(!m_iGroupID)
	{
		m_iGroupID = m_pBk->GetFirstGroupIdx(m_pBk->GetTreeType());
	}

    ((CButton*)this->GetDlgItem(IDC_RADIO_UDP))->SetCheck(m_bSameLocalLan);
    ((CButton*)this->GetDlgItem(IDC_RADIO_TCP))->SetCheck(!m_bSameLocalLan);
	((CButton*)this->GetDlgItem(IDC_RADIO_HTTP))->SetCheck(false);

	USES_CONVERSION;

	pScanList = (CListCtrl *)this->GetDlgItem(IDC_LIST_REMOTE_CAM);
	InitialListCtrl(pScanList, iInitFlag);
	if(iInitFlag == NVR_Etrocenter)
	{
		VerifyCtrlStyle(pScanList);
		//--CURL Get Response
		CString csBuffer(_T(""));
		GetXmlBuffer(csBuffer);

		if(csBuffer.GetLength() == 0)
		{
			CString csString;
			LOADSTRING(csString, IDS_SCAN_REMOTE_ERROR_1);
			AfxMessageBox(csString);
			OnOK();
			return true;
		}

		if (!isErrorResponse(csBuffer))
		{
			const char* pSth = T2A(csBuffer.GetBuffer());

			//--XML Parse Start
			TiXmlDocument xmlDOC;
			xmlDOC.Parse(pSth);
			
			TiXmlElement* p_RootElement = xmlDOC.FirstChildElement();				//--Root Element
			TiXmlElement* p_TotalCamera = p_RootElement->FirstChildElement();		//--Total Camera Element
			const char* szTotalCamera = p_TotalCamera->Attribute("TotalCamera");
			int iTotalCamera = ::atoi(szTotalCamera);


			for(int iTemp = 0; iTemp < iTotalCamera; iTemp++)						//--Initial Camera to Stream vector
				veCamera2Stream.push_back(0);

			TiXmlElement* p_TotalStream = p_TotalCamera->NextSiblingElement();
			const char* szStreamPort = p_TotalStream->Attribute("stream_port");
			if(szStreamPort == NULL)
			{
				this->m_iStreamPort = 554;
			}
			else
			{
				this->m_iStreamPort = ::atoi(szStreamPort);
				if(this->m_iStreamPort == 0)	
					this->m_iStreamPort = 554;
			}
			
			const char* szHttpPort = p_TotalStream->Attribute("http_port");
			if(szHttpPort == NULL)
			{
				m_iHttpPort = 8080;
			}
			else
			{
				m_iHttpPort = ::atoi(szHttpPort);
				if(m_iHttpPort == 0)	
					m_iHttpPort = 8080;
			}
			

			TiXmlElement* p_StreamElement = p_TotalStream->FirstChildElement();
			TiXmlElement* p_CameraElement = p_TotalCamera->FirstChildElement();
			CString csR_Name,csR_IP,csR_MAC,csR_ModelName,csR_TotalStream,csR_StreamURL,csR_VideoFormat, csR_PTZSupport;

			for(int iTemp = 0; iTemp < iTotalCamera; iTemp++)								//--Insert item to Scan List
			{
				if (p_CameraElement)
				{
					csR_Name.Format(_T("%s"), A2W(p_CameraElement->Attribute("cameraname")) );
					csR_IP.Format(_T("%s"), A2W(p_CameraElement->Attribute("IPAddress")) );
					csR_MAC.Format(_T("%s"), A2W(p_CameraElement->Attribute("mac_address")) );
					csR_ModelName.Format(_T("%s"), A2W(p_CameraElement->Attribute("model_name")) );
					csR_TotalStream.Format(_T("%s"), A2W(p_CameraElement->Attribute("total_stream")) );
					csR_StreamURL.Format(_T("%s"), A2W(p_CameraElement->Attribute("stream_url")) );
					csR_VideoFormat = A2W(p_CameraElement->Attribute("video_format"));
					csR_PTZSupport = A2W(p_CameraElement->Attribute("ptz_support"));

					int iItem = pScanList->InsertItem(iTemp, _T(""), 0);
					pScanList->SetItemText(iItem, 1, csR_Name.GetBuffer() );
					pScanList->SetItemText(iItem, 2, csR_IP.GetBuffer() );
					pScanList->SetItemText(iItem, 3, csR_MAC.GetBuffer() );
					pScanList->SetItemText(iItem, 4, csR_ModelName.GetBuffer() );
					pScanList->SetItemText(iItem, 5, csR_TotalStream.GetBuffer() );

					if (csR_StreamURL.GetLength() == 0)
						pScanList->SetItemText(iItem, 6, csR_MAC.GetBuffer() );			
					else
						pScanList->SetItemText(iItem, 6, csR_StreamURL.GetBuffer() );

					pScanList->SetItemText(iItem, 7, csR_VideoFormat.GetBuffer() );

					if( isExistChannel(csR_MAC) )
					{
						pScanList->SetCheck(iItem);
					}

					pScanList->SetItemText(iItem, 8, csR_PTZSupport.GetBuffer() );
				}
				p_CameraElement = p_CameraElement->NextSiblingElement();
			}
			isExistStation();
		}
	}

	UpdateDatabase();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgScanRemoteCam::UpdateDatabase()
{
	int nIdx = 0, nCount = m_vcExistCam.size();

	if (nCount)
	{
		int nReturn = AfxMessageBox(_T("This station already exist.\nAre you sure to re-add it?"), MB_OKCANCEL);
		if (nReturn == IDOK)
		{
			for(int nIdx = 0; nIdx < nCount ;  nIdx++)
			{
				m_pBk->SendMessage(MsgUpdateView, UM_DELETE_STATION, m_vcExistCam[nIdx].cameraid);
				m_pBk->DeleteCamera(m_vcExistCam[nIdx].cameraid);
				m_pBk->DeleteGroupCamera(m_vcExistCam[nIdx].cameraid);
				m_pBk->DeleteCamRecTime(m_vcExistCam[nIdx].cameraid);
				m_pBk->DeleteCamRec(m_vcExistCam[nIdx].cameraid);
			}
		}
	}
}

bool CDlgScanRemoteCam::isExistStation()
{
	bool bResult = false;
	if (m_vcExistCam.size())
	{
		int nCameraid = 0;
		vector<Cam_Group_Cam> vcGroupCam;
		ec_Camera camObj = m_vcExistCam[m_vcExistCam.size()-1];
		
		m_pBk->GetGroupCamera(camObj.cameraid+1,vcGroupCam,false);
		if (vcGroupCam.size())
		{
			nCameraid = vcGroupCam[0].cameraid;
			camObj.clear();
			m_pBk->GetCamera(nCameraid,camObj);

			if (!camObj.isempty() && 
				(m_pBk->GetDevice(camObj.video_format) == eNVR || m_pBk->GetDevice(camObj.video_format) == IPVSTATION))
			{
				bResult = true;
				m_vcExistCam.push_back(camObj);
			}
		}
	}
	return bResult;
}

bool CDlgScanRemoteCam::isExistChannel(const CString& strMac)
{
	bool bResult = false;
	ec_Camera camObj;
	m_pBk->GetCamera(m_csIP, strMac, camObj);

	if (!camObj.isempty() && 
		(m_pBk->GetDevice(camObj.video_format) == eNVR || m_pBk->GetDevice(camObj.video_format) == IPVSTATION))
	{
		bResult = true;
		m_vcExistCam.push_back(camObj);
	}

	return bResult;
}

void CDlgScanRemoteCam::InsertRemoteDevice(int iRTSPPort)
{
	int nCount = pScanList->GetItemCount();
	if (nCount)
	{
		m_iDBExitCams++;
		InsertOneObject2DB(nCount-1, iRTSPPort, false);
	}
}

int CDlgScanRemoteCam::GetVideoFormat(int nVideoFormat, bool bIpvStation, bool isChannel)
{
	int nResult = 0;
	CString str;
	str.Format(_T("%d"),nVideoFormat);

	byte byFirst,bySecond,byThird,byFourth;
	m_pBk->UnpackVideoFormat(str,byFourth,byThird,bySecond,byFirst);
	if(isChannel)
		byFirst |= CHANNEL;
	
	if (bIpvStation)
		bySecond = IPVSTATION;
	else
		bySecond = eNVR;

	nResult = m_pBk->PackVideoFormat(byFourth,byThird,bySecond,byFirst);

	return nResult;
}

void CDlgScanRemoteCam::GetStream(bool bIpvStation, int iRTSPPort, int nStreamCount, vector<ec_Stream>& vcStreams)
{
	ec_Stream streamObj;
	int nCameraid = m_pBk->GetLastCameraId();

	for(int iCount = 1; iCount <= nStreamCount; iCount++)
	{
		streamObj.clear();

		streamObj.stream_name.Format(_T("Stream %d"),iCount); 				
		streamObj.stream_tag = _T("NVR");
		streamObj.cameraid = m_nCameraID;		
		streamObj.stream_port = iRTSPPort;
		streamObj.stream_protocol = m_csStreamProtocol;

		if(iCount == 1)
		{
			if (bIpvStation)
				streamObj.stream_type = _T("RE");
		}
		else
		{
			streamObj.stream_type = _T("");
		}

		vcStreams.push_back(streamObj);
	}
}

void CDlgScanRemoteCam::InsertCamera(int iTemp, int iRTSPPort, bool isChannel, bool bIpvStation)
{
	USES_CONVERSION;

	DWORD nVideoFormat = 0;
	if(isChannel)
		nVideoFormat =  ::_ttoi(pScanList->GetItemText(iTemp, 7));  
	
	ec_Camera cameraObj;
	CString strSationName;

	nVideoFormat = GetVideoFormat(nVideoFormat, bIpvStation, isChannel);

	cameraObj.camera_idx = m_iDBExitCams;						
	cameraObj.ipaddress = m_csIP;					
	cameraObj.httpport = atoi(W2A(m_csPort));
	if (isChannel)
	{
		cameraObj.mac_address = pScanList->GetItemText(iTemp, 3);
	}
	else
	{
		cameraObj.mac_address = pScanList->GetItemText(iTemp, 3);
		int nIdx = _ttoi(cameraObj.mac_address.Left(2)) + 1;
		cameraObj.mac_address.Format(_T("%02d%s"),nIdx,cameraObj.mac_address.Right(10));
	}
	cameraObj.username = m_csUser;
	cameraObj.password = m_csPass;	
	cameraObj.ptz_support = _T("N");
	cameraObj.active_ = _T("Y");
	cameraObj.stream_url = pScanList->GetItemText(iTemp, 6);
	cameraObj.video_format.Format(_T("%d"),nVideoFormat);
	
	if (bIpvStation)
	{
		strSationName = _T("IPVSTATION");
	}
	else
	{
		strSationName = _T("NVR");
	}

	if (isChannel)
	{
		cameraObj.model_code = pScanList->GetItemText(iTemp, 4);
		cameraObj.cameraname = pScanList->GetItemText(iTemp, 1);
	} 
	else
	{
		cameraObj.model_code.Format(_T("%s"),strSationName);
		cameraObj.cameraname.Format(_T("%s"),strSationName);
	}

	if(isChannel)
		GetStream(bIpvStation, iRTSPPort, ::_ttoi(pScanList->GetItemText(iTemp, 5)), cameraObj.vcStream);

	cameraObj.ptz_support =pScanList->GetItemText(iTemp, 8);
	m_pBk->InsertCamera(cameraObj);

	if (isChannel)
	{
		NODEITEM node = {0};
		Trans_Camera2NODEITEM(cameraObj, node);
		m_vcNodes.push_back(node);
	}
}

void CDlgScanRemoteCam::InsertGroupCamera()
{
	m_nCameraID = m_pBk->GetLastCameraId();

	Cam_Group_Cam GroupCamObj;
	GroupCamObj.cameragroupid = m_iGroupID;
	GroupCamObj.cameraid = m_nCameraID;
	GroupCamObj.camera_idx = m_iDBExitCams;
	GroupCamObj.numb_ = m_nDeviceID;
	GroupCamObj.streamid = 1;
	m_pBk->InsertGroupCamera(GroupCamObj);
}

void CDlgScanRemoteCam::InsertCameraRec()
{
	ec_Camera_Rec recObj;
	recObj.camera_idx = m_iDBExitCams;
	recObj.recording_type = _T("0");
	m_pBk->InsertCamRec(recObj);
}

void CDlgScanRemoteCam::InsertOneObject2DB(int iTemp, int iRTSPPort, bool isChannel)
{
	bool bIpvStation(false);

	bIpvStation = (pScanList->GetItemText(iTemp, 4).MakeUpper() == _T("IPVSTATION"));
	InsertCamera(iTemp, iRTSPPort, isChannel, bIpvStation);
	InsertGroupCamera();
	if (bIpvStation && isChannel)
	{
		InsertCameraRec();
	}
}

void CDlgScanRemoteCam::InsertRemoteChannel(int iRTSPPort)
{
	int iTemp = 0, nCount = pScanList->GetItemCount();
	
	for(int iTemp = 0; iTemp < nCount; iTemp++)
	{
		if( pScanList->GetCheck(iTemp) )
		{
			m_iDBExitCams++;
			InsertOneObject2DB(iTemp, iRTSPPort, true);
		}
	}
}

void CDlgScanRemoteCam::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	int iRTSPPort = iGetRTSPPortAndProtocol();
	m_iDBExitCams = m_pBk->GetLastCameraIdx();
	m_nDeviceID = m_pBk->GetLastDeviceIdx() + 1;
	InsertRemoteChannel(iRTSPPort);
	InsertRemoteDevice(iRTSPPort);
    
	OnOK();
}

void CDlgScanRemoteCam::CCheckHeadCtrl::OnItemClicked(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMHEADER* pNMHead = (NMHEADER*)pNMHDR;
	*pResult = 0;

	int nItem = pNMHead->iItem;
	if (0 != nItem)
		return;

	HDITEM hdItem;
	hdItem.mask = HDI_IMAGE;
	VERIFY( GetItem(nItem, &hdItem) );

	if (hdItem.iImage == 1)
		hdItem.iImage = 2;
	else
		hdItem.iImage = 1;

	VERIFY( SetItem(nItem, &hdItem) );
	
	BOOL bl = hdItem.iImage == 2 ? TRUE : FALSE;
	CListCtrl* pListCtrl = (CListCtrl*)GetParent();
	int nCount = pListCtrl->GetItemCount();	
	for(nItem = 0; nItem < nCount; nItem++)
	{
		ListView_SetCheckState(pListCtrl->GetSafeHwnd(), nItem, bl);
	}
}

void CDlgScanRemoteCam::vSetInitFlag(int iFlag, bool bCoLocalLan)
{
	iInitFlag = iFlag;
    m_bSameLocalLan = bCoLocalLan;
}

int CDlgScanRemoteCam::iGetRTSPPortAndProtocol()
{
	if( ((CButton*)this->GetDlgItem(IDC_RADIO_UDP))->GetCheck() )
	{
		m_csStreamProtocol = _T("RTSP");	//--RTP over UDP
		return this->m_iStreamPort;
	}
	else if( ((CButton*)this->GetDlgItem(IDC_RADIO_TCP))->GetCheck() )
	{
		m_csStreamProtocol = _T("RTPTCP");
		return this->m_iStreamPort;
	}
	else if( ((CButton*)this->GetDlgItem(IDC_RADIO_HTTP))->GetCheck() )
	{
		m_csStreamProtocol = _T("RTPHTTP");
		return this->m_iHttpPort;
	}
	
	return this->m_iStreamPort;
}

void CDlgScanRemoteCam::Trans_Camera2NODEITEM(const ec_Camera& camObj, NODEITEM& node)
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
	m_pBk->TransString2Mac(camObj.mac_address, node.mac);
	node.nWebPort = camObj.httpport;

	str = camObj.username;
	strncpy(node.szUser, W2A(str), camObj.username.GetLength());

	str = camObj.password;
	strncpy(node.szPass, W2A(str), camObj.password.GetLength());

	node.bFishEye = m_pBk->IsFishEye(camObj.video_format);
	node.bChannel = m_pBk->IsChannel(camObj.video_format);
	node.byDeviceType = m_pBk->GetDevice(camObj.video_format);

	str = camObj.model_code;
	strcpy(node.modelname, W2A(str));

	node.nAPPort = camObj.vcStream[0].stream_port;
	//node.nProtocol = GetStreamProtocol(camObj);

	CString csSupport=camObj.ptz_support;
	if (csSupport.MakeLower() == L"y")
		node.bSupportPTZ=true;
	else
		node.bSupportPTZ=false;
}