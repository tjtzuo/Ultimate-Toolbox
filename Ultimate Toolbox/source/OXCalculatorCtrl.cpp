// OXCalculatorCtrl.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXCalculatorCtrl.h"

#include <math.h>

#include "UTBStrOp.h"
#include "UTB64Bit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXCalculatorCtrl dialog


COXCalculatorCtrl::COXCalculatorCtrl(CWnd* pParent /*=NULL*/, double dValue/*=0*/)
	: CDialog(COXCalculatorCtrl::IDD, pParent)
{
	//{{AFX_DATA_INIT(COXCalculatorCtrl)
	m_bScientific=FALSE;
	m_nNumberSystem=0;
	m_sDisplayString = _T("");
	//}}AFX_DATA_INIT

	m_nFractionalCount=0;

	m_dValue=dValue;
	SetOperand(m_dValue,TRUE);
	m_operator=OpNone;
	m_lastCompletedOperator=OpNone;

	// get loacale setting
	m_chDecimalSeparator=_T('.');	
	int nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SDECIMAL,NULL,0);
	if(nLength>0)
	{
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SDECIMAL,
			&m_chDecimalSeparator,nLength);
	}

	// Note that LoadAccelerator does not require DestroyAcceleratorTable
	m_hAccel=LoadAccelerators(AfxGetResourceHandle(),MAKEINTRESOURCE(IDD));
}


BOOL COXCalculatorCtrl::Create(CWnd* pParentWnd)
{
	ASSERT(pParentWnd!=NULL);
	return CDialog::Create(IDD,pParentWnd);
}


void COXCalculatorCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXCalculatorCtrl)
	DDX_Control(pDX, IDC_OX_COMBO_NUMBER_SYSTEM, m_comboNumberSystem);
	DDX_Control(pDX, IDC_OX_BUTTON_NATURAL_LOGARITHM, m_btnNaturalAlgorithm);
	DDX_Control(pDX, IDC_OX_BUTTON_LOGARITHM, m_btnLogarithm);
	DDX_Control(pDX, IDC_OX_BUTTON_F, m_btnF);
	DDX_Control(pDX, IDC_OX_BUTTON_E, m_btnE);
	DDX_Control(pDX, IDC_OX_BUTTON_D, m_btnD);
	DDX_Control(pDX, IDC_OX_BUTTON_C, m_btnC);
	DDX_Control(pDX, IDC_OX_BUTTON_B, m_btnB);
	DDX_Control(pDX, IDC_OX_BUTTON_A, m_btnA);
	DDX_Control(pDX, IDC_OX_STATIC_DISPLAY, m_ctlDisplay);
	DDX_Control(pDX, IDC_OX_BUTTON_TAN, m_btnTan);
	DDX_Control(pDX, IDC_OX_BUTTON_SIN, m_btnSin);
	DDX_Control(pDX, IDC_OX_BUTTON_POWER, m_btnPower);
	DDX_Control(pDX, IDC_OX_BUTTON_PI, m_btnPi);
	DDX_Control(pDX, IDC_OX_BUTTON_FACTORIAL, m_btnFactorial);
	DDX_Control(pDX, IDC_OX_BUTTON_COTAN, m_btnCotan);
	DDX_Control(pDX, IDC_OX_BUTTON_COS, m_btnCos);
	DDX_Control(pDX, IDC_OX_BUTTON_SUBTRACT, m_btnSubtract);
	DDX_Control(pDX, IDC_OX_BUTTON_SQRT, m_btnSqrt);
	DDX_Control(pDX, IDC_OX_BUTTON_RECIPROCAL, m_btnReciprocal);
	DDX_Control(pDX, IDC_OX_BUTTON_MULTIPLY, m_btnMultiply);
	DDX_Control(pDX, IDC_OX_BUTTON_MOD, m_btnMod);
	DDX_Control(pDX, IDC_OX_BUTTON_EQUAL, m_btnEqual);
	DDX_Control(pDX, IDC_OX_BUTTON_DIVIDE, m_btnDivide);
	DDX_Control(pDX, IDC_OX_BUTTON_DELIMITER, m_btnDelimiter);
	DDX_Control(pDX, IDC_OX_BUTTON_CLEARNUMBER, m_btnClearNumber);
	DDX_Control(pDX, IDC_OX_BUTTON_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_OX_BUTTON_CHANGESIGN, m_btnChangeSign);
	DDX_Control(pDX, IDC_OX_BUTTON_BACK, m_btnBack);
	DDX_Control(pDX, IDC_OX_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_OX_BUTTON_9, m_btn9);
	DDX_Control(pDX, IDC_OX_BUTTON_8, m_btn8);
	DDX_Control(pDX, IDC_OX_BUTTON_7, m_btn7);
	DDX_Control(pDX, IDC_OX_BUTTON_6, m_btn6);
	DDX_Control(pDX, IDC_OX_BUTTON_5, m_btn5);
	DDX_Control(pDX, IDC_OX_BUTTON_4, m_btn4);
	DDX_Control(pDX, IDC_OX_BUTTON_3, m_btn3);
	DDX_Control(pDX, IDC_OX_BUTTON_2, m_btn2);
	DDX_Control(pDX, IDC_OX_BUTTON_1, m_btn1);
	DDX_Control(pDX, IDC_OX_BUTTON_0, m_btn0);
	DDX_Check(pDX, IDC_OX_CHECK_SCIENTIFIC, m_bScientific);
	DDX_CBIndex(pDX, IDC_OX_COMBO_NUMBER_SYSTEM, m_nNumberSystem);
	DDX_Text(pDX, IDC_OX_STATIC_DISPLAY, m_sDisplayString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COXCalculatorCtrl, CDialog)
	//{{AFX_MSG_MAP(COXCalculatorCtrl)
	ON_BN_CLICKED(IDC_OX_BUTTON_0, OnButton0)
	ON_BN_CLICKED(IDC_OX_BUTTON_1, OnButton1)
	ON_BN_CLICKED(IDC_OX_BUTTON_2, OnButton2)
	ON_BN_CLICKED(IDC_OX_BUTTON_3, OnButton3)
	ON_BN_CLICKED(IDC_OX_BUTTON_4, OnButton4)
	ON_BN_CLICKED(IDC_OX_BUTTON_5, OnButton5)
	ON_BN_CLICKED(IDC_OX_BUTTON_6, OnButton6)
	ON_BN_CLICKED(IDC_OX_BUTTON_7, OnButton7)
	ON_BN_CLICKED(IDC_OX_BUTTON_8, OnButton8)
	ON_BN_CLICKED(IDC_OX_BUTTON_9, OnButton9)
	ON_BN_CLICKED(IDC_OX_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_OX_BUTTON_BACK, OnButtonBack)
	ON_BN_CLICKED(IDC_OX_BUTTON_CHANGESIGN, OnButtonChangesign)
	ON_BN_CLICKED(IDC_OX_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_OX_BUTTON_CLEARNUMBER, OnButtonClearnumber)
	ON_BN_CLICKED(IDC_OX_BUTTON_DELIMITER, OnButtonDelimiter)
	ON_BN_CLICKED(IDC_OX_BUTTON_DIVIDE, OnButtonDivide)
	ON_BN_CLICKED(IDC_OX_BUTTON_EQUAL, OnButtonEqual)
	ON_BN_CLICKED(IDC_OX_BUTTON_MOD, OnButtonMod)
	ON_BN_CLICKED(IDC_OX_BUTTON_MULTIPLY, OnButtonMultiply)
	ON_BN_CLICKED(IDC_OX_BUTTON_RECIPROCAL, OnButtonReciprocal)
	ON_BN_CLICKED(IDC_OX_BUTTON_SQRT, OnButtonSqrt)
	ON_BN_CLICKED(IDC_OX_BUTTON_SUBTRACT, OnButtonSubtract)
	ON_BN_CLICKED(IDC_OX_BUTTON_A, OnButtonA)
	ON_BN_CLICKED(IDC_OX_BUTTON_B, OnButtonB)
	ON_BN_CLICKED(IDC_OX_BUTTON_C, OnButtonC)
	ON_BN_CLICKED(IDC_OX_BUTTON_COS, OnButtonCos)
	ON_BN_CLICKED(IDC_OX_BUTTON_COTAN, OnButtonCotan)
	ON_BN_CLICKED(IDC_OX_BUTTON_D, OnButtonD)
	ON_BN_CLICKED(IDC_OX_BUTTON_E, OnButtonE)
	ON_BN_CLICKED(IDC_OX_BUTTON_F, OnButtonF)
	ON_BN_CLICKED(IDC_OX_BUTTON_FACTORIAL, OnButtonFactorial)
	ON_BN_CLICKED(IDC_OX_BUTTON_LOGARITHM, OnButtonLogarithm)
	ON_BN_CLICKED(IDC_OX_BUTTON_NATURAL_LOGARITHM, OnButtonNaturalLogarithm)
	ON_BN_CLICKED(IDC_OX_BUTTON_PI, OnButtonPi)
	ON_BN_CLICKED(IDC_OX_BUTTON_POWER, OnButtonPower)
	ON_BN_CLICKED(IDC_OX_BUTTON_SIN, OnButtonSin)
	ON_BN_CLICKED(IDC_OX_BUTTON_TAN, OnButtonTan)
	ON_BN_CLICKED(IDC_OX_CHECK_SCIENTIFIC, OnCheckScientific)
	ON_CBN_SELCHANGE(IDC_OX_COMBO_NUMBER_SYSTEM, OnSelchangeComboNumberSystem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCalculatorCtrl message handlers

BOOL COXCalculatorCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if((pMsg->message==WM_KEYDOWN) && 
		(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE))
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}

	if(m_hAccel!=NULL && ::TranslateAccelerator(GetSafeHwnd(),m_hAccel,pMsg))
	{
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL COXCalculatorCtrl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// allow navigating using TAB key
	ModifyStyleEx(0,WS_EX_CONTROLPARENT);

	// setup culculator buttons
	//
	SetupButton(&m_btnF,RGB(0,0,128));
	SetupButton(&m_btnE,RGB(0,0,128));
	SetupButton(&m_btnD,RGB(0,0,128));
	SetupButton(&m_btnC,RGB(0,0,128));
	SetupButton(&m_btnB,RGB(0,0,128));
	SetupButton(&m_btnA,RGB(0,0,128));
	SetupButton(&m_btnPi,RGB(0,0,128));
	SetupButton(&m_btnNaturalAlgorithm,RGB(255,0,0));
	SetupButton(&m_btnLogarithm,RGB(255,0,0));
	SetupButton(&m_btnTan,RGB(255,0,0));
	SetupButton(&m_btnSin,RGB(255,0,0));
	SetupButton(&m_btnPower,RGB(255,0,0));
	SetupButton(&m_btnFactorial,RGB(255,0,0));
	SetupButton(&m_btnCotan,RGB(255,0,0));
	SetupButton(&m_btnCos,RGB(255,0,0));
	SetupButton(&m_btnSubtract,RGB(255,0,0));
	SetupButton(&m_btnSqrt,RGB(255,0,0));
	SetupButton(&m_btnReciprocal,RGB(255,0,0));
	SetupButton(&m_btnMultiply,RGB(255,0,0));
	SetupButton(&m_btnMod,RGB(255,0,0));
	SetupButton(&m_btnEqual,RGB(255,0,0));
	SetupButton(&m_btnDivide,RGB(255,0,0));
	SetupButton(&m_btnAdd,RGB(255,0,0));
	SetupButton(&m_btnClearNumber,RGB(128,0,0));
	SetupButton(&m_btnClear,RGB(128,0,0));
	SetupButton(&m_btnBack,RGB(128,0,0));
	SetupButton(&m_btnDelimiter,RGB(0,0,255));
	SetupButton(&m_btnChangeSign,RGB(0,0,255));
	SetupButton(&m_btn9,RGB(0,0,255));
	SetupButton(&m_btn8,RGB(0,0,255));
	SetupButton(&m_btn7,RGB(0,0,255));
	SetupButton(&m_btn6,RGB(0,0,255));
	SetupButton(&m_btn5,RGB(0,0,255));
	SetupButton(&m_btn4,RGB(0,0,255));
	SetupButton(&m_btn3,RGB(0,0,255));
	SetupButton(&m_btn2,RGB(0,0,255));
	SetupButton(&m_btn1,RGB(0,0,255));
	SetupButton(&m_btn0,RGB(0,0,255));
	//
	/////////////////////////////////////

	m_ctlDisplay.SetBkColor(::GetSysColor(COLOR_WINDOW));

	ShowControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COXCalculatorCtrl::SetupButton(COXBitmapButton* pButton, 
									COLORREF clrText)
{
	ASSERT(pButton!=NULL);

	pButton->SetToolTip(TRUE);
	pButton->SetTextColor(clrText);
	pButton->SetTextOffset(CPoint(-4,-4));
	pButton->SetInnerFocusOffset(CPoint(-3,-3));

	static CFont font;
	if((HFONT)font==NULL)
	{
		font.CreateFont(-8,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,NULL,NULL,NULL,NULL,NULL,
			_T("MS Sans Serif"));
	}
	pButton->SetTextFont(&font);
}


void COXCalculatorCtrl::ShowControls()
{
	if(!::IsWindow(GetSafeHwnd()))
		return;

	UpdateData(TRUE);

	m_btnNaturalAlgorithm.EnableWindow(m_bScientific);
	m_btnLogarithm.EnableWindow(m_bScientific);
	m_btnF.EnableWindow(m_bScientific && 
		m_nNumberSystem==ID_NUMBERSYSTEM_HEXADECIMAL);
	m_btnE.EnableWindow(m_bScientific && 
		m_nNumberSystem==ID_NUMBERSYSTEM_HEXADECIMAL);
	m_btnD.EnableWindow(m_bScientific && 
		m_nNumberSystem==ID_NUMBERSYSTEM_HEXADECIMAL);
	m_btnC.EnableWindow(m_bScientific && 
		m_nNumberSystem==ID_NUMBERSYSTEM_HEXADECIMAL);
	m_btnB.EnableWindow(m_bScientific && 
		m_nNumberSystem==ID_NUMBERSYSTEM_HEXADECIMAL);
	m_btnA.EnableWindow(m_bScientific && 
		m_nNumberSystem==ID_NUMBERSYSTEM_HEXADECIMAL);
	m_btnTan.EnableWindow(m_bScientific);
	m_btnSin.EnableWindow(m_bScientific);
	m_btnPower.EnableWindow(m_bScientific);
	m_btnFactorial.EnableWindow(m_bScientific);
	m_btnCotan.EnableWindow(m_bScientific);
	m_btnCos.EnableWindow(m_bScientific);
	m_btn2.EnableWindow(m_nNumberSystem!=ID_NUMBERSYSTEM_BINARY);
	m_btn3.EnableWindow(m_nNumberSystem!=ID_NUMBERSYSTEM_BINARY);
	m_btn4.EnableWindow(m_nNumberSystem!=ID_NUMBERSYSTEM_BINARY);
	m_btn5.EnableWindow(m_nNumberSystem!=ID_NUMBERSYSTEM_BINARY);
	m_btn6.EnableWindow(m_nNumberSystem!=ID_NUMBERSYSTEM_BINARY);
	m_btn7.EnableWindow(m_nNumberSystem!=ID_NUMBERSYSTEM_BINARY);
	m_btn8.EnableWindow(m_nNumberSystem!=ID_NUMBERSYSTEM_BINARY && 
		m_nNumberSystem!=ID_NUMBERSYSTEM_OCTAL);
	m_btn9.EnableWindow(m_nNumberSystem!=ID_NUMBERSYSTEM_BINARY && 
		m_nNumberSystem!=ID_NUMBERSYSTEM_OCTAL);
	m_btnPi.EnableWindow(m_nNumberSystem==ID_NUMBERSYSTEM_DECIMAL);
	m_comboNumberSystem.EnableWindow(m_bScientific);

	m_btnDelimiter.EnableWindow(m_nNumberSystem==0);

	if(!m_bScientific)
	{
		ASSERT(m_nNumberSystem==0);
	}

	switch(m_errorState)
	{
	case ErrNone:
		if(m_bOperandAvailable)
			m_sDisplayString=GetDisplayString(m_dOperand,m_nFractionalCount);
		else
			m_sDisplayString=GetDisplayString(m_dValue);
		break;
	case ErrDivideByZero:
		VERIFY(m_sDisplayString.LoadString(IDS_OX_CALCDIVIDEZERO)); //"Cannot divide by zero"
		break;
	case ErrOverflow:
		VERIFY(m_sDisplayString.LoadString(IDS_OX_RESULTTOOLARGE)); //"Result is too large"
		break;
	default:
		ASSERT(FALSE);
	}

	UpdateData(FALSE);
}

void COXCalculatorCtrl::OnCheckScientific() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(!m_bScientific && GetNumberSystem()!=ID_NUMBERSYSTEM_DECIMAL)
		SetNumberSystem(ID_NUMBERSYSTEM_DECIMAL);
	else
		ShowControls();
}

