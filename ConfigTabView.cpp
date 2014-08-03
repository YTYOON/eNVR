// ConfigTabView.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "ConfigTabView.h"
#include "ConfigSettingDlg.h"

// CConfigTabView

IMPLEMENT_DYNCREATE(CConfigTabView, CScrollView)
IMPLEMENT_DYNCREATE(CVideoTab, CConfigTabView)
IMPLEMENT_DYNCREATE(CSetIPTab, CConfigTabView)
IMPLEMENT_DYNCREATE(CStreamTab, CVideoTab)
IMPLEMENT_DYNCREATE(CStreamProtocolTab, CVideoTab)
IMPLEMENT_DYNAMIC(CWndCtrl, CWnd)
IMPLEMENT_DYNCREATE(CSetStreamTab, CConfigTabView)
IMPLEMENT_DYNCREATE(CInfomationTab, CStreamTab)
  
BEGIN_MESSAGE_MAP(CWndCtrl, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, &CWndCtrl::OnCbnSelchange)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CConfigTabView, CScrollView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
	ON_REGISTERED_MESSAGE(MsgControlLBtnDown, &CConfigTabView::OnControlLBtnDown)
	ON_REGISTERED_MESSAGE(MsgSelectChange, &CConfigTabView::OnSelectChage)
	ON_REGISTERED_MESSAGE(MsgUpdateUI, &CConfigTabView::OnUpdateUI)
END_MESSAGE_MAP()

CConfigTabView::CConfigTabView():m_pDataCmd(NULL),m_bModify(false)
{
	LOGFONT font;
	memset(&font, 0, sizeof(LOGFONT));
	CString str = _T("Microsoft Sans Serif");
	font.lfHeight = 14;
	font.lfCharSet = ANSI_CHARSET;
	_tcscpy(font.lfFaceName,str);
	m_font.CreateFontIndirect(&font);
}

CConfigTabView::~CConfigTabView()
{
}

LRESULT CConfigTabView::OnUpdateUI(WPARAM wparam, LPARAM lparam)
{
	int nIdx = 0, nCount = m_vcWndCtrl.size();

	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		m_vcWndCtrl[nIdx]->DestroyWindow();
		delete m_vcWndCtrl[nIdx];
		m_vcWndCtrl[nIdx] = NULL;
	}
	m_vcWndCtrl.clear();
	InitialUpdate();
	
	return 0 ;
}

LRESULT CConfigTabView::OnSelectChage(WPARAM wparam, LPARAM lparam)
{
	CWndCtrl *pControl = (CWndCtrl*)wparam;
	if (pControl)
	{
		SelectChage(pControl);
	}
	return 0 ;
}

LRESULT CConfigTabView::OnControlLBtnDown(WPARAM wparam, LPARAM lparam)
{
	CWndCtrl *pControl = NULL;
	POINT point; point.x = wparam; point.y = lparam;

	CRect rec;
	int nIdx = 0, nCount = m_vcWndCtrl.size();
	for (nIdx=0 ; nIdx < nCount ; nIdx++)
	{
		m_vcWndCtrl[nIdx]->GetWindowRect(rec);
		if (rec.PtInRect(point))
		{
			pControl = m_vcWndCtrl[nIdx];
			break;
		}
	}

	if (pControl)
	{
		m_bModify = TRUE;
		ControlLBtnDown(pControl);
	}
	
	return 0 ;
}

void CConfigTabView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	DrawView(pDC);
	// TODO: add draw code here
}

void CConfigTabView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	InitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
}

void CConfigTabView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	MoveWndCtrl();

	// TODO: Add your message handler code here
}

void CConfigTabView::OnDestroy()
{
	int nIdx = 0, nCount = m_vcWndCtrl.size();

	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		m_vcWndCtrl[nIdx]->DestroyWindow();
		delete m_vcWndCtrl[nIdx];
		m_vcWndCtrl[nIdx] = NULL;
	}

	CClientDC pDC(this);
	m_font.DeleteObject();

	CView::OnDestroy();

	// TODO: Add your message handler code here
}

CWndCtrl* CConfigTabView::GetCtrlByKey(const CString& str)
{
	CWndCtrl* pCtrl = NULL;
	CString strkey;
	int nIdx = 0, nCount = m_vcWndCtrl.size();
	for (nIdx=0;nIdx<nCount;nIdx++)
	{
		m_vcWndCtrl[nIdx]->GetCtrlKey(strkey);
		if (strkey == str)
		{
			pCtrl = m_vcWndCtrl[nIdx];
			break;
		}
	}
	return pCtrl;
}

CString CConfigTabView::GetRemoteMac(NODEITEM& node)
{
	USES_CONVERSION;

	CString strMac(_T(""));

// 	if (node.dyDeviceType != IPCAMERA)
// 	{
// 		m_pbk->TransMac2String(node.mac, strMac);
// 	}
	return strMac;
}

