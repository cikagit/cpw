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
#include "cpwDoc.h"
#include "TimeLine.h"
#include "Turnos.h"

#include <strstream>

IMPLEMENT_SERIAL(COrdMaq, CObject, 1)

void
COrdMaq::SetFecFin(CTime p_Fec)
{
	CProg* pProg = GetpProg();
	
	m_Abs.SetFecFin(ZeroSec(p_Fec));
	if (pProg)
	{
		if (p_Fec > pProg->GetFecFin()) pProg->SetFecFin (AddDays(ZeroSec(p_Fec),2));
	}
}
								
COrdMaq&
COrdMaq::operator =(const COrdMaq& p_OM)
{                                           
	if (this == &p_OM) return *this;
	
	m_Abs = p_OM.m_Abs;    
	m_ts = p_OM.m_ts;
	m_tsMargen = p_OM.m_tsMargen;
	m_tsMezcla = p_OM.m_tsMezcla;
	m_Color = p_OM.m_Color;
	m_pPM = p_OM.m_pPM;
	m_lCant = p_OM.m_lCant;
	m_dCadencia = p_OM.m_dCadencia;
	SetpBufArt(p_OM.m_pBufArt);
	SetpBufMolde (p_OM.m_pBufMolde);
	m_BSelected = p_OM.m_BSelected;
	m_BFija = p_OM.m_BFija;
	m_BTiempoFijo = p_OM.m_BTiempoFijo;
	m_lCantFab = p_OM.m_lCantFab;
	m_FecFab = p_OM.m_FecFab;
	m_FecOrd = p_OM.m_FecOrd;
	m_FecMezcla = p_OM.m_FecMezcla;
	m_dPersonas = p_OM.m_dPersonas;
	
	m_lAux = p_OM.m_lAux;
	m_dAux = p_OM.m_dAux;
	m_sAux = p_OM.m_sAux;

	m_iLink = 0;

	return *this;
}      

void
COrdMaq::SetValues(const COrdMaq* p_pOM)
{                                           
	m_Abs = p_pOM->m_Abs;    
	m_ts = p_pOM->m_ts;
	m_tsMargen = p_pOM->m_tsMargen;
	m_tsMezcla = p_pOM->m_tsMezcla;
	m_Color = p_pOM->m_Color;
	m_pPM = p_pOM->m_pPM;
	m_lCant = p_pOM->m_lCant;
	m_dCadencia = p_pOM->m_dCadencia;
	m_dPersonas = p_pOM->m_dPersonas;
	SetpBufArt(p_pOM->m_pBufArt);
	SetpBufMolde(p_pOM->m_pBufMolde);
	m_BSelected = FALSE;
	m_BFija = FALSE;
	m_BTiempoFijo = FALSE;
	m_lCantFab = 0;
	m_FecFab = CTime::GetCurrentTime();
	m_FecMezcla = p_pOM->m_FecMezcla;
	
	m_iLink = 0;

	m_lAux = p_pOM->m_lAux;
	m_dAux = p_pOM->m_dAux;
	m_sAux = p_pOM->m_sAux;

} 

BOOL 
COrdMaq::operator ==(const COrdMaq& p_OM) const
{
	if ( m_pBufArt == p_OM.m_pBufArt && 
		 m_pBufMolde == m_pBufMolde  
		 && GetFecIni() == p_OM.GetFecIni())
		return TRUE;
			
	return FALSE;					
}
		

void
COrdMaq::CalcFecIni() 
{
	
//	ASSERT_VALID(m_pPM);
	if (!m_pPM)	return;
	if (m_BFija) return;
	COrdMaq* pPrev = Prev();
	if (First() || !pPrev)
		m_Abs.m_FecIni = (GetpProg()->GetFecIni());
	else
	{
		//if (GetBPrevLink()) 
		//	m_Abs.m_FecIni = Prev()->GetFecIni();
		//else 
			m_Abs.m_FecIni = (Prev()->GetFecFin());
	}
	m_Abs.m_FecIni = m_pPM->AdjustInact(m_Abs.m_FecIni, TRUE);
}

void
COrdMaq::CalcTsMargen() 
{
	m_tsMargen = (CTimeSpan) 0;
}

// Calculamos el tiempo que tenemos con la mezcla que nos queda
CTimeSpan
COrdMaq::CalcTsMezcla(long lCant) 
{
	
	if ( GetdCadencia() > 0 )
	{
		long lSecsMezcla = (long) ( (double) lCant * TK_SEGS_HORA / GetdCadenciaTL(Mezcla) );
		return (CTimeSpan) ( (long) ((double) lSecsMezcla 
							* ( 1.0 + ( GetdInactividad() / 100.0 ))));
	}

	return (CTimeSpan) 0;
}

// calculamos todos los parámetros de la orden en la máquina.
// Debemos contar con la pieza, la cantidad y la fecha límite
// y la orden debe estar insertada en una ProgMaq
int
COrdMaq::Recalc()
{

	CString sPieza = GetsNombre();
	if (sPieza == "") return -1;

	CalcFecIni();
	CalcTimeSpan();  // tiempo que tarda en segundos
			
	// Fecha fin es la fecha inicio + el tiempo que tarda 
	// teniendo en cuenta el calendario
	CalcFecFin();
// Calculamos el margen
	CalcTsMargen();
	
	if (GetpProg())
	{
		// Si la fecha de fin de la orden es mayor que la de la programación,
		// la modificamos, lo que modifica también los scrollers
		if (GetpProg()->GetFecFin() < GetFecFin())
			GetpProg()->SetFecFin(GetFecFin());
		// Si hay un documento le decimos que ha sido modificado
		if (GetpProg()->GetpDoc())
			GetpProg()->GetpDoc()->SetModifiedFlag(TRUE);
	}
	return 0;
}	

