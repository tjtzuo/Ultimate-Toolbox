// OXColorPickerCtrl.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXColorPickerCtrl.h"
#include "UTB64Bit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COLORREF COXColorPickerCtrl::m_defaultArrColor[] = 
{
    { RGB(0x00, 0x00, 0x00) },
    { RGB(0xA5, 0x2A, 0x2A) },
    { RGB(0x00, 0x40, 0x40) },
    { RGB(0x00, 0x55, 0x00) },
    { RGB(0x00, 0x00, 0x5E) },
    { RGB(0x00, 0x00, 0x8B) },
    { RGB(0x4B, 0x00, 0x82) },
    { RGB(0x28, 0x28, 0x28) },

    { RGB(0x8B, 0x00, 0x00) },
    { RGB(0xFF, 0x68, 0x20) },
    { RGB(0x8B, 0x8B, 0x00) },
    { RGB(0x00, 0x93, 0x00) },
    { RGB(0x38, 0x8E, 0x8E) },
    { RGB(0x00, 0x00, 0xFF) },
    { RGB(0x7B, 0x7B, 0xC0) },
    { RGB(0x66, 0x66, 0x66) },

    { RGB(0xFF, 0x00, 0x00) },
    { RGB(0xFF, 0xAD, 0x5B) },
    { RGB(0x32, 0xCD, 0x32) }, 
    { RGB(0x3C, 0xB3, 0x71) },
    { RGB(0x7F, 0xFF, 0xD4) },
    { RGB(0x7D, 0x9E, 0xC0) },
    { RGB(0x80, 0x00, 0x80) },
    { RGB(0x7F, 0x7F, 0x7F) },

    { RGB(0xFF, 0xC0, 0xCB) },
    { RGB(0xFF, 0xD7, 0x00) },
    { RGB(0xFF, 0xFF, 0x00) },    
    { RGB(0x00, 0xFF, 0x00) },
    { RGB(0x40, 0xE0, 0xD0) },
    { RGB(0xC0, 0xFF, 0xFF) },
    { RGB(0x48, 0x00, 0x48) },
    { RGB(0xC0, 0xC0, 0xC0) },

    { RGB(0xFF, 0xE4, 0xE1) },
    { RGB(0xD2, 0xB4, 0x8C) },
    { RGB(0xFF, 0xFF, 0xE0) },
    { RGB(0x98, 0xFB, 0x98) },
    { RGB(0xAF, 0xEE, 0xEE) },
    { RGB(0x68, 0x83, 0x8B) },
    { RGB(0xE6, 0xE6, 0xFA) },
    { RGB(0xFF, 0xFF, 0xFF) }
};

