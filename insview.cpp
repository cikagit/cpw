// InsView.cpp : implementation file
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
#include "DlgLinea.h"  
#include "DlgOpciones.h"  

#include "ListViewEx.h"
#include "InsView.h"

#define TK_DESPL_CAMPOS 50
#define TK_NUMCOL_FIJAS 7

// Definiciones de posibles elementos opcionales
// Para añadir una columna nueva opcional se debe hacer lo siguiente:
// - Añadirla en estas tres matrices
// - Añadir un código nuevo a los TK_ID_... en el .rh
// - Modificar la función GetsColValue para que devuelva un valor para el TK_ID_... definido

	char* g_ArrsCols[][2] = 
	{ 
		"F.Ini.", "Fecha de Inicio",
		"F.Fin.", "Fecha de Fin",
		"Lote", "Lote de Mezcla",
		"% Chat.", "% de Chatarras",
		"% Inact.", "% de Inactividad",
		"Peso", "Peso total",
		"Peso U.", "Peso unitario",
		"Cant.Fab.", "Cantidad Fabricada",
		"Cant.Chat.", "Cantidad Chatarras",
		"Acum.H.", "Acumulado Horas",
		"Cliente", "Cliente",
		"Margen", "Margen",
		"Nec.Mez", "Necesidad de Mezcla",
		"Mezcla", "Mezcla Restante",
		"H.Mez.", "Horas con esa Mezcla"
	};
	int g_ArriFmts[] =
	{ 
		LVCFMT_LEFT,
		LVCFMT_LEFT,
		LVCFMT_RIGHT,
		LVCFMT_RIGHT,
		LVCFMT_RIGHT,
		LVCFMT_RIGHT,
		LVCFMT_RIGHT,
		LVCFMT_RIGHT,
		LVCFMT_RIGHT,
		LVCFMT_RIGHT,
		LVCFMT_LEFT,
		LVCFMT_RIGHT,
		LVCFMT_RIGHT,
		LVCFMT_RIGHT,
		LVCFMT_RIGHT 
	};
	int g_ArriKeys[] =
	{
		TK_ID_FECINI,
		TK_ID_FECFIN,
		TK_ID_LOTE,
		TK_ID_PORCHAT,
		TK_ID_PORINAC,
		TK_ID_PESTOT,
		TK_ID_PESUNI,
		TK_ID_CANTFAB,
		TK_ID_CANTCHAT,
		TK_ID_TOTHORAS,
		TK_ID_CLIENTE,
		TK_ID_MARGEN,
		TK_ID_NECMEZ,
		TK_ID_HORMEZ,
		TK_ID_MEZCLA
	};


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsView

IMPLEMENT_DYNCREATE(CInsView, CListViewEx)

CInsView::CInsView()
{
	m_pSelOM = NULL;
	m_pSelPM = NULL;
	m_eObjSel = Nada;
	
	CWinApp* wApp = AfxGetApp();
	CString sClaves;
	CString sAnchuras;
	sClaves = wApp->GetProfileString("Columnas Auxiliares", "Claves", "");
	sAnchuras = wApp->GetProfileString("Columnas Auxiliares", "Anchuras", "");	
	int iLim = sClaves.GetLength();
	CString sNum = "";
	for(int i=0; i < iLim; i++)
	{
		char c = sClaves[i];
		if (isdigit(c)) sNum += c;
		else 
		{
			int iNum = atoi(sNum);
			if (iNum) m_ArrUKeys.Add(iNum);
			sNum = "";
		}	
	}
	iLim = sAnchuras.GetLength();
	sNum = "";
	for(int i=0; i < iLim; i++)
	{
		char c = sAnchuras[i];
		if (isdigit(c)) sNum += c;
		else 
		{
			int iNum = atoi(sNum);
			if (iNum) m_ArrUWidths.Add(iNum);
			sNum = "";
		}	
	}
	
	int iLimB = sizeof(g_ArriKeys)/sizeof(int);
	// Comprobamos que coinciden el tamaño del resto de las matrices
	if (iLimB != sizeof(g_ArriFmts)/sizeof(int)) 
	{
		TRACE("Error de g_ArriFmts");
		return;
	}
	if (iLimB != (sizeof(g_ArrsCols)/(sizeof(char*)*2))) 
	{
		TRACE("Error de g_ArrsCols");
		return;
	}
	iLim = m_ArrUKeys.GetSize();
	for(int j=0; j < iLim; j++)
	{
		UINT iKey = m_ArrUKeys.GetAt(j);
		int i = 0;
		for ( ; i < iLimB; i++)
		{
			int iKeyB = g_ArriKeys[i];
			if ((UINT) iKeyB == iKey) break;
		}
		m_ArrsDescCol.Add((i>=iLimB) ? "" : g_ArrsCols[i][1]);
		m_ArrsNomCol.Add((i>=iLimB) ? "" : g_ArrsCols[i][0]);
		m_ArrUFmts.Add((i>=iLimB) ? LVCFMT_RIGHT: g_ArriFmts[i]);		
	}
}