BOOL
COrdMaq::LimitMaquina( const char* p_sMaquina ) const
{
	ASSERT_VALID(g_pCache);
	
	long lGFH = g_pCache->GetlGFH(p_sMaquina);


	if (lGFH == TK_GFH_INYECCION)
		return g_pCache->LimitMolMaq(GetsMolde(), GetsMaquina());
	else 
	{
		CBufArticulo* pBufArt = GetpBufArticulo();
		if (pBufArt) 
		{
			double dCadencia = pBufArt->GetdCadenciaAcabado(atol(GetsMaquina()));
			return !(dCadencia > 0);
		}
		else return TRUE;
	}
	
}

BOOL
COrdMaq::LimitGFH( long p_lGFH ) const
{

	return FALSE;
}



CBrush*
COrdMaq::GetpBrush(enum eTipoPrint p_eTipoPrint) const
{

	if (GetsID() == "") return &g_BrushParada;
	int Peligro = 0;
	if (m_bConfMolde || m_bConfMoldeElem) Peligro = 2;
	ASSERT(Peligro <= CPW_N_BRUSHES);
	//if (p_eTipoPrint == Impresora)
	//	return &(g_BrushArrImp[Peligro*2 + (GetBSelected() ? 1 : 0)]);
	//else
		return &(g_BrushArr[Peligro*2 + (GetBSelected() ? 1 : 0)]);
	
}


void
COrdMaq::UnselectAll()
{
	if (!m_pPM) return;
	COrdMaq* pOMAct;
	
	for(int ind = 0; ind < m_pPM->GetNumOM(); ind++)
	{
		pOMAct = m_pPM->GetpOM(ind);
		ASSERT_VALID (pOMAct);
		pOMAct->SetBSelected(FALSE);
	}
}


void
COrdMaq::SelecTo()
{
	if (!m_pPM) return;
	COrdMaq* pOMAct;
	COrdMaq* pOMUltSel = 0;

	// MaxInd y MinInd indican los índices del primero y el último
	// seleccionados. ActInd el de la Orden actual
	int MaxInd = -1;
	int MinInd = -1;
	int ActInd = -1;
	int TopInd = m_pPM->GetNumOM();
	// DesInd y HasInd marcan los límites de selección finales
	int DesInd;
	int HasInd;

	for(int ind = 0; ind < TopInd; ind++)
	{
		pOMAct = m_pPM->GetpOM(ind);
		ASSERT_VALID (pOMAct);
		if (pOMAct == this)
			ActInd = ind;
		if (pOMAct->GetBSelected()) 
		{
			MaxInd = ind;
			if (MinInd == -1) MinInd = ind;		
		}
	}
	
	// Ahora calculamos DesInd y HasInd
	// Por defecto 
	DesInd = ActInd;	// Seleccionaremos desde la actual
	HasInd = TopInd;	// Hasta el final
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
			HasInd = ActInd;	// Hasta la actual
		}
	}
	if (DesInd < 0) DesInd = 0;
	if (DesInd >= m_pPM->GetNumOM()) DesInd = m_pPM->GetNumOM() - 1;
	if (HasInd < 0) HasInd = 0;
	if (HasInd >= m_pPM->GetNumOM()) HasInd = m_pPM->GetNumOM() - 1;
	// Realizamos la verdadera selección
	for(int ind = DesInd; ind <= HasInd; ind++)
	{
		pOMAct = m_pPM->GetpOM(ind);
		ASSERT_VALID (pOMAct);
	
		pOMAct->SetBSelected(TRUE);
	}
}

CBufArtMol*		
COrdMaq::GetpBufArtMol() const
{
	if (!m_pBufArt) return NULL;
	
	if (!(m_pBufArt->m_pArrArtMol)) return NULL;
	
	for (int i = 0; i <  m_pBufArt->m_pArrArtMol->GetSize() ; i++)
	{
		CBufArtMol* pBufArtMol = (CBufArtMol*) m_pBufArt->m_pArrArtMol->GetAt(i);
		if (pBufArtMol->m_pBufMolde == GetpBufMolde()) return pBufArtMol;
	}
	return NULL;
}

COrdMaq::COrdMaq(const CString& p_sIDArticulo)
{
// Al actualizar, recordar que hay otro constructor
	m_pPM = NULL;
	SetpBufArt(g_pCache->AddArticulo(p_sIDArticulo));
	if (m_pBufArt)
	{	
		if (m_pBufArt->m_pArrArtMol && m_pBufArt->m_pArrArtMol->GetSize() > 0)
		{
			CBufArtMol* pBufArtMol = (CBufArtMol*) m_pBufArt->m_pArrArtMol->GetAt(0);
			SetpBufMolde(pBufArtMol->m_pBufMolde);
		} else
		{
			SetpBufMolde(NULL);
		}

	}
	m_lCant = 0;
	m_dCadencia = 0;
	m_dPersonas = 1;
	m_dChatarras = -1;
	m_tsMargen = (CTimeSpan) 0;
	m_tsMezcla = (CTimeSpan) 0;
	m_ts = (CTimeSpan) 0;
	m_BSelected = FALSE;
	m_iLink = 0;
	m_BFija = FALSE;
	m_BTiempoFijo = FALSE;
	m_lCantFab = 0L;
	m_FecFab = CTime::GetCurrentTime();
	m_FecMezcla = CTime::GetCurrentTime();
	
	m_lAux = 0;
	m_dAux = 0;
	m_sAux = "";
	
	m_bConfMolde = m_bConfMoldeElem = FALSE;
	m_pOMConf = NULL;
}
COrdMaq::COrdMaq(CBufOF* p_pOF, CBufMolde* p_pMolde )
{
// Al actualizar, recordar que hay otro constructor
	m_pPM = NULL;
	SetpBufMolde(p_pMolde);
	if (p_pOF == NULL)
	{
		SetpBufArt(NULL);
		m_lCant = 0;
	}
	else
	{
		SetpBufArt(g_pCache->AddArticulo(p_pOF->m_sIDArticulo));
		m_lCant = (long) p_pOF->m_dCantOrig;
	}
	m_dCadencia = 0;
	m_dPersonas = 1;
	m_dChatarras = -1;
	m_tsMargen = (CTimeSpan) 0;
	m_tsMezcla = (CTimeSpan) 0;
	m_ts = (CTimeSpan) 0;
	m_BSelected = FALSE;
	m_iLink = 0;
	m_BFija = FALSE;
	m_BTiempoFijo = FALSE;
	m_lCantFab = 0L;
	m_FecFab = CTime::GetCurrentTime();
	m_FecMezcla = CTime::GetCurrentTime();
	
	m_lAux = 0;
	m_dAux = 0;
	m_sAux = "";

	m_bConfMolde = m_bConfMoldeElem = FALSE;
	m_pOMConf = NULL;
}

void COrdMaq::Serialize(CArchive& ar)
{   
	long lVoid = 0;
	double dVoid = 0;
	CTime FecVoid = 0;
	CString sVoid = "";

	FecVoid.GetCurrentTime();

	ASSERT_VALID( g_pCache );
	CObject::Serialize(ar);

		
	m_Abs.Serialize(ar);
	if (ar.IsStoring())
	{        
		ar << m_ts;
		ar << GetsID();
		ar << GetsMolde();
		ar << m_lCant;
		ar << m_tsMargen;
		ar << m_BFija;
		ar << m_BTiempoFijo;
		ar << m_lCantFab;
		ar << m_FecFab;

		ar << m_iLink;
		ar << m_dCadencia;
		ar << FecVoid;
		ar << m_SetErr.GetAsString();
		ar << lVoid;
		ar << m_dChatarras;
		ar << FecVoid;
		ar << sVoid;
		ar << lVoid;
		ar << m_dPersonas;
		ar << FecVoid;
		ar << sVoid;
		ar << lVoid;
		ar << dVoid;
		ar << FecVoid;
		ar << sVoid;
		
	}
	else
	{
//		long OFID;
		CString sArtID;
		CString sMoldeID;
		CString sErr;
		
		ar >> m_ts;
	    
		ar >> m_sIDArticulo;
		ar >> m_sIDMolde;

		Relink();

		ar >> m_lCant;
		ar >> m_tsMargen;
		ar >> m_BFija;
		ar >> m_BTiempoFijo;
		ar >> m_lCantFab;
		ar >> m_FecFab;
		
		ar >> m_iLink;
		ar >> m_dCadencia;
		ar >> FecVoid;
		ar >> sErr;
		m_SetErr = CSet(sErr);
		ar >> lVoid;
		ar >> m_dChatarras;
		ar >> FecVoid;
		ar >> sVoid;
		ar >> lVoid;
		ar >> m_dPersonas;
		if (m_dPersonas == 0) m_dPersonas = 1;
		ar >> FecVoid;
		ar >> sVoid;
		ar >> lVoid;
		ar >> dVoid;
		ar >> FecVoid;
		ar >> sVoid;
		
	}
	//AfxMessageBox(CString("Procesada orden :") + GetsNombre());
    
}
void
COrdMaq::Relink()
{		                                                                
	SetpBufArt(g_pCache->AddArticulo( m_sIDArticulo ));
	//m_pBufOF = g_pCache->FindOFArt( m_sIDArticulo );
	SetpBufMolde(g_pCache->FindpMolde( m_sIDMolde ));
}	

long
COrdMaq::GetlCantTot() const
{
	return CalclCantTot(GetlCantidad());
}

long
COrdMaq::GetlTiradas() const
{
	return CalclTiradas(GetlCantTot());
}

long
COrdMaq::CalclCantTot(long p_lCant) const
{
	double dCant = (p_lCant * (1 + (GetdChatarras() / 100)));
	long lCant = (long) dCant;
	if ((dCant - lCant) > .5) lCant++;
	return lCant;
}

long
COrdMaq::CalclTiradas(long p_lCant) const
{
	long lCant = 0;
	if (GetlCavidades() > 0)
	{
		lCant = (long) (p_lCant / GetlCavidades());
		if (p_lCant % GetlCavidades()) lCant++;
	} 
	return lCant;
}

long
COrdMaq::CalclCantDeTiradas(long p_lCant) const
{
	if (GetlCavidades() > 0)
		return (long) (p_lCant * GetlCavidades());
	else
		return p_lCant;
}