void CConfigTabView::CreatePasswordBox(const CString& strValue, const CString& strKey)
{
	CWndCtrl *peditText = new CWndCtrl(_T("EDIT"),strKey);
	peditText->Create(_T("EDIT"), NULL, WS_CHILD|WS_VISIBLE|ES_LEFT|ES_READONLY|ES_PASSWORD, CRect(0,0,0,0), this, 1234);
	peditText->SetWindowText(strValue);

	peditText->SetFont(&m_font);
	m_vcWndCtrl.push_back(peditText);
}

void CConfigTabView::CreateStaticBox(const CString& strValue, const CString& strKey)
{
	CWndCtrl *peditText = new CWndCtrl(_T("EDIT"),strKey);
	peditText->Create(_T("EDIT"), NULL, WS_CHILD|WS_VISIBLE|ES_LEFT|ES_READONLY, CRect(0,0,0,0), this, 1111);
	peditText->SetWindowText(strValue);
	peditText->SetFont(&m_font);
	m_vcWndCtrl.push_back(peditText);
}

void CConfigTabView::CreateEditBox(const CString& strValue,const CString& strKey)
{
	CWndCtrl *peditText = new CWndCtrl(_T("EDIT"),strKey);
	peditText->Create(_T("EDIT"), NULL, WS_CHILD|WS_VISIBLE|ES_LEFT|WS_BORDER, CRect(0,0,0,0), this, 1234);
	peditText->SetWindowText(strValue);

	peditText->SetFont(&m_font);
	m_vcWndCtrl.push_back(peditText);
}

void CConfigTabView::CreateComboBox(const CString& strValue, const vector<CString>& vcOption,const CString& strKey)
{  
	CWndCtrl *pCombobox = new CWndCtrl(_T("COMBOBOX"),strKey);
	pCombobox->Create(_T("COMBOBOX"), NULL, WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST, CRect(0,0,0,0), this, 1234);

	int nIdx = 0, nCount = vcOption.size();
	for (nIdx=0 ; nIdx < nCount ; nIdx++)
	{
		((CComboBox*)pCombobox)->InsertString(nIdx, vcOption[nIdx]);
		if (vcOption[nIdx] == strValue)
		{
			((CComboBox*)pCombobox)->SetCurSel(nIdx);
		}
	}
	if(((CComboBox*)pCombobox)->FindString(0, strValue) == CB_ERR)
		((CComboBox*)pCombobox)->SetCurSel(0);

	pCombobox->SetFont(&m_font);
	m_vcWndCtrl.push_back(pCombobox);
}

void CConfigTabView::CreateButton(const CString& strBtnName,const CString& strKey)
{
	CWndCtrl *pBtn = new CWndCtrl(_T("BUTTON"),strKey);
	pBtn->Create(_T("BUTTON"), strBtnName, WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 1234);

	pBtn->SetFont(&m_font);
	m_vcWndCtrl.push_back(pBtn);
}

void CConfigTabView::CreateRadioButton(const CString& strBtnName,const CString& strKey)
{
	CWndCtrl *pBtn = new CWndCtrl(_T("RADIOBUTTON"),strKey);
	pBtn->Create(_T("BUTTON"), strBtnName, BS_RADIOBUTTON |WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 1234);
	pBtn->SetFont(&m_font);
	m_vcWndCtrl.push_back(pBtn);
}

void CConfigTabView::CreateCheckBox(const CString& strBtnName,const CString& strKey)
{
	CWndCtrl *pBtn = new CWndCtrl(_T("CHECKBOX"),strKey);
	pBtn->Create(_T("BUTTON"), strBtnName, BS_CHECKBOX |WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 1234);
	pBtn->SetFont(&m_font);
	m_vcWndCtrl.push_back(pBtn);
}

void CConfigTabView::DrawLine(CDC* pDC, CPoint& point_from, CPoint& point_to)
{
	pDC->MoveTo(point_from.x, point_from.y);
	pDC->LineTo(point_to.x, point_to.y);
}

void CConfigTabView::GetTabSpace(CRect& recTab, CRect& recDeflate, int& nRowHeight)
{
	GetClientRect(&recTab);
	recDeflate = recTab;
	recDeflate.DeflateRect(20,20);
	nRowHeight = recDeflate.Height()/17;
}

void CConfigTabView::CreateIPAdr(const vector<byte>& ipAdr,const CString& strKey)
{
	CWndCtrl *pIPAdr = new CWndCtrl(WC_IPADDRESS,strKey);
	pIPAdr->Create(WC_IPADDRESS, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 1234);
	if (ipAdr.size() > 4 )
	{
		((CIPAddressCtrl*)pIPAdr)->SetAddress(ipAdr[0],ipAdr[1],ipAdr[2],ipAdr[3]);
	}
	m_vcWndCtrl.push_back(pIPAdr);
}

