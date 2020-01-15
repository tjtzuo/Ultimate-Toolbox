// ==========================================================================
// 					Class Specification : COXRegistryWatchNotifier
// ==========================================================================

// Header File : OXRegistryWatchNotifier.h

// Version: 9.3

// //////////////////////////////////////////////////////////////////////////

// Properties :
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CObject

//	NO	Is a Cwnd.
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Description :         
//	This class encapsulates the notification parameters.
//		When you specify a Registry key to watch for and window to receive notification message,
//		you can specify some additional parameters for this watch: type of changes to be reported,
//		whether or not subkeys should be watched as well, etc. All these parameters are encapsulated
//		in COXRegistryWatchNotifier class that can be used for its retrieving.

// Example :
//		COXRegistryWatchNotifier	RegWatchNotifier;
//		...
//		::RegNotifyChangeKeyValue(RegWatchNotifier.GetRegKey(), RegWatchNotifier.GetWatchSubtree(),
//								  RegWatchNotifier.GetWatchFilter(), (HANDLE)RegWatchNotifier.GetEvent());

// Remark :
//	COXRegistryWatcher class has a "watch queue" of COXRegistryWatchNotifier objects. Each of these
//	notifiers has its own unique ID, that can be retrieved by COXRegistryWatchNotifier::GetWatchID().

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef	__OXREGISTRYWATCHNOTIFIER_H__
#define	__OXREGISTRYWATCHNOTIFIER_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


#include <afxmt.h>


class OX_CLASS_DECL COXRegistryWatchNotifier : public CObject
{
	friend class COXRegistryWatcher;

// Data members -------------------------------------------------------------
public:

protected:
	BOOL		m_bPost;
	BOOL		m_bWatchSubtree;
	DWORD		m_dwNotifyFilter;
	DWORD		m_dwID;
	HKEY		m_hWatchKey;
	HWND		m_hWndDst;
	CEvent*		m_phWatchEvent;
	CTime		m_tNotificationTime;

private:

// Member functions ---------------------------------------------------------
public:
	COXRegistryWatchNotifier();
	// --- In :
	// --- Out :
	// --- Returns :
	// --- Effect :		Constructs the COXRegistryWatchNotifier object and initializes members.

	CTime GetNotificationTime() const;
	// --- In :
	// --- Out :
	// --- Returns :	Notification time.
	// --- Effect :		Returns the time when the notification was received.

	HKEY GetRegKey() const;
	// --- In :
	// --- Out :
	// --- Returns :	Registry key handle.
	// --- Effect :		Returns the original key specification that is being watch.

	BOOL GetWatchSubtree() const;
	// --- In :
	// --- Out :
	// --- Returns :	Subtrees watch flag.
	// --- Effect :		Returns whether subtrees are being watched.

	DWORD GetWatchFilter() const;
	// --- In :
	// --- Out :
	// --- Returns :	Watch filter - combination of flags that control which changes should be
	//					reported. Flags are described in the "Data members - public:" section
	//					of COXRegistryWatcher Class Specification.
	// --- Effect :		Returns the watch filter that is being used.

	DWORD GetWatchID() const;
	// --- In :
	// --- Out :
	// --- Returns :	Watch notifier ID (>0), or 0 - if this notifier is not initialized
	//					in "watch queue" of COXRegistryWatcher.
	// --- Effect :		Returns the watch notifier ID.

	CEvent* GetEvent() const;
	// --- In :
	// --- Out :
	// --- Returns :	Pointer to CEvent object that represents notification event.
	// --- Effect :		Retrieves the Registry notification event. When Registry key is changed,
	//					this event is signaled.

	CWnd* GetWndDst() const;
	// --- In :
	// --- Out :
	// --- Returns :	Pointer to CWnd object that represents receiving window.
	// --- Effect :		Returns the window that receives the notification message.

	BOOL GetPost() const;
	// --- In :
	// --- Out :
	// --- Returns :	If TRUE, notification message is posted to a window, if FALSE, message sent.
	// --- Effect :		Returns whether notification message posted or sent to a window.

	COXRegistryWatchNotifier& operator=(const COXRegistryWatchNotifier& RegistryWatchNotifier);
	// --- In :			RegistryWatchNotifier : source object.
	// --- Out :
	// --- Returns :
	// --- Effect :		Assignment operator.

protected:
	void SetMembers(HKEY hWatchKey, BOOL bWatchSubtree, DWORD dwNotifyFilter, CEvent* phWatchEvent, DWORD dwID);
	void SetNotificationTime();
	void SetPost(BOOL bPost);
	void SetWndDst(CWnd* pWndDst);

private:

};

// Include inline functions
#include "OXRegistryWatchNotifier.inl"

#endif	// __OXREGISTRYWATCHNOTIFIER_H__
// //////////////////////////////////////////////////////////////////////////
