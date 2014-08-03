// DlgSettingSystem.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSettingSystem.h"
#include "MainFrm.h"
#include "Winsvc.h"

#define DATABASE_FOLDER		_T("C:\\database\\")
#define DATABASE_EXT_NAME	_T("bak")
#define REG_EXT_NAME	_T("reg")

// CDlgSettingSystem dialog

IMPLEMENT_DYNAMIC(CDlgSettingSystem, CBaseDialog)

CDlgSettingSystem::CDlgSettingSystem(CWnd* pParent)	: CBaseDialog(CDlgSettingSystem::IDD, pParent) , DirtyFlag(false),m_bEnableRun(true),m_ini(ECINI)
,m_nLocaleID(-1),m_nLocaleIDInit(-1),m_bEnableFullScreen(false),m_bEnableWizard(false),m_pParent(pParent)
{

}

CDlgSettingSystem::~CDlgSettingSystem()
{
}

void CDlgSettingSystem::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AUTORECORDING, m_AutoRecording);
	DDX_Control(pDX, IDC_COMBOSTREAM, m_Stream);
}


BEGIN_MESSAGE_MAP(CDlgSettingSystem, CBaseDialog)
	ON_BN_CLICKED(IDOK, &CDlgSettingSystem::OnBnClickedOk)
//	ON_WM_ERASEBKGND()
//	ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_SETTING_SYSTEM_RUN, &CDlgSettingSystem::OnBnClickedSettingSystemRun)
ON_CBN_SELCHANGE(IDC_COMBOSTREAM, &CDlgSettingSystem::OnCbnSelchangeCombostream)
//ON_BN_CLICKED(IDC_CHECK1, &CDlgSettingSystem::OnBnClickedCheck1)
ON_BN_CLICKED(IDC_RADIO_ENGLISH, &CDlgSettingSystem::OnBnClickedRadioEnglish)
ON_BN_CLICKED(IDC_RADIO_RUSSIAN, &CDlgSettingSystem::OnBnClickedRadioRussian)
ON_BN_CLICKED(IDC_RADIO_CHINESE_TRAN, &CDlgSettingSystem::OnBnClickedRadioChineseTran)
ON_BN_CLICKED(IDC_CHINESE_SIMP, &CDlgSettingSystem::OnBnClickedChineseSimp)
ON_BN_CLICKED(IDC_CHECK_SETTING_SYSTEM_FULLSCREEN, &CDlgSettingSystem::OnBnClickedCheckSettingSystemFullscreen)
ON_BN_CLICKED(IDC_CHECK_SETTING_SYSTEM_WIZARD, &CDlgSettingSystem::OnBnClickedCheckSettingSystemWizard)
ON_BN_CLICKED(IDC_RADIO_GERMAN, &CDlgSettingSystem::OnBnClickedRadioGerman)
ON_BN_CLICKED(IDC_RADIO_FRENCH, &CDlgSettingSystem::OnBnClickedRadioFrench)
ON_BN_CLICKED(IDC_RADIO_SPANISH, &CDlgSettingSystem::OnBnClickedRadioSpanish)
ON_BN_CLICKED(IDC_EXPBTN, &CDlgSettingSystem::OnBnClickedExpbtn)
ON_BN_CLICKED(IDC_IMPBTN, &CDlgSettingSystem::OnBnClickedImpbtn)
END_MESSAGE_MAP()


// CDlgSettingSystem message handlers
void CDlgSettingSystem::ChangeControlLaguage()
{
	CString str(_T(""));
	LOADSTRING(str, IDS_DLG_SETTING_SYSTEM);
	this->SetWindowTextW(str);

	LOADSTRING(str, IDC_EXPBTN);
	SetDlgItemText(IDC_EXPBTN,str);

	LOADSTRING(str, IDC_IMPBTN);
	SetDlgItemText(IDC_IMPBTN,str);
}

