// OrdenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxdtctl.h"
#include "cpw.h"

#include "AbSpan.h"
#include "cpwExt.h"
#include "prog.h" 
#include "cpwDb.h"
#include "cpwDefs.h"
#include "cache.h"            
#include "ProgMaq.h"
#include "OrdMaq.h"    
#include "Dialogs.h"
#include "cpwView.h"
#include "TextView.h"
#include "Turnos.h"
#include "OrdenDlg.h"

#include <strstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static void AddData(CListCtrl& ctrl, int row, int col, const char* str)
{

	LVITEM lv;

	lv.iItem = row;
	lv.iSubItem = col;
	lv.pszText = (LPSTR)str;
	lv.mask = LVIF_TEXT;

	if (col == 0)
		ctrl.InsertItem(&lv);
	else
		ctrl.SetItem(&lv);

}
/////////////////////////////////////////////////////////////////////////////
// COrdenDlg dialog


COrdenDlg::COrdenDlg( COrdMaq* p_pOM, CProg* p_pProg, CWnd* p_pParent /*=NULL*/)
	: CDialog(COrdenDlg::IDD, p_pParent)
{
	ASSERT_VALID(p_pOM);   
	m_bInsertada = FALSE;
	m_bCancelar = FALSE;
	m_pOM = p_pOM;
	m_pProg = p_pProg;
	m_sCaption = "";
	m_pView = (CView*) p_pParent;
	if (m_pView->IsKindOf( RUNTIME_CLASS( CCpwView ) ))
	{
		m_bEntrada = FALSE;
		m_pCpwView = (CCpwView*) m_pView;
	} else
	{
		m_bEntrada = TRUE;
	}

	//{{AFX_DATA_INIT(COrdenDlg)
	m_sCantidad = _T("");
	m_sMaquinas = _T("");
	m_sTiempo = _T("");
	m_FecEnt = 0;
	m_BFecFija = FALSE;
	m_FecIni = 0;
	m_sCadencia = _T("");
	m_sFabricadas = _T("");
	m_FecFab = 0;
	m_sInactividad = _T("");
	m_sRestantes = _T("");
	m_BTiempoFijo = FALSE;
	m_BSelected = FALSE;
	m_sMaqMol = _T("");
	m_lLote = 0;
	m_sChatarras = _T("");
	m_sMolde = _T("");
	m_sInsMaq = _T("");
	m_sCavidades = _T("");
	m_sConsumo = _T("");
	m_sMezcla = _T("");
	m_sMezclaKilos = _T("");
	m_sPesoGramos = _T("");
	m_sRestCant = _T("");
	m_sRestHoras = _T("");
	m_sCantTot = _T("");
	m_sTiempoTot = _T("");
	m_FecFin = 0;
	m_sTiradas = _T("");
	m_sAcabado = _T("");
	m_sCadenciaAcabado = _T("");
	m_sPersonas = _T("");
	//}}AFX_DATA_INIT
}


void COrdenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrdenDlg)
	DDX_Control(pDX, IDC_PERSONAS, m_EditPersonas);
	DDX_Control(pDX, IDC_ACABADO, m_LstAcabado);
	DDX_Control(pDX, IDC_TIRADAS, m_EditTiradas);
	DDX_Control(pDX, IDC_FECFIN, m_DTctrlFecFin);
	DDX_Control(pDX, IDC_TIEMPO_TOT, m_EditTiempoFijo);
	DDX_Control(pDX, IDC_CANT_TOT, m_EditCantTot);
	DDX_Control(pDX, IDC_INSMAQ, m_ComboInsMaq);
	DDX_Control(pDX, IDC_CADENCIA, m_EditCadencia);
	DDX_Control(pDX, IDC_FINALIZAR, m_ButtonFinalizar);
	DDX_Control(pDX, IDC_BORRAR, m_ButtonBorrar);
	DDX_Control(pDX, IDC_AJUSTAR, m_ButtonAjustar);
	DDX_Control(pDX, IDC_MOLDE, m_ComboMolde);
	DDX_Control(pDX, IDC_MAQMOL, m_LstMaqMol);
	DDX_Control(pDX, IDC_TIEMPO, m_EditTiempo);
	DDX_Control(pDX, IDC_SELECTED, m_ButtonSelected);
	DDX_Control(pDX, IDC_CANTIDAD, m_EditCantidad);
	DDX_Control(pDX, IDC_TIEMPO_FIJO, m_ButtonTiempoFijo);
	DDX_Control(pDX, IDC_FECFAB, m_DTctrlFecFab);
	DDX_Control(pDX, IDC_FECINI, m_DTctrlFecIni);
	DDX_Control(pDX, IDC_FECHA_FIJA, m_ButtonFecFija);
	DDX_Control(pDX, IDC_FECENT, m_DTctrlFecEnt);
	DDX_Control(pDX, IDC_MAQUINAS, m_LstMaquinas);
	DDX_Text(pDX, IDC_CANTIDAD, m_sCantidad);
	DDX_LBString(pDX, IDC_MAQUINAS, m_sMaquinas);
	DDX_Text(pDX, IDC_TIEMPO, m_sTiempo);
	DDX_DateTimeCtrl(pDX, IDC_FECENT, m_FecEnt);
	DDX_Check(pDX, IDC_FECHA_FIJA, m_BFecFija);
	DDX_DateTimeCtrl(pDX, IDC_FECINI, m_FecIni);
	DDX_Text(pDX, IDC_CADENCIA, m_sCadencia);
	DDX_Text(pDX, IDC_FABRICADAS, m_sFabricadas);
	DDX_DateTimeCtrl(pDX, IDC_FECFAB, m_FecFab);
	DDX_Text(pDX, IDC_INACTIVIDAD, m_sInactividad);
	DDX_Text(pDX, IDC_RESTANTES, m_sRestantes);
	DDX_Check(pDX, IDC_TIEMPO_FIJO, m_BTiempoFijo);
	DDX_Check(pDX, IDC_SELECTED, m_BSelected);
	DDX_LBString(pDX, IDC_MAQMOL, m_sMaqMol);
	DDX_Text(pDX, IDC_LOTE, m_lLote);
	DDX_Text(pDX, IDC_CHATARRAS, m_sChatarras);
	DDX_CBString(pDX, IDC_MOLDE, m_sMolde);
	DDX_CBString(pDX, IDC_INSMAQ, m_sInsMaq);
	DDX_Text(pDX, IDC_CAVIDADES, m_sCavidades);
	DDX_Text(pDX, IDC_CONSUMO, m_sConsumo);
	DDX_Text(pDX, IDC_MEZCLA, m_sMezcla);
	DDX_Text(pDX, IDC_MEZCLAKILOS, m_sMezclaKilos);
	DDX_Text(pDX, IDC_PESOGRAMOS, m_sPesoGramos);
	DDX_Text(pDX, IDC_RESTCANT, m_sRestCant);
	DDX_Text(pDX, IDC_RESTHORAS, m_sRestHoras);
	DDX_Text(pDX, IDC_CANT_TOT, m_sCantTot);
	DDX_Text(pDX, IDC_TIEMPO_TOT, m_sTiempoTot);
	DDX_DateTimeCtrl(pDX, IDC_FECFIN, m_FecFin);
	DDX_Text(pDX, IDC_TIRADAS, m_sTiradas);
	DDX_LBString(pDX, IDC_ACABADO, m_sAcabado);
	DDX_Text(pDX, IDC_CADENCIA_ACABADO, m_sCadenciaAcabado);
	DDX_Text(pDX, IDC_PERSONAS, m_sPersonas);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MOLDE_DETALLE, m_LstMoldeDetalle);
	DDX_Control(pDX, IDC_LISTCTRL_COMPONENTES, m_ListCtrl_Componentes);
}


