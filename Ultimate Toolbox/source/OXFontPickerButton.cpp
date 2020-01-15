// OXFontPickerButton.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXFontPickerButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFXAPI DDX_FontPickerFont(CDataExchange *pDX, int nIDC, CFont* pFont)
{
	ASSERT(pFont!=NULL);

    HWND hWnd=pDX->PrepareCtrl(nIDC);
    ASSERT(hWnd!=NULL);                
    
    COXFontPickerButton* pButton=(COXFontPickerButton*)CWnd::FromHandle(hWnd);
    if(pDX->m_bSaveAndValidate)
    {
		if((HFONT)*pFont!=NULL)
			pFont->DeleteObject();
		LOGFONT lf;
		if(pButton->GetBuddyLogFont(&lf))
			VERIFY(pFont->CreateFontIndirect(&lf));
    }
    else 
    {
        pButton->SetBuddyFont(pFont);
    }
}

void AFXAPI DDX_FontPickerColor(CDataExchange *pDX, int nIDC, COLORREF& clr)
{
    HWND hWnd=pDX->PrepareCtrl(nIDC);
    ASSERT(hWnd!=NULL);                
    
    COXFontPickerButton* pButton=(COXFontPickerButton*)CWnd::FromHandle(hWnd);
    if(pDX->m_bSaveAndValidate)
    {
        clr=pButton->GetBuddyTextColor();
    }
    else 
    {
        pButton->SetBuddyTextColor(clr);
    }
}

/////////////////////////////////////////////////////////////////////////////
// COXFontPickerButton

IMPLEMENT_DYNAMIC(COXFontPickerButton, COXBitmapButton)

COXFontPickerButton::COXFontPickerButton()
{
	m_pBuddy=new COXStatic;
}

COXFontPickerButton::~COXFontPickerButton()
{
	if((HWND)*m_pBuddy!=NULL)
		m_pBuddy->UnsubclassWindow();

	delete m_pBuddy;
}


BEGIN_MESSAGE_MAP(COXFontPickerButton, COXBitmapButton)
	//{{AFX_MSG_MAP(COXFontPickerButton)
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT_EX(BN_CLICKED,OnClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXFontPickerButton message handlers

BOOL COXFontPickerButton::SetBuddyTextColor(COLORREF clrText, 
											BOOL bRedraw/*=TRUE*/)
{
	if((HWND)*m_pBuddy==NULL)
		return FALSE;

	m_pBuddy->SetTextColor(clrText,bRedraw);
	return TRUE;
}

COLORREF COXFontPickerButton::GetBuddyTextColor() const 
{ 
	if((HWND)*m_pBuddy==NULL)
		return CLR_NONE;

	return m_pBuddy->GetTextColor(); 
}

BOOL COXFontPickerButton::SetBuddyFont(CFont* pFont, BOOL bRedraw/*=TRUE*/)
{
	ASSERT(pFont!=NULL);

	if((HWND)*m_pBuddy==NULL)
		return FALSE;

	LOGFONT lf;
	if(pFont->GetLogFont(&lf))
	{
		CString sFontName;
		sFontName.Format(_T("%d pt, %s"),(ConvertLogUnitToPoint(lf.lfHeight)+5)/10,
			lf.lfFaceName);
		m_pBuddy->SetWindowText(sFontName);
	}
	m_pBuddy->SetTextFont(pFont,bRedraw);

	return TRUE;
}

BOOL COXFontPickerButton::SetBuddyLogFont(LOGFONT* pLF, BOOL bRedraw/*=TRUE*/)
{
	ASSERT(pLF!=NULL);

	if((HWND)*m_pBuddy==NULL)
		return FALSE;

	CString sFontName;
	sFontName.Format(_T("%d pt, %s"),(ConvertLogUnitToPoint(pLF->lfHeight)+5)/10,
		pLF->lfFaceName);
	m_pBuddy->SetWindowText(sFontName);
	m_pBuddy->SetTextLogFont(pLF,bRedraw);
	return TRUE;
}


CFont* COXFontPickerButton::GetBuddyFont() const
{
	if((HWND)*m_pBuddy==NULL)
		return NULL;

	return m_pBuddy->GetTextFont();
}

BOOL COXFontPickerButton::GetBuddyLogFont(LOGFONT* pLF) const
{
	ASSERT(pLF!=NULL);

	if((HWND)*m_pBuddy==NULL)
		return FALSE;

	return m_pBuddy->GetTextLogFont(pLF);
}

BOOL COXFontPickerButton::SetBuddy(UINT nBuddyWndID)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	CWnd* pParentWnd=GetParent();
	ASSERT(pParentWnd!=NULL);

	HWND hBuddyWnd=::GetDlgItem(pParentWnd->GetSafeHwnd(),nBuddyWndID);
	if(hBuddyWnd==NULL)
		return FALSE;

	if((HWND)*m_pBuddy!=NULL)
		m_pBuddy->UnsubclassWindow();

	if(m_pBuddy->SubclassWindow(hBuddyWnd))
	{
		CFont* pFont=GetBuddyFont();
		if(pFont!=NULL)
			SetBuddyFont(pFont,TRUE);
		return TRUE;
	}

	return FALSE;
}

