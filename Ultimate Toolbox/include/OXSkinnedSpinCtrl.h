#if !defined(AFX_OXSKINNEDSPINCTRL_H__66C1202F_521A_4581_99E3_F9F06961DA38__INCLUDED_)
#define AFX_OXSKINNEDSPINCTRL_H__66C1202F_521A_4581_99E3_F9F06961DA38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OXSkinnedSpinCtrl.h : header file
//
// Version: 9.3


#include "OXDllExt.h"

/////////////////////////////////////////////////////////////////////////////
// COXSkinnedSpinCtrl window

class COXTabSkin;

class OX_CLASS_DECL COXSkinnedSpinCtrl : public CSpinButtonCtrl
{
	friend class COXTabSkinClassic;
	friend class COXTabSkinXP;

// Construction
public:
	COXSkinnedSpinCtrl();

private:
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	static HHOOK m_hMouseHook;
	static HWND m_hwndPrevMouseMoveWnd;
	COXTabSkin* m_pTabSkin;
	COXTabSkin* GetTabSkin();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlatUpdown)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXSkinnedSpinCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXSkinnedSpinCtrl)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	virtual void PreSubclassWindow();
private:
	DECLARE_DYNCREATE(COXSkinnedSpinCtrl)
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OXSKINNEDSPINCTRL_H__66C1202F_521A_4581_99E3_F9F06961DA38__INCLUDED_)
