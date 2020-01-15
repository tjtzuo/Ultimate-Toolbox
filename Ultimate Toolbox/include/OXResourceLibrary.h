// ==========================================================================
// 					Class Specification : COXResourceLibrary
// ==========================================================================
// Header file :		OXResourceLibrary.h
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
//		COXResourceLibrary wraps Win32 API's resource functions (together with
//		COXResourceFile). One object of COXResourceLibrary corresponds to a
//		library file (an executable: .DLL or .EXE file), whereas COXResoruceFile
//		corresponds to one resource in a library.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __OXRESOURCELIBRARY_H__
#define __OXRESOURCELIBRARY_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#define OXRESOURCE_DEFLANGID MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL)

#ifdef _UNICODE
#define OX_LPCTSTR2UL wcstoul
#else
#define OX_LPCTSTR2UL strtoul
#endif


/////////////////////////////////////////////////////////////////////////////
class COXResourceFile;
class OX_CLASS_DECL COXResourceLibrary : public CObject
{
	DECLARE_DYNAMIC(COXResourceLibrary)

// Data members -------------------------------------------------------------
public:
protected:
	CString		m_sFileName;
	DWORD		m_dwFlags;
	HANDLE		m_hUpdateRes;
	CObList		m_ResFiles;

private:

// Member functions ---------------------------------------------------------
public:
	COXResourceLibrary();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Constructor of object

	virtual ~COXResourceLibrary();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Destructor of object (will call Close() if opened)

	virtual BOOL Open(LPCTSTR pszLibFileName, DWORD dwFlags = LOAD_LIBRARY_AS_DATAFILE);
	// --- In      : pszLibFileName, path name of a library file to open
	//				 dwFlags, an action to take when loading a library module, can be
	//					one of the following values (see Win32 LoadLibraryEx()'s 
	//					documentation for details):
	//
	//					LOAD_LIBRARY_AS_DATAFILE, 
	//					DONT_RESOLVE_DLL_REFERENCES, or
	//					LOAD_WITH_ALTERED_SEARCH_PATH.
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : open/load a library (.DLL or .EXE file)
	//				 NOTE: if Open() fails, or it's not called, all subsequent resource
	//				 operation will be performed on the current module

	virtual void Close();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : close/free the opened library
	//				 (destructor or Open() will call this if necessary)
	//				 NOTE: no Commit() will be called.

	CString GetFileName() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : the path name of the opened library file; empty string if using
	//				 current module (Open() was not called or failed).
	// --- Effect  : retrieve the library's file name