void COXCalculatorCtrl::OnSelchangeComboNumberSystem() 
{
	// TODO: Add your control notification handler code here
	int nOldNumberSystem=m_nNumberSystem;
	UpdateData();
	int nNewNumberSystem=m_nNumberSystem;
	m_nNumberSystem=nOldNumberSystem;
	SetNumberSystem(nNewNumberSystem);	
}


CString COXCalculatorCtrl::GetDisplayString(double dValue,
											int nFractionalCount/*=-1*/) const
{
	CString sText;
	switch(m_nNumberSystem)
	{
	case ID_NUMBERSYSTEM_DECIMAL:
		{
			int decimal, sign;
#if _MSC_VER >= 1400
			char buffer[MAX_PATH];
			_fcvt_s(buffer, MAX_PATH, dValue, 
				(nFractionalCount==-1 ? GetFractionalDigitCount(dValue) : nFractionalCount),
				&decimal,&sign);
#else
			char *buffer=fcvt(dValue,(nFractionalCount==-1 ? 
				GetFractionalDigitCount(dValue) : nFractionalCount),
				&decimal,&sign);
#endif
			int nSymbolCount=PtrToInt(strlen(buffer));
			TCHAR* chInputData=new TCHAR[nSymbolCount+1];
#ifdef _UNICODE
			UTBStr::mbstowcs(chInputData,nSymbolCount + 1,buffer,nSymbolCount);
#else
			UTBStr::strncpy(chInputData,nSymbolCount + 1,buffer,nSymbolCount);
#endif
			chInputData[nSymbolCount]=_T('\0');

			sText=chInputData;
			CString sDecimalSeparator(GetDecimalSeparator());
			if(!sText.IsEmpty())
			{
				for(int nIndex=decimal; nIndex<1; nIndex++)
				{
					sText=_T("0")+sText;
					decimal++;
					nSymbolCount++;
				}
				sText=sText.Left(decimal)+
					sDecimalSeparator+sText.Mid(decimal);
				if(sign!=0)
					sText=_T("-")+sText;
			}
			else
				sText=_T("0.");

			delete[] chInputData;

			break;
		}
	case ID_NUMBERSYSTEM_HEXADECIMAL:
		{
			sText.Format(_T("%x"),(int)dValue);
			break;
		}
	case ID_NUMBERSYSTEM_OCTAL:
		{
			sText.Format(_T("%o"),(int)dValue);
			break;
		}
	case ID_NUMBERSYSTEM_BINARY:
		{
			sText.Format(_T("%u"),(int)dValue);
			break;
		}
	default:
		ASSERT(FALSE);
	}
	return sText;
}


