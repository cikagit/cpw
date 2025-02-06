// cpwDoc.cpp : implementation of the CCpwDoc class
//

#include "stdafx.h"
#include "afxdtctl.h"
#include "cpw.h"
#include "cpwDefs.h"
#include "ObArrOrd.h"     


#include "Dialogs.h"
#include "AbSpan.h"
#include "cpwExt.h"
#include "Excel.h"
#include "prog.h"
#include "cpwDb.h"
#include "cache.h"
#include "ProgMaq.h"
#include "OrdMaq.h"
#include "CpwView.h"
#include "TextView.h"
#include "Turnos.h"
#include "cpwDoc.h"

#include <strstream>
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCpwDoc

IMPLEMENT_DYNCREATE(CCpwDoc, CDocument)

BEGIN_MESSAGE_MAP(CCpwDoc, CDocument)
	//{{AFX_MSG_MAP(CCpwDoc)
	ON_COMMAND(ID_ESCALA, OnEscala)
	ON_COMMAND(ID_RECALCULAR, OnRecalcular)
	ON_COMMAND(ID_CAMBIAR_FECHA_INICIO, OnCambiarFechaInicio)
	ON_COMMAND(ID_CONS_MAQORD, OnConsMaqord)
	ON_COMMAND(ID_CONS_CAMMOL, OnConsCammol)
	ON_COMMAND(ID_CONS_LOTES, OnConsLotes)
	ON_COMMAND(ID_OF_ORDEN, OnOfOrden)
	ON_COMMAND(ID_SELEC_FECHA, OnSelecFecha)
	ON_COMMAND(ID_NEXT_ART, OnNextArt)
	ON_COMMAND(ID_NEXT_PAGE, OnNextPage)
	ON_COMMAND(ID_PREV_ART, OnPrevArt)
	ON_COMMAND(ID_PREV_PAGE, OnPrevPage)
	ON_COMMAND(ID_PREV_SECCION, OnPrevSeccion)
	ON_COMMAND(ID_NEXT_SECCION, OnNextSeccion)
	ON_COMMAND(ID_SOLONOPROG, OnSolonoprog)
	ON_COMMAND(ID_OF_HASTAFECHA, OnOfHastafecha)
	ON_COMMAND(ID_CAMBIAR_FECHA_FIN, OnCambiarFechaFin)
	ON_COMMAND(ID_ORDENAR_MAQUINAS, OnOrdenarMaquinas)
	ON_COMMAND(ID_CONS_OFS, OnConsOfs)
	ON_COMMAND(ID_DESCONTAR_CIKA, OnDescontarCika)
	ON_COMMAND(ID_DESCONTAR_OTROS, OnDescontarOtros)
	ON_COMMAND(ID_VER_ACUMFECHA, OnVerAcumfecha)
	ON_COMMAND(ID_ESCALA_AMPLIA, OnEscalaAmplia)
	ON_COMMAND(ID_ESCALA_ESTRECHA, OnEscalaEstrecha)
	ON_COMMAND(ID_CONS_MEZ_EXT, OnConsMezExt)
	ON_COMMAND(ID_CLI_ORDEN, OnCliOrden)
	ON_COMMAND(ID_SOLONOFAB, OnSolonofab)
	ON_COMMAND(ID_SACAR_ASCII, OnSacarAscii)
	ON_COMMAND(ID_ARCHIVO_GUARDARCONPEDIDOS, OnArchivoGuardarconpedidos)
	ON_COMMAND(ID_ARCHIVO_ABRIRCONPEDIDOS, OnArchivoAbrirconpedidos)
	ON_COMMAND(IDD_EXCEL, OnExcel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCpwDoc construction/destruction

CCpwDoc::CCpwDoc()
{
	m_Prog.SetpDoc(this);
	m_BReadOnly = FALSE;
	m_Update = Otro;
//	m_BMoviendo = FALSE;
	m_pSelOM = NULL;
	m_pSelPM = NULL;
	m_pIniMovOM = NULL;
}

CCpwDoc::~CCpwDoc()
{
}

BOOL CCpwDoc::OnNewDocument()
{
	CCpwApp* wApp = (CCpwApp*) AfxGetApp();
	ASSERT_VALID(wApp);

	if (!CDocument::OnNewDocument())
		return FALSE;

	if (!g_pCache->GetBConnPedidosIsDefault()) wApp->OnReloadDb();

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCpwDoc serialization

void CCpwDoc::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	m_Prog.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CCpwDoc diagnostics

#ifdef _DEBUG
void CCpwDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCpwDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCpwDoc commands

void CCpwDoc::CargaCelulas(long p_lBloque, long p_lSerieIni, long p_lSerieFin) 
{
}

void CCpwDoc::OnEscala() 
{
	DlgEscala tDlgEscala;

	tDlgEscala.m_Escala = (int) m_Prog.GetTimeScale();
	if (tDlgEscala.DoModal() == IDOK)
	{
		m_Prog.SetTimeScale(tDlgEscala.m_Escala);
		AfxGetApp()->WriteProfileInt("Valores Iniciales", "Escala", tDlgEscala.m_Escala);
	}

	UpdateAllViews(NULL);
	
}


void CCpwDoc::OnRecalcular() 
{
	CWaitCursor WaitCursor;
	
	CProg* pProg = GetpProg();
	pProg->Recalc();
	
	
}

void CCpwDoc::OnCambiarFechaInicio() 
{
	CDlgIntrDate	DlgIntrDate;
	
	DlgIntrDate.m_sCaption = "Introducir Fecha Inicio";
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);
	DlgIntrDate.m_Fec = pProg->GetFecIni();

	if (DlgIntrDate.DoModal() != IDOK)
		return;
	
	if ( DlgIntrDate.m_Fec > pProg->GetFecIni())
	{
		if (AfxMessageBox( STRID(IDS_CONF_ADVINI), MB_OKCANCEL ) != IDOK ) return;
	}
	pProg->SetFecIni(DlgIntrDate.m_Fec);

	SetModifiedFlag();
	UpdateAllViews(NULL);
	
}

void 
CCpwDoc::OnCambiarFechaFin() 
{
	CDlgIntrDate	DlgIntrDate;
	
	DlgIntrDate.m_sCaption = "Introducir Fecha Fin";
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);
	DlgIntrDate.m_Fec = pProg->GetFecFin();

	if (DlgIntrDate.DoModal() != IDOK)
		return;
	
	pProg->SetFecFin(DlgIntrDate.m_Fec);

	SetModifiedFlag();
	UpdateAllViews(NULL);
		
}




