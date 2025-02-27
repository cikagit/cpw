// xcpdb.cpp : implementation file
//

#include "stdafx.h"
#include "cpw.h"
#include "AbSpan.h"
#include "cpwDb.h"
#include <strstream>
#include "cpwExt.h"
#include "cache.h"  
#include "cpwDefs.h"

#include "ProgMaq.h"
#include "OrdMaq.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRsArticulo

IMPLEMENT_DYNAMIC(CRsArticulo, CRecordset)

CRsArticulo::CRsArticulo(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsArticulo)
	m_CantAlmacen = 0;
	m_CantFabric = 0;
	m_CantPauta = 0;
	m_CentroReb = 0;
	m_ID = _T("");
	m_MaduracionMax = 0;
	m_MaduracionMed = 0;
	m_MaduracionMin = 0;
	m_Mezcla = _T("");
	m_P_H_iny = 0.0;
	m_PCAct = 0.0;
	m_PCBase = 0.0;
	m_PCHist = 0.0;
	m_PCUlt = 0.0;
	m_Peso = 0.0;
	m_PHAct = 0.0;
	m_PHBase = 0.0;
	m_PHHist = 0.0;
	m_PHUlt = 0.0;
	m_PiezasHoraReb = 0.0;
	m_PorcChatarras = 0.0;
	m_Planificador = _T("");
	m_nFields = 22;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsArticulo::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsArticulo::GetDefaultSQL()
{
	return _T("[Articulos]");
}

void CRsArticulo::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsArticulo)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[CantAlmacen]"), m_CantAlmacen);
	RFX_Long(pFX, _T("[CantFabric]"), m_CantFabric);
	RFX_Long(pFX, _T("[CantPauta]"), m_CantPauta);
	RFX_Long(pFX, _T("[CentroReb]"), m_CentroReb);
	RFX_Text(pFX, _T("[ID]"), m_ID);
	RFX_Long(pFX, _T("[MaduracionMax]"), m_MaduracionMax);
	RFX_Long(pFX, _T("[MaduracionMed]"), m_MaduracionMed);
	RFX_Long(pFX, _T("[MaduracionMin]"), m_MaduracionMin);
	RFX_Text(pFX, _T("[Mezcla]"), m_Mezcla);
	RFX_Double(pFX, _T("[P/H iny]"), m_P_H_iny);
	RFX_Double(pFX, _T("[PCAct]"), m_PCAct);
	RFX_Double(pFX, _T("[PCBase]"), m_PCBase);
	RFX_Double(pFX, _T("[PCHist]"), m_PCHist);
	RFX_Double(pFX, _T("[PCUlt]"), m_PCUlt);
	RFX_Double(pFX, _T("[Peso]"), m_Peso);
	RFX_Double(pFX, _T("[PHAct]"), m_PHAct);
	RFX_Double(pFX, _T("[PHBase]"), m_PHBase);
	RFX_Double(pFX, _T("[PHHist]"), m_PHHist);
	RFX_Double(pFX, _T("[PHUlt]"), m_PHUlt);
	RFX_Double(pFX, _T("[PiezasHoraReb]"), m_PiezasHoraReb);
	RFX_Double(pFX, _T("[PorcChatarras]"), m_PorcChatarras);
	RFX_Text(pFX, _T("[Planificador]"), m_Planificador);
	//}}AFX_FIELD_MAP
	m_ID.TrimRight();
	m_Mezcla.TrimRight();
}

/////////////////////////////////////////////////////////////////////////////
// CRsArticulo diagnostics

#ifdef _DEBUG
void CRsArticulo::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsArticulo::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBufArticulo

IMPLEMENT_DYNAMIC(CBufArticulo, CObject)

#ifdef _DEBUG
void CBufArticulo::AssertValid() const
{
	// Call inherited AssertValid first.
	CObject::AssertValid();

	ASSERT_VALID(m_pArrArtMol);
	ASSERT_VALID(m_pArrAcabado);
	ASSERT_VALID(m_pArrComponentes);
	
}
#endif

CBufArticulo::CBufArticulo(CRsArticulo& p_RsArticulos)
	: CObject()
{
	AssignData(p_RsArticulos);

	m_pArrArtMol = new CObArray();
	m_pArrAcabado = new CObArray();
	m_pArrComponentes = new CObArray();
}
                               
void CBufArticulo::AssignData(CRsArticulo& p_RsArticulos)
{
	// TODO
	// Pensar en los posibles problemas de los valores nulos
	m_lCantAlmacen = p_RsArticulos.m_CantAlmacen;
	m_lCantFabric = p_RsArticulos.m_CantFabric;
	m_lCantPauta = p_RsArticulos.m_CantPauta;
	m_lCentroReb = p_RsArticulos.m_CentroReb;
	m_sID = p_RsArticulos.m_ID;
	m_lMaduracionMax = p_RsArticulos.m_MaduracionMax;
	m_lMaduracionMed = p_RsArticulos.m_MaduracionMed;
	m_lMaduracionMin = p_RsArticulos.m_MaduracionMin;
	m_sMezcla = p_RsArticulos.m_Mezcla;
	
	if (p_RsArticulos.IsFieldNull(&p_RsArticulos.m_PCAct )) m_dPCAct =0;
	else m_dPCAct = p_RsArticulos.m_PCAct;
	if (p_RsArticulos.IsFieldNull(&p_RsArticulos.m_PCBase )) m_dPCBase =0;
	else m_dPCBase = p_RsArticulos.m_PCBase;
	if (p_RsArticulos.IsFieldNull(&p_RsArticulos.m_PCHist )) m_dPCHist =0;
	else m_dPCHist = p_RsArticulos.m_PCHist;
	if (p_RsArticulos.IsFieldNull(&p_RsArticulos.m_PCUlt )) m_dPCUlt =0;
	else m_dPCUlt = p_RsArticulos.m_PCUlt;
	if (p_RsArticulos.IsFieldNull(&p_RsArticulos.m_Peso )) m_dPeso =0;
	else m_dPeso = p_RsArticulos.m_Peso;
	if (p_RsArticulos.IsFieldNull(&p_RsArticulos.m_PHAct )) m_dPHAct =0;
	else m_dPHAct = p_RsArticulos.m_PHAct;
	if (p_RsArticulos.IsFieldNull(&p_RsArticulos.m_PHBase )) m_dPHBase =0;
	else m_dPHBase = p_RsArticulos.m_PHBase;
	if (p_RsArticulos.IsFieldNull(&p_RsArticulos.m_PHHist )) m_dPHHist =0;
	else m_dPHHist = p_RsArticulos.m_PHHist;
	if (p_RsArticulos.IsFieldNull(&p_RsArticulos.m_PHUlt )) m_dPHUlt =0;
	else m_dPHUlt = p_RsArticulos.m_PHUlt;
	if (p_RsArticulos.IsFieldNull(&p_RsArticulos.m_P_H_iny )) m_dP_H_iny =0;
	else m_dP_H_iny = p_RsArticulos.m_P_H_iny;
	 
	if (p_RsArticulos.IsFieldNull(&p_RsArticulos.m_PiezasHoraReb )) m_dPiezasHoraReb =0;
	else m_dPiezasHoraReb = p_RsArticulos.m_PiezasHoraReb;
	if (p_RsArticulos.IsFieldNull(&p_RsArticulos.m_PorcChatarras )) m_dPorcChatarras =0;
	else m_dPorcChatarras = p_RsArticulos.m_PorcChatarras;
	
	m_LV_sCliente = "";
	m_LV_dP_H_real = 0;
	m_LV_dRechazos = 0;
	m_LV_lCantidad_Fabricada = 0;
	m_LV_lCantidad_Rechazada = 0;
	m_LV_lTotal_Horas = 0;
	m_sPlanificador = p_RsArticulos.m_Planificador;

}

