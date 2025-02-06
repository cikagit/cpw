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
#include "TimeLine.h"  
#include "Turnos.h"  

#include "obarrord.h"

#include <strstream>
#include <fstream>
using namespace std;



IMPLEMENT_SERIAL(CProg, CObject, 1)
 
CProg::CProg()
{
	CTime FecIni = CTime::GetCurrentTime();  
	//FecIni = CTime(FecIni.GetYear(), FecIni.GetMonth(), FecIni.GetDay(), TK_TURNO3_FIN,0,0);
	FecIni = CTurnos::GetFecIni(FecIni, 1);
    CTime FecFin = FecIni;
	FecFin = AddDays(FecFin, TK_MIN_PROG);
    m_Abs = CAbSpan(FecIni , FecFin);
	m_lTimeScale = (long) AfxGetApp()->GetProfileInt("Valores Iniciales", "Escala", TK_ESCALA_DEF);
    ASSERT(g_pCache); 
    m_pArrPMs = new CObArray();         
    ASSERT(m_pArrPMs);
    CStringArray ArrMaquinas;
	if (g_pCache->GetArrMaquinas(ArrMaquinas))
	{                                
		for (int i=0; i< ArrMaquinas.GetSize(); i++)
		{                                  
			CProgMaq* pPM = new CProgMaq(ArrMaquinas[i]);
			pPM->SetpBufMaq( g_pCache->AddMaquina( ArrMaquinas[i] ) );  
			AddPM(pPM);
		}
	} else 
	{
		AfxMessageBox(STRID(IDS_ERR_NOMAQAB));
		return;
	}  
	m_pArrTLPiezas = new CObArrayOrd(&TL_Bigger, &TL_Equal);
	m_pArrTLMargen = new CObArrayOrd(&TL_Bigger, &TL_Equal);
	m_pArrTLMezcla = new CObArrayOrd(&TL_Bigger, &TL_Equal);

}

CProg::~CProg()
{                  
	ClearPMs();    
	delete m_pArrPMs;
	ClearOMs();
	delete m_pArrTLPiezas;
	delete m_pArrTLMargen;
	delete m_pArrTLMezcla;

} 

void
CProg::ClearPMs()
{
	int i;
	ASSERT_VALID (m_pArrPMs);
	for ( i=0; i< m_pArrPMs->GetSize(); i++)
	{                                  
		CProgMaq* pPM = (CProgMaq *) ((*m_pArrPMs)[i]);
		ASSERT( pPM->IsKindOf( RUNTIME_CLASS( CProgMaq ) ) );
		delete pPM;
	}
	m_pArrPMs->RemoveAll();
}

void
CProg::ClearOMs()
{
		int i;
		ASSERT_VALID (m_pArrTLPiezas);
		for ( i=0; i< m_pArrTLPiezas->GetSize(); i++)
		{                                  
			CTimeLine* pTL = (CTimeLine *) m_pArrTLPiezas->GetAt(i);
			ASSERT( pTL->IsKindOf( RUNTIME_CLASS( CTimeLine ) ) );
			delete pTL;
		}
		m_pArrTLPiezas->RemoveAll();
		ASSERT_VALID (m_pArrTLMargen);
		for ( i=0; i< m_pArrTLMargen->GetSize(); i++)
		{                                  
			CTimeLine* pTL = (CTimeLine *) m_pArrTLMargen->GetAt(i);
			ASSERT( pTL->IsKindOf( RUNTIME_CLASS( CTimeLine ) ) );
			delete pTL;
		}
		m_pArrTLMargen->RemoveAll();
		ASSERT_VALID (m_pArrTLMezcla);
		for ( i=0; i< m_pArrTLMezcla->GetSize(); i++)
		{                                  
			CTimeLine* pTL = (CTimeLine *) m_pArrTLMezcla->GetAt(i);
			ASSERT( pTL->IsKindOf( RUNTIME_CLASS( CTimeLine ) ) );
			delete pTL;
		}
		m_pArrTLMezcla->RemoveAll();
}

void
CProg::RelinkAll()
{
	for(int i=0; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		ASSERT_VALID(pPM);
		pPM->Relink();
		for (int j=0; j< pPM->GetNumOM() ; j++)
		{
			COrdMaq* pOM = pPM->GetpOM(j);
			ASSERT_VALID(pOM);
			pOM->Relink();
		}
	}
}
// Realizamos una copia de un CProg existente. Todos los objetos dependientes de él
// (PMs y OMs) deben ser copiados.
CProg&
CProg::operator =(const CProg& p_Prog)
{
	ClearPMs();
	SetFecIni(p_Prog.GetFecIni());
	SetFecFin(p_Prog.GetFecFin());
	m_lTimeScale = p_Prog.GetTimeScale();
    for (int i=0; i< p_Prog.GetNumPM(); i++)
	{                               
		CProgMaq* pPMCop = p_Prog.GetpPM(i);
		CProgMaq* pPM = new CProgMaq(pPMCop->GetsID());
		pPM->SetpBufMaq( g_pCache->AddMaquina( pPMCop->GetsID() ) );  
		AddPM(pPM);
		*pPM = *pPMCop;
	}
	return *this;
}

// Borra la ProgMaq indicada por el índice p_iPM.
void 
CProg::DeletePM( int p_iPM )
{
	CProgMaq* pPM = GetpPM(p_iPM);
	ASSERT_VALID(pPM);
	delete pPM;
	m_pArrPMs->RemoveAt(p_iPM);
}

// Mueve una ProgMaq a la izquierda o a la derecha. Devuelve False si no puede.
BOOL
CProg::MovePM( CProgMaq* p_pPM, enum EDirMove p_eDirMove)
{
	int iPM = Find(p_pPM);
	if (iPM == INT_MAX) return FALSE;
	// Calculamos el indice de fin. 
	int iPMEnd = (p_eDirMove == Left  ? iPM - 1 : iPM + 1); 

	if (iPMEnd < 0 || iPMEnd >= GetNumPM()) return FALSE;
	long lGFH = p_pPM->GetlGFH();
	
	CProgMaq* pPMSubs = GetpPM(iPMEnd);

	if (lGFH != pPMSubs->GetlGFH()) return FALSE; // Los GFHs deben ser continuos. No se
												  // puede mover tras una maquina de otro.

	m_pArrPMs->RemoveAt(iPM);
	m_pArrPMs->InsertAt(iPMEnd, p_pPM);

	return TRUE;
}

void
CProg::SwitchPM( int p_iInd, int p_iInd2 )
{
	CProgMaq* pPM = (CProgMaq*) m_pArrPMs->GetAt(p_iInd);
	CProgMaq* pPM2 = (CProgMaq*) m_pArrPMs->GetAt(p_iInd2);

	m_pArrPMs->SetAt( p_iInd , pPM2);
	m_pArrPMs->SetAt( p_iInd2 , pPM);
}
		
void
CProg::UnselectAll()
{
	int numPM = GetNumPM();
	for (int i = 0; i < numPM; i++)
	{
		CProgMaq* pMaq = GetpPM(i);
		ASSERT_VALID(pMaq);
		int numOM = pMaq->GetNumOM();     
		for (int j=0; j< numOM; j++)
		{                                   
			COrdMaq* pOM = pMaq->GetpOM(j);
			ASSERT_VALID(pOM);
			pOM->SetBSelected(FALSE);
		}
	}
}

void CProg::Serialize(CArchive& ar)
{                     
	long lVoid = 0;
	double dVoid = 0;
	CTime FecVoid = 0;
	CString sVoid = "";

	ASSERT_VALID(g_pCache);

	CObject::Serialize(ar);
	
	if (!ar.IsStoring()) ClearPMs();
	
	//AfxMessageBox("Antes de intervalo");
	m_Abs.Serialize(ar);
	//AfxMessageBox("Antes de máquinas");
	m_pArrPMs->Serialize(ar);   
	//AfxMessageBox("Antes de parámetros");
	
	// Escribimos los miembros de Prog
	if (ar.IsStoring())
	{        
  		ar << m_lTimeScale;
		ar << m_lNumDias;
		
		ar << lVoid;
		ar << dVoid;
		ar << FecVoid;
		ar << sVoid;
		ar << lVoid;
		ar << dVoid;
		ar << FecVoid;
		ar << sVoid;
		ar << lVoid;
		ar << dVoid;
		ar << FecVoid;
		ar << sVoid;
		ar << lVoid;
		ar << dVoid;
		ar << FecVoid;
		ar << sVoid;
  	}
	else
	{
		ar >> m_lTimeScale;
		ar >> m_lNumDias;

		ar >> lVoid;
		ar >> dVoid;
		ar >> FecVoid;
		ar >> sVoid;
		ar >> lVoid;
		ar >> dVoid;
		ar >> FecVoid;
		ar >> sVoid;
		ar >> lVoid;
		ar >> dVoid;
		ar >> FecVoid;
		ar >> sVoid;
		ar >> lVoid;
		ar >> dVoid;
		ar >> FecVoid;
		ar >> sVoid;
		
	}
	
	if (!ar.IsStoring())
	{
		ASSERT_VALID (m_pArrPMs);
		for (int i=0; i< m_pArrPMs->GetSize(); i++)
		{                                  
			//CProgMaq* pPM = (CProgMaq *) ((*m_pArrPMs)[i]);
			CProgMaq* pPM = (CProgMaq *) m_pArrPMs->GetAt(i);
			ASSERT( pPM->IsKindOf( RUNTIME_CLASS( CProgMaq ) ) );
			pPM->SetProg(this);
		}                     
		
	}
}

void
CProg::RecalcInacts()
{                                                               
	for (int i=0; i< GetNumPM(); i++)
	{                                  
		CProgMaq* pPM = GetpPM(i);
    	g_pCache->AssignInacts(*pPM);
	}          
}

void
CProg::Recalc()
{             
	// Hay que recalcular al revés para que los márgenes salgan bien
	for (int i=GetNumPM() - 1; i>= 0; i--)
	{                                  
		CProgMaq* pPM = GetpPM(i);
    	pPM->Recalc();
	}  
	
	UpdateArrTL();
	CalcMezclasTL();
	// Ahora recalculamos las OFs
	
	g_pCache->RecalcOFs(this);
	
	// Y los posibles conflictos de moldes

	CalcConflictosMoldes();
	CalcConflictosMoldesElem();

	CCpwDoc* pDoc = GetpDoc();
	
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL);
}


void
CProg::CalcMezclasTL(const char* p_sIDMezcla)
{
		
	CObArray* pArrBufMez = g_pCache->GetArrBufMezcla();
	int iLim = pArrBufMez->GetSize();
	// Hacemos un ciclo por todas las Mezclas
	for(int i = 0; i < iLim; i++)
	{
		CBufMezcla* pBufMez = (CBufMezcla*) pArrBufMez->GetAt(i);
		ASSERT( pBufMez->IsKindOf( RUNTIME_CLASS( CBufMezcla ) ) );
		if (p_sIDMezcla && pBufMez->m_sID != p_sIDMezcla) continue;

		CTimeLine* pTL = GetpTL(pBufMez->m_sID, Mezcla);
		if (!pTL) continue;
		// La cantidad de mezcla que tenemos es
		long lCant = pBufMez->GetlCantidad();
		
		// Vamos descontando por la matriz de solapados
		// Repartimos la cantidad entre las OMs
		long lRestoCant = pTL->ReparteCant(lCant, Mezcla);
		// Ahora tenemos la cantidad en el m_lAux de cada OM
		// Hacemos un ciclo por las OMs afectadas a ver qué hay
		for (int i=0; i < pTL->m_pArrOM->GetSize(); i++)
		{
			COrdMaq* pOM = (COrdMaq*) pTL->m_pArrOM->GetAt(i);
			long lRepCant = pOM->m_lAux;
			pOM->SetRestoMezcla((lRepCant < 0) ? 0 : lRepCant);
			if (lRepCant <= 0) continue;
		}
		
		// Debug
		/*
		int iNumOM;
		CDlgDebug DlgD;
		iNumOM = pTL->m_pArrOM->GetSize();
		for (int iD=0; iD< iNumOM; iD++)
		{
			COrdMaq* pOM = (COrdMaq*) pTL->m_pArrOM->GetAt(iD);
			ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
			
			if (pOM->GetlPeso() <= 0) continue;
			long lCant = pBufMez->GetlCantMezcla() * 1000 / pOM->GetlPeso();
			
			CString sMsg;
			sMsg.Format(" %s ; %s - %s" 
				, FormatLong(lCant, 10)
				, pOM->GetsMaquina()
				, FormatLong(pOM->m_lAux, 10)
				);
			
			DlgD.m_ArrStr.Add(sMsg);
			
		} 	
		if (DlgD.DoModal() != IDOK) return ;
		
		*/
		// End Debug

	}
}
// Devuelve el numero de ProgMaq en Prog.
int
CProg::GetNumPM() const
{ 
	if (m_pArrPMs) return m_pArrPMs->GetSize();
	else return 0;
}   

