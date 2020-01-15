// ==========================================================================
//						Class Implementation : 
//						COXHyperLinkAction
// ==========================================================================

// Version: 9.3

// Implementation file : OXHyperLinkAction.cpp

#include "StdAfx.h"
#include "OXHyperLinkAction.h"
#include "UTB64Bit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --- Effect : sets all COXHyperLinkAction properties to 
//				their default value
void COXHyperLinkAction::Reset()
{	
	// By default the window associated with the object is undefined.
	// Set it using SetOwnerWnd function, you probably will need to 
	// use this parameter in the handler of your user defined action
	m_hWndOwner=NULL;

	// action is "Open" by default
	m_nActionID=ID_HLACTION_OPEN;
	m_sFile=(LPCTSTR)NULL;
	m_sParameters=(LPCTSTR)NULL;
	m_sDefaultDirectory=(LPCTSTR)NULL;
	// if any application was launched the application window would be 
	// shown using SW_SHOWNORMAL style
	m_nShowCmd=SW_SHOWNORMAL;

	m_nCallbackMsgID=NULL;
	m_hWndRecipient=NULL;

	m_nError=ID_HLERROR_SUCCESS;
	m_hLaunchedApp=NULL;
}

// --- Effect : sets all COXHyperLinkAction properties using one function
void COXHyperLinkAction::Set(int nActionID, LPCTSTR sFile, LPCTSTR sParameters, 
							 LPCTSTR sDefaultDirectory, int nShowCmd, 
							 UINT nCallbackMsgID, HWND hWndRecipient)
{	
	m_nActionID=nActionID;

	m_sFile=sFile;
	m_sParameters=sParameters;
	m_sDefaultDirectory=sDefaultDirectory;
	m_nShowCmd=nShowCmd;

	m_nCallbackMsgID=nCallbackMsgID;
	m_hWndRecipient=hWndRecipient;
}

void COXHyperLinkAction::Set(COXHyperLinkAction& hla)
{	
	*this=hla;
}

// copy constructor
COXHyperLinkAction& COXHyperLinkAction::operator=(const COXHyperLinkAction& hla)
{
	if(this==&hla)
		return *this;
		
	m_hWndOwner=hla.m_hWndOwner;

	m_nActionID=hla.m_nActionID;

	m_sFile=hla.m_sFile;
	m_sParameters=hla.m_sParameters;
	m_sDefaultDirectory=hla.m_sDefaultDirectory;
	m_nShowCmd=hla.m_nShowCmd;

	m_nCallbackMsgID=hla.m_nCallbackMsgID;
	m_hWndRecipient=hla.m_hWndRecipient;

	m_nError=hla.m_nError;
	m_hLaunchedApp=hla.m_hLaunchedApp;

	return *this;
}

// Executes action using ShellExecute function or sending callback message to 
// recipient window. If execution fails and bErrorNotify is TRUE
// then corresponding error message will be displayed
// using ErrorNotify function which is defined as virtual
// just in case you would like more sophisticated error notification
//
BOOL COXHyperLinkAction::ExecuteIt(BOOL bErrorNotify)
{
	BOOL bResult=TRUE;
	COXHyperLinkAction hlaCopy=*this;

	m_hLaunchedApp=NULL;

	CString sAction;
	switch(m_nActionID)
	{
	case ID_HLACTION_NONE:
		{
			m_nError=ID_HLERROR_SUCCESS;
			return TRUE;
		}
	case ID_HLACTION_USERDEFINED:
		{
			// if action is userdefined then we just send m_nCallbackMsgID
			// message to m_hWndRecipient window. Handler function in recipient
			// window have to return 1 if success and 0 otherwise
			if(m_hWndRecipient==HWND_BROADCAST)
				bResult=(BOOL)::PostMessage(m_hWndRecipient,m_nCallbackMsgID,
					(WPARAM)0,(LPARAM)this);
			else
				bResult=(BOOL)::SendMessage(m_hWndRecipient,m_nCallbackMsgID,
					(WPARAM)0,(LPARAM)this);
			if(bResult)
			{
				m_nError=ID_HLERROR_SUCCESS;
				return TRUE;
			}
			else
			{
				TRACE(_T("COXHyperLinkAction::ExecuteIt: unable to execute\n"));
				m_nError=ID_HLERROR_ACTIONFAILED;
			}
			break;
		}
	// if Action is not userdefined then eventually we call ShellExecute function 
	// to execute the Action. If action is Open or Print or Explore everything is
	// pretty staightforward
	// !!!!!!!!!!!!!!!!!!
	// if you want to open a website using browser then you can use 
	// ID_HLACTION_OPEN action with m_sFile set to the address of the 
	// given website
	// !!!!!!!!!!!!!!!!!!
	case ID_HLACTION_OPEN:
		{
			sAction.LoadString(IDS_OX_HLINKACTIONOPEN);
			break;
		}
	case ID_HLACTION_PRINT:
		{
			sAction.LoadString(IDS_OX_HLINKACTIONPRINT);
			break;
		}
	case ID_HLACTION_EXPLORE:
		{
			sAction.LoadString(IDS_OX_HLINKACTIONEXPLORE);
			break;
		}
	case ID_HLACTION_EMAIL:
		{
			// !!!!!!!!!!!!!!!!!!
			// this action is designed to use with Internet Mail or Exchange or
			// Outlook
			// !!!!!!!!!!!!!!!!!!
			// if action is ID_HLACTION_EMAIL then we have to do some preparations
			sAction.LoadString(IDS_OX_HLINKACTIONOPEN);
			CString sTemp;
			//sTemp.Format(IDS_OX_HLINKACTIONMAILTO,m_sFile);//"mailto:" 
			sTemp.LoadString(IDS_OX_HLINKACTIONMAILTO);
			m_sFile = sTemp + m_sFile;
			break;
		}
	default:
		{
			bResult=FALSE;
			m_nError=ID_HLERROR_ACTIONUNKNOWN;
			break;
		}
	}

	if(bResult)
	{
		// eventually we call ShellExecute function to do all dirty job
		m_hLaunchedApp=ShellExecute(NULL, sAction, m_sFile, m_sParameters,
			m_sDefaultDirectory, m_nShowCmd);

		// if succeed (return value is more than 32), ShellExecute will 
		// return handle of the launched application
		if((UINT_PTR)m_hLaunchedApp<=32)
		{
			// if don't succeed, m_hLaunchedApp is code of error
			bResult=FALSE;
			m_nError=PtrToUint((UINT_PTR)m_hLaunchedApp);
			TRACE(_T("COXHyperLinkAction::ExecuteIt: unable to execute\n"));
			m_hLaunchedApp=NULL;
		}
	}

	// we could have changed some properties of the object so we have to reset them
	hlaCopy.m_nError=m_nError;
	hlaCopy.m_hLaunchedApp=m_hLaunchedApp;
	*this=hlaCopy;

	if(!bResult)
	{
		if(bErrorNotify)
		{
			// use inner function to notify about the failed action
			ErrorNotify();
		}
	}
	else
	{
		m_nError=ID_HLERROR_SUCCESS;
	}

	return bResult;
}