BOOL CDlgSettingSystem::OnInitDialog()
{

	CBaseDialog::OnInitDialog();

	ChangeControlLaguage();
	
	m_ini.setINIFullFileName(_T("\ec.ini"));
	CString ls_protocol, ls_autorecord;

	//Protocol
	
	ls_protocol = m_ini.getKeyValue(_T("protocol"),_T("Video"));
	CComboBox* pComboPcol =(CComboBox*) GetDlgItem(IDC_COMBOSTREAM);
	CString ls_value;

	CString* lp_value = new CString;
	lp_value = &ls_value;
	ls_value = _T("");
	pComboPcol->AddString(ls_value);
	pComboPcol->SetItemData(0, (DWORD_PTR)lp_value);
	ls_value = _T("AVI");
	pComboPcol->AddString(ls_value);
	pComboPcol->SetItemData(2, (DWORD_PTR)lp_value);
	ls_value = _T("CGI");
	pComboPcol->AddString(ls_value);
	pComboPcol->SetItemData(1, (DWORD_PTR)lp_value);
	ls_value = _T("RTSP");
	pComboPcol->AddString(ls_value);
	pComboPcol->SetItemData(1, (DWORD_PTR)lp_value);
	ls_value = _T("TCP");
	pComboPcol->AddString(ls_value);
	pComboPcol->SetItemData(1, (DWORD_PTR)lp_value);


 	int nIndex = pComboPcol->FindString(0,ls_protocol);
	pComboPcol->SetCurSel(nIndex);

	// Set AutoRun  checked status according to Registry 
	m_bEnableRun = m_gs.bGetRun();
	((CButton*)GetDlgItem(IDC_SETTING_SYSTEM_RUN))->SetCheck(m_bEnableRun);

	/*LOADSTRING(csMsg,IDS_SETTING_SYSTEM_RUN);
	GetDlgItem(IDC_SETTING_SYSTEM_RUN)->SetWindowTextW(csMsg);*/
	SETITEMTEXT(IDC_SETTING_SYSTEM_RUN,IDS_SETTING_SYSTEM_RUN);
	SETITEMTEXT(IDC_CHECK_SETTING_SYSTEM_FULLSCREEN,IDS_SETTING_SYSTEM_FULLSCREEN);
	SETITEMTEXT(IDC_CHECK_SETTING_SYSTEM_WIZARD,IDS_WIZARD_SETTING);
	CString csFullScreen;
	csFullScreen = m_ini.getKeyValue(_T("RunFullScreen"),_T("system"));
	if( csFullScreen == _T("1") )
	{
		m_bEnableFullScreen = true;		
	}
	((CButton*)GetDlgItem(IDC_CHECK_SETTING_SYSTEM_FULLSCREEN))->SetCheck(m_bEnableFullScreen);

	CString csWiz;
	csWiz = m_ini.getKeyValue(_T("EnableWizard"),_T("system"));
	if( csWiz == _T("1") )
	{
		m_bEnableWizard = true;		
	}
	((CButton*)GetDlgItem(IDC_CHECK_SETTING_SYSTEM_WIZARD))->SetCheck(m_bEnableWizard);
	
	////--Get Language from registry
	//switch(dwGetLanguageFromRegistry())
	//{
	//case ENGLISH:

	//	((CButton*)GetDlgItem(IDC_RADIO_ENGLISH))->SetCheck(1);
	//	((CButton*)GetDlgItem(IDC_RADIO_DEFAULT))->SetCheck(0);

	//	break;
	//case DEFAULT:

	//	((CButton*)GetDlgItem(IDC_RADIO_ENGLISH))->SetCheck(0);
	//	((CButton*)GetDlgItem(IDC_RADIO_DEFAULT))->SetCheck(1);

	//	break;
	//default:
	//	ASSERT(false);
	//	break;
	//};
    //=========Initialize Language radio buttons==============================
	((CButton*)GetDlgItem(IDC_RADIO_ENGLISH))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_GERMAN))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_RUSSIAN))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_CHINESE_TRAN))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHINESE_SIMP))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_FRENCH))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_SPANISH))->SetCheck(0);

	CString csMsg;
	LOADSTRING(csMsg,IDS_LANGUAGE_ENGLISH);
	((CButton*)GetDlgItem(IDC_RADIO_ENGLISH))->SetWindowTextW(csMsg);
	LOADSTRING(csMsg,IDS_LANGUAGE_RUSSIAN);
	((CButton*)GetDlgItem(IDC_RADIO_RUSSIAN))->SetWindowTextW(csMsg);
	LOADSTRING(csMsg,IDS_LANGUAGE_CHINESE_TRADITIONAL);
	((CButton*)GetDlgItem(IDC_RADIO_CHINESE_TRAN))->SetWindowTextW(csMsg);
	LOADSTRING(csMsg,IDS_LANGUAGE_CHINESE_SIMPLIFIED);
	((CButton*)GetDlgItem(IDC_CHINESE_SIMP))->SetWindowTextW(csMsg);
	LOADSTRING(csMsg,IDS_LANGUAGE_GERMAN);
	((CButton*)GetDlgItem(IDC_RADIO_GERMAN))->SetWindowTextW(csMsg);
	LOADSTRING(csMsg,IDS_LANGUAGE_FRENCH);
	((CButton*)GetDlgItem(IDC_RADIO_FRENCH))->SetWindowTextW(csMsg);
	LOADSTRING(csMsg,IDS_LANGUAGE_SPANISH);
	((CButton*)GetDlgItem(IDC_RADIO_SPANISH))->SetWindowTextW(csMsg);

	 m_nLangIDCArray [EN] = IDC_RADIO_ENGLISH;
	 m_nLangIDCArray [CH_T] = IDC_RADIO_CHINESE_TRAN;
	 m_nLangIDCArray [CH_S] = IDC_CHINESE_SIMP;
	 m_nLangIDCArray [RU] = IDC_RADIO_RUSSIAN;
	 m_nLangIDCArray [DE] = IDC_RADIO_GERMAN;
	 m_nLangIDCArray [FR] = IDC_RADIO_FRENCH;
	 m_nLangIDCArray [ESMODERN] = IDC_RADIO_SPANISH;

	((CButton*)GetDlgItem(IDC_RADIO_RUSSIAN))->EnableWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO_CHINESE_TRAN))->EnableWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_CHINESE_SIMP))->EnableWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO_GERMAN))->EnableWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO_FRENCH))->EnableWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO_SPANISH))->EnableWindow(SW_HIDE);
	
	vFindDLL(); // find available dlls and show the buttons

	CGS::bGetLocaleID();
	m_nLocaleIDInit = m_nLocaleID = CGS::m_nLocaleID;

	switch(m_nLocaleID)
	{
	case ZHTW_LCID://Taiwan 
		((CButton*)GetDlgItem(IDC_RADIO_CHINESE_TRAN))->SetCheck(1);
		break;
	case ZHCN_LCID:
		((CButton*)GetDlgItem(IDC_CHINESE_SIMP))->SetCheck(1);
		break;
	case RU_LCID:
		((CButton*)GetDlgItem(IDC_RADIO_RUSSIAN))->SetCheck(1);
		break;
	case DE_LCID:
		((CButton*)GetDlgItem(IDC_RADIO_GERMAN))->SetCheck(1);
		break;
	case FR_LCID:
		((CButton*)GetDlgItem(IDC_RADIO_FRENCH))->SetCheck(1);
		break;
	case ESMODERN_LCID:
		((CButton*)GetDlgItem(IDC_RADIO_SPANISH))->SetCheck(1);
		break;
	case ENUS_LCID:
	default:
		((CButton*)GetDlgItem(IDC_RADIO_ENGLISH))->SetCheck(1);
		break;	
	}
   //=======================================================================

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSettingSystem::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CString	ls_protocol, ls_key, ls_auto;
	CString	*lp_key, lp_value;
	GetDlgItem(IDC_COMBOSTREAM)->GetWindowTextW(ls_protocol);
	
	int ll_check = m_AutoRecording.GetCheck();
	CString ls_check = _T("Y");

	if (ll_check==BST_UNCHECKED) ls_check=_T("N"); 
 	
	m_ini.setKey(ls_protocol,_T("protocol"),_T("Video"));
	//m_ini.setKey(ls_check,_T("autorecording"),_T("Video"));

	// Save AutoRun status to registry
	m_gs.bSetRun(m_bEnableRun);
	CGS::bSetLocaleID(m_nLocaleID);
   
	CBaseDialog::OnOK();
}

