
#include <string>
#include <map>

using namespace std;

class CJason
{
private:
	map<string, string> m_mapJason;
	map<string, map<string, string> > m_mapJasonPair;

	CString m_strBuffer;

	string PaserSimpleJasonFormat(const string& str, map<string, string>& mapJason);
	void PaserMultiJasonFormat(const string& str);
	string GetUniKey(const string& str , map<string, string>& mapJason);
	void PaserMultiCountJasonFormat(const string& str);
	void PaserOldJasonFormat(const string& str, map<string, string>& mapJason);
	void Initial();

public:
	void operator = (const CString& strBuffer)
	{
		m_mapJason.clear();
		m_mapJasonPair.clear();
		m_strBuffer = strBuffer;
		Initial();
	}

	void operator = (const CJason& jason)
	{
		m_mapJason.clear();
		m_mapJasonPair.clear();
		m_strBuffer = jason.m_strBuffer;
		Initial();
	}

	CJason(const CString& strBuffer = _T(""))
	{
		m_strBuffer = strBuffer;
		Initial();
	}
	bool isError();
	void GetValuebyKey(const CString& strKey, map<CString, CString>& mapJason);
	void GetValuebyKey(const CString& strKey, CString& strValue);
	void GetValuebyKey(const CString& strKey1, const CString& strKey2, CString& strValue);
	int GetCountByKey(const CString& strKey);
	void GetAllKey(vector<CString>& vcKey);
	void PaserValue(const CString strValue, vector<CString>& vcString);

};
