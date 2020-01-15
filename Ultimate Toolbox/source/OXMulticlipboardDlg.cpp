// OXMulticlipboardDlg.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXMulticlipboardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef __AFXOLE_H__
#error "You must #include <afxole.h> in stdafx.h Make sure you have called AfxOleInit() in InitInstance()"

#endif 

TCHAR szTips[15][32]=
{
	_T("CF_NONE"),
	_T("CF_TEXT"),
	_T("CF_BITMAP"),
	_T("CF_METAFILEPICT"),
	_T("CF_SYLK"),
	_T("CF_DIF"),
	_T("CF_OEMTEXT"),
	_T("CF_DIB"),
	_T("CF_PALETTE"),
	_T("CF_PENDATA"),
	_T("CF_RIFF"),
	_T("CF_WAVE"),
	_T("CF_UNICODETEXT"),
	_T("CF_ENHMETAFILE"),
	_T("CF_CUSTOMFORMAT")

};


/////////////////////////////////////////////////////////////////////////////
// CMulticlipboardDlg dialog

IMPLEMENT_DYNCREATE(COXMulticlipboardDlg,CDialog);

COXMulticlipboardDlg::COXMulticlipboardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COXMulticlipboardDlg::IDD, pParent),
	m_nIconSpace(OX_CLIP_ICON_SPACE),
	m_hWndNext(NULL), m_nMaxPocketsCount(OX_CLIP_MAX_POCKETS),
	m_nPocketsInRow(OX_CLIP_POCKETS_IN_ROW), m_bReplacePockets(TRUE),
	m_nCurrentPocket(-1), m_nIconHeight(OX_CLIP_ICON_WIDTH),
	m_nIconWidth(OX_CLIP_ICON_WIDTH), m_nEdgeOffset(OX_CLIP_EDGE_OFFSET),
	m_nStaticDimension(OX_CLIP_STATIC_DIMENSION),
	m_bPaste(FALSE)
{
}


void COXMulticlipboardDlg::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(COXMulticlipboardDlg)
	//}}AFX_DATA_MAP(COXMulticlipboardDlg)
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COXMulticlipboardDlg, CDialog)
	//{{AFX_MSG_MAP(COXMulticlipboardDlg)
	ON_WM_DESTROY()
	ON_WM_CHANGECBCHAIN()
	ON_WM_DRAWCLIPBOARD()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMulticlipboardDlg message handlers

BOOL COXMulticlipboardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//register COXMulticlipboardDlg in order 
	//to receive notifications messages
	m_hWndNext=::SetClipboardViewer(m_hWnd);
	
	return TRUE;  
}





void COXMulticlipboardDlg::OnDestroy() 
{

	//clear collection of the pockets
	for (int n=0;n<m_pPockets.GetSize();n++)
	{
		COXClipPocket* pPocket=(COXClipPocket*) m_pPockets.GetAt(n);
		pPocket->DestroyWindow();
		delete pPocket;
	}
	m_pPockets.RemoveAll();


	//remove 'this' from the chain of clipboard vieweres  
	::ChangeClipboardChain(m_hWnd, m_hWndNext);
	
	//call parent
	CDialog::OnDestroy();
	
	
}

void COXMulticlipboardDlg::OnChangeCbChain(HWND hWndRemove, HWND hWndAfter) 
{
	CDialog::OnChangeCbChain(hWndRemove, hWndAfter);
	
	//if the window we currently passing 
	//notification messages changes, update 
	//our handle
	if (hWndRemove==m_hWndNext)
		m_hWndNext=hWndRemove;

	//pass message to the next window
	if (m_hWndNext)
		::SendMessage(m_hWndNext,WM_DRAWCLIPBOARD, NULL, NULL);
	
}

