// ==========================================================================
// 					Class Specification : COXRegistryItem
// ==========================================================================

// Header file : OXRegistryItem.h

// Version: 9.3

          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CObject

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This class encapsulates a registry item : a registry key or a registry value.
//	A registry item can be specified in one string, much like a UNC for a file.
//	A registry key always begins and ends with a backslash (e.g. "\Software\)
//	A value name does not end in a backslash ("\Software\Test").
//  When no root registry is specified "CurrentUser" is used.  You can specify
//	 the root registry in front of the key ("\CurrentUser\Software\")
//	Possible values are "\ClassesRoot", "\CurrentUser", "\LocalMachine", "\Users", 
//	 "\PerformanceData", "\CurrentConfig" or "\DynData"
//	To address a remote computer prefix the registry item with the computer name
//	 (e.g. "\\Comp1\LocalMachine\Software\")

// Remark:

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXREGISTRYITEM_H__
#define __OXREGISTRYITEM_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


class OX_CLASS_DECL COXRegistryItem : public CObject
{
DECLARE_DYNAMIC(COXRegistryItem);

// Data members -------------------------------------------------------------
public:
	static const LPCTSTR m_pszClassesRoot;
	// --- The constant string "\ClassesRoot"
	static const LPCTSTR m_pszCurrentUser;
	// --- The constant string "\CurrentUser"
	static const LPCTSTR m_pszLocalMachine;
	// --- The constant string "\LocalMachine"
	static const LPCTSTR m_pszUsers;
	// --- The constant string "\Users"
	static const LPCTSTR m_pszPerformanceData;
	// --- The constant string "\PerformanceData"
	static const LPCTSTR m_pszCurrentConfig;
	// --- The constant string "\CurrentConfig"
	static const LPCTSTR m_pszDynData;
	// --- The constant string "\DynData"

	static const LPCTSTR m_pszNoName;
	// --- The constant string "NoName"

protected:
	CString	m_sFullRegistryItem;
	BOOL	m_bMainBuilt;

	CString m_sMachineName;
	CString m_sRegistryName;
	CString m_sKeyNames;
	CString m_sValueName;
	BOOL	m_bPartsBuilt;

	HKEY	m_hRootKey;
	HKEY	m_hKey;
	HRESULT	m_nLastError;

	static const TCHAR m_cNull;
	static const TCHAR m_cBackslash;
	static const TCHAR m_cBar;
	static const LPCTSTR m_pszBackslash;
	static const LPCTSTR m_pszDoubleBackslash;
	static const LPCTSTR m_pszBar;
	static const LPCTSTR m_pszSoftware;
	static LPCTSTR m_pszUnknownCompanyName;
	static LPCTSTR m_pszUnknownApplicationName;

	struct COXRootRegistryKey
		{
		LPCTSTR m_pszRegistryName;
		HKEY	m_hRegistryKey;
		};
	static COXRootRegistryKey m_rgRootRegistryKey[];

private:
	
// Member functions ---------------------------------------------------------
public:
	COXRegistryItem(LPCTSTR pszFullRegistryItem = NULL);
	// --- In  : pszFullRegistryItem : Initial full registry item
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	COXRegistryItem(const COXRegistryItem& registryItem);
	// --- In  : registryItem : source object
	// --- Out : 
	// --- Returns :
	// --- Effect : Copy constructor
	//				Note that possible open handles are not copied

	COXRegistryItem& operator=(const COXRegistryItem& registryItem);
	// --- In  : registryItem : source object
	// --- Out : 
	// --- Returns :
	// --- Effect : Assignment operator
	//				Note that possible open handles are not copied

	
	// Access functions


	CString GetFullRegistryItem();
	CString GetMachineName();
	CString GetRegistryName();
	CString GetKeyNames();
	CString GetValueName();
	// --- In  : 
	// --- Out : 
	// --- Returns : The requested part
	// --- Effect : 

	void SetFullRegistryItem(LPCTSTR pszFullRegistryItem);
	void SetMachineName(LPCTSTR pszMachineName);
	void SetRegistryName(LPCTSTR pszRegistryName);
	void SetKeyNames(LPCTSTR pszKeyNames);
	void SetValueName(LPCTSTR pszValueName);
	// --- In  : ...
	// --- Out : 
	// --- Returns : 
	// --- Effect : Sets the requested part
	//				Use "NoName" (m_pszNoName) to reference a value with a name