CInsView::~CInsView()
{

}
BEGIN_MESSAGE_MAP(CInsView, CListViewEx)
	//{{AFX_MSG_MAP(CInsView)
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDITAR_ORDEN, OnEditarOrden)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_MOVER_ORDEN, OnMoverOrden)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(TK_AA_INI_OF_VALUES, TK_ZZ_END_OF_VALUES, OnAddColumn)
	ON_COMMAND_RANGE(TK_AA_INI_OF_VALUES+TK_DESPL_CAMPOS, TK_ZZ_END_OF_VALUES+TK_DESPL_CAMPOS, OnDelColumn)
END_MESSAGE_MAP()

void 
CInsView::OnAddColumn( UINT p_nID )
{
	int iLim = sizeof(g_ArriKeys)/sizeof(int);
	// Comprobamos que coinciden el tamaño del resto de las matrices
	if (iLim != sizeof(g_ArriFmts)/sizeof(int)) 
	{
		TRACE("Error de g_ArriFmts");
		return;
	}
	if (iLim != (sizeof(g_ArrsCols)/(sizeof(char*)*2))) 
	{
		TRACE("Error de g_ArrsCols");
		return;
	}
	int i = 0;
	for ( ; i < iLim; i++)
	{
		int iKey = g_ArriKeys[i];
		if ((UINT) iKey == p_nID) break;
	}
	if (i >= iLim) return;	
	m_ArrUKeys.Add(p_nID);
	m_ArrUWidths.Add(100);
	m_ArrUFmts.Add(g_ArriFmts[i]);
	m_ArrsNomCol.Add(g_ArrsCols[i][0]);
	m_ArrsDescCol.Add(g_ArrsCols[i][1]);

	RecalcMaq();
	Recalc();
}
void 
CInsView::OnDelColumn( UINT p_nID )
{
	// Aquí llega el código desplazado, para diferenciar inserciones de borrados
	p_nID -= TK_DESPL_CAMPOS;
	int iLim = m_ArrUKeys.GetSize();
	int i = 0;
	for ( ; i < iLim; i++)
	{
		UINT iKey = m_ArrUKeys.GetAt(i);
		if (iKey == p_nID) break;
	}
	if (i >= iLim) return;	
	
	m_ArrUKeys.RemoveAt(i);
	m_ArrUWidths.RemoveAt(i);
	m_ArrUFmts.RemoveAt(i);
	m_ArrsNomCol.RemoveAt(i);
	m_ArrsDescCol.RemoveAt(i);

	CListCtrl& ListCtrl = GetListCtrl();
	
	ListCtrl.DeleteColumn(i+TK_NUMCOL_FIJAS);
	Recalc();
}
/////////////////////////////////////////////////////////////////////////////
// CInsView drawing

void CInsView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CInsView diagnostics

#ifdef _DEBUG
void CInsView::AssertValid() const
{
	CListView::AssertValid();
}

void CInsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CCpwDoc* CInsView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCpwDoc)));
	return (CCpwDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInsView message handlers

void CInsView::OnInitialUpdate() 
{
	CListViewEx::OnInitialUpdate();
	

	
	CCpwDoc* pDoc = GetDocument();
	CProg* pProg = pDoc->GetpProg();
	
	pProg->GetArrMaqOM(m_ArrOM);
	
	RecalcMaq();
	Recalc();
	
	
}

void 
CInsView::OnMoverOrden()
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);  
	if (!pDoc->GetBMoviendo())
	{
		HCURSOR	HCursor = AfxGetApp()->LoadCursor( IDC_MOVE );
		SetCursor( HCursor );
	} else
	{
		SetCursor( AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	pDoc->MoverOrden();
	if (pDoc->GetBMoviendo())
	{
		Invalidate();	
	}
	
}


void CInsView::OnRButtonDown(UINT p_iFlags, CPoint p_Point) 
{
	CListViewEx::OnRButtonDown(p_iFlags, p_Point);
	
	CObject* pObject = ObjetoApuntado(p_Point);


	CMenu PopUp;
	PopUp.CreatePopupMenu();
	CMenu PopUpAlter;
	PopUpAlter.CreatePopupMenu();
	CMenu PopUpAlterAdd;
	PopUpAlterAdd.CreatePopupMenu();
	CMenu PopUpAlterDel;
	PopUpAlterDel.CreatePopupMenu();
	
	UINT tTipo = MF_ENABLED | MF_STRING;
	PopUp.AppendMenu(tTipo, ID_EDITAR_ORDEN, "Expandir...");
	PopUp.AppendMenu(tTipo, ID_NUEVA_ORDEN, "Nuevo Bloque...");
	PopUp.AppendMenu(tTipo, ID_MOVER_ORDEN, "Mover");

	int iLim = sizeof(g_ArriKeys)/sizeof(int);
	BOOL bYaSta = FALSE; 
	for (int i = 0; i < iLim; i++)
	{
		int iKey = g_ArriKeys[i];
		int iLimI = m_ArrUKeys.GetSize();
		bYaSta = FALSE;
		for (int j = 0; j < iLimI; j++)
		{
			UINT iKeyI = m_ArrUKeys.GetAt(j);
			if ((UINT) iKey == iKeyI)
			{
				bYaSta = TRUE;
				break;
			}
		}
		if (!bYaSta) PopUpAlterAdd.AppendMenu(tTipo, iKey, g_ArrsCols[i][1]);
		
	}
	int iLimI = m_ArrUKeys.GetSize();
	for (int j = 0; j < iLimI; j++)
	{
		UINT iKeyI = m_ArrUKeys.GetAt(j);
		CString sDescI = m_ArrsDescCol.GetAt(j);
		PopUpAlterDel.AppendMenu(tTipo, iKeyI+TK_DESPL_CAMPOS, sDescI);
	}
	
	
	PopUp.AppendMenu(MF_STRING | MF_POPUP, (UINT) PopUpAlter.m_hMenu, "Alterar");
	PopUpAlter.AppendMenu(MF_STRING | MF_POPUP, (UINT) PopUpAlterAdd.m_hMenu, "Insertar");
	PopUpAlter.AppendMenu(MF_STRING | MF_POPUP, (UINT) PopUpAlterDel.m_hMenu, "Borrar");
		  
	CPoint ScrPoint(p_Point);
	ClientToScreen(&ScrPoint);
	PopUp.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ScrPoint.x, ScrPoint.y, this);
}

void CInsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CCpwDoc* pDoc = GetDocument();
	CProg* pProg = pDoc->GetpProg();
	
	if (nChar == VK_INSERT)
	{
		CListCtrl& ListCtrl = GetListCtrl();
		int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
		
		if(nItem == -1) nItem = 0;
		
		m_ArrOM.InsertAt(nItem, (CObject*) NULL);
		Recalc();
		while (LinEdit(nItem))
		{
			pProg->GetArrMaqOM(m_ArrOM);
			Recalc();
			m_ArrOM.InsertAt(nItem, (CObject*) NULL);
		}
		m_ArrOM.RemoveAt(nItem);
		Recalc();
	}
	else
	{
		CChildFrame* pFrame = (CChildFrame*) GetParentFrame();
		ASSERT (pFrame->IsKindOf( RUNTIME_CLASS( CChildFrame ) ));
		pFrame->OnCharRec( nChar, nRepCnt, nFlags);	
	}
	
	CListViewEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CInsView::Recalc()
{
	const int iNumCol = TK_NUMCOL_FIJAS;
	
	LV_ITEM lvi;
	CListCtrl& ListCtrl = GetListCtrl();
	int iTopIndex = ListCtrl.GetTopIndex();
	ListCtrl.DeleteAllItems();
	int iLim = m_ArrOM.GetSize();
	int iLin = 0;
	CString sMaqAnt = "";
	double dAcum = 0;
	for (int i=0; i < iLim; i++)
	{
		const int TK_SIZE_MAQ = 10;
		char sMaq[TK_SIZE_MAQ+1];
		COrdMaq* pOM =  (COrdMaq*) m_ArrOM.GetAt(i);
		
		COrdMaq* pOMNext = NULL;
		BOOL bRepMaq = FALSE;
		if (pOM) 
		{
			if (sMaqAnt == pOM->GetsMaquina())
			{
				bRepMaq = TRUE;
			}
			sMaqAnt = pOM->GetsMaquina();
			if (bRepMaq)
				strncpy(sMaq,"", TK_SIZE_MAQ);
			else
				strncpy(sMaq,pOM->GetsMaquina(), TK_SIZE_MAQ);
		} else
		{
			strncpy(sMaq,"", TK_SIZE_MAQ);
		}
		lvi.mask = LVIF_TEXT;
		lvi.iItem = iLin;
		lvi.iSubItem = 0;
		lvi.pszText =  sMaq;
		lvi.iImage = 0;
		
		if (pOM)
		{
				
			if (bRepMaq)
				dAcum += (double) pOM->GetTimeSpan().GetTotalHours();
			else
				dAcum = (double) pOM->GetTimeSpan().GetTotalHours();

			pOM->m_dAux = dAcum;

		}

		ListCtrl.InsertItem(&lvi);
		if (pOM)
		{
			ListCtrl.SetItemText(iLin,1,pOM->GetsID());
			ListCtrl.SetItemText(iLin,2,pOM->GetsMoldeDesc());
			ListCtrl.SetItemText(iLin,3,pOM->GetsMezcla());
			ListCtrl.SetItemText(iLin,4,FormatLong(pOM->GetlCantidad(),7));
			ListCtrl.SetItemText(iLin,5,FormatLong((long) pOM->GetdCadenciaGFH(),5));
			ListCtrl.SetItemText(iLin,6,FormatLong(((long) pOM->GetTimeSpan().GetTotalHours()),5));

			for(int j = 0; j < m_ArrUKeys.GetSize(); j++)
			{
				ListCtrl.SetItemText(iLin,iNumCol+j,GetsColValue(m_ArrUKeys.GetAt(j), pOM));
			}
		}
		iLin++;
	}
	ListCtrl.EnsureVisible(iTopIndex, FALSE);
}

void 
CInsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CCpwDoc* pDoc = GetDocument();
	CProg* pProg = pDoc->GetpProg();

	pProg->GetArrMaqOM(m_ArrOM);

	Recalc();	
	
}

void 
CInsView::OnEditarOrden() 
{

	CCpwDoc* pDoc = GetDocument();
	CProg* pProg = pDoc->GetpProg();
	CListCtrl& ListCtrl = GetListCtrl();
	int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);

	if(nItem == -1) return;

	COrdMaq* pOM = (COrdMaq*) m_ArrOM.GetAt(nItem);
	if (pOM == NULL)
	{
		CString sMsg;
		sMsg.Format( STRID (IDS_OF_NOTFOUND) ,	pOM->GetsID() ); 
		MessageBox(sMsg);
		return;
	}
	pOM->SetBSelected(TRUE);
	CCpwView* pCpwView = GetpCpwView();

	pCpwView->m_pSelOM = pOM;
	pCpwView->SendMessage(WM_COMMAND, ID_EDITAR_ORDEN);

}