CProgMaq* 
CProg::GetpPM(int p_NumMaq) const
{                               
	ASSERT_VALID(m_pArrPMs);
	return (CProgMaq*) m_pArrPMs->GetAt(p_NumMaq);
}  

CProgMaq*
CProg::FindMaq(const char* p_ID) const
{
	for(int i=0; i<m_pArrPMs->GetSize(); i++)
	{
		CProgMaq* pPM = (CProgMaq *) m_pArrPMs->GetAt(i);
		ASSERT( pPM->IsKindOf( RUNTIME_CLASS( CProgMaq ) ) );		
		if (pPM->GetsID() == p_ID) return pPM;
	}                                        
	return NULL;
}            

CProgMaq*
CProg::FindMaqByDesc(const char* p_sDesc) const
{
	for(int i=0; i<m_pArrPMs->GetSize(); i++)
	{
		CProgMaq* pPM = (CProgMaq *) m_pArrPMs->GetAt(i);
		ASSERT( pPM->IsKindOf( RUNTIME_CLASS( CProgMaq ) ) );		
		if (pPM->GetsDescripcion() == p_sDesc) return pPM;
	}                                        
	return NULL;
}            


// Obtenemos un array de OMs ordenados por máquinas
int CProg::GetArrMaqOM(CObArray& p_ArrOM)
{
	p_ArrOM.RemoveAll();             
	for(int i=0; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		ASSERT_VALID(pPM);
		for (int j=0; j< pPM->GetNumOM() ; j++)
		{
			COrdMaq* pOM = pPM->GetpOM(j);
			ASSERT_VALID(pOM);
			p_ArrOM.Add(pOM);
		}
	}
	return p_ArrOM.GetSize();
}

// Obtenemos un array de OMs ordenados por máquinas, pero con nuevas OMs cuando se interrumpe una
// por el calendario. Por tanto hacemos copias de todas, que luego habrán de ser borradas.
int CProg::GetArrMaqOMLis(CObArray& p_ArrOM)
{
	p_ArrOM.RemoveAll();             
	for(int i=0; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		ASSERT_VALID(pPM);
		CTime FecAct = DATE_MIN;
		
		for (int j=0; j< pPM->GetNumOM() ; j++)
		{
			COrdMaq* pOM = pPM->GetpOM(j);
			COrdMaq* pOMRest = new COrdMaq();
			long lCantAcum = 0;
			*pOMRest = *pOM;
			if (FecAct == DATE_MIN) FecAct = pOM->GetFecIni();
			// Ya tenemos la orden. 
			// Antes de hacer nada, vemos si hay un hueco de una hora o más con la anterior
			// y le hacemos una OF de parada
			CTimeSpan tsHueco = pOM->GetFecIni() - FecAct;
			if (tsHueco.GetTotalSeconds() > TK_SEGS_HORA) 
			{
				COrdMaq* pOMNew = new COrdMaq();
				*pOMNew = *pOM;
				pOMNew->SetFecIni(FecAct);
				pOMNew->SetFecFin(pOM->GetFecIni());
				CTimeSpan tsInt =  pOMNew->GetFecFin() - pOMNew->GetFecIni();
				long lSecs = (long) tsInt.GetTotalSeconds();
				//pOMNew->ModifTimeSpanInact(lSecs);
				
				pOMNew->SetpBufArt(g_pCache->FindpArticulo(""));
				pOMNew->SetpBufMolde(g_pCache->FindpMolde(""));
				pOMNew->SetlCantidad(1);
				

				p_ArrOM.Add(pOMNew);

			}
			// Actualizamos la fecha actual de fin
			FecAct = pOM->GetFecFin();
			//Vemos si está interrumpida por una inactividad, o más
			// No podemos usar la OMC asociada porque esa está compuesta de todas las OM del artículo
			// y ahora sólo nos interesa ésta.
			CTime FecIni = pOM->GetFecIni();
			CTime FecFin = pOM->GetFecFin();
			// Hacemos un ciclo por las inactividades a ver si alguna afecta a nuestra OM
			for (int iInact = 0; iInact < pPM->GetNumInacts(); iInact++)
			{
				CAbSpan Inact = *(pPM->GetpInact(iInact));
				CString sFecIni = FormatFec(FecIni);
				CString sFecFinInAct = FormatFec(Inact.GetFecFin());
				CString sFecIniInAct = FormatFec(Inact.GetFecIni());
				if (Inact.GetFecIni() >= FecFin) break;
				if (Inact.GetFecFin() <= FecIni) continue;
				// Si llegamos aquí es que se nos solapa, por tanto nos saldrán dos OMs
				// Una, la que teníamos, que ahora termina al empezar la inactividad
				// Si nos queda demasiado pequeña, la borramos
				COrdMaq* pOMNew = new COrdMaq();
				*pOMNew = *pOM;
				pOMNew->SetFecIni(FecIni);
				pOMNew->SetFecFin(Inact.GetFecIni());
				CTimeSpan tsInt =  pOMNew->GetFecFin() - pOMNew->GetFecIni();
				long lSecs = (long) tsInt.GetTotalSeconds();
				if ( lSecs < TK_SEGS_MIN_LIS ) delete pOMNew;
				else
				{
					pOMNew->ModifTimeSpanInact(lSecs);	// Esta función descuenta la inact. y coloca la cantidad
					lCantAcum += pOMNew->GetlCantidad();
					p_ArrOM.Add(pOMNew);
				}
				// Ahora hacemos una OM ficticia con la duración de la inactividad
				COrdMaq* pOMInact = new COrdMaq();
				*pOMInact = *pOM;
				pOMInact->SetFecIni(Inact.GetFecIni());
				pOMInact->SetFecFin(Inact.GetFecFin());

				tsInt =  pOMInact->GetFecFin() - pOMInact->GetFecIni();
				lSecs = (long) tsInt.GetTotalSeconds();
				//pOMInact->ModifTimeSpanInact(lSecs);
				
				pOMInact->SetpBufArt(g_pCache->FindpArticulo(""));
				pOMInact->SetpBufMolde(g_pCache->FindpMolde(""));
				pOMInact->SetlCantidad(1);
				

				p_ArrOM.Add(pOMInact);

				FecIni = pOMInact->GetFecFin();

			}
			// Hacemos el ciclo hasta que ya no se solapan más inactividades, y nos quedará un resto

			pOMRest->SetFecIni(FecIni);
			CTimeSpan tsInt = pOMRest->GetFecFin() - pOMRest->GetFecIni();
			long lSecs = (long) tsInt.GetTotalSeconds();
			long lCantTot = pOMRest->GetlCantidad() - lCantAcum;
			pOMRest->ModifTimeSpanInact(lSecs);	// Esta función descuenta la inact. y coloca la cantidad
			// Ajustamos la cantidad para que sume exacto
			pOMRest->SetlCantidad(lCantTot);
			if (pOMRest->GetTotalTs().GetTotalSeconds() < TK_SEGS_MIN_LIS || lCantTot < 0)
			{
				delete pOMRest;
			}
			else
			{
				p_ArrOM.Add(pOMRest);
			}

		}
	}
	return p_ArrOM.GetSize();
}



// Obtenemos un array de BufLisMezclas para la consulta de necesidades externas de mezclas
int CProg::GetArrMezExt(CObArray& p_ArrMezExt, const CString& p_sDesdeMaq, const CString& p_sHastaMaq)
{
	p_ArrMezExt.RemoveAll();             
	for(int i=0; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		if (pPM->GetlGFH() != TK_GFH_INYECCION) continue;
	
		// Para Berriatua son las del local externo, para las demás plantas, todos los locales
		// Sustituido por selección por número de máquina
		//if (( GetNumEmpresa(g_sEmpresa) != TK_EMPRESA_BERRIATUA ) 
		//	|| pPM->GetsIDLocal() != "1")
		if ((p_sDesdeMaq == "" || atol(pPM->GetsID()) >= atol(p_sDesdeMaq)) && 
			 (p_sHastaMaq == "" || atol(pPM->GetsID()) <= atol(p_sHastaMaq) ) )
		{
			for (int j=0; j< pPM->GetNumOM() ; j++)
			{
				COrdMaq* pOM = pPM->GetpOM(j);
				ASSERT_VALID(pOM);
				int iInd;
				CBufOF* pBufOF = g_pCache->FindOFArt(pOM->GetsID(), iInd);
				
				CBufLisMezclas* pBufLisMezclas = new CBufLisMezclas;
				pBufLisMezclas->m_sIDLocal = pPM->GetsIDLocal();
				pBufLisMezclas->m_sIDArticulo = pOM->GetsID();
				if (pBufOF)
					pBufLisMezclas->m_sIDCliente = pBufOF->m_sNomcli;
				else
					pBufLisMezclas->m_sIDCliente = "";
				pBufLisMezclas->m_lMezcla = pOM->GetlNecMezcla();
				pBufLisMezclas->m_FecIni = AddDays(pOM->GetFecIni(), (pPM->GetiDiasMezcla() * -1));
				pBufLisMezclas->m_lCant = (long) ((double) pOM->GetlCantidad() * pOM->GetdPeso());
				p_ArrMezExt.Add(pBufLisMezclas);
			}
		}
	}
	return p_ArrMezExt.GetSize();
}

// Obtenemos un array de OMs que tengan la misma OF
int	
CProg::GetArrOM( CObArray* p_pArrOM, CBufOF* p_pOF )
{
	ASSERT_VALID(p_pArrOM);
	ASSERT_VALID(p_pOF);
	p_pArrOM->RemoveAll();             
	int retVal = 0;
	for(int i=0; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		ASSERT_VALID(pPM);
		for (int j=0; j< pPM->GetNumOM() ; j++)
		{
			COrdMaq* pOM = pPM->GetpOM(j);
			ASSERT_VALID(pOM);
			if (pOM->GetpOF() == p_pOF->GetID())			
			{
				retVal++;
				p_pArrOM->Add(pOM);
			}
		}
	}
	return retVal;
}
// Obtenemos un array de OMs que tengan el mismo Articulo
int	
CProg::GetArrOMArt( CObArray* p_pArrOM, const CString p_sIDArt )
{
	ASSERT_VALID(p_pArrOM);
	p_pArrOM->RemoveAll();             
	int retVal = 0;
	for(int i=0; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		ASSERT_VALID(pPM);
		for (int j=0; j< pPM->GetNumOM() ; j++)
		{
			COrdMaq* pOM = pPM->GetpOM(j);
			ASSERT_VALID(pOM);
			if (p_sIDArt == pOM->GetsID())
			{
				retVal++;
				p_pArrOM->Add(pOM);
			}
		}
	}
	return retVal;
}


