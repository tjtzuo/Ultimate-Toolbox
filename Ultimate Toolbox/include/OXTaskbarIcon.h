// ==========================================================================
// 					Class Specification : COXTaskbarIcon
// ==========================================================================
// Header file :		OXTaskbarIcon.h
//
// Version: 9.3
//
// Properties:
//		NO		 Abstract class (does not have any objects)
//		YES		 Derived from CObject
//
//		YES		 Is a CWnd.                     
//		YES		 Two stage creation (constructor & Create())
//		YES		 Has a message map
//		NO		 Needs a resource (template)
//
//		NO		 Persistent objects (saveable on disk)      
//		NO		 Uses exceptions
//
// Description:
//
//		The COXTaskbarIcon class encapsulates Win32 API's Shell_NotifyIcon()
//		function to let you easily show, hide, or change icons in Window 95's
//		or Windows NT's taskbar notification area. It is derived from the MFC
//		class CWnd, therefore you can derive a class from it to handle mouse
//		messages (thus hiding codes for taskbar icons from those in your main
//		program). Yet it doesn't abolish the convenience of handling mouse
//		messages within your CWnd classes, because you can always tell it to
//		route those messages to another CWnd object even at run-time.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __OXTASKBARICON_H__
#define __OXTASKBARICON_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"



/////////////////////////////////////////////////////////////////////////////
// COXTaskbarIcon window

class OX_CLASS_DECL COXTaskbarIcon : public CWnd
{
DECLARE_DYNAMIC(COXTaskbarIcon);
// Data members -------------------------------------------------------------
public:
protected:
	BOOL m_bShowing;
	UINT m_nTaskbarIconID;
private:

// Member functions ---------------------------------------------------------
public:
	COXTaskbarIcon();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Constructor of object

	virtual ~COXTaskbarIcon();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Destructor of object

	virtual BOOL Create(UINT nTaskbarIconID = 0);
	// --- In      : nTaskbarIconID, the identifier of a taskbar icon
	//					will be used to identify who sends a message when handling
	//					task bar icon's notifications. Therefore, when there is less
	//					than one taskbar icon to send notifications to your window,
	//					you don't need to specify an ID.
	// --- Out     : 
	// --- Returns : TRUE if successful or FALSE otherwise.
	// --- Effect  : create a taskbar icon (without showing it yet). has to be called
	//				 before calling other functions

	virtual void SetTooltipText(LPCTSTR lpszString, BOOL bRefresh = TRUE);
	virtual void SetTooltipText(UINT nIDResource, BOOL bRefresh = TRUE);
	// --- In      : lpszString,  a string to be used as tooltip text
	//				 nIDResource, a resource identifier of a string to be used as
	//							  tooltip text
	//				 bRefresh,    specify whether to instantly update the task icon
	//							  when TRUE, the taskbar icon will have the new tooltip
	//							  text immediately; when FALSE, the new text will be
	//							  used until next refresh/update. This parameter will
	//							  be neglected when the icon is not showing
	// --- Out     : 
	// --- Returns :
	// --- Effect  : set the tooltip text of a taskbar icon

	virtual HICON SetIcon(HICON hIcon, BOOL bRefresh = TRUE);
	virtual HICON SetIcon(UINT nIDResource, BOOL bRefresh = TRUE);
	virtual HICON SetIcon(LPCTSTR lpszResourceName, BOOL bRefresh = TRUE);
	// --- In      : hIcon, handle of an icon to be used as the icon of a taskbar icon
	//				 nIDResource, a resource identifier of an icon to be used as the
	//							  icon of a taskbar icon
	//				 lpszResourceName, a resource name of an icon to be used as the
	//							  icon of a taskbar icon
	//				 bRefresh,    specify whether to instantly update the task icon
	//							  when TRUE, the taskbar icon will have the new icon
	//							  immediately; when FALSE, the new icon will be
	//							  used until next refresh/update. This parameter will
	//							  be neglected when the icon is not showing
	// --- Out     : 
	// --- Returns : the previous icon that was used, if NULL, no icon was used before
	// --- Effect  : set the icon of a taskbar icon

	virtual BOOL Show();
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if successful or FALSE otherwise.
	// --- Effect  : Show the icon on taskbar

	virtual BOOL Hide();
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if successful or FALSE otherwise.
	// --- Effect  : Hide the icon on taskbar

	virtual BOOL Refresh();
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if successful or FALSE otherwise.
	// --- Effect  : Refresh the icon on taskbar (using current settings of tooltip texts
	//				 and icon).

	UINT GetID() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : the identifier of this taskbar icon
	// --- Effect  :

	virtual int GetTooltipText(LPTSTR lpszStringBuf, int nMaxCount) const;
	// --- In      : nMaxCount, specifies the maximum number of characters to be
	//							copied to the buffer. If the string is longer than
	//							the number of characters specified in nMaxCount,
	//							it is truncated.
	// --- Out     : lpszStringBuf, points to the buffer that is to receive the copied
	//								string of the tooltip text
	// --- Returns : specifies the length, in bytes, of the copied string, not 
	//				 including the terminating null character. it is 0 if it has no
	//				 tooltip text or if the tooltip text is empty.
	// --- Effect  : retrieve the tooltip text

	virtual void GetTooltipText(CString& rString) const;
	// --- In      :
	// --- Out     : rString, a CString object that is to receive the copied string
	//						  of the tooltip text
	// --- Returns :
	// --- Effect  : retrieve the tooltip text

	virtual HICON GetIcon() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : the handle of the currently using icon 
	// --- Effect  : retrieve the icon

	BOOL IsShowing() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if the icon is showing on the taskbar, FALSE otherwise
	// --- Effect  : get the status of this taskbar icon

	//{{AFX_VIRTUAL(COXTaskbarIcon)
	//}}AFX_VIRTUAL

protected:
	virtual BOOL ShellNotify(DWORD dwMessage);

	//{{AFX_MSG(COXTaskbarIcon)
	afx_msg LRESULT OnNotifyTaskbarIcon(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
};

/////////////////////////////////////////////////////////////////////////////

#include "OXTaskbarIcon.inl"

#endif	// __OXTASKBARICON_H__

// end of OXTaskbarIcon.h