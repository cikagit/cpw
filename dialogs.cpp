// Dialogs.cpp : implementation file
//

#include "stdafx.h"
#include "cpw.h"
#include "afxdtctl.h"
#include "AbSpan.h"
#include "cpwdb.h"
#include "cpwDefs.h"
#include "cpwExt.h"
#include "cache.h"
#include "ProgMaq.h"
#include "ObArrOrd.h" 
#include "OrdMaq.h"
#include "prog.h"
#include "cpwDoc.h"
#include "ChildFrm.h"
#include "CpwView.h"

#include "Dialogs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgEscala dialog



BOOL DlgEscala::OnInitDialog()
{
	(void) CDialog::OnInitDialog();

	m_SpinEscala.SetRange(0,9999);
	m_SpinEscala.SetAccel(1, &m_AccelArr);
	
	m_EditEscala.SetFocus();
	return FALSE;
}	

DlgEscala::DlgEscala(CWnd* pParent /*=NULL*/)
	: CDialog(DlgEscala::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgEscala)
	m_Escala = 0;
	//}}AFX_DATA_INIT

	m_AccelArr.nSec = 1;
	m_AccelArr.nInc = 10;
}


void DlgEscala::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgEscala)
	DDX_Control(pDX, IDC_ESCALA, m_EditEscala);
	DDX_Control(pDX, IDC_SPIN_ESCALA, m_SpinEscala);
	DDX_Text(pDX, IDC_ESCALA, m_Escala);
	DDV_MinMaxInt(pDX, m_Escala, 0, 99999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgEscala, CDialog)
	//{{AFX_MSG_MAP(DlgEscala)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgEscala message handlers
/////////////////////////////////////////////////////////////////////////////
// DlgCargaInicial dialog


DlgCargaInicial::DlgCargaInicial(CWnd* pParent /*=NULL*/)
	: CDialog(DlgCargaInicial::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgCargaInicial)
	m_Bloque = 0;
	m_DesdeOrden = 0;
	m_HastaOrden = 0;
	//}}AFX_DATA_INIT
}


void DlgCargaInicial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgCargaInicial)
	DDX_Control(pDX, IDC_HASTA_ORDEN, m_EditHastaOrden);
	DDX_Control(pDX, IDC_DESDE_ORDEN, m_EditDesdeOrden);
	DDX_Control(pDX, IDC_BLOQUE, m_ComboBloque);
	DDX_Text(pDX, IDC_BLOQUE, m_Bloque);
	DDV_MinMaxInt(pDX, m_Bloque, 0, 999999);
	DDX_Text(pDX, IDC_DESDE_ORDEN, m_DesdeOrden);
	DDV_MinMaxInt(pDX, m_DesdeOrden, 0, 9999);
	DDX_Text(pDX, IDC_HASTA_ORDEN, m_HastaOrden);
	DDV_MinMaxInt(pDX, m_HastaOrden, 0, 9999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgCargaInicial, CDialog)
	//{{AFX_MSG_MAP(DlgCargaInicial)
	ON_CBN_SELCHANGE(IDC_BLOQUE, OnSelchangeBloque)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgCargaInicial message handlers
/////////////////////////////////////////////////////////////////////////////
// CDlgValidarCambio dialog


CDlgValidarCambio::CDlgValidarCambio(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgValidarCambio::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgValidarCambio)
	m_sCampo = _T("");
	m_sValAnt = _T("");
	m_sValNue = _T("");
	//}}AFX_DATA_INIT
}


void CDlgValidarCambio::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgValidarCambio)
	DDX_Text(pDX, IDC_CAMPO, m_sCampo);
	DDX_Text(pDX, IDC_VALOR_ANTIGUO, m_sValAnt);
	DDX_Text(pDX, IDC_VALOR_NUEVO, m_sValNue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgValidarCambio, CDialog)
	//{{AFX_MSG_MAP(CDlgValidarCambio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgValidarCambio message handlers
/////////////////////////////////////////////////////////////////////////////
// CIntrLong dialog


CIntrLong::CIntrLong(CWnd* pParent /*=NULL*/)
	: CDialog(CIntrLong::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIntrLong)
	m_lValue = 0;
	m_sText = _T("");
	//}}AFX_DATA_INIT

	m_sCaption = _T("");
}


void CIntrLong::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIntrLong)
	DDX_Control(pDX, IDC_LONG_VALUE, m_EditLong);
	DDX_Control(pDX, IDC_CAPTION, m_EditLongValue);
	DDX_Text(pDX, IDC_LONG_VALUE, m_lValue);
	DDX_Text(pDX, IDC_CAPTION, m_sText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIntrLong, CDialog)
	//{{AFX_MSG_MAP(CIntrLong)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIntrLong message handlers

BOOL CIntrLong::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_sCaption);

	m_EditLong.SetFocus();
		
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CIntrDbl dialog


CIntrDbl::CIntrDbl(CWnd* pParent /*=NULL*/)
	: CDialog(CIntrDbl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIntrDbl)
	m_sText = _T("");
	m_dValue = 0.0;
	//}}AFX_DATA_INIT
}


void CIntrDbl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIntrDbl)
	DDX_Control(pDX, IDC_DBL_VALUE, m_EditDblValue);
	DDX_Text(pDX, IDC_CAPTION, m_sText);
	DDX_Text(pDX, IDC_DBL_VALUE, m_dValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIntrDbl, CDialog)
	//{{AFX_MSG_MAP(CIntrDbl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIntrDbl message handlers

BOOL CIntrDbl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_sCaption);

	m_EditDblValue.SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CDlgPrint dialog


CDlgPrint::CDlgPrint(CProg* p_pProg, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPrint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPrint)
	m_FecDesde = 0;
	m_sDesdeGFH = _T("");
	m_sDesdeMaq = _T("");
	m_FecHasta = 0;
	m_sHastaGFH = _T("");
	m_sHastaMaq = _T("");
	m_sTipoListado = _T("");
	//}}AFX_DATA_INIT
	m_iTipoListado = 0;
	m_pProg = p_pProg;
}


void CDlgPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPrint)
	DDX_Control(pDX, IDC_HASTA_FECHA, m_DTctrlHastaFecha);
	DDX_Control(pDX, IDC_DESDE_FECHA, m_DTctrlDesdeFecha);
	DDX_Control(pDX, IDC_TIPO_LISTADO, m_ComboTipoListado);
	DDX_Control(pDX, IDC_HASTA_MAQ, m_ComboHastaMaq);
	DDX_Control(pDX, IDC_HASTA_GFH, m_ComboHastaGFH);
	DDX_Control(pDX, IDC_DESDE_MAQ, m_ComboDesdeMaq);
	DDX_Control(pDX, IDC_DESDE_GFH, m_ComboDesdeGFH);
	DDX_DateTimeCtrl(pDX, IDC_DESDE_FECHA, m_FecDesde);
	DDX_CBString(pDX, IDC_DESDE_GFH, m_sDesdeGFH);
	DDX_CBString(pDX, IDC_DESDE_MAQ, m_sDesdeMaq);
	DDX_DateTimeCtrl(pDX, IDC_HASTA_FECHA, m_FecHasta);
	DDX_CBString(pDX, IDC_HASTA_GFH, m_sHastaGFH);
	DDX_CBString(pDX, IDC_HASTA_MAQ, m_sHastaMaq);
	DDX_CBString(pDX, IDC_TIPO_LISTADO, m_sTipoListado);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPrint, CDialog)
	//{{AFX_MSG_MAP(CDlgPrint)
	ON_CBN_SELCHANGE(IDC_TIPO_LISTADO, OnSelchangeTipoListado)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPrint message handlers