void CDlgSettingSystem::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

//BOOL CDlgSettingSystem::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	return CBaseDialog::OnEraseBkgnd(pDC);
//}

//HBRUSH CDlgSettingSystem::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  Change any attributes of the DC here
//
//	// TODO:  Return a different brush if the default is not desired
//	return hbr;
//}

bool CDlgSettingSystem::vSaveSystem(void)
{
	// TODO: Add your specialized code here and/or call the base class

	CString	ls_protocol, ls_key, ls_auto;
	CString	*lp_key, lp_value;
	GetDlgItem(IDC_COMBOSTREAM)->GetWindowTextW(ls_protocol);
	
//	int ll_check = m_AutoRecording.GetCheck();
//	CString ls_check = _T("Y");
//	if (ll_check==BST_UNCHECKED) ls_check=_T("N"); 	
	m_ini.setKey(ls_protocol.Trim(),_T("protocol"),_T("Video"));
	//m_ini.setKey(ls_check,_T("autorecording"),_T("Video"));
   
	// save fullscreen to INI
	CString csFull(_T("0"));
	if(m_bEnableFullScreen)
	{
		csFull = _T("1");
	}
	m_ini.setKey(csFull,_T("RunFullScreen"),_T("system"));

	// save Wizard to INI
	CString csWiz(_T("0"));
	if(m_bEnableWizard)
	{
		csWiz = _T("1");
	}
	m_ini.setKey(csWiz,_T("EnableWizard"),_T("system"));
	
	// Save AutoRun status to registry
	m_gs.bSetRun(m_bEnableRun);

	//CBaseDialog::OnOK();

	/*if(((CButton*)GetDlgItem(IDC_RADIO_ENGLISH))->GetCheck())
	{
		if( !bSaveLanguageToRegistry(ENGLISH))
		{
			TRACE(_T("Save Language Error!"));
		}

	}else if(((CButton*)GetDlgItem(IDC_RADIO_DEFAULT))->GetCheck())
	{
		if( !bSaveLanguageToRegistry(DEFAULT))
		{
			TRACE(_T("Save Language Error!"));
		}

	}else
	{
		if( !bSaveLanguageToRegistry(DEFAULT))
		{
			TRACE(_T("Save Language Error!"));
		}
	}*/
	if(m_nLocaleID != -1)
	{
		CGS::bSetLocaleID(m_nLocaleID);
	}
	
	if(m_nLocaleIDInit != m_nLocaleID && m_nLocaleIDInit != -1)
	{
		CString csMsg;
		LOADSTRING(csMsg,IDS_HINT_CHANGE_LANGUAGE);
		if(AfxMessageBox(csMsg,MB_YESNO) == IDYES)
		{
			RestarProcess();
		}		
	}

	DirtyFlag = false;
	return false;
}