// Mueve una orden a una máquina, en el punto indicado por p_iInd (p_iInd == 0, la primera)
// p_iInd es el índice donde vamos a insertar, si en ese índice hay una OM, quedaremos por delante de ella.
void 
CProg::MoveOrdMaq(CProgMaq* p_pPM, COrdMaq* p_pOM, int p_iInd)
{

	// Obtenemos la máquina en la que está ahora la OM
	CProgMaq* pPM = p_pOM->GetpPM();
	ASSERT_VALID(pPM);
	if (!pPM) return;
	// Preparamos una variable para la OM linkada, caso de que exista
	COrdMaq* pOMLink = NULL;
	// Calculamos ahora en oInd, el índice en el que está la OM en su máquina original.
	int oInd = p_pOM->PMIndex();
	if (oInd < 0) return;
	// Es importante saber si estamos moviendo en la misma máquina
	BOOL BMismaMaquina = (pPM == p_pPM);
	int iIndRealMov = 0;
	// Si vamos a mover a la misma máquina, comprobamos que no sea justo 1 arriba (resultado Nulo), ni el mismo
	if (BMismaMaquina)
	{
		if (p_iInd == oInd ) return;
		if (p_iInd == oInd + 1) return;
		// y más abajo, comprobamos que no sea justo 1 más abajo, si está linkado por abajo
		if (p_pOM->GetBNextLink() && p_iInd == (oInd + 2)) return;
		// comprobamos que no sea justo 2 más arriba, si está linkado por arriba
		if (p_pOM->GetBPrevLink() && p_iInd == (oInd - 1)) return;
	
		// Guardamos el índice real de la OM a la que vamos a mover
		iIndRealMov = p_iInd - 1;	// porque p_iInd apunta a la OM delante de la cual movemos
		
		// Si vamos a mover a la misma máquina, y más abajo, habrá que restarle 1 al índice objetivo
		// por que el borrado de la OM que hace que los índices se desplacen 1.hacia arriba
		// Si está linkada, habrá que restar 2, porque se borran 2
	
		if (p_iInd > oInd ) 
		{
			p_iInd--;
			if (p_pOM->GetBNextLink() || p_pOM->GetBPrevLink()) p_iInd--;
		}
	}
	// Si vamos a mover entre dos linkadas, movemos tras la segunda
	// primero obtenemos un puntero a la OM tras la que se va a mover, para saber si está linkada
	COrdMaq* pOMp = NULL;
	if (p_iInd >= 0)
	{
		pOMp = p_pPM->GetpOM(iIndRealMov);
		if (pOMp && pOMp->GetBNextLink() && p_pPM->GetNumOM() > p_iInd) p_iInd++;
	}
	// Eliminamos ahora la OM u OMs de la lista de su PM
	if (oInd > -1) 
	{
		pPM->RemoveAt(oInd);
		// Si está linkada abajo, eliminamos la siguiente también
		if (p_pOM->GetBNextLink() && oInd < pPM->GetNumOM()) 
		{
			pOMLink = pPM->GetpOM(oInd);
			pPM->RemoveAt(oInd);
		}
		// Si está linkada arriba, eliminamos la anterior también
		if (p_pOM->GetBPrevLink() && (oInd-1) >= 0) 
		{
			pOMLink = pPM->GetpOM(oInd-1);
			pPM->RemoveAt(oInd-1);
		}
	}
	else 
	{
		TRACE("Error, no encontrada OM en CProg::MoveOrdMaq()");
		return;
	}
	//if (pPM == p_pPM) oInd = min(oInd, p_iInd);
	
	// Si hay una linkada después, la movemos primero, para que quede detrás.
	if (p_pOM->GetBNextLink() && pOMLink) p_pPM->InsertOM(pOMLink, p_iInd);
	p_pPM->InsertOM(p_pOM, p_iInd);
	// Si hay una linkada antes, la movemos después, para que quede antes.
	if (p_pOM->GetBPrevLink() && pOMLink) p_pPM->InsertOM(pOMLink, p_iInd);
	
	pPM->RecalcTL();
	if (!BMismaMaquina) 
	{
		p_pPM->RecalcTL();
	}
}

  
COrdMaq*
CProg::GetPrevOM(const COrdMaq* p_pOM, long p_lGFH) const
{
	// Obtenemos el índice de la máquina de la OrdMaq para empezar a
	// buscar a la derecha del mismo
	CProgMaq* pPM = p_pOM->GetpPM();
	if (!pPM) return NULL;
	long lGFH = pPM->GetlGFH();
	int ind = Find(pPM);
	if (ind == INT_MAX) return NULL;
	// Calculamos el límite y añadimos uno para buscar más a la derecha
	int lim = GetNumPM();
	ind++;
	//long UniqID = xpOM->GetUniqID();
	CString sPiezaBase = p_pOM->GetsNombre();
	
	while (ind < lim)
	{
		CProgMaq* pPMInd = GetpPM(ind);
		ASSERT_VALID(pPMInd);
		ind++;
		// Si estamos en el mismo GFH no es la operacion anterior, y seguimos
		if (lGFH == pPMInd->GetlGFH()) continue;
		// Si estamos buscando un GFH concreto (parametro p_lGFH > -1)
		// solo paramos en ese
		if (p_lGFH > -1)
			if (pPMInd->GetlGFH() != p_lGFH) continue;

			
		for(int j = 0; j < pPMInd->GetNumOM(); j++)
		{
			COrdMaq* pOMInd = pPMInd->GetpOM(j);
			// Si la OrdMaq que estamos mirando es la misma
			if (*pOMInd == *p_pOM) return pOMInd;
		}
	}
	return NULL;
}

// Devuelve el índice de la ProgMaq en Prog o INT_MAX si no existe.
int 
CProg::Find(const CProgMaq* p_pPM) const 
{
	int lim = GetNumPM();
	for(int i = 0; i < lim; i++)
		if (p_pPM == GetpPM(i)) return i;
	return INT_MAX;
}



// Cargamos órdenes en un GFH. Tomamos todas las órdenes seleccionadas
// de los GFHs anteriores.
void
CProg::CargaGFH(long p_lGFH)
{
	// Des-seleccionamos el GFH al que vamos a cargar
	
	int iNumPMs = 0;
	int lim = GetNumPM();
	// Desseleccionamos el que se va a cargar
	for(int i = 0; i < lim; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		ASSERT_VALID(pPM);
		if (pPM->GetlGFH() == p_lGFH)
			pPM->Selec(FALSE, SelecFalse);
	}
	
	// Creamos una lista con las ordenes que vamos a cargar, que
	// son todas las seleccionadas de los GFH's anteriores
	
	CObArray		ArrOMs;
	
	for(int i = 0; i < lim; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		ASSERT_VALID(pPM);
		if (pPM->GetlGFH() != p_lGFH)
		{
			for(int j = 0; j < pPM->GetNumOM(); j++)
			{
				COrdMaq* pOM = pPM->GetpOM(j);
				// Si está seleccionada y no está limitada al GFH
				if (pOM->GetBSelected() && ! pOM->LimitGFH(p_lGFH))	
				{
					pOM->SetBSelected(FALSE);	// La des-seleccionamos
					// Y la añadimos, pero las dobles y triples hay que multiplicarlas
					// Al multiplicarlas se convierten en otras piezas, que ya no son
					// múltiples, por lo que no hay más que comprobar si son dobles o triples
					
						COrdMaq* pNewOM = new COrdMaq();
						pNewOM->SetValues(pOM);
						pNewOM->m_FecOrd = pOM->GetFecIni();
						AddOMOrd( ArrOMs, pNewOM );
						pNewOM->SetBSelected(TRUE);
				} // Si está seleccionada 
			} // Ciclo de OMs
		}	// Si es del GFH correcto
	} // Ciclo de PMs
	
	// Dado que algunas ordenes pueden haberse traspasado antes,
	// hay que borrarlas. Recorremos el GFH al que traspasamos
	// buscando las ordenes que correspondan.
	
	for(int iLista = 0; iLista < ArrOMs.GetSize(); iLista++) // Hacemos ciclo por las ordenes a traspasar
	{
		COrdMaq* pOMBorr = (COrdMaq	*) ArrOMs.GetAt(iLista);
		ASSERT_VALID(pOMBorr);
		
		for(int i = 0; i < lim; i++)	// Buscamos de las maquinas de GFH a traspasar
		{
			CProgMaq* pPM = GetpPM(i);
			ASSERT_VALID(pPM);
			if (pPM->GetlGFH() == p_lGFH) // Si la máquina es del GFH, miramos si borrar
			{
				for(int j = 0; j < pPM->GetNumOM(); j++)
				{
					COrdMaq* pOM = pPM->GetpOM(j);
					if (pOM->GetsID() == pOMBorr->GetsID())	// Esto tiene problemas si dividimos OMs pero no hacerlo es generar confusión
					{
						delete pOM;
						pPM->RemoveAt(j);
						j--;
						// Aquí probablemente en vez del j--
						// se pueda hacer un break,
						// pero lo dejamos por ahora.

					}	// Si es la misma orden
				} // Ciclo de OMs en la máquina
			} // Si es del GFH
		} //Ciclo de las máquinas en el programa
	} // Ciclo de la lista de OMs a borrar

	
	// Tras borrar las ordenes, las insertamos en su GFH
	InsertaEnGFH(ArrOMs, p_lGFH);
	// Recalculamos todo
	Recalc();
}

// Reprogramamos órdenes en un GFH. Tomamos todas las órdenes seleccionadas
// del GFH seleccionado

void
CProg::ReprogramaGFH(long p_lGFH)
{
	
	int lim = GetNumPM();
	
	// Creamos una lista con las ordenes que vamos a reprogramar,
	// que son todas las seleccionadas del GFH
	
	CObArray		ArrOMs;
	
	for(int i = 0; i < lim; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		ASSERT_VALID(pPM);
		if (pPM->GetlGFH() == p_lGFH)
		{
			for(int j = 0; j < pPM->GetNumOM(); j++)
			{
				COrdMaq* pOM = pPM->GetpOM(j);
				// Si está seleccionada y no está limitada al GFH
				if (pOM->GetBSelected())	
				{
					// La añadimos,
										
					pOM->m_FecOrd = pOM->GetFecLim();
					pOM->SetBFija(FALSE);	// Si la reprogramamos, ya no es fija.
					AddOMOrd( ArrOMs, pOM );
					pPM->RemoveAt(j);
					j--;		// Restamos 1 para no saltarnos 1
				} // Si está seleccionada 
			} // Ciclo de OMs
		}	// Si es del GFH correcto
		pPM->Recalc();	// Recalculamos para que se reduzcan los huecos
	} // Ciclo de PMs

	InsertaEnGFH(ArrOMs, p_lGFH);
	Recalc();
}

void
CProg::InsertaEnGFH(CObArray& p_ArrOMs, long p_lGFH)
{
	CObArray ArrPMs;
	COrdMaq* pOMAct;
	// Hacemos un ciclo para cada orden del Array
	for (int i = 0; i < p_ArrOMs.GetSize(); i++)
	{
		pOMAct = (COrdMaq*) p_ArrOMs.GetAt(i);
		// Hacemos una lista de en qué orden está el primer hueco en cada máquina
		// de ese GFH. Devuelve el número de máquinas válidas. Si es 0 damos un 
		// mensaje de error.
		if (BuscarHuecosGFH(p_lGFH, pOMAct, ArrPMs) == 0)
		{
			CString sMsg;
			sMsg.Format( STRID (IDS_WAR_NOINSOM) ,
				pOMAct->GetlOF(), pOMAct->GetsID() ); 
			AfxMessageBox(sMsg);
			continue;
		}
		if (p_lGFH == TK_GFH_INYECCION)
		{
			CProgMaq* pPMIns = (CProgMaq*) ArrPMs.GetAt(0);
			pPMIns->Add(pOMAct);
			pOMAct->Recalc();
			BuscaOptimizar(p_ArrOMs, i);
		} else	if (p_lGFH == TK_GFH_ACABADO)	// Si estamos en acabado, es que hay que insertarlo en todos los
												// que tengan cadencia, i.e. los que no esten limitados, i.e. todos
												// los de la lista de BuscarHuecos
		{
			for (int j=0; j < ArrPMs.GetSize(); j++)
			{
				CProgMaq* pPMIns = (CProgMaq*) ArrPMs.GetAt(j);
				COrdMaq* pOMNew = pOMAct;
				if (j > 0)
				{
					pOMNew = new COrdMaq();
					*pOMNew = *pOMAct;
				}

				pPMIns->Add(pOMNew);
				pOMNew->Recalc();
				BuscaOptimizar(p_ArrOMs, i);
			}
		}
	}
}

