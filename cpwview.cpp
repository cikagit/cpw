// cpwView.cpp : implementation of the CCpwView class
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

#include "cpwDb.h"
#include "cache.h"            
#include "ProgMaq.h"
#include "OrdMaq.h"
#include "Dialogs.h" 
#include "OrdenDlg.h"  
#include "TextView.h" 
#include "Turnos.h"

#include "process.h"

#include <strstream>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Static variables

BOOL s_BMoviendo = FALSE;


static enum EObjSel s_eObjSel = Nada;


/////////////////////////////////////////////////////////////////////////////
// CCpwView

IMPLEMENT_DYNCREATE(CCpwView, CScrollView)

BEGIN_MESSAGE_MAP(CCpwView, CScrollView)
	//{{AFX_MSG_MAP(CCpwView)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_EDITAR_ORDEN, OnEditarOrden)
	ON_COMMAND(ID_BUSCAR_ORDEN, OnBuscarOrden)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_MOVER_ORDEN, OnMoverOrden)
	ON_COMMAND(ID_TIPO_LETRA, OnTipoLetra)
	ON_COMMAND(ID_TEST, OnTest)
	ON_COMMAND(ID_CARGA_ANTIGUO, OnCargaAntiguo)
	ON_COMMAND(ID_LISTAR_MAQ, OnListarMaq)
	ON_COMMAND(ID_VERIF_STRUCT, OnVerifStruct)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_CARGAINICIAL, OnCargainicial)
	ON_COMMAND(ID_ORDEN_RECALCULAR, OnOrdenRecalcular)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	// Pop-up menu commands
	ON_COMMAND(ID_NUEVA_ORDEN, OnNuevaOrden)
	ON_COMMAND(ID_DIVIDIR_ORDEN, OnDividirOrden)
	ON_COMMAND(ID_UNIR_ORDEN, OnUnirOrden)
	ON_COMMAND(ID_EDITAR_CALEND, OnEditarCalend)
	ON_COMMAND(ID_CALEND_GFH, OnCalendGFH)
	ON_COMMAND(ID_RECALC_MAQ, OnRecalcMaq)
	ON_COMMAND(ID_BORRAR_MAQ, OnBorrarMaq)
	ON_COMMAND(ID_INSERT_MAQ, OnInsertMaq)
	ON_COMMAND(ID_LISTAR_GRAF, OnListarGraf)
	ON_COMMAND(ID_LEFT_MAQ, OnLeftMaq)
	ON_COMMAND(ID_RIGHT_MAQ, OnRightMaq)
	ON_COMMAND(ID_SELECT_END, OnSelectEnd)
	ON_COMMAND(ID_CARGA_GFH, OnCargaGFH)
	ON_COMMAND(ID_REPROG_GFH, OnReprogGFH)
	ON_COMMAND(ID_CONSART_MAQ, OnConsArtMaq)
	ON_COMMAND(ID_CAMBIAR_PERSONAS, OnCambiarPersonas)
			
//	ON_WM_MOUSEHWHEEL()
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCpwView construction/destruction

CCpwView::CCpwView()
	: m_Brush()
{                        


	CWinApp* wApp = AfxGetApp();
	ASSERT_VALID(wApp);
	int iHeight = wApp->GetProfileInt("Font", "Height", -10);
	int iWidth = wApp->GetProfileInt("Font", "Width", 0);
	int iEscapement = wApp->GetProfileInt("Font", "Escapement", 0);
	int iOrientation = wApp->GetProfileInt("Font", "Orientation", 0);
	int iWeight = wApp->GetProfileInt("Font", "Weight", FW_NORMAL);
	int iItalic = wApp->GetProfileInt("Font", "Italic", 0);
	int iUnderline = wApp->GetProfileInt("Font", "Underline", 0);
	int iStrikeOut = wApp->GetProfileInt("Font", "StrikeOut", 0);
	int iCharSet = wApp->GetProfileInt("Font", "CharSet", ANSI_CHARSET);
	int iOutPrecision = wApp->GetProfileInt("Font", "OutPrecision", OUT_CHARACTER_PRECIS);
	int iClipPrecision = wApp->GetProfileInt("Font", "ClipPrecision", CLIP_CHARACTER_PRECIS);
	int iQuality = wApp->GetProfileInt("Font", "Quality", DEFAULT_QUALITY);
	int iPitchAndFamily = wApp->GetProfileInt("Font", "PitchAndFamily", FIXED_PITCH);
	CString sFaceName = wApp->GetProfileString("Font", "FaceName", "FIXEDSYS");
	
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

	m_Update = Otro;
}

CCpwView::~CCpwView()
{
	delete m_pFont;
}

BOOL CCpwView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCpwView drawing
                                                        

void 
CCpwView::OnPrepareDC( CDC* pDC, CPrintInfo* pInfo )
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
			int CodeWidth, DescWidth, MaxPWidth;
			
			CCpwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			CProg* pProg = pDoc->GetpProg();
			ASSERT_VALID(pProg);
			m_FontHeight = t_tm.tmHeight;
			m_HeadHeight = (int) (m_FontHeight * 1.1 ) + 1 ;
			
			m_FontWidth = 0;
			
			// CodeWidth = pDC->GetTextExtent("12345").cx;
			CodeWidth = 0;
			DescWidth = pDC->GetTextExtent("APK 12345").cx;
			// MaxPWidth = pDC->GetTextExtent("3.0").cx;
			MaxPWidth = 0;
			m_ColWidth = CodeWidth + DescWidth + MaxPWidth + 4;
			m_SepWidth =  (int) (m_ColWidth * .15) + 1;
			m_ColWidth += m_SepWidth;
			CTime FecIni = pProg->GetFecIni(); 
			CTime FecFin = pProg->GetFecFin(); 
			long lTimeScale = pProg->GetTimeScale();   
			
			CTimeSpan tsDespl = FecFin - FecIni;
			m_Size.cy = (int) (tsDespl.GetTotalSeconds() / lTimeScale) + m_HeadHeight;
			m_Size.cx = pProg->GetNumPM() * ( m_ColWidth ) + TK_POINTS_COL;
			SetScrollSizes(MM_TEXT, m_Size);
		} else
		{
			TRACE("Error en GetTextMetrics en OnPrepareDC de XcpView");
		}
		m_FontChanged = Igual;
	}  
	CScrollView::OnPrepareDC( pDC, pInfo ); 
}                                                        

void CCpwView::OnDraw(CDC* pDC)
{
	if (g_bException) return;	// Si estamos sacando un mensaje de error, no hay garant�as de
								// que se pueda pintar, as� que lo dejamos.
	// Hacemos c�lculos previos
	CRect RectI;
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	
    CRect RectClip;
    pDC->GetClipBox (&RectClip);

	CRect RectCli;
	GetClientRect(&RectCli);

	CPoint PosScr = GetDeviceScrollPosition();

	CTime FecIniPrg = pProg->GetFecIni(); 
	CTimeSpan tsDespl;

	/*
	CBitmap BiMaREP;
	BiMaREP.LoadBitmap(IDB_MOLDE_REP);
	CBitmap BiMaEA;
	BiMaEA.LoadBitmap(IDB_MOLDE_EA);
	CBitmap BiMaCF;
	BiMaCF.LoadBitmap(IDB_MOLDE_CF);
	CBitmap BiMaDP;
	BiMaDP.LoadBitmap(IDB_MOLDE_DP);
	CBitmap BiMaMK19;
	BiMaMK19.LoadBitmap(IDB_MOLDE_MK19);
	*/
	CBitmap BiMolde;
	BiMolde.LoadBitmap(IDB_MOLDE);

	CRect rcClient;
	rcClient = RectCli;

	// CClientDC dc(this);
	CBitmap* pbmOld = NULL;

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	


// Fin bitmap test



	// Calculamos las fechas de inicio y de fin que realmente hay que pintar
	long lTimeScale = pProg->GetTimeScale();   
	CTime FecIni = FecIniPrg + ((RectClip.top - m_HeadHeight)  * lTimeScale );
	CTime FecFin = FecIniPrg + ((RectClip.bottom - m_HeadHeight) * lTimeScale );
	if ( FecFin > pProg->GetFecFin()) 
		FecFin = pProg->GetFecFin();
	if ( FecIni < pProg->GetFecIni()) 
		FecIni = pProg->GetFecIni();
		
	int y = m_HeadHeight;
	CTime Fec = FecIni; 

	
	
	int NumMaq = pProg->GetNumPM();
	CBrush gBrush(RGB(150,150,150));
	int i;
	CBrush BrushWhite(RGB(255,255,255));
	// Calculamos la posicion de las maquinas		
	for (i=0; i<pProg->GetNumPM() ; i++)
	{
		CProgMaq* pPM = pProg->GetpPM(i);
		ASSERT_VALID(pPM);
		int xi = (i * m_ColWidth ) + TK_POINTS_COL;		           
		pPM->SetPosition(CPoint(xi, m_HeadHeight));
	}

	
	//////// Ahora sacamos las OM's
	
	for ( i=0; i<NumMaq ; i++)
	{
		CProgMaq* pMaq = pProg->GetpPM(i);
		ASSERT_VALID(pMaq);
		int NumOrd = pMaq->GetNumOM(); 
		// Para cada m�quina hacemos un medidor de �ltima l�nea pintada
		int iUltLin = 0;
		for (int j=0; j<NumOrd; j++)
		{                                   
			COrdMaq* pOM = pMaq->GetpOM(j);
			ASSERT_VALID(pOM);
			CRect Rect = GetRect(pOM); 
			// Si tiene interseccion con el RectClip, lo sacamos
			if (RectI.IntersectRect(&Rect, &RectClip))
			{
				if (iUltLin <= Rect.top) Rect.left += m_SepWidth / 2;
				Rect.right -= m_SepWidth / 2;
				int despl = Rect.left + 1;
				pDC->SetBkColor(RGB(255,255,255));
				pDC->SelectObject(pOM->GetpBrush());
				pDC->Rectangle(Rect);
				if (pOM->GetBFija())
					Triangle(pDC,	CPoint(Rect.left-(m_SepWidth/2)-1, Rect.top),
									CPoint(Rect.left-(m_SepWidth/4)+1, Rect.top),
									CPoint(Rect.left-(m_SepWidth/2)-1, Rect.top + (m_SepWidth/4)+1 ) );
				if (pOM->GetBPrevLink())
				{
					Triangle(pDC,	CPoint(Rect.right+(m_SepWidth/4)-1, Rect.top + (m_SepWidth/4)+1 ),
									CPoint(Rect.right+(m_SepWidth/2)-1, Rect.top),
									CPoint(Rect.right+(m_SepWidth/4)-1, Rect.top - (m_SepWidth/4)-1 ) );
				}
			}

			iUltLin = __max( iUltLin, Rect.bottom);
			
		}
	}
	
	// Hacemos un ciclo de m�quinas, pintamos el editor de calendario
	// si hay que pintarlo
	for (i=0; i<NumMaq ; i++)
	{
		CProgMaq* pPM = pProg->GetpPM(i);
		ASSERT_VALID(pPM);
		int xi = (i * m_ColWidth ) + TK_POINTS_COL;		           
		int xf = xi + m_ColWidth;

		
		pDC->FillRect(CRect(xi, PosScr.y, xf, PosScr.y+m_HeadHeight), &BrushWhite);
		pDC->MoveTo(xi,PosScr.y);
		pDC->LineTo(xi, PosScr.y+m_HeadHeight);
		pDC->LineTo(xf, PosScr.y+m_HeadHeight);
		pDC->SetBkMode(TRANSPARENT);                                                                           
		int iMargen = xf - xi - pDC->GetTextExtent(pPM->GetsID()).cx;
		int xc = xi + (int) ( iMargen / 2 ) + 1;
		if (xc < xi) xc = xi;
		pDC->TextOut( xc, PosScr.y+1, pPM->GetsID());
		pDC->SetBkMode(OPAQUE);
		
		
		if (!pPM->GetBEditCal()) continue; // Si no hay que pintar el editor de calendario, hemos terminado

		// Calendario de edici�n
		int iXCal = xi;
		CRect RectCal(iXCal , RectCli.top + PosScr.y + m_HeadHeight, 
			iXCal + TK_EDITCAL_W, RectCli.bottom + PosScr.y);
		if (RectI.IntersectRect(&RectCal, &RectClip))
		{
			Fec = FecIni;
			pDC->SelectObject(&m_Brush);
			pDC->Rectangle(RectCal);
			y = m_HeadHeight;
			int yf;
			CTime Fec = FecIni; 
			while (Fec <= FecFin)
			{
				BOOL pBTurnos[TK_NUM_TURNOS_MAX];
				//CString sDeb3 = FormatFec(Fec, FecHora);
				int iTurnos = g_pCache->GetiNumTurnos(Fec, *pPM, pBTurnos);
					
				for (int ii = 1; ii <= CTurnos::GetiNumTurnos(); ii++) {
					Fec = CTurnos::GetFecIni(Fec, ii);
					if (Fec > FecFin) break;
					tsDespl = Fec - FecIniPrg;
					y = (int) (tsDespl.GetTotalSeconds() / lTimeScale) + m_HeadHeight;
					yf = (int) ((tsDespl.GetTotalSeconds() + CTurnos::GetSecsTurno(ii))/ lTimeScale) + m_HeadHeight;
					pDC->MoveTo(iXCal, y);
					pDC->LineTo(iXCal+TK_EDITCAL_W, y);
					
					if (pBTurnos[ii-1])
					{
						CRect Rect( iXCal + 2, y + 2, iXCal + TK_EDITCAL_W - 2, yf - 2 );
						pDC->FillRect(Rect, &CBrush(RGB(50,50,50)));
					}
				}
				Fec = AddDays(Fec,1);	// Pasamos al d�a siguiente
				Fec = CTime(Fec.GetYear(), Fec.GetMonth(), Fec.GetDay(), 0,0,0); // Al comienzo del d�a
			}   
			
		}
	}	
	// Pintamos las  inactividades de las m�quinas
	
	for (i=0; i<NumMaq ; i++)
	{
		CProgMaq* pPM = pProg->GetpPM(i);
		ASSERT_VALID(pPM);
		int xi = (i * m_ColWidth ) + TK_POINTS_COL;		           
		int xf = xi + m_ColWidth;
		
		Fec = FecIni;
		for (int j = 0; j < pPM->GetNumInacts(); j++)
		{
			RECT RectInact;
			CAbSpan Inact = *(pPM->GetpInact(j));
			
			tsDespl = Fec - FecIniPrg;
			y = (int) (tsDespl.GetTotalSeconds() / lTimeScale) + m_HeadHeight;
			
			tsDespl = Inact.GetFecIni() - FecIniPrg;
			RectInact.top = (int) ((tsDespl.GetTotalSeconds() / lTimeScale) + m_HeadHeight);
			tsDespl = Inact.GetFecFin() - Inact.GetFecIni();
			RectInact.bottom = (int) (RectInact.top + (tsDespl.GetTotalSeconds() / lTimeScale));
			RectInact.left = xi;
			RectInact.right = xf;
			if (RectI.IntersectRect(&RectInact, &RectClip))
			{
				pDC->MoveTo(RectInact.left,RectInact.top);
				pDC->LineTo(RectInact.right, RectInact.top);
				pDC->LineTo(RectInact.left, RectInact.bottom);
				pDC->LineTo(RectInact.right,RectInact.bottom);
				pDC->LineTo(RectInact.left, RectInact.top);
			}
		}		
		
	}
	
	// Ahora pintamos los textos de las OM�s, los iconos de alerta, y la disponibilidad de mezcla
	
	for ( i=0; i<NumMaq ; i++)
	{
		CProgMaq* pMaq = pProg->GetpPM(i);
		ASSERT_VALID(pMaq);
		int NumOrd = pMaq->GetNumOM();     
		for (int j=0; j<NumOrd; j++)
		{                                   
			CString buffer;
			COrdMaq* pOM = pMaq->GetpOM(j);
			ASSERT_VALID(pOM);
			// Primero pintamos la barra de disponibilidad de mezclas, para inyeccion
			CRect Rect = GetRect(pOM); 
			if (RectI.IntersectRect(&Rect, &RectClip))
			{
				
				Rect.left += m_SepWidth / 2;
				Rect.right -= m_SepWidth / 2;
				int despl = Rect.left + 10;
				int iTop = (Rect.top >= RectClip.top+m_HeadHeight) ? Rect.top : RectClip.top+m_HeadHeight;
				if (pMaq->GetlGFH() == TK_GFH_INYECCION)
				{
					if (pOM->GetTsMezcla().GetTotalSeconds() > 0)
					{
						CTimeSpan tsDesplMezcla = pOM->GetFecIni() - FecIniPrg;
						int iTopM = (int) (tsDesplMezcla.GetTotalSeconds() / lTimeScale) + m_HeadHeight;
						tsDesplMezcla = pOM->GetFecMezcla() - pOM->GetFecIni();
						int iBottomM = (int) (iTopM + tsDesplMezcla.GetTotalSeconds() / lTimeScale);
						if (iBottomM > Rect.bottom) iBottomM = Rect.bottom;
						pDC->MoveTo(despl, iTopM);
						pDC->LineTo(despl, iBottomM);
						pDC->MoveTo(despl+1, iTopM);
						pDC->LineTo(despl+1, iBottomM);
					}
				}
				despl = Rect.left + 1;
				pDC->SetBkColor(RGB(255,255,255));
				pDC->SetBkMode(OPAQUE);              
				int iH = pOM->GetTsMargen().GetHours();
				CString sNom = pOM->GetsNombre();
				if (sNom != "")
				{
					buffer.Format( "%-9.9s", (const char*) sNom);
					pDC->TextOut(despl,iTop+1,buffer);

					despl += pDC->GetTextExtent(buffer).cx + 2;
					if (iH < 0) 
					{
						pDC->SetBkColor(RGB(255,0,0));
						pDC->TextOut(despl,iTop+1,"*");
						pDC->SetBkColor(RGB(255,255,255));
						despl += pDC->GetTextExtent("*").cx + 2;
					}
					else 
						despl += 2;
					
					CBufMolde* pBufMolde = (CBufMolde*) pOM->GetpBufMolde();
					if (pBufMolde->TieneDetalle())
					{
						despl = Rect.right - 1 - 20;
						int iDespY = iTop + pDC->GetTextExtent(buffer).cy;
						pbmOld = dcMem.SelectObject(&BiMolde);
						pDC->BitBlt(despl, iDespY, 20, 10,
							&dcMem, 0, 0, SRCCOPY);
					}
					/*
					if (pBufMolde)
					{
						despl = Rect.right - 1 - 20;
						int iDespY = iTop + pDC->GetTextExtent(buffer).cy;
						if (pBufMolde->m_BCercoREP) 
						{
							pbmOld = dcMem.SelectObject(&BiMaREP);
							pDC->BitBlt(despl , iDespY, 20, 10,
								&dcMem, 0, 0, SRCCOPY);
							despl -= 20;
							if (despl < Rect.left)
							{
								despl = Rect.right - 1 - 20;
								iDespY += 10;
							}
							
						}

						if (pBufMolde->m_BCanalFrio) 
						{
							pbmOld = dcMem.SelectObject(&BiMaCF);
							pDC->BitBlt(despl , iDespY , 20, 10,
								&dcMem, 0, 0, SRCCOPY);
							despl -= 20;
							if (despl < Rect.left)
							{
								despl = Rect.right - 1 - 20;
								iDespY += 10;
							}
							
						}
						if (pBufMolde->m_BExtractorAutomatico) 
						{
							pbmOld = dcMem.SelectObject(&BiMaEA);
							pDC->BitBlt(despl , iDespY, 20, 10,
								&dcMem, 0, 0, SRCCOPY);
							despl -= 20;
							if (despl < Rect.left)
							{
								despl = Rect.right - 1 - 20;
								iDespY += 10;
							}
						}
						if (pBufMolde->m_BDoblePiston) 
						{
							pbmOld = dcMem.SelectObject(&BiMaDP);
							pDC->BitBlt(despl , iDespY , 20, 10,
								&dcMem, 0, 0, SRCCOPY);
							despl -= 20;
							if (despl < Rect.left)
							{
								despl = Rect.right - 1 - 20;
								iDespY += 10;
							}
							
						}
						if (pBufMolde->m_BExtractorMK19) 
						{
							pbmOld = dcMem.SelectObject(&BiMaMK19);
							pDC->BitBlt(despl , iDespY , 20, 10,
								&dcMem, 0, 0, SRCCOPY);
							despl -= 20;
							if (despl < Rect.left)
							{
								despl = Rect.right - 1 - 20;
								iDespY += 10;
							}
							
						}
					}
					*/
				}
			}	
		}
	}
	
	// Pintamos el calendario de la izquierda
	y = m_HeadHeight;
	Fec = FecIni;
	CString sDia;
	int iNumTurnos = CTurnos::GetiNumTurnos();
	while (Fec <= FecFin)
	{
		//Hacemos las rayas que marcan los turnos de un d�a
		for (int ii=1; ii <= iNumTurnos; ii++)
		{
			
			Fec = CTurnos::GetFecIni(Fec, ii);
			if (Fec > FecFin) break;
			if (Fec >= FecIni)
			{
				tsDespl = Fec - FecIniPrg;
				y = (int) (tsDespl.GetTotalSeconds() / lTimeScale) + m_HeadHeight;
				pDC->MoveTo(PosScr.x,y);
				pDC->LineTo((ii == 1) ? PosScr.x+TK_POINTS_COL : PosScr.x+TK_POINTS_COL / 2, y);
			}
		}
		//Ahora ponemos el n�mero de d�a, en el centro del d�a
		int iHoraCentral = CTurnos::GetFecIni(Fec, 1).GetHour() + 12;
		Fec = CTime(Fec.GetYear(), Fec.GetMonth(), Fec.GetDay(), iHoraCentral ,0,0);
		sDia.Format("%d", Fec.GetDay());
		tsDespl = Fec - FecIniPrg;
		y = (int) (tsDespl.GetTotalSeconds() / lTimeScale) + m_HeadHeight;
		pDC->TextOut(PosScr.x+3, y, sDia);

		// Y pasamos a hacer el d�a siguiente
		Fec = AddDays(Fec, 1);
		Fec = CTime(Fec.GetYear(), Fec.GetMonth(), Fec.GetDay(), 0,0,0);
		
	}
	/*
	y = m_HeadHeight;
	Fec = FecIni;
	while (Fec <= FecFin)
	{
		CString sDia;
		Fec = CTime(Fec.GetYear(), Fec.GetMonth(), Fec.GetDay(), TK_TURNO3_FIN,0,0);
		if (Fec > FecFin) break;
		if (Fec >= FecIni)
		{
			tsDespl = Fec - FecIniPrg;
			y = (int) (tsDespl.GetTotalSeconds() / lTimeScale) + m_HeadHeight;
			pDC->MoveTo(PosScr.x,y);
			pDC->LineTo(PosScr.x+TK_POINTS_COL, y);
		}
		
		Fec = CTime(Fec.GetYear(), Fec.GetMonth(), Fec.GetDay(), TK_TURNO1_FIN,0,0);
		if (Fec > FecFin) break;
		if (Fec >= FecIni)
		{
			tsDespl = Fec - FecIniPrg;
			y = (int) (tsDespl.GetTotalSeconds() / lTimeScale) + m_HeadHeight;
			pDC->MoveTo(PosScr.x,y);
			pDC->LineTo(PosScr.x+TK_POINTS_COL/2, y);
		}
		
		Fec = CTime(Fec.GetYear(), Fec.GetMonth(), Fec.GetDay(), (TK_TURNO1_FIN + TK_TURNO2_FIN) / 2 ,0,0);
		CTime FecI = Fec - (CTimeSpan) ((m_FontHeight * lTimeScale) / 2);
		CTime FecF = Fec + (CTimeSpan) ((m_FontHeight * lTimeScale) / 2);
		if (FecI > FecFin) break;
		if (FecF >= FecIni)
		{
			tsDespl = FecI - FecIniPrg;
			y = (int) (tsDespl.GetTotalSeconds() / lTimeScale) + m_HeadHeight;
			sDia.Format("%d", Fec.GetDay());
			pDC->TextOut(PosScr.x+3, y, sDia);
		}
		
		
		Fec = CTime(Fec.GetYear(), Fec.GetMonth(), Fec.GetDay(), TK_TURNO2_FIN,0,0);
		if (Fec > FecFin) break;
		if (Fec >= FecIni)
		{
			tsDespl = Fec - FecIniPrg;
			y = (int) (tsDespl.GetTotalSeconds() / lTimeScale) + m_HeadHeight;
			pDC->MoveTo(PosScr.x,y);
			pDC->LineTo(PosScr.x+TK_POINTS_COL/2, y);
		}
		
		Fec += (CTimeSpan) TK_CAMBIO_DIA;

	}   
	*/
	
	pDC->SelectObject(&m_Brush);	// Seleccionamos un Brush aparte de los de peligro por
	// si los cambian.
	dcMem.SelectObject(pbmOld);		// Y por limpieza, el DC de los Bitmaps
}

/*
void
CCpwView::PaintEditCal(CProgMaq* p_pPM)
{

  }
  
	*/
	
	void 
		CCpwView::OnEditarCalend()
	{
		ASSERT(m_pSelPM);
		m_pSelPM->SetBEditCal(!m_pSelPM->GetBEditCal());
		Invalidate();	
	}
	
	void 
		CCpwView::OnMoverOrden()
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
		pDoc->SeteObjSel(s_eObjSel);	
		pDoc->MoverOrden();
		
	}
	
	
	void
		CCpwView::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
	{
		
		CCpwDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);  
		CProg* pProg = pDoc->GetpProg();
		ASSERT_VALID(pProg);
		
		CTime FecIni = pProg->GetFecIni(); 
		CTime FecFin = pProg->GetFecFin(); 
		long lTimeScale = pProg->GetTimeScale();   
		
		CTimeSpan tsDespl = FecFin - FecIni;
		m_Size.cy = (int) (tsDespl.GetTotalSeconds() / lTimeScale) + m_HeadHeight;
		m_Size.cx = pProg->GetNumPM() * ( m_ColWidth ) + TK_POINTS_COL;
		
		SetScrollSizes(MM_TEXT, m_Size);
		//Invalidate();
		CView::OnUpdate(pSender, lHint, pHint);
	}
	/////////////////////////////////////////////////////////////////////////////
	// CCpwView diagnostics
	
#ifdef _DEBUG
	void CCpwView::AssertValid() const
	{
		CView::AssertValid();
	}
	
	void CCpwView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}
	
	CCpwDoc* CCpwView::GetDocument() // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCpwDoc)));
		return (CCpwDoc*)m_pDocument;
	}
