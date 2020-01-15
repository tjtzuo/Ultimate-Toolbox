// OXNotesEditView.cpp : implementation of the COXNotesEditView class
//
// Version: 9.3


#include "stdafx.h"

#include "OXNotesEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXNotesEditView

IMPLEMENT_DYNCREATE(COXNotesEditView, CEditView)

BEGIN_MESSAGE_MAP(COXNotesEditView, CEditView)
	//{{AFX_MSG_MAP(COXNotesEditView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_VSCROLL()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_CHAR()
	ON_MESSAGE(EM_SETMARGINS, OnSetMargins)
	ON_MESSAGE(EM_GETMARGINS, OnGetMargins)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXNotesEditView construction/destruction

COXNotesEditView::COXNotesEditView(UINT nSide, UINT nWidth,
								   UINT nMode)
{
	
	m_clrLines=m_clrFont=::GetSysColor(COLOR_WINDOWTEXT);
	m_nMargins=0;
	m_nSide=nSide;
	if (nWidth==OX_NOTES_WIDTH_DEFAULT)
		m_nNotesWidth=::GetSystemMetrics(SM_CXVSCROLL);
	else
		m_nNotesWidth=nWidth;
	m_bUpdatingMargins=FALSE;
	m_nPosition=NONE;
	m_nMode=nMode;
	m_pLinesFont=NULL;
}

COXNotesEditView::~COXNotesEditView()
{
	::DeleteObject(m_hNotesCursor);
	if (m_pLinesFont)
	{
		m_pLinesFont->DeleteObject();
		delete m_pLinesFont;
	}
}



/////////////////////////////////////////////////////////////////////////////
// COXNotesEditView diagnostics

#ifdef _DEBUG
void COXNotesEditView::AssertValid() const
{
	CEditView::AssertValid();
}

void COXNotesEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COXNotesEditView message handlers



int COXNotesEditView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(OX_NOTES_BRUSH));
	VERIFY(m_bshNotes.CreatePatternBrush(&bmp));
	bmp.DeleteObject();

	m_hNotesCursor=::LoadCursor(/*AfxGetApp()->m_hInstance*/
		AfxGetInstanceHandle(),(LPCTSTR) 
		MAKEINTRESOURCE(OX_CURSOR_NOTES));
	ASSERT(m_hNotesCursor);
	
	LoadNotesImageList();

	VERIFY(m_bshClient.CreateSolidBrush(::GetSysColor(COLOR_WINDOW)));
	CEdit& edt=GetEditCtrl();
	m_bUpdatingMargins=TRUE;
	m_nMargins=edt.GetMargins();

	switch (m_nSide)
	{
	case SIDE_LEFT:
		edt.SetMargins(LOWORD(m_nMargins)+m_nNotesWidth,
			HIWORD(m_nMargins));
		break;
	case SIDE_RIGHT:
		edt.SetMargins(LOWORD(m_nMargins),
			HIWORD(m_nMargins)+m_nNotesWidth);
		break;
	}
	m_bUpdatingMargins=FALSE;

	::SetClassLong(m_hWnd, GCL_HCURSOR,NULL);
	return NULL;
}

BOOL COXNotesEditView::OnEraseBkgnd(CDC* pDC) 
{
	CRect rctClient;

	GetClientRect(&rctClient);
	CRect rctNotes(rctClient);

	switch (m_nSide)
	{
	case SIDE_LEFT:
		rctNotes.right=min(m_nNotesWidth,(UINT) rctNotes.right);
		rctClient.left=rctNotes.right;
		break;
	case SIDE_RIGHT:
		rctNotes.left=max(0, rctNotes.right-m_nNotesWidth);
		rctClient.right=rctNotes.left;
		break;
	default:
		rctNotes.SetRectEmpty();
	}
	if (!rctClient.IsRectEmpty())
	{
		if (IsWindowEnabled())
			pDC->FillRect( &rctClient,&m_bshClient);
		else
			pDC->FillRect(&rctClient, CBrush::FromHandle(::GetSysColorBrush(COLOR_BTNFACE)));
	}


	if (!rctNotes.IsRectEmpty())
	{
		pDC->FillRect(&rctNotes, &m_bshNotes);
	}
	DrawBookmarks(pDC);
	return TRUE;
}

