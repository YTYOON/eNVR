#include "stdafx.h"
#include "mycurl.h"
#include "resource.h"


//CString Curlplus::buffer;



Curlplus::Curlplus(void)
{ 
    CURLcode code = curl_global_init(CURL_GLOBAL_ALL);

    if(code != CURLE_OK)
    {
		//AfxMessageBox(IDS_CURL_INI_FAIL);
       //AfxMessageBox(_T("curl_init failed, error code is: " )); 
    }
}


Curlplus::~Curlplus(void)
{
    curl_global_cleanup();
} 


void Curlplus::post(CString url, CString csUser, CString csPWD, CString content) 
{
	USES_CONVERSION;
    m_csBuffer="";
    CURL *curl = curl_easy_init();

    if(curl == NULL)
    {
		CString cs;
        LOADSTRING(cs, IDS_CURL_EASY_FAIL);

		return;
    }
	char *chUrl = W2A(url);

	CString csUserPWD;
	csUserPWD.Format(_T("%s:%s"), csUser, csPWD);	
	char * cUserPWD = W2A(csUserPWD);

	char *chContent = W2A(content);

	curl_easy_setopt(curl, CURLOPT_URL, chUrl);
	curl_easy_setopt(curl, CURLOPT_USERPWD, cUserPWD );    
	curl_easy_setopt(curl, CURLOPT_POST, 1 );
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, chContent); 
	curl_easy_setopt(curl,CURLOPT_TIMEOUT,10);
	curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,1);
	//struct curl_slist *slist=NULL;
	//slist = curl_slist_append(slist, chContent);  
	//curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist); 


    CURLcode code = curl_easy_perform(curl);

    if(code != 0)//CURLE_OK)
    {
		CString csErrorMsg = errorMsgTranslator(code);		
		TRACE(csErrorMsg);     
    }

    curl_easy_cleanup(curl);
	//curl_slist_free_all(slist); /* free the list again */ 	
} 


CString Curlplus::get( CString url) 
{
	USES_CONVERSION;
	char *chUrl = W2A(url);
	
	m_csBuffer.Empty();
	
    CURL *curl = curl_easy_init();

    if(curl == NULL)
    {		
		return m_csBuffer;
    }   

	curl_easy_setopt(curl, CURLOPT_URL, chUrl);
	curl_easy_setopt(curl, CURLOPT_USERPWD, "root:pass");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	//curl_easy_setopt(curl,CURLOPT_TIMEOUT,120);
	curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,1);

    CURLcode code = curl_easy_perform(curl); 
	
    if(code != 0)//CURLE_OK)
    {
		CString csErrorMsg = _T("CURL Error CODE: ") + errorMsgTranslator(code);		
		TRACE(csErrorMsg);       
    }

	curl_easy_cleanup(curl);

	return m_csBuffer;
}

bool Curlplus::bGet(CString url, CString csUser, CString csPWD, CString& csBuffer)
{
	bool bResult = true;

	USES_CONVERSION;
	char *chUrl = W2A(url);

	m_csBuffer.Empty();

	CURL *curl = curl_easy_init();

	if(curl == NULL)
	{	
		csBuffer = m_csBuffer;
		bResult = false;
	}   
	else
	{	
		CString csUserPWD;
		csUserPWD.Format(_T("%s:%s"), csUser, csPWD);	
		char * chUserPWD = W2A(csUserPWD);

		curl_easy_setopt(curl, CURLOPT_URL, chUrl);
		curl_easy_setopt(curl, CURLOPT_USERPWD, chUserPWD);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(curl,CURLOPT_TIMEOUT,20);

		CURLcode code = curl_easy_perform(curl); 

		if(code != 0)
		{
			CString csErrorMsg = _T("CURL Error CODE: ") + errorMsgTranslator(code);		
			TRACE(csErrorMsg);      
			bResult = false;
		}

		curl_easy_cleanup(curl);

		csBuffer = m_csBuffer;
	}
	return bResult;
}