#endif //_DEBUG
	
	/////////////////////////////////////////////////////////////////////////////
	// CCpwView utility functions
	
	//GetRect: Devuelve el rect�ngulo absoluto que debe ocupar una orden
	CRect
		CCpwView::GetRect(COrdMaq* p_pOM)
	{
		CCpwDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		CProg* pProg = pDoc->GetpProg();
		ASSERT_VALID(pProg);
		
		long lTimeScale = pProg->GetTimeScale();   
		
		ASSERT_VALID(p_pOM);
		CProgMaq* pPM = p_pOM->GetpPM();
		ASSERT_VALID(pPM);
		CRect RetRect;
		CPoint PntIni = pPM->GetPosition();
		RetRect.left = PntIni.x;
		RetRect.right = RetRect.left + m_ColWidth;
		CTimeSpan tsDespl = p_pOM->GetFecIni() - pPM->GetFecIni();
		RetRect.top = (int) (tsDespl.GetTotalSeconds() / lTimeScale) + PntIni.y;
		tsDespl = p_pOM->GetFecFin() - pPM->GetFecIni();
		RetRect.bottom = (int) (tsDespl.GetTotalSeconds() / lTimeScale) + PntIni.y;
		
		return RetRect;	
		
	}
	
	//GetRectPrint: Devuelve el rect�ngulo absoluto que debe ocupar una orden en impresi�n
	CRect
		CCpwView::GetRectPrint(COrdMaq* p_pOM, int p_iCol, CTime p_FecIni)
	{
		CCpwDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		CProg* pProg = pDoc->GetpProg();
		ASSERT_VALID(pProg);
		
		ASSERT_VALID(p_pOM);
		CProgMaq* pPM = p_pOM->GetpPM();
		ASSERT_VALID(pPM);
		CRect RetRect;
		CPoint PntIni = pPM->GetPosition();
		RetRect.left = (int) ((p_iCol + .25) * m_PI.m_iPixCol  + .075 * m_PI.m_iPixCol);
		RetRect.right = (int) (RetRect.left + m_PI.m_iPixCol * 0.925);
		CTimeSpan tsDespl = p_pOM->GetFecIni() - p_FecIni;
		RetRect.top = (int) (tsDespl.GetTotalSeconds() / m_PI.m_lConvFactor) + m_PI.m_iCabHeight;
		tsDespl = p_pOM->GetFecFin() - p_FecIni;
		RetRect.bottom = (int) (tsDespl.GetTotalSeconds() / m_PI.m_lConvFactor) + m_PI.m_iCabHeight;
		
		return RetRect;	
		
	}
	/////////////////////////////////////////////////////////////////////////////
	// CCpwView message handlers
	
	void
		CCpwView::OnLButtonDown(UINT p_uFlags, CPoint p_Point) 
	{
		
		COrdMaq* pOM = NULL;
		CProgMaq* pPM = NULL;
		
		CCpwDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		CProg* pProg = pDoc->GetpProg();
		ASSERT_VALID(pProg);
		
		
		CObject* pObject = ObjetoApuntado(p_Point);
		if (s_eObjSel == Nada)
		{
			pDoc->SetBMoviendo(FALSE);
			pProg->UnselectAll();	
			Invalidate();
			return;
		}
		if (s_eObjSel == EditCal)
		{
			pDoc->SetBMoviendo(FALSE);
			CFecTur* pFecTur = (CFecTur*) pObject;
			g_pCache->SetTurnos(*pFecTur, m_pSelPM );
			g_pCache->AssignInacts(*m_pSelPM);
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
		if (s_eObjSel == Orden)
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
	
	
	CScrollView::OnLButtonDown(p_uFlags, p_Point);
}

void
CCpwView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CMenu PopUp;
	PopUp.CreatePopupMenu();
	CMenu PopUpAlterMaq;
	PopUpAlterMaq.CreatePopupMenu();
	CMenu PopUpGFH;
	PopUpGFH.CreatePopupMenu();
	COrdMaq* pOM = 0;
	CProgMaq* pPM = 0;

	UINT tTipo = MF_ENABLED | MF_STRING;
	CObject* pObj = ObjetoApuntado( point );	                                           	
	if (pObj == NULL) return;
	if (pObj->IsKindOf(RUNTIME_CLASS ( COrdMaq )))
	{
		m_pSelOM = (COrdMaq*) pObj;
		m_pSelPM = NULL;
		PopUp.AppendMenu(tTipo, ID_EDITAR_ORDEN, "Expandir...");
		PopUp.AppendMenu(tTipo, ID_MOVER_ORDEN, "Mover");
		PopUp.AppendMenu(tTipo, ID_NUEVA_ORDEN, "Nuevo Bloque...");
		PopUp.AppendMenu(tTipo, ID_DIVIDIR_ORDEN, "Dividir...");
		COrdMaq* pOMNext = m_pSelOM->Next();
		if (pOMNext != m_pSelOM && pOMNext->GetsMolde() == m_pSelOM->GetsMolde()
			&& (m_pSelOM->GetsID() == pOMNext->GetsID()  || 
				g_pCache->EsMoldeComun(m_pSelOM->GetsID(), pOMNext->GetsID() ) ))
		{
			PopUp.AppendMenu(tTipo, ID_UNIR_ORDEN, "Unir");
		}
		PopUp.AppendMenu(tTipo, ID_SELECT_END, "Selec. Fin");
	} else if (pObj->IsKindOf(RUNTIME_CLASS ( CProgMaq )))
	{
		m_pSelOM = NULL;
		m_pSelPM = (CProgMaq*) pObj;
		PopUp.AppendMenu(tTipo, ID_CONSART_MAQ, "Articulos");
		PopUp.AppendMenu(tTipo, ID_RECALC_MAQ, "Recalcular");
		PopUp.AppendMenu(tTipo, ID_NUEVA_ORDEN, "Nuevo Bloque...");
		PopUp.AppendMenu(tTipo, ID_EDITAR_CALEND, "Calendario");
		PopUp.AppendMenu(tTipo, ID_CAMBIAR_PERSONAS, "Personas...");
		PopUpAlterMaq.AppendMenu(tTipo, ID_BORRAR_MAQ, "Borrar");
		PopUpAlterMaq.AppendMenu(tTipo, ID_INSERT_MAQ, "Insertar...");
		PopUpAlterMaq.AppendMenu(tTipo, ID_LEFT_MAQ, "Izquierda");
		PopUpAlterMaq.AppendMenu(tTipo, ID_RIGHT_MAQ, "Derecha");
		PopUpGFH.AppendMenu(tTipo, ID_CARGA_GFH, "Cargar");
		PopUpGFH.AppendMenu(tTipo, ID_REPROG_GFH, "Reprogramar");
		//PopUpGFH.AppendMenu(tTipo, ID_CALEND_GFH, "Calendario");
		
		PopUp.AppendMenu(MF_STRING | MF_POPUP, (UINT) PopUpAlterMaq.m_hMenu, "Alterar");
		if (m_pSelPM->GetlGFH() != TK_GFH_INYECCION)
			PopUp.AppendMenu(MF_STRING | MF_POPUP, (UINT) PopUpGFH.m_hMenu, m_pSelPM->GetsGFHDesc());
		
	} else if (pObj->IsKindOf(RUNTIME_CLASS ( CFecTur )))
	{
		// Si es una fecha-turno es que se esta editando el calendario y el
		// click es en zona de calendario.

	}
	
	
	CPoint ScrPoint(point);
	ClientToScreen(&ScrPoint);
	PopUp.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ScrPoint.x, ScrPoint.y, this);
	
	CScrollView::OnRButtonDown(nFlags, point);
}

void
CCpwView::OnEditarOrden()
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	
	if (m_pSelOM == NULL)
	{          
		MessageBox(STRID(IDS_ERR_NOORDSEL), STRID(IDS_ERROR));
		return;
	}
	COrdenDlg OrdenDlg(m_pSelOM, pProg, this);
	
	OrdenDlg.m_BReadOnly = pDoc->GetBReadOnly();
	
	OrdenDlg.m_sCantidad = FormatLong(m_pSelOM->GetlCantidad(), 9);
	OrdenDlg.m_sCantTot = FormatLong(m_pSelOM->GetlCantTot(), 9);
	OrdenDlg.m_sTiradas = FormatLong(m_pSelOM->GetlTiradas(), 9);
	OrdenDlg.m_FecEnt = m_pSelOM->GetFecEnt();
	OrdenDlg.m_FecFin =	m_pSelOM->GetFecFin();
	OrdenDlg.m_FecIni = m_pSelOM->GetFecIni();

	OrdenDlg.m_lLote = m_pSelOM->GetlLote();
	OrdenDlg.m_sChatarras.Format("%.1lf", m_pSelOM->GetdChatarras());
	OrdenDlg.m_sTiempo.Format("%.2f", (double) ((double)m_pSelOM->GetTimeSpan().GetTotalSeconds() / TK_SEGS_HORA));
	OrdenDlg.m_sTiempoTot.Format("%.2f", (double) ((double)m_pSelOM->GetTotalTs().GetTotalSeconds() / TK_SEGS_HORA));
	OrdenDlg.m_BFecFija = m_pSelOM->GetBFija();
	BOOL bFecFijaIni = OrdenDlg.m_BFecFija;
	OrdenDlg.m_sCadencia.Format("%.2lf ", m_pSelOM->GetdCadenciaGFH());
	OrdenDlg.m_sPersonas.Format("%.2lf ", m_pSelOM->GetdPersonas());
	OrdenDlg.m_BTiempoFijo = m_pSelOM->GetBTiempoFijo();
	BOOL bTiempoFijoIni = OrdenDlg.m_BTiempoFijo;
	OrdenDlg.m_BSelected = m_pSelOM->GetBSelected();
	CBufArticulo* pBufArticulo = m_pSelOM->GetpBufArticulo();
	OrdenDlg.m_sCavidades = FormatLong(m_pSelOM->GetlCavidades(),5);
	OrdenDlg.m_sMezcla = m_pSelOM->GetsMezcla();
	OrdenDlg.m_sMezclaKilos = m_pSelOM->GetsMezclaKilos();
	long lConsumo = (long) m_pSelOM->GetdCadenciaTL(Mezcla);
	OrdenDlg.m_sConsumo = FormatLong(lConsumo,7);
	OrdenDlg.m_sPesoGramos.Format("%.1lf", m_pSelOM->GetdPeso());
	OrdenDlg.m_sRestHoras = FormatLong((long) m_pSelOM->GetTsMezcla().GetTotalHours(), 7);

	OrdenDlg.m_sRestCant = FormatLong(m_pSelOM->GetlRestoCantConMezcla(), 11);

	if (m_pSelOM->GetlCantFab())
	{
		OrdenDlg.m_sFabricadas = FormatLong(m_pSelOM->GetlCantFab(), 6);
		OrdenDlg.m_FecFab = m_pSelOM->GetFecFab();
		OrdenDlg.m_sRestantes = FormatLong(m_pSelOM->GetlCantidad(), 6);
	} else {
		OrdenDlg.m_sFabricadas = "";
		OrdenDlg.m_FecFab = CTime::GetCurrentTime();
		OrdenDlg.m_sRestantes = "";
	}
	OrdenDlg.m_sInactividad.Format("%.1lf %%", m_pSelOM->GetdInactividad());
	OrdenDlg.m_sCadenciaAcabado = FormatDbl(g_pCache->GetdCadenciaAcabado(m_pSelOM->GetsID()), 7,1 );


	
	CString sTemp = m_pSelOM->GetsNombre() + ' '
				+ " (" + m_pSelOM->GetsID() + ')';
	if (m_pSelOM->GetpPM() != NULL)
	{
		sTemp += " en " + m_pSelOM->GetpPM()->GetsID();
	}
	OrdenDlg.m_sCaption = sTemp;
	
	if (OrdenDlg.DoModal() != IDOK) 
	{
		pDoc->UpdateAllViews(NULL);	
		return;
	}

	if (bFecFijaIni != OrdenDlg.m_BFecFija)
	{
		CWaitCursor WaitCursor;
		m_pSelOM->SetBFija(OrdenDlg.m_BFecFija);
		//pProg->Recalc();
		m_pSelOM->RecalcFin();
	}
	if (bTiempoFijoIni != OrdenDlg.m_BTiempoFijo)
	{
		CWaitCursor WaitCursor;
		m_pSelOM->SetBTiempoFijo(OrdenDlg.m_BTiempoFijo);
		//pProg->Recalc();
		m_pSelOM->RecalcFin();
	}
	
	pDoc->UpdateAllViews(NULL);
	
}

