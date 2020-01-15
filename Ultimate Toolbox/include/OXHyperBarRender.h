#pragma once

#include "oxdllext.h"

// This class is the workhorse of the hyperbar.  Both the CToolbar derived class and the overlay class maintain a pointer to an instance of this class, and call it
// to perform all rendering operations.  The class keeps a list of an internal struct which is used to track numerous things, most importantly, the commandID to send
// to the main window if we need to simulate a click on this button.
class OX_CLASS_DECL COXHyperBarRender
{
private:
	// 
	struct buttonInfo
	{
		int width;
		int defaultWidth;
		int height;
		int defaultHeight;
		int bitmapIndex;
		int commandID;
	};

public:
	COXHyperBarRender(CToolBar * pToolbar);
	~COXHyperBarRender(void);

	// Passing NULL for these last paramters means the mouse is not over the toolbar.
	void RenderToolbar(CDC * pDC, RECT * pRect, SIZE szBtn) { RenderToolbar(pDC, pRect, szBtn, NULL, NULL); }
	void RenderToolbar(CDC * pDC, RECT * pRect, SIZE szBtn, POINT * pMouse, CBitmap * pOverlay);
	void SetActiveItem(POINT pt);

	int GetActiveItemMsg(){return m_nCmd;};

	static double m_Focus;

private:
	int m_nCmd;
	CToolBar * m_pToolbar;
	COXHyperBarRender(void){};
	buttonInfo * m_buttonInfo;
	void Initialise(SIZE szBtn);
	CBitmap * m_buffer;
};
