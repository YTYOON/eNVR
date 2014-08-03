// CGIVar.cpp: implementation of the CGIVar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NVR.h"
#include "CGIVar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define INITMAP(VAR,SECTION,KEY,CAPTION) {aMap[ci].csSection = _T(SECTION); aMap[ci].csKeyName = _T(KEY); aMap[ci].csCaption = _T(CAPTION);}
#define INITVAR(MAP, SECTION, VAR, KEY) VARSET VAR;{VAR.pMap = &MAP;MAP.csSection = _T(SECTION); MAP.csKeyName = _T(KEY); MAP.csCaption = _T(KEY);}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGISession::CGISession( LPCTSTR pstrAgent, DWORD dwContext,
						DWORD dwAccessType , LPCTSTR pstrProxyName,
						LPCTSTR pstrProxyBypass, DWORD dwFlags)
{
	EnableStatusCallback(TRUE);
}

void CGISession::OnStatusCallback(DWORD dwContext, DWORD dwInternalStatus,
		LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int errors[] = {10, 11, 20, 21, 30, 31, 40, 41, 42, 43, 50, 51, 60, 70, 100, 110, 0};
	TCHAR* text[] = { 
		_T("Resolving name"),       	
	     _T("Name resolved"),            
		_T("Connecting to server"),     
		_T("Connected to server"),      
		_T("Sending request"),          
		_T("Request sent"),             
		_T("Receiving response"),       
		_T("Response received"),        
		_T("Ctl response received"),    
		_T("Prefetch"),                 
		_T("Closing connection"),       
		_T("Connection closed"),        
		_T("Handle created"),           
		_T("Handle closing"),           
		_T("Request complete"),         
		_T("Redirect"),                 
		_T("Unknown") };                

	int n;
/*  // demonstrates request cancellation 
	if(dwInternalStatus == INTERNET_STATUS_REQUEST_SENT) {
		AfxThrowInternetException(dwContext, 999);
	}
*/
	for(n = 0; errors[n] != 0; n++) {
		if(errors[n] == (int) dwInternalStatus) break;
	}

//	g_csStatus.Lock();
	CString csError;
	csError =text[n];
	  if(dwInternalStatus  == INTERNET_STATUS_RESOLVING_NAME || 
				dwInternalStatus == INTERNET_STATUS_NAME_RESOLVED) {
		  csError += _T("-");
		  csError += (TCHAR*)lpvStatusInformation;
	  }
	  TRACE(_T("WININET STATUS: %s\n"), csError.LockBuffer());
//	g_csStatus.Unlock();
	// frame doesn't need a handler -- message triggers OnIdle, which updates status bar
//	::PostMessage(g_hMainWnd, WM_CALLBACK, 0, 0);

}

void  CGISession::LogInternetException(LPVOID pParam, CInternetException* pe)
{	// pParam holds the HWND for the destination window (in another thread)

	CString strGmt = CTime::GetCurrentTime().FormatGmt("%m/%d/%y %H:%M:% GMT");
	TCHAR text1[300], text2[100];
	wsprintf(text1, _T("CLIENT ERROR: WinInet error #%d -- %s\r\n   "),
		pe->m_dwError, (const TCHAR*) strGmt);
	pe->GetErrorMessage(text2, 99);
	wcscat(text1, text2);
	if(pe->m_dwError == 12152) {
		wcscat(text1, _T("  URL not found?\r\n"));
	}

//	::SendMessage((HWND) pParam, EM_SETSEL, (WPARAM) 65534, 65535);
//	::SendMessage((HWND) pParam, EM_REPLACESEL, (WPARAM) 0, (LPARAM) text1);
}
CGIVar::CGIVar():
m_bDirty(false),m_bStaticIP(false),hSectionClosed(NULL)
{

	//vector <VERSETS> ;
	varmap[_T("common/info.cgi")];	            
	varmap[_T("config/action_ftp.cgi")];        
	varmap[_T("config/action_mail.cgi")];       
	varmap[_T("config/audio.cgi?profileid=1")]; 
	varmap[_T("config/audio.cgi?profileid=2")]; 
//	varmap[_T("config/auto_pan.cgi")];          
	varmap[_T("config/camera_info.cgi")];       
	varmap[_T("config/datetime.cgi")];          
	varmap[_T("config/ddns.cgi")];              
	varmap[_T("config/ddnsproviders.cgi")];     
	varmap[_T("config/event.cgi")];             
//	varmap[_T("config/event_del.cgi")];         
	varmap[_T("config/event_info.cgi")];        
	varmap[_T("config/event_input.cgi")];       
	varmap[_T("config/event_vloss.cgi")];       
//	varmap[_T("config/firmwareupgrade.cgi")];   
//	varmap[_T("config/focus_cont.cgi")];        
	varmap[_T("config/group_list.cgi")];        
	varmap[_T("config/httpport.cgi")];          
	varmap[_T("config/info_ex.cgi")];           
	varmap[_T("config/io.cgi")];                
	varmap[_T("config/led.cgi")];               
	varmap[_T("config/mic.cgi")];               
	varmap[_T("config/motion.cgi")];            
	varmap[_T("config/network.cgi")];           
	varmap[_T("config/pppoe.cgi")];             
	varmap[_T("config/privilege_info.cgi")];    
//	varmap[_T("config/ptz_move_cont.cgi")];     
//	varmap[_T("config/ptz_move_rel.cgi")];      
//	varmap[_T("config/ptz_pan_auto.cgi")];      
//	varmap[_T("config/ptz_patrol.cgi")];        
//	varmap[_T("config/ptz_patrol_set.cgi")];    
//	varmap[_T("config/ptz_preset.cgi")];        
	varmap[_T("config/record_prepost.cgi")];    
	varmap[_T("config/rs485.cgi")];             
//	varmap[_T("config/rs485_do.cgi")];          
	varmap[_T("config/rs485_info.cgi")];        
	varmap[_T("config/rs485_proto.cgi")];       
	varmap[_T("config/sdcard.cgi")];            
//	varmap[_T("config/sdcard_del.cgi")];        
//	varmap[_T("config/sdcard_download.cgi")];   
	varmap[_T("config/sdcard_format.cgi")];     
//	varmap[_T("config/sdcard_list.cgi")];       
	varmap[_T("config/security.cgi")];          
	varmap[_T("config/security_level.cgi")];    
	varmap[_T("config/security_rule.cgi")];     
//	varmap[_T("config/security_rule_add.cgi")]; 
//	varmap[_T("config/security_rule_del.cgi")]; 
//	varmap[_T("config/security_rule_mod.cgi")]; 
//	varmap[_T("config/security_rule_move.cgi")];
	varmap[_T("config/sensor.cgi")];            
	varmap[_T("config/sensor_info.cgi")];       
//	varmap[_T("config/sensor_reset.cgi")];      
	varmap[_T("config/speaker.cgi")];  
	varmap[_T("config/stream_auth.cgi")];
	varmap[_T("config/stream_info.cgi")];       
//	varmap[_T("config/system_reboot.cgi")];     
//	varmap[_T("config/system_reset.cgi")];      
	varmap[_T("config/upnp.cgi")];              
//	varmap[_T("config/user_del.cgi")];          
	varmap[_T("config/user_list.cgi")];         
//	varmap[_T("config/user_mod.cgi")];          
	varmap[_T("config/video.cgi?profileid=1")]; 
	varmap[_T("config/video.cgi?profileid=2")]; 
	varmap[_T("users/verify.cgi")];             

	CGS gs;
	csTarget = gs.csGetCacheFolder();
	hDestory = ::CreateEvent(NULL, TRUE, FALSE, _T(""));
}

