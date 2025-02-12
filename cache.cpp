// cache.cpp


#include "stdafx.h"

#include "cpw.h"
#include "cpwDefs.h"
#include "AbSpan.h"
#include "cpwdb.h"
#include "cpwExt.h"
#include "prog.h"

#include "cache.h"
#include "ProgMaq.h"
#include "ObArrOrd.h" 
#include <strstream>
#include "OrdMaq.h"
#include "DlgDebug.h"
#include "TimeLine.h"
#include "Turnos.h"

	
                         
                         
CCache::CCache(CDatabase* miDb)
	: m_RsArticulo(miDb)
	, m_RsAcabado(miDb)
	, m_RsMaquinas(miDb)
	, m_RsCliente(miDb)
	, m_RsEntregas(miDb)
	, m_RsArtMol(miDb)
	, m_RsHistorico(miDb)
	, m_RsCalendario(miDb)
	, m_RsLocales(miDb)
	, m_RsMezclas(miDb)
	, m_RsMoldes(miDb)
	, m_RsMolMaq(miDb)
	, m_RsListadoValorado(miDb)
	, m_RsCambiosMoldes(miDb)
	, m_RsMezclasPlanta(miDb)
	, m_RsLisMezclas(miDb)
	, m_RsLisGeneral(miDb)
	, m_RsComponentes(miDb)
	, m_RsMoldes_Soplado(miDb)
	, m_RsMoldes_Inyectora(miDb)
	
{   
	m_pDB = miDb;
	SetsConnPedidos(miDb->GetConnect(), true);	// Por defecto los pedidos se cogen del mismo sitio que las dem�s tablas
	SetBConnPedidosIsDefault(false);	// Todav�a no est�n cargados, por tanto no tienen la BD de pedidos por defecto
	
	// Abrimos las tablas
	
	m_RsCliente.Open(CRecordset::dynaset);
	m_RsEntregas.Open(CRecordset::dynaset);
	m_RsArtMol.Open(CRecordset::dynaset);
	m_RsMaquinas.Open(CRecordset::dynaset);
	m_RsAcabado.Open(CRecordset::dynaset);
	m_RsComponentes.Open(CRecordset::dynaset);
	m_RsHistorico.Open(CRecordset::dynaset);
	m_RsMezclas.Open(CRecordset::dynaset);
	m_RsMoldes.Open(CRecordset::dynaset);
	m_RsMolMaq.Open(CRecordset::dynaset);
	m_RsListadoValorado.Open(CRecordset::dynaset);
	m_RsCambiosMoldes.Open(CRecordset::dynaset);
	m_RsMezclasPlanta.Open(CRecordset::dynaset);
	m_RsArticulo.Open(CRecordset::dynaset);

	// ( No se pueden abrir aqu� los pedidos porque necesitan el valor de m_sConnPedidos y como estamos en 
	//	 el constructor, pues no se puede obtener )

	// Inicializamos los tama�os de los Arrays  
    
    m_pArrLocales = new CObArray;
	m_pArrLocales->SetSize(0, TK_CACHE_LOCALES);
    m_pArrArticulos = new CObArray;
	m_pArrArticulos->SetSize(0, TK_CACHE_ARTIC);
    m_pArrMolMaq = new CObArray;
	m_pArrMolMaq->SetSize(0, TK_CACHE_MOLDES*5);
    m_pArrMoldes = new CObArray;
	m_pArrMoldes->SetSize(0, TK_CACHE_MOLDES);
	m_pArrMaquinas = new CObArray;
	m_pArrMaquinas->SetSize(0, TK_CACHE_MAQ);
    m_pArrOFs = new CObArray;
	m_pArrOFs->SetSize(0, TK_CACHE_ORDMAQ);
	m_pArrMezclas = new CObArray;
	m_pArrMezclas->SetSize(0, TK_CACHE_MEZCLAS);
	m_pArrOFComb = new CObArrayOrd(&OFComb_Bigger , &OFComb_Equal);
	m_pArrOFSeccion = new CObArrayOrd(&OFSeccion_Bigger , &OFSeccion_Equal);
	m_pArrOFSeccion->SetSize(0, TK_CACHE_ORDMAQ);
	m_pArrOFSel = new CObArray();
	m_pArrOFSel->SetSize(0, TK_CACHE_ORDMAQ);
	m_pArrOFAcum = new CObArray();
	m_pArrOFAcum->SetSize(0, TK_CACHE_ORDMAQ);
	m_pArrOFAyer = new CObArray();
	m_pArrOFAyer->SetSize(0, TK_CACHE_ORDMAQ);
	m_pArrCambMold = new CObArray();
	m_pArrCambMold->SetSize(0, TK_CACHE_CAMBMOLD);
	m_pArrHomologadas = new CObArray;
	m_pArrHomologadas->SetSize(0, 500);


	IniFillCache();

}

void  
CCache::IniFillCache(BOOL p_BRecarga)
{
// Llenamos el cache
	IniFillArticulos();
	IniFillLocales();
	IniFillMaquinas();
	IniFillMolMaq();
	IniFillOFs();
	IniFillOFsAyer();
	IniFillMezclas();
	IniFillHomologadas();

	LinkPointer(p_BRecarga);

}


void
CCache::EmptyArrays()
{
	ClearArray(*m_pArrLocales, RUNTIME_CLASS(CBufLocal));
	ClearArray(*m_pArrArticulos, RUNTIME_CLASS(CBufArticulo));
	ClearArray(*m_pArrMolMaq, RUNTIME_CLASS(CBufMolMaq));
	ClearArray(*m_pArrMoldes, RUNTIME_CLASS(CBufMolde));
	ClearArray(*m_pArrMaquinas, RUNTIME_CLASS(CBufMaquina));
	ClearArray(*m_pArrMezclas, RUNTIME_CLASS(CBufMezcla));
	ClearArray(*m_pArrOFComb, RUNTIME_CLASS(COFComb));

	m_pArrOFSeccion->RemoveAll();
	m_pArrOFSel->RemoveAll();
	m_pArrOFAcum->RemoveAll();

	ClearArray(*m_pArrOFs, RUNTIME_CLASS(CBufOF));

	ClearArray(*m_pArrOFAyer, RUNTIME_CLASS(CBufOF));
	ClearArray(*m_pArrCambMold, RUNTIME_CLASS(CBufCambiosMoldes));
	ClearArray(*m_pArrHomologadas, RUNTIME_CLASS(CBufHomologada));

}
	
CCache::~CCache()
{
	m_RsArticulo.Close();
	m_RsMaquinas.Close();
	m_RsCliente.Close();
	m_RsEntregas.Close();
	m_RsArtMol.Close();
	m_RsAcabado.Close();
	m_RsComponentes.Close();
	m_RsHistorico.Close();
	m_RsMezclas.Close();
	m_RsMoldes.Close();
	m_RsMolMaq.Close();
	m_RsListadoValorado.Close();
	m_RsCambiosMoldes.Close();
	m_RsMezclasPlanta.Close();
 
	EmptyArrays();

    delete m_pArrArticulos;
	delete m_pArrLocales;
	delete m_pArrMoldes;
	delete m_pArrMolMaq;
	delete m_pArrMaquinas;
	delete m_pArrMezclas;
	delete m_pArrOFComb;
	delete m_pArrOFSeccion;	
	delete m_pArrOFSel;		
	delete m_pArrOFAcum;		
    delete m_pArrOFs;
    delete m_pArrOFAyer;
	delete m_pArrCambMold;
	delete m_pArrHomologadas;

}          
               
CObArray*
CCache::GetArrBufArt()
{	  
	return m_pArrArticulos;
}

CObArray*
CCache::GetArrBufMezcla()
{	  
	return m_pArrMezclas;
}

void 
CCache::SerializeOFs(CArchive& ar)
{
	m_pArrOFs->Serialize(ar);
}


int 
CCache::GetNumOF()
{
	if (g_VerOF.m_bAcumOFs)
	{
		return m_pArrOFAcum->GetSize();
	} 
	else if (g_VerOF.Selec())
	{
		return m_pArrOFSel->GetSize();
	}
	else // No importa por qu� est� ordenado, el n�mero ser� el mismo
	{
		return m_pArrOFs->GetSize();
	}
}


CBufOF* 
CCache::GetpOFAt( int p_iInd )
{
	
	if (g_VerOF.m_bAcumOFs)
	{
		return (CBufOF*) m_pArrOFAcum->GetAt(p_iInd);
	} 
	else if (g_VerOF.Selec())
	{
		return (CBufOF*) (m_pArrOFSel->GetAt(p_iInd));
	}
	else if (g_VerOF.m_OrdenOF == Seccion)
	{	
		return (CBufOF*) (m_pArrOFSeccion->GetAt(p_iInd));
	}
	else
	{
		return (CBufOF*) (m_pArrOFs->GetAt(p_iInd));
	}
}



void
CCache::AssignInacts(CProgMaq& p_PM)
{
	// Funci�n para introducir los datos de calendario, en forma de Inactividades

	BOOL BHasRecords = FALSE;
	CString sFilter;
    //CString sDeb;
	
	sFilter.Format( "Fecha >= #%s# AND Fecha <= #%s# AND ID = '%s'"
					, FormatFec (p_PM.GetFecIni(), FechaDB) 
					, FormatFec (p_PM.GetFecFin(), FechaDB)
					, p_PM.GetsID() );
    m_RsCalendario.m_strFilter = sFilter; 
	m_RsCalendario.m_strSort = "ID,Fecha";
	
	if (m_RsCalendario.IsOpen()) m_RsCalendario.Close();
	if (m_RsCalendario.Open(CRecordset::dynaset))
		if (m_RsCalendario.GetRecordCount())
		{
			m_RsCalendario.MoveFirst();
			BHasRecords = TRUE;
		}

		 
	// Primero eliminamos las posibles inactividades que ya hubiese
	if (p_PM.m_pArrInacts)
	{
		ClearArray(*(p_PM.m_pArrInacts), RUNTIME_CLASS(CAbSpan));
		delete p_PM.m_pArrInacts;	
		p_PM.m_pArrInacts = NULL;
	}

	if (!p_PM.m_pArrInacts) // A�adido para evitar dobles definiciones
	{
		p_PM.m_pArrInacts = new CObArray();
		p_PM.m_pArrInacts->SetSize(0, TK_CACHE_INACT);
	}

	BOOL EnInact = FALSE;	// Presuponemos que partimos de situacion de
							// actividad

	CTime FecI = p_PM.GetFecIni();

	CAbSpan* pAbSpan = NULL;

	while ( FecI < p_PM.GetFecFin() || IsSameDay(FecI, p_PM.GetFecFin()) )
	{ 
		BOOL Turnos[TK_NUM_TURNOS_MAX];
		if ( BHasRecords && IsSameDay( FecI, m_RsCalendario.m_Fecha ) )
		{
			Turnos[0] = m_RsCalendario.m_Turno1;
			Turnos[1] = m_RsCalendario.m_Turno2;
			Turnos[2] = m_RsCalendario.m_Turno3;
		} else		// Valor por defecto
		{
			GetDefaultTurnos(FecI, Turnos);
		}
		for (int i = 0; i < CTurnos::GetiNumTurnos(); i++)
		{
			if (Turnos[i])	// Si el turno es activo
			{
				if (EnInact)	// Si est�bamos en inactividad
				{
					EnInact = FALSE;		// Cambiamos a actividad
					// Ponemos la fecha de fin dependiendo del turno.
					/*pAbSpan->SetFecFin(	CTime ( FecI.GetYear(),
												FecI.GetMonth(),
												FecI.GetDay(),
										( i == 0 ) ? TK_TURNO3_FIN : ( i == 1) ? TK_TURNO1_FIN: TK_TURNO2_FIN,
												0,
												0 ));
					*/
					pAbSpan->SetFecFin( CTurnos::GetFecIni(FecI, i+1));
					  // Cerramos la inactividad y la guardamos
					p_PM.m_pArrInacts->Add(pAbSpan);
				}
			}
			else	// Si el turno es inactivo
			{
				if (!EnInact)	// Si no est�bamos en inactividad
				{
					EnInact = TRUE;		//Cambiamos a inactividad
					pAbSpan = new CAbSpan;  // Creamos una nueva inactividad
					/*pAbSpan->SetFecIni(	CTime(  FecI.GetYear(),
												FecI.GetMonth(),
												FecI.GetDay(),
										( i == 0 ) ? TK_TURNO3_FIN : ( i == 1) ? TK_TURNO1_FIN: TK_TURNO2_FIN,
												0,
												0 ));
					*/
					pAbSpan->SetFecIni( CTurnos::GetFecIni(FecI, i+1));
					
				}
			}
		}	// End For
		if ( BHasRecords && IsSameDay( FecI, m_RsCalendario.m_Fecha ) )
		{
			m_RsCalendario.MoveNext();
			if (m_RsCalendario.IsEOF()) BHasRecords = FALSE;
		}
		FecI = AddDays(FecI, 1);
		//sDeb = FormatFec(FecI, Fecha);
		
	}	// End While
	if (EnInact)		// Si tenemos una inactividad sin cerrar
	{
		// La cerramos con una fecha adecuada
		pAbSpan->SetFecFin(	/*CTime( FecI.GetYear(), FecI.GetMonth(), FecI.GetDay(), TK_TURNO3_FIN, 0, 0 )*/ CTurnos::GetFecIni(FecI, 1));
		// Y la guardamos
		p_PM.m_pArrInacts->Add(pAbSpan);
	}

}


// GetArrMaquinas : Nos da un Array de * a Strings, vac�o, y lo llenamos con 
// los id de m�quinas programables en orden de programa. El recordset de m�quinas est�
// preparado para acceso directo, por lo que hay que cerrarlo y abrirlo para
// poder acceder secuencialmente. Esto no est� documentado pero parece que
// funciona. Adem�s se comprueba que los nombres de dos m�quinas cargadas no sean
// iguales, porque a veces se busca por nombre
int 
CCache::GetArrMaquinas( CStringArray& p_ArrMaquinas, long p_lGFH)
{
    int NumMaq = 0;
	CStringArray ArrNomMaq;
	int bUniqName = FALSE;
	m_RsMaquinas.Close();
	m_RsMaquinas.m_strFilter = "";
	m_RsMaquinas.m_strSort = "GFH, Orden, ID";		// Orden de la consulta

	if (m_RsMaquinas.Open(CRecordset::dynaset))
	{
		m_RsMaquinas.MoveFirst();
 		
		while (!m_RsMaquinas.IsEOF())
		{
			if (m_RsMaquinas.m_Programable && (p_lGFH == -1 || p_lGFH == m_RsMaquinas.m_GFH))
			{
				
				bUniqName = TRUE;	
				for (int i = 0; i< NumMaq; i++)
				{
					CString sDesc = ArrNomMaq.GetAt(i);
					if (sDesc == m_RsMaquinas.m_Nombre)
					{
						bUniqName = FALSE;
						break;
					}
				}
				if (!bUniqName)
				{
					CString sForm = STRID( IDS_ERR_NOMAQNOM );
					
					CString sMsg =  sForm + m_RsMaquinas.m_Nombre + ").";
					AfxMessageBox(sMsg);
				}
				else
				{
					p_ArrMaquinas.Add(m_RsMaquinas.m_ID);
					ArrNomMaq.Add(m_RsMaquinas.m_Nombre);
					NumMaq++;                           
				}
			}
			m_RsMaquinas.MoveNext();
		}
	}            
	m_RsMaquinas.Close();
	m_RsMaquinas.m_strFilter = "";
	m_RsMaquinas.Open(CRecordset::dynaset);
	return NumMaq;
}

