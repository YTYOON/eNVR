// EtroExplorerDoc.cpp : implementation of the CEtroExplorerDoc class
//

#include "stdafx.h"
#include "NVR.h"
#include "EtroExplorerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEtroExplorerDoc

IMPLEMENT_DYNCREATE(CEtroExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CEtroExplorerDoc, CDocument)
	//{{AFX_MSG_MAP(CEtroExplorerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEtroExplorerDoc construction/destruction

CEtroExplorerDoc::CEtroExplorerDoc()
{
	// TODO: add one-time construction code here

}

CEtroExplorerDoc::~CEtroExplorerDoc()
{
}

BOOL CEtroExplorerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CEtroExplorerDoc serialization

void CEtroExplorerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEtroExplorerDoc diagnostics

#ifdef _DEBUG
void CEtroExplorerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEtroExplorerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEtroExplorerDoc commands

void CEtroExplorerDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: Add your specialized code here and/or call the base class

	CDocument::SetTitle(_T(""));
}