CGIVar::~CGIVar()
{
	if (hSectionClosed != NULL)
	{
	//	debug here
		WORD nRet = ::SignalObjectAndWait(hDestory, hSectionClosed, 1000, false);
		switch (nRet)
		{
			case WAIT_IO_COMPLETION:
				TRACE(_T("WAIT_IO_COMPLETION\r\n"));
				break;
			case WAIT_OBJECT_0:
				TRACE(_T("WAIT_OBJECT_0\r\n"));
				break;
			case STATUS_TIMEOUT:
				::TerminateThread(hThreadRobort, 0);
				TRACE(_T("STATUS_TIMEOUT\r\n"));
				break;
			default:
				::TerminateThread(hThreadRobort, 0);
				TRACE(_T("DEFAULT\r\n"));
				break;
		}
		::CloseHandle(hSectionClosed);
		::CloseHandle(hThreadRobort);
		hSectionClosed = NULL;
		hThreadRobort = NULL;

	}
	::CloseHandle(hDestory);
}
struct TreeThreadParam {
	CGIVar* pThis;
	NODEITEM* pNode;
};

UINT _stdcall CGIVar::CreateCgiRobor(LPVOID lpParam)
{
	//return 0;
	TreeThreadParam* pLoginInfo = ((TreeThreadParam*) (lpParam));	
	CGIVar* pThis = pLoginInfo->pThis;
	NODEITEM* pNode = pLoginInfo->pNode;
/*
	{//for debug temp
		::SetEvent(pThis->hSectionClosed);
		delete lpParam;
		lpParam = NULL;
		return 0;
	}
*/
	CGISession session; // can't get status callbacks for OpenURL
//	pLoginInfo->pThis->hSectionClosed = ::CreateEvent(NULL, TRUE, FALSE, _T(""));
	CStdioFile* pFile1 = NULL; // could call ReadString to get 1 line
	USES_CONVERSION;
	static int nTotalRange = 0;
	static int nPos = 0;
	static int nTotalEntry = 0;
	WORD nEvent(0);
	TCHAR tzIP[32];
	TCHAR tzUser[32];
	TCHAR tzPass[32];
	int ci=0;
//	if (strcmp(pNode->ip, "192.168.1.2") !=0)
//		return 0;
	wcscpy(tzIP,   A2W(pNode->ip));
	wcscpy(tzUser, A2W(pNode->szUser));
	wcscpy(tzPass, A2W(pNode->szPass));
	const int MAXBUF = 4096;
	CGS gs;
	char* buffer = new char[MAXBUF];
	memset(buffer, 0, MAXBUF);
	UINT nBytesRead = 0;
	CString csUrl;

	bool bExist = false;
	nTotalEntry++;
	nTotalRange = nTotalRange + 41;
	TRACE(_T("TOTAL RANGE:%d\r\n"),nTotalRange);
	ci = -1;
	CString csFile;
	CFile fileBuffer;
	map <CString, VERSETS>::iterator pos;
	TRACE(tzIP);
	
	CString csTmp;
	csTmp = pLoginInfo->pThis->csTarget; //pLoginInfo->pThis->csGetAppCacheFolder();
	if ( 0 == csTmp.GetLength() )
	{
		goto exit;
	}
	csFile.Format(_T("%s\\%02d-%02d-%02d-%02d-%02d-%02d.cfg"), csTmp, pLoginInfo->pNode->mac[0], pLoginInfo->pNode->mac[1], pLoginInfo->pNode->mac[2], pLoginInfo->pNode->mac[3], pLoginInfo->pNode->mac[4], pLoginInfo->pNode->mac[5]);
	TRY
	{
		TRACE(csFile);
		if (fileBuffer.Open( csFile, CFile::modeReadWrite | CFile::modeCreate) == false)
			goto exit;
	}
	CATCH( CFileException, e )
	{
	   #ifdef _DEBUG
		  afxDump << "File could not be opened " << e->m_cause << "\n";
	   #endif
		ETROMSG(_T("Create Cache File Fail"));
		goto exit;
	}
	END_CATCH
	for (pos = pThis->varmap.begin(); pos != pThis->varmap.end(); ++pos)
//	while (cgiinfo[++ci].csTitle.GetLength()!=0)
	{
		char szTmp[255];
		sprintf(szTmp, "[%s]\r\n", W2A(pos->first));
		fileBuffer.Write(szTmp, strlen(szTmp));

		nEvent = ::WaitForSingleObject(pThis->hDestory, 0);
		if (nEvent != WAIT_TIMEOUT)
			break;
		nPos ++;
		csUrl.Format( _T("http://%s:%s@%s:%d/%s"), tzUser, tzPass, tzIP, pNode->nWebPort, pos->first);
		TRACE(_T("%s\r\n"),csUrl);
		try {
			pFile1 = session.OpenURL(csUrl, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_KEEP_CONNECTION);
		}
		catch(CInternetException* e) {
		//	e->ReportError();
		//	AfxMessageBox(csUrl);
			int nError = e->m_dwError;
			e->Delete();
			if (nError == 12031)
			{
				goto exit;
			}
			continue;
		}
		nBytesRead = pFile1->Read(buffer, MAXBUF - 1);

		if (strncmp(buffer, "Error: 401", 10) == 0)
			break;
	

		TRACE(A2W(buffer));
		buffer[nBytesRead++] = '\r'; // necessary for message box
		buffer[nBytesRead++] = '\n'; // necessary for message box
		buffer[nBytesRead++] = '\r'; // necessary for message box
		buffer[nBytesRead++] = '\n'; // necessary for message box

		fileBuffer.Write(buffer, nBytesRead);
		buffer[nBytesRead] = '\0'; // necessary for message box

		TCHAR tzBuffer[MAXBUF];
		MultiByteToWideChar(CP_ACP, 0, buffer, -1, tzBuffer, MAXBUF);
		pThis->vParseSegment(tzBuffer, pos);
		TCHAR* tzSeg = wcstok(tzBuffer, _T("\r\n"));
		while (tzSeg)
		{
			nEvent = ::WaitForSingleObject(pThis->hDestory, 0);
			if (nEvent != WAIT_TIMEOUT)
			{
				if (nEvent == WAIT_OBJECT_0)
				{
					ETROLOG(_T("WAIT_OBJECT_0\t\n"));
				}else
				{
					SDKERRMSG(nEvent);
				}
				break;
			}
			TCHAR* tz = wcschr(tzSeg, '=');
			if (tz)
				*tz = ':';
			tzSeg = wcstok(NULL, _T("\r\n"));
		}	

		if (pFile1) 
		{
			pFile1->SetFilePath(csFile);
			pFile1->Close();
			delete pFile1;
			pFile1 = NULL;
		}		

		if (nEvent != WAIT_TIMEOUT)
			break;
	}

exit:
	if (pFile1) 
	{
		pFile1->SetFilePath(csFile);
		pFile1->Close();
		delete pFile1;
		pFile1 = NULL;
		TRACE(_T("%s\r\n"), csFile);
	}
	if ((HANDLE)fileBuffer.m_hFile !=  INVALID_HANDLE_VALUE)
	{
		fileBuffer.Close();
	}
	delete [] buffer;	
	delete lpParam;
	lpParam = NULL;
//	vDataExchange(csFile);
	::SetEvent(pThis->hSectionClosed);
	return 0;
}
void CGIVar::vAttach(NODEITEM* pNode)
{
 	hSectionClosed = ::CreateEvent(NULL, TRUE, FALSE, _T(""));
	TreeThreadParam* pParam = new TreeThreadParam;
 	pParam->pThis = this;
 	pParam->pNode = pNode;	
	UINT nRet(0);
	hThreadRobort = (HANDLE)_beginthreadex(NULL, 0, CreateCgiRobor, pParam, 0, &nRet);
}


