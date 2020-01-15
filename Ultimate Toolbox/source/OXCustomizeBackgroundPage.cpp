// OXCustomizeBackgroundPage.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXCustomizeBackgroundPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeBackgroundPage dialog

IMPLEMENT_DYNCREATE(COXCustomizeBackgroundPage, COXCustomizePage)

COXCustomizeBackgroundPage::COXCustomizeBackgroundPage()
{
	//{{AFX_DATA_INIT(COXCustomizeBackgroundPage)
	m_pHelpWnd=NULL;
	m_nAlign = -1;
	m_sFilename = _T("");
	m_bOnlyClientArea = TRUE;
	m_bPaintBackground = TRUE;
	m_nImageSourceType = 0;
	//}}AFX_DATA_INIT
	m_clrBack=::GetSysColor(COLOR_WINDOW);

	m_bUseDefaultImage=TRUE;

	m_nDialogID=IDD;
	m_sProfileName.Empty();
}


COXCustomizeBackgroundPage::~COXCustomizeBackgroundPage()
{
	if(m_pHelpWnd != NULL)
	{
		m_pHelpWnd->Detach();
		delete m_pHelpWnd;
	}
}


BOOL COXCustomizeBackgroundPage::
InitializeBackgroundPainter(const BOOL bPaintBackground,
							const BOOL bOnlyClientArea,
							LPCTSTR lpszDIBResourceName, 
							const PaintType paintType/*=Tile*/, 
							const COLORREF clrBk/*=CLR_DEFAULT*/, 
							LPCTSTR lpszProfileName/*=_T("CustomizeBackgroundPainter")*/)
{
	COXDIB dib;
	if(lpszDIBResourceName!=NULL)
		if(!dib.LoadResource(lpszDIBResourceName))
			return FALSE;
	return InitializeBackgroundPainter(bPaintBackground,bOnlyClientArea,
		&dib,paintType,clrBk,lpszProfileName);
}

BOOL COXCustomizeBackgroundPage::
InitializeBackgroundPainter(const BOOL bPaintBackground,
							const BOOL bOnlyClientArea,
							const UINT nDIBResourceID, 
							const PaintType paintType/*=Tile*/, 
							const COLORREF clrBk/*=CLR_DEFAULT*/, 
							LPCTSTR lpszProfileName/*=_T("CustomizeBackgroundPainter")*/)
{
	return InitializeBackgroundPainter(bPaintBackground,bOnlyClientArea,
		MAKEINTRESOURCE(nDIBResourceID),paintType,clrBk,lpszProfileName);
}


BOOL COXCustomizeBackgroundPage::
InitializeBackgroundPainter(const BOOL bPaintBackground,
							const BOOL bOnlyClientArea,
							const CString& sDIBFileName, 
							const PaintType paintType/*=Tile*/, 
							const COLORREF clrBk/*=CLR_DEFAULT*/, 
							LPCTSTR lpszProfileName/*=_T("CustomizeBackgroundPainter")*/)
{
	COXDIB dib;
	if(!sDIBFileName.IsEmpty())
		if(!dib.Read(sDIBFileName))
			return FALSE;
	m_sFilename=sDIBFileName;
	return InitializeBackgroundPainter(bPaintBackground,bOnlyClientArea,
		&dib,paintType,clrBk,lpszProfileName);
}