long
COrdMaq::GetlCantSinChatarras(long p_lCantChatarras) const
{
	long lCant = (long) (p_lCantChatarras / (1 + (GetdChatarras() / 100)));
	long lCantChatCalc = CalclCantTot(lCant);
	if (lCantChatCalc < p_lCantChatarras) lCant++;
	else if (lCantChatCalc > p_lCantChatarras) lCant--;

	return lCant;

}

long
COrdMaq::GetlCantTiradas(long p_lCantTiradas) const
{
	long lCantTot = CalclCantDeTiradas(p_lCantTiradas);
	long lCant = GetlCantSinChatarras(lCantTot);
	long lCantTotCalc = CalclCantTot(lCant);
	long lTiradasCalc = CalclTiradas(lCantTotCalc);

	if (lTiradasCalc < p_lCantTiradas) lCant += GetlCavidades();
	else if (lTiradasCalc > p_lCantTiradas) lCant -= GetlCavidades();

	return lCant;

}

void
COrdMaq::CalcTimeSpan()
{                    
	if (GetBTiempoFijo()) return;
	if (GetBPrevLink()) 
	{
		COrdMaq* pPrev = Prev();
		if (pPrev)	
		{
			m_ts = pPrev->GetTimeSpan();
			return;
		}
		
	}
	double dCantidad = (double) GetlCantTot();
	// Calculamos el tiempo que tarda en relizarse la orden
	double dCadencia = GetdCadenciaGFH();
	double dPersonas = GetdPersonas();
	if (dPersonas <= 0) dPersonas = 1;
	m_ts = (CTimeSpan) (long) ((  dCantidad * TK_SEGS_HORA / dCadencia ) / dPersonas);
	
	// Si hemos introducido piezas fabricadas a una fecha se hace el siguiente cálcula
	// 1. Las piezas ya fabricadas se restan an pedir la cantidad (en GetlCantidad) por lo que
	//    el tiempo ya está reducido para esa cantidad
	// 2. Estas piezas están fabricadas a una fecha, por lo que tenemos dos casos:
	//    a) Si esa fecha es anterior a la de inicio de la orden, no habrá que hacer nada.
	//	  b) Si es posterior, habrá que sumar el tiempo necesario 
	//			Para ello restamos la fecha de fabricación de la fecha de inicio, teniendo
	//			en cuenta el calendario.
	/*
	if (GetlCantFab() > 0)
	{
		CTimeSpan tsDiff;
		if (m_pPM)
			tsDiff = m_pPM->DiffCal(GetFecFab(), GetFecIni());
		else
			tsDiff = GetFecFab() -  GetFecIni();
		if (tsDiff.GetTotalSeconds() > 0)	// Si es posterior
		{
			m_ts += tsDiff;
		}
	}
	*/

	// Y ahora redondeamos a enteros en las horas, para arriba
	double dHoras = (double) m_ts.GetTotalSeconds()  / TK_SEGS_HORA;
	double dHorasNd = ((long) dHoras );
	if (dHorasNd != dHoras) dHorasNd++;

	m_ts = (CTimeSpan) (long) (dHorasNd * TK_SEGS_HORA);

		
}

CTimeSpan                             
COrdMaq::GetTotalTs() const
{
	CTimeSpan ts;
	// Si tiene un porcentaje de inactividad, habrá que aplicarlo
	//if (GetBTiempoFijo()) return m_ts;
	ts = (CTimeSpan) ( (long) ((double)m_ts.GetTotalSeconds() 
							* ( 1.0 + ( GetdInactividad() / 100.0 ))));
	//Si la máquina tiene un tiempo de cambio, lo aplicamos
	ts += m_pPM->GettsCambio();

	return ts;
}

void
COrdMaq::CalcFecFin()
{
	if (!m_pPM) return;
	CProg* pProg = GetpProg();
	//ASSERT_VALID(pProg);

	// Si la fecha de inicio es anterior del inicio de la programación
	// no tenemos calendario, y por tanto no calculamos la fecha de fin
	if (GetFecIni() < pProg->GetFecIni()) return;
	CTime FecFin = m_pPM->AddCal(m_Abs.m_FecIni, GetTotalTs());

	// La ajustamos al comienzo de una inactividad, si está a menos de 5 minutos
	SetFecFin(m_pPM->AdjustInact(FecFin));
	
//	CString sTestIni = FormatFec(m_Abs.m_FecIni);
//	CString sTest = FormatFec(GetTotalTs());
//	CString sTestFin = FormatFec(GetFecFin());
}                                  

void
COrdMaq::SetpPM(CProgMaq* p_pPM, BOOL p_bCheck)
{
	ASSERT_VALID(p_pPM);
	m_dAux = GetdCadencia();
	m_pPM = p_pPM;           
	if (!p_pPM) return;
	// Si no hay que chequear (es la carga de un programa), regresamos
	if (!p_bCheck) return;
	if (p_pPM->GetlGFH() == TK_GFH_INYECCION )
	{
		if (p_bCheck && GetsMolde() != "")
		{
			if (LimitMaquina(p_pPM->GetsID()))
			{
				CString sMsg = STRID( IDS_WAR_MAQNOP );
				g_bException = TRUE;		
				if (AfxMessageBox(sMsg, MB_OKCANCEL)  == IDOK)
					g_pCache->ActLimitMaq(GetsMolde(), p_pPM->GetsID());
				g_bException = FALSE;
			}	
		}
	} else if (p_pPM->GetlGFH() == TK_GFH_ACABADO)
	{
		if (p_bCheck)
		{
			if (LimitMaquina(p_pPM->GetsID()))
			{
				CString sMsg = STRID( IDS_WAR_NOCAD );
				g_bException = TRUE;		
				AfxMessageBox(sMsg);	
				SetdCadencia(m_dAux);
				g_bException = FALSE;
			}	
		}
	}
}