CBufArticulo::CBufArticulo()
	: CObject()
{
	m_pArrArtMol = new CObArray();
	m_pArrAcabado = new CObArray();
	m_pArrComponentes = new CObArray();
	m_pBufMezcla = NULL;
	
	m_lCantAlmacen = 0;
	m_lCantFabric = 0;
	m_lCantPauta = 0;
	m_lCentroReb = 0;
	m_sID = "";
	m_lMaduracionMax = 0;
	m_lMaduracionMed = 0;
	m_lMaduracionMin = 0;
	m_sMezcla = "";
	m_dP_H_iny = 0;
	
	m_dPCAct = 0;
	m_dPCBase = 0;
	m_dPCHist = 0;
	m_dPCUlt = 0;
	m_dPeso = 0;
	m_dPHAct = 0;
	m_dPHBase = 0;
	m_dPHHist = 0;
	m_dPHUlt = 0;
	m_dPiezasHoraReb = 0;
	m_dPorcChatarras = 0;
	
	m_LV_sCliente = "";
	m_LV_dP_H_real = 0;
	m_LV_dRechazos = 0;
	m_LV_lCantidad_Fabricada = 0;
	m_LV_lCantidad_Rechazada = 0;
	m_LV_lTotal_Horas = 0;
	m_sPlanificador = "";

}

CBufArticulo::~CBufArticulo()
{
	ClearArray(*m_pArrArtMol, RUNTIME_CLASS(CBufArtMol));
	delete m_pArrArtMol;	
	ClearArray(*m_pArrAcabado, RUNTIME_CLASS(CBufAcabado));
	delete m_pArrAcabado;	
	ClearArray(*m_pArrComponentes, RUNTIME_CLASS(CBufComponentes));
	delete m_pArrComponentes;
}                              


double 
CBufArticulo::GetdChatarras()
{

	if (m_dPorcChatarras) return m_dPorcChatarras;
	
	return 2.5;
}

double 
CBufArticulo::GetdCadencia()
{
	if (m_dP_H_iny) return m_dP_H_iny;

	if (m_dPHBase) return m_dPHBase;
	if (m_dPHUlt) return m_dPHUlt;
	if (m_dPHAct) return m_dPHAct;
	if (m_dPHHist) return m_dPHHist;

	return 33;

}	

double 
CBufArticulo::GetdCadenciaAcabado(long p_lCentro)
{
	double dCadencia = 0;
	if (!m_pArrAcabado) return dCadencia;
	for (int i=0; i< m_pArrAcabado->GetSize(); i++)
	{                                  
		CBufAcabado* pBufAcabado = (CBufAcabado *) m_pArrAcabado->GetAt(i);
		if (p_lCentro < 0 || (p_lCentro == pBufAcabado->m_lCentro))
			dCadencia += ( 1.0 / pBufAcabado->m_dCadencia);
	}
	if (dCadencia > 0) return (1.0 / dCadencia);
	else return 0;

}

long 
CBufArticulo::GetlCantMezcla()
{
	if ( m_pBufMezcla )
	{
		return m_pBufMezcla->GetlCantidad();
	}
	else return 0;

	
}

/////////////////////////////////////////////////////////////////////////////
// CRsArtMol

IMPLEMENT_DYNAMIC(CRsArtMol, CRecordset)

CRsArtMol::CRsArtMol(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsArtMol)
	m_Cantidad = 0;
	m_IDArticulo = _T("");
	m_IDMolde = _T("");
	m_Orden = 0;
	m_Postizo = _T("");
	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsArtMol::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsArtMol::GetDefaultSQL()
{
	return _T("[ArtMol]");
}

void CRsArtMol::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsArtMol)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[Cantidad]"), m_Cantidad);
	RFX_Text(pFX, _T("[IDArticulo]"), m_IDArticulo);
	RFX_Text(pFX, _T("[IDMolde]"), m_IDMolde);
	RFX_Long(pFX, _T("[Orden]"), m_Orden);
	RFX_Text(pFX, _T("[Postizo]"), m_Postizo);
	//}}AFX_FIELD_MAP
	m_IDArticulo.TrimRight();
	m_Postizo.TrimRight();
}

/////////////////////////////////////////////////////////////////////////////
// CRsArtMol diagnostics

#ifdef _DEBUG
void CRsArtMol::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsArtMol::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CBufArtMol

IMPLEMENT_DYNAMIC(CBufArtMol, CObject)

CBufArtMol::CBufArtMol(CRsArtMol& p_RsArtMol)
	: CObject()
{


	m_sIDArticulo = p_RsArtMol.m_IDArticulo;
	m_sIDArticulo.TrimRight();
	m_sIDMolde = p_RsArtMol.m_IDMolde;
	m_sIDMolde.TrimRight();
	if (p_RsArtMol.IsFieldNull(&p_RsArtMol.m_Cantidad)) m_lCantidad = 0;
	else m_lCantidad = p_RsArtMol.m_Cantidad;
	m_lOrden = p_RsArtMol.m_Orden;
	if (p_RsArtMol.IsFieldNull(&p_RsArtMol.m_Postizo)) m_sPostizo = "";
	else m_sPostizo = p_RsArtMol.m_Postizo;

	m_pBufArticulo = NULL;
	m_pBufMolde = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CRsCliente

IMPLEMENT_DYNAMIC(CRsCliente, CRecordset)

CRsCliente::CRsCliente(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsCliente)
	m_ID = _T("");
	m_Nombre = _T("");
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsCliente::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsCliente::GetDefaultSQL()
{
	return _T("[Clientes]");
}

void CRsCliente::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsCliente)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Nombre]"), m_Nombre);
	//}}AFX_FIELD_MAP
	m_ID.TrimRight();
	m_Nombre.TrimRight();
}

/////////////////////////////////////////////////////////////////////////////
// CRsCliente diagnostics

#ifdef _DEBUG
void CRsCliente::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsCliente::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CRsEntregas

IMPLEMENT_DYNAMIC(CRsEntregas, CRecordset)

CRsEntregas::CRsEntregas(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsEntregas)
	m_IDMezcla = _T("");
	m_Orden = 0;
	m_Cantidad = 0;
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsEntregas::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsEntregas::GetDefaultSQL()
{
	return _T("[Entregas]");
}

void CRsEntregas::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsEntregas)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[IDMezcla]"), m_IDMezcla);
	RFX_Date(pFX, _T("[Fecha]"), m_Fecha);
	RFX_Long(pFX, _T("[Orden]"), m_Orden);
	RFX_Long(pFX, _T("[Cantidad]"), m_Cantidad);
	//}}AFX_FIELD_MAP
	m_IDMezcla.TrimRight();
}

/////////////////////////////////////////////////////////////////////////////
// CRsEntregas diagnostics

#ifdef _DEBUG
void CRsEntregas::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsEntregas::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CBufEntregas

IMPLEMENT_DYNAMIC(CBufEntregas, CObject)

CBufEntregas::CBufEntregas(CRsEntregas& p_RsEntregas)
	: CObject()
{

	m_sIDMezcla = p_RsEntregas.m_IDMezcla;
	m_lOrden = p_RsEntregas.m_Orden;
	m_Fecha = p_RsEntregas.m_Fecha;
	m_lCantidad = p_RsEntregas.m_Cantidad;
	
}





/////////////////////////////////////////////////////////////////////////////
// CRsHistorico

IMPLEMENT_DYNAMIC(CRsHistorico, CRecordset)

CRsHistorico::CRsHistorico(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsHistorico)
	m_IDMolde = _T("");
	m_IDMaquina = _T("");
	m_Cantidad = 0;
	m_IDArticulo = _T("");
	m_Cadencia = 0.0;
	m_Rechazos = 0.0;
	m_nFields = 8;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsHistorico::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsHistorico::GetDefaultSQL()
{
	return _T("[Historico]");
}

void CRsHistorico::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsHistorico)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[IDMolde]"), m_IDMolde);
	RFX_Text(pFX, _T("[IDMaquina]"), m_IDMaquina);
	RFX_Date(pFX, _T("[FecIni]"), m_FecIni);
	RFX_Date(pFX, _T("[FecFin]"), m_FecFin);
	RFX_Long(pFX, _T("[Cantidad]"), m_Cantidad);
	RFX_Text(pFX, _T("[IDArticulo]"), m_IDArticulo);
	RFX_Double(pFX, _T("[Cadencia]"), m_Cadencia);
	RFX_Double(pFX, _T("[Rechazos]"), m_Rechazos);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRsHistorico diagnostics

#ifdef _DEBUG
void CRsHistorico::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsHistorico::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CRsLocales

IMPLEMENT_DYNAMIC(CRsLocales, CRecordset)

CRsLocales::CRsLocales(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsLocales)
	m_DiasRetorno = 0;
	m_DiasTransporte = 0;
	m_Frecuencia = 0;
	m_ID = _T("");
	m_Nombre = _T("");
	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsLocales::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsLocales::GetDefaultSQL()
{
	return _T("[Locales]");
}

