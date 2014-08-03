// DlgAddISAP.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAddISAP.h"
#include "IPCamTreeView.h"
#include "MainFrm.h"
#include "xNetCardInfo.h"

// CDlgAddISAP dialog
static int notifyWriterCheck(char* data, size_t size, size_t nmemb,void* lParam);
static int notifyWriter(char* data, size_t size, size_t nmemb,void* lParam);
static int notifyWriterRemoteCheck(char* data, size_t size, size_t nmemb,void* lParam);

//static CString m_csStaIP, m_csStaPort, m_csStaUser;
static CString m_csHostIP, m_csHostPort, m_csUser, m_csPwd, m_csGroupName;

IMPLEMENT_DYNAMIC(CDlgAddISAP, CDialog)

CDlgAddISAP::CDlgAddISAP(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddISAP::IDD, pParent)
	, m_iResponseLength(0)
	, m_iISAPGroupID(0)
	, m_iGroupID(0)
{
	::ZeroMemory(&byIPAddr, sizeof(byIPAddr));
	for(int iTemp =0; iTemp <16; iTemp++)
	{
		veChannelLength.push_back(0);
	}
	

}

CDlgAddISAP::~CDlgAddISAP()
{
}

void CDlgAddISAP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAddISAP, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAddISAP::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddISAP message handlers

DWORD CDlgAddISAP::dwIpStringToDword(CString& csIp)
{
    DWORD nRet;
    CString ip = csIp;
    TCHAR* tzToken;
    tzToken = _tcstok(ip.GetBuffer(), _T("."));
    BYTE ipValue[4]={};
    int ci(0);
    while (tzToken)
    {
        ipValue[ci++] = _ttoi(tzToken);
        tzToken = _tcstok(NULL, _T(".\r\n"));
        if (ci>=4) break;
    }
    nRet = *(int*)ipValue;
    return nRet;
    
}
bool CDlgAddISAP::CheckisOnLocalLan(CString& csIp)
{
    bool bRet(false);
    DWORD nIp = dwIpStringToDword(csIp);
    do
    {
        if ((nIp & 0x00ffffff) == 0x0000007f) //127.0.0.xx
        {
            bRet = true;
            break;
        }
        CxNetCardInfo netCard;
        for (int cj =0; cj < netCard.veNetCard.size(); cj++)
        {
            CString csNetIp = netCard.veNetCard[cj].IpAddress;
            CString csNetMask = netCard.veNetCard[cj].subnet;
            DWORD nNetip = dwIpStringToDword(csNetIp);
            DWORD nNetMask = dwIpStringToDword(csNetMask);
            if ((nIp & nNetMask) == (nNetip & nNetMask))
            {
                 bRet = true;
                 break;
            }           
        }
    }while(false);
    return bRet;
}

