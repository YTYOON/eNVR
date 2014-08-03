// EtroExplorerDoc.h : interface of the CEtroExplorerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ETROEXPLORERDOC_H__F3F56E9F_621F_4777_B349_94D2E2CC1B4B__INCLUDED_)
#define AFX_ETROEXPLORERDOC_H__F3F56E9F_621F_4777_B349_94D2E2CC1B4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEtroExplorerDoc : public CDocument
{
protected: // create from serialization only
	CEtroExplorerDoc();
	DECLARE_DYNCREATE(CEtroExplorerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEtroExplorerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEtroExplorerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CEtroExplorerDoc* GetDocument();
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEtroExplorerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual void SetTitle(LPCTSTR lpszTitle);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETROEXPLORERDOC_H__F3F56E9F_621F_4777_B349_94D2E2CC1B4B__INCLUDED_)