void CRsLocales::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsLocales)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[DiasRetorno]"), m_DiasRetorno);
	RFX_Long(pFX, _T("[DiasTransporte]"), m_DiasTransporte);
	RFX_Long(pFX, _T("[Frecuencia]"), m_Frecuencia);
	RFX_Text(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Nombre]"), m_Nombre);
	//}}AFX_FIELD_MAP
	m_ID.TrimRight();
	m_Nombre.TrimRight();
}

/////////////////////////////////////////////////////////////////////////////
// CRsLocales diagnostics

#ifdef _DEBUG
void CRsLocales::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsLocales::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CRsMaquinas

IMPLEMENT_DYNAMIC(CRsMaquinas, CRecordset)

CRsMaquinas::CRsMaquinas(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsMaquinas)
	m_GFH = 0;
	m_ID = _T("");
	m_IDLocal = _T("");
	m_Inact = 0;
	m_Nombre = _T("");
	m_NombreGFH = _T("");
	m_orden = 0;
	m_Personas = 0.0;
	m_Programable = FALSE;
	m_Tipo = _T("");
	m_lOperacion = 0;
	m_nFields = 11;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsMaquinas::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsMaquinas::GetDefaultSQL()
{
	return _T("[MaqGFH]");
}

void CRsMaquinas::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsMaquinas)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[GFH]"), m_GFH);
	RFX_Text(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[IDLocal]"), m_IDLocal);
	RFX_Long(pFX, _T("[Inact]"), m_Inact);
	RFX_Text(pFX, _T("[Nombre]"), m_Nombre);
	RFX_Text(pFX, _T("[NombreGFH]"), m_NombreGFH);
	RFX_Long(pFX, _T("[orden]"), m_orden);
	RFX_Double(pFX, _T("[Personas]"), m_Personas);
	RFX_Bool(pFX, _T("[Programable]"), m_Programable);
	RFX_Text(pFX, _T("[Tipo]"), m_Tipo);
	RFX_Long(pFX, _T("[Operacion]"), m_lOperacion);
	//}}AFX_FIELD_MAP
	m_ID.TrimRight();
	m_Tipo.TrimRight();
	m_Nombre.TrimRight();
	m_IDLocal.TrimRight();
}

/////////////////////////////////////////////////////////////////////////////
// CRsMaquinas diagnostics

#ifdef _DEBUG
void CRsMaquinas::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsMaquinas::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CBufLocal

IMPLEMENT_DYNAMIC(CBufLocal, CObject)

CBufLocal::CBufLocal(CRsLocales& p_RsLocales)
	: CObject()
{
	AssignData(p_RsLocales);

}
                               
void CBufLocal::AssignData(CRsLocales& p_RsLocales)
{
	// TODO
	m_sID = p_RsLocales.m_ID ;
	m_sNombre = p_RsLocales.m_Nombre ;
	
	if (p_RsLocales.IsFieldNull(&p_RsLocales.m_DiasTransporte)) m_iDiasTransporte = 0;
	else m_iDiasTransporte = p_RsLocales.m_DiasTransporte ;
	if (p_RsLocales.IsFieldNull(&p_RsLocales.m_DiasRetorno)) m_iDiasRetorno = 0;
	else m_iDiasRetorno = p_RsLocales.m_DiasRetorno ;
	if (p_RsLocales.IsFieldNull(&p_RsLocales.m_Frecuencia)) m_iFrecuencia = 0;
	else m_iFrecuencia = p_RsLocales.m_Frecuencia ;
}

CBufLocal::CBufLocal()
	: CObject()
{

	m_sID = "";
	m_sNombre = "";
	m_iDiasTransporte = 0;
	m_iDiasRetorno = 0;
	m_iFrecuencia = 0;
}

CBufLocal::~CBufLocal()
{
}                              

/////////////////////////////////////////////////////////////////////////////
// CBufMaquina

IMPLEMENT_DYNAMIC(CBufMaquina, CObject)

CBufMaquina::CBufMaquina(CRsMaquinas& p_RsMaquinas)
	: CObject()
{
	AssignData(p_RsMaquinas);

}
                               
void CBufMaquina::AssignData(CRsMaquinas& p_RsMaquinas)
{
	// TODO
	// Pensar en los posibles problemas de los valores nulos
	m_sID = p_RsMaquinas.m_ID;
	m_sID.TrimRight();
	m_sTipo = p_RsMaquinas.m_Tipo;
	m_sTipo.TrimRight();
	m_sNombre = p_RsMaquinas.m_Nombre;
	m_sNombre.TrimRight();
	m_sIDLocal = p_RsMaquinas.m_IDLocal;
	m_sIDLocal.TrimRight();
	if (p_RsMaquinas.IsFieldNull(&p_RsMaquinas.m_Inact)) m_lInact = 0;
	else m_lInact = p_RsMaquinas.m_Inact ;
	if (p_RsMaquinas.IsFieldNull(&p_RsMaquinas.m_orden)) m_lOrden = 0;
	else m_lOrden = p_RsMaquinas.m_orden ;
	if (p_RsMaquinas.IsFieldNull(&p_RsMaquinas.m_GFH)) m_lGFH = 1;
	else m_lGFH = p_RsMaquinas.m_GFH ;
	if (p_RsMaquinas.IsFieldNull(&p_RsMaquinas.m_Programable)) m_bProgramable = true;
	else m_bProgramable = p_RsMaquinas.m_Programable ;
	if (p_RsMaquinas.IsFieldNull(&p_RsMaquinas.m_Personas)) m_dPersonas = 0;
	else m_dPersonas = p_RsMaquinas.m_Personas;
	if (m_dPersonas <= 0) m_dPersonas = 1;
	if (p_RsMaquinas.IsFieldNull(&p_RsMaquinas.m_lOperacion)) m_lOperacion = 0;
	else m_lOperacion = p_RsMaquinas.m_lOperacion ;
	
	m_sNombreGFH = p_RsMaquinas.m_NombreGFH;
	m_sNombreGFH.TrimRight();
}

CBufMaquina::CBufMaquina()
	: CObject()
{
	m_sID = "";
	m_sTipo = "";
	m_sNombre = "";
	m_sIDLocal = "";
	m_lInact = 0;
	m_lOrden = 0;
	m_lGFH = 1;
	m_bProgramable = true;
	m_sNombreGFH = "";
	m_dPersonas = 1;
	m_lOperacion = 0;

}

CBufMaquina::~CBufMaquina()
{
}                              

/////////////////////////////////////////////////////////////////////////////
// CRsMezclas

IMPLEMENT_DYNAMIC(CRsMezclas, CRecordset)

CRsMezclas::CRsMezclas(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsMezclas)
	m_ID = _T("");
	m_Color = _T("");
	m_Cantidad = 0;
	m_Lote = 0;
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsMezclas::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsMezclas::GetDefaultSQL()
{
	return _T("[Mezclas]");
}

void CRsMezclas::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsMezclas)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Color]"), m_Color);
	RFX_Long(pFX, _T("[Cantidad]"), m_Cantidad);
	RFX_Long(pFX, _T("[Lote]"), m_Lote);
	//}}AFX_FIELD_MAP
	m_ID.TrimRight();
	m_Color.TrimRight();
}

/////////////////////////////////////////////////////////////////////////////
// CRsMezclas diagnostics

#ifdef _DEBUG
void CRsMezclas::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsMezclas::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CBufMezcla

IMPLEMENT_DYNAMIC(CBufMezcla, CObject)

CBufMezcla::CBufMezcla(CRsMezclas& p_RsMezclas)
	: CObject()
{
	AssignData(p_RsMezclas);

	m_pArrEntregas = new CObArray();
//	m_pTL = NULL;
}
                               
void
CBufMezcla::AssignData(CRsMezclas& p_RsMezclas)
{
	// TODO
	// Pensar en los posibles problemas de los valores nulos
	m_sID = p_RsMezclas.m_ID ;
	m_sColor = p_RsMezclas.m_Color ;
	m_lCantidad = p_RsMezclas.m_Cantidad ;
	m_lCantPlanta = 0;
	m_lLote = p_RsMezclas.m_Lote;
}

CBufMezcla::CBufMezcla()
	: CObject()
{
	m_pArrEntregas = new CObArray();
//	m_pTL = NULL;
}

