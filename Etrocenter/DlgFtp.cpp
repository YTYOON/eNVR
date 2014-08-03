// DlgFtp.cpp : implementation file
//

#include "stdafx.h"
#include "DlgFtp.h"
#include "DlgSettingLogin.h"
#include "DlgSettingLoginAuth.h"

// CDlgFtp dialog

IMPLEMENT_DYNAMIC(CDlgFtp, CBaseDialog)

CDlgFtp::CDlgFtp(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgFtp::IDD, pParent)
	, m_dirty(false)
	, m_curTabPage(0),m_DlgSettingLoginAuth(NULL),m_DlgFtpPage(NULL), 
	m_DlgEmailPage(NULL),m_DlgSystemPage(NULL),m_DlgSettingLogin(NULL),
	m_RemoteSrvPort(NULL),m_CDlgSMTPSetting(NULL),m_mainTab(NULL)
{
}

CDlgFtp::~CDlgFtp()
{
	delete m_DlgSystemPage;
	//delete m_DlgFtpPage;
	//delete m_DlgEmailPage;

	delete m_DlgSettingLoginAuth;
	delete m_RemoteSrvPort;
	delete m_CDlgSMTPSetting;
}

void CDlgFtp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_TABFTP, m_tabFtp);
}


BEGIN_MESSAGE_MAP(CDlgFtp, CDialog)
//	ON_WM_ERASEBKGND()
//	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
//	ON_WM_PAINT()
//	ON_WM_SIZE()
//	ON_NOTIFY(TCN_SELCHANGE, IDC_TABFTP, &CDlgFtp::OnTcnSelchangeTabftp)
    ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDOK, &CDlgFtp::OnBnClickedOk)
	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABFTP, &CDlgFtp::OnTcnSelchangeTabftp)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTSAVE, &CDlgFtp::OnBnClickedButsave)
	ON_WM_MOVE()
//    ON_WM_DELETEITEM()
//ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgFtp message handlers

//BOOL CDlgFtp::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: Add your message handler code here and/or call default
//	CBrush brush(GLOBALBKCOLOR);
//	CRect rect;
//	GetClientRect(rect);
//	pDC->FillRect(&rect, &brush);
//	//return CBaseDialog::OnEraseBkgnd(pDC);
//	return true;
//}

//HBRUSH CDlgFtp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	//HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  Change any attributes of the DC here
//
//	// TODO:  Return a different brush if the default is not desired
//	CBrush brush(GLOBALBKCOLOR);
//	//CBrush brush(LISTFRONTCOLOR);
//	CRect rect;
//	GetClientRect(rect);
//	pDC->FillRect(&rect, &brush);
//	return brush;
//}

void CDlgFtp::ChangeControlLanguage()
{
	CString str, csMsg;
	LOADSTRING(str,IDS_DLG_SETTING_SYSTEM)
	this->SetWindowTextW(str);

	m_mainTab = ((CTabCtrl*)GetDlgItem(IDC_TABFTP));
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;

	LOADSTRING(str,IDS_GENERAL);
	csMsg.Format(_T("%s"), str);
	tcItem.pszText = (LPWSTR)(LPCTSTR)csMsg;
	m_mainTab->InsertItem(GENERALPAGE, &tcItem);

	LOADSTRING(str,IDS_PERMISSION);
	csMsg.Format(_T("%s"), str);
	tcItem.pszText = (LPWSTR)(LPCTSTR)csMsg;
	m_mainTab->InsertItem(USERAUTH, &tcItem);

	LOADSTRING(str,IDS_REMOTEPORT);
	csMsg.Format(_T("%s"), str);
	tcItem.pszText = (LPWSTR)(LPCTSTR)csMsg;
	m_mainTab->InsertItem(WEBPORT, &tcItem);

	LOADSTRING(str,IDS_SMTP);
	csMsg.Format(_T("%s"), str);
	tcItem.pszText = (LPWSTR)(LPCTSTR)csMsg;
	m_mainTab->InsertItem(SMTPSERVER, &tcItem);

// 	csMsg.Format(_T("  ³Ñ¾lªÅ¶¡"), str);
// 	tcItem.pszText = (LPWSTR)(LPCTSTR)csMsg;
// 	m_mainTab->InsertItem(SMTPSERVER, &tcItem);
}