void COXCalculatorCtrl::SetNumberSystem(int nNumberSystem)
{
	ASSERT(nNumberSystem==ID_NUMBERSYSTEM_DECIMAL ||
		((nNumberSystem==ID_NUMBERSYSTEM_HEXADECIMAL ||
		nNumberSystem==ID_NUMBERSYSTEM_OCTAL ||
		nNumberSystem==ID_NUMBERSYSTEM_BINARY) & m_bScientific));

	if(m_nNumberSystem!=nNumberSystem)
	{
		m_nNumberSystem=nNumberSystem;
		UpdateData(FALSE);

		if(m_bOperandAvailable)
		{
			SetResult(m_dOperand);
			m_bOperandAvailable=FALSE;
		}

		if(m_nNumberSystem!=ID_NUMBERSYSTEM_DECIMAL)
		{
			m_bAddingFractional=FALSE;
		}

		ShowControls();
	}
}


void COXCalculatorCtrl::SetDecimalSeparator(TCHAR chDecimalSeparator) 
{
	ASSERT(!((chDecimalSeparator>=_T('0') && chDecimalSeparator<=_T('9')) ||
		chDecimalSeparator==_T('-') || chDecimalSeparator==NULL));
	if(m_chDecimalSeparator!=chDecimalSeparator)
	{
		m_chDecimalSeparator=chDecimalSeparator;
		if(::IsWindow(GetSafeHwnd()))
			ShowControls();
	}
}


void COXCalculatorCtrl::AddDigit(double dDigit)
{
	// check the digit
	switch(m_nNumberSystem)
	{
	case ID_NUMBERSYSTEM_DECIMAL:
		{
			if(dDigit<0 || dDigit>9)
				return;
			break;
		}
	case ID_NUMBERSYSTEM_HEXADECIMAL:
		{
			if(dDigit<0 || dDigit>15)
				return;
			break;
		}
	case ID_NUMBERSYSTEM_OCTAL:
		{
			if(dDigit<0 || dDigit>7)
				return;
			break;
		}
	case ID_NUMBERSYSTEM_BINARY:
		{
			if(dDigit<0 || dDigit>1)
				return;
			break;
		}
	default:
		ASSERT(FALSE);
	}
	//
	/////////////////////


	if(!m_bOperandAvailable)
	{
		m_dOperand=0;
		m_bAddingFractional=FALSE;
		m_bOperandAvailable=TRUE;
		m_nFractionalCount=0;
	}

	int sign=(m_dOperand<0 ? -1 : 1);

	switch(m_nNumberSystem)
	{
	case ID_NUMBERSYSTEM_DECIMAL:
		{
			if(m_nFractionalCount+GetDecimalDigitCount(m_dOperand)<MAX_DIGIT_COUNT)
			{
				if(m_bAddingFractional)
				{
					double dFractional=1;
					for(int nIndex=0; nIndex<=m_nFractionalCount;nIndex++)
					{
						dFractional/=(double)10;
					}
					m_nFractionalCount++;
					// SetOperand might change the number of fractional digits
					SetOperand(m_dOperand+sign*dDigit*dFractional);
				}
				else
					SetOperand(m_dOperand*(double)10+sign*dDigit);
			}
			break;
		}
	case ID_NUMBERSYSTEM_HEXADECIMAL:
		{
			SetOperand(m_dOperand*16+sign*dDigit);
			break;
		}
	case ID_NUMBERSYSTEM_OCTAL:
		{
			SetOperand(m_dOperand*8+sign*dDigit);
			break;
		}
	case ID_NUMBERSYSTEM_BINARY:
		{
			SetOperand(m_dOperand*2+sign*dDigit);
			break;
		}
	default:
		ASSERT(FALSE);
	}
}


void COXCalculatorCtrl::RemoveLastDigit()
{
	if(!m_bOperandAvailable)
		return;

	switch(m_nNumberSystem)
	{
	case ID_NUMBERSYSTEM_DECIMAL:
		{
			if(m_bAddingFractional)
			{
				ASSERT(m_nFractionalCount>0);
				double dFractional=10;
				double dFractionalMultiplier=1;
				for(int nIndex=0; nIndex<m_nFractionalCount; nIndex++)
				{
					dFractional/=(double)10;
					dFractionalMultiplier*=(double)10;
				}
				double dFractionalPart, dIntegerPart;
				dFractionalPart=modf(m_dOperand,&dIntegerPart);
				dFractionalPart*=dFractionalMultiplier;
				dFractionalPart=dFractionalPart/(double)10-
					fmod(dFractionalPart,(double)10)/(double)10;
				dFractionalPart*=dFractional;
				m_nFractionalCount--;
				SetOperand(dIntegerPart+dFractionalPart);
				if(m_nFractionalCount==0)
					m_bAddingFractional=FALSE;
			}
			else
				SetOperand(m_dOperand/(double)10-
					fmod(m_dOperand,(double)10)/(double)10);
			break;
		}
	case ID_NUMBERSYSTEM_HEXADECIMAL:
		{
			SetOperand(m_dOperand/(double)16-
				fmod(m_dOperand,(double)16)/(double)16);
			break;
		}
	case ID_NUMBERSYSTEM_OCTAL:
		{
			SetOperand(m_dOperand/(double)8-
				fmod(m_dOperand,(double)8)/(double)8);
			break;
		}
	case ID_NUMBERSYSTEM_BINARY:
		{
			SetOperand(m_dOperand/(double)2-
				fmod(m_dOperand,(double)2)/(double)2);
			break;
		}
	default:
		ASSERT(FALSE);
	}
}


int COXCalculatorCtrl::GetDecimalDigitCount(double dValue) const
{
	int nCount=0;
	double dIntegerPart=dValue;
	while(TRUE)
	{
		modf(dIntegerPart,&dIntegerPart);
		if(dIntegerPart==(double)0)
			break;
		nCount++;
		dIntegerPart/=(double)10;
	}

	return nCount;
}


