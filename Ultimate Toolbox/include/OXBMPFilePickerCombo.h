// Version: 9.3

#if !defined(_OXBMPFILEPICKERCOMBO_H__)
#define _OXBMPFILEPICKERCOMBO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

#include "OXHistoryCombo.h"
#include "OXPreviewDialog.h"
#include "OXMainRes.h"

/////////////////////////////////////////////////////////////////////////////
// COXBMPFilePickerCombo window

class OX_CLASS_DECL COXBMPFilePickerCombo : public COXHistoryCombo
{
// Construction
public:
	COXBMPFilePickerCombo(CString sFileExtFilter);
	COXBMPFilePickerCombo(int nID=IDS_OX_FILEPICKERFILTER);
// Attributes
public:

protected:
	CString m_sFileExtFilter;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXBMPFilePickerCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXBMPFilePickerCombo();

	virtual BOOL BrowseItem();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXBMPFilePickerCombo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXBMPFILEPICKERCOMBO_H__)
