// TextView.cpp : implementation file
//

#include "stdafx.h"
#include "afxdtctl.h"
#include "cpw.h"
#include "cpwDefs.h"

#include "AbSpan.h"
#include "cpwExt.h"
#include "prog.h"

#include "cpwDoc.h"

#include "ChildFrm.h"
#include "cpwView.h"
#include "TextView.h"

#include "cpwDb.h"
#include "cache.h"            
#include "ProgMaq.h"
#include "OrdMaq.h"
#include "Dialogs.h" 
#include "OrdenDlg.h"  
#include "DlgOpciones.h"  

#include "process.h"

#include <strstream>
#include <fstream>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextView

IMPLEMENT_DYNCREATE(CTextView, CView)

CTextView::CTextView()
	: m_Brush()
{
	// TODO : Que el pintado no pinte mas que lo que se ve
	// TODO : Columnas fijas para que se puedan usar fonts variables
	CWinApp* wApp = AfxGetApp();
	ASSERT_VALID(wApp);
	int iHeight = wApp->GetProfileInt("FontText", "Height", -10);
	int iWidth = wApp->GetProfileInt("FontText", "Width", 0);
	int iEscapement = wApp->GetProfileInt("FontText", "Escapement", 0);
	int iOrientation = wApp->GetProfileInt("FontText", "Orientation", 0);
	int iWeight = wApp->GetProfileInt("FontText", "Weight", FW_NORMAL);
	int iItalic = wApp->GetProfileInt("FontText", "Italic", 0);
	int iUnderline = wApp->GetProfileInt("FontText", "Underline", 0);
	int iStrikeOut = wApp->GetProfileInt("FontText", "StrikeOut", 0);
	int iCharSet = wApp->GetProfileInt("FontText", "CharSet", ANSI_CHARSET);
	int iOutPrecision = wApp->GetProfileInt("FontText", "OutPrecision", OUT_CHARACTER_PRECIS);
	int iClipPrecision = wApp->GetProfileInt("FontText", "ClipPrecision", CLIP_CHARACTER_PRECIS);
	int iQuality = wApp->GetProfileInt("FontText", "Quality", DEFAULT_QUALITY);
	int iPitchAndFamily = wApp->GetProfileInt("FontText", "PitchAndFamily", FIXED_PITCH);
	CString sFaceName = wApp->GetProfileString("FontText", "FaceName", "FIXEDSYS");
	
	m_FontChanged = Primero;
	m_FontHeight = 0;   
	m_FontWidth = 0;           
	m_ColWidth = 0; 
	m_HeadHeight = TK_POINTS_HDR;
	m_pFont = new CFont();
	m_pOldFont = NULL;
	m_pFont->CreateFont(iHeight, iWidth, iEscapement, iOrientation, iWeight, iItalic, 
						iUnderline, iStrikeOut, iCharSet, iOutPrecision, iClipPrecision,
						iQuality, iPitchAndFamily, sFaceName );
	m_Brush.CreateSolidBrush(RGB(255,255,255));
	m_pSelOM = NULL;
	m_pSelPM = NULL;

	m_Update = Otro;

	m_sSearch = "";

	m_Size.cx = m_Size.cy = 10;

	m_iIni = 0;
	m_iPage = 1;

	m_pArrCompArt = NULL;
	CFile f;
	char buf[512];
	if( !f.Open( "C:\\cpw\\Persist.dat", CFile::modeRead ) ) 
	{
		m_pArrCompArt = new CStringArray;
		return;
	}
	CArchive ar( &f, CArchive::load, 512, buf );
	ar >> m_pArrCompArt;
	ar.Close();
	f.Close();
}

CTextView::~CTextView()
{
	delete m_pFont;

	CFile f;
	char buf[512];
	if( !f.Open( "C:\\cpw\\Persist.dat", CFile::modeCreate | CFile::modeWrite ) ) 
	{
		m_pArrCompArt = NULL;
		return;
	}
	CArchive ar( &f, CArchive::store, 512, buf );
	ar << m_pArrCompArt;
	delete m_pArrCompArt;
	ar.Close();
	f.Close(); 
}


BEGIN_MESSAGE_MAP(CTextView, CView)
	//{{AFX_MSG_MAP(CTextView)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_CARGAINICIAL, OnCargainicial)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_TIPO_LETRA_TXT, OnTipoLetraTxt)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDITAR_ORDEN, OnEditarOrden)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_CERRAR_OFS, OnCerrarOFs)
	ON_COMMAND(ID_ABRIR_OFS, OnAbrirOFs)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTextView drawing