void CDlgAddISAP::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_iRemote = 0;
	GetDlgItemText(IDC_ISAP_HOST, m_csHostIP);
	GetDlgItemText(IDC_ISAP_PORT, m_csHostPort);
	GetDlgItemText(IDC_ISAP_USER, m_csUser);
	GetDlgItemText(IDC_ISAP_PWD, m_csPwd);
	GetDlgItemText(IDC_EDIT_GROUP_NAME, m_csGroupName);

	//IP Address
	((CIPAddressCtrl*)GetDlgItem(IDC_ISAP_HOST))->GetAddress(byIPAddr[0], byIPAddr[1], byIPAddr[2], byIPAddr[3]);

	{
		//--Connect to ISAP
		CString csRemoteNVR, csUserPass;
		csRemoteNVR.Format(_T("http://%s:%s/cgi-bin/sysconfig?action=loadserver"), m_csHostIP, m_csHostPort);
		csUserPass.Format(_T("%s:%s"), m_csUser, m_csPwd);

		char *chUserPass = new char[csUserPass.GetLength() + 1];
		wcstombs_s(NULL, chUserPass, csUserPass.GetLength() + 1, csUserPass, _TRUNCATE);

		char *chRemoteNVR = new char[csRemoteNVR.GetLength() + 1];
		wcstombs_s(NULL, chRemoteNVR, csRemoteNVR.GetLength() + 1, csRemoteNVR, _TRUNCATE);

		CURLcode gCode = curl_global_init(CURL_GLOBAL_ALL);
		if(gCode != CURLE_OK)
		{	
			CString csTemp;
			LOADSTRING(csTemp, IDS_CURL_INI_FAIL);
			AfxMessageBox(IDS_CURL_INI_FAIL);	
		}

		CURL* m_curl = curl_easy_init();
		if(m_curl == NULL)
		{
			CString csTemp;
			LOADSTRING(csTemp, IDS_CURL_EASY_FAIL);
			AfxMessageBox(IDS_CURL_EASY_FAIL);	
		}

		curl_easy_setopt(m_curl, CURLOPT_URL, chRemoteNVR);
		curl_easy_setopt(m_curl, CURLOPT_USERPWD, chUserPass);
		curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, notifyWriterCheck);
		curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 15);

		CURLcode pCode = curl_easy_perform(m_curl);
		curl_easy_cleanup(m_curl);
		curl_global_cleanup();
		
		if( pCode != CURLE_OK )
		{
			//m_iRemote =  NODEITEM::REMOTE_ETROCENTER;

			////--Connect to NVR
			//csRemoteNVR.Format(_T("http://%s:%s/?auth.xml"), m_csHostIP, m_csHostPort);
			//csUserPass.Format(_T("%s:%s"), m_csUser, m_csPwd);

			//chUserPass = new char[csUserPass.GetLength() + 1];
			//wcstombs_s(NULL, chUserPass, csUserPass.GetLength() + 1, csUserPass, _TRUNCATE);

			//chRemoteNVR = new char[csRemoteNVR.GetLength() + 1];
			//wcstombs_s(NULL, chRemoteNVR, csRemoteNVR.GetLength() + 1, csRemoteNVR, _TRUNCATE);

			//gCode = curl_global_init(CURL_GLOBAL_ALL);
			//if(gCode != CURLE_OK)
			//{	AfxMessageBox(_T("curl_init failed." ));	}

			//m_curl = curl_easy_init();
			//if(m_curl == NULL)
			//{	AfxMessageBox(_T("curl_easy_init failed."));	}

			//curl_easy_setopt(m_curl, CURLOPT_URL, chRemoteNVR);
			//curl_easy_setopt(m_curl, CURLOPT_USERPWD, chUserPass);
			//curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, notifyWriterRemoteCheck);
			//curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);

			//CURLcode pCode2 = curl_easy_perform(m_curl);
			//curl_easy_cleanup(m_curl);
			//curl_global_cleanup();

			///*delete [] chUserPass;
			//delete [] chRemoteNVR;*/

			////--Check if TypeInfo Error
			//if(pCode2 != CURLE_OK)
			//{
			//	AfxMessageBox(_T("Connection Error! Please Check the NVR Information."));
			//	return;
			//}
		}

		delete [] chUserPass;
		delete [] chRemoteNVR;
	}

	CDlgScanRemoteCam* pCDlgScanRemoteCam = NULL;

	//if(m_iRemote == NODEITEM::ISAP)
	{
		::Sleep(500);
		for(m_CurlChannel = 0; m_CurlChannel <= 15; m_CurlChannel++)
		{
			//m_iResponseLength = 0;
			CString csURL, csUserPass;
			csURL.Format(_T("http://%s:%s/cgi-bin/deviceconfig?action=load&channel=channel%d"), m_csHostIP, m_csHostPort, m_CurlChannel+1);
			csUserPass.Format(_T("%s:%s"), m_csUser, m_csPwd);
		
			char *chUserPass = new char[csUserPass.GetLength() + 1];
			wcstombs_s(NULL, chUserPass, csUserPass.GetLength() + 1, csUserPass, _TRUNCATE);

			char *chUrl = new char[csURL.GetLength() + 1];
			wcstombs_s(NULL, chUrl, csURL.GetLength() + 1, csURL, _TRUNCATE);

			CURLcode gCode = curl_global_init(CURL_GLOBAL_ALL);
			if(gCode != CURLE_OK)
			{
				CString csTemp;
				LOADSTRING(csTemp, IDS_CURL_INI_FAIL);
				AfxMessageBox(IDS_CURL_INI_FAIL);	
			}

			CURL* m_curl = curl_easy_init();
			if(m_curl == NULL)
			{
				CString csTemp;
				LOADSTRING(csTemp, IDS_CURL_EASY_FAIL);
				AfxMessageBox(IDS_CURL_EASY_FAIL);	
			}

			curl_easy_setopt(m_curl, CURLOPT_URL, chUrl);
			curl_easy_setopt(m_curl, CURLOPT_USERPWD, chUserPass);
			curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, notifyWriter);
			curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);

			CURLcode pCode = curl_easy_perform(m_curl); 
			curl_easy_cleanup(m_curl);
			curl_global_cleanup();

			delete [] chUserPass;
			delete [] chUrl;

		}

		vAddISAPGroup();
		vAddISAPChannel();

	}
	//else if(m_iRemote == NODEITEM::REMOTE_ETROCENTER)
	{
		pCDlgScanRemoteCam = new CDlgScanRemoteCam(m_csHostIP,m_csHostPort,m_csUser,m_csPwd,CheckisOnLocalLan(m_csHostIP));
		if( pCDlgScanRemoteCam->DoModal() != IDOK )
		{
			return;
		}
	}

    delete pCDlgScanRemoteCam;
	CNVRBk::GetBkObject()->SendMessage(MsgUpdateView, UM_REFRESH_DEVICETREE, 0);

	OnOK();
}