BOOL CDlgSettingSystem::PreTranslateMessage(MSG *pMsg)
{
	if   (pMsg-> message   ==   WM_KEYDOWN) 
    { 
        if   (pMsg-> wParam   ==   VK_RETURN   ||   pMsg-> wParam   ==   VK_ESCAPE) 
            return   TRUE; 
    } 

    return   CDialog::PreTranslateMessage(pMsg); 
}
void CDlgSettingSystem::OnBnClickedSettingSystemRun()
{
	// TODO: Add your control notification handler code here

	bool bRet = true;
	bRet = ((CButton*)GetDlgItem(IDC_SETTING_SYSTEM_RUN))->GetCheck();
	m_bEnableRun = bRet;
	DirtyFlag = true;

	//AfxMessageBox(_T("dirty"));
}

void CDlgSettingSystem::OnCbnSelchangeCombostream()
{
	// TODO: Add your control notification handler code here
	DirtyFlag = true;
}

void CDlgSettingSystem::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
}

bool CDlgSettingSystem::bSaveLanguageToRegistry(int iLanguage)
{
	CRegKey regKey;
	if( regKey.Open(HKEY_LOCAL_MACHINE, REGISTSUBFOLDER) != ERROR_SUCCESS)
	{
		TRACE(_T("CRegKey Open Error!"));
		return false;
	}

	DWORD dwLanguage = 0;

	switch(iLanguage)
	{
	case ENGLISH:
		dwLanguage = ENGLISH;
		regKey.SetDWORDValue(_T("SystemLanguage"), dwLanguage);
		break;
	case DEFAULT:
		dwLanguage = DEFAULT;
		regKey.SetDWORDValue(_T("SystemLanguage"), dwLanguage);
		break;
	default:
		ASSERT(false);
		break;
	};

	regKey.Close();
	return true;
}

