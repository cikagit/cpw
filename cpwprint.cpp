// cpwPrint.cpp : implementation of the Printing funcions of the CCpwView class
//

#include "stdafx.h"
#include "afxdtctl.h"
#include "cpw.h"
#include "cpwDefs.h"

#include "AbSpan.h"
#include "cpwExt.h"
#include "prog.h"

#include "cpwDoc.h"

#include "cpwView.h"

#include "cpwDb.h"
#include "cache.h"            
#include "ProgMaq.h"
#include "OrdMaq.h"
#include "Dialogs.h" 
#include "OrdenDlg.h" 
#include "Turnos.h"   

#include <strstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




void CCpwView::BeginPrintingGrafico(CDC* pDC, CPrintInfo* pInfo)
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);


	m_PI.m_iNumCols = GetNumPMsInGFHs();

	// Cuantos pixels tenemos para cada una, teniendo en cuenta que 1/6
	// de columna es para separación entre ellas, y hay una escala de 1/4
	// de columna al principio
	m_PI.m_iHres = pDC->GetDeviceCaps(HORZRES); 
	m_PI.m_iVres = pDC->GetDeviceCaps(VERTRES); 
	m_PI.m_iPixCol = (int) (m_PI.m_iHres / (m_PI.m_iNumCols + .25));
	// Teniendo en cuenta que por cada columna tenemos que meter 8 carac
	// teres, calculamos la anchura del font
	int iPixReal = (int) ( m_PI.m_iPixCol * .925 );
	int iWidth = iPixReal / 10;
	// Para las máquinas calculamos 3
	int iPixRealMaq = (int) ( m_PI.m_iPixCol * .985 );
	int iWidthMaq = (int) (iPixReal / 3.5);
	// calculamos la altura del font
	double dRatio = 1.67;
	m_PI.m_iFontHeight = (int) (iWidth*dRatio) + 1;
	int iFontMaqHeight = ((int) (iWidthMaq*dRatio) + 1);
	
	if (m_PI.m_pFontNormal) 
	{
		m_PI.m_pFontNormal->DeleteObject();
		delete m_PI.m_pFontNormal;
	}
	if (m_PI.m_pFontMaq) 
	{
		m_PI.m_pFontMaq->DeleteObject();
		delete m_PI.m_pFontMaq;
	}
	if (m_PI.m_pFontNormalBold) 
	{
		m_PI.m_pFontNormalBold->DeleteObject();
		delete m_PI.m_pFontNormalBold;
	}
	if (m_PI.m_pFont90) 
	{
		m_PI.m_pFont90->DeleteObject();
		delete m_PI.m_pFont90;
	}
	
	m_PI.m_pFontNormal = new CFont;
	m_PI.m_pFontNormal->CreateFont(m_PI.m_iFontHeight , iWidth, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 
		OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, NULL); 
	m_PI.m_pFontNormalBold = new CFont;
	m_PI.m_pFontNormalBold->CreateFont(m_PI.m_iFontHeight+2, iWidth, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, 
		OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, NULL); 
	m_PI.m_pFont90 = new CFont;
	m_PI.m_pFont90->CreateFont((int) (m_PI.m_iHres/90*dRatio+1), m_PI.m_iHres/90, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, 
		OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, NULL); 
	
	
	pDC->SelectObject(m_PI.m_pFont90);
	TEXTMETRIC TM;
	pDC->GetTextMetrics(&TM);

	m_PI.m_iLinHeight = TM.tmHeight;
	m_PI.m_iCabHeight = (int) (m_PI.m_iLinHeight * 3.5);
	m_PI.m_iTitleHeight = (int) (m_PI.m_iLinHeight * 2);
	m_PI.m_iFootHeight = m_PI.m_iLinHeight;
	m_PI.m_iWidth90 = TM.tmAveCharWidth;

	if (iFontMaqHeight > (m_PI.m_iCabHeight-m_PI.m_iTitleHeight)) {
		iFontMaqHeight = (m_PI.m_iCabHeight-m_PI.m_iTitleHeight);
		iWidthMaq = (int) ((iFontMaqHeight - 1)/dRatio);
	}
	m_PI.m_pFontMaq = new CFont;
	m_PI.m_pFontMaq->CreateFont( iFontMaqHeight, iWidthMaq, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 
		OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, NULL); 
	
	
	pDC->SelectObject(m_PI.m_pFontNormal);
	pDC->GetTextMetrics(&TM);

	m_PI.m_iWidthNormal = TM.tmAveCharWidth;
	
	pDC->SelectObject(m_PI.m_pFontMaq);
	pDC->GetTextMetrics(&TM);

	m_PI.m_iWidthMaq = TM.tmAveCharWidth;
	
	long lTimeScale = pProg->GetTimeScale();   
	
	CClientDC screenDC(NULL);
	CSize screenRes(screenDC.GetDeviceCaps(LOGPIXELSX),
						  screenDC.GetDeviceCaps(LOGPIXELSY));
	CSize printRes(pDC->GetDeviceCaps(LOGPIXELSX),
						 pDC->GetDeviceCaps(LOGPIXELSY));
	m_PI.m_lConvFactor = MulDiv(lTimeScale, screenRes.cy, printRes.cy);
 	
	// Calculamos ahora cuántos día entran en cada página

	double dPixDia = (double)TK_SEGS_DIA / m_PI.m_lConvFactor;
	double dDiaPag = (double) (m_PI.m_iVres - m_PI.m_iCabHeight) / dPixDia;
	int iDaysPerPage = (dDiaPag > 1) ? (int) dDiaPag : 1;
	m_PI.m_iDaysPerPage = iDaysPerPage;

	// Y ahora cuántas páginas necesitamos
	int iNumDias = (int) ((m_PI.m_Abs.GetFecFin() - 
							m_PI.m_Abs.GetFecIni()).GetDays());
	int iMaxPg = (int) (iNumDias / iDaysPerPage) + ((iNumDias % iDaysPerPage) ? 1 : 0); 
	if (iMaxPg < 1) iMaxPg = 1;
	// Actualizamos el número de páginas

	 pInfo->SetMaxPage(iMaxPg);
  
}




void CCpwView::EndPrintingGrafico(CDC* pDC, CPrintInfo* pInfo)
{
	
}

// Listado de Máquinas

void CCpwView::EndPrintingProgMaq(CDC* pDC, CPrintInfo* pInfo)
{
	// Borramos la lista, porque se han creado las OMs nuevas, para dividir en 2 en las inactividades
	// de calendario
	int iLim = m_PI.m_ArrOMs.GetSize();
	for (int i = 0; i < iLim; i++)
	{
		COrdMaq* pOM = (COrdMaq*) m_PI.m_ArrOMs.GetAt(i);
		delete pOM;
	}
	
}