// GetArrOFs : Nos da un Array de punteros a objetos y lo llenamos con las OFs.

int 
CCache::GetArrOFs( CObArray& p_ArrOFs )
{
	for (int i = 0; i < m_pArrOFs->GetSize(); i++)
	{
		p_ArrOFs.Add( m_pArrOFs->GetAt(i));
	}

	return m_pArrOFs->GetSize();
}

IMPLEMENT_SERIAL(CComp, CObject, 1)    

BOOL 
CComp::operator <(const CComp& p_Comp) const
{
	if ( m_sIDArticulo < p_Comp.m_sIDArticulo || 
		 ( m_sIDArticulo == p_Comp.m_sIDArticulo && m_FecNec < p_Comp.m_FecNec ) )
		return TRUE;
			
	return FALSE;					
}

BOOL 
CComp::operator ==(const CComp& p_Comp) const
{
	if ( m_sIDArticulo == p_Comp.m_sIDArticulo && m_FecNec == p_Comp.m_FecNec )
		return TRUE;
			
	return FALSE;					
}

CComp::CComp(CBufOF* pBufOF)
{
	m_sIDArticulo = pBufOF->m_sIDArticulo;
	m_sIDCliente = pBufOF->m_sNomcli;
	m_FecNec = pBufOF->m_FecNec;
	m_dCantAyer = pBufOF->m_dNoProg;
	m_dCantHoy = 0;
	m_lSeccion = pBufOF->m_lSeccion;
}

CComp::CComp(CComp* pComp)
{
	if (pComp)
	{
		m_sIDArticulo = pComp->m_sIDArticulo;
		m_sIDCliente = pComp->m_sIDCliente;
		m_FecNec = pComp->m_FecNec;
		m_dCantAyer = pComp->m_dCantAyer;
		m_dCantHoy = pComp->m_dCantHoy;
		m_lSeccion = pComp->m_lSeccion;
	}
}

void CCache::GetArrAcum(CObArray *p_pArrOF, CObArray *p_pArrAcum)
{
	p_pArrAcum->RemoveAll();
	int iLim = p_pArrOF->GetSize();
	CBufOF* pBufOF = (CBufOF*) p_pArrOF->GetAt(0);
	CString sIDAnt = pBufOF->m_sIDArticulo;
	CTime FecAnt = pBufOF->m_FecNec;
	CComp* pComp = new CComp(pBufOF);
	p_pArrAcum->Add(pComp);
	for(int i=1; i < iLim; i++)
	{
		pBufOF = (CBufOF*) p_pArrOF->GetAt(i);
		if (pBufOF->m_sIDArticulo == sIDAnt && pBufOF->m_FecNec == FecAnt)
		{
			pComp->m_dCantAyer = pBufOF->m_dNoProg;
		} else
		{
			pComp = new CComp(pBufOF);
			sIDAnt = pBufOF->m_sIDArticulo;
			FecAnt = pBufOF->m_FecNec;
			p_pArrAcum->Add(pComp);
		}
	}
}

// GetArrConsOF : Nos da un Array de punteros a objetos y lo llenamos con las OFs
// que resultan de comparar el array de OFs de hoy con el de ayer.

int 
CCache::GetArrConsOF( CObArray& p_ArrComp )
{
	
	p_ArrComp.RemoveAll();
	
	// Hacemos dos nuevos arrays, con los acumulados diarios.
	CObArray ArrHoy;
	CObArray ArrAyer;
	
	GetArrAcum(m_pArrOFs, &ArrHoy);
	GetArrAcum(m_pArrOFAyer, &ArrAyer);
	
	int iLimHoy = ArrHoy.GetSize();
	int iLimAyer = ArrAyer.GetSize();
	int j = 0;
	BOOL bFinHoy = (j >= iLimHoy);
	for (int i=0; i < iLimAyer; i++)
	{
		bFinHoy = (j >= iLimHoy);
		CComp* pCompAyer = (CComp*) ArrAyer.GetAt(i);
		CComp* pCompHoy = (bFinHoy) ? NULL : (CComp*) ArrHoy.GetAt(j);
		if (bFinHoy || (*pCompAyer) < (*pCompHoy))  // Si es menor, es que ese dia no est� hoy
		{
			if (pCompAyer->m_dCantAyer > 0)
			{
				CComp* pComp = new CComp(pCompAyer);
				p_ArrComp.Add(pComp);
			}
		} else if ((*pCompAyer) == (*pCompHoy))
		{
			if (pCompAyer->m_dCantAyer != pCompHoy->m_dCantAyer && 
				( pCompHoy->m_dCantAyer > 0 || pCompAyer->m_dCantAyer > 0) )
			{
				CComp* pComp = new CComp(pCompAyer);
				pComp->m_dCantHoy = pCompHoy->m_dCantAyer;
				p_ArrComp.Add(pComp);
			}
			j++;
		} else // Es que es mayor
		{
			if (pCompHoy->m_dCantAyer > 0)
			{
				CComp* pComp = new CComp(pCompHoy);
				pComp->m_dCantHoy = pCompHoy->m_dCantAyer;
				pComp->m_dCantAyer = 0;
				p_ArrComp.Add(pComp);
			}
			j++;
			i--;
		}
	}
	for ( ; j < iLimHoy; j++)
	{
		CComp* pCompHoy = (bFinHoy) ? NULL : (CComp*) ArrHoy.GetAt(j);
		if (pCompHoy->m_dCantAyer > 0)
		{
			CComp* pComp = new CComp(pCompHoy);
			pComp->m_dCantHoy = pCompHoy->m_dCantAyer;
			pComp->m_dCantAyer = 0;
			p_ArrComp.Add(pComp);
		}
		
	}
    for( int i=0; i < ArrAyer.GetSize() ; i++ )
    {
		CComp* pComp = (CComp*) ArrAyer.GetAt(i);
		delete pComp;    	
    }      
	for( int i=0; i < ArrHoy.GetSize() ; i++ )
    {
		CComp* pComp = (CComp*) ArrHoy.GetAt(i);
		delete pComp;    	
    }      
	
	
	return p_ArrComp.GetSize();
}

// GetArrGFHs : Nos da un Array de * a Strings, vac�o, y lo llenamos con 
// los GFH programables en orden de programa. El recordset de GFHs est�
// preparado para acceso directo, por lo que hay que cerrarlo y abrirlo para
// poder acceder secuencialmente. Esto no est� documentado pero parece que
// funciona.
/*
int 
CCache::GetArrGFHs( CStringArray& p_ArrGFHs )
{

  int NumGFH = 0;
  m_RsGFH.Close();
  m_RsGFH.m_strFilter = "";
  m_RsGFH.m_strSort = "ID";
  if (m_RsGFH.Open())
  {
		m_RsGFH.MoveFirst();
		
		  while (!m_RsGFH.IsEOF())
		  {
		  p_ArrGFHs.Add(m_RsGFH.m_ID);
		  NumGFH++;                           
		  m_RsGFH.MoveNext();
		  } 
		  }            
		  m_RsGFH.Close();
		  m_RsGFH.m_strFilter = "";
		  m_RsGFH.Open();       
		  return NumGFH;
		  }
*/

// GetArrOrdBloques : Nos da un Array Ordenado de * a Objetos, vac�o, y lo llenamos con 
// el Bloque indicado, desde la serie hasta la serie indicadas. El recordset de Bloques
// est� cerrado normalmente, as� que hay que abrirlo. El objeto utilizado para llenar el
// Array ser� CBufOFs
/*int
CCache::GetArrOrdBlq( CObArrayOrd& p_aoBloques , int p_iBloque, int p_iDesSer, int p_iHasSer)
{
	return 0;
}
*/

