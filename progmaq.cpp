#include "stdafx.h"
#include "cpw.h"  
#include "cpwDefs.h"  
#include "AbSpan.h"
#include "cpwExt.h"
#include "prog.h"
#include "cpwDb.h"
#include "cache.h"            
#include "ProgMaq.h"
#include "OrdMaq.h"
#include <strstream>

IMPLEMENT_SERIAL(CProgMaq, CObject, 1)    

CProgMaq& 
CProgMaq::operator =(const CProgMaq& p_PM)
{
		m_BSelected = p_PM.GetBSelected();
        m_BEditCal = p_PM.GetBEditCal();
		m_FecOrd = p_PM.m_FecOrd;		// Fecha para ordenaciones

        for(int i = 0; i < p_PM.GetNumOM(); i++)
		{
			COrdMaq* pOMCop = p_PM.GetpOM(i);
			COrdMaq* pOM = new COrdMaq();
			*pOM = *pOMCop;
			Add(pOM);
		}
		if (p_PM.m_pArrInacts == NULL)
		{
			m_pArrInacts = NULL;
		}
		else
		{
			m_pArrInacts = new CObArray();
			m_pArrInacts->SetSize(0, TK_CACHE_INACT);
			for (int i = 0; i < p_PM.m_pArrInacts->GetSize(); i++)
			{
				CAbSpan* pAbsCop = (CAbSpan *) p_PM.m_pArrInacts->GetAt(i);
				CAbSpan* pAbs = new CAbSpan;
				*pAbs = *pAbsCop;
				m_pArrInacts->Add(pAbs);
			}
		}
		return *this;
}

 
void CProgMaq::Serialize(CArchive& ar)
{

	long lVoid = 0;
	double dVoid = 0;
	CTime FecVoid = CTime(0);
	CString sVoid = "";
//	AfxMessageBox(CString("Entrando a máquina"));

	
	CObject::Serialize(ar);
	if (ar.IsStoring())
	{        
 		ar << GetsID();
 		ar << m_PntPosition;

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
		ar >> m_sID;
		ar >> m_PntPosition;

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
		
		Relink();
		if (!m_pBufMaquina) {
			g_bException = true;
			AfxMessageBox("La máquina '" + m_sID + "' no existe en la base de datos, o no es programable. No se puede continuar.");
			ExitProcess(0);
		}
	}
//	AfxMessageBox(CString("Entrando a órdenes de máquina :") + this->GetsID());
	
    m_pArrOMs->Serialize(ar);
    if (!ar.IsStoring())
    {
		for (int i=0; i < m_pArrOMs->GetSize() ; i++)
    	{
			COrdMaq* pOM = (COrdMaq *) m_pArrOMs->GetAt(i);
			ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
   			pOM->SetpPM( this );
   		}
   }
}

void
CProgMaq::Relink()
{
		m_pBufMaquina = g_pCache->AddMaquina( m_sID );  
}

CProgMaq::CProgMaq( const char* p_sID )
{                      
	if (p_sID != NULL)
		m_pBufMaquina = g_pCache->AddMaquina( p_sID );
	else
		m_pBufMaquina = NULL;
	// m_pArrInacts debe ser NULL para no ser erroneamente dealocado
	m_pArrInacts = NULL;
	m_BEditCal = FALSE;
	m_pArrOMs = new CObArray();
	m_pArrOMs->SetSize(0, 50);
	m_sID = "";
}
 
CProgMaq::~CProgMaq()
{
	int i;
	ASSERT_VALID (m_pArrOMs);
	for ( i=0; i< m_pArrOMs->GetSize(); i++)
	{                                  
		COrdMaq* pOM = (COrdMaq *) m_pArrOMs->GetAt(i);
		ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
		delete pOM;
	}
	delete m_pArrOMs;	

	ASSERT_VALID (m_pArrInacts);
	for ( i=0; i< m_pArrInacts->GetSize(); i++)
	{                                  
		CAbSpan* pAbs = (CAbSpan *) m_pArrInacts->GetAt(i);
		ASSERT( pAbs->IsKindOf( RUNTIME_CLASS( CAbSpan ) ) );
		delete pAbs;
	}
	delete m_pArrInacts;	
}                 

int
CProgMaq::Add(COrdMaq* p_pOM)
{
	ASSERT_VALID(m_pArrOMs);
	int ind = m_pArrOMs->Add(p_pOM);
	p_pOM->SetpPM(this, TRUE);
	p_pOM->SetdPersonas(GetdPersonas());
	return ind;
}