	void InitializeFromApplication(LPCTSTR pszSubkeyName = NULL);
	// --- In  : pszSubkeyName : The name of a subkey (NULL if no subkey will be used)
	// --- Out : 
	// --- Returns : 
	// --- Effect : Initializes this object to
	//              "\Software\<pszCompany>\<pszApplication>\<pszSubkeyName>\"
	//				with <pszCompany> : AfxGetApp()->m_pszRegistryKey
	//					 <pszApplication> : AfxGetApp()->m_pszProfileName
	//					 <pszSubkeyName> : The specified subkey (may be empty)
	//				The company name can be set through CWinApp::SetRegistryKey()
	//				 and then the application will be filled out as well
	//				 (by derivng it from the executable name)

	void Empty();
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : Clears the entire objects (and closes a possible open key)

	BOOL IsEmpty();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether this registry specification is empty 
	// --- Effect : 

	virtual void Serialize(CArchive& ar);
	// --- In  : ar : Archive used in serialization
	// --- Out : 
	// --- Returns :
	// --- Effect : Serializes the object

	HRESULT GetLastError() const;
	// --- In  :
	// --- Out : 
	// --- Returns : The last error code
	// --- Effect : 

	void SetLastError(HRESULT nLastError);
	// --- In  : nLastError : The new last error code
	// --- Out : 
	// --- Returns : 
	// --- Effect : 


	// Actions


	BOOL Open(BOOL bCreate = TRUE, REGSAM samDesired = KEY_ALL_ACCESS,
		LPCTSTR pszClass = NULL, DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, LPDWORD pdwDisposition = NULL);
	// --- In  : bCreate : Whether to create the key if it does not exist
	//			 samDesired : Desired security access 
	//				(Possible values are KEY_ALL_ACCESS, KEY_CREATE_LINK, KEY_CREATE_SUB_KEY,
	//				KEY_ENUMERATE_SUB_KEYS, KEY_EXECUTE, KEY_NOTIFY, KEY_QUERY_VALUE, KEY_READ,
	//				KEY_SET_VALUE, KEY_WRITE)
	//			 pszClass : Class name
	//			 dwOptions : Spzcial options 
	//				(Possible values are REG_OPTION_NON_VOLATILE, REG_OPTION_VOLATILE,
	//				REG_OPTION_BACKUP_RESTORE);
	//			 lpSecurityAttributes = Security structure
	//			 pdwDisposition : Pointer to value receiving the disposition value 
	//				(REG_CREATED_NEW_KEY or REG_OPENED_EXISTING_KEY)
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Opens (and if requested creates) a registry key

	BOOL IsOpen() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it key is currently opnened
	// --- Effect : 

 	BOOL Delete();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Deletes this registry item (key or value)
	//				Values without a name should have the name "NoName" (m_pszNoName)

	BOOL GetSecurity(SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor, LPDWORD pcbSecurityDescriptor);
	// --- In  : SecurityInformation : The requested security information
	//			 pSecurityDescriptor : Buffer that will receive the requested info
	//			 pcbSecurityDescriptor : Length of the buffer
	// --- Out : pSecurityDescriptor : Buffer that received the requested info
	//			 pcbSecurityDescriptor : Number of bytes written in the buffer
	//									 or needed size when failed with ERROR_INSUFFICIENT_BUFFER
	// --- Returns : Whether it succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Retrieves a copy of the security descriptor protecting this registry key. 

	BOOL SetSecurity(SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor);
	// --- In  : SecurityInformation : The new security information
	//			 pSecurityDescriptor : The security descriptor to set
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Sets the security descriptor protecting this registry key. 

 	BOOL Flush();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Writes all changes to the key to registry now
	//				The function only returns after all the changes have been written

	BOOL Close();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Closes the registry key (if still open)


	// Values

	CString GetStringValue(LPCTSTR pszValueName = NULL, BOOL bAllowUnexpanded = TRUE, 
		BOOL bAutoExpand = TRUE, CString sDefault = _T(""));
	// --- In  : pszValueName : (Optional) The name of the value
	//			 bAllowUnexpanded : Whether the type REG_EXPAND_SZ is allowed as well
	//			 bAutoExpand : Whether the string should expand REG_EXPAND_SZ values
	//			 sDefault : default value that will be returned in a case registry 
	//						item wasn't found
	// --- Out : 
	// --- Returns : The string value (or an empty value in case of error)
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Get a string value
	//				When no value name is specified, the value name of this object is used

