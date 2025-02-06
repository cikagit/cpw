// DlgOpciones.cpp : implementation file
//

#include "stdafx.h"
#include "cpw.h"
#include "cpwDefs.h"
#include "ObArrOrd.h"
#include "AbSpan.h"
#include "cpwExt.h"
#include "prog.h"
#include "cpwDb.h"
#include "cache.h"            
#include "ProgMaq.h"
#include "ChildFrm.h"
#include "cpwDoc.h"
#include "cpwView.h"

#include "DlgOpciones.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOpciones dialog


CDlgOpciones::CDlgOpciones(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOpciones::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOpciones)
	m_Max3 = 0;
	m_Max4 = 0;
	m_Max_5 = 0;
	m_Min_2 = 0;
	m_Min_3 = 0;
	m_Min_4 = 0;
	//}}AFX_DATA_INIT
}


void CDlgOpciones::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOpciones)
	DDX_Control(pDX, IDC_RICHEDIT6, m_DispColor6);
	DDX_Control(pDX, IDC_RICHEDIT5, m_DispColor5);
	DDX_Control(pDX, IDC_RICHEDIT4, m_DispColor4);
	DDX_Control(pDX, IDC_RICHEDIT3, m_DispColor3);
	DDX_Control(pDX, IDC_RICHEDIT2, m_DispColor2);
	DDX_Control(pDX, IDC_RICHEDIT1, m_DispColor1);
	DDX_Text(pDX, IDC_MARGEN_MAX_3, m_Max3);
	DDX_Text(pDX, IDC_MARGEN_MAX_4, m_Max4);
	DDX_Text(pDX, IDC_MARGEN_MAX_5, m_Max_5);
	DDX_Text(pDX, IDC_MARGEN_MIN_2, m_Min_2);
	DDX_Text(pDX, IDC_MARGEN_MIN_3, m_Min_3);
	DDX_Text(pDX, IDC_MARGEN_MIN_4, m_Min_4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOpciones, CDialog)
	//{{AFX_MSG_MAP(CDlgOpciones)
	ON_BN_CLICKED(IDC_COLOR_0, OnColor0)
	ON_BN_CLICKED(IDC_COLOR_1, OnColor1)
	ON_BN_CLICKED(IDC_COLOR_2, OnColor2)
	ON_BN_CLICKED(IDC_COLOR_3, OnColor3)
	ON_BN_CLICKED(IDC_COLOR_4, OnColor4)
	ON_BN_CLICKED(IDC_COLOR_5, OnColor5)
	ON_EN_CHANGE(IDC_MARGEN_MIN_2, OnChangeMargenMin2)
	ON_EN_CHANGE(IDC_MARGEN_MIN_4, OnChangeMargenMin4)
	ON_EN_CHANGE(IDC_MARGEN_MIN_3, OnChangeMargenMin3)
	ON_EN_CHANGE(IDC_MARGEN_MAX_5, OnChangeMargenMax5)
	ON_EN_CHANGE(IDC_MARGEN_MAX_4, OnChangeMargenMax4)
	ON_EN_CHANGE(IDC_MARGEN_MAX_3, OnChangeMargenMax3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOpciones message handlers

COLORREF
CDlgOpciones::OnColor(int p_iNum)
{
	CCpwApp* pApp = (CCpwApp*) AfxGetApp();
 
	CString sInt;
	//sInt = '0' + p_iNum;
	sInt.Format("%d", p_iNum);
	CColorDialog ColorDialog;
	ColorDialog.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	ColorDialog.m_cc.rgbResult = pApp->GetProfileInt("Valores Iniciales", "Color Peligro " + sInt, TK_COLOR_0);
	
	
	if (ColorDialog.DoModal() == IDOK)
	{
		pApp->WriteProfileInt("Valores Iniciales", "Color Peligro " + sInt, ColorDialog.m_cc.rgbResult);
	}
	
	return ColorDialog.m_cc.rgbResult;
}

int
CDlgOpciones::SetValueMargen(const CString& p_sMargenID, int p_sValue)
{
	CCpwApp* pApp = (CCpwApp*) AfxGetApp();
 
	pApp->WriteProfileInt("Valores Iniciales", "Margen " + p_sMargenID, p_sValue);

	return p_sValue;
}

void CDlgOpciones::OnColor0() 
{
	g_MC.m_Color1 = OnColor(0);
	m_DispColor1.SetBackgroundColor(FALSE, g_MC.m_Color1);
}
void CDlgOpciones::OnColor1() 
{
	g_MC.m_Color2 = OnColor(1);
	m_DispColor2.SetBackgroundColor(FALSE, g_MC.m_Color2);
}
void CDlgOpciones::OnColor2() 
{
	g_MC.m_Color3 = OnColor(2);
	m_DispColor3.SetBackgroundColor(FALSE, g_MC.m_Color3);
}
void CDlgOpciones::OnColor3() 
{
	g_MC.m_Color4 = OnColor(3);
	m_DispColor4.SetBackgroundColor(FALSE, g_MC.m_Color4);
}
void CDlgOpciones::OnColor4() 
{
	g_MC.m_Color5 = OnColor(4);
	m_DispColor5.SetBackgroundColor(FALSE, g_MC.m_Color5);
}
void CDlgOpciones::OnColor5() 
{
	g_MC.m_Color6 = OnColor(5);
	m_DispColor6.SetBackgroundColor(FALSE, g_MC.m_Color6);
}


void CDlgOpciones::OnChangeMargenMin2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	const int BufLen = 100;
	char Buffer[BufLen];

	GetDlgItemText(IDC_MARGEN_MIN_2, Buffer, BufLen);
	g_MC.m_iMin2 = SetValueMargen("Min_2", atoi(Buffer));

}