void CConfigTabView::MoveWndCtrl()
{
	if (m_vcRec.size() && m_vcRec.size() <= m_vcWndCtrl.size())
	{
		POINT p; p.x=0;p.y=0;
		ScrollToPosition(p);
		int nIdx = 0, nCount = m_vcRec.size();
		CRect rec;
		CString str;
		GetClientRect(rec);
		CSize csize(rec.Width()-20, m_vcRec[nCount-1].bottom - rec.top + 20 );
		SetScrollSizes(MM_TEXT, csize);

		nCount = m_vcWndCtrl.size();
		for (nIdx = 0; nIdx < nCount; nIdx++)
		{
			m_vcWndCtrl[nIdx]->ShowWindow(SW_HIDE);
		}

		nCount = m_vcRec.size();
		for (nIdx = 0; nIdx < nCount; nIdx++)
		{
			m_vcWndCtrl[nIdx]->MoveWindow(m_vcRec[nIdx]);
			m_vcWndCtrl[nIdx]->ShowWindow(SW_SHOW);
			m_vcWndCtrl[nIdx]->GetClassName(str);
			if (str == _T("COMBOBOX"))
			{
				::SendMessage(m_vcWndCtrl[nIdx]->m_hWnd,CB_SETITEMHEIGHT,(WPARAM) -1,(LPARAM)13);
			}
		}
	}
}
void CConfigTabView::UpdateUI()
{
	m_vcRec.clear();
	m_vcFirstLv.clear();
	m_vcSecondLv.clear();
	SendMessage(MsgUpdateUI,NULL,NULL);
}

void CVideoTab::InitialUpdate()
{
	USES_CONVERSION;

	CString strValue1, strValue2, str;
	vector<CString> vcStr;
	pair<CString, byte> pairStr;
	vector<pair<CString, byte> > vcVideoProfile;
	int nCount = 0, nIdx = 0;
	NODEITEM node = {0};
	map<CString , CJason> mapJason;
	m_pDataCmd->GetModifyNodeItem(node);
	m_pDataCmd->GetCGI(mapJason);
	
	str.Format(_T("%d"), node.nWebPort);
	if (node.byModelType == MODEL_1150)
	{
		pairStr.first = _T("Resolution");
		pairStr.second = existCtrl;
		m_vcFirstLv.push_back(pairStr);
		m_vcSecondLv.push_back(vcVideoProfile);

		vcStr.clear();
		mapJason[_T("stream_info")].GetValuebyKey(_T("resolutions"), strValue1);
		mapJason[_T("stream_info")].PaserValue(strValue1, vcStr);
		CreateComboBox(vcStr[0], vcStr,_T("resolutions"));
	}
	else
	{
		CreateFrequency(mapJason);
		CreateMaxResolution(mapJason);
		//CreateTVOutPut(mapJason);
		CreateMaxProfile(mapJason);
		CreateAllProfile(mapJason);
	}
}

void CVideoTab::DrawSecondLv(CDC* pDC, const vector<pair<CString, byte> >& vcItem, int nLeft, int& nTop, int nRight, int nGap)
{
	CRect recControl;
	int nIdx = 0, nCount = vcItem.size();
	
	DrawLine(pDC, CPoint(nLeft,nTop-common_gap), CPoint(nRight+ctrl_width,nTop-common_gap));
	
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		nTop += (nGap+common_gap);

		if (vcItem[nIdx].first.IsEmpty())
		{
			recControl.SetRect(nLeft+decrease,nTop, nLeft+decrease+ctrl_width, nTop+ctrl_high);
			m_vcRec.push_back(recControl);
		}
		else
		{
			pDC->TextOut(nLeft+decrease,nTop,vcItem[nIdx].first);

			recControl.SetRect(nRight,nTop, nRight+ctrl_width, nTop+ctrl_high);
			m_vcRec.push_back(recControl);
		}
	}
}

void CVideoTab::SelectChage(CWndCtrl *pWndCtrl)
{
	CString str;
	pWndCtrl->GetCtrlKey(str);
	if (str.Find(_T("frequency")) != -1)
	{
		ChangeResolutionByFrequency(pWndCtrl);
	}
	else if(str.Find(_T("max_profile")) != -1)
	{
		ChangeProfileListByProfileMode(pWndCtrl);
	}
	else if(str.Find(_T("sensor_mode")) != -1)
	{
		ChangeProfileListBySensorMode(pWndCtrl);
	}
}

void CVideoTab::Save()
{
	if(m_bModify)
	{
		USES_CONVERSION;
		CString strCtrl, strValue, str;
		map<CString, CString> mapCtrlValue;
		int nIdx = 0, nCount = m_vcWndCtrl.size();
		for (nIdx=0 ; nIdx < nCount ; nIdx++)
		{
			m_vcWndCtrl[nIdx]->GetCtrlKey(strCtrl);
			m_vcWndCtrl[nIdx]->GetWindowText(strValue);
			mapCtrlValue[strCtrl] = strValue;
		}

		m_pDataCmd->SetVideoCGI(mapCtrlValue);
	}
}

