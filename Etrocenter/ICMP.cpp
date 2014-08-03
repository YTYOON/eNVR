// Ping.cpp: implementation of the CICMP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ICMP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CICMP::CICMP()
{
	icmp_data = NULL;
	seq_no = 0;
	recvbuf = NULL;
	m_bRecordRout = FALSE;
	lpdest = NULL;
	datasize = DEF_PACKET_SIZE;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		AfxMessageBox(IDS_SOCKET_LOAD_ERR);
		//AfxMessageBox(_T("Sorry, you cannot load socket dll!"));
		return ;
	}
	m_hSocket = INVALID_SOCKET;

}

CICMP::~CICMP()
{
	if(lpdest)
		delete []lpdest;
}

  

bool CICMP::bPing(int timeout)
{   
	bool bRet = false;
	m_hSocket = WSASocket (AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0,
                           WSA_FLAG_OVERLAPPED);

    if (m_hSocket == INVALID_SOCKET) 
    {
		int nRet = GetLastError();
		CString ls_txt, ls_msg;
		ls_txt.LoadString(CProductDefine::g_hResStr, IDS_SOCKET_CREATE_ERR);
		ls_msg.Format(_T("%s%d"), ls_txt, nRet);
		AfxMessageBox(ls_msg);
		//AfxMessageBox(_T("socket 建立失敗!\r\n Error Code:%d\r\n"), nRet);
        return bRet;
    }

    if (m_bRecordRout)
    {
        // Setup the IP option header to go out on every ICMP packet
        //
        ZeroMemory(&m_ipopt, sizeof(m_ipopt));
        m_ipopt.code = IP_RECORD_ROUTE; // Record route option
        m_ipopt.ptr  = 4;               // Point to the first addr offset
        m_ipopt.len  = 39;              // Length of option header
  
        int ret = setsockopt(m_hSocket, IPPROTO_IP, IP_OPTIONS, 
            (char *)&m_ipopt, sizeof(m_ipopt));
        if (ret == SOCKET_ERROR)
        {
            TRACE(_T("設定socket協議選項錯誤!"));
        }
    }

    // Set the send/recv timeout values
    //
    int bread = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, 
                (char*)&timeout, sizeof(timeout));
    if(bread == SOCKET_ERROR) 
    {
        TRACE(_T("設定socket接收超時選項錯誤!"));
        return false;
    }
    timeout = 10000;
    bread = setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO, 
                (char*)&timeout, sizeof(timeout));
    if (bread == SOCKET_ERROR) 
    {
         TRACE(_T("設定socket發送超時選項錯誤!"));
        return bRet;
    }
    memset(&m_addrDest, 0, sizeof(m_addrDest));
    //
    // Resolve the endpoint's name if necessary
    //
    m_addrDest.sin_family = AF_INET;
    if ((m_addrDest.sin_addr.s_addr = inet_addr(lpdest)) == INADDR_NONE)
    {   
		 struct hostent *hp = NULL;

        if ((hp = gethostbyname(lpdest)) != NULL)
        {
            memcpy(&(m_addrDest.sin_addr), hp->h_addr, hp->h_length);
            m_addrDest.sin_family = hp->h_addrtype;
//            printf("m_addrDest.sin_addr = %s\n", inet_ntoa(m_addrDest.sin_addr));
        }
        else
        {
//            printf("gethostbyname() failed: %d\n", 
 //               WSAGetLastError());
			TRACE(_T("輸入的主機不存在!"));
            return bRet;
        }
    }        

    // 
    // Create the ICMP packet
    //       
    datasize += sizeof(IcmpHeader);  

    icmp_data =(char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                  MAX_PACKET);
    recvbuf =(char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                  MAX_PACKET);
    if (!icmp_data) 
    {
        TRACE(_T("堆分發錯誤!"));
        return false;
    }
    memset(icmp_data,0,MAX_PACKET);
    FillICMPData(icmp_data,datasize);
    //
    // Start sending/receiving ICMP packets
    //
	//static int nCount = 0;
	int nCount=0;
    while(1) 
    {
        int        bwrote;
                
        if (nCount++ == 4) 
            break;
                
        ((IcmpHeader*)icmp_data)->i_cksum = 0;
        ((IcmpHeader*)icmp_data)->timestamp = GetTickCount();
        ((IcmpHeader*)icmp_data)->i_seq = seq_no++;
        ((IcmpHeader*)icmp_data)->i_cksum = 
            checksum((USHORT*)icmp_data, datasize);

        bwrote = sendto(m_hSocket, icmp_data, datasize, 0, 
                     (struct sockaddr*)&m_addrDest, sizeof(m_addrDest));
        if (bwrote == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) 
            {
				TRACE(_T("Timed out ! \r\n"));
                break;
            }
			TRACE(_T("發送數據函數調用錯誤!"));
            break ;
        }
        if (bwrote < datasize) 
        {
			TRACE(_T("Wrote %d bytes \r\n"), bwrote);
        }

		int fromlen = sizeof(m_addrFrom);
        bread = recvfrom(m_hSocket, recvbuf, MAX_PACKET, 0, 
                    (struct sockaddr*)&m_addrFrom, &fromlen);
        if (memcmp(&m_addrFrom.sin_addr, &m_addrDest.sin_addr, sizeof(m_addrDest.sin_addr)))
            continue;
        if (bread == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) 
            {
				TRACE(_T("Timed out !\r\n"));
                continue;
            }
			TRACE(_T("接收數據函數調用錯誤!"));
            break ;
        }
        if (bDecodeICMPHeader(recvbuf, bread, &m_addrFrom)) 
		{
			bRet = true;
			break;
		}
        
    }
    USES_CONVERSION;
    TRACE(_T("ping %s %d\r\n"), A2W(lpdest), bRet);
	return bRet;
}

