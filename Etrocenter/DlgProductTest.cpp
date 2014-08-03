// DlgProductTest.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "DlgProductTest.h"
#include "CGIVar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UPGRADE_FIRMWARE 1
/////////////////////////////////////////////////////////////////////////////
// CDlgProductTest dialog


CDlgProductTest::CDlgProductTest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProductTest::IDD, pParent), m_pvtNode(NULL), m_pBuffer(NULL)
{
	//{{AFX_DATA_INIT(CDlgProductTest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CDlgProductTest::~CDlgProductTest()
{
	if (m_pBuffer)
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}
}

void CDlgProductTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProductTest)
	DDX_Control(pDX, IDC_LIST1, m_listctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProductTest, CDialog)
	//{{AFX_MSG_MAP(CDlgProductTest)
	ON_BN_CLICKED(IDC_BTN_UPGRADE_FW, OnBtnUpgradeFw)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProductTest message handlers

BOOL CDlgProductTest::OnInitDialog() 
{
	if (m_pvtNode == NULL) return false;
	CDialog::OnInitDialog();
	const int MAX_DISPLAY_NUM = 6;
	TCHAR* listcolumn[MAX_DISPLAY_NUM] = { _T("Mac."),_T("IP"),_T("Model"),_T("firmware ver"),_T("Status"),_T("New Firmware Ver")};
    int width[MAX_DISPLAY_NUM] = {150, 150, 100, 100, 200, 100};
	int i;
	for( i= 0; i < MAX_DISPLAY_NUM; i++)
	{
		LV_COLUMN lvcolumn;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = listcolumn[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = width[i];
		int nCol = m_listctrl.InsertColumn(i,&lvcolumn);
	}

	DWORD nRet = m_listctrl.SetExtendedStyle(LBS_OWNERDRAWVARIABLE | WS_TABSTOP | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP );//| LVS_EX_ONECLICKACTIVATE

	SetDlgItemText(IDC_STATIC_TIME_HINT, _T(""));
	vCreateListData();		
	return TRUE; 
}

void CDlgProductTest::vAssignNode(vector <NODEITEM*>* pvtNode)
{
	m_pvtNode = pvtNode;
}

void CDlgProductTest::OnBtnUpgradeFw() 
{
//	NODEITEM* pNode = GetCurrentNodeItem();
	
	if (bPrepareFirmwareBuffer())
	{
		GetDlgItem(IDOK)->EnableWindow(false);
		GetDlgItem(IDC_BTN_UPGRADE_FW)->EnableWindow(false);
		GetLocalTime(&m_TimerStart);
		m_lTimerStartSec = m_TimerStart.wHour*60*60 + m_TimerStart.wMinute*60 + m_TimerStart.wSecond;
		SetTimer(UPGRADE_FIRMWARE, 1000, NULL);

		m_Counter = 0;
		for (int ci=0; ci < m_pvtNode->size(); ci++)
		{
			m_Counter++;
			FWUPGRADEPARAM* param = new FWUPGRADEPARAM;
			param->pThis = this;
			param->pNode = (*m_pvtNode)[ci];
			AfxBeginThread(fnFWUpgrade, param);
		}
	}
}

UINT CDlgProductTest::fnFWUpgrade(LPVOID param)
{
	const UINT FW_UPGRADE_RETRY_TIME = 1;
	FWUPGRADEPARAM* paramanter = (FWUPGRADEPARAM*) param;
	UINT nRetry = FW_UPGRADE_RETRY_TIME;
	while (((nRetry--) >0) && !paramanter->pThis->bUpdateFirmware(paramanter->pNode));
	paramanter->pThis->m_Counter--;
	if (paramanter->pThis->m_Counter == 0)
	{
		HANDLE hScanRequire =  OpenEvent(EVENT_ALL_ACCESS, TRUE, _T("Global\\ETRO_SCAN_REQUIRE"));
		::SetEvent(hScanRequire);
		::CloseHandle(hScanRequire);	
		paramanter->pThis->GetDlgItem(IDOK)->EnableWindow(true);
		paramanter->pThis->GetDlgItem(IDC_BTN_UPGRADE_FW)->EnableWindow(true);
		paramanter->pThis->KillTimer(UPGRADE_FIRMWARE);
		CString csText;
		paramanter->pThis->GetDlgItemText(IDC_STATIC_TIME_HINT, csText);
		csText.Replace(_T("Time Now"), _T("Finish Time"));
		paramanter->pThis->SetDlgItemText(IDC_STATIC_TIME_HINT, csText);
	}
	delete paramanter;
	return 0;
}

bool CDlgProductTest::bPrepareFirmwareBuffer()
{
#define BOUNDARY "{00FD9B83-C692-459a-B652-6D993C0875FA}"
	bool bRet = false;
	CDlgProductTest* pThis = this;
	const TCHAR wszBoundary[] = _T(BOUNDARY);

	static TCHAR BASED_CODE sFilter[] = _T("Binary File (*.bin)|*.bin|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, sFilter, NULL);
	CString csFile;
	if (dlg.DoModal() == IDOK)
	{
		csFile = dlg.GetPathName();
	}

	CFileFind finder;
	if (finder.FindFile(csFile) == FALSE) 
		return false;
	finder.FindNextFile();
	int nFileSize = finder.GetLength();

	CFile fileFw;
	fileFw.Open(csFile, CFile::modeRead);

	USES_CONVERSION;
	WORD nEvent(0);

	UINT nBytesRead = 0;
	CFile fileBuffer;

	char szBodyHead[] = "--" BOUNDARY "\r\nContent-Disposition: form-data; name=\"updateType\"" "\r\nfirmware\r\n" 
						"Accept: *//*" 
						"Content-Disposition: form-data; name=\"flash\"; filename=\"firmware.BIN\"\r\n"
						"Content-Type: multipart/form-data\r\n\r\n";

	char szBodyTail[] = "\r\n--" BOUNDARY "--\r\n";

	DWORD nHeadSize = strlen(szBodyHead);
	DWORD nTailSize = strlen(szBodyTail);
	m_nBufferSize = nHeadSize +nFileSize +nTailSize;

	WCHAR wszContentLength[16];
	wsprintf(wszContentLength, _T("%d"), m_nBufferSize);

	m_csHead = _T("Content-Type: multipart/form-data; boundary=");
	m_csHead += wszBoundary;
	m_csHead += _T("\r\nContent-Length: ");
	m_csHead +=	wszContentLength;
	m_csHead += _T("\r\n\r\n");

	if (m_pBuffer)
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}
	m_pBuffer = new char[m_nBufferSize];
	::ZeroMemory(m_pBuffer, m_nBufferSize);

	strncpy(m_pBuffer, szBodyHead, nHeadSize);
#if _MSC_VER > 1200
	nFileSize = fileFw.Read(&m_pBuffer[nHeadSize], nFileSize);
#else
	nFileSize = fileFw.ReadHuge(&m_pBuffer[nHeadSize], nFileSize);
#endif
	strncpy(&m_pBuffer[nHeadSize+nFileSize], szBodyTail, nTailSize);	
	return true;
}

bool CDlgProductTest::bUpdateFirmware(NODEITEM *pNode)
{

	bool bRet = false;
	DWORD nTimeOut, nConnectTimeout;
	CGISession session;//(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS, NULL, NULL, INTERNET_FLAG_ASYNC);
	//session.EnableStatusCallback();

	CHttpConnection* pServer=NULL;
	CHttpFile* pFile=NULL;
	USES_CONVERSION;
	CString csUrl;
	csUrl.Format( _T("/config/firmwareupgrade.cgi?keep_net=yes&keep_user"));

	nConnectTimeout =  60000;
	nTimeOut        = 500000;
	BOOL bOptionRet;
//	bOptionRet = session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nConnectTimeout);
//	bOptionRet = session.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, nTimeOut);
//	bOptionRet = session.SetOption(	INTERNET_OPTION_RECEIVE_TIMEOUT, nTimeOut);
	pServer = session.GetHttpConnection( A2W(pNode->ip), INTERNET_PORT(pNode->nWebPort), A2W(pNode->szUser), A2W(pNode->szPass));
//	if (!bTurnLed(session, pNode, false))
//		goto exit;

	bOptionRet = pServer->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nConnectTimeout);
	bOptionRet = pServer->SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, nTimeOut);
	bOptionRet = pServer->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, nTimeOut);
	pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, csUrl, NULL, 1, NULL, NULL, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_AUTO_REDIRECT );	
	bOptionRet = pFile->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nConnectTimeout);
	bOptionRet = pFile->SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, nTimeOut);
	bOptionRet = pFile->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, nTimeOut);
	while (true)
	{
		try {
			CString csMsg;
			vUpdataStatus(pNode, _T("Firmware Updateing!!!"));
			pFile->AddRequestHeaders(m_csHead);
			pFile->SendRequestEx(m_nBufferSize);
			pFile->Write(m_pBuffer, m_nBufferSize);  
			csMsg = _T("Update Firmware Fail!!!");
			if (pFile->EndRequest())
			{
				const int BUFFSIZE =1024;
				char buffer[BUFFSIZE] = {0};

				int nBytesRead = pFile->Read(buffer, BUFFSIZE - 1);
				// for tolerence firmware bug 1.8.7 ~ 1.9.1 ; so we have "reboot = yes" token 
				char* szResultTag="upgrade";
				char szResultValue[2][4]={"ok","yes"};
				char* sz = strstr(buffer, szResultTag);
				if (sz)
				{
					char* szToken = strtok(sz, "=");
					if (szToken)
					{
						szToken = strtok(NULL, "\r\n");
						for (int ci=0; ci < 2, szToken != NULL; ci++)
						{
							if (strstr(szToken, szResultValue[ci]))
							{
	//							bTurnLed(session, pNode, true);
								csMsg = _T("Update Firmware Sucess!!!");
								if (bSendRebootCmd(session, pNode, csMsg))
								{
									if (bRet = bGetFWVersion(session, pNode, csMsg))
									{
										m_vtNodeOK.push_back(pNode);
										break;
									}
								}			
							}
						}
					}
				}
			}else
			{
				
			}
			vUpdataStatus(pNode, csMsg);
			break;
		}
		catch(CInternetException* e) {
			pFile->QueryOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
			int nError = e->m_dwError;
			CString csMsg;
			csMsg.Format(_T("Update Firmware Fail!!!;Error : %d;"), nError);
			TCHAR   szCause[255];
			e->GetErrorMessage(szCause, 255);
			csMsg += szCause;
			vUpdataStatus(pNode, csMsg);
			e->Delete();
			if (nError== ERROR_INTERNET_FORCE_RETRY) 
				continue;
		}
	}
