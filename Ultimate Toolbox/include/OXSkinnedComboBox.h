#if !defined(AFX_OXSKINNEDCOMBOBOX_H__73D96056_B76C_4C47_B4C0_3F3B2D0EC501__INCLUDED_)
#define AFX_OXSKINNEDCOMBOBOX_H__73D96056_B76C_4C47_B4C0_3F3B2D0EC501__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OXSkinnedComboBox.h : header file
//

// Version: 9.3

/////////////////////////////////////////////////////////////////////////////
// COXSkinnedComboBox window

#include "OXDllExt.h"

class COXToolbarSkin;

class OX_CLASS_DECL COXSkinnedComboBox : public CComboBox
{
	friend class COXToolbarSkinClassic;
	friend class COXToolbarSkinXP;

// Construction
public:
	COXSkinnedComboBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXSkinnedComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXSkinnedComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXSkinnedComboBox)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnKillFocus();
	afx_msg BOOL OnSetFocus();
	afx_msg BOOL OnSelChange();
	afx_msg BOOL OnCloseUp();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
    DECLARE_DYNCREATE(COXSkinnedComboBox)
	COXToolbarSkin* m_pToolbarSkin;
	COXToolbarSkin* GetToolbarSkin();
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	static HHOOK m_hMouseHook;
	static HWND m_hwndPrevMouseMoveWnd;
	bool m_bCommonControls6;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OXSKINNEDCOMBOBOX_H__73D96056_B76C_4C47_B4C0_3F3B2D0EC501__INCLUDED_)
