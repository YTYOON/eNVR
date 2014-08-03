#include "stdafx.h"

void CJason::PaserMultiCountJasonFormat(const string& str)
{
	char *buffer = new char[str.length()+1];
	strcpy(buffer,str.c_str());
	char *chKey = NULL, *chValue = NULL;
	string strKey, strValue, strbuffer;;
	map<string, string> mapTempJason, mapJason;

	chKey = strtok (buffer,"[");
	strbuffer = chKey;
	chValue = strtok (NULL,"]");
	strValue = chValue;
	while (chValue != NULL && chKey != NULL)
	{
		chKey = strtok (NULL,"[");
		chValue = strtok (NULL,"]");
		if (chKey != NULL && chValue!= NULL)
		{
			mapTempJason[chKey] = chValue;
		}
	}
	strKey = GetUniKey(strbuffer, mapTempJason);
	mapTempJason[strKey] = strValue;

	map<string, string>::iterator it = mapTempJason.begin();

	while(it != mapTempJason.end())
	{
		memset(buffer, 0x00, sizeof(buffer));
		strcpy(buffer,it->first.c_str());

		size_t found = it->first.find("\"");
		if (found > 0 && found != -1)
		{
			strtok (buffer,"\"");
			chKey = strtok (NULL,"\"");
		}
		else
		{
			chKey = strtok (buffer,"\"");
		}
		if (chKey)
		{
			strKey = chKey;
			memset(buffer, 0x00, sizeof(buffer));
			strcpy(buffer, it->second.c_str());
			chValue = strtok (buffer,"{");
			//chValue = strtok (NULL,"}");
			m_mapJason[strKey] = chValue;
		}
		it++;
	}

	it = m_mapJason.begin();
	while(it != m_mapJason.end())
	{
		mapTempJason.clear();
		PaserSimpleJasonFormat(it->second, mapTempJason);
		m_mapJasonPair[it->first] = mapTempJason;
		it++;
	}

	map<string, map<string, string> >::iterator itt = m_mapJasonPair.begin();
	while(itt != m_mapJasonPair.end())
	{
		string strr = itt->first;
		map<string, string>	 mapp = itt->second;
		itt++;
	}

	delete [] buffer;
}

void CJason::PaserMultiJasonFormat(const string& str)
{
	char *buffer = new char[str.length()+1];
	strcpy(buffer,str.c_str());
	char *chKey = NULL, *chValue = NULL;
	string strKey, strValue;
	map<string, string> mapTempJason, mapJason;

	chKey = strtok (buffer,":");
	chValue = strtok (NULL,"}");
	mapTempJason[chKey] = chValue;

	while (chValue != NULL && chKey != NULL)
	{
		chKey = strtok (NULL,"{");
		chValue = strtok (NULL,"}");
		if (chKey != NULL && chValue!= NULL)
		{
			mapTempJason[chKey] = chValue;
		}
	}

	map<string, string>::iterator it = mapTempJason.begin();

	while(it != mapTempJason.end())
	{
		memset(buffer, 0x00, sizeof(buffer));
		strcpy(buffer,it->first.c_str());
		strtok (buffer,"\"");
		chKey = strtok (NULL,"\"");

		if (chKey)
		{
			strKey = chKey;
			memset(buffer, 0x00, sizeof(buffer));
			strcpy(buffer, it->second.c_str());
			chValue = strtok (buffer,"{");
			m_mapJason[strKey] = chValue;
		}
		it++;
	}

	it = m_mapJason.begin();
	while(it != m_mapJason.end())
	{
		mapTempJason.clear();
		PaserSimpleJasonFormat(it->second, mapTempJason);
		m_mapJasonPair[it->first] = mapTempJason;
		it++;
	}

	delete [] buffer;
}