int COXCalculatorCtrl::GetFractionalDigitCount(double dValue) const
{
	int decimal, sign;
#if _MSC_VER >= 1400
			int count = MAX_DIGIT_COUNT+MAX_FRACTIONAL_COUNT+2;
			char buffer[MAX_PATH];	
			_fcvt_s(buffer, MAX_PATH, dValue, 
				count,
				&decimal,&sign);
#else
			char *buffer=fcvt(dValue,MAX_DIGIT_COUNT,&decimal,&sign);
#endif

	int nSymbolCount=PtrToInt(strlen(buffer));
	int nCount=nSymbolCount-decimal;
	for(int nIndex=nSymbolCount-1; nIndex>=decimal; nIndex--)
	{
		if(buffer[nIndex]!='0')
			break;
		nCount--;
	}

	ASSERT(nCount>=0);
	return nCount;
}


void COXCalculatorCtrl::SetOperand(double dOperand, 
								   BOOL bRecalcLayout/*=FALSE*/)
{
	m_dOperand=dOperand;

	m_bOperandAvailable=TRUE;
	m_errorState=ErrNone;

	if(bRecalcLayout)
	{
		double dFractionalPart, dIntegerPart;
		dFractionalPart=modf(m_dOperand,&dIntegerPart);
		if(dFractionalPart!=(double)0)
		{
			m_bAddingFractional=TRUE;
			m_nFractionalCount=GetFractionalDigitCount(m_dOperand);
			if(m_nFractionalCount>MAX_FRACTIONAL_COUNT)
				m_nFractionalCount=MAX_FRACTIONAL_COUNT;
		}	
		else
		{
			m_bAddingFractional=FALSE;
			m_nFractionalCount=0;
		}
	}

	ShowControls();
}


void COXCalculatorCtrl::SetResult(double dValue)
{
	m_dValue=dValue;

	m_bOperandAvailable=FALSE;
	m_errorState=ErrNone;
	m_nFractionalCount=0;

	ShowControls();
}


void COXCalculatorCtrl::PerformUnaryOperation(Operator oper)
{
	m_operator=oper;
	if(!m_bOperandAvailable)
		m_dOperand=m_dValue;
	PerformLastOperation();
	m_operator=OpNone;
	m_lastCompletedOperator=OpNone;
}


void COXCalculatorCtrl::PerformBinaryOperation(Operator oper)
{
	if(m_bOperandAvailable)
		PerformLastOperation();
	m_operator=oper;
	if(!m_bOperandAvailable)
		m_dOperand=m_dValue;

}


void COXCalculatorCtrl::PerformLastOperation()
{
	// Return if in an invalid state
	if (m_errorState != ErrNone)
		return;

	// Perform the selected operation
	switch(m_operator)
	{
	case OpNone:
		m_dValue=m_dOperand;
		break;
	case OpSubtract:
		m_dValue-=m_dOperand;
		break;
	case OpAdd:
		m_dValue+=m_dOperand;
		break;
	case OpMultiply:
		m_dValue*=m_dOperand;
		break;
	case OpDivide:
		m_dValue/=m_dOperand;
		break;
	case OpMod:
		m_dValue=fmod(m_dValue,m_dOperand);
		break;
	case OpRaiseToPower:
		{
			double dValue=m_dValue;
			m_dValue=1;
			double dIntegerPart;
			modf(m_dOperand,&dIntegerPart);
			for(double dIndex=0; dIndex<dIntegerPart; dIndex++)
				m_dValue*=dValue;
		}
		break;
	case OpSin:
		m_dValue=sin(m_dOperand);
		break;
	case OpCos:
		m_dValue=cos(m_dOperand);
		break;
	case OpTan:
		m_dValue=tan(m_dOperand);
		break;
	case OpCotan:
		m_dValue=tanh(m_dOperand);
		break;
	case OpFactorial:
		{
			double dIntegerPart;
			modf(m_dOperand,&dIntegerPart);
			if((int)dIntegerPart!=0)
			{
				m_dValue=1;
				while(dIntegerPart>0)
				{
					m_dValue*=dIntegerPart;
					dIntegerPart--;
				}
			}
			else
				m_dValue=0;
		}
		break;
	case OpLogarithm:
		m_dValue=log10(m_dOperand);
		break;
	case OpNaturalLogarithm:
		m_dValue=log(m_dOperand);
		break;
	case OpReciprocal:
		m_dValue=(double)1/m_dOperand;
		break;
	case OpSqrt:
		m_dValue=sqrt(m_dOperand);
		break;
	default:
		ASSERT(FALSE);
	}
	m_lastCompletedOperator=m_operator;

	m_bOperandAvailable=FALSE;

	ShowControls();
}


void COXCalculatorCtrl::OnButton0() 
{
	// TODO: Add your control notification handler code here
	AddDigit(0);
}

void COXCalculatorCtrl::OnButton1() 
{
	// TODO: Add your control notification handler code here
	AddDigit(1);
}

void COXCalculatorCtrl::OnButton2() 
{
	// TODO: Add your control notification handler code here
	AddDigit(2);
}

void COXCalculatorCtrl::OnButton3() 
{
	// TODO: Add your control notification handler code here
	AddDigit(3);
}

void COXCalculatorCtrl::OnButton4() 
{
	// TODO: Add your control notification handler code here
	AddDigit(4);
}

void COXCalculatorCtrl::OnButton5() 
{
	// TODO: Add your control notification handler code here
	AddDigit(5);
}

void COXCalculatorCtrl::OnButton6() 
{
	// TODO: Add your control notification handler code here
	AddDigit(6);
}

void COXCalculatorCtrl::OnButton7() 
{
	// TODO: Add your control notification handler code here
	AddDigit(7);
}

void COXCalculatorCtrl::OnButton8() 
{
	// TODO: Add your control notification handler code here
	AddDigit(8);
}

void COXCalculatorCtrl::OnButton9() 
{
	// TODO: Add your control notification handler code here
	AddDigit(9);
}

void COXCalculatorCtrl::OnButtonA() 
{
	// TODO: Add your control notification handler code here
	AddDigit(10);
}

void COXCalculatorCtrl::OnButtonB() 
{
	// TODO: Add your control notification handler code here
	AddDigit(11);
}

void COXCalculatorCtrl::OnButtonC() 
{
	// TODO: Add your control notification handler code here
	AddDigit(12);
}

void COXCalculatorCtrl::OnButtonD() 
{
	// TODO: Add your control notification handler code here
	AddDigit(13);
}

void COXCalculatorCtrl::OnButtonE() 
{
	// TODO: Add your control notification handler code here
	AddDigit(14);
}

