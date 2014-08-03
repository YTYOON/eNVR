#include "stdafx.h"
#include "MainFrm.h"
#include "xNetCardInfo.h"
#include "mycurl.h"
#include "UpnpClient.h"

#define OEM_DLL					_T("Manufacturer.dll")
#define OEM_COMPANY				_T("Company")
#define OEM_XTS					_T("XTS")
#define OEM_CONVISION			_T("CONVISION")
#define OEM_NGO					_T("NGO")
#define OEM_ETRO				_T("Etrovision")
#define OEM_NVR					_T("NVR")
#define EC_INI					_T(".\\ec.ini")

CNVRBk::CNVRBk(void):m_bOpenDatabase(false),m_bATM(false)/*,m_pSqlEx(NULL)*/
{
	Initial();
}

CNVRBk::~CNVRBk(void)
{
	m_DB.Close();
}

CNVRBk* CNVRBk::GetBkObject()
{
	static CNVRBk object;
	return &object;
}

bool CNVRBk::InitialDB()
{
	USES_CONVERSION;
	int nType = GetPrivateProfileInt(_T("system"), _T("Device"),1,EC_INI);

	bool bResult = true;

	int nRetry = 10;
	while (--nRetry > 0)
	{
		try
		{
			CString cpDns,cpUid,cpPwd,csDbName;
			GetPrivateProfileString(_T("database"), _T("DSN"),_T(""),(LPWSTR)(LPCTSTR)cpDns.GetBuffer(255),255,EC_INI);
			GetPrivateProfileString(_T("database"), _T("UID"),_T(""),(LPWSTR)(LPCTSTR)cpUid.GetBuffer(255),255,EC_INI);
			GetPrivateProfileString(_T("database"), _T("PWD"),_T(""),(LPWSTR)(LPCTSTR)cpPwd.GetBuffer(255),255,EC_INI);
			GetPrivateProfileString(_T("database"), _T("DBNAME"),_T(""),(LPWSTR)(LPCTSTR)csDbName.GetBuffer(255),255,EC_INI);

			CString csDBOpenString;
			csDBOpenString.Format(_T("DSN=%s;UID=%s;PWD=%s;LANGUAGE=us_english"), cpDns, cpUid, cpPwd);
			BOOL bRet = m_DB.OpenEx(csDBOpenString, CDatabase::noOdbcDialog);
			if (bRet)
			{
				m_bOpenDatabase = true;
				int nRetryDatabaseStartup(4);
				while (--nRetryDatabaseStartup > 0)
				{
					CString csQuery; 
					csQuery.Format(_T("SELECT state_desc FROM sys.databases WHERE name = '%s'"), csDbName);
					CString csTmp;
					CRecordset recordSet(&m_DB);		
					CDBVariant var;
					recordSet.Open(CRecordset::snapshot, csQuery);
					recordSet.GetFieldValue(_T("state_desc"), csTmp);
					bRet = (csTmp == "ONLINE") ? true : false;
					if (bRet)
						return bRet;
					else
						Sleep(1000);
				}
			}
			cpDns.ReleaseBuffer();
			cpUid.ReleaseBuffer();
			cpPwd.ReleaseBuffer();
			csDbName.ReleaseBuffer();
		}
		catch(CDBException * p)
		{
			p->Delete();
		}
	}
	return bResult;
}

bool CNVRBk::InitialOEM()
{
	bool bResult = false;
	HMODULE hResOem = ::LoadLibrary(OEM_DLL);
	if (hResOem)
	{
		TCHAR tBrand[10],tProductName[10],tATM[10];
		CString strBrand, strProductName, strATM;
		LOADOEMSTRING(hResOem, IDS_BRAND, tBrand);
		LOADOEMSTRING(hResOem, IDS_PRODUCTNAME, tProductName);
		LOADOEMSTRING(hResOem, IDS_ATM, tATM);

		strBrand.Format(_T("%s"),tBrand);
		strProductName.Format(_T("%s"),tProductName);
		strATM.Format(_T("%s"),tATM);

		if (strBrand.Find(OEM_COMPANY) != -1)
		{
			m_byProductCompany = GNC_Company;
		} 
		else if  (strBrand.Find(OEM_XTS) != -1)
		{
			m_byProductCompany = XTS_Company;
		}
		else if  (strBrand.Find(OEM_CONVISION) != -1)
		{
			m_byProductCompany = CVS_Company;
		}
		else if  (strBrand.Find(OEM_NGO) != -1)
		{
			m_byProductCompany = NGO_Company;
		}
		else if  (strBrand.Find(OEM_ETRO) != -1)
		{
			m_byProductCompany = Etro_Company;
		}

		if (strProductName.Find(OEM_NVR) != -1)
		{
			m_byProductType = NVR_Type;
		} 
		else
		{
			m_byProductType = CMS_Type;
		}

		if (!strATM.IsEmpty())
		{
			m_bATM = true;
		}
		bResult = true;
	}

	return bResult;
}


void CNVRBk::TokenStr(const CString& strSec, const CString& strKeyFormat, fpsByProfile& mapValue, LPWSTR lpstr)
{
	wchar_t *wstr = NULL;
	vector<CString> vcStr;
	CString strKey;

	GetPrivateProfileString(strSec, _T("count"),_T(""),lpstr,255,EC_INI);
	int nidx = 0, nCount = 0;
	nCount = _ttoi(lpstr);
	for (nidx = 0; nidx < nCount; nidx++)
	{
		vcStr.clear();
		strKey.Format(strKeyFormat,nidx+1);
		if (GetPrivateProfileString(strSec, strKey,_T(""),lpstr,255,EC_INI))
		{
			wstr = wcstok(lpstr,_T(","));
			while(wstr)
			{
				vcStr.push_back(wstr);
				wstr = wcstok(NULL,_T(","));
			}

			mapValue[strKey] = vcStr;
		}
	}
}
void CNVRBk::TokenStr(const CString& strSec, const CString& strKeyFormat, ResolutionByProfile& mapValue, LPWSTR lpstr)
{
	wchar_t *wstr = NULL;
	wchar_t *wstrProfile= NULL;
	vector<vector<CString> > vcvcStr;
	vector<CString> vcStr;
	CString strKey;

	GetPrivateProfileString(strSec, _T("count"),_T(""),lpstr,255,EC_INI);
	int nidx = 0, nCount = 0;
	nCount = _ttoi(lpstr);
	wstrProfile = new wchar_t[255];
	for (nidx = 0; nidx < nCount; nidx++)
	{
		vcvcStr.clear();
		strKey.Format(strKeyFormat,nidx+1);
		if (GetPrivateProfileString(strSec, strKey,_T(""),lpstr,255,EC_INI))
		{
			wcscpy(wstrProfile, lpstr);
			wstr = wcstok(wstrProfile,_T("-"));
			size_t strlenth = wcslen(wstr);
			while(wstr)
			{
				vcStr.clear();
				wstr = wcstok(wstr,_T(","));
				while(wstr)
				{
					vcStr.push_back(wstr);
					wstr = wcstok (NULL,_T(","));
				}
				wcscpy(wstrProfile, lpstr);
				wstr = wcstok((wstrProfile+strlenth),_T("-"));
				if(wstr)
					strlenth += (wcslen(wstr)+1);
				vcvcStr.push_back(vcStr);
			}

			mapValue[strKey] = vcvcStr;
		}
	}
	delete [] wstrProfile;
}

void CNVRBk::GetFpsBySensor(const CString& strSec, LPWSTR lpstr)
{
	CString strKey;
	strKey.Format(_T("%s"),_T("profile%d_fps"));
	fpsByProfile mapFps;
	TokenStr(strSec, strKey, mapFps, lpstr);
	m_fps[strSec] = mapFps;
}

void CNVRBk::GetResolutionBySensor(const CString& strSec, LPWSTR lpstr)
{
	CString strKey;
	strKey.Format(_T("%s"),_T("profile%d"));
	ResolutionByProfile mapResolution;
	TokenStr(strSec, strKey, mapResolution, lpstr);
	m_Resolutions[strSec] = mapResolution;
}

void CNVRBk::GetBitrate(LPWSTR lpstr)
{
	GetDefaultValue(_T("BitRate"),lpstr,m_mapBitrateByProfileId);
}

void CNVRBk::GetFramerate(LPWSTR lpstr)
{
	GetDefaultValue(_T("FrameRate"),lpstr,m_mapFramerateByProfileId);
}

void CNVRBk::GetDefaultValue(const CString& strSec, LPWSTR lpstr, map<int ,CString >& mapDefaultValue)
{
	CString str;
	GetPrivateProfileString(strSec, _T("count"),_T(""),lpstr,255,EC_INI);
	int nidx = 0, nCount = 0;
	nCount = _ttoi(lpstr);
	for (nidx = 0; nidx < nCount; nidx++)
	{
		str.Format(_T("profile%d"),nidx+1);
		if (GetPrivateProfileString(strSec, str,_T(""),lpstr,255,EC_INI))
		{
			mapDefaultValue[nidx+1] = lpstr;
		}
	}
}

bool CNVRBk::InitialResolution()
{
	CString str;
	LPWSTR lpstr = (LPWSTR)(LPCTSTR)str.GetBuffer(255);

	GetResolutionBySensor(_T("5mp"), lpstr);
	GetResolutionBySensor(_T("3mp"), lpstr);
	GetResolutionBySensor(_T("1080p"), lpstr);
	GetResolutionBySensor(_T("1mp"), lpstr);
	GetResolutionBySensor(_T("720pb"), lpstr);

	GetFpsBySensor(_T("5mp"), lpstr);
	GetFpsBySensor(_T("3mp"), lpstr);
	GetFpsBySensor(_T("1080p"), lpstr);
	GetFpsBySensor(_T("1mp"), lpstr);
	GetFpsBySensor(_T("720pb"), lpstr);

	GetBitrate(lpstr);
	GetFramerate(lpstr);

	str.ReleaseBuffer();

	return true;
}