CString CGIVar::csMakeTempFilename()
{
    CString strFilename;
	TCHAR szSuffix[] =_T("TMP");

    // find the path
    TCHAR szPath[MAX_PATH];
    if(::GetTempPath(MAX_PATH, szPath)>0)
    {
        // ask for a new file in the path
        TCHAR szFilename[MAX_PATH];
        if(::GetTempFileName(
            szPath,         // path
            CProductDefine::m_tzPrefix,          // prefix string
            0,              // let the system pick a number
            szFilename)>0)  // buffer
        {
            strFilename.Format(_T("%s.%s"), szFilename, szSuffix);
			::_wrename(szFilename, strFilename.LockBuffer());
        }
    }
    return(strFilename);
}

void CGIVar::vParseSegment(TCHAR* tszBuffer, map <CString, VERSETS>::iterator pos)
{
	TCHAR* tszLine;
	tszLine = wcstok(tszBuffer, _T("\r\n"));
	while (tszLine)
	{
		TCHAR* tszName = tszLine;
		TCHAR* tsz = wcschr(tszLine, '=');		
		if (tsz)
		{
			*tsz = '\0';
			tsz++;
			pos->second.aSet[tszName] = SETS(tsz, tszName);
		}
		tszLine = wcstok(NULL, _T("\r\n"));
	}		
	return;
}
#define ADDPROPSTRING(LIST, VAR, CGI, NAME, TF) LIST.AddPropString( this , VAR[CGI].aSet[NAME].csCaption  , &VAR[CGI].aSet[NAME].csKey , TF);
//void  CGIVar::GetProperties( EPropList& PropList )
//{
//	int ci=0;
//	EPropertyCombo* pcombo = NULL;
//	PropList.AddTab(_T("Overview"));
//	AddPropString(PropList, _T("common/info.cgi"), _T("model"), false);
//	AddPropString(PropList, _T("common/info.cgi"), _T("version"), false);
//	AddPropString(PropList, _T("common/info.cgi"), _T("build"), false);
//	AddPropString(PropList, _T("common/info.cgi"), _T("name"), false);
//	AddPropString(PropList, _T("common/info.cgi"), _T("location"), false);
//	AddPropString(PropList, _T("common/info.cgi"), _T("ipaddr"), false);
//	AddPropString(PropList, _T("common/info.cgi"), _T("netmask"), false);
//	AddPropString(PropList, _T("common/info.cgi"), _T("gateway"), false);
//	AddPropString(PropList, _T("common/info.cgi"), _T("ptz"), false);
//	AddPropString(PropList, _T("common/info.cgi"), _T("inputs"), false);
//	AddPropString(PropList, _T("common/info.cgi"), _T("output"), false);
//	AddPropString(PropList, _T("common/info.cgi"), _T("wireless"), false);
//	AddPropString(PropList, _T("common/info.cgi"), _T("speaker"), false);
//
//	PropList.AddTab(_T("NetWork"));
//
//	AddPropCheck(PropList, _T("config/network.cgi"), _T("dhcp"));
//
//	AddPropCheck(PropList, _T("config/pppoe.cgi"), _T("pppoe"));
//	PropList.StepIndentIn();	
//	AddPropString(PropList, _T("config/pppoe.cgi"), _T("user"));
//	AddPropString(PropList, _T("config/pppoe.cgi"), _T("pass"));
//	PropList.StepIndentOut();
//
//	m_bStaticIP = !(varmap["config/pppoe.cgi"].aSet["pppoe"].bCheck || varmap["config/network.cgi"].aSet["dhcp"].bCheck);
//	PropList.AddPropCheck ( this , "Static IP", &m_bStaticIP );
//	varmap["config/network.cgi"].aSet["staticip"] = SETS(_T("off"),_T("staticip"));
//	PropList.StepIndentIn();
//	AddPropString(PropList, _T("config/network.cgi"), _T("ip"));
//	AddPropString(PropList, _T("config/network.cgi"), _T("netmask"));
//	AddPropString(PropList, _T("config/network.cgi"), _T("gateway"));
//	AddPropString(PropList, _T("config/network.cgi"), _T("dns1"));
//	PropList.StepIndentOut();
//
//
//	AddPropCheck(PropList, _T("config/ddns.cgi"), _T("ddns"));
//	PropList.StepIndentIn();	
//	AddPropCombo(PropList, _T("config/ddns.cgi"), _T("provider"), _T("common/ddnsproviders.cgi"), _T("providers"));
//	AddPropString(PropList, _T("config/ddns.cgi"), _T("host"));
//	AddPropString(PropList, _T("config/ddns.cgi"), _T("user"));
//	AddPropString(PropList, _T("config/ddns.cgi"), _T("pass"));
//	PropList.StepIndentOut();
//
//	AddPropCheck(PropList, _T("config/network.cgi"), _T("upnp"));
//	AddPropInt(PropList, _T("config/network.cgi"), _T("httpport"));
//
//	PropList.AddTab(_T("Video"));
//	PropList.AddPropSeparator( this , "Stream 1" );
//	PropList.StepIndentIn();	
//	AddPropInt(PropList, _T("config/video.cgi?profileid=1"), _T("port"));
//	AddPropCombo(PropList,  _T("config/video.cgi?profileid=1"), _T("codec"), _T("config/stream_info.cgi"), _T("videos"));
//	AddPropCombo(PropList,  _T("config/video.cgi?profileid=1"), _T("format"), _T("VSM,RTP"));
//	AddPropCombo(PropList,  _T("config/video.cgi?profileid=1"), _T("resolution"), _T("config/stream_info.cgi"), _T("resolutions"));
//	AddPropCombo(PropList,  _T("config/video.cgi?profileid=1"), _T("goplength"), _T("config/stream_info.cgi"), _T("goplengths"));
//	AddPropCombo(PropList,  _T("config/video.cgi?profileid=1"), _T("framerate"), _T("config/stream_info.cgi"), _T("framerates"));
//	AddPropString(PropList, _T("config/audio.cgi?profileid=1"), _T("codec"),false);
//	AddPropString(PropList, _T("config/audio.cgi?profileid=1"), _T("samplerate"),false);
//	AddPropString(PropList, _T("config/audio.cgi?profileid=1"), _T("channel"),false);
//	AddPropString(PropList, _T("config/audio.cgi?profileid=1"), _T("bitrate"),false);
//	PropList.StepIndentOut();	
//	PropList.AddPropSeparator( this , "Stream 2" );
//	PropList.StepIndentIn();	
//	AddPropCheck ( PropList , _T("config/video.cgi?profileid=2"), _T("enable"));
//	AddPropInt(PropList, _T("config/video.cgi?profileid=2"), _T("port"));
//	AddPropCombo(PropList,  _T("config/video.cgi?profileid=2"), _T("codec"), _T("config/stream_info.cgi"), _T("videos"));
//	AddPropCombo(PropList,  _T("config/video.cgi?profileid=2"), _T("format"), _T("VSM,RTP"));
//	AddPropCombo(PropList,  _T("config/video.cgi?profileid=2"), _T("resolution"), _T("config/stream_info.cgi"), _T("resolutions"));
//	AddPropCombo(PropList,  _T("config/video.cgi?profileid=2"), _T("goplength"), _T("config/stream_info.cgi"), _T("goplengths"));
//	AddPropCombo(PropList,  _T("config/video.cgi?profileid=2"), _T("framerate"), _T("config/stream_info.cgi"), _T("framerates"));
//	AddPropString(PropList, _T("config/audio.cgi?profileid=2"), _T("codec"),false);
//	AddPropString(PropList, _T("config/audio.cgi?profileid=2"), _T("samplerate"),false);
//	AddPropString(PropList, _T("config/audio.cgi?profileid=2"), _T("channel"),false);
//	AddPropString(PropList, _T("config/audio.cgi?profileid=2"), _T("bitrate"),false);
//	PropList.StepIndentOut();
//	PropList.AddPropSeparator( this , "Image Setting" );
//	PropList.StepIndentIn();	
//	AddPropInt(PropList, _T("config/sensor.cgi"), _T("brightness"));
//	AddPropInt(PropList, _T("config/sensor.cgi"), _T("contrast"));
//	AddPropInt(PropList, _T("config/sensor.cgi"), _T("saturation"));
//	AddPropInt(PropList, _T("config/sensor.cgi"), _T("whitebalance"));
//	AddPropInt(PropList, _T("config/sensor.cgi"), _T("autoexposure"));
//	AddPropInt(PropList, _T("config/sensor.cgi"), _T("autogaincontrol"));
//	AddPropInt(PropList, _T("config/sensor.cgi"), _T("mirror"));
//	AddPropInt(PropList, _T("config/sensor.cgi"), _T("flip"));
//	AddPropInt(PropList, _T("config/sensor.cgi"), _T("color"));
//	PropList.StepIndentOut();
//
//
//	PropList.AddTab("Date/Time");
//	AddPropString(PropList, _T("config/datetime.cgi"), _T("timeserver"));
//	AddPropCombo(PropList,  _T("config/datetime.cgi"), _T("method"), _T("Adjust with Current PC Time,Adjust With Time Server"));
//	AddPropString(PropList, _T("config/datetime.cgi"), _T("date"),false);
//	AddPropString(PropList, _T("config/datetime.cgi"), _T("time"),false);
//
//	PropList.AddTab("OSD");
//
//	PropList.AddTab("PTZ Control");
//
//	AddPropCheck(PropList , _T("config/rs485.cgi"), _T("enable"));
//	AddPropCombo(PropList,  _T("config/rs485.cgi"), _T("proto"), _T("config/rs485_info.cgi"), _T("proto"));
//	AddPropCombo(PropList,  _T("config/rs485.cgi"), _T("baudrate"), _T("config/rs485_info.cgi"), _T("baudrate"));
//	AddPropCombo(PropList,  _T("config/rs485.cgi"), _T("databits"), _T("config/rs485_info.cgi"), _T("databits"));
//	AddPropCombo(PropList,  _T("config/rs485.cgi"), _T("parity"), _T("config/rs485_info.cgi"), _T("parity"));
//	AddPropCombo(PropList,  _T("config/rs485.cgi"), _T("stopbits"), _T("config/rs485_info.cgi"), _T("stpobits"));
//
//	PropList.AddTab("SMTP/FTP");
//	PropList.AddPropSeparator( this , "Remote Ftp Setup" );
//	PropList.StepIndentIn();
//	AddPropString(PropList, _T("config/action_ftp.cgi"), _T("host"));
//	AddPropInt(PropList, _T("config/action_ftp.cgi"), _T("port"));	
//	AddPropString(PropList, _T("config/action_ftp.cgi"), _T("user"));
//	AddPropString(PropList, _T("config/action_ftp.cgi"), _T("pass"));
//	AddPropString(PropList, _T("config/action_ftp.cgi"), _T("path"));
//	AddPropString(PropList, _T("config/action_ftp.cgi"), _T("prefix"));
//	AddPropCheck(PropList, _T("config/action_ftp.cgi"), _T("passive"),_T(""),_T("yes"));
//	PropList.StepIndentOut();
//	
//	PropList.AddPropSeparator( this , "Remote MailServer Setup" );
//	PropList.StepIndentIn();
//	AddPropString(PropList, _T("config/action_mail.cgi"), _T("sender"));
//	AddPropString(PropList, _T("config/action_mail.cgi"), _T("to"));
//	AddPropString(PropList, _T("config/action_mail.cgi"), _T("host"));
//	AddPropInt(PropList, _T("config/action_mail.cgi"), _T("port"));	
//	AddPropString(PropList, _T("config/action_mail.cgi"), _T("user"));
//	AddPropString(PropList, _T("config/action_mail.cgi"), _T("pass"));
//	AddPropCheck(PropList, _T("config/action_mail.cgi"), _T("auth"),_T(""),_T("yes"));
//	AddPropCheck(PropList, _T("config/action_mail.cgi"), _T("attach"),_T(""),_T("yes"));
//	PropList.StepIndentOut();
//
//	PropList.AddTab("Account");
//
//	PropList.AddTab("Misc.");
//	PropList.AddPropSeparator( this , "Led" );
//	PropList.StepIndentIn();
//	AddPropCheck(PropList, _T("config/led.cgi"), _T("led"));
//	PropList.StepIndentOut();
//
//}
//
//void CGIVar::AddPropString(EPropList& PropList, TCHAR* tszCgi, TCHAR* tszName, bool bWrite)
//{
//	if (varmap[tszCgi].aSet[tszName].csCaption == _T("")) return;
//	PropList.AddPropString(this, varmap[tszCgi].aSet[tszName].csCaption , &varmap[tszCgi].aSet[tszName].csKey, bWrite);
//}
//void CGIVar::AddPropCheck(EPropList& PropList, TCHAR* tszCgi, TCHAR* tszName,const TCHAR* tszTitle,const TCHAR* tszOn)
//{
//	if (varmap[tszCgi].aSet[tszName].csCaption == _T("")) return;
//	if (wcslen(tszTitle) >0)
//		varmap[tszCgi].aSet[tszName].csCaption = tszTitle;
//
//	PropList.AddPropCheck( this , varmap[tszCgi].aSet[tszName].csCaption , &varmap[tszCgi].aSet[tszName].bCheck );	
//	varmap[tszCgi].aSet[tszName].bCheck = (wcscmp(varmap[tszCgi].aSet[tszName].csKey, tszOn) == 0) ? true : false;
//}
//void CGIVar::AddPropInt(EPropList& PropList, TCHAR* tszCgi, TCHAR* tszName, bool bWrite)
//{
//	if (varmap[tszCgi].aSet[tszName].csCaption == _T("")) return;
//	varmap[tszCgi].aSet[tszName].nInt = wcstol(varmap[tszCgi].aSet[tszName].csKey,NULL, 10);
//	PropList.AddPropInt(this, varmap[tszCgi].aSet[tszName].csCaption , &varmap[tszCgi].aSet[tszName].nInt, _T(""), bWrite);
//
//}
//void  CGIVar::AddPropCombo(EPropList& PropList, TCHAR* tszCgi, TCHAR* tszName, const TCHAR* tszCombSrc)
//{
//	//return;
//	if (varmap[tszCgi].aSet[tszName].csCaption == _T("")) return;
//	if (wcslen(tszCombSrc) == 0) return;
//
//	EPropertyCombo* pCombo = PropList.AddPropCombo(this, varmap[tszCgi].aSet[tszName].csCaption , &varmap[tszCgi].aSet[tszName].nInt);
//	TCHAR* tszSrc = new TCHAR[wcslen(tszCombSrc)+1];
//	wcscpy(tszSrc, tszCombSrc);
//	TCHAR* tszPart = wcstok(tszSrc, _T(","));
//	int ci=0;
//	while (tszPart)
//	{
//		pCombo->AddString(tszPart);
//		if (
//			wcsncmp(
//			varmap[tszCgi].aSet[tszName].csKey.LockBuffer(), 
//			tszPart, 
//			varmap[tszCgi].aSet[tszName].csKey.GetLength()
//			) == 0) 
//			varmap[tszCgi].aSet[tszName].nInt = ci ;
//		tszPart = wcstok(NULL, _T(","));
//		ci++;
//	}
//	delete tszSrc;
//}
//void  CGIVar::AddPropCombo(EPropList& PropList, TCHAR* tszCgi, TCHAR* tszName, TCHAR* tszSrcCgi, TCHAR* tszSrcName)
//{
//	if (varmap[tszCgi].aSet[tszName].csCaption == _T("")) return;
//	EPropertyCombo* pCombo = PropList.AddPropCombo(this, varmap[tszCgi].aSet[tszName].csCaption , &varmap[tszCgi].aSet[tszName].nInt);
//	CString cs = varmap[tszSrcCgi].aSet[tszSrcName].csKey.LockBuffer();
//	if (cs.GetLength() > 0)
//	{
//		TCHAR* tszSrc = cs.LockBuffer();
//		TCHAR* tszPart = wcstok(tszSrc, _T(","));
//		int ci=0;
//		while (tszPart)
//		{
//			pCombo->AddString(tszPart);
//			if (0 == wcsncmp( varmap[tszCgi].aSet[tszName].csKey.LockBuffer(),
//							  tszPart, 
//							  varmap[tszCgi].aSet[tszName].csKey.GetLength() )) 
//			{
//				varmap[tszCgi].aSet[tszName].nInt = ci ;
//			}
//			tszPart = wcstok(NULL, _T(","));
//			ci++;
//		}
//	}
//}
bool CGIVar::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bRet = true;;
	m_bDirty = true;
	bool& bDhcp =  varmap[_T("config/network.cgi")].aSet[_T("dhcp")].bCheck;
	bool& bPppoe =  varmap[_T("config/pppoe.cgi")].aSet[_T("pppoe")].bCheck;

	if (pProperty == &m_bStaticIP)
	{
		if (!m_bStaticIP)
		{
			bDhcp = false;
			bPppoe = false;
		}else
		{
			bDhcp = true;
			bPppoe = false;
		}
	}
	else if (pProperty == &bPppoe)
	{
		if (!bPppoe)
		{
			bDhcp = false;
			m_bStaticIP = false;
		}else
		{
			bDhcp = true;
			m_bStaticIP = false;
		}
	}
	else if (pProperty == &bDhcp)
	{
		if (!bDhcp)
		{
			m_bStaticIP = false;
			bPppoe  = false;
		}else
		{
			m_bStaticIP = true;
			bPppoe  = false;
		}
	}
	return bRet;
}
bool   CGIVar::IsPropertyEnabled( const void* pProperty )
{
	bool bEnabled = true;
/*
	bool bDhcp =  varmap["config/network.cgi"].aSet["dhcp"].bCheck;
	bool bPppoe =  varmap["config/pppoe.cgi"].aSet["pppoe"].bCheck;
	bool bDdns =  varmap["config/ddns.cgi"].aSet["ddns"].bCheck;
	int* pProvider =  &varmap["config/ddns.cgi"].aSet["provider"].nInt;
	CString* pDdnsHost =  &varmap["config/ddns.cgi"].aSet["host"].csKey;
	CString* pDdnsUser =  &varmap["config/ddns.cgi"].aSet["user"].csKey;
	CString* pDdnsPass =  &varmap["config/ddns.cgi"].aSet["pass"].csKey;
	CString* pPppoeUser =  &varmap["config/pppoe.cgi"].aSet["user"].csKey;
	CString* pPppoePass =  &varmap["config/pppoe.cgi"].aSet["pass"].csKey;
	CString* pIp        =  &varmap["config/network.cgi"].aSet["ip"].csKey;
	CString* pNetmask   =  &varmap["config/network.cgi"].aSet["netmask"].csKey;
	CString* pGeteway   =  &varmap["config/network.cgi"].aSet["gateway"].csKey;
	CString* pDns1      =  &varmap["config/network.cgi"].aSet["dns1"].csKey;


//	int  nPPPoEMTU =  varmap["config/pppoe.cgi"].aSet["pppoe"].nInt;
	

	if( pProperty==pProvider || pProperty==pDdnsHost ||  pProperty==pDdnsUser ||   pProperty==pDdnsPass)
	{
		bEnabled = bDdns;
	}
	else if( pProperty==pPppoeUser || pProperty==pPppoePass )// ||  pProperty==&m_nPPPoEMTU)
	{
		bEnabled = bPppoe;
	}
	else if( pProperty==&pIp || pProperty==&pNetmask ||  pProperty==&pGeteway ||   pProperty==&pDns1)
	{
		bEnabled = m_bStaticIP;
	}
*/
/*
	else if ( pProperty == &m_nVPF2Port 
				||  pProperty == &m_nVPF2CodecInx
				||  pProperty == &m_nVPF2ProtocolIdx
				||  pProperty == &m_nVPF2ResolutionIdx
				||  pProperty == &m_nVPF2GOPIdx
				||  pProperty == &m_nVPF2FrameRateIdx)
	{
		bEnabled = m_bVPF2Enable;
	}
*/
	return bEnabled;
}