void 
CCpwView::SetSelec(CObject* p_pObjSel, enum EObjSel p_eObjSel)
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	pDoc->SetSelec(p_pObjSel, p_eObjSel);
	s_eObjSel = p_eObjSel;
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
		
CObject* 
CCpwView::ObjetoApuntado( CPoint p_Pnt )
{            
	s_eObjSel = Nada;
	static CFecTur FecTur;	// Por si hay que devolver un FecTur
	// Si no tenemos un documento construido, pasamos
	if (m_ColWidth == 0) return NULL;
	
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	
	long lTimeScale = pProg->GetTimeScale();   
	
	// El punto que entra es relativo. Primero lo convertimos
	// a absoluto.
	CPoint PntAbs = p_Pnt;
	PntAbs.Offset(GetDeviceScrollPosition());
	// Primero encontramos en qu� m�quina se encuentra
	if ( PntAbs.x < TK_POINTS_COL ) return NULL;
	int indMaq = (int) ((PntAbs.x - TK_POINTS_COL) / m_ColWidth );
	if (indMaq >= pProg->GetNumPM()) 
		return NULL;
	else if ( p_Pnt.y < m_HeadHeight ) 		// cogemos el relativo a prop�sito
	{		// Devolver la maquina
		SetSelec( pProg->GetpPM(indMaq) , Maquina) ;
		return m_pSelPM;
	}
	// Parece que va a ser una orden. 
	int DesplCol = PntAbs.x - ( indMaq * m_ColWidth) - TK_POINTS_COL;
	if ( DesplCol > TK_EDITCAL_XDESP  && 
		DesplCol < ( TK_EDITCAL_XDESP+ TK_EDITCAL_W))
	{	// Comprobamos por si est�n editando el calendario
		CProgMaq* pPM = pProg->GetpPM(indMaq);
		if (pPM->GetBEditCal())
		{
			m_pSelPM = pPM;
			long lDespl = (long)((PntAbs.y - m_HeadHeight) * lTimeScale);
			CTime Fec = pProg->GetFecIni() + (CTimeSpan) lDespl;
			CString sDeb = FormatFec(Fec, FecHora);
			FecTur = Fec;
			CString sDeb2 = FormatFec(FecTur.GetFec(), FecHora);
			SetSelec(NULL, EditCal);
			return &FecTur;
		}
		//return NULL;
	}
	CProgMaq* pPM = pProg->GetpPM(indMaq);
	ASSERT_VALID(pPM);                              
	long lDespl = (long)((PntAbs.y - m_HeadHeight) * lTimeScale);
	CTime Fec = pProg->GetFecIni() + (CTimeSpan) lDespl;
	
	for (int i=pPM->GetNumOM()-1; i >= 0; i--)
	{
		COrdMaq* pOM = pPM->GetpOM(i);
		ASSERT_VALID(pOM);
		if ((Fec >= pOM->GetFecIni() && Fec <= pOM->GetFecFin()
			&& DesplCol > m_SepWidth / 2 && DesplCol < (m_ColWidth - m_SepWidth/2))
			||		// Si es fija hay que considerar tambi�n el triangulo
			( pOM->GetBFija() && Fec >= pOM->GetFecIni() 
			&& Fec <= pOM->GetFecIni() + (CTimeSpan) (m_SepWidth / 2)+1
			&& DesplCol >= 0 && DesplCol <= (m_SepWidth / 4)+1) )
		{
			// Devolvemos la orden
			SetSelec( pOM , Orden );
			return pOM;
		}
	}
	SetSelec (NULL, Nada);
	return NULL;
}    


void CCpwView::OnBuscarOrden() 
{
	ASSERT_VALID(g_pCache);
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	CDlgBuscarOrden DlgBuscarOrden;
	                                              
		                                              
	if (DlgBuscarOrden.DoModal() != IDOK)
	{
		pDoc->UpdateAllViews(NULL);
		return;
	}	                       
	CString sSel = DlgBuscarOrden.m_sOrden;
	int iSp = sSel.Find(' ');
	if (iSp >= 0) sSel = sSel.Left(iSp);

	CBufOF* pOF = g_pCache->FindOFNum( atol(sSel) );	                       
	if (pOF == NULL)
	{
		MessageBox(STRID(IDS_ERR_ORDNOTF),STRID(IDS_WARNING));
		return;
	}          
	CObArray ArrOM;
	
	if (pProg->GetArrOM(&ArrOM, pOF) > 0)
	{
		COrdMaq* pOM = (COrdMaq*) ArrOM.GetAt(ArrOM.GetUpperBound());
		ASSERT_VALID(pOM);
		m_pSelOM = pOM; 
		PostMessage(WM_COMMAND, ID_EDITAR_ORDEN); 
	} else
	{                  
		m_pSelOM = NULL;
		MessageBox(STRID(IDS_ERR_OFNOOM), STRID(IDS_WARNING));
	}	
	pDoc->UpdateAllViews(NULL);	

	
}


void CCpwView::OnLButtonDblClk(UINT nFlags, CPoint p_Point) 
{
	CScrollView::OnLButtonDblClk(nFlags, p_Point);
	CObject* pObj = ObjetoApuntado( p_Point );   	
	if (pObj == NULL) return;
	if (pObj->IsKindOf(RUNTIME_CLASS ( COrdMaq )))
	{
		m_pSelOM = (COrdMaq*) pObj;
		m_pSelPM = NULL;
		CChildFrame* pFrame = (CChildFrame*) GetParentFrame();
		ASSERT (pFrame->IsKindOf( RUNTIME_CLASS( CChildFrame ) ));
		pFrame->SearchOF(m_pSelOM->GetsID());
		
	} else if (pObj->IsKindOf(RUNTIME_CLASS ( CProgMaq )))
	{
		m_pSelOM = NULL;
		m_pSelPM = (CProgMaq*) pObj;
	}

}


BOOL CCpwView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CCpwDoc* pDoc = GetDocument();
	if (pDoc && pDoc->GetBMoviendo())
	{
		HCURSOR	HCursor = AfxGetApp()->LoadCursor( IDC_MOVE );
		SetCursor( HCursor );
		return TRUE;
	} else
		return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
// CCpwView printing

BOOL CCpwView::OnPreparePrinting(CPrintInfo* pInfo)
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);

	CTime FecFin = pProg->GetFecFin();
	//pApp->WriteProfileInt("Dialogo de Impresion", "HastaFecYY", FecFin.GetYear());
	//pApp->WriteProfileInt("Dialogo de Impresion", "HastaFecMM", FecFin.GetMonth());
	//pApp->WriteProfileInt("Dialogo de Impresion", "HastaFecDD", FecFin.GetDay());

	CDlgPrint DlgPrint(pProg);
	if (DlgPrint.DoModal() != IDOK || DlgPrint.m_iTipoListado < 1) 
	{
		pDoc->UpdateAllViews(NULL);	

		return FALSE;
	}

	pDoc->UpdateAllViews(NULL);	

	
	m_PI.m_sDesdeGFH = DlgPrint.m_sDesdeGFH;
	m_PI.m_sHastaGFH = DlgPrint.m_sHastaGFH;
	m_PI.m_sDesdeMaq = DlgPrint.m_sDesdeMaq;
	m_PI.m_sHastaMaq = DlgPrint.m_sHastaMaq;
	/*m_PI.m_Abs.SetFecIni( CTime( DlgPrint.m_FecDesde.GetYear(),
								 DlgPrint.m_FecDesde.GetMonth(),
								 DlgPrint.m_FecDesde.GetDay(), TK_TURNO3_FIN,0,0));
	*/
	CString sDeb = FormatFec(DlgPrint.m_FecDesde, Fecha);
	m_PI.m_Abs.SetFecIni( CTurnos::GetFecIni(DlgPrint.m_FecDesde, 1));
	sDeb = FormatFec(DlgPrint.m_FecDesde, Fecha);
	
	/*
	m_PI.m_Abs.SetFecFin( AddDays(CTime( DlgPrint.m_FecHasta.GetYear(),
								 DlgPrint.m_FecHasta.GetMonth(),
								 DlgPrint.m_FecHasta.GetDay(), TK_TURNO3_FIN,0,0), 1));
	*/
	sDeb = FormatFec(DlgPrint.m_FecHasta, Fecha);
	m_PI.m_Abs.SetFecFin( CTurnos::GetFecIni(DlgPrint.m_FecHasta, 1));

	m_PI.m_iTipoListado = DlgPrint.m_iTipoListado;
	// default preparation
	return DoPreparePrinting(pInfo);
}
	