BOOL CDlgFtp::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	m_dirty = false;

	ChangeControlLanguage();

	m_DlgSystemPage = new CDlgSettingSystem(this);
	m_DlgSystemPage->Create(IDD_SETTING_SYSTEM, m_mainTab);
	m_DlgSettingLoginAuth = new CDlgSettingLoginAuth;
	m_DlgSettingLoginAuth->Create(IDD_DLG_USER_AUTH, m_mainTab);
	m_RemoteSrvPort = new CRemoteSrvPort;
	m_RemoteSrvPort->Create(IDD_SETTING_REMOTE_SRV, m_mainTab);
	m_CDlgSMTPSetting = new CDlgSMTPSetting;
	m_CDlgSMTPSetting->Create(IDD_SETTING_SMTP, m_mainTab);

	CRect rect;
	m_mainTab->GetClientRect(&rect);
	rect.top+=20;
	rect.bottom-=4;
	rect.left+=4;
	rect.right-=4;

	m_DlgSystemPage->MoveWindow(&rect);
	m_DlgSettingLoginAuth->MoveWindow(&rect);
	m_RemoteSrvPort->MoveWindow(&rect);
	m_CDlgSMTPSetting->MoveWindow(&rect);

	m_DlgSystemPage->ShowWindow(SW_SHOW);
	m_DlgSettingLoginAuth->ShowWindow(SW_HIDE);
	m_RemoteSrvPort->ShowWindow(SW_HIDE);
	m_CDlgSMTPSetting->ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



//void CDlgFtp::OnPaint()
//{
//	int ygap=20;
//	CPaintDC dc(this); // device context for painting
//	// TODO: Add your message handler code here
//	CRect crTab;
//	m_mainTab->GetWindowRect(crTab);
//	//m_mainTab->AdjustRect(false, &crTab);
//
//	//crTab.top += ygap;
//
//	m_DlgSystemPage->MoveWindow(crTab);
//	m_DlgFtpPage->MoveWindow(crTab);
//	m_DlgEmailPage->MoveWindow(crTab);
//	// Do not call CBaseDialog::OnPaint() for painting messages
//
//}

//void CDlgFtp::OnSize(UINT nType, int cx, int cy)
//{
//	CBaseDialog::OnSize(nType, cx, cy);
//
//	// TODO: Add your message handler code here
//}

void CDlgFtp::OnTcnSelchangeTabftp(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	m_curTabPage = m_mainTab->GetCurSel();

	switch (m_curTabPage)
	{
		case 0: //ftp page
			m_DlgSystemPage->ShowWindow(SW_SHOW);
			//m_DlgSettingLogin->ShowWindow(SW_HIDE);
			m_DlgSettingLoginAuth->ShowWindow(SW_HIDE);
			//m_DlgFtpPage->ShowWindow(SW_HIDE);
			//m_DlgEmailPage->ShowWindow(SW_HIDE);
			m_RemoteSrvPort->ShowWindow(SW_HIDE);
			m_CDlgSMTPSetting->ShowWindow(SW_HIDE);
			break;
		case 1: //ftp page
			m_DlgSystemPage->ShowWindow(SW_HIDE);
			//m_DlgSettingLogin->ShowWindow(SW_SHOW);
			m_DlgSettingLoginAuth->ShowWindow(SW_SHOW);
			//m_DlgFtpPage->ShowWindow(SW_SHOW);
			//m_DlgEmailPage->ShowWindow(SW_HIDE);
			m_RemoteSrvPort->ShowWindow(SW_HIDE);
			m_CDlgSMTPSetting->ShowWindow(SW_HIDE);
			break;
		case 2:
			m_DlgSystemPage->ShowWindow(SW_HIDE);
			//m_DlgSettingLogin->ShowWindow(SW_SHOW);
			m_DlgSettingLoginAuth->ShowWindow(SW_HIDE);
			//m_DlgFtpPage->ShowWindow(SW_SHOW);
			//m_DlgEmailPage->ShowWindow(SW_HIDE);
			m_RemoteSrvPort->ShowWindow(SW_SHOW);
			m_CDlgSMTPSetting->ShowWindow(SW_HIDE);
			break;
		case 3:
			m_DlgSystemPage->ShowWindow(SW_HIDE);
			//m_DlgSettingLogin->ShowWindow(SW_SHOW);
			m_DlgSettingLoginAuth->ShowWindow(SW_HIDE);
			//m_DlgFtpPage->ShowWindow(SW_SHOW);
			//m_DlgEmailPage->ShowWindow(SW_HIDE);
			m_RemoteSrvPort->ShowWindow(SW_HIDE);
			m_CDlgSMTPSetting->ShowWindow(SW_SHOW);
			break;
	}
	/*GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);*/
	*pResult = 0;
}

void CDlgFtp::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//vSaveFtp();
	CDlgFtp::OnBnClickedButsave();
	OnOK();
}

