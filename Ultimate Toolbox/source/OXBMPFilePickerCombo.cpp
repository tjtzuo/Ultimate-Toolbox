// OXBMPFilePickerCombo.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXBMPFilePickerCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXBMPFilePickerCombo

COXBMPFilePickerCombo::COXBMPFilePickerCombo(CString sFileExtFilter) :
	m_sFileExtFilter(sFileExtFilter)
{
}

COXBMPFilePickerCombo::COXBMPFilePickerCombo(int nID) 
{
	m_sFileExtFilter.LoadString(nID);
}

COXBMPFilePickerCombo::~COXBMPFilePickerCombo()
{
}


BEGIN_MESSAGE_MAP(COXBMPFilePickerCombo, COXHistoryCombo)
	//{{AFX_MSG_MAP(COXBMPFilePickerCombo)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXBMPFilePickerCombo message handlers

BOOL COXBMPFilePickerCombo::BrowseItem()
{
	ASSERT_VALID(this);

	BOOL bBrowseOK = FALSE;
	CString sFileName;
	GetWindowText(sFileName);

	COXPreviewDialog dlgOpen(TRUE,NULL,sFileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,m_sFileExtFilter);
	if(dlgOpen.DoModal()==IDOK)
	{
		sFileName=dlgOpen.GetPathName();
		SetWindowText(sFileName);

		CWnd* pParentWnd=GetParent();
		if(pParentWnd!=NULL)
		{
			pParentWnd->SendMessage(WM_COMMAND,
				MAKEWPARAM(GetDlgCtrlID(),CBN_EDITCHANGE),
				(LPARAM)GetSafeHwnd());
		}
	
		bBrowseOK=TRUE;
	}

	ASSERT_VALID(this);

	return bBrowseOK;
}