void CVideoTab::DrawView(CDC* pDC)
{
	int nGap = 0;
	CRect rec, recAll, recControl;
	GetTabSpace(recAll, rec, nGap);
	
	int nLeft = rec.left, nTop = rec.top, nRight = rec.Width()/2+rec.left, nIdx = 0, nCount;
	bool bInvalidate = false;

	if (m_vcRec.size() == 0 && m_vcFirstLv.size())
		bInvalidate = true;

	CString str;
	CFont *pfont = NULL;
	CPen pen;
	CBrush br(RGB(255,255,255));
	pDC->FillRect(recAll,&br);
	pen.CreatePen(PS_SOLID, 1, RGB(195,195,195));
	pfont = pDC->SelectObject(&m_font);

	nIdx = 0, nCount = m_vcFirstLv.size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		if(m_vcFirstLv[nIdx].second & hideCtrl)
			continue;
		nLeft = rec.left;
		pDC->SelectObject(&m_font);
		
		if (m_vcFirstLv[nIdx].second == nonexistCtrl && nIdx > 0)
			nTop += nGap;
		
		pDC->TextOut(nLeft,nTop,m_vcFirstLv[nIdx].first);

		if (m_vcFirstLv[nIdx].second == nonexistCtrl)
		{
			DrawSecondLv(pDC, m_vcSecondLv[nIdx], nLeft, nTop, nRight, nGap);
		}
		else
		{
			recControl.SetRect(nRight,nTop, nRight+ctrl_width, nTop+ctrl_high);
			m_vcRec.push_back(recControl);
		}

		pDC->SelectObject(&pen);
		nTop += (nGap+common_gap);
	}

	pDC->SelectObject(pfont);
	pen.DeleteObject();
	br.DeleteObject();

	if (bInvalidate)
		SendMessage(WM_SIZE, NULL, NULL);
}

void CSetIPTab::DrawView(CDC* pDC)
{
	int nGap = 0;
	CRect rec, recAll, recControl;
	GetTabSpace(recAll, rec, nGap);
	int nLeft = rec.left, nRight = rec.Width() / 2 + rec.left, nTop = rec.top;

	bool bInvalidate = false;
	if (m_vcRec.size() == 0 && m_vcStr.size())
	{
		bInvalidate = true;
	}

	CString str;

	CFont *pfont = NULL;
	pfont = pDC->SelectObject(&m_font);

	int nIdx = 0, nCount = m_vcStr.size();
	for (nIdx=0 ; nIdx < nCount ; nIdx++)
	{
		nLeft = rec.left;
		if (m_vcStr[nIdx].IsEmpty())
		{
			recControl.SetRect(nLeft, nTop, nLeft + 100, nTop + 20);
			m_vcRec.push_back(recControl);
		}
		else
		{
			if (m_bDHCP)
			{
				pDC->FillRect(CRect(nLeft+20, nTop, nLeft+20+100, nTop+20), &CBrush(RGB(255,255,255)));
			} 
			else
			{
				pDC->TextOut(nLeft+20, nTop, m_vcStr[nIdx]);
			}
			recControl.SetRect(nRight, nTop, nRight + 150, nTop + 20);
			m_vcRec.push_back(recControl);
		}

		nTop += nGap;
	}

	pDC->SelectObject(pfont);
	ShowIPAddress(!m_bDHCP);

	if (bInvalidate)
		SendMessage(WM_SIZE, NULL, NULL);
}

void CSetIPTab::ShowIPAddress(bool bShow)
{
	CString str;
	int nIdx = 0, nCount = m_vcWndCtrl.size();
	for (nIdx=0 ; nIdx < nCount ; nIdx++)
	{
		m_vcWndCtrl[nIdx]->GetClassName(str);
		if (str.Find(_T("EDIT")) != -1)
		{
			if(bShow)
				m_vcWndCtrl[nIdx]->ShowWindow(SW_SHOW);
			else
				m_vcWndCtrl[nIdx]->ShowWindow(SW_HIDE);
		}

		if (str.Find(_T("BUTTON")) != -1)
		{
			m_vcWndCtrl[nIdx]->GetWindowText(str);
			if (str.Find(_T("Dynamic IP")) != -1)
			{
				((CButton*)m_vcWndCtrl[nIdx])->SetCheck(m_bDHCP);
			} 
			else
			{
				((CButton*)m_vcWndCtrl[nIdx])->SetCheck(!m_bDHCP);
			}
		}
	}	
}

