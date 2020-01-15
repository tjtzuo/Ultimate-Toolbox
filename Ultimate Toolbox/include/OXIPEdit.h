#if !defined(AFX_OXIPEDIT_H__0775CC24_115D_44E1_9F9E_082FE070040A__INCLUDED_)
#define AFX_OXIPEDIT_H__0775CC24_115D_44E1_9F9E_082FE070040A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OXIPEdit.h : header file
//

#include "OXDllExt.h"
#include "OXMaskedEdit.h"

/////////////////////////////////////////////////////////////////////////////
// COXIPEdit window

class OX_CLASS_DECL COXIPEdit : public COXMaskedEdit
{
// Construction
public:
	COXIPEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXIPEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	// Determines if the given IP address is empty
	BOOL IsEmptyIP(LPCTSTR lpszTestIP);

	// Returns a string with the IP address
	CString GetStringFromIP(BYTE nField0, BYTE nField1, BYTE nField2, BYTE nField3);

	// Returns an IP address from a formatted string
	BOOL GetIPFromString(LPCTSTR lpszIPString, BYTE& nField0, BYTE& nField1, BYTE& nField2, BYTE& nField3);

	// Return the IP address in a CString object
	virtual CString GetText();
	
	// Determines if the given IP address is valid (in terms of format only)
	virtual BOOL IsValidIP(LPCTSTR lpszTestIP);

	// Sets the IP address from a string
	virtual void SetText(LPCTSTR lpszIP);
	
	virtual ~COXIPEdit();

	// Generated message map functions
protected:
	// Returns the error string to be displayed when a bad IP address is entered.
	virtual CString GetErrorString();

	DECLARE_DYNCREATE(COXIPEdit)
	//{{AFX_MSG(COXIPEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OXIPEDIT_H__0775CC24_115D_44E1_9F9E_082FE070040A__INCLUDED_)