void
CCache::IniFillArticulos()
{
	m_RsArticulo.m_strFilter = "";
	m_RsArticulo.m_strSort = "ID";
	
	m_RsListadoValorado.m_strFilter = "";
	m_RsListadoValorado.m_strSort = "Articulo";

	// TODO : Ordenar tambien por campo Orden
	
	m_RsArtMol.m_strFilter = ""; 
	m_RsArtMol.m_strSort = "IDArticulo, Orden";

	m_RsMoldes.m_strFilter = "";
	m_RsMoldes.m_strSort = "ID";
                                
	m_RsMolMaq.m_strFilter = "";
	m_RsMolMaq.m_strSort = "IDMolde";
	m_RsMoldes_Soplado.m_strFilter = "";
	m_RsMoldes_Soplado.m_strSort = "IDMolde";
	m_RsMoldes_Inyectora.m_strFilter = "";
	m_RsMoldes_Inyectora.m_strSort = "IDMolde";

	m_RsAcabado.m_strFilter = ""; 
	m_RsAcabado.m_strSort = "articulo, operacion, centro";

	m_RsComponentes.m_strFilter = "";
	m_RsComponentes.m_strSort = "Articulo, Componente";

	if (m_RsArticulo.IsOpen()) m_RsArticulo.Close();
	if (m_RsListadoValorado.IsOpen()) m_RsListadoValorado.Close();
	if (m_RsArtMol.IsOpen()) m_RsArtMol.Close();
	if (m_RsAcabado.IsOpen()) m_RsAcabado.Close();
	if (m_RsMoldes.IsOpen()) m_RsMoldes.Close();
	if (m_RsMolMaq.IsOpen()) m_RsMolMaq.Close();
	if (m_RsComponentes.IsOpen()) m_RsComponentes.Close();
	if (m_RsMoldes_Soplado.IsOpen()) m_RsMoldes_Soplado.Close();
	if (m_RsMoldes_Inyectora.IsOpen()) m_RsMoldes_Inyectora.Close();

	if (!m_RsArticulo.Open(CRecordset::dynaset)) return;
	if (!m_RsListadoValorado.Open(CRecordset::dynaset)) return;
	if (!m_RsArtMol.Open(CRecordset::dynaset)) return;
	if (!m_RsAcabado.Open(CRecordset::dynaset)) return;
	if (!m_RsComponentes.Open(CRecordset::dynaset)) return;
	if (!m_RsMoldes.Open(CRecordset::dynaset)) return;
	if (!m_RsMolMaq.Open(CRecordset::dynaset)) return;
	if (!m_RsMoldes_Soplado.Open(CRecordset::dynaset)) return;
	if (!m_RsMoldes_Inyectora.Open(CRecordset::dynaset)) return;
	
	
	if (m_RsArticulo.GetRecordCount()) m_RsArticulo.MoveFirst();
	if (m_RsListadoValorado.GetRecordCount()) m_RsListadoValorado.MoveFirst();
	if (m_RsArtMol.GetRecordCount()) m_RsArtMol.MoveFirst();
	if (m_RsAcabado.GetRecordCount()) m_RsAcabado.MoveFirst();
	if (m_RsComponentes.GetRecordCount()) m_RsComponentes.MoveFirst();
	if (m_RsMoldes.GetRecordCount()) m_RsMoldes.MoveFirst();
	if (m_RsMolMaq.GetRecordCount()) m_RsMolMaq.MoveFirst();
	if (m_RsMoldes_Soplado.GetRecordCount()) m_RsMoldes_Soplado.MoveFirst();
	if (m_RsMoldes_Inyectora.GetRecordCount()) m_RsMoldes_Inyectora.MoveFirst();

	// A�adimos el art�culo vac�o de Paradas.
	CBufArticulo* pNewArticulo = new CBufArticulo();
	pNewArticulo->m_dPCAct = 0;
	pNewArticulo->m_dPHAct = 1;
	pNewArticulo->m_sID = pNewArticulo->m_sMezcla = pNewArticulo->m_sPlanificador = "";
	m_RsAcabado.m_articulo.TrimRight();
	m_RsComponentes.m_articulo.TrimRight();
	m_pArrArticulos->Add(pNewArticulo);
	while (!m_RsArticulo.IsEOF())
	{   
		if (TRUE)
		{
			CBufArticulo* pNewArticulo = new CBufArticulo(m_RsArticulo);
			//if(pNewArticulo->m_sID == "01911207Z")
			//	int iDebug = 1;
			while (!m_RsArtMol.IsEOF() && m_RsArticulo.m_ID > m_RsArtMol.m_IDArticulo) m_RsArtMol.MoveNext();
			if (m_RsArtMol.IsEOF()) m_RsArtMol.m_IDArticulo = "";
			
			while (!m_RsArtMol.IsEOF() && m_RsArticulo.m_ID == m_RsArtMol.m_IDArticulo)
			{
				CBufArtMol* pNewArtMol = new CBufArtMol(m_RsArtMol);
				ASSERT_VALID(pNewArticulo->m_pArrArtMol);
				pNewArticulo->m_pArrArtMol->Add(pNewArtMol);   
				m_RsArtMol.MoveNext();
			}
			
			while (!m_RsAcabado.IsEOF() && m_RsArticulo.m_ID > m_RsAcabado.m_articulo) m_RsAcabado.MoveNext();
			if (m_RsAcabado.IsEOF()) m_RsAcabado.m_articulo = "";
			
			while (!m_RsAcabado.IsEOF() && m_RsArticulo.m_ID == m_RsAcabado.m_articulo)
			{
				m_RsAcabado.m_articulo.TrimRight();
				CBufAcabado* pNewAcabado = new CBufAcabado(m_RsAcabado);
				ASSERT_VALID(pNewArticulo->m_pArrAcabado);
				pNewArticulo->m_pArrAcabado->Add(pNewAcabado);   
				m_RsAcabado.MoveNext();
			}

			while (!m_RsComponentes.IsEOF() && m_RsArticulo.m_ID > m_RsComponentes.m_articulo) m_RsComponentes.MoveNext();
			if (m_RsComponentes.IsEOF()) m_RsComponentes.m_articulo = "";

			while (!m_RsComponentes.IsEOF() && m_RsArticulo.m_ID == m_RsComponentes.m_articulo)
			{
				m_RsComponentes.m_articulo.TrimRight();
				CBufComponentes* pNewComponentes = new CBufComponentes(m_RsComponentes);
				ASSERT_VALID(pNewArticulo->m_pArrComponentes);
				pNewArticulo->m_pArrComponentes->Add(pNewComponentes);
				m_RsComponentes.MoveNext();
			}

			//while (!m_RsListadoValorado.IsEOF() && m_RsArticulo.m_ID > m_RsListadoValorado.m_Articulo.Left(TK_SIZE_CODART)) 
			while (!m_RsListadoValorado.IsEOF() && m_RsArticulo.m_ID > m_RsListadoValorado.m_Articulo) 
				m_RsListadoValorado.MoveNext();
			
			//if (!m_RsListadoValorado.IsEOF() && m_RsArticulo.m_ID == m_RsListadoValorado.m_Articulo.Left(TK_SIZE_CODART)) 
			if (!m_RsListadoValorado.IsEOF() && m_RsArticulo.m_ID == m_RsListadoValorado.m_Articulo) 
			{
				
				if (m_RsListadoValorado.IsFieldNull(&m_RsListadoValorado.m_P_H_real ))  pNewArticulo->m_LV_dP_H_real = 0;
				else pNewArticulo->m_LV_dP_H_real = m_RsListadoValorado.m_P_H_real;
				if (m_RsListadoValorado.IsFieldNull(&m_RsListadoValorado.m__Rechazos ))  pNewArticulo->m_LV_dRechazos = 0;
				else pNewArticulo->m_LV_dRechazos = m_RsListadoValorado.m__Rechazos;
				if (m_RsListadoValorado.IsFieldNull(&m_RsListadoValorado.m_Total_Horas ))  pNewArticulo->m_LV_lTotal_Horas = 0;
				else pNewArticulo->m_LV_lTotal_Horas = m_RsListadoValorado.m_Total_Horas;
				if (m_RsListadoValorado.IsFieldNull(&m_RsListadoValorado.m_Cantidad_Fabricada ))  pNewArticulo->m_LV_lCantidad_Fabricada = 0;
				else pNewArticulo->m_LV_lCantidad_Fabricada = m_RsListadoValorado.m_Cantidad_Fabricada;
				if (m_RsListadoValorado.IsFieldNull(&m_RsListadoValorado.m_Cantidad_Rechazada ))  pNewArticulo->m_LV_lCantidad_Rechazada = 0;
				else pNewArticulo->m_LV_lCantidad_Rechazada = m_RsListadoValorado.m_Cantidad_Rechazada;
				if (m_RsListadoValorado.IsFieldNull(&m_RsListadoValorado.m_Cliente ))  pNewArticulo->m_LV_sCliente = "";
				else pNewArticulo->m_LV_sCliente = m_RsListadoValorado.m_Cliente;
			}
			
			m_pArrArticulos->Add(pNewArticulo);
			m_RsArticulo.MoveNext();
		}
	}            
	m_RsArticulo.Close();
	m_RsArticulo.m_strFilter = "";
	m_RsArtMol.Close();
	m_RsArtMol.m_strFilter = "";

// Ahora metemos en el cache los moldes, para lo que hay que acceder a RsArtMol por Molde

	m_RsArtMol.m_strFilter = "";
	m_RsArtMol.m_strSort = "IDMolde";
	if (!m_RsArtMol.Open(CRecordset::dynaset)) return;
	if (m_RsArtMol.GetRecordCount()) m_RsArtMol.MoveFirst();
	
	while (!m_RsMoldes.IsEOF())
	{   
		CBufMolde* pNewMoldes = new CBufMolde(m_RsMoldes);
		while (!m_RsArtMol.IsEOF() && m_RsMoldes.m_ID > m_RsArtMol.m_IDMolde) m_RsArtMol.MoveNext();
		if (m_RsArtMol.IsEOF()) m_RsArtMol.m_IDMolde = "";
		
		while (!m_RsArtMol.IsEOF() && m_RsMoldes.m_ID == m_RsArtMol.m_IDMolde)
		{
			CBufArtMol* pNewArtMol = new CBufArtMol(m_RsArtMol);
			ASSERT_VALID(pNewMoldes->m_pArrMolArt);
			pNewMoldes->m_pArrMolArt->Add(pNewArtMol);   
			m_RsArtMol.MoveNext();
		}
		
		while (!m_RsMolMaq.IsEOF() && m_RsMoldes.m_ID > m_RsMolMaq.m_IDMolde) m_RsMolMaq.MoveNext();
		if (m_RsMolMaq.IsEOF()) m_RsMolMaq.m_IDMolde = "";
		
		while (!m_RsMolMaq.IsEOF() && m_RsMoldes.m_ID == m_RsMolMaq.m_IDMolde)
		{
			CBufMolMaq* pNewMolMaq = new CBufMolMaq(m_RsMolMaq);
			ASSERT_VALID(pNewMoldes->m_pArrMolMaq);
			pNewMoldes->m_pArrMolMaq->Add(pNewMolMaq);   
			m_RsMolMaq.MoveNext();
		}

		while (!m_RsMoldes_Inyectora.IsEOF() && m_RsMoldes.m_ID > m_RsMoldes_Inyectora.m_idMolde) m_RsMoldes_Inyectora.MoveNext();
		if (m_RsMoldes_Inyectora.IsEOF()) m_RsMoldes_Inyectora.m_idMolde = "";

		while (!m_RsMoldes_Inyectora.IsEOF() && m_RsMoldes.m_ID == m_RsMoldes_Inyectora.m_idMolde)
		{
			CBufMoldesElem* pNewMoldes_Inyectora = new CBufMoldesElem(m_RsMoldes_Inyectora);
			ASSERT_VALID(pNewMoldes->m_pArrMolElem);
			pNewMoldes->m_pArrMolElem->Add(pNewMoldes_Inyectora);
			m_RsMoldes_Inyectora.MoveNext();
		}	

		while (!m_RsMoldes_Soplado.IsEOF() && m_RsMoldes.m_ID > m_RsMoldes_Soplado.m_idMolde) m_RsMoldes_Soplado.MoveNext();
		if (m_RsMoldes_Soplado.IsEOF()) m_RsMoldes_Soplado.m_idMolde = "";

		while (!m_RsMoldes_Soplado.IsEOF() && m_RsMoldes.m_ID == m_RsMoldes_Soplado.m_idMolde)
		{
			CBufMoldesElem* pNewMoldes_Soplado = new CBufMoldesElem(m_RsMoldes_Soplado);
			pNewMoldes->m_pArrMolElem->Add(pNewMoldes_Soplado);
			m_RsMoldes_Soplado.MoveNext();
		}

		m_pArrMoldes->Add(pNewMoldes);
		m_RsMoldes.MoveNext();
	}            
	m_RsMoldes.Close();
	m_RsMoldes.m_strFilter = "";
	m_RsArtMol.Close();
	m_RsArtMol.m_strFilter = "";
	m_RsMolMaq.Close();
	m_RsMolMaq.m_strFilter = "";

}
void CCache::LinkPointer(BOOL p_BRecarga)
{
	g_iContErr = 0;

	// Primero recorremos las OFs
	int iLim = m_pArrOFs->GetSize();
	for(int i=0; i < iLim; i++ )
	{
		CBufOF* pBufOF = (CBufOF*) m_pArrOFs->GetAt(i);
		ASSERT( pBufOF->IsKindOf( RUNTIME_CLASS( CBufOF ) ) );
		pBufOF->m_pBufArt = FindpArticulo( pBufOF->m_sIDArticulo );
		if (!pBufOF->m_pBufArt) ErrStruct(IDS_ERR_OFNOART, pBufOF->m_sIDArticulo, "", p_BRecarga);
    }      

	// Recorremos los articulos
	iLim = m_pArrArticulos->GetSize();
	for( int i = 0; i < iLim ; i++)
	{
		CBufArticulo* pBufArticulo = (CBufArticulo*) m_pArrArticulos->GetAt(i);
		ASSERT_VALID(pBufArticulo);
		// Primero buscamos las mezclas
		pBufArticulo->m_pBufMezcla = FindpMezcla( pBufArticulo->m_sMezcla );
		if (!pBufArticulo->m_pBufMezcla) ErrStruct(IDS_ERR_ARTNOMEZCLA, pBufArticulo->m_sID, "", p_BRecarga);
		// Y luego las relaciones con los moldes
		int iLimArtMol = pBufArticulo->m_pArrArtMol->GetSize();
		if (!iLimArtMol) ErrStruct(IDS_ERR_ARTNOMOLDE, pBufArticulo->m_sID, "", p_BRecarga);
		for (int j = 0; j < iLimArtMol; j++)
		{
			CBufArtMol* pBufArtMol = (CBufArtMol*) pBufArticulo->m_pArrArtMol->GetAt(j);
			ASSERT_VALID(pBufArtMol);
			pBufArtMol->m_pBufMolde = FindpMolde( pBufArtMol->m_sIDMolde );
			if (!pBufArtMol->m_pBufMolde) ErrStruct(IDS_ERR_MOLNOEXISTE, pBufArtMol->m_sIDMolde, pBufArticulo->m_sID , p_BRecarga);
			
		}
		// Tambi�n ponemos un puntero a la primera OF de ese art�culo
		pBufArticulo->m_pBufOF = FindOFArt(pBufArticulo->m_sID, pBufArticulo->m_iIndFirstOF);
		
	}
	// Y ahora recorremos los moldes
	iLim = m_pArrMoldes->GetSize();
	for(int i = 0; i < iLim ; i++)
	{
		CBufMolde* pBufMolde = (CBufMolde*) m_pArrMoldes->GetAt(i);
		ASSERT_VALID(pBufMolde);
//		if (pBufMolde->m_sID == "2951-2")
//			AfxMessageBox("2951-2");
		int iLimMolArt = pBufMolde->m_pArrMolArt->GetSize();
		if (!iLimMolArt) ErrStruct(IDS_ERR_MOLDENOART, pBufMolde->m_sID, "", p_BRecarga);

		for (int j = 0; j < iLimMolArt; j++)
		{
			CBufArtMol* pBufArtMol = (CBufArtMol*) pBufMolde->m_pArrMolArt->GetAt(j);
			ASSERT_VALID(pBufArtMol);
			pBufArtMol->m_pBufArticulo = FindpArticulo( pBufArtMol->m_sIDArticulo );
			if (!pBufArtMol->m_pBufArticulo) ErrStruct(IDS_ERR_MOLNOEXISTE, pBufArtMol->m_sIDMolde, pBufArtMol->m_sIDArticulo , p_BRecarga);
			
		}

		int iLimMolMaq = pBufMolde->m_pArrMolMaq->GetSize();
		if (!iLimMolMaq) ErrStruct(IDS_ERR_MOLNOMAQ, pBufMolde->m_sID, "" , p_BRecarga);
			
		for (int j = 0; j < iLimMolMaq; j++)
		{
			CBufMolMaq* pBufMolMaq = (CBufMolMaq*) pBufMolde->m_pArrMolMaq->GetAt(j);
			ASSERT_VALID(pBufMolMaq);
			pBufMolMaq->m_pBufMaquina = FindpMaquina( pBufMolMaq->m_sIDMaquina );
			pBufMolMaq->m_pBufMolde = FindpMolde( pBufMolMaq->m_sIDMolde );
			if (!pBufMolMaq->m_pBufMaquina) ErrStruct(IDS_ERR_MOLMAQNOEXISTE, pBufMolMaq->m_sIDMolde, pBufMolMaq->m_sIDMaquina , p_BRecarga);
			
		}

	}
	
	// Y los moldes-m�quinas
	int iLimMolMaq = m_pArrMolMaq->GetSize();
	for (int j = 0; j < iLimMolMaq; j++)
	{
		CBufMolMaq* pBufMolMaq = (CBufMolMaq*) m_pArrMolMaq->GetAt(j);
		ASSERT_VALID(pBufMolMaq);
		pBufMolMaq->m_pBufMaquina = FindpMaquina( pBufMolMaq->m_sIDMaquina );
		pBufMolMaq->m_pBufMolde = FindpMolde( pBufMolMaq->m_sIDMolde );
		if (!pBufMolMaq->m_pBufMaquina) ErrStruct(IDS_ERR_MOLMAQNOEXISTE, pBufMolMaq->m_sIDMolde, pBufMolMaq->m_sIDMaquina , p_BRecarga);
	}


}

void
CCache::IniFillLocales()
{

	m_RsLocales.m_strFilter = "";
	m_RsLocales.m_strSort = "ID";
                                
	if (m_RsLocales.IsOpen()) m_RsLocales.Close();
	if (!m_RsLocales.Open(CRecordset::dynaset)) return;
	if (!m_RsLocales.GetRecordCount()) return;
	
	m_RsLocales.MoveFirst();
		 		
	while (!m_RsLocales.IsEOF())
	{   
		CBufLocal* pNewLocales = new CBufLocal(m_RsLocales);
		m_pArrLocales->Add(pNewLocales);
		m_RsLocales.MoveNext();
	}            
	m_RsLocales.Close();
	m_RsLocales.m_strFilter = "";

}


void
CCache::IniFillMaquinas()
{

	m_RsMaquinas.m_strFilter = "";
	m_RsMaquinas.m_strSort = "ID";
                                
	if (m_RsMaquinas.IsOpen()) m_RsMaquinas.Close();
	if (!m_RsMaquinas.Open(CRecordset::dynaset)) return;
	if (!m_RsMaquinas.GetRecordCount()) return;
	
	m_RsMaquinas.MoveFirst();
		 		
	while (!m_RsMaquinas.IsEOF())
	{   
		CBufMaquina* pNewMaquinas = new CBufMaquina(m_RsMaquinas);
		m_pArrMaquinas->Add(pNewMaquinas);
		m_RsMaquinas.MoveNext();
	}            
	m_RsMaquinas.Close();
	m_RsMaquinas.m_strFilter = "";

}

void
CCache::IniFillMolMaq()
{

	m_RsMolMaq.m_strFilter = "";
	m_RsMolMaq.m_strSort = "IDMaquina, Prioridad";
                                
	if (m_RsMolMaq.IsOpen()) m_RsMolMaq.Close();
	if (!m_RsMolMaq.Open(CRecordset::dynaset)) return;
	if (!m_RsMolMaq.GetRecordCount()) return;
	
	m_RsMolMaq.MoveFirst();
		 		
	while (!m_RsMolMaq.IsEOF())
	{   
		CBufMolMaq* pNewMolMaq = new CBufMolMaq(m_RsMolMaq);
		m_pArrMolMaq->Add(pNewMolMaq);
		m_RsMolMaq.MoveNext();
	}            
	m_RsMolMaq.Close();
	m_RsMolMaq.m_strFilter = "";

}