CBufMezcla::~CBufMezcla()
{
	ASSERT_VALID (m_pArrEntregas);
	for (int i=0; i< m_pArrEntregas->GetSize(); i++)
	{                                  
		CBufEntregas* pBufEntregas = (CBufEntregas *) m_pArrEntregas->GetAt(i);
		ASSERT( pBufEntregas->IsKindOf( RUNTIME_CLASS( CBufEntregas ) ) );
		delete pBufEntregas;
	}
	delete m_pArrEntregas;	
}                              

long
CBufMezcla::GetlCantidad()
{
	long lExist = 0;
	if ( m_lCantidad > 0 ) lExist +=  m_lCantidad ; 
	// Antes las sumábamos. Ahora sustituimos
	//	if ( m_pBufMezcla->m_lCantPlanta > 0 )	lExist +=  m_pBufMezcla->m_lCantPlanta ; 
	if ( m_lCantPlanta > 0 ) lExist =  m_lCantPlanta ; 
	return lExist;
}


/////////////////////////////////////////////////////////////////////////////
// CRsMoldes

IMPLEMENT_DYNAMIC(CRsMoldes, CRecordset)

CRsMoldes::CRsMoldes(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsMoldes)
	m_ID = _T("");
	m_Alternancia = _T("");
	m_Tipo = _T("");
	m_Tiradas = 0;
	m_Estado = _T("");
	m_CanalFrio = FALSE;
	m_CercoREP = FALSE;
	m_ExtractorAutomatico = FALSE;
	m_DoblePiston = FALSE;
	m_ExtractorMK19 = FALSE;
	m_nFields = 11;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsMoldes::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsMoldes::GetDefaultSQL()
{
	return _T("[Moldes]");
}

void CRsMoldes::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsMoldes)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Alternancia]"), m_Alternancia);
	RFX_Text(pFX, _T("[Tipo]"), m_Tipo);
	RFX_Long(pFX, _T("[Tiradas]"), m_Tiradas);
	RFX_Text(pFX, _T("[Estado]"), m_Estado);
	RFX_Date(pFX, _T("[Disponibilidad]"), m_Disponibilidad); 
	RFX_Bool(pFX, _T("[CanalFrio]"), m_CanalFrio);
	RFX_Bool(pFX, _T("[CercoREP]"), m_CercoREP);
	RFX_Bool(pFX, _T("[ExtractorAutomatico]"), m_ExtractorAutomatico);
	RFX_Bool(pFX, _T("[DoblePiston]"), m_DoblePiston);
	RFX_Bool(pFX, _T("[ExtractorMK19]"), m_ExtractorMK19);
	//}}AFX_FIELD_MAP
	m_ID.TrimRight();
	m_Alternancia.TrimRight();
	m_Tipo.TrimRight();
	m_Estado.TrimRight();
}

/////////////////////////////////////////////////////////////////////////////
// CRsMoldes diagnostics

#ifdef _DEBUG
void CRsMoldes::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsMoldes::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBufMolde
#ifdef _DEBUG
void CBufMolde::AssertValid() const
{
	// Call inherited AssertValid first.
	CObject::AssertValid();

	ASSERT(!m_sID.IsEmpty());
	ASSERT_VALID(m_pArrMolArt);
	ASSERT_VALID(m_pArrMolMaq);
	ASSERT_VALID(m_pArrMolElem);
}
#endif


IMPLEMENT_DYNAMIC(CBufMolde, CObject)

CBufMolde::CBufMolde(CRsMoldes& p_RsMoldes)
	: CObject()
{
	AssignData(p_RsMoldes);

	m_pArrMolArt = new CObArray();
	m_pArrMolMaq = new CObArray();
	m_pArrMolElem = new CObArray();
}
                               
void CBufMolde::AssignData(CRsMoldes& p_RsMoldes)
{
	// TODO
	// Pensar en los posibles problemas de los valores nulos
	m_sID = p_RsMoldes.m_ID;
	m_sID.TrimRight();
	m_sAlternancia = p_RsMoldes.m_Alternancia;
	m_sAlternancia.TrimRight();
	m_sTipo = p_RsMoldes.m_Tipo;
	m_sTipo.TrimRight();
	m_lTiradas = p_RsMoldes.m_Tiradas;
	m_sEstado = p_RsMoldes.m_Estado;
	m_sEstado.TrimRight();
	m_FecDisponibilidad = p_RsMoldes.m_Disponibilidad;
	if (p_RsMoldes.IsFieldNull(&p_RsMoldes.m_CanalFrio)) m_BCanalFrio = FALSE;
	else m_BCanalFrio = p_RsMoldes.m_CanalFrio;
	if (p_RsMoldes.IsFieldNull(&p_RsMoldes.m_CercoREP)) m_BCercoREP = FALSE;
	else m_BCercoREP = p_RsMoldes.m_CercoREP;
	if (p_RsMoldes.IsFieldNull(&p_RsMoldes.m_ExtractorAutomatico)) m_BExtractorAutomatico = FALSE;
	else m_BExtractorAutomatico = p_RsMoldes.m_ExtractorAutomatico;
	if (p_RsMoldes.IsFieldNull(&p_RsMoldes.m_ExtractorMK19)) m_BExtractorMK19 = FALSE;
	else m_BExtractorMK19 = p_RsMoldes.m_ExtractorMK19;
	if (p_RsMoldes.IsFieldNull(&p_RsMoldes.m_DoblePiston)) m_BDoblePiston = FALSE;
	else m_BDoblePiston = p_RsMoldes.m_DoblePiston;
	
}

CBufMolde::CBufMolde()
	: CObject()
{
	m_pArrMolArt = new CObArray();
	m_pArrMolMaq = new CObArray();
	m_pArrMolElem = new CObArray();

}

CBufMolde::~CBufMolde()
{
	ClearArray(*m_pArrMolArt, RUNTIME_CLASS(CBufArtMol));
	delete m_pArrMolArt;
	ClearArray(*m_pArrMolMaq, RUNTIME_CLASS(CBufMolMaq));
	delete m_pArrMolMaq;
	ClearArray(*m_pArrMolElem, RUNTIME_CLASS(CBufMoldesElem));
	delete m_pArrMolElem;
	
}                              

bool 
CBufMolde::TieneDetalle()
{
	return (m_pArrMolElem->GetSize() > 0);
}
/////////////////////////////////////////////////////////////////////////////
// CRsMolMaq

IMPLEMENT_DYNAMIC(CRsMolMaq, CRecordset)

CRsMolMaq::CRsMolMaq(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsMolMaq)
	m_IDMolde = _T("");
	m_Prioridad = 0;
	m_IDMaquina = _T("");
	m_nFields = 3;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsMolMaq::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsMolMaq::GetDefaultSQL()
{
	return _T("[MolMaq]");
}

void CRsMolMaq::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsMolMaq)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[IDMolde]"), m_IDMolde);
	RFX_Long(pFX, _T("[Prioridad]"), m_Prioridad);
	RFX_Text(pFX, _T("[IDMaquina]"), m_IDMaquina);
	//}}AFX_FIELD_MAP
	m_IDMolde.TrimRight();
	m_IDMaquina.TrimRight();
}

/////////////////////////////////////////////////////////////////////////////
// CRsMolMaq diagnostics

#ifdef _DEBUG
void CRsMolMaq::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsMolMaq::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CBufArtMol

IMPLEMENT_DYNAMIC(CBufMolMaq, CObject)

CBufMolMaq::CBufMolMaq(CRsMolMaq& p_RsMolMaq)
	: CObject()
{
	m_sIDMolde = p_RsMolMaq.m_IDMolde;
	m_sIDMolde.TrimRight();
	m_lPrioridad = p_RsMolMaq.m_Prioridad;
	m_sIDMaquina = p_RsMolMaq.m_IDMaquina;
	m_sIDMaquina.TrimRight();
}


/////////////////////////////////////////////////////////////////////////////
// CRsPedidos

IMPLEMENT_DYNAMIC(CRsPedidos, CRecordset)

CRsPedidos::CRsPedidos(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsPedidos)
	m_CantAlmac = 0.0;
	m_CantCurso = 0.0;
	m_CantOrig = 0.0;
	m_CantPauta = 0.0;
	m_CantPend = 0.0;
	m_IDArticulo = _T("");
	m_IDCliente = 0;
	m_Necesidad = _T("");
	m_Nomcli = _T("");
	m_Seccion = 0;
	m_CantObsoletos = 0.0;
	m_CantStockSeguridad = 0.0;
	m_Contratas = 0.0;
	m_nFields = 14;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsPedidos::GetDefaultConnect()
{
	return GetsConnPedidos();
}

