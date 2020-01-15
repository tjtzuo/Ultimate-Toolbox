// ==========================================================================
// 						Class Implementation : CMFCToolBar
// ==========================================================================

// Source file MFCToolBar.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                         
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MFCToolBar.h"

#ifdef CToolBar
	#undef CToolBar
#endif

#define TOOLBAR_BASE_CLASS CToolBar

#include "WTypes.h"

#ifndef __OXMFCIMPL_H__
#if _MFC_VER >= 0x0700
#if _MFC_VER >= 1400
#include <afxtempl.h>
#endif
#include <..\src\mfc\afximpl.h>
#else
#include <..\src\afximpl.h>
#endif
#define __OXMFCIMPL_H__
#endif

#if _MFC_VER >= 0x700

struct AFX_DLLVERSIONINFO
{
		DWORD cbSize;
		DWORD dwMajorVersion;                   // Major version
		DWORD dwMinorVersion;                   // Minor version
		DWORD dwBuildNumber;                    // Build number
		DWORD dwPlatformID;                     // DLLVER_PLATFORM_*
};

typedef HRESULT (CALLBACK* AFX_DLLGETVERSIONPROC)(AFX_DLLVERSIONINFO *);

#endif

// CMFCToolBar

IMPLEMENT_DYNAMIC(CMFCToolBar, TOOLBAR_BASE_CLASS)

CMFCToolBar::CMFCToolBar()
{
}

CMFCToolBar::~CMFCToolBar()
{
}


BEGIN_MESSAGE_MAP(CMFCToolBar, TOOLBAR_BASE_CLASS)
END_MESSAGE_MAP()



// CMFCToolBar message handlers

BOOL CMFCToolBar::CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle, DWORD dwStyle, CRect rcBorders, UINT nID)
{
#if _MFC_VER >= 0x0700
	// Remove all alignment styles
	dwStyle &= ~CBRS_ALIGN_ANY;
	// Add only one alignment style
	dwStyle |= CBRS_ALIGN_TOP;
#endif

	return TOOLBAR_BASE_CLASS::CreateEx(pParentWnd, dwCtrlStyle, dwStyle, rcBorders, nID);
}

void CMFCToolBar::SetBarStyle(DWORD dwStyle)
{
#if _MFC_VER >= 0x0700
	ASSERT((dwStyle & CBRS_ALL) == dwStyle);

	EnableToolTips(dwStyle & CBRS_TOOLTIPS);

	if (m_dwStyle != dwStyle)
	{
		DWORD dwOldStyle = m_dwStyle;
		m_dwStyle = dwStyle;
		OnBarStyleChange(dwOldStyle, dwStyle);
	}
#else
	TOOLBAR_BASE_CLASS::SetBarStyle(dwStyle);
#endif
}