DWORD CDlgSettingSystem::dwGetLanguageFromRegistry()
{
	DWORD dwLanguage = 0;
	CRegKey regKey;
	if( regKey.Open(HKEY_LOCAL_MACHINE, REGISTSUBFOLDER) != ERROR_SUCCESS)
	{
		TRACE(_T("CRegKey Open Error!"));
		return DEFAULT;
	}

	regKey.QueryDWORDValue(_T("SystemLanguage"), dwLanguage);
	regKey.Close();

	return dwLanguage;
}
void CDlgSettingSystem::OnBnClickedRadioEnglish()
{

	for(int i=0;i< numOfLang;i++)
	{
		DWORD nIDC;
		nIDC = m_nLangIDCArray[i];
		if(i == EN)
		{
			((CButton*)GetDlgItem(nIDC))->SetCheck(1);
		}
		else
		{
			((CButton*)GetDlgItem(nIDC))->SetCheck(0);
		}
	}

	m_nLocaleID = ENUS_LCID;

}

void CDlgSettingSystem::OnBnClickedRadioRussian()
{
	for(int i=0;i< numOfLang;i++)
	{
		DWORD nIDC;
		nIDC = m_nLangIDCArray[i];
		if(i == RU)
		{
			((CButton*)GetDlgItem(nIDC))->SetCheck(1);
		}
		else
		{
			((CButton*)GetDlgItem(nIDC))->SetCheck(0);
		}
	}

	m_nLocaleID = RU_LCID;
	
}

void CDlgSettingSystem::OnBnClickedRadioChineseTran()
{
	for(int i=0;i< numOfLang;i++)
	{
		DWORD nIDC;
		nIDC = m_nLangIDCArray[i];
		if(i == CH_T)
		{
			((CButton*)GetDlgItem(nIDC))->SetCheck(1);
		}
		else
		{
			((CButton*)GetDlgItem(nIDC))->SetCheck(0);
		}
	}
	m_nLocaleID = ZHTW_LCID;
	
}

void CDlgSettingSystem::OnBnClickedChineseSimp()
{
	for(int i=0;i< numOfLang;i++)
	{
		DWORD nIDC;
		nIDC = m_nLangIDCArray[i];
		if(i == CH_S)
		{
			((CButton*)GetDlgItem(nIDC))->SetCheck(1);
		}
		else
		{
			((CButton*)GetDlgItem(nIDC))->SetCheck(0);
		}
	}
	m_nLocaleID = ZHCN_LCID;
}
void CDlgSettingSystem::OnBnClickedRadioGerman()
{
	for(int i=0;i< numOfLang;i++)
	{
		DWORD nIDC;
		nIDC = m_nLangIDCArray[i];
		if(i == DE)
		{
			((CButton*)GetDlgItem(nIDC))->SetCheck(1);
		}
		else
		{
			((CButton*)GetDlgItem(nIDC))->SetCheck(0);
		}
	}
	m_nLocaleID = DE_LCID;
}

void CDlgSettingSystem::OnBnClickedRadioFrench()
{
	for(int i=0;i< numOfLang;i++)
	{
		DWORD nIDC;
		nIDC = m_nLangIDCArray[i];
		if(i == FR)
		{
			((CButton*)GetDlgItem(nIDC))->SetCheck(1);
		}
		else
		{
			((CButton*)GetDlgItem(nIDC))->SetCheck(0);
		}
	}
	m_nLocaleID = FR_LCID;
}