CString CRsPedidos::GetDefaultSQL()
{
	return _T("[Pedidos]");
}

void CRsPedidos::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsPedidos)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Double(pFX, _T("[CantAlmac]"), m_CantAlmac);
	RFX_Double(pFX, _T("[CantCurso]"), m_CantCurso);
	RFX_Double(pFX, _T("[CantOrig]"), m_CantOrig);
	RFX_Double(pFX, _T("[CantPauta]"), m_CantPauta);
	RFX_Double(pFX, _T("[CantPend]"), m_CantPend);
	RFX_Date(pFX, _T("[FecNec]"), m_FecNec);
	RFX_Text(pFX, _T("[IDArticulo]"), m_IDArticulo);
	RFX_Long(pFX, _T("[IDCliente]"), m_IDCliente);
	RFX_Text(pFX, _T("[Necesidad]"), m_Necesidad);
	RFX_Text(pFX, _T("[Nomcli]"), m_Nomcli);
	RFX_Long(pFX, _T("[Seccion]"), m_Seccion);
	RFX_Double(pFX, _T("[CantObsoletos]"), m_CantObsoletos);
	RFX_Double(pFX, _T("[CantStockSeguridad]"), m_CantStockSeguridad);
	RFX_Double(pFX, _T("[Contratas]"), m_Contratas);
	//}}AFX_FIELD_MAP
	m_IDArticulo.TrimRight();
	m_Necesidad.TrimRight();
	m_Nomcli.TrimRight();

}

/////////////////////////////////////////////////////////////////////////////
// CRsPedidos diagnostics

#ifdef _DEBUG
void CRsPedidos::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsPedidos::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBufOF

IMPLEMENT_DYNAMIC(CBufOF, CObject)

CBufOF::CBufOF(CRsPedidos& p_RsPedidos)
	: CObject()
{
	AssignData(p_RsPedidos);

}
                               
void CBufOF::AssignData(CRsPedidos& p_RsPedidos)
{
	// TODO
	// Pensar en los posibles problemas de los valores nulos

	if (p_RsPedidos.IsFieldNull(&p_RsPedidos.m_CantAlmac)) m_dCantAlmac = 0;
	else m_dCantAlmac = p_RsPedidos.m_CantAlmac;
	if (p_RsPedidos.IsFieldNull(&p_RsPedidos.m_CantCurso)) m_dCantCurso = 0;
	else m_dCantCurso = p_RsPedidos.m_CantCurso;
	if (p_RsPedidos.IsFieldNull(&p_RsPedidos.m_CantOrig)) m_dCantOrig = 0;
	else m_dCantOrig = p_RsPedidos.m_CantOrig;
	if (p_RsPedidos.IsFieldNull(&p_RsPedidos.m_CantPauta)) m_dCantPauta = 0;
	else m_dCantPauta = p_RsPedidos.m_CantPauta;
	if (p_RsPedidos.IsFieldNull(&p_RsPedidos.m_CantPend)) m_dCantPend = 0;
	else m_dCantPend = p_RsPedidos.m_CantPend;
	if (p_RsPedidos.IsFieldNull(&p_RsPedidos.m_CantObsoletos)) m_dCantObsoletos = 0;
	else m_dCantObsoletos = p_RsPedidos.m_CantObsoletos;
	if (p_RsPedidos.IsFieldNull(&p_RsPedidos.m_CantStockSeguridad)) m_dCantStockSeguridad = 0;
	else m_dCantStockSeguridad = p_RsPedidos.m_CantStockSeguridad;
	if (p_RsPedidos.IsFieldNull(&p_RsPedidos.m_Contratas)) m_dContratas = 0;
	else m_dContratas = p_RsPedidos.m_Contratas;
	m_FecNec = p_RsPedidos.m_FecNec;
	m_sIDArticulo = p_RsPedidos.m_IDArticulo;
	m_sIDArticulo.TrimRight();
	if (p_RsPedidos.IsFieldNull(&p_RsPedidos.m_IDCliente)) m_lIDCliente = 0;
	else m_lIDCliente = p_RsPedidos.m_IDCliente;
	m_sNecesidad = p_RsPedidos.m_Necesidad;
	m_sNecesidad.TrimRight();
	m_sNomcli = p_RsPedidos.m_Nomcli;
	m_sNomcli.TrimRight();
	m_bSelected = FALSE;
	m_bComprimido = FALSE;
	if (p_RsPedidos.IsFieldNull(&p_RsPedidos.m_Seccion)) m_lSeccion = 0;
	else m_lSeccion = p_RsPedidos.m_Seccion;
	m_lMargenMed = m_lMargenMin = m_lMargenMax = 0;
	m_FecFab = m_FecNec;
	m_pBufArt = NULL;
}

CBufOF::CBufOF(CRsPedidosAyer& p_RsPedidosAyer)
	: CObject()
{
	if (p_RsPedidosAyer.IsFieldNull(&p_RsPedidosAyer.m_CantAlmac)) m_dCantAlmac = 0;
	else m_dCantAlmac = p_RsPedidosAyer.m_CantAlmac;
	if (p_RsPedidosAyer.IsFieldNull(&p_RsPedidosAyer.m_CantCurso)) m_dCantCurso = 0;
	else m_dCantCurso = p_RsPedidosAyer.m_CantCurso;
	if (p_RsPedidosAyer.IsFieldNull(&p_RsPedidosAyer.m_CantOrig)) m_dCantOrig = 0;
	else m_dCantOrig = p_RsPedidosAyer.m_CantOrig;
	if (p_RsPedidosAyer.IsFieldNull(&p_RsPedidosAyer.m_CantPauta)) m_dCantPauta = 0;
	else m_dCantPauta = p_RsPedidosAyer.m_CantPauta;
	if (p_RsPedidosAyer.IsFieldNull(&p_RsPedidosAyer.m_CantPend)) m_dCantPend = 0;
	else m_dCantPend = p_RsPedidosAyer.m_CantPend;
	if (p_RsPedidosAyer.IsFieldNull(&p_RsPedidosAyer.m_CantObsoletos)) m_dCantObsoletos = 0;
	else m_dCantObsoletos = p_RsPedidosAyer.m_CantObsoletos;
	if (p_RsPedidosAyer.IsFieldNull(&p_RsPedidosAyer.m_CantStockSeguridad)) m_dCantStockSeguridad = 0;
	else m_dCantStockSeguridad = p_RsPedidosAyer.m_CantStockSeguridad;
	if (p_RsPedidosAyer.IsFieldNull(&p_RsPedidosAyer.m_Contratas)) m_dContratas = 0;
	else m_dContratas = p_RsPedidosAyer.m_Contratas;
	m_FecNec = p_RsPedidosAyer.m_FecNec;
	m_sIDArticulo = p_RsPedidosAyer.m_IDArticulo;
	m_sIDArticulo.TrimRight();
	if (p_RsPedidosAyer.IsFieldNull(&p_RsPedidosAyer.m_IDCliente)) m_lIDCliente = 0;
	else m_lIDCliente = p_RsPedidosAyer.m_IDCliente;
	m_sNecesidad = p_RsPedidosAyer.m_Necesidad;
	m_sNecesidad.TrimRight();
	m_sNomcli = p_RsPedidosAyer.m_Nomcli;
	m_sNomcli.TrimRight();
	m_bComprimido = FALSE;
	m_bSelected = FALSE;
	if (p_RsPedidosAyer.IsFieldNull(&p_RsPedidosAyer.m_Seccion)) m_lSeccion = 0;
	else m_lSeccion = p_RsPedidosAyer.m_Seccion;
	m_pBufArt = NULL;

}

CBufOF::CBufOF()
	: CObject()
{
}

COLORREF
CBufOF::GetColor()
{
	COLORREF ColorRet = RGB(255,255,255);	// Por defecto es blanco
	if ( m_lMargenMed == INT_MIN)
		ColorRet = g_MC.m_Color6;
	else if ( m_lMargenMed == INT_MAX)
		ColorRet = g_MC.m_Color1;
	else if ( m_lMargenMed < INT_MAX && m_lMargenMed >= g_MC.m_iMin2)
		ColorRet = g_MC.m_Color2;
	else if ( m_lMargenMed < g_MC.m_iMax3 && m_lMargenMed >= g_MC.m_iMin3)
		ColorRet = g_MC.m_Color3;
	else if ( m_lMargenMed < g_MC.m_iMax4 && m_lMargenMed >= g_MC.m_iMin4)
		ColorRet = g_MC.m_Color4;
	else if ( m_lMargenMed < g_MC.m_iMax5 && m_lMargenMed > INT_MIN)
		ColorRet = g_MC.m_Color5;

	return ColorRet;
}