int notifyWriter(char* data, size_t size, size_t nmemb,void *lParam)
{
	CDlgAddISAP* pThis = (CDlgAddISAP*)lParam;
	int len = size*nmemb;

	TCHAR tzBuffer[4096];
	MultiByteToWideChar(CP_ACP, 0, data, -1, tzBuffer, 4096);

	CString csNotifyBuffer(_T(""));
	csNotifyBuffer.Format(_T("%s"), tzBuffer);

	if(csNotifyBuffer.Find(_T("DeviceConnectorConfiguration")) == -1)
	{
		pThis->veChannelLength[pThis->m_CurlChannel] = 0;
	}
	else
	{
		//if(lParam == NULL) return len;
		pThis->veChannelLength[pThis->m_CurlChannel] = len;
	}
	return len;
}

int notifyWriterCheck(char* data, size_t size, size_t nmemb,void *lParam)
{
	CDlgAddISAP* pThis = (CDlgAddISAP*)lParam;
	int len = size*nmemb;

	TCHAR tzBuffer[4096];
	MultiByteToWideChar(CP_ACP, 0, data, -1, tzBuffer, 4096);

	CString csNotifyBuffer(_T(""));
	csNotifyBuffer.Format(_T("%s"), tzBuffer);

	if(csNotifyBuffer.Find(_T("404")) != -1 && csNotifyBuffer.Find(_T("Not Found")) != -1)
	{
		//pThis->m_iRemote = NODEITEM::REMOTE_ETROCENTER;
	}
	else
	{
		//pThis->m_iRemote = NODEITEM::ISAP;
	}

	

	return len;
}

int notifyWriterRemoteCheck(char* data, size_t size, size_t nmemb,void *lParam)
{
	CDlgAddISAP* pThis = (CDlgAddISAP*)lParam;
	int len = size*nmemb;

	TCHAR tzBuffer[4096];
	MultiByteToWideChar(CP_ACP, 0, data, -1, tzBuffer, 4096);

	CString csNotifyBuffer(_T(""));
	csNotifyBuffer.Format(_T("%s"), tzBuffer);

	if(csNotifyBuffer.Find(_T("404 Not Found")) != -1)
	{
		//pThis->m_iRemote = NODEITEM::ISAP;
	}
	else
	{
		//pThis->m_iRemote = NODEITEM::REMOTE_ETROCENTER;
	}

	return len;
}

