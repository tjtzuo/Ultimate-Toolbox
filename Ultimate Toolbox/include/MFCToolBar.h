// ==========================================================================
// 							Class Specification : CMFCToolBar
// ==========================================================================

// Header file : MFCToolBar.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

#pragma once

#include "OXDllExt.h"

#define TOOLBAR_BASE_CLASS CToolBar

// CMFCToolBar

class OX_CLASS_DECL CMFCToolBar : public TOOLBAR_BASE_CLASS
{
	DECLARE_DYNAMIC(CMFCToolBar)

public:
	void SetBarStyle(DWORD dwStyle);
	CMFCToolBar();
	virtual ~CMFCToolBar();
	virtual BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = TBSTYLE_FLAT,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
		CRect rcBorders = CRect(0, 0, 0, 0),
		UINT nID = AFX_IDW_TOOLBAR);

protected:
	DECLARE_MESSAGE_MAP()
};