void CTextView::OnDraw(CDC* pDC)
{
	
	if (g_bException) return;	// Si estamos sacando un mensaje de error, no hay garantías de
								// que se pueda pintar, así que lo dejamos.
	// Hacemos cálculos previos
	CRect RectI;
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	
    CRect RectClip;
    pDC->GetClipBox (&RectClip);

	CRect RectCli;
	GetClientRect(&RectCli);

//	CPoint PosScr = GetDeviceScrollPosition();
	CPoint PosScr = CPoint(0,0);
	int iLim = GetNumOF();
	
	// Calculamos las líneas de inicio y de fin que realmente hay que pintar
//	int iLinIni = (RectClip.top - m_HeadHeight)  / m_FontHeight ;
//	int iLinFin = ((RectClip.bottom - m_HeadHeight)  / m_FontHeight ) + 2;
	int iLinIni = m_iIni;
	m_iPage = ((RectCli.bottom-RectCli.top - m_HeadHeight) / 
					(m_FontHeight > 0 ? m_FontHeight : 1 )) - 1;
	
	if (m_iPage <= 0) m_iPage = 1;
	
	int iLinFin = m_iIni + m_iPage + 2;
	if ( iLinIni < 0) iLinIni = 0;
	if ( iLinFin > iLim) iLinFin = iLim;
		
	int iY = m_HeadHeight;
	int iX = 0;
	
	CString sCodAnt = "";
	BOOL bRep = FALSE;
	for (int i=0 ; i < iLinFin; i++)
	{
		CString sLin;
		CString sCant;
		CBufOF* pBufOF = GetpBufOFAt(i);
		if (sCodAnt == pBufOF->m_sIDArticulo) bRep = TRUE;
		else bRep = FALSE;
		pDC->SetBkColor(pBufOF->GetColor());
		if ( i < iLinFin && i >=  iLinIni )
		{
			iY = (i - iLinIni)* m_FontHeight + m_HeadHeight;
		
			iX = m_ArrUPos.GetAt(0);
			sLin.Format("%c"
				,(pBufOF->GetbComprimido() ? '+' : '-'));
			pDC->TextOut(iX, iY, sLin);
			
			iX = m_ArrUPos.GetAt(1);
			sLin.Format("%c"
				,(pBufOF->GetbSelected() ? '*' : ' '));
			pDC->TextOut(iX, iY, sLin);
			
			pDC->SetTextAlign(TA_RIGHT);
			iX = m_ArrUPos.GetAt(2);
			sLin.Format("%3.3s"
				,( pBufOF->m_lSeccion ?  FormatLong(pBufOF->m_lSeccion,3) : ""));
			pDC->TextOut(iX, iY, sLin);
			
			pDC->SetTextAlign(TA_RIGHT);
			iX = m_ArrUPos.GetAt(3);
			sLin.Format("%-9.9s"
				,( bRep ? "" : pBufOF->m_sIDArticulo ));
			pDC->TextOut(iX, iY, sLin);
			
			iX = m_ArrUPos.GetAt(4);
			sLin.Format("%-3.3s"
				,( bRep ? "" : g_pCache->GetsPropietario(pBufOF->m_sIDArticulo) ));
			pDC->TextOut(iX, iY, sLin);
			
			iX = m_ArrUPos.GetAt(5);
			sLin.Format("%-3.3s"
				,( bRep ? "" : g_pCache->GetsHomologacion(pBufOF->m_sIDArticulo) ));
			pDC->TextOut(iX, iY, sLin);
			

			pDC->SetTextAlign(TA_RIGHT);
			iX = m_ArrUPos.GetAt(6);
			sLin.Format("%9.9s"
				,FormatLong((long) pBufOF->m_dCantOrig, 9));
			pDC->TextOut(iX, iY, sLin);
			
			iX = m_ArrUPos.GetAt(7);
			sLin.Format("%9.9s"
				,FormatLong((long) pBufOF->m_dCantPend, 9));
			pDC->TextOut(iX, iY, sLin);
			
			iX = m_ArrUPos.GetAt(8);
			sLin.Format("%9.9s"
				,FormatLong((long) pBufOF->m_dPendAcum, 9));
			pDC->TextOut(iX, iY, sLin);
			
			iX = m_ArrUPos.GetAt(9);
			if (pBufOF->m_dNoProg > 0) 
			{
				sLin.Format("%9.9s"
					,FormatLong((long) pBufOF->m_dNoProg, 9));
				pDC->TextOut(iX, iY, sLin);
			} else {
				pDC->TextOut(iX, iY, "");
			}

			iX = m_ArrUPos.GetAt(10);
			if (pBufOF->m_dNoCubierta > 0) 
			{
				sLin.Format("%9.9s"
					,FormatLong((long) pBufOF->m_dNoCubierta, 9));
				pDC->TextOut(iX, iY, sLin);
			} else {
				pDC->TextOut(iX, iY, "");
			}

			iX = m_ArrUPos.GetAt(11);
			sLin.Format("%s"
				,FormatFec(pBufOF->m_FecNec, Fecha));
			pDC->TextOut(iX, iY, sLin);
			
			iX = m_ArrUPos.GetAt(12);
			sLin.Format("%9.9s"
				,( bRep ? "" : FormatLong((long) pBufOF->m_dCantAlmac, 9)));
			pDC->TextOut(iX, iY, sLin);
			
			iX = m_ArrUPos.GetAt(13);
			sLin.Format("%9.9s"
				,( bRep ? "" : FormatLong((long) pBufOF->m_dCantCurso , 9)));
			pDC->TextOut(iX, iY, sLin);
			
			iX = m_ArrUPos.GetAt(14);
			sLin.Format("%9.9s"
				,( bRep ? "" : FormatLong((long) pBufOF->m_dContratas , 9)));
			pDC->TextOut(iX, iY, sLin);
			
			iX = m_ArrUPos.GetAt(15);
			sLin.Format("%9.9s"
				,( bRep ? "" : FormatLong((long) pBufOF->m_dCantObsoletos , 9)));
			pDC->TextOut(iX, iY, sLin);
			
			
			iX = m_ArrUPos.GetAt(16);
			sLin.Format("%9.9s"
				,( bRep ? "" : FormatLong((long) pBufOF->m_dCantStockSeguridad , 9)));
			pDC->TextOut(iX, iY, sLin);
			
			iX = m_ArrUPos.GetAt(17);
			if (pBufOF->m_lHorasAcum > 0)
			{
				sLin.Format("%5.5s", FormatLong(pBufOF->m_lHorasAcum, 5));
				pDC->TextOut(iX, iY, sLin);
			}
			else
				pDC->TextOut(iX, iY, "");
			
			pDC->SetTextAlign(TA_LEFT);
			iX = m_ArrUPos.GetAt(18);
			sLin.Format("%-30.30s"
				,pBufOF->m_sNomcli );
			pDC->TextOut(iX, iY, sLin);
			
			/*
			pDC->SetTextAlign(TA_LEFT);
			iX = m_ArrUPos.GetAt(15);
			if (pBufOF->m_FecFab == pProg->GetFecIni() ||
				pBufOF->m_FecFab == pProg->GetFecFin())
			{
				sLin.Format("");
			} else
			{
				sLin.Format("%s"
					,FormatFec(pBufOF->m_FecFab, FecHora));
			}
			pDC->TextOut(iX, iY, sLin);
		*/
			pDC->SetTextAlign(TA_LEFT);
			iX = m_ArrUPos.GetAt(19);
			if (pBufOF->m_lMargenMed == INT_MIN)
			{
				sLin.Format("NO FAB.");
			} else if (pBufOF->m_lMargenMed == INT_MAX)
			{
				sLin.Format("HECHO");
			} else
			{
				sLin.Format("%6ld"
					,pBufOF->m_lMargenMed);
			}
			pDC->TextOut(iX, iY, sLin);
			pDC->SetBkColor(RGB(255,255,255));
		
		}
		sCodAnt = pBufOF->m_sIDArticulo;

	}
	
	CRect RectCab = RectClip;
	RectCab.bottom = RectCab.top + m_HeadHeight;
	pDC->Rectangle(&RectCab);
	
	pDC->SetTextAlign(TA_RIGHT);
	pDC->TextOut(m_ArrUPos.GetAt(2), PosScr.y+1, "SEC.");
	pDC->TextOut(m_ArrUPos.GetAt(3), PosScr.y+1, "OFERTA");
	
	pDC->TextOut(m_ArrUPos.GetAt(4), PosScr.y+1, "PR.");
	pDC->TextOut(m_ArrUPos.GetAt(5), PosScr.y+1, "HO.");

	pDC->TextOut(m_ArrUPos.GetAt(6), PosScr.y+1, "CANT.");
	pDC->TextOut(m_ArrUPos.GetAt(7), PosScr.y+1, "PEND.");
	pDC->TextOut(m_ArrUPos.GetAt(8), PosScr.y+1, "ACUM.");
	pDC->TextOut(m_ArrUPos.GetAt(9), PosScr.y+1, "N.P.");
	pDC->TextOut(m_ArrUPos.GetAt(10), PosScr.y+1, "N.Cub.");
	pDC->TextOut(m_ArrUPos.GetAt(11), PosScr.y+1, "Fec.Nec.");
	pDC->TextOut(m_ArrUPos.GetAt(12), PosScr.y+1, "APT.");
	pDC->TextOut(m_ArrUPos.GetAt(13), PosScr.y+1, "CURSO");
	pDC->TextOut(m_ArrUPos.GetAt(14), PosScr.y+1, "CONTR.");
	pDC->TextOut(m_ArrUPos.GetAt(15), PosScr.y+1, "OBS.");
	pDC->TextOut(m_ArrUPos.GetAt(16), PosScr.y+1, "CONSI.");
	pDC->TextOut(m_ArrUPos.GetAt(17), PosScr.y+1, "H.AC.");
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOut(m_ArrUPos.GetAt(18), PosScr.y+1, "CLIENTE");
}