//exit:
	if (pFile)
	{
		pFile->Close();
		delete pFile;
	}
	if (pServer)
	{
		pServer->Close();
		delete pServer;
	}
	return bRet;
}
void CDlgProductTest::vUpdataStatus(NODEITEM *pItem, CString csStatus, WORD nColumn)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_STRING;//LVFI_PARAM;
	TCHAR wsz[256];
	//sprintf(sz, _T(""), pItem->mac
	_stprintf(wsz, _T("%02x-%02x-%02x-%02x-%02x-%02x"),pItem->mac[0], pItem->mac[1], pItem->mac[2], pItem->mac[3], pItem->mac[4], pItem->mac[5]);
	FindInfo.psz = wsz;
//	FindInfo.lParam = (LPARAM)pItem;
	int nItem = m_listctrl.FindItem(&FindInfo);
	if (nItem != -1)
	{
		m_listctrl.SetItemText(nItem, nColumn , csStatus);
	}
}

bool CDlgProductTest::bSendRebootCmd(CInternetSession& session, NODEITEM *pNode, CString& csMsg)
{
	bool bRet = false;
	USES_CONVERSION;
	CString csUrl;
	csUrl.Format( _T("http://%s:%s@%s:%d/config/system_reboot.cgi?reboot=go"), A2W(pNode->szUser), A2W(pNode->szPass), A2W(pNode->ip), pNode->nWebPort);
//	csUrl.Format( _T("http://%s:%d/config/system_reboot.cgi?reboot=go"), A2W(pNode->ip), pNode->nWebPort);

	ETROLOG(_T("%s\r\n"),csUrl);
	CStdioFile* pRetFile =NULL;
	csMsg = _T("Reboot Fail!!!");
	try {
		pRetFile = session.OpenURL(csUrl, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_KEEP_CONNECTION);
		char buffer[1024]={0};
		int nBytesRead = pRetFile->Read(buffer, 1024 - 1);
		char* sz = strstr(buffer, "reboot");
		if (sz)
		{
			char* szToken = strtok(sz, "=");
			if (szToken)
			{
				szToken = strtok(NULL, "\r\n");
				if (strstr(szToken, "yes"))
				{
					bRet = true;
					csMsg = _T("Rebooting");
				}
			}
		}
	}
	catch(CInternetException* e) {
		int nError = e->m_dwError;
		e->Delete();
	}
	vUpdataStatus(pNode, csMsg);
	if (pRetFile)
		delete pRetFile;
	return bRet;
}