void CSetIPTab::InitialUpdate()
{
	USES_CONVERSION;
	CString str;
	map<CString , CJason> mapJason;
	m_pDataCmd->GetCGI(mapJason);

	mapJason[_T("network")].GetValuebyKey(_T("dhcp"),str);
	if (str.Find(_T("on")) != -1)
	{
		m_bDHCP = true;
	}
	else
	{
		m_bDHCP = false;
	}
	CreateRadioButton(_T("Dynamic IP"),_T("dhcp"));
	CreateRadioButton(_T("Static IP"),_T("static"));

	m_vcStr.push_back(_T(""));
	m_vcStr.push_back(_T(""));

	m_vcStr.push_back(_T("IP Address"));
	mapJason[_T("network")].GetValuebyKey(_T("ip"),str);
	CreateEditBox(str, _T("ip"));

	m_vcStr.push_back(_T("Subnet Mask"));
	mapJason[_T("network")].GetValuebyKey(_T("netmask"),str);
	CreateEditBox(str, _T("netmask"));

	m_vcStr.push_back(_T("Gateway"));
	mapJason[_T("network")].GetValuebyKey(_T("gateway"),str);
	CreateEditBox(str,_T("gateway"));

	m_vcStr.push_back(_T("Primary DNS"));
	mapJason[_T("network")].GetValuebyKey(_T("dns1"),str);
	CreateEditBox(str,_T("dns1"));

	m_vcStr.push_back(_T("Secondary DNS"));
	mapJason[_T("network")].GetValuebyKey(_T("dns2"),str);
	CreateEditBox(str,_T("dns2"));
}

void CSetIPTab::ControlLBtnDown(CWndCtrl *pWndCtrl)
{
	CString str;

	pWndCtrl->GetClassName(str);
	if (str.Find(_T("BUTTON")) != -1)
	{
		pWndCtrl->GetWindowText(str);
		if (str.Find(_T("Dynamic IP")) != -1)
		{
			m_bDHCP = true;
		}
		else
		{
			m_bDHCP = false;
		}
	}
	
	Invalidate(FALSE);
}

void CSetIPTab::Save()
{
	/*int nStatus = AfxMessageBox(_T("This camera will be remove.\nAre you sure to change ip?"),MB_OKCANCEL);

	if (nStatus == IDOK)
	{
		USES_CONVERSION;
		CString strCtrl, strValue, str, strClassName;
		map<CString, CString> mapCtrlValue;
		int nIdx = 0, nCount = m_vcWndCtrl.size();
		int nCheck = 0;
		for (nIdx=0 ; nIdx < nCount ; nIdx++)
		{
			m_vcWndCtrl[nIdx]->GetCtrlKey(strCtrl);
			m_vcWndCtrl[nIdx]->GetWindowText(strValue);
			m_vcWndCtrl[nIdx]->GetClassName(strClassName);

			if (strValue.Find(_T("Dynamic")) != -1 && strClassName.Find(_T("BUTTON")) != -1)
			{
				nCheck = ((CButton*)m_vcWndCtrl[nIdx])->GetCheck();
				if (nCheck == BST_CHECKED)
				{
					strValue = _T("on");
				}
				else
				{
					strValue = _T("off");
				}
			}
			mapCtrlValue[strCtrl] = strValue;
		}

		str.Format(_T("%d"), m_node.nWebPort);
		CString strMac = GetRemoteMac(m_node);
		m_pbk->SetNetwork(mapCtrlValue,A2W(m_node.ip), str, A2W(m_node.szUser), A2W(m_node.szPass),m_jason,strMac);
		RemoveCamera();
	}*/
}

void CSetIPTab::RemoveCamera()
{
	/*CString strMac,strValue;
	ec_Camera camObj;
	m_pbk->TransMac2String(m_node.mac, strMac);
	m_pbk->GetCamera(strMac, camObj);
	m_pbk->DeleteGroupCamera(camObj.cameraid);
	m_pbk->DeleteCamera(camObj.cameraid);
	m_pbk->SendMessage(MsgUpdateView, UM_REFRESH_ALLTREE, 0);*/
}

void CStreamTab::InitialUpdate()
{
	USES_CONVERSION;
	CString strValue, str;
	pair<CString, bool> pairStr;

	map<CString , CJason> mapJason;
	m_pDataCmd->GetCGI(mapJason);

	pairStr.first = _T("Access Name");
	pairStr.second = existCtrl;
	m_vcFirstLv.push_back(pairStr);
	mapJason[_T("stream")].GetValuebyKey(_T("stream_name"), strValue);
	CreateEditBox(strValue,_T("stream_name"));

	pairStr.first = _T("RTSP Port");
	pairStr.second = existCtrl;
	m_vcFirstLv.push_back(pairStr);
	mapJason[_T("stream")].GetValuebyKey(_T("rtsp_port"), strValue);
	CreateEditBox(strValue,_T("rtsp_port"));
}