ButtonToolTipIDTable COXColorPickerCtrl::m_defaultArrToolTipText[] = 
{
    { RGB(0x00, 0x00, 0x00),    IDS_OX_CLRPICKER_BLACK			/*"Black"             */},
    { RGB(0xA5, 0x2A, 0x2A),    IDS_OX_CLRPICKER_BROWN			/*"Brown"             */},
    { RGB(0x00, 0x40, 0x40),    IDS_OX_CLRPICKER_DARKOLIVEGREEN	/*"Dark Olive Green"  */},
    { RGB(0x00, 0x55, 0x00),    IDS_OX_CLRPICKER_DARKGREEN		/*"Dark Green"        */},
    { RGB(0x00, 0x00, 0x5E),    IDS_OX_CLRPICKER_DARKTEAL		/*"Dark Teal"         */},
    { RGB(0x00, 0x00, 0x8B),    IDS_OX_CLRPICKER_DARKBLUE		/*"Dark blue"         */},
    { RGB(0x4B, 0x00, 0x82),    IDS_OX_CLRPICKER_INDIGO			/*"Indigo"            */},
    { RGB(0x28, 0x28, 0x28),    IDS_OX_CLRPICKER_DARKGREY		/*"Dark grey"         */},

    { RGB(0x8B, 0x00, 0x00),    IDS_OX_CLRPICKER_DARKRED		/*"Dark red"          */},
    { RGB(0xFF, 0x68, 0x20),    IDS_OX_CLRPICKER_ORANGE			/*"Orange"            */},
    { RGB(0x8B, 0x8B, 0x00),    IDS_OX_CLRPICKER_DARKYELLOW		/*"Dark yellow"       */},
    { RGB(0x00, 0x93, 0x00),    IDS_OX_CLRPICKER_GREEN			/*"Green"             */},
    { RGB(0x38, 0x8E, 0x8E),    IDS_OX_CLRPICKER_TEAL			/*"Teal"              */},
    { RGB(0x00, 0x00, 0xFF),    IDS_OX_CLRPICKER_BLUE			/*"Blue"              */},
    { RGB(0x7B, 0x7B, 0xC0),    IDS_OX_CLRPICKER_BLUEGREY		/*"Blue-grey"         */},
    { RGB(0x66, 0x66, 0x66),    IDS_OX_CLRPICKER_GREY60			/*"Grey - 60"         */},

    { RGB(0xFF, 0x00, 0x00),    IDS_OX_CLRPICKER_RED			/*"Red"               */},
    { RGB(0xFF, 0xAD, 0x5B),    IDS_OX_CLRPICKER_LIGHTORANGE	/*"Light orange"      */},
    { RGB(0x32, 0xCD, 0x32),    IDS_OX_CLRPICKER_LIME			/*"Lime"              */}, 
    { RGB(0x3C, 0xB3, 0x71),    IDS_OX_CLRPICKER_SEAGREEN		/*"Sea green"         */},
    { RGB(0x7F, 0xFF, 0xD4),    IDS_OX_CLRPICKER_AGUA			/*"Aqua"              */},
    { RGB(0x7D, 0x9E, 0xC0),    IDS_OX_CLRPICKER_LIGHTBLUE		/*"Light blue"        */},
    { RGB(0x80, 0x00, 0x80),    IDS_OX_CLRPICKER_VIOLET			/*"Violet"            */},
    { RGB(0x7F, 0x7F, 0x7F),    IDS_OX_CLRPICKER_GREY50			/*"Grey - 50"         */},

    { RGB(0xFF, 0xC0, 0xCB),    IDS_OX_CLRPICKER_PINK			/*"Pink"              */},
    { RGB(0xFF, 0xD7, 0x00),    IDS_OX_CLRPICKER_GOLD			/*"Gold"              */},
    { RGB(0xFF, 0xFF, 0x00),    IDS_OX_CLRPICKER_YELLOW			/*"Yellow"            */},    
    { RGB(0x00, 0xFF, 0x00),    IDS_OX_CLRPICKER_BRIGHTGREEN	/*"Bright green"      */},
    { RGB(0x40, 0xE0, 0xD0),    IDS_OX_CLRPICKER_TURQUOISE		/*"Turquoise"         */},
    { RGB(0xC0, 0xFF, 0xFF),    IDS_OX_CLRPICKER_SKYBLUE		/*"Skyblue"           */},
    { RGB(0x48, 0x00, 0x48),    IDS_OX_CLRPICKER_PLUM			/*"Plum"              */},
    { RGB(0xC0, 0xC0, 0xC0),    IDS_OX_CLRPICKER_LIGHTGREY		/*"Light grey"        */},

    { RGB(0xFF, 0xE4, 0xE1),    IDS_OX_CLRPICKER_ROSE			/*"Rose"              */},
    { RGB(0xD2, 0xB4, 0x8C),    IDS_OX_CLRPICKER_TAN			/*"Tan"               */},
    { RGB(0xFF, 0xFF, 0xE0),    IDS_OX_CLRPICKER_LIGHTYELLOW	/*"Light yellow"      */},
    { RGB(0x98, 0xFB, 0x98),    IDS_OX_CLRPICKER_PALEGREEN		/*"Pale green "       */},
    { RGB(0xAF, 0xEE, 0xEE),    IDS_OX_CLRPICKER_PALETURQUOISE	/*"Pale turquoise"    */},
    { RGB(0x68, 0x83, 0x8B),    IDS_OX_CLRPICKER_PALEBLUE		/*"Pale blue"         */},
    { RGB(0xE6, 0xE6, 0xFA),    IDS_OX_CLRPICKER_LAVENDER		/*"Lavender"          */},
    { RGB(0xFF, 0xFF, 0xFF),    IDS_OX_CLRPICKER_WHITE			/*"White"             */}
};

/////////////////////////////////////////////////////////////////////////////
// COXColorPickerCtrl

COXColorPickerCtrl::COXColorPickerCtrl(UINT nColors, UINT nRows, COLORREF clrDefault,
									   CSize sizeButton, int nIDdefault,
									   int nIDcustom):
	COXPopupBarCtrl(nColors, nRows, clrDefault, sizeButton,
		nIDdefault, nIDcustom)
{

}