void COXCalculatorCtrl::OnButtonF() 
{
	// TODO: Add your control notification handler code here
	AddDigit(15);
}

void COXCalculatorCtrl::OnButtonBack() 
{
	// TODO: Add your control notification handler code here
	RemoveLastDigit();	
}

void COXCalculatorCtrl::OnButtonChangesign() 
{
	// TODO: Add your control notification handler code here
	if(m_bOperandAvailable)
		SetOperand(m_dOperand*-1);
	else
		SetResult(m_dValue*-1);
}

void COXCalculatorCtrl::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	SetResult(0);
}

void COXCalculatorCtrl::OnButtonClearnumber() 
{
	// TODO: Add your control notification handler code here
	if(m_bOperandAvailable)
		SetOperand(0,TRUE);
	else
		SetResult(0);
}

void COXCalculatorCtrl::OnButtonDelimiter() 
{
	// TODO: Add your control notification handler code here
	if(GetNumberSystem()==ID_NUMBERSYSTEM_DECIMAL)
	{
		if(!m_bOperandAvailable)
		{
			m_dOperand=0;
			m_bOperandAvailable=TRUE;
			ShowControls();
		}
		m_bAddingFractional=TRUE;	
	}
}

void COXCalculatorCtrl::OnButtonPi() 
{
	// TODO: Add your control notification handler code here
	if(GetNumberSystem()==ID_NUMBERSYSTEM_DECIMAL)
		SetOperand(PI,TRUE);
}

void COXCalculatorCtrl::OnButtonCos() 
{
	// TODO: Add your control notification handler code here
	PerformUnaryOperation(OpCos);
}

void COXCalculatorCtrl::OnButtonCotan() 
{
	// TODO: Add your control notification handler code here
	PerformUnaryOperation(OpCotan);
}

void COXCalculatorCtrl::OnButtonReciprocal() 
{
	// TODO: Add your control notification handler code here
	PerformUnaryOperation(OpReciprocal);
}

void COXCalculatorCtrl::OnButtonSqrt() 
{
	// TODO: Add your control notification handler code here
	PerformUnaryOperation(OpSqrt);
}

void COXCalculatorCtrl::OnButtonFactorial() 
{
	// TODO: Add your control notification handler code here
	PerformUnaryOperation(OpFactorial);
}

void COXCalculatorCtrl::OnButtonLogarithm() 
{
	// TODO: Add your control notification handler code here
	PerformUnaryOperation(OpLogarithm);
}

void COXCalculatorCtrl::OnButtonNaturalLogarithm() 
{
	// TODO: Add your control notification handler code here
	PerformUnaryOperation(OpNaturalLogarithm);
}

void COXCalculatorCtrl::OnButtonSin() 
{
	// TODO: Add your control notification handler code here
	PerformUnaryOperation(OpSin);
}

void COXCalculatorCtrl::OnButtonTan() 
{
	// TODO: Add your control notification handler code here
	PerformUnaryOperation(OpTan);
}

void COXCalculatorCtrl::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	PerformBinaryOperation(OpAdd);
}

void COXCalculatorCtrl::OnButtonDivide() 
{
	// TODO: Add your control notification handler code here
	PerformBinaryOperation(OpDivide);
}

void COXCalculatorCtrl::OnButtonMod() 
{
	// TODO: Add your control notification handler code here
	PerformBinaryOperation(OpMod);
}

void COXCalculatorCtrl::OnButtonMultiply() 
{
	// TODO: Add your control notification handler code here
	PerformBinaryOperation(OpMultiply);
}

void COXCalculatorCtrl::OnButtonSubtract() 
{
	// TODO: Add your control notification handler code here
	PerformBinaryOperation(OpSubtract);
}

void COXCalculatorCtrl::OnButtonPower() 
{
	// TODO: Add your control notification handler code here
	PerformBinaryOperation(OpRaiseToPower);
}

void COXCalculatorCtrl::OnButtonEqual() 
{
	// TODO: Add your control notification handler code here
	if(!m_bOperandAvailable)
	{
		if(m_operator==OpNone)
		{
			m_operator=m_lastCompletedOperator;
		}
	}
	PerformLastOperation();
	m_operator=OpNone;
}


////////////////////////////////////////////////////////////////


COXCalculatorPopup::COXCalculatorPopup()
{
	m_pParentWnd=NULL;
}


COXCalculatorPopup::~COXCalculatorPopup()
{
}


BEGIN_MESSAGE_MAP(COXCalculatorPopup, CWnd)
	//{{AFX_MSG_MAP(COXCalculatorPopup)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL COXCalculatorPopup::Create(CWnd* pParentWnd/*=NULL*/, 
								HBRUSH hbrBackground/*=NULL*/)
{
    ASSERT(pParentWnd==NULL || ::IsWindow(pParentWnd->GetSafeHwnd()));

	// creation of window
    // 
	if(hbrBackground==NULL)
	{
		hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
	}

	// define our own window class 
	WNDCLASS wndClass;
	wndClass.style=CS_SAVEBITS; 
    wndClass.lpfnWndProc=AfxWndProc; 
    wndClass.cbClsExtra=0; 
    wndClass.cbWndExtra=0; 
    wndClass.hInstance=AfxGetInstanceHandle(); 
    wndClass.hIcon=0; 
    wndClass.hCursor=::LoadCursor(NULL,IDC_ARROW); 
    wndClass.hbrBackground=hbrBackground; 
    wndClass.lpszMenuName=NULL; 
	wndClass.lpszClassName=_T("CalculatorPopupWindow");
	
	if(!AfxRegisterClass(&wndClass))
	{
		return FALSE;
	}

	CRect rect(0,0,0,0);
    if(!CWnd::CreateEx(
		WS_EX_DLGMODALFRAME|WS_EX_CONTROLPARENT,wndClass.lpszClassName,_T(""), 
		WS_POPUP,rect,pParentWnd,0,NULL))
	{
		TRACE(_T("COXCalculatorPopup::Create: failed to create popup window\n"));
        return FALSE;
	}

	// create calculator control
    if(!m_calculator.Create(this)) 
	{
		DestroyWindow();
		TRACE(_T("COXCalculatorPopup::Create: failed to create calculator control\n"));
		return FALSE;
	}
	m_calculator.ModifyStyle(NULL,WS_VISIBLE);

	// create "Ok" and "Cancel" buttons
	CString sName;
	VERIFY(sName.LoadString(IDS_OX_CALCOKBUTTON)); //"OK"
	if(!m_btnOk.Create(sName,
		WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON|BS_OWNERDRAW,rect,this,IDOK))
	{
		DestroyWindow();
		TRACE(_T("COXCalculatorPopup::Create: failed to create 'OK' button\n"));
		return FALSE;
	}
	VERIFY(sName.LoadString(IDS_OX_CALCCANCELBUTTON));//"Cancel"
	if(!m_btnCancel.Create(sName,
		WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON|BS_OWNERDRAW,rect,this,IDCANCEL))
	{
		DestroyWindow();
		TRACE(_T("COXCalculatorPopup::Create: failed to create 'Cancel' button\n"));
		return FALSE;
	}

	m_pParentWnd=pParentWnd;

    return TRUE;
}

