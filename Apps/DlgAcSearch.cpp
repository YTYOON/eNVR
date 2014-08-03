//	DlgAcSearch.cpp : implementation file
//	Access Control Edit Date

#include "stdafx.h"
#include "DlgAcSearch.h"
#include "TabpageAcHol.h"


// CDlgAcSearch dialog

IMPLEMENT_DYNAMIC(CDlgAcSearch, CBaseDialog)

CDlgAcSearch::CDlgAcSearch(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAcSearch::IDD, pParent)
{

}

CDlgAcSearch::~CDlgAcSearch()
{
}

 
void CDlgAcSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	 
}

BEGIN_MESSAGE_MAP(CDlgAcSearch, CBaseDialog)

	 
END_MESSAGE_MAP()


BOOL CDlgAcSearch::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	//if (m_dept!=_T(""))
	//{
//		GetDlgItem(IDC_EDITDEPT)->SetWindowTextW(m_dept);
//	}
	char	lc;
	if (m_hol.Find('/',0)>0)
		lc = '/';
	else if (m_hol.Find('-',0)>0)
		lc = '-';

	if (m_hol!=_T(""))
	{
		CString ls_yr = m_hol.Mid(0,m_hol.Find(lc,0));
		m_hol = m_hol.Mid(m_hol.Find(lc,0) + 1);
		CString ls_mon = m_hol.Mid(0,m_hol.Find(lc,0));
		m_hol = m_hol.Mid(m_hol.Find(lc,0) + 1);
		CString ls_date = m_hol;

		if (_ttoi(ls_yr )<0 || _ttoi(ls_mon )<0 || _ttoi(ls_date )<0 || ls_yr==_T(""))
			return false;

		CTime ctHol(_ttoi(ls_yr), _ttoi(ls_mon) , _ttoi(ls_date), 0, 0, 0);
		((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKERHOL))->SetTime(&ctHol);
	}
	return TRUE;   
}


void CDlgAcSearch::OnOK()
{
	GetDlgItem(IDC_DATETIMEPICKERHOL)->GetWindowTextW(m_hol);
	if (fnDateExist(m_hol))
	{
		AfxMessageBox(_T("Date already exist"));
		return;
	}

	CBaseDialog::OnOK();
}

bool CDlgAcSearch::fnDateExist(CString oDate)
{
			//Check if date already exist, check from m_parent
			//if exist, return true;
			char	lc;

			int l_found = oDate.Find('/', 0); 
			int l_found1 = oDate.Find('/', l_found + 1); 
			int iy = _ttoi(oDate.Mid(0,4));			
			int im = _ttoi(oDate.Mid(l_found + 1, l_found1-1 - l_found));		
			int id = _ttoi(oDate.Mid(l_found1+ 1, 2));


		for (int i=0; i<m_parent->m_vpHol.size(); i++)
		{
			if (m_parent->m_vpHol[i]->mode == 'D' || m_parent->m_vpHol[i]->mode == 'A') //Delete mode
					continue;

				CString	ls_date;
				ls_date = CString(m_parent->m_vpHol[i]->date_ );
	 
				int l_dbfound, l_dbfound1;
				l_dbfound = ls_date.Find('/', 0); 
				if (l_dbfound<0) // No / char
				{
					l_dbfound = ls_date.Find('-', 0); 
					l_dbfound1 = ls_date.Find('-', l_dbfound + 1);
				}
				else
				{
					l_dbfound1 = ls_date.Find('/', l_dbfound + 1);
				}
					 
				int db_y = _ttoi(ls_date.Mid(0,4));
				int db_m =  _ttoi(ls_date.Mid(l_dbfound+1, l_dbfound1-1 - l_dbfound));		
				int db_d = _ttoi(ls_date.Mid(l_dbfound1+ 1,2));			
		
				if ( db_y== iy && db_m==im  && db_d ==id)
					return true;
	}
	//if (psDB)
	//{
	//		CDBVariant	vargid, varIdx, varAcId, varAcGroupid;		 
	//		CRecordset m_rs(psDB);
	//		CString ls_date, ls_sql;

	//		ls_sql = _T("SELECT date_ FROM ecac_hholiday ");
	//		m_rs.Open(CRecordset::dynaset, ls_sql);
	//  	
	//		while (!m_rs.IsEOF()) { 
	//			int l_found = oDate.Find('/', 0); 
	//			int l_found1 = oDate.Find('/', l_found + 1); 
	//			
	//			if (l_found<0 || l_found>10) return false;

	//			m_rs.GetFieldValue(_T("date_"), ls_date );
	//			int db_y = _ttoi(ls_date.Mid(0,4));
	//			int db_m =  _ttoi(ls_date.Mid(5,2));
	//			int db_d = _ttoi(ls_date.Mid(8,2));			
	//	
	//			int iy = _ttoi(oDate.Mid(0,4));			
	//			int im = _ttoi(oDate.Mid(l_found + 1, l_found1-1 - l_found));		
	//			int id = _ttoi(oDate.Mid(l_found1+ 1, 2));

	//			if ( db_y== iy && db_m==im  && db_d ==id)
	//				return true;
	//			m_rs.MoveNext();
	//		}
	//	
	//}
	return false;
}
