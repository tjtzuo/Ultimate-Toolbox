// ==========================================================================
// 					Class Specification : COXResourceFile
// ==========================================================================
// Header file :		OXResourceFile.h
//
// Version: 9.3
//
// Properties:
//		NO		 Abstract class (does not have any objects)
//		YES		 Derived from CObject
//
//		NO		 Is a CWnd.                     
//		NO		 Two stage creation (constructor & Create())
//		NO		 Has a message map
//		NO		 Needs a resource (template)
//
//		NO		 Persistent objects (saveable on disk)      
//		NO		 Uses exceptions
//
// Description:
//		COXResourceFile provides a resource with a CSharedFile (thus CFile)
//		interface. One can read or write any type (e.g. a GIF-picture, a movie 
//		etc. as in RT_RCDATA) of resources directly through familiar Read(), 
//		Write(), Seek(), Flush(), Close(), etc.. The resource will be loaded
//		into memory and wrapped by a COXResourceFile. All changes will be applied 
//		to the resource in memory and will be committed when the file is flushed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __OXRESOURCEFILE_H__
#define __OXRESOURCEFILE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include <afxpriv.h>
#include "OXResourceLibrary.h"


/////////////////////////////////////////////////////////////////////////////
class OX_CLASS_DECL COXResourceFile : public CSharedFile
{
	DECLARE_DYNAMIC(COXResourceFile)

// Data members -------------------------------------------------------------
public:
protected:
	COXResourceLibrary* m_pResLib;
	BOOL	m_bAutoDeleteByLib;
	BOOL	m_bFlushOnClose;
	CString m_sResType;
	CString m_sResName;
	WORD	m_nResLanguage;

private:

// Member functions ---------------------------------------------------------
public:
	COXResourceFile(UINT nAllocFlags = GMEM_DDESHARE | GMEM_MOVEABLE, 
		UINT nGrowBytes = 4096);
	// --- In      : nAllocFlags, specifies how to allocate memory (see Win32
	//					GlobalAlloc() for details)
	//				 nGrowBytes, (same as in CMemFile)
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Constructor of object

	virtual ~COXResourceFile();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Destructor of object

	virtual BOOL Open(UINT nOpenFlags, COXResourceLibrary* pResLib, BOOL bAutoDeleteByLib, 
		LPCTSTR pszType, LPCTSTR pszName, WORD nLanguage = OXRESOURCE_DEFLANGID,
		BOOL bMakeUniqueName = FALSE);
	virtual BOOL Open(UINT nOpenFlags, COXResourceLibrary* pResLib, BOOL bAutoDeleteByLib,
		LPCTSTR pszType, UINT nID, WORD nLanguage = OXRESOURCE_DEFLANGID,
		BOOL bMakeUniqueName = FALSE);
	// --- In      : nOpenFlags, specifies the action to take when opening the resource,
	//					can be a combination of the following values:
	//					CFile::modeReadWrite   open an existing resource, will call Flush() on 
	//										   Close() or destruction
	//					CFile::modeRead		   open an existing resource, will not call Flush()
	//										   on Close() or destruction (Flush() is disabled)
	//					CFile::modeCreate	   create a new resource, if the resource exists already,
	//										   it is truncated to 0 length.
	//					CFile::modeNoTruncate  create a new resource, (Combine this value with modeCreate)
	//										   If the resource exists already, it is not truncated to 0 length, 
	//										   thus the resource is guaranteed to open, either as a newly 
	//										   created resource or as an existing resource.
	//				 pResLib, the library from which this resource is opening
	//					(or to which this resource will be updated, if creating a new
	//					resource)
	//				 bAutoDeleteByLib, whether let the library to delete this object
	//					when the library is closed
	//				 pszType, the type of the resource to open
	//				 pszName, the name of the resource to open
	//				 nID, the integer ID of the resource to open
	//				 nLanguage, the language  of the resource to open
	//				 bMakeUniqueName, specify whether to make sure the name of this
	//					resource is not duplicated (with another resource, in the library
	//					file and its registered COXResourceFile objects, of the same type
	//					and language). neglected when opening an existing resource
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : load an existing resource into memory from a library, or create a
	//				 new empty resource

	virtual void Close();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : flush the contents, close the resource and release memory
	//				 (destructor will call this if necessary)

	virtual void Abort();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : close the resource and release memory without flushing the contents

	virtual void Flush();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : write changes back to the library file
	//				 (if COXResourceLibrary::BeginUpdate() was called, the change will
	//				 be made until COXResourceLibrary::EndUpdate() is called; otherwise
	//				 the change will be written back to the library file immediately.)

	virtual HGLOBAL DetachEx();
	// --- In      :
	// --- Out     : 
	// --- Returns : the handle of the global memory that was tagged with type, name and
	//				 language block
	// --- Effect  : take over the memory that contains all resource binary data plus
	//				 type, name and language tag. Called in clipboard/OLE operations

	virtual BOOL SetHandleEx(HGLOBAL hTaggedResData, BOOL bAllowGrow = TRUE);
	// --- In      : hTaggedResData, the handle of the memory that was returned by 
	//					DetachEx()
	//				 bAllowGrow, specify whether the memory can be reallocated
	//					(same as in CShareFile::SetHandle())
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : restore resource binary data and its type, name and language from a
	//				 tagged memory block. Called in clipboard/OLE operations