void CDlgAddISAP::vAddISAPGroup()
{
	try
	{
		//--Delete ISAP Group
		CString csSQL, csGroupCount;
		csSQL.Format(_T("DELETE FROM ec_camera_group WHERE camera_group_desc = '%s' AND module_code = 'LIVEVIEW' "), m_csGroupName);
		((CMainFrame*)AfxGetMainWnd())->m_DB.ExecuteSQL(csSQL);

		//--Get Count
		csSQL.Format(_T("SELECT Max(cameragroupid) AS TotalGroups From ec_camera_group"));
		CRecordset rGroupDB(&(((CMainFrame*)AfxGetMainWnd())->m_DB)); 
		rGroupDB.Open(CRecordset::forwardOnly, csSQL.GetBuffer());
		while(!rGroupDB.IsEOF())
		{
			rGroupDB.GetFieldValue(_T("TotalGroups"), csGroupCount);
			rGroupDB.MoveNext();
		}
		m_iGroupID = _ttoi(csGroupCount) + 1;
		//m_iISAPGroupID = _ttoi(csGroupCount) + 1;
		rGroupDB.Close();

		//--Get ISAP Group Count
		CString csISAPGroupCount;
		CRecordset rISAPGroupDB(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
		rISAPGroupDB.Open(CRecordset::forwardOnly, _T("SELECT COUNT(cameragroupid) AS ISAPCount FROM ec_camera_group WHERE camera_group_code = 'ISAP'"));
		while(!rISAPGroupDB.IsEOF())
		{
			rISAPGroupDB.GetFieldValue(_T("ISAPCount"), csGroupCount);
			rISAPGroupDB.MoveNext();
		}
		m_iISAPGroupID =  _ttoi(csGroupCount) + 1;
		rISAPGroupDB.Close();

		//--Add ISAP Group
		csSQL.Format(_T("INSERT INTO ec_camera_group (cameragroupid, camera_group_desc, module_code) VALUES (%d , '%s', 'LIVEVIEW')"), m_iGroupID, m_csGroupName);
		((CMainFrame*)AfxGetMainWnd())->m_DB.ExecuteSQL(csSQL);

	}catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return;
}

void CDlgAddISAP::vAddISAPChannel()
{
	try
	{
		CString csSQL, csCameraCount, csTopCameraid;
		csSQL.Format(_T("SELECT top 1 cameraid FROM ec_camera WHERE model_code = 'NVR-%s'"), m_csHostIP);
		CRecordset rCamDel(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
		rCamDel.Open(CRecordset::forwardOnly, csSQL.GetBuffer());
		while(!rCamDel.IsEOF())
		{
			rCamDel.GetFieldValue(_T("cameraid"), csTopCameraid);
			rCamDel.MoveNext();
		}

		m_iTopCamera = ::_ttoi(csTopCameraid);
		rCamDel.Close();

		if(m_iTopCamera != 0)
		{
			//--Delete Camera from ec_camera
			csSQL.Format(_T("DELETE FROM ec_camera WHERE model_code = 'NVR-%s'"), m_csHostIP);
			((CMainFrame*)AfxGetMainWnd())->m_DB.ExecuteSQL(csSQL);

			//--Delete Stream From ec_stream
			for(int iTemp = 0; iTemp <16; iTemp++)
			{
				csSQL.Format(_T("DELETE FROM ec_stream WHERE cameraid = '%d'"), iTemp+m_iTopCamera);
				((CMainFrame*)AfxGetMainWnd())->m_DB.ExecuteSQL(csSQL);
			}
		}

		////--Get Count
		//csSQL.Format(_T("SELECT COUNT(*) AS TotalCameras FROM ec_camera"));
		//CRecordset rCamDB(&(((CMainFrame*)AfxGetMainWnd())->m_DB)); 
		//rCamDB.Open(CRecordset::forwardOnly, csSQL.GetBuffer());
		//while(!rCamDB.IsEOF())
		//{
		//	rCamDB.GetFieldValue(_T("TotalCameras"), csCameraCount);
		//	rCamDB.MoveNext();
		//}
		//m_iDBExitCams = _ttoi(csCameraCount);
		//rCamDB.Close();

		//--Get last camera_idx
		csSQL.Format(_T("SELECT top 1 * FROM ec_camera ORDER BY camera_idx desc"));
		CRecordset rCamDB(&(((CMainFrame*)AfxGetMainWnd())->m_DB)); 
		rCamDB.Open(CRecordset::forwardOnly, csSQL.GetBuffer());
		while(!rCamDB.IsEOF())
		{
			rCamDB.GetFieldValue(_T("camera_idx"), csCameraCount);
			rCamDB.MoveNext();
		}

		m_iDBExitCams = _ttoi(csCameraCount);
		rCamDB.Close();

		


		//--Add IPCam
		for(int iChannelNum =0; iChannelNum <16; iChannelNum++)
		{
			CString csCameraid;
			if(veChannelLength[iChannelNum] > 0)
			{
				csSQL.Format(_T("INSERT INTO ec_camera(camera_idx, cameraname, ipaddress, httpport, mac_address, username, password, model_code, ptz_support, total_stream, active_) VALUES (%d, '%s-Channel%02d', '%s', '%s', 'FFFFFFFF%02X%02X', '%s', '%s', 'NVR-%s', 'N', '1', 'Y')"), m_iDBExitCams+iChannelNum+1, m_csHostIP, iChannelNum+1, m_csHostIP, m_csHostPort, m_iISAPGroupID, iChannelNum+1, m_csUser, m_csPwd, m_csHostIP);
				((CMainFrame*)AfxGetMainWnd())->m_DB.ExecuteSQL(csSQL);

				//--Get cameraid
				csSQL.Format(_T("SELECT cameraid FROM ec_camera WHERE camera_idx = %d"), m_iDBExitCams+iChannelNum+1);
				CRecordset rCamidDB(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
				rCamidDB.Open(CRecordset::forwardOnly, csSQL.GetBuffer());
				while(!rCamidDB.IsEOF())
				{
					rCamidDB.GetFieldValue(_T("cameraid"), csCameraid);
					rCamidDB.MoveNext();
				}
				rCamDB.Close();

				//--Insert camera group camera
				csSQL.Format(_T("INSERT INTO ec_camera_group_camera(cameragroupid, cameraid, camera_idx) VALUES(%d, %s, %d)"), m_iGroupID, csCameraid, m_iDBExitCams+iChannelNum+1);
				((CMainFrame*)AfxGetMainWnd())->m_DB.ExecuteSQL(csSQL);

				//--delete from ec_stream
				csSQL.Format(_T("DELETE FROM ec_stream WHERE cameraid = %s "), csCameraid);
				((CMainFrame*)AfxGetMainWnd())->m_DB.ExecuteSQL(csSQL);

				//--insert camera into ec_stream
				csSQL.Format(_T("INSERT INTO ec_stream (stream_name, cameraid, stream_type, stream_port, stream_protocol) VALUES('Stream 1', %s, 'LV', '%s', 'CGI_ISAP')"), csCameraid, m_csHostPort);
				((CMainFrame*)AfxGetMainWnd())->m_DB.ExecuteSQL(csSQL);
			}
		}
		

		//--Add IPCam
		//for(int iTemp = 1; iTemp <= m_iTotalChannel; iTemp++)
		//{
		//	CString csCameraid;
		//	csSQL.Format(_T("INSERT INTO ec_camera(camera_idx, cameraname, ipaddress, httpport, mac_address, username, password, model_code, active_) VALUES(%d, 'ISAP-Channel%02d', '%s', '%s', 'eNVRCamera%02d', '%s', '%s', 'eNVR-%s', 'Y')"), m_iDBExitCams+iTemp, iTemp, m_csHostIP, m_csHostPort, iTemp, m_csUser, m_csPwd, m_csHostIP);
		//	((CMainFrame*)AfxGetMainWnd())->m_DB.ExecuteSQL(csSQL);

		//	//--Get cameraid
		//	csSQL.Format(_T("SELECT cameraid FROM ec_camera WHERE camera_idx = %d"), m_iDBExitCams+iTemp);
		//	CRecordset rCamidDB(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
		//	rCamidDB.Open(CRecordset::forwardOnly, csSQL.GetBuffer());
		//	while(!rCamidDB.IsEOF())
		//	{
		//		rCamidDB.GetFieldValue(_T("cameraid"), csCameraid);
		//		rCamidDB.MoveNext();
		//	}
		//	rCamDB.Close();
		//	//--Insert camera group camera
		//	csSQL.Format(_T("INSERT INTO ec_camera_group_camera(cameragroupid, cameraid, camera_idx) VALUES(%d, %s, %d)"), m_iISAPGroupID, csCameraid, m_iDBExitCams+iTemp);
		//	((CMainFrame*)AfxGetMainWnd())->m_DB.ExecuteSQL(csSQL);

		//	//--delete from ec_stream
		//	csSQL.Format(_T("DELETE FROM ec_stream WHERE cameraid = %s "), csCameraid);
		//	((CMainFrame*)AfxGetMainWnd())->m_DB.ExecuteSQL(csSQL);

		//	//--insert camera into ec_stream
		//	csSQL.Format(_T("INSERT INTO ec_stream (stream_name, cameraid, stream_type, stream_port, stream_protocol) VALUES('Stream 1', %s, 'LV', '%s', 'CGI_ISAP')"), csCameraid, m_csHostPort);
		//	((CMainFrame*)AfxGetMainWnd())->m_DB.ExecuteSQL(csSQL);
		//}
	
	}catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
} 
BOOL CDlgAddISAP::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	SetDlgItemText(IDC_ISAP_HOST, m_csHostIP);
	SetDlgItemText(IDC_ISAP_PORT, m_csHostPort);
	SetDlgItemText(IDC_ISAP_USER, m_csUser);	
	SetDlgItemText(IDC_EDIT_GROUP_NAME, _T("NVR Default Group"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