BOOL CDlgPrint::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CStringArray ArrMaquinas;
	CStringArray ArrGFHs;

	if (m_pProg->GetArrMaquinas(ArrMaquinas))
	{                                
		for (int i=0; i< ArrMaquinas.GetSize(); i++)
		{                                  
			m_ComboDesdeMaq.AddString(FormatLong(atol(ArrMaquinas[i]),3));
			m_ComboHastaMaq.AddString(FormatLong(atol(ArrMaquinas[i]),3));
		}
	}

	if (m_pProg->GetArrGFHs(ArrGFHs))
	{                                
		for (int i=0; i< ArrGFHs.GetSize(); i++)
		{                                  
			m_ComboDesdeGFH.AddString(ArrGFHs[i]);
			m_ComboHastaGFH.AddString(ArrGFHs[i]);
		}
	}

	// Ponemos los valores almacenados en el registry o bien valores por defecto
	CString sTemp;
	int iSel;
	CWinApp* pApp = AfxGetApp();
   
	iSel = pApp->GetProfileInt("Dialogo de Impresion", "TipoListado", 0);
	m_ComboTipoListado.SetCurSel(iSel);

	if ( iSel == 0)
	{
		m_ComboDesdeMaq.EnableWindow();
		m_ComboHastaMaq.EnableWindow();
	} else
	{
		m_ComboDesdeGFH.EnableWindow(FALSE);
		m_ComboHastaGFH.EnableWindow(FALSE);
	}

		

	
	sTemp = pApp->GetProfileString("Dialogo de Impresion", "DesdeGFH", "");
	m_ComboDesdeGFH.SetWindowText(sTemp);

	sTemp = pApp->GetProfileString("Dialogo de Impresion", "HastaGFH", "");
	m_ComboHastaGFH.SetWindowText(sTemp);

	sTemp = pApp->GetProfileString("Dialogo de Impresion", "DesdeMaq", "");
	m_ComboDesdeMaq.SetWindowText(sTemp);

	sTemp = pApp->GetProfileString("Dialogo de Impresion", "HastaMaq", "");
	m_ComboHastaMaq.SetWindowText(sTemp);

	CTime FecTemp = CTime::GetCurrentTime();
	FecTemp = CTime( pApp->GetProfileInt("Dialogo de Impresion", "DesdeFecYY", FecTemp.GetYear()),
					 pApp->GetProfileInt("Dialogo de Impresion", "DesdeFecMM", FecTemp.GetMonth()),
					 pApp->GetProfileInt("Dialogo de Impresion", "DesdeFecDD", FecTemp.GetDay()) , 0, 0, 0);

	m_DTctrlDesdeFecha.SetTime(&FecTemp);
	
	FecTemp = CTime::GetCurrentTime();
	FecTemp = CTime( pApp->GetProfileInt("Dialogo de Impresion", "HastaFecYY", FecTemp.GetYear()),
					 pApp->GetProfileInt("Dialogo de Impresion", "HastaFecMM", FecTemp.GetMonth()),
					 pApp->GetProfileInt("Dialogo de Impresion", "HastaFecDD", FecTemp.GetDay()) , 0, 0, 0);

	m_DTctrlHastaFecha.SetTime(&FecTemp);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPrint::OnOK() 
{
	m_iTipoListado = m_ComboTipoListado.GetCurSel() + 1;

	// Guardamos los valores en el registry para la próxima vez que se abra el
	// diálogo. Estos valores se resetean al comenzar la aplicación.
	CString sTemp;
	CWinApp* pApp = AfxGetApp();
   
	pApp->WriteProfileInt("Dialogo de Impresion", "TipoListado", m_ComboTipoListado.GetCurSel());
   	
	m_ComboDesdeGFH.GetWindowText(sTemp);
	pApp->WriteProfileString("Dialogo de Impresion", "DesdeGFH", sTemp);
	m_ComboHastaGFH.GetWindowText(sTemp);
	pApp->WriteProfileString("Dialogo de Impresion", "HastaGFH", sTemp);
	m_ComboDesdeMaq.GetWindowText(sTemp);
	pApp->WriteProfileString("Dialogo de Impresion", "DesdeMaq", sTemp);
	m_ComboHastaMaq.GetWindowText(sTemp);
	pApp->WriteProfileString("Dialogo de Impresion", "HastaMaq", sTemp);
	
	CTime FecTemp;
	DWORD dwStatus;
	dwStatus = m_DTctrlDesdeFecha.GetTime(FecTemp);
	pApp->WriteProfileInt("Dialogo de Impresion", "DesdeFecYY", FecTemp.GetYear());
	pApp->WriteProfileInt("Dialogo de Impresion", "DesdeFecMM", FecTemp.GetMonth());
	pApp->WriteProfileInt("Dialogo de Impresion", "DesdeFecDD", FecTemp.GetDay());

	dwStatus = m_DTctrlHastaFecha.GetTime(FecTemp);
	pApp->WriteProfileInt("Dialogo de Impresion", "HastaFecYY", FecTemp.GetYear());
	pApp->WriteProfileInt("Dialogo de Impresion", "HastaFecMM", FecTemp.GetMonth());
	pApp->WriteProfileInt("Dialogo de Impresion", "HastaFecDD", FecTemp.GetDay());

	CDialog::OnOK();
}

void CDlgPrint::OnSelchangeTipoListado() 
{
	if (m_ComboTipoListado.GetCurSel() == 0)
	{
		m_ComboDesdeMaq.EnableWindow();
		m_ComboHastaMaq.EnableWindow();
		m_ComboDesdeGFH.EnableWindow();
		m_ComboHastaGFH.EnableWindow();
	} else 
	{
		m_ComboDesdeGFH.EnableWindow(FALSE);
		m_ComboHastaGFH.EnableWindow(FALSE);
		m_ComboDesdeMaq.EnableWindow();
		m_ComboHastaMaq.EnableWindow();
	}
}

BOOL DlgCargaInicial::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	CUIntArray ArrBloques;
	
	g_pCache->GetArrBloques(ArrBloques);
	int i = 0;
	for ( ; i < ArrBloques.GetSize(); i++)
	{
		CString sTemp;
		sTemp.Format("%u" , (unsigned) ArrBloques[i]);
		m_ComboBloque.AddString(sTemp);
	}
	if (i > 0) 
	{
		m_ComboBloque.SetCurSel(0);
		OnSelchangeBloque();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgCargaInicial::OnSelchangeBloque() 
{
	int iMinBlq, iMaxBlq, iBloque;
	CString sTemp;

	m_ComboBloque.GetLBText(m_ComboBloque.GetCurSel(), sTemp);
	iBloque = atoi(sTemp);

	g_pCache->GetMaxMinBlq(iBloque, iMinBlq, iMaxBlq);
	sTemp.Format("%d", iMinBlq);
	m_EditDesdeOrden.SetWindowText(sTemp);
	sTemp.Format("%d", iMaxBlq);
	m_EditHastaOrden.SetWindowText(sTemp);
	
}

/////////////////////////////////////////////////////////////////////////////
// CDlgBuscarOrden dialog


CDlgBuscarOrden::CDlgBuscarOrden(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBuscarOrden::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBuscarOrden)
	m_sOrden = _T("");
	//}}AFX_DATA_INIT
}


void CDlgBuscarOrden::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBuscarOrden)
	DDX_Control(pDX, IDC_ORDEN, m_ComboOrden);
	DDX_CBString(pDX, IDC_ORDEN, m_sOrden);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBuscarOrden, CDialog)
	//{{AFX_MSG_MAP(CDlgBuscarOrden)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBuscarOrden message handlers

