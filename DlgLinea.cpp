// DlgLinea.cpp : implementation file
//

#include "stdafx.h"
#include "afxcview.h"
#include "afxdtctl.h"
#include "cpw.h"
#include "cpwDefs.h"

#include "AbSpan.h"
#include "cpwExt.h"
#include "prog.h"

#include "cpwDoc.h"

#include "ChildFrm.h"
#include "cpwView.h"

#include "cpwDb.h"
#include "cache.h"            
#include "ProgMaq.h"
#include "OrdMaq.h"
#include "Dialogs.h" 
#include "OrdenDlg.h"  
#include "ListViewEx.h"
#include "InsView.h"
#include "DlgAccel.h"
#include "DlgLinea.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLinea dialog


CDlgLinea::CDlgLinea(CWnd* pParent)
	: CDialog(CDlgLinea::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLinea)
	m_sLinArt = _T("");
	m_sLinCad = _T("");
	m_sLinCant = _T("");
	m_sLinHoras = _T("");
	m_sLinMaq = _T("");
	m_sLinMezcla = _T("");
	m_sLinMolde = _T("");
	//}}AFX_DATA_INIT
	m_pParent = (CInsView*) pParent;
	m_bClose = FALSE;
	ASSERT_VALID(m_pParent);
	ASSERT(m_pParent->IsKindOf(RUNTIME_CLASS(CInsView)));

	
}


void CDlgLinea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLinea)
	DDX_Control(pDX, IDC_LIN_MOLDE, m_ComboLinMolde);
	DDX_Control(pDX, IDC_LIN_MEZCLA, m_EditLinMezcla);
	DDX_Control(pDX, IDC_LIN_MAQ, m_EditLinMaq);
	DDX_Control(pDX, IDC_LIN_HORAS, m_EditLinHoras);
	DDX_Control(pDX, IDC_LIN_CANT, m_EditLinCant);
	DDX_Control(pDX, IDC_LIN_CAD, m_EditLinCad);
	DDX_Control(pDX, IDC_LIN_ART, m_EditLinArt);
	DDX_Text(pDX, IDC_LIN_ART, m_sLinArt);
	DDX_Text(pDX, IDC_LIN_CAD, m_sLinCad);
	DDX_Text(pDX, IDC_LIN_CANT, m_sLinCant);
	DDX_Text(pDX, IDC_LIN_HORAS, m_sLinHoras);
	DDX_Text(pDX, IDC_LIN_MAQ, m_sLinMaq);
	DDX_Text(pDX, IDC_LIN_MEZCLA, m_sLinMezcla);
	DDX_Text(pDX, IDC_LIN_MOLDE, m_sLinMolde);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLinea, CDialog)
	//{{AFX_MSG_MAP(CDlgLinea)
	ON_EN_CHANGE(IDC_LIN_ART, OnChangeLinArt)
	ON_EN_CHANGE(IDC_LIN_CANT, OnChangeLinCant)
	ON_EN_CHANGE(IDC_LIN_CAD, OnChangeLinCad)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_MY_ENTER, OnMyEnter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLinea message handlers

BOOL CDlgLinea::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Load dialog's accelerators
	m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(),
		m_lpszTemplateName); // use same resource name as dialog


	CRect WinRect;
	GetWindowRect(&WinRect);
	
	
	CListCtrl& ListCtrl = m_pParent->GetListCtrl();
	
	LV_COLUMN lvc;
	lvc.mask = LVCF_WIDTH;

	CObArray ArrListaMaq;
	CObArray ArrListaMoldes;
	
	int iX = 0;
	ListCtrl.GetColumn(0, &lvc);
	m_EditLinMaq.SetWindowPos(NULL, iX, 0, lvc.cx, WinRect.Height(), SWP_NOZORDER);
	
	if (m_sLinMaq.GetLength() > 0) 
	{
		m_EditLinMaq.SetReadOnly(TRUE);
		m_EditLinArt.SetFocus();
	}
		
	else 
	{
		m_EditLinMaq.SetReadOnly(FALSE);
		m_EditLinMaq.SetFocus();
	}
	iX += lvc.cx;
	ListCtrl.GetColumn(1, &lvc);
	m_EditLinArt.SetWindowPos(NULL, iX, 0, lvc.cx, WinRect.Height(), SWP_NOZORDER);
	iX += lvc.cx;
	ListCtrl.GetColumn(2, &lvc);
	m_ComboLinMolde.SetWindowPos(NULL, iX, 0, lvc.cx, WinRect.Height(), SWP_NOZORDER);
	g_pCache->GetArrMaq(m_sLinArt, ArrListaMaq, ArrListaMoldes);
	CString sMolAnt = "";
	for(int i=0; i<ArrListaMoldes.GetSize();i++)
	{
		CBufMolde* pBufMolde = (CBufMolde*) ArrListaMoldes.GetAt(i);
		if (sMolAnt != pBufMolde->m_sID ) m_ComboLinMolde.AddString(pBufMolde->m_sID);
	}
	m_ComboLinMolde.SetWindowText(m_sLinMolde);
	iX += lvc.cx;
	ListCtrl.GetColumn(3, &lvc);
	m_EditLinMezcla.SetWindowPos(NULL, iX, 0, lvc.cx, WinRect.Height(), SWP_NOZORDER);
	iX += lvc.cx;
	ListCtrl.GetColumn(4, &lvc);
	m_EditLinCant.SetWindowPos(NULL, iX, 0, lvc.cx, WinRect.Height(), SWP_NOZORDER);
	iX += lvc.cx;
	ListCtrl.GetColumn(5, &lvc);
	m_EditLinCad.SetWindowPos(NULL, iX, 0, lvc.cx, WinRect.Height(), SWP_NOZORDER);
	iX += lvc.cx;
	ListCtrl.GetColumn(6, &lvc);
	m_EditLinHoras.SetWindowPos(NULL, iX, 0, lvc.cx, WinRect.Height(), SWP_NOZORDER);
	
	if (m_Point.x != -1) SetWindowPos(NULL, m_Point.x, m_Point.y, iX+lvc.cx+10, WinRect.Height(), SWP_NOZORDER);	
	
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