double
COrdMaq::GetdPeso() const
{
	if (m_pBufArt)
		return m_pBufArt->m_dPeso;
	else 
		return 0;
}

long
COrdMaq::GetlCantReal() const
{
	return CalclCantReal(m_ts);
}

long
COrdMaq::CalclCantReal(CTimeSpan p_ts) const
{
	// Calculamos las piezas que salen para un tiempo, (totales, con chatarras y todo)
	// Echamos para atrás.
	long lCadencia = (long) GetdCadenciaGFH();
	if (GetdCadencia() - lCadencia >= .5) lCadencia++;
	return (long) ( (double) p_ts.GetTotalSeconds()  * (double) lCadencia / (double) TK_SEGS_HORA );
}

double		
COrdMaq::GetdCadencia(long p_lCentro) const
{
	if (GetlGFH() == TK_GFH_INYECCION)
	{
		if (m_dCadencia > 0) return m_dCadencia;
		else if (m_pBufArt)	return m_pBufArt->GetdCadencia();

	} else if (GetlGFH() == TK_GFH_ACABADO)
	{
		if (m_dCadencia > 0) return m_dCadencia;
		if (m_pBufArt)	return m_pBufArt->GetdCadenciaAcabado(p_lCentro);
	}
	return 1000;
}

double		
COrdMaq::GetdCadenciaGFH() const
{
	if (GetlGFH() == TK_GFH_INYECCION)
	{
		if (m_dCadencia > 0) return m_dCadencia;
		else if (m_pBufArt)	return m_pBufArt->GetdCadencia();

	} else if (GetlGFH() == TK_GFH_ACABADO)
	{
		if (m_dCadencia > 0) return m_dCadencia;
		long lCentro = atol(GetsMaquina());
		if (m_pBufArt)	return m_pBufArt->GetdCadenciaAcabado(lCentro);
	}
	return 1000;
}

double		
COrdMaq::GetdCadenciaTL(enum  eTimeLine p_eTL) const
{
	if (p_eTL == Mezcla)
	{
		return (GetdCadencia() * GetdPeso()) / 1000;	// El peso está en gramos
	}
	else
	{
		return GetdCadencia();
	}
}

long
COrdMaq::GetlRestoCantConMezcla() const 
{ 
	int lCant = GetlCantTot();
	CProg* pProg = m_pPM->GetpProg();
	if (!pProg) return lCant;

	CTimeLine* pTL = pProg->GetpTL(GetpBufArticulo(), Mezcla);
	if (!pTL) return lCant;
	long lResto = pTL->GetlResto();
	if (lResto > 0)
	{	// Si nos sobra cantidad, devolvemos las piezas de la OM más lo que se puede hacer con lo que nos sobra
		return (long) (lCant + ((double) pTL->GetlResto() * 1000 / (double) GetdPeso())); 
	} else
	{	// Si nos falta cantidad, devolvemos la cantidad que se puede hacer
		return m_lRestoMezcla;
	}
}

long
COrdMaq::GetlCavidades() const
{
	CBufArtMol* pBufArtMol = GetpBufArtMol();
	if (pBufArtMol) return	pBufArtMol->m_lCantidad;
	else return 0;
}
	
double COrdMaq::GetdChatarras() const
{
	if (m_dChatarras >= 0) return m_dChatarras;
	if (m_pBufArt)
		return m_pBufArt->GetdChatarras();
	else
		return 0;
}


CString 
COrdMaq::GetsMezcla() const
{
	if (!m_pBufArt) return "*NO ART*";
	if (m_pBufArt->m_pBufMezcla)
		return (m_pBufArt->m_pBufMezcla->m_sID);
	else
		return m_pBufArt->m_sMezcla;

}

CString 
COrdMaq::GetsPlanificador() const
{
	if (!m_pBufArt) return "*NO ART*";
	return m_pBufArt->m_sPlanificador;

}

CString 
COrdMaq::GetsMezclaKilos() const
{
	BOOL BMezcla = (GetpBufMezcla() != NULL);
	CString sMez = "**No Mezcla";
	if (BMezcla)
	{
		sMez = FormatLong(GetlCantMezcla(),7);
	} 	
	return sMez;
}


long COrdMaq::GetlLote()
{
	// Esta función no debe devolver 0 porque se usa en divisores
	long lLote = TK_LOTE_DEF;
	
	if (m_pBufArt->m_pBufMezcla && m_pBufArt->m_pBufMezcla->m_lLote > 0)
		lLote = m_pBufArt->m_pBufMezcla->m_lLote;
	
	return lLote;
}
                             
CString  
COrdMaq::GetsID() const
{
	if (m_pBufArt)
		return m_pBufArt->m_sID;
	else
		return "";
}

CString  
COrdMaq::GetsNombre() const
{
	if (m_pBufArt)
		return m_pBufArt->m_sID;
	else
		return "";
}

CString  
COrdMaq::GetsDescripcion() const
{
	if (m_pBufArt)
		return m_pBufArt->m_sID;
	else
		return "";
}


long  
COrdMaq::GetlGFH() const
{
	if (m_pPM)
		return m_pPM->GetlGFH();
	else
		return -1;
}

CString 
COrdMaq::GetsMolde() const
{
	if (m_pBufMolde)
		return m_pBufMolde->m_sID;
	else
		return "";

}

