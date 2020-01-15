// OXSoundCustomizeDlg.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXSoundCustomizeDlg.h"

#include "UTBStrOp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXSoundCustomizeDlg dialog

// array of predefined sound events descriptions
//

static LPTSTR arrPredefinedEventDescription[][2] = {
	{_T("Mouse enter"),					_T("HWM_MOUSEENTER")},	
	{_T("Mouse leave"),					_T("HWM_MOUSELEAVE")},	
	{_T("Left button down"),			_T("WM_LBUTTONDOWN")},	
	{_T("Left button up"),				_T("WM_LBUTTONUP")},	
	{_T("Left button double click"),	_T("WM_LBUTTONDBLCLK")},	
	{_T("Right button down"),			_T("WM_RBUTTONDOWN")},	
	{_T("Right button up"),				_T("WM_RBUTTONUP")},		
	{_T("Right button double click"),	_T("WM_RBUTTONDBLCLK")},	
	{_T("Middle button down"),			_T("WM_MBUTTONDOWN")},	
	{_T("Middle button up"),			_T("WM_MBUTTONUP")},	
	{_T("Middle button double click"),	_T("WM_MBUTTONDBLCLK")},	
};

int arrSoundDescriptionID [11]=
{
	IDS_OX_SOUNDCSTMZDLGMOUSEENTER,
	IDS_OX_SOUNDCSTMZDLGMOUSELEAVE,
	IDS_OX_SOUNDCSTMZDLGLBDOWN,
	IDS_OX_SOUNDCSTMZDLGLBUP,
	IDS_OX_SOUNDCSTMZDLGLBDBL,
	IDS_OX_SOUNDCSTMZDLGRBDOWN,
	IDS_OX_SOUNDCSTMZDLGRBUP,
	IDS_OX_SOUNDCSTMZDLGRBDBL,
	IDS_OX_SOUNDCSTMZDLGMBDOWN,
	IDS_OX_SOUNDCSTMZDLGMBUP,
	IDS_OX_SOUNDCSTMZDLGMBDBL
};

TCHAR arrDescrs [11][_MAX_PATH];

/////////////////////////////////////////////////////////////////////////////

// array of predefined sound events identificators
//
static UINT arrPredefinedEventID[11] = {
	HWM_MOUSEENTER,
	HWM_MOUSELEAVE,
	WM_LBUTTONDOWN,
	WM_LBUTTONUP,
	WM_LBUTTONDBLCLK,
	WM_RBUTTONDOWN,
	WM_RBUTTONUP,
	WM_RBUTTONDBLCLK,
	WM_MBUTTONDOWN,
	WM_MBUTTONUP,
	WM_MBUTTONDBLCLK,
};
/////////////////////////////////////////////////////////////////////////////


// constructor
COXSoundCustomizeDlg::COXSoundCustomizeDlg(CWnd* pParent, 
										   COXSoundEffectOrganizer* pSoundEffectOrganizer,
										   enum LEVELTYPE nStartLevel/*=LT_EVENT*/,
										   DWORD dwDlgStyle/*=SCDS_NOTOGGLE*/)
	: CDialog(COXSoundCustomizeDlg::IDD, pParent),
	m_cmbFileName(COXHistoryCombo::TBPHorizontalTopRight,TRUE)
{
	//{{AFX_DATA_INIT(COXSoundCustomizeDlg)
	m_nEvent = -1;
	m_sFileName = _T("");
	m_bForceToStart = FALSE;
	m_nLoopCount = 0;
	m_nLoopType = -1;
	//}}AFX_DATA_INIT

	// save specified parameters for future use
	//
	ASSERT(pSoundEffectOrganizer!=NULL);
	m_pSoundEffectOrganizer=pSoundEffectOrganizer;

	m_nStartLevel=nStartLevel;
	m_dwDlgStyle=dwDlgStyle;
	/////////////////////////////////////////////////////////////////////////////
	
	// initialize internal variables
	//
	m_bEditing=FALSE;
	m_bAddingNew=FALSE;

	m_dwControlState=0xffffffff;
	/////////////////////////////////////////////////////////////////////////////

	// fill array of all predefined events
	//
	for(int nIndex=0; 
		nIndex<sizeof(arrPredefinedEventID)/sizeof(arrPredefinedEventID[0]);
		nIndex++)
	{
			CString sItem;
			VERIFY(sItem.LoadString(arrSoundDescriptionID[nIndex]));
			lstrcpy((LPTSTR) &arrDescrs[nIndex], sItem);
			LPTSTR lpszString=(LPTSTR) &arrDescrs[nIndex];
			arrPredefinedEventDescription[nIndex][0]=lpszString;
		SetEvent(arrPredefinedEventID[nIndex],
			(LPCTSTR*) arrPredefinedEventDescription[nIndex]);
	}
	/////////////////////////////////////////////////////////////////////////////
}

// destructor
COXSoundCustomizeDlg::~COXSoundCustomizeDlg()
{
	// delete all the structures we allocated and clear up all arrays
	//
	LPITEMINFO pItemInfo;
	HANDLE hItem;
	POSITION pos=m_allTreeItems.GetStartPosition();
	while(pos!=NULL)
	{
		m_allTreeItems.GetNextAssoc(pos,hItem,pItemInfo);
		ASSERT(hItem!=NULL);
		ASSERT(pItemInfo!=NULL);
		delete pItemInfo;
	}
	m_allTreeItems.RemoveAll();

	m_allDefinedEvents.RemoveAll();
	/////////////////////////////////////////////////////////////////////////////
}

void COXSoundCustomizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXSoundCustomizeDlg)
	DDX_Control(pDX, IDC_OX_BUTTON_RESTORE_OXSOUND, m_btnRestore);
	DDX_Control(pDX, IDC_OX_EDIT_LOOPCOUNT_OXSOUND, m_editLoopCount);
	DDX_Control(pDX, IDC_OX_CHECK_FORCE_OXSOUND, m_btnForceToStart);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_OX_EXTTREE_EFFECTSLIST_OXSOUND, m_treeEffect);
	DDX_Control(pDX, IDC_OX_COMBO_FILENAME_OXSOUND, m_cmbFileName);
	DDX_Control(pDX, IDC_OX_COMBO_EVENT_OXSOUND, m_cmbEvent);
	DDX_Control(pDX, IDC_OX_BUTTON_STOP_OXSOUND, m_btnStop);
	DDX_Control(pDX, IDC_OX_BUTTON_SAVE_OXSOUND, m_btnSave);
	DDX_Control(pDX, IDC_OX_BUTTON_REMOVE_OXSOUND, m_btnRemove);
	DDX_Control(pDX, IDC_OX_BUTTON_PLAY_OXSOUND, m_btnPlay);
	DDX_Control(pDX, IDC_OX_BUTTON_ADD_OXSOUND, m_btnAdd);
	DDX_CBIndex(pDX, IDC_OX_COMBO_EVENT_OXSOUND, m_nEvent);
	DDX_CBString(pDX, IDC_OX_COMBO_FILENAME_OXSOUND, m_sFileName);
	DDX_Check(pDX, IDC_OX_CHECK_FORCE_OXSOUND, m_bForceToStart);
	DDX_Text(pDX, IDC_OX_EDIT_LOOPCOUNT_OXSOUND, m_nLoopCount);
	DDX_Radio(pDX, IDC_OX_RADIO_LOOPONCE_OXSOUND, m_nLoopType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COXSoundCustomizeDlg, CDialog)
	//{{AFX_MSG_MAP(COXSoundCustomizeDlg)
	ON_BN_CLICKED(IDC_OX_BUTTON_ADD_OXSOUND, OnButtonAdd)
	ON_BN_CLICKED(IDC_OX_BUTTON_PLAY_OXSOUND, OnButtonPlay)
	ON_BN_CLICKED(IDC_OX_BUTTON_REMOVE_OXSOUND, OnButtonRemove)
	ON_BN_CLICKED(IDC_OX_BUTTON_SAVE_OXSOUND, OnButtonSave)
	ON_BN_CLICKED(IDC_OX_BUTTON_STOP_OXSOUND, OnButtonStop)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OX_EXTTREE_EFFECTSLIST_OXSOUND, OnItemchangedExttreeEffectslist)
	ON_BN_CLICKED(IDC_OX_BUTTON_RESTORE_OXSOUND, OnButtonRestore)
	ON_BN_CLICKED(IDC_OX_CHECK_FORCE_OXSOUND, OnCheckForce)
	ON_CBN_EDITCHANGE(IDC_OX_COMBO_EVENT_OXSOUND, OnEditchangeComboEvent)
	ON_CBN_SELCHANGE(IDC_OX_COMBO_EVENT_OXSOUND, OnSelchangeComboEvent)
	ON_CBN_EDITCHANGE(IDC_OX_COMBO_FILENAME_OXSOUND, OnEditchangeComboFilename)
	ON_CBN_SELCHANGE(IDC_OX_COMBO_FILENAME_OXSOUND, OnSelchangeComboFilename)
	ON_EN_CHANGE(IDC_OX_EDIT_LOOPCOUNT_OXSOUND, OnChangeEditLoopcount)
	ON_BN_CLICKED(IDC_OX_RADIO_LOOPINFINITELY_OXSOUND, OnRadioLoopinfinitely)
	ON_BN_CLICKED(IDC_OX_RADIO_LOOPONCE_OXSOUND, OnRadioLooponce)
	ON_BN_CLICKED(IDC_OX_RADIO_LOOPREPEAT_OXSOUND, OnRadioLooprepeat)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_OX_EXTTREE_EFFECTSLIST_OXSOUND, OnItemExpandingExttreeEffectslist)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_MESSAGE(WM_OX_SOUNDPLAYBACKCOMPLETE, OnPlaybackComplete)
	ON_MESSAGE(SMDM_UPDATEVARS, OnUpdateVars)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXSoundCustomizeDlg message handlers