void CCpwView::PrintProgMaq(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Actualizamos los parámetros de la página 
	//AfxMessageBox("PrintPM");
	

	//if (!m_PI.m_pPM) return;
	
	//AfxMessageBox("2");

	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);

	m_PI.m_iNumOM = (pInfo->m_nCurPage - 1) * m_PI.m_iLinsPerPage;
   


	CString sLin;
	int iLinH = m_PI.m_iLinHeight;
	// Sacamos cabeceras
	pDC->SelectObject(m_PI.m_pFont90b);
	sLin.Format("                     P R O G R A M A      D E     I N Y E C C I O N");
	pDC->TextOut( 80, iLinH, sLin );
	pDC->SelectObject(m_PI.m_pFont90);

//	sLin.Format( "MAQUINA : %s", m_PI.m_pPM->GetsDescripcion());
//	pDC->TextOut( 80, 3*iLinH, sLin );
	BOOL BNuevo = FALSE;
	sLin.Format( "MAQ.  OFERTA   MEZCLA    PIEZAS    P/H   TIEMPO   FEC.INICIO");
	pDC->TextOut( 80, 6*iLinH, sLin );
	sLin.Format( "---- -------- --------   ------- ------- ------ --------------");
	pDC->TextOut( 80, 7*iLinH, sLin );
	int iLin = 1;		// Línea que vamos imprimiendo
	BOOL BPrimera = TRUE;
	// Imprimimos las líneas de detalle
	while ( GetLinProgMaq( sLin, BPrimera, BNuevo ))
	{
		BPrimera = FALSE;
		// Si es nuevo lo sacamos en negrita
		if (BNuevo)	
			pDC->SelectObject(m_PI.m_pFont90b);
		else 
			pDC->SelectObject(m_PI.m_pFont90);
	
		pDC->TextOut( 80, (iLin+7)*iLinH, sLin);
			
		// Si ya no tenemos sitio para más líneas en la página, hacemos otra
		iLin++;
		if (iLin > m_PI.m_iLinsPerPage) break;
		
	}

	// Pie
	CTime FecHoy = CTime::GetCurrentTime();
	int iYear = FecHoy.GetYear();
	int iSiglo = (iYear / 100) * 100;

	sLin.Format( "          FECHA: %2d-%2d-%2d  %2d:%2d    SUSTITUYE A:                 FIRMA:"
				,FecHoy.GetDay(), FecHoy.GetMonth(), iYear % iSiglo, 
				 FecHoy.GetHour(), FecHoy.GetMinute() );
	//pDC->TextOut( 10, ( m_PI.m_iLinsPerPage+1)*iLinH, sLin );
	pDC->TextOut(10, m_PI.m_iVres - m_PI.m_iFootHeight , sLin);
  
  
}



void CCpwView::BeginPrintingProgMaq(CDC* pDC, CPrintInfo* pInfo)
{
	//AfxMessageBox("Entrando en Impresion");
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	// Recalculamos por si acaso
	pProg->Recalc();
	//AfxMessageBox("Obteniendo OFs");
	
	int iLim = pProg->GetArrMaqOMLis(m_PI.m_ArrOMs);
	//CString sMsg;
	//sMsg.Format("Obtenidas %d OFs", m_PI.m_ArrOMs.GetSize());
	//AfxMessageBox(sMsg);
	
	// Vamos a filtrarlo para quedarnos sólo con máquinas y fechas seleccionadas
	for (int i=0; i < iLim; i++)
	{
		COrdMaq* pOM = (COrdMaq*) m_PI.m_ArrOMs.GetAt(i);
		BOOL bRepMaq = FALSE;
		if (pOM->GetFecIni() > m_PI.m_Abs.GetFecFin() || 
		 (pOM->GetFecFin() < m_PI.m_Abs.GetFecIni()) ||
		 (pOM->GetsMaquina() < m_PI.m_sDesdeMaq) ||
		 (pOM->GetsMaquina() > m_PI.m_sHastaMaq) )
		{
			m_PI.m_ArrOMs.RemoveAt(i);
			delete pOM;
			i--;
			iLim--;
		}
	}
	//sMsg.Format("Filtradas a %d OFs", m_PI.m_ArrOMs.GetSize());
	//AfxMessageBox(sMsg);
	
	m_PI.m_FecPrev = CTime(1900,1,1);

	m_PI.m_iHres = pDC->GetDeviceCaps(HORZRES); 
	m_PI.m_iVres = pDC->GetDeviceCaps(VERTRES); 

	double dRatio = 1.67;
	if (m_PI.m_pFont90) 
	{
		m_PI.m_pFont90->DeleteObject();
		delete m_PI.m_pFont90;
	}
	
	if (m_PI.m_pFont90b) 
	{
		m_PI.m_pFont90b->DeleteObject();
		delete m_PI.m_pFont90b;
	}
	
	m_PI.m_pFont90 = new CFont;
	m_PI.m_pFont90->CreateFont((int) (m_PI.m_iHres/100*dRatio+1), m_PI.m_iHres/100, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 
		OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, NULL); 
	m_PI.m_pFont90b = new CFont;
	m_PI.m_pFont90b->CreateFont((int) (m_PI.m_iHres/100*dRatio+1), m_PI.m_iHres/100, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, 
		OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, NULL); 

	
	pDC->SelectObject(m_PI.m_pFont90);
	TEXTMETRIC TM;
	pDC->GetTextMetrics(&TM);

	m_PI.m_iLinHeight = TM.tmHeight + TM.tmExternalLeading;

	m_PI.m_iCabHeight = (int) (m_PI.m_iLinHeight * 4);
	m_PI.m_iTitleHeight = (int) (m_PI.m_iLinHeight * 2);
	m_PI.m_iFootHeight = (int) (m_PI.m_iLinHeight * 5);

	m_PI.m_iWidth90 = TM.tmAveCharWidth;
	
	
	// Calculamos ahora cuántas líneas entran en cada página

	m_PI.m_iLinsPerPage = (m_PI.m_iVres / m_PI.m_iLinHeight) - 15;

	// Y ahora cuántas páginas necesitamos
	int iMaxPg = (m_PI.m_ArrOMs.GetSize() / m_PI.m_iLinsPerPage) + 1;
	if (iMaxPg < 1) iMaxPg = 1;
	// Actualizamos el número de páginas

	 pInfo->SetMaxPage(iMaxPg);
	m_PI.m_iNumOM = 0;
	
	//sMsg.Format("Saliendo inicio, paginas %d", iMaxPg);
	//AfxMessageBox(sMsg);
	

}