void 
CProgMaq::SetpBufMaq(CBufMaquina* p_pBufMaq)
{
	m_pBufMaquina = p_pBufMaq;
	if (p_pBufMaq) m_sID = p_pBufMaq->m_sID;
	else m_sID = "";
	m_iDiasRetorno = g_pCache->GetiDiasRetorno(p_pBufMaq->m_sIDLocal);
};


int
CProgMaq::GetNumOM() const
{ 
	if (m_pArrOMs) return m_pArrOMs->GetSize();
	else return 0;
}   

COrdMaq* 
CProgMaq::GetpOM(int p_NumOrd) const
{
	if (p_NumOrd < 0 || p_NumOrd >= m_pArrOMs->GetSize()) return NULL;
	ASSERT_VALID(m_pArrOMs);
	return (COrdMaq*) m_pArrOMs->GetAt(p_NumOrd);
}  

int
CProgMaq::GetNumInacts() const
{ 
	if (m_pArrInacts) return m_pArrInacts->GetSize();
	else return 0;
}

CAbSpan*
CProgMaq::GetpInact(int p_NumInact) const
{
	ASSERT_VALID(m_pArrInacts);
	return (CAbSpan*) m_pArrInacts->GetAt(p_NumInact);
}  



CTime 
CProgMaq::GetFecIni() const
{
	ASSERT_VALID(m_pProg);
	return m_pProg->GetFecIni();
}

CTime 
CProgMaq::GetFecFin() const
{
	ASSERT_VALID(m_pProg);
	return m_pProg->GetFecFin();
}

int 
CProgMaq::Find(const COrdMaq* p_pOM) const 
{
// Encuentra una OM en una PM. Devuelve INT_MAX si no está
	int lim = GetNumOM();
	for(int i = 0; i < lim; i++)
		// Si el puntero es el mismo, la OM es la misma
		if (p_pOM == GetpOM(i)) return i;
	return INT_MAX;
}

void
CProgMaq::RecalcTL()
{
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);
	ASSERT_VALID(g_pCache);

	if (GetNumOM() == 0) return;
	if (GetlGFH() != TK_GFH_INYECCION) return;
	

	COrdMaq* pOM = GetpOM(0);
	pOM->RecalcFin();

}

void
CProgMaq::Recalc(int p_IndFrom)
{
	CProg* pProg = GetpProg();
	ASSERT_VALID(pProg);
	ASSERT_VALID(g_pCache);

	g_pCache->AssignInacts(*this);

	if (GetNumOM() == 0) return;

	COrdMaq* pOM;
	for(int ind = p_IndFrom; ind < GetNumOM(); ind++)
	{
		pOM = GetpOM(ind);
		ASSERT_VALID(pOM);
		pOM->SetpPM(this);
		pOM->Recalc();
		// Eliminamos las que tengan tiempo 0
		if (pOM->GetFecIni() == pOM->GetFecFin()) 
		{
			m_pArrOMs->RemoveAt(ind);
			delete pOM;
			ind--;
		}
		// Esto es por si acaso se han separado por error unas linkadas, para que no molesten
		if (pOM->GetBNextLink())
		{
			if (( ind + 1 ) >= GetNumOM()) pOM->SetBNextLink(FALSE);
			else
			{
				COrdMaq* pOMNext = GetpOM(ind+1);
				if (!pOMNext->GetBPrevLink()) pOM->SetBNextLink(FALSE);
			}
		}
		if (pOM->GetBPrevLink())
		{
			if (( ind - 1 ) < 0) pOM->SetBPrevLink(FALSE);
			else
			{
				COrdMaq* pOMPrev = GetpOM(ind-1);
				if (!pOMPrev->GetBNextLink()) pOM->SetBPrevLink(FALSE);
			}
		}
	}
	if (pOM->GetFecFin() >= pProg->GetFecFin())
		pProg->SetFecFin(pOM->GetFecFin());

}


void
CProgMaq::InsertOM(COrdMaq* p_pOM, int p_Ind)
{
	ASSERT_VALID(m_pArrOMs);
	p_pOM->SetBFija(FALSE);	// Si se inserta, pierde el estado
	m_pArrOMs->InsertAt(p_Ind, p_pOM);
	p_pOM->SetpPM(this, TRUE);
	p_pOM->Recalc();
}
 





// Funciones de calendario