BOOL COXSoundCustomizeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// initialize event multi combo box
	//
	m_cmbEvent.SetColumnCount(ID_EVENTINFOCOLUMNS);
	m_cmbEvent.SetColumnWidth(0,130);
	m_cmbEvent.SetColumnWidth(1,100);

	POSITION posDefinedEvents=m_allDefinedEvents.GetStartPosition();
	while(posDefinedEvents!=NULL)
	{
		LPCTSTR* ppszText=NULL;
		UINT nMsgID;
		m_allDefinedEvents.GetNextAssoc(posDefinedEvents,nMsgID,ppszText);
		ASSERT(ppszText!=NULL);
		ASSERT(nMsgID!=SNDEVENT_NONE);

		m_cmbEvent.AddString(ppszText,ID_EVENTINFOCOLUMNS);
	}
	//////////////////////////////////////////////

	// initialize file name history combo box
	//
	// Use auto persistence
	m_cmbFileName.SetAutoPersistent(_T("History - FileName"));
	m_cmbFileName.SetFileDialogParams(TRUE,_T("wav"),NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("WAV Files (*.wav)|*.wav|All Files (*.*)|*.*|"));
	m_cmbFileName.SetMaxHistoryCount(-1);
	m_cmbFileName.SetFlatToolbar(TRUE);
	m_cmbFileName.RefreshToolbar();
	//////////////////////////////////////////////

	// initialize tree control
	//
	// columns' width
	UINT nEventColWidth=150;
	UINT nFileNameColWidth=150;
	UINT nEventInfoColWidth=120;
	UINT nLoopColWidth=70;
	// sort order
	BOOL bSortAscending=TRUE;
	// sorted column
	int nSortCol=-1;

	// load saved state from registry if corresponding style is specified
	//
	if((m_dwDlgStyle&SCDS_LOADSTATE)!=0)
	{
		CWinApp* pApp=AfxGetApp();
		if(pApp!=NULL)
		{
			nEventColWidth=pApp->GetProfileInt(_T("Sound Customize Dialog"),
				_T("EventColWidth"),nEventColWidth);
			nFileNameColWidth=pApp->GetProfileInt(_T("Sound Customize Dialog"),
				_T("FileNameColWidth"),nFileNameColWidth);
			nEventInfoColWidth=pApp->GetProfileInt(_T("Sound Customize Dialog"),
				_T("EventInfoColWidth"),nEventInfoColWidth);
			nLoopColWidth=pApp->GetProfileInt(_T("Sound Customize Dialog"),
				_T("LoopColWidth"),nLoopColWidth);

			bSortAscending=pApp->GetProfileInt(_T("Sound Customize Dialog"),
				_T("SortAscending"),bSortAscending);
			nSortCol=pApp->GetProfileInt(_T("Sound Customize Dialog"),
				_T("SortCol"),nSortCol);
		}
	}

	// create the image list that will be associated with the tree control
	m_ilTree.Create(IDB_OX_TREELIST_OXSOUND,16,1,RGB(255,0,255));

	// insert tree columns 
	LV_COLUMN lvc;
	memset(&lvc,0,sizeof(lvc));
	lvc.fmt=LVCFMT_LEFT;
	lvc.cx=nEventColWidth;
	CString sItem;
	VERIFY(sItem.LoadString(IDS_OX_SOUND_EVENT));//"Event"
	lvc.pszText=sItem.GetBuffer(sItem.GetLength());
	lvc.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
	m_treeEffect.SetColumn(0,&lvc);
	sItem.ReleaseBuffer();
	VERIFY(sItem.LoadString(IDS_OX_SOUNDFILE));//"Sound File"
	m_treeEffect.InsertColumn(1,sItem,LVCFMT_LEFT,nFileNameColWidth,1);
	VERIFY(sItem.LoadString(IDS_OX_SOUNDEVENT));//"Event Info"
	m_treeEffect.InsertColumn(2,sItem,LVCFMT_LEFT,nEventInfoColWidth,2);
	VERIFY(sItem.LoadString(IDS_OX_SOUNDLOOP));//"Loop"
	m_treeEffect.InsertColumn(3,sItem,LVCFMT_CENTER,nLoopColWidth,3);
	m_treeEffect.SetImageList(&m_ilTree,TVSIL_NORMAL);

	// create fonts to customize tree items appearance
	LOGFONT lf;
	CFont fontRoot, fontSubroot;
	CFont* pFont=GetFont();
	if(pFont->GetLogFont(&lf))
	{
		lf.lfHeight=-14;
		UTBStr::tcsncpy(lf.lfFaceName, 6, _T("Arial"),LF_FACESIZE);
		fontRoot.CreateFontIndirect(&lf);

		UTBStr::tcsncpy(lf.lfFaceName, 16, _T("Times New Roman"),LF_FACESIZE);
		fontSubroot.CreateFontIndirect(&lf);
	}

	// loop through all registered windows
	POSITION pos=m_pSoundEffectOrganizer->GetFirstRegisteredWindow();
	while(pos!=NULL)
	{
		CWnd* pWnd=m_pSoundEffectOrganizer->GetNextRegisteredWindow(pos);
		ASSERT(pWnd);
		ASSERT(pWnd->GetSafeHwnd());

		// get corresponding COXSoundEffectManager object
		COXSoundEffectManager* pSoundEffectManager=
			m_pSoundEffectOrganizer->GetManager(pWnd);
		ASSERT(pSoundEffectManager);

		// get item text
		//
		CString sItemText;

		// for items that represent registered windows level we use type 
		// of the control and window's text as item text
		CString sWindowText;
		pWnd->GetWindowText(sItemText);
		sWindowText=GetSubString(sItemText,1,_T('\n'));
		if(sWindowText.IsEmpty())
			sWindowText=GetSubString(sItemText,2,_T('\n'));

		// flag that specifies if we have to add the window's text to the 
		// window's type
		BOOL bAddWindowText=!sWindowText.IsEmpty();

		// get windows type
		CString sWindowType=_T("Window");
		if(pWnd->IsKindOf(RUNTIME_CLASS(CStatic)))
			sWindowType=_T("Static");
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CButton)))
			sWindowType=_T("Button");
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CAnimateCtrl)))
			sWindowType=_T("Animation");
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CTreeCtrl)))
			sWindowType=_T("Tree Control");
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)))
			sWindowType=_T("List Control");
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CComboBox)))
			sWindowType=_T("ComboBox");
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CListBox)))
			sWindowType=_T("ListBox");
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CSliderCtrl)))
			sWindowType=_T("Slider");
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CTabCtrl)))
			sWindowType=_T("Tab Control");
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CEdit)))
			sWindowType=_T("Edit");
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CRichEditCtrl)))
			sWindowType=_T("RichEdit");
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CProgressCtrl)))
			sWindowType=_T("Progress Bar");
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CSpinButtonCtrl)))
			sWindowType=_T("Spin Button");
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CScrollBar)))
			sWindowType=_T("Scroll Bar");


		sItemText=sWindowType;
		if(bAddWindowText)
			sItemText.Format(_T("%s: %s"),sItemText,sWindowText);

		TV_ITEM tvi;
		HTREEITEM htiSubroot;
		POSITION posEvent;
		LPITEMINFO pItemInfo;

		// registered window
		HTREEITEM htiRoot=m_treeEffect.InsertItem(sItemText);
		ASSERT(htiRoot);
		// associate new font
		if((HFONT)fontRoot)
			VERIFY(m_treeEffect.SetItemFont(htiRoot,&fontRoot));
		// and new color
		VERIFY(m_treeEffect.SetItemColor(htiRoot,RGB(0,0,255)));

		// create description of the new tree item
		pItemInfo=new ITEMINFO;
		pItemInfo->type=IT_WINDOW;
		pItemInfo->pWnd=pWnd;
		pItemInfo->dwControlState=CTL_OK|CTL_CANCEL|CTL_TREE|CTL_REMOVE;
		m_allTreeItems.SetAt((HANDLE)htiRoot,pItemInfo);

		// expand it if otherwise wasn't specified
		if(m_nStartLevel!=LT_WINDOW)
			m_treeEffect.Expand(htiRoot,TVE_EXPAND);
		///////////////////////////

		// registered play events
		//
		// item for list of all start playing sound events
		CString sItem;
		VERIFY(sItem.LoadString(IDS_OX_SOUNDPLAYSOUND));//"Play sound events"
		htiSubroot=m_treeEffect.InsertItem(sItem,htiRoot);
		ASSERT(htiSubroot);
		// expand it if otherwise wasn't specified
		if(m_nStartLevel==LT_EVENT)
			m_treeEffect.Expand(htiSubroot,TVE_EXPAND);
		// associate new font
		if((HFONT)fontSubroot)
			VERIFY(m_treeEffect.SetItemFont(htiSubroot,&fontSubroot));
		// and new color
		VERIFY(m_treeEffect.SetItemColor(htiSubroot,RGB(128,0,0)));
		// insert the item
		memset(&tvi,0,sizeof(tvi));
		tvi.mask=TVIF_IMAGE|TVIF_HANDLE|TVIF_SELECTEDIMAGE;
		tvi.hItem=htiSubroot;
		tvi.iImage=1;
		tvi.iSelectedImage=1;
		m_treeEffect.SetItem(&tvi);

		// create description of the new tree item
		pItemInfo=new ITEMINFO;
		pItemInfo->type=IT_PLAYEVENTLIST;
		pItemInfo->pWnd=pWnd;
		pItemInfo->dwControlState=CTL_OK|CTL_CANCEL|CTL_TREE|CTL_REMOVE|CTL_ADD;
		m_allTreeItems.SetAt((HANDLE)htiSubroot,pItemInfo);

		// loop through all play events
		posEvent=pSoundEffectManager->GetFirstPlayEvent();
		while(posEvent!=NULL)
		{
			// get the registered event ID
			UINT nPlayMsgID=pSoundEffectManager->GetNextPlayEvent(posEvent);
			ASSERT(nPlayMsgID!=SNDEVENT_NONE);
			
			// add new play event item to the tree
			AddEventItem(pWnd,htiSubroot,nPlayMsgID,pSoundEffectManager,TRUE);
		}
		///////////////////////////

		// registered stop events
		//
		// item for list of all stop playing sound events
		VERIFY(sItem.LoadString(IDS_OX_SOUNDSTOPPLAYING));//"Stop playing events"
		htiSubroot=m_treeEffect.InsertItem(sItem,htiRoot);
		ASSERT(htiSubroot);
		// expand it if otherwise wasn't specified
		if(m_nStartLevel==LT_EVENT)
			m_treeEffect.Expand(htiSubroot,TVE_EXPAND);
		// associate new font
		if((HFONT)fontSubroot)
			VERIFY(m_treeEffect.SetItemFont(htiSubroot,&fontSubroot));
		// and new color
		VERIFY(m_treeEffect.SetItemColor(htiSubroot,RGB(128,0,0)));
		// insert the item
		memset(&tvi,0,sizeof(tvi));
		tvi.mask=TVIF_IMAGE|TVIF_HANDLE|TVIF_SELECTEDIMAGE;
		tvi.hItem=htiSubroot;
		tvi.iImage=2;
		tvi.iSelectedImage=2;
		m_treeEffect.SetItem(&tvi);

		// create description of the new tree item
		pItemInfo=new ITEMINFO;
		pItemInfo->type=IT_STOPEVENTLIST;
		pItemInfo->pWnd=pWnd;
		pItemInfo->dwControlState=CTL_OK|CTL_CANCEL|CTL_TREE|CTL_REMOVE|CTL_ADD;
		m_allTreeItems.SetAt((HANDLE)htiSubroot,pItemInfo);

		// loop through all stop events
		posEvent=pSoundEffectManager->GetFirstStopEvent();
		while(posEvent!=NULL)
		{
			// get the registered event ID
			UINT nStopMsgID=pSoundEffectManager->GetNextStopEvent(posEvent);
			ASSERT(nStopMsgID!=SNDEVENT_NONE);
			
			// add new stop event item to the tree
			AddEventItem(pWnd,htiSubroot,nStopMsgID,pSoundEffectManager,FALSE);
		}
		///////////////////////////
	}

	// set COXTreeCtrl styles
	m_treeEffect.ModifyExStyle(0,TVOXS_PICKANYWHERE|TVOXS_ROWSEL|TVOXS_MULTISEL);
	m_treeEffect.ModifyStyle(0,TVS_SHOWSELALWAYS);

	// sort the tree control if needed
	if(nSortCol!=-1)
	{
		ASSERT(nSortCol>=0 && nSortCol<m_treeEffect.GetColumnsCount());
		m_treeEffect.SortChildren(NULL,nSortCol,bSortAscending);
	}
	//////////////////////////////////////////////

	// initialize bitmap buttons
	//
	SetupBitmapButton(&m_btnPlay);
	SetupBitmapButton(&m_btnStop);

	VERIFY(m_btnPlay.LoadBitmap(IDB_OX_BITMAP_PLAY_OXSOUND, FALSE, RGB(255,0,255)));
	VERIFY(m_btnStop.LoadBitmap(IDB_OX_BITMAP_STOP_OXSOUND, FALSE, RGB(255,0,255)));
	//////////////////////////////////////////////

	// initialize sound object
	//
	m_TestSound.SetCallbackWnd(this);
	//////////////////////////////////////////////

	// change the dialog caption text if read-only style was specified
	if((m_dwDlgStyle&SCDS_READONLY)!=0)
	{
		CString sWindowText;
		GetWindowText(sWindowText);
		CString sText;
		VERIFY(sText.LoadString(IDS_OX_SOUNDREADONLY));//" (Read Only)"
		sWindowText+=sText;
		SetWindowText(sWindowText);
	}

	// populate the dialog controls
	GetVars();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COXSoundCustomizeDlg::GetVars()
{
	// initialize
	//
	m_nEvent=-1;
	m_sFileName=_T("");
	m_bForceToStart=FALSE;
	m_nLoopCount=0;
	m_nLoopType=-1;
	DWORD dwControlState=CTL_OK|CTL_CANCEL|CTL_TREE;
	//////////////////////

	// populate the dialog controls depending on the selected tree item(s)
	HTREEITEM htiSelected=m_treeEffect.GetNextItem(m_treeEffect.GetRootItem(),
		TVGN_FIRSTSELECTED);
	// loop through all selected items
	BOOL bForFirstTime=TRUE;
	while(htiSelected!=NULL)
	{
		LPITEMINFO pItemInfo;
		VERIFY(m_allTreeItems.Lookup(htiSelected,pItemInfo));
		ASSERT(pItemInfo!=NULL);

		// control's state flag
		if(bForFirstTime)
			dwControlState=pItemInfo->dwControlState;
		else
			dwControlState&=pItemInfo->dwControlState;

		// populate controls depending on the type of the item
		switch(pItemInfo->type)
		{
		case IT_WINDOW:
		case IT_PLAYEVENTLIST:
		case IT_STOPEVENTLIST:
			{
				if(!bForFirstTime)
				{
					m_nEvent=-1;
					m_sFileName.Empty();
					m_nLoopCount=0;
					m_nLoopType=-1;
				}
				break;
			}
		case IT_PLAYEVENT:
			{
				ASSERT(pItemInfo->nMsgID!=SNDEVENT_NONE);

				// sound event info
				LPCTSTR* ppszText=NULL;
				if(m_allDefinedEvents.Lookup(pItemInfo->nMsgID,ppszText))
				{
					ASSERT(ppszText!=NULL);
					CString sEvent=ppszText[0];
					if(bForFirstTime)
						m_nEvent=m_cmbEvent.FindStringExact(-1,sEvent);
					else if(m_nEvent!=m_cmbEvent.FindStringExact(-1,sEvent)) 
						m_nEvent=-1;
				}
				else
					m_nEvent=-1;

				
				ASSERT(pItemInfo->pWnd!=NULL);
				ASSERT(::IsWindow(pItemInfo->pWnd->GetSafeHwnd()));

				LPSOUNDEFFECT pSoundEffect=pItemInfo->pSoundEffect;
				if(pSoundEffect!=NULL)
				{
					// sound source info 
					LPSOUNDSOURCEINFO pSoundSourceInfo=pItemInfo->pSoundSourceInfo;
					ASSERT(pSoundSourceInfo!=NULL);
					CString sFileName;
					switch(pSoundSourceInfo->source)
					{
					case SNDSRC_FILE:
						sFileName=pSoundSourceInfo->sFileName;
						break;
					case SNDSRC_CFILE:
						VERIFY(sFileName.LoadString(IDS_OX_SOUNDOPENCFILE));//"Opened CFile object"
						//sFileName=_T();
						break;
					case SNDSRC_INTRESOURCE:
						sFileName.Format(IDS_OX_SOUNDRESOURCEID,//"Resource: ID=%d"
							pSoundSourceInfo->nResourceID);
						break;
					default:
						ASSERT(FALSE);
					}
					if(bForFirstTime)
						m_sFileName=sFileName;
					else if(m_sFileName!=sFileName) 
						m_sFileName.Empty();

					// "Force To Start" flag
					if(bForFirstTime)
						m_bForceToStart=pSoundEffect->bForceToStart;
					else if(m_bForceToStart!=pSoundEffect->bForceToStart) 
						m_bForceToStart=2;

					// loop type
					int nLoopCount=pSoundEffect->nLoopCount;
					int nLoopType=0;
					if(!pSoundEffect->bLoop || nLoopCount==0 || nLoopCount==1)
					{
						nLoopCount=0;
						nLoopType=0;
					}
					else if(nLoopCount==-1)
					{
						nLoopCount=0;
						nLoopType=1;
					}
					else if(nLoopCount>1)
						nLoopType=2;
					else
						ASSERT(FALSE);

					if(bForFirstTime)
						m_nLoopCount=nLoopCount;
					else if(m_nLoopCount!=nLoopCount) 
						m_nLoopCount=0;
					if(bForFirstTime)
						m_nLoopType=nLoopType;
					else if(m_nLoopType!=nLoopType) 
						m_nLoopType=-1;
				}

				break;
			}
		case IT_STOPEVENT:
			{
				ASSERT(pItemInfo->nMsgID!=SNDEVENT_NONE);

				if(!bForFirstTime)
				{
					m_sFileName.Empty();
					m_nLoopCount=0;
					m_nLoopType=-1;
				}

				// sound event info
				LPCTSTR* ppszText=NULL;
				if(m_allDefinedEvents.Lookup(pItemInfo->nMsgID,ppszText))
				{
					ASSERT(ppszText!=NULL);
					CString sEvent=ppszText[0];
					if(bForFirstTime)
						m_nEvent=m_cmbEvent.FindStringExact(-1,sEvent);
					else if(m_nEvent!=m_cmbEvent.FindStringExact(-1,sEvent)) 
						m_nEvent=-1;
				}
				else
					m_nEvent=-1;

				break;
			}
		default:
			ASSERT(FALSE);
		}

		bForFirstTime=FALSE;

		htiSelected=m_treeEffect.GetNextItem(htiSelected,TVGN_NEXTSELECTED);
	}

	// setupo availability of "Add" and "Remove" buttons
	if(m_treeEffect.GetSelectedCount()>1)
		dwControlState&=~(CTL_ADD|CTL_REMOVE);
	if(m_nEvent==-1)
		dwControlState&=~(CTL_EVENT);

	// enable/disable controls
	ShowControls(dwControlState);

	// show it
	UpdateData(FALSE);
}

