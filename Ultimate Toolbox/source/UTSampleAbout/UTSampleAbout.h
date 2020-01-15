#if !defined(_UTSAMPLEABOUTDLG_H_)
#define _UTSAMPLEABOUTDLG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "UTSampleAboutResource.h"
#include "HtmlCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CUTSampleAboutDlg dialog used for App About

class CUTSampleAboutDlg : public CDialog
{
protected:
	CHtmlCtrl m_page;
public:
	CUTSampleAboutDlg();
	CUTSampleAboutDlg(CString sTitle, CString sDescriptionFile);
	CUTSampleAboutDlg(UINT nTitleID, UINT nDescriptionFileID); 

// Dialog Data
	//{{AFX_DATA(CUTSampleAboutDlg)
	enum { IDD = IDD_UT_SAMPLE_ABOUTBOX };
	CString m_sTitle;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUTSampleAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CUTSampleAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // _UTSAMPLEABOUTDLG_H_