CCpwView* 
CInsView::GetpCpwView()
{
	CCpwDoc* pDoc = GetDocument();
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if ( pView->IsKindOf( RUNTIME_CLASS( CCpwView )))
			return (CCpwView*) pView;
	}
	return NULL;
}

int 
CInsView::LinEdit(int p_iItem)
{
	CCpwDoc* pDoc = GetDocument();
	CProg* pProg = pDoc->GetpProg();
	CListCtrl& ListCtrl = GetListCtrl();
	
	COrdMaq* pOM = (COrdMaq*) m_ArrOM.GetAt(p_iItem);
	CPoint Point;
	int iY = ListCtrl.GetItemPosition(p_iItem, &Point);
	CRect RectLabel;
	ListCtrl.GetItemRect(p_iItem, RectLabel, LVIR_LABEL);
	
	CRect RectWin;
	GetWindowRect(&RectWin);
	CDlgLinea DlgLinea(this);
	DlgLinea.m_Point.x = 0;
	DlgLinea.m_Point.y = Point.y+RectWin.top;
	
	if (pOM)
	{
		DlgLinea.m_sLinMaq = pOM->GetsMaquina();
		DlgLinea.m_sLinArt = pOM->GetsID();
		DlgLinea.m_sLinCad = FormatLong((long)pOM->GetdCadencia(),7, NoPoint);
		DlgLinea.m_sLinCant = FormatLong(pOM->GetlCantidad(),9, NoPoint);
		DlgLinea.m_sLinHoras.Format("%.2f", (double) ((double)pOM->GetTimeSpan().GetTotalSeconds() / TK_SEGS_HORA));
		DlgLinea.m_sLinMezcla = pOM->GetsMezcla();
		DlgLinea.m_sLinMolde = pOM->GetsMolde();
	}
	
	
	
	int iRes;
	for(;;)
	{
		iRes = DlgLinea.DoModal();
		if (iRes == IDOK) 
		{
			if (DlgLinea.Validate()) break;
		}
		else break;
	}
	if (iRes == IDOK)
	{
		if (!pOM)		// Es una inserción nueva
		{
			pOM = new COrdMaq(DlgLinea.m_sLinArt);
			CProgMaq* pPM = pProg->FindMaq(DlgLinea.m_sLinMaq);
			if (!pPM)
			{
				delete pOM;
				return FALSE;
			}
			pPM->InsertOM(pOM, pPM->GetNumOM());	// La insertamos al final.
		}
		pOM->SetpBufArt(g_pCache->FindpArticulo(DlgLinea.m_sLinArt));
		pOM->SetpBufMolde(g_pCache->FindpMolde(DlgLinea.m_sLinMolde));
		pOM->SetTs((CTimeSpan) (long) (atof(DlgLinea.m_sLinHoras)*TK_SEGS_HORA));
		double dCad = atof(DlgLinea.m_sLinCad);
		// Sólo actualizamos la cadencia si se ha metido una diferente a la del valor por defecto
		if (dCad > 0 && dCad != pOM->GetpBufArticulo()->GetdCadencia())
			pOM->SetdCadencia(dCad);
		//long lCant = (long) (pOM->GetTimeSpan().GetTotalSeconds() * pOM->GetdCadencia() / (double) TK_SEGS_HORA );
		long lCant = atol(DlgLinea.m_sLinCant);
		
		pOM->SetlCantidad(pOM->GetlCantSinChatarras(lCant));
		pOM->SyncCant();
		pOM->GetpPM()->Recalc();
		pDoc->UpdateAllViews(this);
		return TRUE;
		
	} else
	{
		return FALSE;
	}
}



