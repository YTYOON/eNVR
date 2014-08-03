#ifndef MYCURL_H
#define MYCURL_H

#include <curl.h>
#include <easy.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Curlplus
{
public:

	 Curlplus(void);
	 ~Curlplus(void);

	 void post(CString url,CString csUser, CString csPWD,CString content) ;
	 CString get(CString url);
  	 CString get(CString url, CString csUser, CString csPWD);
	 bool bGet(CString url, CString csUser, CString csPWD, CString& csBuffer);

	 CString errorMsgTranslator(CURLcode code);	 
	 static int writer(char* data, size_t size, size_t nmemb,void *lParam);

	 CString RemoteGet(CString url, CString csUser, CString csPWD);
	 static int RemoteWriter(char* data, size_t size, size_t nmemb,void *lParam);

private:
	 CString m_csBuffer;
	 CString csNotifyBuffer;
     CString csRemoteBuffer;



};
#endif