BOOL 
CCpwView::GetLinProgMaq(CString& p_sLin, BOOL p_BPrimera, BOOL& pr_BNuevo)
{
	static CString s_sMaqPrev = "";
	if (m_PI.m_iNumOM >= m_PI.m_ArrOMs.GetSize()) return FALSE;
	// Obtenemos la Orden indicada por m_PI.m_iNumOM, que es el contador

	COrdMaq* pOM = (COrdMaq*) m_PI.m_ArrOMs.GetAt(m_PI.m_iNumOM);
	// Sumamos uno al contador para la siguiente vez
	m_PI.m_iNumOM++;

	CTime FecIni = pOM->GetFecIni();
	CTime FecFin = pOM->GetFecFin();
	CTimeSpan tsTiempo = pOM->GetTimeSpan();
	
	double dCadencia = pOM->GetdCadenciaGFH();
	
	
	// Pasamos la cantidad a una cadena para ponerle puntos de millar
	CString sMaq;
	if (p_BPrimera || s_sMaqPrev != pOM->GetsMaquina()) 
	{
		sMaq = pOM->GetsMaquina();
		s_sMaqPrev = sMaq;
	}
	else
	{
		sMaq = "";
	}
	double dTiempo = (double) (tsTiempo.GetTotalSeconds() / (double) TK_SEGS_HORA);
	
		p_sLin.Format( "%-5.5s%-8.8s  %-8.8s  %s   %s  %s %14.14s",
					sMaq,
					pOM->GetsID(),
					pOM->GetsMezcla(),
					//(pOM->GetsID() == "" ) ? "       " :	FormatLong(pOM->GetlCantReal(), 7),
					(pOM->GetsID() == "" ) ? "       " :	FormatLong(pOM->GetlCantidad(), 7),
					(pOM->GetsID() == "" ) ? "     " :	FormatLong((long) pOM->GetdCadenciaGFH(),5) ,
					(pOM->GetsID() == "" ) ? FormatLong((long) (pOM->GetFecFin() - pOM->GetFecIni()).GetTotalHours(),5) : FormatLong((long) (pOM->GetTimeSpan().GetTotalHours()),5),
					FormatFec(pOM->GetFecIni(), FecHora));
			
	return TRUE;
}


// Listado de Máquinas

void CCpwView::EndPrintingMoldes(CDC* pDC, CPrintInfo* pInfo)
{
	
}

void CCpwView::PrintMoldes(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Actualizamos los parámetros de la página 

	if (!m_PI.m_pPM) return;
	
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);

	m_PI.m_iNumOM = (pInfo->m_nCurPage - 1) * m_PI.m_iLinsPerPage;
   


	CString sLin;
	int iLinH = m_PI.m_iLinHeight;
	// Sacamos cabeceras
	pDC->SelectObject(m_PI.m_pFont90b);
	sLin.Format("                     C A M B I O S      D E     M O L D E S");
	pDC->TextOut( 80, iLinH, sLin );
	pDC->SelectObject(m_PI.m_pFont90);

	BOOL BNuevo = FALSE;
	sLin.Format( "MAQ. ENTRANTE SALIENTE  HORA       CAUSA        MEZCLA ");
	pDC->TextOut( 80, 6*iLinH, sLin );
	sLin.Format( "--- --------- -------- ------- ---------------  ------ ");
	pDC->TextOut( 80, 7*iLinH, sLin );
	int iLin = 1;		// Línea que vamos imprimiendo
	BOOL BPrimera = TRUE;
	// Imprimimos las líneas de detalle
	while ( GetLinMoldes( sLin, BPrimera, BNuevo ))
	{
		BPrimera = FALSE;
		// Si es nuevo lo sacamos en negrita
		pDC->SelectObject(m_PI.m_pFont90);
	
		pDC->TextOut( 80, (iLin+7)*iLinH, sLin);
			
		// Si ya no tenemos sitio para más líneas en la página, hacemos otra
		iLin++;
		if (iLin > m_PI.m_iLinsPerPage) break;
		
	}

	// Pie
	CTime FecHoy = CTime::GetCurrentTime();
	int iYear = FecHoy.GetYear();
	int iSiglo = (iYear / 100) * 100;

	sLin.Format( "          FECHA: %2d-%2d-%2d  %2d:%2d    SUSTITUYE A:                 FIRMA:"
				,FecHoy.GetDay(), FecHoy.GetMonth(), iYear % iSiglo, 
				 FecHoy.GetHour(), FecHoy.GetMinute() );
	//pDC->TextOut( 10, ( m_PI.m_iLinsPerPage+1)*iLinH, sLin );
	pDC->TextOut(10, m_PI.m_iVres - m_PI.m_iFootHeight , sLin);
  
  
}



