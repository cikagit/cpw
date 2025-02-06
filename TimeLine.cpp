#include "stdafx.h"
#include "afxdtctl.h"

#include "cpw.h"  
#include "cpwdefs.h"  
#include "AbSpan.h"
#include "cpwExt.h"
#include "prog.h" 
#include "cpwDoc.h"
#include "cpwDb.h"
#include "cache.h"            
#include "ProgMaq.h"
#include "OrdMaq.h"
#include <strstream>
#include "DlgDebug.h"
#include "OrdenDlg.h"  

#include "obarrord.h"

#include <fstream>
#include "TimeLine.h"


// Estructuras previas


IMPLEMENT_DYNAMIC(CLinSeg, CObject)

CLinSeg::CLinSeg( CLinSeg& p_LS)
	: CObject()
{
	m_AbSpan.SetFecIni(p_LS.m_AbSpan.GetFecIni());
	m_AbSpan.SetFecFin(p_LS.m_AbSpan.GetFecFin());
	m_dCadencia = p_LS.m_dCadencia ;
}				

CLinSeg::CLinSeg()
: CObject()
{
	m_AbSpan.SetFecIni(DATE_MIN);
	m_AbSpan.SetFecFin(DATE_MIN);
	m_dCadencia = 0;
}

CLinSeg& 
CLinSeg::operator =(const CLinSeg& p_LS)
{
	m_AbSpan.SetFecIni(p_LS.m_AbSpan.GetFecIni());
	m_AbSpan.SetFecFin(p_LS.m_AbSpan.GetFecFin());
	m_dCadencia = p_LS.m_dCadencia;
	return *this;
}

long
CLinSeg::GetlCantFab()
{
	double dCantFab = GetdCantFab();
	long lCantFab = (long) dCantFab;
	if ((dCantFab - lCantFab) > .5) lCantFab++;
	return lCantFab;
}

double
CLinSeg::GetdCantFab()
{
	double dSecs =  ((double) m_AbSpan.GetSecs() / ( 1.0 + ( TK_INACT_DEF / 100.0 )));
	return (dSecs * m_dCadencia / (double) TK_SEGS_HORA );
}

//----------------- CProdSeg

IMPLEMENT_DYNAMIC(CProdSeg, CObject)

CProdSeg::CProdSeg( CProdSeg& p_PS)
	: CObject()
{
	m_AbSpan.SetFecIni(p_PS.m_AbSpan.GetFecIni());
	m_AbSpan.SetFecFin(p_PS.m_AbSpan.GetFecFin());
	m_pOM = p_PS.m_pOM;

}				

CProdSeg::CProdSeg( COrdMaq* p_pOM)
: CObject()
{
	if (p_pOM)
	{
		m_AbSpan.SetFecIni(p_pOM->GetFecIni());
		m_AbSpan.SetFecFin(p_pOM->GetFecFin());
	}
	else
	{
		m_AbSpan.SetFecIni(DATE_MIN);
		m_AbSpan.SetFecFin(DATE_MIN);
	}
	m_pOM = p_pOM; 
}

CProdSeg& 
CProdSeg::operator =(const CProdSeg& p_PS)
{
	m_AbSpan.SetFecIni(p_PS.m_AbSpan.GetFecIni());
	m_AbSpan.SetFecFin(p_PS.m_AbSpan.GetFecFin());
	m_pOM = p_PS.m_pOM;
	return *this;
}




// Comc Class

IMPLEMENT_SERIAL(CTimeLine, CObject, 1)

BOOL
CTimeLine::HaySolapes()
{
	// Si los arrays son distintos, es que hay solapes, dado que el calendario ha debido ser aplicado al primer
	// array.
	if (m_pArrPS->GetSize() != m_pArrLS->GetSize()) return TRUE;

	return FALSE;
}

// Obtenemos un array de PSs a los que afecta un LinSeg
void
CTimeLine::GetArrPSsAfec( CLinSeg* p_pLS, CObArray* p_pArrPSs)
{
	p_pArrPSs->RemoveAll();

	for (int i=0; i < m_pArrPS->GetSize(); i++)
	{
		CProdSeg* pPS = (CProdSeg*) m_pArrPS->GetAt(i);
		// Si está comprendido
		if (pPS->m_AbSpan.GetFecIni() <= p_pLS->m_AbSpan.GetFecIni() &&
			pPS->m_AbSpan.GetFecFin() >= p_pLS->m_AbSpan.GetFecFin() )
		{
			p_pArrPSs->Add(pPS);
		}
	}
}