CRITICAL_SECTION cs;

CString CGIVar::csGetAppCacheFolder()
{
	static bool bFirst = true;
	if (bFirst)
	{
		InitializeCriticalSection(&cs);
		bFirst = false;
	}
	EnterCriticalSection( &cs );

 	static CString csFolder(_T( ""));
	TRACE(csFolder);
	if (csFolder.GetLength() >0) return csFolder;
   TCHAR szPath[MAX_PATH];
    if(!SUCCEEDED(::SHGetFolderPath(
        NULL,       // use the desktop for the owner window
        CSIDL_APPDATA,     // current CSIDL to look up
        NULL,       // use the token of the logged in user
        SHGFP_TYPE_CURRENT, // get the current value
        szPath)))   // the buffer to hold the path
	{
		return CString("");
	}
    csFolder.Format(_T("%s\\%s"),szPath, CProductDefine::m_tzBrand);
	WIN32_FIND_DATA FindFileData = {0};
    HANDLE hFind = ::FindFirstFile(csFolder.LockBuffer(), &FindFileData);
    if(INVALID_HANDLE_VALUE == hFind)
	{
		if (!SUCCEEDED (::_wmkdir( csFolder.LockBuffer() )))
			return CString("");
	}
	csFolder += _T("\\Cache");
    hFind = ::FindFirstFile(csFolder.LockBuffer(), &FindFileData);
    if(INVALID_HANDLE_VALUE == hFind)
	{
		if (!SUCCEEDED (::_wmkdir( csFolder.LockBuffer() )))
			return CString("");
	}
	FindClose( hFind );
	CloseHandle(hFind);
	LeaveCriticalSection( &cs );
	return csFolder;

}