void CInsView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CListCtrl& ListCtrl = GetListCtrl();
	int iItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if(iItem == -1) return;
	
	if (LinEdit(iItem))
		Recalc();
	
	CListViewEx::OnLButtonDblClk(nFlags, point);
}

void CInsView::RecalcMaq()
{
	
	LV_COLUMN lvc;
	const int iNumCol = TK_NUMCOL_FIJAS;
	
	CListCtrl& ListCtrl = GetListCtrl();
	
	char* ArrsCols[] = { "Máq.", "Oferta", "Molde", "Mezcla", "Piezas",  "P/H", "Horas" };
	int ArriWidths[] = { 30, 100, 100, 100, 60, 40, 40 };
	int ArriFmts[] = { LVCFMT_RIGHT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT };
	
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	// Borramos todas las columnas
	for (; ListCtrl.DeleteColumn(0););
	{
	}
	for(int i = 0; i < iNumCol; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = ArrsCols[i];
		lvc.cx = ArriWidths[i];
		lvc.fmt = ArriFmts[i];
		ListCtrl.InsertColumn(i,&lvc);
	}
	
	for(int j = 0; j < m_ArrUKeys.GetSize(); j++)
	{
		lvc.iSubItem = iNumCol+j;
		char Buffer[512];
		strcpy(Buffer, m_ArrsNomCol.GetAt(j));
		lvc.pszText = Buffer;
		lvc.cx = m_ArrUWidths.GetAt(j);
		lvc.fmt = m_ArrUFmts.GetAt(j);
		ListCtrl.InsertColumn(iNumCol+j,&lvc);
	}
}

CString 
CInsView::GetsColValue(int p_iClave, COrdMaq* p_pOM)
{
	CString sValue;
	
	CBufArticulo* pBufArt = p_pOM->GetpBufArticulo();

	if (p_iClave == TK_ID_FECINI) return FormatFec(p_pOM->GetFecIni(), FecHora);
	else if (p_iClave == TK_ID_FECFIN) return FormatFec(p_pOM->GetFecFin(), FecHora);
	else if (p_iClave == TK_ID_LOTE) return FormatLong(p_pOM->GetlLote(), 5);
	else if (p_iClave == TK_ID_PORCHAT)
	{
		sValue.Format("%6.2f", p_pOM->GetdChatarras());
	}
	else if (p_iClave == TK_ID_PORINAC) 
	{
		sValue.Format("%6.2f", p_pOM->GetdInactividad());
	}
	else if (p_iClave == TK_ID_PESTOT) return FormatLong((long) ((double)p_pOM->GetlCantidad()*p_pOM->GetdPeso()),10);
	else if (p_iClave == TK_ID_PESUNI) sValue.Format("%.1lf", p_pOM->GetdPeso());
	else if (p_iClave == TK_ID_CANTFAB) return ( pBufArt ? FormatLong(pBufArt->m_LV_lCantidad_Fabricada, 6) : "");
	else if (p_iClave == TK_ID_CANTCHAT) return ( pBufArt ? FormatLong(pBufArt->m_LV_lCantidad_Rechazada, 6) : "");
	else if (p_iClave == TK_ID_TOTHORAS) return FormatLong((long) p_pOM->m_dAux,6);
	else if (p_iClave == TK_ID_CLIENTE) return ( pBufArt ? pBufArt->m_LV_sCliente : "");
	else if (p_iClave == TK_ID_MEZCLA) return FormatLong(p_pOM->GetlCantMezcla(), 7);
	else if (p_iClave == TK_ID_HORMEZ) return FormatLong((long) p_pOM->GetTsMezcla().GetTotalHours(), 7);
	else if (p_iClave == TK_ID_NECMEZ) return FormatLong(p_pOM->GetlNecMezcla(), 7);
	else if (p_iClave == TK_ID_MARGEN) return "0";


	return sValue;
}