/////////////////////////////////////////////////////////////////////////////
// CTextView diagnostics

#ifdef _DEBUG
void CTextView::AssertValid() const
{
	CView::AssertValid();
}

void CTextView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCpwDoc* CTextView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCpwDoc)));
	return (CCpwDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextView message handlers

void CTextView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	pDC->SelectObject(m_pFont);
	if (m_FontChanged == Cambiado || m_FontChanged == Primero)
	{
		
		if (m_pOldFont) 
		{
			delete m_pOldFont;
			m_pOldFont = NULL;
		}		
		TEXTMETRIC	t_tm;
		if (pDC->GetTextMetrics(&t_tm))         
		{
			CCpwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			CProg* pProg = pDoc->GetpProg();
			ASSERT_VALID(pProg);
			m_FontHeight = t_tm.tmHeight;
			m_HeadHeight = (int) (m_FontHeight * 1.1 ) + 1 ;
			
			m_FontWidth = t_tm.tmAveCharWidth;
			// Ahora calculamos los valores de las columnas
			m_ArrUPos.RemoveAll();
			m_ArrUPos.Add(0);
			m_ArrUPos.Add( pDC->GetTextExtent("+X").cx );
			m_ArrUPos.Add(m_ArrUPos.GetAt(1) + pDC->GetTextExtent("*X888").cx );	//  (Derecha)
			m_ArrUPos.Add(m_ArrUPos.GetAt(2) + pDC->GetTextExtent("X88888888Z").cx );	// (Izquierda)
			
			m_ArrUPos.Add(m_ArrUPos.GetAt(3) + pDC->GetTextExtent("XXXX").cx );	// (Derecha)
			m_ArrUPos.Add(m_ArrUPos.GetAt(4) + pDC->GetTextExtent("XXXX").cx );	// (Derecha)

			m_ArrUPos.Add(m_ArrUPos.GetAt(5) + pDC->GetTextExtent("8.888.888X").cx ); //  (Derecha)
			m_ArrUPos.Add(m_ArrUPos.GetAt(6) + pDC->GetTextExtent("8.888.888X").cx ); //  (Derecha)
			m_ArrUPos.Add(m_ArrUPos.GetAt(7) + pDC->GetTextExtent("8.888.888X").cx ); //  (Derecha)
			m_ArrUPos.Add(m_ArrUPos.GetAt(8) + pDC->GetTextExtent("8.888.888X").cx ); //  (Derecha)
			m_ArrUPos.Add(m_ArrUPos.GetAt(9) + pDC->GetTextExtent("8.888.888X").cx ); //  (Derecha)
			m_ArrUPos.Add(m_ArrUPos.GetAt(10) + pDC->GetTextExtent("88-88-88X").cx ); //  (Derecha) 
			m_ArrUPos.Add(m_ArrUPos.GetAt(11) + pDC->GetTextExtent("8.888.888X").cx ); //  (Derecha) 
			m_ArrUPos.Add(m_ArrUPos.GetAt(12) + pDC->GetTextExtent("8.888.888X").cx ); //  (Derecha)
			m_ArrUPos.Add(m_ArrUPos.GetAt(13) + pDC->GetTextExtent("8.888.888X").cx ); //  (Derecha)
			m_ArrUPos.Add(m_ArrUPos.GetAt(14) + pDC->GetTextExtent("8.888.888X").cx ); //  (Derecha)
			m_ArrUPos.Add(m_ArrUPos.GetAt(15) + pDC->GetTextExtent("8.888.888X").cx ); //  (Derecha)
			m_ArrUPos.Add(m_ArrUPos.GetAt(16) + pDC->GetTextExtent("888888X").cx ); //  (Derecha)
			m_ArrUPos.Add(m_ArrUPos.GetAt(17) + pDC->GetTextExtent("X").cx); //  (Derecha)
			m_ArrUPos.Add(m_ArrUPos.GetAt(18) + pDC->GetTextExtent("ABCDEFGHIJ KLMNOPQRST ABCDERE").cx); 
			m_ArrUPos.Add(m_ArrUPos.GetAt(19) + pDC->GetTextExtent("8.888.888X").cx); 

			// Calculamos la anchura del scroll
			m_Size.cy = (int) GetNumOF()*m_FontHeight + m_HeadHeight;
			m_Size.cx = m_ArrUPos.GetAt(m_ArrUPos.GetUpperBound());
			CRect RectCli;
			GetClientRect(&RectCli);
			CSize SizePage(RectCli.right-RectCli.left / 10 ,
					(((RectCli.bottom-RectCli.top - m_HeadHeight) / 
					(m_FontHeight > 0 ? m_FontHeight : 1 )) - 1 ) * m_FontHeight);
//			SetScrollSizes(MM_TEXT, m_Size, 
//				SizePage,
//				CSize(m_FontWidth , m_FontHeight));
		} else
		{
			TRACE("Error en GetTextMetrics en OnPrepareDC de XcpView");
		}
		m_FontChanged = Igual;
	}  
	CView::OnPrepareDC(pDC, pInfo);
}