void COXMulticlipboardDlg::OnDrawClipboard() 
{
	//we've got notification
	CDialog::OnDrawClipboard();
	
	//if we does put the data to the clipboard,
	//m_bPaste will be TRUE
	// do nothing in this case
	if (!m_bPaste)
	{
		//get a pocket for the data
		COXClipPocket* pPocket=GetNextPocket();

		if (pPocket)
		{
			//put data to the pocket and call virtual
			//function OnClipInfoChanged()
			ClipToPocket(pPocket);
			OnClipInfoChanged(GetCurrentPocket());
		}
	}

	//pass to the next window
	if (m_hWndNext)
		::SendMessage(m_hWndNext,WM_DRAWCLIPBOARD, NULL, NULL);
	
}

int COXMulticlipboardDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//try to create tooltip control
	if (!m_tip.Create(this))
	{
		TRACE0("COXMulticlipboardDlg::OnCreate() failed to create COXToolTipCtrl\r\n");
	}

	//load icons for different data types
	if (!LoadImageList())
	{
		TRACE0("COXMulticlipboardDlg::OnCreate() failed to create image list\r\n");
		return -1;
	}

	return 0;
}




COXClipPocket* COXMulticlipboardDlg::GetNextPocket()
{

	//this function does much work -
	//it returns pocket to put clipboard info
	//in case no pockets available it will add
	//one row of pockets and return first available
	COXClipPocket* pPocket=NULL;

	m_nCurrentPocket++;

	//we've reached max count of pockets
	//avalables, so go to the first one
	//if m_bReplacePockets permit us to do it
	if (m_nCurrentPocket==m_nMaxPocketsCount)
	{
		if (m_bReplacePockets && m_pPockets.GetSize())
		{
			m_nCurrentPocket=0;
			pPocket=(COXClipPocket*) m_pPockets.GetAt(m_nCurrentPocket);
			ASSERT(::IsWindow(pPocket->GetSafeHwnd()));
		}
		return pPocket;
	}

	//determine existence of the requested pocket
	//if exists, return this pocket
	if (m_nCurrentPocket<m_pPockets.GetSize())
	{
		pPocket=(COXClipPocket*) m_pPockets.GetAt(m_nCurrentPocket);
		ASSERT(::IsWindow(pPocket->GetSafeHwnd()));
			return pPocket;
	}

	//no luck to find pocket, have to create
	//new pockets row

	//get number of last pocket we will create
	int nLastPocket=(m_nCurrentPocket/m_nPocketsInRow+1)*4-1;
	nLastPocket=min(m_nMaxPocketsCount-1,nLastPocket);
	
	//calculate rect
	//if there is no place to create new pockets return NULL
	int nMaxPockets=AdjustSize(nLastPocket);

	if (!nMaxPockets)
		return NULL;
	nLastPocket=nMaxPockets-1;

	if (m_nCurrentPocket>nLastPocket)
	{
		if (m_bReplacePockets)
		{
			m_nCurrentPocket=0;
			pPocket=(COXClipPocket*) m_pPockets.GetAt(m_nCurrentPocket);
			ASSERT(::IsWindow(pPocket->GetSafeHwnd()));
		}
		else
			m_nCurrentPocket=nLastPocket;
		return pPocket;
	}


	if (m_pPockets.GetSize() &&
		m_pPockets.GetUpperBound()>=m_nCurrentPocket-1)
		pPocket=(COXClipPocket*) m_pPockets.GetAt(m_nCurrentPocket-1);

	for (int n=m_nCurrentPocket;n<=nLastPocket;n++)
	{
		CRect rect;
		COXClipPocket* pPocketsRow;

		//no more pockets can be created
		VERIFY(GetProposedRect(n, rect));
			
		pPocketsRow=new COXClipPocket(this);
		m_pPockets.Add(pPocketsRow);
		VERIFY(pPocketsRow->Create(rect.TopLeft(), m_nCurrentPocket+OX_CLIP_FIRST_ID));
		
		//this pocket (the first in the row) we have to return
		if (n==m_nCurrentPocket)
			pPocket=pPocketsRow;
	}
	return pPocket;

}