// Definiciones de estructuras

IMPLEMENT_DYNAMIC(CInfoMolArt, CObject)

CInfoMolArt::CInfoMolArt()
	: CObject()
{
	m_lPrioridad = 0;
	m_sIDArticulo = "";
	m_lPorcHist = 0;
	m_BProg = FALSE;

}


/////////////////////////////////////////////////////////////////////////////
// CRsListadoValorado

IMPLEMENT_DYNAMIC(CRsListadoValorado, CRecordset)

CRsListadoValorado::CRsListadoValorado(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsListadoValorado)
	m_Articulo = _T("");
	m_Cliente = _T("");
	m_P_H_real = 0.0;
	m__Rechazos = 0.0;
	m_Cantidad_Fabricada = 0;
	m_Cantidad_Rechazada = 0;
	m_Total_Horas = 0;
	m_nFields = 7;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsListadoValorado::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsListadoValorado::GetDefaultSQL()
{
	return _T("[ListadoValorado]");
}

void CRsListadoValorado::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsListadoValorado)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[Articulo]"), m_Articulo);
	RFX_Text(pFX, _T("[Cliente]"), m_Cliente);
	RFX_Double(pFX, _T("[P/H real]"), m_P_H_real);
	RFX_Double(pFX, _T("[%Rechazos]"), m__Rechazos);
	RFX_Long(pFX, _T("[Cantidad Fabricada]"), m_Cantidad_Fabricada);
	RFX_Long(pFX, _T("[Cantidad Rechazada]"), m_Cantidad_Rechazada);
	RFX_Long(pFX, _T("[Total Horas]"), m_Total_Horas);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRsListadoValorado diagnostics

#ifdef _DEBUG
void CRsListadoValorado::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsListadoValorado::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CRsPedidosAyer

IMPLEMENT_DYNAMIC(CRsPedidosAyer, CRecordset)

CRsPedidosAyer::CRsPedidosAyer(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsPedidosAyer)
	m_IDArticulo = _T("");
	m_IDCliente = 0;
	m_Nomcli = _T("");
	m_Necesidad = _T("");
	m_CantOrig = 0.0;
	m_CantPend = 0.0;
	m_CantAlmac = 0.0;
	m_CantCurso = 0.0;
	m_CantPauta = 0.0;
	m_Seccion = 0;
	m_CantObsoletos = 0.0;
	m_CantStockSeguridad = 0.0;
	m_Contratas = 0.0;
	m_nFields = 14;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsPedidosAyer::GetDefaultConnect()
{
	return GetsConnPedidos();
}

CString CRsPedidosAyer::GetDefaultSQL()
{
	return _T("[Pedidos_Ayer]");
}

void CRsPedidosAyer::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsPedidosAyer)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[IDArticulo]"), m_IDArticulo);
	RFX_Long(pFX, _T("[IDCliente]"), m_IDCliente);
	RFX_Text(pFX, _T("[Nomcli]"), m_Nomcli);
	RFX_Text(pFX, _T("[Necesidad]"), m_Necesidad);
	RFX_Double(pFX, _T("[CantOrig]"), m_CantOrig);
	RFX_Double(pFX, _T("[CantPend]"), m_CantPend);
	RFX_Date(pFX, _T("[FecNec]"), m_FecNec);
	RFX_Double(pFX, _T("[CantAlmac]"), m_CantAlmac);
	RFX_Double(pFX, _T("[CantCurso]"), m_CantCurso);
	RFX_Double(pFX, _T("[CantPauta]"), m_CantPauta);
	RFX_Long(pFX, _T("[Seccion]"), m_Seccion);
	RFX_Double(pFX, _T("[CantObsoletos]"), m_CantObsoletos);
	RFX_Double(pFX, _T("[CantStockSeguridad]"), m_CantStockSeguridad);
	RFX_Double(pFX, _T("[Contratas]"), m_Contratas);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRsPedidosAyer diagnostics

#ifdef _DEBUG
void CRsPedidosAyer::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsPedidosAyer::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CRsCambiosMoldes

IMPLEMENT_DYNAMIC(CRsCambiosMoldes, CRecordset)

CRsCambiosMoldes::CRsCambiosMoldes(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsCambiosMoldes)
	m_Contador = 0;
	m_IDMaquina = _T("");
	m_IDMoldeEnt = _T("");
	m_IDMoldeSal = _T("");
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsCambiosMoldes::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsCambiosMoldes::GetDefaultSQL()
{
	return _T("[CambiosMoldes]");
}

void CRsCambiosMoldes::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsCambiosMoldes)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[Contador]"), m_Contador);
	RFX_Text(pFX, _T("[IDMaquina]"), m_IDMaquina);
	RFX_Text(pFX, _T("[IDMoldeEnt]"), m_IDMoldeEnt);
	RFX_Text(pFX, _T("[IDMoldeSal]"), m_IDMoldeSal);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRsCambiosMoldes diagnostics

#ifdef _DEBUG
void CRsCambiosMoldes::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsCambiosMoldes::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

IMPLEMENT_DYNAMIC(CBufCambiosMoldes, CObject)

CBufCambiosMoldes::CBufCambiosMoldes(CRsCambiosMoldes& p_RsCambiosMoldes)
	: CObject()
{
//	m_Fecha = p_RsCambiosMoldes.m_Fecha;
	m_sIDMaquina = p_RsCambiosMoldes.m_IDMaquina;
	m_sIDMaquina.TrimRight();
	if (p_RsCambiosMoldes.IsFieldNull(&p_RsCambiosMoldes.m_IDMoldeEnt ))
		m_sIDMoldeEnt = "";
	else
		m_sIDMoldeEnt = p_RsCambiosMoldes.m_IDMoldeEnt;
	m_sIDMoldeEnt.TrimRight();
	if (p_RsCambiosMoldes.IsFieldNull(&p_RsCambiosMoldes.m_IDMoldeSal ))
		m_sIDMoldeSal = "";
	else
		m_sIDMoldeSal = p_RsCambiosMoldes.m_IDMoldeSal;
	m_sIDMoldeSal.TrimRight();
	
}

CBufCambiosMoldes::CBufCambiosMoldes()
	: CObject()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRsMezclasPlanta

IMPLEMENT_DYNAMIC(CRsMezclasPlanta, CRecordset)

CRsMezclasPlanta::CRsMezclasPlanta(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsMezclasPlanta)
	m_ID = _T("");
	m_Cantidad = 0;
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsMezclasPlanta::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsMezclasPlanta::GetDefaultSQL()
{
	return _T("[Mezclas_Planta]");
}

void CRsMezclasPlanta::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsMezclasPlanta)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[ID]"), m_ID);
	RFX_Long(pFX, _T("[Cantidad]"), m_Cantidad);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRsMezclasPlanta diagnostics

#ifdef _DEBUG
void CRsMezclasPlanta::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsMezclasPlanta::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CRsLisMezclas

IMPLEMENT_DYNAMIC(CRsLisMezclas, CRecordset)

CRsLisMezclas::CRsLisMezclas(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsLisMezclas)
	m_Cant = 0;
	m_IDArticulo = _T("");
	m_IDCliente = _T("");
	m_IDLocal = _T("");
	m_Mezcla = 0;
	m_nFields = 6;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsLisMezclas::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsLisMezclas::GetDefaultSQL()
{
	return _T("[Lis_Mezclas]");
}

void CRsLisMezclas::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsLisMezclas)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[Cant]"), m_Cant);
	RFX_Date(pFX, _T("[FecIni]"), m_FecIni);
	RFX_Text(pFX, _T("[IDArticulo]"), m_IDArticulo);
	RFX_Text(pFX, _T("[IDCliente]"), m_IDCliente);
	RFX_Text(pFX, _T("[IDLocal]"), m_IDLocal);
	RFX_Long(pFX, _T("[Mezcla]"), m_Mezcla);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRsLisMezclas diagnostics

