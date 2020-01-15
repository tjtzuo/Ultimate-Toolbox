// OXBrowseDirEdit.cpp : implementation file
//

// Version: 9.3


#include "stdafx.h"
#include "OXBrowseDirEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXBrowseDirEdit

IMPLEMENT_DYNCREATE(COXBrowseDirEdit, COXEdit)

CString COXBrowseDirEdit::GetPath() const 
{ 
	ASSERT(::IsWindow(GetSafeHwnd()));
	return GetInputData(); 
}

void COXBrowseDirEdit::SetPath(LPCTSTR lpszPath) 
{ 
	ASSERT(::IsWindow(GetSafeHwnd()));
	
	EmptyData(TRUE);
	SetInputData(lpszPath);
	UpdateInsertionPointForward(0);
}


/////////////////////////////////////////////////////////////////////////////
// COXBrowseDirEdit16

CString COXBrowseDirEdit16::GetPath() const 
{ 
	ASSERT(::IsWindow(GetSafeHwnd()));
	return GetInputData(); 
}

void COXBrowseDirEdit16::SetPath(LPCTSTR lpszPath) 
{ 
	ASSERT(::IsWindow(GetSafeHwnd()));
	
	EmptyData(TRUE);
	SetInputData(lpszPath);
	UpdateInsertionPointForward(0);
}


/////////////////////////////////////////////////////////////////////////////
// COXBrowseFileEdit

IMPLEMENT_DYNCREATE(COXBrowseFileEdit, COXEdit)

CString COXBrowseFileEdit::GetPath() const 
{ 
	ASSERT(::IsWindow(GetSafeHwnd()));
	return GetInputData(); 
}

void COXBrowseFileEdit::SetPath(LPCTSTR lpszPath) 
{ 
	ASSERT(::IsWindow(GetSafeHwnd()));
	
	EmptyData(TRUE);
	SetInputData(lpszPath);
	UpdateInsertionPointForward(0);
}