BOOL CCpwDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{

	CCpwApp* wApp = (CCpwApp*) AfxGetApp();
	ASSERT_VALID(wApp);

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	CProg* pProg = GetpProg();
	if (pProg) pProg->Recalc();
	// Miramos si el fichero es Read Only, para no escribir en él.
	m_BReadOnly = FALSE;
	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyNone, &fe);
	if (pFile != NULL)
	{
		CFileStatus FStatus;
		pFile->GetStatus(FStatus);
		if (FStatus.m_attribute & 0x01) m_BReadOnly = TRUE;
		pFile->Close();
		delete pFile;
	}
	if (!g_pCache->GetBConnPedidosIsDefault()) wApp->OnReloadDb();

	GetpTextView()->SeteOrden();

	
	return TRUE;
}

void CCpwDoc::OnConsMaqord() 
{
	CWaitCursor WaitCursor;
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);

	CDlgMaqOrd DlgMaqOrd(this);
	DlgMaqOrd.m_FecDesde = pProg->GetFecIni();	
	DlgMaqOrd.m_FecHasta = pProg->GetFecFin();	
	pProg->GetArrMaqOM(DlgMaqOrd.m_ArrOM);
	DlgMaqOrd.m_eTipoCons = ConsMaqOrd;

	DlgMaqOrd.DoModal();
	UpdateAllViews(NULL);	
}


void CCpwDoc::OnConsCammol() 
{
	CWaitCursor WaitCursor;
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);

	CDlgMaqOrd DlgMaqOrd(this);
	DlgMaqOrd.m_FecDesde = pProg->GetFecIni();	
	DlgMaqOrd.m_FecHasta = AddDays(pProg->GetFecIni(), 1);	
		
	pProg->GetArrMaqOM(DlgMaqOrd.m_ArrOM);
	DlgMaqOrd.m_eTipoCons = ConsCambioMolde;

	DlgMaqOrd.DoModal();
	UpdateAllViews(NULL);	
	
	
}

void CCpwDoc::OnConsLotes() 
{
	
	CWaitCursor WaitCursor;
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);

	CDlgMaqOrd DlgMaqOrd(this);
	DlgMaqOrd.m_FecDesde = pProg->GetFecIni();	
	DlgMaqOrd.m_FecHasta = pProg->GetFecFin();	
	pProg->GetArrMaqOM(DlgMaqOrd.m_ArrOM);
	DlgMaqOrd.m_eTipoCons = ConsLotes;

	DlgMaqOrd.DoModal();
	UpdateAllViews(NULL);	
	
}