BOOL CDlgBuscarOrden::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CObArray ArrOFs;

	g_pCache->GetArrOFs( ArrOFs );
	for (int i = 0; i < ArrOFs.GetSize(); i++)
	{
		CBufOF* pOF = (CBufOF*) ArrOFs.GetAt(i);
		ASSERT( pOF->IsKindOf( RUNTIME_CLASS( CBufOF ) ) );		

		CString sOFData;
//		sOFData.Format("%ld", pOF->m_OrdFab);
//		m_ComboOrden.AddString( sOFData );
	}
	m_ComboOrden.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CDlgIntrDate dialog


CDlgIntrDate::CDlgIntrDate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIntrDate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgIntrDate)
	m_sCaption = _T("");
	m_Fec = 0;
	//}}AFX_DATA_INIT
}


void CDlgIntrDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgIntrDate)
	DDX_Control(pDX, IDC_FECHA, m_DTctrlFecha);
	DDX_Text(pDX, IDC_CAPTION, m_sCaption);
	DDX_DateTimeCtrl(pDX, IDC_FECHA, m_Fec);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgIntrDate, CDialog)
	//{{AFX_MSG_MAP(CDlgIntrDate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgIntrDate message handlers

BOOL CDlgIntrDate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_sCaption);

	m_DTctrlFecha.SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CDlgInsMaq dialog


CDlgInsMaq::CDlgInsMaq(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInsMaq::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInsMaq)
	m_sInsMaq = _T("");
	m_iAntes = -1;
	//}}AFX_DATA_INIT
}


void CDlgInsMaq::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInsMaq)
	DDX_Control(pDX, IDC_INSMAQ_ANTES, m_ButtonAntes);
	DDX_Control(pDX, IDC_INSMAQ_MAQ, m_ComboInsMaq);
	DDX_CBString(pDX, IDC_INSMAQ_MAQ, m_sInsMaq);
	DDX_Radio(pDX, IDC_INSMAQ_ANTES, m_iAntes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInsMaq, CDialog)
	//{{AFX_MSG_MAP(CDlgInsMaq)
	ON_CBN_EDITCHANGE(IDC_INSMAQ_MAQ, OnEditchangeInsmaqMaq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInsMaq message handlers

void CDlgInsMaq::OnEditchangeInsmaqMaq() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CDlgInsMaq::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CStringArray ArrMaquinas;

	if (g_pCache->GetArrMaquinas(ArrMaquinas), TK_GFH_INYECCION)
	{                                
		for (int i=0; i< ArrMaquinas.GetSize(); i++)
		{                                  
			m_ComboInsMaq.AddString(ArrMaquinas[i]);
		}
	}

	m_ButtonAntes.SetCheck(1);
	m_ComboInsMaq.SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CDlgMaqOrd dialog


CDlgMaqOrd::CDlgMaqOrd(CCpwDoc* p_pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMaqOrd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMaqOrd)
	m_FecDesde = 0;
	m_FecHasta = 0;
	m_sDesdeMaq = _T("");
	m_sHastaMaq = _T("");
	//}}AFX_DATA_INIT
	m_pDoc = p_pDoc;
}


void CDlgMaqOrd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMaqOrd)
	DDX_Control(pDX, IDC_HASTA_MAQ, m_ComboHastaMaq);
	DDX_Control(pDX, IDC_DESDE_MAQ, m_ComboDesdeMaq);
	DDX_Control(pDX, IDC_FEC_HASTA, m_DTctrlFecHasta);
	DDX_Control(pDX, IDC_FEC_DESDE, m_DTctrlDesde);
	DDX_Control(pDX, IDC_LSC_MAQORD, m_ListCtrl);
	DDX_DateTimeCtrl(pDX, IDC_FEC_DESDE, m_FecDesde);
	DDX_DateTimeCtrl(pDX, IDC_FEC_HASTA, m_FecHasta);
	DDX_CBString(pDX, IDC_DESDE_MAQ, m_sDesdeMaq);
	DDX_CBString(pDX, IDC_HASTA_MAQ, m_sHastaMaq);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMaqOrd, CDialog)
	//{{AFX_MSG_MAP(CDlgMaqOrd)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FEC_DESDE, OnDatetimechangeFecDesde)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FEC_HASTA, OnDatetimechangeFecHasta)
	ON_BN_CLICKED(IDC_PRINT_MAQORD, OnPrintMaqord)
	ON_CBN_EDITCHANGE(IDC_DESDE_MAQ, OnEditchangeDesdeMaq)
	ON_CBN_EDITCHANGE(IDC_HASTA_MAQ, OnEditchangeHastaMaq)
	ON_CBN_SELCHANGE(IDC_HASTA_MAQ, OnSelchangeHastaMaq)
	ON_CBN_SELCHANGE(IDC_DESDE_MAQ, OnSelchangeDesdeMaq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDlgMaqOrd::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	CStringArray ArrMaquinas;
	CStringArray ArrGFHs;

	if (g_pCache->GetArrMaquinas(ArrMaquinas))
	{                                
		for (int i=0; i< ArrMaquinas.GetSize(); i++)
		{                                  
			m_ComboDesdeMaq.AddString(ArrMaquinas[i]);
			m_ComboHastaMaq.AddString(ArrMaquinas[i]);
		}
	}
	
	if (m_eTipoCons != ConsMezExt) 
	{
		m_ComboDesdeMaq.EnableWindow(FALSE);
		m_ComboHastaMaq.EnableWindow(FALSE);

	}



	UpdateList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMaqOrd::IniMaqOrd()
{
	
	LV_COLUMN lvc;
	LV_ITEM lvi;
	const int iNumCol = 6;
	char* ArrsCols[] = { "Máq.", "Oferta", "Mezcla", "Piezas", "Horas", "P/H" };
	int ArriWidths[] = { 30, 100, 100, 60, 40, 40 };
	int ArriFmts[] = { LVCFMT_RIGHT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT };
	int iLim = m_ArrOM.GetSize();
	
	CTime FecDesde, FecHasta;
	CString sMaqIni, sMaqFin;
	m_DTctrlDesde.GetTime(FecDesde);
	m_DTctrlFecHasta.GetTime(FecHasta);
	
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	for(int i = 0; i < iNumCol; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = ArrsCols[i];
		lvc.cx = ArriWidths[i];
		lvc.fmt = ArriFmts[i];
		m_ListCtrl.InsertColumn(i,&lvc);
	}
	
	
	int iLin = 0;
	CString sMaqAnt = "";
	for (int i=0; i < iLim; i++)
	{
		const int TK_SIZE_MAQ = 10;
		char sMaq[TK_SIZE_MAQ+1];
		COrdMaq* pOM = (COrdMaq*) m_ArrOM.GetAt(i);
			
		COrdMaq* pOMNext = NULL;
		BOOL bRepMaq = FALSE;
		
		if (pOM->GetFecFin() < FecDesde || pOM->GetFecIni() > FecHasta) continue;
		if (sMaqAnt == pOM->GetsMaquina())
		{
			bRepMaq = TRUE;
		}
		sMaqAnt = pOM->GetsMaquina();
		if (bRepMaq)
			strncpy(sMaq,"", TK_SIZE_MAQ);
		else
			strncpy(sMaq,pOM->GetsMaquina(), TK_SIZE_MAQ);
		lvi.mask = LVIF_TEXT;
		lvi.iItem = iLin;
		lvi.iSubItem = 0;
		lvi.pszText =  sMaq;
		lvi.iImage = 0;
		
		m_ListCtrl.InsertItem(&lvi);
		
		m_ListCtrl.SetItemText(iLin,1,pOM->GetsID());
		m_ListCtrl.SetItemText(iLin,2,pOM->GetsMezcla());
		m_ListCtrl.SetItemText(iLin,3,FormatLong(pOM->GetlCantidad(),7));
		m_ListCtrl.SetItemText(iLin,4,FormatLong(((long) pOM->GetTimeSpan().GetTotalHours()),5));
		m_ListCtrl.SetItemText(iLin,5,FormatLong((long) pOM->GetdCadenciaGFH(),5));
		iLin++;
	}
}

void CDlgMaqOrd::IniOF()
{
	
	LV_COLUMN lvc;
	LV_ITEM lvi;
	const int iNumCol = 9;
	char* ArrsCols[] = { "Oferta", "Fecha","Ayer", "Hoy", "Dif.", "Horas", "Cliente", "Seccion", "Planif."};
	int ArriWidths[] = { 80, 80, 60, 60, 60 , 60, 100, 50, 30};
	int ArriFmts[] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_RIGHT};
	int iLim = m_ArrOF.GetSize();
	
	CTime FecDesde, FecHasta;
	m_DTctrlDesde.GetTime(FecDesde);
	m_DTctrlFecHasta.GetTime(FecHasta);
	
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	for(int i = 0; i < iNumCol; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = ArrsCols[i];
		lvc.cx = ArriWidths[i];
		lvc.fmt = ArriFmts[i];
		m_ListCtrl.InsertColumn(i,&lvc);
	}
	
	CString sIDAnt = "";
	int iLin = 0;
	for (int i=0; i < iLim; i ++)
	{
		const int TK_SIZE_MAQ = 10;
		char sMaq[TK_SIZE_MAQ+1];
		long lCantAnt = -1;
		CString sArtAnt = "";
		CComp* pComp = (CComp*) m_ArrOF.GetAt(i);
			
		//if (pOF->m_FecNec < FecDesde || pOF->m_FecNec > FecHasta) continue;
		// Estas lineas evitan que campos con la misma diferencia salgan en la consulta,
		// es decir que una diferencia no se perpetue en los días siguientes
		long lCant = (long) (pComp->m_dCantHoy - pComp->m_dCantAyer);
		if (pComp->m_sIDArticulo == sArtAnt && lCant == lCantAnt) continue;
		sArtAnt = pComp->m_sIDArticulo;
		lCantAnt = lCant;

		lvi.mask = LVIF_TEXT;
		lvi.iItem = iLin;
		lvi.iSubItem = 0;
		lvi.pszText =  sMaq;
		lvi.iImage = 0;
		
		m_ListCtrl.InsertItem(&lvi);
		if (pComp->m_sIDArticulo == sIDAnt)
		{
			m_ListCtrl.SetItemText(iLin,0, "" );
		} else
		{
			m_ListCtrl.SetItemText(iLin,0, pComp->m_sIDArticulo );
			sIDAnt = pComp->m_sIDArticulo;
		}
		CBufArticulo* pBufArt = g_pCache->FindpArticulo(pComp->m_sIDArticulo);
		m_ListCtrl.SetItemText(iLin,1, FormatFec( pComp->m_FecNec, Fecha) );
		m_ListCtrl.SetItemText(iLin,2, FormatLong((long) pComp->m_dCantAyer,7) );
		m_ListCtrl.SetItemText(iLin,3, FormatLong((long) pComp->m_dCantHoy,7) );
		m_ListCtrl.SetItemText(iLin,4, FormatLong( lCant,7) );
		if (pBufArt)
			m_ListCtrl.SetItemText(iLin,5, FormatLong((long) ((lCant / pBufArt->GetdCadencia()) + 1) ,7) );
		else
			m_ListCtrl.SetItemText(iLin,5, "-");

		m_ListCtrl.SetItemText(iLin,6, pComp->m_sIDCliente);
		m_ListCtrl.SetItemText(iLin,7, FormatLong(pComp->m_lSeccion ,7) );
		CProg* pProg = m_pDoc->GetpProg();
		CString sPlanif = "";
		
		CTimeLine* pTL = pProg->GetpTL(pComp->m_sIDArticulo, Piezas);
		if (pTL) sPlanif = "P";
		m_ListCtrl.SetItemText(iLin,8, sPlanif );
		iLin++;
	}
}