bool CNVRBk::Initial()
{
	bool bResult = false;

	bResult = InitialDB();
	bResult = InitialOEM();
	bResult = InitialResolution();
	//if (isNVRType())
	{
		InitialONVIF();
	}
	return bResult;
}

void CNVRBk::GetGroup(TreeType treeType, const CString& str, Cam_Group& Group)
{
	if (!m_bOpenDatabase) return;

	CString strSql(_T(""));
	strSql.Format(_T("select * from ec_camera_group where camera_group_desc = '%s' and category_code = '%d'"), str, treeType);
	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);

		while (!m_rs.IsEOF()) 
		{ 
			GetGroupObj(m_rs,Group);
			m_rs.MoveNext();  
		} 
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetGroup(int nGrpId, Cam_Group& Group)
{
	if (!m_bOpenDatabase) return;

	CString strSql(_T(""));
	strSql.Format(_T("select * from ec_camera_group where cameragroupid = %d"), nGrpId);
	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);
		while (!m_rs.IsEOF()) 
		{ 
			GetGroupObj(m_rs,Group);
			m_rs.MoveNext();  
		} 
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetGroup(TreeType byTreeType, vector<Cam_Group>& vcGroup, bool bAutoCreate)
{
	if (!m_bOpenDatabase) return;

	USES_CONVERSION;
	CString strSQL;
	Cam_Group grpObj;
	bool bFound = false;
	byte byType = byTreeType;

	if(byTreeType != AllTree)
		strSQL.Format(_T("select * from ec_camera_group where category_code = %d order by cameragroupid"), byTreeType);
	else
		strSQL.Format(_T("select * from ec_camera_group"));
    try
    {
		CRecordset m_rs(&m_DB);
	    m_rs.Open(CRecordset::dynaset,strSQL);
	    if (m_rs.IsEOF() && byType == LayoutTree)
	    {
		    byType = AllTree;
		    m_rs.Close();
		    m_rs.Open(CRecordset::dynaset,_T("select * from ec_camera_group order by cameragroupid"));
	    }
	    while (!m_rs.IsEOF()) 
	    { 
		    bFound = true;
		    GetGroupObj(m_rs, grpObj);
		    vcGroup.push_back(grpObj);
		    m_rs.MoveNext();  
	    } 
	    m_rs.Close();
    }
	catch(CDBException * p) 	
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}	


	if (vcGroup.size() == 0 && bAutoCreate)
	{
		for (int nIdx = DeviceTree; nIdx <= LayoutTree; nIdx++)
		{
			grpObj.clear();
			grpObj.camera_group_desc = CProductDefine::m_tzProductName;
			grpObj.camera_group_desc += _T(" Group");

			grpObj.cameragroupid = GetLastGroupIdx() + 1;
			grpObj.category_code.Format(_T("%d"),nIdx);
			grpObj.default_ = _T("N");
			grpObj.module_code = _T("LIVEVIEW");

			InsertGroup(grpObj);
		}
	}
}

bool CNVRBk::CheckTreeGroup(TreeType treeType)
{
	if (!m_bOpenDatabase) return false;

	bool bResult = false;
	CString strSQL;
	try
	{
		CRecordset m_rs(&m_DB);
		strSQL.Format(_T("select * from ec_camera_group where category_code = %d order by cameragroupid"), treeType);
		m_rs.Open(CRecordset::dynaset,strSQL);

		if (!m_rs.IsEOF())
		{
			bResult = true;
		}

		m_rs.Close();
	}
	catch(CDBException * p) 	
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}	
	return bResult;
}

bool CNVRBk::InsertGroup(const Cam_Group& Group)
{
	bool bResult = true;
	static CString strSql(_T(""));
	try
	{
		strSql.Empty();
		strSql.Format( 
			_T(
			"Insert INTO ec_camera_group (cameragroupid, camera_group_desc, module_code, category_code, default_) VALUES ('%d', '%s', '%s', '%s', '%s')"),
			Group.cameragroupid, Group.camera_group_desc, Group.module_code, Group.category_code, Group.default_
			);

		m_DB.ExecuteSQL(strSql.GetBuffer());
	}
	catch(CDBException* p)
	{
		AfxMessageBox(p->m_strError);
		bResult = false;
		p->Delete();
	}

	return bResult;
}