// Funcion que cambia la ordenacion de CTextview
void CCpwDoc::OnOfOrden() 
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	if (!pMenu) return;

	// BOfOrden nos dice si está check o no ahora BCliOrden nos dice si está la ordenación por cliente
	BOOL BOfOrden = pMenu->GetMenuState(ID_OF_ORDEN, MF_BYCOMMAND) == MF_CHECKED;
	BOOL BCliOrden = pMenu->GetMenuState(ID_CLI_ORDEN, MF_BYCOMMAND) == MF_CHECKED;

	// Si está el cliente, hay que quitarlo
	if (BCliOrden)
		pMenu->CheckMenuItem(ID_CLI_ORDEN, MF_UNCHECKED);

	// Alternamos el estado del menú
	enum eOrdenOF eOrdenOF = (BOfOrden) ? Articulo : Seccion ;

	pMenu->CheckMenuItem(ID_OF_ORDEN, (BOfOrden) ? MF_UNCHECKED : MF_CHECKED);

	g_VerOF.m_OrdenOF = eOrdenOF;
	
	GetpTextView()->SeteOrden();
}


// Funcion que cambia la ordenacion de CTextview
void CCpwDoc::OnCliOrden() 
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	if (!pMenu) return;

	// BOfOrden nos dice si está check o no ahora BCliOrden nos dice si está la ordenación por cliente
	BOOL BOfOrden = pMenu->GetMenuState(ID_OF_ORDEN, MF_BYCOMMAND) == MF_CHECKED;
	BOOL BCliOrden = pMenu->GetMenuState(ID_CLI_ORDEN, MF_BYCOMMAND) == MF_CHECKED;

	// Si está por Seccion, hay que quitarlo
	if (BOfOrden)
		pMenu->CheckMenuItem(ID_OF_ORDEN, MF_UNCHECKED);

	// Alternamos el estado del menú
	enum eOrdenOF eOrdenOF = (BOfOrden) ? Articulo : Cliente ;

	pMenu->CheckMenuItem(ID_CLI_ORDEN, (BCliOrden) ? MF_UNCHECKED : MF_CHECKED);

	g_VerOF.m_OrdenOF = eOrdenOF;
	
	GetpTextView()->SeteOrden();
}


void
CCpwDoc::UpdateTextView()
{
	GetpTextView()->SeteOrden();
}

// Funcion que permite seleccionar los pedidos hasta una fecha
void CCpwDoc::OnSelecFecha() 
{
	CDlgIntrDate	DlgIntrDate;
	
	DlgIntrDate.m_sCaption = "Introducir Fecha : ";
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);
	DlgIntrDate.m_Fec = CTime::GetCurrentTime();

	if (DlgIntrDate.DoModal() != IDOK)
	{
		UpdateAllViews(NULL);	
		return;
	}
	

	GetpTextView()->SelecToFec(DlgIntrDate.m_Fec);
	UpdateAllViews(NULL);	

}

void CCpwDoc::OnNextArt() 
{
	GetpTextView()->DoScroll(VK_F11, 1, 0);
}
void CCpwDoc::OnPrevArt() 
{
	GetpTextView()->DoScroll(VK_F11, 1,	VK_SHIFT);
}
void CCpwDoc::OnNextSeccion() 
{
	GetpTextView()->DoScroll(VK_F12, 1, 0);
}
void CCpwDoc::OnPrevSeccion() 
{
	GetpTextView()->DoScroll(VK_F12, 1, VK_SHIFT);
}
void CCpwDoc::OnNextPage() 
{
	GetpTextView()->DoScroll(VK_NEXT, 1, 0);
}
void CCpwDoc::OnPrevPage() 
{
	GetpTextView()->DoScroll(VK_PRIOR, 1, 0);
}

/*
void CCpwDoc::OnNextLine() 
{
	GetpTextView()->DoScroll(VK_DOWN, 1, 0);
}
void CCpwDoc::OnPrevLine() 
{
	GetpTextView()->DoScroll(VK_UP, 1, 0);
}
*/

CTextView*
CCpwDoc::GetpTextView()
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if ( pView->IsKindOf( RUNTIME_CLASS( CTextView ))) return (CTextView*) pView;
	}
	return NULL;
}


void CCpwDoc::OnSolonoprog() 
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	if (!pMenu) return;

	BOOL bActCheck = pMenu->GetMenuState(ID_SOLONOPROG, MF_BYCOMMAND) == MF_CHECKED;
	pMenu->CheckMenuItem(ID_SOLONOPROG, bActCheck ? MF_UNCHECKED : MF_CHECKED);

	if (!bActCheck)
	{
		g_VerOF.m_bSoloNoFab = FALSE;
		pMenu->CheckMenuItem(ID_SOLONOFAB, MF_UNCHECKED );
	}
	g_VerOF.m_bSoloNoProg = (pMenu->GetMenuState(ID_SOLONOPROG, MF_BYCOMMAND) == MF_CHECKED);

	GetpTextView()->SeteOrden();	
}