LRESULT COXSoundCustomizeDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	UNUSED(wParam);
	//use this function to let History Combo's toolbars to get messages
	ASSERT_VALID(this);
	ASSERT(wParam == MSGF_DIALOGBOX);
	BOOL bContinueIdle = TRUE;

	// Pass on to the history combo
	m_cmbFileName.OnIdle();

	return bContinueIdle;
}

LRESULT COXSoundCustomizeDlg::OnUpdateVars(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	// update only once !!!
	//
	MSG msg;
	while(::PeekMessage(&msg,GetSafeHwnd(),SMDM_UPDATEVARS,
		SMDM_UPDATEVARS,PM_REMOVE));

	GetVars();
	//////////////////////////

	return (LRESULT)0;
}

void COXSoundCustomizeDlg::SetupBitmapButton(COXBitmapButton* m_pctl) 
{
	// new font
	CFont* pFont=m_pctl->GetTextFont();
	if(pFont)
	{
		LOGFONT lf;
		if(pFont->GetLogFont(&lf))
		{
			lf.lfWeight=FW_BOLD;
			lf.lfHeight=-14;
			UTBStr::tcsncpy(lf.lfFaceName, 16, _T("Times New Roman"),LF_FACESIZE);
			pFont->DeleteObject();
			if(pFont->CreateFontIndirect(&lf))
				m_pctl->SetTextFont(pFont);
		}
	}
	// new color
	m_pctl->SetTextColor(RGB(128,0,0));
	// tooltip is enabled
	m_pctl->SetToolTip(TRUE);
	// track look is set
	m_pctl->SetTrackLook(TRUE);
	// alignment
	m_pctl->SetVerticalAlignment(BS_VCENTER);
	m_pctl->SetHorizontalAlignment(BS_LEFT);
}