void 
CCache::IniFillOFs()
{

	m_pRsPedidos = new CRsPedidos();
	
	m_pRsPedidos->m_strFilter = "";
	m_pRsPedidos->m_strSort = "IDArticulo, FecNec";
	if (m_pRsPedidos->Open(CRecordset::dynaset))
	{
		int iCnt = m_pRsPedidos->GetRecordCount();
		if (iCnt)
		{
			m_pRsPedidos->MoveFirst();
		 		
			while (!m_pRsPedidos->IsEOF())
			{   
				CBufOF* pBufOF = new CBufOF( *m_pRsPedidos );
				m_pArrOFs->Add(pBufOF);
				// Lo a�adimos tambien a la ordenacion por seccion
				m_pArrOFSeccion->Add(pBufOF);
				m_pRsPedidos->MoveNext();
			} 

		}
		// Si el que acabamos de abrir est� sobre la base de datos por defecto, ponemos el flag
	if (IsDefaultConnPedidos(m_pRsPedidos->GetDefaultConnect()))
		SetBConnPedidosIsDefault(true);
	else
		SetBConnPedidosIsDefault(false);
	}
	m_pRsPedidos->Close();
	delete m_pRsPedidos;

	
	// Ahora calculamos los acumulados
	CString sCodAnt;
	sCodAnt = "";
	BOOL bRep = FALSE;
	long lCantAcum = 0;
	int iLim = m_pArrOFs->GetSize();
	int iComb = 0;
	int i;
	for ( i = 0; i < iLim; i++)
	{
		CBufOF* pBufOF = (CBufOF*) m_pArrOFs->GetAt(i);
		if (sCodAnt == pBufOF->m_sIDArticulo) bRep = TRUE;
		else bRep = FALSE;
		CBufArticulo* pBufArticulo = pBufOF->m_pBufArt;
		
		if (!bRep) 
		{
			lCantAcum = 0;
		}
		lCantAcum += (long) pBufOF->m_dCantPend;
		pBufOF->m_dPendAcum = lCantAcum;
		pBufOF->m_dNoProg = pBufOF->m_dPendAcum - 
							pBufOF->m_dCantAlmac -
							pBufOF->m_dCantCurso -
							pBufOF->m_dContratas
							; 
		if (pBufArticulo)
		{
			if (pBufOF->m_dNoProg > 0 && pBufArticulo->GetdCadencia() > 0)
			{
				pBufOF->m_lHorasAcum = (long) ( pBufOF->m_dNoProg / 
								pBufArticulo->GetdCadencia() * (1- (TK_INACT_DEF / 100))) + 1;
			} else
				pBufOF->m_lHorasAcum = 0;
		}
		pBufOF->m_dNoCubierta = pBufOF->m_dPendAcum - 
							pBufOF->m_dCantAlmac -
							pBufOF->m_dCantCurso -
							pBufOF->m_dContratas
							; 
		//					(pBufOF->m_dCantAlmac > 0 ? pBufOF->m_dCantAlmac : 0) -
		//					(pBufOF->m_dCantCurso > 0 ? pBufOF->m_dCantCurso : 0); 
		//Ahora calculamos un array ordenado de punteros a ordenes, para encontrarlas mas r�pido
		// Este array es combinado, es decir hay un solo elemento por cada art�culo, se�alando
		// el �ndice superior e inferior de OFs
		if (!bRep)
		{
			COFComb* pOFComb = new COFComb;
			pOFComb->m_sID = pBufOF->m_sIDArticulo;
			pOFComb->m_iIni = i;
			m_pArrOFComb->Add(pOFComb);
			if (iComb > 0)
			{
				COFComb* pOFCombAnt = (COFComb*) m_pArrOFComb->GetAt(iComb - 1);
				pOFCombAnt->m_iFin = i - 1;
			}

			iComb++;
		}
			
		sCodAnt = pBufOF->m_sIDArticulo;
	}
	COFComb* pOFCombAnt = NULL;
	if (iComb > 0) pOFCombAnt = (COFComb*) m_pArrOFComb->GetAt(iComb - 1);
	
	if (pOFCombAnt) pOFCombAnt->m_iFin = i - 1;
	

}

void 
CCache::IniFillOFsAyer()
{

	m_pRsPedidosAyer = new CRsPedidosAyer();

	m_pRsPedidosAyer->m_strFilter = "";
	m_pRsPedidosAyer->m_strSort = "IDArticulo, FecNec";
	if (m_pRsPedidosAyer->IsOpen()) m_pRsPedidosAyer->Close();
	if (m_pRsPedidosAyer->Open(CRecordset::dynaset))
	{
		int iCnt = m_pRsPedidosAyer->GetRecordCount();
		if (iCnt)
		{
			m_pRsPedidosAyer->MoveFirst();
		 		
			while (!m_pRsPedidosAyer->IsEOF())
			{   
				CBufOF* pBufOF = new CBufOF( *m_pRsPedidosAyer );
				m_pArrOFAyer->Add(pBufOF);
				pBufOF->m_pBufArt = FindpArticulo(pBufOF->m_sIDArticulo);
				m_pRsPedidosAyer->MoveNext();
			} 

		}
	}
	m_pRsPedidosAyer->Close();
	
	delete m_pRsPedidosAyer;
	// Ahora calculamos los acumulados
	CString sCodAnt;
	sCodAnt = "";
	BOOL bRep = FALSE;
	long lCantAcum = 0;

	int iLim = m_pArrOFAyer->GetSize();
	int iComb = 0;
	for (int i = 0; i < iLim; i++)
	{
		CBufOF* pBufOF = (CBufOF*) m_pArrOFAyer->GetAt(i);
		CBufArticulo* pBufArt = NULL;
		if (sCodAnt == pBufOF->m_sIDArticulo) bRep = TRUE;
		else bRep = FALSE;
		if (!bRep) 
		{
			lCantAcum = 0;
			pBufArt = pBufOF->m_pBufArt;
		}
		lCantAcum += (long) pBufOF->m_dCantPend;
		pBufOF->m_dPendAcum = lCantAcum;
		pBufOF->m_dNoProg = pBufOF->m_dPendAcum - 
							pBufOF->m_dCantAlmac -
							pBufOF->m_dCantCurso -
							pBufOF->m_dContratas
							;
		pBufOF->m_dNoCubierta = pBufOF->m_dPendAcum - 
							pBufOF->m_dCantAlmac -
							pBufOF->m_dCantCurso -
							pBufOF->m_dContratas
							;
		if (pBufArt) 
		{
		//	pBufOF->m_dNoProg -= pBufArt->m_LV_lCantidad_Fabricada; 
		}
		sCodAnt = pBufOF->m_sIDArticulo;

	}	

}

void 
CCache::IniFillMezclas()
{

	CString sFilter;
	BOOL bEntregas = TRUE;

	m_RsMezclas.m_strFilter = "";
	m_RsMezclas.m_strSort = "ID";
	if (m_RsMezclas.IsOpen()) m_RsMezclas.Close();
	if (!m_RsMezclas.Open(CRecordset::dynaset)) return;

	m_RsEntregas.m_strFilter = "";
	m_RsEntregas.m_strSort = "IDMezcla";
	if (m_RsEntregas.IsOpen()) m_RsEntregas.Close();
	if (!m_RsEntregas.Open(CRecordset::dynaset)) bEntregas = FALSE;
	
	int iCnt = m_RsMezclas.GetRecordCount();
	if (iCnt)
	{
		m_RsMezclas.MoveFirst();
		
		while (!m_RsMezclas.IsEOF())
		{   
			CBufMezcla* pBufMezcla = new CBufMezcla( m_RsMezclas );

			
			while (!m_RsEntregas.IsEOF() && m_RsMezclas.m_ID > m_RsEntregas.m_IDMezcla) m_RsEntregas.MoveNext();
			if (m_RsEntregas.IsEOF()) m_RsEntregas.m_IDMezcla = "";
			
			while (!m_RsEntregas.IsEOF() && m_RsMezclas.m_ID == m_RsEntregas.m_IDMezcla)
			{
				CBufEntregas* pNewEntregas = new CBufEntregas(m_RsEntregas);
				ASSERT_VALID(pBufMezcla->m_pArrEntregas);
				pBufMezcla->m_pArrEntregas->Add(pNewEntregas);   
				m_RsEntregas.MoveNext();
			}

			
			m_pArrMezclas->Add(pBufMezcla);
			m_RsMezclas.MoveNext();
		} 
		
	}

	m_RsMezclas.Close();
	UpdateMezclasPlanta();
	
}


int
CCache::GetiNumTurnos(CTime p_Fec, const CProgMaq& p_PM, BOOL p_pBTurnos[]) 
{
// Funci�n que devuelve para una fecha y una m�quina, el n�mero de turnos que trabajan

	int iTurnos = 0;
	int iDefTurnos = GetDefaultTurnos(p_Fec, p_pBTurnos);
	int iNumTurnosPlanta = CTurnos::GetiNumTurnos();
	
	CString sFilter;
	// Primero miramos si hay un registro en la base de datos.
	// Si no lo hay devolveremos un valor por defecto.
	sFilter.Format( "Fecha = #%s# AND ID = '%s'"
					, FormatFec (p_Fec, FechaDB) 
					, p_PM.GetsID() );
    m_RsCalendario.m_strFilter = sFilter;                                 
	
	if ( m_RsCalendario.IsOpen() )	
		m_RsCalendario.Close();
	if ( m_RsCalendario.Open(CRecordset::dynaset) )
		if ( m_RsCalendario.GetRecordCount() )
			m_RsCalendario.MoveFirst();
		else 
			return iDefTurnos;

	
	if ( m_RsCalendario.IsEOF() ) return iDefTurnos;
	
	if ( m_RsCalendario.m_Turno1 ) iTurnos++;
	if ( m_RsCalendario.m_Turno2 ) iTurnos++;
	if ( m_RsCalendario.m_Turno3 ) iTurnos++;

	if (p_pBTurnos) 
	{
		p_pBTurnos[0] = m_RsCalendario.m_Turno1;
		if (iNumTurnosPlanta > 1) p_pBTurnos[1] = m_RsCalendario.m_Turno2;
		if (iNumTurnosPlanta > 2) p_pBTurnos[2] = m_RsCalendario.m_Turno3;
	}
	
	return iTurnos;
}

void
CCache::SetTurnos(const CFecTur p_FecTur, const CProgMaq* p_pPM)
{
	// Esta funci�n nos indica que en la maquina p_pPM, en la Fecha/Turno p_FecTur
	// la situaci�n de trabajo de ese turno ha cambiado.
	// Lo cambiamos en la base de datos y luego volvemos a crear las inactividades
	// de esa m�quina.
	BOOL BTurnos[TK_NUM_TURNOS_MAX];
	// Funci�n que devuelve para una fecha y una m�quina, el n�mero de turnos que trabajan
	int i = GetDefaultTurnos(p_FecTur.GetFec(), BTurnos);
	int iNumTurnosPlanta = CTurnos::GetiNumTurnos();
	
	
	CString sFilter;
	BOOL bNewRec = FALSE;
	sFilter.Format( "Fecha = #%s# AND ID = '%s'"
		, FormatFec (p_FecTur.GetFec(), FechaDB) 
		, p_pPM->GetsID() );
    m_RsCalendario.m_strFilter = sFilter;                                 
	if ( m_RsCalendario.IsOpen() ) 
		m_RsCalendario.Close();
	if (m_RsCalendario.Open(CRecordset::dynaset))
	{
		if (!m_RsCalendario.CanUpdate())
		{
			AfxMessageBox(STRID(IDS_ERR_NOMODCAL));
			return;
		}
		if (m_RsCalendario.IsBOF() || m_RsCalendario.IsEOF())
		{
			GetDefaultTurnos(p_FecTur.GetFec(), BTurnos);
			bNewRec = TRUE;
			//m_RsCalendario.AddNew();
			m_RsCalendario.m_Fecha = p_FecTur.GetFec();
			m_RsCalendario.m_ID = p_pPM->GetsID();
		} else 
		{
//			m_RsCalendario.Edit();
			// Si hay un registro, sus datos suprimen al valor por defecto.
			BTurnos[0] = m_RsCalendario.m_Turno1;
			BTurnos[1] = iNumTurnosPlanta > 1 ? m_RsCalendario.m_Turno2 : 0;
			BTurnos[2] = iNumTurnosPlanta > 2 ? m_RsCalendario.m_Turno3 : 0;
		}
		
		// Cambiamos el estado del Turno seleccionado antes de actualizar
		int iTurno = p_FecTur.GetTurno() - 1;
		BTurnos[iTurno] = BTurnos[iTurno] ? FALSE : TRUE;
		m_RsCalendario.m_Turno1 = BTurnos[0];
		m_RsCalendario.m_Turno2 = BTurnos[1];
		m_RsCalendario.m_Turno3 = BTurnos[2];
		
		if (bNewRec)
		{
			m_RsCalendario.Close();
			CString sCmd;
			sCmd.Format("INSERT INTO Calendario VALUES ('%s', #%s#, %d, %d, %d)",
				m_RsCalendario.m_ID,
				FormatFec(m_RsCalendario.m_Fecha, FechaDB),
				m_RsCalendario.m_Turno1,
				iNumTurnosPlanta > 1 ? m_RsCalendario.m_Turno2 : 0,
				iNumTurnosPlanta > 2 ? m_RsCalendario.m_Turno3 : 0
				);
			m_pDB->ExecuteSQL(sCmd);
			
		} else
		{
			m_RsCalendario.Close();
			CString sCmd;
			sCmd.Format("UPDATE Calendario SET Turno1 = %d, Turno2 = %d, Turno3 = %d WHERE ID = '%s' AND Fecha = #%s#",
				m_RsCalendario.m_Turno1,
				iNumTurnosPlanta > 1 ? m_RsCalendario.m_Turno2 : 0,
				iNumTurnosPlanta > 2 ? m_RsCalendario.m_Turno3 : 0,
				m_RsCalendario.m_ID,
				FormatFec(m_RsCalendario.m_Fecha, FechaDB)
				);
			m_pDB->ExecuteSQL(sCmd);
/*
			if (!m_RsCalendario.Update())
			{
				AfxMessageBox(STRID(IDS_ERR_NOMODCAL));
				return;
			}
			m_RsCalendario.Close();
*/
		}            
	}
}

CBufOF* 
CCache::FindOFNum( long p_lOFNum )
{
	// Busca una orden por el n�mero de OF, NO por el ID �nico
	int iLim = m_pArrOFs->GetSize();
	for( int i = 0; i < iLim ; i++)
	{	
		CBufOF* pBufOF = (CBufOF*) m_pArrOFs->GetAt(i);
		ASSERT( pBufOF->IsKindOf( RUNTIME_CLASS( CBufOF ) ) );
//		if (pBufOF->m_OrdFab == p_lOFNum) return pBufOF;
	}                                            
	return NULL;
}

// Busca una orden por el c�digo de art�culo, se queda con la primera que ecuentra
CBufOF* 
CCache::FindOFArt( const CString& p_sArticulo, int& p_iIndFirstOM)
{
	int iLim = m_pArrOFs->GetSize();
	for( int i = 0; i < iLim ; i++)
	{	
		CBufOF* pBufOF = (CBufOF*) m_pArrOFs->GetAt(i);
		ASSERT( pBufOF->IsKindOf( RUNTIME_CLASS( CBufOF ) ) );
		if (pBufOF->m_sIDArticulo == p_sArticulo) 
		{
			p_iIndFirstOM = i;
			return pBufOF;
		}
	}                                            
	return NULL;
}