BOOL COXCustomizeBackgroundPage::
InitializeBackgroundPainter(const BOOL bPaintBackground,
							const BOOL bOnlyClientArea,
							const COXDIB* pDIB, 
							const PaintType paintType/*=Tile*/, 
							const COLORREF clrBk/*=CLR_DEFAULT*/, 
							LPCTSTR lpszProfileName/*=_T("CustomizeBackgroundPainter")*/)
{
	m_bPaintBackground=bPaintBackground;
	m_bOnlyClientArea=bOnlyClientArea;
	m_bUseDefaultImage=TRUE;

	if(lpszProfileName!=NULL)
	{
		m_sProfileName=lpszProfileName;

		CWinApp* pApp=AfxGetApp();
		ASSERT(pApp!=NULL);

		m_bPaintBackground=pApp->GetProfileInt(m_sProfileName,
			_T("PaintBackground"),m_bPaintBackground);
		m_bOnlyClientArea=pApp->GetProfileInt(m_sProfileName,
			_T("OnlyClientArea"),m_bOnlyClientArea);
		m_sFilename=pApp->GetProfileString(m_sProfileName,
			_T("ImageFileName"),m_sFilename);
		m_nAlign=(int)pApp->GetProfileInt(lpszProfileName,
			_T("PaintType"),(int)paintType);
		m_clrBack=pApp->GetProfileInt(lpszProfileName,_T("BackgroundColor"),clrBk);
		m_bUseDefaultImage=pApp->GetProfileInt(m_sProfileName,
			_T("UseDefaultImage"),m_bUseDefaultImage);
	}

	if(pDIB!=NULL)
		m_dibDefault=*pDIB;
	else
		m_dibDefault.Empty();

	SetupBackgroundPainting();

	return TRUE;
}


BOOL COXCustomizeBackgroundPage::OnCloseManager(BOOL bIsOk) 
{	
	CWinApp* pApp=AfxGetApp();
	ASSERT(pApp!=NULL);

	if(bIsOk)
	{
		if(!ApplyChanges())
			return FALSE;
		if(!m_sProfileName.IsEmpty())
		{
			pApp->WriteProfileInt(m_sProfileName,_T("PaintBackground"),
				m_bPaintBackground);
			pApp->WriteProfileInt(m_sProfileName,_T("OnlyClientArea"),
				m_bOnlyClientArea);
			pApp->WriteProfileString(m_sProfileName,_T("ImageFileName"),
				m_sFilename);
			pApp->WriteProfileInt(m_sProfileName,_T("PaintType"),m_nAlign);
			pApp->WriteProfileInt(m_sProfileName,_T("BackgroundColor"),m_clrBack);
			pApp->WriteProfileInt(m_sProfileName,_T("UseDefaultImage"),
				m_bUseDefaultImage);
		}
	}
	else
	{
		if(!m_sProfileName.IsEmpty())
		{
			m_bPaintBackground=pApp->GetProfileInt(m_sProfileName,
				_T("PaintBackground"),m_bPaintBackground);
			m_bOnlyClientArea=pApp->GetProfileInt(m_sProfileName,
				_T("OnlyClientArea"),m_bOnlyClientArea);
			m_sFilename=pApp->GetProfileString(m_sProfileName,
				_T("ImageFileName"),m_sFilename);
			m_nAlign=(int)pApp->GetProfileInt(m_sProfileName,
				_T("PaintType"),m_nAlign);
			m_clrBack=pApp->GetProfileInt(m_sProfileName,
				_T("BackgroundColor"),m_clrBack);
			m_bUseDefaultImage=pApp->GetProfileInt(m_sProfileName,
				_T("UseDefaultImage"),m_bUseDefaultImage);

			SetupBackgroundPainting();
		}
	}

	return TRUE;
}


void COXCustomizeBackgroundPage::DoDataExchange(CDataExchange* pDX)
{
	COXCustomizePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXCustomizeBackgroundPage)
	DDX_Control(pDX, IDC_OX_SEPARATOR2, m_ctlSeparator2);
	DDX_Control(pDX, IDC_OX_SEPARATOR1, m_ctlSeparator1);
	DDX_Control(pDX, IDC_OX_FILENAME, m_cmbFilename);
	DDX_Control(pDX, IDC_OX_BACK_COLOR, m_btnBackColor);
	DDX_Radio(pDX, IDC_OX_ALIGN_TILE, m_nAlign);
	DDX_CBString(pDX, IDC_OX_FILENAME, m_sFilename);
	DDX_Check(pDX, IDC_OX_CHECK_ONLYCLIENT, m_bOnlyClientArea);
	DDX_Check(pDX, IDC_OX_CHECK_PAINT_BACKGROUND, m_bPaintBackground);
	DDX_Radio(pDX, IDC_OX_RADIO_BCKGND_IMG_SOURCE_DEFAULT, m_nImageSourceType);
	//}}AFX_DATA_MAP
	DDX_ColorPicker(pDX, IDC_OX_BACK_COLOR, m_clrBack);
}