BEGIN_MESSAGE_MAP(COXColorPickerCtrl, COXPopupBarCtrl)
	//{{AFX_MSG_MAP(COXColorPickerCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL COXColorPickerCtrl::Pick(UINT nAlignment, CRect* pParentRect, CSize sizeOffset)
{
	ASSERT_VALID(m_pParentWnd);
    ASSERT(::IsWindow(m_pParentWnd->GetSafeHwnd()));
	ASSERT(::IsWindow(m_hWnd));

	ASSERT(m_nButtons<=ID_POPUPBAR_MAX_BUTTONS && m_nButtons>0);
	ASSERT(m_nRows<=ID_POPUPBAR_MAX_BUTTONS && m_nRows>0 && m_nRows<=m_nButtons);

	// nobody has cared to fill array of colors
	// then we'll do that using predefined array
	if(m_arrData.GetSize()==0)
	{
		if(!SetButtonDataTable(m_defaultArrColor,CLRPICK_DEFINEDCOLORS))
		{
			return FALSE;
		}
	}
	for(int nIndex=PtrToInt(m_arrData.GetSize()); nIndex<(int)m_nButtons; nIndex++)
	{
		m_arrData.Add(RGB(0,0,0));
	}
	ASSERT(m_arrData.GetSize()>=(int)m_nButtons);


	// nobody has cared to fill array of tooltip text 
	// then we'll do that using default table
	if(m_arrToolTipText.GetCount()==0)
	{
		if(!SetToolTipTextIDTable(m_defaultArrToolTipText,CLRPICK_DEFINEDCOLORS))
		{
			return FALSE;
		}
	}

	// build custom palette for 256-color video mode
	BuildCustomPalette(GetButtonDataTable(),&m_CustomPalette);

	return COXPopupBarCtrl::Pick(nAlignment,pParentRect,sizeOffset);
}

////////////////////////////////////
// Attributes

/////////////////////////////////////////////////////////////////////////////
// COXColorPickerCtrl message handlers

BOOL COXColorPickerCtrl::OnCustomButton()
{
	ASSERT(IsCustomButton());

	ReleaseCapture();
	// Use color common dialog
	CColorDialog colorDlg(m_dwSelected,CC_FULLOPEN|CC_ANYCOLOR,GetParent());
	if(colorDlg.DoModal()==IDOK)
	{
		m_nSelectedIndex=m_nButtons+1;
		m_dwSelected=colorDlg.GetColor();
		return TRUE;
	}

	return FALSE;
}


////////////////////////////////////////////////////////////////

// fills array of colors with values from current palette 
// starting from m_nStartFrom color index in the palette
int COXColorPickerCtrl::PopulateColorTable(UINT nStartFrom/*=0*/, 
										   BOOL bUseSystemPalette/*=FALSE*/)
{
	ASSERT(m_nButtons>0 && m_nButtons<=ID_POPUPBAR_MAX_BUTTONS);
	ASSERT(nStartFrom<ID_POPUPBAR_MAX_BUTTONS);

	CPalette sysPalette;
	CPalette* pPalette=NULL;
	if(bUseSystemPalette)
	{
		if(!GetSystemPalette(&sysPalette))
		{
			return 0;
		}
		pPalette=&sysPalette;
	}
	else
	{
		CDC* pDC=CDC::FromHandle(::GetDC(NULL));
		pPalette=pDC->GetCurrentPalette();
	}

	ASSERT(pPalette);

	UINT nEntryCount=pPalette->GetEntryCount();
	ASSERT(nEntryCount>0);

	m_arrData.RemoveAll();

	PALETTEENTRY pe;
	int nAddedEntries=0;
	for(int nIndex=nStartFrom; nIndex<(int)(nStartFrom+m_nButtons); nIndex++)
	{
		if(pPalette->GetPaletteEntries(nIndex, 1, &pe))
		{
			m_arrData.Add(RGB(pe.peRed,pe.peGreen,pe.peBlue));
			nAddedEntries++;
		}
	}

	return nAddedEntries;
}

// fills array of colors with values from current palette 
// using array of color indexes in the palette
int COXColorPickerCtrl::PopulateColorTable(UINT* arrIndexes, UINT nElements, 
										   BOOL bUseSystemPalette/*=FALSE*/)
{
	ASSERT(m_nButtons>0 && m_nButtons<=ID_POPUPBAR_MAX_BUTTONS);
	ASSERT(nElements<ID_POPUPBAR_MAX_BUTTONS);

	CPalette sysPalette;
	CPalette* pPalette=NULL;
	if(bUseSystemPalette)
	{
		if(!GetSystemPalette(&sysPalette))
		{
			return 0;
		}
		pPalette=&sysPalette;
	}
	else
	{
		CDC* pDC=CDC::FromHandle(::GetDC(NULL));
		pPalette=pDC->GetCurrentPalette();
	}

	ASSERT(pPalette);

	UINT nEntryCount=pPalette->GetEntryCount();
	ASSERT(nEntryCount>0 && nEntryCount<nElements);

	m_arrData.RemoveAll();

	PALETTEENTRY pe;
	int nAddedEntries=0;
	for(int nIndex=0; nIndex<(int)nElements; nIndex++)
	{
		if(pPalette->GetPaletteEntries(arrIndexes[nIndex], 1, &pe))
		{
			m_arrData.Add(RGB(pe.peRed,pe.peGreen,pe.peBlue));
			nAddedEntries++;
		}
	}

	return nAddedEntries;
}