// Devuelve la primera fecha que no est� programada para un art�culo
CTime 
CCache::GetFecNoProg( const CBufArticulo*  p_pBufArticulo )
{
	int iIni = p_pBufArticulo->GetiIndFirstOF();
	CBufOF* pBufOF = p_pBufArticulo->GetpBufOF();
	if (!pBufOF) return DATE_MIN;
	CTime FecMin = DATE_MAX;
	int iLim = m_pArrOFs->GetSize();
	int i;
	for( i = iIni; i < iLim ; i++)
	{	
		CBufOF* pBufOF = (CBufOF*) m_pArrOFs->GetAt(i);
		if ( pBufOF->m_sIDArticulo != p_pBufArticulo->m_sID  ) break;

		if (pBufOF->m_FecNec < FecMin) FecMin = pBufOF->m_FecNec;
		if (pBufOF->m_dNoProg > 0) return pBufOF->m_FecNec;
		
	}
	if (i == iIni) TRACE("Error en iIndFirstOF() en GetFecNoProg()");
	if (FecMin < DATE_MAX) return FecMin;
	else return DATE_MIN;
}

// Devuelve la primera fecha que no tiene para un art�culo
CTime 
CCache::GetFecNoStock( const CBufArticulo*  p_pBufArticulo )
{
	int iIni = p_pBufArticulo->GetiIndFirstOF();
	CBufOF* pBufOF = p_pBufArticulo->GetpBufOF();
	if (!pBufOF) return DATE_MIN;
	CTime FecMin = DATE_MAX;
	int iLim = m_pArrOFs->GetSize();
	int i;
	for( i = iIni; i < iLim ; i++)
	{	
		CBufOF* pBufOF = (CBufOF*) m_pArrOFs->GetAt(i);
		if ( pBufOF->m_sIDArticulo != p_pBufArticulo->m_sID  ) break;

		if (pBufOF->m_FecNec < FecMin) FecMin = pBufOF->m_FecNec;
		if (pBufOF->m_dNoCubierta > 0) return pBufOF->m_FecNec;
	}
	if (i == iIni) TRACE("Error en iIndFirstOF() en GetFecNoStock()");
	if (FecMin < DATE_MAX) return FecMin;
	else return DATE_MIN;
}

// Devuelve el menor de los m�rgenes para un art�culo
long 
CCache::GetlPeorMargen( const CBufArticulo*  p_pBufArticulo )
{
	long lPeorMargen = INT_MAX;

	int iIni = p_pBufArticulo->GetiIndFirstOF();
	CBufOF* pBufOF = p_pBufArticulo->GetpBufOF();
	if (!pBufOF) return lPeorMargen;
	int iLim = m_pArrOFs->GetSize();
	int i;
	for(  i = iIni; i < iLim ; i++)
	{	
		CBufOF* pBufOF = (CBufOF*) m_pArrOFs->GetAt(i);
		if ( pBufOF->m_sIDArticulo != p_pBufArticulo->m_sID  ) break;
		
		// Las l�neas no fabricadas son consideradas de menos prioridad que las ya 
		long lMargenAct = (pBufOF->m_lMargenMed == INT_MIN) ? INT_MAX - 1 : pBufOF->m_lMargenMed;
		if (lMargenAct < lPeorMargen ) 
			lPeorMargen = lMargenAct;
	}
	if (i == iIni) TRACE("Error en iIndFirstOF() en GetlPeorMargen()");
	return lPeorMargen;
}

long 
CCache::GetlHorasFab( const CBufArticulo*  p_pBufArticulo , CTime p_Fec)
{
	long lHoras = 0;

	int iIni = p_pBufArticulo->GetiIndFirstOF();
	CBufOF* pBufOF = p_pBufArticulo->GetpBufOF();
	if (!pBufOF) return lHoras;
	int iLim = m_pArrOFs->GetSize();
	for( int i = iIni; i < iLim ; i++)
	{	
		CBufOF* pBufOF = (CBufOF*) m_pArrOFs->GetAt(i);
		if (pBufOF->m_sIDArticulo != p_pBufArticulo->m_sID  ) break;
		if (pBufOF->m_FecNec > p_Fec ) break;
		
		lHoras = pBufOF->m_lHorasAcum;

	}
	return lHoras;
}


void
CCache::UpdateOF( const CString& p_sIDArt , int p_iCant)
{
	if (!GetBConnPedidosIsDefault())
	{
		AfxMessageBox("No est� trabajando con la base de datos de hoy. No se actualizar�n los datos");
		return;
	}

	int iIni;
	CBufOF* pBufOF = FindOFArt(p_sIDArt, iIni);
//	logFile.Write("%s - %s", "UpdateOF", "Buscando OF");
	if (!pBufOF) return;
//	logFile.Write("%s - %s", "Encontrada 0F", pBufOF->m_sIDArticulo);
	
	int iLim = m_pArrOFs->GetSize();
	for( int i = iIni; i < iLim ; i++)
	{	
		CBufOF* pBufOF = (CBufOF*) m_pArrOFs->GetAt(i);
		ASSERT( pBufOF->IsKindOf( RUNTIME_CLASS( CBufOF ) ) );
		if (pBufOF->m_sIDArticulo == p_sIDArt)
			pBufOF->m_dCantCurso += p_iCant;
		else break;
	}                                            


	CString sFilter;
	CString strCmd;
//	logFile.Write("%s - %d : %s", "Actualizando Pedidos", p_iCant, p_sIDArt);
	strCmd.Format("UPDATE Pedidos SET CantCurso = CantCurso + %d WHERE IDArticulo = '%s'",
						p_iCant, p_sIDArt );
	m_pDB->ExecuteSQL(strCmd);


}

CBufArticulo*
CCache::AddArticulo (const char* p_IDArticulo)
{
	BOOL BEnBD = TRUE;
	// Primero comprobamos si el identificador ya existe
	for( int i=0; i< m_pArrArticulos->GetSize(); i++)
	{	
		CBufArticulo* pBufArt = (CBufArticulo*) m_pArrArticulos->GetAt(i);
		ASSERT( pBufArt->IsKindOf( RUNTIME_CLASS( CBufArticulo ) ) );
		if (pBufArt->m_sID == p_IDArticulo) return pBufArt;
	}

	return NULL;
/*
	m_RsArticulo.m_IDParam = p_IDArticulo;
	if (!m_RsArticulo.CanRestart()) BEnBD = FALSE;
	if (BEnBD && !m_RsArticulo.Requery()) BEnBD = FALSE;
	if (BEnBD && m_RsArticulo.IsEOF()) BEnBD = FALSE;

	if (!BEnBD)
	{
		CString sMsg;
		CString sForm = STRID( IDS_ERR_NOART );
		
		sMsg.Format( sForm,	p_IDArticulo);
		g_bException = TRUE;
		AfxMessageBox(sMsg);
		ExitProcess(0);
		
	}
	
	CBufArticulo* pNewArticulo = new CBufArticulo(m_RsArticulo);

	ASSERT_VALID (pNewArticulo);
	m_RsArtGFH.m_IDParam = p_IDArticulo;
	if (!m_RsArtGFH.CanRestart()) BEnBD = FALSE;
	if (BEnBD && !m_RsArtGFH.Requery())  BEnBD = FALSE;
	
	if (!BEnBD)
	{
		CString sMsg;
		TRACE(STRID(IDS_ERR_NOGFHART ));
			
		CString sForm = STRID( IDS_ERR_NOGFHART );
		
		sMsg.Format( sForm,	p_IDArticulo);
		g_bException = TRUE;
		AfxMessageBox(sMsg);
		ExitProcess(0);
		
	}
	
	while ( !m_RsArtGFH.IsEOF())
	{ 
		CBufArtGFH* pNewArtGFH = new CBufArtGFH(m_RsArtGFH);
		ASSERT_VALID(pNewArticulo->m_pArrArtGFH);
		pNewArticulo->m_pArrArtGFH->Add(pNewArtGFH);   
		m_RsArtGFH.MoveNext();
	}
	m_RsArtMaq.m_IDParam = p_IDArticulo;
	if (!m_RsArtMaq.CanRestart()) BEnBD = FALSE;
	if (!m_RsArtMaq.Requery()) BEnBD = FALSE;
	if (BEnBD)
	{
		while (!m_RsArtMaq.IsEOF())
		{ 
			CBufArtMaq* pNewArtMaq = new CBufArtMaq(m_RsArtMaq);
			ASSERT_VALID(pNewArticulo->m_pArrArtMaq);
			pNewArticulo->m_pArrArtMaq->Add(pNewArtMaq);
			m_RsArtMaq.MoveNext();
		}
	}
	m_pArrArticulos->Add(pNewArticulo);
	return pNewArticulo;
*/
}

CBufMaquina*
CCache::AddMaquina (const char* p_IDMaquina)
{
	
	// Primero comprobamos si el identificador ya existe
	for( int i=0; i< m_pArrMaquinas->GetSize(); i++)
	{	
		CBufMaquina* pBufMaq = (CBufMaquina*) m_pArrMaquinas->GetAt(i);
		ASSERT( pBufMaq->IsKindOf( RUNTIME_CLASS( CBufMaquina ) ) );
		if (pBufMaq->m_sID == p_IDMaquina) return pBufMaq;
	}

	return NULL;
}

void
CCache::AddOF( CBufOF* p_pOF )
{
}

// GetArrGFHs : Nos da un Array de * a Strings, vac�o, y lo llenamos con 
// los GFH programables en orden de programa. El recordset de GFHs est�
// preparado para acceso directo, por lo que hay que cerrarlo y abrirlo para
// poder acceder secuencialmente. Esto no est� documentado pero parece que
// funciona.
int 
CCache::GetArrGFHs( CStringArray& p_ArrGFHs )
{
    int NumGfh = 0;
	int bUniqName = FALSE;
	long lUltGFH = -1;

	p_ArrGFHs.RemoveAll();

	m_RsMaquinas.Close();
	m_RsMaquinas.m_strFilter = "";
	m_RsMaquinas.m_strSort = "GFH, Orden, ID";		// Orden de la consulta

	if (m_RsMaquinas.Open(CRecordset::dynaset))
	{
		m_RsMaquinas.MoveFirst();
 		
		while (!m_RsMaquinas.IsEOF())
		{
			if (m_RsMaquinas.m_Programable && m_RsMaquinas.m_GFH != lUltGFH)
			{
				CString sGFH;
				sGFH.Format("%ld", m_RsMaquinas.m_GFH);
				p_ArrGFHs.Add(sGFH);
				lUltGFH = m_RsMaquinas.m_GFH;
				NumGfh++;                           
			}
			
			m_RsMaquinas.MoveNext();
		}
	}            
	m_RsMaquinas.Close();
	m_RsMaquinas.m_strFilter = "";
	m_RsMaquinas.Open(CRecordset::dynaset);
	return NumGfh;
}

// GetArrBloques : Nos da un Array de enteros vacio y lo llenamos con los bloques  
// disponibles en orden de mayor a menor
int
CCache::GetArrBloques( CUIntArray& p_ArrBloques )
{
	return 0;
}


void
CCache::GetMaxMinBlq( int p_iBloque, int& p_iMinBlq, int& p_iMaxBlq)
{
	p_iMinBlq = p_iMaxBlq = 0;
}


CBufOF* 
CCache::AddOF( long p_IDOF , BOOL p_bUpdateFec )
{
	return NULL;
/*
	for( int i=0; i< m_pArrOFs->GetSize(); i++)
	{	
		CBufOF* pBufOF = (CBufOF*) m_pArrOFs->GetAt(i);
		ASSERT( pBufOF->IsKindOf( RUNTIME_CLASS( CBufOF ) ) );
		if (pBufOF->m_ID == p_IDOF) return pBufOF;
	}                                            

	BOOL BEnBD = TRUE;
	
	CString sFilter;
    sFilter.Format("ID = %ld", p_IDOF);
	m_RsBlqLin.m_strFilter = sFilter;
	if (!m_RsBlqLin.Open()) BEnBD = FALSE;
	if (BEnBD) if (!m_RsBlqLin.GetRecordCount()) BEnBD = FALSE;
	if (BEnBD)
	{
		m_RsBlqLin.MoveFirst();
		if (m_RsBlqLin.IsEOF()) BEnBD = FALSE;
	}
	// Si hay que actualizar la fecha de traspaso, lo hacemos ahora
	if (p_bUpdateFec)
	{
		if (!m_RsBlqLin.CanUpdate())
		{	
			TRACE(STRID(IDS_ERR_NOACT));
			AfxMessageBox(STRID(IDS_ERR_NOACT));
		} else
		{
			m_RsBlqLin.Edit();
			m_RsBlqLin.m_FecTrasp = CTime::GetCurrentTime();
			m_RsBlqLin.Update();
		}
	}

	m_RsBlqLin.Close();
	
	if (!BEnBD) 
	{
		CString sMsg;
		CString sForm = STRID( IDS_ERR_NOID );
		
		sMsg.Format( sForm,	p_IDOF);
		g_bException = TRUE;
		AfxMessageBox(sMsg);
		ExitProcess(0);
		
	}
	
	
	CBufOF* pNewBufOF = new CBufOF(m_RsBlqLin);
	m_pArrOFs->Add(pNewBufOF);
	pNewBufOF->m_pBufArt = FindpArticulo(pNewBufOF->m_sIDArticulo);
	
	return pNewBufOF;
*/

}

// Recibe un codigo de articulo y para este articulo rellena dos Arrays con punteros
// a las m�quinas que pueden fabricarlo, y con qu� moldes
int 
CCache::GetArrMaq(const CString &p_sArticulo, CObArray &p_ArrMaq, CObArray &p_ArrMoldes)
{
	// Vaciamos los arrays
	p_ArrMaq.RemoveAll();
	p_ArrMoldes.RemoveAll();
	// Buscamos el articulo indicado
	CBufArticulo* pBufArticulo = FindpArticulo(p_sArticulo);
	if (pBufArticulo == NULL) return 0;
	// Recorremos los moldes posibles de este articulo
	int iLimArtMol = pBufArticulo->m_pArrArtMol->GetSize();
	for (int i = 0; i < iLimArtMol; i++)
	{
		CBufArtMol* pBufArtMol = (CBufArtMol*) pBufArticulo->m_pArrArtMol->GetAt(i);
		ASSERT_VALID(pBufArtMol);
		CBufMolde* pBufMolde = pBufArtMol->m_pBufMolde;
		ASSERT_VALID(pBufMolde);
		if (!pBufMolde) continue;
		int iLimMolMaq = pBufMolde->m_pArrMolMaq->GetSize();
		for (int j = 0; j < iLimMolMaq; j++)
		{
			// Obtenemos las maquinas del molde
			CBufMolMaq* pBufMolMaq = (CBufMolMaq*) pBufMolde->m_pArrMolMaq->GetAt(j);
			ASSERT_VALID(pBufMolMaq);
			// Y a�adimos la maquina al array de maquinas. Nos dan igual los duplicados por el
			// momento.
			if (pBufMolMaq->m_pBufMaquina != NULL)
			{
				p_ArrMaq.Add(pBufMolMaq->m_pBufMaquina);
				p_ArrMoldes.Add(pBufMolde);
			}
		}
	}

	return p_ArrMaq.GetSize();

}