// Obtenemos un array de OMs que entran en la TimeLine
void
CTimeLine::GetArrOMs( CObArray* p_pArrOMs)
{
	p_pArrOMs->RemoveAll();
	BOOL bEncontrado;

	for (int i=0; i < m_pArrPS->GetSize(); i++)
	{
		CProdSeg* pPS = (CProdSeg*) m_pArrPS->GetAt(i);
		COrdMaq* pOM = pPS->m_pOM;
		if (pOM)
		{
			bEncontrado = FALSE;
			for (int j = 0; j < p_pArrOMs->GetSize(); j++)
			{
				if (((COrdMaq*) p_pArrOMs->GetAt(j)) == pOM)
				{
					bEncontrado = TRUE;
					break;
				}
			}
			if (!bEncontrado) p_pArrOMs->Add(pOM);		
			
		}
	}
}

// Reparte una cantidad entre las OMs de la CTimeLine, de acuerdo con los solapes
// e inactividades reflejadas en las CProdSeg. Las cantidades que le tocan a
// cada OM se meten en las OMs, en el campo m_lAux
long
CTimeLine::ReparteCant(long p_lCant, enum  eTimeLine p_eTL)
{
//	logFile.Write("%s - %s", "Entrando a ReparteCant de : ", this->m_sID);
	
	long lCantIni = p_lCant;		// Guardamos la cantidad inicial
	// Primero ponemos a 0 los m_lAux
	for(int i = 0; i < m_pArrPS->GetSize(); i++)
	{
		CProdSeg* pProdSeg = (CProdSeg*) m_pArrPS->GetAt(i);
	// Probablemente se haga varias veces en alguna OM, pero no es importante
		if (pProdSeg->m_pOM) 
		{
			pProdSeg->m_pOM->m_lAux = 0;
			pProdSeg->m_pOM->m_dAuxCant = 0;
		}
	} 
	// Necesitaremos una matriz intermedia de PSs, para guardar los PSs a los que afecta cada LS
	CObArray ArrPSs;
	// Y Un acumulado para saber cuántas piezas llevamos repartidas
	double dAcumCant = 0;
	// Hacemos un ciclo por los elementos del pArrLS
//	logFile.Write("%s - %ld", "Tamaño del ArrLS : ", m_pArrLS->GetSize());
	for (int i = 0; i < m_pArrLS->GetSize(); i++)
	{
		CLinSeg* pLS = (CLinSeg*) m_pArrLS->GetAt(i);
//		logFile.Write("%s : %d", "Elemento del ArrLS : ", i);
		
		ASSERT( pLS->IsKindOf( RUNTIME_CLASS( CLinSeg ) ) );
		// Ahora tenemos que saber a cuántas OMs afecta esta LinSeg
		GetArrPSsAfec(pLS, &ArrPSs);
		// También el número de piezas que se fabrica en esta combinada
		long lCant = pLS->GetlCantFab();
		// No podemos descontar más de lo que hay
		if (lCant > p_lCant) lCant = p_lCant;
		// Calculamos el porcentaje que le corresponde a cada orden. Primero la suma
		// de las cadencias afectadas
		double dSumaCad = 0;
		int iLim = ArrPSs.GetSize();

		bool bDeb = false;
		for (int iK = 0; iK < iLim; iK++)
		{
			CProdSeg* pPS = (CProdSeg*) ArrPSs.GetAt(iK);
			COrdMaq* pOM = pPS->m_pOM;
			dSumaCad += pOM->GetdCadenciaTL(p_eTL);
		}
		if ((int) dSumaCad != (int) pLS->m_dCadencia)
		{
			CString sMsg;
			sMsg.Format("Distintas cadencias en ReparteCant(TimeLine) : %f %f %s\n", dSumaCad, pLS->m_dCadencia, m_sID );
			TRACE(sMsg);
		}
		// Ahora repartimos la cantidad de acuerdo a los porcentajes
		// Primero ponemos el porcentaje en m_dAux
		for (int iK = 0; iK < ArrPSs.GetSize(); iK++)
		{
			CProdSeg* pPS = (CProdSeg*) ArrPSs.GetAt(iK);
			COrdMaq* pOM = pPS->m_pOM;
			pOM->m_dAux = pOM->GetdCadenciaTL(p_eTL) / dSumaCad;
		}
		for (int iK = 0; iK < ArrPSs.GetSize(); iK++)
		{
			CProdSeg* pPS = (CProdSeg*) ArrPSs.GetAt(iK);
			COrdMaq* pOM = pPS->m_pOM;
			
			pOM->m_dAuxCant += (pOM->m_dAux * (double) lCant);
			
			long lDescCant = (long) pOM->m_dAuxCant;
			if ((pOM->m_dAuxCant - lDescCant) > .5) lDescCant++;
			pOM->m_lAux = lDescCant;
			
			dAcumCant += pOM->m_dAuxCant;

		}

		// Descontamos las piezas asignadas
		p_lCant -= lCant;
		// Si ya hemos asignado todas, terminamos
		if (p_lCant <= 0) break;
	}
	// Devolvemos las piezas que no se han asignado, es decir, que "sobran".
	long lAcumCant = (long) dAcumCant;
	if ((dAcumCant - lAcumCant) > .5) lAcumCant++;
	// Actualizamos 
	m_lResto = lCantIni - lAcumCant;
	return p_lCant;
}