BOOL CDlgLinea::Validate()
{

	if (g_pCache->FindpMaquina(m_sLinMaq) == NULL)
	{
		CString sMsg = "No existe la maquina " + m_sLinMaq;
		m_sLinMaq = "";
		MessageBox(sMsg, "Error de Validación");
		return FALSE;
	}
	if (g_pCache->FindpArticulo(m_sLinArt) == NULL)
	{
		CString sMsg = "No existe el Artículo " + m_sLinArt;
		MessageBox(sMsg, "Error de Validación");
		return FALSE;
	}
	if (m_sLinArt != "" && g_pCache->FindpMolde(m_sLinMolde) == NULL)
	{
		CString sMsg = "No existe el Molde " + m_sLinMolde;
		MessageBox(sMsg, "Error de Validación");
		return FALSE;
	}
	return TRUE;
}

void CDlgLinea::OnChangeLinArt() 
{
	CString sArt = GetsDlgText(IDC_LIN_ART, this);
	
	CBufArticulo* pBufArticulo = g_pCache->FindpArticulo(sArt);
	if (pBufArticulo && pBufArticulo->m_pBufMezcla)
	{
		m_EditLinMezcla.SetWindowText(pBufArticulo->m_pBufMezcla->m_sID);
	}
	else
	{
		m_EditLinMezcla.SetWindowText("");
	}
	
	CObArray ArrListaMaq;
	CObArray ArrListaMoldes;
	g_pCache->GetArrMaq(sArt, ArrListaMaq, ArrListaMoldes);
	
	m_ComboLinMolde.ResetContent();
	CString sMolAnt = "";
	m_ComboLinMolde.SetWindowText("");
	for(int i=0; i<ArrListaMoldes.GetSize();i++)
	{
		CBufMolde* pBufMolde = (CBufMolde*) ArrListaMoldes.GetAt(i);
		if (sMolAnt == "") m_ComboLinMolde.SetWindowText(pBufMolde->m_sID);
		if (sMolAnt != pBufMolde->m_sID ) m_ComboLinMolde.AddString(pBufMolde->m_sID);
	}
	if (pBufArticulo)
	{
		double dCadencia = pBufArticulo->GetdCadencia();
		m_EditLinCad.SetWindowText(FormatLong((long)dCadencia,6));
	}		
	
}

void CDlgLinea::OnChangeLinCant() 
{
	CalcTime();
}

void CDlgLinea::CalcTime() 
{
	CString sCant = GetsDlgText(IDC_LIN_CANT, this);
	CString sCad = GetsDlgText(IDC_LIN_CAD, this);
	
	long lCant = atol(sCant);
	long lCad = atol(sCad);
	
	if (lCant > 0 && lCad > 0)
	{
		CTimeSpan ts = (CTimeSpan) (long) ( (double) lCant * TK_SEGS_HORA / (double) lCad );
		
		// Y ahora redondeamos a enteros en las horas, para arriba
		double dHoras = (double) ((ts.GetTotalSeconds() * (1 + TK_INACT_DEF / 100) )/ TK_SEGS_HORA);
		double dHorasNd = ((long) dHoras );
		if (dHorasNd != dHoras) dHorasNd++;
		
		long lTime = (long) (dHorasNd);
		
		CString sTime;
		sTime.Format("%ld",lTime);
		m_EditLinHoras.SetWindowText(sTime);
	}	
}

void CDlgLinea::OnChangeLinCad() 
{
	CalcTime();	
}

// Funciones para que funcionen los accelerators


BOOL CDlgLinea::PreTranslateMessage(MSG* pMsg) 
{
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST && GetNextDlgTabItem(GetFocus()) != GetDlgItem(IDC_LIN_MAQ)) {
		HACCEL hAccel = m_hAccel;
		if (hAccel && ::TranslateAccelerator(m_hWnd, hAccel, pMsg))
			return TRUE;
	}	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgLinea::OnMyEnter()
{
	TRACE(_T("CDlgLinea::OnMyEnter\n"));
	NextInTabOrder(); // move to next control
}

//////////////////
// Helper function to move focus to the next control.
// GetNextDlgTabItem finds the control.
//
void CDlgLinea::NextInTabOrder()
{
	CWnd* pWndNext = GetNextDlgTabItem(GetFocus());
	if (pWndNext) {
		CWnd* pFin = GetDlgItem(IDC_LIN_MAQ);
		if (pWndNext == pFin)
			pWndNext->SetFocus();
		pWndNext->SetFocus();
		CEdit* pEdit = (CEdit*) pWndNext;
		if ( pEdit->IsKindOf( RUNTIME_CLASS( CEdit )))	pEdit->SetSel(0,-1);
	}
}