	virtual BOOL SetResType(LPCTSTR pszType);
	// --- In      : pszType, the resource type to set
	// --- Out     : 
	// --- Returns : (not used)
	// --- Effect  : set the type of a resource (not necessarily already opened)

	virtual BOOL SetResName(LPCTSTR pszName);
	virtual BOOL SetResName(WORD nID);
	// --- In      : pszName, the resource name to set
	//				 nID, the ID of the resource to set
	// --- Out     : 
	// --- Returns : (not used)
	// --- Effect  : set the name of a resource (not necessarily already opened)

	virtual BOOL SetResLanguage(WORD nLanguage);
	// --- In      : nLanguage, the resource language to set
	// --- Out     : 
	// --- Returns : (not used)
	// --- Effect  : set the langauge of a resource (not necessarily already opened)

	virtual BOOL SetResourceLibrary(COXResourceLibrary* pResLib, 
		BOOL bAutoDeleteByLib = FALSE, BOOL bMakeUniqueName = FALSE);
	// --- In      : pResLib, the new target library to set
	//				 bAutoDeleteByLib, whether let the library to delete this object
	//					when the library is closed
	//				 bMakeUniqueName, specify whether to make sure the name of this
	//					resource is not duplicated (with another resource, in the library
	//					file and its registered COXResourceFile objects, of the same type
	//					and language).
	// --- Out     :
	// --- Returns : (not used)
	// --- Effect  : set a new target (of updates) library (i.e. transferring)
	//				 (the library that was used in Open() is the default target library
	//				 when first opened)

	BOOL IsAutoDeleteByLib() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if it will be automatically deleted when the library (specified
	//				 when calling Open() or SetResourceLibrary()) is closed
	// --- Effect  : determine whether it was set to let the library to delete this
	//				 object when the library is closed

	CString GetResType() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : the type in CString format of the resource
	// --- Effect  : retrieve the type of the resource

	CString GetResName() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : the name in CString format of the resource
	// --- Effect  : retrieve the name of the resource

	WORD GetResLanguage() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : the language of the resource
	// --- Effect  : retrieve the language of the resource

	COXResourceLibrary* GetResourceLibrary() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : the library (specified when calling Open() or 
	//					SetResourceLibrary()) of the resource
	// --- Effect  : retrieve the target library

	// *** type/name format conversion functions ***
	//------------------------------------------------------------------------------
	// Name	    In	      Return		Explanation
	//------------------------------------------------------------------------------
	// CString  LPCTSTR   CString		the CString format this class (and COXResourceFile)
	//										uses: either #65535 or "string"
	// Item     LPCTSTR   LPCTSTR		the internal format Win32 used in resources'
	//										types and names
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Int 		WORD      WORD			the integer value you usually used with
	//										resource editor
	// String   LPCTSTR   CString		the resource name that you can use directly
	//										(not recommended by Win32 when storing
	//										resources, yet usable)
	//------------------------------------------------------------------------------
	// (1) OXRes...ToInt() or OXRes...ToString() may return 0 or empty string, 
	//	   respectively, when not convertable;
	//
	// (2) for these conversions:
	//		Int     --> String:		(n/a);
	//		String  --> Int:		(n/a);
	//      String  --> CString:	implicit or CString(LPCTSTR);
	//		Int     --> Item:		MAKEINTRESOURCE() macro;
	//		String  --> Item:		LPTSTR = LPCTSTR
	//
	// (3) for CString --> Item (rarely it is needed, because all member functions of
	//	   COXResourceLibrary and COXResourceFile accept any formats including CString,
	//	   and all Win32 resource functions that needs item format are wrapped):
	//	   write something like this:
	//
	//			WORD nID = COXResourceFile::OXResCStringToInt(pszTypeOrName);
	//			LPCTSTR pszItemTypeOrName = nID ? MAKEINTRESOURCE(nID) : pszTypeOrName;

	static BOOL		OXResCStringIsInt	(LPCTSTR lpszCStringTypeOrName);
	static BOOL		OXResItemIsInt		(LPCTSTR lpszTypeOrName);

	static WORD		OXResCStringToInt	(LPCTSTR lpszCStringTypeOrName);
	static CString	OXResCStringToString(LPCTSTR lpszCStringTypeOrName);
	static CString	OXResItemToCString	(LPCTSTR lpszItemTypeOrName);
	static WORD		OXResItemToInt		(LPCTSTR lpszItemTypeOrName);
	static CString	OXResItemToString	(LPCTSTR lpszItemTypeOrName);
	static CString	OXResIntToCString	(WORD nID);

	static WORD		OXResToInt			(LPCTSTR lpszTypeOrName); // takes any formats
	static CString	OXResToCString		(LPCTSTR lpszTypeOrName); // takes any formats

protected:
	virtual BOOL MakeUniqueName(COXResourceLibrary* pSearchLibrary = NULL);
	static CString ValidateResString(LPCTSTR pszTypeOrName);

private:
};

/////////////////////////////////////////////////////////////////////////////

#include "OXResourceFile.inl"

#endif	// __OXRESOURCEFILE_H__

// end of OXResourceFile.h