// Recibe un codigo de articulo y para este articulo rellena un Array con 
// los componentes de ese art�culo (sin Mezcla)
int
CCache::GetArrComponentes(const CString& p_sArticulo, CObArray& p_ArrComponentes)
{
	// Vaciamos los arrays
	p_ArrComponentes.RemoveAll();
	// Buscamos el articulo indicado
	CBufArticulo* pBufArticulo = FindpArticulo(p_sArticulo);
	if (pBufArticulo == NULL) return 0;
	// Recorremos los componentes de este articulo
	int iLimComponentes = pBufArticulo->m_pArrComponentes->GetSize();
	for (int i = 0; i < iLimComponentes; i++)
	{
		CBufComponentes* pBufComponentes = (CBufComponentes*)pBufArticulo->m_pArrComponentes->GetAt(i);
		ASSERT_VALID(pBufComponentes);
		p_ArrComponentes.Add(pBufComponentes);
	}

	return p_ArrComponentes.GetSize();

}

// Recibe un codigo de articulo y para este articulo rellena un Array con punteros
// a las operaciones de acabado que tiene este art�culo
int 
CCache::GetArrAcabado(const CString &p_sArticulo, CObArray &p_ArrAcabado)
{
	// Vaciamos los arrays
	p_ArrAcabado.RemoveAll();
	// Buscamos el articulo indicado
	CBufArticulo* pBufArticulo = FindpArticulo(p_sArticulo);
	if (pBufArticulo == NULL) return 0;
	// Recorremos las operaciones de acabado posibles de este articulo
	int iLimAcabado = pBufArticulo->m_pArrAcabado->GetSize();
	for (int i = 0; i < iLimAcabado; i++)
	{
		CBufAcabado* pBufAcabado = (CBufAcabado*) pBufArticulo->m_pArrAcabado->GetAt(i);
		ASSERT_VALID(pBufAcabado);
		p_ArrAcabado.Add(pBufAcabado);
	}

	return p_ArrAcabado.GetSize();

}

// Recibe un codigo de articulo y para este articulo devuelve la suma de las cadencias de 
// a las operaciones de acabado que tiene este art�culo, si el p_lCentro es positivo,
// queremos solo la suma de ese centro
double 
CCache::GetdCadenciaAcabado(const CString &p_sArticulo, long p_lCentro)
{
	// Buscamos el articulo indicado
	CBufArticulo* pBufArticulo = FindpArticulo(p_sArticulo);
	if (pBufArticulo == NULL) return 0;
	// Recorremos los moldes posibles de este articulo
	int iLimAcabado = pBufArticulo->m_pArrAcabado->GetSize();
	double dCadencia = 0;
	for (int i = 0; i < iLimAcabado; i++)
	{
		CBufAcabado* pBufAcabado = (CBufAcabado*) pBufArticulo->m_pArrAcabado->GetAt(i);
		ASSERT_VALID(pBufAcabado);
		if (p_lCentro < 0 || p_lCentro == pBufAcabado->m_lCentro)
		{
			if (pBufAcabado->m_dCadencia > 0) dCadencia += 1 / pBufAcabado->m_dCadencia;
		}
		
	}

	if (dCadencia > 0) return (1 / dCadencia);
	else return 0;

}



// Recibe un codigo de maquina y para esta maquina rellena un Arrays con punteros
// a objetos CMolMaq que esa m�quina puede realizar, en orden de prioridad
// Si se da una referencia a un c�digo de art�culo, se seleccionan s�lo los moldes
// que pudan fabricar ese art�culo
int 
CCache::GetArrMolMaq(const CString &p_sMaquina, CObArray &p_ArrMolMaq, const CString* p_psIDArt)
{
	// Vaciamos el array
	p_ArrMolMaq.RemoveAll();
	// Buscamos la m�quina indicada
	CBufMaquina* pBufMaquina = FindpMaquina(p_sMaquina);
	if (pBufMaquina == NULL) return 0;
	BOOL bFinMaq = FALSE;
	for (int i=0; i < m_pArrMolMaq->GetSize() ; i++)
	{
		CBufMolMaq* pBufMolMaq = (CBufMolMaq*) m_pArrMolMaq->GetAt(i);
		if ( pBufMolMaq->m_sIDMaquina == p_sMaquina )
		{
			if (p_psIDArt)
			{
				CBufMolde* pBufMolde = pBufMolMaq->m_pBufMolde;
				ASSERT_VALID(pBufMolde);
				int iLim = pBufMolde->m_pArrMolArt->GetSize();
				for (int iArt = 0; iArt < iLim; iArt ++)
				{
					CBufArtMol* pBufArtMol = (CBufArtMol*) pBufMolde->m_pArrMolArt->GetAt(iArt);
					ASSERT( pBufArtMol->IsKindOf( RUNTIME_CLASS( CBufArtMol ) ) );
 
					if (pBufArtMol->m_sIDArticulo == *p_psIDArt)
					{
						p_ArrMolMaq.Add(pBufMolMaq);
					}
				}
			} else
			{
				p_ArrMolMaq.Add(pBufMolMaq);
			}
		}
	}
	return p_ArrMolMaq.GetSize();
}

// Recibe un codigo de maquina y para esta maquina rellena un Arrays con objetos ArrInfoMolArt
// Ojo, GetArrMolArt crea objetos en ArrInfoMolArt, se deben borrar luego
int 
CCache::GetArrMolArt(const CString &p_sMaquina, CObArray &p_ArrInfoMolArt)
{
	CObArray ArrMoldes;
	CUIntArray ArrPrioridad;
	// Vaciamos el array de entrada por si acaso
	for (int iB = 0; iB < p_ArrInfoMolArt.GetSize(); iB++)
	{
		CInfoMolArt* pInfoMolArt = (CInfoMolArt*) p_ArrInfoMolArt.GetAt(iB);
		delete pInfoMolArt;
	}
	p_ArrInfoMolArt.RemoveAll();
	// Buscamos la m�quina indicada
	CBufMaquina* pBufMaquina = FindpMaquina(p_sMaquina);
	if (pBufMaquina == NULL) return 0;
	BOOL bFinMaq = FALSE;
	// Recorremos el array de moldes/m�quina buscando los que puede hacer esta m�quina
	// Y hacemos una lista de moldes (CBufMolde*) en ArrMoldes
	for (int i=0; i < m_pArrMolMaq->GetSize() ; i++)
	{
		CBufMolMaq* pBufMolMaq = (CBufMolMaq*) m_pArrMolMaq->GetAt(i);
		if (!pBufMolMaq) continue;
		if ( pBufMolMaq->m_sIDMaquina == p_sMaquina )
		{
			CBufMolde* pBufMolde = pBufMolMaq->m_pBufMolde;
			ASSERT_VALID(pBufMolde);
			if (!pBufMolde) continue;
			ArrMoldes.Add(pBufMolde);
			ArrPrioridad.Add(pBufMolMaq->m_lPrioridad);
			bFinMaq = TRUE;
		}
		if (bFinMaq && pBufMolMaq->m_sIDMaquina != p_sMaquina) // para hacerlo m�s corto, ya que est� ordenado por maq.
			break;
	}
	// Ahora para cada molde buscamos su art�culo
	for (int i = 0; i < ArrMoldes.GetSize(); i++)
	{
		CBufMolde* pBufMolde = (CBufMolde*) ArrMoldes.GetAt(i);
		ASSERT_VALID(pBufMolde);
		if (!pBufMolde) continue;
		long lPrioridad = ArrPrioridad.GetAt(i);
		// Un molde puede hacer varias referencias
		for (int j=0; j < pBufMolde->m_pArrMolArt->GetSize(); j++)
		{
			CBufArtMol* pBufArtMol = (CBufArtMol*) pBufMolde->m_pArrMolArt->GetAt(j);
			ASSERT( pBufArtMol->IsKindOf( RUNTIME_CLASS( CBufArtMol ) ) );
  
			// Nos aseguramos de que la referencia no est� ya en la matriz
			BOOL bInMatrix = FALSE;
			for ( int k = 0; k < p_ArrInfoMolArt.GetSize(); k++)
			{
				CInfoMolArt* pInfoMolArt = (CInfoMolArt*) p_ArrInfoMolArt.GetAt(k);
				ASSERT( pInfoMolArt->IsKindOf( RUNTIME_CLASS( CInfoMolArt ) ) );
  
				if (pBufArtMol->m_sIDArticulo == pInfoMolArt->m_sIDArticulo )
				{
					bInMatrix = TRUE;
					break;
				}
			}
			// Si no est� la a�adimos
			if (!bInMatrix)
			{
				CBufArticulo* pBufArticulo = pBufArtMol->m_pBufArticulo;
				if (pBufArticulo) 
				{
					CInfoMolArt* pInfoMolArt = new CInfoMolArt;
					pInfoMolArt->m_sIDArticulo = pBufArtMol->m_sIDArticulo;
					pInfoMolArt->m_lPrioridad = lPrioridad;
					pInfoMolArt->m_lPorcHist = 0;
					pInfoMolArt->m_FecProg = GetFecNoProg(pBufArticulo);
					pInfoMolArt->m_FecStock = GetFecNoStock(pBufArticulo);
					pInfoMolArt->m_lHorasRetraso = GetlPeorMargen(pBufArticulo);
					pInfoMolArt->m_lHoras0 = GetlHorasFab(pBufArticulo, CTime::GetCurrentTime());
					pInfoMolArt->m_lHoras7 = GetlHorasFab(pBufArticulo, AddDays(CTime::GetCurrentTime(), 7));
					pInfoMolArt->m_lHoras15 = GetlHorasFab(pBufArticulo, AddDays(CTime::GetCurrentTime(), 15));
					pInfoMolArt->m_lHoras21 = GetlHorasFab(pBufArticulo, AddDays(CTime::GetCurrentTime(), 21));
					pInfoMolArt->m_lHoras30 = GetlHorasFab(pBufArticulo, AddDays(CTime::GetCurrentTime(), 30));
					pInfoMolArt->m_lHoras37 = GetlHorasFab(pBufArticulo, AddDays(CTime::GetCurrentTime(), 37));
					pInfoMolArt->m_lHoras45 = GetlHorasFab(pBufArticulo, AddDays(CTime::GetCurrentTime(), 45));
					pInfoMolArt->m_lHoras52 = GetlHorasFab(pBufArticulo, AddDays(CTime::GetCurrentTime(), 52));
					pInfoMolArt->m_lHoras60 = GetlHorasFab(pBufArticulo, AddDays(CTime::GetCurrentTime(), 60));
					
					
//					if (pBufArticulo->m_pOMC) pInfoMolArt->m_BProg = TRUE;
//					else pInfoMolArt->m_BProg = FALSE;


					p_ArrInfoMolArt.Add(pInfoMolArt);
				}
			}
		}

	}
	return p_ArrInfoMolArt.GetSize();

}

/*
// Recibe un codigo de maquina y para esta maquina rellena un Arrays con punteros
// a los moldes que puede recibir
int 
CCache::GetArrMolArt(const CString &p_sMaquina, CObArray &p_ArrInfoMolArt)
{
	CObArray ArrMoldes;
	CUIntArray ArrPrioridad;
	// Vaciamos los arrays
	for (int iB = 0; iB < p_ArrInfoMolArt.GetSize(); iB++)
	{
		CInfoMolArt* pInfoMolArt = (CInfoMolArt*) p_ArrInfoMolArt.GetAt(iB);
		delete pInfoMolArt;
	}
	p_ArrInfoMolArt.RemoveAll();
	// Buscamos la m�quina indicada
	CBufMaquina* pBufMaquina = FindpMaquina(p_sMaquina);
	if (pBufMaquina == NULL) return 0;
	BOOL bFinMaq = FALSE;
	for (int i=0; i < m_pArrMolMaq->GetSize() ; i++)
	{
		CBufMolMaq* pBufMolMaq = (CBufMolMaq*) m_pArrMolMaq->GetAt(i);
		if ( pBufMolMaq->m_sIDMaquina == p_sMaquina )
		{
			CBufMolde* pBufMolde = FindpMolde(pBufMolMaq->m_sIDMolde);
			ArrMoldes.Add(pBufMolde);
			ArrPrioridad.Add(pBufMolMaq->m_lPrioridad);
			bFinMaq = TRUE;
		}
		if (bFinMaq && pBufMolMaq->m_sIDMaquina != p_sMaquina) // para hacerlo m�s corto, ya que est� ordenado por maq.
			break;
	}
	// Ahora para cada molde buscamos su art�culo
	for ( i = 0; i < ArrMoldes.GetSize(); i++)
	{
		CBufMolde* pBufMolde = (CBufMolde*) ArrMoldes.GetAt(i);
		long lPrioridad = ArrPrioridad.GetAt(i);
		// Un molde puede hacer varias referencias
		for (int j=0; j < pBufMolde->m_pArrMolArt->GetSize(); j++)
		{
			CBufArtMol* pBufArtMol = (CBufArtMol*) pBufMolde->m_pArrMolArt->GetAt(j);
			ASSERT( pBufArtMol->IsKindOf( RUNTIME_CLASS( CBufArtMol ) ) );
  
			// Nos aseguramos de que no est� ya en la matriz
			BOOL bInMatrix = FALSE;
			for ( int k = 0; k < p_ArrInfoMolArt.GetSize(); k++)
			{
				CInfoMolArt* pInfoMolArt = (CInfoMolArt*) p_ArrInfoMolArt.GetAt(k);
				ASSERT( pInfoMolArt->IsKindOf( RUNTIME_CLASS( CInfoMolArt ) ) );
  
				if (pBufArtMol->m_sIDArticulo == pInfoMolArt->m_sIDArticulo )
				{
					bInMatrix = TRUE;
					break;
				}
			}
			if (!bInMatrix)
			{
				CBufArticulo* pBufArticulo = g_pCache->FindpArticulo(pBufArtMol->m_sIDArticulo);
				if (pBufArticulo) 
				{
					CInfoMolArt* pInfoMolArt = new CInfoMolArt;
					pInfoMolArt->m_sIDArticulo = pBufArtMol->m_sIDArticulo;
					pInfoMolArt->m_lPrioridad = lPrioridad;
					pInfoMolArt->m_lPorcHist = 0;
					p_ArrInfoMolArt.Add(pInfoMolArt);
				}
			}
		}

	}
	return p_ArrInfoMolArt.GetSize();

}
*/

CBufLocal*
CCache::FindpLocal(const CString &p_sLocal)
{
	int iLim = m_pArrLocales->GetSize();
	for(int i = 0; i < iLim ; i++)
	{
		CBufLocal* pBufLocal = (CBufLocal*) m_pArrLocales->GetAt(i);
		ASSERT_VALID(pBufLocal);
		if (pBufLocal->m_sID == p_sLocal) return pBufLocal;
	}
	return NULL;
}