bool CDlgFtp::vSaveSystem(void)
{
	m_DlgSystemPage->vSaveSystem();	
	//m_DlgSettingLogin->vSaveUserLogin();
	m_DlgSettingLoginAuth->bSaveInfo2DB();

	BOOL  bChkHttpPort(false);
	BOOL  bChkRTPPort(false);
	CString csHttpPort,csRTPPort;
	CString csRestartService,csHttpSvr,csRTPSvr;

	m_RemoteSrvPort->GetDlgItemText(IDC_EDIT_SRV_PORT, csHttpPort);
	m_RemoteSrvPort->GetDlgItemText(IDC_EDIT_RTSPSRV_PORT, csRTPPort);
	int nHttPort=_wtoi(csHttpPort);
	int nRTPPort=_wtoi(csRTPPort);
	if (nHttPort != m_RemoteSrvPort->m_nHttpPort)
	{
		bChkHttpPort = true;
		csHttpSvr=L"Http Service";
	}
	
	if (nRTPPort != m_RemoteSrvPort->m_nRTSPPort)
	{
		bChkRTPPort = true;
		csRTPSvr=L"Streaming Service";
		if (bChkHttpPort)
			csRTPSvr = L" + "+csRTPSvr;
	}

	CString csRestart;
	LOADSTRING(csRestart, IDS_MSG_RESTART_REMOTESVR);
	csRestartService.Format(L"%s %s%s ?", csRestart,csHttpSvr, csRTPSvr); 
	int nRet(0);
	if (bChkHttpPort || bChkRTPPort)
	{
		nRet=MessageBox(csRestartService, 0, MB_YESNO);
	}
		
	if (nRet==IDYES)
	{
		m_RemoteSrvPort->vSetPort2Reg();
		m_RemoteSrvPort->vSetRTSPPort2Reg();
		if (bChkHttpPort)
		{
			CNVRBk::GetBkObject()->GetMainFrame()->OnRemoteRestart();
		}
		if (bChkRTPPort)
		{
			CNVRBk::GetBkObject()->GetMainFrame()->OnStreamRestart();
		}
	}
	else
	{
		csHttpPort.Format(L"%d", m_RemoteSrvPort->m_nHttpPort);
		csRTPPort.Format(L"%d", m_RemoteSrvPort->m_nRTSPPort);
		m_RemoteSrvPort->SetDlgItemText(IDC_EDIT_SRV_PORT, csHttpPort);
		m_RemoteSrvPort->SetDlgItemText(IDC_EDIT_RTSPSRV_PORT, csRTPPort);
	}  	
	
	m_CDlgSMTPSetting->vSetSMTP2Reg();
	return false;
}
bool CDlgFtp::vSaveFtp(void)
{
	//m_DlgFtpPage->vSaveFtp();
	return false;
}

bool CDlgFtp::vSaveEmail(void)
{
	//m_DlgEmailPage->vSaveEmail();
	return false;
}

bool CDlgFtp::vSaveUserLogin(void)
{
	//m_DlgSettingLogin->vSaveUserLogin();
	return false;
}

int CDlgFtp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	/*DWORD dwStyle =WS_CHILD | WS_VISIBLE | TCS_SINGLELINE;
	m_mainTab = new CTabCtrl;
	m_mainTab->Create(dwStyle, CRect(5,5,650,380), this, IDC_TABFTP);*/
	return 0;
}

//void CDlgFtp::OnTcnSelchangeTabftp(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

BOOL CDlgFtp::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

//This function check the dirty status before select item changed or cancel
bool CDlgFtp::vCheckDirty(void)
{
	//if(m_DlgSystemPage->DirtyFlag == true || m_DlgFtpPage->DirtyFlag == true || m_DlgEmailPage->DirtyFlag == true)
	if(m_DlgSystemPage->DirtyFlag == true)
	{m_dirty = true;}

	return m_dirty;
}

void CDlgFtp::OnBnClickedButsave()
{
	// TODO: Add your control notification handler code here
	//if (m_curTabPage ==GENERALPAGE)
		vSaveSystem();
	/*else if (m_curTabPage ==FTPPAGE)
		vSaveFtp();
	else if (m_curTabPage ==EMAILPAGE)
		vSaveEmail();*/
	
	m_DlgSystemPage->DirtyFlag = false;
	//m_DlgFtpPage->DirtyFlag = false;
	//m_DlgEmailPage->DirtyFlag = false;
}