void CStreamTab::Save()
{
	if (m_bModify)
	{
		USES_CONVERSION;
		CString strCtrl, strValue, str;
		map<CString, CString> mapCtrlValue;
		int nIdx = 0, nCount = m_vcWndCtrl.size();
		for (nIdx=0 ; nIdx < nCount ; nIdx++)
		{
			m_vcWndCtrl[nIdx]->GetCtrlKey(strCtrl);
			m_vcWndCtrl[nIdx]->GetWindowText(strValue);
			mapCtrlValue[strCtrl] = strValue;
		}

		m_pDataCmd->SetStreamCGI(mapCtrlValue);
	}
}

void CStreamProtocolTab::InitialUpdate()
{
	USES_CONVERSION;
	
	CString strValue;
	pair<CString, byte> pairStr;
	vector<pair<CString, byte> >	vcProtocolOption;

	ec_Camera camObj;
	NODEITEM node = {0};

	m_pDataCmd->GetModifyNodeItem(node);
	m_pDataCmd->GetModifyCamObj(camObj);

	if (node.nProtocol == PROTOCOL::VSMP)
	{
		m_strProtocol = _T("TCP");
		pairStr.first = _T("Protocol");
		pairStr.second = nonexistCtrl;
		m_vcFirstLv.push_back(pairStr);

		pairStr.first = _T("");
		pairStr.second = existCtrl;
		vcProtocolOption.push_back(pairStr);
		CreateRadioButton(_T("TCP"),_T("tcp"));
		m_vcSecondLv.push_back(vcProtocolOption);
	}
	else
	{
		CString strValue;
		pair<CString, byte> pairStr;
		vector<pair<CString, byte> >	vcProtocolOption;
		
		//m_pbk->GetCamera(node.cameraid, camObj);

		if (camObj.vcStream.size())
		{
			if (camObj.vcStream[0].stream_protocol.Find(_T("RTSP")) != -1)
			{
				m_strProtocol = _T("UDP");
			}
			else if (camObj.vcStream[0].stream_protocol.Find(_T("RTPTCP")) != -1)
			{
				m_strProtocol = _T("TCP");
			}
			else if (camObj.vcStream[0].stream_protocol.Find(_T("RTPHTTP")) != -1)
			{
				m_strProtocol = _T("HTTP");
			}
			
		}

		pairStr.first = _T("RTSP Over");
		pairStr.second = nonexistCtrl;
		m_vcFirstLv.push_back(pairStr);

		pairStr.first = _T("");
		pairStr.second = existCtrl;
		vcProtocolOption.push_back(pairStr);
		CreateRadioButton(_T("UDP"),_T("udp"));

		pairStr.first = _T("");
		pairStr.second = existCtrl;
		vcProtocolOption.push_back(pairStr);
		CreateRadioButton(_T("TCP"),_T("tcp"));

		pairStr.first = _T("");
		pairStr.second = existCtrl;
		vcProtocolOption.push_back(pairStr);
		CreateRadioButton(_T("HTTP"),_T("http"));
		m_vcSecondLv.push_back(vcProtocolOption);
	}
	
	ShowProtocol();
}

void CStreamProtocolTab::ControlLBtnDown(CWndCtrl *pWndCtrl)
{
	CString str;
	pWndCtrl->GetWindowText(str);
	m_strProtocol = str;
	ShowProtocol();
}

void CStreamProtocolTab::Save()
{
	if(m_bModify)
		m_pDataCmd->SetProtocol(m_strProtocol);
}

void CStreamProtocolTab::ShowProtocol()
{
	CString str;
	int nIdx = 0, nCount = m_vcWndCtrl.size();
	for (nIdx=0 ; nIdx < nCount ; nIdx++)
	{
		m_vcWndCtrl[nIdx]->GetWindowText(str);

		if (m_strProtocol == str)
		{
			((CButton*)m_vcWndCtrl[nIdx])->SetCheck(true);
		}
		else
		{
			((CButton*)m_vcWndCtrl[nIdx])->SetCheck(false);
		}
	}
}