bool CDlgProductTest::bGetFWVersion(CInternetSession& session, NODEITEM *pNode, CString& csMsg)
{
	bool bRet = false;
	::Sleep(30000); // sleep for ipcam rebooting
	USES_CONVERSION;
	CString csUrl;
	csUrl.Format( _T("http://%s:%s@%s:%d/common/info.cgi"), A2W(pNode->szUser), A2W(pNode->szPass), A2W(pNode->ip), pNode->nWebPort);

	ETROLOG(_T("%s\r\n"),csUrl);
	CStdioFile* pRetFile = NULL;
	int nRetry =0;
	csMsg =  _T("Reboot Fail!");
	while (!bRet && (nRetry++ < 6))
	{
		::Sleep(10000);
		try {
			pRetFile = session.OpenURL(csUrl, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_KEEP_CONNECTION);	
			char buffer[1024]={0};
			int nBytesRead = pRetFile->Read(buffer, 1024 - 1);
			char* sz = strstr(buffer, "version");
			if (sz)
			{
				char* szToken = strtok(sz, "=");
				if (szToken)
				{
					szToken = strtok(NULL, "\r\n");
					csMsg =  _T("Re-boot OK");
					vUpdataStatus(pNode, A2W(szToken), 5);
					pNode->nStatus = RESETTING;
					bRet = true;
				}
			}
		}
		catch(CInternetException* e) {			
			int nError = e->m_dwError;
			e->Delete();
		}
	}
	vUpdataStatus(pNode, csMsg);
	if (pRetFile)
		delete pRetFile;
	return bRet;
}

