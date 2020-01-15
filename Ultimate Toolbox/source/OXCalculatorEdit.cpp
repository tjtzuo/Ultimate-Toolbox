// OXCalculatorEdit.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXCalculatorEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXCalculatorEdit

IMPLEMENT_DYNCREATE(COXCalculatorEdit, COXNumericEdit)

BEGIN_MESSAGE_MAP(COXCalculatorEdit, COXNumericEdit)
	//{{AFX_MSG_MAP(COXCalculatorEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


COXCalculatorEdit::COXCalculatorEdit()
{
}

/////////////////////////////////////////////////////////////////////

BOOL COXCalculatorEdit::InitializeDropEdit()
{
	if(!COXDropEdit<COXNumericEdit>::InitializeDropEdit())
		return FALSE;

	ASSERT(!::IsWindow(m_calculatorPopup.GetSafeHwnd()));
	if(!m_calculatorPopup.Create(this))
	{
		TRACE(_T("COXCalculatorEdit::InitializeDropEdit: Creation of COXCalculatorPopup failed\n"));
		return FALSE;
	}

	SetFocus(); //Nish - June 14, 2005

	return TRUE;
}


void COXCalculatorEdit::OnDropButton()
{
	ASSERT(::IsWindow(m_calculatorPopup.m_hWnd));

	if(m_calculatorPopup.Pick(GetValue(),
		(GetButtonAlignment()==OXDROPEDIT_BUTTONLEFT ? 
		ID_OXCALCULATOR_ALIGNBOTTOM : ID_OXCALCULATOR_ALIGNBOTTOMRIGHT)))
	{
		SetValue(m_calculatorPopup.GetResult());
	}
	SetFocus();
}