void CCpwView::BeginPrintingMoldes(CDC* pDC, CPrintInfo* pInfo)
{
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);

	int iLim = pProg->GetArrMaqOM(m_PI.m_ArrOMs);
	// Vamos a filtrarlo para quedarnos sólo con máquinas y fechas seleccionadas
	for (int i=0; i < iLim; i++)
	{
		COrdMaq* pOM = (COrdMaq*) m_PI.m_ArrOMs.GetAt(i);
		BOOL bRepMaq = FALSE;
		if (pOM->GetFecIni() > m_PI.m_Abs.GetFecFin() || 
		 (pOM->GetFecFin() < m_PI.m_Abs.GetFecIni()) ||
		 (pOM->GetsMaquina() < m_PI.m_sDesdeMaq) ||
		 (pOM->GetsMaquina() > m_PI.m_sHastaMaq) )
		{
			m_PI.m_ArrOMs.RemoveAt(i);
			i--;
			iLim--;
		}
	}
	iLim = m_PI.m_ArrOMs.GetSize();
	// Vamos a filtrarlo para quedarnos sólo con los cambios de moldes
	for (int i=0; i < iLim; i++)
	{
		COrdMaq* pOM = (COrdMaq*) m_PI.m_ArrOMs.GetAt(i);
		COrdMaq* pOMPrev = NULL;
		COrdMaq* pOMNext = NULL;
		if ( (i+1) < iLim)
		{
			pOMNext = (COrdMaq*) m_PI.m_ArrOMs.GetAt(i+1);
			if ( pOM->GetsMolde() == pOMNext->GetsMolde() && 
				pOM->GetsMaquina() == pOMNext->GetsMaquina())  
			{
				m_PI.m_ArrOMs.RemoveAt(i);
				i--;
				iLim--;
			}
		}
	}	

	m_PI.m_FecPrev = CTime(1900,1,1);

	m_PI.m_iHres = pDC->GetDeviceCaps(HORZRES); 
	m_PI.m_iVres = pDC->GetDeviceCaps(VERTRES); 

	double dRatio = 1.67;
	if (m_PI.m_pFont90) 
	{
		m_PI.m_pFont90->DeleteObject();
		delete m_PI.m_pFont90;
	}
	
	if (m_PI.m_pFont90b) 
	{
		m_PI.m_pFont90b->DeleteObject();
		delete m_PI.m_pFont90b;
	}
	
	m_PI.m_pFont90 = new CFont;
	m_PI.m_pFont90->CreateFont((int) (m_PI.m_iHres/100*dRatio+1), m_PI.m_iHres/100, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 
		OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, NULL); 
	m_PI.m_pFont90b = new CFont;
	m_PI.m_pFont90b->CreateFont((int) (m_PI.m_iHres/100*dRatio+1), m_PI.m_iHres/100, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, 
		OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, NULL); 

	
	pDC->SelectObject(m_PI.m_pFont90);
	TEXTMETRIC TM;
	pDC->GetTextMetrics(&TM);

	m_PI.m_iLinHeight = TM.tmHeight + TM.tmExternalLeading;

	m_PI.m_iCabHeight = (int) (m_PI.m_iLinHeight * 4);
	m_PI.m_iTitleHeight = (int) (m_PI.m_iLinHeight * 2);
	m_PI.m_iFootHeight = (int) (m_PI.m_iLinHeight * 5);

	m_PI.m_iWidth90 = TM.tmAveCharWidth;
	
	
	// Calculamos ahora cuántas líneas entran en cada página

	m_PI.m_iLinsPerPage = (m_PI.m_iVres / m_PI.m_iLinHeight) - 15;

	// Y ahora cuántas páginas necesitamos
	int iMaxPg = (m_PI.m_ArrOMs.GetSize() / m_PI.m_iLinsPerPage) + 1;
	if (iMaxPg < 1) iMaxPg = 1;
	// Actualizamos el número de páginas

	 pInfo->SetMaxPage(iMaxPg);
	m_PI.m_iNumOM = 0;

}

BOOL 
CCpwView::GetLinMoldes(CString& p_sLin, BOOL p_BPrimera, BOOL& pr_BNuevo)
{
	static CString s_sMaqPrev = "";
	if (m_PI.m_iNumOM >= m_PI.m_ArrOMs.GetSize()) return FALSE;
	// Obtenemos la Orden indicada por m_PI.m_iNumOM, que es el contador

	COrdMaq* pOM = (COrdMaq*) m_PI.m_ArrOMs.GetAt(m_PI.m_iNumOM);
	COrdMaq* pOMPrev = NULL;
	if (m_PI.m_iNumOM-1 >= 0)
	{
		pOMPrev = (COrdMaq*) m_PI.m_ArrOMs.GetAt(m_PI.m_iNumOM-1);
		if (pOMPrev->GetsMaquina() != pOM->GetsMaquina())
			pOMPrev = NULL;
	}
	// Sumamos uno al contador para la siguiente vez
	m_PI.m_iNumOM++;

	CTime FecIni = pOM->GetFecIni();
	CTime FecFin = pOM->GetFecFin();
	CTimeSpan tsTiempo = pOM->GetTimeSpan();
	
	long lCant = pOM->GetlCantidad();
	double dCadencia = pOM->GetdCadenciaGFH();
	
	
	// Pasamos la cantidad a una cadena para ponerle puntos de millar
	CString sCant;
	FormatLong(lCant, sCant, 6);
	CString sMaq;
	if (p_BPrimera || s_sMaqPrev != pOM->GetsMaquina()) 
	{
		sMaq = pOM->GetsMaquina();
		s_sMaqPrev = sMaq;
	}
	else
	{
		sMaq = "";
	}
	double dTiempo = (double) (tsTiempo.GetTotalSeconds() / (double) TK_SEGS_HORA);
	
		p_sLin.Format( "%-4.4s%-8.8s  %-8.8s  %5.5s  %15.15s  %s",
					sMaq,
					pOM->GetsID(),
					(pOMPrev != NULL) ? pOMPrev->GetsID() : "",
					FormatFec(pOM->GetFecFin(),Hora),
					"",
					pOM->GetsMezcla());
	return TRUE;
}