void CInsView::OnDestroy() 
{
	CListCtrl& ListCtrl = GetListCtrl();
	CWinApp* wApp = AfxGetApp();

	CString sClaves;
	CString sAnchuras;
	
	for (int i=0; i < m_ArrUKeys.GetSize(); i++)
	{
		CString sInt;
		sInt.Format("%u,",m_ArrUKeys.GetAt(i));
		sClaves += sInt;
		
		sInt.Format("%u,", (UINT) ListCtrl.GetColumnWidth(i+TK_NUMCOL_FIJAS));
		sAnchuras += sInt;
	}
	wApp->WriteProfileString("Columnas Auxiliares", "Claves", sClaves);
	wApp->WriteProfileString("Columnas Auxiliares", "Anchuras", sAnchuras);

	CListViewEx::OnDestroy();
	
	
}

BOOL CInsView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CCpwDoc* pDoc = GetDocument();
	if (pDoc && pDoc->GetBMoviendo())
	{
		HCURSOR	HCursor = AfxGetApp()->LoadCursor( IDC_MOVE );
		SetCursor( HCursor );
		return TRUE;
	} else
		return CListViewEx::OnSetCursor(pWnd, nHitTest, message);
	
}

CObject* 
CInsView::ObjetoApuntado( CPoint p_Pnt )
{            
	enum EObjSel eObjSel = Nada;
	
	CCpwDoc* pDoc = GetDocument();
	if (!pDoc) return NULL;
	ASSERT_VALID(pDoc);
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	
	CListCtrl& ListCtrl = GetListCtrl();
	
	int iItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if(iItem == -1 || iItem > m_ArrOM.GetSize()) 
	{
		SetSelec (NULL, Nada);
		return NULL;
	}
	else
	{
		COrdMaq* pOM = (COrdMaq*) m_ArrOM.GetAt(iItem);
		SetSelec( pOM , Orden );
		return pOM;
	}

}    


void 
CInsView::SetSelec(CObject* p_pObjSel, enum EObjSel p_eObjSel)
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	pDoc->SetSelec(p_pObjSel, p_eObjSel);
	m_eObjSel = p_eObjSel;
	if (p_eObjSel == Orden)
	{
		COrdMaq* pOM =(COrdMaq*) p_pObjSel;
		ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
		m_pSelOM = pOM;
	} 
	else if (p_eObjSel == Maquina)
	{
		CProgMaq* pPM =(CProgMaq*) p_pObjSel;
		ASSERT( pPM->IsKindOf( RUNTIME_CLASS( CProgMaq ) ) );
		m_pSelPM = pPM;
	}
}


void CInsView::OnLButtonDown(UINT p_uFlags, CPoint p_Point) 
{
	
	CListViewEx::OnLButtonDown(p_uFlags, p_Point);

	COrdMaq* pOM = NULL;
	CProgMaq* pPM = NULL;

	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	

	CObject* pObject = ObjetoApuntado(p_Point);
	if (m_eObjSel == Nada)
	{
		pDoc->SetBMoviendo(FALSE);
		pProg->UnselectAll();	
		Invalidate();
		return;
	}
	// Si hemos apuntado a un objeto
	if (pDoc->GetBMoviendo())
	{
		pDoc->SelectOM(m_pSelOM);
		OnMoverOrden();
		return;
	} 
	// Si no estamos moviendo, seleccionamos
	if (m_eObjSel == Orden)
	{
		pOM = (COrdMaq*) pObject;
		ASSERT_VALID(pOM);
		ASSERT (pOM->IsKindOf(RUNTIME_CLASS ( COrdMaq )));
		
		if (p_uFlags & MK_SHIFT) 	// Shift
			pOM->SelecTo();
		else 	if (p_uFlags & MK_CONTROL) 	// Ctrl
			pOM->SetBSelected(!(pOM->GetBSelected()));
		else 
		{
			pProg->UnselectAll();			
			pOM->SetBSelected(!(pOM->GetBSelected()));
		}
		Invalidate();
	} 
	
	
}