void 
CTextView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{

	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);  
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);

 	CTime FecIni = pProg->GetFecIni(); 
	CTime FecFin = pProg->GetFecFin(); 
	long lTimeScale = pProg->GetTimeScale();   

	CTimeSpan tsDespl = FecFin - FecIni;
	m_Size.cy = (int)  GetNumOF()*m_FontHeight + m_HeadHeight;
	m_Size.cx = pProg->GetNumPM() * ( m_ColWidth ) + TK_POINTS_COL;
    
	CRect RectCli;
	GetClientRect(&RectCli);
	CSize SizePage(RectCli.right-RectCli.left / 10 ,
					(( ( RectCli.bottom - RectCli.top - m_HeadHeight) / 
					(m_FontHeight > 0 ? m_FontHeight : 1 )) - 1 ) * m_FontHeight);
//	SetScrollSizes(MM_TEXT, m_Size, 
//		SizePage,
//		CSize(m_FontWidth , m_FontHeight));
	//Invalidate();
	CView::OnUpdate(pSender, lHint, pHint);
	
}

void
CTextView::SwitchComprimido(CBufOF* p_pBufOF)
{
	int iLoc = -1;
	for (int i = 0; i < GetNumOF(); i++)
	{
		CBufOF* pBufOF = GetpBufOFAt(i);
		if (pBufOF->m_sIDArticulo  == p_pBufOF->m_sIDArticulo )
		{
			iLoc = i;
			break;
		}
	}
	if (iLoc < 0) return;

	// Si está expandido
	if (!p_pBufOF->GetbComprimido())
	{
		// Lo comprimimos
		CBufOF* pBufOF = GetpBufOFAt(iLoc);
		pBufOF->SetComprimido(TRUE);
		// Y lo añadimos a la matriz de persistencia si no está ya añadido
		BOOL bFind = FALSE;
		for (int j = 0; j < m_pArrCompArt->GetSize(); j++)
		{
			if (m_pArrCompArt->GetAt(j) == pBufOF->m_sIDArticulo)
			{
				bFind = TRUE;
			}
		}
		if (!bFind)	m_pArrCompArt->Add(pBufOF->m_sIDArticulo);

		for (int i = iLoc + 1; i < GetNumOF(); )
		{
			CBufOF* pBufOFNext = GetpBufOFAt(i);
			if (pBufOFNext->m_sIDArticulo == p_pBufOF->m_sIDArticulo)
			{
				m_ArrOFs.RemoveAt(i);
			} else
			{
				break;
			}
		}
	}
	else 
	{
		// Buscamos dónde está nuestra OF
		// Vamos al cache a buscar la expansión
		BOOL bEncontrado = FALSE;
		int iIni = 0;
		int i;
		for (i = 0; i < g_pCache->GetNumOF(); i++)
		{
			CBufOF* pBufOF = g_pCache->GetpOFAt(i);
			if (!bEncontrado && pBufOF == p_pBufOF)
			{
				// Encontramos el principio
				bEncontrado = TRUE;
				iIni = i;
			}
			if (bEncontrado && (pBufOF->m_sIDArticulo != p_pBufOF->m_sIDArticulo ))
			{
				// Ya estamos al final
				break;
			}
		}
		if (!bEncontrado) return;
		
		// lo borramos de la matriz de persistencia si está
		CBufOF* pBufOFPer = g_pCache->GetpOFAt(iIni);
		for (int j = 0; j < m_pArrCompArt->GetSize(); j++)
		{
			if (m_pArrCompArt->GetAt(j) == pBufOFPer->m_sIDArticulo)
			{
				m_pArrCompArt->RemoveAt(j);
			}
		}
		// Borramos el elemento comprimido antes de insertarlos todos
		m_ArrOFs.RemoveAt(iLoc);
		
		for (i = i-1; i >= iIni; i--)
		{
			CBufOF* pBufOF = g_pCache->GetpOFAt(i);
			pBufOF->SetComprimido(FALSE);
			m_ArrOFs.InsertAt(iLoc, pBufOF);
		}
	}
}

void CTextView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	if (point.y <= m_HeadHeight) return;
	CBufOF* pBufOF = GetpBufOFAt(point);
	if ((unsigned) point.x < (unsigned) m_ArrUPos.GetAt(1))
	{
		SwitchComprimido(pBufOF);
		Invalidate();
		return;
	}

	if (nFlags & MK_SHIFT) 	// Shift
		SelectToBufOF(pBufOF);
	else 	if (nFlags & MK_CONTROL) 	// Ctrl
		pBufOF->SetSelect(!(pBufOF->GetbSelected()));
	else 
	{
		UnselectAllBufOF();			
		pBufOF->SetSelect(!(pBufOF->GetbSelected()));
	}
	
	Invalidate();
	
}