void CDlgMaqOrd::IniMezExt()
{
	
	LV_COLUMN lvc;
	LV_ITEM lvi;
	const int iNumCol = 5;
	char* ArrsCols[] = { "Local", "Oferta","Cliente", "Mezcla","Fecha Ini."};
	int ArriWidths[] = { 80, 80, 180, 80, 100};
	int ArriFmts[] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_LEFT};
	int iLim = m_ArrMezExt.GetSize();
	
	CTime FecDesde, FecHasta;
	m_DTctrlDesde.GetTime(FecDesde);
	m_DTctrlFecHasta.GetTime(FecHasta);
	CString sMaqIni, sMaqFin;
	m_ComboDesdeMaq.GetWindowText(sMaqIni);
	m_ComboHastaMaq.GetWindowText(sMaqFin);
	CProg* pProg = m_pDoc->GetpProg();
	
	iLim = pProg->GetArrMezExt(m_ArrMezExt, sMaqIni, sMaqFin);
	
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	for(int i = 0; i < iNumCol; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = ArrsCols[i];
		lvc.cx = ArriWidths[i];
		lvc.fmt = ArriFmts[i];
		m_ListCtrl.InsertColumn(i,&lvc);
	}
	
	CString sIDAnt = "";
	int iLin = 0;
	for ( int i=0; i < iLim; i ++)
	{
		const int TK_SIZE_MAQ = 10;
		char sMaq[TK_SIZE_MAQ+1];
		CBufLisMezclas* pBufLisMezclas = (CBufLisMezclas*) m_ArrMezExt.GetAt(i);
			
		lvi.mask = LVIF_TEXT;
		lvi.iItem = iLin;
		lvi.iSubItem = 0;
		lvi.pszText =  sMaq;
		lvi.iImage = 0;
		
		m_ListCtrl.InsertItem(&lvi);
		if (pBufLisMezclas->m_sIDLocal == sIDAnt)
		{
			m_ListCtrl.SetItemText(iLin,0, "" );
		} else
		{
			m_ListCtrl.SetItemText(iLin,0, pBufLisMezclas->m_sIDLocal );
			sIDAnt = pBufLisMezclas->m_sIDLocal;
		}
		long lCant = (long) (pBufLisMezclas->m_lMezcla);
		CBufArticulo* pBufArt = g_pCache->FindpArticulo(pBufLisMezclas->m_sIDArticulo);
		m_ListCtrl.SetItemText(iLin,1, pBufLisMezclas->m_sIDArticulo );
		m_ListCtrl.SetItemText(iLin,2, pBufLisMezclas->m_sIDCliente );
		m_ListCtrl.SetItemText(iLin,3, FormatLong(lCant, 7));
		m_ListCtrl.SetItemText(iLin,4, FormatFec( pBufLisMezclas->m_FecIni, FecHora) );
		
		iLin++;
	}
}

