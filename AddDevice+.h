#pragma once


// CAddDevice dialog
class CNVRBk;
class CAddDevice : public CDialog
{
	DECLARE_DYNAMIC(CAddDevice)

public:
	CAddDevice(int nSelectGrpId = 0, CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddDevice();

// Dialog Data
	enum { IDD = IDD_ADDDEVICE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
	enum ErrorMsg{NOT_FOUND_DEVICE=0,SERCHING_DEVICE};
	MODELTYPE m_DeviceType;
	CString m_strPassword;
	vector<NODEITEM> m_vcNodes;
	CNVRBk *m_pbk;
	int m_nSelectGroupId;
	map<CString,CJason> m_mapJason;
	bool m_isExist;
	bool m_isAddSuccess;
	bool m_isONVIF;
	BOOL m_bIP;
	bool m_isStation;
public:
	bool isExistDevice(){return m_isExist;};
private:
	bool isONVIF();
	void UpdateUI();
	void GetKeyinAddress(CString& strAddress, CString& strPort);
	void UpdateRadio();
	bool IsStation(CString& strBuffer);
	void AddDevice();
	bool AddStation(const CString& strBuffer);
	void GetCameraName(CString& strName);
	void GetConnectTypeCode(CString& strConnectTypeCode);
	void GetBrandCode(CString& strBrand_code);
	void GetCameraObj(ec_Camera& camObj);
	void GetDigitalStatus(CJason& jason, const CString& strKey, CString& strValue);
	void GetVideoStream(ec_Camera& camObj);
	void Trans_Camera2NODEITEM(const ec_Camera& camObj, NODEITEM& node);
	bool isExistDevice(ec_Camera& camObj);
	void DisplayError(ErrorMsg msg = NOT_FOUND_DEVICE);
	void GetVideoFormat(CJason& jason, CString& video_format);
	void GetNetMask(CJason& jason, ec_Camera& camObj);
	bool GetCGI();
	void GetPTZSupport(CString& ptz_support);
	void GetStreamUrl(CString& stream_url);
	void GetResAndCMPR(int nStreamIdx, CString& video_res, CString& video_cmpr_type);
	void GetStreamProtocol(int nStreamIdx, CString& stream_protocol);
	void GetStreamPort(int nStreamIdx, short& stream_port);
	void GetONVIFStreamPort(const CString& str, short& nPort);
	void GetONVIFStreamURL(const CString& strRTSP, TCHAR *tzPath);
	bool CheckONVIF();
	void InsertDevice2DB(ec_Camera& camObj);
	void CallConfigDlg();
	static UINT AddDeviceThread (LPVOID lpParam);
	
	DECLARE_MESSAGE_MAP()
public:
	LRESULT OnFinishAddDevice(WPARAM,LPARAM);
	afx_msg void OnBnClickedScanButton();
	afx_msg void OnBnClickedAddButton();
	afx_msg void OnBnClickedIpRadio();
	afx_msg void OnBnClickedUrlRadio();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