void CCpwView::PrintGrafico(CDC* pDC, CPrintInfo* pInfo) 
{
	
	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);
	
	CString buffer;
		
	CBitmap* pbmOld = NULL;

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

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
	


	
	CTime tempFec = m_PI.m_Abs.GetFecIni();
	CString sDeb = FormatFec(tempFec, Fecha);
	tempFec += (CTimeSpan) (((pInfo->m_nCurPage - 1) * 
		m_PI.m_iDaysPerPage ) * TK_SEGS_DIA);
	
	CTimeSpan tsDespl;
	
	// Calculamos las fechas de inicio y de fin que realmente hay que pintar
	// en esta página
	CTime FecIni = tempFec;
	CTime FecFin = tempFec + m_PI.m_iDaysPerPage * TK_SEGS_DIA;
	if (FecFin > m_PI.m_Abs.m_FecFin) FecFin = m_PI.m_Abs.m_FecFin;
	sDeb = FormatFec(FecIni, Fecha);
	sDeb = FormatFec(FecFin, Fecha);

	// Sacamos las cabeceras
	CTime Hoy = CTime::GetCurrentTime();
	CString Tit;
	pDC->SelectObject(m_PI.m_pFont90);
	int iSiglo = (Hoy.GetYear() / 100) * 100;
	Tit.Format("P R O G R A M A C I O N     D E    I N Y E C C I O N      %d-%d-%d",
		Hoy.GetDay(),
		Hoy.GetMonth(),
		Hoy.GetYear() % iSiglo );
	pDC->TextOut(60, 1, Tit);
	pDC->SelectObject(m_PI.m_pFontNormal);
	
	
	// E imprimimos los días del calendario
	int x = (int) (m_PI.m_iPixCol / 4) ;
	int y = m_PI.m_iCabHeight;
	int yf;
	
	FecIni = CTurnos::GetFecIni(FecIni, 1);
	CTime Fec = FecIni; 
	FecFin = CTurnos::GetFecIni(FecFin, 1);
	
	CPen PenDash(PS_DASH, 1, RGB(0,0,0));
	CPen PenSolid(PS_SOLID, 1, RGB(0,0,0));
	CPen PenGordo(PS_SOLID, m_PI.m_iWidthNormal / 3, RGB(0,0,0));
	
	pDC->SelectObject(m_PI.m_pFont90);
	
	int iDespIni = m_PI.m_iCabHeight;
	BOOL bResaltado = TRUE;
	while (Fec <= FecFin)
	{
		CString sDia;
		
		Fec = CTime(Fec.GetYear(), Fec.GetMonth(), Fec.GetDay(), 0,0,0); // Al comienzo del día
		
		for (int ii = 1; ii <= CTurnos::GetiNumTurnos(); ii++) {
			Fec = CTurnos::GetFecFin(Fec, ii);
			if (Fec > FecFin) break;
			tsDespl = Fec - FecIni;
			y = (int) (tsDespl.GetTotalSeconds() / m_PI.m_lConvFactor) + m_PI.m_iCabHeight;
			yf = (int) ((tsDespl.GetTotalSeconds() + CTurnos::GetSecsTurno(ii))/ m_PI.m_lConvFactor) + m_PI.m_iCabHeight;
			pDC->MoveTo(0, y);
			
			if (ii >= 1 && ii < CTurnos::GetiNumTurnos()) pDC->SelectObject(&PenDash);
			else pDC->SelectObject(&PenSolid);
			
			pDC->LineTo(m_PI.m_iHres, y);
			
			
		}
		//Ahora ponemos el número de día, en el centro del día
		Fec = AddDays(Fec, -1); // Pasamos al día anterior porque estaremos en el fin del último turno, que es al día siguiente
		int iHoraCentral = CTurnos::GetFecIni(Fec, 1).GetHour() + 12;
		Fec = CTime(Fec.GetYear(), Fec.GetMonth(), Fec.GetDay(), iHoraCentral ,0,0);
		sDia.Format("%d", Fec.GetDay());
		tsDespl = Fec - FecIni;
		y = (int) (tsDespl.GetTotalSeconds() / m_PI.m_lConvFactor) + m_PI.m_iCabHeight;
		pDC->TextOut(3, y, sDia);
		
		// Y pasamos a hacer el día siguiente
		Fec = AddDays(Fec, 1);
		
	}
	int MaxY = y;
	pDC->SelectObject(&PenSolid);			
	pDC->SelectObject(m_PI.m_pFontNormal);
	
	int NumMaq = pProg->GetNumPM();
	CBrush gBrush(RGB(150,150,150));
	
	int iCol = 0;
	// Ahora sacamos las OM's
	for (int i=0; i<NumMaq ; i++)
	{
		CProgMaq* pMaq = pProg->GetpPM(i);
		ASSERT_VALID(pMaq);
		
		if (!EsSelectedMaq(pMaq)) continue;

		int NumOrd = pMaq->GetNumOM();
		for (int j=0; j<NumOrd; j++)
		{
			COrdMaq* pOM = pMaq->GetpOM(j);
			ASSERT_VALID(pOM);
			if (pOM->GetFecIni() > FecFin) break;
			CRect Rect = GetRectPrint(pOM, iCol, FecIni);
			//Rect.left += int (m_PI.m_iPixCol*.25 / 2);
			//Rect.right -=  int (m_PI.m_iPixCol*.25 / 2);
			if (Rect.top < m_PI.m_iCabHeight) 
				Rect.top =  m_PI.m_iCabHeight - 2;
			if (Rect.bottom < m_PI.m_iCabHeight) 
				Rect.bottom =  m_PI.m_iCabHeight;
			if (Rect.top > MaxY ) 
				Rect.top =  MaxY;
			if (Rect.bottom > MaxY ) 
				Rect.bottom =  MaxY;
			int despl = Rect.left + 1;
			pDC->SelectObject(pOM->GetpBrush(Impresora));
			pDC->Rectangle(Rect);
			
		}
		iCol++;
	}
	
	// Pintamos las máquinas
	CBrush BrushWhite(RGB(255,255,255));
	iCol = 0;
	for (int i=0; i<NumMaq ; i++)
	{
		CProgMaq* pPM = pProg->GetpPM(i);
		ASSERT_VALID(pPM);
		
		if (!EsSelectedMaq(pPM)) continue;

		
		int xi = (int) ((iCol + .25 ) * m_PI.m_iPixCol); 
		int xf = xi + m_PI.m_iPixCol;
		pDC->FillRect(CRect(xi, m_PI.m_iTitleHeight, xf, m_PI.m_iCabHeight), &BrushWhite);
		pDC->MoveTo(xi, m_PI.m_iTitleHeight);
		pDC->LineTo(xi, m_PI.m_iCabHeight);
		pDC->LineTo(xf, m_PI.m_iCabHeight);
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SelectObject(m_PI.m_pFontMaq);
		int xc = (xi+xf)/2 - pPM->GetsID().GetLength() * m_PI.m_iWidthMaq / 2 ;
		if (xc < xi) xc = xi;
		pDC->TextOut( xc, m_PI.m_iTitleHeight, pPM->GetsID());
		pDC->SetBkMode(OPAQUE);
		pPM->SetPosition(CPoint(xi, m_PI.m_iCabHeight));
		pDC->SelectObject(m_PI.m_pFontNormal);
		
		// Pintamos ahora las inactividades de esa máquina
		
		Fec = FecIni;
		for (int j = 0; j < pPM->GetNumInacts(); j++)
		{
			RECT InactRect;
			CAbSpan Inact = *(pPM->GetpInact(j));
			if (Inact.GetFecIni() > FecFin) break;
			tsDespl = Fec - FecIni;
			y = (int) (tsDespl.GetTotalSeconds() / m_PI.m_lConvFactor) + m_PI.m_iCabHeight;
			
			tsDespl = Inact.GetFecIni() - FecIni;
			InactRect.top = (int) ((tsDespl.GetTotalSeconds() 
				/ m_PI.m_lConvFactor) + m_PI.m_iCabHeight);
			tsDespl = Inact.GetFecFin() - Inact.GetFecIni();
			InactRect.bottom = (int) (InactRect.top + 
				(tsDespl.GetTotalSeconds() 
				/ m_PI.m_lConvFactor));
			InactRect.left = xi;
			InactRect.right = xf;
			if (InactRect.top < m_PI.m_iCabHeight) 
				InactRect.top =  m_PI.m_iCabHeight;
			if (InactRect.bottom < m_PI.m_iCabHeight) 
				InactRect.bottom =  m_PI.m_iCabHeight;
			if (InactRect.top > MaxY ) 
				InactRect.top =  MaxY;
			if (InactRect.bottom > MaxY ) 
				InactRect.bottom =  MaxY;
			pDC->MoveTo(InactRect.left,InactRect.top);
			pDC->LineTo(InactRect.right, InactRect.top);
			pDC->LineTo(InactRect.left, InactRect.bottom);
			pDC->LineTo(InactRect.right,InactRect.bottom);
			pDC->LineTo(InactRect.left, InactRect.top);
		}
		
		iCol++;
	}
	
	// Ahora pintamos los textos de las OM´s
	iCol = 0;
	for (int i=0; i<NumMaq ; i++)
	{
		CProgMaq* pMaq = pProg->GetpPM(i);
		ASSERT_VALID(pMaq);
		
		if (!EsSelectedMaq(pMaq)) continue;

		int NumOrd = pMaq->GetNumOM();     
		for (int j=0; j<NumOrd; j++)
		{   
			CString buffer;
			COrdMaq* pOM = pMaq->GetpOM(j);
			ASSERT_VALID(pOM);
			if (pOM->GetFecIni() > FecFin) break;
			CRect Rect = GetRectPrint(pOM, iCol, FecIni); 
			int despl = Rect.left + 1;
			if (Rect.top < m_PI.m_iCabHeight) 
				Rect.top =  m_PI.m_iCabHeight - 2;
			if (Rect.bottom < m_PI.m_iCabHeight) 
				continue;
			pDC->SetBkColor(RGB(255,255,255));
			pDC->SetBkMode(OPAQUE);              
			CFont FontReducido;
			int iMaxHeight = Rect.bottom - Rect.top - 4;
			if (iMaxHeight < 10) iMaxHeight = 10;
			
			if (pMaq->GetlGFH() == TK_GFH_INYECCION)
			{
				if (pOM->GetTsMezcla().GetTotalSeconds() > 0)
				{
					despl = Rect.left + 10;
					CTimeSpan tsDesplMezcla = pOM->GetFecIni() - FecIni;
					int iTopM = (int) (tsDesplMezcla.GetTotalSeconds() / m_PI.m_lConvFactor) + m_PI.m_iCabHeight;
					tsDesplMezcla = pOM->GetFecMezcla() - pOM->GetFecIni();
					int iBottomM = (int) (iTopM + tsDesplMezcla.GetTotalSeconds() / m_PI.m_lConvFactor);
					if (iBottomM > Rect.bottom) iBottomM = Rect.bottom;
					pDC->SelectObject(&PenGordo);
					pDC->MoveTo(despl + m_PI.m_iWidthNormal, iTopM);
					pDC->LineTo(despl + m_PI.m_iWidthNormal, iBottomM);
					pDC->SelectObject(&PenSolid);
				}
			}
			
			FontReducido.CreateFont( iMaxHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 
				OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, NULL); 
			CFont FontReducidoBold;
			FontReducidoBold.CreateFont( iMaxHeight, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, 
				OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, NULL); 
			
			
			//buffer.Format("%5.5ld", 0);
			//pDC->TextOut(despl,Rect.top+2,buffer);
			//despl += 5 * m_PI.m_iWidthNormal + 4;
			buffer.Format( "%-9.9s", (const char*) pOM->GetsNombre());
			pDC->TextOut(despl,Rect.top+1,buffer);
			//despl += 9 * m_PI.m_iWidthNormal + 4;
			//pDC->TextOut(despl,Rect.top+2, buffer,3);
			//despl += 3 * m_PI.m_iWidthNormal + 4;
			int iH = pOM->GetTsMargen().GetHours();
			if (buffer != "")
				{
					despl += pDC->GetTextExtent(buffer).cx + 2;
					if (iH < 0) 
					{
						pDC->SetBkColor(RGB(255,0,0));
						pDC->TextOut(despl,Rect.top+1,"*");
						pDC->SetBkColor(RGB(255,255,255));
						despl += pDC->GetTextExtent("*").cx + 2;
					}
					else 
						despl += 2;
					
					CBufMolde* pBufMolde = (CBufMolde*) pOM->GetpBufMolde();
					
					if (pBufMolde)
					{
						despl = Rect.right - 1 - 20;
						int iDespY = Rect.top + pDC->GetTextExtent(buffer).cy;
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
			}
				
		}
		iCol++;
	}
	
	// Y sacamos el pie, a ser posible por la ventanilla
	pDC->SelectObject(m_PI.m_pFont90);
	CString sPie;
	
	iSiglo = (Hoy.GetYear() / 100) * 100;
	
	sPie.Format( "          FECHA: %d-%d-%d  %d:%d    SUSTITUYE A:                 FIRMA:"
		,Hoy.GetDay(), Hoy.GetMonth(), Hoy.GetYear() % iSiglo, 
		Hoy.GetHour(), Hoy.GetMinute() );
	pDC->TextOut(10, m_PI.m_iVres - m_PI.m_iFootHeight , sPie);
	
	
	pDC->SelectObject(&m_Brush);
}