// Convierte el array de PSs, ordenado por FecIni, al array de LS, eliminando solapes a base de combinar
int 
CTimeLine::Desolapar(enum  eTimeLine p_eTL)
{
	CObArray ArrOM, ArrOMSort;
	// Borramos lo que pueda haber en ArrLS
	for (int iDel = 0; iDel < m_pArrLS->GetSize(); iDel++)
	{
		CLinSeg* pLinSeg = (CLinSeg*) m_pArrLS->GetAt(iDel);
		delete pLinSeg;
	}
	m_pArrLS->RemoveAll();

	int iNumOM = m_pArrPS->GetSize();
	if (iNumOM <= 0) return 0;
	
	// Creamos la primera LinSeg como una copia del primer PS
	CProdSeg* pPSBase = (CProdSeg*) m_pArrPS->GetAt(0);
	CLinSeg* pLSBase = new CLinSeg();
	pLSBase->m_AbSpan = pPSBase->m_AbSpan;
	pLSBase->m_dCadencia = pPSBase->m_pOM->GetdCadenciaTL(p_eTL);
	
	m_pArrLS->Add(pLSBase);
	// Si hay más, vamos creando el array de LinSeg, con las PSs combinadas
	if (iNumOM > 1)
	{
		int iIns = 0;
		for (int i=1; i < iNumOM; i++)  // Empezamos en 1 porque el 0 está ya procesado
		{
			CProdSeg* pPS = (CProdSeg*) m_pArrPS->GetAt(i);
			// Ahora buscamos dentro del array de OM´s al primero que tenga la fecha
			// de fin mayor que la de inicio de la nueva OM. Esto nos da el punto de
			// insercion. Guardamos el índice en iIns y un puntero a la OM en pLSCIni.
			iIns = -1;
			CLinSeg* pLS;
			// Para empezar creamos una nueva CLinSeg
			BOOL bNewInsert = FALSE;
			CLinSeg* pLSNew = new CLinSeg();
			pLSNew->m_AbSpan = pPS->m_AbSpan;
			pLSNew->m_dCadencia = pPS->m_pOM->GetdCadenciaTL(p_eTL);
			for(;;)
			{
				// Buscamos ahora un solapamiento entre la nueva LS que tenemos y las
				// LS que están ya en la matriz.
				// Primero buscamos el punto de inserción, en iINs
				for (int j=0; j < m_pArrLS->GetSize(); j++)
				{
					CLinSeg* pLSj = (CLinSeg*) m_pArrLS->GetAt(j);
										
					if (pLSj->m_AbSpan.GetFecFin() > pLSNew->m_AbSpan.GetFecIni())
					{
						iIns = j;
						pLS = pLSj;
						break;
					}
				}
				if (iIns < 0) // Si no hemos encontrado ninguno es que no hay solapamiento
				{			 // por lo que añadimos una nueva LinSeg al final del array.
					m_pArrLS->Add(pLSNew);
					bNewInsert = TRUE;
					break;
				}
				// Si iIns >= 0, es que hay que insertar, y puede haber solapamiento.
				// Comprobamos si hay solapamiento
				/* Debug
				CString sU;
				
				sU.Format("%s %s %s %s",
				FormatFec(pLSNew->m_AbSpan.GetFecIni()),
				FormatFec(pLSNew->m_AbSpan.GetFecFin()),
				FormatFec(pLS->m_AbSpan.GetFecIni()),
				FormatFec(pLS->m_AbSpan.GetFecFin()));
				*/
				if (!pLSNew->m_AbSpan.Overlaps(pLS->m_AbSpan))
				{
					// Si no hay solapamiento, sencillamente insertamos, porque todas
					// las LinSeg no tienen solapamientos entre si
					m_pArrLS->InsertAt( iIns+1, pLSNew );
					bNewInsert = TRUE;
					break;
				}
				// Si hay solapamiento, podemos tener varios casos.
				
				//	Que sean exactamente iguales
				if (pLSNew->m_AbSpan == pLS->m_AbSpan)
				{
					// Sumamos las dos cadencias y borramos la nueva
					pLS->m_dCadencia += pLSNew->m_dCadencia ;
					break;
				}
				// Que la fecha de inicio de la nueva sea menor que la de inicio de la insertada
				if (pLSNew->m_AbSpan.GetFecIni() < pLS->m_AbSpan.GetFecIni())
				{
					// Creamos una orden con lo que sobra por arriba de la nueva
					CLinSeg* pLSArriba = new CLinSeg(*pLSNew);
					pLSArriba->m_AbSpan.SetFecFin(pLS->m_AbSpan.GetFecIni());
					// Y actualizamos la nueva para que comienze a la par con la insertada
					pLSNew->m_AbSpan.SetFecIni(pLS->m_AbSpan.GetFecIni());
					
					// Cerramos el nuevo al comenzar el insertado y lo insertamos antes
					m_pArrLS->InsertAt( iIns, pLSArriba );
					continue;
				}
				// Ahora el comienzo de la nueva es igual o superior al de la antigua
				
				// Si es superior, dividimos la insertada en 2 para que sean iguales
				if (pLSNew->m_AbSpan.GetFecIni() > pLS->m_AbSpan.GetFecIni())
				{
					// Creamos una orden con lo que sobra por arriba de la insertada
					CLinSeg* pLSArriba = new CLinSeg(*pLS);
					pLSArriba->m_AbSpan.SetFecFin(pLSNew->m_AbSpan.GetFecIni());
					m_pArrLS->InsertAt( iIns, pLSArriba );
					// Y actualizamos la insertada para que comienze a la par con la nueva
					pLS->m_AbSpan.SetFecIni(pLSNew->m_AbSpan.GetFecIni());
					iIns++;
					continue;
				}
				// Ahora comienzan a la vez. Nos preocupamos por el final
				// Ahora tenemos 3 posibles casos:
				// 1) Que el fin de la nueva sea antes que el de la insertada
				if ( pLSNew->m_AbSpan.GetFecFin() < pLS->m_AbSpan.GetFecFin())
				{
					// Creamos una nueva LinSeg para cubrir el resto, basándonos en la 
					// ya insertada
					CLinSeg* pLSComp = new CLinSeg(*pLS);
					// Sumamos las dos cadencias en la ya insertada y le ponemos la fecha
					// de fin igual a la de fin de la nueva
					pLSComp->m_dCadencia += pLSNew->m_dCadencia ;
					pLSComp->m_AbSpan.SetFecFin(pLSNew->m_AbSpan.GetFecFin());
					// En la recién creada, ponemos la fecha de inicio como la misma
					pLS->m_AbSpan.SetFecIni(pLSNew->m_AbSpan.GetFecFin());
					// Y la insertamos tras la nueva insertada, actualizando los índices
					m_pArrLS->InsertAt( iIns, pLSComp );
					iIns++;					
					break;
					
				}
				// 2) Que sea el mismo
				if ( pLSNew->m_AbSpan.GetFecFin() == pLS->m_AbSpan.GetFecFin())
				{
					// Nos limitamos a cambiar la cadencia
					pLS->m_dCadencia += pLSNew->m_dCadencia ;
					break;
				}
				// 3) Que sea después
				if ( pLSNew->m_AbSpan.GetFecFin() > pLS->m_AbSpan.GetFecFin())
				{
					
					// Cambiamos la cadencia de la actual
					pLS->m_dCadencia += pLSNew->m_dCadencia ;
					
					// Y actualizamos el nuevo para que comienze al final del insertado
					pLSNew->m_AbSpan.SetFecIni(pLS->m_AbSpan.GetFecFin());
				}
			} // end for(;;)
			if (!bNewInsert) delete pLSNew;
			bNewInsert = FALSE;
		}
	}
	return m_pArrLS->GetSize();
}

