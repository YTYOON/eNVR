#pragma once

#include "ConfigSettingDlg.h"
const UINT  MsgControlLBtnDown = RegisterWindowMessage(_T("MsgControlLBtnDown") );
const UINT  MsgSelectChange = RegisterWindowMessage(_T("MsgSelectChange") );
const UINT  MsgUpdateUI = RegisterWindowMessage(_T("MsgUpdateUI") );

class CWndCtrl : public CWnd
{
	DECLARE_DYNAMIC(CWndCtrl)
private:
	CString m_strClassName;		//Control type: button¡Bedit¡Bcombobox
	CString m_strCtrlKey;		//Control Unikey
	bool m_bShow;
public:
	CWndCtrl(const CString& strClass, const CString& strKey)
	{
		m_strClassName = strClass;
		m_strCtrlKey=strKey;
		m_bShow = true;
	};
	virtual ~CWndCtrl(){};

protected:
	DECLARE_MESSAGE_MAP()
public:
	bool isShow(){return m_bShow;};
	bool SetShow(bool bShow){m_bShow=bShow;};
	void GetClassName(CString& str){ str = m_strClassName; };
	void GetCtrlKey(CString& str){ str = m_strCtrlKey; };
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point)
	{
		ClientToScreen(&point);
		GetParent()->SendMessage(MsgControlLBtnDown, point.x, point.y);
		CWnd::OnLButtonDown(nFlags, point);
	};

	afx_msg void OnCbnSelchange()
	{
		GetParent()->SendMessage(MsgSelectChange, (WPARAM)this, NULL);
	}
	afx_msg BOOL OnEraseBkgnd(CDC* pDC)
	{
		pDC->SetBkColor(RGB(0,0,0));
		pDC->SetTextColor(RGB(0,0,0));

		CBrush br(RGB(255,255,255));
		CRect rec;
		GetClientRect(rec);
		pDC->FillRect(rec, &br);
		return true;
	}
};

// CConfigTabView view
class CNVRBk;
class CConfigTabView : public CScrollView
{
	DECLARE_DYNCREATE(CConfigTabView)

protected:
	CConfigTabView(void);
	CConfigTabView(CConfigData* pDataCmd){};      
	virtual ~CConfigTabView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

	void SaveData(){Save();};

protected:
	enum {ctrl_high=15,ctrl_width=170,decrease=20,common_gap=4};
	enum CtrlDynamicType{nonexistCtrl=0x00,existCtrl=0x01,hideCtrl=0x02};
	CFont m_font;
	vector<CWndCtrl*> m_vcWndCtrl;
	vector<CRect> m_vcRec;
	CConfigData *m_pDataCmd;
	BOOL m_bModify;
	vector<pair<CString, byte> > m_vcFirstLv;
	vector<vector<pair<CString, byte> > > 	 m_vcSecondLv;

protected:
	void CreateEditBox(const CString& strValue, const CString& strKey=_T(""));
	void CreateStaticBox(const CString& strValue, const CString& strKey=_T(""));
	void CreatePasswordBox(const CString& strValue, const CString& strKey=_T(""));
	void CreateComboBox(const CString& strValue, const vector<CString>& vcOption,const CString& strKey=_T(""));
	void CreateRadioButton(const CString& strBtnName,const CString& strKey=_T(""));
	void CreateCheckBox(const CString& strBtnName,const CString& strKey=_T(""));
	void CreateButton(const CString& strBtnName,const CString& strKey=_T(""));
	void CreateIPAdr(const vector<byte>& ipAdr,const CString& strKey=_T(""));
	CString GetRemoteMac(NODEITEM& node);
	CWndCtrl* GetCtrlByKey(const CString& str);

	virtual void InitialUpdate(){};
	virtual void MoveWndCtrl();
	virtual void DrawView(CDC* pDC){};
	virtual void ControlLBtnDown(CWndCtrl *pWndCtrl){};
	virtual void SelectChage(CWndCtrl *pWndCtrl){};
	virtual void GetTabSpace(CRect& recTab, CRect& recDeflate, int& nRowHeight);
	virtual void DrawLine(CDC* pDC, CPoint& point_from, CPoint& point_to);
	virtual void Save(){};

	DECLARE_MESSAGE_MAP()
public:
	void UpdateUI();
	LRESULT OnControlLBtnDown(WPARAM,LPARAM);
	LRESULT OnSelectChage(WPARAM,LPARAM);
	LRESULT OnUpdateUI(WPARAM,LPARAM);
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message){return 0;};
};