void* CGIVar::pLookupProperty(TCHAR* tszCgi, TCHAR* tszName)
{
	return &varmap[tszCgi].aSet[tszName];
}

bool CGIVar::bUpdateFirmware(CString csFile, NODEITEM* pNode)
{
#define BOUNDARY "{00FD9B83-C692-459a-B652-6D993C0875FA}"
	bool bRet = false;
	CGIVar* pThis = this;
	CInternetSession session;
	const TCHAR wszBoundary[] = _T(BOUNDARY);
	CFileFind finder;
	if (finder.FindFile(csFile) == FALSE) 
		return false;
	finder.FindNextFile();
	int nFileSize = finder.GetLength();

	CFile fileFw;
	fileFw.Open(csFile, CFile::modeRead);

	CHttpConnection* pServer=NULL;
	CHttpFile* pFile=NULL;

	USES_CONVERSION;
	WORD nEvent(0);

	UINT nBytesRead = 0;
	CString csUrl;

	csUrl.Format( _T("/config/firmwareupgrade.cgi"));
	CFile fileBuffer;

	pServer = session.GetHttpConnection( A2W(pNode->ip), INTERNET_PORT(pNode->nWebPort), A2W(pNode->szUser), A2W(pNode->szPass));
	pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, csUrl, NULL, 1, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_RELOAD);	
	CString csHead;
	csHead += _T("Content-Type: multipart/form-data; boundary=");
	csHead += wszBoundary;
	csHead += _T("\r\n\r\n");

	char szBodyHead[] = BOUNDARY "\r\nContent-Disposition: form-data; name=\"updateType\"" "\r\nfirmware\r\n" 
						"Accept: */*" 
						"Content-Disposition: form-data; name=\"flash\"; filename=\"EV1150-1.8.0-20100225-EV3151A.BIN\"\r\n"
						"Content-Type: multipart/form-data\r\n\r\n";//application/octet-stream\r\n\r\n";
	char szBodyTail[] = "\r\n" BOUNDARY "--\r\n";
	DWORD nHeadSize = strlen(szBodyHead);
	DWORD nTailSize = strlen(szBodyTail);

	int nBufferSize = nFileSize + nHeadSize + nTailSize;
	char* szBuffer = new char[nBufferSize];
	::ZeroMemory(szBuffer, nBufferSize);

	strncpy(szBuffer, szBodyHead, nHeadSize);
	nFileSize = fileFw.Read(&szBuffer[nHeadSize], nFileSize);
	strncpy(&szBuffer[nHeadSize+nFileSize], szBodyTail, nTailSize);
	
	nEvent = ::WaitForSingleObject(pThis->hDestory, 0);
	if (nEvent != WAIT_TIMEOUT)
		goto exit;
	DWORD nTimeOut;

	try {
		BOOL bOptionRet = pFile->QueryOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
		nTimeOut = 300000; // 5 minute
		pServer->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
		bOptionRet = pFile->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
		if (pFile->SendRequest(csHead, (LPVOID)szBuffer, nHeadSize+nFileSize+nTailSize)==0)
		{
			DWORD nRetStatus;
			pFile->QueryInfoStatusCode(nRetStatus);
			AfxMessageBox(_T("¤W¶Ç¥¢±Ñ"),MB_ICONINFORMATION);
		}
		else
		{
			CString csUrl;
			csUrl.Format( _T("http://%s:%d/config/system_reboot.cgi?reboot=go"), A2W(pNode->ip), pNode->nWebPort);
			TRACE(_T("%s\r\n"),csUrl);
			CStdioFile* pRetFile = NULL;
			try {
				pRetFile = session.OpenURL(csUrl, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_KEEP_CONNECTION);
			}
			catch(CInternetException* e) {
			//	e->ReportError();
			//	AfxMessageBox(csUrl);
				int nError = e->m_dwError;
				e->Delete();
			}
			if (pRetFile)
				delete pRetFile;
		}
	}
	catch(CInternetException* e) {
		pFile->QueryOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
		int nError = e->m_dwError;
		e->Delete();			
	}

