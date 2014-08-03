// xNetCardInfo.h: interface for the CxNetCardInfo class.
//
//////////////////////////////////////////////////////////////////////
using namespace std;
#include <vector>

#if !defined(AFX_XNETCARDINFO_H__FC408BC6_3516_4481_AF7E_0B875C0E2B2C__INCLUDED_)
#define AFX_XNETCARDINFO_H__FC408BC6_3516_4481_AF7E_0B875C0E2B2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct NetCard {
	CString ErrMsg;
	CString macaddress;
	CString description;
	CString type;
	CString subnet;
	CString IpAddress;
	CString gateway;
	CString PrimaryWinsServer;
	CString dhcp;
	CString DNSServer;
};

class CxNetCardInfo  
{
public:
	CxNetCardInfo();
	virtual ~CxNetCardInfo();

private:
	void ParseData();
	void GetInfo();

public:
	vector<NetCard> veNetCard;
	NetCard m_NetCard;

	CString GetErrorMsg(int iNetCard);
	CString GetNetCardWINS(int iNetCard);
	CString GetNetCardDeviceName(int iNetCard);
	CString GetNetCardMACAddress(int iNetCard);
	CString GetDHCPServer(int iNetCard);
	CString GetNetCardGateWay(int iNetCard);
	CString GetNetCardSubnetMask(int iNetCard);
	CString GetNetCardIPAddress(int iNetCard);
	CString GetNetCardType(int iNetCard);
	int GetNetCardNum();
	CString GetNetCardDNS(int iNetCard);
	
private:
	BYTE m_data[4096];
	
	unsigned long len;
	PIP_ADAPTER_INFO pinfo;

	int iNetCardNum;
};

#endif // !defined(AFX_XNETCARDINFO_H__FC408BC6_3516_4481_AF7E_0B875C0E2B2C__INCLUDED_)