#ifdef _DEBUG
void CRsLisMezclas::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsLisMezclas::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


IMPLEMENT_DYNAMIC(CBufLisMezclas, CObject)

CBufLisMezclas::CBufLisMezclas(CRsLisMezclas& p_RsLisMezclas)
	: CObject()
{
	m_sIDLocal = p_RsLisMezclas.m_IDLocal ;
	m_sIDCliente = p_RsLisMezclas.m_IDCliente ;
	m_sIDArticulo = p_RsLisMezclas.m_IDArticulo ;
	m_lMezcla = p_RsLisMezclas.m_Mezcla ;
	m_lCant = p_RsLisMezclas.m_Cant ;
	m_FecIni = p_RsLisMezclas.m_FecIni;
	
}

CBufLisMezclas::CBufLisMezclas()
	: CObject()
{
}
/////////////////////////////////////////////////////////////////////////////
// CRsCalendario

IMPLEMENT_DYNAMIC(CRsCalendario, CRecordset)

CRsCalendario::CRsCalendario(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsCalendario)
	m_ID = _T("");
	m_Turno1 = FALSE;
	m_Turno2 = FALSE;
	m_Turno3 = FALSE;
	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsCalendario::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access Database");
}

CString CRsCalendario::GetDefaultSQL()
{
	return _T("[Calendario]");
}

void CRsCalendario::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsCalendario)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[ID]"), m_ID);
	RFX_Date(pFX, _T("[Fecha]"), m_Fecha);
	RFX_Bool(pFX, _T("[Turno1]"), m_Turno1);
	RFX_Bool(pFX, _T("[Turno2]"), m_Turno2);
	RFX_Bool(pFX, _T("[Turno3]"), m_Turno3);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRsCalendario diagnostics

#ifdef _DEBUG
void CRsCalendario::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsCalendario::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CRsLisGeneral

IMPLEMENT_DYNAMIC(CRsLisGeneral, CRecordset)

CRsLisGeneral::CRsLisGeneral(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsLisGeneral)
	m_Texto1 = _T("");
	m_Texto10 = _T("");
	m_Texto11 = _T("");
	m_Texto12 = _T("");
	m_Texto13 = _T("");
	m_Texto14 = _T("");
	m_Texto15 = _T("");
	m_Texto16 = _T("");
	m_Texto17 = _T("");
	m_Texto18 = _T("");
	m_Texto19 = _T("");
	m_Texto2 = _T("");
	m_Texto20 = _T("");
	m_Texto3 = _T("");
	m_Texto4 = _T("");
	m_Texto5 = _T("");
	m_Texto6 = _T("");
	m_Texto7 = _T("");
	m_Texto8 = _T("");
	m_Texto9 = _T("");
	m_nFields = 20;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_sArrVal[0] = &m_Texto1;
	m_sArrVal[1] = &m_Texto2;
	m_sArrVal[2] = &m_Texto3;
	m_sArrVal[3] = &m_Texto4;
	m_sArrVal[4] = &m_Texto5;
	m_sArrVal[5] = &m_Texto6;
	m_sArrVal[6] = &m_Texto7;
	m_sArrVal[7] = &m_Texto8;
	m_sArrVal[8] = &m_Texto9;
	m_sArrVal[9] = &m_Texto10;
	m_sArrVal[10] = &m_Texto11;
	m_sArrVal[11] = &m_Texto12;
	m_sArrVal[12] = &m_Texto13;
	m_sArrVal[13] = &m_Texto14;
	m_sArrVal[14] = &m_Texto15;
	m_sArrVal[15] = &m_Texto16;
	m_sArrVal[16] = &m_Texto17;
	m_sArrVal[17] = &m_Texto18;
	m_sArrVal[18] = &m_Texto19;
	m_sArrVal[19] = &m_Texto20;
}


CString CRsLisGeneral::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsLisGeneral::GetDefaultSQL()
{
	return _T("[Lis_General]");
}

void CRsLisGeneral::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsLisGeneral)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[Texto1]"), m_Texto1);
	RFX_Text(pFX, _T("[Texto10]"), m_Texto10);
	RFX_Text(pFX, _T("[Texto11]"), m_Texto11);
	RFX_Text(pFX, _T("[Texto12]"), m_Texto12);
	RFX_Text(pFX, _T("[Texto13]"), m_Texto13);
	RFX_Text(pFX, _T("[Texto14]"), m_Texto14);
	RFX_Text(pFX, _T("[Texto15]"), m_Texto15);
	RFX_Text(pFX, _T("[Texto16]"), m_Texto16);
	RFX_Text(pFX, _T("[Texto17]"), m_Texto17);
	RFX_Text(pFX, _T("[Texto18]"), m_Texto18);
	RFX_Text(pFX, _T("[Texto19]"), m_Texto19);
	RFX_Text(pFX, _T("[Texto2]"), m_Texto2);
	RFX_Text(pFX, _T("[Texto20]"), m_Texto20);
	RFX_Text(pFX, _T("[Texto3]"), m_Texto3);
	RFX_Text(pFX, _T("[Texto4]"), m_Texto4);
	RFX_Text(pFX, _T("[Texto5]"), m_Texto5);
	RFX_Text(pFX, _T("[Texto6]"), m_Texto6);
	RFX_Text(pFX, _T("[Texto7]"), m_Texto7);
	RFX_Text(pFX, _T("[Texto8]"), m_Texto8);
	RFX_Text(pFX, _T("[Texto9]"), m_Texto9);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRsLisGeneral diagnostics

#ifdef _DEBUG
void CRsLisGeneral::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsLisGeneral::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CRsAcabado

IMPLEMENT_DYNAMIC(CRsAcabado, CRecordset)

CRsAcabado::CRsAcabado(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsAcabado)
	m_articulo = _T("");
	m_operacion = 0;
	m_NombreOperacion = _T("");
	m_centro = 0;
	m_nombrecentro = _T("");
	m_cadencia = 0.0;
	m_nFields = 6;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsAcabado::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsAcabado::GetDefaultSQL()
{
	return _T("[Acabado]");
}

void CRsAcabado::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsAcabado)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[articulo]"), m_articulo);
	RFX_Long(pFX, _T("[operacion]"), m_operacion);
	RFX_Text(pFX, _T("[NombreOperacion]"), m_NombreOperacion);
	RFX_Long(pFX, _T("[centro]"), m_centro);
	RFX_Text(pFX, _T("[nombrecentro]"), m_nombrecentro);
	RFX_Double(pFX, _T("[cadencia]"), m_cadencia);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRsAcabado diagnostics

#ifdef _DEBUG
void CRsAcabado::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsAcabado::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBufAcabado

IMPLEMENT_DYNAMIC(CBufAcabado, CObject)

CBufAcabado::CBufAcabado(CRsAcabado& p_RsAcabado)
	: CObject()
{

	m_sArticulo = p_RsAcabado.m_articulo;
	m_sNombreOperacion = p_RsAcabado.m_NombreOperacion;
	m_sNombreCentro = p_RsAcabado.m_nombrecentro;
	
	if (p_RsAcabado.IsFieldNull(&p_RsAcabado.m_operacion)) m_lOperacion = 0;
	else m_lOperacion = p_RsAcabado.m_operacion;
	if (p_RsAcabado.IsFieldNull(&p_RsAcabado.m_centro)) m_lCentro = 0;
	else m_lCentro = p_RsAcabado.m_centro;
	if (p_RsAcabado.IsFieldNull(&p_RsAcabado.m_cadencia)) m_dCadencia = 0;
	else m_dCadencia = p_RsAcabado.m_cadencia;
	
	m_pBufArticulo = NULL;
}

CString
CBufAcabado::GetsInfo(long p_lCant)
{
	CString sTemp;
	long lSecs = (long) (((double)p_lCant / m_dCadencia) * 3600);
	CTimeSpan tsTemp(lSecs);
	sTemp.Format("[%-6.6s] %-20.20s [%-6.6s] %-20.20s [%-7.7s] = %6.6s",
					FormatLong(m_lOperacion, 6, NoPoint),
					m_sNombreOperacion,
					FormatLong(m_lCentro, 6, NoPoint),
					m_sNombreCentro,
					FormatDbl(m_dCadencia, 6,1),
					FormatFec(tsTemp, FecHora)
					);
	return sTemp;
}
/////////////////////////////////////////////////////////////////////////////
// CRsHomologadas