bool CNVRBk::InsertGroups(vector<Cam_Group>& vcGroup)
{
	if (!m_bOpenDatabase) return false;
	
	bool bResult = true;
	CString str;
	int nIdex = 0, nCount = vcGroup.size();

	for (nIdex = 0; nIdex < nCount; nIdex++)
	{
		if (!InsertGroup(vcGroup[nIdex]))
		{
			bResult = false;
			break;
		}
	}
	return bResult;
}
int CNVRBk::GetLastCameraId()
{
	if (!m_bOpenDatabase) return 0;

	int nCameraid = 0;
	CString csSQL;
	CDBVariant varValue;

	csSQL.Format(_T("SELECT top 1 * FROM ec_camera ORDER BY cameraid desc"));
	try
	{
		CRecordset m_rs(&m_DB); 
		m_rs.Open(CRecordset::forwardOnly, csSQL.GetBuffer());
		while(!m_rs.IsEOF())
		{
			varValue.m_iVal = 0;
			m_rs.GetFieldValue(_T("cameraid"), varValue, SQL_C_SSHORT);
			nCameraid = varValue.m_iVal;
			m_rs.MoveNext();
		}
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return nCameraid;
}

int CNVRBk::GetLastCameraIdx()
{
	if (!m_bOpenDatabase) return 0;

	int nCamera_idx = 0;
	CString csSQL;
	CDBVariant varValue;

	csSQL.Format(_T("SELECT top 1 * FROM ec_camera ORDER BY camera_idx desc"));
	try
	{
		CRecordset m_rs(&m_DB); 
		m_rs.Open(CRecordset::forwardOnly, csSQL.GetBuffer());
		while(!m_rs.IsEOF())
		{
			varValue.m_iVal = 0;
			m_rs.GetFieldValue(_T("camera_idx"), varValue, SQL_C_SSHORT);
			nCamera_idx = varValue.m_iVal;
			m_rs.MoveNext();
		}
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
	
	return nCamera_idx;
}

void CNVRBk::GetLastCameraIdx(int& nCameraid, int& nCamera_idx)
{
	nCameraid = GetLastCameraId();
	nCamera_idx = GetLastCameraIdx();
}

int CNVRBk::GetFirstGroupIdx(TreeType treeType)
{
	if (!m_bOpenDatabase) return 0;

	CString csSQL;
	CDBVariant varValue;
	int nIdx = 1;

	csSQL.Format(_T("SELECT top 1 * FROM ec_camera_group where category_code = '%d' ORDER BY cameragroupid"), treeType);
	
	try
	{
		CRecordset m_rs(&m_DB); 
		m_rs.Open(CRecordset::forwardOnly, csSQL.GetBuffer());
		while(!m_rs.IsEOF())
		{
			varValue.m_iVal = 0;
			m_rs.GetFieldValue(_T("cameragroupid"), varValue, SQL_C_SSHORT);
			nIdx = varValue.m_iVal;

			m_rs.MoveNext();
		}
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return nIdx;
}

int CNVRBk::GetLastGroupIdx()
{
	if (!m_bOpenDatabase) return 0;

	CString csSQL;
	CDBVariant varValue;
	int nIdx = 0;

	csSQL.Format(_T("SELECT top 1 * FROM ec_camera_group ORDER BY cameragroupid desc"));
	
	try
	{
		CRecordset m_rs(&m_DB); 
		m_rs.Open(CRecordset::forwardOnly, csSQL.GetBuffer());
		while(!m_rs.IsEOF())
		{
			varValue.m_iVal = 0;
			m_rs.GetFieldValue(_T("cameragroupid"), varValue, SQL_C_SSHORT);
			nIdx = varValue.m_iVal;

			m_rs.MoveNext();
		}
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return nIdx;
}

void CNVRBk::GetLastGroupCamera(Cam_Group_Cam& grpCam)
{
	if (!m_bOpenDatabase) return;

	Cam_Group_Cam GroupCam;
	CString strSql(_T(""));
	strSql.Format(_T("select top 1 *from ec_camera_group_camera order by cameragroupcameraid desc"));

	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);

		if(!m_rs.IsEOF()) 
		{ 
			grpCam.clear();
			GetCameraGroupCameraObj(m_rs,grpCam);
			m_rs.MoveNext();  
		} 
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetAllGroupCamera(vector<Cam_Group_Cam>& vcGroupCam)
{
	if (!m_bOpenDatabase) return;

	Cam_Group_Cam GroupCam;
	CString strSql(_T(""));
	strSql.Format(_T("select *from ec_camera_group_camera order by cameragroupid"));

	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);

		while (!m_rs.IsEOF()) 
		{ 
			GroupCam.clear();
			GetCameraGroupCameraObj(m_rs,GroupCam);
			vcGroupCam.push_back(GroupCam);
			m_rs.MoveNext();  
		} 
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetGroupCamera(int nCamId, vector<Cam_Group_Cam>& vcGroupCam, bool isIndex)
{
	if (!m_bOpenDatabase) return;

	Cam_Group_Cam GroupCam;
	CString strSql(_T(""));
	if (isIndex)
	{
		strSql.Format(_T("select * from ec_camera_group_camera where camera_idx = '%d'"), nCamId);
	} 
	else
	{
		strSql.Format(_T("select * from ec_camera_group_camera where cameraid = '%d'"), nCamId);
	}

	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);

		while (!m_rs.IsEOF()) 
		{ 
			GroupCam.clear();
			GetCameraGroupCameraObj(m_rs,GroupCam);
			vcGroupCam.push_back(GroupCam);
			m_rs.MoveNext();  
		} 
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}
void CNVRBk::GetGroupCamera(int nGrpId, int nCamId, vector<Cam_Group_Cam>& vcGroupCam)
{
	if (!m_bOpenDatabase) return;

	Cam_Group_Cam GroupCam;
	CString strSql(_T(""));
	strSql.Format(_T("select * from ec_camera_group_camera where cameraid = '%d' and cameragroupid = %d"), nCamId, nGrpId);

	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);

		while (!m_rs.IsEOF()) 
		{ 
			GroupCam.clear();
			GetCameraGroupCameraObj(m_rs,GroupCam);
			vcGroupCam.push_back(GroupCam);
			m_rs.MoveNext();  
		} 
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetGroupCamera(int nGrpId, vector<Cam_Group_Cam>& vcGroupCam)
{
	if (!m_bOpenDatabase) return;

	CString strSql(_T(""));
	Cam_Group_Cam nodeObj;
	
	strSql.Format(_T("select * from ec_camera_group_camera where cameragroupid = '%d' order by cameragroupid"), nGrpId);

	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);

		while (!m_rs.IsEOF()) 
		{ 
			GetCameraGroupCameraObj(m_rs,nodeObj);
			vcGroupCam.push_back(nodeObj);
			m_rs.MoveNext();  
		} 
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetAllCameras(vector<ec_Camera>& vcCam)
{
	if (!m_bOpenDatabase) return;

	CString strSql(_T(""));
	ec_Camera camObj;

	strSql.Format(_T("select * from ec_camera"));

	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);

		while (!m_rs.IsEOF()) 
		{ 
			camObj.clear();
			GetCameraObj(m_rs, camObj);
			vcCam.push_back(camObj);
			m_rs.MoveNext();  
		} 

		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetCamera(const CString& strMac, ec_Camera& camObj)
{
	if (!m_bOpenDatabase) return;

	CString strSql(_T(""));
	CString mac = strMac;

	strSql.Format(_T("SELECT * FROM ec_camera WHERE mac_address = '%s'"), mac.Trim());

	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);

		while (!m_rs.IsEOF()) 
		{ 
			GetCameraObj(m_rs, camObj);
			m_rs.MoveNext();  
		} 

		m_rs.Close();

		if (!camObj.isempty())
		{
			GetStream(camObj.cameraid, camObj.vcStream);
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetCamera(const CString& strMac, vector<ec_Camera>& vccamObj)
{
	if (!m_bOpenDatabase) return;

	CString strSql(_T(""));
	ec_Camera camObj;
	CString mac = strMac;

	strSql.Format(_T("SELECT * FROM ec_camera WHERE mac_address = '%s'"), mac.Trim());

	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);

		while (!m_rs.IsEOF()) 
		{ 
			camObj.clear();

			GetCameraObj(m_rs, camObj);

			if (!camObj.isempty())
			{
				GetStream(camObj.cameraid, camObj.vcStream);
			}

			vccamObj.push_back(camObj);
			m_rs.MoveNext();  
		} 
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetCamera(int nCamId, ec_Camera& camObj, bool isIndex)
{
	if (!m_bOpenDatabase) return;

	CString strSql(_T(""));

	if (isIndex)
	{
		strSql.Format(_T("select * from ec_camera where camera_idx = '%d'"), nCamId);
	}
	else
	{
		strSql.Format(_T("select * from ec_camera where cameraid = '%d'"), nCamId);
	}

	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);

		while (!m_rs.IsEOF()) 
		{ 
			GetCameraObj(m_rs, camObj);
			m_rs.MoveNext();  
		} 

		m_rs.Close();

		if (!camObj.isempty())
		{
			GetStream(camObj.cameraid, camObj.vcStream);
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetCamera(const CString& strIP, const CString& strMac, ec_Camera& camObj)
{
	if (!m_bOpenDatabase) return;

	CString strSql(_T(""));
	CString ip = strIP;
	CString mac = strMac;

	strSql.Format(_T("SELECT * FROM ec_camera WHERE mac_address = '%s' and ipaddress = '%s'"), mac.Trim(), ip.Trim());

	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);

		while (!m_rs.IsEOF()) 
		{ 
			GetCameraObj(m_rs, camObj);
			m_rs.MoveNext();  
		} 

		m_rs.Close();

		if (!camObj.isempty())
		{
			GetStream(camObj.cameraid, camObj.vcStream);
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetRecordingObj(CRecordset& RecordObj, ec_Camera_Rec& RecObj)
{
	static CString str(_T(""));
	static CDBVariant varValue;

	if (!RecordObj.IsEOF())
	{
		RecObj.clear();

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("camera_idx"), varValue, SQL_C_SSHORT);
		RecObj.camera_idx = varValue.m_iVal;

		str.Empty();
		RecordObj.GetFieldValue(_T("recording_type"), str);
		str.Trim();
		RecObj.recording_type = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("recording_time"), str);
		str.Trim();
		RecObj.recording_time = str;
	}
}

void CNVRBk::GetStreamObj(CRecordset& RecordObj, ec_Stream& StreamObj)
{
	static CString str(_T(""));
	static CDBVariant varValue;

	if (!RecordObj.IsEOF())
	{
		StreamObj.clear();

		str.Empty();
		RecordObj.GetFieldValue(_T("stream_name"), str);
		str.Trim();
		StreamObj.stream_name = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("stream_tag"), str);
		str.Trim();
		StreamObj.stream_tag = str;

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("cameraid"), varValue, SQL_C_SSHORT);
		StreamObj.cameraid = varValue.m_iVal;

		str.Empty();
		RecordObj.GetFieldValue(_T("stream_type"), str);
		str.Trim();
		StreamObj.stream_type = str;

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("stream_port"), varValue, SQL_C_SSHORT);
		StreamObj.stream_port = varValue.m_iVal;

		str.Empty();
		RecordObj.GetFieldValue(_T("stream_protocol"), str);
		str.Trim();
		StreamObj.stream_protocol = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("video_cmpr_type"), str);
		str.Trim();
		StreamObj.video_cmpr_type = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("video_res"), str);
		str.Trim();
		StreamObj.video_res = str;
	}
}

void CNVRBk::GetStorageObj(CRecordset& RecordObj, ec_Storage& StorageObj)
{
	static CString str(_T(""));

	if (!RecordObj.IsEOF())
	{
		StorageObj.clear();

		str.Empty();
		RecordObj.GetFieldValue(_T("storage_type"), str);
		str.Trim();
		StorageObj.storage_type = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("store_location"), str);
		str.Trim();
		StorageObj.store_location = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("buffer_size"), str);
		str.Trim();
		StorageObj.buffer_size = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("server_user"), str);
		str.Trim();
		StorageObj.server_user = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("server_password"), str);
		str.Trim();
		StorageObj.server_password = str;
	}
}

void CNVRBk::GetCameraObj(CRecordset& RecordObj, ec_Camera& CamObj)
{
	static CString str(_T(""));
	static CDBVariant varValue;

	if (!RecordObj.IsEOF())
	{
		CamObj.clear();

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("cameraid"), varValue, SQL_C_SSHORT);
		CamObj.cameraid = varValue.m_iVal;

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("camera_idx"), varValue, SQL_C_SSHORT);
		CamObj.camera_idx = varValue.m_iVal;

		str.Empty();
		RecordObj.GetFieldValue(_T("cameraname"), str);
		str.Trim();
		CamObj.cameraname = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("connect_type_code"), str);
		str.Trim();
		CamObj.connect_type_code = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("ipaddress"), str);
		str.Trim();
		CamObj.ipaddress = str;

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("httpport"), varValue, SQL_C_SSHORT);
		CamObj.httpport = varValue.m_iVal;

		str.Empty();
		RecordObj.GetFieldValue(_T("gateway"), str);
		CamObj.gateway = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("name_server"), str);
		CamObj.name_server = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("mac_address"), str);
		str.Trim();
		CamObj.mac_address = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("username"), str);
		str.Trim();
		CamObj.username = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("password"), str);
		str.Trim();
		CamObj.password = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("brand_code"), str);
		str.Trim();
		CamObj.brand_code = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("model_code"), str);
		str.Trim();
		CamObj.model_code = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("ptz_support"), str);
		str.Trim();
		CamObj.ptz_support = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("ptz_protocol"), str);
		str.Trim();
		CamObj.ptz_protocol = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("digital_in1"), str);
		str.Trim();
		CamObj.digital_in1 = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("digital_in2"), str);
		str.Trim();
		CamObj.digital_in2 = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("digital_out"), str);
		str.Trim();
		CamObj.digital_out = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("video_format"), str);
		str.Trim();
		CamObj.video_format = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("speaker_support"), str);
		str.Trim();
		CamObj.speaker_support = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("mic_support"), str);
		str.Trim();
		CamObj.mic_support = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("subnet_mask1"), str);
		str.Trim();
		CamObj.subnet_mask1 = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("subnet_mask2"), str);
		str.Trim();
		CamObj.subnet_mask2 = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("subnet_mask3"), str);
		str.Trim();
		CamObj.subnet_mask3 = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("subnet_mask4"), str);
		str.Trim();
		CamObj.subnet_mask4 = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("active_"), str);
		str.Trim();
		CamObj.active_ = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("stream_url"), str);
		str.Trim();
		CamObj.stream_url = str;
	}
}