void
CTextView::UnselectAllBufOF()
{
	int TopInd = GetNumOF();

	for(int ind = 0; ind < TopInd; ind++)
	{
		CBufOF* pBufOFAct = GetpBufOFAt(ind);
		ASSERT_VALID (pBufOFAct);
	
		pBufOFAct->SetSelect(FALSE);
	}
}

void
CTextView::SelecToFec(CTime p_Fec)
{
	UnselectAllBufOF();
	
	int TopInd = GetNumOF();
	
	for(int ind = 0; ind < TopInd; ind++)
	{
		CBufOF* pBufOFAct = GetpBufOFAt(ind);
		ASSERT_VALID (pBufOFAct);
		
		if (pBufOFAct->m_FecNec <= p_Fec)
			pBufOFAct->SetSelect(TRUE);
		else
			pBufOFAct->SetSelect(FALSE);
	}
	
	Invalidate();
}

void 
CTextView::SelectToBufOF(CBufOF *p_pBufOF)
{
	CBufOF* pBufOFAct;
	CBufOF* pBufOFUltSel = 0;

	// MaxInd y MinInd indican los índices del primero y el último
	// seleccionados. ActInd el de la Orden actual
	int MaxInd = -1;
	int MinInd = -1;
	int ActInd = -1;
	int TopInd = GetNumOF();
	// DesInd y HasInd marcan los límites de selección finales
	int DesInd;
	int HasInd;

	for(int ind = 0; ind < TopInd; ind++)
	{
		pBufOFAct = GetpBufOFAt(ind);
		ASSERT_VALID (pBufOFAct);
		if (pBufOFAct == p_pBufOF)
			ActInd = ind;
		if (pBufOFAct->GetbSelected()) 
		{
			MaxInd = ind;
			if (MinInd == -1)
				MinInd = ind;		
		}
	}

	// Ahora calculamos DesInd y HasInd
	// Por defecto 
	DesInd = ActInd;	// Seleccionaremos desde la actual
	HasInd = ActInd;	// Hasta la misma
	if (MaxInd == - 1)	// Ninguna orden seleccionada
	{	
	} else // Una o varias órdenes
	{
		if (ActInd < MinInd)	// Si actual menor que el mínimo
		{
			DesInd = ActInd;	// Seleccionamos desde la seleccionada
			HasInd = MinInd;	// Hasta la actual
		} else	if (MinInd < ActInd  && ActInd < MaxInd) // Si entremedias
		{
		} else 
		{
			DesInd = MaxInd;	// Seleccionamos desde la última
			HasInd = ActInd+1;	// Hasta la actual
		}
	}
	// Realizamos la verdadera selección
	for(int ind = DesInd; ind < HasInd; ind++)
	{
		pBufOFAct = GetpBufOFAt(ind);
		ASSERT_VALID (pBufOFAct);
	
		pBufOFAct->SetSelect(TRUE);
	}

}

BOOL CTextView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	// Si se hace un scroll real, lo invalidamos todo, porque tenemos
	// las cabeceras de máquinas y el calendario
//	CPoint PointApre = GetDeviceScrollPosition();
	//BOOL bRetVal =  CView::OnScroll(nScrollCode, nPos, bDoScroll);
	//if (PointApre != GetDeviceScrollPosition())
	{
		Invalidate();	
	}
//	return bRetVal;
	return TRUE;
}

void CTextView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CMenu PopUp;
	PopUp.CreatePopupMenu();

	if (nFlags & MK_SHIFT) {
			CBufOF* pBufOF = GetpBufOFAt(point);
			SelectToBufOF(pBufOF);
			AfxMessageBox(pBufOF->m_sIDArticulo);
			AfxMessageBox(g_pCache->GetsHomologacion(pBufOF->m_sIDArticulo, true));
	}

	m_Point = point;
	UINT tTipo = MF_ENABLED | MF_STRING;
	PopUp.AppendMenu(tTipo, ID_CARGAINICIAL, "Programar");
	PopUp.AppendMenu(tTipo, ID_EDITAR_ORDEN, "Expandir...");
	PopUp.AppendMenu(tTipo, ID_CERRAR_OFS, "Cerrar todas");
	PopUp.AppendMenu(tTipo, ID_ABRIR_OFS, "Abrir todas");

	
	CPoint ScrPoint(point);
	ClientToScreen(&ScrPoint);
	PopUp.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ScrPoint.x, ScrPoint.y, this);
	
	CView::OnRButtonDown(nFlags, point);
}