class CVideoTab : public CConfigTabView
{
	DECLARE_DYNCREATE(CVideoTab)
public:
	CVideoTab(void){};
	CVideoTab(CConfigData* pDataCmd):m_nProfilemode(0),m_b60Hz(false){m_pDataCmd = pDataCmd;};
	~CVideoTab(void){};

private:
	enum profile_mode{mode_1=1, mode_2=2, mode_4=4};
	
	int m_nProfilemode;
	CString m_strSensormode;
	bool  m_b60Hz;

	void GetCodectype(vector<CString>& vcStr)
	{
		vcStr.push_back(_T("H264"));
		vcStr.push_back(_T("MPEG4"));
		vcStr.push_back(_T("MJPEG"));
	}
	void GetFpsList(const CString& strSensor, int nProfilemode, vector<CString>& vcStr)
	{
		CString str;
		fpsBySensor fps;

		m_pDataCmd->GetFpsArray(fps);

		fpsBySensor::iterator it = fps.find(strSensor);

		if (it != fps.end())
		{
			str.Format(_T("profile%d_fps"),nProfilemode);
			fpsByProfile::iterator itres = it->second.find(str);
			if (itres != it->second.end())
			{
				vcStr = itres->second;
			}
		}
	}

	void GetResolutionList(const CString& strSensor, int nProfilemode, vector<vector<CString> >& vcvcStr)
	{
		CString str;
		ResolutionBySensor Resolutions;

		m_pDataCmd->GetResolutionArray(Resolutions);

		ResolutionBySensor::iterator it = Resolutions.find(strSensor);

		if (it != Resolutions.end())
		{
			str.Format(_T("profile%d"),nProfilemode);
			ResolutionByProfile::iterator itres = it->second.find(str);
			if (itres != it->second.end())
			{
				vcvcStr = itres->second;
			}
		}

		if (!m_b60Hz)
		{
			if (vcvcStr.size() == 2)
			{
				vcvcStr[1][0] = _T("736x576");
			}
		}
	}
	void CreateFrequency(map<CString , CJason>& mapJason)
	{
		CString str;
		vector<CString> vcStr;
		pair<CString, byte> pairStr;
		vector<pair<CString, byte> > vcEmpty;

		vcStr.clear();
		vcStr.push_back(_T("60Hz"));
		vcStr.push_back(_T("50Hz"));
		mapJason[_T("sensor")].GetValuebyKey(_T("deflicker"), str);
		pairStr.first = _T("Power Frequency");
		pairStr.second = true;
		m_vcFirstLv.push_back(pairStr);
		m_vcSecondLv.push_back(vcEmpty);
		ChangeFrequency(str);
		CreateComboBox(str+_T("Hz"), vcStr,_T("frequency"));	
	}
	void CreateMaxResolution(map<CString , CJason>& mapJason)
	{
		CString strKey, strValue;
		vector<CString> vcStr;
		pair<CString, byte> pairStr;
		vector<pair<CString, byte> > vcEmpty;

		vcStr.clear();
		mapJason[_T("mcp_video")].GetValuebyKey(_T("Video"), _T("mcp_video_sensor_mode"), strValue);
		mapJason[_T("mcp_video")].PaserValue(strValue, vcStr);
		if (vcStr.size() > 1)
		{
			pairStr.first = _T("Max. Resolution");
			pairStr.second = true;
			m_vcFirstLv.push_back(pairStr);
			m_vcSecondLv.push_back(vcEmpty);

			mapJason[_T("video")].GetValuebyKey(_T("sensor_mode"), strKey);
			CreateComboBox(strKey, vcStr,_T("sensor_mode"));
			m_strSensormode = strKey;
		}
		else
			m_strSensormode = strValue;
	}
	void CreateTVOutPut(map<CString , CJason>& mapJason)
	{
		CString str, strValue;
		vector<CString> vcStr;
		pair<CString, byte> pairStr;
		vector<pair<CString, byte> > vcEmpty;

		mapJason[_T("mcp_tvout")].GetValuebyKey(_T("TvOutput"), _T("mcp_tv_cvbs"), str);
		if (str.Find(_T("yes")) != -1)
		{
			pairStr.first = _T("Enable TV output");
			pairStr.second = true;
			m_vcFirstLv.push_back(pairStr);
			m_vcSecondLv.push_back(vcEmpty);
			CreateCheckBox(_T(""), _T("tvoutput"));
		}
	}
	void CreateMaxProfile(map<CString , CJason>& mapJason)
	{
		CString str, strValue;
		vector<CString> vcStr;
		pair<CString, byte> pairStr;
		vector<pair<CString, byte> > vcEmpty;
		int nCount=0, nIdx=0;

		vcStr.clear();
		pairStr.first = _T("Profile Mode");
		pairStr.second = true;
		m_vcFirstLv.push_back(pairStr);
		m_vcSecondLv.push_back(vcEmpty);
		mapJason[_T("mcp_video")].GetValuebyKey(_T("Video"), _T("mcp_video_max_profile"), strValue);
		nCount = _ttoi(strValue);
		for (nIdx = 0; nIdx < nCount ; nIdx++)
		{
			if (nIdx == 2)
				continue;

			str.Format(_T("%d Profile"), nIdx+1);
			vcStr.push_back(str);
		}
		m_nProfilemode = mapJason[_T("video")].GetCountByKey(_T("profile_id"));
		str.Format(_T("%d Profile"), m_nProfilemode);
		CreateComboBox(str, vcStr,_T("max_profile"));
	}
	vector<CString> GetFpsList(int nMaxFps)
	{
		CString str;
		vector<CString> vcStr;
		int nIdx = 0, nCount = nMaxFps;
		for(nIdx=0;nIdx<nCount;nIdx++)
		{
			str.Format(_T("%d"),nIdx+1);
			vcStr.push_back(str);
		}
		return vcStr;
	}
	void CreateAllProfile(map<CString , CJason>& mapJason)
	{
		CString strValue, str;
		vector<vector<CString> > vcvcStr;
		vector<CString> vcStr,vcFps;
		pair<CString, byte> pairStr;
		vector<pair<CString, byte> > vcVideoProfile;
		int nCount=0, nIdx=0, nMaxCount = 0, nMaxFps = 0;
		map<int,CString> mapDefaultFramerate;
		map<int,CString> mapDefaultBitrate;
		map<int,CString>::iterator it;

		m_pDataCmd->GetDefaultBitrate(mapDefaultBitrate);
		m_pDataCmd->GetDefaultFramerate(mapDefaultFramerate);
		mapJason[_T("mcp_video")].GetValuebyKey(_T("Video"), _T("mcp_video_max_profile"), strValue);
		nMaxCount = _ttoi(strValue);

		nCount = mapJason[_T("video")].GetCountByKey(_T("profile_id"));
		GetResolutionList(m_strSensormode, m_nProfilemode,vcvcStr);
		GetFpsList(m_strSensormode, m_nProfilemode,vcFps);

		for (nIdx = 0; nIdx < nMaxCount ; nIdx++)
		{
			vcVideoProfile.clear();

			str.Format(_T("%s_%d"), _T("profile_id"), nIdx+1);
			pairStr.first.Format(_T("Profile %d"), nIdx+1);
			pairStr.second = nonexistCtrl;
			if ((nIdx+1) > m_nProfilemode)
				pairStr.second |= hideCtrl;
			m_vcFirstLv.push_back(pairStr);

			vcStr.clear();
			pairStr.first = _T("Resolution");
			pairStr.second = true;
			vcVideoProfile.push_back(pairStr);
			mapJason[_T("video")].GetValuebyKey(str, _T("resolution"), strValue);
			if(vcvcStr.size() > nIdx)
				vcStr = vcvcStr[nIdx];
			CreateComboBox(strValue, vcStr,str+_T("resolution"));

			pairStr.first = _T("Max Bitrate");
			pairStr.second = true;
			vcVideoProfile.push_back(pairStr);
			mapJason[_T("video")].GetValuebyKey(str, _T("bitrate"), strValue);
			if(strValue.IsEmpty()) 
			{
				it = mapDefaultBitrate.find(nIdx+1);
				if (it!=mapDefaultBitrate.end())
				{
					strValue = it->second;
				}
			}
			CreateEditBox(strValue, str+_T("bitrate"));

			vcStr.clear();
			pairStr.first = _T("Encode Type");
			pairStr.second = true;
			vcVideoProfile.push_back(pairStr);
			mapJason[_T("video")].GetValuebyKey(str, _T("codec"), strValue);
			GetCodectype(vcStr);
			CreateComboBox(strValue, vcStr,str+_T("codec"));

			pairStr.first = _T("Max Frame Rate ");
			pairStr.second = true;
			vcVideoProfile.push_back(pairStr);
			mapJason[_T("video")].GetValuebyKey(str, _T("framerate"), strValue);
			if(strValue.IsEmpty()) 
			{
				it = mapDefaultFramerate.find(nIdx+1);
				if (it!=mapDefaultFramerate.end())
				{
					strValue = it->second;
				}
			}
			nMaxFps = _ttoi(strValue);
			if(vcFps.size() > nIdx) nMaxFps = _ttoi(vcFps[nIdx]);
			CreateComboBox(strValue, GetFpsList(nMaxFps) ,str+_T("framerate"));
			m_vcSecondLv.push_back(vcVideoProfile);
		}
	}
	bool ChangeFrequency(const CString& str)
	{
		bool bResult = false;
		bool b60Hz = false;
		int nHz = _ttoi(str.Left(2));
		nHz==60?b60Hz=true:b60Hz=false;
		if (b60Hz != m_b60Hz)
		{
			m_b60Hz = b60Hz;
			bResult = true;
		}
		return bResult;
	}
	void ChangeResolutionByFrequency(CWndCtrl *pWndCtrl)
	{
		CWndCtrl* pCtrl = NULL;
		CString str;
		pWndCtrl->GetWindowText(str);
		if (ChangeFrequency(str) && m_nProfilemode == mode_2)
		{
			pCtrl = GetCtrlByKey(_T("profile_id_2resolution"));
			if (pCtrl)
			{
				((CComboBox*)pCtrl)->DeleteString(0);
				if (m_b60Hz)
				{
					((CComboBox*)pCtrl)->InsertString(0,_T("736x480"));
					((CComboBox*)pCtrl)->SelectString(0,_T("736x480"));
				} 
				else
				{
					((CComboBox*)pCtrl)->InsertString(0,_T("736x576"));
					((CComboBox*)pCtrl)->SelectString(0,_T("736x576"));
				}
			}
		}
	}