void
CProg::BuscaOptimizar( CObArray& p_ArrOMs, int p_iInd )
{
/*
	COrdMaq* pOMBase = (COrdMaq*) p_ArrOMs.GetAt(p_iInd);
	ASSERT_VALID(pOMBase);

	CString sGFH = pOMBase->GetsGFH();
	CProgMaq* pPMIns = pOMBase->GetpPM();
	CTime FecBase = pOMBase->GetFecIni();
	CTimeSpan tsDespl = pOMBase->GetTimeSpan();
	// Solo optimizaremos llenadoras por el momento
	if (sGFH != TK_GFH_LLENADORAS) return;
	

	// La optimizacion en llenadoras consiste en buscar ordenes
	// con la misma masa y longitud dentro de un rango razonable (1 dia)
	// y agruparlas.
	
	for (int j = p_iInd + 1; j < p_ArrOMs.GetSize(); j++)
	{
		COrdMaq* pOM = (COrdMaq*) p_ArrOMs.GetAt(j);
		ASSERT_VALID(pOM);
		
				
		// Si la diferencia entre los dos comienzos es de mas
		// de un dia, lo dejamos

		CTimeSpan tsDiff = pOM->m_FecOrd - pOMBase->m_FecOrd;
		if (tsDiff.GetTotalSeconds() > TK_SEGS_DIA) break;
		if (tsDespl.GetTotalSeconds() > TK_SEGS_DIA) break;

				
		// Si no, están lo suficientemente proximas para agruparlas
		// si usan la misma masa y longitud.
		// Y siempre que la segunda esté seleccionada
		// Ver2. Realizar una segunda fase de optimizacion basándose en el
		// resultado y tratando de mejorar la suma de márgenes
		if ( pOM->GetiMasa() == pOMBase->GetiMasa() &&
			 pOM->GetlLongitud() == pOMBase->GetlLongitud())
		{
			p_ArrOMs.RemoveAt(j);
			j--;
			pPMIns->Add(pOM);
			pOM->Recalc();
			tsDespl += pOM->GetTimeSpan();

		}
	}
*/
}

int
CProg::BuscarHuecosGFH(long p_lGFH, COrdMaq* p_pOM, CObArray& p_ArrPMs) const 
{
	
	p_ArrPMs.RemoveAll();
	for (int ind = 0; ind < GetNumPM(); ind++)
	{
		CProgMaq* pActPM = GetpPM(ind);
		ASSERT_VALID(pActPM);
		// Si es del GFH correcto y la orden no está limitada a la máquina
		if (pActPM->GetlGFH() == p_lGFH && !(p_pOM->LimitMaquina(pActPM->GetsID())))
		{
			CTime FecUlt;
			// Si la máquina tiene ordenes, es la fecha de fin de la última orden
			if (pActPM->GetNumOM() > 0)
			{
				COrdMaq* pOM = pActPM->GetpOM(pActPM->GetNumOM() - 1);
				ASSERT_VALID(pOM);
				FecUlt = pOM->GetFecFin();
			} else	// si no, es la fecha de inicio del programa
				FecUlt = GetFecIni();
			
			pActPM->m_FecOrd = FecUlt;
			AddPMOrd(p_ArrPMs, pActPM);
		} // Si es máquina válida
	} // Ciclo de máquinas
	return p_ArrPMs.GetSize();
}

void
CProg::AddPM( CProgMaq* p_pPM, int p_iInd)
{
	if (p_iInd == -1)
		m_pArrPMs->Add ((CObject*) p_pPM);
	else
		m_pArrPMs->InsertAt ( p_iInd, (CObject*) p_pPM);
	
	p_pPM->SetProg(this);
}

void
CProg::SetFecIni(CTime p_Fec)
{	
	CTime FecTemp = AddDays(p_Fec, TK_MIN_PROG);
	m_Abs.SetFecIni(p_Fec); 
	if (FecTemp > GetFecFin()) SetFecFin(FecTemp);
	
	Recalc();
	RecalcInacts();
}


void
CProg::DescuentaVal(CTime p_Fec, int p_iLocal)
{
//	logFile.Write("%s - %s", "Entrando", "DescuentaVal");
	UpdateArrTL(); // Calculamos las OMCs por si no estaban
	
	
    CObArray* pArrNoCambio = new CObArray();
	CObArray* pArrBufArt = g_pCache->GetArrBufArt();
	
	int iLim = pArrBufArt->GetSize();
	// Hacemos un ciclo por todos los Artículos
	for(int i = 0; i < iLim; i++)
	{
		CBufArticulo* pBufArt = (CBufArticulo*) pArrBufArt->GetAt(i);
//		logFile.Write("%s - %s", "Nuevo Articulo", pBufArt->m_sID);
		ASSERT( pBufArt->IsKindOf( RUNTIME_CLASS( CBufArticulo ) ) );
		// Si tienen una cantidad fabricada en el Listado Valorado
		long lCantR = pBufArt->m_LV_lCantidad_Fabricada;
//		if (lCantR > 0) logFile.Write("%s - %ld", "Cantidad Fabricada", lCantR);
	
		if (lCantR <= 0) continue;
		
		// La vamos descontando
		
//		logFile.Write("%s - %s", "Buscando TimeLine", "");
	
		// buscamos el TimeLine que le corresponde
		CTimeLine* pTL = GetpTL(pBufArt->m_sID);
		if (!pTL) continue;
		
//		logFile.Write("%s - %s", "Encontrado TimeLine", pBufArt->m_sID);
	
		// Lo añadimos a la cantidad de las OFs
		g_pCache->UpdateOF(pBufArt->m_sID, lCantR);

		// Hacemos un ciclo ahora por las órdenes
		int iLimI = pTL->m_pArrOM->GetSize();
		
		// Las órdenes deben estar clasificadas por fecha de inicio
		
		// Primero borramos todas las que no cumplen los requisitos de Local
		BOOL bAlgoBorrado = FALSE;
		for(int iI = 0; iI < iLimI; iI++)
		{
			COrdMaq* pOM = (COrdMaq*) pTL->m_pArrOM->GetAt(iI);
			ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
			CProgMaq* pPM = pOM->GetpPM();
			if (pPM->GetlGFH() != TK_GFH_INYECCION) continue;
	
			if (!(p_iLocal == 1 && pPM->GetsIDLocal() == "1" || p_iLocal != 1 && pPM->GetsIDLocal() != "1" ))
			{
				bAlgoBorrado = TRUE;
				pTL->m_pArrOM->RemoveAt(iI);
				iI--;
				iLimI--;	
			}
		}
		// Si nos hemos quedado sin ordenes, continuamos
		if (pTL->m_pArrOM->GetSize() <= 0) continue;
		// Sincronizamos el Array de combinadas si es necesario
		if (bAlgoBorrado) pTL->Recalc(Piezas);
		// Y ahora descontamos
//		logFile.Write("%s - %s", "Tenemos ordenes, vamos a DescuentoConSolapes", "");
	
		DescuentoConSolapes(lCantR, pTL, pArrNoCambio, p_Fec);
	}
	
	
	
	// Ahora aplicamos los cambios de moldes
	
	CObArray* pArrNoFab = new CObArray();
	CStringArray* pArrNotFound = new CStringArray();
	CObArray* pArrBorrar = new CObArray();
	AplicarCambiosMoldes(p_Fec, pArrNoCambio, pArrBorrar, pArrNoFab, pArrNotFound);
	
	
	InformarErrores(pArrNoCambio, pArrNoFab, pArrNotFound, pArrBorrar);
	
	//Ahora borramos las de ArrBorrar
	for(int i=0; i < pArrBorrar->GetSize(); i++)
	{
		COrdMaq* pOM = (COrdMaq*) pArrBorrar->GetAt(i);
		// La añadimos al histórico
		g_pCache->AddHistorico(pOM);
		if (pOM->GetBNextLink() && pOM->Next()) g_pCache->AddHistorico(pOM->Next()); 
		// Y la borramos
		DeleteOM(pOM);
	}
	//Ahora borramos las de ArrNoCambio
	for(int i=0; i < pArrNoCambio->GetSize(); i++)
	{
		COrdMaq* pOM = (COrdMaq*) pArrNoCambio->GetAt(i);
		if (pOM->GetlCantidad() > 0) continue;
		// La añadimos al histórico
		g_pCache->AddHistorico(pOM);
		if (pOM->GetBNextLink() && pOM->Next()) g_pCache->AddHistorico(pOM->Next()); 
		// Y la borramos
		DeleteOM(pOM);
	}
	// y borramos y cambiamos las órdenes sin artículo que se solapen con el día.
//	logFile.Write("%s - %s", "Entrando a cambiar las órdenes sin artículo", "");
	
	for(int i=0; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		if (pPM->GetlGFH() != TK_GFH_INYECCION) continue;
	
		ASSERT_VALID(pPM);
		if (p_iLocal == 1 && pPM->GetsIDLocal() == "1" || p_iLocal != 1 && pPM->GetsIDLocal() != "1" )
		{
			for (int j = 0; j < pPM->GetNumOM(); j++)
			{
				COrdMaq* pOM = pPM->GetpOM(j);
				if (pOM->GetsID() != "") continue;
				if (pOM->GetFecIni() < p_Fec) // Hay solape
				{
					if (pOM->GetFecFin() <= p_Fec) // Está entera dentro
					{
						// La borramos
						DeleteOM(pOM);
						
					} else // Sólo está en parte
					{
						
						pOM->SetTs(pOM->GetFecFin() - p_Fec);
						pOM->SetBTiempoFijo(TRUE);
						pOM->SetFecIni(p_Fec);		// Recortamos lo que sobra
					}
				}
				
				
			}
		}
	}
	
	// Y cambiamos el orden en las que se ha cambiado el molde pero no se han completado,
	// buscamos el molde que venia detras y lo ponemos el primero.
	for(int i=0; i < pArrNoFab->GetSize(); i++)
	{
		COrdMaq* pOM = (COrdMaq*) pArrNoFab->GetAt(i);
		CProgMaq* pPM = pOM->GetpPM();
		if (pPM->GetlGFH() != TK_GFH_INYECCION) continue;
		if (pOM->m_sAux == "") continue;
		COrdMaq* pPrimOM = pPM->FindOMbyMolde( pOM->m_sAux );
		if (pPrimOM)
		{
			MoveOrdMaq(pPM, pPrimOM, 0);
		}
	}
	
	
	
	delete pArrNoCambio;
	delete pArrNoFab;
	delete pArrNotFound;
	delete pArrBorrar;
	
	// Ahora ponemos las máquinas afectadas un día más avanzado
	
	for(int i=0; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		ASSERT_VALID(pPM);
		if (p_iLocal == 1 && pPM->GetsIDLocal() == "1" || p_iLocal != 1 && pPM->GetsIDLocal() != "1" )
		{
			if (pPM->GetNumOM())
			{
				COrdMaq* pOM = pPM->GetpOM(0);
				ASSERT_VALID(pOM);
				pOM->SetFecIni(p_Fec);
				pOM->SetBFija(TRUE);
				// Actualizamos también la cadencia, para esta primera orden, por la del listado valorado.
				// (Se quita por petición de fabricación el 27/08/2012, dejándose en la cadencia normal)
				//CBufArticulo* pBufArt = pOM->GetpBufArticulo();
				//if (pBufArt->m_LV_dP_H_real > 0) pOM->SetdCadencia(pBufArt->m_LV_dP_H_real);
			}
		}
	}
	
	
	Recalc();
	RecalcInacts();
	UpdateArrTL();		// Porque ha quedado mal tras borrar las máquinas que no cumplen el p_iLocal
	
}