BOOL COXMulticlipboardDlg::GetProposedRect(UINT nNumber, CRect &rect)
{
	CRect rectDialog;
	CRect rectClient;

	//the function calculates the placement for the
	//specified pocket
	//this placement does not included edge offset
	//(borders of the pocket), just size of the icon
	//because COXClipPocket::Create() will increase
	//this rect on edge offset itself.
	GetClientRect(&rectClient);	

	rect.left=GetIconSpace()+GetStaticDimension()+
		GetEdgeOffset()+(GetIconWidth()+2*GetEdgeOffset()+
		GetIconSpace())*(nNumber%m_nPocketsInRow);
	rect.right=rect.left+GetIconWidth();
	rect.top=GetButtonsHeight()+GetIconSpace()+GetStaticDimension()+
		+GetEdgeOffset()+
		(GetIconSpace()+GetIconHeight()+GetEdgeOffset()*2)*
		(nNumber/m_nPocketsInRow);
	
	rect.bottom=rect.top+GetIconHeight();

	return TRUE;

}



BOOL COXMulticlipboardDlg::CanResize()
{
	//override it and return FALSE 
	//in case of docked non-sizeable state
	return TRUE;
}

int COXMulticlipboardDlg::AdjustSize(UINT nLast)
{
	CRect rectDialog;
	CRect rectClient;
	CRect rectDesired;
	CRect rectStatic;
	CRect rect;

	GetClientRect(&rectClient);	
	GetWindowRect(&rectDialog);

	//calculate desired rect for specified number
	//of the pockets
	CalculateDesiredRect(nLast+1, rectDesired);

	if (rectDesired==rectClient)
	{
		//it has the same size,
		//just make sure the pockets are
		//placed properly
		MovePockets(nLast);
		MoveStatic();
		GetClientRect(&rect);
		InvalidateRect(&rect);
		return nLast+1;
	}

	//well, desired size is different from what we have
	//can we resize the dialog?
	if (CanResize())
	{
		CRect rectOffset;
		rectOffset=rectDesired;
		rectOffset-=rectClient;
		rectDialog+=rectOffset;
	
		//move dialog to approved position
		MoveWindow(&rectDialog);

		//calculate and move static to a new position
		MoveStatic();
		//move buttons to new positions
		MovePockets(nLast);

		GetClientRect(&rect);
		InvalidateRect(&rect);
		return nLast+1;
	}

	//no, cannot resize
	MoveStatic();
	
	//calculate number of the pockets currently can be placed
	//on this dialog
	int n=0;
	for (n=nLast;n>=0;n--)
	{
		GetProposedRect(n,rect);
		rect.InflateRect(GetEdgeOffset(),GetEdgeOffset());
		if (rectClient.PtInRect(rect.BottomRight())
			&& rectClient.PtInRect(rect.TopLeft()))
			break;
	}
	MovePockets(nLast);
	GetClientRect(&rect);
	InvalidateRect(&rect);

	return n+1;
}

UINT COXMulticlipboardDlg::GetButtonsHeight()
{
	//override this function and provide 
	//height of your buttons if you have placed 
	//any at the top of the dialog
	return NULL;
}


void COXMulticlipboardDlg::OnClipInfoChanged(UINT nPocketNumber)
{

	//this function sets the tips for the pockets 
	//and caption text for the dialog
	//you can override it

	ASSERT(nPocketNumber<(UINT) m_pPockets.GetSize());
	COXClipPocket* pPocket=(COXClipPocket*) m_pPockets.GetAt(nPocketNumber);
	ASSERT(pPocket);

	UINT nFormat=pPocket->GetDataType();
	
	//get clipboard format name
	TCHAR szBuffer[_MAX_PATH];
	if (nFormat>=OX_CLIP_CUSTOM_FORMAT)
		::GetClipboardFormatName(nFormat,(LPTSTR) &szBuffer,_MAX_PATH-1);
	else
		lstrcpy(szBuffer,szTips[nFormat]);

	if (::IsWindow(m_tip.m_hWnd))
	{
		m_tip.DelTool(pPocket);
		VERIFY(m_tip.AddTool(pPocket,szBuffer));
	}

	//set caption text
	CString sText;
	sText.Format(IDD_OX_MULTICLIPBOARDDLG_POCKET,
		m_nCurrentPocket+1,m_nMaxPocketsCount);
	SetWindowText((LPCTSTR) sText);

}

