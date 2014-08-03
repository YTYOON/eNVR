// DlgAcEmpEdit.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAcEmpEdit.h"
#include "TabpageAcEmp.h"
#include "MainFrm.h"

#define DEFAULTFROM "09:00"
#define DEFAULTTO "17:00"


// CDlgAcEmpEdit dialog

IMPLEMENT_DYNAMIC(CDlgAcEmpEdit, CBaseDialog)

CDlgAcEmpEdit::CDlgAcEmpEdit(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAcEmpEdit::IDD, pParent)
{

}

CDlgAcEmpEdit::~CDlgAcEmpEdit()
{
	for (int i=0; i<m_vtEmp.size(); i++)
	{
		if (m_vtEmp[i]) delete m_vtEmp[i];
	}
}

void CDlgAcEmpEdit::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_STPIC, m_pic);
	DDX_Control(pDX, IDC_BUTPIC, m_butpic);
	DDX_Control(pDX, IDC_STPIC2, m_pic2);
	DDX_Control(pDX, IDC_BUTPICDEL, m_butdel);
}


BEGIN_MESSAGE_MAP(CDlgAcEmpEdit, CBaseDialog)
	ON_BN_CLICKED(IDC_BUTMORE, &CDlgAcEmpEdit::OnBnClickedMore)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTPIC, &CDlgAcEmpEdit::OnBnClickedButpic)
	ON_BN_CLICKED(IDC_BUTPICDEL, &CDlgAcEmpEdit::OnBnClickedButpicdel)
END_MESSAGE_MAP()


// CDlgAcEmpEdit message handlers

BOOL CDlgAcEmpEdit::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	GetDlgItem(IDC_COMBODEPT)->ShowWindow(0);
 
	if (m_entryMode == _T("DA") || m_entryMode == _T("DM"))		//Department Mode
	{		
				GetDlgItem(IDC_STEMP)->ShowWindow(0);
				GetDlgItem(IDC_STID)->ShowWindow(0);
				GetDlgItem(IDC_EDITEMP)->ShowWindow(0);
				GetDlgItem(IDC_EDITID)->ShowWindow(0);
			
				if (m_entryMode == _T("DM")) //Department Modify
				{
					((CEdit*)GetDlgItem(IDC_EDITDEPT))->SetWindowTextW(m_dept);
					((CEdit*)GetDlgItem(IDC_COMBOFROM))->SetWindowTextW(m_from);
					((CEdit*)GetDlgItem(IDC_COMBOTO))->SetWindowTextW(m_to);
				}
				else if (m_entryMode == _T("DA")) //Department Add
				{
					((CEdit*)GetDlgItem(IDC_COMBOFROM))->SetWindowTextW(_T("09:00"));
					((CEdit*)GetDlgItem(IDC_COMBOTO))->SetWindowTextW(_T("17:00"));
				}
				m_butpic.LoadBitmaps(IDB_EMP, IDB_EMP, IDB_EMP, IDB_EMP);
				m_butpic.ShowWindow(false);
				m_butdel.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL);
				m_butdel.ShowWindow(false);
				m_pic2.ShowWindow(false);
				GetDlgItem(IDC_STNOTE)->ShowWindow(false);

	}
	else	//Employee Mode
	{		
				//Create folder AC
				CString ls_appPath = ((CMainFrame*)AfxGetMainWnd())->m_csAppPath;
				CString ls_name = ls_appPath +_T("ac\\"); 
						
				SHCreateDirectoryEx(NULL, ls_name, NULL);

				//disable from and to
				((CEdit*)GetDlgItem(IDC_COMBOFROM))->EnableWindow(false);
				((CEdit*)GetDlgItem(IDC_COMBOTO))->EnableWindow(false);
				((CEdit*)GetDlgItem(IDC_COMBOFROM))->SetWindowTextW(m_from);
					((CEdit*)GetDlgItem(IDC_COMBOTO))->SetWindowTextW(m_to);
				GetDlgItem(IDC_EDITDEPT)->EnableWindow(0);
				GetDlgItem(IDC_EDITDEPT)->SetWindowTextW(m_dept);
				((CEdit*)GetDlgItem(IDC_EDITEMP))->SetFocus();
				if (m_entryMode == _T("EA")) //Add New employee
				{
					(CButton*)GetDlgItem(IDC_BUTMORE)->ShowWindow(true);
				}
				else if (m_entryMode == _T("EM"))	//Modify employee
				{
					((CEdit*)GetDlgItem(IDC_EDITEMP))->SetWindowTextW(m_empname);
					((CEdit*)GetDlgItem(IDC_EDITID))->SetWindowTextW(m_empid);
					((CEdit*)GetDlgItem(IDC_EDITEMP))->SetSel(0, 0);
				}
				
				m_butpic.LoadBitmaps(IDB_EMP, IDB_EMP, IDB_EMP, IDB_EMP);
				((CStatic*)GetDlgItem(IDC_STATPIC))->SetWindowTextW(m_picFile);

				 m_butdel.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL);
				if (m_picFile!=_T(""))
					fnLoadBmp(m_picFile);
 	}

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CDlgAcEmpEdit::OnOK()
{
	//GetControl values into valiables
	((CEdit*)GetDlgItem(IDC_EDITDEPT))->GetWindowTextW(m_dept);
	((CEdit*)GetDlgItem(IDC_EDITEMP))->GetWindowTextW(m_empname);
 	((CEdit*)GetDlgItem(IDC_EDITID))->GetWindowTextW(m_empid);
	
	((CEdit*)GetDlgItem(IDC_COMBOFROM))->GetWindowTextW(m_from);
	((CEdit*)GetDlgItem(IDC_COMBOTO))->GetWindowTextW(m_to);

	if (m_entryMode==_T("DA") || m_entryMode==_T("DM"))	 //Department
	{
		if (m_dept.Trim()==  _T(""))
		{
			AfxMessageBox(_T("Department can not be empty."));
			return;
		}
	}
	else if (m_entryMode==_T("EA") || m_entryMode==_T("EM"))	 //Employee
	{
		if (m_empname.Trim() == _T(""))
		{
			AfxMessageBox(_T("Employee can not be empty."));
			return;
		}

		if (m_entryMode==_T("EA"))	//Add employee
		{
				CString lsAcid, lsEmp;		
				STREMP* lEmp = new STREMP;
				((CEdit*)GetDlgItem(IDC_EDITEMP))->GetWindowTextW(lsEmp);
				lEmp->csEmp = lsEmp.Trim() + '\0';
				wcscpy(lEmp->picFile, m_picFile.Trim() + '\0');

				((CEdit*)GetDlgItem(IDC_EDITID))->GetWindowTextW(lsAcid);
				lEmp->csAcid = lsAcid.Trim() + '\0';
				m_vtEmp.push_back(lEmp);
		}
		
	}

	CBaseDialog::OnOK();
}

