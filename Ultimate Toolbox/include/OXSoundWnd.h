// ==========================================================================
// 					Class Specification : COXSoundWnd
// ==========================================================================

// Header file : OXSoundWnd.h

// Version: 9.3

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CWnd

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This is a helper window for the COXSound class. It should never be used directly.

// Remark:
//	The class is used to delay certain actions. this is done by posting a message to this window.
//	Only one object of this class should exist 

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXSOUNDWND_H__
#define __OXSOUNDWND_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


class OX_CLASS_DECL COXSoundWnd : public CWnd
{
DECLARE_DYNAMIC(COXSoundWnd);

// Data members -------------------------------------------------------------
public:
protected:
	static COXSoundWnd m_theSoundWnd;

private:
	
// Member functions ---------------------------------------------------------
public:
	COXSoundWnd();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	static HWND CreateTheSoundWindow();
	// --- In  :
	// --- Out : 
	// --- Returns : The one and only sound window
	// --- Effect : If it does not yet exist, creates the sound helper window

	static HWND GetTheSoundWindow();
	// --- In  :
	// --- Out : 
	// --- Returns : The one and only sound window
	// --- Effect : If it does not yet exist, NULL will be returned

#ifdef _DEBUG
	virtual void AssertValid() const;
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : AssertValid performs a validity check on this object 
	//				by checking its internal state. 
	//				In the Debug version of the library, AssertValid may assert and 
	//				thus terminate the program.
	
	virtual void Dump(CDumpContext& dc) const;
	// --- In  : dc : The diagnostic dump context for dumping, usually afxDump.
	// --- Out : 
	// --- Returns :
	// --- Effect : Dumps the contents of the object to a CDumpContext object. 
	//				It provides diagnostic services for yourself and 
	//				 other users of your class. 
	//				Note  The Dump function does not print a newline character
	//				 at the end of its output. 
#endif

	virtual ~COXSoundWnd();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

protected:
	//{{AFX_MSG(COXSoundWnd)
	LRESULT OnInternalSoundCallback(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
                   
};

#endif // __OXSOUNDWND_H__
// ==========================================================================