IMPLEMENT_DYNAMIC(CRsHomologadas, CRecordset)

CRsHomologadas::CRsHomologadas(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsHomologadas)
	m_Empresa = 0;
	m_Articulo = _T("");
	m_Tipo = _T("");
	m_Homologacion = _T("");
	m_lRespFab = 0;
	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsHomologadas::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access 97 Database");
}

CString CRsHomologadas::GetDefaultSQL()
{
	return _T("[Homologadas]");
}

void CRsHomologadas::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsHomologadas)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[Empresa]"), m_Empresa);
	RFX_Text(pFX, _T("[Articulo]"), m_Articulo);
	RFX_Text(pFX, _T("[Tipo]"), m_Tipo);
	RFX_Text(pFX, _T("[Homologacion]"), m_Homologacion);
	RFX_Long(pFX, _T("[respFab]"), m_lRespFab);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRsHomologadas diagnostics

#ifdef _DEBUG
void CRsHomologadas::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsHomologadas::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBufHomologada

IMPLEMENT_DYNAMIC(CBufHomologada, CObject)

CBufHomologada::CBufHomologada(CRsHomologadas& p_RsHomologada)
	: CObject()
{

	m_sArticulo = p_RsHomologada.m_Articulo;
	m_sTipo = p_RsHomologada.m_Tipo;
	m_sHomologacion = p_RsHomologada.m_Homologacion;
	
	if (p_RsHomologada.IsFieldNull(&p_RsHomologada.m_Empresa)) m_lEmpresa = 0;
	else m_lEmpresa = p_RsHomologada.m_Empresa;
	
	if (p_RsHomologada.IsFieldNull(&p_RsHomologada.m_lRespFab)) m_lRespFab = 0;
	else m_lRespFab = p_RsHomologada.m_lRespFab;
	
	m_pBufArticulo = NULL;
}


///
/////////////////////////////////////////////////////////////////////////////
// CRsComponentes

IMPLEMENT_DYNAMIC(CRsComponentes, CRecordset)

CRsComponentes::CRsComponentes(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsComponentes)
	m_articulo = _T("");
	m_componente = _T("");
	m_cantidad = 0.0;
	m_stock = 0.0;
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsComponentes::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access Database");
}

CString CRsComponentes::GetDefaultSQL()
{
	return _T("[Articulos_Componentes]");
}

void CRsComponentes::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsComponentes)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[Articulo]"), m_articulo);
	RFX_Text(pFX, _T("[Componente]"), m_componente);
	RFX_Double(pFX, _T("[Cantidad]"), m_cantidad);
	RFX_Double(pFX, _T("[stock]"), m_stock);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRsComponentes diagnostics

#ifdef _DEBUG
void CRsComponentes::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsComponentes::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBufComponentes

IMPLEMENT_DYNAMIC(CBufComponentes, CObject)

CBufComponentes::CBufComponentes(CRsComponentes& p_RsComponentes)
	: CObject()
{

	m_sArticulo = p_RsComponentes.m_articulo;
	m_sComponente = p_RsComponentes.m_componente;
	if (p_RsComponentes.IsFieldNull(&p_RsComponentes.m_cantidad)) m_dCantidad = 0;
	else m_dCantidad = p_RsComponentes.m_cantidad;
	if (p_RsComponentes.IsFieldNull(&p_RsComponentes.m_stock)) m_dStock = 0;
	else m_dStock = p_RsComponentes.m_stock;

	m_pBufArticulo = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CRsMoldes_Inyectora

IMPLEMENT_DYNAMIC(CRsMoldes_Inyectora, CRecordset)

CRsMoldes_Inyectora::CRsMoldes_Inyectora(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsMoldes_Inyectora)
	m_idMolde = _T("");
	m_id = 0;
	m_nombre = _T("");
	m_cantidad = 0.0;
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsMoldes_Inyectora::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access Database");
}

CString CRsMoldes_Inyectora::GetDefaultSQL()
{
	return _T("[Moldes_Inyectora_Comp]");
}

void CRsMoldes_Inyectora::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsMoldes_Inyectora)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[IDMolde]"), m_idMolde);
	RFX_Long(pFX, _T("[IDInyectora]"), m_id);
	RFX_Text(pFX, _T("[Nombre]"), m_nombre);
	RFX_Double(pFX, _T("[Cantidad]"), m_cantidad);
	//}}AFX_FIELD_MAP
	m_idMolde.TrimRight();
	m_nombre.TrimRight();
}

/////////////////////////////////////////////////////////////////////////////
// CRsMoldes_Inyectora diagnostics

#ifdef _DEBUG
void CRsMoldes_Inyectora::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsMoldes_Inyectora::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CBufMoldes_Inyectora

IMPLEMENT_DYNAMIC(CBufMoldesElem, CObject)

CBufMoldesElem::CBufMoldesElem(CRsMoldes_Inyectora& p_RsMoldes_Inyectora)
	: CObject()
{
	m_sTipo = TK_MOLDE_ELEM_INYECTORA;
	m_sIDMolde = p_RsMoldes_Inyectora.m_idMolde;
	m_sIDMolde.TrimRight();
	if (p_RsMoldes_Inyectora.IsFieldNull(&p_RsMoldes_Inyectora.m_id)) m_lId = 0;
	else m_lId = p_RsMoldes_Inyectora.m_id;
	m_sNombre = p_RsMoldes_Inyectora.m_nombre;
	if (p_RsMoldes_Inyectora.IsFieldNull(&p_RsMoldes_Inyectora.m_cantidad)) m_dCantidadMax = 0.0;
	else m_dCantidadMax = p_RsMoldes_Inyectora.m_cantidad;
	m_iAux = 0;

}
CBufMoldesElem::CBufMoldesElem(CRsMoldes_Soplado& p_RsMoldes_Soplado)
	: CObject()
{
	m_sTipo = TK_MOLDE_ELEM_SOPLADO;
	m_sIDMolde = p_RsMoldes_Soplado.m_idMolde;
	m_sIDMolde.TrimRight();
	m_sNombre = p_RsMoldes_Soplado.m_nombre;
	if (p_RsMoldes_Soplado.IsFieldNull(&p_RsMoldes_Soplado.m_id)) m_lId = 0;
	else m_lId = p_RsMoldes_Soplado.m_id;
	if (p_RsMoldes_Soplado.IsFieldNull(&p_RsMoldes_Soplado.m_cantidad)) m_dCantidadMax = 0.0;
	else m_dCantidadMax = p_RsMoldes_Soplado.m_cantidad;
	m_iAux = 0;
}

CBufMoldesElem::CBufMoldesElem(CBufMoldesElem& p_BufMoldesElem)
	: CObject()
{
	m_sTipo = p_BufMoldesElem.m_sTipo;
	m_lId = p_BufMoldesElem.m_lId;
	m_sIDMolde = p_BufMoldesElem.m_sIDMolde;
	m_sNombre = p_BufMoldesElem.m_sNombre;
	m_dCantidadMax = p_BufMoldesElem.m_dCantidadMax;
	m_iAux = 0;
}

/////////////////////////////////////////////////////////////////////////////
// 
// CRsMoldes_Soplado

IMPLEMENT_DYNAMIC(CRsMoldes_Soplado, CRecordset)

CRsMoldes_Soplado::CRsMoldes_Soplado(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRsMoldes_Soplado)
	m_idMolde = _T("");
	m_id = 0;
	m_nombre = _T("");
	m_cantidad = 0.0;
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRsMoldes_Soplado::GetDefaultConnect()
{
	return _T("ODBC;DSN=MS Access Database");
}

CString CRsMoldes_Soplado::GetDefaultSQL()
{
	return _T("[Moldes_Soplado_Comp]");
}

void CRsMoldes_Soplado::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRsMoldes_Soplado)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[IDMolde]"), m_idMolde);
	RFX_Long(pFX, _T("[IDSoplado]"), m_id);
	RFX_Text(pFX, _T("[Nombre]"), m_nombre);
	RFX_Double(pFX, _T("[Cantidad]"), m_cantidad);
	//}}AFX_FIELD_MAP
	m_idMolde.TrimRight();
	m_nombre.TrimRight();
}

/////////////////////////////////////////////////////////////////////////////
// CRsMoldes_Soplado diagnostics

#ifdef _DEBUG
void CRsMoldes_Soplado::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRsMoldes_Soplado::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CBufMoldes_Soplado