// Devolvemos si la máquina en el parámetro p_pPM está entre las seleccionadas para
// salir en el informe gráfico. Como el usuario ha podido cambiar el orden de las máquinas
// en el programa, tenemos que ver si está entre la primera y la última seleccionadas, siguiendo
// el orden del programa.
bool
CCpwView::EsSelectedMaq(CProgMaq* p_pPM)
{
	if (p_pPM->GetlGFH() < atol(m_PI.m_sDesdeGFH)) return true;
	if (p_pPM->GetlGFH() > atol(m_PI.m_sHastaGFH)) return true;

	CCpwDoc* pDoc = GetDocument();
	CProg* pProg = pDoc->GetpProg();
	
	int NumMaq = pProg->GetNumPM();
	CBrush gBrush(RGB(150,150,150));
	
	bool bPasadoInicio = false;
	bool bPasadoFin = false;


	for (int i=0; i<NumMaq ; i++)
	{
		CProgMaq* pMaq = pProg->GetpPM(i);
		if ( atol(pMaq->GetsID()) == atol(m_PI.m_sDesdeMaq) ) bPasadoInicio = true;

		if (atol(pMaq->GetsID()) == atol(p_pPM->GetsID())) 
		{
			// Si estamos entre la primera y la última seleccionadas, devolvemos que sí
			if (bPasadoInicio && !bPasadoFin) return true;
			else return false;
		} 
		if ( atol(pMaq->GetsID()) == atol(m_PI.m_sHastaMaq) ) bPasadoFin = true;
		
	}
	return false;
}