BOOL COXCalculatorPopup::Pick(double dStartValue/*=0*/,
							  int nAlignment/*=ID_OXCALCULATOR_ALIGNBOTTOM*/, 
							  CRect rectParent/*=CRect(0,0,0,0)*/, 
							  CSize szOffset/*=CSize(0,0)*/)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(::IsWindow(m_calculator.GetSafeHwnd()));
	ASSERT(::IsWindow(m_btnOk.GetSafeHwnd()));
	ASSERT(::IsWindow(m_btnCancel.GetSafeHwnd()));

	// now we've got to calculate size of the window and the controls 
	// positioning
	if(!CalcWindowSize())
	{
		return FALSE;
	}


	// we've got size of window but we have to display it
	// in a nice spot (at least in the place user wants us to put in)
	if(!AdjustWindowPos(nAlignment,rectParent,szOffset))
	{
		return FALSE;
	}

	// it's show time!
	SetWindowPos(NULL,m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height(),
		SWP_NOZORDER|SWP_NOACTIVATE);
	m_calculator.MoveWindow(m_rectCalculator);
	m_btnOk.MoveWindow(m_rectBtnOk);
	m_btnCancel.MoveWindow(m_rectBtnCancel);
	ShowWindow(SW_SHOWNA);
	
	m_calculator.SetFocus();

	// set operand's value
	m_calculator.SetOperand(dStartValue,TRUE);
	m_calculator.m_operator=OpNone;
	m_calculator.m_lastCompletedOperator=OpNone;

	// init message loop
	// m_nState = 0 - control is active
	// m_nState = 1 - user pressed 'OK'
	// m_nState = 2 - user left the window without choosing any number
	m_nState=0;
	while(m_nState==0)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg,NULL,0,0));
		if(msg.message==WM_COMMAND)
		{
			ASSERT(FALSE);
		}

		CWnd* pWndHasFocus=GetFocus();
		if(pWndHasFocus!=this)
		{
			if(pWndHasFocus==NULL || !IsDescendant(this,pWndHasFocus))
				break;
		}

		if((msg.message==WM_KEYDOWN))
		{
			if(msg.wParam==VK_RETURN)
			{
				if(m_calculator.m_operator==OpNone &&
					m_calculator.m_lastCompletedOperator==OpNone)
				{
					m_calculator.PerformLastOperation();
				}
				OnOk();
				continue;
			}
			else if(msg.wParam==VK_ESCAPE)
			{
				OnCancel();
				continue;
			}
		}

		if(!m_calculator.PreTranslateMessage(&msg))
		{
			::DispatchMessage(&msg);
		}

	}

	ShowWindow(SW_HIDE);

	BOOL bResult=m_nState==1 ? TRUE : FALSE;

	return bResult;
}


void COXCalculatorPopup::OnOk()
{
	m_nState=1;
}


void COXCalculatorPopup::OnCancel()
{
	m_nState=2;
}


BOOL COXCalculatorPopup::CalcWindowSize()
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(::IsWindow(m_calculator.GetSafeHwnd()));
	ASSERT(::IsWindow(m_btnOk.GetSafeHwnd()));
	ASSERT(::IsWindow(m_btnCancel.GetSafeHwnd()));

	// calcurator position
	CRect rectCalculator;
	m_calculator.GetWindowRect(rectCalculator);
	m_rectCalculator.TopLeft()=
		CPoint(OXCALCULATORPOPUP_XMARGIN,OXCALCULATORPOPUP_YMARGIN);
	m_rectCalculator.right=OXCALCULATORPOPUP_XMARGIN+rectCalculator.Width();
	m_rectCalculator.bottom=OXCALCULATORPOPUP_YMARGIN+rectCalculator.Height();

	// OK button position
	m_rectBtnOk.left=m_rectCalculator.left;
	m_rectBtnOk.top=m_rectCalculator.bottom+OXCALCULATORPOPUP_YMARGIN;
	m_rectBtnOk.right=m_rectBtnOk.left+m_rectCalculator.Width()/2-
		OXCALCULATORPOPUP_XMARGIN;
	m_rectBtnOk.bottom=m_rectBtnOk.top+OXCALCULATORPOPUP_BUTTON_HEIGHT;

	// Cancel button position
	m_rectBtnCancel.right=m_rectCalculator.right;
	m_rectBtnCancel.top=m_rectBtnOk.top;
	m_rectBtnCancel.left=m_rectBtnCancel.right-m_rectCalculator.Width()/2+
		OXCALCULATORPOPUP_XMARGIN;
	m_rectBtnCancel.bottom=m_rectBtnOk.bottom;

	// popup window size 
	m_rect.SetRectEmpty();
	m_rect.right=m_rectCalculator.Width()+OXCALCULATORPOPUP_XMARGIN*2+
		::GetSystemMetrics(SM_CXDLGFRAME)*2;
	m_rect.bottom=m_rectCalculator.Height()+OXCALCULATORPOPUP_YMARGIN*3+
		OXCALCULATORPOPUP_BUTTON_HEIGHT+::GetSystemMetrics(SM_CYDLGFRAME)*2;

	return TRUE;
}