// Sumamos un CTimeSpan a un CTime, pero teniendo en cuenta el calendario
// de la máquina, es decir, los periodos de inactividad guardados en m_pArrInacts
CTime
CProgMaq::AddCal(const CTime p_FecDT, CTimeSpan p_tsSecs) const
{
	
	CTime FecIni = p_FecDT;
	CTime FecFin = p_FecDT + p_tsSecs;
	if (p_tsSecs.GetTotalSeconds() <= 0L) return FecIni;
	// Hacemos un ciclo con las inactividades, hasta que alguna se solape con nuestro periodo

	CAbSpan* pInact = NULL;
	int lim = m_pArrInacts->GetSize();
	int i;
	for ( i = 0; i < lim ; i++)
	{
		pInact = (CAbSpan*) m_pArrInacts->GetAt(i);
		if (pInact->GetFecFin() > FecIni ) break;
	}
	// Al salir hemos alcanzado la fecha.
	// Si no tenemos inactividades por delante, devolvemos la suma normal
	// Si la primera inactividad empieza demasiado tarde para afectarnos,
	// lo mismo
	if (i >= lim) return FecFin; 
	if (pInact->GetFecIni() >= FecFin) return FecFin;
	// En caso contrario comenzamos una acumulacion de tiempo de actividad
	// hasta llegar a lo que tenemos que sumar (p_tsSecs)

	CTimeSpan tsActiv = 0;
	CTime		FecAct = FecIni;

	// Si estamos en una inactividad, comenzamos en la siguiente actividad

	if (FecIni >= pInact->GetFecIni() ) 
	{
		FecIni = pInact->GetFecFin();
		// Buscamos la siguiente inactividad. Si no hay, devolvemos la suma normal
		// la fecha por la que vamos mas los segundos brutos
		i++;
		if (i < lim) pInact = (CAbSpan*) m_pArrInacts->GetAt(i);
		else return FecIni + p_tsSecs;
	}

	for(;;)
	{
		// Vamos sumando segundos de actividad
		tsActiv += pInact->GetFecIni() - FecIni;
		
//		CString sTsActiv = FormatFec(tsActiv);
//		CString sFecIniInact = FormatFec(pInact->GetFecIni());
//		CString sFecIni = FormatFec(FecIni);
		
		// Cuando sumamos suficientes, devolvemos la fecha de inicio
		// mas los segundos que nos quedaban (la expresion que se
		// devuelve es equivalente a eso)
		if (tsActiv >= p_tsSecs)
		{
			return (CTime) pInact->GetFecIni() - (tsActiv - p_tsSecs);
			//return (CTime) FecIni - (tsActiv - p_tsSecs);
		}
		FecIni = pInact->GetFecFin();
		// Buscamos la siguiente inactividad. Si no hay, devolvemos
		// la fecha por la que vamos mas los segundos que quedan por
		// ejecutar
		i++;
		if (i < lim) pInact = (CAbSpan*) m_pArrInacts->GetAt(i);
		else return (CTime) FecIni + (p_tsSecs - tsActiv);
	} 
}

/*
CTime
CProgMaq::SubsCal(const CTime p_FecDT, CTimeSpan p_tsSecs) const
{
	CTime FecDT = p_FecDT;
	if (p_tsSecs == 0L) return FecDT;
	if (EsInactPeriod(FecDT)) FecDT = GetPrevActEnd(FecDT);
	for(;;)
	{
		CTime Prev = GetPrevActStart(FecDT);
		CTimeSpan tsDiff = FecDT - Prev;
		if (p_tsSecs < tsDiff) break;
		p_tsSecs -= tsDiff;
		FecDT = GetPrevInactStart(FecDT, p_tsSecs);
	} // for (;;)
	FecDT = FecDT - p_tsSecs;
	return FecDT;
}
*/

CTimeSpan
CProgMaq::DiffCal(const CTime p_FecB, const CTime p_FecA, CTimeSpan p_tsLim) const
{
	// Hacemos una resta de fechas (B - A), según el calendario, es decir las inactividades
	// que tengamos. Restamos una fecha de otra y sale un tiempo, del que hay que
	// restar los tiempos de inactividad. El CTimeSpan resultante puede ser negativo.
	long Invert = 1;
	CTime FecB = p_FecB;
	CTime FecA = p_FecA;
	
	// Vamos a hacer siempre la resta en el mismo sentido,
	// con B > A. Si no lo es, lo cambiamos y luego cambiamos el signo
	// del resultado

	if (FecB < FecA)
	{
		CTime Temp(FecB);
		FecB = FecA;
		FecA = Temp;
		Invert = -1;
	}

	// Nos olvidamos del caso en que las fechas estén fuera del
	// rango de la programación. En ese caso consideraremos todo
	// el tiempo sin inactividades. Normalmente no influirá en el
	// resultado.

//	if (FecA < GetFecIni() || FecB > GetFecFin()) 
//		return DiffCalF(p_FecB, p_FecA, p_tsLim);
	
	CTimeSpan tsLim = FecB - FecA;
	CTimeSpan tsNoValid = 0L;
	// Hacemos un ciclo por las inactividades.
	int lim = m_pArrInacts->GetSize();
	for (int i = 0; i < lim ; i++)
	{
		CAbSpan* pInact = (CAbSpan*) m_pArrInacts->GetAt(i);
		ASSERT_VALID(pInact);
		if (pInact->GetFecFin() > FecA && pInact->GetFecIni() < FecB) 
		{	
			tsNoValid += pInact->GetFecFin() - pInact->GetFecIni();
			if (pInact->GetFecIni() < FecA) tsNoValid -= FecA - pInact->GetFecIni();
			if (pInact->GetFecFin() > FecB) tsNoValid -= pInact->GetFecFin() - FecB;
		}
		if (pInact->GetFecIni() > FecB) break; 
	}
	return (CTimeSpan) ((tsLim - tsNoValid).GetTotalSeconds() * Invert);
}



