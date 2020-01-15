// ==========================================================================
// 						Class Implementation : 
//						COXStaticHyperLink
// ==========================================================================

// Implementation file : OXStaticHyperLink.cpp

// Version: 9.3

// COXStaticHyperLink implements a static control that's a hyperlink
// to any file on your desktop or web. You can use it in dialog boxes
// to create hyperlinks to web sites. When clicked, opens the file/URL
//
#include "StdAfx.h"
#include "OXStaticHyperLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////	
	
IMPLEMENT_DYNAMIC(COXStaticHyperLink, COXStatic)

BEGIN_MESSAGE_MAP(COXStaticHyperLink, COXStatic)
	//{{AFX_MSG_MAP(COXStaticHyperLink)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////
// Constructor sets default colors = blue/purple.
//
COXStaticHyperLink::COXStaticHyperLink():COXStatic()
{
	// blue
	m_clrUnvisited=RGB(0,0,255); 
	// purple
	m_clrVisited=RGB(128,0,128); 
	// not Visited yet
	m_bVisited=FALSE; 

	m_hCursor=NULL;

	m_bUseWindowText=TRUE;
	m_bErrorNotify=TRUE;

	m_bShowToolTip=TRUE;
	m_bToolTipUserDefined=FALSE;

	m_bFitToText=TRUE;

	m_hla.SetOwnerWnd(this);

}

// sets the COXHyperLinkAction object that will be executed if user clicks
void COXStaticHyperLink::SetAction(COXHyperLinkAction& hla) 
{ 
	m_hla=hla; 
	FormatToolTipText();
}
void COXStaticHyperLink::SetAction(int nActionID, LPCTSTR sFile, LPCTSTR sParameters, 
								   LPCTSTR sDefaultDirectory, int nShowCmd, 
								   UINT nCallbackMsgID, HWND hWndRecipient) 
{ 
	m_hla.Set(nActionID, sFile, sParameters, sDefaultDirectory, nShowCmd,
		nCallbackMsgID, hWndRecipient); 
	FormatToolTipText();
}

// sets the color of COXStaticHyperLink object that wasn't visited
// (like unvisited link in HTML)
void COXStaticHyperLink::SetUnvisitedColor(COLORREF clr, BOOL bRedraw) 
{ 
	m_clrUnvisited=clr; 
	if(bRedraw)
	{
		Invalidate();
	}
}

// sets the color of COXStaticHyperLink object that was visited
// (like visited link in HTML)
void COXStaticHyperLink::SetVisitedColor(COLORREF clr, BOOL bRedraw) 
{ 
	m_clrVisited=clr; 
	if(bRedraw)
	{
		Invalidate();
	}
}

// sets the state of control as visited or unvisited
void COXStaticHyperLink::SetVisited(BOOL bVisited, BOOL bRedraw) 
{ 
	m_bVisited=bVisited; 
	if(bRedraw)
	{
		Invalidate();
	}
}

// by default if COXHyperLinkAction::m_sFile is NULL we use text of 
// control's window as COXHyperLinkAction::m_sFile. Use next function 
// to set/unset this feature
void COXStaticHyperLink::SetUseWindowText(BOOL bUseWindowText) 
{ 
	m_bUseWindowText=bUseWindowText; 
	FormatToolTipText();
}

// returns TRUE if succeed and creates font used to draw text from plf, 
// otherwise FALSE
BOOL COXStaticHyperLink::SetDefaultTextFont()
{
	if(!IsStaticText(this)) 
		return TRUE;

	CFont* pFont=GetFont();
	if(pFont==NULL)
		return FALSE;

	ASSERT_VALID(pFont);

	if((HFONT)m_font!=NULL) 
		m_font.DeleteObject();

	LOGFONT lf;
	BOOL bResult=pFont->GetObject(sizeof(lf), &lf);
	if(bResult)
	{
		// by default we make it underline
		lf.lfUnderline=TRUE;
		bResult=m_font.CreateFontIndirect(&lf);

		if (bResult && m_bFitToText)
			AdjustToFitText(); 
	}

	return bResult;
}

//////////////////
// Handle reflected WM_CTLCOLOR to set custom control color.
// For non-text controls, do nothing. Also ensures SS_NOTIFY is on.
//
HBRUSH COXStaticHyperLink::CtlColor(CDC* pDC, UINT nCtlColor)
{
	HBRUSH hbr = COXStatic::CtlColor(pDC,nCtlColor);
	pDC->SetTextColor(m_bVisited ? m_clrVisited : m_clrUnvisited);
	return hbr;
}

/////////////////
// Handle mouse click: executes the associated action.
//
void COXStaticHyperLink::OnClicked()
{
	BOOL bUsedWindowText=FALSE;
	// if the file to open is NULL and m_bUseWindowText is TRUE then
	// we use text of control as the file to open
	if (m_hla.GetFile().IsEmpty() && m_bUseWindowText)
	{
		CString sWinText;
		// ..get it from window text
		GetWindowText(sWinText);	
		m_hla.SetFile(sWinText);
		bUsedWindowText=TRUE;
	}

	// execute the action
	if (m_hla.ExecuteIt(m_bErrorNotify)) 
	{
		// if action was successfully executed, set the control as visited
		m_bVisited = TRUE;
		Invalidate();		// repaint to show Visited color
	}

	if(bUsedWindowText)
	{
		m_hla.SetFile((LPCTSTR)NULL);
	}

}

// change the cursor 
BOOL COXStaticHyperLink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(message);

	// if the cursor wasn't set by a programmer,
	// initialize it
	if(m_hCursor==NULL)
	{
		return FALSE;
	}

	ASSERT(m_hCursor);
	
	::SetCursor(m_hCursor);

	return TRUE;
}

// builds tooltip text
void COXStaticHyperLink::FormatToolTipText()
{
	// set only if tooltip is shown
	if(m_bShowToolTip)
	{
		// if tooltip text wasn't set then we set it to the value of 
		// COXHyperLinkAction::m_sFile or if it is empty to text 
		// of the control
		if(!m_bToolTipUserDefined)
		{
			m_sToolTipText=m_hla.GetFile();
			if(m_sToolTipText.IsEmpty() && m_bUseWindowText)
			{
				if(IsStaticText(this))
				{
					GetWindowText(m_sToolTipText);
				}
				else
				{
					m_sToolTipText.Empty();
				}
			}
		}

		m_ttc.UpdateTipText(m_sToolTipText, this, ID_HLTOOLTIP);
	}
}

BOOL COXStaticHyperLink::InitStatic() 
{
	// set default cursor
	SetDefaultLinkCursor();

	return COXStatic::InitStatic();
}