CString
COrdMaq::GetsCliente() const
{
	CBufOF* pOF = GetpOF();
	return  pOF ? pOF->m_sNomcli : "";
}

CString 
COrdMaq::GetsMoldeDesc() const
{
	if (!m_pBufMolde) return "";
	CString sMolde = m_pBufMolde->m_sID;
	/*
	if (m_pBufMolde->m_BCercoREP) sMolde += " [REP]";
	if (m_pBufMolde->m_BCanalFrio) sMolde += " [CF]";
	if (m_pBufMolde->m_BExtractorAutomatico) sMolde += " [EA]";
	if (m_pBufMolde->m_BExtractorMK19) sMolde += " [MK19]";
	if (m_pBufMolde->m_BDoblePiston) sMolde += " [2P]";
	*/
	return sMolde;

}
CString
COrdMaq::GetsMoldeInySop() const
{
	if (!m_pBufMolde) return "";
	CObArray* pArr = m_pBufMolde->m_pArrMolElem;
	if (!pArr) return "";
	CString str = "";
	for (int i = 0; i < pArr->GetSize(); i++)
	{
		CBufMoldesElem* pBufMI = (CBufMoldesElem*) pArr->GetAt(i);
		str += (str == "") ? _T("") : _T(", ");
		str += pBufMI->m_sNombre;
	}
	if (str != "") str = _T("(") + str + _T(")");
	return str;

}

CString  
COrdMaq::GetsMaquina() const
{
	if (m_pPM)
		return m_pPM->GetsID();
	else
		return "";
}

CString COrdMaq::GetsMaqNom() const
{
	if (m_pPM)
		return m_pPM->GetsNombre();
	else
		return "";

}

double  
COrdMaq::GetdInactividad() const
{
	if (m_pPM)
		return m_pPM->GetdInactividad();
	else
		return 0;
}


CTime
COrdMaq::GetFecEnt() const
{
	
/*
	ASSERT_VALID(m_pBufOF);
	return m_pBufOF->m_FecEnt;
*/
	return CTime::GetCurrentTime();
}
		
CString
COrdMaq::FormatLimit( long p_lGFH) const
{
	return "";
}

CString
COrdMaq::GetsInfo(bool p_bFull /* = false*/) const
{ 
	CString sTemp;
	ASSERT_VALID(m_pPM);
	CString sMolde = "";
	if (GetbConfMolde())
	{
		COrdMaq* pOMConf = GetpOMConfMolde();
		if (pOMConf)
			sMolde = " - Conf.Molde: " + pOMConf->GetsID() + " (" + pOMConf->GetsMaquina() + ")";
	}
	sTemp.Format( "%s (%s) [%s] %s - %s [%s]  Tir: %s %s",
					m_pPM->GetsID(),
					GetsMoldeDesc(),
					FormatLong((long) GetlCantTot(), 7),
					FormatFec(GetFecIni()),
					FormatFec(GetFecFin()),
					FormatFec(GetTimeSpan()),
					FormatLong(GetlTiradas(), 7),
					sMolde
					);
	if (p_bFull) sTemp += GetsMoldeInySop();
	return sTemp;
	
}


int
COrdMaq::PMIndex() const
{
	ASSERT_VALID(m_pPM);
	int iLim = m_pPM->GetNumOM();
	for(int i=0; i < iLim; i++)
	{     
		COrdMaq* pOM = m_pPM->GetpOM(i);
		if ( pOM == this ) return i;
	}                               
	return -1;
}

void
COrdMaq::ModifCant( const char* sCant )
{
	long lCant = atol( sCant );
	if (lCant == 0)
	{
		AfxMessageBox(STRID(IDS_ERR_CANTNOVAL ));
		return;
	}
	ModifCant(lCant);
	return;
}


void
COrdMaq::ModifCantChatarras( const char* sCant )
{
	long lCantChatarras = atol( sCant );
	if (lCantChatarras == 0)
	{
		AfxMessageBox(STRID(IDS_ERR_CANTNOVAL ));
		return;
	}
	ModifCantChatarras(lCantChatarras);
	return;
}

void
COrdMaq::ModifCantTiradas( const char* sCant )
{
	long lTiradas = atol( sCant );
	if (lTiradas == 0)
	{
		AfxMessageBox(STRID(IDS_ERR_CANTNOVAL ));
		return;
	}
	ModifCantTiradas(lTiradas);
	return;
}

void
COrdMaq::ModifPersonas( const char* sPersonas )
{
	double dPersonas = atof( sPersonas );
	if (dPersonas == 0.0)
	{
		AfxMessageBox(STRID(IDS_ERR_CANTNOVAL ));
		return;
	}          
//	m_pBufOF->SetPersonas(dPersonas);
	return;
}

void
COrdMaq::ModifTimeSpan( const char* sTimeSpan )
{
	CTimeSpan tsNew = StringToTs( sTimeSpan );
	if (tsNew.GetTotalSeconds() <= 0)
	{
		AfxMessageBox(STRID(IDS_ERR_CANTNOVAL ));
		return;
	}          
	UpdateTs(tsNew);
	return;
}

void
COrdMaq::ModifTimeSpanInact( const char* sTimeSpan )
{
	CTimeSpan tsNewInact = StringToTs( sTimeSpan );
	long lSecs = (long) tsNewInact.GetTotalSeconds();
	ModifTimeSpanInact(lSecs);
}