void CDlgOpciones::OnChangeMargenMin4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	const int BufLen = 100;
	char Buffer[BufLen];

	GetDlgItemText(IDC_MARGEN_MIN_4, Buffer, BufLen);
	g_MC.m_iMin4 = SetValueMargen("Min_4", atoi(Buffer));
	
}

void CDlgOpciones::OnChangeMargenMin3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	const int BufLen = 100;
	char Buffer[BufLen];

	GetDlgItemText(IDC_MARGEN_MIN_3, Buffer, BufLen);
	g_MC.m_iMin3 = SetValueMargen("Min_3", atoi(Buffer));
}

void CDlgOpciones::OnChangeMargenMax5() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	const int BufLen = 100;
	char Buffer[BufLen];

	GetDlgItemText(IDC_MARGEN_MAX_5, Buffer, BufLen);
	g_MC.m_iMax5 = SetValueMargen("Max_5", atoi(Buffer));
	
}

void CDlgOpciones::OnChangeMargenMax4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	const int BufLen = 100;
	char Buffer[BufLen];

	GetDlgItemText(IDC_MARGEN_MAX_4, Buffer, BufLen);
	g_MC.m_iMax4 = SetValueMargen("Max_4", atoi(Buffer));
	
	
}

void CDlgOpciones::OnChangeMargenMax3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	const int BufLen = 100;
	char Buffer[BufLen];

	GetDlgItemText(IDC_MARGEN_MAX_3, Buffer, BufLen);
	g_MC.m_iMax3 = SetValueMargen("Max_3", atoi(Buffer));
	
	
}

BOOL CDlgOpciones::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWinApp* pApp = AfxGetApp();
	
	CString sText = FormatLong(g_MC.m_iMax3, 8, NoPoint);
	SetDlgItemText(IDC_MARGEN_MAX_3, sText);
	
	sText = FormatLong(g_MC.m_iMax4, 8, NoPoint);
	SetDlgItemText(IDC_MARGEN_MAX_4, sText);

	sText = FormatLong(g_MC.m_iMax5, 8, NoPoint);
	SetDlgItemText(IDC_MARGEN_MAX_5, sText);

	sText = FormatLong(g_MC.m_iMin2, 8, NoPoint);
	SetDlgItemText(IDC_MARGEN_MIN_2, sText);
	
	sText = FormatLong(g_MC.m_iMin3, 8, NoPoint);
	SetDlgItemText(IDC_MARGEN_MIN_3, sText);
	
	sText = FormatLong(g_MC.m_iMin4, 8, NoPoint);
	SetDlgItemText(IDC_MARGEN_MIN_4, sText);
	
	m_DispColor1.SetBackgroundColor(FALSE, g_MC.m_Color1);
	m_DispColor2.SetBackgroundColor(FALSE, g_MC.m_Color2);
	m_DispColor3.SetBackgroundColor(FALSE, g_MC.m_Color3);
	m_DispColor4.SetBackgroundColor(FALSE, g_MC.m_Color4);
	m_DispColor5.SetBackgroundColor(FALSE, g_MC.m_Color5);
	m_DispColor6.SetBackgroundColor(FALSE, g_MC.m_Color6);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