void CCpwDoc::OnSolonofab() 
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	if (!pMenu) return;

	BOOL bActCheck = pMenu->GetMenuState(ID_SOLONOFAB, MF_BYCOMMAND) == MF_CHECKED;
	pMenu->CheckMenuItem(ID_SOLONOFAB, bActCheck ? MF_UNCHECKED : MF_CHECKED);

	if (!bActCheck)
	{
		g_VerOF.m_bSoloNoProg = FALSE;
		pMenu->CheckMenuItem(ID_SOLONOPROG, MF_UNCHECKED );
	}
	g_VerOF.m_bSoloNoFab = (pMenu->GetMenuState(ID_SOLONOFAB, MF_BYCOMMAND) == MF_CHECKED);

	GetpTextView()->SeteOrden();	
}

void CCpwDoc::OnOfHastafecha() 
{
	CDlgIntrDate	DlgIntrDate;
	
	DlgIntrDate.m_sCaption = "Introducir Fecha";
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);
	DlgIntrDate.m_Fec = g_VerOF.m_FecSelec;
	
	
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	if (!pMenu) return;
	
	CString sMsg;
	UINT uStatFec = pMenu->GetMenuState(ID_OF_HASTAFECHA, MF_BYCOMMAND);
	if ( uStatFec == MF_UNCHECKED)  // Si estamos poniendo por fecha
	{
		if (DlgIntrDate.DoModal() != IDOK) 
		{
			UpdateAllViews(NULL);	
			return;
		}
		g_VerOF.m_FecSelec = DlgIntrDate.m_Fec;
		g_VerOF.m_bSelecFec = TRUE;
		sMsg = "HastaFecha : " + FormatFec(g_VerOF.m_FecSelec, Fecha);
	}
	else
	{
		g_VerOF.m_bSelecFec = FALSE;
		sMsg = "HastaFecha...";
	}
	pMenu->ModifyMenu(ID_OF_HASTAFECHA, MF_BYCOMMAND, ID_OF_HASTAFECHA, sMsg);
	pMenu->CheckMenuItem(ID_OF_HASTAFECHA, 
		(uStatFec == MF_UNCHECKED) ? MF_CHECKED : MF_UNCHECKED);

	GetpTextView()->SeteOrden();	
	UpdateAllViews(NULL);	

}


void CCpwDoc::OnOrdenarMaquinas() 
{
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);

	int iLim = pProg->GetNumPM();
	for (int i = 0; i < iLim; i++)
	{
		CProgMaq* pPM = pProg->GetpPM(i);
		CProgMaq* pPMNext = NULL;
		for (int j = i+1; j < iLim; j++)
		{
			pPMNext = pProg->GetpPM(j);
			CString sID = pPM->GetsID();
			CString sIDNext = pPMNext->GetsID();
			long lID = atol(sID);
			long lIDNext = atol(sIDNext);
			if (lID > lIDNext)
			{
				pProg->SwitchPM(i, j);
				CProgMaq* pPMInt = pPM;
				pPM = pPMNext;
				pPMNext = pPMInt;
			}
		}
	}
	SetModifiedFlag();
	UpdateAllViews(NULL);

}

void CCpwDoc::OnConsOfs() 
{
	CWaitCursor WaitCursor;
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);

	CDlgMaqOrd DlgMaqOrd(this);
	DlgMaqOrd.m_FecDesde = pProg->GetFecIni();	
	DlgMaqOrd.m_FecHasta = pProg->GetFecFin();	
	int iLim = g_pCache->GetArrConsOF(DlgMaqOrd.m_ArrOF);
	DlgMaqOrd.m_eTipoCons = ConsOF;
	int iLim2 = DlgMaqOrd.m_ArrOF.GetSize();
	DlgMaqOrd.DoModal();	

	for(int i=0; i < iLim2 ; i++ )
    {
		CComp* pComp = (CComp*) DlgMaqOrd.m_ArrOF.GetAt(i);
		delete pComp;    	
    }      
	UpdateAllViews(NULL);	

}


void CCpwDoc::OnConsMezExt() 
{
	CWaitCursor WaitCursor;
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);

	CDlgMaqOrd DlgMaqOrd(this);
	DlgMaqOrd.m_FecDesde = pProg->GetFecIni();	
	DlgMaqOrd.m_FecHasta = pProg->GetFecFin();
	// Eliminado al añadir selección por máquinas. La carga se realizará en el propio diálogo
	//pProg->GetArrMezExt(DlgMaqOrd.m_ArrMezExt);
	DlgMaqOrd.m_eTipoCons = ConsMezExt;

	DlgMaqOrd.DoModal();

	int iLim2 = DlgMaqOrd.m_ArrMezExt.GetSize();

	for(int i=0; i < iLim2 ; i++ )
    {
		CBufLisMezclas* pBufLisMezclas = (CBufLisMezclas*) DlgMaqOrd.m_ArrMezExt.GetAt(i);
		delete pBufLisMezclas;    	
    }      
	UpdateAllViews(NULL);	


}