BEGIN_MESSAGE_MAP(COrdenDlg, CDialog)
	//{{AFX_MSG_MAP(COrdenDlg)
	ON_BN_CLICKED(IDC_BORRAR, OnBorrar)
	ON_LBN_DBLCLK(IDC_MAQUINAS, OnDblclkMaquinas)
	ON_LBN_SELCHANGE(IDC_MAQUINAS, OnSelchangeMaquinas)
	ON_LBN_DBLCLK(IDC_MAQMOL, OnDblclkMaqmol)
	ON_BN_CLICKED(IDC_FINALIZAR, OnFinalizar)
	ON_BN_CLICKED(IDC_AJUSTAR, OnAjustar)
	ON_CBN_SELCHANGE(IDC_INSMAQ, OnSelchangeInsmaq)
	ON_BN_CLICKED(IDC_FABRICADA, OnFabricada)
	ON_BN_CLICKED(IDC_AJUSTAR_A_MEZCLA, OnAjustarAMezcla)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_CBN_SELENDOK(IDC_MOLDE, &COrdenDlg::OnCbnSelchangeMolde)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrdenDlg message handlers

BOOL COrdenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ASSERT_VALID(m_pOM);
	
	m_ListCtrl_Componentes.InsertColumn(0, "Componente");
	m_ListCtrl_Componentes.SetColumnWidth(0, 100);
	m_ListCtrl_Componentes.InsertColumn(1, "Cant.", LVCFMT_RIGHT);
	m_ListCtrl_Componentes.SetColumnWidth(1, 50);
	m_ListCtrl_Componentes.InsertColumn(2, "Stock", LVCFMT_RIGHT);
	m_ListCtrl_Componentes.SetColumnWidth(2, 80);


	// Si no esta ya hacemos una lista de maquinas a las que puede ir
	g_pCache->GetArrMaq(m_pOM->GetsID(), m_ArrListaMaq, m_ArrListaMoldes);
	int iLimListaMaq = m_ArrListaMaq.GetSize();
	CString sMolAnt = "";
	BOOL bRep = FALSE;
	for(int ix = 0; ix < iLimListaMaq; ix++)
	{
		CBufMaquina* pBufMaquina = (CBufMaquina*) m_ArrListaMaq.GetAt(ix);
		CBufMolde* pBufMolde = (CBufMolde*) m_ArrListaMoldes.GetAt(ix);
		//ASSERT_VALID(pBufMaquina);
		CProgMaq* pPM = m_pProg->FindMaq(pBufMaquina->m_sID);
		CString sLin;
		if (sMolAnt == pBufMolde->m_sID ) bRep = TRUE;
		else bRep = FALSE;
		sLin.Format("%5.5s %s", pBufMaquina->m_sID, ( bRep ? "" : pBufMolde->m_sID));
		sMolAnt = pBufMolde->m_sID;
		m_LstMaqMol.AddString(sLin);
		// Tambi�n a�adimos los moldes al combo de moldes
		if (!bRep) m_ComboMolde.AddString(pBufMolde->m_sID);
	}
	// Hacemos una lista de todas las m�quina de inyeccion, para que inserte si no le valen las otras
	m_pProg->GetArrPM( m_ArrTodasMaq , TK_GFH_INYECCION);
	for (int i=0; i<m_ArrTodasMaq.GetSize(); i++)
	{                           
		CProgMaq* pPM = (CProgMaq*) m_ArrTodasMaq.GetAt(i);
		//ASSERT_VALID(pPM);           
		m_ComboInsMaq.AddString(pPM->GetsID());
	}               
	
	m_pProg->GetArrOMArt( &m_ArrOM , m_pOM->GetsID());
	
	for (int i=0; i<m_ArrOM.GetSize(); i++)
	{                           
		COrdMaq* pOM = (COrdMaq*) m_ArrOM.GetAt(i);
		//ASSERT_VALID(pOM);           
		m_LstMaquinas.AddString(pOM->GetsInfo());
	}               
	
	g_pCache->GetArrComponentes(m_pOM->GetsID(), m_ArrComponentes);

	for (int i = 0; i < m_ArrComponentes.GetSize(); i++)
	{
		CBufComponentes* pBufAc = (CBufComponentes*)m_ArrComponentes.GetAt(i);
		AddData(m_ListCtrl_Componentes, i, 0, pBufAc->m_sComponente);
		AddData(m_ListCtrl_Componentes, i, 1, FormatLong((long) pBufAc->m_dCantidad, 7));
		AddData(m_ListCtrl_Componentes, i, 2, FormatLong((long) pBufAc->m_dStock, 12));
	}
	
	RefreshMoldeDetalle();

	g_pCache->GetArrAcabado (m_pOM->GetsID(), m_ArrAcabado);
	
	for (int i=0; i<m_ArrAcabado.GetSize(); i++)
	{                           
		CBufAcabado* pBufAc = (CBufAcabado*) m_ArrAcabado.GetAt(i);
		m_LstAcabado.AddString(pBufAc->GetsInfo(m_pOM->GetlCantidad()));
	}               
	
	m_DTctrlFecIni.SetFormat("dd/MM/yy H:mm");
	m_DTctrlFecFin.SetFormat("dd/MM/yy H:mm");
	if (!m_bEntrada)
	{
		m_sCaption = " DATOS OFERTA " + m_pOM->GetsID() + ' ';
		if (m_pOM->GetpPM() != NULL)
		{
			m_sCaption += " en " + m_pOM->GetpPM()->GetsNombre();
		}
	}
	else
	{
		m_sCaption = "PROGRAMANDO OFERTA " + m_pOM->GetsID();
	}
	
	SetWindowText(m_sCaption);
	
	m_sMolde = m_pOM->GetsMolde();
	m_sInsMaq = m_pOM->GetsMaquina();
	

	m_ComboMolde.SetWindowText(m_sMolde);
	m_ComboInsMaq.SetWindowText(m_sInsMaq);
	//m_ButtonAjustar.SetWindowText(FormatLong(m_pOM->CalcCantLote(),7));
	
	if (m_bEntrada)
	{
		m_ButtonBorrar.EnableWindow(FALSE);
		m_ButtonFinalizar.EnableWindow(TRUE);
	}
	else
	{
		m_ButtonBorrar.EnableWindow(TRUE);
		m_ButtonFinalizar.EnableWindow(FALSE);
	}
	// Si el documento es Read Only, no permitiremos modificar la BD
	
	if (m_BReadOnly)
	{
		m_EditCantidad.SetReadOnly();
		m_DTctrlFecEnt.EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COrdenDlg::RefreshMoldeDetalle(CString p_sMoldeAlt)
{
	m_ArrMoldeDetalle.RemoveAll();
	CBufMolde* pBufMolde = NULL;
	if (p_sMoldeAlt == "") {
		pBufMolde = m_pOM->GetpBufMolde();
	}
	else {
		pBufMolde = g_pCache->FindpMolde(p_sMoldeAlt);
	}
	if (pBufMolde) {
		CObArray* pArr = pBufMolde->m_pArrMolElem;
		if (pArr) {
			CString str = "";
			for (int i = 0; i < pArr->GetSize(); i++)
			{
				CBufMoldesElem* pBufMI = (CBufMoldesElem*)pArr->GetAt(i);
				m_ArrMoldeDetalle.Add(pBufMI->m_sTipo + _T(" ") + pBufMI->m_sNombre);
			}
		}
	}
	m_LstMoldeDetalle.ResetContent();
	for (int i = 0; i < m_ArrMoldeDetalle.GetSize(); i++)
	{
		m_LstMoldeDetalle.AddString(m_ArrMoldeDetalle.GetAt(i));
	}

}

void COrdenDlg::OnBorrar() 
{
	CString sMsg = STRID( IDS_CONF_BORRORD );
	CString sCap = STRID( IDS_CONF_CAPTION );
	if (MessageBox( sMsg, sCap, MB_OKCANCEL ) != IDOK ) return;


	m_pProg->DeleteOM(m_pOM);

	CWaitCursor();
	m_pProg->Recalc();
	
	EndDialog(IDOK);	
}

void COrdenDlg::OnFabricada() 
{
	CString sMsg = "� Borrar �sta orden y a�adirla al hist�rico ?";
	CString sCap = STRID( IDS_CONF_CAPTION );
	if (MessageBox( sMsg, sCap, MB_OKCANCEL ) != IDOK ) return;

	CObArray DelArray;                                             
	
	CProgMaq* pPM = m_pOM->GetpPM();
	ASSERT_VALID(pPM);
//	CObArray ArrOM;
//	if ( m_pProg->GetArrOM( &ArrOM , m_pOM->GetpOF()) == 0)
//		TRACE("Error en COrdenDlg::OnInitDialog, no hay OMs asociadas"); 

	COrdMaq* pOM = m_pOM;
	ASSERT_VALID(pOM); 

	// La a�adimos al hist�rico
	g_pCache->AddHistorico(pOM);
	// Y la borramos
	m_pProg->DeleteOM(pOM);
	
	CWaitCursor();
	m_pProg->Recalc();

	EndDialog(IDOK);	
}

void COrdenDlg::OnOK() 
{
	ActOM();		// Actualizamos la OM
	CDialog::OnOK();
}

void COrdenDlg::OnDblclkMaquinas() 
{
	ASSERT_VALID(m_pOM);
	
	CString sSelection;
	int iSel = m_LstMaquinas.GetCurSel();
	if (iSel == LB_ERR || iSel >= m_ArrOM.GetSize()) return;
	
	
	COrdMaq* pOMenPM = (COrdMaq*) m_ArrOM.GetAt(iSel);
	ActOM();
	if (!m_bEntrada)
	{
		m_pProg->UnselectAll();			
	
		
		CRect RectOM = m_pCpwView->GetRect(pOMenPM);
		RectOM.left += m_pCpwView->m_SepWidth;
		CRect RectWnd;
		m_pCpwView->GetClientRect(&RectWnd);
		CFrameWnd* pFrame = GetParentFrame();
		if (!pFrame) return;
		
		CPoint PointDesp;
		PointDesp.x = RectOM.left - TK_POINTS_COL;
		if (PointDesp.x < 0) PointDesp.x = 0;
		m_pCpwView->SetScrollPos(SB_HORZ, PointDesp.x);
		PointDesp.y = RectOM.top - m_pCpwView->m_HeadHeight;
		if (PointDesp.y < 0) PointDesp.y = 0;
		m_pCpwView->SetScrollPos(SB_VERT, PointDesp.y);
		
		PointDesp = m_pCpwView->GetDeviceScrollPosition();
		CRect RectWin;
		m_pCpwView->GetWindowRect(&RectWin);
		CRect RectCursorPos = RectOM - PointDesp + CPoint(RectWin.left, RectWin.top);
		SetCursorPos(pOMenPM->GetBFija() ? RectCursorPos.left - m_pCpwView->m_SepWidth + 3 : RectCursorPos.left,
			RectCursorPos.top + 3	);
		
		PostMessage(WM_CLOSE);
	}
	else // Es una entrada de OM
	{
		// Como es una entrada de �rdenes, quiere decir que lo queremos a�adir
		// a la ya existente. Pedimos confirmaci�n.
		CString sMsg;
		sMsg.Format(STRID(IDS_CONF_ADDCANT), m_pOM->GetlCantidad() );
		if (MessageBox(sMsg, STRID(IDS_CONF_CAPTION), MB_OKCANCEL) != IDOK) return;
		
		pOMenPM->ModifCant(pOMenPM->GetlCantidad() + m_pOM->GetlCantidad());
		pOMenPM->RecalcFin();
		PostMessage(WM_CLOSE);

	}
	
}

void COrdenDlg::OnSelchangeMaquinas() 
{
	ASSERT_VALID(m_pOM);
	
	CString sSelection;
	int iSel = m_LstMaquinas.GetCurSel();
	if (iSel == LB_ERR || iSel >= m_ArrOM.GetSize()) return;
	
	
		
	// Buscamos la m�quina por ID
	COrdMaq* pOMenPM = (COrdMaq*) m_ArrOM.GetAt(iSel);
	if (pOMenPM == NULL)
	{
		CString sMsg = STRID(IDS_ERR_NOOMENPM );
		MessageBox(sMsg, "Error");
		return;
	}
	CProgMaq* pPM = pOMenPM->GetpPM();
	if (pPM == NULL)
	{
		CString sMsg = STRID( IDS_ERR_NOMAQID );
		MessageBox(sMsg, STRID( IDS_ERROR ));
		return;
	}
	// Ahora ya tenemos la orden, rellenamos los datos del di�logo con los valores nuevos
	// Si es que estamos en consulta
	if (!m_bEntrada)
	{
		m_pCpwView = (CCpwView*) m_pView;

		m_FecFin = pOMenPM->GetFecFin() ;
		m_FecIni = pOMenPM->GetFecIni() ;
		
		m_sTiempo = FormatFec( pOMenPM->GetTimeSpan() );
		m_BFecFija = pOMenPM->GetBFija();
		m_sCadencia.Format("%.0lf", pOMenPM->GetdCadenciaGFH());
		m_sPersonas.Format("%.0lf", pOMenPM->GetdPersonas());
		m_BTiempoFijo = pOMenPM->GetBTiempoFijo();
		m_BSelected = pOMenPM->GetBSelected();
		if (pOMenPM->GetlCantFab())
		{
			m_sFabricadas = FormatLong(pOMenPM->GetlCantFab(), 6);
			m_FecFab = pOMenPM->GetFecFab();
			m_sRestantes = FormatLong(pOMenPM->GetlCantidad(), 6);
		} else {
			m_sFabricadas = "";
			m_FecFab = CTime::GetCurrentTime();
			m_sRestantes = "";
		}
		m_sCantidad = FormatLong(pOMenPM->GetlCantidad(), 6);
		m_sInactividad.Format("%.0lf %%", pOMenPM->GetdInactividad());
		
		m_sCaption = " DATOS OFERTA " + m_pOM->GetsID() + ' ';
		if (m_pOM->GetpPM() != NULL)
		{
			m_sCaption += " en " + m_pOM->GetpPM()->GetsNombre();
		}
		
		// Ahora tenemos que hacerla la seleccionada
		m_pProg->UnselectAll();			
		m_pCpwView->m_pSelOM = pOMenPM;			
		m_pCpwView->Invalidate();
		UpdateData(FALSE);
		SetWindowText(m_sCaption);
	}

}

void 
COrdenDlg::OnDblclkMaqmol() 
{
	ASSERT_VALID(m_pOM);
	//Obtenemos el �ndice seleccionado
	CString sSelection;
	int iSel = m_LstMaqMol.GetCurSel();
	if (iSel == LB_ERR || iSel >= m_ArrListaMaq.GetSize()) return;
	//Obtenemos el molde y la m�quina del �ndice seleccionado
	CProgMaq* pPM = NULL;
	CBufMaquina* pBufMaq = (CBufMaquina*) m_ArrListaMaq.GetAt(iSel);
	CBufMolde* pBufMolde = (CBufMolde*) m_ArrListaMoldes.GetAt(iSel);
	// Buscamos la m�quina entre las cargadas
	int iLim = m_pProg->GetNumPM();
	for (int i = 0; i < iLim; i++)
	{
		CProgMaq* pPMi = m_pProg->GetpPM(i);
		if (pBufMaq->m_sID == pPMi->GetsID())
		{
			pPM = pPMi;
			break;
		}
	}
	// Si no existe, salimos
	if (pPM == NULL) return;
	
	COrdMaq* pOMLink = NULL;
	if (!m_bEntrada)
	{
		if (pPM == m_pOM->GetpPM()) return;
		m_pProg->MoveOrdMaq(pPM, m_pOM, pPM->GetNumOM());
	}
	else
	{
		ActOM();
		if (MessageBox(STRID(IDS_CONF_ADDMAQ) + pPM->GetsID(), STRID(IDS_CONF_CAPTION), MB_OKCANCEL)
			!= IDOK) return;
		m_pOM->SetpBufMolde(pBufMolde);
		if (m_pOM->GetsID() == "02777311" || m_pOM->GetsID() == "02777321")
		{
			CString sMsg = "� Esta pieza comparte Molde con otra, programar las dos ?";
			if (AfxMessageBox(sMsg , MB_YESNO)  == IDYES)	m_pOM->SetBNextLink(TRUE);
		}
		
		pPM->Add(m_pOM);
		if (m_pOM->GetBNextLink())
		{
			pOMLink = new COrdMaq();
			*pOMLink = *m_pOM;
			CBufArticulo* pBufArt = g_pCache->FindpArticulo(m_pOM->GetsID() == "02777311" ? "02777321" : "02777311");
			if (pBufArt)
			{
				pOMLink->SetpBufArt(pBufArt);
				pOMLink->SetBPrevLink(TRUE);
				pPM->Add(pOMLink);
			}
		}
		if (m_pOM->GetdCadenciaGFH() > 0) m_pOM->RecalcFin();
		else {
			CString sMsg =  "La referencia " + m_pOM->GetsID() + " no tiene metidas Piezas/Hora, no se puede programar.";
			MessageBox( sMsg, "Aviso", MB_OK);
			return;
		}

		CBufArticulo* pBufArt = m_pOM->GetpBufArticulo();
		if (!pBufArt) return;
		if (pBufArt->m_pArrArtMol->GetSize() > 1) // Si s�lo hay un molde no tenemos m�s opciones
		{
			// Si no, miramos si hay algun otro molde mejor
			// Lo mejor es un molde que ya se haya usado hace poco en la m�quina
			BOOL bMejorMolde = FALSE;
			CObArray ArrMoldesUsados;
			pPM->GetArrMoldesYaUsados(m_pOM->GetsID(), ArrMoldesUsados);
			// Lo recorremos al rev�s porque es mejor usar el molde cuanto m�s cerca est� del final
			for (int iMU=ArrMoldesUsados.GetSize()-1;  iMU >= 0; iMU--)
			{
				CBufMolde* pBufMoldeUsado = (CBufMolde*) ArrMoldesUsados.GetAt(iMU);
				m_pOM->SetpBufMolde(pBufMoldeUsado);
				m_pOM->RecalcFin();
				m_pProg->UpdateArrTL(); // Para ver si hay conflictos de moldes
				if (m_pOM->GetbConfMolde()) continue; // Si lo hay, probamos con otro
				else
				{
					bMejorMolde = TRUE;
					break;
				}
			}
			// Si no hemos encontrado un mejor molde, seguimos buscando, ahora por todos
			// los posibles moldes
			CObArray ArrMoldesEnMaq;
			if (!bMejorMolde)
			{
				g_pCache->GetArrMolMaq(pPM->GetsID(), ArrMoldesEnMaq, &(m_pOM->GetsID()));
				for (int iMM=0; iMM < ArrMoldesEnMaq.GetSize(); iMM++)
				{
					CBufMolMaq* pBufMoldeEnMaq = (CBufMolMaq*) ArrMoldesEnMaq.GetAt(iMM);
					if (pBufMoldeEnMaq->IsKindOf( RUNTIME_CLASS( CBufMolMaq ) ))

					m_pOM->SetpBufMolde(pBufMoldeEnMaq->m_pBufMolde);
					m_pOM->RecalcFin();
					m_pProg->UpdateArrTL(); // Para ver si hay conflictos de moldes
					if (m_pOM->GetbConfMolde()) continue; // Si lo hay, probamos con otro
					else
					{
						bMejorMolde = TRUE;
						break;
					}
				}
				
			}		
			if (!bMejorMolde)	// Si no hay un mejor molde, cogemos el seleccionado
			{
				m_pOM->SetpBufMolde(pBufMolde);
				m_pOM->RecalcFin();			
			}
		}
		if (m_pOM->GetBNextLink())
		{
			pOMLink->SetpBufMolde(m_pOM->GetpBufMolde());
			pOMLink->RecalcFin();
		}
		m_bInsertada = TRUE;
		PostMessage(WM_CLOSE);	
	}
}	
	

void COrdenDlg::OnFinalizar() 
{
	m_bCancelar = TRUE;	
	PostMessage(WM_CLOSE);
}

void COrdenDlg::OnAjustar() 
{
	/*
	CString sCant;
	sCant.Format("%ld", m_pOM->CalcCantLote());
	m_EditCantidad.SetWindowText(sCant);
	*/
	CDlgAjustar DlgAjustar;
	DlgAjustar.m_pOrdenDlg = this;
	DlgAjustar.m_sCantAct = m_sCantidad;
	DlgAjustar.m_sCadAnt = m_sCadencia;
	DlgAjustar.m_sExistencias = FormatLong(m_pOM->GetlCantMezcla(),7);
	CTimeSpan tsOM = m_pOM->GetTimeSpan();
	// Lo convertimos a horas * 100, esto es porque necesitamos enteros para que funcionen los spinners
	DlgAjustar.m_lHoras = (long) ((double) tsOM.GetTotalMinutes() / .6); 
	DlgAjustar.m_lRatio = (long) (((double) m_pOM->GetlCantidad() * m_pOM->GetdPeso()) / (10 * m_pOM->GetlLote())); 
	DlgAjustar.m_lCadNue = (long) (m_pOM->GetdCadencia() * 100);
											
	DlgAjustar.m_sLote = FormatLong(m_lLote,5);
	DlgAjustar.m_sMaterial.Format("%.1f", (double) (m_pOM->GetlCantidad() * m_pOM->GetdPeso()) / 1000);
	
	if (DlgAjustar.DoModal() == IDOK)
	{
	}
}

void COrdenDlg::OnSelchangeInsmaq() 
{
	
	ASSERT_VALID(m_pOM);
	
	int iSel = m_ComboInsMaq.GetCurSel();
	if (iSel == LB_ERR || iSel >= m_ArrTodasMaq.GetSize()) return;
	
	CProgMaq* pPM = (CProgMaq*) m_ArrTodasMaq.GetAt(iSel);
	if (pPM == NULL) return;
/*	
	if (m_pOM->LimitMaquina(pPM->GetsID()))
	{
		CString sMsg = STRID( IDS_WAR_MAQNOP );
		CString sCap = STRID( IDS_WARNING );
		
		if (MessageBox(sMsg, sCap, MB_OKCANCEL)  != IDOK)
			return;
		g_pCache->ActLimitMaq(m_pOM->GetsMolde(), pPM->GetsID());
	}
*/
	ActOM();	// Actualizamos la OrdMaq
	if (!m_bEntrada)
	{
		if (pPM == m_pOM->GetpPM()) return;
		m_pProg->MoveOrdMaq(pPM, m_pOM, pPM->GetNumOM());
		PostMessage(WM_CLOSE);
		
	}
	else
	{
		//m_pOM->SetpBufMolde(pBufMolde);
		pPM->Add(m_pOM);
		m_pOM->RecalcFin();
		m_bInsertada = TRUE;
		PostMessage(WM_CLOSE);

	}

	
}
/////////////////////////////////////////////////////////////////////////////
// CDlgAjustar dialog

	static BOOL bCut = FALSE;

CDlgAjustar::CDlgAjustar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAjustar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAjustar)
	m_sCadAnt = _T("");
	m_sCantAct = _T("");
	m_sCantNue = _T("");
	m_sExistencias = _T("");
	m_sMaterial = _T("");
	m_sLote = _T("");
	m_lCadNue = 0;
	m_lHoras = 0;
	m_lRatio = 0;
	//}}AFX_DATA_INIT
	m_UdAcRatio.nSec = 1;
	m_UdAcRatio.nInc = 10;
	m_UdAcHoras.nSec = 1;
	m_UdAcHoras.nInc = 10;
	m_UdAcCad.nSec = 1;
	m_UdAcCad.nInc = 10;

	m_pOrdenDlg = NULL; // Habr� de ser rellenado antes de OnInitDialog()
}


