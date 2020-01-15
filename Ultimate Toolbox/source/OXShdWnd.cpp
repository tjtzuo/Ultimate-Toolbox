// OXShdWnd.cpp : implementation file
//

#include "stdafx.h"
#include "OXShdWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const int m_giFirst = 5;
const int m_giSecond = 5;

/////////////////////////////////////////////////////////////////////////////
// COXShdWnd

COXShdWnd::COXShdWnd()
{
	m_pCastingWindow = NULL;
}

COXShdWnd::~COXShdWnd()
{
}


BEGIN_MESSAGE_MAP(COXShdWnd, CWnd)
	//{{AFX_MSG_MAP(COXShdWnd)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COXShdWnd message handlers

//DEL void COXShdWnd::OnPaint() 
//DEL {
//DEL 	CPaintDC dc(this); // device context for painting
//DEL 	
//DEL 	CRect rect;
//DEL 	GetWindowRect(rect);
//DEL 
//DEL 	DrawShadow(dc.m_hDC, rect, NULL);
//DEL }

COLORREF COXShdWnd::DarkenColor(long lScale, COLORREF lColor)
{ 
	long R = MulDiv(GetRValue(lColor), (255 - lScale), 255);
	long G = MulDiv(GetGValue(lColor), (255 - lScale), 255);
	long B = MulDiv(GetBValue(lColor), (255 - lScale), 255);

	return RGB(R, G, B); 
}

// Wrapper function for ::GetPixel(...) that facilitates the shadow buffering
COLORREF COXShdWnd::GetShadowPixel(HDC hDC, int iXPos, int iYPos, bool bUseSavedShadow)
{
	if (bUseSavedShadow)
		return 0; // we don't need this pixel

	return ::GetPixel(hDC, iXPos, iYPos);
};

// Wrapper function for ::SetPixelV(...) that facilitates the shadow buffering
void COXShdWnd::SetShadowPixel(HDC hDC, int iXPos, int iYPos, COLORREF clr, bool bSaveShadow, bool bUseSavedShadow, OXSHADOWARRAY* pShadowArray, int& iIndex)
{
	if (bUseSavedShadow)
		::SetPixelV(hDC, iXPos, iYPos, pShadowArray->GetAt(iIndex++));
	else
		::SetPixelV(hDC, iXPos, iYPos, clr);

	if (bSaveShadow)
		pShadowArray->Add(clr);

//	AddPixelToMap(iXPos, iYPos, clr);
};