void COXNotesEditView::OnLButtonDown(UINT nFlags, CPoint point) 
{

	if (IsWindowEnabled())
	{
			if (m_nPosition==NOTES)
			{
				UINT nChar=GetEditCtrl().CharFromPos(point);
				UINT nLine=GetEditCtrl().LineFromChar(LOWORD(nChar));
				nChar=GetEditCtrl().LineIndex(nLine);
				UINT nLength=GetEditCtrl().LineLength(nChar);
				UINT nEnd=nChar+nLength;
				GetEditCtrl().SetSel(nChar,nEnd,FALSE);
				return;
			}
	}
	
	CEditView::OnLButtonDown(nFlags, point);
}


HBRUSH COXNotesEditView::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	CRect rect;

	GetClientRect(&rect);
	CRect rctNotes(rect);

	switch (m_nSide)
	{
	case SIDE_LEFT:
		rect.right=min((UINT) rect.left+m_nNotesWidth, (UINT) rect.right);
		break;
	case SIDE_RIGHT:
		rect.left=max(0,rect.right-m_nNotesWidth);
		break;
	default:
		rect.SetRectEmpty();
	}
	if (!rect.IsRectEmpty())
		pDC->ExcludeClipRect(&rect);
	if (CTLCOLOR_EDIT==nCtlColor)
	{
		pDC->SetTextColor(m_clrFont);
		return (HBRUSH) m_bshClient;
	}
	else
		return NULL;
}

LRESULT COXNotesEditView::OnSetMargins(WPARAM wParam, LPARAM lParam)
{
	if (m_bUpdatingMargins)
		return Default();
	switch (wParam)
	{
	case EC_USEFONTINFO:
		{
			LRESULT lRslt=Default();
			m_bUpdatingMargins=TRUE;
			m_nMargins=PtrToUint(SendMessage(EM_GETMARGINS,0,0));
			switch (m_nSide)
			{
			case SIDE_LEFT:
				GetEditCtrl().SetMargins(LOWORD(m_nMargins)+m_nNotesWidth,
					HIWORD(m_nMargins));
				break;
			case SIDE_RIGHT:
				GetEditCtrl().SetMargins(LOWORD(m_nMargins),
					HIWORD(m_nMargins)+m_nNotesWidth);
				break;
			}
			m_bUpdatingMargins=FALSE;
			return lRslt;
		}
		break;
	case EC_LEFTMARGIN:
		m_nMargins=MAKELONG(lParam,HIWORD(m_nMargins));
		if (m_nSide==SIDE_LEFT)
		{
			m_bUpdatingMargins=TRUE;
			LPARAM lPrm=MAKELONG(LOWORD(m_nMargins)+m_nNotesWidth,
				HIWORD(m_nMargins));
			LRESULT lRslt=SendMessage(EM_SETMARGINS,wParam,lPrm);
			m_bUpdatingMargins=FALSE;
			return lRslt;
		}
		else
			return Default();
		break;
	case EC_RIGHTMARGIN:
		m_nMargins=MAKELONG(LOWORD(m_nMargins),lParam);
		if (m_nSide==SIDE_RIGHT)
		{
			m_bUpdatingMargins=TRUE;
			LPARAM lPrm=MAKELONG(LOWORD(m_nMargins),
				HIWORD(m_nMargins)+m_nNotesWidth);
			LRESULT lRslt=SendMessage(EM_SETMARGINS,wParam,lPrm);
			m_bUpdatingMargins=FALSE;
			return lRslt;
		}
		else
			return Default();
		break;
	default:
		return Default();
	}
}

void COXNotesEditView::OnMouseMove(UINT nFlags, CPoint point) 
{
	HCURSOR hCursor;
	CRect rect;
	UINT nPosition;

	GetNotesRect(&rect);
	if (rect.PtInRect(point))
	{
		hCursor=m_hNotesCursor;
		nPosition=NOTES;
	}
	else
	{
		hCursor=::LoadCursor(NULL,IDC_IBEAM);
		nPosition=EDIT;
	}

	if (nPosition!=m_nPosition)
		OnChangeCursor(&hCursor,nPosition);

	m_nPosition=nPosition;
	SetCursor(hCursor);
	CEditView::OnMouseMove(nFlags, point);
}