CString COXSoundCustomizeDlg::GetSubString(LPCTSTR pszFullString, int nSubIndex, 
										   TCHAR cDelimiter)
	// --- In  : pszFullString : The full string
	//			 nSubIndex : The ONE-based index of the substring requested
	//			 cDelimiter : Delimiter character used between all substrings
	// --- Out : 
	// --- Returns : The requested substring or an empty string otherwise
	// --- Effect : 
{
	ASSERT(0 < nSubIndex);

	CString sSubString;
	if (pszFullString == NULL)
		{
		// Nothing to search : nothing to find
		ASSERT(sSubString.IsEmpty());
		return sSubString;
		}

	// Set pszStart to first charecter and pszEnd after last charecter
	LPCTSTR pszBegin = pszFullString;
	LPCTSTR pszEnd = pszFullString + _tcslen(pszFullString);
	LPCTSTR pszDelimiter = _tcschr(pszBegin, cDelimiter); 
	if (pszDelimiter == NULL)
		pszDelimiter = pszEnd;

	ASSERT((*pszDelimiter == cDelimiter) || (*pszDelimiter == _T('\0')));
	ASSERT(pszBegin <= pszDelimiter);

	while (--nSubIndex != 0)
		{
		if (*pszDelimiter == _T('\0'))
			{
			// Search to end of string and not found
			ASSERT(sSubString.IsEmpty());
			return sSubString;
			}
		pszBegin = pszDelimiter + 1;
		pszDelimiter = _tcschr(pszBegin, cDelimiter); 
		if (pszDelimiter == NULL)
			pszDelimiter = pszEnd;
		ASSERT((*pszDelimiter == cDelimiter) || (*pszDelimiter == _T('\0')));
		ASSERT(pszBegin <= pszDelimiter);
		}

	int nLen = PtrToInt(pszDelimiter) - PtrToInt(pszBegin);
	ASSERT(0 <= nLen);
	LPTSTR pszSubString = sSubString.GetBufferSetLength(nLen);
	::CopyMemory(pszSubString, pszBegin, nLen * sizeof(TCHAR));
	return sSubString;
}

HTREEITEM COXSoundCustomizeDlg::AddEventItem(CWnd* pWnd, 
											 HTREEITEM htiParent, 
											 UINT nMsgID,
											 COXSoundEffectManager* pSoundEffectManager,
											 BOOL bPlayEvent)
{
	CString sEvent=_T("");
	CString sFileName=_T("");
	CString sEventID=_T("");
	CString sLoop=_T("");
	BOOL bIsSoundEditable=FALSE;

	// search for specified sound event ID
	LPCTSTR* ppszText=NULL;
	if(m_allDefinedEvents.Lookup(nMsgID,ppszText))
	{
		ASSERT(ppszText!=NULL);
		sEvent=ppszText[0];
		sEventID=ppszText[1];
	}
	else
	{
		VERIFY(sEvent.LoadString(IDS_OX_SOUNDUNDEFINED));//"Undefined"
		sEventID.Format(_T("ID=%d"),nMsgID);
	}

	LPSOUNDEFFECT pSoundEffect=NULL;
	LPSOUNDSOURCEINFO pSoundSourceInfo=NULL;
	if(bPlayEvent)
	{
		pSoundEffect=pSoundEffectManager->GetSoundEffect(nMsgID);
		ASSERT(pSoundEffect!=NULL);

		COXSound* pSound=pSoundEffect->pSound;
		ASSERT(pSound!=NULL);

		// setup file name combo box
		pSoundSourceInfo=pSound->GetSoundSourceInfo();
		ASSERT(pSoundSourceInfo!=NULL);
		switch(pSoundSourceInfo->source)
		{
		case SNDSRC_FILE:
			sFileName=pSoundSourceInfo->sFileName;
			bIsSoundEditable=TRUE;
			break;
		case SNDSRC_INTRESOURCE:
			sFileName.Format(IDS_OX_SOUNDRESOURCEID,pSoundSourceInfo->nResourceID);
			break;
		case SNDSRC_CFILE:
			VERIFY(sFileName.LoadString(IDS_OX_SOUNDOPENCFILE));//"Opened CFile object"
			break;
		default:
			ASSERT(FALSE);
		}

		// loop type
		if(pSoundEffect->bLoop && pSoundEffect->nLoopCount!=0 && 
			pSoundEffect->nLoopCount!=1)
		{
			if(pSoundEffect->nLoopCount==-1)
				VERIFY(sLoop.LoadString(IDS_OX_SOUNDINFINITELY));//"Infinitely"
			else
			{
				CString sExt;
				VERIFY(sExt.LoadString(IDS_OX_SOUNDEXT));
				sLoop.Format(IDS_OX_SOUNDTIMES/*"%d time%s"*/,pSoundEffect->nLoopCount,
					(pSoundEffect->nLoopCount>1 ? sExt : _T("")));
			}
		}
		else
			VERIFY(sLoop.LoadString(IDS_OX_SOUNDONETIME)); //"One time"
	}

	// insert item
	HTREEITEM hti=m_treeEffect.InsertItem(sEvent,htiParent);
	if(hti==NULL)
		return NULL;

	// customize the inserted tree item
	//
	TV_ITEM tvi;
	memset(&tvi,0,sizeof(tvi));
	tvi.mask=TVIF_IMAGE|TVIF_HANDLE|TVIF_SELECTEDIMAGE;
	tvi.hItem=hti;
	tvi.iImage=bIsSoundEditable ? 3 : 4;
	tvi.iSelectedImage=tvi.iImage;
	VERIFY(m_treeEffect.SetItem(&tvi));

	// setup subitems
	VERIFY(m_treeEffect.SetSubItem(hti,1,OX_SUBITEM_TEXT|OX_SUBITEM_COLOR,
		sFileName,0,NULL,RGB(0,128,0)));
	VERIFY(m_treeEffect.SetItemDrawEllipsis(hti,DT_PATH_ELLIPSIS,1));

	VERIFY(m_treeEffect.SetSubItem(hti,2,OX_SUBITEM_TEXT|OX_SUBITEM_COLOR,
		sEventID,0,NULL,RGB(0,0,128)));
	
	VERIFY(m_treeEffect.SetSubItem(hti,3,OX_SUBITEM_TEXT|OX_SUBITEM_COLOR,
		sLoop,0,NULL,RGB(255,0,0)));

	// create new item description
	LPITEMINFO pItemInfo=new ITEMINFO;
	pItemInfo->type=bPlayEvent ? IT_PLAYEVENT : IT_STOPEVENT;
	pItemInfo->pWnd=pWnd;
	pItemInfo->nMsgID=nMsgID;
	if(pSoundEffect!=NULL)
	{
		pItemInfo->pSoundEffect=new SOUNDEFFECT;
		*pItemInfo->pSoundEffect=*pSoundEffect;
	}
	if(pSoundSourceInfo!=NULL)
	{
		pItemInfo->pSoundSourceInfo=new SOUNDSOURCEINFO;
		*pItemInfo->pSoundSourceInfo=*pSoundSourceInfo;
	}
	pItemInfo->dwControlState=CTL_OK|CTL_CANCEL|CTL_TREE|CTL_REMOVE|CTL_ADD|CTL_EVENT;
	if(bPlayEvent)
	{
		pItemInfo->dwControlState|=CTL_LOOPTYPE|CTL_FORCETOSTART;
		if(bIsSoundEditable)
			pItemInfo->dwControlState|=CTL_FILENAME|CTL_PLAY;
	}
	m_allTreeItems.SetAt((HANDLE)hti,pItemInfo);

	return hti;
}