void CDlgAjustar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAjustar)
	DDX_Control(pDX, IDC_LOTE, m_EditLote);
	DDX_Control(pDX, IDC_SPIN_RATIO, m_SpinRatio);
	DDX_Control(pDX, IDC_SPIN_HORAS, m_SpinHoras);
	DDX_Control(pDX, IDC_SPIN_CAD, m_SpinCad);
	DDX_Control(pDX, IDC_RATIO, m_EditRatio);
	DDX_Control(pDX, IDC_MATERIAL, m_EditMaterial);
	DDX_Control(pDX, IDC_HORAS, m_EditHoras);
	DDX_Control(pDX, IDC_EXISTENCIAS, m_EditExistencias);
	DDX_Control(pDX, IDC_CANT_NUE, m_EditCantNue);
	DDX_Control(pDX, IDC_CANT_ACT, m_EditCantAct);
	DDX_Control(pDX, IDC_CAD_NUE, m_EditCadNue);
	DDX_Control(pDX, IDC_CAD_ANT, m_EditCadAnt);
	DDX_Text(pDX, IDC_CAD_ANT, m_sCadAnt);
	DDX_Text(pDX, IDC_CANT_ACT, m_sCantAct);
	DDX_Text(pDX, IDC_CANT_NUE, m_sCantNue);
	DDX_Text(pDX, IDC_EXISTENCIAS, m_sExistencias);
	DDX_Text(pDX, IDC_MATERIAL, m_sMaterial);
	DDX_Text(pDX, IDC_LOTE, m_sLote);
	DDX_Text(pDX, IDC_CAD_NUE, m_lCadNue);
	DDX_Text(pDX, IDC_HORAS, m_lHoras);
	DDX_Text(pDX, IDC_RATIO, m_lRatio);
	//}}AFX_DATA_MAP
	CString sText;
	sText = FormatLong(m_lHoras,8, Real2Dec);
	SetDlgItemText(IDC_HORAS_D, sText);
	sText = FormatLong(m_lCadNue,8, Real2Dec);
	SetDlgItemText(IDC_CAD_D, sText);
	sText = FormatLong(m_lRatio,8, Real2Dec);
	SetDlgItemText(IDC_RATIO_D, sText);

}