BOOL COXFontPickerButton::SetBuddy(COXStatic* pBuddy)
{
	ASSERT(pBuddy!=NULL && pBuddy->IsKindOf(RUNTIME_CLASS(COXStatic)));

	CWnd* pParentWnd=GetParent();
	ASSERT(pParentWnd!=NULL);
	if(pParentWnd!=pBuddy->GetParent())
		return FALSE;

	if((HWND)*m_pBuddy!=NULL)
		m_pBuddy->UnsubclassWindow();

	HWND hBuddyWnd=pBuddy->Detach();
	ASSERT(hBuddyWnd!=NULL);
	if(m_pBuddy->SubclassWindow(hBuddyWnd))
	{
		CFont* pFont=GetBuddyFont();
		if(pFont!=NULL)
			SetBuddyFont(pFont,TRUE);
		return TRUE;
	}

	return FALSE;
}

BOOL COXFontPickerButton::OnClicked()
{
	ASSERT(::IsWindow(m_hWnd));

	// Use font common dialog
	CFontDialog* pFontDlg;
	CFont* pFont=GetBuddyFont();
	LOGFONT lf;
	if(pFont==NULL)
	{
		pFontDlg=new CFontDialog();
	}
	else
	{
		pFont->GetLogFont(&lf);
		pFontDlg=new CFontDialog(&lf);
	}

	pFontDlg->m_cf.rgbColors=GetBuddyTextColor();
	if(pFontDlg->DoModal()==IDOK && m_pBuddy!=NULL)
	{
		pFontDlg->GetCurrentFont(&lf);
		SetBuddyLogFont(&lf,FALSE);
		COLORREF clr=pFontDlg->GetColor();
		SetBuddyTextColor(clr,TRUE);
	}

	delete pFontDlg;

	return FALSE;
}

int COXFontPickerButton::ConvertPointToLogUnit(int nPointSize, CDC* pDC/*=NULL*/)
{
	HDC hDC;
	if (pDC!=NULL)
	{
		ASSERT_VALID(pDC);
		ASSERT(pDC->m_hAttribDC != NULL);
		hDC=pDC->m_hAttribDC;
	}
	else
		hDC=::GetDC(NULL);

	// convert nPointSize to logical units based on pDC
	POINT pt;
	pt.y=::GetDeviceCaps(hDC,LOGPIXELSY)*nPointSize;
	pt.y/=720;    // 72 points/inch, 10 decipoints/point
	::DPtoLP(hDC,&pt,1);
	POINT ptOrg = {0,0};
	::DPtoLP(hDC,&ptOrg,1);

	if(pDC==NULL)
		::ReleaseDC(NULL,hDC);

	return -abs(pt.y-ptOrg.y);
}

int COXFontPickerButton::ConvertLogUnitToPoint(int nLogUnitSize, 
											   CDC* pDC/*=NULL*/)
{
	HDC hDC;
	if (pDC!=NULL)
	{
		ASSERT_VALID(pDC);
		ASSERT(pDC->m_hAttribDC != NULL);
		hDC=pDC->m_hAttribDC;
	}
	else
		hDC=::GetDC(NULL);

	POINT ptOrg={0,0};
	::DPtoLP(hDC,&ptOrg,1);
	POINT pt={0,0};

	pt.y=abs(nLogUnitSize)+ptOrg.y;
	::LPtoDP(hDC,&pt,1);
	pt.y*=720;    // 72 points/inch, 10 decipoints/point
	
	int nPointSize=(pt.y)/(::GetDeviceCaps(hDC,LOGPIXELSY));

	if (pDC==NULL)
		::ReleaseDC(NULL,hDC);

	return nPointSize;
}