void 
CDlgMaqOrd::IniCambioMolde()
{

	LV_COLUMN lvc;
	const int iNumCol = 9;
	static char* ArrsCols[] = { "Máq.", "", "Entrante", "Saliente", "Cliente", "Horas", "Causa", "Mezcla", "Dia" };
	int ArriWidths[] = { 30, 0, 70, 70, 140, 50, 80, 100 ,100};
	int ArriFmts[] = { LVCFMT_RIGHT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_LEFT, LVCFMT_LEFT , LVCFMT_LEFT};
	int iLim = m_ArrOM.GetSize();
	
	
	CTime FecDesde, FecHasta;
	m_DTctrlDesde.GetTime(FecDesde);
	m_DTctrlFecHasta.GetTime(FecHasta);
		
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	CString sMaqAnt = "";
	for(int i = 0; i < iNumCol; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = ArrsCols[i];
		lvc.cx = ArriWidths[i];
		lvc.fmt = ArriFmts[i];
		m_ListCtrl.InsertColumn(i,&lvc);
	}
	
	
	int iLin = 0;
	for ( int i=0; i < iLim; i++)
	{
		const int TK_SIZE_MAQ = 10;
		char sMaq[TK_SIZE_MAQ+1];
		COrdMaq* pOM = (COrdMaq*) m_ArrOM.GetAt(i);
		COrdMaq* pOMNext = NULL;
		COrdMaq* pOMPrev = NULL;
		BOOL bRepMaq = FALSE;
		CString sDebff = FormatFec(pOM->GetFecFin(), FecHoraSec);
		CString sDebin = FormatFec(pOM->GetFecIni(), FecHoraSec);
		CString sDebD = FormatFec(FecDesde, FecHoraSec);
		CString sDebH = FormatFec(FecHasta, FecHoraSec);
		
		
		if ( (i+1) < iLim)
		{
			pOMNext = (COrdMaq*) m_ArrOM.GetAt(i+1);
		} else {
			pOMNext = NULL;
		}
		if ( (i-1) >= 0)
		{
			pOMPrev = (COrdMaq*) m_ArrOM.GetAt(i-1);
		} else {
			pOMPrev = NULL;
		}

		if  ((pOM->GetFecFin() < FecDesde || pOM->GetFecIni() > FecHasta))  continue; // Si no estamos entre las fechas no salimos
		if  ((pOM->GetFecFin() < FecDesde || pOM->GetFecFin() > FecHasta ) && ! EsPrimeraConHueco( pOM, pOMPrev)) continue;	// quitamos también
																	// las que no entran por fecha de fin, salvo aquellas que son primeras y tienen un hueco

		CString sMolde = pOM->GetsMolde();
		CString sMoldeNext = (pOMNext) ? pOMNext->GetsMolde() : "";

		bool bUltimo = false;
		if (!pOMNext || (pOMNext->GetpPM() != pOM->GetpPM())) bUltimo = true;		// Si están en máquinas distintas
		bool bHayHueco = false;
		if (!pOMNext || pOM->GetFecFin() < pOMNext->GetFecIni()) bHayHueco = true;	// Si hay un hueco con la siguiente
		if (sMaqAnt == pOM->GetsMaquina()) bRepMaq = TRUE;							// Si estamos en la misma máquina
		
		if (pOM->GetsMaquina() == "32")
			sMaqAnt = sMaqAnt;

		if (EsPrimeraConHueco( pOM, pOMPrev) )
		{   
			char sMaq2[TK_SIZE_MAQ+1];
			if (bRepMaq)
				strncpy(sMaq2,"", TK_SIZE_MAQ);
			else 
				strncpy(sMaq2, pOM->GetsMaquina(), TK_SIZE_MAQ); 
			AddItemMoldes(NULL, pOM, iLin, sMaq2); // Hacemos una linea extra
			sMaqAnt = pOM->GetsMaquina();
			if (sMaqAnt == pOM->GetsMaquina()) bRepMaq = TRUE;							// Si estamos en la misma máquina
		
		} 
		
		// A partir de ahora 16/4/14 sacaremos por cada referencia
		//if ( sMolde == sMoldeNext ) continue;				// o tienen el mismo molde
		if (!bUltimo && pOMNext && (pOM->GetsID() == pOMNext->GetsID()) // Si tiene el mismo artículo 
			&& ! bHayHueco												// Y NO HAY UN HUECO ENTRE ELLAS
			) continue;														// No sacamos cambio de molde
		
		
		sMaqAnt = pOM->GetsMaquina();
		if (bRepMaq)
			strncpy(sMaq,"", TK_SIZE_MAQ);
		else 
			strncpy(sMaq,pOM->GetsMaquina(), TK_SIZE_MAQ); 

		if (bUltimo || bHayHueco) pOMNext = NULL;
		
		AddItemMoldes(pOM, pOMNext, iLin, sMaq);
		
	
	}
}

bool
CDlgMaqOrd::EsPrimeraConHueco(COrdMaq* pOM, COrdMaq* pOMPrev)
{
	bool bPrimMaq = (!pOMPrev || pOMPrev->GetpPM() != pOM->GetpPM()); // flag para saber si somos el primero de la máquina
	
	if (bPrimMaq) {
		if (pOM->GetFecIni() > pOM->GetpProg()->GetFecIni()) return true;
	} else {
		if 	(pOM->GetFecIni() > pOMPrev->GetFecFin()) return true;
	}

	return false;
	
}
void
CDlgMaqOrd::AddItemMoldes(COrdMaq* pOM, COrdMaq* pOMNext, int& iLin, char* sMaq)
{
	LV_ITEM lvi;
		CString sCliente = (pOMNext) ? pOMNext->GetsCliente() : "";


		lvi.mask = LVIF_TEXT;
		lvi.iItem = iLin;
		lvi.iSubItem = 0;
		lvi.pszText =  sMaq;
		lvi.iImage = 0;
		
		m_ListCtrl.InsertItem(&lvi);

		CString sRef = pOM ? pOM->GetsID() : "";
		CString sRefNext = (pOMNext) ? pOMNext->GetsID() : "";
		m_ListCtrl.SetItemText(iLin,1,"");
		m_ListCtrl.SetItemText(iLin,2,sRef);
		//m_ListCtrl.SetItemText(iLin,3,sMoldeNext);
		m_ListCtrl.SetItemText(iLin,3,sRefNext);
		m_ListCtrl.SetItemText(iLin,4,sCliente);
		m_ListCtrl.SetItemText(iLin,5,pOM ? FormatFec(pOM->GetFecFin(),Hora) : FormatFec(pOMNext->GetFecIni(),Hora));
		if (pOM && pOM->GetlCavidades() > 0)
		{ 
			m_ListCtrl.SetItemText(iLin,6,FormatLong(pOM->GetlCantidad() / pOM->GetlCavidades(),6));
		}
		else
		{
			m_ListCtrl.SetItemText(iLin,6,"");
		}
		m_ListCtrl.SetItemText(iLin,7,(pOMNext) ? pOMNext->GetsMezcla(): "");
		m_ListCtrl.SetItemText(iLin,8,pOM ? FormatFec(pOM->GetFecFin(),Fecha) : FormatFec(pOMNext->GetFecIni(),Fecha));

		iLin++;
		
}

void CDlgMaqOrd::OnDatetimechangeFecDesde(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateList();

	*pResult = 0;
}

void CDlgMaqOrd::OnDatetimechangeFecHasta(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateList();
	
	*pResult = 0;
}

void CDlgMaqOrd::OnEditchangeDesdeMaq() 
{
	UpdateList();
}

void CDlgMaqOrd::OnEditchangeHastaMaq() 
{
	UpdateList();
}

void 
CDlgMaqOrd::UpdateList()
{
	m_ListCtrl.DeleteAllItems();
	while (m_ListCtrl.DeleteColumn(0)) ;
	if (m_eTipoCons == ConsMaqOrd)
	{
		SetWindowText("Consulta Máquinas / Órdenes");
		IniMaqOrd();
	}
	else if (m_eTipoCons == ConsCambioMolde) 
	{
		SetWindowText("Consulta Cambios Moldes");
		IniCambioMolde();
	}
	else if (m_eTipoCons == ConsLotes) 
	{
		SetWindowText("Consulta de Lotes");
		IniLotes();
	}
	else if (m_eTipoCons == ConsOF) 
	{
		SetWindowText("Consulta de Diferencias Ayer / Hoy");
		IniOF();
	}
	else if (m_eTipoCons == ConsMezExt) 
	{
		SetWindowText("Consulta de Necesidades de Mezclas Externas");
		IniMezExt();
	}
}