CString Curlplus::get( CString url, CString csUser, CString csPWD) 
{
	USES_CONVERSION;
	char *chUrl = W2A(url);

	m_csBuffer.Empty();
	
    CURL *curl = curl_easy_init();

    if(curl == NULL)
    {		
		return m_csBuffer;// Initialize failed return empty cstring
    }   

	CString csUserPWD;
	csUserPWD.Format(_T("%s:%s"), csUser, csPWD);	
	char * chUserPWD = W2A(csUserPWD);

	curl_easy_setopt(curl, CURLOPT_URL, chUrl);
	curl_easy_setopt(curl, CURLOPT_USERPWD, chUserPWD);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	//curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
	curl_easy_setopt(curl,CURLOPT_TIMEOUT,20);
	//curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,1);
	//curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 131072);

    CURLcode code = curl_easy_perform(curl); 
	
    if(code != 0)//CURLE_OK)
    {
		CString csErrorMsg = _T("CURL Error CODE: ") + errorMsgTranslator(code);		
		TRACE(csErrorMsg);        
    }

	curl_easy_cleanup(curl);

	return m_csBuffer;
}


int Curlplus::writer(char *data, size_t size, size_t nmemb, void *lParam)
{
	if (lParam ==	NULL)
        return 0;

	USES_CONVERSION;

	Curlplus* pThis = (Curlplus* )lParam;
    int len = size*nmemb;
	
	pThis->m_csBuffer+= A2W(data);

    return len;
}
CString Curlplus::errorMsgTranslator(CURLcode code)
{
	CString csCurlErrorMsg ;
	csCurlErrorMsg.Format( _T("Curl Proceesing unsuccessful and return the code:%d"), code);
	switch(code)
	{
	case 1:
		csCurlErrorMsg = _T("CURLE_UNSUPPORTED_PROTOCOL");    // 1 //
	break;

	case 2:
		csCurlErrorMsg = _T("CURLE_FAILED_INIT");    // 2 //
	break;

	case 3:
		csCurlErrorMsg = _T("CURLE_URL_MALFORMAT");    // 3 //
	break;

	case 5:
		csCurlErrorMsg = _T("CURLE_COULDNT_RESOLVE_PROXY");    // 5 //
	break;

	case 6:
		csCurlErrorMsg = _T("CURLE_COULDNT_RESOLVE_HOST");    // 6 //
	break;
	
	case 7:
		csCurlErrorMsg = _T("CURLE_COULDNT_CONNECT");    // 7 //
	break;

	case 8:
		csCurlErrorMsg = _T("CURLE_FTP_WEIRD_SERVER_REPLY");    // 8 //
	break;

	case 9:
		csCurlErrorMsg = _T("CURLE_REMOTE_ACCESS_DENIED");     // 9 a service was denied by the server
																//	due to lack of access - when login fails
																//	this is not returned. //
	break;
	
	case 11:
		csCurlErrorMsg = _T("CURLE_FTP_WEIRD_PASS_REPLY");    // 11 //
	break;

	case 13:
		csCurlErrorMsg = _T("CURLE_FTP_WEIRD_PASV_REPLY");    // 13 //
	break;
	
	case 14:
		csCurlErrorMsg = _T("CURLE_FTP_WEIRD_227_FORMAT");    // 14 //
	break;

	case 15:
		csCurlErrorMsg = _T("CURLE_FTP_CANT_GET_HOST");    // 15 //
	break;

	case 17:
		csCurlErrorMsg = _T("CURLE_FTP_COULDNT_SET_TYPE");    // 17 //
	break;

	case 18:
		csCurlErrorMsg = _T("CURLE_PARTIAL_FILE");    // 18 //
	break;
	case 19:
		csCurlErrorMsg = _T("CURLE_FTP_COULDNT_RETR_FILE");    // 19 //
	break;

	case 21:
		csCurlErrorMsg = _T("CURLE_QUOTE_ERROR");    // 21 //
	break;
	case 22:
		csCurlErrorMsg = _T("CURLE_HTTP_RETURNED_ERROR");    // 22 //
	break;
	case 23:
		csCurlErrorMsg = _T("CURLE_WRITE_ERROR");    // 23 //
	break;
	case 25:
		csCurlErrorMsg = _T(" CURLE_UPLOAD_FAILED");    // 25 - failed upload "command" //
	break;

	case 26:
		csCurlErrorMsg = _T("CURLE_READ_ERROR");              // 26 - couldn't open/read from file //
	break;
	case 27:
		csCurlErrorMsg = _T("CURLE_OUT_OF_MEMORY");              // 27 //
																  // Note: CURLE_OUT_OF_MEMORY may sometimes indicate a conversion error
																//instead of a memory allocation error if CURL_DOES_CONVERSIONS is defined	 //
	break;
	case 28:
		csCurlErrorMsg = _T("CURLE_OPERATION_TIMEDOUT");    // 28 //
	break;
	case 52:
		csCurlErrorMsg = _T("CURLE_GOT_NOTHING");             // 52 - when this is a specific error //
	default:
		break;
	 



/*

 // CURLE_FTP_PORT_FAILED,         // 30 - FTP PORT operation failed //
 // CURLE_FTP_COULDNT_USE_REST,    // 31 - the REST command failed //
 
 // CURLE_RANGE_ERROR,             // 33 - RANGE "command" didn't work //
 // CURLE_HTTP_POST_ERROR,         // 34 //
 // CURLE_SSL_CONNECT_ERROR,       // 35 - wrong when connecting with SSL //
 // CURLE_BAD_DOWNLOAD_RESUME,     // 36 - couldn't resume download //
 // CURLE_FILE_COULDNT_READ_FILE,  // 37 //
 // CURLE_LDAP_CANNOT_BIND,        // 38 //
//  CURLE_LDAP_SEARCH_FAILED,      // 39 //
 
 // CURLE_FUNCTION_NOT_FOUND,      // 41 //
 // CURLE_ABORTED_BY_CALLBACK,     // 42 //
 /// CURLE_BAD_FUNCTION_ARGUMENT,   // 43 //
 // CURLE_INTERFACE_FAILED,        // 45 - CURLOPT_INTERFACE failed //
 // CURLE_TOO_MANY_REDIRECTS ,     // 47 - catch endless re-direct loops //
 // CURLE_UNKNOWN_TELNET_OPTION,   // 48 - User specified an unknown option //
 // CURLE_TELNET_OPTION_SYNTAX ,   // 49 - Malformed telnet option //
 // CURLE_PEER_FAILED_VERIFICATION, // 51 - peer's certificate or fingerprint
                                     wasn't verified fine //
 // CURLE_GOT_NOTHING,             // 52 - when this is a specific error //
 // CURLE_SSL_ENGINE_NOTFOUND,     // 53 - SSL crypto engine not found //
 // CURLE_SSL_ENGINE_SETFAILED,    // 54 - can not set SSL crypto engine as
                                 //   default //
 // CURLE_SEND_ERROR,              // 55 - failed sending network data //
 // CURLE_RECV_ERROR,              // 56 - failure in receiving network data //
 // CURLE_SSL_CERTPROBLEM,         // 58 - problem with the local certificate //
 // CURLE_SSL_CIPHER,              // 59 - couldn't use specified cipher //
 // CURLE_SSL_CACERT,              // 60 - problem with the CA cert (path?) //
 // CURLE_BAD_CONTENT_ENCODING,    // 61 - Unrecognized transfer encoding //
 // CURLE_LDAP_INVALID_URL,        // 62 - Invalid LDAP URL //
 // CURLE_FILESIZE_EXCEEDED,       // 63 - Maximum file size exceeded //
 // CURLE_USE_SSL_FAILED,          // 64 - Requested FTP SSL level failed //
 // CURLE_SEND_FAIL_REWIND,        // 65 - Sending the data requires a rewind
                                 //   that failed //
 // CURLE_SSL_ENGINE_INITFAILED,   // 66 - failed to initialise ENGINE //
 // CURLE_LOGIN_DENIED,            // 67 - user, password or similar was not
                                 //   accepted and we failed to login //
 // CURLE_TFTP_NOTFOUND,           // 68 - file not found on server //
  //CURLE_TFTP_PERM,               // 69 - permission problem on server //
 //// CURLE_REMOTE_DISK_FULL,        // 70 - out of disk space on server //
 // CURLE_TFTP_ILLEGAL,            // 71 - Illegal TFTP operation //
 // CURLE_TFTP_UNKNOWNID,          // 72 - Unknown transfer ID //
 // CURLE_REMOTE_FILE_EXISTS,      // 73 - File already exists //
 // CURLE_TFTP_NOSUCHUSER,         // 74 - No such user //
 // CURLE_CONV_FAILED,             // 75 - conversion failed //
 // CURLE_CONV_REQD,               // 76 - caller must register conversion
                                    callbacks using curl_easy_setopt options
                                    CURLOPT_CONV_FROM_NETWORK_FUNCTION,
                                    CURLOPT_CONV_TO_NETWORK_FUNCTION, and
                                    CURLOPT_CONV_FROM_UTF8_FUNCTION //
  CURLE_SSL_CACERT_BADFILE,      // 77 - could not load CACERT file, missing
                                    or wrong format //
  CURLE_REMOTE_FILE_NOT_FOUND,   // 78 - remote file not found //
  CURLE_SSH,                     // 79 - error from the SSH layer, somewhat
                                    generic so the error message will be of
                                    interest when this has happened //

  CURLE_SSL_SHUTDOWN_FAILED,     // 80 - Failed to shut down the SSL
                                    connection //
  CURLE_AGAIN,                   // 81 - socket is not ready for send/recv,
                                    wait till it's ready and try again (Added
                                    in 7.18.2) //
  CURLE_SSL_CRL_BADFILE,         // 82 - could not load CRL file, missing or
                                    wrong format (Added in 7.19.0) //
  CURLE_SSL_ISSUER_ERROR,        // 83 - Issuer check failed.  (Added in
                                    7.19.0) //
  CURLE_FTP_PRET_FAILED,         // 84 - a PRET command failed //
  CURLE_RTSP_CSEQ_ERROR,         // 85 - mismatch of RTSP CSeq numbers //
  CURLE_RTSP_SESSION_ERROR,      // 86 - mismatch of RTSP Session Identifiers //
  CURLE_FTP_BAD_FILE_LIST,       // 87 - unable to parse FTP file list //
  CURLE_CHUNK_FAILED,            // 88 - chunk callback reported error //
*/

	}



 return csCurlErrorMsg;
}

