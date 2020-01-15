#pragma once

#pragma comment(lib, "uxtheme")

#include "oxdllext.h"

#include <shlwapi.h>

#pragma warning (push,3)
#include <vector>
#pragma warning (pop)

using namespace std;

// The following declarations are for the XP theme API (for pre-XP OS support)
typedef HANDLE HTHEME;
#define WM_THEMECHANGED 0x031A

#define TMT_TEXTCOLOR 3803
#define TMT_FONT 210

bool G_LoadThemeLibrary();

typedef HANDLE(__stdcall *OPENTHEMEDATA)(HWND hwnd, LPCTSTR pszClassList);
HTHEME G_OpenThemeData(HWND hwnd, LPCWSTR pszClassList);

typedef HRESULT(__stdcall *CLOSETHEMEDATA)(HANDLE hTheme);
HRESULT G_CloseThemeData(HTHEME hTheme);

typedef HRESULT(__stdcall *DRAWTHEMEBACKGROUND)(HANDLE hTheme, HDC hdc, 
		int iPartId, int iStateId, const RECT *pRect,  const RECT *pClipRect);
HRESULT G_DrawThemeBackground(HTHEME hTheme, HDC hdc, 
    int iPartId, int iStateId, const RECT *pRect, OPTIONAL const RECT *pClipRect);

typedef HRESULT (__stdcall *DRAWTHEMETEXT)(HANDLE hTheme, HDC hdc, int iPartId, 
		int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, 
		DWORD dwTextFlags2, const RECT *pRect);
HRESULT G_DrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, 
    int iStateId, LPCTSTR pszText, int iCharCount, DWORD dwTextFlags, 
    DWORD dwTextFlags2, const RECT *pRect);

typedef HRESULT (__stdcall *DRAWTHEMEEDGE)(HANDLE hTheme, HDC hdc, int iPartId, int iStateId, 
		const RECT *pDestRect, UINT uEdge, UINT uFlags,   RECT *pContentRect);
HRESULT G_DrawThemeEdge(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, 
                       const RECT *pDestRect, UINT uEdge, UINT uFlags, OPTIONAL OUT RECT *pContentRect);

typedef HRESULT (__stdcall *GETTHEMECOLOR)(HTHEME hTheme, int iPartId, 
	int iStateId, int iPropId, OUT COLORREF *pColor);
HRESULT G_GetThemeColor(HTHEME hTheme, int iPartId, 
    int iStateId, int iPropId, OUT COLORREF *pColor);

typedef HRESULT (__stdcall *GETTHEMEFONT)(HTHEME hTheme, OPTIONAL HDC hdc, int iPartId, 
    int iStateId, int iPropId, OUT LOGFONT *pFont);
HRESULT G_GetThemeFont(HTHEME hTheme, OPTIONAL HDC hdc, int iPartId, 
    int iStateId, int iPropId, OUT LOGFONT *pFont);

#define ANIMATIONTIMERID 3001
#define REVANIMATIONTIMERID 3002
#define ANIMATIONINTERVAL 60
#define NUMANIMSTEPS 8

enum GroupType
{
	GTList
};

class Item
{
	friend class COXTaskPanel;
	friend struct TaskGroup;
private:
	CRect m_ItemRect;
public:
	Item():m_bIsMouseHovered(false)
	{
	}
	CString m_strText;
	UINT m_nIconID;	
	CString m_strCmd;
	bool m_bIsMouseHovered;
};

struct TaskGroup
{
	TaskGroup(CString group = _T("")):m_strGroupName(group),
		m_bIsMouseHovered(false), m_GroupType(GTList),
		m_bIsCollapsed(true),m_bAnimationEnabled(false)
	{
	}

	CString m_strGroupName;
	operator CString()
	{
		return m_strGroupName;
	}
	CRect m_HeaderRect;
	bool m_bIsMouseHovered;
	GroupType m_GroupType;
	bool m_bIsCollapsed;
	bool m_bAnimationEnabled;

	void AddItem(const Item& item)
	{
		m_vecItems.push_back(item);
	}

	Item* FindItemWithPt(const CPoint& pt)
	{
		for(vector<Item>::iterator it = m_vecItems.begin(); it != m_vecItems.end(); it++)	
		{
			if(it->m_ItemRect.PtInRect(pt))
			{
				return &(*it);				
			}
		}
		return NULL;
	}
	vector<Item> m_vecItems;
};

// COXTaskPanel

#ifndef OCR_HAND
#define OCR_HAND 32649
#endif

enum PanelDimensions
{
	PDPanelFixedWidth = 225,
	PDLeftMargin = 10,
	PDTopMargin = 10,
	PDHeaderWidth = 190,
	PDHeaderHeight = 25,
	PDHeaderTextDisplacement = 9,
	PDHeaderIconDisplacement = 170,
	PDGroupSeparatorWidth = 40,
	PDGroupItemHeight = 20,
	PDGroupItemLeftMargin = 35,
	PDGroupIconLeftMargin = 9,
	PDGroupIconVerticalDisplacement = 1
};

class OX_CLASS_DECL COXTaskPanel : public CWnd
{
	DECLARE_DYNAMIC(COXTaskPanel)

public:
	COXTaskPanel();
	virtual ~COXTaskPanel();
protected:
	HTHEME m_hTheme;
	vector<TaskGroup> m_vecGroups;
	bool m_bLastHover;
	Item* m_ActiveItem;
	bool m_bMouseOnControl;
	bool m_bIgnoreButtonUp;
	int m_nAnimSteps;
	CWnd* pOldFocus;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThemeChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL Create(RECT rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	TaskGroup* AddGroup(LPCTSTR szGroupName);
	static bool IsThemed();
private:
	void DrawGroups(HDC hDC);
	void DrawGroupHeader(HDC hDC, const CRect& recGroupHeader, vector<TaskGroup>::iterator it);
	void DrawGroupItem(HDC hDC, const CRect& recGroupItem, vector<Item>::iterator it);
	void DrawDownArrow(HDC hDC, const CRect& recGroupIcon);
	void DrawUpArrow(HDC hDC, const CRect& recGroupIcon);
	bool RenderAlphaBlend(HDC hDC, HDC tmpDC, CRect recGroupBodyTarget, CRect recGroupBodySource);
	void AdjustScrollBars(CRect recGroupHeader);	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};