int COXColorPickerCtrl::PopulateColorTable(COLORREF* pColorArray, UINT nElements) 
{
	ASSERT(pColorArray);
	ASSERT(nElements);
	UINT n=0;
	for (n=0; n<nElements; n++)
	{
		m_arrData.Add(*(pColorArray+n));
	}
	return n;
}

// draw button
void COXColorPickerCtrl::DrawButton(CDC* pDC, UINT nIndex)
{
	ASSERT(m_nButtons>0 && m_nButtons<=ID_POPUPBAR_MAX_BUTTONS);
	ASSERT((int)m_nButtons==m_arrButtonRect.GetSize());
	ASSERT((int)m_nButtons<=m_arrData.GetSize());
	ASSERT(nIndex<=m_nButtons);

	COLORREF clrShadow=::GetSysColor(COLOR_BTNSHADOW);
	CRect rect=m_arrButtonRect[nIndex];

	DrawFrameRect(pDC,rect,nIndex+1);

	CBrush brushFrame(clrShadow);
	rect.DeflateRect(2,2);
	pDC->FrameRect(&rect,&brushFrame);
	rect.DeflateRect(1,1);
    CBrush brush(PALETTERGB(GetRValue(m_arrData[nIndex]), 
		GetGValue(m_arrData[nIndex]), GetBValue(m_arrData[nIndex])));
	pDC->FillRect(&rect,&brush);
}

// draw default button
void COXColorPickerCtrl::DrawDefaultButton(CDC* pDC)
{
	ASSERT(IsDefaultButton());

	COLORREF clrShadow=::GetSysColor(COLOR_BTNSHADOW);
	CRect rect=m_rectDefaultButton;
	
	DrawFrameRect(pDC,rect,0);

	CBrush brushFrame(clrShadow);
	rect.DeflateRect(2,2);
	pDC->FrameRect(&rect,&brushFrame);
	rect.DeflateRect(1,1);

	if(m_dwDefault!=ID_CLRPICK_COLOR_NONE)
	{
		CRect rectColor=rect;
		rectColor.right=rectColor.left+rectColor.Height();
		rectColor.bottom=rectColor.top+rectColor.Height();

		rectColor.DeflateRect(1,1);
		CBrush brushFrame(clrShadow);
		pDC->FrameRect(&rectColor,&brushFrame);
		rectColor.DeflateRect(1,1);
	    CBrush brush(PALETTERGB(GetRValue(m_dwDefault), GetGValue(m_dwDefault), 
			GetBValue(m_dwDefault)));
		pDC->FillRect(&rectColor,&brush);

		rect.OffsetRect(rectColor.Width(),0);
	}
	
	CFont* pOldFont=(CFont*)pDC->SelectObject(&m_font);
	int oldBkMode=pDC->SetBkMode(TRANSPARENT);					
	pDC->DrawText(m_sDefaultButtonText,&rect,DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS);
	pDC->SetBkMode(oldBkMode);					
	pDC->SelectObject(pOldFont);
}