void 
CProgMaq::SetProg( CProg* p_pProg )
{  
	m_pProg = p_pProg;
	g_pCache->AssignInacts(*this);
}

void 
CProgMaq::Selec( BOOL p_BSelecMaq, enum enumSelecOrd p_eSelecOrd,
				int p_iDesdeOrden , int p_iHastaOrden )
{
	if (p_BSelecMaq) m_BSelected = TRUE;
	else m_BSelected = FALSE;
	
	if (p_eSelecOrd != SinCambios)
	{
		int lim = GetNumOM();
		for(int i = 0; i < lim; i++)
			if (i >= p_iDesdeOrden && i <= p_iHastaOrden)
			{
				COrdMaq* pOM = GetpOM(i);
				if (p_eSelecOrd == SelecTrue)
					pOM->SetBSelected(TRUE);
				else if (p_eSelecOrd == SelecFalse)
					pOM->SetBSelected(FALSE);
			}
	}
}


double 
CProgMaq::GetdInactividad()
{
	if (m_pBufMaquina != NULL)
		return (double) m_pBufMaquina->m_lInact;
//		return (double) TK_INACT_DEF;
	else
		return 0.0;
}

CString 
CProgMaq::GetsIDLocal()
{
	if (m_pBufMaquina != NULL)
		return m_pBufMaquina->m_sIDLocal;
	else
		return "";
}