void COXSoundCustomizeDlg::OnItemchangedExttreeEffectslist(NMHDR* pNMHDR, 
														   LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	if((pNMListView->uChanged&LVIF_STATE)!=0)
	{
		if(((pNMListView->uNewState&LVIS_SELECTED)==0 && 
			(pNMListView->uOldState&LVIS_SELECTED)!=0) || 
			((pNMListView->uNewState&LVIS_SELECTED)!=0 && 
			(pNMListView->uOldState&LVIS_SELECTED)==0))
			PostMessage(SMDM_UPDATEVARS);
	}

	*pResult = 0;
}

void COXSoundCustomizeDlg::OnItemExpandingExttreeEffectslist(NMHDR* pNMHDR, 
															 LRESULT* pResult) 
{
	LPNMTREEVIEW pnmtv=(LPNMTREEVIEW)pNMHDR;
	// TODO: Add your control notification handler code here

	// toggling is on/off depending on the specified at the creation time style
	if((m_dwDlgStyle&SCDS_NOTOGGLE)!=0)
	{
		if(pnmtv->action==TVE_EXPAND)
			*pResult=0;
		else
			*pResult=1;
	}
	else
		*pResult=0;
}

void COXSoundCustomizeDlg::ShowControls(DWORD dwControlState)
{
	if((m_dwDlgStyle&SCDS_READONLY)!=0)
		dwControlState&=~(CTL_FILENAME|CTL_EVENT|CTL_SAVE|CTL_RESTORE|
			CTL_REMOVE|CTL_ADD|CTL_FORCETOSTART|CTL_LOOPTYPE);

	if((dwControlState&CTL_TREE)!=(m_dwControlState&CTL_TREE))
		m_treeEffect.EnableWindow((dwControlState&CTL_TREE));

	if((dwControlState&CTL_OK)!=(m_dwControlState&CTL_OK))
		m_btnOk.EnableWindow((dwControlState&CTL_OK));

	if((dwControlState&CTL_CANCEL)!=(m_dwControlState&CTL_CANCEL))
		m_btnCancel.EnableWindow((dwControlState&CTL_CANCEL));

	if((dwControlState&CTL_FILENAME)!=(m_dwControlState&CTL_FILENAME))
		m_cmbFileName.EnableWindow((dwControlState&CTL_FILENAME));

	if((dwControlState&CTL_EVENT)!=(m_dwControlState&CTL_EVENT))
		m_cmbEvent.EnableWindow((dwControlState&CTL_EVENT));
	
	if((dwControlState&CTL_STOP)!=(m_dwControlState&CTL_STOP))
		m_btnStop.EnableWindow((dwControlState&CTL_STOP));
	
	if((dwControlState&CTL_SAVE)!=(m_dwControlState&CTL_SAVE))
		m_btnSave.EnableWindow((dwControlState&CTL_SAVE));
	
	if((dwControlState&CTL_RESTORE)!=(m_dwControlState&CTL_RESTORE))
		m_btnRestore.EnableWindow((dwControlState&CTL_RESTORE));

	if((dwControlState&CTL_REMOVE)!=(m_dwControlState&CTL_REMOVE))
		m_btnRemove.EnableWindow((dwControlState&CTL_REMOVE));

	if((dwControlState&CTL_PLAY)!=(m_dwControlState&CTL_PLAY))
		m_btnPlay.EnableWindow((dwControlState&CTL_PLAY));

	if((dwControlState&CTL_ADD)!=(m_dwControlState&CTL_ADD))
		m_btnAdd.EnableWindow((dwControlState&CTL_ADD));

	if((dwControlState&CTL_FORCETOSTART)!=(m_dwControlState&CTL_FORCETOSTART))
		m_btnForceToStart.EnableWindow((dwControlState&CTL_FORCETOSTART));

	if((dwControlState&CTL_LOOPTYPE)!=(m_dwControlState&CTL_LOOPTYPE))
	{
		GetDlgItem(IDC_OX_RADIO_LOOPONCE_OXSOUND)->EnableWindow((dwControlState&CTL_LOOPTYPE));
		GetDlgItem(IDC_OX_RADIO_LOOPINFINITELY_OXSOUND)->EnableWindow((dwControlState&CTL_LOOPTYPE));
		GetDlgItem(IDC_OX_RADIO_LOOPREPEAT_OXSOUND)->EnableWindow((dwControlState&CTL_LOOPTYPE));
	}

	m_editLoopCount.EnableWindow(((dwControlState&CTL_LOOPTYPE)!=0)&(m_nLoopType==2));

	DWORD dwStyle=m_btnForceToStart.GetStyle();
	if((dwStyle&0x0f)!=BS_AUTO3STATE && 
		(m_bForceToStart!=FALSE && m_bForceToStart!=TRUE))
		VERIFY(::SetWindowLongPtr(m_btnForceToStart.GetSafeHwnd(),GWL_STYLE,
			dwStyle+(BS_AUTO3STATE-BS_AUTOCHECKBOX)));
	else if((dwStyle&0x0f)==BS_AUTO3STATE && 
		(m_bForceToStart==FALSE || m_bForceToStart==TRUE))
		VERIFY(::SetWindowLongPtr(m_btnForceToStart.GetSafeHwnd(),GWL_STYLE,
			dwStyle-(BS_AUTO3STATE-BS_AUTOCHECKBOX)));

	m_dwControlState=dwControlState;
}


void COXSoundCustomizeDlg::StartEditItem(BOOL bAddingNew/*=FALSE*/) 
{
	// make sure we are not editing any item
	if(!m_bEditing)
	{
		m_bEditing=TRUE;
		m_bAddingNew=bAddingNew;

		DWORD dwControlState=m_dwControlState;

		dwControlState&=~(CTL_TREE|CTL_ADD);
		dwControlState|=(CTL_SAVE|CTL_RESTORE);

		ShowControls(dwControlState);
	}
}

BOOL COXSoundCustomizeDlg::VerifySoundFile(CString sFileName, BOOL bReportError) 
{
	// if COXSound::Open fail then there is something wrong with the specified
	// sound source
	BOOL bResult=m_TestSound.Open(sFileName);
	if(!bResult && bReportError)
	{
		CString sMessage;
		sMessage.Format(IDS_OX_SOUNDWRONGFILE,sFileName);//"Specified %s sound file doesn't exist or is not valid WAV object"
		AfxMessageBox(sMessage,MB_OK|MB_ICONEXCLAMATION);
	}

	if(!bResult)
		TRACE(_T("COXSoundCustomizeDlg::VerifySoundFile: failed to verify sound file"));

	return bResult;
}