BOOL COXMulticlipboardDlg::PreTranslateMessage(MSG* pMsg) 
{
	//handle tips
	m_tip.RelayEvent(pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL COXMulticlipboardDlg::LoadImageList()
{
	//virtual function - override it to provide your
	//own image list
	return m_imgList.Create(IDB_OX_BITMAP_LIST,GetIconWidth(),2,(COLORREF) 0xFF00FF);

}

void COXMulticlipboardDlg::CalculateDesiredRect(int nPockets, CRect &rect)
{
	//the function calculates desired rect
	//for certain number of the pockets

	nPockets=nPockets?nPockets:1;

	UINT nRows=nPockets/m_nPocketsInRow;
	nRows+=(nPockets%m_nPocketsInRow)?1:0;
	UINT nCols=min(m_nPocketsInRow,nPockets);

	rect.left=rect.top=0;
	rect.right=GetIconSpace()+2*GetStaticDimension()+
		nCols*(GetIconWidth()+
		GetIconSpace()+2*GetEdgeOffset());

	rect.bottom=GetButtonsHeight()+GetIconSpace()+
		2*GetStaticDimension()+nRows*
		(GetIconHeight()+GetIconSpace()+2*GetEdgeOffset());

}



void COXMulticlipboardDlg::RemovePocket(int nPocket)
{
	//the function remove pocket from 
	//the collection, actually destroying 
	//the window and deleting COXClipPocket object 
	
	if (nPocket>m_pPockets.GetUpperBound())
		return;
	
	COXClipPocket* pPocket=(COXClipPocket*) m_pPockets.GetAt(nPocket);

	if (m_pPockets.GetUpperBound())
	{
		pPocket->DestroyWindow();
		delete pPocket;
		m_pPockets.RemoveAt(nPocket);
		if (m_nCurrentPocket>=nPocket)
			m_nCurrentPocket--;
	}
	else
		pPocket->SetData(NULL,NULL);

	//one pocket less - we have to adjust ths size
	AdjustSize(PtrToUint(m_pPockets.GetUpperBound()));

}

void COXMulticlipboardDlg::MovePockets(int nLast)
{
	CRect rect;
	CRect rectDesired;

	//calculate proper position for
	//the pockets and move them if nessesary
	for (int n=0;n<= nLast;n++)
	{
		if (m_pPockets.GetUpperBound()<n)
			break;

		COXClipPocket* pPocket=(COXClipPocket*) m_pPockets.GetAt(n);
		if (!::IsWindow(pPocket->GetSafeHwnd()))
			continue;

		pPocket->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.DeflateRect(GetEdgeOffset(),GetEdgeOffset());
		GetProposedRect(n,rectDesired);
		if (rectDesired!=rect)
		{
			rectDesired.InflateRect(GetEdgeOffset(),GetEdgeOffset());
			pPocket->MoveWindow(&rectDesired);
		}
	}
}

BOOL COXMulticlipboardDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD(wParam)==BN_CLICKED)
	{
		HWND hWnd=(HWND) lParam;
		CWnd* pWnd=CWnd::FromHandle(hWnd);
		if (pWnd && pWnd->IsKindOf(RUNTIME_CLASS(COXClipPocket)))
		{
			//call PocketToClip() function to copy data from 
			//pocket to the clipboard
			COXClipPocket* pPocket=(COXClipPocket*) pWnd;
			if (pPocket)
			{
				m_bPaste=TRUE;
				PocketToClip(pPocket);
			}
		}
	}
	m_bPaste=FALSE;
	return CDialog::OnCommand(wParam, lParam);
}

void COXMulticlipboardDlg::MoveStatic()
{

	//this function calculates and moves
	//static control to proper position
	CRect rectStatic;

	GetClientRect(&rectStatic);
	rectStatic.DeflateRect(GetStaticDimension(),
		GetStaticDimension());
	rectStatic.top+=GetButtonsHeight();
	CWnd* pWnd=GetDlgItem(IDC_OX_STATIC_EDGE);
	pWnd->MoveWindow(&rectStatic);

}