void 
CTextView::OnCargainicial() 
{
	m_bCancelar = FALSE;
	
	ASSERT_VALID(g_pCache);
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	
	BOOL bModif = FALSE; // Para saber si ha habido alguna modificacion.

	// hacemos un ciclo para buscar las órdenes seleccionadas
	int iLim = GetNumOF();
	for (int i=0 ; i < iLim ; i++)
	{
		CBufOF* pBufOF = GetpBufOFAt(i);
		
		if (pBufOF->GetbSelected())
		{
			if (g_pCache->GetNoHomologada(pBufOF->m_sIDArticulo)) {
				if (AfxMessageBox("La referencia " + pBufOF->m_sIDArticulo + " no está homologada en esta empresa.¿ Programar a pesar de ello?", MB_YESNOCANCEL) != IDYES) {
					continue;
				}
			}
			pBufOF->SetSelect(FALSE);
			long lCant = (long) pBufOF->m_dNoProg;
			// Buscamos la última OF con el mismo artículo, que es en la que nos tendremos que basar
			for(int j=i+1;  j < iLim; j++)
			{
				CBufOF* pBufOFAdd = GetpBufOFAt(j);
				if (pBufOFAdd->m_sIDArticulo != pBufOF->m_sIDArticulo ) break;
				// La cantidad será igual al acumulado no programado de la última OF seleccionada
				if (pBufOFAdd->GetbSelected() ) lCant = (long) pBufOFAdd->m_dNoProg;
				pBufOFAdd->SetSelect(FALSE);
			}

			// Ahora damos la opción para insertar

			// Si no esta ya hacemos una lista de maquinas a las que puede ir
			CObArray ArrListaMaq, ArrListaMoldes;
			g_pCache->GetArrMaq(pBufOF->m_sIDArticulo, ArrListaMaq, ArrListaMoldes);
			int iLimListaMaq = ArrListaMaq.GetSize();
			if (iLimListaMaq <= 0)
			{
				CString sMsg;
				sMsg.Format(STRID(IDS_ERR_NOMAQMOL),pBufOF->m_sIDArticulo); 
				MessageBox(sMsg, STRID(IDS_ERROR));
				continue;
			}
			bool bEncontrado = false;
			for(int ix = 0; ix < iLimListaMaq; ix++)
			{
				CBufMaquina* pBufMaquina = (CBufMaquina*) ArrListaMaq.GetAt(ix);
				CBufMolde* pBufMolde = (CBufMolde*) ArrListaMoldes.GetAt(ix);
				ASSERT_VALID(pBufMaquina);
				CProgMaq* pPM = pProg->FindMaq(pBufMaquina->m_sID);
				if (pPM != NULL && lCant > 0)
				{
					// Creamos una nueva OM
					COrdMaq* pOM = new COrdMaq(pBufOF, pBufMolde);
					pOM->SetlCantidad(lCant);
					BOOL bModifTemp;
					m_bCancelar = pProg->InsertNewOM(pOM, bModifTemp);
					bEncontrado = true;
					if (bModifTemp) bModif = TRUE;
					Invalidate();
					break;
				}
			
			}
			if (!bEncontrado && lCant > 0) {
				// Si sale por aquí es que no hay máquinas adecuadas. Avisamos
				CString sMsg = "Las máquinas de esta orden son :";
				for (int iz = 0; iz < iLimListaMaq; iz++) {
					CBufMaquina* pBufMaquina = (CBufMaquina*) ArrListaMaq.GetAt(iz);
					sMsg += " " + pBufMaquina->m_sID;
				}
				sMsg += ". Ninguna de estas máquinas está en el programa, por lo que no se pueden programar.";
				MessageBox(sMsg, STRID(IDS_ERROR));
			}
		}
		if (m_bCancelar) break;					
	}
	CWaitCursor WaitCursor;
	if (bModif) 
	{
		pProg->Recalc();
	}
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL);
}


void 
CTextView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{

	CBufOF* pBufOF = GetpBufOFAt(point);
	if (pBufOF == NULL) return;
	

	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);  
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	COrdMaq* pOM = pProg->FindOMArt(pBufOF->m_sIDArticulo);
	if (pOM == NULL)
	{
		CString sMsg;
		sMsg.Format( STRID (IDS_OF_NOTFOUND) ,	pBufOF->m_sIDArticulo ); 
		MessageBox(sMsg);
		return;
	}

	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if ( pView->IsKindOf( RUNTIME_CLASS( CCpwView )))
		{
			CCpwView* pCpwView = (CCpwView*) pView;
			CRect RectOM = pCpwView->GetRect(pOM);
			RectOM.left += pCpwView->m_SepWidth;
			CRect RectWnd;
			pCpwView->GetClientRect(&RectWnd);
			CFrameWnd* pFrame = GetParentFrame();
			if (!pFrame) return;
			
			CPoint PointDesp;
			PointDesp.x = RectOM.left - TK_POINTS_COL;
			if (PointDesp.x < 0) PointDesp.x = 0;
			pCpwView->SetScrollPos(SB_HORZ, PointDesp.x);
			PointDesp.y = RectOM.top - pCpwView->m_HeadHeight;
			if (PointDesp.y < 0) PointDesp.y = 0;
			pCpwView->SetScrollPos(SB_VERT, PointDesp.y);
			pProg->UnselectAll();
			pOM->SetBSelected(TRUE);

		// No cambiamos el cursor de sitio
		/*	

			PointDesp = pCpwView->GetDeviceScrollPosition();
			CRect RectWin;
			pCpwView->GetWindowRect(&RectWin);
			CRect RectCursorPos = RectOM - PointDesp + CPoint(RectWin.left, RectWin.top);
			SetCursorPos(pOM->GetBFija() ? RectCursorPos.left - pCpwView->m_SepWidth + 3 : RectCursorPos.left,
				RectCursorPos.top + 3	);
		*/	
			pCpwView->Invalidate();
		}
	}   
	
	CView::OnLButtonDblClk(nFlags, point);
}

// Rellena una lista con identificadores de Artículos que están en las OF´s que se están viendo
void
CTextView::GetArrsIDArt(CStringArray& p_ArrsIDArt)
{
	p_ArrsIDArt.RemoveAll();
	int iNumOF = GetNumOF();
	BOOL bEncontrado;
	
	CString sIDAnt = "";
	for (int i=0; i < iNumOF; i++)
	{
		CBufOF* pBufOF = GetpBufOFAt(i);
		if (pBufOF->m_sIDArticulo == sIDAnt) continue;
		sIDAnt = pBufOF->m_sIDArticulo;
		// Buscamos posibles duplicados
		bEncontrado = FALSE;
		for (int j = 0; j < p_ArrsIDArt.GetSize(); j++)
		{
			if (pBufOF->m_sIDArticulo == p_ArrsIDArt.GetAt(j))
			{
				bEncontrado = TRUE;
				break;
			}
		}
			if (!bEncontrado) p_ArrsIDArt.Add(pBufOF->m_sIDArticulo);		
	}

}

int
CTextView::GetNumOF()
{
	return m_ArrOFs.GetSize();
}

CBufOF* 
CTextView::GetpBufOFAt(int p_iIndex)
{
	if (p_iIndex < 0 || p_iIndex >= GetNumOF()) return NULL;
	return (CBufOF*) m_ArrOFs.GetAt(p_iIndex);
}

CBufOF* 
CTextView::GetpBufOFAt(CPoint p_Point)
{
	int iLin = (p_Point.y - m_HeadHeight) / m_FontHeight + m_iIni;

	return GetpBufOFAt(iLin);

}