BOOL COXSoundCustomizeDlg::VerifyEvent(CString sEvent, CWnd* pWnd, BOOL bReportError)
{
	BOOL bResult=FALSE;

	// sound vent should be defined by default or set using 
	// COXSoundCustomizeDlg::SetEvent function
	UINT nMsgID=SNDEVENT_NONE;
	CString sEventCompare;
	LPCTSTR* ppszText;
	POSITION pos=m_allDefinedEvents.GetStartPosition();
	while(pos!=NULL)
	{
		m_allDefinedEvents.GetNextAssoc(pos,nMsgID,ppszText);
		ASSERT(ppszText!=NULL);
		ASSERT(nMsgID!=SNDEVENT_NONE);
		sEventCompare=ppszText[0];
		if(sEvent==sEventCompare)
		{
			bResult=TRUE;
			break;
		}
	}

	CString sMessage;
	if(!bResult)
	{
		sMessage.Format(IDS_OX_SOUNDSPECEVENT,sEvent);//"Specified %s event wasn't defined"
	}
	else
	{
		// check if specified sound event wasn't registered before
		HTREEITEM hti=m_treeEffect.GetRootItem();
		ASSERT(hti!=NULL);
		COXTreeItem* xti=m_treeEffect.GetXItem(hti);
		ASSERT(xti);
		while((xti=xti->GetNextInTree())!=NULL)
		{
			hti=(HTREEITEM)xti;
			LPITEMINFO pItemInfo=NULL;
			VERIFY(m_allTreeItems.Lookup(hti,pItemInfo));

			if(pItemInfo->pWnd==pWnd && pItemInfo->nMsgID==nMsgID &&
				(pItemInfo->type==IT_PLAYEVENT || pItemInfo->type==IT_STOPEVENT))
			{
				bResult=FALSE;
				if(pItemInfo->type==IT_PLAYEVENT)
				{
					ASSERT(pItemInfo->pSoundEffect!=NULL);
					ASSERT(pItemInfo->pSoundSourceInfo!=NULL);
	
					sMessage.Format(IDS_OX_SOUNDEVENTREGRED,sEvent);//"Specified %s event is already registered as start playing event"
				}
				else
					sMessage.Format(IDS_OX_SOUNDEVENTREGSTOP,sEvent);//"Specified %s event is already registered as stop playing event"
				break;
			}
		}
	}

	if(!bResult && bReportError)
		AfxMessageBox(sMessage,MB_OK|MB_ICONEXCLAMATION);

	if(!bResult)
		TRACE(_T("COXSoundCustomizeDlg::VerifyEvent: failed to verify event"));

	return bResult;
}

BOOL COXSoundCustomizeDlg::VerifyLoopType(int nLoopType, int nLoopCount, 
										  BOOL bReportError)
{
	UNREFERENCED_PARAMETER(bReportError);
	UNUSED(nLoopType);
	UNUSED(nLoopCount);

	ASSERT(nLoopType>=0 && nLoopType<=2 && nLoopCount>=0);

	return TRUE;
}

BOOL COXSoundCustomizeDlg::VerifyForceToStart(BOOL bForceToStart,	BOOL bReportError)
{
	UNREFERENCED_PARAMETER(bReportError);

	BOOL bResult=((bForceToStart==FALSE) | (bForceToStart==TRUE));
	if(!bResult && bReportError)
	{
		CString sMessage;
		VERIFY(sMessage.LoadString(IDS_OX_SOUND_FORCEPLAYASSIGNED));//"<Force to start playing> has to be assigned TRUE or FALSE value"
		AfxMessageBox(sMessage,MB_OK|MB_ICONEXCLAMATION);
	}

	if(!bResult)
		TRACE(_T("COXSoundCustomizeDlg::VerifyForceToStart: failed to verify <Force to start playing> value"));

	return bResult;
}

LRESULT COXSoundCustomizeDlg::OnPlaybackComplete(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	DWORD dwControlState=m_dwControlState;

	dwControlState&=~(CTL_STOP);
	dwControlState|=(CTL_FILENAME|CTL_PLAY);

	ShowControls(dwControlState);

	return (LRESULT)0;
}

void COXSoundCustomizeDlg::OnCheckForce() 
{
	// TODO: Add your control notification handler code here
	
	StartEditItem();
}

void COXSoundCustomizeDlg::OnEditchangeComboEvent() 
{
	// TODO: Add your control notification handler code here
	
	StartEditItem();
}

void COXSoundCustomizeDlg::OnSelchangeComboEvent() 
{
	// TODO: Add your control notification handler code here
	
	if(m_cmbEvent.GetCurSel()!=m_nEvent)
		StartEditItem();
}

void COXSoundCustomizeDlg::OnEditchangeComboFilename() 
{
	// TODO: Add your control notification handler code here
	
	StartEditItem();
}

void COXSoundCustomizeDlg::OnSelchangeComboFilename() 
{
	// TODO: Add your control notification handler code here
	
	CString sText=m_sFileName;
	int nCurSel=m_cmbFileName.GetCurSel();
	if(nCurSel!=CB_ERR)
		m_cmbFileName.GetLBText(nCurSel,sText);
	if(sText!=m_sFileName)
		StartEditItem();
}

void COXSoundCustomizeDlg::OnChangeEditLoopcount() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_CHANGE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
	StartEditItem();
}

void COXSoundCustomizeDlg::OnRadioLoopinfinitely() 
{
	// TODO: Add your control notification handler code here

	if(m_nLoopType!=1)
		StartEditItem();

	m_editLoopCount.EnableWindow(FALSE);
}

void COXSoundCustomizeDlg::OnRadioLooponce() 
{
	// TODO: Add your control notification handler code here
	
	if(m_nLoopType!=0)
		StartEditItem();

	m_editLoopCount.EnableWindow(FALSE);
}

void COXSoundCustomizeDlg::OnRadioLooprepeat() 
{
	// TODO: Add your control notification handler code here
	
	if(m_nLoopType!=3)
		StartEditItem();

	m_editLoopCount.EnableWindow(TRUE);
}

void COXSoundCustomizeDlg::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here

	// it can happen only if one item in the tree is selected
	ASSERT(m_treeEffect.GetSelectedCount()==1);

	// get selected item in order to define whether start or stop playing event
	// will be added
	HTREEITEM htiSelected=m_treeEffect.GetNextItem(m_treeEffect.GetRootItem(),
		TVGN_FIRSTSELECTED);
	ASSERT(htiSelected!=NULL);

	LPITEMINFO pItemInfo;
	VERIFY(m_allTreeItems.Lookup(htiSelected,pItemInfo));
	ASSERT(pItemInfo!=NULL);

	ASSERT(pItemInfo->type==IT_PLAYEVENT || pItemInfo->type==IT_STOPEVENT ||
		pItemInfo->type==IT_PLAYEVENTLIST || pItemInfo->type==IT_STOPEVENTLIST);
		
	if((pItemInfo->type==IT_PLAYEVENTLIST || pItemInfo->type==IT_STOPEVENTLIST))
	{
		m_nEvent=0;
		m_sFileName=_T("");
		m_bForceToStart=TRUE;
		m_nLoopCount=0;
		m_nLoopType=0;
	
		UpdateData(FALSE);

		DWORD dwControlState=CTL_OK|CTL_CANCEL;
		if(pItemInfo->type==IT_PLAYEVENTLIST)
			dwControlState|=CTL_EVENT|CTL_FILENAME|CTL_PLAY|CTL_LOOPTYPE|
				CTL_FORCETOSTART|CTL_REMOVE;
		else
			dwControlState|=CTL_EVENT|CTL_ADD|CTL_REMOVE;

		ShowControls(dwControlState);
	}

	StartEditItem(TRUE);
}

void COXSoundCustomizeDlg::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	
	CString sFileName;
	m_cmbFileName.GetWindowText(sFileName);
	ASSERT(!m_TestSound.IsPlaying());
	if(m_TestSound.Open(sFileName))
	{
		if(m_TestSound.Play())
		{
			DWORD dwControlState=m_dwControlState;

			dwControlState|=(CTL_STOP);
			dwControlState&=~(CTL_FILENAME|CTL_PLAY);

			ShowControls(dwControlState);
		}
	}
}

