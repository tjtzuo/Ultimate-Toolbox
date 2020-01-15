// OXFolderPickerDlg.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXFolderPickerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXFolderPickerDlg dialog

COXFolderPickerDlg::COXFolderPickerDlg(CString sCurrentFolder/*=_T("")*/,
									   CString sRootFolder/*=_T("")*/,
									   BOOL bDisplayDesktopItem/*=TRUE*/,
									   BOOL bShowPopupMenu/*=TRUE*/,
									   BOOL bOnlyFileSystemFolders/*=TRUE*/, 
									   BOOL bShowFiles/*=FALSE*/,
									   LPCTSTR lpszFilter/*=_T("")*/,
									   BOOL bErrorNotify/*=TRUE*/,
									   CWnd* pParent/*=NULL*/,
									   int nIDTitle/*=_T("Pick Folder")*/)
	:	CDialog(COXFolderPickerDlg::IDD, pParent),
		m_sRootFolder(sRootFolder)		
{
	//{{AFX_DATA_INIT(COXFolderPickerDlg)
	m_bOnlyFileSystemFolders=bOnlyFileSystemFolders;
	m_bShowPopupMenu=bShowPopupMenu;
	m_bErrorNotify=bErrorNotify;
	m_sCurrentFolder=sCurrentFolder;
	m_bDisplayDesktopItem=bDisplayDesktopItem;
	m_bShowFiles=bShowFiles;
	//}}AFX_DATA_INIT
	m_sFilter=lpszFilter;
	VERIFY(m_sTitle.LoadString(nIDTitle)); 
}

void COXFolderPickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXFolderPickerDlg)
	DDX_Control(pDX, IDC_OX_COMBO_CURRENT_FOLDER, m_cmbCurrentFolder);
	DDX_Control(pDX, IDC_OX_TREE_SHELL_FOLDERS, m_treeShellFolders);
	DDX_Check(pDX, IDC_OX_CHECK_ONLY_FILE_SYSTEM_FOLDERS, m_bOnlyFileSystemFolders);
	DDX_Check(pDX, IDC_OX_CHECK_POPUP_MENU, m_bShowPopupMenu);
	DDX_CBStringExact(pDX, IDC_OX_COMBO_CURRENT_FOLDER, m_sCurrentFolder);
	DDX_Check(pDX, IDC_OX_CHECK_ERROR_NOTIFY, m_bErrorNotify);
	DDX_Check(pDX, IDC_OX_CHECK_SHOW_FILES, m_bShowFiles);
	DDX_Check(pDX, IDC_OX_CHECK_DISPLAY_DESKTOP_ITEM, m_bDisplayDesktopItem);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COXFolderPickerDlg, CDialog)
	//{{AFX_MSG_MAP(COXFolderPickerDlg)
	ON_BN_CLICKED(IDC_OX_CHECK_ONLY_FILE_SYSTEM_FOLDERS, OnCheckOnlyFileSystemFolders)
	ON_BN_CLICKED(IDC_OX_CHECK_POPUP_MENU, OnCheckPopupMenu)
	ON_NOTIFY(TVN_SELCHANGED, IDC_OX_TREE_SHELL_FOLDERS, OnSelChangedTreeShellFolders)
	ON_BN_CLICKED(IDC_OX_CHECK_ERROR_NOTIFY, OnCheckErrorNotify)
	ON_BN_CLICKED(IDC_OX_BUTTON_BROWSE_FOLDER, OnButtonBrowseFolder)
	ON_BN_CLICKED(IDC_OX_CHECK_DISPLAY_DESKTOP_ITEM, OnCheckDisplayDesktopItem)
	ON_BN_CLICKED(IDC_OX_CHECK_SHOW_FILES, OnCheckShowFiles)
	ON_BN_CLICKED(IDC_OX_BUTTON_REFRESH_ALL, OnRefreshAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXFolderPickerDlg message handlers

BOOL COXFolderPickerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	// setup history combo
	m_cmbCurrentFolder.SetAutoPersistent(_T("Current Folder"));
	for(int nIndex=0; nIndex<OX_HISTORY_COMBO_MAX_TOOLBUTTONS; nIndex++)
		m_cmbCurrentFolder.ShowButton(nIndex,FALSE);
	m_cmbCurrentFolder.RefreshToolbar();
	//////////////////////////////////////
	CString sTitle;
	VERIFY(sTitle.LoadString(IDS_OX_FOLDERPICKERTITLE));//"Shell Namespace"
	m_treeShellFolders.SetWindowText(sTitle);
	m_treeShellFolders.SetEnableContextMenu(m_bShowPopupMenu);
	m_treeShellFolders.SetOnlyFileSystemFolders(m_bOnlyFileSystemFolders);
	m_treeShellFolders.SetShowFiles(m_bShowFiles);
	m_treeShellFolders.SetNotifyError(m_bErrorNotify);
	m_treeShellFolders.SetFilter(m_sFilter);

	VERIFY(m_treeShellFolders.InitializeTree(m_sRootFolder));
	m_treeShellFolders.OpenFolder(m_sCurrentFolder);
	
	CString sDialogTitle=m_sTitle;
	VERIFY(sTitle.LoadString(IDS_OX_FOLDERPICKERROOT));//" [ Root Folder - "
	CString sDesktop;
	VERIFY(sDesktop.LoadString(IDS_OX_FOLDERPICKERDESKTOP));//"Desktop"
	sDialogTitle+=sTitle+
		(m_sRootFolder.IsEmpty() ? sDesktop : m_sRootFolder)+_T(" ]");
	SetWindowText(sDialogTitle);
  
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COXFolderPickerDlg::OnCheckOnlyFileSystemFolders() 
{
	// TODO: Add your control notification handler code here

	UpdateData();	

	m_treeShellFolders.SetOnlyFileSystemFolders(m_bOnlyFileSystemFolders);
	m_treeShellFolders.Refresh();
}

void COXFolderPickerDlg::OnCheckShowFiles() 
{
	// TODO: Add your control notification handler code here

	UpdateData();	

	m_treeShellFolders.SetShowFiles(m_bShowFiles);
	m_treeShellFolders.Refresh();
}

void COXFolderPickerDlg::OnCheckPopupMenu() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();	
	m_treeShellFolders.SetEnableContextMenu(m_bShowPopupMenu);
}

void COXFolderPickerDlg::OnCheckDisplayDesktopItem() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();	

	m_treeShellFolders.SetShowDesktopItem(m_bDisplayDesktopItem);
	m_treeShellFolders.Refresh();
}

void COXFolderPickerDlg::OnSelChangedTreeShellFolders(NMHDR* pNMHDR, 
													  LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView=(NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	HTREEITEM htiSelected=pNMTreeView->itemNew.hItem;
	if(htiSelected!=NULL)
	{
		m_sCurrentFolder=m_treeShellFolders.GetFullPath(htiSelected);
		UpdateData(FALSE);
	}

	*pResult=0;
}

void COXFolderPickerDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}


void COXFolderPickerDlg::OnCheckErrorNotify() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();	
	m_treeShellFolders.SetNotifyError(m_bErrorNotify);
}


void COXFolderPickerDlg::OnButtonBrowseFolder() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();	

	CWaitCursor wait;

	m_treeShellFolders.SetRedraw(FALSE);
	if(m_treeShellFolders.OpenFolder(m_sCurrentFolder))
		m_treeShellFolders.SetFocus();
	m_treeShellFolders.SetRedraw(TRUE);
}


void COXFolderPickerDlg::OnRefreshAll() 
{
	// TODO: Add your control notification handler code here
	
	m_treeShellFolders.Refresh();
}