BOOL CDlgAcEmpEdit::OnEraseBkgnd(CDC* pDC)
{
	
	return CBaseDialog::OnEraseBkgnd(pDC);
}

void CDlgAcEmpEdit::OnBnClickedMore()
{
	//Check if name is empty
	CString lsEmp;
	((CEdit*)GetDlgItem(IDC_EDITEMP))->GetWindowTextW(lsEmp);
	if (lsEmp == _T(""))
	{
		AfxMessageBox(_T("Employee can not be blank"));
		return;
	}

	CString lsAcid;
	STREMP* lEmp = new STREMP;

	lEmp->csEmp = lsEmp.Trim() + '\0';
	wcscpy(lEmp->picFile, m_picFile.Trim() + '\0');   
	((CEdit*)GetDlgItem(IDC_EDITID))->GetWindowTextW(lsAcid);
	lEmp->csAcid = lsAcid.Trim() + '\0';
	m_vtEmp.push_back(lEmp);

	//Resume blank for edit control
	((CEdit*)GetDlgItem(IDC_EDITEMP))->SetWindowTextW(_T(""));
	((CEdit*)GetDlgItem(IDC_EDITID))->SetWindowTextW(_T(""));
	((CEdit*)GetDlgItem(IDC_STATPIC))->SetWindowTextW(_T(""));
	m_pic2.ShowWindow(SW_HIDE);

	((CEdit*)GetDlgItem(IDC_EDITEMP))->SetFocus();

}

void CDlgAcEmpEdit::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CBaseDialog::OnClose();
}

//Add modify picture
void CDlgAcEmpEdit::OnBnClickedButpic()
{
		CString	ls_tfolder =_T("");
 
		CFileDialog fOpenDlg(TRUE, _T("jpg"), _T("JPG File"), OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, 	_T("JPEG(*.jpg)|*.jpg|"), this);
	  
		fOpenDlg.m_pOFN->lpstrTitle=_T("Select JPG File");
		CString ls_appPath = ((CMainFrame*)AfxGetMainWnd())->m_csAppPath;
 
		if (ls_appPath !=_T(""))
		{
				ls_appPath += _T("ac\\");
				fOpenDlg.m_pOFN->lpstrInitialDir = ls_appPath ;
		}
		else
				fOpenDlg.m_pOFN->lpstrInitialDir=_T("c:");
	  
		if(fOpenDlg.DoModal()==IDOK)
		{
				CString ls_path = fOpenDlg.GetPathName();
				CString ls_name = fOpenDlg.GetFileName();
				((CStatic*)GetDlgItem(IDC_STATPIC))->SetWindowTextW(ls_name);
				mb_picChanged = true;
				m_picFile = ls_name;
			
				//CString ls_path = fOpenDlg.GetPathName();
				//CString ls_name = fOpenDlg.GetFileName();
				CString ls_sfile, ls_tfile;
	
				//((CEdit*)GetDlgItem(IDC_EDITMAP))->SetWindowTextW(ls_name.Trim());

	 		//	((CEdit*)GetDlgItem(IDC_EDITFOLDER))->GetWindowTextW(ls_tfolder);
				ls_sfile.Format(_T("%s%s"),ls_appPath, ls_name);		
				bool ls_ret = CopyFile(ls_path,ls_sfile, false); 
				fnLoadBmp(ls_name);			


		}

}

bool CDlgAcEmpEdit::fnLoadBmp(CString o_file)
{
		CString ls_appPath = ((CMainFrame*)AfxGetMainWnd())->m_csAppPath;

		HBITMAP hbm = (HBITMAP) m_pic2.GetBitmap();
		BITMAP bm;
		SIZE szBitmap;
		GetObject(hbm, sizeof(bm), &bm);
		CBitmap hbmp;
		 HBITMAP hbitmap;
	
		 //Load JPG File
		 o_file.Format(_T("%sac\\%s"), ls_appPath, o_file);
		m_pic2.Load(o_file);
		m_pic2.ShowWindow(SW_SHOW);

		return false;
}


void CDlgAcEmpEdit::OnBnClickedButpicdel()
{
		//if there's no pic, return
		CString	ls_pic;
		GetDlgItem(IDC_STATPIC)->GetWindowTextW(ls_pic);
		if (ls_pic == _T("")) return;

		m_pic2.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATPIC)->SetWindowTextW(_T(""));
		m_picFile = _T("");
}
