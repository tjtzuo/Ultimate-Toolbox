// ==========================================================================
// 					Class Specification : COXRegistryValFile
// ==========================================================================

// Header file : OXRegistryValFile.h

// Version: 9.3

// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CMemFile

//	NO	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Open())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	YES	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//  This class can be used to use MFCs file access and archive functions to
//  write to a binary registry value.

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXREGISTRYVALFILE_H__
#define __OXREGISTRYVALFILE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


class OX_CLASS_DECL COXRegistryValFile : public CMemFile
{
	DECLARE_DYNAMIC(COXRegistryValFile)

// Data members -------------------------------------------------------------

public:

protected:
	HKEY m_key;			// stores handle to open registry key
	CString m_value;	// stores the registry value's name

private:

// Member functions ---------------------------------------------------------

public:
// Constructors
	COXRegistryValFile();
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the COXRegistryValFile object. Use this constructor 
	//              for two step construction. You must call the Open method to 
	//              fully initialize the COXRegistryValFile object.

	COXRegistryValFile(HKEY hkey, LPCTSTR lpszKey, LPCTSTR lpszValue);
	// --- In  : hkey : One of the following predefined reserved handle values: 
	//                  HKEY_CLASSES_ROOT, HKEY_CURRENT_CONFIG, HKEY_CURRENT_USER,
	//                  HKEY_LOCAL_MACHINE, HKEY_USERS,
	//                  Windows NT only: HKEY_PERFORMANCE_DATA ,
	//                  Windows 95 only: HKEY_DYN_DATA 
	//           lpszKey : Points to a null-terminated string specifying the name 
	//                     of a key that this function opens or creates. This key 
	//                     must be a subkey of the key identified by the hKey 
	//                     parameter. 
	//           lpszValue : Points to a null-terminated string containing the name 
	//                       of the value to be queried.
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the COXRegistryValFile object. Use this constructor 
	//              for single step construction. This constructor creates a fully
	//              usable open file.
	//              If the file/registry could not be opened a CFileException is
	//              thrown.

// Attributes

// Operations
	BOOL Open(HKEY hkey, LPCTSTR lpszKey, LPCTSTR lpszValue, LONG& error);
	// --- In  : hkey : One of the following predefined reserved handle values: 
	//                  HKEY_CLASSES_ROOT, HKEY_CURRENT_CONFIG, HKEY_CURRENT_USER,
	//                  HKEY_LOCAL_MACHINE, HKEY_USERS,
	//                  Windows NT only: HKEY_PERFORMANCE_DATA ,
	//                  Windows 95 only: HKEY_DYN_DATA 
	//           lpszKey : Points to a null-terminated string specifying the name 
	//                     of a key that this function opens or creates. This key 
	//                     must be a subkey of the key identified by the hKey 
	//                     parameter. 
	//           lpszValue : Points to a null-terminated string containing the name 
	//                       of the value to be queried.
	// --- Out : error : A pointer to an existing LONG value that will receive the
	//                   status of a failed operation. If the function fails, the 
	//                   return value is either a nonzero error code defined in 
	//                   WINERROR.H. You can then use the FormatMessage function 
	//                   with the FORMAT_MESSAGE_FROM_SYSTEM flag to get a generic 
	//                   description of the error. Alternatively this value may be
	//                   0, which means some other unspecified error, such as using
	//                   an existing registry value that is not binary or a low
	//                   memory condition that causes the funciton to fail.
	// --- Returns : TRUE if the file/registry could be opened, FALSE otherwise.
	//               the error contains 
	// --- Effect : Initializes the COXRegistryValFile object. Use this function 
	//              after the two-step constructor  for final initialization. This 
	//              method renders the file object fully usable.
	void Flush();
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Forces any data remaining in the file buffer to be written to 
	//              the file/registry. 
	//              The use of Flush does not guarantee flushing of CArchive buffers. 
	//              If you are using an archive, call CArchive::Flush first. 
	void Close();
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Closes the file/registry associated with this object and makes the 
	//              file/registry unavailable for reading or writing. If you have not 
	//              closed the file/registry before destroying the object, the destructor 
	//              closes it for you. 
	void Abort();
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Closes the file/registry associated with this object and makes the 
	//              file/registry unavailable for reading or writing. If you have not 
	//              closed the file/registry before destroying the object, the destructor 
	//              closes it for you. 
	//              When handling exceptions, COXRegistryValFile::Abort differs from 
	//              COXRegistryValFile::Close in one important way: Abort will not ASSERT 
	//              if the file/registry has not been opened or was closed previously.

// Implementation
public:
	virtual ~COXRegistryValFile();
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Destroys the COXRegistryValFile object. If the file/registry
	//              value is still open, any data that is not yet written is flushed
	//              to the file/registry and the file/registry is closed.
};

#endif // __OXREGISTRYVALFILE_H__