int
CCache::GetiDiasMezcla(const CString &p_sLocal)
{
	CBufLocal* pBufLocal = FindpLocal(p_sLocal);
	if (pBufLocal) return pBufLocal->m_iDiasTransporte;
	else return 0;
}

int
CCache::GetiDiasRetorno(const CString &p_sLocal)
{
	CBufLocal* pBufLocal = FindpLocal(p_sLocal);
	if (pBufLocal) return pBufLocal->m_iDiasRetorno;
	else return 0;
}

int
CCache::GetiFrecuenciaMezcla(const CString &p_sLocal)
{
	CBufLocal* pBufLocal = FindpLocal(p_sLocal);
	if (pBufLocal) return pBufLocal->m_iFrecuencia;
	else return 0;
}

CBufArticulo* 
CCache::FindpArticulo(const CString &p_sArticulo)
{
	CBufArticulo BufArt;
	BufArt.m_sID = p_sArticulo;
	int iRes = FindOrd(m_pArrArticulos, &BufArt, BufArt_Equal, BufArt_Bigger );
	if (iRes >= 0) return (CBufArticulo*) m_pArrArticulos->GetAt(iRes);
	return NULL;
}


CBufMolde* 
CCache::FindpMolde(const CString &p_sMolde)
{
	int iLim = m_pArrMoldes->GetSize();
	for(int i = 0; i < iLim ; i++)
	{
		CBufMolde* pBufMoldes = (CBufMolde*) m_pArrMoldes->GetAt(i);
		ASSERT_VALID(pBufMoldes);
		if (pBufMoldes->m_sID == p_sMolde) return pBufMoldes;
	}
	return NULL;
}

CBufMezcla* 
CCache::FindpMezcla(const CString &p_sMezcla)
{
	int iLim = m_pArrMezclas->GetSize();
	for(int i = 0; i < iLim ; i++)
	{
		CBufMezcla* pBufMezclas = (CBufMezcla*) m_pArrMezclas->GetAt(i);
		ASSERT_VALID(pBufMezclas);
		if (pBufMezclas->m_sID == p_sMezcla) return pBufMezclas;
	}
	return NULL;
}

CBufMaquina* 
CCache::FindpMaquina(const CString &p_sMaquina)
{
	int iLim = m_pArrMaquinas->GetSize();
	for(int i = 0; i < iLim ; i++)
	{
		CBufMaquina* pBufMaquinas = (CBufMaquina*) m_pArrMaquinas->GetAt(i);
		ASSERT_VALID(pBufMaquinas);
		if (pBufMaquinas->m_sID == p_sMaquina) return pBufMaquinas;
	}
	return NULL;

}

long
CCache::GetlGFH(const CString& p_sMaquina)
{
	CBufMaquina* pBufM = FindpMaquina(p_sMaquina);
	if (pBufM) return pBufM->m_lGFH;
	else return 0;
}