void CDlgMaqOrd::IniLotes()
{
	LV_COLUMN lvc;
	LV_ITEM lvi;
	const int iNumCol = 6;
	static char* ArrsCols[] = { "Máq.", "Oferta", "Mezcla", "Piezas", "Peso Nec.", "Lote" };
	int ArriWidths[] = { 30, 100, 100, 60, 40, 40 };
	int ArriFmts[] = { LVCFMT_RIGHT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT };
	int iLim = m_ArrOM.GetSize();
	
	CTime FecDesde, FecHasta;
	m_DTctrlDesde.GetTime(FecDesde);
	m_DTctrlFecHasta.GetTime(FecHasta);
	
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	for(int i = 0; i < iNumCol; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = ArrsCols[i];
		lvc.cx = ArriWidths[i];
		lvc.fmt = ArriFmts[i];
		m_ListCtrl.InsertColumn(i,&lvc);
	}
	
	
	int iLin = 0;
	CString sMaqAnt = "";
	for ( int i=0; i < iLim; i++)
	{
		const int TK_SIZE_MAQ = 10;
		char sMaq[TK_SIZE_MAQ+1];
		COrdMaq* pOM = (COrdMaq*) m_ArrOM.GetAt(i);
			
		COrdMaq* pOMNext = NULL;
		BOOL bRepMaq = FALSE;
		
		if (pOM->GetFecFin() < FecDesde || pOM->GetFecIni() > FecHasta) continue;
		if (sMaqAnt == pOM->GetsMaquina())
		{
			bRepMaq = TRUE;
		}
		sMaqAnt = pOM->GetsMaquina();
		if (bRepMaq)
			strncpy(sMaq,"", TK_SIZE_MAQ);
		else
			strncpy(sMaq,pOM->GetsMaquina(), TK_SIZE_MAQ);
		lvi.mask = LVIF_TEXT;
		lvi.iItem = iLin;
		lvi.iSubItem = 0;
		lvi.pszText =  sMaq;
		lvi.iImage = 0;
		
		m_ListCtrl.InsertItem(&lvi);
		
		m_ListCtrl.SetItemText(iLin,1,pOM->GetsID());
		m_ListCtrl.SetItemText(iLin,2,pOM->GetsMezcla());
		m_ListCtrl.SetItemText(iLin,3,FormatLong(pOM->GetlCantidad(),7));
		m_ListCtrl.SetItemText(iLin,4,FormatLong((long) ((double)pOM->GetlCantidad()*pOM->GetdPeso()),7));
		m_ListCtrl.SetItemText(iLin,5,FormatLong((long) pOM->GetlLote(),5));
		iLin++;
	}
}

void CDlgMaqOrd::OnPrintMaqord() 
{
	CWaitCursor WaitCursor;
	if (m_eTipoCons == ConsMezExt) 
	{
		// Cargamos la base de datos con los datos del listado
		g_pCache->CargaLisMezclas(&m_ArrMezExt);
		
		// Y ejecutamos Access con la macro adecuada
		LisAccess("Mezclas Externas", "LisMezclas");
	}
	else if (m_eTipoCons == ConsCambioMolde) 
	{

		if (ChequeaListGeneral()) g_pCache->CargaLisGeneral(&m_ListCtrl);
		LisAccess("Cambios de Molde", "LisCambioMoldes");
	} 
	else if (m_eTipoCons == ConsLotes) 
	{
		if (ChequeaListGeneral()) g_pCache->CargaLisGeneral(&m_ListCtrl);
		LisAccess("Lotes", "LisLotes");
	} 
	else if (m_eTipoCons == ConsOF) 
	{
		if (ChequeaListGeneral()) g_pCache->CargaLisGeneral(&m_ListCtrl);
		LisAccess("Diferencias Ayer-Hoy", "LisDifAyerHoy");
	} 
	else	if (m_eTipoCons == ConsMaqOrd)
	{
		CWinApp* wApp = AfxGetApp();
		ASSERT_VALID(wApp);
		wApp->WriteProfileInt("Dialogo de Impresion", "TipoListado", 1);
		wApp->WriteProfileString("Dialogo de Impresion", "DesdeMaq", "1");
		wApp->WriteProfileString("Dialogo de Impresion", "HastaMaq", "1");
		
		POSITION pos = m_pDoc->GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = m_pDoc->GetNextView(pos);
			if ( pView->IsKindOf( RUNTIME_CLASS( CCpwView )))
			{
				CCpwView* pCpwView = (CCpwView*) pView;
				pCpwView->SendMessage(WM_COMMAND, ID_FILE_PRINT);
			}
		}
	}
	
}

bool CDlgMaqOrd::ChequeaListGeneral()
{
	if (AfxMessageBox("¿Generar de nuevo los datos? Al generar de nuevo los datos se perderán los cambios realizados manualmente.", MB_YESNO) == IDYES) {
		return true;
	} else {
		return false;
	}

}
/////////////////////////////////////////////////////////////////////////////
// CDlgIntrStr dialog


CDlgIntrStr::CDlgIntrStr(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIntrStr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgIntrStr)
	m_sCaption = _T("");
	m_sValue = _T("");
	//}}AFX_DATA_INIT
}


void CDlgIntrStr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgIntrStr)
	DDX_Control(pDX, IDC_STR_VALUE, m_EditStr);
	DDX_Text(pDX, IDC_CAPTION, m_sCaption);
	DDX_Text(pDX, IDC_STR_VALUE, m_sValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgIntrStr, CDialog)
	//{{AFX_MSG_MAP(CDlgIntrStr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgIntrStr message handlers

BOOL CDlgIntrStr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	SetWindowText(m_sCaption);

	m_EditStr.SetFocus();
		
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CDlgLista dialog


CDlgLista::CDlgLista(CObArray* p_pArrInfoMolArt, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLista::IDD, pParent)
{
	m_pArrInfoMolArt = p_pArrInfoMolArt;
	m_ArrSort.SetSize(0, m_pArrInfoMolArt->GetSize());
	m_ListCtrl.m_pArrSort = &m_ArrSort;

	//{{AFX_DATA_INIT(CDlgLista)
	m_BNoProg = FALSE;
	//}}AFX_DATA_INIT
	m_pFrame = NULL;
	m_pProg = NULL;
	m_pDoc = NULL;
}