BOOL COXCalculatorPopup::AdjustWindowPos(int nAlignment/*=ID_OXCALCULATOR_ALIGNBOTTOM*/, 
										 CRect rectParent/*=CRect(0,0,0,0)*/, 
										 CSize szOffset/*=CSize(0,0)*/)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(nAlignment==ID_OXCALCULATOR_ALIGNLEFT || 
		nAlignment==ID_OXCALCULATOR_ALIGNRIGHT || 
		nAlignment==ID_OXCALCULATOR_ALIGNTOP || 
		nAlignment==ID_OXCALCULATOR_ALIGNBOTTOM ||
		nAlignment==ID_OXCALCULATOR_ALIGNLEFTBOTTOM || 
		nAlignment==ID_OXCALCULATOR_ALIGNRIGHTBOTTOM || 
		nAlignment==ID_OXCALCULATOR_ALIGNTOPRIGHT || 
		nAlignment==ID_OXCALCULATOR_ALIGNBOTTOMRIGHT);

	CRect rect;
	::SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
    int nScreenWidth=rect.Width();
	int nScreenHeight=rect.Height();

	if(m_rect.Width()>nScreenWidth || m_rect.Height()>nScreenHeight)
	{
		return FALSE;
	}

	CSize sizeOffset;

	if(rectParent.IsRectEmpty())
	{
		if(m_pParentWnd!=NULL)
			m_pParentWnd->GetWindowRect(rectParent);
		else
		{
			ASSERT(GetParent()!=NULL);
			GetParent()->GetWindowRect(rectParent);
		}
	}

	switch(nAlignment)
	{
	case ID_OXCALCULATOR_ALIGNLEFT:
		{
			sizeOffset.cx=rectParent.left-szOffset.cx-m_rect.Width();
			sizeOffset.cy=rectParent.top+szOffset.cy;
			if(sizeOffset.cx<0)
			{
				sizeOffset.cx=rectParent.right+szOffset.cx;
				if(sizeOffset.cx+m_rect.Width()>nScreenWidth)
				{
					return FALSE;
				}
			}
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	case ID_OXCALCULATOR_ALIGNLEFTBOTTOM:
		{
			sizeOffset.cx=rectParent.left-szOffset.cx-m_rect.Width();
			sizeOffset.cy=rectParent.bottom-szOffset.cy-m_rect.Height();
			if(sizeOffset.cx<0)
			{
				sizeOffset.cx=rectParent.right+szOffset.cx;
				if(sizeOffset.cx+m_rect.Width()>nScreenWidth)
				{
					return FALSE;
				}
			}
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	case ID_OXCALCULATOR_ALIGNRIGHT:
		{
			sizeOffset.cx=rectParent.right+szOffset.cx;
			sizeOffset.cy=rectParent.top+szOffset.cy;
			if(sizeOffset.cx+m_rect.Width()>nScreenWidth)
			{
				sizeOffset.cx=rectParent.left-szOffset.cx-m_rect.Width();
				if(sizeOffset.cx<0)
				{
					return FALSE;
				}
			}
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	case ID_OXCALCULATOR_ALIGNRIGHTBOTTOM:
		{
			sizeOffset.cx=rectParent.right+szOffset.cx;
			sizeOffset.cy=rectParent.bottom-szOffset.cy-m_rect.Height();
			if(sizeOffset.cx+m_rect.Width()>nScreenWidth)
			{
				sizeOffset.cx=rectParent.left-szOffset.cx-m_rect.Width();
				if(sizeOffset.cx<0)
				{
					return FALSE;
				}
			}
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	case ID_OXCALCULATOR_ALIGNTOP:
		{
			sizeOffset.cx=rectParent.left+szOffset.cx;
			sizeOffset.cy=rectParent.top-szOffset.cy-m_rect.Height();
			if(sizeOffset.cy<0)
			{
				sizeOffset.cy=rectParent.bottom+szOffset.cy;
				if(sizeOffset.cy+m_rect.Height()>nScreenHeight)
				{
					return FALSE;
				}
			}
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	case ID_OXCALCULATOR_ALIGNTOPRIGHT:
		{
			sizeOffset.cx=rectParent.right-szOffset.cx-m_rect.Width();
			sizeOffset.cy=rectParent.top-szOffset.cy-m_rect.Height();
			if(sizeOffset.cy<0)
			{
				sizeOffset.cy=rectParent.bottom+szOffset.cy;
				if(sizeOffset.cy+m_rect.Height()>nScreenHeight)
				{
					return FALSE;
				}
			}
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	case ID_OXCALCULATOR_ALIGNBOTTOM:
		{
			sizeOffset.cx=rectParent.left+szOffset.cx;
			sizeOffset.cy=rectParent.bottom+szOffset.cy;
			if(sizeOffset.cy+m_rect.Height()>nScreenHeight)
			{
				sizeOffset.cy=rectParent.top-szOffset.cy-m_rect.Height();
				if(sizeOffset.cy<0)
				{
					return FALSE;
				}
			}
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	case ID_OXCALCULATOR_ALIGNBOTTOMRIGHT:
		{
			sizeOffset.cx=rectParent.right-szOffset.cx-m_rect.Width();
			sizeOffset.cy=rectParent.bottom+szOffset.cy;
			if(sizeOffset.cy+m_rect.Height()>nScreenHeight)
			{
				sizeOffset.cy=rectParent.top-szOffset.cy-m_rect.Height();
				if(sizeOffset.cy<0)
				{
					return FALSE;
				}
			}
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	}

	sizeOffset.cx=0;
	sizeOffset.cy=0;
	if(m_rect.left<0)
	{
		sizeOffset.cx=-m_rect.left;
	}
	else if(m_rect.right>nScreenWidth)
	{
		sizeOffset.cx=nScreenWidth-m_rect.right;
	}
	if(m_rect.top<0)
	{
		sizeOffset.cy=-m_rect.top;
	}
	else if(m_rect.bottom>nScreenHeight)
	{
		sizeOffset.cy=nScreenHeight-m_rect.bottom;
	}
	m_rect.OffsetRect(sizeOffset);

	return TRUE;
}


BOOL COXCalculatorPopup::IsDescendant(CWnd* pWndParent, CWnd* pWndChild)
	// helper for detecting whether child descendent of parent
	//  (works with owned popups as well)
{
	ASSERT(pWndParent!=NULL);
	ASSERT(::IsWindow(pWndParent->GetSafeHwnd()));
	ASSERT(pWndChild!=NULL);
	ASSERT(::IsWindow(pWndChild->GetSafeHwnd()));

	HWND hWndParent=pWndParent->GetSafeHwnd();
	HWND hWndChild=pWndChild->GetSafeHwnd();
	do
	{
		if (hWndParent == hWndChild)
			return TRUE;

		// check for permanent-owned window first
		CWnd* pWnd=CWnd::FromHandlePermanent(hWndChild);
		if(pWnd!=NULL)
			hWndChild=pWnd->GetOwner()->GetSafeHwnd();
		else
			hWndChild=(::GetWindowLongPtr(hWndChild,GWL_STYLE)&WS_CHILD) ?
				::GetParent(hWndChild) : ::GetWindow(hWndChild,GW_OWNER);

	} while (hWndChild != NULL);

	return FALSE;
}