// Draws a menu shadow for the given rectangle
void COXShdWnd::DrawShadow(HDC hDC, LPRECT lpRect, OXSHADOWARRAY* pShadowArray)
{

	int iArrayIndex = 0;
	bool bSaveShadow, bUseSavedShadow;
	if (pShadowArray == NULL)
	{
		// We don't need to use the shadow array
		bSaveShadow = false;
		bUseSavedShadow = false;
	}
	else if (pShadowArray->GetSize() == 0)
	{
		// Save the shadow in the array
		bSaveShadow = true;
		bUseSavedShadow = false;
	}
	else
	{
		// Use the shadow from the array
		bSaveShadow = false;
		bUseSavedShadow = true;
	}

	CRect rectWindow(lpRect);

	// Draw the shadow - get the pixels from the desktop, darken them
	// and place them on the popup window
	HDC hDesktopDC = ::GetWindowDC(::GetDesktopWindow());

	// copy desktop to mem DC and work with that for Get/Set pixel (Vista bug)
	CDC* pDC = CDC::FromHandle(hDesktopDC);

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rectWindow.Width(), rectWindow.Height());
	CBitmap *pOldBitmap = dcMem.SelectObject(&bitmap);
	
	// working with a mem bitmap at 0,0 for GetPixel
	dcMem.BitBlt(0,0, rectWindow.Width(), rectWindow.Height(), pDC, rectWindow.left, rectWindow.top, SRCCOPY);

	int x, y;
	CPoint pt;
	COLORREF clr;

	int iLeft = 4;	
	int iTop =  4;
	int iRight = rectWindow.Width();
	int iBottom = rectWindow.Height();

	// Right shadow 
	for (x = 1; x <= 4; x++)
	{ 
		// Copy the top right pixels
		for (y = 1; y <= 4; y++)
		{
			pt.x = iRight - x;
			pt.y = iTop + y - 4 - 1;
			clr = GetShadowPixel(dcMem.m_hDC, pt.x, pt.y, bUseSavedShadow);
			SetShadowPixel(hDC, pt.x, pt.y, clr, bSaveShadow, bUseSavedShadow, pShadowArray, iArrayIndex);
		}

		// Top right corner
		for (y = 4; y >= 1; y--)
		{
			pt.x = iRight - x;
			pt.y = iTop + y - 1;
			clr = GetShadowPixel(dcMem.m_hDC, pt.x, pt.y, bUseSavedShadow);
			SetShadowPixel(hDC, pt.x, pt.y, DarkenColor(3 * x * y, clr), bSaveShadow, bUseSavedShadow, pShadowArray, iArrayIndex);
		}

		// Vertical line
		for (y = iTop + 4; y <= iBottom - 4 - 1; y++)
		{
			pt.x = iRight - x;
			pt.y = y;
			clr = GetShadowPixel(dcMem.m_hDC, pt.x, pt.y, bUseSavedShadow);
			SetShadowPixel(hDC, pt.x, pt.y, DarkenColor(15 * x, clr), bSaveShadow, bUseSavedShadow, pShadowArray, iArrayIndex);
		}

		// Bottom right corner
		for (y = 1; y <= 4; y++)
		{
			pt.x = iRight - x;
			pt.y = iBottom - y;
			clr = GetShadowPixel(dcMem.m_hDC,  pt.x, pt.y, bUseSavedShadow);
			SetShadowPixel(hDC, pt.x, pt.y, DarkenColor(3 * x * y,  clr), bSaveShadow, bUseSavedShadow, pShadowArray, iArrayIndex);
		}
	} 

	// Bottom shadow
	for (y = 1; y <= 4; y++)
	{
		// Copy the bottom left pixels
		for (x = 1; x <= 4; x++)
		{
			pt.x = iLeft - x;
			pt.y = iBottom - y;
			clr = GetShadowPixel(dcMem.m_hDC, pt.x, pt.y, bUseSavedShadow);
			SetShadowPixel(hDC, pt.x, pt.y, clr, bSaveShadow, bUseSavedShadow, pShadowArray, iArrayIndex);
		}

		// Bottom left corner
		for (x = 1; x <= 4; x++)
		{
			pt.x = iLeft - x + 4;
			pt.y = iBottom - y;
			clr = GetShadowPixel(dcMem.m_hDC, pt.x, pt.y, bUseSavedShadow);
			SetShadowPixel(hDC, pt.x, pt.y, DarkenColor(3 * (5 - x) * y, clr), bSaveShadow, bUseSavedShadow, pShadowArray, iArrayIndex);
		}
		
		// Horizontal line
		for (x = iLeft + 4; x <= iRight - 5; x++)
		{
			pt.x = x;
			pt.y = iBottom - y;
			clr = GetShadowPixel(dcMem.m_hDC, pt.x, pt.y, bUseSavedShadow);
			SetShadowPixel(hDC, pt.x, pt.y, DarkenColor(15 * y, clr), bSaveShadow, bUseSavedShadow, pShadowArray, iArrayIndex);
		}
	} 
	// done with bitmap here
	dcMem.SelectObject(pOldBitmap);


	::ReleaseDC(0, hDesktopDC);


}

void COXShdWnd::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
//Invalidate();

	
}

BOOL COXShdWnd::OnEraseBkgnd(CDC* pDC) 
{
    // handle pending WM_PAINT messages
    MSG msg;
    while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
	{
//		if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
//			return FALSE;
		ASSERT(msg.message == WM_PAINT);
        DispatchMessage(&msg);
	}

	CRect rect;
	GetWindowRect(rect);

	DrawShadow(pDC->m_hDC, rect, NULL);

//	if (m_pCastingWindow != NULL)
//		::SetWindowPos(m_hWnd, m_pCastingWindow->m_hWnd, 0, 0, 0, 0, 
//			SWP_NOSENDCHANGING | SWP_NOMOVE | SWP_NOSIZE);

	return FALSE;
}


void COXShdWnd::SetCastingWindow(CWnd* pWnd)
{
	m_pCastingWindow = pWnd;
}