BEGIN_MESSAGE_MAP(CDlgAjustar, CDialog)
	//{{AFX_MSG_MAP(CDlgAjustar)
	ON_EN_CHANGE(IDC_RATIO, OnChangeRatio)
	ON_EN_CHANGE(IDC_HORAS, OnChangeHoras)
	ON_EN_CHANGE(IDC_CAD_NUE, OnChangeCadNue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAjustar message handlers

BOOL CDlgAjustar::OnInitDialog() 
{
	CDialog::OnInitDialog();


	m_SpinRatio.SetRange32(0,99999);
	m_SpinRatio.SetAccel(1, &m_UdAcRatio);

	m_SpinCad.SetRange32(0,99999);
	m_SpinCad.SetAccel(1, &m_UdAcCad);
	
	m_SpinHoras.SetRange32(0,999999);
	m_SpinHoras.SetAccel(1, &m_UdAcHoras);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAjustar::OnChangeRatio() 
{
	long lRatio = atol(GetsDlgText(IDC_RATIO, this));
	
	CString sText;
	sText = FormatLong(lRatio,8, Real2Dec);
	SetDlgItemText(IDC_RATIO_D, sText);



}

void CDlgAjustar::OnChangeHoras() 
{
	long lHoras = atol(GetsDlgText(IDC_HORAS, this));
	
	CString sText;
	sText = FormatLong(lHoras,8, Real2Dec);
	bCut = TRUE;
	SetDlgItemText(IDC_HORAS_D, sText);

	
}

void CDlgAjustar::OnChangeCadNue() 
{
	long lCadencia = atol(GetsDlgText(IDC_CAD_NUE, this));
	
	CString sText;
	sText = FormatLong(lCadencia,8, Real2Dec);
	bCut = TRUE;
	SetDlgItemText(IDC_CAD_D, sText);

	if (m_pOrdenDlg)
	{
		m_OM = *(m_pOrdenDlg->m_pOM);	// Hacemos una copia de la OM para los c�lculos
		m_OM.SetdCadencia((double) lCadencia / 100);
		m_OM.CalcTimeSpan();
		sText.Format("%ld", (long) (m_OM.GetTimeSpan().GetTotalMinutes() / .6));
		SetDlgItemText(IDC_HORAS, sText);
	}
}
// Actualiza la OrdMaq con los datos del Di�logo
void COrdenDlg::ActOM()
{

	const int BufLen = 100;
	CTime FecEnt;    
	CTime FecIni;
	CTime FecFin;
	
	CDlgValidarCambio DlgVal;
	BOOL bModif = FALSE;
	char Buffer[BufLen];
	GetDlgItemText(IDC_CANTIDAD, Buffer, BufLen);
	if (m_sCantidad != Buffer)
	{
		DlgVal.m_sValAnt = m_sCantidad;
		DlgVal.m_sValNue = Buffer; 
		DlgVal.m_sCampo = "Cantidad";
		//if (DlgVal.DoModal() == IDOK)
		{
			bModif = TRUE;
			m_pOM->ModifCant(Buffer);
			m_pOM->SyncCant();	// Sincroniza con las posibles linkadas
		}
	}	

	GetDlgItemText(IDC_CANT_TOT, Buffer, BufLen);
	if (m_sCantTot != Buffer)
	{
		DlgVal.m_sValAnt = m_sCantTot;
		DlgVal.m_sValNue = Buffer; 
		DlgVal.m_sCampo = "Cantidad Con Chatarras";
		//if (DlgVal.DoModal() == IDOK)
		{
			bModif = TRUE;
			m_pOM->ModifCantChatarras(Buffer);
			m_pOM->SyncCant();	// Sincroniza con las posibles linkadas
		}
	}	

	GetDlgItemText(IDC_TIRADAS, Buffer, BufLen);
	if (m_sTiradas != Buffer && (m_pOM->GetlCavidades() > 0))
	{
		DlgVal.m_sValAnt = m_sTiradas;
		DlgVal.m_sValNue = Buffer; 
		DlgVal.m_sCampo = "Tiradas";
		//if (DlgVal.DoModal() == IDOK)
		{
			bModif = TRUE;
			m_pOM->ModifCantTiradas(Buffer);
			m_pOM->SyncCant();	// Sincroniza con las posibles linkadas
		}
	}	

	GetDlgItemText(IDC_CADENCIA, Buffer, BufLen);
	if (m_sCadencia != Buffer)
	{
		DlgVal.m_sValAnt = m_sCadencia;
		DlgVal.m_sValNue = Buffer; 
		DlgVal.m_sCampo = "Cadencia";
		//if (DlgVal.DoModal() == IDOK)
		{
			bModif = TRUE;
			double dCadencia = atof( Buffer );
			if (dCadencia == 0)
			{
				AfxMessageBox(STRID(IDS_ERR_CANTNOVAL ));
			}
			else
			{
				m_pOM->SetdCadencia(dCadencia);
			}
		}
	}	

	GetDlgItemText(IDC_PERSONAS, Buffer, BufLen);
	if (m_sPersonas != Buffer)
	{
		DlgVal.m_sValAnt = m_sPersonas;
		DlgVal.m_sValNue = Buffer; 
		DlgVal.m_sCampo = "Personas";
		//if (DlgVal.DoModal() == IDOK)
		{
			bModif = TRUE;
			double dPersonas = atof( Buffer );
			if (dPersonas == 0)
			{
				AfxMessageBox(STRID(IDS_ERR_CANTNOVAL ));
			}
			else
			{
				m_pOM->SetdPersonas(dPersonas);
			}
		}
	}	

	GetDlgItemText(IDC_CHATARRAS, Buffer, BufLen);
	if (m_sChatarras != Buffer)
	{
		DlgVal.m_sValAnt = m_sChatarras;
		DlgVal.m_sValNue = Buffer; 
		DlgVal.m_sCampo = "Chatarras";
		//if (DlgVal.DoModal() == IDOK)
		{
			bModif = TRUE;
			double dChatarras = atof( Buffer );
			if (dChatarras == 0)
			{
				AfxMessageBox(STRID(IDS_ERR_CANTNOVAL ));
			}
			else
			{
				m_pOM->SetdChatarras(dChatarras);
			}
		}
	}	

	GetDlgItemText(IDC_MOLDE, Buffer, BufLen);
	if (m_sMolde != Buffer)
	{
		DlgVal.m_sValAnt = m_sMolde;
		DlgVal.m_sValNue = Buffer; 
		DlgVal.m_sCampo = "Molde";
		//if (DlgVal.DoModal() == IDOK)
		{
			bModif = TRUE;
			if (!m_pOM->ModifMolde(Buffer))
			{
				MessageBox(STRID( IDS_ERR_MOLNOTFOUND ) + Buffer , STRID(IDS_ERROR));
			}
			else {
				RefreshMoldeDetalle();
			}
		}
	}	

	GetDlgItemText(IDC_TIEMPO, Buffer, BufLen);
	if (m_sTiempo != Buffer)
	{
		DlgVal.m_sValAnt = m_sTiempo;
		DlgVal.m_sValNue = Buffer; 
		DlgVal.m_sCampo = "Tiempo";
		//if (DlgVal.DoModal() == IDOK)
		{
			bModif = TRUE;
			//m_pOM->SetBTiempoFijo(TRUE);
			m_pOM->ModifTimeSpan(Buffer);
			//m_ButtonTiempoFijo.SetCheck(1);
		}
	}	
	
	GetDlgItemText(IDC_TIEMPO_TOT, Buffer, BufLen);
	if (m_sTiempoTot != Buffer)
	{
		DlgVal.m_sValAnt = m_sTiempoTot;
		DlgVal.m_sValNue = Buffer; 
		DlgVal.m_sCampo = "Tiempo Con Inactividad)";
		//if (DlgVal.DoModal() == IDOK)
		{
			bModif = TRUE;
			m_pOM->ModifTimeSpanInact(Buffer);
			//m_pOM->SetBTiempoFijo(TRUE);
			//m_ButtonTiempoFijo.SetCheck(1);
			m_pOM->SyncCant();	// Sincroniza con las posibles linkadas
		}
	}	
	
	m_DTctrlFecEnt.GetTime(FecEnt);
	if (m_FecEnt != FecEnt)
	{
		bModif = TRUE;
		DlgVal.m_sValAnt = FormatFec(m_FecEnt, Fecha);
		DlgVal.m_sValNue = FormatFec(FecEnt, Fecha); 
		DlgVal.m_sCampo = "Fecha de Entrega";
		//if (DlgVal.DoModal() == IDOK) 
		{
			bModif = TRUE;
			m_pOM->ModifFecEnt(FecEnt);
		}
	}
	m_DTctrlFecIni.GetTime(FecIni);
	if (m_FecIni != FecIni)
	{
		bModif = TRUE;
		DlgVal.m_sValAnt = FormatFec(m_FecIni, FecHoraSec);
		DlgVal.m_sValNue = FormatFec(FecIni, FecHoraSec); 
		DlgVal.m_sCampo = "Fecha de Inicio";
		//if (DlgVal.DoModal() == IDOK) 
		{
			bModif = TRUE;
			m_pOM->SetFecIni(FecIni);
			m_pOM->SetBFija(TRUE);
		}
	}
	
	m_DTctrlFecFin.GetTime(FecFin);
	if (m_FecFin != FecFin)
	{
		bModif = TRUE;
		DlgVal.m_sValAnt = FormatFec(m_FecFin, FecHoraSec);
		DlgVal.m_sValNue = FormatFec(FecFin, FecHoraSec); 
		DlgVal.m_sCampo = "Fecha de Fin";
		//if (DlgVal.DoModal() == IDOK) 
		{
			bModif = TRUE;
			CProgMaq* pPM = m_pOM->GetpPM();
			CTimeSpan tsInt;
			if (pPM)
				tsInt = pPM->DiffCal(FecFin, m_pOM->GetFecIni());
			else
				tsInt = FecFin - m_pOM->GetFecIni();
			m_pOM->ModifTimeSpanInact((long) tsInt.GetTotalSeconds());
			m_pOM->SyncCant();	// Sincroniza con las posibles linkadas

			//m_pOM->SetBTiempoFijo(TRUE);
			//m_ButtonTiempoFijo.SetCheck(1);
		}
	}
	
	CTime FecFab;
	GetDlgItemText(IDC_FABRICADAS, Buffer, BufLen);
	m_DTctrlFecFab.GetTime(FecFab);
	if (m_sFabricadas != Buffer || FecFab != m_FecFab)
	{
		//FecFab = CTime(FecFab.GetYear(), FecFab.GetMonth(), FecFab.GetDay(), TK_TURNO3_FIN,0,0);
		FecFab = CTurnos::GetFecIni(FecFab, 1);
		long lCantFab = atol(Buffer);
		CTime FecIni = m_pOM->GetFecIni();
		DlgVal.m_sValAnt = m_sFabricadas + " - " + FormatFec(FecFab, Fecha);
		DlgVal.m_sValNue = (CString) Buffer + " - " + FormatFec(m_FecFab, Fecha); 
		DlgVal.m_sCampo = "Fabricaci�n";
		//if (DlgVal.DoModal() == IDOK)
		{
			bModif = TRUE;
			m_pOM->SetFab(lCantFab, FecFab);
		}
	}	

	if (bModif) 
	{
		//m_pProg->Recalc();
		m_pOM->RecalcFin();
	}


}

void COrdenDlg::OnAjustarAMezcla() 
{
	int lResto = m_pOM->GetlRestoCantConMezcla();

	m_pOM->ModifCantChatarras(lResto);
	m_pOM->RecalcFin();
	PostMessage(WM_CLOSE);

}


void COrdenDlg::OnClose() 
{
	
	CDialog::OnClose();
	//Causa error
	//if (m_pCpwView)	m_pCpwView->Invalidate();
}


void COrdenDlg::OnCbnSelchangeMolde()
{

	CString cStr;
	m_ComboMolde.GetLBText(m_ComboMolde.GetCurSel(), cStr);
	RefreshMoldeDetalle(cStr);
}