void CJason::PaserOldJasonFormat(const string& str, map<string, string>& mapJason)
{
	char *buffer = new char[str.length()+1];
	strcpy(buffer,str.c_str());
	char *chkey = NULL, *chValue = NULL;
	vector<string> vcstring;
	chValue = strtok (buffer,"\r\n");
	while (chValue != NULL)
	{
		vcstring.push_back(chValue);
		chValue = strtok (NULL,"\r\n");
	}

	int nIdx = 0, nCount = vcstring.size();
	for (nIdx=0; nIdx<nCount; nIdx++)
	{
		memset(buffer, 0x00, sizeof(buffer));
		strcpy(buffer,vcstring[nIdx].c_str());

		chkey = strtok (buffer,"=");
		chValue = strtok (NULL,"=");
		if (chkey != NULL && chValue != NULL)
		{
			mapJason[chkey] = chValue;
		}
	}

	delete []buffer;
}

string CJason::PaserSimpleJasonFormat(const string& str, map<string, string>& mapJason)
{
	string strReturn;
	char* chReturn = NULL;
	char *buffer = new char[str.length()+1];
	strcpy(buffer,str.c_str());
	char *chkey = NULL, *chValue = NULL;

	if (str.find("[") != -1)
	{
		map<string, string> maptempJason;

		chkey = strtok (buffer,"[");
		chValue = strtok (NULL, "]");

		maptempJason[chkey] = chValue;

		while (chValue != NULL)
		{
			chkey = NULL;chValue = NULL;

			chkey = strtok (NULL,"[");
			chValue = strtok (NULL, "]");
			if (chkey != NULL && chValue != NULL)
			{
				maptempJason[chkey] = chValue;
			}
			else
			{
				if (chkey != NULL)
				{
					string strRemain = chkey;
					maptempJason[strRemain] = "";
				}
			}
		}

		string strtemp;
		map<string, string>::iterator it = maptempJason.begin();
		while(it != maptempJason.end())
		{
			strtemp = GetUniKey(it->first, mapJason);
			mapJason[strtemp] = it->second;
			it++;
		}

	}
	else
	{
		size_t found = str.find("\"");
		if (found > 0)
		{
			chkey = strtok (buffer,"\"");
			chkey = strtok (NULL, "\"");
		}
		else
		{
			chkey = strtok (buffer,"\"");
		}

		strtok (NULL, "\"");
		chValue = strtok (NULL, "\"");

		if (chkey != NULL && chValue != NULL)
		{
			mapJason[chkey] = chValue;
		}
		else
		{
			if (chkey != NULL)
				strReturn = chkey;
		}

		while (chValue != NULL)
		{
			chkey = NULL;chValue = NULL;
			strtok (NULL,"\"");
			chkey = strtok (NULL, "\"");
			strtok (NULL, "\"");
			chValue = strtok (NULL, "\"");
			if (chkey != NULL && chValue != NULL)
			{
				mapJason[chkey] = chValue;
			}
			else
			{
				if (chkey != NULL)
				{
					strReturn = chkey;
				}
			}
		}
	}

	delete []buffer;
	return strReturn;
}

string CJason::GetUniKey(const string& str, map<string, string>& mapJason)
{
	char *chKey = NULL, *chValue = NULL, *chReturn = NULL, *buffer = NULL;
	string strReturn;
	if (str.length())
	{
		size_t found = str.find(":");
		if (found != -1)
		{
			strReturn = PaserSimpleJasonFormat(str, mapJason);
		}
		else
		{
			buffer = new char[str.length()+1];
			chKey = strtok (buffer,"\"");
			if (chKey != NULL)
			{
				chReturn = strtok (NULL,"\"");
				strReturn = chReturn;
			}

		}
	}
	delete[] buffer;
	return strReturn;
}

void CJason::GetValuebyKey(const CString& strKey, CString& strValue)
{
	USES_CONVERSION;
	strValue.Empty();
	map<string, string>::iterator it = m_mapJason.find(W2A(strKey));
	if(it != m_mapJason.end())
	{
		strValue.Format(_T("%S"),it->second.c_str());
	}
}