	CString GetMultiStringValue(LPCTSTR pszValueName = NULL, CString sDefault = _T(""));
	// --- In  : pszValueName : (Optional) The name of the value
	//			 sDefault : default value that will be returned in a case registry 
	//						item wasn't found
	// --- Out : 
	// --- Returns : The multi string value (or an empty value in case of error)
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Get a multi-string value
	//				The different parts are seperated by a bar '|'
	//				When no value name is specified, the value name of this object is used

	BOOL GetMultiStringValue(CStringArray& stringArray, LPCTSTR pszValueName = NULL);
	// --- In  : pszValueName : (Optional) The name of the value
	// --- Out : stringArray
	// --- Returns : Whether the operation succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Get a multi-string value
	//				The array is emptied prior to adding new values
	//				When no value name is specified, the value name of this object is used

	DWORD GetNumberValue(LPCTSTR pszValueName = NULL, BOOL bLittleEndian = TRUE, 
		DWORD dwDefault = 0);
	// --- In  : pszValueName : (Optional) The name of the value
	//			 bLittleEndian : Whether the type of the numebr should be Little Endian
	//				(Intel) or Big Endian 
	//			 dwDefault	:	default value that will be returned in a case registry 
	//							item wasn't found
	// --- Out : 
	// --- Returns : The number value (or 0 in case of error)
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Get a number value
	//				When no value name is specified, the value name of this object is used

	BOOL GetBinaryValue(CByteArray& binaryValue, LPCTSTR pszValueName = NULL);
	// --- In  : pszValueName : (Optional) The name of the value
	// --- Out : binaryValue
	// --- Returns : Whether the operation succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Get a binary value
	//				The array is emptied prior to adding new values
	//				When no value name is specified, the value name of this object is used

	BOOL SetStringValue(LPCTSTR pszNewStringValue, LPCTSTR pszValueName = NULL, BOOL bUnexpanded = FALSE);
	// --- In  : pszNewStringValue : The new value
	//			 pszValueName : (Optional) The name of the value
	//			 bUnexpanded : Whether the value is a normal REG_SZ value or not
	// --- Out : 
	// --- Returns : Whether the operation succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Sets a new string value
	//				When no value name is specified, the value name of this object is used

	BOOL SetMultiStringValue(LPCTSTR pszNewMultiStringValue, LPCTSTR pszValueName = NULL);
	// --- In  : pszNewMultiStringValue : The new value
	//			 pszValueName : (Optional) The name of the value
	// --- Out : 
	// --- Returns : Whether the operation succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Sets a new multi string value
	//				The different parts should be seperated by a bar '|'
	//				When no value name is specified, the value name of this object is used

	BOOL SetMultiStringValue(const CStringArray& stringArray, LPCTSTR pszValueName = NULL);
	// --- In  : stringArray : The new value
	//			 pszValueName : (Optional) The name of the value
	// --- Out : 
	// --- Returns : Whether the operation succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Sets a new multi string value
	//				When no value name is specified, the value name of this object is used

	BOOL SetNumberValue(DWORD nNewNumberValue, LPCTSTR pszValueName = NULL, BOOL bLittleEndian = TRUE);
	// --- In  : nNewNumberValue : The new value
	//			 pszValueName : (Optional) The name of the value
	//			 bLittleEndian : Whether the type of the numebr should be Little Endian
	//				(Intel) or Big Endian 
	// --- Out : 
	// --- Returns : Whether the operation succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Sets a new number value
	//				When no value name is specified, the value name of this object is used

	BOOL SetBinaryValue(const CByteArray& newBinaryValue, LPCTSTR pszValueName = NULL);
	// --- In  : newBinaryValue : The new value
	//			 pszValueName : (Optional) The name of the value
	// --- Out : 
	// --- Returns : Whether the operation succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Sets a new binary value
	//				When no value name is specified, the value name of this object is used

	DWORD GetValueSize(LPCTSTR pszValueName = NULL);
	// --- In  : pszValueName : (Optional) The name of the value
	// --- Out : 
	// --- Returns : The size of the value (or 0 when failed)
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Gets the size of a value
	//				When no value name is specified, the value name of this object is used