void CCpwDoc::OnDescontarCika() 
{
	CDlgIntrDate	DlgIntrDate;
	
	DlgIntrDate.m_sCaption = "Fecha de avance : ";
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);
	DlgIntrDate.m_Fec = pProg->GetFecIni();

	if (DlgIntrDate.DoModal() != IDOK)
		return;
	
	if ( DlgIntrDate.m_Fec > pProg->GetFecIni())
	{
		if (AfxMessageBox( STRID(IDS_CONF_DESC), MB_OKCANCEL ) != IDOK ) return;
	}
	pProg->DescuentaVal(DlgIntrDate.m_Fec, 1);

	SetModifiedFlag();
	UpdateAllViews(NULL);
	
	
}

void CCpwDoc::OnDescontarOtros() 
{
	CDlgIntrDate	DlgIntrDate;
	
	DlgIntrDate.m_sCaption = "Fecha de avance : ";
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);
	DlgIntrDate.m_Fec = pProg->GetFecIni();

	if (DlgIntrDate.DoModal() != IDOK)
		return;
	
	if ( DlgIntrDate.m_Fec > pProg->GetFecIni())
	{
		if (AfxMessageBox( STRID(IDS_CONF_DESC), MB_OKCANCEL ) != IDOK ) return;
	}
	pProg->DescuentaVal(DlgIntrDate.m_Fec, 0);

	SetModifiedFlag();
	UpdateAllViews(NULL);
	
	
}

void CCpwDoc::OnVerAcumfecha() 
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	if (!pMenu) return;
	g_VerOF.m_bAcumOFs = (pMenu->GetMenuState(ID_VER_ACUMFECHA, MF_BYCOMMAND) == MF_UNCHECKED );
	pMenu->CheckMenuItem(ID_VER_ACUMFECHA, (g_VerOF.m_bAcumOFs) ? MF_CHECKED : MF_UNCHECKED);
		
	CTextView* pTextView = GetpTextView();

	pTextView->SeteOrden();
	
}

void CCpwDoc::OnEscalaAmplia() 
{

	int iEscala = AfxGetApp()->GetProfileInt("Escalas", "Uno", 600);

	AfxGetApp()->WriteProfileInt("Escalas", "Uno", iEscala);
	m_Prog.SetTimeScale(iEscala);

	UpdateAllViews(NULL);
	
}

void CCpwDoc::OnEscalaEstrecha() 
{

	int iEscala = AfxGetApp()->GetProfileInt("Escalas", "Dos", 2700);

	AfxGetApp()->WriteProfileInt("Escalas", "Dos", iEscala);
	m_Prog.SetTimeScale(iEscala);

	UpdateAllViews(NULL);	
}


void CCpwDoc::MoverOrden()
{

	int NumIni, NumFin;
	int Ind = 0;

	long lGFHIni;
	CString sMsg;
	CString sCap;
	CProgMaq* pPM;			

	
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);

	COrdMaq* pIniOM = GetpIniMovOM();
// Si no estamos ya moviendo, seleccionamos la orden actual como la orden
// a mover. 
	if (!GetBMoviendo())
	{
		SetIniMovOM(m_pSelOM);		 
		SetBMoviendo(TRUE);
		return;
	}
	else
	{
// Si ya estamos moviendo, movemos la orden ya seleccionada tras la orden
// sobre la que esta el cursor
		if (!pIniOM) return;
		// Primero calculamos el indice tras el que se ha de mover
		// y el GFH del que se parte
		if (GeteObjSel() == Orden)
		{
			COrdMaq* pOMSel = GetpSelOM();
			if (!pOMSel) return;
			lGFHIni = pOMSel->GetlGFH();
			pPM = pOMSel->GetpPM();
			Ind = pPM->Find(pOMSel) + 1;

		} else if (GeteObjSel() == Maquina)
		{
			CProgMaq* pPMSel = GetpSelPM();
			if (!pPMSel) return;
			lGFHIni = pPMSel->GetlGFH();    
//			if (pPMSel == pIniOM->GetpPM()) Ind = -1;
			pPM = pPMSel;
		}
		else return;		

		if ( lGFHIni != pIniOM->GetlGFH())
		{
			sMsg = STRID( IDS_ERR_MOVGFH );
			//sCap = STRID( IDS_ERROR );
			AfxMessageBox( sMsg );

			SetBMoviendo(FALSE);
			return;
		}
			
		// Y movemos finalmente
		
		CProgMaq* pIniPM = pIniOM->GetpPM();
		NumIni = pIniPM->GetNumOM();
		NumFin = pPM->GetNumOM();
		
		pProg->MoveOrdMaq(pPM, pIniOM, Ind);

		SetBMoviendo(FALSE);

		UpdateAllViews(NULL);
						
	}
}

