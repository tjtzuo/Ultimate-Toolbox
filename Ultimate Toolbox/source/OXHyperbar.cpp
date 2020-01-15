// Hyperbar.cpp : implementation file
//

#include "stdafx.h"
#include "OXHyperbar.h"

#include "OXHyperbarOverlay.h"
#include "OXHyperBarRender.h"

#include <Winuser.h>

#include <algorithm>

// COXHyperBar

// These constants currently define the size of the overlay window that is generated.  
//The margin specified is applied in both directions, so if OffsetY is 10, then the 
// window is 10 pixels above and below the toolbar itself.
//
// A better solution may be to have the window resize itself as the icons resize.

int COXHyperBar::OffsetY = 140;
std::vector<COXHyperBar*> COXHyperBar::m_AllBars;

IMPLEMENT_DYNAMIC(COXHyperBar, CToolBar)

using std::vector;
using std::iterator;

COXHyperBar::COXHyperBar()
{
	m_wndInvisible = NULL;
	m_pRender = new COXHyperBarRender(this);
	m_bHasRedrawn = true;
	COXHyperBar::m_AllBars.push_back(this);
	m_backColor = RGB(0, 255, 255);
}

COXHyperBar::~COXHyperBar()
{
	if (m_wndInvisible)
	{
		m_wndInvisible->DestroyWindow();
		delete m_wndInvisible;
		m_wndInvisible = NULL;
	}

	if (m_pRender)
	{
		delete m_pRender;
		m_pRender = NULL;
	}

	std::vector<COXHyperBar*>::iterator it = std::find(m_AllBars.begin(), m_AllBars.end(), this);

	if (it !=  m_AllBars.end())
		COXHyperBar::m_AllBars.erase(it);
}


BEGIN_MESSAGE_MAP(COXHyperBar, CToolBar)
	//{{AFX_MSG_MAP(COXHyperBar)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// COXHyperBar message handlers

void COXHyperBar::OnPaint() 
{
	if (!m_wndInvisible->IsWindowVisible())
	{
		CPaintDC dc(this); // device context for painting
		RECT rc;
		this->GetClientRect(&rc);

		if (m_pRender)
			m_pRender->RenderToolbar(&dc, &rc, m_sizeImage);
	}
}

// To avoid flicker
BOOL COXHyperBar::OnEraseBkgnd(CDC* /* pDC */)
{
	return FALSE;
}

int COXHyperBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create invisible window
     if (!m_wndInvisible)
     {
		m_wndInvisible = new COXHyperBarOverlay(this, m_pRender, m_sizeImage);

		RECT rc;
		GetWindowRect(&rc);
		InflateRect(&rc, 0, OffsetY * 2);
		OffsetRect(&rc, 0, -OffsetY);

        LPCTSTR pstrOwnerClass = AfxRegisterWndClass(0);
		if (!m_wndInvisible->CreateEx(WS_EX_TOOLWINDOW, pstrOwnerClass, _T(""), WS_POPUP | WS_VISIBLE,
				rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
                m_hWnd, 0))
            return FALSE;

		m_wndInvisible->ShowWindow(SW_HIDE);
     }
	
	return 0;
}


// This function is called when our window is about to be shown.  It moves the invisible window into place,
// grabs a shot of the screen area that will be under it, and passes it through.
void COXHyperBar::RepositionInvisibleWindow()
{
	RECT rc;
	GetWindowRect(&rc);
	InflateRect(&rc, 0, OffsetY);

	RECT rcClient;
	GetClientRect(&rcClient);
	ClientToScreen(&rcClient);
	rc.left = rcClient.left;
	rc.right = rcClient.right;

	// Capture screenshot
	CDC dcScreen;
	dcScreen.Attach(::GetDC(NULL));
	CBitmap * bitmap = new CBitmap();
	bitmap->CreateCompatibleBitmap(&dcScreen, rc.right - rc.left, rc.bottom - rc.top);
	CDC dc;
	dc.CreateCompatibleDC(&dcScreen);
	CBitmap * pOld = dc.SelectObject(bitmap);

	RECT scrn = rc;
	ClientToScreen(&scrn);
	dc.BitBlt(0, 0, rc.right - rc.left, rc.bottom - rc.top, &dcScreen, rc.left, rc.top, SRCCOPY);
	dc.SelectObject(pOld);

	m_wndInvisible->SetWindowPos(NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER);
	m_wndInvisible->SetBitmap(bitmap);
}

void COXHyperBar::OnMouseMove(UINT /* nFlags */, CPoint point) 
{
	if(AfxGetMainWnd()->GetActiveWindow())
	// Changed this for VC6 support with no PSDK
//	WINDOWINFO in;
//	AfxGetMainWnd()->GetWindowInfo(&in);
//	if (in.dwWindowStatus == WS_ACTIVECAPTION)
	{
		RECT rcClient;
		GetClientRect(&rcClient);

		if (PtInRect(&rcClient, point))
		{
			if(!HasVisibleOverlay())
			{
				RepositionInvisibleWindow();
				// Make sure the main window remains active ( in truth, this message generally arrives too late )
				AfxGetMainWnd()->SendMessage(WM_NCACTIVATE, TRUE);
				m_wndInvisible->ShowWindow(SW_SHOW);
			}
		}
	}
}

// This method is used by the mousemove, to make sure no other bar is visible before showing this one.
// The m_bHasRedrawn bool is set to true when the unexpanded toolbar has been rendered, all this is needed
// because we can call Invalidate(), but cannot tell without a flag when the paint message has been processed,
// returning our window to the correct state for a new window to be drawn.
bool COXHyperBar::HasVisibleOverlay()
{
	std::vector<COXHyperBar*>::iterator it = m_AllBars.begin();

	while(it != m_AllBars.end())
	{
		if (!(*it)->m_bHasRedrawn)
			return true;

		++it;
	}

	return false;
}

// Used to expose setting the back col where no class has been changed.
void COXHyperBar::SetBackColor(COLORREF col)
{
	m_backColor = col;

	if (m_wndInvisible->IsWindowVisible())
	{
		m_wndInvisible->Invalidate();
	}
	else
	{
		Invalidate();
	}
}

// The default render background just fills with a solid color.
void COXHyperBar::RenderBackground(CDC * pDC, RECT * pRect)
{
	pDC->FillSolidRect(pRect, m_backColor);
}

// This method exists as a matter of convenience, it's private and the one that takes a RECT is the one users should override.
// This method is private so it cannot be overridden, eliminating the risk of two methods that do two different things.
void COXHyperBar::RenderBackground(CDC * pDC, int left, int top, int width, int height)
{
	RECT rc;
	rc.left = left;
	rc.top = top;
	rc.bottom = height + top;
	rc.right = width + left;

	RenderBackground(pDC, &rc);
}
