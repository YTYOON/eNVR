// xNetCardInfo.cpp: implementation of the CxNetCardInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "NetCardInfo.h"
#include "xNetCardInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxNetCardInfo::CxNetCardInfo()
{
	m_NetCard.ErrMsg = _T( "" );

	m_NetCard.macaddress = _T( "" );
	m_NetCard.description = _T( "" );
	m_NetCard.type = _T( "" );
	m_NetCard.subnet = _T( "" );
	m_NetCard.IpAddress = _T( "" );
	m_NetCard.gateway = _T( "" );
	m_NetCard.PrimaryWinsServer = _T( "" );
	m_NetCard.dhcp = _T( "" );

	ZeroMemory( m_data,4096 );
	len = 0;
	pinfo = ( PIP_ADAPTER_INFO )m_data;

	GetInfo();
}

CxNetCardInfo::~CxNetCardInfo()
{

}

void CxNetCardInfo::GetInfo()
{
	m_NetCard.ErrMsg = _T( "Success!" );

	unsigned long nError;

	nError = GetAdaptersInfo( pinfo,&len );
	switch( nError ) {
		case 0:
			ParseData();
			break;
		case ERROR_NO_DATA:
			m_NetCard.ErrMsg = _T( "No net device information!" );
			break;
		case ERROR_NOT_SUPPORTED:
			m_NetCard.ErrMsg = _T( "The system not support GetAdaptersInfo API function!" );
			break;
		case ERROR_BUFFER_OVERFLOW:
			nError = GetAdaptersInfo( pinfo,&len );
			if( nError == 0 ) ParseData();
			else m_NetCard.ErrMsg = _T("Unknow error!");
			break;
	}
}

void CxNetCardInfo::ParseData()
{
	iNetCardNum = 0;
	while(pinfo != NULL)
	{
		USES_CONVERSION;
		m_NetCard.macaddress.Format( _T("%02X:%02X:%02X:%02X:%02X:%02X"), pinfo->Address[0], pinfo->Address[1], pinfo->Address[2], pinfo->Address[3], pinfo->Address[4], pinfo->Address[5] );
		m_NetCard.description = pinfo->Description;
		m_NetCard.type.Format(_T("%d"), pinfo->Type);

		PIP_ADDR_STRING pAddressList = &(pinfo->IpAddressList);
		m_NetCard.IpAddress = _T("");
		do {
			m_NetCard.IpAddress += pAddressList->IpAddress.String;
			pAddressList = pAddressList->Next;
			if( pAddressList != NULL ) m_NetCard.IpAddress += _T( "\r\n" );
		}while( pAddressList != NULL );

		m_NetCard.subnet.Format( _T("%s"), A2W(pinfo->IpAddressList.IpMask.String) );
		m_NetCard.gateway.Format( _T("%s"),A2W(pinfo->GatewayList.IpAddress.String) );
		if( pinfo->HaveWins )
			m_NetCard.PrimaryWinsServer.Format( _T("%s"), A2W(pinfo->PrimaryWinsServer.IpAddress.String) );
		else
			m_NetCard.PrimaryWinsServer.Format( _T("%s"),_T("N/A") );
		if( pinfo->DhcpEnabled )
			m_NetCard.dhcp.Format( _T("%s"), A2W(pinfo->DhcpServer.IpAddress.String) );
		else
			m_NetCard.dhcp.Format( _T("%s"),_T("N/A") );
		
		//Get DNS-IP
		PIP_ADAPTER_INFO pAdapter = NULL;
		pAdapter = pinfo;

		DWORD dwErr;
		ULONG nSize = 0;
		PIP_PER_ADAPTER_INFO pPerAdapterInfo = NULL;
		if( (dwErr = GetPerAdapterInfo(pAdapter->Index, pPerAdapterInfo, &nSize)) == ERROR_BUFFER_OVERFLOW )
		{
			pPerAdapterInfo = (IP_PER_ADAPTER_INFO *) malloc(nSize);
			if ( (dwErr = GetPerAdapterInfo(pAdapter->Index, pPerAdapterInfo, &nSize)) == ERROR_SUCCESS)
			{
				if (pPerAdapterInfo == NULL) {
					TRACE(_T("Error allocating memory needed to call GetAdaptersinfo\n"));
					return ;
				}
				m_NetCard.DNSServer.Format(_T("%s"), A2W(pPerAdapterInfo->DnsServerList.IpAddress.String));
				if(m_NetCard.DNSServer == _T(""))
					m_NetCard.DNSServer = _T("0.0.0.0");
				//GetDlgItem(IDC_IPADDRESS_DNS)->SetWindowText(A2W(pPerAdapterInfo->DnsServerList.IpAddress.String));
			}
		}
		if (pPerAdapterInfo)
		{
			free( pPerAdapterInfo );
		}

		//--Check NetCard Description Blank or not
		if(m_NetCard.description.GetLength() != 0)
		{
			veNetCard.push_back(m_NetCard);
			iNetCardNum++;
		}

		pinfo = pinfo->Next;
		pAdapter = pAdapter->Next;
	}
}

CString CxNetCardInfo::GetNetCardType(int iNetCard)
{
	return veNetCard[iNetCard-1].type;
}

CString CxNetCardInfo::GetNetCardIPAddress(int iNetCard)
{
	return veNetCard[iNetCard-1].IpAddress;
}

CString CxNetCardInfo::GetNetCardSubnetMask(int iNetCard)
{
	return veNetCard[iNetCard-1].subnet;
}

CString CxNetCardInfo::GetNetCardGateWay(int iNetCard)
{
	return veNetCard[iNetCard-1].gateway;
}

CString CxNetCardInfo::GetDHCPServer(int iNetCard)
{
	return veNetCard[iNetCard-1].dhcp;
}

CString CxNetCardInfo::GetNetCardMACAddress(int iNetCard)
{
	return veNetCard[iNetCard-1].macaddress;
}

CString CxNetCardInfo::GetNetCardDeviceName(int iNetCard)
{
	return veNetCard[iNetCard-1].description;
}

CString CxNetCardInfo::GetNetCardWINS(int iNetCard)
{
	return veNetCard[iNetCard-1].PrimaryWinsServer;
}

CString CxNetCardInfo::GetErrorMsg(int iNetCard)
{
	return veNetCard[iNetCard-1].ErrMsg;
}

int CxNetCardInfo::GetNetCardNum()
{
	return iNetCardNum;
}

CString CxNetCardInfo::GetNetCardDNS(int iNetCard)
{
	return veNetCard[iNetCard-1].DNSServer;
}