// Modifica el tiempo, ajustando la cantidad
void
COrdMaq::ModifTimeSpanInact( long p_lSecs )
{
	long lSecs = p_lSecs;
	if (lSecs <= 0)
	{
//		AfxMessageBox(STRID(IDS_ERR_CANTNOVAL ));
		return;
	}          
	long lSecSin = (long) ((double) lSecs / ( 1.0 + ( GetdInactividad() / 100.0 )));
	long lSecCon = (long) ((double) lSecSin * ( 1.0 + ( GetdInactividad() / 100.0 )));

	if (lSecCon > lSecs) lSecs--;
	else if (lSecCon < lSecs) lSecs++;

	UpdateTs((CTimeSpan) lSecSin);
	return;
}

// Actualiza el tiempo y sincroniza la cantidad
void
COrdMaq::UpdateTs(CTimeSpan p_ts)
{
	// p_ts tiene ya la inactividad descontada, por lo que calculamos la cantidad
	long lCant = CalclCantReal(p_ts);
	ModifCantChatarras(lCant);
	SetTs(p_ts);
}

void
COrdMaq::SetlCantidad(long p_lCant)
{
	m_lCant = p_lCant;

}
		
void
COrdMaq::ModifFecEnt( CTime p_FecEnt )
{
//	m_pBufOF->SetFecEnt(p_FecEnt);
}

BOOL
COrdMaq::First() const 
{
	int i = (m_pPM) ? m_pPM->Find(this) : INT_MAX;
	if (i == INT_MAX)
		return FALSE;
	else
		return (i == 0);
}

BOOL
COrdMaq::Last() const 
{
	int i = (m_pPM) ? m_pPM->Find(this) : INT_MAX;
	if (i == INT_MAX)
		return FALSE;
	else
		return (i == m_pPM->GetNumOM()-1);
}

COrdMaq*
COrdMaq::Prev() 		  
{
	int i = (m_pPM) ? m_pPM->Find(this) : INT_MAX;
	if (i == INT_MAX)
		return this;
	else
		if (i > 0)
			return m_pPM->GetpOM(i-1);
		else
			return this;
}

COrdMaq*
COrdMaq::Next() 
{
	int i = (m_pPM) ? m_pPM->Find(this) : INT_MAX;
	if (i == INT_MAX)
		return this;
	else
		if (i < (m_pPM->GetNumOM() - 1))
			return m_pPM->GetpOM(i+1);
		else
			return this;
}


CTime
COrdMaq::GetFecLim() const
{
	CTime FecLim;
	ASSERT_VALID(m_pPM);
	COrdMaq* pOMl = m_pPM->GetpProg()->GetPrevOM(this, TK_GFH_ACABADO);
	// Si no hay ordenes anteriores, la fecha límite es la de entrega
	if (!pOMl)
	{
		FecLim = AddDays(GetFecEnt(),1);
		//FecLim = CTime(FecLim.GetYear(), FecLim.GetMonth(), FecLim.GetDay(), TK_TURNO3_FIN, 0, 0);
		FecLim = CTurnos::GetFecIni(FecLim, 1);
		return FecLim;
	}
	// Si hay una OM anterior, la fecha límite es la de comienzo de esa
	// otra OM. Es decir, no se puede empezar un proceso mas tarde de que
	// este programado empezar su siguiente operacion.
	FecLim =  pOMl->GetFecIni();
	return FecLim;
}	


void
COrdMaq::SetRestoMezcla(long p_lCantResto)
{
	m_lRestoMezcla = (long) ((double) p_lCantResto * 1000 / GetdPeso());

	m_tsMezcla = CalcTsMezcla(p_lCantResto); 
	if (m_pPM)
	{
		m_FecMezcla = m_pPM->AddCal(GetFecIni(), m_tsMezcla);
	}
}


		
void 
COrdMaq::ModifCant(long p_lCant)
{
	SetlCantidad(p_lCant);
}

// Sincroniza la cantidad en el caso
void
COrdMaq::SyncCant()
{
	if (GetBPrevLink())
	{
		COrdMaq* pPrev = Prev();
		if (pPrev && pPrev != this && pPrev->GetBNextLink())
			pPrev->SetlCantidad(GetlCantidad());
	}
	else if (GetBNextLink())
	{
		COrdMaq* pNext = Next();
		if (pNext && pNext != this && pNext->GetBPrevLink())
			pNext->SetlCantidad(GetlCantidad());
	}

}

void 
COrdMaq::ModifCantChatarras(long p_lCant)
{

	m_lCant = GetlCantSinChatarras(p_lCant);	
}

void 
COrdMaq::ModifCantTiradas(long p_lCant)
{
	m_lCant = GetlCantTiradas(p_lCant);	
}

