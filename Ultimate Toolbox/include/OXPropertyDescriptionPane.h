#if !defined(AFX_OXPROPERTYDESCRIPTIONPANE_H__D6507A74_8D1F_48BB_B820_099366191D16__INCLUDED_)
#define AFX_OXPROPERTYDESCRIPTIONPANE_H__D6507A74_8D1F_48BB_B820_099366191D16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OXPropertyDescriptionPane.h : header file
//

#include "OXDllExt.h"

// Version: 9.3

/////////////////////////////////////////////////////////////////////////////
// COXPropertyDescriptionPane window

class OX_CLASS_DECL COXPropertyDescriptionPane : public CWnd
{
// Construction
public:
	COXPropertyDescriptionPane();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXPropertyDescriptionPane)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetDescription(LPCTSTR lpszProperty, LPCTSTR lpszDescription);
	virtual ~COXPropertyDescriptionPane();

	// Generated message map functions
protected:
	CFont m_fontProperty;
	CFont m_fontDescription;
	CString m_strDescription;
	CString m_strProperty;
	//{{AFX_MSG(COXPropertyDescriptionPane)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OXPROPERTYDESCRIPTIONPANE_H__D6507A74_8D1F_48BB_B820_099366191D16__INCLUDED_)