exit:
	pFile->Close();
	pServer->Close();
	delete pFile;
	delete pServer;
	delete szBuffer;
	return bRet;
    
}

int CGIVar::vCheckVideoStatus(NODEITEM* pNode)
{
	int bRet = 3;
	USES_CONVERSION;
	CGISession session;
	CString csUrl;
	csUrl.Format( _T("http://%s:%s@%s:%d/common/info.cgi"), A2W(pNode->szUser), A2W(pNode->szPass), A2W(pNode->ip), pNode->nWebPort);

	TRACE(_T("%s\r\n"),csUrl);
	CStdioFile* pRetFile =NULL;
	try {
			pRetFile = session.OpenURL(csUrl, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_KEEP_CONNECTION);
			bRet=2;
	/*	char buffer[1024]={0};
		int nBytesRead = pRetFile->Read(buffer, 1024 - 1);
		char* sz = strstr(buffer, "reboot");
		if (sz)
		{
			char* szToken = strtok(sz, "=");
			if (szToken)
			{
				szToken = strtok(NULL, "\r\n");
				if (strstr(szToken, "yes"))
				{
					bRet = true;
				}
			}
		}*/
	}
	catch(CInternetException* e) {
		TRACE(_T("Error - %s", csUrl));
		int nError = e->m_dwError;
		e->Delete();
	}
	if (pRetFile)
		delete pRetFile;

	return bRet;
}//vCheckVideoStatus