void 
CCpwDoc::SetSelec(CObject* p_pObjSel, enum EObjSel p_eObjSel)
{
	
	m_eObjSel = p_eObjSel;
	if (p_eObjSel == Orden)
	{
		COrdMaq* pOM =(COrdMaq*) p_pObjSel;
		ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
		SelectOM(pOM);
	} 
	else if (p_eObjSel == Maquina)
	{
		CProgMaq* pPM =(CProgMaq*) p_pObjSel;
		ASSERT( pPM->IsKindOf( RUNTIME_CLASS( CProgMaq ) ) );
		SelectPM(pPM);
	}
	//UpdateAllViews(NULL);
}

void 
CCpwDoc::OnSacarAscii() 
{
	
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);
	
	CTime FecHoy = CTime::GetCurrentTime();
	//FecHoy = CTime(FecHoy.GetYear(), FecHoy.GetMonth(), FecHoy.GetDay(), TK_TURNO3_FIN ,0,0);
	FecHoy = CTurnos::GetFecIni(FecHoy, 1);
	
	
	int Bloque = 0;
	
	CWinApp* wApp = AfxGetApp();
	ASSERT_VALID(wApp);
	CString sFileName = wApp->GetProfileString("Valores Iniciales", "Fichero Ascii", "J:\\cpw\\CpwAscii.csv");
	if (sFileName == "C:\\CpwAscii.dat") sFileName = "J:\\cpw\\CpwAscii.csv";	// Cambio de default
	wApp->WriteProfileString("Valores Iniciales", "Fichero Ascii", sFileName);

	CString sLin;
	CObArray		ArrOMs;
	
	//AfxMessageBox("Escribiendo a fichero : " + sFileName);
	int iDlin = 0;
	pProg->GetArrMaqOM(ArrOMs);
	ofstream outf( sFileName, ios::out );  // Output file stream.
	if (outf)
	{
		sLin.Format( "Maq.nº; OFERTA; NºPiezas a fabricar; Piezas por hora; Horas máquina; Fecha inicio fabricación; Fecha fin fabricación;MEZCLA; Peso Unitario (gr); Necesidad mezcla (Kg); OBSERVACIONES");
		outf.write(sLin, sLin.GetLength() );
		outf.put(';');
		outf.put('\n');
		iDlin++;
		for(int ind = 0; ind < ArrOMs.GetSize() ; ind++)
		{
			COrdMaq* pOMAct = (COrdMaq*) ArrOMs.GetAt(ind);
			ASSERT_VALID(pOMAct);
			
			long lCant = pOMAct->GetlCantidad();
			CProgMaq* pPM = pOMAct->GetpPM();
			ASSERT_VALID(pPM);
			
			sLin.Format( "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s",
				pPM->GetsID(),
				pOMAct->GetsID(),
				FormatLong(lCant, 7),
				FormatDbl(pOMAct->GetdCadenciaGFH(), 5,1),
				FormatLong((long) (pOMAct->GetTimeSpan().GetTotalSeconds()/ TK_SEGS_HORA), 6), 
				FormatFec(pOMAct->GetFecIni(), FecHora),
				FormatFec(pOMAct->GetFecFin(), FecHora),
				pOMAct->GetsMezcla(),
				FormatDbl(pOMAct->GetdPeso(), 5, 1),
				pOMAct->GetsMezclaKilos()
				);
			outf.write(sLin, sLin.GetLength() );
			outf.put(';');
			outf.put('\n');
			iDlin++;
		}
		outf.close();
	//	CString sMsg;
	//	sMsg.Format("Escritas %d líneas", iDlin);
	//	AfxMessageBox(sMsg);
	}
}

void CCpwDoc::OnArchivoGuardarconpedidos() 
{
	CCpwApp* wApp = (CCpwApp*) AfxGetApp();
	ASSERT_VALID(wApp);

	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);

	// Pedimos el fichero
	CString sExtDef = STRID(IDS_DEF_EXT_PED);
	CString sExt = AfxGetApp()->GetProfileString("Valores Iniciales", "Extension Pedidos", sExtDef);
	CString sDlgDef = STRID(IDS_DEF_OPENDLG_PED);
	CString sDlg = AfxGetApp()->GetProfileString("Valores Iniciales", "Open File Pedidos", sDlgDef);

	CFileDialog DlgFile( TRUE, 
						sExt, 
						NULL, 
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						sDlg);

	if (DlgFile.DoModal() == IDCANCEL) return;
	
	CFile FileTest;
	if (FileTest.Open(DlgFile.GetPathName(), CFile::modeRead))
	{
		if (AfxMessageBox ("El fichero ya existe. ¿Sobreescribir?", MB_OKCANCEL) != IDOK) return;
	}
	
	CFile FilePCP;
	if (!FilePCP.Open(DlgFile.GetPathName(), CFile::modeCreate | CFile::modeWrite))
	{
		AfxMessageBox("No se pudo abrir el fichero. No se grabaron los datos.");
		return;
	}
	CArchive archive(&FilePCP, CArchive::store);
	Serialize(archive);

	wApp->CreateDbPedidos(DlgFile.GetPathName() + ".mdb");	
}