void CNVRBk::GetAllCamRecs(vector<ec_Camera_Rec>& vcRec)
{
	if (!m_bOpenDatabase) return;

	CString strSql(_T(""));
	ec_Camera_Rec CamRec;

	strSql.Format(_T("select * from ec_camera_recording"));

	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);

		while (!m_rs.IsEOF()) 
		{ 
			CamRec.clear();
			GetRecordingObj(m_rs, CamRec);
			vcRec.push_back(CamRec);
			m_rs.MoveNext();  
		} 

		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetCamRec(int nCamIdx, ec_Camera_Rec& CamRec)
{
	if (!m_bOpenDatabase) return;

	CString str;
	CDBVariant varValue;	
	CString strSql(_T(""));

	strSql.Format(_T("select * from ec_camera_recording where camera_idx = '%d'"), nCamIdx);

	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);

		while (!m_rs.IsEOF()) 
		{ 
			CamRec.clear();
			GetRecordingObj(m_rs, CamRec);
			m_rs.MoveNext();  
		} 

		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetStream(int nCamId, vector<ec_Stream>& vcStream)
{
	if (!m_bOpenDatabase) return;

	CString str;
	CDBVariant varValue;	
	CString strSql(_T(""));
	ec_Stream streamObj;
	vcStream.clear();

	strSql.Format(_T("select * from ec_stream where cameraid = '%d'"), nCamId);

	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::dynaset,strSql);

		while (!m_rs.IsEOF()) 
		{ 
			streamObj.clear();
			GetStreamObj(m_rs, streamObj);
			vcStream.push_back(streamObj);
			m_rs.MoveNext();  
		} 

		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetAllStreamByCamid(map<int, vector<ec_Stream> >& mapStream)
{
	if (!m_bOpenDatabase) return;

	vector<int> vcCameraid;
	CDBVariant varValue;
	CString strSql(_T("SELECT DISTINCT cameraid FROM ec_camera")), str(_T(""));
	ec_Stream streamObj;
	vector<ec_Stream> vcStream;
	vcCameraid.clear();

	try
	{
		CRecordset m_rs(&m_DB);
		m_rs.Open(CRecordset::forwardOnly,strSql);
		while (!m_rs.IsEOF()) 
		{
			varValue.m_iVal = 0;
			m_rs.GetFieldValue(_T("cameraid"), varValue, SQL_C_SSHORT);
			vcCameraid.push_back(varValue.m_iVal);
			m_rs.MoveNext();
		}
		m_rs.Close();

		int nCount = vcCameraid.size();
		for (int nIdx = 0; nIdx < nCount; nIdx++)
		{
			vcStream.clear();
			strSql.Format(_T("SELECT* FROM ec_stream where cameraid = %d"), vcCameraid[nIdx]);
			m_rs.Open(CRecordset::forwardOnly,strSql);
			while (!m_rs.IsEOF()) 
			{
				streamObj.clear();
				varValue.m_iVal = 0;

				str.Empty();
				m_rs.GetFieldValue(_T("stream_name"), str);
				str.Trim();
				streamObj.stream_name = str;

				varValue.m_iVal = 0;
				m_rs.GetFieldValue(_T("cameraid"), varValue,SQL_C_SSHORT);
				streamObj.cameraid = varValue.m_iVal;

				str.Empty();
				m_rs.GetFieldValue(_T("stream_type"), str);
				str.Trim();
				streamObj.stream_type = str;

				varValue.m_iVal = 0;
				m_rs.GetFieldValue(_T("stream_port"), varValue,SQL_C_SSHORT);
				streamObj.stream_port = varValue.m_iVal;

				str.Empty();
				m_rs.GetFieldValue(_T("stream_protocol"), str);
				str.Trim();
				streamObj.stream_protocol = str;

				vcStream.push_back(streamObj);
				mapStream[vcCameraid[nIdx]] = vcStream;
				m_rs.MoveNext();
			}
			m_rs.Close();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

bool CNVRBk::UpdateStreamType(const vector<ec_Stream> vcStream)
{
	CString strSql(_T(""));
	int nIdx = 0, nCount = vcStream.size();
	bool bResult = true;

	try
	{
		for(int nIdx = 0; nIdx < nCount; nIdx++)
		{
			strSql.Format(_T("UPDATE ec_stream SET stream_type = 'LV' WHERE cameraid = %d AND stream_name = 'Stream 1'"), vcStream[nIdx].cameraid);
			m_DB.ExecuteSQL(strSql);
		}
	}
	catch (CDBException* p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
	return bResult;
}

void CNVRBk::GetGroupObj(CRecordset& RecordObj, Cam_Group& GroupObj)
{
	static CString str(_T(""));
	static CDBVariant varValue;

	if (!RecordObj.IsEOF())
	{
		GroupObj.clear();

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("cameragroupid"), varValue, SQL_C_SSHORT);
		GroupObj.cameragroupid = varValue.m_iVal;

		str.Empty();
		RecordObj.GetFieldValue(_T("camera_group_desc"), str);
		str.Trim();
		GroupObj.camera_group_desc = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("module_code"), str);
		str.Trim();
		GroupObj.module_code = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("category_code"), str);
		str.Trim();
		GroupObj.category_code = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("map_file"), str);
		str.Trim();
		GroupObj.map_file = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("acmap_file"), str);
		str.Trim();
		GroupObj.acmap_file = str;

		str.Empty();
		RecordObj.GetFieldValue(_T("default_"), str);
		str.Trim();
		GroupObj.default_ = str;
	}
}

void CNVRBk::GetCameraGroupCameraObj(CRecordset& RecordObj, Cam_Group_Cam& GroupCamObj)
{
	static CDBVariant varValue;

	if (!RecordObj.IsEOF())
	{
		GroupCamObj.clear();

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("cameragroupcameraid"), varValue, SQL_C_SSHORT);
		GroupCamObj.cameragroupcameraid = varValue.m_iVal;

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("cameragroupid"), varValue, SQL_C_SSHORT);
		GroupCamObj.cameragroupid = varValue.m_iVal;

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("cameraid"), varValue, SQL_C_SSHORT);
		GroupCamObj.cameraid = varValue.m_iVal;

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("camera_idx"), varValue, SQL_C_SSHORT);
		GroupCamObj.camera_idx = varValue.m_iVal;

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("streamid"), varValue, SQL_C_SSHORT);
		GroupCamObj.streamid = varValue.m_iVal;

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("numb_"), varValue, SQL_C_SSHORT);
		GroupCamObj.numb_ = varValue.m_iVal;

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("emap_x"), varValue, SQL_C_SSHORT);
		GroupCamObj.emap_x = varValue.m_iVal;

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("emap_y"), varValue, SQL_C_SSHORT);
		GroupCamObj.emap_y = varValue.m_iVal;

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("acmap_x"), varValue, SQL_C_SSHORT);
		GroupCamObj.acmap_x = varValue.m_iVal;

		varValue.m_iVal = 0;
		RecordObj.GetFieldValue(_T("acmap_y"), varValue, SQL_C_SSHORT);
		GroupCamObj.acmap_y = varValue.m_iVal;

	}
}