void CCpwView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_PI.m_iTipoListado == 1)	// Seleccionado tipo de Listado 1 (Gr�fico)
		BeginPrintingGrafico(pDC, pInfo);
	else if (m_PI.m_iTipoListado == 2)	// Seleccionado tipo de Listado 2 (Programa de M�quina)
		BeginPrintingProgMaq(pDC, pInfo);
	else if (m_PI.m_iTipoListado == 3)	// Seleccionado tipo de Listado 3 (Cambios de Moldes)
		BeginPrintingMoldes(pDC, pInfo);

}

void CCpwView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_PI.m_iTipoListado == 1)	// Seleccionado tipo de Listado 1 (Gr�fico)
		EndPrintingGrafico(pDC, pInfo);
	else if (m_PI.m_iTipoListado == 2)	// Seleccionado tipo de Listado 2 (Programa de M�quina)
		EndPrintingProgMaq(pDC, pInfo);
	else if (m_PI.m_iTipoListado == 3)	// Seleccionado tipo de Listado 3 (Cambio de Moldes)
		EndPrintingMoldes(pDC, pInfo);

}

void CCpwView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	if (m_PI.m_iTipoListado == 1)	// Seleccionado tipo de Listado 1 (Gr�fico)
		PrintGrafico(pDC, pInfo);
	else if (m_PI.m_iTipoListado == 2)	// Seleccionado tipo de Listado 2 (Programa de M�quina)
		PrintProgMaq(pDC, pInfo);
	else if (m_PI.m_iTipoListado == 3)	// Seleccionado tipo de Listado 3 (Cambio de Moldes)
		PrintMoldes(pDC, pInfo);
}

void 
CCpwView::OnConsArtMaq() 
{
	CWaitCursor WaitCursor;
	CObArray ArrInfoMolArt;
	
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
		
	// Ojo, GetArrMolArt crea objetos en ArrInfoMolArt, se borran luego 
	if (!g_pCache->GetArrMolArt(m_pSelPM->GetsID(), ArrInfoMolArt))
	{
		MessageBox("No hay ning�n art�culo para esta m�quina dentro de las OFs visualizadas");
		return;
	}

	CTextView* pTextView = pDoc->GetpTextView();
	if (!pTextView) return;
	CStringArray ArrsIDArt;
	// Hacemos una lista con los art�culos que se est�n viendo
	pTextView->GetArrsIDArt(ArrsIDArt);
	
	BOOL bEncontrado;
	for (int i = 0; i < ArrInfoMolArt.GetSize(); i++)
	{
		CInfoMolArt* pInfoMolArt = (CInfoMolArt*) ArrInfoMolArt.GetAt(i);
		ASSERT( pInfoMolArt->IsKindOf( RUNTIME_CLASS( CInfoMolArt ) ) );
 
		bEncontrado = FALSE;		
		for (int j = 0; j < ArrsIDArt.GetSize(); j++)
		{
			if (pInfoMolArt->m_sIDArticulo == ArrsIDArt.GetAt(j))
			{
				bEncontrado = TRUE;
				break;
			}
		}
		// Borramos los que no est�n en la matriz de las OFs, porque no nos interesan
		if (!bEncontrado) 
		{
			CInfoMolArt* pInfoMolArt = (CInfoMolArt*) ArrInfoMolArt.GetAt(i);
			delete pInfoMolArt;
			ArrInfoMolArt.RemoveAt(i);
			i--;
		}
	}
	
	CDlgLista DlgLista(&ArrInfoMolArt);
	CChildFrame* pFrame = (CChildFrame*) GetParentFrame();
	ASSERT (pFrame->IsKindOf( RUNTIME_CLASS( CChildFrame ) ));
	DlgLista.m_pFrame = pFrame;
	DlgLista.m_pProg = pProg;
	DlgLista.m_pDoc = pDoc;
	DlgLista.DoModal();
	pDoc->UpdateAllViews(NULL);

	for (int i = 0; i < ArrInfoMolArt.GetSize(); i++)
	{
		CInfoMolArt* pInfoMolArt = (CInfoMolArt*) ArrInfoMolArt.GetAt(i);
		ASSERT( pInfoMolArt->IsKindOf( RUNTIME_CLASS( CInfoMolArt ) ) );
		delete pInfoMolArt;
	}
	ArrInfoMolArt.RemoveAll();

}

void 
CCpwView::OnRecalcMaq() 
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	m_pSelPM->Recalc();
	pDoc->SetModifiedFlag();
	Invalidate();
}

void 
CCpwView::OnBorrarMaq() 
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);

	if (m_pSelPM->GetNumOM())
	{
		if (MessageBox(STRID(IDS_CONF_MAQNOVAC), STRID(IDS_CONF_CAPTION), 
			MB_YESNOCANCEL ) != IDYES)
		return;
	}
	int iPM = pProg->Find(m_pSelPM);

	pProg->DeletePM(iPM);

	pDoc->SetModifiedFlag();
	m_FontChanged = Cambiado;	// Para que se recalculen los scrollers
	Invalidate();

}

void 
CCpwView::OnInsertMaq() 
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);

	
	CDlgInsMaq DlgInsMaq;

	if (DlgInsMaq.DoModal() == IDOK)
	{
		if (pProg->FindMaq(DlgInsMaq.m_sInsMaq) != NULL)
		{
			MessageBox(STRID(IDS_WAR_NOINS), STRID(IDS_WARNING));
			return;
		}
		CBufMaquina* pBufMaquina = g_pCache->AddMaquina( DlgInsMaq.m_sInsMaq );
		if (!pBufMaquina) {
			MessageBox("La m�quina a insertar no existe en la base de datos. Hay que crearla antes de poderla insertar.", STRID(IDS_WARNING));
			return;
		}
		int iPM = pProg->Find(m_pSelPM);
		if (DlgInsMaq.m_iAntes == 1) iPM++;
		
		CProgMaq* pPM = new CProgMaq(DlgInsMaq.m_sInsMaq);
		pPM->SetpBufMaq( pBufMaquina );  
	
		pProg->AddPM (pPM, iPM);

		// Ya hemos insertado la m�quina, pero tenemos que ver si la hemos metido 
		// en medio de otro GFH o separada de su GFH
		BOOL BBienInsertada = TRUE;
		long lGFH = pPM->GetlGFH();
		int iIni, iFin;
		iIni = -1;
		for (int i = 0; i < pProg->GetNumPM(); i++)
		{
			CProgMaq* pPMi = pProg->GetpPM(i);
			if (i != iPM && pPMi->GetlGFH() == lGFH)
			{
				if (iIni == -1) iIni = i;
				iFin = i;
			}
		}
		if (iIni == -1)		// Esto es que no hay m�s de su GFH
		{
			// Ahora hemos de comprobar que no est� metido dentro de otro GFH
			if (iPM == 0 || iPM == pProg->GetNumPM() - 1) // Si es primero o �ltimo no puede ser
				;
			else
			{
				CProgMaq* pPMPrev = pProg->GetpPM(iPM-1);
				long lGFHPrev = pPMPrev->GetlGFH();
				CProgMaq* pPMSig= pProg->GetpPM(iPM+1);
				long lGFHSig = pPMSig->GetlGFH();
				if (lGFHPrev == lGFHSig ) // Si los GFH de las m�quinas de los lados son iguales
					BBienInsertada = FALSE;	// es que nos hemos metido en medio de donde nadie nos llamaba
			}

		} else // si hay m�s de si GFH, recogidas en los �ndices iIni e iFin
		{
			if ( (iPM + 1) == iIni ||		// Si esta al comienzo del GFH
				 (iPM - 1) == iFin ||		// O esta al final
				 (iPM > iIni && iPM < iFin) ) // O esta en el medio
				 ;		// Todo va bien
			else //en caso contrario se rechaza
				BBienInsertada = FALSE;
		}		


		if (!BBienInsertada)
		{
			pProg->DeletePM(iPM);
			MessageBox(STRID(IDS_WAR_INSJUN), STRID(IDS_WARNING));
		} else
		{
			pDoc->SetModifiedFlag();
		}
	}
	pDoc->UpdateAllViews(NULL);	

}

void 
CCpwView::OnLeftMaq() 
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	
	if (!pProg->MovePM(m_pSelPM, Left))
	{
		MessageBox(STRID(IDS_ERR_NOMOV), STRID(IDS_ERROR));
	}
	pDoc->SetModifiedFlag();
	Invalidate();
}

void 
CCpwView::OnRightMaq() 
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	
	if (!pProg->MovePM(m_pSelPM, Right))
	{
		MessageBox(STRID(IDS_ERR_NOMOVULT), STRID(IDS_ERROR));
	}
	pDoc->SetModifiedFlag();
	Invalidate();
}

void 
CCpwView::OnSelectEnd() 
{
	ASSERT_VALID(m_pSelOM);
	CProgMaq* pPM = m_pSelOM->GetpPM();
	ASSERT_VALID(m_pSelOM);
	int iSel = pPM->Find(m_pSelOM);

	for (int i = iSel; i < pPM->GetNumOM(); i++)
	{
		COrdMaq* pOM = pPM->GetpOM(i);
		ASSERT_VALID(pOM);
		pOM->SetBSelected(TRUE);
	}
	Invalidate();
}

void 
CCpwView::OnCargaGFH() 
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	ASSERT_VALID(m_pSelPM);
	CProg* pProg = m_pSelPM->GetpProg();
	ASSERT_VALID(pProg);
	if (m_pSelPM->GetlGFH() == TK_GFH_INYECCION)
	{
		//OnCargainicial();
	}
	else
	{
		pProg->CargaGFH(m_pSelPM->GetlGFH());	
	}
	
	pDoc->SetModifiedFlag();
	Invalidate();

}