void CSetStreamTab::InitialUpdate()
{
	USES_CONVERSION;
	int nIdx = 0,nCount = 0;
	CString str, strCtrlKey;

	ec_Camera camObj;
	Cam_Group_Cam grpCam;
	NODEITEM node = {0};
	m_pDataCmd->GetModifyNodeItem(node);
	m_pDataCmd->GetModifyGrpCam(grpCam);
	m_pDataCmd->GetModifyCamObj(camObj);

	str.Format(_T("%d"), node.nWebPort);
	pair<CString, byte> pairString;
	vector<CString> vcString;
	vector<pair<CString, byte> > vcCtrls;
	pair<CString, byte> prCtrl;
	
	nCount = camObj.vcStream.size();

	for (nIdx=0 ; nIdx < nCount ; nIdx++)
	{
		str.Format(_T("Stream %d"), nIdx+1);
		vcString.push_back(str);
	}

	pairString.first = _T("Default Liveview Stream");
	pairString.second = nonexistCtrl;
	m_vcFirstLv.push_back(pairString);
	
	prCtrl.first = _T("Stream");
	prCtrl.second = existCtrl;
	vcCtrls.push_back(prCtrl);
	str.Format(_T("Stream %d"), grpCam.streamid);
	CreateComboBox(str,vcString,_T("livestream"));

	prCtrl.first = _T("Codec");
	prCtrl.second = existCtrl;
	vcCtrls.push_back(prCtrl);
	str.Format(_T("%s"), camObj.vcStream[grpCam.streamid-1].video_cmpr_type);
	CreateStaticBox(str,_T("codec"));
	
	prCtrl.first = _T("Resolution");
	prCtrl.second = existCtrl;
	vcCtrls.push_back(prCtrl);
	str.Format(_T("%s"), camObj.vcStream[grpCam.streamid-1].video_res);
	CreateStaticBox(str,_T("resolution"));
	m_vcSecondLv.push_back(vcCtrls);

	pairString.first = _T("Recording Stream");
	pairString.second = nonexistCtrl;
	m_vcFirstLv.push_back(pairString);

	vcCtrls.clear();
	prCtrl.first = _T("Stream");
	prCtrl.second = existCtrl;
	vcCtrls.push_back(prCtrl);
	m_vcSecondLv.push_back(vcCtrls);
	ResetRecordingStreamList(vcString, node);
	CreateComboBox(str,vcString,_T("recording"));
	InitRecordingComboBox();
//	str.Format(_T("Stream %d"), m_grpCam.streamid);
// 	pairString.first = _T("Recording Stream");
// 	pairString.second = false;
// 	m_vcFirstLv.push_back(pairString);
// 
// 	for (nIdx=0 ; nIdx < nCount ; nIdx++)
// 	{
// 		pairString.first = _T("");
// 		pairString.second = true;
// 		vcCtrls.push_back(pairString);
// 
// 		strCtrlKey.Format(_T("recordingstream %d"), nIdx+1);
// 		CreateCheckBox(vcString[nIdx],strCtrlKey);
// 	}
// 	m_vcSecondLv.push_back(vcCtrls);
	//InintilCheckbox();
}

void CSetStreamTab::SelectChage(CWndCtrl *pWndCtrl)
{
	CString str;
	pWndCtrl->GetCtrlKey(str);
	if (str.Find(_T("livestream")) != -1)
	{
		ec_Camera camObj;
		m_pDataCmd->GetModifyCamObj(camObj);
		CWndCtrl *pCtrl = GetCtrlByKey(_T("codec"));
		if (pCtrl)
		{
			pWndCtrl->GetWindowText(str);
			int nStreamid = _ttoi(str.Right(1));
			if(nStreamid && camObj.vcStream.size() > (nStreamid-1))
			{
				str.Format(_T("%s"), camObj.vcStream[nStreamid-1].video_cmpr_type);
				pCtrl->SetWindowText(str);
			}
		}

		pCtrl = GetCtrlByKey(_T("resolution"));
		if (pCtrl)
		{
			pWndCtrl->GetWindowText(str);
			int nStreamid = _ttoi(str.Right(1));
			if(nStreamid && camObj.vcStream.size() > (nStreamid-1))
			{
				str.Format(_T("%s"), camObj.vcStream[nStreamid-1].video_res);
				pCtrl->SetWindowText(str);
			}
		}
	}
}

void CSetStreamTab::InitRecordingComboBox()
{
	CString strCtrl,str;
	CWndCtrl* pCtrl = NULL;
	ec_Camera camObj;
	m_pDataCmd->GetModifyCamObj(camObj);

	pCtrl = GetCtrlByKey(_T("recording"));
	if (pCtrl)
	{
		((CComboBox*)pCtrl)->SelectString(0, _T("No Record"));
		for (int nSet=0; nSet<camObj.vcStream.size(); nSet++)
		{		
			if (camObj.vcStream[nSet].stream_type.Find(_T("RE")) != -1)
			{
				((CComboBox*)pCtrl)->SelectString(0, camObj.vcStream[nSet].stream_name);
				break;
			}
		}
	}
}

void CSetStreamTab::InintilCheckbox()
{
// 	CString strCtrl,str;
// 	int nID = 0;
// 	
// 	int nCount = m_vcWndCtrl.size();
// 	for(int nIdx = 0; nIdx < nCount ; nIdx++)
// 	{
// 		m_vcWndCtrl[nIdx]->GetCtrlKey(strCtrl);
// 		if (strCtrl.Find(_T("recordingstream")) != -1)
// 		{
// 			nID = _ttoi(strCtrl.Right(1));
// 			if (m_camObj.vcStream.size() >= nID)
// 			{
// 				if (m_camObj.vcStream[nID-1].stream_type.Find(_T("RE")) != -1)
// 				{
// 					((CButton*)m_vcWndCtrl[nIdx])->SetCheck(TRUE);
// 				}
// 			}
// 		}
// 	}
}