	bool ChangeProfileMode(const CString& str)
	{
		bool bResult = false;
		int nProfileMode = _ttoi(str.Left(1));
		if (nProfileMode != m_nProfilemode)
		{
			bResult = true;
			m_nProfilemode = nProfileMode;
		}
		return bResult;
	}
	void ChangeProfileListByProfileMode(CWndCtrl *pWndCtrl)
	{
		CString str;
		pWndCtrl->GetWindowText(str);
		if (ChangeProfileMode(str))
		{
			vector<vector<CString> > vcvcStr;
			vector<CString> vcStr;
			GetResolutionList(m_strSensormode, m_nProfilemode,vcvcStr);
			GetFpsList(m_strSensormode, m_nProfilemode,vcStr);
			UpdateProfileCtrl(vcvcStr,vcStr);
		}
	}

	bool ChangeSensorMode(const CString& str)
	{
		bool bResult = false;
		if (m_strSensormode.Compare(str) != 0)
		{
			bResult = true;
			m_strSensormode = str;
		}
		return bResult;
	}
	void ChangeProfileListBySensorMode(CWndCtrl *pWndCtrl)
	{
		CString str;
		pWndCtrl->GetWindowText(str);
		if (ChangeSensorMode(str))
		{
			vector<vector<CString> > vcvcStr;
			vector<CString> vcStr;
			GetResolutionList(m_strSensormode, m_nProfilemode,vcvcStr);
			GetFpsList(m_strSensormode, m_nProfilemode,vcStr);
			UpdateProfileCtrl(vcvcStr,vcStr);
		}
	}
	void UpdateProfileCtrl(const vector<vector<CString>>& vcvcRes, const vector<CString>& vcFps)
	{
		CWndCtrl* pCtrlRes = NULL, *pCtrlFps = NULL;
		int nmodeIdx=0;
		int nidx=0, ncount=0;
		CString strRes, strFps, str;
		vector<CString> vcStr;

		for (nmodeIdx=0; nmodeIdx < m_nProfilemode; nmodeIdx++)
		{
			strRes.Format(_T("profile_id_%dresolution"),nmodeIdx+1);
			strFps.Format(_T("profile_id_%dframerate"),nmodeIdx+1);
			pCtrlRes = GetCtrlByKey(strRes);
			pCtrlFps = GetCtrlByKey(strFps);
			
			if (pCtrlRes && vcvcRes.size() == m_nProfilemode &&
				pCtrlFps && vcFps.size() == m_nProfilemode)
			{
				((CComboBox*)pCtrlRes)->ResetContent();
				nidx=0, ncount=vcvcRes[nmodeIdx].size();
				for (nidx=0;nidx<ncount;nidx++)
				{
					((CComboBox*)pCtrlRes)->InsertString(-1,vcvcRes[nmodeIdx][nidx]);
					((CComboBox*)pCtrlRes)->SetCurSel(0);
				}

				((CComboBox*)pCtrlFps)->ResetContent();
				nidx = 0, ncount = _ttoi(vcFps[nmodeIdx]);
				vcStr = GetFpsList(ncount);
				for (nidx=0;nidx<ncount;nidx++)
				{
					((CComboBox*)pCtrlFps)->InsertString(-1,vcStr[nidx]);
					((CComboBox*)pCtrlFps)->SetCurSel(0);
				}
			}
		}

		ncount=m_vcFirstLv.size();
		for (nidx=0;nidx<ncount;nidx++)
		{
			m_vcFirstLv[nidx].second &= ~hideCtrl;

			if ((m_vcFirstLv[nidx].first.Find(_T("Profile 2")) != -1) && m_nProfilemode == mode_1)
			{
				m_vcFirstLv[nidx].second |= hideCtrl;
			}
			else if ((m_vcFirstLv[nidx].first.Find(_T("Profile 3")) != -1) && 
				(m_nProfilemode == mode_1 || m_nProfilemode == mode_2))
			{
				m_vcFirstLv[nidx].second |= hideCtrl;
			}
			else if ((m_vcFirstLv[nidx].first.Find(_T("Profile 4")) != -1) &&
				(m_nProfilemode == mode_1 || m_nProfilemode == mode_2))
			{
				m_vcFirstLv[nidx].second |= hideCtrl;
			}
		}
		m_vcRec.clear();
		CClientDC dc(this);
		DrawView(&dc);
		Invalidate(false);
	}

protected:
	virtual void InitialUpdate();
	virtual void SelectChage(CWndCtrl *pWndCtrl);
	virtual void DrawView(CDC* pDC);
	virtual void DrawSecondLv(CDC* pDC, const vector<pair<CString, byte> >& vcItem, int nLeft, int& nTop, int nRight, int nGap);
	virtual void Save();
};