void COXNotesEditView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CRect rct;
	GetNotesRect(&rct);

	if (rct.PtInRect(point))
	{
		int n=HIWORD(GetEditCtrl().CharFromPos(point));
		int nChar=GetEditCtrl().LineIndex(n);
		if (IsMarked(nChar))
			RemoveBookmarks(nChar,nChar);
		else
			SetBookmark(nChar);
	}
	CEditView::OnLButtonDblClk(nFlags, point);
}












BOOL	 COXNotesEditView::InsertNotes(UINT nSide, UINT nWidth)
{
	m_nSide=nSide;
	m_nNotesWidth=nWidth;
	RedrawWindow();
	return TRUE;
}


void	COXNotesEditView::DeleteNotes()
{
	m_nSide=SIDE_NONE;
	m_nNotesWidth=0;
	ClearBookmarks();
	RedrawWindow();
}


BOOL	 COXNotesEditView::IsMarked(UINT nChar) const
{
	
	for (int n=0; n<m_arBookmarks.GetSize();n++)
	{
		if ((0x00FFFFFF & m_arBookmarks.GetAt(n))==nChar)
			return TRUE;
	}
	return FALSE;
}




void	COXNotesEditView::SetBookmark(UINT nChar, BYTE nType)
{
	DWORD dwChar;
	DWORD dwInsert=nType;
	if (!OnSetBookmark(nChar, &dwInsert))
		return;

	nChar=nChar & 0x00FFFFFF;
	dwInsert=(dwInsert<<24) | nChar;
	int n=0;

	CDC* pDC=GetDC();

	while (n<m_arBookmarks.GetSize())
	{
		dwChar=0x00FFFFFF & m_arBookmarks.GetAt(n);
		if (dwChar==nChar)
		{
			m_arBookmarks.SetAt(n,dwInsert);
			DrawBookmarks(pDC);
			return;
		}
		else
			if (dwChar>nChar)
			{
				m_arBookmarks.InsertAt(n,dwInsert);
				DrawBookmarks(pDC);
				return;
			}
			n++;
	}
	m_arBookmarks.Add(dwInsert);
	DrawBookmarks(pDC);
	ReleaseDC(pDC);

}


BOOL	 COXNotesEditView::ShowBookmark(UINT nChar)
{

	if (IsMarked(nChar))
	{
		UINT nLine=GetEditCtrl().LineFromChar(nChar);
		UINT nFirstVisible=GetEditCtrl().GetFirstVisibleLine();
		UINT nLastVisible=GetLastVisibleLine();
		if (nFirstVisible>nLine || nLastVisible<nLine)
			GetEditCtrl().LineScroll(nLine-nFirstVisible);
		return TRUE;
	}
	else
		return FALSE;
}


void COXNotesEditView::SetNotesBackground(COLORREF clrNotesBkgnd)
{
	if (m_bshNotes.m_hObject)
		m_bshNotes.DeleteObject();
	m_bshNotes.CreateSolidBrush(clrNotesBkgnd);
}


void   COXNotesEditView::SetNotesBackground(CBrush* pBrush)
{
	if (m_bshNotes.m_hObject)
		m_bshNotes.DeleteObject();
	LOGBRUSH LogBrush;
	VERIFY(pBrush->GetLogBrush(&LogBrush));
	VERIFY(m_bshNotes.CreateBrushIndirect(&LogBrush));
}



int COXNotesEditView::ImageFromLine(UINT nLine) const
{
	int nRet=-1;

	CEdit& edt=GetEditCtrl();
	UINT nFirst=edt.LineIndex(nLine);
	UINT nLast=edt.LineLength(edt.LineIndex(nLine));
	if (nLast)
		nLast=nLast+nFirst-1;
	else
		nLast=nFirst;
	
	for (int n=0;n<m_arBookmarks.GetSize();n++)
	{
		//every value in the m_arBookmarks array 
		//consists of index of the char the bookmark set to
		//(that is 24 least significant bits in the value) and
		//8 most significant bits represents image index of 
		//the bookmark in m_imgBookmarks
		DWORD dwIndex=m_arBookmarks.GetAt(n);
		DWORD dwChar=0x00FFFFFF & dwIndex;

		if (dwChar>=nFirst)
		{
			if (dwChar<=nLast)
				nRet=dwIndex>>24;
			else
				return nRet;
		}
	}
	return nRet;
}

int COXNotesEditView::GetLastVisibleLine()
{
	CRect rect;

	CEdit& edt=GetEditCtrl();
	edt.GetRect(&rect);
	UINT nHeight=GetLineHeight();
	UINT nLast=edt.GetFirstVisibleLine()+rect.Height()/nHeight;
	return nLast;
}