void CTimeLine::Empty(BOOL p_BOrdMaqs)
{
	for (int j = 0; j < m_pArrPS->GetSize() ; j++)
	{
		CProdSeg* pProdSeg = (CProdSeg*) m_pArrPS->GetAt(j);
		ASSERT( pProdSeg->IsKindOf( RUNTIME_CLASS( CProdSeg ) ) );

		delete pProdSeg;
	}
	m_pArrPS->RemoveAll();
	for (int j = 0; j < m_pArrLS->GetSize() ; j++)
	{
		CLinSeg* pLinSeg = (CLinSeg*) m_pArrLS->GetAt(j);
		ASSERT( pLinSeg->IsKindOf( RUNTIME_CLASS( CLinSeg ) ) );

		delete pLinSeg;
	}
	m_pArrLS->RemoveAll();
	if (p_BOrdMaqs) m_pArrOM->RemoveAll();
}

CTimeLine::CTimeLine()
{ 
	m_pArrPS = new CObArrayOrd(PS_Bigger, PS_Equal);
	m_pArrLS = new CObArray();
	m_pArrOM = new CObArray();
	m_lResto = 0;
}

CTimeLine::~CTimeLine()
{
	Empty();
	delete m_pArrPS;
	delete m_pArrLS;
	delete m_pArrOM;
}

