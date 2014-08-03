// ProductDefine.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "ProductDefine.h"

HINSTANCE CProductDefine::g_hResStr;
HINSTANCE CProductDefine::g_hResOEMStr;
TCHAR CProductDefine::m_tzBrand[32];
TCHAR CProductDefine::m_tzProductName[32];
TCHAR CProductDefine::m_tzPlaybackName[32];
TCHAR CProductDefine::m_tzPrefix[32];
TCHAR CProductDefine::m_tzRegistSubFolder[MAX_PATH];

CProductDefine::CProductDefine()
{
}

CProductDefine::~CProductDefine()
{
}


// CProductDefine member functions