void
CProg::DescuentoConSolapes(long& p_rlCant, CTimeLine* p_pTL, CObArray* p_pArrNoCambio, CTime p_Fec)
{
//	logFile.Write("%s - %s", "Entrando a DescuentoConSolapes", "");
	
	// Repartimos la cantidad entre las OMs
	(void) p_pTL->ReparteCant(p_rlCant, Piezas);
	// Ahora tenemos la cantidad en el m_lAux de cada OM
	// Hacemos un ciclo por las OMs afectadas a ver qué hay
	
	for (int i=0; i < p_pTL->m_pArrOM->GetSize(); i++)
	{
		COrdMaq* pOM = (COrdMaq*) p_pTL->m_pArrOM->GetAt(i);
//		logFile.Write("%s - %ld", "Procesando orden: ", pOM->GetlOF());
		long lDescCant = pOM->m_lAux;
		if (lDescCant <= 0) continue;
//		logFile.Write("%s - %ld", "Encontrada cantidad", lDescCant);
	
		pOM->SetFab(pOM->GetlCantFab() + (lDescCant > pOM->GetlCantidad() ? pOM->GetlCantidad() : lDescCant)
			, p_Fec);

		//pOM->SetlCantidad(pOM->GetlCantidad() - lDescCant);
		pOM->ModifCantChatarras(pOM->GetlCantTot() - lDescCant);
		if (pOM->GetlCantidad() < 0) pOM->SetlCantidad(0);
		pOM->SetBTiempoFijo(FALSE);
		pOM->CalcTimeSpan();
		// Si ya hemos quedado sin cantidad
		if (pOM->GetlCantidad() <= 0)
		{
			p_pArrNoCambio->Add(pOM);
		}
	}
	

}

void 
CProg::InformarErrores(CObArray *p_pArrNoCambio, CObArray *p_pArrNoFab, CStringArray *p_pArrNotFound, CObArray *p_pArrBorrar)
{
	int iNumOM;
	CDlgDebug DlgD;
	DlgD.m_ArrStr.Add("Bloques Fabricados pero sin Cambio de Molde");
	DlgD.m_ArrStr.Add("-------------------------------------------");
	
	iNumOM = p_pArrNoCambio->GetSize();
	for (int iD=0; iD< iNumOM; iD++)
	{
		COrdMaq* pOM = (COrdMaq*) p_pArrNoCambio->GetAt(iD);
		ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
		
		CString sMsg;
		sMsg.Format(" [%s] ; %s (%s)"
			, pOM->GetsMaquina()
			, pOM->GetsID()
			, pOM->GetsMolde()
			);
		
		DlgD.m_ArrStr.Add(sMsg);
	}
	
	
	iNumOM = p_pArrNotFound->GetSize();
	if (iNumOM > 0)
	{
		DlgD.m_ArrStr.Add("");
		DlgD.m_ArrStr.Add("CAMBIOS DE MOLDE NO ENCONTRADOS");
		DlgD.m_ArrStr.Add("-------------------------------");
		
		for (int iD=0; iD< iNumOM; iD++)
		{
			CString sMsg = p_pArrNotFound->GetAt(iD);
			
			DlgD.m_ArrStr.Add(sMsg);
		}
	}

	DlgD.m_ArrStr.Add("");
	DlgD.m_ArrStr.Add("Bloques con Cambio de Molde pero con + de 5 horas restantes");
	DlgD.m_ArrStr.Add("-----------------------------------------------------------");
	
	iNumOM = p_pArrNoFab->GetSize();
	for (int iD=0; iD< iNumOM; iD++)
	{
		COrdMaq* pOM = (COrdMaq*) p_pArrNoFab->GetAt(iD);
		ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
		
		CString sMsg;
		sMsg.Format(" [%s] ; %s (%s)"
			, pOM->GetsMaquina()
			, pOM->GetsID()
			, pOM->GetsMolde()
			);
		
		DlgD.m_ArrStr.Add(sMsg);
	}
	
	DlgD.m_ArrStr.Add("");
	DlgD.m_ArrStr.Add("Bloques que se borran");
	DlgD.m_ArrStr.Add("---------------------");
	
	iNumOM = p_pArrBorrar->GetSize();
	for (int iD=0; iD< iNumOM; iD++)
	{
		COrdMaq* pOM = (COrdMaq*) p_pArrBorrar->GetAt(iD);
		ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
		
		CString sMsg;
		sMsg.Format(" [%s] ; %s (%s)"
			, pOM->GetsMaquina()
			, pOM->GetsID()
			, pOM->GetsMolde()
			);
		
		DlgD.m_ArrStr.Add(sMsg);
	}
	
	//Volcamos la matriz en un fichero, para otras consultas.
	ofstream outf( TK_FILE_CAMBIO, ios::out );  // Output file stream.
	if (outf)
	{
		for(int ind = 0; ind < DlgD.m_ArrStr.GetSize() ; ind++)
		{
			CString sLin = DlgD.m_ArrStr.GetAt(ind);
			outf.write(sLin, sLin.GetLength() );
			outf.put('\n');
		}
		outf.close();
	}
	DlgD.m_ArrStr.Add("");
	CString sInt;
	sInt.Format("(%s %s)","Esta informacion se encuentra en el fichero : ", TK_FILE_CAMBIO);
	DlgD.m_ArrStr.Add(sInt);


	DlgD.DoModal();	
	
}

void
CProg::AplicarCambiosMoldes(CTime p_Fec, CObArray* p_pArrNoCambio, CObArray* p_pArrBorrar, CObArray* p_pArrNoFab, CStringArray* p_pArrNotFound )
{
	// Hacemos un ciclo por los cambios de los moldes
	CBufCambiosMoldes* pBufCM = g_pCache->GetFirstCambioMolde(p_Fec);
	while (pBufCM)
	{
		CProgMaq* pPM = FindMaq(pBufCM->m_sIDMaquina);
		if (!pPM) {
			p_pArrNotFound->Add(pBufCM->m_sIDMoldeSal + " en Maquina [" + pBufCM->m_sIDMaquina + "]");
		} else if (pPM->GetlGFH() == TK_GFH_INYECCION) {
	
			// Buscamos el primer molde en la maquina, teniendo en cuenta las ya encontradas,
			// que o bien estan en ArrBorrar o en ArrNoFab
			COrdMaq* pOM = pPM->FindOMbyMolde(pBufCM->m_sIDMoldeSal, p_pArrBorrar, p_pArrNoFab);
			if (pOM)
			{
				// Y la añadimos al array de OMs a borrar (antes era sólo si les quedaban menos de 5 horas)
				//if (pOM->GetTimeSpan().GetTotalSeconds() <= TK_DEL_MINSECS )
				//{
					p_pArrBorrar->Add(pOM);
					// Y lo quitamos del array de sin Cambios, si esta
					for (int iJ = 0; iJ < p_pArrNoCambio->GetSize(); iJ++)
					{
						COrdMaq* pOMJ = (COrdMaq*) p_pArrNoCambio->GetAt(iJ);
						if (pOM == pOMJ)
						{
							p_pArrNoCambio->RemoveAt(iJ);
							break;
						}
					}
				//} else
				//{
					// Guardamos el molde entrante para poderlo poner por delante luego
				//	pOM->m_sAux = pBufCM->m_sIDMoldeEnt;
				//	p_pArrNoFab->Add(pOM);
				//}	
			}
		}
		pBufCM = g_pCache->GetNextCambioMolde(p_Fec);
	}

}

void
CProg::DeleteOM(COrdMaq* p_pOM)
{
	COrdMaq* pOM = p_pOM;
	ASSERT_VALID(pOM); 
	CProgMaq* pPMi = pOM->GetpPM();
	ASSERT_VALID(pPMi);
	int i = pOM->PMIndex();
	if ( i < 0 ) TRACE("Error en CProg::DeleteOM, no se encuentra la orden en la máquina");

	COrdMaq* pOMNext = ( i < (pPMi->GetNumOM() - 1) ) ? pPMi->GetpOM(i+1) : NULL;
	COrdMaq* pOMPrev = ( i > 0) ? pPMi->GetpOM(i-1) : NULL;
	
	if (pOM->GetBNextLink()) 
	{
		pPMi->RemoveAt(i);
		if (pOMNext && pOMNext->GetBPrevLink()) 
		{
			pPMi->RemoveAt(i);
			delete pOMNext;
		}
	} else if (pOM->GetBPrevLink()) 
	{
		pPMi->RemoveAt(i);
		if (pOMPrev && pOMPrev->GetBNextLink())
		{
			pPMi->RemoveAt(i-1);
			delete pOMPrev;
		}
	} else
	{
		pPMi->RemoveAt(i);
	}
	
	delete pOM;
	
}

BOOL
CProg::VerifStruct()
{	
	BOOL BErr = FALSE;
	BOOL BRes;
	CString sMsg;
	
	// Recorremos todas las ordenes en celulas (a partir de TK_NUMDIAS_VERIF
	// días tras el inicio de la programación) y miramos que estén
	// completas, es decir que estén en todos los GFH´s para los
	// que no estén limitadas
	for( int i = GetNumPM() - 1; i >= 0 ; i--)
	{
		
		CProgMaq* pPM = (CProgMaq*) GetpPM(i);
		ASSERT_VALID(pPM);
		// Si nos salimos del primer GFH, nos vamos.
		if (pPM->GetlGFH() != TK_GFH_ACABADO) break;
		
		long lUltGFH;
		int iLimOM = pPM->GetNumOM();
		int iPM;
		for(int j = 0; j < iLimOM; j++)
		{
			COrdMaq* pOM = pPM->GetpOM(j);
			BRes = TRUE;
			lUltGFH = TK_GFH_ACABADO;
			iPM = i - 1;
			CTimeSpan tsDiff = pOM->GetFecIni() - GetFecIni();
			// Tenemos una orden, si está a menos de siete días del comienzo
			// de la programación, es que a lo mejor no está completa porque
			// empezaba antes del comienzo, y por tanto no se puede comprobar.
			if (tsDiff.GetDays() < TK_NUMDIAS_VERIF) continue;
			// Hacemos un ciclo por los GFH´s anteriores comprobando que se encuentra en todos
			for (;;)
			{
				// Ahora buscamos el siguiente GFH hacia atrás, comenzando por una máquina
				// anterior a la que vamos comprobando del primer GFH
				for (; iPM >= 0; iPM--)
				{
					CProgMaq* pPM = (CProgMaq*) GetpPM(iPM);
					ASSERT_VALID(pPM);
					if (pPM->GetlGFH() != lUltGFH) 
					{
						lUltGFH = pPM->GetlGFH();
						break;
					}
				}
				// Si no hay más GFH, nos salimos del ciclo de buscar GFHs
				if ( iPM < 0 ) break;
				// Si está limitada a ese GFH, vamos a por otro GFH
				if (pOM->LimitGFH(lUltGFH)) continue;
				// Buscamos dentro del GFH hasta encontrarla, iPM marca la primera maquina
				// de ese GFH. Marcamos el resultado como FALSE hasta encontrarla
				BRes = FALSE;
				for (iPM; iPM >= 0; iPM--)
				{
					CProgMaq* pPMCic = GetpPM(iPM);
					if (pPMCic->GetlGFH() != lUltGFH) break;
					
					int iLimOM = pPMCic->GetNumOM();
					for(int l = 0; l < iLimOM; l++)
					{
						COrdMaq* pOMCic = pPMCic->GetpOM(l);
						// Si la encontramos, marcamos el resultado como  TRUE y nos salimos
						if (pOMCic->GetlOF() == pOM->GetlOF())
						{
							BRes = TRUE;
							break;
						}
					} // Fin ciclo interno órdenes
				}	// Fin ciclo interno máquinas
				// Si BRes es FALSE, es que no esta limitada al GFH y no se encuentra en el.
				// Entonces damos un aviso.
				if (!BRes)
				{
					// Marcamos que ha habido errores, para el mensaje final
					BErr = TRUE;
					CTime FecEnt = pOM->GetFecEnt();
					sMsg.Format( STRID( IDS_ERR_OFINC ), 
						pOM->GetlOF(),
						FormatFec(FecEnt,Fecha));
					AfxMessageBox(sMsg);
				}
			} // Fin ciclo de buscar GFHs anteriores
		} // Fin ciclo externo órdenes
	} // Fin ciclo externo máquinas
	if (!BErr)
	{
		CTime FecEnt = GetFecIni();
		FecEnt = AddDays(FecEnt, TK_NUMDIAS_VERIF);
		sMsg.Format(STRID( IDS_ERR_NOOMINC ),
			FormatFec(FecEnt,Fecha));
		AfxMessageBox(sMsg);
	}
	return BErr;
}