// Encuentra una OM con un molde determinado en una PM. Devuelve NULL si no está
// Considera las OMs que estan en dos arrays como no válidas
COrdMaq* 
CProgMaq::FindOMbyMolde(const CString &p_sIDMolde, CObArray *p_pArrBorrar, CObArray *p_pArrNoFab)
{
	BOOL bEnMatriz;
	int lim = GetNumOM();
	COrdMaq* pOM;
	for(int i = 0; i < lim; i++)
	{
		pOM = GetpOM(i);
		if (p_sIDMolde == pOM->GetsMolde()) // Si hemos encontrado el molde
		{
			bEnMatriz = FALSE;
			if (p_pArrBorrar)
			{
				// Comprobamos que no esté en ninguna de las dos matrices
				for (int iB = 0; iB < p_pArrBorrar->GetSize(); iB++)
				{
					COrdMaq* pOMB = (COrdMaq*) p_pArrBorrar->GetAt(iB);
					ASSERT( pOMB->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
					if (pOMB == pOM) 
					{
						bEnMatriz = TRUE;
						break;
					}
				}
			}
			if (p_pArrNoFab && !bEnMatriz)
			{
				for (int iB = 0; iB < p_pArrNoFab->GetSize(); iB++)
				{
					COrdMaq* pOMB = (COrdMaq*) p_pArrNoFab->GetAt(iB);
					ASSERT( pOMB->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
					if (pOMB == pOM) 
					{
						bEnMatriz = TRUE;
						break;
					}
				}
				
			}
			if (!bEnMatriz) return pOM;
		} // If molde encontrado
	} // Ciclo OMs
	
	return NULL;
}

BOOL
CProgMaq::HayInactEn(CAbSpan p_AbSpan)
{
	int lim = m_pArrInacts->GetSize();
	for (int i = 0; i < lim ; i++)
	{
		CAbSpan* pInact = (CAbSpan*) m_pArrInacts->GetAt(i);
		if (pInact->GetFecFin() <= p_AbSpan.GetFecIni() ) continue;
		if (pInact->GetFecIni() >= p_AbSpan.GetFecFin() ) break;
		return TRUE;
	}
	return FALSE;
}

// Obtiene un array de moldes que pueden fabricar un artículo y que ya están en el 
// programa en esa máquina
int 
CProgMaq::GetArrMoldesYaUsados(const CString &p_sIDArt, CObArray& p_ArrMoldes)
{
	p_ArrMoldes.RemoveAll();
	int lim = GetNumOM();
	COrdMaq* pOM;
	for(int i = 0; i < lim-1; i++)	// No llegamos hasta el final porque ya hemos insertado la orden, que no se debe
									// tener en cuenta
	{
		pOM = GetpOM(i);
		CBufMolde* pBufMolde = pOM->GetpBufMolde();
		if (pBufMolde)
		{
			int iLimArt = pBufMolde->m_pArrMolArt->GetSize();
			for (int j=0; j < iLimArt; j++)
			{
				CBufArtMol* pBufArtMol = (CBufArtMol*) pBufMolde->m_pArrMolArt->GetAt(j);
				ASSERT( pBufArtMol->IsKindOf( RUNTIME_CLASS( CBufArtMol ) ) );
				if (p_sIDArt == pBufArtMol->m_sIDArticulo)
				{
					p_ArrMoldes.Add(pBufMolde);
				}
			}
		}
	}
	return p_ArrMoldes.GetSize();
}

int 
CProgMaq::GetiDiasMezcla()
{
	if (!m_pBufMaquina)	return 0;
	return g_pCache->GetiDiasMezcla(m_pBufMaquina->m_sIDLocal);
}

double 
CProgMaq::GetdPersonas()
{
	if (!m_pBufMaquina)	return 1;
	return m_pBufMaquina->m_dPersonas;
}

int 
CProgMaq::GetiDiasRetorno()
{
	return m_iDiasRetorno;
}

int 
CProgMaq::GetiFrecuenciaMezcla()
{
	if (!m_pBufMaquina)	return 0;
	return g_pCache->GetiFrecuenciaMezcla(m_pBufMaquina->m_sIDLocal);
}

// Obtiene un array de moldes que pueden fabricar un artículo y que ya están en el 
// programa en esa máquina
int 
CProgMaq::GetArrMoldesEnMaq(const CString &p_sIDArt, CObArray &p_ArrMoldes)
{
	p_ArrMoldes.RemoveAll();

	int lim = GetNumOM();
	COrdMaq* pOM;
	for(int i = 0; i < lim; i++)
	{
		pOM = GetpOM(i);
		CBufMolde* pBufMolde = pOM->GetpBufMolde();
		if (pBufMolde)
		{
			int iLimArt = pBufMolde->m_pArrMolArt->GetSize();
			for (int j=0; j < iLimArt; j++)
			{
				CBufArticulo* pBufArt = (CBufArticulo*) pBufMolde->m_pArrMolArt->GetAt(j);
				ASSERT( pBufArt->IsKindOf( RUNTIME_CLASS( CBufArticulo ) ) );
				if (p_sIDArt == pBufArt->m_sID)
				{
					p_ArrMoldes.Add(pBufMolde);
				}
			}
		}
	}
	return p_ArrMoldes.GetSize();
}

CTime 
CProgMaq::AdjustInact(CTime p_ts, BOOL BIni)
{
	CAbSpan* pInact = NULL;
	int lim = m_pArrInacts->GetSize();
	if (!BIni)		// Ajustamos el final de la orden
	{
		for (int i = 0; i < lim ; i++)
		{
			pInact = (CAbSpan*) m_pArrInacts->GetAt(i);
			if (pInact->GetFecIni() > p_ts )
			{
				CTimeSpan tsDiff = pInact->GetFecIni() - p_ts;
				if (tsDiff.GetTotalSeconds() < TK_ADJUST_INACT_SECS) return pInact->GetFecIni();
				else return p_ts;
			}
		}
	}
	else
	{
		for (int i = 0; i < lim ; i++)
		{
			pInact = (CAbSpan*) m_pArrInacts->GetAt(i);
			if (pInact->GetFecIni() >= p_ts )
			{
				CTimeSpan tsDiff = pInact->GetFecIni() - p_ts;
				if (tsDiff.GetTotalSeconds() < TK_ADJUST_INACT_SECS) return pInact->GetFecFin();
				else return p_ts;
			}
		}
	}
	return p_ts;
}

int 
CProgMaq::GetDiasRetorno()
{
	if (!m_pBufMaquina)	return 0;
	return g_pCache->GetiDiasRetorno(m_pBufMaquina->m_sIDLocal);
	
}

long 
CProgMaq::GetlGFH()
{
	if (!m_pBufMaquina)	return -1;
	return m_pBufMaquina->m_lGFH;
}