	BOOL IsModifiable();
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if the library is updatable; FALSE otherwise (always FALSE in 
	//				 Win95)
	// --- Effect  : test if we can get write access to the library file
	//				 CAUTION: do NOT call this function repeatedly (e.g. in an UpdateCmdUI
	//				 handler) if BeginUpdate() was not called. The memory it takes up 
	//				 during the checking may not be released until Close() due to current
	//				 Win32 implementation.

	HRSRC FindResource(LPCTSTR pszType, LPCTSTR pszName, 
		WORD nLanguage = OXRESOURCE_DEFLANGID) const;
	// --- In      : pszType, the type of the resource to find
	//				 pszName, the name of the resource to find
	//				 nLanguage, the language of the resource to find
	// --- Out     : 
	// --- Returns : the handle of the found resource; NULL if not found
	// --- Effect  : retrieve the handle of a resource to use in GetResourceSize() or
	//				 LoadResource().
	//				 NOTE: it doesn't search registered COXResourceFile objects

	DWORD GetResourceSize(HRSRC hRes) const;
	// --- In      : hRes, the handle of the resource from FindResource()
	// --- Out     : 
	// --- Returns : the size in bytes of a resource
	// --- Effect  : get the size of a resource

	HGLOBAL LoadResource(HRSRC hRes) const;
	// --- In      : hRes, the handle of the resource from FindResource()
	// --- Out     : 
	// --- Returns : the memory handle of loaded resource binary data
	// --- Effect  : load a resource into memory (you may call ::LockResource()
	//				 subsequently).

	COXResourceFile* GetOpenedResFile(LPCTSTR pszType, LPCTSTR pszName, 
		WORD nLanguage = OXRESOURCE_DEFLANGID) const;
	// --- In      : pszType, the type of the resource to find
	//				 pszName, the name of the resource to find
	//				 nLanguage, the language of the resource to find
	// --- Out     : 
	// --- Returns : pointer to a registered COXResourceFile object
	// --- Effect  : find a resource in all registered COXResourceFile objects

	virtual BOOL Commit(BOOL bDeleteExistingResources = FALSE);
	// --- In      : bDeleteExistingResources, specify whether to delete all existing
	//				 resources in the library file before updating
	// --- Out     :
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : flush all registered COXResourceFile objects and commit all changes
	//				 by writing them back to the library file

	virtual BOOL DeleteResource(LPCTSTR pszType, LPCTSTR pszName,
		WORD nLanguage = OXRESOURCE_DEFLANGID, BOOL bInstant = FALSE);
	// --- In      : pszType, the type of the resource to delete
	//				 pszName, the name of the resource to delete
	//				 nLanguage, the language of the resource to delete
	//				 bInstant, specify whether to delete it from the library file instantly
	//					by writing the change into the file; neglected if BeginUpdate() was
	//					called (always FALSE in this case).
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : delete a resource

	BOOL BeginUpdate(BOOL bDeleteExistingResources = FALSE);
	// --- In      : bDeleteExistingResources, specify whether to delete all existing
	//				 resources in the library file before updating
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : begin a multiple updates session

	BOOL Update(BYTE* lpBuffer, DWORD dwSize, LPCTSTR pszType, LPCTSTR pszName,
		WORD nLanguage = OXRESOURCE_DEFLANGID);
	// --- In      : lpBuffer, points to the first byte of the binary resource data
	//				 dwSize, specify the size of a resource
	//				 pszType, the type of the resource to update
	//				 pszName, the name of the resource to update
	//				 nLanguage, the language of the resource to update
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : add a resource using a block of binary data (BeginUpdate() has to
	//				 be called already). if there is a resource in the library with the
	//				 same type, name and language, it will be replaced.

	BOOL EndUpdate(BOOL bFlushAll = FALSE, BOOL bDiscard = FALSE);
	// --- In      : bFlushAll, flush all registered COXResourceFile objects before ending
	//					updates.
	//				 bDiscard, discard all changes (if TRUE, bFlushAll will be neglected)
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : end a multiple updates session (either write all changes to 
	//				 the library file, or discard all changes).

	BOOL EnumResources(CStringArray* pResTypes, CStringArray* pResNames, 
						CWordArray* pResLangs = NULL, int nGrowBy = -1);
	// --- In      : pResTypes, pointer to a string array to store types of resources
	//					cannot be NULL
	//				 pResNames, pointer to a string array to store names of resources
	//					if NULL, only types will be enumerated (will neglect pResLangs)
	//				 pResLangs, pointer to a word array to store langauges of resources
	//					if NULL, no langauges will be enumerated
	//				 nGrowBy, specify the size to grow if neccessary for the above three
	//					arrays
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : enumerate all resources in the opened library file, and store types, 
	//				 names and langauges into the three arrays (the resulted arrays will
	//				 have the same size if successful; when fails, you may check sizes
	//				 and last elements of the arrays to find the stop point)
	//				 NOTE: resulted types and names will use CString form

	BOOL EnumResources(LPCTSTR lpszType, CStringArray* pResNames,
						CWordArray* pResLangs = NULL, int nGrowBy = -1);
	// --- In      : lpszType, type of resources to enumerate
	//				 pResNames, pointer to a string array to store names of resources
	//					cannot be NULL
	//				 pResLangs, pointer to a word array to store langauges of resources
	//					if NULL, no langauges will be enumerated
	//				 nGrowBy, specify the size to grow if neccessary for the above two
	//					arrays
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : enumerate all resources with a specified type in the opened library
	//				 file, and store their names and langauges into arrays (the resulted
	//				 arrays will have the same size if successful; when fails, you may 
	//				 check their sizes and last elements to find the stop point)
	//				 NOTE: resulted names will use CString form

	BOOL EnumResources(LPCTSTR lpszType, LPCTSTR lpszName,
						CWordArray* pResLangs, int nGrowBy = -1);
	// --- In      : lpszType, type of resources to enumerate
	//				 lpszName, name of resources to enumerate
	//				 pResLangs, pointer to a word array to store langauges of resources
	//				 nGrowBy, specify the size to grow if neccessary for pResLangs
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : enumerate all resources with specified type and name in the opened
	//				 library file, and store their langauges into an array

	HINSTANCE GetLibraryHandle() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : the module handle of the loaded library (NULL if current module)
	// --- Effect  : retrieve the handle of the loaded library

	HANDLE GetUpdateHandle() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : the update handle (NULL if BeginUpdate() was not called)
	// --- Effect  : retrieve the update handle
	HINSTANCE	m_hLib;

protected:
	virtual void Register(COXResourceFile* pResFile);
	virtual void Unregister(COXResourceFile* pResFile);
	virtual void FlushAll();
private:

	friend class COXResourceFile;	// for calling Register() and Unregister()
};

/////////////////////////////////////////////////////////////////////////////

#include "OXResourceLibrary.inl"

#endif	// __OXRESOURCELIBRARY_H__

// end of OXResourceLibrary.h