COrdMaq* 
CProg::FindOMArt(const CString &p_sArticulo)
{
	for(int i=0; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		ASSERT_VALID(pPM);
		for (int j=0; j< pPM->GetNumOM() ; j++)
		{
			COrdMaq* pOM = pPM->GetpOM(j);
			ASSERT_VALID(pOM);
			if (pOM->GetsID() == p_sArticulo)
			{
				return pOM;
				break;
			}
		}
	}
	return NULL;
	
}

long 
CProg::GetlCantArt(const CString &m_sIDArt)
{
	// Hacemos un array para saber las OM´s que ya hemos metido
	CObArray ArrOMs;
	
	CTimeLine* pTL = GetpTL(m_sIDArt, Piezas);
	if (!pTL) return 0;
	
	pTL->GetArrOMs(&ArrOMs);	
				
	int iLim = ArrOMs.GetSize();
	long lCant = 0;
	for (int i = 0; i < iLim; i++)
	{
		COrdMaq* pOM = (COrdMaq*) ArrOMs.GetAt(i);
		ASSERT_VALID(pOM);
		lCant += pOM->GetlCantidad();
	}
	
	return lCant;
}



CTime
CProg::CalcFecParaCantTL(CTimeLine* p_pTL, long p_lCant)
{
	 if (p_lCant <= 0) return GetFecIni();

	 if (!p_pTL) return GetFecFin();

	 int iLimComb = p_pTL->m_pArrLS->GetSize();
	 if (iLimComb <= 0) return GetFecFin();
	 // Establecemos un acumulado de lo que vamos fabricando en cada Combinada
	 long lCant = 0;
	 CLinSeg LSFin;
	 // Las fechas de las combinadas deben ser consecutivas.
	 for (int i=0; i< iLimComb; i++)
	 {
		 CLinSeg* pLS = (CLinSeg*) p_pTL->m_pArrLS->GetAt(i);
		 // Despreciamos los Combinados menores de un minuto.
		 if (pLS->m_AbSpan.GetSecs() < 60 ) continue;
		 // Si ya hemos alcanzado la cantidad, salimos
		// Sumamos toda la fabricación
		 lCant += pLS->GetlCantFab();
		 if (lCant >= p_lCant)
		 {
			 LSFin = *pLS;
			 break;
		 }
	 }
	 if (lCant >= p_lCant) // Eso es que lo llegamos a fabricar
	 {
		 long lDiff = lCant - p_lCant;
		 CTime FecFin = LSFin.m_AbSpan.GetFecFin();
		 long lDiffSecs = (long) (( lDiff * (double) TK_SEGS_HORA ) / LSFin.m_dCadencia );
		 lDiffSecs = (long) ((double) lDiffSecs * ( 1.0 + ( TK_INACT_DEF / 100.0 )));
		 FecFin -= (CTimeSpan) lDiffSecs;
		 return FecFin;
	 }
	 return GetFecFin();
}


// Calcula un Array de TimeLines con una entrada para cada artículo,
int 
CProg::CalcArrTL(enum  eTimeLine p_eTL)
{
	CTimeLine TLt;
	CObArrayOrd* pArrTL;
	if (p_eTL == Mezcla)
	{
		pArrTL = m_pArrTLMezcla;
	}
	else if (p_eTL == Margen)
	{
		pArrTL = m_pArrTLMargen;
	}
	else
	{
		pArrTL = m_pArrTLPiezas;
	}
	// Hacemos un ciclo por todas las máquinas
	for(int i=0; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		if (pPM->GetlGFH() != TK_GFH_INYECCION) continue;
		ASSERT_VALID(pPM);
		for (int j=0; j< pPM->GetNumOM() ; j++)
		{
			COrdMaq* pOM = pPM->GetpOM(j);
			ASSERT_VALID(pOM);
			pOM->SetbConfMolde(FALSE);
			
			// No usamos la función GetpTL por velocidad
			CString sID = (p_eTL == Mezcla) ? pOM->GetsMezcla() : pOM->GetsID();
			TLt.m_sID = sID;
			int iInd = pArrTL->Find(&TLt);
			CTimeLine* pTL = NULL;
			if ( iInd < 0)
			{
				// Si no encontrado, hacemos uno nuevo
				pTL = new CTimeLine;
				pTL->m_sID = sID;

			// Y lo añadimos al Array
				pArrTL->Add(pTL);
			}
			else
			{
				pTL = (CTimeLine*) pArrTL->GetAt(iInd);
			}
			// Ya tenemos pTL apuntando a una TimeLine correcta.
			// Añadimos la OM al array de OMs
			pTL->m_pArrOM->Add(pOM);
		}
	}
	
	// Ahora ya tenemos el array inicial por orden de fecha de inicio, vamos
	// a calcular la matriz de los LinSeg. Lo hacemos en una función 
	
	int iLimTL = pArrTL->GetSize();
	for (int i=0; i < iLimTL; i++)
	{
		CTimeLine* pTL = (CTimeLine*) pArrTL->GetAt(i);
		// Y recalculamos la TL
		pTL->Recalc(p_eTL);
//	}
		
		// Debug
		/*
		int iNumPS;
		CDlgDebug DlgD;
		
		iNumPS = pTL->m_pArrPS->GetSize();
		for (int iD=0; iD< iNumPS; iD++)
		{
			CProdSeg* pPS = (CProdSeg*) pTL->m_pArrPS->GetAt(iD);
			ASSERT( pPS->IsKindOf( RUNTIME_CLASS( CProdSeg ) ) );
			COrdMaq* pOM = pPS->m_pOM;
			ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
			
			CString sMsg;
			sMsg.Format(" %s ; %s - %s (%s - %s)", pOM->GetsMaquina()
				, FormatFec(pPS->m_AbSpan.GetFecIni(), FecHoraSec)
				, FormatFec(pPS->m_AbSpan.GetFecFin(), FecHoraSec)
				, FormatFec(pOM->GetFecIni(), FecHoraSec)
				, FormatFec(pOM->GetFecFin(), FecHoraSec)
				);
			
			DlgD.m_ArrStr.Add(sMsg);
			
		}
		  DlgD.m_ArrStr.Add("-----Combinadas-----");
		  int iNumLS = pTL->m_pArrLS->GetSize();
		  for ( iD=0; iD< iNumLS; iD++)
		  {
			  CObArray ArrPSAfec;
			  CLinSeg* pLS = (CLinSeg*) pTL->m_pArrLS->GetAt(iD);
			  pTL->GetArrPSsAfec(pLS, &ArrPSAfec);
			  CString sMsg;
			  sMsg.Format(" %s ; %s - %s Afectadas: ", FormatLong((long) pLS->m_dCadencia, 6)
				  , FormatFec(pLS->m_AbSpan.GetFecIni(), FecHoraSec)
				  , FormatFec(pLS->m_AbSpan.GetFecFin(), FecHoraSec)
				  );
			  for (int iAdd = 0; iAdd < ArrPSAfec.GetSize(); iAdd++)
			  {
				  CProdSeg* pPS = (CProdSeg*) ArrPSAfec.GetAt(iAdd);
				  ASSERT( pPS->IsKindOf( RUNTIME_CLASS( CProdSeg ) ) );
				  COrdMaq* pOM = pPS->m_pOM;
				  ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
				  
				  sMsg += pOM->GetsMaquina() + " ";
			  }
			  
			  DlgD.m_ArrStr.Add(sMsg);
		  }
		  
		  if (DlgD.DoModal() != IDOK) return 0;
		  */
		  // End Debug
	}
	return pArrTL->GetSize();
}

void 
CProg::CalcConflictosMoldes()
{
	for(int i=0; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);	
		if (pPM->GetlGFH() != TK_GFH_INYECCION) continue;
		for (int j=0; j< pPM->GetNumOM() ; j++)
		{
			COrdMaq* pOM = pPM->GetpOM(j);
			CheckConfOM(pOM, i+1);
		}
	}
}

void
CProg::CalcConflictosMoldesElem()
{
	for (int i = 0; i < GetNumPM(); i++)
	{
		CProgMaq* pPM = GetpPM(i);
		if (pPM->GetlGFH() != TK_GFH_INYECCION) continue;
		for (int j = 0; j < pPM->GetNumOM(); j++)
		{
			COrdMaq* pOM = pPM->GetpOM(j);
			CheckConfOMElem(pOM, i + 1);
		}
	}
}

void 
CProg::CheckConfOM(COrdMaq* p_pOM, int p_iPM)
{			
	CTime FecIni = p_pOM->GetFecIni();
	CTime FecFin = p_pOM->GetFecFin();

	CProgMaq* pPMPropia = p_pOM->GetpPM();

	for(int i=p_iPM; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		if (pPMPropia == pPM) continue;
		if (pPM->GetlGFH() != TK_GFH_INYECCION) continue;
	
		for (int j=0; j< pPM->GetNumOM() ; j++)
		{
			COrdMaq* pOM = pPM->GetpOM(j);
			if (FecIni >= pOM->GetFecFin()) continue;
			if (FecFin <= pOM->GetFecIni()) break;
			if (pOM->GetsMolde() == p_pOM->GetsMolde() 
				&& pOM->GetsID() != ""	)	// Si es una parada, el molde no importa
			{
				pOM->SetbConfMolde(TRUE, p_pOM);
				p_pOM->SetbConfMolde(TRUE, pOM);
				// Si ya tenemos un conflicto, lo dejamos
				return;
			}
		}
	}
}

