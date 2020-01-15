// OXCoolControlOrganizer.h: interface for the COXCoolControlOrganizer class.
//
//////////////////////////////////////////////////////////////////////

// Version: 9.3

#if !defined(_OXCOOLCONTROLORGANIZER_H__)
#define _OXCOOLCONTROLORGANIZER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

#include "OXCoolComboBox.h"


#define EDIT_CLASSNAME				_T("Edit")
#define BUTTON_CLASSNAME			_T("Button")
#define SPIN_CLASSNAME				_T("msctls_updown32")
#define HOTKEY_CLASSNAME			_T("msctls_hotkey32")
#define COMBOBOX_CLASSNAME			_T("ComboBox")
#define SCROLLBAR_CLASSNAME			_T("ScrollBar")
#define PROGRESS_CLASSNAME			_T("msctls_progress32")
#define SLIDER_CLASSNAME			_T("msctls_trackbar32")
#define LISTVIEW_CLASSNAME			_T("SysListView32")
#define TREEVIEW_CLASSNAME			_T("SysTreeView32")
#define LISTBOX_CLASSNAME			_T("LISTBOX")
#define RICHEDIT_CLASSNAME			_T("RICHEDIT")
#if _MFC_VER > 0x0421
#define DATETIME_CLASSNAME			_T("SysDateTimePick32")
#define MONTHCAL_CLASSNAME			_T("SysMonthCal32")
#define IPADDRESS_CLASSNAME			_T("SysIPAddress32")
#endif	//	_MFC_VER > 0x0421

class OX_CLASS_DECL COXCoolControlOrganizer  
{
public:
	COXCoolControlOrganizer();
	virtual ~COXCoolControlOrganizer();

public:
	// map of all organizers that has been created to handle caption painting
	// for all windows created by particular thread
	static CMap<DWORD,DWORD,COXCoolControlOrganizer*,
		COXCoolControlOrganizer*> m_arrThreadOrganizers;

protected:
	// map that associate attached control with corresponding COXCoolCtrl object
	CMap<HWND,HWND,void*,void*> m_mapAttachedControls;

	// map of windows that wasn't qualified to be substituted with cool controls
	CMap<HWND,HWND,DWORD,DWORD> m_mapRejectedControls;

	// ID of the attached thread 
	DWORD m_dwThreadID;
	// handle of the old CBT hook procedure
	HHOOK m_pfnOldCBTHookProc;
	// handle of the original CBT hook procedure
	static HHOOK m_pfnOriginalCBTHookProc;

	// handle of the old GetMessage hook procedure
	HHOOK m_pfnOldGetMessageHookProc;
	// handle of the original GetMessage hook procedure
	static HHOOK m_pfnOriginalGetMessageHookProc;


public:
	BOOL Attach(HWND hWnd);


	BOOL Detach(HWND hWnd);


	// --- In  :	dwThreadID	-	thread ID which all windows will be attached
	//								to caption painter objects. By default we
	//								use the current process thread
	// --- Out : 
	// --- Returns:	TRUE if all windows created by the specified thread were 
	//				successfully hooked, otherwise FALSE
	// --- Effect : Creates COXCoolControlOrganizer objects for all windows created 
	//				by the specified thread. The windows 
	//				that will be created by the thread in the future will be 
	//				attached as soon as they will be created. Whenever a window
	//				is attached it uses the caption settings of the 
	//				mainframe window
	BOOL AttachAllInThread(DWORD dwThreadID=::GetCurrentThreadId());


	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if all windows were successfully unhooked, 
	//				otherwise FALSE
	// --- Effect : Detaches all previously attached windows that has been 
	//				created by the thread specified in AttachAllInThread()
	//				function that was called before
	BOOL DetachAllInThread();


	void AttachAllWindows(HWND hWndStartFrom);


	inline BOOL IsAttachedAllInThread() const { return (m_dwThreadID!=NULL); }
	inline DWORD GetAttachedThread() const { return m_dwThreadID; }
	inline HHOOK GetSavedGetMessageHookProc() const { 
		return m_pfnOldGetMessageHookProc; 
	}
	static HHOOK GetOriginalGetMessageHookProc() {
		return COXCoolControlOrganizer::m_pfnOriginalGetMessageHookProc; 
	}
	inline HHOOK GetSavedCBTHookProc() const { return m_pfnOldCBTHookProc; }
	static HHOOK GetOriginalCBTHookProc() {
		return COXCoolControlOrganizer::m_pfnOriginalCBTHookProc; 
	}


	// --- In  :	hWnd	-	handle to the window
	// --- Out : 
	// --- Returns:	TRUE if specified window was hooked before using 
	//				Attach function or FALSE otherwise
	// --- Effect : Retrieves the flag that specifies whether the given 
	//				window was attached before or not.
	BOOL IsAttached(const HWND hWnd) const;

	BOOL IsRejected(const HWND hWnd) const;

protected:
	virtual BOOL IsQualified(HWND hWnd, LPCTSTR lpszWndClass);
	virtual void* CreateCoolCtrl(HWND hWnd, LPCTSTR lpszWndClass);

	// hook procedure for CBT
	static LRESULT CALLBACK CoolControlOrganizerCBTHookProc(int nCode, 
		WPARAM wParam, LPARAM lParam);

	// hook procedure for CBT
	static LRESULT CALLBACK CoolControlOrganizerGetMessageHookProc(int nCode, 
		WPARAM wParam, LPARAM lParam);

	// callback for enumerating topmost windows
	static BOOL CALLBACK EnumThreadWindows(HWND hWnd, LPARAM lParam);

};

#endif // !defined(_OXCOOLCONTROLORGANIZER_H__)