void COXMulticlipboardDlg::ClipToPocket(COXClipPocket *pPocket)
{
	//the function opens clipboard and
	//calls virtual ClipObjectToPocket()
	//to make copy of the clipboard data
	ASSERT(pPocket);

	COleDataObject clipobj;

	if (!clipobj.AttachClipboard())
	{
		TRACE0("CMulticlipboardDlg::ClipToPocket : Failed to attach to clipboard\r\n");
		return;
	}

	ClipObjectToPocket(pPocket, &clipobj);
}

BOOL COXMulticlipboardDlg::ClipObjectToPocket(COXClipPocket *pPocket, COleDataObject *pObject)
{
	//this function actually does coping
	//clipboard data to the pocket
	//Clipboard data usually may be retrieved in
	//more than one format. The best matching format is
	//that that first will be retrieved by
	//GetNextFormat() function.
	
	ASSERT(pPocket);
	//pObject must be attached to the clipboard already

	ASSERT(pObject);

	pObject->BeginEnumFormats();

	FORMATETC Format;
	STGMEDIUM Stgmed;
	BOOL bRet;

	//try to do loop retrieving the best available format
	do
	{
		 bRet=pObject->GetNextFormat(&Format);
		 if (!bRet)
			 return FALSE;
		 //by default it does not save info in custom registered formats
		 //to do it you have to define OX_CLIP_DO_CUSTOM_FORMATS
#ifndef OX_CLIP_DO_CUSTOM_FORMATS
		 if (Format.cfFormat>=OX_CLIP_CUSTOM_FORMAT)
			 continue;
#endif		 
		 if (pObject->IsDataAvailable(Format.cfFormat,&Format))
		 {
			BOOL bData;

			//trying to get data
			bData=pObject->GetData(Format.cfFormat, &Stgmed);
			if (!bData)
			{
				//no luck to get data
				//try to get it with global memory handle
				Stgmed.hGlobal=pObject->GetGlobalData(Format.cfFormat);
				if (!Stgmed.hGlobal)
				{
					//cannot access this format, will try another one
					TRACE1("Unable to access data from clipboard. Format=0x%08x\r\n",Format.cfFormat);
					continue;
				}
				//we have it in global memory
				Stgmed.pUnkForRelease=NULL;
				Stgmed.tymed=TYMED_HGLOBAL;
			}
			// put it in the pocket and return
			pPocket->SetData(Format.cfFormat, &Stgmed);
			return TRUE;
		 }

	}while(bRet);

	return FALSE;

}


void COXMulticlipboardDlg::PocketToClip(COXClipPocket *pPocket)
{
	//the function actually does copy from
	//the pocket to the clipboard
	tOXData* pData;
	pData=pPocket->GetData();
	ASSERT(pData);
	
	COleDataSource* pSource=new COleDataSource;

	STGMEDIUM Stgmed;

	//copy data from the pocket
	HRESULT hRslt=::CopyStgMedium(&pData->Stgmed,&Stgmed);
	if (hRslt==ERROR_SUCCESS)
	{
		if (Stgmed.tymed==TYMED_HGLOBAL)
		{
			pSource->CacheGlobalData((unsigned short) pData->nFormat,
				Stgmed.hGlobal);
		}
		else
			pSource->CacheData((unsigned short) pData->nFormat,&Stgmed);
		pSource->SetClipboard();
	}
}



void COXMulticlipboardDlg::EmptyPocket(int nPocket)
{
	if (nPocket<m_pPockets.GetSize() && nPocket>=NULL)
	{
		COXClipPocket* pPocket=(COXClipPocket*) m_pPockets.GetAt(nPocket);
		EmptyPocket(pPocket);
	}
}
void COXMulticlipboardDlg::EmptyPocket(COXClipPocket* pPocket)
{
	ASSERT(pPocket);
	tOXData* pData=pPocket->GetData();
	pData->Clear();
}