void 
CCpwView::OnReprogGFH() 
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	ASSERT_VALID(m_pSelPM);
	CProg* pProg = m_pSelPM->GetpProg();
	ASSERT_VALID(pProg);
	pProg->ReprogramaGFH(m_pSelPM->GetlGFH());	
	
	pDoc->SetModifiedFlag();
	Invalidate();

}

void CCpwView::OnTipoLetra() 
{
	LOGFONT LogFontAct;
	CWinApp* wApp = AfxGetApp();
	ASSERT_VALID(wApp);
	
	LogFontAct.lfHeight = wApp->GetProfileInt("Font", "Height", -10);
	LogFontAct.lfHeight = wApp->GetProfileInt("Font", "Width", 0);
	LogFontAct.lfEscapement = wApp->GetProfileInt("Font", "Escapement", 0);
	LogFontAct.lfOrientation = wApp->GetProfileInt("Font", "Orientation", 0);
	LogFontAct.lfWeight = wApp->GetProfileInt("Font", "Weight", FW_NORMAL);
	LogFontAct.lfItalic = wApp->GetProfileInt("Font", "Italic", 0);
	LogFontAct.lfUnderline = wApp->GetProfileInt("Font", "Underline", 0);
	LogFontAct.lfStrikeOut = wApp->GetProfileInt("Font", "StrikeOut", 0);
	LogFontAct.lfCharSet = wApp->GetProfileInt("Font", "CharSet", ANSI_CHARSET);
	LogFontAct.lfOutPrecision = wApp->GetProfileInt("Font", "OutPrecision", OUT_CHARACTER_PRECIS);
	LogFontAct.lfClipPrecision = wApp->GetProfileInt("Font", "ClipPrecision", CLIP_CHARACTER_PRECIS);
	LogFontAct.lfQuality = wApp->GetProfileInt("Font", "Quality", DEFAULT_QUALITY);
	LogFontAct.lfPitchAndFamily = wApp->GetProfileInt("Font", "PitchAndFamily", FIXED_PITCH);
	strncpy(LogFontAct.lfFaceName, wApp->GetProfileString("Font", "FaceName", "FIXEDSYS"), sizeof(LogFontAct.lfFaceName));
	
	
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
	wApp->WriteProfileInt("Font", "Height", LogFont.lfHeight);
	wApp->WriteProfileInt("Font", "Width", LogFont.lfWidth);
	wApp->WriteProfileInt("Font", "Escapement", LogFont.lfEscapement);
	wApp->WriteProfileInt("Font", "Orientation", LogFont.lfOrientation);
	wApp->WriteProfileInt("Font", "Weight", LogFont.lfWeight);
	wApp->WriteProfileInt("Font", "Italic", LogFont.lfItalic);
	wApp->WriteProfileInt("Font", "Underline", LogFont.lfUnderline);
	wApp->WriteProfileInt("Font", "StrikeOut", LogFont.lfStrikeOut);
	wApp->WriteProfileInt("Font", "CharSet", LogFont.lfCharSet);
	wApp->WriteProfileInt("Font", "OutPrecision", LogFont.lfOutPrecision);
	wApp->WriteProfileInt("Font", "ClipPrecision", LogFont.lfClipPrecision);
	wApp->WriteProfileInt("Font", "Quality", LogFont.lfQuality);
	wApp->WriteProfileInt("Font", "PitchAndFamily", LogFont.lfPitchAndFamily);
	wApp->WriteProfileString("Font", "FaceName", LogFont.lfFaceName);
	


	Invalidate();


}


void CCpwView::OnTest() 
{
	return;

	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CColorDialog ColorDialog;

	ColorDialog.DoModal();
}


void CCpwView::OnCargaAntiguo() 
{
	return;

	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);

	for (int i=0; i< pProg->GetNumPM(); i++)
	{                                  
		CProgMaq* pPM = (CProgMaq *) pProg->GetpPM(i);
		ASSERT( pPM->IsKindOf( RUNTIME_CLASS( CProgMaq ) ) );
   		if (pPM->GetNumOM())
		{
			MessageBox(STRID(IDS_ERR_PROGNOVAC), STRID(IDS_ERROR));
			return;
		}
	}   
	// Pedimos el fichero
	
	CString sExtDef = STRID(IDS_DEF_EXT);
	CString sExt = AfxGetApp()->GetProfileString("Valores Iniciales", "Extension", sExtDef);
	AfxGetApp()->WriteProfileString("Valores Iniciales", "Extension", sExt);

	CString sDlgDef = STRID(IDS_DEF_OPENDLG);
	CString sDlg = AfxGetApp()->GetProfileString("Valores Iniciales", "Open File", sDlgDef);
	AfxGetApp()->WriteProfileString("Valores Iniciales", "Open File", sDlg);

	CFileDialog DlgFile( TRUE, 
						sExt, 
						NULL, 
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						sDlg);
	
	if (DlgFile.DoModal() == IDCANCEL) return;
	

	CWaitCursor WaitCursor;
	//pDoc->CargarPrograma(DlgFile.GetPathName());	

	pDoc->SetModifiedFlag();
	Invalidate();

}

void CCpwView::OnListarMaq() 
{
	return;
	CWinApp* wApp = AfxGetApp();
	ASSERT_VALID(wApp);
	wApp->WriteProfileInt("Dialogo de Impresion", "TipoListado", 1);
	wApp->WriteProfileString("Dialogo de Impresion", "DesdeMaq", m_pSelPM->GetsID());
	wApp->WriteProfileString("Dialogo de Impresion", "HastaMaq", m_pSelPM->GetsID());
	PostMessage(WM_COMMAND, ID_FILE_PRINT);
}

void CCpwView::OnListarGraf() 
{
	return;
	CWinApp* wApp = AfxGetApp();
	ASSERT_VALID(wApp);
	wApp->WriteProfileInt("Dialogo de Impresion", "TipoListado", 0);
	PostMessage(WM_COMMAND, ID_FILE_PRINT);
}

void CCpwView::OnVerifStruct() 
{
	return; 
	
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);

	(void) pProg->VerifStruct();
	
}

void CCpwView::OnMouseMove(UINT p_uFlags, CPoint p_Point) 
{
	CString sMsg;
	CObject* pObject = ObjetoApuntado(p_Point);
	CFrameWnd* pFrame = GetParentFrame();
	if (s_eObjSel == Orden)
	{

		COrdMaq* pOM = (COrdMaq*) pObject;
		ASSERT_VALID(pOM);
		ASSERT (pOM->IsKindOf(RUNTIME_CLASS ( COrdMaq )));
		
		if (p_uFlags & MK_CONTROL) 	// Ctrl (Posteriores usos)
		{

		}
		sMsg.Format("%-9.9s %s", pOM->GetsNombre(), pOM->GetsInfo(true));
		pFrame->SetMessageText(sMsg);

		
	} else
	{
		pFrame->SetMessageText(AFX_IDS_IDLEMESSAGE);
	}
		
	CScrollView::OnMouseMove(p_uFlags, p_Point);
}


void CCpwView::OnEditPaste() 
{
	// Lo que se pegue deben ser ordenes. Vendr�n en principio de Access con el formato:
/*
Blq.	Num.	Art.	Nombre	O.F.	Cantidad	Entrega	Pers.Max.	Traspaso	Celula	Horas	H. Aux
9947	100	53413	CPM 606	33341	800	22/11/99	3	01/01/99	5	17.1111111111111	0
9947	110	53415	CPM 676	33343	800	22/11/99	3	01/01/99	7	20.6666666666667	0
9947	120	53426	CPM 1231	33452	400	23/11/99	3	01/01/99	7	10.6666666666667	0
9947	130	53428	CPM 1251	33453	400	23/11/99	3	01/01/99	8	10.6666666666667	0
9947	140	53427	CPM 241	33455	400	23/11/99	4	01/01/99	3	13.8888888888889	0
9947	150	53355	CFM 60501	33479	600	24/11/99	3	01/01/99	2	16.3333333333333	1288490188.2
9947	160	55713	ZNS 15702	33718	1250	22/11/99	3	01/01/99	6	25	0
 etc.
*/	
	
	CWaitCursor WaitCursor;
	
	// Comprobamos que es texto ANSI o OEM	
	if (!IsClipboardFormatAvailable(CF_TEXT) && !IsClipboardFormatAvailable(CF_OEMTEXT)) 
		return; 
	// Abrimos el Clibpoard
	if (!OpenClipboard()) 
		return; 
	// Consieguimos el puntero al texto
	HGLOBAL	hglb = ::GetClipboardData(CF_TEXT); 

	// Lo copiamos
	CString sClip = (LPCTSTR) hglb;
	// Y liberamos el clipboard
	CloseClipboard();

	// Ya tenemos el texto del clipboard metido en la CString
	// Ahora tenemos que sacar el bloque, la primera secuencia y la �ltima.
	// Vamos de l�nea en l�nea

	CString sLin = GetsLinea(sClip,  FALSE);	// Despreciamos la primera linea que trae cabeceras
	sLin = GetsLinea(sClip,  FALSE);
	CString sCopia = "";
	CString sCampo;
	long lBloque, lSerieIni, lSerieFin;
	lBloque = lSerieIni = lSerieFin = 0;
	sCampo = GetsLinea(sClip, TRUE, '\t');	// Access separa los campos por tabuladores
	lBloque = atol(sCampo);
	sCampo = GetsLinea(sClip, TRUE, '\t');	// Segundo campo de la primera linea
	lSerieIni = atol(sCampo);
	lSerieFin = lSerieIni;					// Por si solo hay una linea
	sLin = GetsLinea(sClip, TRUE);
	sLin = GetsLinea(sClip, TRUE);
	while (sLin != "")
	{
		sCopia = sLin;		// Mantenemos una copia de seguridad para tener acceso a la �ltima l�nea
		sLin = GetsLinea(sClip);
	}
	if (sCopia != "")			// Esto es que hay m�s de una l�nea
	{
		sCampo = GetsLinea(sCopia, FALSE, '\t');	// Despreciamos el primer campo con el bloque
		sCampo = GetsLinea(sCopia, TRUE, '\t');	
		lSerieFin = atol(sCampo);
	}

	
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->CargaCelulas(lBloque, lSerieIni, lSerieFin);	
	
	Invalidate();
	return; 
    
}