void CTextView::OnTipoLetraTxt() 
{
	LOGFONT LogFontAct;
	CWinApp* wApp = AfxGetApp();
	ASSERT_VALID(wApp);
	
	LogFontAct.lfHeight = wApp->GetProfileInt("FontText", "Height", -10);
	LogFontAct.lfHeight = wApp->GetProfileInt("FontText", "Width", 0);
	LogFontAct.lfEscapement = wApp->GetProfileInt("FontText", "Escapement", 0);
	LogFontAct.lfOrientation = wApp->GetProfileInt("FontText", "Orientation", 0);
	LogFontAct.lfWeight = wApp->GetProfileInt("FontText", "Weight", FW_NORMAL);
	LogFontAct.lfItalic = wApp->GetProfileInt("FontText", "Italic", 0);
	LogFontAct.lfUnderline = wApp->GetProfileInt("FontText", "Underline", 0);
	LogFontAct.lfStrikeOut = wApp->GetProfileInt("FontText", "StrikeOut", 0);
	LogFontAct.lfCharSet = wApp->GetProfileInt("FontText", "CharSet", ANSI_CHARSET);
	LogFontAct.lfOutPrecision = wApp->GetProfileInt("FontText", "OutPrecision", OUT_CHARACTER_PRECIS);
	LogFontAct.lfClipPrecision = wApp->GetProfileInt("FontText", "ClipPrecision", CLIP_CHARACTER_PRECIS);
	LogFontAct.lfQuality = wApp->GetProfileInt("FontText", "Quality", DEFAULT_QUALITY);
	LogFontAct.lfPitchAndFamily = wApp->GetProfileInt("FontText", "PitchAndFamily", FIXED_PITCH);
	strncpy(LogFontAct.lfFaceName, wApp->GetProfileString("FontText", "FaceName", "FIXEDSYS"), sizeof(LogFontAct.lfFaceName));
	
	
	CFontDialog FontDialog;
	FontDialog.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;
	FontDialog.m_cf.lpLogFont = &LogFontAct;
	
	if (FontDialog.DoModal() != IDOK) return;

	LOGFONT LogFont;
	FontDialog.GetCurrentFont(&LogFont);
	
	m_pOldFont = m_pFont;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&LogFont);
	m_FontChanged = Cambiado;

	//Actualizamos el Registry
	wApp->WriteProfileInt("FontText", "Height", LogFont.lfHeight);
	wApp->WriteProfileInt("FontText", "Width", LogFont.lfWidth);
	wApp->WriteProfileInt("FontText", "Escapement", LogFont.lfEscapement);
	wApp->WriteProfileInt("FontText", "Orientation", LogFont.lfOrientation);
	wApp->WriteProfileInt("FontText", "Weight", LogFont.lfWeight);
	wApp->WriteProfileInt("FontText", "Italic", LogFont.lfItalic);
	wApp->WriteProfileInt("FontText", "Underline", LogFont.lfUnderline);
	wApp->WriteProfileInt("FontText", "StrikeOut", LogFont.lfStrikeOut);
	wApp->WriteProfileInt("FontText", "CharSet", LogFont.lfCharSet);
	wApp->WriteProfileInt("FontText", "OutPrecision", LogFont.lfOutPrecision);
	wApp->WriteProfileInt("FontText", "ClipPrecision", LogFont.lfClipPrecision);
	wApp->WriteProfileInt("FontText", "Quality", LogFont.lfQuality);
	wApp->WriteProfileInt("FontText", "PitchAndFamily", LogFont.lfPitchAndFamily);
	wApp->WriteProfileString("FontText", "FaceName", LogFont.lfFaceName);
	
	Invalidate();
}

BOOL 
CTextView::LocateOF(const CString& p_sSearch)
{
	int TopInd = GetNumOF();
	int iLen = p_sSearch.GetLength();
	if (iLen <= 0) return -1;

	for(int ind = 0; ind < TopInd; ind++)
	{
		CBufOF* pBufOFAct = GetpBufOFAt(ind);
		if (pBufOFAct->m_sIDArticulo.Left(iLen) == p_sSearch) 
		{
			m_iIni = ind;
			Invalidate();
			
			return TRUE;
		}
	}
	return FALSE;
}

void
CTextView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	CRect RectCli;
	GetClientRect(&RectCli);
	CSize SizePage(RectCli.right-RectCli.left / 10 ,
		(((RectCli.bottom-RectCli.top - m_HeadHeight) / 
		(m_FontHeight > 0 ? m_FontHeight : 1 )) - 1 ) * m_FontHeight);
//	SetScrollSizes(MM_TEXT, m_Size, 
//		SizePage,
//		CSize(m_FontWidth , m_FontHeight));
				
}


void CTextView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CChildFrame* pFrame = (CChildFrame*) GetParentFrame();
	ASSERT (pFrame->IsKindOf( RUNTIME_CLASS( CChildFrame ) ));
	pFrame->OnCharRec( nChar, nRepCnt, nFlags);	
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void 
CTextView::SeteOrden()
{
	g_pCache->SetOrdenOF();
	CopyOFsFromCache();
	Invalidate();
}