BOOL CDlgFtp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	//vCheckDirty();

	switch ( pMsg->message )
	{
		case WM_KEYDOWN:
		{
			switch( pMsg->wParam )
			{
				case VK_ESCAPE:
						//if(m_DlgSystemPage->DirtyFlag == true || m_DlgFtpPage->DirtyFlag == true || m_DlgEmailPage->DirtyFlag == true)
						if(m_DlgSystemPage->DirtyFlag == true)
						{
							//CString ls_confirm = _T("Data Already Modified, Would you want save Data ?");
							//LOADSTRING(ls_confirm, IDS_SAVE_DATA);
							int ll_ret = AfxMessageBox(IDS_SAVE_DATA, MB_YESNO|MB_ICONQUESTION);		
							if (ll_ret==IDYES) 	
							{				
								OnBnClickedButsave();
							}
							else
							{
								CDialog::OnCancel();
								return true;
							}
							::DispatchMessage( pMsg );
						}

				break;
			} //switch
		} // Case WM_KEYDOWN:
	}

	return CBaseDialog::PreTranslateMessage(pMsg);
}
void CDlgFtp::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	//if(m_DlgSystemPage->DirtyFlag == true || m_DlgFtpPage->DirtyFlag == true || m_DlgEmailPage->DirtyFlag == true)
	if(m_DlgSystemPage->DirtyFlag == true)
	{
		//CString ls_confirm = _T("Data Already Modified, Would you want save Data ?");
		int ll_ret = AfxMessageBox(IDS_SAVE_DATA, MB_YESNO|MB_ICONQUESTION);		
		if (ll_ret==IDYES) 	
		{				
			OnBnClickedButsave();
		}
	}

	m_DlgSettingLoginAuth->mapUserPwd.clear();

	CDialog::OnCancel();
}

//void CDlgFtp::OnSize(UINT nType, int cx, int cy)
//{
//	CDialog::OnSize(nType, cx, cy);
//}
void CDlgFtp::OnMove(int x, int y)
{
	/*CBaseDialog::OnMove(x, y);

	CRect crTab;
	int nYGap(20);
	m_mainTab->GetWindowRect(crTab);
	crTab.top += nYGap;
	m_DlgSystemPage->MoveWindow(crTab);
	m_DlgFtpPage->MoveWindow(crTab);
	m_DlgEmailPage->MoveWindow(crTab);*/
}

//void CDlgFtp::OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct)
//{
//    // TODO: Add your message handler code here and/or call default
//
//    CBaseDialog::OnDeleteItem(nIDCtl, lpDeleteItemStruct);
//}

void CDlgFtp::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    #define RED     RGB(245,245,245)
    #define YELLOW  RGB(245,245,245)
    #define MAGENTA RGB(245,245,245)
    #define WHITE   RGB(245,245,245)
    #define BLUE    RGB(245,245,245)

    CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
    static CBrush m_brRed(RED),m_brYellow(YELLOW),m_brMagenta(MAGENTA), m_brWhite(WHITE),m_brBlue(BLUE);

          TCHAR       szTabText[100];
          RECT        rect;
          UINT        bkColor;
          CBrush      *cbr;
          TC_ITEM     tci;

          CTabCtrl    *pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_TABFTP);

          if (pTabCtrl->m_hWnd == lpDrawItemStruct->hwndItem)
          {
              // which tab?
              switch (lpDrawItemStruct->itemID)
              {
              case 0:
                  cbr = &m_brRed;
                  bkColor = RED;
                  break;

              case 1:
                  cbr = &m_brYellow;
                  bkColor = YELLOW;
                  break;

              case 2:
                  cbr = &m_brMagenta;
                  bkColor = MAGENTA;
                  break;

              case 3:
                  cbr = &m_brWhite;
                  bkColor = WHITE;
                  break;

              case 4:
                  cbr = &m_brBlue;
                  bkColor = BLUE;
                  break;
              }

              memset(szTabText, '\0', sizeof(szTabText));

              tci.mask        = TCIF_TEXT;
              tci.pszText     = szTabText;
              tci.cchTextMax  = sizeof(szTabText)-1;

              pTabCtrl->GetItem(lpDrawItemStruct->itemID, &tci);

              CDC *dc = CDC::FromHandle(lpDrawItemStruct->hDC);

              dc->FillRect(&lpDrawItemStruct->rcItem, cbr);
              dc->SetBkColor(bkColor);

              TextOut(lpDrawItemStruct->hDC,
                      lpDrawItemStruct->rcItem.left+6,
                      lpDrawItemStruct->rcItem.top,
                      tci.pszText,
                      lstrlen(tci.pszText));
          }
}