bool CNVRBk::UpdateGroupCamera(const Cam_Group_Cam& GroupCam)
{
	bool bResult = true;
	CString strSql(_T(""));
	try
	{
		strSql.Format(_T("UPDATE ec_camera_group_camera set cameragroupid='%d',cameraid='%d',camera_idx='%d',streamid='%d',numb_='%d',emap_x='%d',emap_y='%d',acmap_x='%d',acmap_y='%d' where cameragroupcameraid='%d'"),
			GroupCam.cameragroupid,
			GroupCam.cameraid,
			GroupCam.camera_idx, 
			GroupCam.streamid,
			GroupCam.numb_,
			GroupCam.emap_x,
			GroupCam.emap_y,
			GroupCam.acmap_x,
			GroupCam.acmap_y,
			GroupCam.cameragroupcameraid);

		m_DB.ExecuteSQL(strSql);
	}
	catch (CDBException* p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return bResult;
}

bool CNVRBk::UpdateGroupCameraGrpId(int OldGroupid, int NewGroupid, int CamId)
{
	bool bResult = false;
	if (OldGroupid != NewGroupid)
	{
		CString strSql(_T(""));
		vector<Cam_Group_Cam> vcGrpCamObj;
		GetGroupCamera(OldGroupid,CamId,vcGrpCamObj);
		
		if (vcGrpCamObj.size() && vcGrpCamObj[0].cameragroupid == OldGroupid)
		{
			try
			{
				strSql.Format(_T("UPDATE  ec_camera_group_camera SET cameragroupid = %d WHERE cameragroupcameraid = %d"), NewGroupid, vcGrpCamObj[0].cameragroupcameraid);
				m_DB.ExecuteSQL(strSql);
				bResult = true;
			}
			catch(CDBException * p)
			{
				AfxMessageBox(p->m_strError);
				p->Delete();
			}
		}
	}
	return bResult;
}

bool CNVRBk::UpdateGroupName(const CString& strNewGrpName, int GrpId)
{
	bool bResult = false;
	Cam_Group Group;
	GetGroup(GrpId, Group);

	if (!Group.isempty())
	{
		CString strSql(_T(""));
		strSql.Format(_T("UPDATE ec_camera_group SET camera_group_desc='%s' WHERE cameragroupid= %d"), strNewGrpName, GrpId);				

		try 
		{
			m_DB.ExecuteSQL(strSql);
			bResult = true;
		}
		catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
	}
	return bResult;
}

bool CNVRBk::DeleteGroup(int nGrpId)
{
	bool bResult = false;
	Cam_Group Group;
	GetGroup(nGrpId, Group);

	if (!Group.isempty())
	{
		CString strSql(_T(""));
		strSql.Format(_T("DELETE FROM ec_camera_group WHERE cameragroupid = %d"), nGrpId);				

		try 
		{
			m_DB.ExecuteSQL(strSql);
			bResult = true;
		}
		catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
	}

	return bResult;
}

bool CNVRBk::UpdateGroupId(int nNewGroup, int OldGroup)
{
	bool bResult = true;

	CString strSql(_T(""));			
	try 
	{
		strSql.Format(_T("UPDATE ec_camera_group SET cameragroupid='%d', camera_group_code='GROUP%d' WHERE cameragroupid = '%d'"), nNewGroup, nNewGroup, OldGroup);
		m_DB.ExecuteSQL(strSql);

		strSql.Format(_T("UPDATE ec_camera_group_camera SET cameragroupid='%d' WHERE cameragroupid='%d'"), nNewGroup, OldGroup);
		m_DB.ExecuteSQL(strSql);
		
	}
	catch(CDBException * p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return bResult;
}

bool CNVRBk::DeleteGroupCameraByGroup(int nGrpId)
{
	bool bResult = false;
	vector<Cam_Group_Cam> vcGroupCam;
	GetGroupCamera(nGrpId, vcGroupCam);

	if (vcGroupCam.size())
	{
		CString strSql(_T(""));
		strSql.Format(_T("DELETE FROM ec_camera_group_camera WHERE cameragroupid = %d"), nGrpId);				

		try 
		{
			m_DB.ExecuteSQL(strSql);
			bResult = true;
		}
		catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
	}

	return bResult;
}

bool CNVRBk::DeleteCamRecTime(int nCamIdx)
{
	bool bResult = true;
	CString strSql(_T(""));
	strSql.Format(_T("DECLARE @REC_ID int; SELECT @REC_ID = camera_recordingid FROM ec_camera_recording WHERE camera_idx = %d; DELETE FROM ec_camera_recording_time WHERE camera_recordingid = @REC_ID"), nCamIdx);
	try 
	{
		m_DB.ExecuteSQL(strSql);
		bResult = true;
	}
	catch(CDBException * p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return bResult;
}

bool CNVRBk::DeleteCamRec(int nCamIdx)
{
	bool bResult = true;
	CString strSql(_T(""));
	strSql.Format(_T("DELETE FROM ec_camera_recording WHERE camera_idx = %d"), nCamIdx);

	try 
	{
		m_DB.ExecuteSQL(strSql);
		bResult = true;
	}
	catch(CDBException * p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return bResult;
}

bool CNVRBk::DeleteGroupCameraByGroupCameraId(int nGrpCamId)
{
	bool bResult = true;
	CString strSql(_T(""));
	strSql.Format(_T("DELETE FROM ec_camera_group_camera WHERE cameragroupcameraid = %d"), nGrpCamId);

	try 
	{
		m_DB.ExecuteSQL(strSql);
		bResult = true;
	}
	catch(CDBException * p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return bResult;
}

bool CNVRBk::DeleteGroupCamera(int nCamId, int nGroupId)
{
	bool bResult = true;
	CString strSql(_T(""));
	strSql.Format(_T("DELETE FROM ec_camera_group_camera WHERE cameraid = %d and cameragroupid = %d"), nCamId,nGroupId);

	try 
	{
		m_DB.ExecuteSQL(strSql);
		bResult = true;
	}
	catch(CDBException * p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return bResult;
}

bool CNVRBk::DeleteGroupCamera(int nCamId)
{
	bool bResult = true;
	CString strSql(_T(""));
	strSql.Format(_T("DELETE FROM ec_camera_group_camera WHERE cameraid = %d"), nCamId);

	try 
	{
		m_DB.ExecuteSQL(strSql);
		bResult = true;
	}
	catch(CDBException * p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return bResult;
}

bool CNVRBk::DeleteStream(int nCamId)
{
	bool bResult = true;
	CString strSql(_T(""));
	strSql.Format(_T("DELETE FROM ec_stream WHERE cameraid = %d"), nCamId);

	try 
	{
		m_DB.ExecuteSQL(strSql);
		bResult = true;
	}
	catch(CDBException * p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return bResult;
}

bool CNVRBk::DeleteCamera(int nCamId)
{
	bool bResult = true;
	CString strSql(_T(""));
	strSql.Format(_T("DELETE FROM ec_camera WHERE cameraid = %d"), nCamId);

	try 
	{
		m_DB.ExecuteSQL(strSql);
		bResult = true;
	}
	catch(CDBException * p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	if (bResult)
	{
		bResult = DeleteStream(nCamId);
	}

	return bResult;
}

bool CNVRBk::UpdateGroupType(byte byType, int nGrpId)
{
	bool bResult = true;
	CString strSql(_T(""));
	strSql.Format(_T("UPDATE ec_camera_group SET category_code = '%d' WHERE cameragroupid = %d"), byType, nGrpId);

	try 
	{
		m_DB.ExecuteSQL(strSql);
		bResult = true;
	}
	catch(CDBException * p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return bResult;
}

bool CNVRBk::UpdateDefaultGroup()
{
	bool bResult = true;
	CString strSql(_T(""));
	strSql.Format(_T("UPDATE ec_camera_group SET default_ = 'Y' WHERE cameragroupid = 1"));

	try 
	{
		m_DB.ExecuteSQL(strSql);
		bResult = true;
	}
	catch(CDBException * p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return bResult;
}

bool CNVRBk::InsertRecs(const vector<ec_Camera_Rec>& vcRec)
{
	bool bResult = true;

	int nIdx = 0, nCount = vcRec.size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		bResult = InsertCamRec(vcRec[nIdx]);
	}

	return bResult;
}

bool CNVRBk::InsertCameras(const vector<ec_Camera>& vcCamera)
{
	bool bResult = true;
	
	int nIdx = 0, nCount = vcCamera.size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		bResult = InsertCamera(vcCamera[nIdx]);
	}

	return bResult;
}

bool CNVRBk::UpdateCamera(const ec_Camera& Camera, bool bUpdateStream)
{
	bool bResult = true;
	CString strSql(_T(""));

	try
	{
		strSql.Format(_T("UPDATE ec_camera set camera_idx='%d',cameraname='%s',connect_type_code='%s',ipaddress='%s',httpport='%d',gateway='%s',name_server='%s',mac_address='%s',username='%s',password='%s',brand_code='%s',model_code='%s',ptz_support='%s',ptz_protocol='%s',digital_in1='%s',digital_in2='%s',digital_out='%s',video_format='%s',speaker_support='%s',mic_support='%s',subnet_mask1='%s',subnet_mask2='%s',subnet_mask3='%s',subnet_mask4='%s',total_stream='%d',active_='%s',stream_url='%s' where cameraid='%d'"),
			Camera.camera_idx,
			Camera.cameraname,
			Camera.connect_type_code, 
			Camera.ipaddress,
			Camera.httpport,
			Camera.gateway,
			Camera.name_server,
			Camera.mac_address,
			Camera.username,
			Camera.password,
			Camera.brand_code,
			Camera.model_code,
			Camera.ptz_support,
			Camera.ptz_protocol,
			Camera.digital_in1,
			Camera.digital_in2,						
			Camera.digital_out,
			Camera.video_format,
			Camera.speaker_support,
			Camera.mic_support,
			Camera.subnet_mask1,
			Camera.subnet_mask2,
			Camera.subnet_mask3,
			Camera.subnet_mask4,
			Camera.vcStream.size(),
			Camera.active_,
			Camera.stream_url,
			Camera.cameraid);

		m_DB.ExecuteSQL(strSql);
	}
	catch (CDBException* p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	if (bResult && bUpdateStream)
	{
		bResult = UpdateStreams(Camera.vcStream, Camera.cameraid);
	}

	return bResult;
}

bool CNVRBk::InsertCamera(const ec_Camera& CameraObj)
{
	bool bResult = true;
	CString strSql(_T(""));
	ec_Camera Camera = CameraObj;
	try
	{
		strSql.Format(_T("INSERT INTO ec_camera(camera_idx, cameraname, connect_type_code, ipaddress, httpport, gateway, name_server, mac_address, username, password, brand_code, model_code, ptz_support, ptz_protocol,digital_in1,digital_in2,digital_out,video_format,speaker_support,mic_support,subnet_mask1,subnet_mask2,subnet_mask3,subnet_mask4,total_stream,active_,stream_url) VALUES ('%d', '%s', '%s', '%s', '%d', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%d','%s','%s')"),
			Camera.camera_idx,
			Camera.cameraname.Trim(),
			Camera.connect_type_code.Trim(), 
			Camera.ipaddress.Trim(),
			Camera.httpport,
			Camera.gateway.Trim(),
			Camera.name_server.Trim(),
			Camera.mac_address.Trim(),
			Camera.username.Trim(),
			Camera.password.Trim(),
			Camera.brand_code.Trim(),
			Camera.model_code.Trim(),
			Camera.ptz_support.Trim(),
			Camera.ptz_protocol.Trim(),
			Camera.digital_in1.Trim(),
			Camera.digital_in2.Trim(),						
			Camera.digital_out.Trim(),
			Camera.video_format.Trim(),
			Camera.speaker_support.Trim(),
			Camera.mic_support.Trim(),
			Camera.subnet_mask1.Trim(),
			Camera.subnet_mask2.Trim(),
			Camera.subnet_mask3.Trim(),
			Camera.subnet_mask4.Trim(),
			Camera.vcStream.size(),
			Camera.active_.Trim(),
			Camera.stream_url.Trim());

		m_DB.ExecuteSQL(strSql);
	}
	catch (CDBException* p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	if (bResult)
	{
		int nCameraId = GetLastCameraId();
		bResult = InsertStreams(Camera.vcStream, nCameraId);
	}
	
	return bResult;
}

bool CNVRBk::UpdateStreams(const vector<ec_Stream>& vcStream, int nCameraId)
{
	bool bResult = true;

	bResult = DeleteStream(nCameraId);
	bResult = InsertStreams(vcStream,nCameraId);

	return bResult;
}

bool CNVRBk::InsertStreams(const vector<ec_Stream>& vcStream, int nCameraId)
{
	bool bResult = true;
	CString strSql(_T(""));
	ec_Stream streamObj;

	int nIdx = 0, nCount = vcStream.size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		streamObj = vcStream[nIdx];
		try
		{
			strSql.Format(_T("INSERT INTO ec_stream (stream_name, stream_tag, cameraid, stream_type, stream_port, stream_protocol, video_cmpr_type, video_res) VALUES('%s', '%s', %d, '%s', '%d', '%s', '%s', '%s')"),
				streamObj.stream_name,
				streamObj.stream_tag,
				nCameraId,
				streamObj.stream_type,
				streamObj.stream_port,
				streamObj.stream_protocol, 
				streamObj.video_cmpr_type, 
				streamObj.video_res);

			m_DB.ExecuteSQL(strSql);
		}
		catch (CDBException* p)
		{
			bResult = false;
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
	}

	return bResult;
}

int CNVRBk::GetLastDeviceIdx()
{	
	if (!m_bOpenDatabase) return 0;

	CString csSQL;
	CDBVariant varValue;
	int nIdx = 1;

	csSQL.Format(_T("SELECT top 1 * FROM ec_camera_group_camera ORDER BY numb_ desc"));
	
	try
	{
		CRecordset m_rs(&m_DB); 
		m_rs.Open(CRecordset::forwardOnly, csSQL.GetBuffer());
		while(!m_rs.IsEOF())
		{
			varValue.m_iVal = 0;
			m_rs.GetFieldValue(_T("numb_"), varValue, SQL_C_SSHORT);
			nIdx = varValue.m_iVal;

			m_rs.MoveNext();
		}
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return nIdx;
}

bool CNVRBk::InsertGroupCamera(const Cam_Group_Cam& GroupCam)
{
	bool bResult = true;
	CString strSql(_T(""));
	
	try
	{	
		strSql.Format(_T("INSERT INTO ec_camera_group_camera (cameragroupid, cameraid, camera_idx, streamid, numb_) VALUES('%d', '%d', '%d', '%d', '%d')"),
			GroupCam.cameragroupid,GroupCam.cameraid,GroupCam.camera_idx,GroupCam.streamid,GroupCam.numb_);

		m_DB.ExecuteSQL(strSql);
	}
	catch (CDBException* p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return bResult;
}

bool CNVRBk::InsertCamRec(const ec_Camera_Rec& CamRec)
{
	bool bResult = true;
	CString strSql(_T(""));

	try
	{	
		strSql.Format(_T("INSERT INTO ec_camera_recording (camera_idx, recording_type) VALUES('%d', '%s')"),
			CamRec.camera_idx,CamRec.recording_type);

		m_DB.ExecuteSQL(strSql);
	}
	catch (CDBException* p)
	{
		bResult = false;
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return bResult;
}

bool CNVRBk::DeleteGroupByTreeType(TreeType treeType)
{
	bool bResult = false;
	CString strSql(_T(""));
	strSql.Format(_T("DELETE FROM ec_camera_group WHERE category_code = %d"), treeType);				

	try 
	{
		m_DB.ExecuteSQL(strSql);
		bResult = true;
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return bResult;
}

void CNVRBk::GetAllCameraId(vector<int>& vcCamId, bool isIdx)
{
	if (!m_bOpenDatabase) return;

	CString csSQL;

	CDBVariant varValue;
	if (isIdx)
	{
		csSQL.Format(_T("select cameraid from ec_camera order by camera_idx"));
	} 
	else
	{
		csSQL.Format(_T("select cameraid from ec_camera order by cameraid"));
	}
	
	try
	{
		CRecordset m_rs(&m_DB); 
		m_rs.Open(CRecordset::forwardOnly, csSQL.GetBuffer());
		vcCamId.clear();
		while(!m_rs.IsEOF())
		{
			varValue.m_iVal = 0;
			if (isIdx)
			{
				m_rs.GetFieldValue(_T("camera_idx"), varValue, SQL_C_SSHORT);
			} 
			else
			{
				m_rs.GetFieldValue(_T("cameraid"), varValue, SQL_C_SSHORT);
			}

			vcCamId.push_back(varValue.m_iVal);
			m_rs.MoveNext();
		}
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::UnregistWnd(CWnd* wnd)
{	
	vector<CWnd*>::iterator it = m_RegistWnd.begin();
	while(it != m_RegistWnd.end())
	{
		if ( wnd == (*it) )
		{
			m_RegistWnd.erase(it);
			break;
		}
		it++;
	}
}

void CNVRBk::SendMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	int nIdx = 0, nCount = m_RegistWnd.size(); 
	for (nIdx=0 ; nIdx<nCount ; nIdx++)
	{
		m_RegistWnd[nIdx]->SendMessage(uMessage, wParam, lParam);
	}
}

void CNVRBk::GetStorageByType(vector<ec_Storage>& vcStorageObj, const CString& strStorageType)
{
	if (!m_bOpenDatabase) return;

	CString csSQL;
	ec_Storage StorageObj;

	csSQL.Format(_T("SELECT * FROM ec_storage WHERE storage_type='%s'"), strStorageType);
	
	try
	{
		CRecordset m_rs(&m_DB); 
		m_rs.Open(CRecordset::forwardOnly, csSQL.GetBuffer());

		while(!m_rs.IsEOF())
		{
			StorageObj.clear();
			GetStorageObj(m_rs, StorageObj);
			vcStorageObj.push_back(StorageObj);
			m_rs.MoveNext();
		}
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CNVRBk::GetEcParam(const CString& strParam, CString& strValue)
{
	if (!m_bOpenDatabase) return;

	CString csSQL;
	csSQL.Format(_T("SELECT parm_value FROM ecparms WHERE parm_name = '%s'"), strParam);
	
	try
	{
		CRecordset m_rs(&m_DB); 
		m_rs.Open(CRecordset::forwardOnly, csSQL.GetBuffer());

		while(!m_rs.IsEOF())
		{
			strValue.Empty();
			m_rs.GetFieldValue(_T("parm_value"), strValue);
			m_rs.MoveNext();
		}
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

BOOL CNVRBk::isNVRType()
{
	if (m_byProductType == NVR_Type)
	{
		return true;
	}
	else
	{
		return false;
	}
}

BOOL CNVRBk::isLocalLan(const CString& strAddress, bool& isAddSelf)
{
	isAddSelf = false;
	bool bRet(false);
	DWORD nIp = dwIpStringToDword(strAddress);
	do
	{
		if ((nIp & 0x00ffffff) == 0x0000007f) //127.0.0.xx
		{
			isAddSelf = true;
			bRet = true;
			break;
		}
		CxNetCardInfo netCard;
		CString csNetIp, csNetMask;
		DWORD nNetip=0, nNetMask=0;

		for (int cj =0; cj < netCard.veNetCard.size(); cj++)
		{
			csNetIp = netCard.veNetCard[cj].IpAddress;
			csNetMask = netCard.veNetCard[cj].subnet;
			nNetip = dwIpStringToDword(csNetIp);
			nNetMask = dwIpStringToDword(csNetMask);
			
			if ((nIp & nNetMask) == (nNetip & nNetMask))
			{
				if (csNetIp == strAddress)
				{
					isAddSelf = true;
				}

				bRet = true;
				break;
			}           
		}
	}while(false);
	return bRet;
}

CString CNVRBk::strIpDwordToString(const DWORD& dwIp)
{
	CString str;
	byte byIp1,byIp2,byIp3,byIp4;
	
	byIp1 = (dwIp&0xff000000)>>24;
	byIp2 = (dwIp&0x00ff0000)>>16;
	byIp3 = (dwIp&0x0000ff00)>>8;
	byIp4 = (dwIp&0x000000ff);

	str.Format(_T("%d.%d.%d.%d"), byIp1, byIp2, byIp3, byIp4);
	return str;
}

DWORD CNVRBk::dwIpStringToDword(const CString& csIp, bool bForward)
{
	DWORD nRet;
	CString ip = csIp;
	TCHAR* tzToken;
	tzToken = _tcstok(ip.GetBuffer(), _T("."));
	BYTE ipValue[4]={};
	int ci(0);

	if (bForward)
		ci=3;
	
	while (tzToken)
	{
		if (bForward)
		{
			ipValue[ci--] = _ttoi(tzToken);
		} 
		else
		{
			ipValue[ci++] = _ttoi(tzToken);
		}
		
		tzToken = _tcstok(NULL, _T(".\r\n"));

		if (bForward)
		{
			if (ci<0) break;
		} 
		else
		{
			if (ci>=4) break;
		}
	}
	nRet = *(int*)ipValue;
	return nRet;

}

BOOL CNVRBk::isStation(CString& strBuffer, const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword)
{
	bool bResult = false;
	CString strURL;
	Curlplus CGIScanData;

	strURL.Format(_T("http://%s:%s/config/AddCamera.cgi"), strAddress, strPort);
	strBuffer = CGIScanData.RemoteGet(strURL, strUserName, strPassword);

	if (strBuffer.GetLength() > 0)
	{
		if (strBuffer.Find(_T("CameraInfo")) != -1)
		{
			bResult = true;
		}
		else
		{
			bResult = false;
		}
	}
	else
	{
		bResult = false;
	}
	return bResult;
}

MODELTYPE CNVRBk::GetDeviceModelType(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword)
{
	MODELTYPE type = ERROR_TYPE;
	Curlplus CGIScanData;
	CString csOldUrl, csOldBuffer, csNewUrl, csNewBuffer;
	CString csMsg(_T(""));

	csOldUrl.Format(_T("http://%s:%s/config/stream_info.cgi"), strAddress, strPort);	
	csOldBuffer = CGIScanData.get(csOldUrl, strUserName, strPassword);

	csNewUrl.Format(_T("http://%s:%s/config/stream.cgi"), strAddress, strPort);
	csNewBuffer = CGIScanData.get(csNewUrl, strUserName, strPassword);

	if(csOldBuffer.Find(_T("vprofilenum")) != -1 && csNewBuffer.Find(_T("rtsp_port")) == -1)
	{
		type = Model_1150;
	}
	else if(csOldBuffer.Find(_T("vprofilenum")) == -1 && csNewBuffer.Find(_T("rtsp_port")) != -1)
	{
		type = Model_1180;
	}
	else if(csOldBuffer.Find(_T("401")) != -1 && csNewBuffer.Find(_T("401")) != -1)
	{
		type = ERROR_TYPE;
	}
	else if(csOldBuffer == _T("") && csNewBuffer == _T(""))
	{
		type = ERROR_TYPE;
	}

	return type;
}

CJason CNVRBk::GetCGI(const CString& strCGI, const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;
	CString csBuffer;
	CString csCGI = strCGI;
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);

	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);

	return jason;
}

CJason CNVRBk::GetNetwork(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;
	CString csBuffer;
	CString csCGI(_T("network.cgi"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);
	
	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);

	return jason;
}

CJason CNVRBk::GetCommonInfo(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;
	CString csBuffer;
	CString csCGI(_T("info.cgi"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);
	csUrl.Replace(_T("config"), _T("common"));

	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);
	return jason;
}

CJason CNVRBk::GetInfo(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;
	CString csBuffer;
	CString csCGI(_T("info.cgi"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);

	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);
	return jason;
}

CJason CNVRBk::GetAudio(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;
	CString csBuffer;
	CString csCGI(_T("audio.cgi"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);

	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);

	return jason;
}

CJason CNVRBk::GetRS485(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;
	CString csBuffer;
	CString csCGI(_T("rs485.cgi"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);

	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);

	return jason;
}

CJason CNVRBk::GetIO(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;
	CString csBuffer;
	CString csCGI(_T("io.cgi"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);

	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);
	return jason;
}

CJason CNVRBk::GetMcp_Lens(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;	
	CString csBuffer;
	CString csCGI(_T("mcp.cgi?mcp=Lens"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);

	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);

	return jason;
}

CJason CNVRBk::GetMcp_System(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;	
	CString csBuffer;
	CString csCGI(_T("mcp.cgi?mcp=System"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);

	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);

	return jason;
}

CJason CNVRBk::GetMcp_Video(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;	
	CString csBuffer;
	CString csCGI(_T("mcp.cgi?mcp=Video"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);

	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);

	return jason;
}

CJason CNVRBk::GetMcp(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;	
	CString csBuffer;
	CString csCGI(_T("mcp.cgi"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);

	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);

	return jason;
}

CJason CNVRBk::GetSensor(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;
	CString csBuffer;
	CString csCGI(_T("sensor.cgi"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);
	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);
	return jason;
}

CJason CNVRBk::GetVideo_Profile(int nProfileId, const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;
	CString csBuffer;
	CString csCGI;
	csCGI.Format(_T("video.cgi?profileid=%d"),nProfileId);
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);
	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);
	return jason;
}

CJason CNVRBk::GetVideo_Info(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;
	CString csBuffer;
	CString csCGI(_T("video_info.cgi"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);
	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);
	return jason;
}

CJason CNVRBk::GetVideo(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;
	CString csBuffer;
	CString csCGI(_T("video.cgi"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);

	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);
	return jason;
}

CJason CNVRBk::GetStream(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;	
	CString csBuffer;
	CString csCGI(_T("stream.cgi"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);
	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);
	return jason;
}

CJason CNVRBk::GetStream_Info(const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CString& strMac)
{
	Curlplus CGIScanData;
	CString csBuffer;
	CString csCGI(_T("stream_info.cgi"));
	CString csUrl = CombineURL(csCGI, strAddress,strPort, strMac);

	csBuffer = CGIScanData.get(csUrl, strUserName, strPassword);
	CJason jason(csBuffer);
	return jason;
}
						
CJason CNVRBk::SetVideo(map<CString,CString>& mapValue, const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CJason& jason, const CString& strMac)
{
	int nIdx = 0,nCount = 0;
	CJason jasonVideo = jason;
	CString strKey;
	CString str;
	map<CString,CString>::iterator it;
	map<CString, CString> mapJason;
	bool bFail = false;

	if (jasonVideo.isError())
	{
		jasonVideo = GetVideo(strAddress,strPort,strUserName,strPassword,strMac);
	}
	
	jasonVideo.GetValuebyKey(_T("sensor_mode"),str);
	it = mapValue.find(_T("sensor_mode"));
	if(it != mapValue.end())
		str = it->second;

	CString strURL;
	strURL.Format(_T("%s?sensor_mode=%s&"),
		CombineURL(_T("video.cgi"),strAddress,strPort,strMac), str);
	
	it = mapValue.find(_T("max_profile"));
	if (it != mapValue.end())
	{	
		nCount = _ttoi(it->second.Left(1));
	}
	
	for (nIdx=0 ; nIdx < nCount ; nIdx++)
	{
		strKey.Format(_T("profile_id_%d"),nIdx+1);
		jasonVideo.GetValuebyKey(strKey,mapJason);
		it = mapJason.find(_T("gopsize"));
		if(it == mapJason.end())
			str.Format(_T("gopsize_p%d=%d&"),nIdx+1,4);
		else
			str.Format(_T("gopsize_p%d=%s&"),nIdx+1,it->second);
		strURL += str;

		strKey.Format(_T("profile_id_%dframerate"),nIdx+1);
		it = mapValue.find(strKey); 
		if(it == mapValue.end())bFail = true;
		str.Format(_T("framerate_p%d=%s&"),nIdx+1,it->second);
		strURL += str;

		strKey.Format(_T("profile_id_%dbitrate"),nIdx+1);
		it = mapValue.find(strKey);
		if(it == mapValue.end())bFail = true;
		str.Format(_T("bitrate_p%d=%s&"),nIdx+1,it->second);
		strURL += str;

		strKey.Format(_T("profile_id_%dresolution"),nIdx+1);
		it = mapValue.find(strKey);
		if(it == mapValue.end())bFail = true;
		str.Format(_T("resolution_p%d=%s&"),nIdx+1,it->second);
		strURL += str;

		strKey.Format(_T("profile_id_%dcodec"),nIdx+1);
		it = mapValue.find(strKey);
		if(it == mapValue.end())bFail = true;
		str.Format(_T("codec_p%d=%s&"),nIdx+1,it->second.MakeUpper());
		strURL += str;

		if (bFail)
		{
			break;
		}
	}

	CString csBuffer;
	if(!bFail)
	{
		Curlplus CGIScanData;
		csBuffer = CGIScanData.get(strURL, strUserName, strPassword);
	}
	jasonVideo = csBuffer;
	return jasonVideo;
}

CJason CNVRBk::SetNetwork(map<CString,CString>& mapValue, const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CJason& jason, const CString& strMac)
{
	CJason jasonNetwork = jason;
	CString csBuffer;
	map<CString,CString>::iterator it1,it2,it3,it4,it5,it6;
	bool bFail = false;

	it1 = mapValue.find(_T("dhcp"));
	it2 = mapValue.find(_T("ip"));
	it3 = mapValue.find(_T("netmask"));
	it4 = mapValue.find(_T("gateway"));
	it5 = mapValue.find(_T("dns1"));
	it6 = mapValue.find(_T("dns2"));

	if(it1==mapValue.end()||it2==mapValue.end()||it3==mapValue.end()||it4==mapValue.end()||it5==mapValue.end()||it6==mapValue.end())
		bFail = true;
	
	if (!bFail)
	{
		CString strURL;
		strURL.Format(_T("%s?dhcp=%s&ip=%s&netmask=%s&gateway=%s&dns1=%s&dns2=%s"),
			CombineURL(_T("network.cgi"),strAddress,strPort,strMac),
			it1->second,it2->second,it3->second,it4->second,it5->second,it6->second);

		Curlplus CGIScanData;
		csBuffer = CGIScanData.get(strURL, strUserName, strPassword);
		strURL = CombineURL(_T("system_reboot.cgi"),strAddress,strPort,strMac);
		CGIScanData.get(strURL, strUserName, strPassword);
	}

	jasonNetwork = csBuffer;
	return jasonNetwork;
}

CJason CNVRBk::SetStream(map<CString,CString>& mapValue, const CString& strAddress, const CString& strPort, const CString& strUserName, const CString& strPassword, const CJason& jason, const CString& strMac)
{
	CJason jasonStream = jason;
	if (jasonStream.isError())
	{
		jasonStream = GetStream(strAddress,strPort,strUserName,strPassword,strMac);
	}

	CString csBuffer;
	map<CString,CString>::iterator it1,it2;
	bool bFail = false;

	it1 = mapValue.find(_T("stream_name"));
	it2 = mapValue.find(_T("rtsp_port"));

	if(it1==mapValue.end()||it2==mapValue.end())
		bFail = true;

	if (!bFail)
	{
		CString strURL,strValue;
		jasonStream.GetValuebyKey(_T("auth"),strValue);
		
		strURL.Format(_T("%s?auth=%s&stream_name=%s&rtsp_port=%s"),
			CombineURL(_T("stream.cgi"),strAddress,strPort,strMac),
			strValue,it1->second,it2->second);

		Curlplus CGIScanData;
		csBuffer = CGIScanData.get(strURL, strUserName, strPassword);
	}

	jasonStream = csBuffer;
	return jasonStream;
}

CString CNVRBk::CombineURL(const CString& strCGI, const CString& strAddress, const CString& strPort, const CString& strMac)
{
	CString strUrl;

	if (strMac.IsEmpty())
	{
		strUrl.Format(_T("http://%s:%s/config/%s"), strAddress, strPort, strCGI);	
	}
	else
	{
		strUrl.Format(_T("http://%s:%s/%s/config/%s"), strAddress, strPort,strMac, strCGI);
	}

	return strUrl;
}

void CNVRBk::UnpackVideoFormat(const CString& strVideoFormat, byte& FourthByte, byte& ThirdByte, byte& SecondByte, byte& FirstByte)
{
	DWORD video_format = 0;
	video_format = _tstoi(strVideoFormat.GetString());
	FirstByte = (video_format & FILTER_FIRST_BYTE);
	SecondByte = (video_format & FILTER_SECOND_BYTE) >> 8;
	ThirdByte = (video_format & FILTER_THIRD_BYTE) >> 16;
	FourthByte = (video_format & FILTER_FOURTH_BYTE) >> 24;
}

DWORD CNVRBk::PackVideoFormat(byte FourthByte, byte ThirdByte, byte SecondByte, byte FirstByte)
{
	DWORD dwVideoFormat = 0;
	dwVideoFormat += FourthByte << 24;
	dwVideoFormat += ThirdByte << 16;
	dwVideoFormat += SecondByte << 8;
	dwVideoFormat += FirstByte;

	return dwVideoFormat;
}

BOOL CNVRBk::IsFishEye(const CString& strVideoFormat)
{
	BOOL bResult = false;
	byte byFirst;
	byte bySecon;
	byte byThird;
	byte byFourth;
	UnpackVideoFormat(strVideoFormat,byFourth,byThird,bySecon,byFirst);
	if (byFirst & FISHEYE)
	{
		bResult = TRUE;
	}
	return bResult;
}

BOOL CNVRBk::IsChannel(const CString& strVideoFormat)
{
	BOOL bResult = false;
	byte byFirst;
	byte bySecon;
	byte byThird;
	byte byFourth;
	UnpackVideoFormat(strVideoFormat,byFourth,byThird,bySecon,byFirst);
	if (byFirst & CHANNEL)
	{
		bResult = TRUE;
	}
	return bResult;
}

BYTE CNVRBk::GetPanarama(const CString& strVideoFormat)
{
	BYTE bResult = 0;
	byte byFirst;
	byte bySecon;
	byte byThird;
	byte byFourth;
	UnpackVideoFormat(strVideoFormat,byFourth,byThird,bySecon,byFirst);
	if (byFirst & PANORAMA_180)
	{
		bResult = PANORAMA_180;
	}

	if (byFirst & PANORAMA_360)
	{
		bResult = PANORAMA_360;
	}
	return bResult;
}

BYTE CNVRBk::GetModelType(const CString& strVideoFormat)
{
	byte byFirst;
	byte bySecon;
	byte byThird;
	byte byFourth;
	UnpackVideoFormat(strVideoFormat,byFourth,byThird,bySecon,byFirst);
	return byThird;
}

BYTE CNVRBk::GetDevice(const CString& strVideoFormat)
{
	byte byFirst;
	byte bySecon;
	byte byThird;
	byte byFourth;
	UnpackVideoFormat(strVideoFormat,byFourth,byThird,bySecon,byFirst);
	return bySecon;
}

void CNVRBk::TransMac2String(BYTE* mac, CString& strMac, bool bDash)
{
	strMac.Empty();
	CString str;
	for (int nIdx=0; nIdx < 6; nIdx++)
	{
		str.Format(_T("%02X"),*(mac+nIdx));
		strMac += str;

		if (bDash && nIdx <5)
		{
			strMac += _T("-");
		}
	}
}

void CNVRBk::TransString2Mac(const CString& strMac, BYTE* mac)
{
	USES_CONVERSION;
	char szMac[13]={0};
	strncpy(szMac, W2A(strMac), 12); 
	for (int ci=0; ci < 6; ci++)
	{
		char szTmp[3]={0};
		memcpy(szTmp, szMac+ci*2, 2);
		int szBuf;
		sscanf_s(szTmp, "%02X", &szBuf);
		*mac++ = (BYTE)szBuf;
	}
}

void CNVRBk::NotifyRecordSvr()
{
	CSA csa;
	HANDLE hCameraRecording = CreateEvent(&csa.m_sa, false, false, RECORDING_STATUS_CHANGE);
	if (hCameraRecording == NULL)	TRACE(_T("Error code :%d"), ::GetLastError());
	SetEvent( hCameraRecording );
	CloseHandle(hCameraRecording);
}

void CNVRBk::NotifyHttpSvr()
{
	CSA csa;
	HANDLE hRestartEvent = ::CreateEvent(&csa.m_sa, false, false, ETROCENTER_HTTP_SERVER);
	if(hRestartEvent == NULL)
	{TRACE(_T("Create Restart Event Error Code: %d"), ::GetLastError());}
	SetEvent(hRestartEvent);
	CloseHandle(hRestartEvent);	
}

void CNVRBk::NotifyEventSvr()
{
	CSA csa;
	HANDLE hRestartEvent = ::CreateEvent(&csa.m_sa, false, false, EVENT_SERVER_RESTART);
	if(hRestartEvent == NULL)
	{TRACE(_T("Create Restart Event Error Code: %d"), ::GetLastError());}
	SetEvent(hRestartEvent);
	CloseHandle(hRestartEvent);
}

CString CNVRBk::get_ONVIF_RTSP(CString url, CString csUser, CString csPWD)
{
	return ONVIF_GET_RTSP(url, csUser, csPWD);
}

void CNVRBk::scan_ONVIF()
{
	USES_CONVERSION;
	CString csIP, csName, csModel, csMac;
	PDEVICE_LIST pDeviceList = ONVIF_Discovery(1);
	vector<DEV_CAMERA> vcONVIFDevice;
	m_vcOnvif.clear();

	if(pDeviceList)
	{
		NODEITEM Node = {0};
		CString csName;
		DEV_CAMERA *pDevice = NULL;
		int nIdx = 0, nCount = pDeviceList->dwFound;
		DWORD dwTime = 0;
		for(nIdx = 0; nIdx < nCount; nIdx++)
		{
			memset(&Node,0x00,sizeof(NODEITEM));
			pDevice = &pDeviceList->Camera[nIdx];
			csIP.Format(_T("%s"), A2W(inet_ntoa(pDevice->ip.in)));
			strncpy(Node.ip , W2A(csIP), csIP.GetLength());
			csModel.Format(_T("%s"), A2W(pDevice->Name));
			strncpy(Node.camera_name , W2A(csModel), csModel.GetLength());
			strncpy(Node.modelname , W2A(csModel), csModel.GetLength());
	
			if(strcmp(Node.modelname, "hisi") == 0)
			{
				if (dwTime == GetTickCount())
					dwTime += 1;
				else
					dwTime = GetTickCount();
				csMac.Format(_T("%012d"), dwTime);
			}
			else
			{
				TransMac2String(pDevice->MAC, csMac, false);
			}
			TransString2Mac(csMac, Node.mac);

			Node.byDeviceType = ONVIF;
			Node.nWebPort = pDevice->wPort;
			wcscpy(Node.stream_url , A2W(pDevice->Xml));
			m_vcOnvif.push_back(Node);	
		}
	}
}

PROTOCOL CNVRBk::GetStreamProtocol(const ec_Camera& camObj, int nStreamIdx)
{
	PROTOCOL protocolType = VSMP;

	if (camObj.vcStream.size() > nStreamIdx)
	{
		if  (camObj.vcStream[nStreamIdx].stream_protocol == _T("AVIFILE"))
			protocolType = AVIFILE;
		else if (camObj.vcStream[nStreamIdx].stream_protocol == _T("RTPTCP"))
			protocolType = RTPOVERTCP;
		else if (camObj.vcStream[nStreamIdx].stream_protocol == _T("RTSP"))
			protocolType = RTSP;
		else if (camObj.vcStream[nStreamIdx].stream_protocol == _T("RTPHTTP"))
			protocolType = RTPOVERHTTP;
		else if (camObj.vcStream[nStreamIdx].stream_protocol == _T("CGI"))
			protocolType = CGI;
		else if (camObj.vcStream[nStreamIdx].stream_protocol == _T("CGI_ISAP"))
			protocolType = CGI_ISAP;
		else
			protocolType = VSMP;
	}

	return protocolType;
}

void CNVRBk::GetResAndCMPR(CJason& jVideo, int nStreamIdx, CString& video_res, CString& video_cmpr_type)
{
	CString str;
	str.Format(_T("profile_id_%d"), nStreamIdx+1);
	jVideo.GetValuebyKey(str, _T("resolution"), video_res);
	video_res = GetResolutionDesc(video_res);

	jVideo.GetValuebyKey(str, _T("codec"), video_cmpr_type);
	if(video_cmpr_type == _T(""))
		video_cmpr_type = _T("Unknown");
}

CString CNVRBk::GetResolutionDesc(const CString& csScan)
{
	CString strRes;
	if(csScan == _T("720x480"))
		strRes = _T("D1(NTSC)");
	else if(csScan == _T("352x240"))
		strRes = _T("CIF(NTSC)");
	else if(csScan == _T("176x120"))
		strRes = _T("QCIF(NTSC)");
	else if(csScan == _T("720x576"))
		strRes = _T("D1(PAL)");
	else if(csScan == _T("352x288"))
		strRes = _T("CIF(PAL)");
	else if(csScan == _T("176x144"))
		strRes = _T("QCIF(PAL)");
	else if(csScan == _T("1280x1024"))
		strRes = _T("SXGA");
	else if(csScan == _T("640x480"))
		strRes = _T("VGA");
	else if(csScan == _T("320x240"))
		strRes = _T("QVGA");
	else if(csScan == _T("160x120"))
		strRes = _T("QQVGA(NTSC)");
	else if(csScan == _T("160x128"))
		strRes = _T("QQVGA(PAL)");
	else if(csScan == _T("1920x1080"))
		strRes = _T("FULL HD");
	else if(csScan == _T(""))
		strRes = _T("Unknown");
	else
		strRes = csScan;

	return strRes;
}

void CNVRBk::InitialONVIF()
{
	scan_ONVIF();
}