BEGIN_MESSAGE_MAP(COXCustomizeBackgroundPage, COXCustomizePage)
	//{{AFX_MSG_MAP(COXCustomizeBackgroundPage)
	ON_BN_CLICKED(IDC_OX_ALIGN_BOTTOMCENTER, OnAlign)
	ON_BN_CLICKED(IDC_OX_BACK_COLOR, OnBackColor)
	ON_CBN_EDITCHANGE(IDC_OX_FILENAME, OnEditchangeFilename)
	ON_CBN_SELCHANGE(IDC_OX_FILENAME, OnSelchangeFilename)
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_BN_CLICKED(IDC_OX_CHECK_PAINT_BACKGROUND, OnEnable)
	ON_BN_CLICKED(IDC_OX_ALIGN_BOTTOMLEFT, OnAlign)
	ON_BN_CLICKED(IDC_OX_ALIGN_BOTTOMRIGHT, OnAlign)
	ON_BN_CLICKED(IDC_OX_ALIGN_CENTER, OnAlign)
	ON_BN_CLICKED(IDC_OX_ALIGN_CENTERLEFT, OnAlign)
	ON_BN_CLICKED(IDC_OX_ALIGN_CENTERRIGHT, OnAlign)
	ON_BN_CLICKED(IDC_OX_ALIGN_STRETCH, OnAlign)
	ON_BN_CLICKED(IDC_OX_ALIGN_TILE, OnAlign)
	ON_BN_CLICKED(IDC_OX_ALIGN_TOPCENTER, OnAlign)
	ON_BN_CLICKED(IDC_OX_ALIGN_TOPLEFT, OnAlign)
	ON_BN_CLICKED(IDC_OX_ALIGN_TOPRIGHT, OnAlign)
	ON_BN_CLICKED(IDC_OX_RADIO_BCKGND_IMG_SOURCE_DEFAULT, OnRadioBackgroundImageSourceDefault)
	ON_BN_CLICKED(IDC_OX_RADIO_BCKGND_IMG_SOURCE_FILENAME, OnRadioBackgroundImageSourceFilename)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeBackgroundPage message handlers

BOOL COXCustomizeBackgroundPage::Load(const COXCustomizeManager* pCustomizeManager)
{
	// Call default implementation. It will load this demo dialog as the first 
	// page and will create About and CodeSample pages if specified.
	if(!COXCustomizePage::Load(pCustomizeManager))
		return FALSE;

	return TRUE;
}


void COXCustomizeBackgroundPage::Unload()
{
	// add here code for cleaning up all objects created by demo
	//
	//
	//////////////////////////////////////////////////////////////////////////

	COXCustomizePage::Unload();
}