	DWORD GetValueType(LPCTSTR pszValueName = NULL);
	// --- In  : pszValueName : (Optional) The name of the value
	// --- Out : 
	// --- Returns : The type of the value (or REG_NONE when failed)
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Gets the type of a value
	//				When no value name is specified, the value name of this object is used
	//				Possible types are :
	//				 REG_NONE, REG_SZ, REG_EXPAND_SZ, REG_BINARY, REG_DWORD_LITTLE_ENDIAN
	//				 (=REG_DWORD), REG_DWORD_BIG_ENDIAN, REG_LINK, REG_MULTI_SZ, 
	//				 REG_RESOURCE_LIST

	BOOL GetValue(LPBYTE* ppData, LPCTSTR pszValueName = NULL, DWORD nRequestedType = 0, 
		LPDWORD pnType = NULL, LPDWORD pnSize = NULL);
	// --- In  : ppData : Pointer to LPBYTE that will receive the address of the data
	//			 pszValueName : (Optional) The name of the value
	//			 nRequestedType : The type of the value that it must have
	//				for this function to succeed (0 = all types allowed)
	//			pnType : Pointer to a variable that will get the type
	//			pnSize  : Pointer to a variable that will get the size in bytes
	// --- Out : ppData : Points to LPBYTE that points to the data  if successful
	//			 pnType : The type of the value if successful
	//			 pnSize  : The size in bytes if successful
	// --- Returns : Whether the operation succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Get a generic value
	//				The caller should call delete[] to destroy the returned byte buffer
	//				When no value name is specified, the value name of this object is used

	BOOL SetValue(LPCTSTR pszValueName, const BYTE* pData, DWORD nType, DWORD nSize);
	// --- In  : pszValueName : (Optional) The name of the value
	//			 pData : The new value (should not be NULL)
	//			 nType : The type
	//			 nSize : The size of pData in bytes
	// --- Out : 
	// --- Returns : Whether the operation succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Sets a generic value
	//				When no value name is specified, the value name of this object is used

	static CString ExpandString(LPCTSTR pszUnexpanded);
	// --- In  : pszUnexpanded : The unexpanded string containing references to
	//				environment variables (e.g. %path%)
	// --- Out : 
	// --- Returns : The expanded string value or empty when the function failed
	// --- Effect : Expands environment-variable strings and 
	//			    replaces them with their defined values. 

	BOOL GetInfo(LPTSTR pClass, LPDWORD pcbClass = NULL, LPDWORD pcSubKeys = NULL,
		LPDWORD pcbMaxSubKeyLen = NULL, LPDWORD pcbMaxClassLen = NULL, 
		LPDWORD pcValues = NULL, LPDWORD pcbMaxValueNameLen = NULL, 
		LPDWORD pcbMaxValueLen = NULL, LPDWORD pcbSecurityDescriptor = NULL,
		PFILETIME lpftLastWriteTime = NULL);
	// --- In  : pClass : Pointer to string that will receive the class name
	//		     pcbClass : Pointer to number that specifies the number of characters in pClass
	//			 pcSubKeys : Pointer to variable that will receive a number
	//			 pcbMaxSubKeyLen : Pointer to variable that will receive a number
	//			 pcbMaxClassLen : Pointer to variable that will receive a number
	//			 pcValues : Pointer to variable that will receive a number
	//			 pcbMaxValueNameLen : Pointer to variable that will receive a number
	//			 pcbMaxValueLen : Pointer to variable that will receive a number
	//			 pcbSecurityDescriptor : Pointer to variable that will receive a number
	//			 lpftLastWriteTime : Pointer to variable that will receive the last write time
	// --- Out : pClass : The class name of the key
	//		     pcbClass : The number of characters in the class name (exclusing termination null)
	//			 pcSubKeys : Number of subkeys
	//			 pcbMaxSubKeyLen : Longest subkey name length
	//			 pcbMaxClassLen : Longest class name length
	//			 pcValues : Number of values
	//			 pcbMaxValueNameLen : Longest valua name length
	//			 pcbMaxValueLen : Longest value
	//			 pcbSecurityDescriptor : The security descriptor length
	//			 lpftLastWriteTime : The last write time
	// --- Returns : Whether the operation succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Retrieves information about the key
	//				For information that is not needed NULL may be specified