void COXHyperLinkAction::ErrorNotify()
{
	if(m_nError==ID_HLERROR_SUCCESS)
	{
		return;
	}

	// unable to execute the action!
	MessageBeep(0);		
	CString sErrorMsg;
	VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERRORUNKNOWN)); //"Unknown error!"

	switch(m_nError)
	{
	case ID_HLERROR_ACTIONUNKNOWN:
		{
			VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERRORACTIONUNKNOWN));//"Failed to execute unknown action!"
			break;
		}
	case ID_HLERROR_ACTIONFAILED:
		{
			CString sAction;
			VERIFY(sAction.LoadString(m_szAction[m_nActionID]));
			sErrorMsg.Format(IDS_OX_HLINKERRORACTIONFAILED, //"%s action failed!"
				sAction);
			break;
		}
	// ShellExecute error messages
	case 0:
		{
			VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERROROUTMEMORY));//"The operating system is out\nof memory or resources."
			break;
		}
	case SE_ERR_PNF:
		{
			VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERRORPATHNOTFOUND)); //"The specified path was not found."
			break;
		}
	case SE_ERR_FNF: 
		{
			VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERRORFILENOTFOUND)); //"The specified file was not found."
			break;
		}
	case ERROR_BAD_FORMAT:
        {
			VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERROREXENOTVALID)); //"The .EXE file is invalid\n(non-Win32 .EXE or error in .EXE image)."
			break;
		}
	case SE_ERR_ACCESSDENIED: 
		{
			VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERRORACCESSDENIED));//"The operating system denied\naccess to the specified file."
			break;
		}
	case SE_ERR_ASSOCINCOMPLETE:
		{
			VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERRORASSOCINCOMPLETE)); //"The filename association is\nincomplete or invalid."
			break;
		}
	case SE_ERR_DDEBUSY:
		{
			VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERRORDDEBUSY));// "The DDE transaction could not\nbe completed because other DDE transactions\nwere being processed."
			break;
		}
	case SE_ERR_DDEFAIL: 
		{
			VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERRORDDEFAILED));	//"The DDE transaction failed."
			break;
		}
	case SE_ERR_DDETIMEOUT:
		{
			VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERRORDDETIMEOUT)); //"The DDE transaction could not\nbe completed because the request timed out."
			break;
		}
	case SE_ERR_DLLNOTFOUND:
		{
			VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERRORDLLNOTFOUND));	//"The specified dynamic-link library was not found."
			break;
		}
	case SE_ERR_NOASSOC:
		{
			VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERRORNOTASSOC)); //"There is no application associated\nwith the given filename extension."
			break;
		}
	case SE_ERR_OOM: 
		{
			VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERROROOM));	//"There was not enough memory to coplete the operation."
			break;
		}
	case SE_ERR_SHARE:
		{
			VERIFY(sErrorMsg.LoadString(IDS_OX_HLINKERRORSHARE)); //"A sharing violation occurred."
			break;
		}
	}

	AfxMessageBox(sErrorMsg,MB_ICONSTOP);
}