void CCpwDoc::OnArchivoAbrirconpedidos() 
{
	CCpwApp* wApp = (CCpwApp*) AfxGetApp();
	ASSERT_VALID(wApp);

	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);

	if (pProg->GetiNumOM())
	{
		AfxMessageBox("El programa no está vacío. No se puede cargar otro.");
		return;
	}

	// Pedimos el fichero
	CString sExtDef = STRID(IDS_DEF_EXT_PED);
	CString sExt = AfxGetApp()->GetProfileString("Valores Iniciales", "Extension Pedidos", sExtDef);
	CString sDlgDef = STRID(IDS_DEF_OPENDLG_PED);
	CString sDlg = AfxGetApp()->GetProfileString("Valores Iniciales", "Open File Pedidos", sDlgDef);

	CFileDialog DlgFile( TRUE, 
						sExt, 
						NULL, 
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						sDlg);

	if (DlgFile.DoModal() == IDCANCEL) return;
	
	
	CFile FilePCP;
	if (!FilePCP.Open(DlgFile.GetPathName(), CFile::modeRead))
	{
		AfxMessageBox("No se pudo abrir el fichero. No se cargarán los datos.");
		return;
	}
	CArchive archive(&FilePCP, CArchive::load);
	Serialize(archive);
	pProg->Recalc();
	CString sDbPath = DlgFile.GetPathName() + ".mdb";
	CString sDir = DlgFile.GetPathName();
	sDir = sDir.Left(sDir.GetLength() - DlgFile.GetFileName().GetLength());
	CString	sDb = "ODBC;DSN=MS Access 97 Database;DBQ=" + sDbPath
				+ ";DefaultDir=" + sDir
				+ ";DriverId=25;FIL=MS Access;MaxBufferSize=512;PageTimeout=5;";

	CString sDbIni = GetsConnPedidos();
	SetsConnPedidos(sDb);
	wApp->OnReloadDb();
	SetsConnPedidos(sDbIni);
	

	UpdateAllViews(NULL);
}