void CDlgSettingSystem::OnBnClickedRadioSpanish()
{
	for(int i=0;i< numOfLang;i++)
	{
		DWORD nIDC;
		nIDC = m_nLangIDCArray[i];
		if(i == ESMODERN)
		{
			((CButton*)GetDlgItem(nIDC))->SetCheck(1);
		}
		else
		{
			((CButton*)GetDlgItem(nIDC))->SetCheck(0);
		}
	}
	m_nLocaleID = ESMODERN_LCID;
}

void CDlgSettingSystem::vFindDLL()
{
	bool bRet = false;
	CFileFind cFinder;
	CString csFileName;
	CString csAppPath;

	TCHAR apppath[MAX_PATH]={0};
    GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
    csAppPath.Format(_T("%s"),apppath);
    int nPos = csAppPath.ReverseFind('\\');
    csAppPath = csAppPath.Left(nPos + 1); 
	csAppPath+= _T("Lingual*.dll");	

	bRet = cFinder.FindFile(csAppPath);
		
	while(bRet)
	{
		bRet = cFinder.FindNextFileW();	
		csFileName = cFinder.GetFileName();
		vDisplayLanguageOption(csFileName);			
	}
}

void CDlgSettingSystem::vDisplayLanguageOption(CString csFileName)
{
	if(csFileName == _T("Lingual0404.dll"))
	{
		((CButton*)GetDlgItem(IDC_RADIO_CHINESE_TRAN))->EnableWindow(SW_SHOW);
	}

	if(csFileName == _T("Lingual0419.dll"))
	{
		((CButton*)GetDlgItem(IDC_RADIO_RUSSIAN))->EnableWindow(SW_SHOW);
	}

	if(csFileName == _T("Lingual0804.dll"))
	{
		((CButton*)GetDlgItem(IDC_CHINESE_SIMP))->EnableWindow(SW_SHOW);
	}

	if(csFileName == _T("Lingual0407.dll"))
	{
		((CButton*)GetDlgItem(IDC_RADIO_GERMAN))->EnableWindow(SW_SHOW);
	}

	if(csFileName == _T("Lingual040C.dll"))
	{
		((CButton*)GetDlgItem(IDC_RADIO_FRENCH))->EnableWindow(SW_SHOW);
	}

	if(csFileName == _T("Lingual0C0A.dll"))
	{
		((CButton*)GetDlgItem(IDC_RADIO_SPANISH))->EnableWindow(SW_SHOW);
	}

}
void CDlgSettingSystem::OnBnClickedCheckSettingSystemFullscreen()
{
	bool bRet = false;
	bRet = ((CButton*)GetDlgItem(IDC_CHECK_SETTING_SYSTEM_FULLSCREEN))->GetCheck();	
	m_bEnableFullScreen = bRet;	
	DirtyFlag = true;
	
}

void CDlgSettingSystem::OnBnClickedCheckSettingSystemWizard()
{
	bool bRet = false;
	bRet = ((CButton*)GetDlgItem(IDC_CHECK_SETTING_SYSTEM_WIZARD))->GetCheck();	
	m_bEnableWizard = bRet;	
	DirtyFlag = true;
}

//////////////////////////////////////////////////////////////////////////Lynn for Database Exp/Imp
void CDlgSettingSystem::GetDatabaseFileName(CString& strFileName)
{
	if (CNVRBk::GetBkObject()->GetOEMCompany() == Etro_Company)
		strFileName = _T("etrocenter");
	else
		strFileName = _T("cms");
}

bool CDlgSettingSystem::IsFileExist()
{
	int nResult = IDYES;
	CString	strFileBakPath(_T("")), strFileName(_T("")),strFileRegPath(_T("")),strMsg(_T(""));

	GetDatabaseFileName(strFileName);
	strFileBakPath.Format(_T("%s%s.%s"), DATABASE_FOLDER, strFileName, DATABASE_EXT_NAME);
	strFileRegPath.Format(_T("%s%s.%s"), DATABASE_FOLDER, strFileName, REG_EXT_NAME);

	if (GetFileAttributes(strFileBakPath) != INVALID_FILE_ATTRIBUTES)
	{
		if (GetFileAttributes(strFileRegPath) != INVALID_FILE_ATTRIBUTES)
			strMsg.Format(_T("The file \"%s.%s\" and \"%s.%s\" already exists.\nDo you want to replace the existing file?"), strFileName, DATABASE_EXT_NAME, strFileName, REG_EXT_NAME);
		else
			strMsg.Format(_T("The file \"%s.%s\" already exists.\nDo you want to replace the existing file?"), strFileName, DATABASE_EXT_NAME);
		nResult = AfxMessageBox(strMsg, MB_YESNO);
	}

	if (nResult == IDYES)
	{
		DeleteFile(strFileBakPath);
		DeleteFile(strFileRegPath);
		return false;
	}
	else
		return true;
}