void
CProg::CheckConfOMElem(COrdMaq* p_pOM, int p_iPM)
{

	CTime FecIni = p_pOM->GetFecIni();
	CTime FecFin = p_pOM->GetFecFin();
	CProgMaq* pPMPropia = p_pOM->GetpPM();
	CObArray* pArrOrig = p_pOM->GetpArrMolElem();
	CObArray arrOMsAfectadas;
	int numElem = pArrOrig->GetSize();
	if (numElem == 0) return; // Si no tiene elementos, no puede tener conflictos
	CObArray arrCopia;
	//Hacemos una copia de los elementos
	for (int ii = 0; ii < numElem; ii++) {
		CBufMoldesElem* pCopiaBuf = new CBufMoldesElem(*((CBufMoldesElem*)pArrOrig->GetAt(ii)));
		arrCopia.Add(pCopiaBuf);
	}
	// Puede ser que la cantidad de algún elemento sea 0, así que ella misma estaría en conflicto
	if (CheckArrElem(arrCopia)) {
		arrOMsAfectadas.Add(p_pOM);
		p_pOM->SetbConfMoldeElem(TRUE, arrOMsAfectadas);
		return;	// Y no hay que buscar más
	}
	//Recorremos las máquinas
	for (int ii = p_iPM; ii < GetNumPM(); ii++)
	{
		CProgMaq* pPM = GetpPM(ii);
		if (pPMPropia == pPM) continue;
		if (pPM->GetlGFH() != TK_GFH_INYECCION) continue;
		//Y las OM'e en cada máquina
		for (int jj = 0; jj < pPM->GetNumOM(); jj++)
		{
			COrdMaq* pOM = pPM->GetpOM(jj);
			if (FecIni >= pOM->GetFecFin()) continue;
			if (FecFin <= pOM->GetFecIni()) break;
			//Si es una parada, el molde no importa
			if (pOM->GetsID() == "") continue;
			//Se solapan, si no tiene elementos de inyectora, seguimos
			CObArray* pArrSolape = pOM->GetpArrMolElem();
			if (!pArrSolape || pArrSolape->GetSize() == 0) continue;
			//Comparamos los elementos de inyectora entre los dos arrays
			// Si alguno coincide, le restamos 1 a su cantidad
			// Necesitamos el índice de máquina para no descontar dos veces un elemento
			// en la misma máquina
			if (CheckConfInArraysElem(arrCopia, *pArrSolape, ii)) {
				if (FindInObArray(arrOMsAfectadas, pOM) < 0) {
					arrOMsAfectadas.Add(pOM);
				}
			}
			if (CheckArrElem(arrCopia))
			{
				//Marcamos la OM base como conflicto
				p_pOM->SetbConfMoldeElem(TRUE, arrOMsAfectadas);
				//Repasamos el array de OMs afectadas, marcando todas las que estén afectadas
				// y borrando todas las que no
				for (int ii = arrOMsAfectadas.GetSize() - 1; ii >= 0; ii--) {
					COrdMaq* pOMaf = (COrdMaq*)arrOMsAfectadas.GetAt(ii);
					if (pOMaf->GetpBufMolde())
						pOMaf->SetbConfMoldeElem(TRUE, arrOMsAfectadas);
					else arrOMsAfectadas.RemoveAt(ii);
				}
				//Eliminamos los elementos creados
				ClearArray(arrCopia, RUNTIME_CLASS(CBufMoldesElem));
				return;
			}
		}
	}
	
	//Eliminamos los elementos creados
	ClearArray(arrCopia, RUNTIME_CLASS(CBufMoldesElem));
}

bool 
CProg::CheckConfInArraysElem(CObArray& p_arrOrig, CObArray& p_arrCheck, int indMaq)
{
	bool result = false;
	for (int ii = 0; ii < p_arrOrig.GetSize(); ii++) {
		CBufMoldesElem* p_mi = (CBufMoldesElem*)p_arrOrig.GetAt(ii);
		for (int jj = 0; jj < p_arrCheck.GetSize(); jj++) {
			CBufMoldesElem* p_mic = (CBufMoldesElem*)p_arrCheck.GetAt(jj);
			// Si es del mismo tipo
			if ( p_mi->m_sTipo == p_mic->m_sTipo && p_mi->m_lId == p_mic->m_lId  && indMaq != p_mi->m_iAux) {
				//Descontamos uno del array original, en ese id
				p_mi->m_dCantidadMax--;
				p_mi->m_iAux = indMaq; // Guardamos el índice de la máquina para no volver a descontar de esa 
										// máquina para esa OF
				result = true;
				//Ya no habrá más con la misma id
				break;
			}
		}
	}
	return result;
}

bool
CProg::CheckArrElem(CObArray& p_arrOrig)
{
	for (int ii = 0; ii < p_arrOrig.GetSize(); ii++) {
		CBufMoldesElem* p_mi = (CBufMoldesElem*)p_arrOrig.GetAt(ii);
		if (p_mi->m_dCantidadMax <= 0) return true;
	}
	return false;
}

void 
CProg::UpdateArrTL()
{
	ClearOMs();
	CalcArrTL(Piezas);
	CalcArrTL(Margen);
	CalcArrTL(Mezcla);
	
}


// Inserta una nueva orden, dando la opcion de modificar los datos o cancelar
BOOL 
CProg::InsertNewOM(COrdMaq* p_pOM, BOOL& p_rbModif)
{
	BOOL bCancelar;
	p_rbModif = FALSE;
	if (p_pOM == NULL)
	{          
		AfxMessageBox(STRID(IDS_ERR_NOORDSEL));
		return FALSE;
	}
	COrdenDlg OrdenDlg(p_pOM, this, AfxGetMainWnd());
	
	OrdenDlg.m_BReadOnly = FALSE;
	
	OrdenDlg.m_sCantidad = FormatLong(p_pOM->GetlCantidad(), 9);
	OrdenDlg.m_sCantTot = FormatLong(p_pOM->GetlCantTot(), 9);
	OrdenDlg.m_FecEnt = p_pOM->GetFecEnt();
	OrdenDlg.m_FecFin =	p_pOM->GetFecFin();
	OrdenDlg.m_FecIni = p_pOM->GetFecIni();
	
	OrdenDlg.m_lLote = p_pOM->GetlLote();
	OrdenDlg.m_sChatarras.Format("%.1lf", p_pOM->GetdChatarras());
	OrdenDlg.m_sTiempo = FormatFec( p_pOM->GetTimeSpan() );
	OrdenDlg.m_sTiempoTot = FormatFec( p_pOM->GetTotalTs() );

	OrdenDlg.m_BFecFija = p_pOM->GetBFija();
	BOOL bFecFijaIni = OrdenDlg.m_BFecFija;
	OrdenDlg.m_sCadencia.Format("%.0lf ", p_pOM->GetdCadenciaGFH());
	OrdenDlg.m_sPersonas.Format("%.0lf ", p_pOM->GetdPersonas());
	OrdenDlg.m_BTiempoFijo = p_pOM->GetBTiempoFijo();
	BOOL bTiempoFijoIni = OrdenDlg.m_BTiempoFijo;
	OrdenDlg.m_BSelected = p_pOM->GetBSelected();
	if (p_pOM->GetlCantFab())
	{
		OrdenDlg.m_sFabricadas = FormatLong(p_pOM->GetlCantFab(), 6);
		OrdenDlg.m_FecFab = p_pOM->GetFecFab();
		OrdenDlg.m_sRestantes = FormatLong(p_pOM->GetlCantidad() - p_pOM->GetlCantFab(), 6);
	} else {
		OrdenDlg.m_sFabricadas = "";
		OrdenDlg.m_FecFab = CTime::GetCurrentTime();
		OrdenDlg.m_sRestantes = "";
	}
	OrdenDlg.m_sInactividad.Format("%.1lf %%", p_pOM->GetdInactividad());
	OrdenDlg.m_sCadenciaAcabado = FormatDbl(g_pCache->GetdCadenciaAcabado(p_pOM->GetsID()), 7,1 );

	
	CString sTemp = p_pOM->GetsNombre() + ' '
		+ " (" + p_pOM->GetsID() + ')';
	if (p_pOM->GetpPM() != NULL)
	{
		sTemp += " en " + p_pOM->GetpPM()->GetsID();
	}
	OrdenDlg.m_sCaption = sTemp;
	
	int iDlgStat = OrdenDlg.DoModal();
	CCpwDoc* pDoc = GetpDoc();
	pDoc->UpdateAllViews(NULL);

	bCancelar = OrdenDlg.m_bCancelar;
	p_rbModif = OrdenDlg.m_bInsertada;
	if (!OrdenDlg.m_bInsertada) 
	{
		delete p_pOM;
		return bCancelar;
	}
		
	if (iDlgStat != IDOK) return bCancelar;
	
	if (bFecFijaIni != OrdenDlg.m_BFecFija)
	{
		CWaitCursor WaitCursor;
		p_pOM->SetBFija(OrdenDlg.m_BFecFija);
		Recalc();
	}
	if (bTiempoFijoIni != OrdenDlg.m_BTiempoFijo)
	{
		CWaitCursor WaitCursor;
		p_pOM->SetBTiempoFijo(OrdenDlg.m_BTiempoFijo);
		Recalc();
	}

	return bCancelar;
}



void CProg::GetArrPM(CObArray &p_PMArr, long p_lGFH)
{
	p_PMArr.RemoveAll();
	for (int i=0; i< GetNumPM(); i++)
	{                               
		CProgMaq* pPM = GetpPM(i);
		// Si hemos pedido todos los GFHs o si es el que queremos...
		if (p_lGFH == -1 || (p_lGFH == pPM->GetlGFH() ))
		{
			//...la añadimos
			p_PMArr.Add(pPM);
		}
	}
}

int CProg::GetNumOM()
{
	int iNumOM = 0;
	for (int i=0; i< GetNumPM(); i++)
	{                               
		CProgMaq* pPM = GetpPM(i);
		iNumOM += pPM->GetNumOM();
	}
	return iNumOM;

}



/*
CTimeLine*
CProg::GetpTL(CBufMezcla* p_pBufMez)
{
//	return p_pBufMez->m_pTL;
}
*/

CTimeLine*
CProg::GetpTL(CBufArticulo* p_pBufArt, enum  eTimeLine p_eTL)
{
	if (!p_pBufArt) return NULL;
/*
	if (p_eTL == Mezcla)
	{
		if (p_pBufArt->m_pBufMezcla && p_pBufArt->m_pBufMezcla->m_pTL)
		{
			return p_pBufArt->m_pBufMezcla->m_pTL;
		}
		else
		{
			return GetpTL(p_pBufArt->m_sMezcla, p_eTL);
		}
	}
	else
	{
		if (p_pBufArt->m_pTL)
		{
			return (p_eTL == Piezas) ? p_pBufArt->m_pTL : p_pBufArt->m_pTLMargen;
		}
		else
		{
			return NULL;
		//	return GetpTL(p_pBufArt->m_sID, p_eTL);
		}
	}
*/
	if (p_eTL == Mezcla)
			return GetpTL(p_pBufArt->m_sMezcla, p_eTL);
	else		
			return GetpTL(p_pBufArt->m_sID, p_eTL);
}

CTimeLine*
CProg::GetpTL(const CString& p_sID, enum  eTimeLine p_eTL)
{

	if (p_sID == "") return NULL;
	CTimeLine BuscTL;
	BuscTL.m_sID = p_sID;
	// Cogemos una matriz u otra dependiendo del recurso del que tratemos
	CObArrayOrd* pArrTL;
	if (p_eTL == Mezcla)
	{
		pArrTL = m_pArrTLMezcla;
	}
	else if (p_eTL == Margen)
	{
		pArrTL = m_pArrTLMargen;
	}
	else
	{
		pArrTL = m_pArrTLPiezas;
	}
	
	int iInd = pArrTL->Find(&BuscTL);
	if (iInd < 0) return NULL;
	CTimeLine* pTL = (CTimeLine*) pArrTL->GetAt(iInd);
	if (!pTL) return NULL;
	ASSERT( pTL->IsKindOf( RUNTIME_CLASS( CTimeLine ) ) );
	return pTL;
}