int COXNotesEditView::GetLineHeight()
{
	CDC* pdc=GetDC();
	TEXTMETRIC Metrics;
	VERIFY(pdc->GetTextMetrics(&Metrics));
	ReleaseDC(pdc);
	return Metrics.tmHeight;
}

void COXNotesEditView::OnChangeCursor(HCURSOR *phCursor, UINT nPosition)
{
	UNUSED_ALWAYS(phCursor);
	UNUSED_ALWAYS(nPosition);
	//does nothing
	//here you can change HCURSOR to your custom cursor
	//on the fly, depends of cursor position
}

void COXNotesEditView::GetNotesRect(LPRECT pRect)
{
	GetClientRect(pRect);
	switch (m_nSide)
	{
	case SIDE_LEFT:
		pRect->right=pRect->left+m_nNotesWidth;
		break;
	case SIDE_RIGHT:
		pRect->left=pRect->right-m_nNotesWidth;
		break;
	default:
		::ZeroMemory(&pRect, sizeof(pRect));
	}
}

void COXNotesEditView::DrawBookmarks(CDC *pDC)
{
	CRect rect;
	GetNotesRect(&rect);
	if (!rect.IsRectEmpty())
	{
		pDC->FillRect(&rect, &m_bshNotes);
		CRect rctClient;
		GetClientRect(&rctClient);
		int nLine=GetEditCtrl().GetFirstVisibleLine();
		int nLastLine=GetLastVisibleLine();
		CRect rct;
		GetNotesRect(&rct);
		UINT nMode=pDC->SetBkMode(TRANSPARENT);
		CFont* pOldFont=NULL;
		if (m_pLinesFont)
			pOldFont=pDC->SelectObject(m_pLinesFont);
		COLORREF clrOld=pDC->SetTextColor(m_clrLines);

		TEXTMETRIC Metrics;
		VERIFY(pDC->GetTextMetrics(&Metrics));
		int nHeight=Metrics.tmHeight;

		TCHAR szBuffer[_MAX_PATH];
		for (nLine;nLine<nLastLine;nLine++)
		{
			if (GetEditCtrl().LineIndex(nLine)!=-1)
			{
				int nCount=GetEditCtrl().GetLine(nLine,(LPTSTR) &szBuffer, _MAX_PATH);
				if (!nCount && GetEditCtrl().GetLineCount()==(nLine+1))
					break;
				CPoint pt=GetEditCtrl().PosFromChar(GetEditCtrl().LineIndex(nLine));
					pt.x=(rct.Width()-GetBookmarkImageSize().cx)/2+rct.left;
				if (m_nMode==MODE_BKMARKS)
				{
					int nIndex=ImageFromLine(nLine);
					if (nIndex!=-1 && nIndex<m_imgBookmarks.GetImageCount())
					{
						VERIFY(m_imgBookmarks.DrawIndirect(pDC, nIndex,
							pt, GetBookmarkImageSize(),CPoint(0,0)));
					}
				}
				else
					if (m_nMode==MODE_LINES)
					{
							CString sLine;
							SIZE sz;

							sLine.Format(_T("%d"),nLine+1);

							VERIFY(::GetTextExtentExPoint(pDC->m_hDC,
								(LPCTSTR) sLine,sLine.GetLength(),
								rect.Width(), NULL,NULL,//&nCount, &nWidth,
								&sz));
							ASSERT(GetNotesSide()!=SIDE_NONE);
							pt.x=(GetNotesSide()==SIDE_LEFT)?0:rctClient.right-rect.Width();
							pt.y+=(nHeight>=GetLineHeight())?0:
							(GetLineHeight()-nHeight);
							pDC->TextOut(pt.x+rect.Width()-sz.cx,pt.y,(LPCTSTR) sLine/*,nCount*/);
							
					}
					else
						ASSERT(FALSE);//not valid mode
			}
		}
		pDC->SetBkMode(nMode);
		pDC->SetTextColor(clrOld);
		if (m_pLinesFont)
			pDC->SelectObject(pOldFont);
	}
}