void
CTimeLine::Recalc(enum  eTimeLine p_eTL)
{
	Empty(FALSE); // Vaciamos todo menos las OMs
	
	for (int j = 0; j < m_pArrOM->GetSize() ; j++)
	{
		COrdMaq* pOrdMaq = (COrdMaq*) m_pArrOM->GetAt(j);
		ASSERT( pOrdMaq->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
		
		CProdSeg* pPS = new CProdSeg(pOrdMaq);	
		m_pArrPS->Add(pPS);
	}
	
	AplicarCalendarioArrPSs();
	// Si es para calcular márgenes, aplicamos las distancias de los centros
	if (p_eTL == Margen) AplicarDistancias();
	// Esta función es independiente de los datos del programa, por lo que la hacemos miembro de CTimeLine
	Desolapar(p_eTL);

}

void 
CTimeLine::AplicarCalendarioArrPSs()
{

	// Em esta matriz iremos guardando los PSs limpios, a los que ya no afectan inactividades
	CObArray ArrPStemp;
	// Empezamos buscando inactividades que afecten a alguna de las órdenes. Para ello primero hacemos 
	// un ciclo por las OMs, para ver cuáles están afectadas por alguna inactividad.
	for (int i = 0; i < m_pArrPS->GetSize(); i++)
	{
		CProdSeg* pPS = (CProdSeg*) m_pArrPS->GetAt(i);
		ASSERT( pPS->IsKindOf( RUNTIME_CLASS( CProdSeg ) ) );
		COrdMaq* pOM = (COrdMaq*) pPS->m_pOM;
		ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
		CObArray pArrInact;
		// GetArrInacts nos da un array de Inactividades ordenadas por fecha de inicio
		if (!pOM->GetArrInacts(pArrInact)) 	// Si no tiene inactividades lo copiamos y seguimos
		{
			//CProdSeg* pPS = (CProdSeg*) m_pArrPS->GetAt(iM);
			//ASSERT( pPS->IsKindOf( RUNTIME_CLASS( CProdSeg ) ) );
			//m_pArrPSout->Add(pOMComb);
			// Lo guardamos en la matriz temporal
			ArrPStemp.Add(pPS);
			continue;

		}
		CTime FecIniPS = pPS->m_AbSpan.GetFecIni();
		CTime FecFinPS = pPS->m_AbSpan.GetFecFin();
		// Resulta que sí que tenemos inactividades. Bueno, pues vamos a ver 
		// con qué tramos de las combinadas nos coinciden.
		// Hacemos un ciclo por cada una de las inactividades
		for (int j=0; j < pArrInact.GetSize(); j++)
		{
			CAbSpan* pAbSpanInact = (CAbSpan*) pArrInact.GetAt(j);
			CTime FecIniI = pAbSpanInact->GetFecIni();
			CTime FecFinI = pAbSpanInact->GetFecFin();
			// En principio las inactividades deberían estar o fuera, o comprendidas dentro de una orden
			// pero por si acaso, consideramos los otros casos.
			if (FecFinI <= FecIniPS) continue;
			if (FecIniI >= FecFinPS) break;

			if (FecIniI <= FecIniPS && FecFinI <= FecFinPS) FecIniPS = FecFinI;
			if (FecFinI >= FecFinPS && FecIniI >= FecIniPS) FecFinPS = FecIniI;
			// Este es el caso normal, que esté comprendida
			if (FecIniI >= FecIniPS && FecFinI <= FecFinPS)
			{
				// Nos generará un nuevo segmento de producción sobre la misma OM
				CProdSeg* pPS = new CProdSeg(pOM);
				// cuya fecha de inicio es la de  inicio de la PS actual
				pPS->m_AbSpan.SetFecIni(FecIniPS);
				// y cuya fecha de fin es la de  inicio de la inactividad
				pPS->m_AbSpan.SetFecFin(FecIniI);
				// Guardamos este nuevo en la matriz temporal, para que no moleste, dado que no le puede afectar
				// ninguna inactividad ya
				ArrPStemp.Add(pPS);
				// Y ahora la PS queda reducida a comenzar tras la inactividad
				FecIniPS = FecFinI;
			}
		}
		// Ponemos los valores a lo que hemos calculado que queda de la PS original
		pPS->m_AbSpan.SetFecFin(FecFinPS);
		pPS->m_AbSpan.SetFecIni(FecIniPS);
		// Y la metemos también en el Array
		ArrPStemp.Add(pPS);
	}
	// Ya hemos terminado con todas. Ahora copiamos el array temporal sobre el original, ordenando de paso
	m_pArrPS->RemoveAll();
	for (int i = 0; i < ArrPStemp.GetSize(); i++)
	{
		m_pArrPS->Add(ArrPStemp.GetAt(i));
	}
}


// Aplicamos las distancias que pueda haber entre centros
void 
CTimeLine::AplicarDistancias()
{
	// Em esta matriz iremos guardando los PSs limpios, a los que ya no afectan inactividades
	CObArray ArrPStemp;
	// Empezamos buscando OMs que tengan una distancia, dentro del Array de PS
	for (int i = 0; i < m_pArrPS->GetSize(); i++)
	{
		CProdSeg* pPS = (CProdSeg*) m_pArrPS->GetAt(i);
		ASSERT( pPS->IsKindOf( RUNTIME_CLASS( CProdSeg ) ) );
		COrdMaq* pOM = (COrdMaq*) pPS->m_pOM;
		ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
				
		int iDiasRetorno = pOM->GetDiasRetorno();
		if ( iDiasRetorno > 0)
		{
			pPS->m_AbSpan.SetFecIni(AddDays(pOM->GetFecIni(), iDiasRetorno));
			pPS->m_AbSpan.SetFecFin(AddDays(pOM->GetFecFin(), iDiasRetorno));
			// La borramos y la volvemos a crear, para que se ordene
		}
		ArrPStemp.Add(pPS);
	}
	// Ya hemos terminado con todas. Ahora copiamos el array temporal sobre el original, ordenando de paso
	m_pArrPS->RemoveAll();
	for (int i = 0; i < ArrPStemp.GetSize(); i++)
	{
		m_pArrPS->Add(ArrPStemp.GetAt(i));
	}
	
}

