// Version: 9.3
// OXPropertyDescriptionPane.cpp : implementation file
//

#include "stdafx.h"
#include "OXPropertyDescriptionPane.h"
#include "OXPropertiesWnd.h"
#include "OXSkins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXPropertyDescriptionPane

COXPropertyDescriptionPane::COXPropertyDescriptionPane()
{
	// Create the fonts
	m_fontProperty.CreatePointFont(85, _T("Tahoma,,BOLD"));
	m_fontDescription.CreatePointFont(85, _T("Tahoma"));
}

COXPropertyDescriptionPane::~COXPropertyDescriptionPane()
{
}


BEGIN_MESSAGE_MAP(COXPropertyDescriptionPane, CWnd)
	//{{AFX_MSG_MAP(COXPropertyDescriptionPane)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COXPropertyDescriptionPane message handlers

void COXPropertyDescriptionPane::SetDescription(LPCTSTR lpszProperty, LPCTSTR lpszDescription)
{
	m_strProperty = lpszProperty;
	m_strDescription = lpszDescription;
	Invalidate();
}



void COXPropertyDescriptionPane::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	COXPropertiesWnd* pPropertiesWnd = (COXPropertiesWnd*) GetParent();
	COLORREF clrBackground = pPropertiesWnd->GetFrameSkin()->GetBackgroundColor();

	CRect rectClient;
	GetClientRect(rectClient);
	dc.FillSolidRect(rectClient, clrBackground);

	// Draw the frame rectangle
	rectClient.top += 4;
	CBrush brush;
	brush.CreateSolidBrush(clrBackground);
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, pPropertiesWnd->GetFrameSkin()->GetBorderColor());
	CBrush* pOldBrush = dc.SelectObject(&brush);
	CPen* pOldPen = dc.SelectObject(&pen);
	dc.Rectangle(rectClient);
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);

	// Draw the text - property name first
	CFont* pOldFont = dc.SelectObject(&m_fontProperty);
	rectClient.DeflateRect(5, 3);
	dc.DrawText(m_strProperty, rectClient, DT_SINGLELINE);

	// Draw the description
	rectClient.top += 25;
	dc.SelectObject(&m_fontDescription);
	dc.DrawText(m_strDescription, rectClient, DT_SINGLELINE);

	dc.SelectObject(pOldFont);
}