void CDlgLista::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLista)
	DDX_Control(pDX, IDC_LISTA, m_ListCtrl);
	DDX_Check(pDX, IDC_BUT_NOPROG, m_BNoProg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLista, CDialog)
	//{{AFX_MSG_MAP(CDlgLista)
	ON_BN_CLICKED(IDC_BUT_NOPROG, OnButNoprog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLista message handlers

BOOL CDlgLista::OnInitDialog() 
{
	m_ListCtrl.m_pFrame = m_pFrame;
	m_ListCtrl.m_pProg = m_pProg;
	m_ListCtrl.m_pDoc = m_pDoc;
	CDialog::OnInitDialog();
	
	UpdateList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}

void CDlgLista::OnButNoprog() 
{
	UpdateData();
	UpdateList();
}

void
CDlgLista::UpdateList() 
{
	m_ListCtrl.DeleteAllItems();
	while (m_ListCtrl.DeleteColumn(0)) ;

	LV_COLUMN lvc;
	LV_ITEM lvi;
	const int iNumCol = 14;
	char* ArrsCols[] = {  "Margen", "Oferta",  "NoProg", "NoStock", "Prg.", 
							"H.0", "H.7", "H.15", "H.21", "H.30", "H.37", "H.45", "H.52 ","H.60"};
	int ArriWidths[] = { 50, 60, 60, 60, 35, 40, 40, 40, 40, 45, 45, 45, 45, 45};
	int ArriFmts[] = {  LVCFMT_RIGHT, LVCFMT_LEFT,LVCFMT_RIGHT,LVCFMT_RIGHT,LVCFMT_RIGHT,
				LVCFMT_RIGHT ,LVCFMT_RIGHT ,LVCFMT_RIGHT ,			LVCFMT_RIGHT ,LVCFMT_RIGHT ,LVCFMT_RIGHT ,
							LVCFMT_RIGHT ,LVCFMT_RIGHT ,LVCFMT_RIGHT };
	int iLim = m_pArrInfoMolArt->GetSize();
	
	CTime FecDesde, FecHasta;
	
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	for(int i = 0; i < iNumCol; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = ArrsCols[i];
		lvc.cx = ArriWidths[i];
		lvc.fmt = ArriFmts[i];
		m_ListCtrl.InsertColumn(i,&lvc);
	}
	
	int iLin = 0;
	// Ordenamos los valores por horas de retraso y artículo
	m_ArrSort.RemoveAll();
	BOOL bFind;
	for ( int i=0; i < iLim; i++)
	{
		CInfoMolArt* pInfoMolArt = (CInfoMolArt*) m_pArrInfoMolArt->GetAt(i);
		ASSERT( pInfoMolArt->IsKindOf( RUNTIME_CLASS( CInfoMolArt ) ) );
		bFind = FALSE;
		for (int j=0; j < m_ArrSort.GetSize(); j++)
		{
			CInfoMolArt* pSortMolArt = (CInfoMolArt*) m_ArrSort.GetAt(j);
			if (pSortMolArt->m_lHorasRetraso > pInfoMolArt->m_lHorasRetraso || 
				( (pSortMolArt->m_lHorasRetraso == pInfoMolArt->m_lHorasRetraso) &&
				   (pSortMolArt->m_FecProg > pInfoMolArt->m_FecProg ) ) )
			{
				m_ArrSort.InsertAt(j, pInfoMolArt);
				bFind = TRUE;
				break;
			}
		}
		if (!bFind) m_ArrSort.Add(pInfoMolArt);
	}
	for ( int i=0; i < iLim; i++)
	{
		CInfoMolArt* pInfoMolArt = (CInfoMolArt*) m_ArrSort.GetAt(i);
		ASSERT( pInfoMolArt->IsKindOf( RUNTIME_CLASS( CInfoMolArt ) ) );
		
		if (m_pProg->GetpTL(pInfoMolArt->m_sIDArticulo)) 
			pInfoMolArt->m_BProg = TRUE;
		else
			pInfoMolArt->m_BProg = FALSE;


		if (pInfoMolArt->m_BProg && m_BNoProg) continue;

		lvi.mask = LVIF_TEXT;
		lvi.iItem = iLin;
		lvi.iSubItem = 0;
		lvi.pszText =  "";
		lvi.iImage = 0;
		
		m_ListCtrl.InsertItem(&lvi);
		CString sLin;
		if (pInfoMolArt->m_lHorasRetraso == INT_MIN || pInfoMolArt->m_lHorasRetraso == (INT_MAX - 1))
		{
			sLin.Format("NO FAB.");
		} else if (pInfoMolArt->m_lHorasRetraso == INT_MAX)
		{
			sLin.Format("HECHO");
		} else
		{
			sLin = FormatLong(pInfoMolArt->m_lHorasRetraso , 6);
		}
		
		m_ListCtrl.SetItemText(iLin,0, sLin);
		m_ListCtrl.SetItemText(iLin,1,pInfoMolArt->m_sIDArticulo);
		if (pInfoMolArt->m_FecProg > DATE_MIN)
			m_ListCtrl.SetItemText(iLin,2, FormatFec(pInfoMolArt->m_FecProg, Fecha));
		if (pInfoMolArt->m_FecStock > DATE_MIN)
			m_ListCtrl.SetItemText(iLin,3, FormatFec(pInfoMolArt->m_FecStock, Fecha));
		m_ListCtrl.SetItemText(iLin,4,pInfoMolArt->m_BProg ? "x" : " ");
		m_ListCtrl.SetItemText(iLin,5,FormatLong(pInfoMolArt->m_lHoras0,6, Long, TRUE));
		m_ListCtrl.SetItemText(iLin,6,FormatLong(pInfoMolArt->m_lHoras7,6, Long, TRUE));
		m_ListCtrl.SetItemText(iLin,7,FormatLong(pInfoMolArt->m_lHoras15,6, Long, TRUE));
		m_ListCtrl.SetItemText(iLin,8,FormatLong(pInfoMolArt->m_lHoras21,6, Long, TRUE));
		m_ListCtrl.SetItemText(iLin,9,FormatLong(pInfoMolArt->m_lHoras30,6, Long, TRUE));
		m_ListCtrl.SetItemText(iLin,10,FormatLong(pInfoMolArt->m_lHoras37,6, Long, TRUE));
		m_ListCtrl.SetItemText(iLin,11,FormatLong(pInfoMolArt->m_lHoras45,6, Long, TRUE));
		m_ListCtrl.SetItemText(iLin,12,FormatLong(pInfoMolArt->m_lHoras52,6, Long, TRUE));
		m_ListCtrl.SetItemText(iLin,13,FormatLong(pInfoMolArt->m_lHoras60,6, Long, TRUE));
		
		iLin++;
	}


}



/////////////////////////////////////////////////////////////////////////////
// CMyCtrlList

CMyCtrlList::CMyCtrlList()
{
	m_pFrame = NULL;
	m_pProg = NULL;
	m_pDoc = NULL;
	m_sID = "";
}

CMyCtrlList::~CMyCtrlList()
{
}


BEGIN_MESSAGE_MAP(CMyCtrlList, CListCtrl)
	//{{AFX_MSG_MAP(CMyCtrlList)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_CARGAINICIAL, OnCargaInicial)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCtrlList message handlers

void CMyCtrlList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	
	LVHITTESTINFO LvHitTestInfo;
	LvHitTestInfo.pt = point;
	int item = SubItemHitTest(&LvHitTestInfo);
	
	// if an item is actually selected, then process
	if (item != -1)
	{
		// get the item's current selection state based on subitem text
		CString text = GetItemText(item, 1);
	
		if (text != "")
		{
			
			m_pFrame->SearchOF(text);	
		}
	}
	CListCtrl::OnLButtonDblClk(nFlags, point);
}


void 
CMyCtrlList::OnRButtonDown(UINT p_iFlags, CPoint p_Point) 
{
	CMenu PopUp;
	PopUp.CreatePopupMenu();

	CPoint Point = p_Point;
	LVHITTESTINFO LvHitTestInfo;
	LvHitTestInfo.pt = Point;
	int item = SubItemHitTest(&LvHitTestInfo);
	m_sID = "";
	// if an item is actually selected, then process
	if (item != -1)
	{
		// get the item's current selection state based on subitem text
		m_sID = GetItemText(item, 1);
	}
	
	
	UINT tTipo = MF_ENABLED | MF_STRING;
	PopUp.AppendMenu(tTipo, ID_CARGAINICIAL, "Programar");
	
	
	CPoint ScrPoint(p_Point);
	ClientToScreen(&ScrPoint);
	PopUp.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ScrPoint.x, ScrPoint.y, this);
	
	//	CListCtrl::OnRButtonDown(p_iFlags, p_Point);
	
}

