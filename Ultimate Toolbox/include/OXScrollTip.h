// ==========================================================================
// 					Class Specification : COXScrollTip
// ==========================================================================

// Header file : OXScrollTip.h

// Version: 9.3

// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CWnd

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This class is used by the COXScrollTipOwner class.
//	It encapsulates the scroll tip window itself

// Remark:

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXSCROLLTIP_H__
#define __OXSCROLLTIP_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


class OX_CLASS_DECL COXScrollTip : public CWnd
{
DECLARE_DYNAMIC(COXScrollTip);

// Data members -------------------------------------------------------------
public:
protected:
	CPen	m_framePen;
	CBrush	m_backgroundBrush;
	COLORREF m_textColor;
	CFont	m_font;
	int		m_cxHScroll;
	int		m_cyVScroll;

	BOOL	m_bVisible;
	CString	m_sText;
	BOOL	m_bTextChanged;
	CRect	m_rect;
	BOOL	m_bRectChanged;
	BOOL	m_bFastBackgroundRepaint;

	static BOOL m_bWindowClassCreated;
	static CString m_sWindowClass;

private:
	
// Member functions ---------------------------------------------------------
public:
	COXScrollTip();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	int GetHorizontalArrowWidth() const;
	// --- In  :
	// --- Out : 
	// --- Returns : The width of the arrow (in pixels) in an horizontal scroll bar
	// --- Effect : 
	int GetVerticalArrowHeight() const;
	// --- In  :
	// --- Out : 
	// --- Returns : The height of the arrow (in pixels) in a vertical scroll bar
	// --- Effect : 

	BOOL Create(CWnd* pParentWnd);
	// --- In  : pParentWnd : The owner window
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : The window is created invisible and at position (0,0) with size (0,0)

	void Show(BOOL bShow);
	// --- In  : bShow : TRUE to show, FALSE to hide
	// --- Out : 
	// --- Returns : Shows or hides the scroll tip window
	// --- Effect : Do not used another function to show or hide the window

	CSize ComputeSize(LPCTSTR pszText);
	// --- In  : pszText : The text
	// --- Out : 
	// --- Returns : The size of the scroll tip window so that the text 
	//               can be shown nicely by the window
	// --- Effect : 

	void Adjust(CRect rect, LPCTSTR pszText);
	// --- In  : rect : New position (in screen coordinates)
	//			 pszText : New text
	// --- Out : 
	// --- Returns : 
	// --- Effect : Sets the new text and changes the position and size

	void EnableFastBackgroundRepaint(BOOL bEnable = TRUE);
	// --- In  : bEnable : Whether to enable or disable
	// --- Out : 
	// --- Returns : 
	// --- Effect : The scroll tip can request the background to be repainted 
	//              when it is moved (during scrolling)
	//				When it is enabled WM_PAINTs will be send, otherwise they will be
	//				posted (will introduces some delay)
	//				This feature is enabled by default

#ifdef _DEBUG
	virtual void AssertValid() const;
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : AssertValid performs a validity check on this object 
	//				by checking its internal state. 
	//				In the Debug version of the library, AssertValid may assert and 
	//				thus terminate the program.
	
	virtual void Dump(CDumpContext& dc) const;
	// --- In  : dc : The diagnostic dump context for dumping, usually afxDump.
	// --- Out : 
	// --- Returns :
	// --- Effect : Dumps the contents of the object to a CDumpContext object. 
	//				It provides diagnostic services for yourself and 
	//				 other users of your class. 
	//				Note  The Dump function does not print a newline character
	//				 at the end of its output. 
#endif

	virtual ~COXScrollTip();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

	//{{AFX_VIRTUAL(COXScrollTip)
	//}}AFX_VIRTUAL

protected:
	void AdjustColorsFontsMetrics();
	void Redraw();
	void RedrawBackground(CRect rect);

	//{{AFX_MSG(COXScrollTip)
	afx_msg void OnWinIniChange(LPCTSTR lpszSection);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
                   
};

#endif // __OXSCROLLTIP_H__
// ==========================================================================