void CCpwView::OnCargainicial() 
{
	CWaitCursor WaitCursor;

	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	DlgCargaInicial DlgCargaIni;
	if (DlgCargaIni.DoModal() != IDOK) return;
	
	pDoc->CargaCelulas(DlgCargaIni.m_Bloque, 
					DlgCargaIni.m_DesdeOrden, DlgCargaIni.m_HastaOrden);	
	
	
	Invalidate();
	
}

void CCpwView::OnOrdenRecalcular() 
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if (m_pSelOM == NULL)
	{          
		MessageBox(STRID(IDS_ERR_NOORDSEL), STRID(IDS_ERROR));
		return;
	}

	m_pSelOM->Recalc();
	Invalidate();
}


BOOL CCpwView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	// Si se hace un scroll real, lo invalidamos todo, porque tenemos
	// las cabeceras de m�quinas y el calendario
	CPoint PointApre = GetDeviceScrollPosition();
	BOOL bRetVal =  CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
	if (PointApre != GetDeviceScrollPosition())
	{

		Invalidate();	
	}
	return bRetVal;
}

void CCpwView::OnCalendGFH()
{
	ASSERT(m_pSelPM);
	long lGFH = m_pSelPM->GetlGFH();
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	ASSERT_VALID(m_pSelPM);
	CProg* pProg = m_pSelPM->GetpProg();
	ASSERT_VALID(pProg);
	BOOL bState = !m_pSelPM->GetBEditCal();
	for (int i = 0; i < pProg->GetNumPM(); i++)
	{
		CProgMaq* pPM = pProg->GetpPM(i);
		ASSERT_VALID(pPM);
		if (lGFH == pPM->GetlGFH())
		{
			pPM->SetBEditCal(bState);
		}
	}
	Invalidate();	
}


void 
CCpwView::OnDividirOrden()
{
	CIntrLong IntrCant;
	IntrCant.m_sCaption = STRID(IDS_DLG_CANTTIT);
	IntrCant.m_sText = STRID(IDS_DLG_CANTTXT);	//Personas destinadas
	
	CString sMsg, sCap;
	if (IntrCant.DoModal() != IDOK) return;
	if (m_pSelOM->GetlCantidad() <= IntrCant.m_lValue)
	{
		sMsg = STRID( IDS_ERR_CANTEXC );
		sCap = STRID( IDS_ERROR );
		MessageBox(sMsg, sCap);
	}
	else
	{
		COrdMaq* pOM = new COrdMaq();
		*pOM = *m_pSelOM;
		CProgMaq* pPM = m_pSelOM->GetpPM();
		ASSERT_VALID(pPM);
		m_pSelOM->ModifCant(m_pSelOM->GetlCantidad() - IntrCant.m_lValue);
		pOM->ModifCant(IntrCant.m_lValue);
		pPM->InsertOM(pOM, pPM->Find(m_pSelOM)+1);
		pOM->SetBTiempoFijo(FALSE);
		pPM->Recalc();
		Invalidate();
	}
				
}			

void 
CCpwView::OnUnirOrden()
{
		
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);

	
	COrdMaq* pOMNext = m_pSelOM->Next();
//	if (m_pSelOM->GetBPrevLink() || m_pSelOM->GetBNextLink() || pOMNext->GetBNextLink() || pOMNext->GetBPrevLink() )
//	{
//		MessageBox("No se pueden unir bloques combinados (2 piezas con el mismo molde)");
//		return;
//	}

	if (pOMNext && pOMNext->GetsMolde() == m_pSelOM->GetsMolde()
		&& m_pSelOM->GetsID() == pOMNext->GetsID())
	{
		m_pSelOM->ModifCant(m_pSelOM->GetlCantidad() + pOMNext->GetlCantidad());
		pProg->DeleteOM(pOMNext);
		
		pProg->Recalc();
	} else
	{
		m_pSelOM->SetBNextLink(TRUE);
		pOMNext->SetBPrevLink(TRUE);
	}
	Invalidate();
}

void CCpwView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CChildFrame* pFrame = (CChildFrame*) GetParentFrame();
	ASSERT (pFrame->IsKindOf( RUNTIME_CLASS( CChildFrame ) ));
	pFrame->OnCharRec( nChar, nRepCnt, nFlags);	
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCpwView::OnNuevaOrden()
{

	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);


	CDlgIntrStr DlgIntrStr;
	DlgIntrStr.m_sCaption = STRID(IDS_DLG_ARTICTXT);
	
	CString sMsg, sCap;
	if (DlgIntrStr.DoModal() != IDOK) return;
	if (g_pCache->FindpArticulo(DlgIntrStr.m_sValue) == NULL)
	{
		sMsg = STRID( IDS_ERR_ARTNOEX ) + DlgIntrStr.m_sValue;
		sCap = STRID( IDS_ERROR );
		MessageBox(sMsg, sCap);
		return;
	}
	COrdMaq* pOM = new COrdMaq(DlgIntrStr.m_sValue);
	CProgMaq* pPM;
	int ind;
	if (m_pSelOM)
	{
		pPM = m_pSelOM->GetpPM();
		ind = pPM->Find(m_pSelOM);
	}
	else
	{
		pPM = m_pSelPM;
		ind = 0;
	}
	pPM->InsertOM(pOM, ind);
	m_pSelOM = pOM;

	OnEditarOrden();
}

void 
CCpwView::OnCambiarPersonas() 
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	
	
	CIntrDbl	IntrDbl;
	
	IntrDbl.m_sCaption = "Personas para bloques seleccionados";
	IntrDbl.m_sText = "Personas : ";
	double dVal = 0;
	if (m_pSelPM)
	{
		dVal = m_pSelPM->GetdPersonas();
	} else
	{
		if (m_pSelOM)
		{
			CProgMaq* pPM = m_pSelOM->GetpPM();
			dVal = pPM->GetdPersonas();
		}
	}
	
	IntrDbl.m_dValue = dVal;
	if (IntrDbl.DoModal() != IDOK)
		return;
	
	int lim = pProg->GetNumPM();
	
	CWaitCursor WaitCursor;
	
	for(int i = 0; i < lim; i++)
	{
		CProgMaq* pPM = pProg->GetpPM(i);
		ASSERT_VALID(pPM);
		for(int j = 0; j < pPM->GetNumOM(); j++)
		{
			COrdMaq* pOM = pPM->GetpOM(j);
			// Si est� seleccionada
			if (pOM->GetBSelected())	
			{
				pOM->SetdPersonas(IntrDbl.m_dValue);
				
			} // Si est� seleccionada 
		} // Ciclo de OMs
	} // Ciclo de PMs
	
	pProg->Recalc();
	pDoc->SetModifiedFlag();
	Invalidate();
}


BOOL CCpwView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	BOOL bHasHorzBar, bHasVertBar;
	CheckScrollBars(bHasHorzBar, bHasVertBar);
	if (!bHasVertBar && !bHasHorzBar)
		return FALSE;

	BOOL bResult = FALSE;
	UINT uWheelScrollLines;
	::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &uWheelScrollLines, 0);
	int nDisplacement;


	// we keep a member variable m_nAccumulatedDelta which may be non-zero in the event the method is called with a delta too small to cause a visible scroll.
	// if this occurs we accumulate the deltas until a visible scroll occurs.
	if ((m_nAccumulatedDelta * zDelta) > 0) { // we're scrolling in the same direction and there is a non-zero value in m_nAccumulatedDelta
		m_nAccumulatedDelta += zDelta;
	}
	else {
		m_nAccumulatedDelta = zDelta;
	}
	if (bHasVertBar)
	{
		if (uWheelScrollLines == WHEEL_PAGESCROLL)
		{
			nDisplacement = -m_pageDev.cy * m_nAccumulatedDelta / WHEEL_DELTA;
		}
		else
		{
			int nToScroll = ::MulDiv(-m_nAccumulatedDelta, (uWheelScrollLines * m_lineDev.cy), WHEEL_DELTA);
			nDisplacement = min(nToScroll, m_pageDev.cy);
		}
		if (nDisplacement != 0) {
			bResult = OnScrollBy(CSize(0, nDisplacement), TRUE);
			m_nAccumulatedDelta = 0;
		}
	}
	else if (bHasHorzBar)
	{
		if (uWheelScrollLines == WHEEL_PAGESCROLL)
		{
			nDisplacement = -m_pageDev.cx * m_nAccumulatedDelta / WHEEL_DELTA;
		}
		else
		{
			int nToScroll = ::MulDiv(-m_nAccumulatedDelta, (uWheelScrollLines * m_lineDev.cx), WHEEL_DELTA);
			nDisplacement = min(nToScroll, m_pageDev.cx);
		}
		if (nDisplacement != 0) {
			bResult = OnScrollBy(CSize(nDisplacement, 0), TRUE);
			m_nAccumulatedDelta = 0;
		}
	}
	if (bResult)
		Invalidate();

	return bResult;
	//return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}