// Aplicamos las distancias que pueda haber entre centros
void 
CProg::AplicarDistancias(CObArrayOrd* p_pArrPS)
{
	// Em esta matriz iremos guardando los PSs limpios, a los que ya no afectan inactividades
	CObArray ArrPStemp;
	// Empezamos buscando OMs que tengan una distancia, dentro del Array de PS
	for (int i = 0; i < p_pArrPS->GetSize(); i++)
	{
		CProdSeg* pPS = (CProdSeg*) p_pArrPS->GetAt(i);
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
	p_pArrPS->RemoveAll();
	for (int i = 0; i < ArrPStemp.GetSize(); i++)
	{
		p_pArrPS->Add(ArrPStemp.GetAt(i));
	}
	
}

void 
CProg::AplicarCalendarioArrPSs(CObArrayOrd* p_pArrPS)
{

	// Em esta matriz iremos guardando los PSs limpios, a los que ya no afectan inactividades
	CObArray ArrPStemp;
	// Empezamos buscando inactividades que afecten a alguna de las órdenes. Para ello primero hacemos 
	// un ciclo por las OMs, para ver cuáles están afectadas por alguna inactividad.
	for (int i = 0; i < p_pArrPS->GetSize(); i++)
	{
		CProdSeg* pPS = (CProdSeg*) p_pArrPS->GetAt(i);
		ASSERT( pPS->IsKindOf( RUNTIME_CLASS( CProdSeg ) ) );
		COrdMaq* pOM = (COrdMaq*) pPS->m_pOM;
		ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
		CObArray pArrInact;
		// GetArrInacts nos da un array de Inactividades ordenadas por fecha de inicio
		if (!pOM->GetArrInacts(pArrInact)) 	// Si no tiene inactividades lo copiamos y seguimos
		{
			//CProdSeg* pPS = (CProdSeg*) p_pArrPS->GetAt(iM);
			//ASSERT( pPS->IsKindOf( RUNTIME_CLASS( CProdSeg ) ) );
			//p_pArrPSout->Add(pOMComb);
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
	p_pArrPS->RemoveAll();
	for (int i = 0; i < ArrPStemp.GetSize(); i++)
	{
		p_pArrPS->Add(ArrPStemp.GetAt(i));
	}
}
/*
void
CProg::DescuentoConSolapes(long& p_rlCant, COMC* p_pOMC, CObArray* p_pArrNoCambio, CTime p_Fec)
{
	// Repartimos la cantidad entre las OMs
	(void) p_pOMC->ReparteCant(p_rlCant);
	// Ahora tenemos la cantidad en el m_lAux de cada OM
	// Hacemos un ciclo por las OMs afectadas a ver qué hay
	//	if (!p_pOMC->m_pOM)	return;
	
	for (int i=0; i < p_pOMC->m_pArrOM->GetSize(); i++)
	{
		COrdMaq* pOM = (COrdMaq*) p_pOMC->m_pArrOM->GetAt(i);
		long lDescCant = pOM->m_lAux;
		if (lDescCant <= 0) continue;
		
		pOM->SetlCantidad(pOM->GetlCantidad() - lDescCant);
		pOM->SetFab(pOM->GetlCantFab() + (lDescCant > pOM->GetlCantidad() ? pOM->GetlCantidad() : lDescCant)
			, p_Fec);
		if (pOM->GetlCantidad() < 0) pOM->SetlCantidad(0);
		pOM->SetBTiempoFijo(FALSE);
		pOM->CalcTimeSpan();
		// Si ya hemos quedado sin cantidad
		if (pOM->GetlCantidad() <= 0)
		{
			p_pArrNoCambio->Add(pOM);
		}
	}
	

}
*/
/*
void
CProg::DescuentaVal(CTime p_Fec, int p_iLocal)
{
	UpdateArrTL(); // Calculamos las OMCs por si no estaban
	
	
    CObArray* pArrNoCambio = new CObArray();
	CObArray* pArrBufArt = g_pCache->GetArrBufArt();
	
	int iLim = pArrBufArt->GetSize();
	// Hacemos un ciclo por todos los Artículos
	for(int i = 0; i < iLim; i++)
	{
		CBufArticulo* pBufArt = (CBufArticulo*) pArrBufArt->GetAt(i);
		ASSERT( pBufArt->IsKindOf( RUNTIME_CLASS( CBufArticulo ) ) );
		// Si tienen una cantidad fabricada en el Lisado Valorado
		if (pBufArt->m_LV_lCantidad_Fabricada <= 0) continue;
		
		// La vamos descontando
		
		// Lo añadimos a la cantidad de las OFs
		g_pCache->UpdateOF(pBufArt->m_sID, pBufArt->m_LV_lCantidad_Fabricada);
		
		// buscamos la Orden combinada que le corresponde
		COMC* pOMC = GetpOMC(pBufArt->m_sID);
		if (!pOMC) continue;
		// Si sólo hay una orden y no tiene inactividades, descontamos y acabamos.
		if (pOMC->m_pOM && pOMC->m_pArrCOMComb->GetSize() < 2)
		{
			CProgMaq* pPM = pOMC->m_pOM->GetpPM();
			if (p_iLocal == 1 && pPM->GetsIDLocal() == "1" || p_iLocal != 1 && pPM->GetsIDLocal() != "1" )
			{
				pOMC->m_pOM->SetlCantidad(pOMC->m_pOM->GetlCantidad() - pBufArt->m_LV_lCantidad_Fabricada);
				// Como sólo hay una orden, ponemos toda la cantidad a esa orden
				pOMC->m_pOM->SetFab(pOMC->m_pOM->GetlCantFab() + pBufArt->m_LV_lCantidad_Fabricada, p_Fec);
				pOMC->m_pOM->SetBTiempoFijo(FALSE);
				pOMC->m_pOM->CalcTimeSpan();
				// Si ya hemos quedado sin cantidad
				if (pOMC->m_pOM->GetlCantidad() <= 0)
				{
					// Ponemos la orden en una hora para que sea eliminada por
					// AplicarCambiosMoldes
					pOMC->m_pOM->SetlCantidad(0);
					pOMC->m_pOM->SetTs((CTimeSpan) TK_SEGS_HORA);
					CString sTest = FormatLong(pOMC->m_pOM->GetTimeSpan().GetTotalSeconds(), 5);
					// Y la añadimos a la matriz para comprobar que todas son eliminadas
					pArrNoCambio->Add(pOMC->m_pOM);
				}
			}
			// Por ahora el posible remanente de fabricadas nos da igual
		} else if (pOMC->m_pArrOM) // Si tenemos varias órdenes 
		{
			// Hacemos un ciclo
			int iLimI = pOMC->m_pArrOM->GetSize();
			// Las órdenes deben estar clasificadas por fecha de inicio
			
			long lCantR = pBufArt->m_LV_lCantidad_Fabricada;
			// Primero borramos todas las que no cumplen los requisitos de Local
			BOOL bAlgoBorrado = FALSE;
			for(int iI = 0; iI < iLimI; iI++)
			{
				COrdMaq* pOM = (COrdMaq*) pOMC->m_pArrOM->GetAt(iI);
				ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
				CProgMaq* pPM = pOM->GetpPM();
				if (!(p_iLocal == 1 && pPM->GetsIDLocal() == "1" || p_iLocal != 1 && pPM->GetsIDLocal() != "1" ))
				{
					bAlgoBorrado = TRUE;
					pOMC->m_pArrOM->RemoveAt(iI);
					iI--;
					iLimI--;	
				}
			}
			// Si nos hemos quedado sin ordenes, continuamos
			if (pOMC->m_pArrOM->GetSize() <= 0) continue;
			// Sincronizamos el Array de combinadas si es necesario
			if (bAlgoBorrado) DesolaparArrOMC( pOMC->m_pArrOM, pOMC->m_pArrCOMComb);
			
			// Y ahora descontamos
			// Si hay solapes
				DescuentoConSolapes(lCantR, pOMC, pArrNoCambio, p_Fec);
			// Antes hacíamos diferencias entre descuento con solapes y sin solapes.
			// pero al haber calendario, no se puede hacer ésto, porque el calendario afecta
			// al descuento incluso aunque no haya solapes.
		}
	}
	
	
	// Ahora aplicamos los cambios de moldes
	
	CObArray* pArrNoFab = new CObArray();
	CStringArray* pArrNotFound = new CStringArray();
	CObArray* pArrBorrar = new CObArray();
	AplicarCambiosMoldes(p_Fec, pArrNoCambio, pArrBorrar, pArrNoFab, pArrNotFound);
	
	
	InformarErrores(pArrNoCambio, pArrNoFab, pArrNotFound, pArrBorrar);
	
	//Ahora borramos las de ArrBorrar
	for(i=0; i < pArrBorrar->GetSize(); i++)
	{
		COrdMaq* pOM = (COrdMaq*) pArrBorrar->GetAt(i);
		// La añadimos al histórico
		g_pCache->AddHistorico(pOM);
		if (pOM->GetBNextLink() && pOM->Next()) g_pCache->AddHistorico(pOM->Next()); 
		// Y la borramos
		DeleteOM(pOM);
	}
	// y borramos y cambiamos las órdenes sin artículo que se solapen con el día.

	for(i=0; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		ASSERT_VALID(pPM);
		if (p_iLocal == 1 && pPM->GetsIDLocal() == "1" || p_iLocal != 1 && pPM->GetsIDLocal() != "1" )
		{
			for (int j = 0; j < pPM->GetNumOM(); j++)
			{
				COrdMaq* pOM = pPM->GetpOM(j);
				if (pOM->GetsID() != "") continue;
				if (pOM->GetFecIni() < p_Fec) // Hay solape
				{
					if (pOM->GetFecFin() <= p_Fec) // Está entera dentro
					{
						// La borramos
						DeleteOM(pOM);

					} else // Sólo está en parte
					{
						
						pOM->SetTs(pOM->GetFecFin() - p_Fec);
						pOM->SetBTiempoFijo(TRUE);
						pOM->SetFecIni(p_Fec);		// Recortamos lo que sobra
					}
				}
				
			
			}
		}
	}

	// Y cambiamos el orden en las que se ha cambiado el molde pero no se han completado,
	// buscamos el molde que venia detras y lo ponemos el primero.
	for(i=0; i < pArrNoFab->GetSize(); i++)
	{
		COrdMaq* pOM = (COrdMaq*) pArrNoFab->GetAt(i);
		CProgMaq* pPM = pOM->GetpPM();
		if (pOM->m_sAux == "") continue;
		COrdMaq* pPrimOM = pPM->FindOMbyMolde( pOM->m_sAux );
		if (pPrimOM)
		{
			MoveOrdMaq(pPM, pPrimOM, 0);
		}
	}
	


	delete pArrNoCambio;
	delete pArrNoFab;
	delete pArrNotFound;
	delete pArrBorrar;

	// Ahora ponemos las máquinas afectadas un día más avanzado
	
	for(i=0; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		ASSERT_VALID(pPM);
		if (p_iLocal == 1 && pPM->GetsIDLocal() == "1" || p_iLocal != 1 && pPM->GetsIDLocal() != "1" )
		{
			if (pPM->GetNumOM())
			{
				COrdMaq* pOM = pPM->GetpOM(0);
				ASSERT_VALID(pOM);
				pOM->SetFecIni(p_Fec);
				pOM->SetBFija(TRUE);
				// Actualizamos también la cadencia, para esta primera orden, por la del listado valorado
				CBufArticulo* pBufArt = pOM->GetpBufArticulo();
				if (pBufArt->m_LV_dP_H_real > 0) pOM->SetdCadencia(pBufArt->m_LV_dP_H_real);
			}
		}
	}
	

	Recalc();
	RecalcInacts();
	UpdateArrTL();		// Porque ha quedado mal tras borrar las máquinas que no cumplen el p_iLocal
	
}
*/

int CProg::GetiNumOM()
{
	int iNumOM = 0;
	for (int i=0; i< m_pArrPMs->GetSize(); i++)
	{                                  
		CProgMaq* pPM = (CProgMaq *) ((*m_pArrPMs)[i]);
		ASSERT( pPM->IsKindOf( RUNTIME_CLASS( CProgMaq ) ) );

		for (int j=0; j< pPM->GetNumOM() ; j++)
		{
			COrdMaq* pOM = pPM->GetpOM(j);
			ASSERT_VALID(pOM);
			
			iNumOM++;
		}
	}
	return iNumOM;
}

// Obtenemos un array de códigos de Máquinas ordenados por orden en programa
int CProg::GetArrMaquinas(CStringArray& p_ArrMaquinas)
{
	p_ArrMaquinas.RemoveAll();             
	for(int i=0; i< GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		p_ArrMaquinas.Add(pPM->GetsID());
	}
	return p_ArrMaquinas.GetSize();
}

// Obtenemos un array de códigos de Máquinas ordenados por orden en programa
int CProg::GetArrGFHs(CStringArray& p_ArrGFHs)
{
	p_ArrGFHs.RemoveAll();             
	for(int i=0; i < GetNumPM() ; i++)
	{
		CProgMaq* pPM = GetpPM(i);
		p_ArrGFHs.Add(pPM->GetsGFHDesc() );
	}
	return p_ArrGFHs.GetSize();
}