void CDlgProductTest::vCreateListData()
{
	USES_CONVERSION;
	m_listctrl.DeleteAllItems();
	LV_ITEM lvi;
	TCHAR szItem[256];
	lvi.mask = LVIF_TEXT;
	for (int ci=0; ci < m_pvtNode->size(); ci++)
	{
		lvi.iItem = ci;

		int nColumn =0;
		lvi.iSubItem = nColumn;
		_stprintf(szItem, _T("%02x-%02x-%02x-%02x-%02x-%02x"),(*m_pvtNode)[ci]->mac[0], (*m_pvtNode)[ci]->mac[1], (*m_pvtNode)[ci]->mac[2], (*m_pvtNode)[ci]->mac[3], (*m_pvtNode)[ci]->mac[4], (*m_pvtNode)[ci]->mac[5]);
		lvi.pszText = szItem;
		lvi.lParam = (LPARAM)(*m_pvtNode)[ci];
		m_listctrl.InsertItem(&lvi);

		// SubItem 1
		lvi.iSubItem = ++nColumn;
		_stprintf(szItem, A2W((*m_pvtNode)[ci]->ip));
		lvi.pszText = szItem;
		m_listctrl.SetItem(&lvi);

		// SubItem 1
		lvi.iSubItem = ++nColumn;
//		_stprintf(szItem, A2W((*m_pvtNode)[ci]->ip));
		lvi.pszText = (A2W((*m_pvtNode)[ci]->modelname));
		m_listctrl.SetItem(&lvi);


		lvi.iSubItem = ++nColumn;
		_stprintf(szItem, _T("%d.%d.%d"), (*m_pvtNode)[ci]->firmwarever[0], (*m_pvtNode)[ci]->firmwarever[1], (*m_pvtNode)[ci]->firmwarever[2]);
		//swprintf(pDispInfo->item.pszText, _T("%d.%d.%d"), pItem->firmwarever[0], pItem->firmwarever[1], pItem->firmwarever[2]);
		lvi.pszText = szItem;
		m_listctrl.SetItem(&lvi);		
	}
}