CSize COXNotesEditView::GetBookmarkImageSize()
{
	
	CSize sz(0,0);
	if (m_imgBookmarks.GetImageCount())
	{
		//all bitmaps in the image list
		//supposed to have equal size
		IMAGEINFO ImgInfo;
		VERIFY(m_imgBookmarks.GetImageInfo(0,&ImgInfo));
		sz.cy=ImgInfo.rcImage.bottom-ImgInfo.rcImage.top;
		sz.cx=ImgInfo.rcImage.right-ImgInfo.rcImage.left;
	}
	return sz;
}




LRESULT COXNotesEditView::OnGetMargins(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(wParam);
	UNUSED_ALWAYS(lParam);

	LRESULT lRslt=Default();
//	ASSERT((UINT) lRslt==m_nMargins);

	if (m_bUpdatingMargins)
		return lRslt;

	switch (m_nSide)
	{
	case SIDE_LEFT:
		lRslt=MAKELONG(LOWORD(lRslt)+m_nNotesWidth,
			HIWORD(lRslt));
		break;
	case SIDE_RIGHT:
		lRslt=MAKELONG(LOWORD(lRslt),
			HIWORD(lRslt)+m_nNotesWidth);
		break;
	}

	return lRslt;

}

void COXNotesEditView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CEditView::OnVScroll(nSBCode, nPos, pScrollBar);
	CDC* pDC=GetDC();
	DrawBookmarks(pDC);
	ReleaseDC(pDC);
}

void COXNotesEditView::OnChange() 
{
	CString sText;
	UINT nStartPos=0;
	UINT nEndPos;
	
	GetWindowText(sText);
	UINT nOldLength=m_sCopy.GetLength();
	UINT nNewLength=sText.GetLength();
	UINT nDirection=LENGTH_THESAME;
	if (nNewLength>nOldLength)
		nDirection=LENGTH_MORE;
	else
		if (nNewLength<nOldLength)
			nDirection=LENGTH_LESS;

	int nDiff=nNewLength-nOldLength;

	while(nStartPos<(UINT) sText.GetLength() && 
		nStartPos<(UINT) m_sCopy.GetLength() && 
		sText.GetAt(nStartPos)==m_sCopy.GetAt(nStartPos))
		nStartPos++;
	switch (nDirection)
	{
	case LENGTH_THESAME:
		nEndPos=sText.GetLength()-1;
		while (nEndPos>nStartPos)
		{
			if (sText.GetAt(nEndPos)!=m_sCopy.GetAt(nEndPos))
			{
				if (nEndPos>nStartPos)
				{
					RemoveBookmarks(nStartPos, nEndPos);
				}
			}
			nEndPos--;
		}
		break;
	case LENGTH_MORE:
		if (m_sCopy.GetLength())
		{
			nEndPos=m_sCopy.GetLength()-1;
			while (nEndPos>nStartPos)
			{
				if (m_sCopy.GetAt(nEndPos)!=sText.GetAt(nEndPos+nDiff))
				{
					RemoveBookmarks(nStartPos,nEndPos);

				}
				nEndPos--;
			}
		}
		OffsetBookmarks(nStartPos,nDiff);
		break;
	case LENGTH_LESS:
		if (sText.GetLength())
		{
			nEndPos=sText.GetLength()-1;
			while (nEndPos>nStartPos)
			{
				if (sText.GetAt(nEndPos)!=m_sCopy.GetAt(nEndPos-nDiff))
				{
					RemoveBookmarks(nStartPos,nEndPos);

				}
				nEndPos--;
			}
		}
		OffsetBookmarks(nStartPos,nDiff);
		break;
	}
	nEndPos=nStartPos;
	
	m_sCopy=sText;
	CDC* pDC=GetDC();
	DrawBookmarks(pDC);
	ReleaseDC(pDC);
	
}

LRESULT COXNotesEditView::OnSetText(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(wParam);
	UNUSED_ALWAYS(lParam);

	LRESULT lRslt=Default();
	OnChange();
	return lRslt;
}

void COXNotesEditView::RemoveBookmarks( UINT nStartChar=OX_NOTES_ALL, UINT nEndChar=OX_NOTES_ALL)
{
	//remove the bookmarks that are in [nStartPos...nEndPos] range
	if (nStartChar==OX_NOTES_ALL)
		nStartChar=0;
	if (nEndChar==OX_NOTES_ALL)
		nEndChar=GetWindowTextLength();
	for (int n=0;n<m_arBookmarks.GetSize();n++)
	{
		DWORD dwChar=m_arBookmarks.GetAt(n) & 0x00FFFFFF;
		if (dwChar>=nStartChar)
		{
			if (dwChar<=nEndChar)
			{
				m_arBookmarks.RemoveAt(n);
				n--;
			}
			else
				break;
		}

	}
	CRect rctNotes;
	GetNotesRect(&rctNotes);
	InvalidateRect(&rctNotes);
	RedrawWindow();
}