bool CGIVar::bfnReboot(NODEITEM* pNode)
{
	bool bRet = false;
	USES_CONVERSION;
	CGISession session;
	CString csUrl;
	csUrl.Format( _T("http://%s:%s@%s:%d/config/system_reboot.cgi?reboot=go"), A2W(pNode->szUser), A2W(pNode->szPass), A2W(pNode->ip), pNode->nWebPort);

	TRACE(_T("%s\r\n"),csUrl);
	CStdioFile* pRetFile =NULL;
	try {
		pRetFile = session.OpenURL(csUrl, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_KEEP_CONNECTION);
		char buffer[1024]={0};
		int nBytesRead = pRetFile->Read(buffer, 1024 - 1);
		char* sz = strstr(buffer, "reboot");
		if (sz)
		{
			char* szToken = strtok(sz, "=");
			if (szToken)
			{
				szToken = strtok(NULL, "\r\n");
				if (strstr(szToken, "yes"))
				{
					bRet = true;
				}
			}
		}
	}
	catch(CInternetException* e) {
		int nError = e->m_dwError;
		e->Delete();
	}
	if (pRetFile)
		delete pRetFile;
	return bRet;
}

bool CGIVar::bfnTurnLed(NODEITEM *pNode, bool bOn)
{
	CInternetSession session;
	bool bRet = false;
	USES_CONVERSION;
	CString csUrl;
	csUrl.Format( _T("http://%s:%s@%s:%d/config/led.cgi?led=%s"), A2W(pNode->szUser), A2W(pNode->szPass), A2W(pNode->ip), pNode->nWebPort, (bOn ? _T("on") : _T("off")) );
	TRACE(_T("%s\r\n"),csUrl);
	CStdioFile* pRetFile = NULL;
	int nRetry =0;
	try {
		pRetFile = session.OpenURL(csUrl, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_KEEP_CONNECTION);	
		char buffer[1024]={0};
		int nBytesRead = pRetFile->Read(buffer, 1024 - 1);
		char* sz = strstr(buffer, "led");
		if (sz)
		{
			char* szToken = strtok(sz, "=");
			if (szToken)
			{
				szToken = strtok(NULL, "\r\n");
				bRet = !strcmp(bOn ? "ok" : "off", szToken);
			
			}
		}
	}
	catch(CInternetException* e) {			
		int nError = e->m_dwError;
		e->Delete();
	}
	if (pRetFile)
		delete pRetFile;
	return bRet;	
}