void CICMP::Cleanup()
{
  if (m_hSocket != INVALID_SOCKET) 
        closesocket(m_hSocket);
    HeapFree(GetProcessHeap(), 0, recvbuf);
    HeapFree(GetProcessHeap(), 0, icmp_data);

//    WSACleanup();
    return ;
}

void CICMP::FillICMPData(char *icmp_data, int datasize)
{
   IcmpHeader *icmp_hdr = NULL;
    char       *datapart = NULL;

    icmp_hdr = (IcmpHeader*)icmp_data;
    icmp_hdr->i_type = ICMP_ECHO;        // Request an ICMP echo
    icmp_hdr->i_code = 0;
    icmp_hdr->i_id = (USHORT)GetCurrentProcessId();
    icmp_hdr->i_cksum = 0;
    icmp_hdr->i_seq = 0;
  
    datapart = icmp_data + sizeof(IcmpHeader);
}

void CICMP::DecodeIPOptions(char *buf, int bytes)
{
     IpOptionHeader *ipopt = NULL;
    IN_ADDR         inaddr;
    int             i;
    HOSTENT        *host = NULL;

    ipopt = (IpOptionHeader *)(buf + 20);

	TRACE(_T("Ping 結果:   \r\n"));
    for(i = 0; i < (ipopt->ptr / 4) - 1; i++)
    {
        inaddr.S_un.S_addr = ipopt->addr[i];
        if (i != 0)
		{
			TRACE(_T("\r\n"));
		}
        host = gethostbyaddr((char *)&inaddr.S_un.S_addr,
                    sizeof(inaddr.S_un.S_addr), AF_INET);

		CString temp;
        if (host)
		{
			TRACE(_T("(%-15s) %s \r\n"), inet_ntoa(inaddr), host->h_name);
		}
        else
		{
			TRACE(_T("(%-15s)\r\n"), inet_ntoa(inaddr));
		}
    }
    return;
}

USHORT CICMP::checksum(USHORT *buffer, int size)
{
   unsigned long cksum=0;

    while (size > 1) 
    {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if (size) 
    {
        cksum += *(UCHAR*)buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >>16);
    return (USHORT)(~cksum);
}

void CICMP::SetConfigure(char * host,BOOL recordrout,int size)
{
	if(lpdest)
	{
		delete[] lpdest;
		lpdest=NULL;
	}
    m_bRecordRout = recordrout;
    datasize = size;
    lpdest = new char [strlen(host)+1];
	strcpy(lpdest,host);
}

BOOL CICMP::bDecodeICMPHeader(char *buf, int bytes, SOCKADDR_IN *from)
{
    IpHeader       *iphdr = NULL;
    IcmpHeader     *icmphdr = NULL;
    unsigned short  iphdrlen;
    DWORD           tick;
    int    icmpcount = 0;

    iphdr = (IpHeader *)buf;
	// Number of 32-bit words * 4 = bytes
    iphdrlen = iphdr->h_len * 4;
    tick = GetTickCount();

    if ((iphdrlen == MAX_IP_HDR_SIZE) && (!icmpcount))
        DecodeIPOptions(buf, bytes);

	CString temp;
    if (bytes  < iphdrlen + ICMP_MIN) 
    {
		TRACE(_T("Too few bytes from %s \r\n"),inet_ntoa(from->sin_addr));
    }
    icmphdr = (IcmpHeader*)(buf + iphdrlen);

    if (icmphdr->i_type != ICMP_ECHOREPLY) 
    {
		TRACE(_T("nonecho type %d recvd \r\n"), icmphdr->i_type);
        return false;
    }
    // Make sure this is an ICMP reply to something we sent!
    //
    if (icmphdr->i_id != (USHORT)GetCurrentProcessId()) 
    {
		TRACE(_T("someone else's packet! \r\n"));
        return true;
    }

	TRACE(_T("%d bytes from %s: \r\n"), bytes, inet_ntoa(from->sin_addr));
	TRACE(_T(" icmp_seq = %d. \r\n"), icmphdr->i_seq);
	TRACE(_T(" time: %d ms \r\n"), tick - icmphdr->timestamp);
    icmpcount++;
    return true;
}