void COXCustomizeBackgroundPage::OnInitDialog()
{
	// must call default implementation
	COXCustomizePage::OnInitDialog();

	// add here initialization code for your demo dialog. Treat it as a
	// normal CDialog::OnInitDialog function

	m_cmbFilename.
		SetAutoPersistent(_T("CustomizeBackgroundPainter_ImageFileName"));
	for(int nIndex=4; nIndex<OX_HISTORY_COMBO_MAX_TOOLBUTTONS; nIndex++)
	{
		m_cmbFilename.ShowButton(nIndex,FALSE);
	}
	CToolBar* pToolbar=m_cmbFilename.GetToolBar();
	pToolbar->ModifyStyle(NULL,TBSTYLE_FLAT);
	m_cmbFilename.RefreshToolbar();

	m_btnBackColor.SetToolTip(TRUE);
	m_btnBackColor.SetDefaultColor(::GetSysColor(COLOR_BTNFACE));

	CWnd* pCtrl=GetDlgItem(IDC_OX_IMAGE);
	ASSERT(pCtrl!=NULL);

	CRect rect;
	pCtrl->GetWindowRect(rect);
	pCtrl->DestroyWindow();
	ScreenToClient(rect);
	m_pictureWnd.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rect,this,IDC_OX_IMAGE);

	if(!m_sFilename.IsEmpty() && !m_bUseDefaultImage)
	{
		VERIFY(m_backgroundPainterOrganizer.Attach(&m_pictureWnd,
			m_sFilename,(PaintType)m_nAlign,m_clrBack));
	}
	else
	{
		VERIFY(m_backgroundPainterOrganizer.Attach(&m_pictureWnd,
			&m_dibDefault,(PaintType)m_nAlign,m_clrBack));
	}
	
	m_pictureWnd.ModifyStyleEx(NULL,WS_EX_CLIENTEDGE,SWP_DRAWFRAME);
	m_pictureWnd.RedrawWindow();

	m_nImageSourceType=(m_bUseDefaultImage ? 0 : 1);

	// layout
	//
	m_LayoutManager.TieChild(&m_ctlSeparator1,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_ctlSeparator2,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_cmbFilename,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);

	m_LayoutManager.TieChild(IDC_OX_GROUP_PAINT_TYPE,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_ALIGN_STRETCH,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_ALIGN_TOPRIGHT,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_ALIGN_CENTERRIGHT,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_ALIGN_BOTTOMRIGHT,OX_LMS_RIGHT,OX_LMT_SAME);

	m_LayoutManager.TieChild(IDC_OX_STATIC_BACKGROUND_COLOR,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_btnBackColor,OX_LMS_RIGHT,OX_LMT_SAME);

	m_LayoutManager.TieChild(IDC_OX_IMAGE,OX_LMS_ANY,OX_LMT_SAME);
	//
	///////////////////////////////////////

	if (m_bUseDefaultImage)
		m_sFilename = _T("");

	UpdateData(FALSE);

	OnEnable();

	m_bInitialized=TRUE;
}


void COXCustomizeBackgroundPage::OnAlign() 
{
	// TODO: Add your control notification handler code here
	
	if(!m_bInitialized)
		return;

	UpdateData(TRUE);
	m_backgroundPainterOrganizer.SetPaintType(&m_pictureWnd,(PaintType)m_nAlign);
}

void COXCustomizeBackgroundPage::OnBackColor() 
{
	// TODO: Add your control notification handler code here
	
	if(!m_bInitialized)
		return;

	UpdateData(TRUE);

	m_backgroundPainterOrganizer.SetBkColor(&m_pictureWnd,m_clrBack);
}

void COXCustomizeBackgroundPage::OnEditchangeFilename() 
{
	// TODO: Add your control notification handler code here
	
	if(!m_bInitialized)
		return;

	UpdateData(TRUE);
	m_backgroundPainterOrganizer.SetWallpaper(&m_pictureWnd,m_sFilename);
}

void COXCustomizeBackgroundPage::OnSelchangeFilename() 
{
	// TODO: Add your control notification handler code here
	
	if(!m_bInitialized)
		return;

	if(m_cmbFilename.GetCurSel()!=CB_ERR)
	{
		m_cmbFilename.GetLBText(m_cmbFilename.GetCurSel(),m_sFilename);
		m_backgroundPainterOrganizer.SetWallpaper(&m_pictureWnd,m_sFilename);
	}
}

void COXCustomizeBackgroundPage::OnPaletteChanged(CWnd* pFocusWnd) 
{
	COXCustomizePage::OnPaletteChanged(pFocusWnd);
	
	// TODO: Add your message handler code here
	SendMessageToDescendants(WM_PALETTECHANGED,(WPARAM)((HWND)*pFocusWnd),NULL);
}

BOOL COXCustomizeBackgroundPage::OnQueryNewPalette() 
{
	// TODO: Add your message handler code here and/or call default
	
	SendMessageToDescendants(WM_QUERYNEWPALETTE,NULL,NULL);
	return COXCustomizePage::OnQueryNewPalette();
}

LRESULT COXCustomizeBackgroundPage::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	// Update the context list
	m_cmbFilename.OnIdle();

	return TRUE;
}