void 
CCache::RecalcOFs(CProg *p_pProg, const char* p_sIDArt)
{
	CString sCodAnt = "";
	BOOL bRep = FALSE;
	long lCantAcum = 0;
	
	// Hacemos un ciclo por las OFs de Ayer
	int iNumOF = m_pArrOFAyer->GetSize();
	
	for(int ind = 0; ind < iNumOF; ind++)
	{
		CBufOF* pBufOF = (CBufOF*) m_pArrOFAyer->GetAt(ind);
		if (p_sIDArt != NULL && pBufOF->m_sIDArticulo != p_sIDArt) continue;
		
		// Calculamos las cantidades acumuladas
		long lNumProg = p_pProg->GetlCantArt(pBufOF->m_sIDArticulo);
		pBufOF->m_dNoProg = pBufOF->m_dPendAcum - 
							pBufOF->m_dCantAlmac - 
							pBufOF->m_dCantCurso -
							pBufOF->m_dContratas -
							(double) lNumProg;
							
		pBufOF->m_dNoCubierta = pBufOF->m_dPendAcum - 
							pBufOF->m_dCantAlmac - 
							pBufOF->m_dCantCurso -
							pBufOF->m_dContratas
							;
							
			
	}
	
	// Hacemos un ciclo por las OFs
	iNumOF = GetNumOF();
	
	for(int ind = 0; ind < iNumOF; ind++)
	{
		CBufOF* pBufOF = GetpOFAt(ind);
		CBufArticulo* pBufArticulo = pBufOF->m_pBufArt;
		if (p_sIDArt != NULL && pBufOF->m_sIDArticulo != p_sIDArt) continue;
		
		// Calculamos las cantidades acumuladas
	
		CBufArticulo* pBufArt = pBufOF->m_pBufArt;
		
		long lNumProg = p_pProg->GetlCantArt(pBufOF->m_sIDArticulo);
		pBufOF->m_dNoProg = pBufOF->m_dPendAcum - 
							pBufOF->m_dCantAlmac - 
							pBufOF->m_dCantCurso - 
							pBufOF->m_dContratas -
							(double) lNumProg;
		if (pBufArticulo)
		{
			if (pBufOF->m_dNoProg > 0 && pBufArticulo->GetdCadencia() > 0)
			{
				pBufOF->m_lHorasAcum = (long) ( pBufOF->m_dNoProg / 
								pBufArticulo->GetdCadencia() * (1 - (TK_INACT_DEF / 100))) + 1;
			} else
				pBufOF->m_lHorasAcum = 0;
		}

		pBufOF->m_dNoCubierta = pBufOF->m_dPendAcum - 
							pBufOF->m_dCantAlmac - 
							pBufOF->m_dCantCurso -
							pBufOF->m_dContratas;
		
		if (pBufOF->m_pBufArt)
		{
			pBufOF->m_FecFab =
				p_pProg->CalcFecParaCantTL(p_pProg->GetpTL(pBufOF->m_pBufArt, Margen), (long) (pBufOF->m_dNoProg + (double) lNumProg));
		}
		else
		{
			pBufOF->m_FecFab =
				p_pProg->CalcFecParaCantTL(p_pProg->GetpTL(pBufOF->m_sIDArticulo, Margen), (long) (pBufOF->m_dNoProg + (double) lNumProg));
		}
		if (pBufOF->m_FecFab == p_pProg->GetFecFin())
		{
			pBufOF->m_lMargenMed = INT_MIN;
			pBufOF->m_lMargenMin = INT_MIN;
			pBufOF->m_lMargenMax = INT_MIN;
		} else if (pBufOF->m_FecFab == p_pProg->GetFecIni())
		{
			pBufOF->m_lMargenMed = INT_MAX;
			pBufOF->m_lMargenMin = INT_MAX;
			pBufOF->m_lMargenMax = INT_MAX;
		} else
		{
			CTimeSpan tsMargen = pBufOF->m_FecNec - pBufOF->m_FecFab;
			pBufOF->m_lMargenMed = (long) tsMargen.GetTotalHours() - pBufArt->m_lMaduracionMed;
			pBufOF->m_lMargenMin = (long) tsMargen.GetTotalHours() - pBufArt->m_lMaduracionMin;
			pBufOF->m_lMargenMax = (long) tsMargen.GetTotalHours() - pBufArt->m_lMaduracionMax;
		}
		// Ahora que tenemos el margen, se lo pasamos a las OMs
		
		CTimeLine* pTL;
		// Esto no se puede hacer al poder haber varios progs y s�lo un gcache
		//			if (pBufOF->m_pBufArt)
		//			{
		//				pTL = pBufOF->m_pBufArt->m_pTL;
		//			}
		//			else
		//			{
		pTL = p_pProg->GetpTL(pBufOF->m_sIDArticulo);
		//			}
		if (pTL)
		{		
			{
				int iNumPS = pTL->m_pArrOM->GetSize();
				for (int iD=0; iD< iNumPS; iD++)
				{
					COrdMaq* pOM = (COrdMaq*) pTL->m_pArrOM->GetAt(iD);;
					if (pBufOF->m_lMargenMed != INT_MAX && pBufOF->m_lMargenMed != INT_MIN)
					{
						if ( pOM->GetTsMargen() >= (pBufOF->m_lMargenMed * TK_SEGS_HORA ))
							pOM->SetTsMargen((CTimeSpan) (pBufOF->m_lMargenMed * TK_SEGS_HORA ));
					}
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// COFComb

IMPLEMENT_SERIAL(COFComb, CObject, 1)

COFComb::COFComb()
{
}


COFComb::~COFComb()
{
}


/////////////////////////////////////////////////////////////////////////////
// COFComb diagnostics

#ifdef _DEBUG
void COFComb::AssertValid() const
{
	CObject::AssertValid();
}

void COFComb::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COFComb serialization

void COFComb::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// Funciones de ordenacion por seccion

BOOL 
OFSeccion_Bigger( CObject* p_pA, CObject* p_pB )
{
	CBufOF* pOFa = (CBufOF*) p_pA;
	CBufOF* pOFb = (CBufOF*) p_pB;
	if (pOFa->m_lSeccion > pOFb->m_lSeccion ) return TRUE;
	if (pOFa->m_lSeccion == pOFb->m_lSeccion && 
		pOFa->m_sIDArticulo > pOFb->m_sIDArticulo ) return TRUE;
	if (pOFa->m_lSeccion == pOFb->m_lSeccion && 
		pOFa->m_sIDArticulo == pOFb->m_sIDArticulo &&
		pOFa->m_FecNec > pOFb->m_FecNec ) return TRUE;
	if (pOFa->m_lSeccion == pOFb->m_lSeccion && 
		pOFa->m_sIDArticulo == pOFb->m_sIDArticulo &&
		pOFa->m_FecNec == pOFb->m_FecNec ) return TRUE;
	return FALSE;

}

BOOL 
OFSeccion_Equal( CObject* p_pA, CObject* p_pB )
{
	CBufOF* pOFa = (CBufOF*) p_pA;
	CBufOF* pOFb = (CBufOF*) p_pB;
	return (pOFa->m_lSeccion == pOFb->m_lSeccion && 
			pOFa->m_sIDArticulo == pOFb->m_sIDArticulo &&
			pOFa->m_FecNec == pOFb->m_FecNec );
}

BOOL 
BufArt_Bigger( CObject* p_pA, CObject* p_pB )
{
	CBufArticulo* pBufArta = (CBufArticulo*) p_pA;
	CBufArticulo* pBufArtb = (CBufArticulo*) p_pB;
	return pBufArta->m_sID > pBufArtb->m_sID;
}

BOOL 
BufArt_Equal( CObject* pBufArtA, CObject* pBufArtB )
{
	CBufArticulo* pBufArta = (CBufArticulo*) pBufArtA;
	CBufArticulo* pBufArtb = (CBufArticulo*) pBufArtB;
	return pBufArta->m_sID == pBufArtb->m_sID;

}





void CCache::SetOrdenOF()
{
	// Tenemos dos arrays, uno ordenado por secci�n y otro por art�culo
	// Elegimos el que corresponde
	CObArray* pObArrayOF;
	if (g_VerOF.m_OrdenOF == Seccion)
		pObArrayOF = m_pArrOFSeccion;
	else
		pObArrayOF = m_pArrOFs;
	
	
	m_pArrOFSel->RemoveAll();
	m_pArrOFAcum->RemoveAll();
	int iLim = pObArrayOF->GetSize();
	
	CBufOF* pBufOFNext;
	
	for(int i=0; i < iLim; i++)
	{
		CBufOF* pBufOF = (CBufOF*) pObArrayOF->GetAt(i);
		if (i < (iLim-1)) pBufOFNext = (CBufOF*) pObArrayOF->GetAt(i+1);
		else pBufOFNext = NULL;
		
		if (g_VerOF.m_bSelecFec)
			if (pBufOF->m_FecNec > g_VerOF.m_FecSelec ) continue;
			
		if (g_VerOF.m_bSoloNoProg)
			if (pBufOF->m_dNoProg <= 0) continue;

		if (g_VerOF.m_bSoloNoFab)
			if (pBufOF->m_lMargenMed != INT_MIN) continue;

		m_pArrOFSel->Add(pBufOF);

		if ( pBufOFNext)
		{
			if ( pBufOF->m_sIDArticulo != pBufOFNext->m_sIDArticulo ||
				 pBufOF->m_FecNec != pBufOFNext->m_FecNec ) 
			{
				if  (g_VerOF.Selec())
				{
					if ( pBufOF->m_dNoProg > 0) 
					{
						m_pArrOFAcum->Add(pBufOF);
					}
				} else
				{
						m_pArrOFAcum->Add(pBufOF);
				}
			}
		} else
		{
			m_pArrOFAcum->Add(pBufOF);
		}
			
	}
}

void CCache::ActLimitMaq(const CString &p_sMolde, const CString &p_sMaq)
{
	CString sMsg;
	sMsg.Format( STRID( IDS_CONF_ACTLIMIT ), p_sMolde, p_sMaq);
	
	if (AfxMessageBox(sMsg, MB_YESNO)  != IDYES)
		return;
	
	if (m_RsMolMaq.IsOpen()) m_RsMolMaq.Close();
	if (!m_RsMolMaq.Open(CRecordset::dynaset)) return;
	if (!m_RsMolMaq.CanUpdate())
	{	
		TRACE(STRID(IDS_ERR_NOACT));
		AfxMessageBox(STRID(IDS_ERR_NOACT));
		return;
	} 
	CBufMolde* pBufMolde = FindpMolde(p_sMolde);
	int iUltMolde = 1;
	if (pBufMolde)
	{
		int iLimMolMaq = pBufMolde->m_pArrMolMaq->GetSize();
		for (int j = 0; j < iLimMolMaq; j++)
		{
			CBufMolMaq* pBufMolMaq = (CBufMolMaq*) pBufMolde->m_pArrMolMaq->GetAt(j);
			ASSERT_VALID(pBufMolMaq);
			if (pBufMolMaq->m_lPrioridad >= iUltMolde) iUltMolde = pBufMolMaq->m_lPrioridad + 1;
		}
		
		// A�adimos el nuevo al cache

		CBufMolMaq* pBufMolMaq = new CBufMolMaq;
		pBufMolMaq->m_sIDMaquina = p_sMaq;
		pBufMolMaq->m_sIDMolde = p_sMolde;
		pBufMolMaq->m_pBufMaquina = FindpMaquina( p_sMaq );
		pBufMolMaq->m_pBufMolde = FindpMolde( p_sMolde );
		pBufMolMaq->m_lPrioridad = iUltMolde;
		pBufMolde->m_pArrMolMaq->Add(pBufMolMaq);
	}

	// Y a la base de datos

	m_RsMolMaq.AddNew();
	
	m_RsMolMaq.m_IDMaquina = p_sMaq;
	m_RsMolMaq.m_IDMolde = p_sMolde;
	m_RsMolMaq.m_Prioridad = 0;
	
	m_RsMolMaq.Update();

	m_RsMolMaq.Close();
	
}

static int s_iGetCambioMolde_count = 0;
CBufCambiosMoldes*
CCache::GetFirstCambioMolde(CTime p_Fec)
{

	m_pArrCambMold->RemoveAll();
	
	m_RsCambiosMoldes.m_strFilter = "";
	m_RsCambiosMoldes.m_strSort = "Contador";
                                
	if (m_RsCambiosMoldes.IsOpen()) m_RsCambiosMoldes.Close();
	if (!m_RsCambiosMoldes.Open(CRecordset::dynaset)) return NULL;
	if (!m_RsCambiosMoldes.GetRecordCount()) return NULL;
	
	m_RsCambiosMoldes.MoveFirst();
		 		
	while (!m_RsCambiosMoldes.IsEOF())
	{   
		CBufCambiosMoldes* pNewCM = new CBufCambiosMoldes(m_RsCambiosMoldes);
		m_pArrCambMold->Add(pNewCM);
		m_RsCambiosMoldes.MoveNext();
	}            
	m_RsCambiosMoldes.Close();
	m_RsCambiosMoldes.m_strFilter = "";
	s_iGetCambioMolde_count = 0;

	return GetNextCambioMolde(p_Fec);
}


CBufCambiosMoldes*
CCache::GetNextCambioMolde(CTime p_Fec)
{
	if (s_iGetCambioMolde_count < m_pArrCambMold->GetSize())
	{
		s_iGetCambioMolde_count++;
		return (CBufCambiosMoldes*) m_pArrCambMold->GetAt(s_iGetCambioMolde_count-1);
	}
	else
		return NULL;
}

// UpdateMezclasPlanta : Actualizar los datos de Mezcla de plantas
void
CCache::UpdateMezclasPlanta()
{
    if (m_RsMezclasPlanta.IsOpen())
		m_RsMezclasPlanta.Close();
	m_RsMezclasPlanta.m_strFilter = "";

	if (m_RsMezclasPlanta.Open(CRecordset::dynaset))
	{
		if (!m_RsMezclasPlanta.GetRecordCount()) return;
		m_RsMezclasPlanta.MoveFirst();
 		
		while (!m_RsMezclasPlanta.IsEOF())
		{
			for (int i = 0; i < m_pArrMezclas->GetSize(); i++)
			{
				CBufMezcla* pBufMezcla = (CBufMezcla*) m_pArrMezclas->GetAt(i);
				ASSERT( pBufMezcla->IsKindOf( RUNTIME_CLASS( CBufMezcla ) ) );
				if (pBufMezcla->m_sID == m_RsMezclasPlanta.m_ID)
					pBufMezcla->m_lCantPlanta = m_RsMezclasPlanta.m_Cantidad;
	
			}
			m_RsMezclasPlanta.MoveNext();
		}
	}            
	m_RsMezclasPlanta.Close();
}

// CargaLisMezclas : Carga la tabla del listado de Mezclas desde un Array de BufLisMezclas
void CCache::CargaLisMezclas(CObArray* p_pArrBufLisMezclas)
{
	if (m_RsLisMezclas.IsOpen())
		m_RsLisMezclas.Close();
	// Borramos lo que pueda haber de antes
	CString strCmd;
	strCmd.Format("DELETE * FROM Lis_Mezclas");
	m_pDB->ExecuteSQL(strCmd);
	// Insertamos un registro "Dummy" porque las fechas no se insertan bien en Recordsets vac�os
	strCmd.Format("INSERT INTO Lis_Mezclas VALUES ('***DUMMY', #01/01/01#, '0', '0', 0, 0)");
	m_pDB->ExecuteSQL(strCmd);

	
	m_RsLisMezclas.m_strFilter = "";
	
	if (!m_RsLisMezclas.Open(CRecordset::dynaset)) return;
	for (int i = 0; i < p_pArrBufLisMezclas->GetSize(); i++)
	{
		CBufLisMezclas* pBufLisMezclas = (CBufLisMezclas*) p_pArrBufLisMezclas->GetAt(i);
		ASSERT( pBufLisMezclas->IsKindOf( RUNTIME_CLASS( CBufLisMezclas ) ) );
		m_RsLisMezclas.AddNew();

		m_RsLisMezclas.m_IDLocal = pBufLisMezclas->m_sIDLocal;
		m_RsLisMezclas.m_IDArticulo = pBufLisMezclas->m_sIDArticulo;
		m_RsLisMezclas.m_IDCliente = pBufLisMezclas->m_sIDCliente;
		m_RsLisMezclas.m_FecIni = pBufLisMezclas->m_FecIni;
		m_RsLisMezclas.m_Mezcla = pBufLisMezclas->m_lMezcla;
		m_RsLisMezclas.m_Cant = pBufLisMezclas->m_lCant;
		
		m_RsLisMezclas.Update();
	}

	m_RsLisMezclas.Close();

	// Borramos el registro "Dummy"
	strCmd.Format("DELETE * FROM Lis_Mezclas WHERE [IDLocal] = '***DUMMY'");
	m_pDB->ExecuteSQL(strCmd);

}

// CargaLisMezclas : Carga la tabla del listado de Mezclas desde un Array de BufLisMezclas
void CCache::CargaLisGeneral(const CListCtrl* p_pListCtrl)
{
	_TCHAR szBuff[MAX_PATH];

	if (m_RsLisGeneral.IsOpen())
		m_RsLisGeneral.Close();
	// Borramos lo que pueda haber de antes
	CString strCmd;
	strCmd.Format("DELETE * FROM Lis_General");
	m_pDB->ExecuteSQL(strCmd);
	
	m_RsLisGeneral.m_strFilter = "";
	
	if (!m_RsLisGeneral.Open(CRecordset::dynaset)) return;
	
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;
	
	CString sLastVal = "";
	for (int iItem = 0; iItem < p_pListCtrl->GetItemCount(); iItem++)
	{
		m_RsLisGeneral.AddNew();
		for(int iColumn = 0; p_pListCtrl->GetColumn(iColumn, &lvc) && iColumn <= m_ciNumVal; iColumn++)
		{
			int nRetLen = p_pListCtrl->GetItemText(iItem, iColumn, szBuff, sizeof(szBuff));
			if (iColumn == 0)
			{
				if (nRetLen == 0)			
				{
					strncpy(szBuff, sLastVal, MAX_PATH);
					nRetLen = strlen(szBuff);
				}
				else 
				{
					sLastVal = szBuff;
				}
			} else
				if (nRetLen == 0) continue;

			*(m_RsLisGeneral.m_sArrVal[iColumn]) = szBuff;			
		}
		m_RsLisGeneral.Update();
	}
	
	m_RsLisGeneral.Close();
	
	// Esto de ejecutar el SQL es porque le sienta bien a la base de datos.
	// Si no, a veces se come el �ltimo registro.
	strCmd.Format("DELETE * FROM Lis_General WHERE [Texto1] = '***DUMMY'");
	m_pDB->ExecuteSQL(strCmd);


}

void CCache::AddHistorico(COrdMaq* p_pOM)
{
	BOOL bVacio = !m_RsHistorico.GetRecordCount();
	if (bVacio)
	{
		m_RsHistorico.Close();
		CString strCmd;
		// Insertamos un registro "Dummy" porque las fechas no se insertan bien en Recordsets vac�os
		strCmd.Format("INSERT INTO Historico VALUES ('***DUMMY', ' ', #01/01/01#, #01/01/01#, 0, ' ',0,0)");
		TRY
		{
			m_pDB->ExecuteSQL(strCmd);
			m_RsHistorico.Open(CRecordset::dynaset);

		}
		    CATCH_ALL(e)
		{
			AfxMessageBox("Error al actualizar el hist�rico.");
		}
		END_CATCH_ALL
		}
	
	m_RsHistorico.m_strFilter = "";
	
  		
  TRY
    {
		m_RsHistorico.AddNew();
		
		m_RsHistorico.m_IDMolde = p_pOM->GetsMolde();
		m_RsHistorico.m_IDMaquina = p_pOM->GetsMaquina();
		m_RsHistorico.m_FecIni = p_pOM->GetFecIni();
		m_RsHistorico.m_FecFin = p_pOM->GetFecFin();
		m_RsHistorico.m_Cantidad = p_pOM->GetlCantidad() + p_pOM->GetlCantFab();
		m_RsHistorico.m_IDArticulo = p_pOM->GetsID();
		m_RsHistorico.m_Cadencia = p_pOM->GetdCadenciaGFH();
		m_RsHistorico.m_Rechazos = p_pOM->GetdChatarras();
		
		m_RsHistorico.Update();
	}
	    CATCH_ALL(e)
    {
		AfxMessageBox("El Hist�rico no se Actualiz�, normalmente por tratar de guardar el mismo bloque 2 veces.");
	}
    END_CATCH_ALL

	if (bVacio)
	{
		CString strCmd;
		// Borramos el registro "Dummy"
		strCmd.Format("DELETE * FROM Historico WHERE [IDMolde] = '***DUMMY'");
		m_pDB->ExecuteSQL(strCmd);
	}


}

BOOL 
CCache::LimitMolMaq(const CString &p_sIDMolde, const CString &p_sIDMaquina)
{
	CBufMolde* pBufMolde = FindpMolde(p_sIDMolde);
	ASSERT_VALID(pBufMolde);
	if (!pBufMolde) return FALSE;
	int iLimMolMaq = pBufMolde->m_pArrMolMaq->GetSize();
	for (int j = 0; j < iLimMolMaq; j++)
	{
		CBufMolMaq* pBufMolMaq = (CBufMolMaq*) pBufMolde->m_pArrMolMaq->GetAt(j);
		ASSERT_VALID(pBufMolMaq);
		if (pBufMolMaq->m_sIDMaquina == p_sIDMaquina ) return FALSE;
	}
	return TRUE;
}


BOOL 
CCache::EsMoldeComun(const CString &p_sIDA, const CString &p_sIDB)
{
	if ( (p_sIDA == "02777311" && p_sIDB == "02777321") || 
		(p_sIDA == "02777321" && p_sIDB == "02777311") )

		return TRUE;
	else
		return FALSE;
}


void
CCache::EscribirFechaParam(const CString& p_sClave)
{
	CString sCmd;

	if (LeerFechaParam(p_sClave) == "") {
		sCmd = "INSERT INTO Param_Fechas VALUES ('" + p_sClave + "', Now())";
	} else {
		sCmd = "UPDATE Param_Fechas SET FECHA = Now() WHERE CLAVE = '" + p_sClave + "'";
	}
	
	m_pDB->ExecuteSQL(sCmd);

}

CString
CCache::LeerFechaParam(const CString& p_sClave)
{
	
	CString selQuery = "SELECT FECHA";
			selQuery += " FROM Param_Fechas";
			selQuery += " WHERE CLAVE = '" + p_sClave + "'";

	 return GetStringValue(selQuery);


}

CString
CCache::GetStringValue(CString& selQuery)
{
	CRecordset rs(m_pDB);
	rs.Open(CRecordset::forwardOnly, selQuery, CRecordset::readOnly);
	if (rs.IsEOF()) return "";
	    
	CString sResult = "";
	rs.GetFieldValue((short) 0, sResult);
	rs.Close();

	return sResult;
}

void 
CCache::IniFillHomologadas()
{

	CString sFilter;
	BOOL bEntregas = TRUE;
	
	//AfxMessageBox("Entrando a leer Homologadas");
    
	try 
	{
	CRsHomologadas m_RsHomologadas(m_pDB);

	CString sEmpresa;
	//sEmpresa.Format("Empresa = %d",GetNumEmpresa(g_sEmpresa));
	//m_RsHomologadas.m_strFilter = sEmpresa;
	m_RsHomologadas.m_strSort = "Articulo, Empresa";
	
	if (m_RsHomologadas.IsOpen()) m_RsHomologadas.Close();
	
	if (!m_RsHomologadas.Open(CRecordset::dynaset)) return;

	int iCnt = m_RsHomologadas.GetRecordCount();
	
	//CString sMsg;
	//sMsg.Format("Registros : %d",iCnt);
	//AfxMessageBox(sMsg);
    
	if (iCnt)
	{
		m_RsHomologadas.MoveFirst();
		
		while (!m_RsHomologadas.IsEOF())
		{   
			CBufHomologada* pBufHomologada = new CBufHomologada( m_RsHomologadas );
			pBufHomologada->m_sArticulo.TrimRight();
			m_pArrHomologadas->Add(pBufHomologada);
			m_RsHomologadas.MoveNext();
		} 
		
	}

	m_RsHomologadas.Close();
	} catch (...) // Por si no existe la tabla "Homologadas"
	{
		return;
	//	AfxMessageBox("Error  al leer Homologadas");
    
	}
	
	CString sMsg;
	sMsg.Format("Cargados : %d",m_pArrHomologadas->GetSize());
	
	//AfxMessageBox(sMsg);
    //AfxMessageBox("Saliendo de leer Homologadas");
    
}

bool 
CCache::GetNoHomologada(CString& p_sArticulo)
{
	for( int i=0; i < m_pArrHomologadas->GetSize() ; i++ )
    {
		CBufHomologada* pBufHomologada = (CBufHomologada*) m_pArrHomologadas->GetAt(i);
		ASSERT( pBufHomologada->IsKindOf( RUNTIME_CLASS( CBufHomologada ) ) );
		if (pBufHomologada->m_sArticulo == p_sArticulo) {
			if (pBufHomologada->m_sTipo == "A" && pBufHomologada->m_sHomologacion == "NH" ) {
				return true;
			}
		}
   }      
    return false;

}

CString
CCache::GetsHomologacion(CString& p_sArticulo, bool trace)
{
	
	if (trace){
		CString sMsg;
		sMsg.Format("%d",m_pArrHomologadas->GetSize());
		AfxMessageBox(sMsg);
	}
		
	p_sArticulo.TrimRight();
	for( int i=0; i < m_pArrHomologadas->GetSize() ; i++ )
    {
		CBufHomologada* pBufHomologada = (CBufHomologada*) m_pArrHomologadas->GetAt(i);
		if (trace)AfxMessageBox(pBufHomologada->m_sArticulo + " - " + p_sArticulo);
		if (pBufHomologada->m_sArticulo == p_sArticulo) {
			return pBufHomologada->m_sHomologacion;
		}
   }      
   return "   ";

}

// Obtiene el responsable de fabricaci�n. Est� metido en la tabla de Homologaciones
// porque era m�s f�cil en su momento. La tabla de homologaciones deber�a cambiar el nombre
// a algo como DatosAuxArt o similar. 
// Devuelve -1 si no existe.
CString
CCache::GetsPropietario(CString& p_sArticulo)
{
	p_sArticulo.TrimRight();
	for( int i=0; i < m_pArrHomologadas->GetSize() ; i++ )
    {
		CBufHomologada* pBufHomologada = (CBufHomologada*) m_pArrHomologadas->GetAt(i);
		if (pBufHomologada->m_sArticulo == p_sArticulo)
		 {
			if (pBufHomologada->m_lRespFab == 10) 
				return "IND";
			else if (pBufHomologada->m_lRespFab == 20)
				return "PRO";
			else {
				CString sErr;
				sErr.Format("%ld",pBufHomologada->m_lRespFab);
				return sErr;
			}
		}
   }      
   return "   ";

}