void
CTextView::CopyOFsFromCache()
{
	CWaitCursor WaitCursor;

	m_ArrOFs.RemoveAll();
	int iLim = g_pCache->GetNumOF();
	// Si estamos clasificados por cliente, lo clasificamos por cliente
	
	if (g_VerOF.m_OrdenOF == Cliente)
	{
		BOOL bFind;
		for (int i=0 ; i < iLim ; i++)
		{
			CBufOF* pBufOF = g_pCache->GetpOFAt(i);
			bFind = FALSE;
			for (int j=0; j < m_ArrOFs.GetSize(); j++)
			{
				CBufOF* pBufOFIn = (CBufOF*) m_ArrOFs.GetAt(j);
				ASSERT (pBufOFIn->IsKindOf( RUNTIME_CLASS( CBufOF ) ));
				if (pBufOFIn->m_sNomcli > pBufOF->m_sNomcli || 
					( (pBufOFIn->m_sNomcli == pBufOF->m_sNomcli) &&
					(pBufOFIn->m_sIDArticulo > pBufOF->m_sIDArticulo ) ) )
				{
					m_ArrOFs.InsertAt(j, pBufOF);
					bFind = TRUE;
					break;
				}
			}
			if (!bFind) m_ArrOFs.Add(pBufOF);
		}

	}
	else   // Si no, cargamos y comprimimos los que estén comprimidos
	{
		for (int i=0 ; i < iLim ; i++)
		{
			CBufOF* pBufOF = g_pCache->GetpOFAt(i);
			m_ArrOFs.Add(pBufOF);
		}
		// Comparamos ahora la lista de comprimidos con lo que tenemos, y los comprimimos
		for (int i=0 ; i < GetNumOF() ; i++)
		{
			CBufOF* pBufOF = GetpBufOFAt(i);
			for (int j = 0; j < m_pArrCompArt->GetSize(); j++)
			{
				if (m_pArrCompArt->GetAt(j) == pBufOF->m_sIDArticulo)
				{
					pBufOF->SetComprimido(FALSE);
					SwitchComprimido(pBufOF);
					break;
				}
			}
		}
	}
}

void CTextView::OnEditarOrden() 
{
	CBufOF* pBufOF = GetpBufOFAt(m_Point);
	if (pBufOF == NULL) return;
	

	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);  
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	COrdMaq* pOM = pProg->FindOMArt(pBufOF->m_sIDArticulo);
	if (pOM == NULL)
	{
		CString sMsg;
		sMsg.Format( STRID (IDS_OF_NOTFOUND) ,	pBufOF->m_sIDArticulo ); 
		MessageBox(sMsg);
		return;
	}
	pOM->SetBSelected(TRUE);
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if ( pView->IsKindOf( RUNTIME_CLASS( CCpwView )))
		{
			CCpwView* pCpwView = (CCpwView*) pView;
			pCpwView->m_pSelOM = pOM;
			pCpwView->SendMessage(WM_COMMAND, ID_EDITAR_ORDEN);
		}
	}
}

void CTextView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	CopyOFsFromCache();
	//EnableScrollBarCtrl(SB_VERT);	
}

void CTextView::DoScroll(UINT p_nChar, UINT p_nRepCnt, UINT p_nFlags)
{
	int iLim = GetNumOF();
	if (!p_nRepCnt) p_nRepCnt = 1;
	for(UINT i = 0; i < p_nRepCnt; i++)
	{
		if (p_nChar == VK_NEXT ) 
		{
			m_iIni += m_iPage;
		}
		else if (p_nChar == VK_PRIOR ) 
		{
			m_iIni -= m_iPage;
		}
		else if (p_nChar == VK_END ) 
		{
			m_iIni = iLim - m_iPage;
		}
		else if (p_nChar == VK_HOME ) 
		{
			m_iIni = 0;
		}
		else if (p_nChar == VK_UP ) 
		{
			m_iIni--;
		}
		else if (p_nChar == VK_DOWN ) 
		{
			m_iIni++;
		}
		else if (p_nChar == VK_F12 ) 
		{
			m_iIni = GetiOFSec(m_iIni, (p_nFlags & VK_SHIFT) ? FALSE : TRUE );
		}
		else if (p_nChar == VK_F11 ) 
		{
			m_iIni = GetiOFArt(m_iIni, (p_nFlags & VK_SHIFT) ? FALSE : TRUE );
		}
		
		if ( m_iIni < 0) m_iIni = 0;
		if ( m_iIni > iLim) m_iIni = iLim;
	}
	Invalidate();
}
//Mueve hasta la siguiente sección, o la anterior
int 
CTextView::GetiOFSec(int p_iOFAct, BOOL p_bNext )
{
	int iLim = GetNumOF();
	long lSec = GetpBufOFAt(p_iOFAct)->m_lSeccion;
	int i;
	if (p_bNext)
		for( i=p_iOFAct; i < iLim && lSec == GetpBufOFAt(i)->m_lSeccion; i++)
			;
		else
		{
			for( i=p_iOFAct; i >= 0 && lSec == GetpBufOFAt(i)->m_lSeccion; i--)
				;
			if (i > 0)
			{
				lSec = GetpBufOFAt(i)->m_lSeccion;
				for( i=i-1; i >= 0 && lSec == GetpBufOFAt(i)->m_lSeccion; i--)
					;
				i++;
			}	
		}
		return i;
}
// Mueve hasta el siguiente artículo, o el anterior
int 
CTextView::GetiOFArt(int p_iOFAct, BOOL p_bNext )
{
	int iLim = GetNumOF();
	CString sArt = GetpBufOFAt(p_iOFAct)->m_sIDArticulo;
	int i;
	if (p_bNext)
		for( i=p_iOFAct; i < iLim && sArt == GetpBufOFAt(i)->m_sIDArticulo; i++)
			;
		else
		{
			for( i=p_iOFAct; i >= 0 && sArt == GetpBufOFAt(i)->m_sIDArticulo; i--)
				;
			if (i > 0)
			{
				sArt = GetpBufOFAt(i)->m_sIDArticulo;
				for( i=i-1; i >= 0 && sArt == GetpBufOFAt(i)->m_sIDArticulo; i--)
					;
				i++;
			}	
		}
		
		return i;
}

void 
CTextView::DoCargaInicial()
{
	OnCargainicial();
}

void
CTextView::OnCerrarOFs()
{
	for (int i = 0; i < GetNumOF(); i++)
	{
		CBufOF* pBufOF = GetpBufOFAt(i);
		if (!pBufOF->GetbComprimido())
		{
			SwitchComprimido(pBufOF);
		}
	}
	Invalidate();		
}

void 
CTextView::OnAbrirOFs()
{
	for (int i = 0; i < GetNumOF(); i++)
	{
		CBufOF* pBufOF = GetpBufOFAt(i);
		if (pBufOF->GetbComprimido())
		{
			SwitchComprimido(pBufOF);
		}
	}
	Invalidate();		

}