int 
CCpwView::GetNumPMsInGFHs()
{
	CCpwDoc* pDoc = GetDocument();
	CProg* pProg = pDoc->GetpProg();
	int iNumPMs = 0;
	
	int lim = pProg->GetNumPM();
	for(int i = 0; i < lim; i++)
	{
		CProgMaq* pPMInd = pProg->GetpPM(i);
		if (EsSelectedMaq(pPMInd)) iNumPMs++;
	}
	return iNumPMs;
}


/*	Anterior version
void CCpwView::PrintGrafico(CDC* pDC, CPrintInfo* pInfo) 
{

	CCpwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CProg* pProg = pDoc->GetpProg();
	ASSERT_VALID(pProg);

	CString buffer;

	CTime tempFec = m_PI.m_Abs.GetFecIni();
	tempFec += (CTimeSpan) (((pInfo->m_nCurPage - 1) * 
							m_PI.m_iDaysPerPage ) * TK_SEGS_DIA);
   
	CTimeSpan tsDespl;

	// Calculamos las fechas de inicio y de fin que realmente hay que pintar
	// en esta página
	CTime FecIni = tempFec;
	CTime FecFin = tempFec + m_PI.m_iDaysPerPage * TK_SEGS_DIA;
	if (FecFin > m_PI.m_Abs.m_FecFin) FecFin = m_PI.m_Abs.m_FecFin;

 	// Sacamos las cabeceras
	CTime Hoy = CTime::GetCurrentTime();
	CString Tit;
	pDC->SelectObject(m_PI.m_pFont90);
	int iSiglo = (Hoy.GetYear() / 100) * 100;
	Tit.Format("P R O G R A M A C I O N     D E    I N Y E C C I O N      %d-%d-%d",
					Hoy.GetDay(),
					Hoy.GetMonth(),
					Hoy.GetYear() % iSiglo );
	pDC->TextOut(60, 1, Tit);
	pDC->SelectObject(m_PI.m_pFontNormal);
	
	
	// E imprimimos los días del calendario
	int x = (int) (m_PI.m_iPixCol / 4) ;
	int y = m_PI.m_iCabHeight;
	FecIni = CTime(FecIni.GetYear(), FecIni.GetMonth(), FecIni.GetDay(), TK_TURNO3_FIN,0,0);
	CTime Fec = FecIni; 
	FecFin = CTime(FecFin.GetYear(), FecFin.GetMonth(), FecFin.GetDay(), TK_TURNO3_FIN,0,0);
	
	CPen PenDash(PS_DASH, 1, RGB(0,0,0));
	CPen PenSolid(PS_SOLID, 1, RGB(0,0,0));
	int iDespIni = m_PI.m_iCabHeight;
	BOOL bResaltado = TRUE;
	while (Fec <= FecFin)
	{
		CString sDia;
		pDC->SelectObject(&PenSolid);			
		
		Fec = CTime(Fec.GetYear(), Fec.GetMonth(), Fec.GetDay(), TK_TURNO3_FIN,0,0);
		if (Fec > FecFin) break;
		if (Fec >= FecIni)
		{
			tsDespl = Fec - FecIni;
			y = (int) (tsDespl.GetTotalSeconds() / m_PI.m_lConvFactor) + m_PI.m_iCabHeight;
			pDC->MoveTo(0,y);
			pDC->LineTo(m_PI.m_iHres, y);
		}
		if (!bResaltado)
			bResaltado = TRUE;
		else
		{
			pDC->FillRect(CRect((int) (.25 * m_PI.m_iPixCol), 
							iDespIni, 
							m_PI.m_iHres, 
							y), 
							&CBrush(RGB(180,180,180)));
			bResaltado = FALSE;
		}
		iDespIni = y;

		Fec = CTime(Fec.GetYear(), Fec.GetMonth(), Fec.GetDay(), TK_TURNO1_FIN,0,0);
		if (Fec > FecFin) break;
		if (Fec >= FecIni)
		{
			tsDespl = Fec - FecIni;
			y = (int) (tsDespl.GetTotalSeconds() /  m_PI.m_lConvFactor) + m_PI.m_iCabHeight;
			pDC->SelectObject(&PenDash);
			pDC->MoveTo(0,y);
			pDC->LineTo(m_PI.m_iHres, y);
		}

		Fec = CTime(Fec.GetYear(), Fec.GetMonth(), Fec.GetDay(), (TK_TURNO1_FIN + TK_TURNO2_FIN) / 2,0,0);
		CTime FecI = Fec - (CTimeSpan) ((m_FontHeight *  m_PI.m_lConvFactor) / 2);
		CTime FecF = Fec + (CTimeSpan) ((m_FontHeight *  m_PI.m_lConvFactor) / 2);
		if (FecI > FecFin) break;
		if (FecF >= FecIni)
		{
			tsDespl = FecI - FecIni;
			y = (int) (tsDespl.GetTotalSeconds() /  m_PI.m_lConvFactor) + m_PI.m_iCabHeight;
			sDia.Format("%d", Fec.GetDay());
			pDC->TextOut(3, y, sDia );
		}


		Fec = CTime(Fec.GetYear(), Fec.GetMonth(), Fec.GetDay(), 22,0,0);
		if (Fec > FecFin) break;
		if (Fec >= FecIni)
		{
			tsDespl = Fec - FecIni;
			y = (int) (tsDespl.GetTotalSeconds() /  m_PI.m_lConvFactor) + m_PI.m_iCabHeight;
			pDC->MoveTo(0,y);
			pDC->LineTo(m_PI.m_iHres, y);
		}

		Fec += (CTimeSpan) TK_CAMBIO_DIA;
	}   
	int MaxY = y;
	if (Fec.GetDay() % 2)
	{
		pDC->FillRect(CRect((int) (.25 * m_PI.m_iPixCol), 
			iDespIni, 
			m_PI.m_iHres, 
			y), 
			&CBrush(RGB(180,180,180)));
	}
	pDC->SelectObject(&PenSolid);			
		
	int NumMaq = pProg->GetNumPM();
	CBrush gBrush(RGB(150,150,150));
	
	int iCol = 0;
	// Ahora sacamos las OM's
	for (int i=0; i<NumMaq ; i++)
	{
		CProgMaq* pMaq = pProg->GetpPM(i);
		ASSERT_VALID(pMaq);
			
		if (!EsSelectedMaq(pMaq)) continue;

		int NumOrd = pMaq->GetNumOM();
		for (int j=0; j<NumOrd; j++)
		{
			COrdMaq* pOM = pMaq->GetpOM(j);
			ASSERT_VALID(pOM);
			if (pOM->GetFecIni() > FecFin) break;
			CRect Rect = GetRectPrint(pOM, iCol, FecIni);
			//Rect.left += int (m_PI.m_iPixCol*.25 / 2);
			//Rect.right -=  int (m_PI.m_iPixCol*.25 / 2);
			if (Rect.top < m_PI.m_iCabHeight) 
				Rect.top =  m_PI.m_iCabHeight - 2;
			if (Rect.bottom < m_PI.m_iCabHeight) 
				Rect.bottom =  m_PI.m_iCabHeight;
			if (Rect.top > MaxY ) 
				Rect.top =  MaxY;
			if (Rect.bottom > MaxY ) 
				Rect.bottom =  MaxY;
			int despl = Rect.left + 1;
			pDC->SelectObject(pOM->GetpBrush(Impresora));
			pDC->Rectangle(Rect);

		}
		iCol++;
	}
	
	// Pintamos las máquinas
	CBrush BrushWhite(RGB(255,255,255));
	iCol = 0;
	for (i=0; i<NumMaq ; i++)
	{
		CProgMaq* pPM = pProg->GetpPM(i);
		ASSERT_VALID(pPM);
		
		if (!EsSelectedMaq(pPM)) continue;

		int xi = (int) ((iCol + .25 ) * m_PI.m_iPixCol); 
		int xf = xi + m_PI.m_iPixCol;
		pDC->FillRect(CRect(xi, m_PI.m_iTitleHeight, xf, m_PI.m_iCabHeight), &BrushWhite);
		pDC->MoveTo(xi, m_PI.m_iTitleHeight);
		pDC->LineTo(xi, m_PI.m_iCabHeight);
		pDC->LineTo(xf, m_PI.m_iCabHeight);
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SelectObject(m_PI.m_pFont90);
		int xc = (xi+xf)/2 - pPM->GetsID().GetLength() * m_PI.m_iWidth90 / 2 ;
		if (xc < xi) xc = xi;
		pDC->TextOut( xc, m_PI.m_iTitleHeight, pPM->GetsID());
		pDC->SetBkMode(OPAQUE);
		pPM->SetPosition(CPoint(xi, m_PI.m_iCabHeight));
		pDC->SelectObject(m_PI.m_pFontNormal);
		
		// Pintamos ahora las inactividades de esa máquina
	
		Fec = FecIni;
		for (int j = 0; j < pPM->GetNumInacts(); j++)
		{
			RECT InactRect;
			CAbSpan Inact = *(pPM->GetpInact(j));
			if (Inact.GetFecIni() > FecFin) break;
			tsDespl = Fec - FecIni;
			y = (int) (tsDespl.GetTotalSeconds() / m_PI.m_lConvFactor) + m_PI.m_iCabHeight;
			
			tsDespl = Inact.GetFecIni() - FecIni;
			InactRect.top = (int) ((tsDespl.GetTotalSeconds() 
									/ m_PI.m_lConvFactor) + m_PI.m_iCabHeight);
			tsDespl = Inact.GetFecFin() - Inact.GetFecIni();
			InactRect.bottom = (int) (InactRect.top + 
									(tsDespl.GetTotalSeconds() 
									/ m_PI.m_lConvFactor));
			InactRect.left = xi;
			InactRect.right = xf;
			if (InactRect.top < m_PI.m_iCabHeight) 
				InactRect.top =  m_PI.m_iCabHeight;
			if (InactRect.bottom < m_PI.m_iCabHeight) 
				InactRect.bottom =  m_PI.m_iCabHeight;
			if (InactRect.top > MaxY ) 
				InactRect.top =  MaxY;
			if (InactRect.bottom > MaxY ) 
				InactRect.bottom =  MaxY;
				pDC->MoveTo(InactRect.left,InactRect.top);
			pDC->LineTo(InactRect.right, InactRect.top);
			pDC->LineTo(InactRect.left, InactRect.bottom);
			pDC->LineTo(InactRect.right,InactRect.bottom);
			pDC->LineTo(InactRect.left, InactRect.top);
		}

		iCol++;
	}

// Ahora pintamos los textos de las OM´s
	iCol = 0;
	for ( i=0; i<NumMaq ; i++)
	{
		CProgMaq* pMaq = pProg->GetpPM(i);
		ASSERT_VALID(pMaq);
		
		if (!EsSelectedMaq(pMaq)) continue;


		int NumOrd = pMaq->GetNumOM();     
		for (int j=0; j<NumOrd; j++)
		{   
			CString buffer;
			COrdMaq* pOM = pMaq->GetpOM(j);
			ASSERT_VALID(pOM);
			if (pOM->GetFecIni() > FecFin) break;
			CRect Rect = GetRectPrint(pOM, iCol, FecIni); 
			int despl = Rect.left + 1;
			if (Rect.top < m_PI.m_iCabHeight) 
				Rect.top =  m_PI.m_iCabHeight - 2;
			if (Rect.bottom < m_PI.m_iCabHeight) 
				continue;
			pDC->SetBkColor(RGB(255,255,255));
			pDC->SetBkMode(OPAQUE);              
			CFont FontReducido;
			int iMaxHeight = Rect.bottom - Rect.top - 4;
			if (iMaxHeight < 10) iMaxHeight = 10;
			FontReducido.CreateFont( iMaxHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 
				OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, NULL); 
			CFont FontReducidoBold;
			FontReducidoBold.CreateFont( iMaxHeight, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, 
				OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, NULL); 
			
			
			buffer.Format("%5.5ld", 0);
			pDC->TextOut(despl,Rect.top+2,buffer);
			despl += 5 * m_PI.m_iWidthNormal + 4;
			buffer.Format( "%-9.9s", (const char*) pOM->GetsNombre());
			pDC->TextOut(despl,Rect.top+2,buffer);
			despl += 9 * m_PI.m_iWidthNormal + 4;
			pDC->TextOut(despl,Rect.top+2, buffer,3);
			despl += 3 * m_PI.m_iWidthNormal + 4;
			
		}
		iCol++;
	}

	// Y sacamos el pie, a ser posible por la ventanilla
	pDC->SelectObject(m_PI.m_pFont90);
	CString sPie;

	iSiglo = (Hoy.GetYear() / 100) * 100;

	sPie.Format( "          FECHA: %d-%d-%d  %d:%d    SUSTITUYE A:                 FIRMA:"
				,Hoy.GetDay(), Hoy.GetMonth(), Hoy.GetYear() % iSiglo, 
				 Hoy.GetHour(), Hoy.GetMinute() );
	pDC->TextOut(10, m_PI.m_iVres - m_PI.m_iFootHeight , sPie);

	
	pDC->SelectObject(&m_Brush);
}
*/