	CString GetClassName();
	int GetNumberOfSubkeys();
	int GetLongestSubkeyNameLength();
	int GetLongestClassNameLength();
	int GetNumberOfValues();
	int GetLongestValueNameLength();
	int GetLongestValueDataLength();
	int GetSecurityDescriptorLength();
	CTime GetLastWriteTime();
	// --- In  : 
	// --- Out : 
	// --- Returns : The requested value (or an empty string or -1 in case of error)
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : 


	// Notification


	BOOL NotifyChange(BOOL bWatchSubtree = FALSE, 
		DWORD dwNotifyFilter = REG_NOTIFY_CHANGE_NAME, HANDLE hEvent = NULL);	
	// --- In  : bWatchSubtree : Whether to report changes in this key and all of 
	//							 its subkeys (TRUE) or only in this key (FALSE)
	//			 dwNotifyFilter : The changes which should be reported (REG_NOTIFY_CHANGE_NAME, 
	//					REG_NOTIFY_CHANGE_ATTRIBUTES, REG_NOTIFY_CHANGE_LAST_SET, 
	//					REG_NOTIFY_CHANGE_SECURITY)
	//			 hEvent : Event used in asynchronous signalling or NULL otherwise
	// --- Out : 
	// --- Returns : Whether the operation succeeded or not
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Notifies the caller about changes to the attributes or contents 
	//				of the key. 
	//				Note that the function does not notify the caller if the key is deleted.
	//				When this function is used synchrnously (hEvent == NULL), the function
	//				 will only return when the specified situation occurs

 
	// Key handles


	HKEY GetRootKey() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : The current value of the root key
	// --- Effect : This should not be stored for later use, because subsequent functions
	//              may change (e.g. Close) this handle

	HKEY GetKey() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : The current value of the registry key
	// --- Effect : This should not be stored for later use, because subsequent functions
	//              may change (e.g. Close) this handle


	// Enumeration
	// (Note : Enumeration is much simplier when you use COXIteratorRegistryItem)

	CString EnumerateSubkey(DWORD nIndex);
	// --- In  : nIndex : Zero-based index of the subkey that is reuqested
	// --- Out : 
	// --- Returns : The name of the subkey (if successful, otherwise empty)
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Retrieves the name of the specified subkey
	//				It is advised to use COXIteratorRegistryItem for enumeration

	CString EnumerateValue(DWORD nIndex);
	// --- In  : nIndex : Zero-based index of the value that is reuqested
	// --- Out : 
	// --- Returns : The name of the value (if successful)
	//				 See GetLastError() for more detailed error code upon failure
	// --- Effect : Retrieves the name of the specified value
	//				It is advised to use COXIteratorRegistryItem for enumeration
	//				Values without a name are given the name "NoName" (m_pszNoName)

	BOOL GetSortedValueList(CStringArray& arrValueName);
	// --- In  : arrValueName : reference to string array that will be populated 
	//							value names in alphabetical order
	// --- Out : 
	// --- Returns:	TRUE if successful, or FALSE otherwise (See GetLastError() 
	//				for more detailed error code upon failure)
	// --- Effect : Retrieves the names of all key values in alphabetical order.
	//				Values without a name are given the name "NoName" (m_pszNoName)
	//				and will be put on the top of the array

	BOOL IsDefaultValueSet();
	// --- In  : 
	// --- Out : 
	// --- Returns:	TRUE if default value of current key is set 
	//				or FALSE otherwise (if successful).
	//				See GetLastError() for more detailed error code upon failure
	// --- Effect : Retrieves the flag that ppecifies whether current key default
	//				value is set or not.


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

	virtual ~COXRegistryItem();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

protected:
	void BuildMain();
	void BuildParts();
	void AdjustMain();
	void AdjustParts();

	void SetMainBuilt();
	void SetPartsBuilt();
	void DestroyMain();
	void DestroyParts();
	BOOL IsMainBuilt() const;
	BOOL ArePartsBuilt() const;

	HRESULT HResultFromWin32(int nWin32Error) const;

	BOOL OpenRootKey();
	void CloseKey();
	void CloseRootKey();

	static int GetRegistryIndex(LPCTSTR pszRegistryName);
	static void NullToBarSeparator(LPTSTR pStringData);
	static void BarToNullSeparator(LPTSTR pStringData);

#ifdef _DEBUG
	static CString GetResultMessage(HRESULT hResult);
#endif // _DEBUG

private:
                   
};

// Include inline functions
#include "OXRegistryItem.inl"

#endif // __OXREGISTRYITEM_H__
// ==========================================================================
