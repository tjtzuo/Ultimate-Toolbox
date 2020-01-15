
// Version: 9.3

#if !defined(_OXCUSTOMIZECOOLCONTROLSPAGE_H__)
#define _OXCUSTOMIZECOOLCONTROLSPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

#include "OXCustomizePage.h"
#include "OXOptionTreeCtrl.h"
#include "OXCoolControlOrganizer.h"
#include "OXMainRes.h"

class COXCustomizeCoolControlsPage;

class OX_CLASS_DECL COXCustomizeCoolControlOrganizer : public COXCoolControlOrganizer
{
public:
	COXCustomizeCoolControlOrganizer() : m_pCustomizeCoolControlsPage(NULL) {};

public:
	COXCustomizeCoolControlsPage* m_pCustomizeCoolControlsPage;

protected:
	virtual BOOL IsQualified(HWND hWnd, LPCTSTR lpszWndClass);
};


/////////////////////////////////////////////////////////////////////////////
// COXCustomizeCoolControlsPage dialog

class OX_CLASS_DECL COXCustomizeCoolControlsPage : public COXCustomizePage
{
	DECLARE_DYNCREATE(COXCustomizeCoolControlsPage)
// Construction
public:
	COXCustomizeCoolControlsPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(COXCustomizeCoolControlsPage)
	enum { IDD = IDD_OX_PAGE_COOLCONTROLS };
	COXOptionTreeCtrl	m_treeControlTypes;
	BOOL	m_bConvertToCool;
	//}}AFX_DATA


protected:
	COXCustomizeCoolControlOrganizer m_coolControlOrganizer;

	// map of all controls that might be converted
	CMap<int,int,CString,CString> m_mapControls;

	// map of the control's state (must be converted or not)
	CMap<int,int,int,int> m_mapControlState;

	// map of all control's window class and indexes 
	CMap<CString,LPCTSTR,int,int> m_mapControlWndClass;

public:
	virtual BOOL Load(const COXCustomizeManager* pCustomizeManager);
	virtual void Unload();
	virtual BOOL IsSupportingApplyChanges() { return TRUE; }
	virtual BOOL ApplyChanges();

	virtual CString GetTitle() const {
		CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZECOOLCTRLSTITLE));
		return sRet; /*_T("Cool Controls"); */
	}
	virtual LPCTSTR GetImageResource() const { 
		return MAKEINTRESOURCE(IDB_OX_CUSTOMIZE_COOLCONTROLS); 
	}
	virtual CString GetTooltip() const {
		CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZECOOLCTRLSTOOLTIP));
		return sRet;
		//_T("Tooltip for Cool Controls"); 
	}

	virtual BOOL OnCloseManager(BOOL bIsOk);

	virtual BOOL InitializeCoolControls(BOOL bConvertToCool=TRUE,
		LPCTSTR lpszProfileName=_T("CustomizeCoolControls"));

	inline COXCoolControlOrganizer* GetCoolControlOrganizer() {
		return &m_coolControlOrganizer;
	}

	virtual BOOL IsQualifiedForCoolControl(HWND hWnd, LPCTSTR lpszWndClass);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCustomizeCoolControlsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXCustomizeCoolControlsPage();   // destructor

protected:
	virtual void OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(COXCustomizeCoolControlsPage)
	afx_msg void OnButtonConvertAll();
	afx_msg void OnCheckConvert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXCUSTOMIZECOOLCONTROLSPAGE_H__)