void COXSoundCustomizeDlg::OnButtonRemove() 
{
	// TODO: Add your control notification handler code here
	
	HTREEITEM htiSelected=m_treeEffect.GetNextItem(m_treeEffect.GetRootItem(),
		TVGN_FIRSTSELECTED);
	ASSERT(htiSelected!=NULL);

	LPITEMINFO pItemInfo;
	VERIFY(m_allTreeItems.Lookup(htiSelected,pItemInfo));
	ASSERT(pItemInfo!=NULL);

	switch(pItemInfo->type)
	{
	case IT_WINDOW:
		{
			// if item is on the window level then we remove all start and stop
			// playing events registered with the window
			HTREEITEM htiPlayEventsList=
				m_treeEffect.GetNextItem(htiSelected,TVGN_CHILD);
			ASSERT(htiPlayEventsList!=NULL);
			HTREEITEM hti;
			while((hti=m_treeEffect.GetNextItem(htiPlayEventsList,TVGN_CHILD))!=NULL)
				VERIFY(m_treeEffect.DeleteItem(hti));

			HTREEITEM htiStopEventsList=
				m_treeEffect.GetNextSiblingItem(htiPlayEventsList);
			ASSERT(htiStopEventsList!=NULL);
			while((hti=m_treeEffect.GetNextItem(htiStopEventsList,TVGN_CHILD))!=NULL)
				VERIFY(m_treeEffect.DeleteItem(hti));
			break;
		}
	case IT_PLAYEVENT:
	case IT_STOPEVENT:
		{
			VERIFY(m_treeEffect.DeleteItem(htiSelected));
			ShowControls(CTL_TREE|CTL_OK|CTL_CANCEL);
			break;
		}
	case IT_PLAYEVENTLIST:
	case IT_STOPEVENTLIST:
		{
			HTREEITEM hti;
			while((hti=m_treeEffect.GetNextItem(htiSelected,TVGN_CHILD))!=NULL)
				VERIFY(m_treeEffect.DeleteItem(hti));
			break;
		}
	}
}

void COXSoundCustomizeDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	
	if(m_TestSound.IsPlaying())
		m_TestSound.Stop();

	int nOldEvent=m_nEvent;
	CString sOldFileName=m_sFileName;
	BOOL bOldForceToStart=m_bForceToStart;
	int nOldLoopCount=m_nLoopCount;
	int nOldLoopType=m_nLoopType;

	VERIFY(UpdateData());

	CString sEvent;
	m_cmbEvent.GetLBText(m_nEvent,0,sEvent);

	BOOL bSaveEvent=((m_dwControlState&CTL_EVENT)!=0) & 
		(nOldEvent!=m_nEvent || m_bAddingNew);
	BOOL bSaveFileName=((m_dwControlState&CTL_FILENAME)!=0) & 
		(sOldFileName!=m_sFileName || m_bAddingNew);
	BOOL bSaveLoopType=((m_dwControlState&CTL_LOOPTYPE)!=0) & 
		(nOldLoopType!=m_nLoopType || m_bAddingNew || 
		(m_nLoopType==2 && nOldLoopCount!=m_nLoopCount));
	BOOL bSaveForceToStart=((m_dwControlState&CTL_FORCETOSTART)!=0) &
		(bOldForceToStart!=m_bForceToStart || m_bAddingNew);

	HTREEITEM htiSelected=m_treeEffect.GetNextItem(m_treeEffect.GetRootItem(),
		TVGN_FIRSTSELECTED);
	ASSERT(htiSelected!=NULL);

	LPITEMINFO pItemInfo;
	VERIFY(m_allTreeItems.Lookup(htiSelected,pItemInfo));
	ASSERT(pItemInfo!=NULL);

	// verified changed info
	if(bSaveEvent && !VerifyEvent(sEvent,pItemInfo->pWnd,TRUE))
		return;
	if(pItemInfo->type==IT_PLAYEVENT || (m_bAddingNew && 
		pItemInfo->type==IT_PLAYEVENTLIST))
		if((bSaveFileName && !VerifySoundFile(m_sFileName,TRUE)) || 
			(bSaveLoopType && !VerifyLoopType(m_nLoopType,m_nLoopCount,TRUE)) || 
			(bSaveForceToStart && !VerifyForceToStart(m_bForceToStart,TRUE)))
			return;
	

	if(m_bAddingNew)
	{
		// add new tree item if needed
		//
		ASSERT(m_treeEffect.GetSelectedCount()==1);

		ASSERT(pItemInfo->type==IT_PLAYEVENT || pItemInfo->type==IT_STOPEVENT ||
			pItemInfo->type==IT_PLAYEVENTLIST || pItemInfo->type==IT_STOPEVENTLIST);
		
		LPITEMINFO pNewItemInfo=new ITEMINFO;
		*pNewItemInfo=*pItemInfo;
		pItemInfo=pNewItemInfo;

		HTREEITEM htiParent=NULL;
		if(pItemInfo->type==IT_PLAYEVENTLIST || pItemInfo->type==IT_STOPEVENTLIST)
			htiParent=htiSelected;
		else
			htiParent=m_treeEffect.GetParentItem(htiSelected);
		ASSERT(htiParent!=NULL);

		htiSelected=m_treeEffect.InsertItem(_T(""),htiParent);
		ASSERT(htiSelected!=NULL);
	
		if(pItemInfo->type==IT_PLAYEVENTLIST) 
		{
			pItemInfo->type=IT_PLAYEVENT;
			pItemInfo->pSoundSourceInfo->source=SNDSRC_FILE;
		}
		else if(pItemInfo->type==IT_STOPEVENTLIST) 
			pItemInfo->type=IT_STOPEVENT;

		TV_ITEM tvi;
		memset(&tvi,0,sizeof(tvi));
		tvi.mask=TVIF_IMAGE|TVIF_HANDLE|TVIF_SELECTEDIMAGE;
		tvi.hItem=htiSelected;
		tvi.iImage=pItemInfo->type==IT_PLAYEVENT ? 3 : 4;
		tvi.iSelectedImage=tvi.iImage;
		VERIFY(m_treeEffect.SetItem(&tvi));

		VERIFY(m_treeEffect.SetSubItem(htiSelected,1,OX_SUBITEM_COLOR,
			NULL,0,NULL,RGB(0,128,0)));
		VERIFY(m_treeEffect.SetItemDrawEllipsis(htiSelected,DT_PATH_ELLIPSIS,1));

		VERIFY(m_treeEffect.SetSubItem(htiSelected,2,OX_SUBITEM_COLOR,
			NULL,0,NULL,RGB(0,0,128)));

		VERIFY(m_treeEffect.SetSubItem(htiSelected,3,OX_SUBITEM_COLOR,
			NULL,0,NULL,RGB(255,0,0)));

		pItemInfo->dwControlState=
			CTL_TREE|CTL_OK|CTL_CANCEL|CTL_REMOVE|CTL_ADD|CTL_EVENT;
		if(pItemInfo->type==IT_PLAYEVENT)
		{
			pItemInfo->dwControlState|=CTL_LOOPTYPE|CTL_FORCETOSTART|
				CTL_FILENAME|CTL_PLAY;
		}

		m_allTreeItems.SetAt((HANDLE)htiSelected,pItemInfo);
	}
	else
	{
		ASSERT(pItemInfo->type==IT_PLAYEVENT || pItemInfo->type==IT_STOPEVENT);
	}

	while(htiSelected!=NULL)
	{
		// update item and subitems text
		//
		if(bSaveEvent)
		{
			TV_ITEM tvi;
			memset(&tvi,0,sizeof(tvi));
			tvi.mask=TVIF_HANDLE|TVIF_TEXT;
			tvi.hItem=htiSelected;
			tvi.pszText=sEvent.GetBuffer(sEvent.GetLength());
			VERIFY(m_treeEffect.SetItem(&tvi));
			sEvent.ReleaseBuffer();

			CString sEventID;
			m_cmbEvent.GetLBText(m_nEvent,1,sEventID);
			VERIFY(m_treeEffect.SetSubItem(htiSelected,2,OX_SUBITEM_TEXT,sEventID));

			pItemInfo->nMsgID=0;
			UINT nMsgID;
			CString sEventIDCompare;
			LPCTSTR* ppszText;
			POSITION pos=m_allDefinedEvents.GetStartPosition();
			while(pos!=NULL)
			{
				m_allDefinedEvents.GetNextAssoc(pos,nMsgID,ppszText);
				ASSERT(ppszText!=NULL);
				ASSERT(nMsgID!=SNDEVENT_NONE);
				sEventIDCompare=ppszText[1];
				if(sEventID==sEventIDCompare)
				{
					pItemInfo->nMsgID=nMsgID;
					break;
				}	
			}
		}

		if(pItemInfo->type==IT_PLAYEVENT)
		{
			ASSERT(pItemInfo->pSoundEffect!=NULL);
			ASSERT(pItemInfo->pSoundSourceInfo!=NULL);
		}

		if(bSaveFileName)
		{
			ASSERT(pItemInfo->type==IT_PLAYEVENT);
			ASSERT(pItemInfo->pSoundSourceInfo->source==SNDSRC_FILE);

			pItemInfo->pSoundSourceInfo->sFileName=m_sFileName;
			VERIFY(m_treeEffect.SetSubItem(htiSelected,1,OX_SUBITEM_TEXT,m_sFileName));
		}
	
		if(bSaveLoopType)
		{
			ASSERT(pItemInfo->type==IT_PLAYEVENT);
			CString sLoop;
			pItemInfo->pSoundEffect->bLoop=TRUE;
			if(m_nLoopType==0 || (m_nLoopType==2 && 
				(m_nLoopCount==0 || m_nLoopCount==1)))
			{
				VERIFY(sLoop.LoadString(IDS_OX_SOUNDONETIME));//"One time"
				pItemInfo->pSoundEffect->bLoop=FALSE;
				pItemInfo->pSoundEffect->nLoopCount=0;
			}
			else if(m_nLoopType==1)
			{
				VERIFY(sLoop.LoadString(IDS_OX_SOUNDINFINITELY));//"Infinitely"
				pItemInfo->pSoundEffect->nLoopCount=-1;
			}
			else if(m_nLoopType==2)
			{
				CString sExt;
				VERIFY(sExt.LoadString(IDS_OX_SOUNDEXT));
				sLoop.Format(IDS_OX_SOUNDTIMES,m_nLoopCount,//"%d time%s"
					(m_nLoopCount>1 ? sExt : _T("")));
				pItemInfo->pSoundEffect->nLoopCount=m_nLoopCount;
			}
			else
				ASSERT(FALSE);
			VERIFY(m_treeEffect.SetSubItem(htiSelected,3,OX_SUBITEM_TEXT,sLoop));
		}

		if(bSaveForceToStart)
		{
			ASSERT(pItemInfo->type==IT_PLAYEVENT);
			pItemInfo->pSoundEffect->bForceToStart=m_bForceToStart;
		}

		if(!m_bAddingNew)
		{
			htiSelected=m_treeEffect.GetNextItem(htiSelected,TVGN_NEXTSELECTED);
			if(htiSelected!=NULL)
			{
				VERIFY(m_allTreeItems.Lookup(htiSelected,pItemInfo));
				ASSERT(pItemInfo!=NULL);
			}
		}
		else
			break;
	}

	m_bEditing=FALSE;

	if(m_bAddingNew)
	{
		m_bAddingNew=FALSE;
		m_treeEffect.EnsureVisible(htiSelected);
		m_treeEffect.SelectItem(NULL);
		m_treeEffect.SelectItem(htiSelected);
	}
	else
		GetVars();
}

void COXSoundCustomizeDlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here

	m_TestSound.Stop();
	OnPlaybackComplete(0,0);
}

void COXSoundCustomizeDlg::OnButtonRestore() 
{
	// TODO: Add your control notification handler code here
	
	if(m_TestSound.IsPlaying())
		m_TestSound.Stop();

	UpdateData(FALSE);

	m_bEditing=FALSE;
	m_bAddingNew=FALSE;

	GetVars();
}

void COXSoundCustomizeDlg::OnOK() 
{
	// TODO: Add extra validation here

	if(m_TestSound.IsPlaying())
		m_TestSound.Stop();

	if((m_dwDlgStyle&SCDS_SAVESTATE)!=0)
		SaveState();

	if((m_dwDlgStyle&SCDS_READONLY)!=0)
		CDialog::OnOK();

	if(m_bEditing)
	{
		CString sMsg;
		VERIFY(sMsg.LoadString(IDS_OX_SOUNDCURRENTLYEDITING));//"You are currently editing a sound event.\n Would you like to save changes before exit?"
		if(AfxMessageBox(sMsg,MB_YESNO|MB_ICONQUESTION)==IDYES)
			OnButtonSave();
	}

	m_pSoundEffectOrganizer->StopPlaying();

	// reregister all stop events
	m_pSoundEffectOrganizer->UnregisterAllStopEvents();
	// reregister all start playing events that use a file or resource ID 
	// as sound source
	POSITION pos=m_pSoundEffectOrganizer->GetFirstRegisteredWindow();
	while(pos!=NULL)
	{
		CWnd* pWnd=m_pSoundEffectOrganizer->GetNextRegisteredWindow(pos);
		ASSERT(pWnd);
		ASSERT(pWnd->GetSafeHwnd());

		COXSoundEffectManager* pSoundEffectManager=
			m_pSoundEffectOrganizer->GetManager(pWnd);
		ASSERT(pSoundEffectManager);

		POSITION posEvent=pSoundEffectManager->GetFirstPlayEvent();
		while(posEvent!=NULL)
		{
			UINT nPlayMsgID=pSoundEffectManager->GetNextPlayEvent(posEvent);
			ASSERT(nPlayMsgID!=SNDEVENT_NONE);
			
			LPSOUNDEFFECT pSoundEffect=pSoundEffectManager->
				GetSoundEffect(nPlayMsgID);
			ASSERT(pSoundEffect!=NULL);
			ASSERT(pSoundEffect->pSound!=NULL);
			LPSOUNDSOURCEINFO pSoundSourceInfo=pSoundEffect->pSound->
				GetSoundSourceInfo();
			ASSERT(pSoundSourceInfo!=NULL);
			ASSERT(pSoundSourceInfo->source!=SNDSRC_UNKNOWN);
			if(pSoundSourceInfo->source==SNDSRC_FILE || 
				pSoundSourceInfo->source==SNDSRC_INTRESOURCE)
				pSoundEffectManager->UnregisterPlayEvent(nPlayMsgID);
		}
	}


	// register all stop playing and sound events all start playing events 
	// that use a file or resource ID as sound source from the tree
	HTREEITEM hti=m_treeEffect.GetRootItem();
	ASSERT(hti!=NULL);
	COXTreeItem* xti=m_treeEffect.GetXItem(hti);
	ASSERT(xti);
	while((xti=xti->GetNextInTree())!=NULL)
	{
		hti=(HTREEITEM)xti;
		LPITEMINFO pItemInfo=NULL;
		VERIFY(m_allTreeItems.Lookup(hti,pItemInfo));

		if(pItemInfo->type==IT_PLAYEVENT || pItemInfo->type==IT_STOPEVENT)
		{
			ASSERT(pItemInfo->type!=IT_UNKNOWN);
			ASSERT(pItemInfo->pWnd!=NULL);
			ASSERT(pItemInfo->nMsgID!=SNDEVENT_NONE);
			ASSERT(::IsWindow(pItemInfo->pWnd->GetSafeHwnd()));

			if(pItemInfo->type==IT_PLAYEVENT)
			{
				ASSERT(pItemInfo->pSoundEffect!=NULL);
				ASSERT(pItemInfo->pSoundSourceInfo!=NULL);
	
				switch(pItemInfo->pSoundSourceInfo->source)
				{
				case SNDSRC_FILE:
					VERIFY(m_pSoundEffectOrganizer->
						RegisterPlayEvent(pItemInfo->pWnd,pItemInfo->nMsgID,
						pItemInfo->pSoundSourceInfo->sFileName,
						pItemInfo->pSoundEffect->bLoop,
						pItemInfo->pSoundEffect->nLoopCount,
						pItemInfo->pSoundEffect->bForceToStart));
					break;
				case SNDSRC_INTRESOURCE:
					VERIFY(m_pSoundEffectOrganizer->
						RegisterPlayEvent(pItemInfo->pWnd,pItemInfo->nMsgID,
						pItemInfo->pSoundSourceInfo->nResourceID,
						pItemInfo->pSoundSourceInfo->hInstance,
						pItemInfo->pSoundEffect->bLoop,
						pItemInfo->pSoundEffect->nLoopCount,
						pItemInfo->pSoundEffect->bForceToStart));
					break;
				case SNDSRC_CFILE:
					break;
				default:
					ASSERT(FALSE);
				}
			}
			else if(pItemInfo->type==IT_STOPEVENT)
			{
				VERIFY(m_pSoundEffectOrganizer->
					RegisterStopEvent(pItemInfo->pWnd,pItemInfo->nMsgID));
			}
		}
	}
	
	CDialog::OnOK();
}

void COXSoundCustomizeDlg::OnCancel() 
{
	// TODO: Add extra validation here

	if(m_TestSound.IsPlaying())
		m_TestSound.Stop();

	if(m_bEditing)
	{
		CString sMsg;
		VERIFY(sMsg.LoadString(IDS_OX_SOUNDCURRENTLYEDITING));//"You are currently editing a sound event.\n Would you like to save changes before exit?"
		if(AfxMessageBox(sMsg,MB_YESNO|MB_ICONQUESTION)==IDYES)
			return;
	}

	if((m_dwDlgStyle&SCDS_SAVESTATE)!=0)
		SaveState();

	CDialog::OnCancel();
}

void COXSoundCustomizeDlg::SaveState()
{
	ASSERT(::IsWindow(m_treeEffect.GetSafeHwnd()));

	CWinApp* pApp=AfxGetApp();
	if(pApp!=NULL)
	{
		pApp->WriteProfileInt(_T("Sound Customize Dialog"),
			_T("EventColWidth"),m_treeEffect.GetColumnWidth(0));
		pApp->WriteProfileInt(_T("Sound Customize Dialog"),
			_T("FileNameColWidth"),m_treeEffect.GetColumnWidth(1));
		pApp->WriteProfileInt(_T("Sound Customize Dialog"),
			_T("EventInfoColWidth"),m_treeEffect.GetColumnWidth(2));
		pApp->WriteProfileInt(_T("Sound Customize Dialog"),
			_T("LoopColWidth"),m_treeEffect.GetColumnWidth(3));

		pApp->WriteProfileInt(_T("Sound Customize Dialog"),
			_T("SortAscending"),(m_treeEffect.GetSortOrder()==1 ? TRUE : FALSE));
		pApp->WriteProfileInt(_T("Sound Customize Dialog"),
			_T("SortCol"),m_treeEffect.GetSortCol());
	}
}