BOOL COXCustomizeBackgroundPage::ApplyChanges()
{
	if(!UpdateData(TRUE))
		return FALSE;

	m_bUseDefaultImage=(m_nImageSourceType==0);

	SetupBackgroundPainting();

	return TRUE;
}


void COXCustomizeBackgroundPage::SetupBackgroundPainting()
{
	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL && ::IsWindow(pWnd->GetSafeHwnd()));

	CRect rect;
	pWnd->GetWindowRect(&rect);
	pWnd->InvalidateRect(&rect);
	pWnd->SendMessage(WM_ERASEBKGND);


	POSITION pos=NULL;
	COXBackgroundPainter* pBackgroundPainter=
		m_backgroundPainterOrganizer.GetFirstPainter(pos);

	COXDIB dib=((pBackgroundPainter!=NULL && !m_bUseDefaultImage) ? 
		*pBackgroundPainter->GetWallpaperImage() :	m_dibDefault);

	if(!m_bPaintBackground)
	{
		if(m_backgroundPainterOrganizer.IsAttachedFrame())
			VERIFY(m_backgroundPainterOrganizer.DetachFrame());
		else
			VERIFY(m_backgroundPainterOrganizer.Detach(NULL));

		// restore the state of the "Preview" window
		if(::IsWindow(m_pictureWnd.GetSafeHwnd()))
		{
			VERIFY(m_backgroundPainterOrganizer.Attach(&m_pictureWnd,&dib,
				(PaintType)m_nAlign,m_clrBack));
		}

		return;
	}

	if(m_bOnlyClientArea && m_backgroundPainterOrganizer.IsAttachedFrame())
	{
		ASSERT_KINDOF(CFrameWnd,pWnd);

		VERIFY(m_backgroundPainterOrganizer.DetachFrame());

		// restore the state of the "Preview" window
		if(::IsWindow(m_pictureWnd.GetSafeHwnd()))
		{
			VERIFY(m_backgroundPainterOrganizer.Attach(&m_pictureWnd,&dib,
				(PaintType)m_nAlign,m_clrBack));
		}

		if(pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
		{
			CWnd* pMDIClient=CWnd::FromHandlePermanent(((CMDIFrameWnd*)pWnd)->
				m_hWndMDIClient);
			ASSERT(pMDIClient!=NULL);
			// hook MDIClient window
			VERIFY(m_backgroundPainterOrganizer.Attach(pMDIClient,&dib,
				(PaintType)m_nAlign,m_clrBack));
		}
	}
	else if(!m_bOnlyClientArea && !m_backgroundPainterOrganizer.IsAttachedFrame())
	{
		if(pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
		{
			CWnd* pMDIClient=CWnd::FromHandlePermanent(((CMDIFrameWnd*)pWnd)->
				m_hWndMDIClient);
			ASSERT(pMDIClient!=NULL);
			if(m_backgroundPainterOrganizer.IsAttached(pMDIClient))
				VERIFY(m_backgroundPainterOrganizer.Detach(pMDIClient));
		}

		VERIFY(m_backgroundPainterOrganizer.AttachFrame((CFrameWnd*)pWnd,&dib,
			(PaintType)m_nAlign,m_clrBack));
	}
	else
	{
		if(pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		{

			if(m_pHelpWnd)
			{
				m_pHelpWnd->Detach();
				delete m_pHelpWnd;
			}
			
			m_pHelpWnd = new COXHelperWnd;
			if(m_bOnlyClientArea)
			{
				if(pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
				{
					// check if MDIClient has already been attached to a CWnd object
					CWnd* pMDIClient=
						CWnd::FromHandlePermanent(((CMDIFrameWnd*)pWnd)->
						m_hWndMDIClient);
					// if MDIClient is not attached yet
					if(pMDIClient==NULL)
					{
						// then attach it
						VERIFY(m_pHelpWnd->Attach(((CMDIFrameWnd*)pWnd)->
							m_hWndMDIClient));
						pMDIClient=m_pHelpWnd;
					}
					if(!m_backgroundPainterOrganizer.IsAttached(pMDIClient))
					{
						// hook MDIClient window
						VERIFY(m_backgroundPainterOrganizer.Attach(pMDIClient,&dib,
							(PaintType)m_nAlign,m_clrBack));
					}
				}
			}
			else
			{
				if(pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
				{
					// check if MDIClient has already been attached to a CWnd object
					CWnd* pMDIClient=
						CWnd::FromHandlePermanent(((CMDIFrameWnd*)pWnd)->
						m_hWndMDIClient);
					// if MDIClient is not attached yet
					if(pMDIClient==NULL)
					{
						if(!m_backgroundPainterOrganizer.IsAttached(pMDIClient))
						{
							// then attach it
							VERIFY(m_pHelpWnd->Attach(((CMDIFrameWnd*)pWnd)->
								m_hWndMDIClient));
						}
					}
				}

				if(!m_backgroundPainterOrganizer.IsAttachedFrame())
				{
					VERIFY(!m_backgroundPainterOrganizer.
						AttachFrame((CFrameWnd*)pWnd,&dib,
						(PaintType)m_nAlign,m_clrBack));
				}
			}
		}
	}

	// restore the state of the "Preview" window
	if(::IsWindow(m_pictureWnd.GetSafeHwnd()))
	{
		VERIFY(m_backgroundPainterOrganizer.
			SetOriginWnd(&m_pictureWnd,NULL));
	}

	if(!m_sFilename.IsEmpty() && !m_bUseDefaultImage)
		m_backgroundPainterOrganizer.SetWallpaper(NULL,m_sFilename,FALSE);
	else
		m_backgroundPainterOrganizer.SetWallpaper(NULL,&m_dibDefault,FALSE);
	m_backgroundPainterOrganizer.SetBkColor(NULL,m_clrBack,FALSE);
	m_backgroundPainterOrganizer.SetPaintType(NULL,(PaintType)m_nAlign,TRUE);
}



void COXCustomizeBackgroundPage::OnRadioBackgroundImageSourceDefault() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData(TRUE))
		return;

	m_cmbFilename.EnableWindow(FALSE);
	m_bUseDefaultImage = TRUE;

	m_backgroundPainterOrganizer.SetWallpaper(&m_pictureWnd,&m_dibDefault);

	m_sFilename = _T("");
	UpdateData(FALSE);
}

void COXCustomizeBackgroundPage::OnRadioBackgroundImageSourceFilename() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData(TRUE))
		return;

	m_cmbFilename.EnableWindow(TRUE);
	m_cmbFilename.RefreshToolbar();
	m_bUseDefaultImage = FALSE;
	m_backgroundPainterOrganizer.SetWallpaper(&m_pictureWnd,m_sFilename);
}

void COXCustomizeBackgroundPage::OnEnable()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_OX_GROUP_PAINT_TYPE)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_ALIGN_TILE)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_ALIGN_STRETCH)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_ALIGN_TOPLEFT)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_ALIGN_CENTERLEFT)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_ALIGN_BOTTOMLEFT)->EnableWindow(m_bPaintBackground);

	GetDlgItem(IDC_OX_CHECK_ONLYCLIENT)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_ALIGN_TOPCENTER)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_ALIGN_CENTER)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_ALIGN_BOTTOMCENTER)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_ALIGN_TOPRIGHT)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_ALIGN_CENTERRIGHT)->EnableWindow(m_bPaintBackground);

	GetDlgItem(IDC_OX_ALIGN_BOTTOMRIGHT)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_BACK_COLOR)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_RADIO_BCKGND_IMG_SOURCE_DEFAULT)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_RADIO_BCKGND_IMG_SOURCE_FILENAME)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_STATIC_BACKGROUND_COLOR)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_SEPARATOR2)->EnableWindow(m_bPaintBackground);
	GetDlgItem(IDC_OX_IMAGE)->EnableWindow(m_bPaintBackground);

	m_cmbFilename.EnableWindow(!m_bUseDefaultImage && m_bPaintBackground);

}