bool CDlgProductTest::bTurnLed(CInternetSession& session, NODEITEM *pNode, bool bOn)
{
	bool bRet = false;
	USES_CONVERSION;
	CString csUrl;
	csUrl.Format( _T("http://%s:%s@%s:%d/config/led.cgi?led=%s"), A2W(pNode->szUser), A2W(pNode->szPass), A2W(pNode->ip), pNode->nWebPort, (bOn ? _T("on") : _T("off")) );
	ETROLOG(_T("%s\r\n"),csUrl);
	CStdioFile* pRetFile = NULL;
	int nRetry =3;
	while ((bRet == false) && (nRetry-- > 0))
	{
		try {
			pRetFile = session.OpenURL(csUrl, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_KEEP_CONNECTION);	
			char buffer[1024]={0};
			int nBytesRead = pRetFile->Read(buffer, 1024 - 1);
			char* sz = strstr(buffer, "led");
			if (sz)
			{
				char* szToken = strtok(sz, "=");
				if (szToken)
				{
					szToken = strtok(NULL, "\r\n");
					bRet = !strcmp(bOn ? "on" : "off", szToken);			
				}
			}
		}
		catch(CInternetException* e) {			
			int nError = e->m_dwError;
			e->Delete();
		}
	}
	CString csMsg;
	csMsg.Format(_T("Led turn %s %s"), (bOn ? _T("on") : _T("off")), (bRet ? _T("OK") : _T("Fail")));
	vUpdataStatus(pNode, csMsg);
	if (pRetFile)
		delete pRetFile;
	return bRet;	
}

void CDlgProductTest::OnTimer(UINT_PTR nIDEvent) 
{

	if (nIDEvent == 1)
	{
		SYSTEMTIME TimerNow={0}, TimeDuration={0};
		ULONG TimeDurationSec;
		GetLocalTime(&TimerNow);
		TimeDurationSec = TimerNow.wHour*60*60 + TimerNow.wMinute*60 + TimerNow.wSecond - m_lTimerStartSec;
		
		TimeDuration.wHour = TimeDurationSec /60 /60;
		TimeDuration.wMinute = (TimeDurationSec % (60 * 60)) / 60;
		TimeDuration.wSecond = (TimeDurationSec % (60 * 60)) % 60;
		CWnd* pWnd = GetDlgItem(IDC_STATIC_TIME_HINT);
		CString cs;
//		TimeDuraion = m_TimerStart - TimerNow;
		cs.Format(_T("Start Time: %02d-%02d-%02d\tTime Noew: %02d-%02d-%02d\tElapsed Time: %02d-%02d-%02d"),
			m_TimerStart.wHour, m_TimerStart.wMinute, m_TimerStart.wSecond,
			TimerNow.wHour, TimerNow.wMinute, TimerNow.wSecond,
			TimeDuration.wHour, TimeDuration.wMinute, TimeDuration.wSecond);
		pWnd->SetWindowText(cs);

	}
	CDialog::OnTimer(nIDEvent);
}
