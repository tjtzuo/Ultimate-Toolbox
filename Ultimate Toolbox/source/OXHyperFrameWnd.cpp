// HyperFrameWnd.cpp : implementation file
//

#include "stdafx.h"
#include "OXHyperFrameWnd.h"


// COXHyperFrameWnd

IMPLEMENT_DYNCREATE(COXHyperFrameWnd, CFrameWnd)

COXHyperFrameWnd::COXHyperFrameWnd()
{

}

COXHyperFrameWnd::~COXHyperFrameWnd()
{
}


BEGIN_MESSAGE_MAP(COXHyperFrameWnd, CFrameWnd)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// COXHyperFrameWnd message handlers

void COXHyperFrameWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	if(WA_INACTIVE == nState && pWndOther && pWndOther->GetParentFrame() == this)
	{
		AfxGetMainWnd()->SendMessage(WM_NCACTIVATE, TRUE);
	}
}