void CDlgSettingSystem::OnBnClickedExpbtn()
{
	// TODO: Add your control notification handler code here
	
	if (!IsFileExist())
	{
		ExportDatabase();
		ExportReg();
		::ShellExecute( NULL, _T("open"),  DATABASE_FOLDER, NULL, NULL, SW_RESTORE );
	}
}

void CDlgSettingSystem::ExportReg()
{
	char str[128];
	
	USES_CONVERSION;

 	CString	strFileName(_T("")), strFilePath(_T("")), strTmpFilePath(_T(""));
	GetDatabaseFileName(strFileName);
	strFilePath.Format(_T("%s%s.%s"), DATABASE_FOLDER, strFileName, REG_EXT_NAME);
	strTmpFilePath.Format(_T("%s%s.%s"), DATABASE_FOLDER, _T("tmp"), REG_EXT_NAME);
	
	if (GetFileAttributes(strFilePath) != INVALID_FILE_ATTRIBUTES)
		DeleteFile(strFilePath);
	
	sprintf(str, "regedit.exe /e %s%s.%s \"HKEY_CURRENT_USER\\%s\"", W2A(DATABASE_FOLDER), W2A(strFileName), W2A(REG_EXT_NAME), W2A(CProductDefine::m_tzRegistSubFolder) );
	system(str);
	sprintf(str, "regedit.exe /e %s%s.%s \"HKEY_LOCAL_MACHINE\\Software\\Etrovision Technology\"", W2A(DATABASE_FOLDER), W2A(_T("tmp")), W2A(REG_EXT_NAME));
	system(str);

	MergeRegFile(strTmpFilePath, strFilePath);
		
}

void CDlgSettingSystem::MergeRegFile(const CString& strTempFile, const CString& strRegFile)
{
	USES_CONVERSION;
	FILE *pFile = NULL;
	int nTempFileSize = 0, nEtroFileSize = 0;
	char *buffer = NULL;
	pFile = fopen( W2A(strTempFile), "r" );
	if (pFile)
	{	
		fseek(pFile, 0, SEEK_END);
		nTempFileSize = ftell(pFile);
		rewind(pFile);

		buffer = new char[nTempFileSize];
		memset(buffer, 0x00, nTempFileSize);

		for(int n = 0; n < nTempFileSize; n++)
		{
			fread(buffer+n, 1, 1, pFile);
		}

		fclose(pFile);
	}

	pFile = fopen( W2A(strRegFile), "ab+" );
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		nEtroFileSize = ftell(pFile);
		fseek(pFile, nEtroFileSize, SEEK_SET);

		fwrite(buffer, 1, nTempFileSize, pFile);
		fclose(pFile);
	}

	delete [] buffer;
	DeleteFile(strTempFile);
}

void CDlgSettingSystem::ExportDatabase()
{
	USES_CONVERSION;
	char str[128];
	int nResult = IDYES;
	CString	strFilePath(_T("")), strFileName(_T(""));

	GetDatabaseFileName(strFileName);
	strFilePath.Format(_T("%s%s.%s"), DATABASE_FOLDER, strFileName, DATABASE_EXT_NAME);

	if (GetFileAttributes(DATABASE_FOLDER) == INVALID_FILE_ATTRIBUTES) 
	{
		CreateDirectory(DATABASE_FOLDER,NULL);
	}
	sprintf(str, "SQLCMD -E -S (local)\\SQLEXPRESS  -Q \"BACKUP DATABASE %s TO DISK='%s'\"", W2A(strFileName), W2A(strFilePath));
	system(str);
}

