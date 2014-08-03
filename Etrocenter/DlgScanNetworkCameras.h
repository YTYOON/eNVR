#pragma once
#include "resource.h"
#include "atltypes.h"
#include "afxwin.h"
#include "IPCamDiscovery.h"
#include "afxcmn.h"


// CDlgScanNetworkCameras dialog
struct ListSort
{
	CListCtrl *lcMyList;
	int iSub;
	int seq;
};

class CDlgScanNetworkCameras : public CDialog
{
	DECLARE_DYNAMIC(CDlgScanNetworkCameras)

public:
	CDlgScanNetworkCameras(CWnd* pParent = NULL);   // standard constructor
	CDlgScanNetworkCameras(bool isONVIF, CWnd* pParent = NULL);
	virtual ~CDlgScanNetworkCameras();
	BOOL OnInitDialog(void);

// Dialog Data
	enum { IDD = IDD_DLG_SCAN_CAMERAS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


private:
	vector <NODEITEM*> m_pvtNode;
	int m_AllScanListCount;
	NODEITEM* m_pCheckNode;
	CImageList m_SmallImg;
	map<int, CString> m_mapMacIdx;
	ListSort * m_pList;
	bool m_FirstFlag;
	bool m_isONVIF;
	CNVRBk *m_pbk;
	vector <NODEITEM*> m_pONVIFNode;

	void GetONVIF();
	void InsertONVIF(vector <NODEITEM>& vcONVIFNode);
	void InsertDevice(CListCtrl * pScanList);
	void FilterSameIP(vector<NODEITEM>& vcONVINodes);
	void InsertNodes2List();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRescan();
	int vScanListItemCount(void);
	afx_msg void OnLvnItemchangedListScanCameras(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickListScanCameras(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListScanCameras(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	void vResetNodeItem();
	NODEITEM* GetCheckNodeItem(){return m_pCheckNode;};
};
