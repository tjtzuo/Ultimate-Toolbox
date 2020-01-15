#if !defined(AFX_OXPROPERTYTREE_H__7A0BD709_1B44_4680_8920_D8A7281E0A44__INCLUDED_)
#define AFX_OXPROPERTYTREE_H__7A0BD709_1B44_4680_8920_D8A7281E0A44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OXPropertyTree.h : header file
//
#include "OXDllExt.h"
#include "OXTreeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// COXPropertyTree window

class COXPropertiesWnd;

class OX_CLASS_DECL COXPropertyTree : public COXTreeCtrl
{
	friend class COXPropertiesWnd;

// Construction
public:
	COXPropertyTree(COXPropertiesWnd* pPropertiesWnd);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXPropertyTree)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXPropertyTree();

	// Generated message map functions
protected:
	DECLARE_DYNAMIC(COXPropertyTree)
	//{{AFX_MSG(COXPropertyTree)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillEditFocus();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	COXPropertiesWnd* m_pPropertiesWnd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OXPROPERTYTREE_H__7A0BD709_1B44_4680_8920_D8A7281E0A44__INCLUDED_)
