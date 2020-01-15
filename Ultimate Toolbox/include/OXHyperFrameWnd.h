#pragma once

#include "oxdllext.h"

// COXHyperFrameWnd frame

class OX_CLASS_DECL COXHyperFrameWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(COXHyperFrameWnd)
protected:
	COXHyperFrameWnd();           // protected constructor used by dynamic creation
	virtual ~COXHyperFrameWnd();

protected:
	// Generated message map functions
	//{{AFX_MSG(CConfig)
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


