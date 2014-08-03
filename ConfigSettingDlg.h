#pragma once

#include "ConfigData.h"
enum TabsType{DeviceTabs=0, ClientTabs};
const UINT  MsgChangeTabPage = RegisterWindowMessage(_T("MsgChangeTabPage") );

class CNVRBk;
class CConfigTabView;
class CConfigTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CConfigTabCtrl)
public:
	CConfigTabCtrl(){};
private:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct  )
	{
		TC_ITEM     tci;
		TCHAR       szTabText[100] = {0};
		CBrush		br;
		if (m_hWnd == lpDrawItemStruct->hwndItem)
		{
			tci.mask        = TCIF_TEXT;
			tci.pszText = szTabText;
			tci.cchTextMax  = sizeof(szTabText)-1;

			GetItem(lpDrawItemStruct->itemID, &tci);

			CDC *dc = CDC::FromHandle(lpDrawItemStruct->hDC);
			if (lpDrawItemStruct->itemState)
			{
				dc->SetBkColor(RGB(255,255,255));
				br.CreateSolidBrush(RGB(255,255,255));
			}
			else
			{
				dc->SetBkColor(RGB(245,245,245));
				br.CreateSolidBrush(RGB(245,245,245));
			}
			
			dc->FillRect(&lpDrawItemStruct->rcItem, &br);
			dc->TextOut(lpDrawItemStruct->rcItem.left+6,lpDrawItemStruct->rcItem.top+3,tci.pszText);
			br.DeleteObject();
		}
	};
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
	{
		int nPage = GetCurSel();
		if (GetParent())
		{
			GetParent()->SendMessage(MsgChangeTabPage, nPage, NULL);
		}
		*pResult = 0;
	};
};

class CTabManager
{
private:
	int m_nodeIdx;
	CWnd *m_parent;
	TabsType m_tabsType;
	vector<CWnd* > m_vcDeviceTabs;
	vector<CWnd* > m_vcClientTabs;
	CConfigData m_DataCmd;

public:
	CTabManager():m_nodeIdx(0),m_parent(NULL){};
	~CTabManager(){};
	
	static void UpdateCgiFun(void*);
	void GetDeviceTabs(vector<CWnd* >& vcDeviceTabs){m_vcDeviceTabs = vcDeviceTabs;};
	
	void Initial(vector<NODEITEM>& vcNodeItem, CWnd* pParent);
	void ShowTabs(TabsType tapstype);
	void ShowPage(int nPage);
	void SetNodeIdx(int nodeIdx){m_nodeIdx = nodeIdx;}
	void DeletePage();
	void Save();
	void Exsit();
	
private:
	void CreateTable(CConfigTabView* tab, const CString& strTableName, const CRect& rec);
	void CreateDeviceTabs(const CRect& recTab);
	void CreateClientTabs(const CRect& recTab);
	bool GetCGIInfo();
	bool isNeedDeviceTab();
};

class CConfigSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigSettingDlg)

public:
	CConfigSettingDlg(vector<NODEITEM>& vcNodeItem, CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigSettingDlg();

// Dialog Data
	enum { IDD = IDD_CONFIG_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
	CTreeCtrl m_DeviceTree;
	CConfigTabCtrl m_Tab;
	vector<NODEITEM> m_vcNodeItem;
	bool m_isStation;
	NODEITEM m_SelectNode;
	CTabManager m_tabMgr;
	int m_nodeIdx;

private:
	void InitialTree();
	void InitialTabView();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedIpcamButton();
	afx_msg void OnBnClickedClientButton();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	LRESULT OnChangeTabPage(WPARAM,LPARAM);
	LRESULT OnChangeTreeItem(WPARAM,LPARAM);
	afx_msg void OnTvnSelchangedDeviceTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedExit();
};