void CSetStreamTab::Save()
{
	if(m_bModify)
	{
		CString strCtrl, strValue;
		map<CString, CString> mapCtrlValue;
		int nIdx = 0, nCount = m_vcWndCtrl.size();
		for (nIdx=0 ; nIdx < nCount ; nIdx++)
		{
			m_vcWndCtrl[nIdx]->GetCtrlKey(strCtrl);
			m_vcWndCtrl[nIdx]->GetWindowText(strValue);
			mapCtrlValue[strCtrl] = strValue;
		}

		m_pDataCmd->SetStreamInfo(mapCtrlValue);
	}
}

void CSetStreamTab::ControlLBtnDown(CWndCtrl *pWndCtrl)
{
	CString str;
	pWndCtrl->GetCtrlKey(str);
	if (str.Find(_T("recordingstream")) != -1)
	{
		if (((CButton*)pWndCtrl)->GetCheck())
		{
			((CButton*)pWndCtrl)->SetCheck(false);
		}
		else
		{
			((CButton*)pWndCtrl)->SetCheck(true);
		}
	}
}

void CInfomationTab::InitialUpdate()
{
	USES_CONVERSION;
	CString str;
	pair<CString, bool> pairStr;
	vector<Cam_Group> vcGroup;
	vector<CString> vcGroupstr;
	int nIdx = 0,nCount = 0;

	ec_Camera camObj;
	NODEITEM node = {0};
	m_pDataCmd->GetModifyNodeItem(node);
	m_pDataCmd->GetGroups(vcGroup);
	
	pairStr.first = _T("Camera name");
	pairStr.second = true;
	m_vcFirstLv.push_back(pairStr);
	CreateEditBox(A2W(node.camera_name),_T("camera_name"));

	int nGrp = 0;
	nCount = vcGroup.size();
	pairStr.first = _T("Select group");
	pairStr.second = true;
	m_vcFirstLv.push_back(pairStr);
	for ( nIdx=0; nIdx< nCount; nIdx++)
	{
		vcGroupstr.push_back(vcGroup[nIdx].camera_group_desc);
		if (vcGroup[nIdx].cameragroupid == node.nGroup)
		{
			nGrp = nIdx;
		}
	}
	CreateComboBox(vcGroupstr[nGrp],vcGroupstr,_T("group"));

	pairStr.first = _T("IP Address");
	pairStr.second = true;
	m_vcFirstLv.push_back(pairStr);
	CreateStaticBox(A2W(node.ip),_T("camera_ip"));

	pairStr.first = _T("Web port");
	pairStr.second = true;
	m_vcFirstLv.push_back(pairStr);
	str.Format(_T("%d"),node.nWebPort);
	CreateStaticBox(str,_T("camera_port"));

	str.Empty();
	pairStr.first = _T("Mac Address");
	pairStr.second = true;
	m_vcFirstLv.push_back(pairStr);
	CNVRBk::GetBkObject()->TransMac2String(node.mac,str,true);
	CreateStaticBox(str,_T("camera_mac"));

	pairStr.first = _T("User Name");
	pairStr.second = true;
	m_vcFirstLv.push_back(pairStr);
	CreateStaticBox(A2W(node.szUser),_T("username"));

	pairStr.first = _T("Password");
	pairStr.second = true;
	m_vcFirstLv.push_back(pairStr);
	CreatePasswordBox(A2W(node.szPass),_T("password"));

	pairStr.first = _T("PTZ Support");
	pairStr.second = true;
	m_vcFirstLv.push_back(pairStr);
	str.Format(_T("%s"),node.bSupportPTZ?_T("Yes"):_T("No"));
	CreateStaticBox(str,_T("ptz"));
}

void CInfomationTab::Save()
{
	if(m_bModify)
	{
		CString strCtrl, strValue, str;
		map<CString, CString> mapCtrlValue;
		int nIdx = 0, nCount = m_vcWndCtrl.size();
		for (nIdx=0 ; nIdx < nCount ; nIdx++)
		{
			m_vcWndCtrl[nIdx]->GetCtrlKey(strCtrl);
			m_vcWndCtrl[nIdx]->GetWindowText(strValue);
			mapCtrlValue[strCtrl] = strValue;
		}

		m_pDataCmd->SetCameraInfo(mapCtrlValue);
	}
}

void CInfomationTab::GetTabSpace(CRect& recTab, CRect& recDeflate, int& nRowHeight)
{
	GetClientRect(&recTab);
	recDeflate = recTab;
	recDeflate.DeflateRect(20,20);
	nRowHeight = recDeflate.Height()/10;
}