bool CDlgSettingSystem::bSelectDatabaseFile(CString& strFilePath)
{
	CString	ls_tfolder(_T("")), strDatabaseFileName(_T("")), strFileName(_T(""));
	bool bResult = false;

	GetDatabaseFileName(strFileName);
	strDatabaseFileName.Format(_T("%s.%s"), strFileName, DATABASE_EXT_NAME);


	CFileDialog fOpenDlg(TRUE, DATABASE_EXT_NAME, strDatabaseFileName, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,_T("Select Database File (*.bak)|*.bak|Select Register File (*.reg)|*.reg|"), this);

	fOpenDlg.m_pOFN->lpstrTitle=_T("Select Database File");
	fOpenDlg.m_pOFN->lpstrInitialDir = DATABASE_FOLDER;

	if(fOpenDlg.DoModal()==IDOK)
	{
		strFilePath = fOpenDlg.GetPathName();
		bResult = true;
	}

	return bResult;
}

void CDlgSettingSystem::ImportDatabase(const CString& strFilePath)
{
	char str[128];
	CString strFileName(_T(""));

	sprintf(str, "net stop mssql$sqlexpress /Y");
	system(str);
	sprintf(str, "net start mssql$sqlexpress /Y");
	system(str);

	USES_CONVERSION;
	GetDatabaseFileName(strFileName);
	sprintf(str, "SQLCMD -E -S (local)\\SQLEXPRESS  -Q \"RESTORE DATABASE %s FROM DISK='%s' WITH REPLACE\"", W2A(strFileName), W2A(strFilePath));
	system(str);

	AfxMessageBox(_T("Restart process, please wait!"),MB_ICONEXCLAMATION);
	
	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::EVENT));
	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::RECORD));
	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::STREAM));
	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::HEALTH));
	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::REMOTE));
	
	RestarProcess();
}

void CDlgSettingSystem::ImportReg(const CString& strFilePath)
{
	char str[128];
	
	USES_CONVERSION;
	sprintf(str, "regedit.exe /s %s", W2A(strFilePath));
	system(str);

	AfxMessageBox(_T("Restart process, please wait!"),MB_ICONEXCLAMATION);

	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::EVENT));
	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::RECORD));
	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::STREAM));
	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::HEALTH));
	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::REMOTE));

	RestarProcess();	
}

void CDlgSettingSystem::RestarProcess()
{
	((CMainFrame*)AfxGetMainWnd())->m_bRestartByHealth = true;
	::PostMessage(((CMainFrame*)AfxGetMainWnd())->m_hWnd,WM_CLOSE,NULL,NULL);
}

void CDlgSettingSystem::OnBnClickedImpbtn()
{
	// TODO: Add your control notification handler code here
	
	CString strFilePath(_T(""));

	if (bSelectDatabaseFile(strFilePath))
	{
		if (strFilePath.Find(REG_EXT_NAME) != -1)
		{
			ImportReg(strFilePath);
		}
		else
		{
			ImportDatabase(strFilePath);
		}
	}
}

void CDlgSettingSystem::RestartService(const CString& str)
{
	bool bCloseSCManager = false;
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager!=0) 
	{
		SC_HANDLE schService = OpenService( schSCManager, str, SERVICE_ALL_ACCESS);
		bool bCloseService = false;
		if (schService!=0) 
		{	
			SERVICE_STATUS status;

			int nRetryStart = 5;
			int nRetryQuery = 3;

			while(nRetryQuery--)
			{			
				SERVICE_STATUS ss;
				if(QueryServiceStatus(schService, &ss))
				{
					if(ss.dwCurrentState == SERVICE_STOPPED)
					{
						nRetryQuery = 0;							
					}
				}
			}

			while(nRetryStart--)
			{
				int nRet= StartService(schService, 0, (const TCHAR**)NULL);
				if( nRet == S_OK)
				{						
					CloseServiceHandle(schService);
					CloseServiceHandle(schSCManager); 

					bCloseService = true;
					bCloseSCManager = true;
					nRetryStart = 0;
				}
			}
		}
		if(!bCloseService)
			CloseServiceHandle(schService); 
	}

	if(!bCloseSCManager)
		CloseServiceHandle(schSCManager);
}