#define PALVERSION			0x300
#define MAX_PALETTE_ENTRIES 0x100
// Well, the class is for professional, so we provide as
// much functionality as we can. You can even populate 
// array of displayed colors with entries from current palette. 
// Following function retrieves current system palette
BOOL COXColorPickerCtrl::GetSystemPalette(CPalette* pPalette)
{
	ASSERT(pPalette != NULL);

    HDC hDC;                // handle to a DC
    HANDLE hLogPal;         // handle to a logical palette
    LPLOGPALETTE lpLogPal;  // pointer to a logical palette

    // Find out how many palette entries we want.
    hDC=::GetDC(NULL);
    if(hDC==NULL)
	{
        return FALSE;
	}

    // For non-palette devices, there's no palette available
    if((::GetDeviceCaps(hDC, RASTERCAPS) & RC_PALETTE) != RC_PALETTE)
	{
		TRACE0("COXColorPickerCtrl::GetSystemPalette : No palette device\n");
		return FALSE;
	}

    // Find out the number of palette entries on this device.
    int nColors = GetDeviceCaps(hDC, SIZEPALETTE);

    // Sometimes we'll use the # of system reserved
    // colors for our palette size.
    if(nColors==0)
	{
        nColors=GetDeviceCaps(hDC, NUMCOLORS);
	}

    ASSERT(nColors>0);

    // Allocate room for the palette and lock it.

  	hLogPal=::GlobalAlloc(GHND, sizeof(LOGPALETTE)+sizeof(PALETTEENTRY)*nColors);

	// if not enough memory, clean up and return NULL 
	if (hLogPal==NULL)
	{
		TRACE0("COXColorPickerCtrl::GetSystemPalette : Not enough memory for logical  palette\n");
		::ReleaseDC(NULL, hDC);
		return FALSE;
	}

    // get a pointer to the logical palette
	lpLogPal=(LPLOGPALETTE)::GlobalLock((HGLOBAL)hLogPal);

	// set version and number of palette entries
	lpLogPal->palVersion=PALVERSION;
	lpLogPal->palNumEntries=(WORD)nColors;

    // Copy the current system palette into our logical palette
    ::GetSystemPaletteEntries(hDC, 0, nColors,
            (LPPALETTEENTRY)(lpLogPal->palPalEntry));

    // Go ahead and create the palette.  Once it's created,
    // we no longer need the LOGPALETTE, so free it.    
	// -- create the palette and get handle to it
	BOOL bResult=pPalette->CreatePalette(lpLogPal);
	::GlobalUnlock((HGLOBAL)hLogPal);
	::GlobalFree((HGLOBAL)hLogPal);

    ::ReleaseDC(NULL, hDC);

    return bResult;
}

BOOL COXColorPickerCtrl::BuildCustomPalette(COLORREF* arrColor, UINT nEntries, 
											CPalette* pPalette)
{
	ASSERT(pPalette!=NULL);
	if((HPALETTE)pPalette!=NULL)
	{
		pPalette->DeleteObject();
	}

	nEntries=__min(nEntries,MAX_PALETTE_ENTRIES);

    // Create the palette
    struct {
        LOGPALETTE    LogPalette;
        PALETTEENTRY  PalEntry[MAX_PALETTE_ENTRIES];
    } pal;

    LOGPALETTE* pLogPalette = (LOGPALETTE*) &pal;
    pLogPalette->palVersion=PALVERSION;
    pLogPalette->palNumEntries=(BYTE)nEntries; 

    for(int nIndex=0; nIndex<(int)nEntries; nIndex++)
    {
        pLogPalette->palPalEntry[nIndex].peRed=GetRValue(arrColor[nIndex]);
        pLogPalette->palPalEntry[nIndex].peGreen=GetGValue(arrColor[nIndex]);
        pLogPalette->palPalEntry[nIndex].peBlue=GetBValue(arrColor[nIndex]);
        pLogPalette->palPalEntry[nIndex].peFlags=0;
    }

    return pPalette->CreatePalette(pLogPalette);
}


BOOL COXColorPickerCtrl::BuildCustomPalette(ButtonDataTable* arrColor, 
											CPalette* pPalette)
{
	ASSERT(pPalette!=NULL);
	if((HPALETTE)pPalette!=NULL)
	{
		pPalette->DeleteObject();
	}

	UINT nEntries=PtrToUint(arrColor->GetSize()); 
	nEntries=__min(nEntries,MAX_PALETTE_ENTRIES);

    // Create the palette
    struct {
        LOGPALETTE    LogPalette;
        PALETTEENTRY  PalEntry[MAX_PALETTE_ENTRIES];
    } pal;

    LOGPALETTE* pLogPalette = (LOGPALETTE*) &pal;
    pLogPalette->palVersion=PALVERSION;
    pLogPalette->palNumEntries=(BYTE)nEntries; 

    for(int nIndex=0; nIndex<(int)nEntries; nIndex++)
    {
        pLogPalette->palPalEntry[nIndex].peRed=GetRValue(arrColor->GetAt(nIndex));
        pLogPalette->palPalEntry[nIndex].peGreen=GetGValue(arrColor->GetAt(nIndex));
        pLogPalette->palPalEntry[nIndex].peBlue=GetBValue(arrColor->GetAt(nIndex));
        pLogPalette->palPalEntry[nIndex].peFlags=0;
    }

    return pPalette->CreatePalette(pLogPalette);
}