class CSetIPTab : public CConfigTabView
{
	DECLARE_DYNCREATE(CSetIPTab)
public:
	CSetIPTab(void){};
	CSetIPTab(CConfigData* pDataCmd){m_pDataCmd = pDataCmd;};
	~CSetIPTab(void){};

private:
	vector<CString> m_vcStr;
	bool m_bDHCP;

	void ShowIPAddress(bool bShow);
	void RemoveCamera();
protected:
	virtual void InitialUpdate();
	virtual void DrawView(CDC* pDC);
	virtual void ControlLBtnDown(CWndCtrl *pWndCtrl);
	virtual void Save();
};

class CStreamTab : public CVideoTab
{
	DECLARE_DYNCREATE(CStreamTab)
public:
	CStreamTab(void){};
	CStreamTab(CConfigData* pDataCmd){m_pDataCmd = pDataCmd;};
	~CStreamTab(void){};

protected:
	virtual void InitialUpdate();
	virtual void Save();
};

class CStreamProtocolTab : public CVideoTab
{
	DECLARE_DYNCREATE(CStreamProtocolTab)
public:
	CStreamProtocolTab(void){};
	CStreamProtocolTab(CConfigData* pDataCmd){m_pDataCmd = pDataCmd;};
	~CStreamProtocolTab(void){};

private:
	CString m_strProtocol;