CString Curlplus::RemoteGet(CString url, CString csUser, CString csPWD)
{
	USES_CONVERSION;
	char *chUrl = W2A(url);
    //buffer="";
	//char errorBuffer [CURL_ERROR_SIZE]; 
	csRemoteBuffer = _T("");

    CURL *curl = curl_easy_init();

    if(curl == NULL)
    {
		//AfxMessageBox(IDS_CURL_EASY_FAIL );
        //AfxMessageBox(_T("curl_easy_init failed "));
		return m_csBuffer;
    }   

	CString csUserPWD;
	csUserPWD.Format(_T("%s:%s"), csUser, csPWD);	
	char * cUserPWD = W2A(csUserPWD);

	curl_easy_setopt(curl, CURLOPT_URL, chUrl);
	curl_easy_setopt(curl, CURLOPT_USERPWD, cUserPWD);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->RemoteWriter);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	//curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
	curl_easy_setopt(curl,CURLOPT_TIMEOUT,20);
	//curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,1);
	//curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 100);

    CURLcode code = curl_easy_perform(curl); 
	
	

    if(code != 0)//CURLE_OK)
    {
		CString csErrorMsg = _T("CURL Error CODE: ") + errorMsgTranslator(code);
		
		TRACE(csErrorMsg);
         //AfxMessageBox(csErrorMsg) ;
    }

	curl_easy_cleanup(curl);

	return csRemoteBuffer;
}

int Curlplus::RemoteWriter(char *data, size_t size, size_t nmemb, void *lParam)
{
	if (lParam == NULL)
        return 0;

	USES_CONVERSION;
	Curlplus* pThis = (Curlplus* )lParam;
    int len = size*nmemb;

	//pThis->buffer = pThis->buffer + (data);
	//CString csThisBuffer;
	//csThisBuffer.Format(_T("%s"), data);
	pThis->csRemoteBuffer = pThis->csRemoteBuffer + A2W(data);

    return len;
}