void COXNotesEditView::OffsetBookmarks(UINT nStart, int nOffset)
{
	for (int n=0; n<m_arBookmarks.GetSize(); n++)
	{
		DWORD dwIndex=m_arBookmarks.GetAt(n) ;
		if ((dwIndex & 0x00FFFFFF)>=nStart)
		{
			int nNew=nOffset+(dwIndex &0x00FFFFFF);
			if (nNew>0)
			{
				nNew|=0xFF000000 & dwIndex;
				m_arBookmarks.SetAt(n,nNew);
			}
			else
			{
				m_arBookmarks.RemoveAt(n);
				n--;
			}
		}
	}
}


void COXNotesEditView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int nStart, nEnd;

	if (nChar==VK_TAB)	
	{
		GetEditCtrl().GetSel(nStart, nEnd);
		if (nEnd>nStart)
		{
			int i=::GetAsyncKeyState(VK_SHIFT);
			BOOL bInsert=(i<0?FALSE:TRUE);
			
				UINT nFirstLine=GetEditCtrl().LineFromChar(nStart);
				UINT nLastLine=GetEditCtrl().LineFromChar(nEnd);
				for (int nLine=nFirstLine;(UINT) nLine<=nLastLine;nLine++)
				{
					int nChar=GetEditCtrl().LineIndex(nLine);
					CString sText;
					GetWindowText(sText);
					if (!bInsert)
					{
						if (sText.GetAt(nChar)==TEXT('\t'))
						{
								sText=sText.Left(nChar)+sText.Right(sText.GetLength()
								-nChar-1);
							SetWindowText((LPCTSTR) sText);
						}
					}
					else
					{
						sText.Insert(nChar,TEXT('\t'));
						SetWindowText((LPCTSTR) sText);
					}
				}
				nStart=GetEditCtrl().LineIndex(nFirstLine);
				nEnd=GetEditCtrl().LineIndex(nLastLine);
				int nLength=GetEditCtrl().LineLength(GetEditCtrl().LineIndex(nLastLine));

				nEnd+=nLength;//?(nLength-1):NULL;
				GetEditCtrl().SetSel(nStart, nEnd);
			return;
		}
	}
	CEditView::OnChar(nChar, nRepCnt, nFlags);
}

BOOL COXNotesEditView::OnSetBookmark(UINT nChar, DWORD *pType)
{
	UNUSED_ALWAYS(nChar);
	UNUSED_ALWAYS(pType);

	return TRUE;
}


void COXNotesEditView::SetMode(UINT nMode, BOOL bRedraw)
{
	if (nMode!=m_nMode)
	{
		m_nMode=nMode;
		if (bRedraw)
			RedrawWindow();
	}
}

UINT COXNotesEditView::GetMode() const
{
	return m_nMode;
}

BOOL COXNotesEditView::SetLinesFont(CFont *pFont, BOOL bRedraw)
{
	BOOL bRet=TRUE;
	if (m_pLinesFont)
	{
		m_pLinesFont->DeleteObject();
		delete m_pLinesFont;
		m_pLinesFont=NULL;
	}
	if (pFont)
	{
		m_pLinesFont= new CFont;
		LOGFONT LogFont;
		if (pFont->GetLogFont(&LogFont))
		{
			if (!m_pLinesFont->CreateFontIndirect(&LogFont))
			{
				delete m_pLinesFont;
				m_pLinesFont=NULL;
				bRet=FALSE;
			}	
		}
		else
			bRet=FALSE;
	}

	if (bRedraw)
		RedrawWindow();

	return bRet;
}

const CFont* COXNotesEditView::GetLinesFont() const
{
	return m_pLinesFont;
}

void COXNotesEditView::SetLinesColor(COLORREF clr, BOOL bRedraw)
{
	m_clrLines=clr;
	if (bRedraw)
		RedrawWindow();
}

COLORREF COXNotesEditView::GetLinesColor() const
{
	return m_clrLines;
}