void 
CMyCtrlList::OnCargaInicial() 
{
	
	BOOL bModif = FALSE; // Para saber si ha habido alguna modificacion.
	
	if (m_sID == "") return;
	int iInd;
	CBufOF* pBufOF = g_pCache->FindOFArt(m_sID, iInd);
	if (!pBufOF) return;
	if (g_pCache->GetNoHomologada(pBufOF->m_sIDArticulo)) {
		if (AfxMessageBox("La referencia " + pBufOF->m_sIDArticulo + " no está homologada en esta empresa.¿ Programar a pesar de ello?", MB_YESNOCANCEL) != IDYES) {
			return;
		}
	}
	// Si no esta ya hacemos una lista de maquinas a las que puede ir
	CObArray ArrListaMaq, ArrListaMoldes;
	g_pCache->GetArrMaq(pBufOF->m_sIDArticulo, ArrListaMaq, ArrListaMoldes);
	int iLimListaMaq = ArrListaMaq.GetSize();
	if (iLimListaMaq <= 0)
	{
		CString sMsg;
		sMsg.Format(STRID(IDS_ERR_NOMAQMOL),pBufOF->m_sIDArticulo); 
		MessageBox(sMsg, STRID(IDS_ERROR));
		return;
	}
	for(int ix = 0; ix < iLimListaMaq; ix++)
	{
		CBufMaquina* pBufMaquina = (CBufMaquina*) ArrListaMaq.GetAt(ix);
		CBufMolde* pBufMolde = (CBufMolde*) ArrListaMoldes.GetAt(ix);
		ASSERT_VALID(pBufMaquina);
		CProgMaq* pPM = m_pProg->FindMaq(pBufMaquina->m_sID);
		long lCant = 0;
		if (pPM != NULL)
		{
			// Creamos una nueva OM
			COrdMaq* pOM = new COrdMaq(pBufOF, pBufMolde);
			pOM->SetlCantidad(lCant);
			BOOL bModifTemp;
			BOOL bCancelar = m_pProg->InsertNewOM(pOM, bModifTemp);
			if (bModifTemp) bModif = TRUE;
			Invalidate();
			break;
		}
	}
	
	CWaitCursor WaitCursor;
	if (bModif) 
	{
		m_pProg->Recalc();
		m_pDoc->SetModifiedFlag();
		m_pDoc->UpdateAllViews(NULL);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CDlgSelecEmpresa dialog


CDlgSelecEmpresa::CDlgSelecEmpresa(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelecEmpresa::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelecEmpresa)
	m_sEmpresa = _T("");
	//}}AFX_DATA_INIT
	m_psArrEmpresas = NULL;
}


void CDlgSelecEmpresa::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelecEmpresa)
	DDX_Control(pDX, IDC_EMPRESA, m_ComboEmpresa);
	DDX_CBString(pDX, IDC_EMPRESA, m_sEmpresa);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelecEmpresa, CDialog)
	//{{AFX_MSG_MAP(CDlgSelecEmpresa)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelecEmpresa message handlers

BOOL CDlgSelecEmpresa::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_psArrEmpresas) {
		for (int i = 0; i < m_psArrEmpresas->GetSize(); i++)
		{
			m_ComboEmpresa.AddString( m_psArrEmpresas->GetAt(i) );
		}
	}
	m_ComboEmpresa.SetCurSel(0);
	m_ComboEmpresa.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgMaqOrd::OnSelchangeHastaMaq() 
{
	UpdateList();	
}

void CDlgMaqOrd::OnSelchangeDesdeMaq() 
{
	UpdateList();	
}
/////////////////////////////////////////////////////////////////////////////
// CDlgExcel dialog


CDlgExcel::CDlgExcel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExcel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExcel)
	m_FecDesde = 0;
	m_sDesdeMaq = _T("");
	m_FecHasta = 0;
	m_sHastaMaq = _T("");
	//}}AFX_DATA_INIT
}


void CDlgExcel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExcel)
	DDX_Control(pDX, IDC_HASTA_MAQ, m_ComboHastaMaq);
	DDX_Control(pDX, IDC_DESDE_MAQ, m_ComboDesdeMaq);
	DDX_Control(pDX, IDC_HASTA_FECHA, m_DTctrlHastaFecha);
	DDX_Control(pDX, IDC_DESDE_FECHA, m_DTctrlDesdeFecha);
	DDX_DateTimeCtrl(pDX, IDC_DESDE_FECHA, m_FecDesde);
	DDX_CBString(pDX, IDC_DESDE_MAQ, m_sDesdeMaq);
	DDX_DateTimeCtrl(pDX, IDC_HASTA_FECHA, m_FecHasta);
	DDX_CBString(pDX, IDC_HASTA_MAQ, m_sHastaMaq);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExcel, CDialog)
	//{{AFX_MSG_MAP(CDlgExcel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExcel message handlers

BOOL CDlgExcel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CStringArray ArrMaquinas;

	if (g_pCache->GetArrMaquinas(ArrMaquinas))
	{                                
		for (int i=0; i< ArrMaquinas.GetSize(); i++)
		{                                  
			m_ComboDesdeMaq.AddString(FormatLong(atol(ArrMaquinas[i]),3));
			m_ComboHastaMaq.AddString(FormatLong(atol(ArrMaquinas[i]),3));
		}
	}


	// Ponemos los valores almacenados en el registry o bien valores por defecto
	CString sTemp;
	CWinApp* pApp = AfxGetApp();
   
	
	sTemp = pApp->GetProfileString("Dialogo de Excel", "DesdeMaq", "");
	m_ComboDesdeMaq.SetWindowText(sTemp);

	sTemp = pApp->GetProfileString("Dialogo de Excel", "HastaMaq", "");
	m_ComboHastaMaq.SetWindowText(sTemp);

	CTime FecTemp = CTime::GetCurrentTime();
	FecTemp = CTime( pApp->GetProfileInt("Dialogo de Excel", "DesdeFecYY", FecTemp.GetYear()),
					 pApp->GetProfileInt("Dialogo de Excel", "DesdeFecMM", FecTemp.GetMonth()),
					 pApp->GetProfileInt("Dialogo de Excel", "DesdeFecDD", FecTemp.GetDay()) , 0, 0, 0);

	m_DTctrlDesdeFecha.SetTime(&FecTemp);
	
	FecTemp = CTime::GetCurrentTime();
	FecTemp = CTime( pApp->GetProfileInt("Dialogo de Excel", "HastaFecYY", FecTemp.GetYear()),
					 pApp->GetProfileInt("Dialogo de Excel", "HastaFecMM", FecTemp.GetMonth()),
					 pApp->GetProfileInt("Dialogo de Excel", "HastaFecDD", FecTemp.GetDay()) , 0, 0, 0);

	m_DTctrlHastaFecha.SetTime(&FecTemp);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgExcel::OnOK() 
{

	// Guardamos los valores en el registry para la próxima vez que se abra el
	// diálogo. Estos valores se resetean al comenzar la aplicación.
	CString sTemp;
	CWinApp* pApp = AfxGetApp();
   
	m_ComboDesdeMaq.GetWindowText(sTemp);
	pApp->WriteProfileString("Dialogo de Excel", "DesdeMaq", sTemp);
	m_ComboHastaMaq.GetWindowText(sTemp);
	pApp->WriteProfileString("Dialogo de Excel", "HastaMaq", sTemp);
	
	CTime FecTemp;
	DWORD dwStatus;
	dwStatus = m_DTctrlDesdeFecha.GetTime(FecTemp);
	pApp->WriteProfileInt("Dialogo de Excel", "DesdeFecYY", FecTemp.GetYear());
	pApp->WriteProfileInt("Dialogo de Excel", "DesdeFecMM", FecTemp.GetMonth());
	pApp->WriteProfileInt("Dialogo de Excel", "DesdeFecDD", FecTemp.GetDay());

	dwStatus = m_DTctrlHastaFecha.GetTime(FecTemp);
	pApp->WriteProfileInt("Dialogo de Excel", "HastaFecYY", FecTemp.GetYear());
	pApp->WriteProfileInt("Dialogo de Excel", "HastaFecMM", FecTemp.GetMonth());
	pApp->WriteProfileInt("Dialogo de Excel", "HastaFecDD", FecTemp.GetDay());
	
	CDialog::OnOK();
}
