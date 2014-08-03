#pragma once
#include "resource.h"
#include "BaseDialog.h"
#include "mycurl.h"
#include "xNetCardInfo.h"
#include "MainFrm.h"
#include <vector>
#include "afxcmn.h"

using namespace std;

#define UNAUTHORIZED 12
#define OLD_TYPE 10
#define NEW_TYPE 11
#define ERROR_TYPE 13

class CNVRBk;
// CDlgAuthorize
class CDlgAuthorize : public CDialog
{
	DECLARE_DYNAMIC(CDlgAuthorize)
public:
	CDlgAuthorize(DWORD dwDlgID, CString* pStrMsg = NULL, CString* pStrPaw = NULL, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAuthorize();
	enum { IDD = IDD_DLG_AUTHORIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
private:
	CString m_strModelName;
	CString m_strMacAddress;
	CString m_strIPAddress;
	CString m_strID;
	CString m_strPassword;
	CString m_strMsg;
public:
	void vGetIDnPassword(CString& strID, CString& strPassword);
	void vSetMsgString(const CString& strMsg){ m_strMsg = strMsg; }
};

// CListCtrlColorRow
class CListCtrlColorRow : public CListCtrl
{
public:
	CListCtrlColorRow(){};
	virtual ~CListCtrlColorRow(){};

protected:
	afx_msg void OnDrawItemColor(NMHDR * pNMHDR, LRESULT * pResult)
	{
		_ASSERTE(*pResult == 0);

		NMLVCUSTOMDRAW * pnmlvcd = (NMLVCUSTOMDRAW *)pNMHDR;
		DWORD dwDrawStage = pnmlvcd->nmcd.dwDrawStage;
		dwDrawStage &= ~CDDS_SUBITEM;

		switch (dwDrawStage)
		{
		case CDDS_PREPAINT:
			{
				*pResult = CDRF_NOTIFYITEMDRAW;
				break;
			}
		case CDDS_ITEMPREPAINT:
			{
				pnmlvcd->clrText = GetItemData(pnmlvcd->nmcd.dwItemSpec);   
				*pResult = 0;               	
				break;
			}
		case CDDS_SUBITEM:
			{
				break;
			}
		default:
			{
				*pResult = 0;
				break;
			}
		}
	}
	DECLARE_MESSAGE_MAP()
};

class CDlgMassAssignIP : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgMassAssignIP)

public:
	CDlgMassAssignIP(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMassAssignIP();
	enum { IDD = IDD_DLG_MASS_ASSIGNIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	
	LRESULT UpdateListCtrl(WPARAM,LPARAM);

	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioStaticip();
	afx_msg void OnBnClickedRadioDhcp();
	afx_msg void OnBnClickedBtnDetectAvailableIp();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnBnClickedBtnMassSet();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnPrevious();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckAssignIp();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeComboNetcard();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonMassAssignRescan();
	afx_msg void OnClose();

private:
	struct IPCAM_BASE_INFO
	{
		CString csMac;
		CString csUser;
		CString csPassword;
	};
	struct SuccessNode
	{
		NODEITEM* pNode;
		int nJasonMapIdx;
	};
	struct THREAD_INFO
	{
		CDlgMassAssignIP *pMassAssignIP;
		NODEITEM* pNodeItem;
		bool bFinish;
		int		nNodeIdx;
		int		nThreadIdx;
		bool bResult;


		THREAD_INFO()
			:pMassAssignIP(NULL),pNodeItem(NULL),bFinish(false),nNodeIdx(0),bResult(false)
		{}
	};

	class CCheckHeadCtrl : public CHeaderCtrl
	{
	public:
		CCheckHeadCtrl(){};
		virtual ~CCheckHeadCtrl(){};

	protected:
		afx_msg void OnItemClicked(NMHDR* pNMHDR, LRESULT* pResult)
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
		DECLARE_MESSAGE_MAP()
	};

	struct oldCoordinate
	{
		CRect recDlg;
		CRect recGroup;
		CRect recChangeIP;
		CRect recStatic;
		CRect recDHCP;
		CRect recNetwork;
		CRect recCombobox;
		CRect recIPAdress;
		CRect recIPAdress_Enter;
		CRect recIPMask;
		CRect recIPMask_Enter;
		CRect recIPGetway;
		CRect recIPGetway_Enter;
		CRect recIPDNS;
		CRect recIPDNS_Enter;

		CRect recRescan;
		CRect recList;
		CRect recProgrss;
		CRect recProgressBar;

		CRect recPrevious;
		CRect recNext;
		CRect recExit;
		CRect recRun;
	};

	
	enum {NOT_ANY_ACTION_YET=0, PRESET_IP, SETIP, ADDTOTABLE, DONE};
	enum ModifyFailIPArray{ Modify_Remove = 0, Modify_Insert};
    short m_nTimerCountdown;
    CRichEditCtrl m_richEdt;
	HANDLE m_hWaitIpCamWakeup, m_hDestory;
	bool m_bAssignIP;
	WORD m_nStepCursor;
	CImageList	m_checkImgList;
	CCheckHeadCtrl	m_checkHeadCtrl;
	LONG m_nProgress;
	LONG m_btStartIP[4];
	LONG m_btFinalIP[4];
	vector <CString> m_PickUpIP;
	CRITICAL_SECTION m_criSec;
	CListCtrlColorRow m_wndList;
	CxNetCardInfo NCI;
	int iSolveNode, iTotalSolveNode;
	CProgressCtrl* pProgress;
	HANDLE hAddIPCam2DB;
	map<CString, int>	mapPathBuffer;
	vector<THREAD_INFO>	m_vcMultiThreadInfo;
	CDatabase *m_pDB;
	vector<SuccessNode> m_vcSuccessNode;
	vector<THREAD_INFO>	m_vcFailIPAddress;
	BOOL	m_bWriteDatabaseFinish;
	CString m_strID;
	CString m_strPassword;
	int m_nProgress_FullScale;
	int m_nProgress_CurrentSection;
	int m_nProgress_SectionCount;
	oldCoordinate	m_oldCoordinate;
	CNVRBk *m_pBK;
	vector< map<CString, CJason> > m_vcJason;
	vector <IPCAM_BASE_INFO> m_vcExistDevice;
	
public:
	static UINT _stdcall ConnectThread(LPVOID lpParam);
	static UINT _stdcall ConnectOneIPCam(LPVOID lpParam);
	static UINT _stdcall  vSaveCGIData2DB(LPVOID lpParam);
	static UINT _stdcall fnWaitIpCamWakeUp(LPVOID lpParam);
	static UINT _stdcall fnPingIP(LPVOID lpParam);
	static UINT fnWaitForInsertDB(LPVOID lpParam);
	static UINT fnDetectAvailableIP(LPVOID lpParam);

private:
	bool CompareExistDevice(const CString& csMac);
    void vSetGuideStep(DWORD nStep);
	void GetDlgWindowRect(CRect& rec);
	void ShowChangeIPGroup(BOOL bShow);
	void CountRetangle(const CRect& recOriginal, const CRect& recMove, CRect& recResult);
	void KeepOldCoordinate();
	void MoveDlgItem(BOOL bExpand);
	void AddProgress_CurrentSection()
	{
		m_nProgress_CurrentSection++;
	}

	void RestartProgress( int SectionCount );
	void RedrawPorgress();
	CProgressCtrl* GetProgressCtrl(){ return (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1); }
	void vDoStatus(WORD nStatus);
	bool bGetAlreadyDefinedIPCamMac(vector <IPCAM_BASE_INFO>& vtcsMacs);
	bool GetCGI(NODEITEM* pNode, int nJasonMapIdx);
	bool vGetNetWorkInfo(void);
	bool bGetNextSuitableIP(CString& csIP);
	bool bGetNextAvailableIP(char* szIP);
	void vSetListCtrlCheckBoxHeader(void);
	int iGetCameraIDX();
	MODELTYPE iGetIPCamType(CString csIP, CString csWebPort);
	bool bGetOldCGIData(CString csIP, CString csWebPort, int nJasonMapIdx);
	bool bGetNewCGIData(CString csIP, CString csWebPort, int nJasonMapIdx);
	int	iGetListCheckedCount();
	bool bCheckPathBuffer( CString csPath, int iBuffer);
	void vGetPathBufferFromDB();
	BOOL bCheckBuffer(WORD& nStatus);
	void SetGlobalEvent();
	void vUpdatevcFailIPAddress(ModifyFailIPArray ModifyType, THREAD_INFO *pThreadinfo);
	void vSetAuthorize(const CString& strID, const CString& strPassword) { m_strID = strID; m_strPassword = strPassword; };
	void vCheckFailIPAddress();
	void vWaitMultithreadNSaveData(vector<HANDLE>& vcMultiThreadHand);
	void vCreateMultiThread(vector<HANDLE>&	vcMultiThreadHand);
	void vInsertThreadInfo2Array();
	void InitialControlText();
	void InitialColumn();
	void InitialControlStatus();
	void InitialListBox();
	void vGetMacAddress(NODEITEM* pNode, CString& str, BOOL bDash = TRUE);
	void InsertCamera(NODEITEM* pNode, int iIPCamType, int nJasonMapIdx, int nCameraIdx, const vector<ec_Stream>& vcStream);
	void GetCameraStream(NODEITEM* pNode, int iIPCamType, int nJasonMapIdx, vector<ec_Stream>& vcStream);
	void InsertCamRec(int nCameraIdx);
	void InsertGroupCamera(int nCamId, int nCameraIdx);
	void GetConnectType(int nType, CString& connect_type_code);
	void GetDigitalStatus(CJason& jason, const CString& strKey, CString& strValue);
	void GetVideoFormat(NODEITEM* pNode, CJason& jason, CString& video_format);
	void GetStreamProtocol(int nStreamIdx, map<CString,CJason>& mapJason, int iIPCamType, CString& stream_protocol);
	void GetNetMask(CJason& jason, ec_Camera& camObj);
	void GetStreamPort(int nStreamIdx,int iIPCamType, NODEITEM* pNode, map<CString,CJason>& mapJason, short& stream_port);
	void GetResAndCMPR(int nStreamIdx, int iIPCamType, map<CString,CJason>& mapJason, CString& video_res, CString& video_cmpr_type);
	void GetPTZSupport(int iIPCamType, map<CString,CJason>& mapJason, CString& ptz_support);
	void GetStreamUrl(int iIPCamType, map<CString,CJason>& mapJason, CString& stream_url);

	CNVRBk *GetNVRBk()
	{
		if (TryEnterCriticalSection(&m_criSec) != 0)
		{
			LeaveCriticalSection(&m_criSec);
			return m_pBK;
		}
		else
		{
			LeaveCriticalSection(&m_criSec);
			return NULL;
		}
	}

	/************************************************************************/
	/*	m_vcMultiThreadInfo Function                                                                      */
	/************************************************************************/
	void vSetMultiThreadInfo(THREAD_INFO strThreadInfo)
	{
		if (TryEnterCriticalSection(&m_criSec) != 0)
		{
			if (m_vcMultiThreadInfo.size() > strThreadInfo.nThreadIdx)
			{
				m_vcMultiThreadInfo[strThreadInfo.nThreadIdx] = strThreadInfo;
				LeaveCriticalSection(&m_criSec);
			}
		}
		else
		{
			LeaveCriticalSection(&m_criSec);
		}
	}

	void vGetMultiThreadInfo(THREAD_INFO& strThreadInfo, int nIdx)
	{
		if (TryEnterCriticalSection(&m_criSec) != 0)
		{
			if (m_vcMultiThreadInfo.size() > nIdx)
			{
				strThreadInfo = m_vcMultiThreadInfo[nIdx];
			}
			LeaveCriticalSection(&m_criSec);
		}
		else
		{
			LeaveCriticalSection(&m_criSec);
		}
	}

	/************************************************************************/
	/*   m_vcJason Function                                                                   */
	/************************************************************************/
	map<CString, CJason> GetJasonMap(int nIdx)
	{
		map<CString, CJason> mapJason;
		if (TryEnterCriticalSection(&m_criSec) != 0)
		{
			if (m_vcJason.size() > nIdx)
			{
				mapJason = m_vcJason[nIdx];
			}
			LeaveCriticalSection(&m_criSec);
		}
		else
		{
			LeaveCriticalSection(&m_criSec);
		}

		return mapJason;
	}


	/************************************************************************/
	/*   m_vcSuccessNode Function                                                                   */
	/************************************************************************/
	void vSetSuccessNode(NODEITEM* pNode, int nJasonMapIdx, int iIPCamType)
	{
		SuccessNode strData;
		strData.pNode = pNode;
		strData.nJasonMapIdx = nJasonMapIdx;
		//strData.iIPCamType = iIPCamType;
		m_vcSuccessNode.push_back(strData);
	}

	void vGetSuccessNode(vector<SuccessNode>& vcSuccessNode)
	{
		vcSuccessNode = m_vcSuccessNode;
	}

};