int
COrdMaq::ModifMolde(const char *p_sIDMolde)
{
	ASSERT_VALID(g_pCache);
	CBufMolde* pBufMolde = g_pCache->FindpMolde(p_sIDMolde);
	if (pBufMolde)
	{
		SetpBufMolde(pBufMolde);
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}

// Calcula y devuelve la cantidad de la orden ajustada a los lotes.
// TODO : Ajustar tambien a la existencias
long 
COrdMaq::CalcCantLote()
{
	// Ajustamos al lote superior
	long lLote = GetlLote();
	long lCant = GetlCantidad();
	if (lLote <= 0) return lCant;
	
	if (lCant % lLote) return ((lCant / lLote) + 1) * lLote;
	else return lCant;
}
// Devuelve una cadencia neta,es decir, las piezas reales que se fabrican por segundo,
// teniendo en cuenta inactividades y chatarras
double COrdMaq::GetdCadAbsPs() const
{
	return (GetdCadenciaGFH() * ( 1 - GetdInactividad() / 100 ) ) 
							* ( 1 - GetdChatarras() / 100 ) ;
}

BOOL 
COrdMaq::Overlaps(COrdMaq *p_pOM)
{
	return m_Abs.Overlaps(p_pOM->m_Abs);
}

void 
COrdMaq::RecalcTL()
{
	CProg* pProg = m_pPM->GetpProg();
	if (!pProg) return;

	CTimeLine* pTL = pProg->GetpTL(GetpBufArticulo(), Piezas);
	if (pTL) pTL->Recalc(Piezas);
	pTL = pProg->GetpTL(GetpBufArticulo(), Mezcla);
	if (pTL) pTL->Recalc(Mezcla);
	pTL = pProg->GetpTL(GetpBufArticulo(), Margen);
	if (pTL) pTL->Recalc(Margen);
}

void 
COrdMaq::RecalcFin()
{
	if (m_pPM)
	{
		m_pPM->Recalc();
		if (GetlGFH() != TK_GFH_INYECCION) return;
		CProg* pProg = m_pPM->GetpProg();
		if (pProg)
		{
			int iOM;
			for ( iOM = 0; (iOM < m_pPM->GetNumOM()) && (m_pPM->GetpOM(iOM) != this); iOM++)
				;
			for ( ; iOM < m_pPM->GetNumOM(); iOM++)
			{
				COrdMaq* pOM = m_pPM->GetpOM(iOM);
				pOM->RecalcTL();
				g_pCache->RecalcOFs(pProg, pOM->GetsID());
				pProg->CalcMezclasTL(pOM->GetsMezcla());
				pProg->CheckConfOM(pOM);
			}
		}
	}
	
}

long 
COrdMaq::GetlCantMezcla() const
{
	if (!m_pBufArt) return 0;
	else return m_pBufArt->GetlCantMezcla();

}

void
COrdMaq::SetbConfMolde(BOOL p_bConfMolde, COrdMaq* p_pOM)		
{
	m_bConfMolde = p_bConfMolde;
	if (p_bConfMolde)
	{
		m_SetErr.Add(TK_OMERR_CONFMOLDE);
		m_pOMConf = p_pOM;
	}
	else
	{
		m_SetErr.Remove(TK_OMERR_CONFMOLDE);
		m_pOMConf = NULL;
	}
}

void
COrdMaq::SetbConfMoldeElem(BOOL p_bConfMoldeElem, CObArray& arrOMConfMoldeElem)
{
	m_bConfMoldeElem = p_bConfMoldeElem;
	if (p_bConfMoldeElem)
	{
		m_SetErr.Add(TK_OMERR_CONFMOLDEELEM);
		m_arrOMConfMoldeElem.RemoveAll();
		m_arrOMConfMoldeElem.Copy( arrOMConfMoldeElem);
	}
	else
	{
		m_SetErr.Remove(TK_OMERR_CONFMOLDEELEM);
		m_arrOMConfMoldeElem.RemoveAll();
	}
}


CObArray*
COrdMaq::GetpArrMolElem()
{
	if (m_pBufMolde)
	{
		return m_pBufMolde->m_pArrMolElem;
	}
	else
		return NULL;
}

long
COrdMaq::GetlNecMezcla(BOOL p_bRoundUp, BOOL p_bKilos)
{
	long lNecMezcla = 0;
	
	lNecMezcla = (long) ((double)GetlCantidad() * GetdPeso());
	
	if (p_bRoundUp && GetlCantidad() > 0)
	{
		lNecMezcla = (( lNecMezcla / (GetlLote() * 1000) ) + 1) * GetlLote();
		if (!p_bKilos) lNecMezcla *= 1000;
	} else
	{
		if (p_bKilos)
		{
			lNecMezcla = (lNecMezcla / 1000) + ( (lNecMezcla % 1000 ) ? 1 : 0);
		}
	}

	return lNecMezcla;
}

int
COrdMaq::GetArrInacts(CObArray &p_ArrInact) const
{
	p_ArrInact.RemoveAll();	// No hace falta dealocar los elementos, pues son referencias a las Inacts de PM
	CProgMaq* pPM = GetpPM();
	if (!pPM) return 0;
	for (int i = 0; i < pPM->GetNumInacts(); i++)
	{
		CAbSpan* pAbSpanInact = pPM->GetpInact(i);
		if (pAbSpanInact->GetFecIni() >= GetFecFin()) break;
		if (pAbSpanInact->GetFecFin() <= GetFecIni()) continue;
		p_ArrInact.Add(pAbSpanInact);
	}
	return p_ArrInact.GetSize();
}	

BOOL
COrdMaq::TieneInacts() const
{
	CProgMaq* pPM = GetpPM();
	if (!pPM) return FALSE;
	return pPM->HayInactEn(m_Abs);
}

int 
COrdMaq::GetDiasRetorno()
{
	CProgMaq* pPM = GetpPM();
	if (!pPM) return FALSE;
	return pPM->GetDiasRetorno();
	
}

void 
COrdMaq::Delay()
{
	int iDays = GetDiasRetorno();
	m_FecIniMargen = AddDays(GetFecIni(), iDays);
	m_FecFinMargen = AddDays(GetFecFin(), iDays);
}