void CJason::GetValuebyKey(const CString& strKey, map<CString, CString>& mapJason)
{
	USES_CONVERSION;
	mapJason.clear();
	CString strFirst, strSecond;
	map<string, string>::iterator itString;
	map<string, map<string, string> >::iterator it = m_mapJasonPair.find(W2A(strKey));
	if(it != m_mapJasonPair.end())
	{
		itString = it->second.begin();
		while (itString != it->second.end())
		{
			strFirst.Format(_T("%S"),itString->first.c_str());
			strSecond.Format(_T("%S"),itString->second.c_str());
			mapJason[strFirst] = strSecond;

			itString++;
		}
	}
}

void CJason::GetValuebyKey(const CString& strKey1, const CString& strKey2, CString& strValue)
{
	USES_CONVERSION;
	strValue.Empty();
	map<string, map<string, string> >::iterator it = m_mapJasonPair.find(W2A(strKey1));
	map<string, string>::iterator itJason;

	if(it != m_mapJasonPair.end())
	{
		itJason = it->second.find(W2A(strKey2));
		if(itJason != it->second.end())
			strValue.Format(_T("%S"),itJason->second.c_str());
	}
}

int CJason::GetCountByKey(const CString& strKey)
{
	USES_CONVERSION;
	int nCount = 0;
	string str = W2A(strKey);
	size_t found = 0;
	map<string, string>::iterator it = m_mapJason.begin();
	while(it != m_mapJason.end())
	{
		found = it->first.find(str);
		if (found != -1)
		{
			nCount++;
		}
		it++;
	}

	return nCount;
}

void CJason::GetAllKey(vector<CString>& vcKey)
{
	map<string, string>::iterator it = m_mapJason.begin();
	CString str;

	while(it != m_mapJason.end())
	{
		str.Format(_T("%S"),it->first.c_str());
		vcKey.push_back(str);
		it++;
	}

}

void CJason::PaserValue(const CString strValue, vector<CString>& vcString)
{
	USES_CONVERSION;

	string str = W2A(strValue), strtocken;
	char *buffer = new char[str.length()+1], *ch;
	strcpy(buffer,str.c_str());
	CString cstr;

	ch = strtok (buffer,",");

	while (ch != NULL)
	{
		strtocken = ch;
		cstr.Format(_T("%S"), strtocken.c_str());
		cstr.Replace(_T("\""), _T(""));

		vcString.push_back(cstr);
		ch = strtok (NULL,",");
	}

	delete []buffer;
}

void CJason::Initial()
{
	USES_CONVERSION;
	string str = W2A(m_strBuffer);

	if (m_strBuffer.Find(_T(":{")) != -1)
	{
		PaserMultiJasonFormat(str);
	}
	else if (m_strBuffer.Find(_T(":[")) != -1)
	{
		PaserMultiCountJasonFormat(str);
	}
	else if (m_strBuffer.Find(_T("\n")) != -1 && m_strBuffer.Find(_T("=")) != -1)
	{
		PaserOldJasonFormat(str, m_mapJason);
	}
	else
	{
		PaserSimpleJasonFormat(str, m_mapJason);
	}
}

bool CJason::isError()
{
	bool bResult = false; 
	if( m_strBuffer.IsEmpty() )
	{
		bResult = true;
	}
	else if(m_strBuffer.Find(_T("401")) != -1 && m_strBuffer.Find(_T("Unauthorized")) != -1)
	{
		bResult = true;
	}
	else if(m_strBuffer.Find(_T("Error")) != -1)
	{
		bResult = true;
	}
	else if(m_strBuffer.Find(_T("Video service is not ready.")) != -1)
	{
		bResult = true;
	}
	else if(m_strBuffer.Find(_T("parameter not vaild")) != -1)
	{	
		bResult = true;
	}
	return bResult;
}