	void ShowProtocol();
protected:
	virtual void InitialUpdate();
	virtual void ControlLBtnDown(CWndCtrl *pWndCtrl);
	virtual void DrawLine(CDC* pDC, CPoint& point_from, CPoint& point_to){};
	virtual void Save();
};

class CSetStreamTab : public CVideoTab
{
	DECLARE_DYNCREATE(CSetStreamTab)
public:
	CSetStreamTab(void){};
	CSetStreamTab(CConfigData* pDataCmd){m_pDataCmd = pDataCmd;};
	~CSetStreamTab(void){};

private:
	void InintilCheckbox();
	void InitRecordingComboBox();
	void ResetRecordingStreamList(vector<CString>& vcString, const NODEITEM& node)
	{
		if (node.byDeviceType == eNVR || node.byDeviceType == IPVSTATION)
		{
			vcString.clear();
		}
		vcString.push_back(_T("No Record"));
	}

protected:
	virtual void InitialUpdate();
	virtual void ControlLBtnDown(CWndCtrl *pWndCtrl);
	virtual void Save();
	virtual void DrawLine(CDC* pDC, CPoint& point_from, CPoint& point_to){};
	virtual void SelectChage(CWndCtrl *pWndCtrl);
};

class CInfomationTab : public CStreamTab
{
	DECLARE_DYNCREATE(CInfomationTab)

public:
	CInfomationTab(void){};
	CInfomationTab(CConfigData* pDataCmd){m_pDataCmd = pDataCmd;};
	~CInfomationTab(void){};

protected:
	virtual void InitialUpdate();
	virtual void GetTabSpace(CRect& recTab, CRect& recDeflate, int& nRowHeight);
	virtual void DrawLine(CDC* pDC, CPoint& point_from, CPoint& point_to){};
	virtual void Save();
	
};