void CCpwDoc::OnExcel() 
{
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);
	
	
	CDlgExcel	DlgExcel;
	
	if (DlgExcel.DoModal() != IDOK)
		return;
	CTime FecIni = DlgExcel.m_FecDesde;
	CTime FecFin = DlgExcel.m_FecHasta;
	CString sDesdeMaq = DlgExcel.m_sDesdeMaq;
	CString sHastaMaq = DlgExcel.m_sHastaMaq;


	pProg->Recalc();
	//AfxMessageBox("Obteniendo OFs");
	CObArray	ArrOMs;		// Array de OMs a listar

	
	int iLim = pProg->GetArrMaqOMLis(ArrOMs);
	//CString sMsg;
	//sMsg.Format("Obtenidas %d OFs", ArrOMs.GetSize());
	//AfxMessageBox(sMsg);
	
	// Vamos a filtrarlo para quedarnos sólo con máquinas y fechas seleccionadas
	for (int i=0; i < iLim; i++)
	{
		COrdMaq* pOM = (COrdMaq*) ArrOMs.GetAt(i);
		BOOL bRepMaq = FALSE;
		if (pOM->GetFecIni() > FecFin  || 
			pOM->GetFecFin() < FecIni  ||
			pOM->GetsMaquina() < sDesdeMaq ||
			pOM->GetsMaquina() > sHastaMaq )
		{
			ArrOMs.RemoveAt(i);
			delete pOM;
			i--;
			iLim--;
		}
	}
	
	iLim = ArrOMs.GetSize();

	if (iLim == 0) {
		AfxMessageBox("No hay ordenes seleccionadas");
		return;
	}

	const int numCol = 8;
	long lEmpresa = (long) GetNumEmpresa(g_sEmpresa);

	VARIANT arr;
   arr.vt = VT_ARRAY | VT_VARIANT;
   {
      SAFEARRAYBOUND sab[2];
      sab[0].lLbound = 1; sab[0].cElements = iLim;
      sab[1].lLbound = 1; sab[1].cElements = numCol;
      arr.parray = SafeArrayCreate(VT_VARIANT, 2, sab);
   }
   CString tmpStr = "";
   CObArray strArr;
   strArr.SetSize(100, 100);
   for (int ii = 0; ii < strArr.GetSize(); ii++) {
	   CStringArray* pNewStringArray = new CStringArray();
	   pNewStringArray->SetSize(9, 1);

	   strArr.SetAt(ii, pNewStringArray);
   }
	for(int j=1; j<= numCol; j++) {
						 
			VARIANT tmp;
			tmp.vt = VT_BSTR;
			if (j == 1) {
				tmp.bstrVal = ::SysAllocString(L"EMP.");
				tmpStr = "EMP.";
			} else if (j == 2) {
				tmp.bstrVal = ::SysAllocString(L"CENTRO");
				tmpStr = "CENTRO";
			} else if (j == 3) {
				tmp.bstrVal = ::SysAllocString(L"OPERACION");
				tmpStr = "OPERACION";
			} else if (j == 4) {
				tmp.bstrVal = ::SysAllocString(L"TIPO_LINEA");
				tmpStr = "TIPO_LINEA";
			} else if (j == 5) {
				tmp.bstrVal = ::SysAllocString(L"OFERTA");
				tmpStr = "OFERTA";
			} else if (j == 6) {
				tmp.bstrVal = ::SysAllocString(L"CANTIDAD");
				tmpStr = "CANTIDAD";
			} else if (j == 7) {
				tmp.bstrVal = ::SysAllocString(L"CADENCIA");
				tmpStr = "CADENCIA";
			} else if (j == 8) {
				tmp.bstrVal = ::SysAllocString(L"PLANIFICADOR");
				tmpStr = "PLANIFICADOR";
			}
			// Add to safearray...
			long indices[] = {1,j};
         //SafeArrayPutElement(arr.parray, indices, (void *)&tmp);
		 StringArrayPutElement(strArr, 0, j, tmpStr);
		}

	for (int i = 0; i < iLim; i++)
	{
		COrdMaq* pOM = (COrdMaq*) ArrOMs.GetAt(i);
	   	for(int j=1; j<= numCol; j++) {
			 
			VARIANT tmp;
			tmp.vt = VT_I4;
			if (j == 1) {
				tmp.lVal = lEmpresa;
				tmpStr.Format("%ld", lEmpresa);
			}
			else if (j == 2) {
				tmp.lVal = atol(pOM->GetsMaquina());
				tmpStr = pOM->GetsMaquina();
			} else if (j == 3) {
				tmp.lVal = pOM->GetpPM()->GetlOperacion();;
				tmpStr.Format("%ld", tmp.lVal);
			} else if (j == 4) {
				tmp.vt = VT_BSTR;
				tmp.bstrVal = (pOM->GetsID() == "" ) ? ::SysAllocString(L"P") : ::SysAllocString(L"N");
				tmpStr = tmp.bstrVal;
			} else if (j == 5) {
				CString sTmp = "'" + pOM->GetsID();
				tmp.vt = VT_BSTR;
				tmp.bstrVal  = sTmp.AllocSysString();
				//tmpStr = pOM->GetsID() == "" ? "" : "=\"\"" + pOM->GetsID() + "\"\"";
				tmpStr = pOM->GetsID();
			} else if (j == 6) {
				CString sTmp = "";
				if (pOM->GetsID() != "" ) {
					sTmp.Format("%ld",pOM->GetlCantReal());
				}
				tmp.vt = VT_BSTR;
				tmp.bstrVal =  sTmp.AllocSysString();
				tmpStr = tmp.bstrVal;
				//debug
				
			} else if (j == 7) {
				tmp.lVal = (pOM->GetsID() == "" ) ?  (long) (pOM->GetFecFin() - pOM->GetFecIni()).GetTotalHours()  : (long) pOM->GetdCadenciaGFH();
				//tmpStr = FormatLong(tmp.lVal, 6);
				tmpStr.Format("%ld", tmp.lVal);
			} else if (j == 8) {
				CString sTmp = "'" + pOM->GetsPlanificador();
				tmp.vt = VT_BSTR;
				tmp.bstrVal  = sTmp.AllocSysString();
				tmpStr = pOM->GetsPlanificador();
			}
			// Add to safearray...
			long indices[] = {i+2,j};
         //SafeArrayPutElement(arr.parray, indices, (void *)&tmp);
		 StringArrayPutElement(strArr, i+1, j, tmpStr);
		}
	}
	
   //PasarAExcel(numCol, iLim, arr);
	//VariantClear(&arr);

	PasarACsv(strArr);
	ClearArray(strArr, RUNTIME_CLASS(CStringArray));

	// Borramos la lista, porque se han creado las OMs nuevas, para dividir en 2 en las inactividades
	// de calendario
	for (int i = 0; i < iLim; i++)
	{
		COrdMaq* pOM = (COrdMaq*) ArrOMs.GetAt(i);
		delete pOM;
	}
	

}
