// cpwdb.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CRsArticulo recordset

#include <afxdb.h>
class CRsArticulo : public CRecordset
{
public:
	CRsArticulo(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsArticulo)

// Field/Param Data
	//{{AFX_FIELD(CRsArticulo, CRecordset)
	long	m_CantAlmacen;
	long	m_CantFabric;
	long	m_CantPauta;
	long	m_CentroReb;
	CString	m_ID;
	long	m_MaduracionMax;
	long	m_MaduracionMed;
	long	m_MaduracionMin;
	CString	m_Mezcla;
	double	m_P_H_iny;
	double	m_PCAct;
	double	m_PCBase;
	double	m_PCHist;
	double	m_PCUlt;
	double	m_Peso;
	double	m_PHAct;
	double	m_PHBase;
	double	m_PHHist;
	double	m_PHUlt;
	double	m_PiezasHoraReb;
	double	m_PorcChatarras;
	CString	m_Planificador;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsArticulo)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

class CBufMezcla;
class CBufOF;
class CTimeLine;

class CBufArticulo : public CObject
{
public:
#ifdef _DEBUG
		// Override
		virtual void AssertValid() const;
#endif
	double GetdCadencia();
	double GetdCadenciaAcabado(long p_lCentro = -1);
	double GetdChatarras();
	long GetlCantMezcla();
	CBufOF* GetpBufOF() const { return m_pBufOF;};
	int	    GetiIndFirstOF() const { return m_iIndFirstOF; };

	void SetpBufOF(const CString& p_sIDArticulo);

	CBufArticulo();
	CBufArticulo(CRsArticulo& p_RsArticulo);
	void AssignData(CRsArticulo& p_RsArticulo);

	~CBufArticulo();

	long	m_lCantAlmacen;
	long	m_lCantFabric;
	long	m_lCantPauta;
	long	m_lCentroReb;
	CString	m_sID;
	long	m_lMaduracionMax;
	long	m_lMaduracionMed;
	long	m_lMaduracionMin;
	CString	m_sMezcla;
	double	m_dP_H_iny;
	double	m_dPCAct;
	double	m_dPCBase;
	double	m_dPCHist;
	double	m_dPCUlt;
	double	m_dPeso;
	double	m_dPHAct;
	double	m_dPHBase;
	double	m_dPHHist;
	double	m_dPHUlt;
	double	m_dPiezasHoraReb;
	double	m_dPorcChatarras;

	CString	m_LV_sCliente;
	double	m_LV_dP_H_real;
	double	m_LV_dRechazos;
	long	m_LV_lCantidad_Fabricada;
	long	m_LV_lCantidad_Rechazada;
	long	m_LV_lTotal_Horas;


	CBufMezcla* m_pBufMezcla;
	
	CObArray*	m_pArrArtMol;
	CObArray* m_pArrAcabado;
	CObArray* m_pArrComponentes;

	CBufOF*		m_pBufOF;
	int			m_iIndFirstOF;		// Index to the first OF in g_pCache::m_pArrOFs
	CString	m_sPlanificador;
// Implementation
protected:
	DECLARE_DYNAMIC(CBufArticulo)
	
};


/////////////////////////////////////////////////////////////////////////////
// CRsArtMol recordset

class CRsArtMol : public CRecordset
{
public:
	CRsArtMol(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsArtMol)

// Field/Param Data
	//{{AFX_FIELD(CRsArtMol, CRecordset)
	long	m_Cantidad;
	CString	m_IDArticulo;
	CString	m_IDMolde;
	long	m_Orden;
	CString	m_Postizo;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsArtMol)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};


class CBufMolde;

class CBufArtMol : public CObject
{
public:
	CBufArtMol(CRsArtMol& p_RsArtMol);

	CString	m_sIDArticulo;
	long	m_lOrden;
	CString	m_sIDMolde;
	long	m_lCantidad;
	CString	m_sPostizo;

	CBufMolde* m_pBufMolde;
	CBufArticulo* m_pBufArticulo;
	// Implementation
protected:
	DECLARE_DYNAMIC(CBufArtMol)
	
//	double GetdCantidad( const char* IDMolde );
};


/////////////////////////////////////////////////////////////////////////////
// CRsCliente recordset

class CRsCliente : public CRecordset
{
public:
	CRsCliente(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsCliente)

// Field/Param Data
	//{{AFX_FIELD(CRsCliente, CRecordset)
	CString	m_ID;
	CString	m_Nombre;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsCliente)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
/////////////////////////////////////////////////////////////////////////////
// CRsEntregas recordset

class CRsEntregas : public CRecordset
{
public:
	CRsEntregas(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsEntregas)

// Field/Param Data
	//{{AFX_FIELD(CRsEntregas, CRecordset)
	CString	m_IDMezcla;
	CTime	m_Fecha;
	long	m_Orden;
	long	m_Cantidad;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsEntregas)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};


class CBufEntregas : public CObject
{
public:
	CBufEntregas(CRsEntregas& p_RsEntregas);

	CString	m_sIDMezcla;
	CTime	m_Fecha;
	long	m_lOrden;
	long	m_lCantidad;

	// Implementation
protected:
	DECLARE_DYNAMIC(CBufEntregas)
	
};




/////////////////////////////////////////////////////////////////////////////
// CRsHistorico recordset

class CRsHistorico : public CRecordset
{
public:
	CRsHistorico(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsHistorico)

// Field/Param Data
	//{{AFX_FIELD(CRsHistorico, CRecordset)
	CString	m_IDMolde;
	CString	m_IDMaquina;
	CTime	m_FecIni;
	CTime	m_FecFin;
	long	m_Cantidad;
	CString	m_IDArticulo;
	double	m_Cadencia;
	double	m_Rechazos;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsHistorico)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
/////////////////////////////////////////////////////////////////////////////
// CRsLocales recordset

class CRsLocales : public CRecordset
{
public:
	CRsLocales(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsLocales)

// Field/Param Data
	//{{AFX_FIELD(CRsLocales, CRecordset)
	long	m_DiasRetorno;
	long	m_DiasTransporte;
	long	m_Frecuencia;
	CString	m_ID;
	CString	m_Nombre;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsLocales)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

class CBufLocal : public CObject
{
public:
	CBufLocal();
	CBufLocal(CRsLocales& p_RsLocales);
	CBufLocal(CBufLocal& p_BufLocal);
	~CBufLocal();
	void AssignData(CRsLocales& p_RsLocales);
	

	CString	m_sID;
	CString	m_sNombre;
	long	m_iDiasTransporte;
	long	m_iFrecuencia;
	long	m_iDiasRetorno;
	
// Implementation
protected:
	DECLARE_DYNAMIC(CBufLocal)
	
};


/////////////////////////////////////////////////////////////////////////////
// CRsMaquinas recordset

class CRsMaquinas : public CRecordset
{
public:
	CRsMaquinas(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsMaquinas)

// Field/Param Data
	//{{AFX_FIELD(CRsMaquinas, CRecordset)
	long	m_GFH;
	CString	m_ID;
	CString	m_IDLocal;
	long	m_Inact;
	CString	m_Nombre;
	CString	m_NombreGFH;
	long	m_orden;
	double	m_Personas;
	BOOL	m_Programable;
	CString	m_Tipo;
	long	m_lOperacion;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsMaquinas)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};


class CBufMaquina : public CObject
{
public:
	CBufMaquina();
	CBufMaquina(CRsMaquinas& p_RsMaquinas);
	CBufMaquina(CBufMaquina& p_BufMaquina);
	~CBufMaquina();
	void AssignData(CRsMaquinas& p_RsMaquinas);
	
	CString	m_sID;
	CString	m_sTipo;
	CString	m_sNombre;
	CString	m_sIDLocal;
	long	m_lInact;
	long	m_lOrden;
	long	m_lGFH;
	CString	m_sNombreGFH;
	BOOL	m_bProgramable;
	double  m_dPersonas;
	long	m_lOperacion;
	
// Implementation
protected:
	DECLARE_DYNAMIC(CBufMaquina)
	
};




/////////////////////////////////////////////////////////////////////////////
// CRsMezclas recordset

class CRsMezclas : public CRecordset
{
public:
	CRsMezclas(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsMezclas)

// Field/Param Data
	//{{AFX_FIELD(CRsMezclas, CRecordset)
	CString	m_ID;
	CString	m_Color;
	long	m_Cantidad;
	long	m_Lote;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsMezclas)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};


class CBufMezcla : public CObject
{
public:
	CBufMezcla();
	CBufMezcla(CRsMezclas& p_RsMezclas);
	CBufMezcla(CBufMezcla& p_BufMezclas);
	~CBufMezcla();
	void AssignData(CRsMezclas& p_RsMezclas);
	long GetlCantidad();

	CString	m_sID;
	CString	m_sColor;
	long	m_lCantidad;
	long	m_lCantPlanta;
	long	m_lLote;

	CObArray*	m_pArrEntregas;

//	CTimeLine*	m_pTL;
	
// Implementation
protected:
	DECLARE_DYNAMIC(CBufMezcla)
	
};





/////////////////////////////////////////////////////////////////////////////
// CRsMoldes recordset

class CRsMoldes : public CRecordset
{
public:
	CRsMoldes(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsMoldes)

// Field/Param Data
	//{{AFX_FIELD(CRsMoldes, CRecordset)
	CString	m_ID;
	CString	m_Alternancia;
	CString	m_Tipo;
	long	m_Tiradas;
	CString	m_Estado;
	CTime	m_Disponibilidad;
	BOOL	m_CanalFrio;
	BOOL	m_CercoREP;
	BOOL	m_ExtractorAutomatico;
	BOOL	m_DoblePiston;
	BOOL	m_ExtractorMK19;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsMoldes)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};


/////////////////////////////////////////////////////////////////////////////
// CBufMolde recordset

class CBufMolde : public CObject
{

public:
#ifdef _DEBUG
	// Override
	virtual void AssertValid() const;
#endif
	CBufMolde();
	CBufMolde(CRsMoldes& p_RsMoldes);
	CBufMolde(CBufMolde& p_BufMoldes);
	~CBufMolde();
	void AssignData(CRsMoldes& p_RsMoldes);
	bool TieneDetalle();
	
	CString	m_sID;
	CString	m_sAlternancia;
	CString	m_sTipo;
	long	m_lTiradas;
	CString	m_sEstado;
	CTime	m_FecDisponibilidad;

	BOOL	m_BCanalFrio;
	BOOL	m_BCercoREP;
	BOOL	m_BExtractorAutomatico;
	BOOL	m_BDoblePiston;
	BOOL	m_BExtractorMK19;



	CObArray*	m_pArrMolArt;
	CObArray*	m_pArrMolMaq;

	CObArray*	m_pArrMolElem;


// Implementation
protected:
	DECLARE_DYNAMIC(CBufMolde)
	
};



/////////////////////////////////////////////////////////////////////////////
// CRsMolMaq recordset

class CRsMolMaq : public CRecordset
{
public:
	CRsMolMaq(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsMolMaq)

// Field/Param Data
	//{{AFX_FIELD(CRsMolMaq, CRecordset)
	CString	m_IDMolde;
	long	m_Prioridad;
	CString	m_IDMaquina;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsMolMaq)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

class CBufMolMaq : public CObject
{
public:
	CBufMolMaq() {};
	CBufMolMaq(CRsMolMaq& p_RsMolMaq);
	~CBufMolMaq() {};

	CString	m_sIDMolde;
	long	m_lPrioridad;
	CString	m_sIDMaquina;
	
	CBufMaquina* m_pBufMaquina;
	CBufMolde* m_pBufMolde;
	// Implementation
protected:
	DECLARE_DYNAMIC(CBufMolMaq)
	
};


/////////////////////////////////////////////////////////////////////////////
// CRsPedidos recordset

class CRsPedidos : public CRecordset
{
public:
	CRsPedidos(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsPedidos)

// Field/Param Data
	//{{AFX_FIELD(CRsPedidos, CRecordset)
	double	m_CantAlmac;
	double	m_CantCurso;
	double	m_CantOrig;
	double	m_CantPauta;
	double	m_CantPend;
	CTime	m_FecNec;
	CString	m_IDArticulo;
	long	m_IDCliente;
	CString	m_Necesidad;
	CString	m_Nomcli;
	long	m_Seccion;
	double	m_CantObsoletos;
	double	m_CantStockSeguridad;
	double	m_Contratas;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsPedidos)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

class CRsPedidosAyer;

class CBufOF : public CObject
{
public:
	void SetSelect(BOOL p_bSelected) {	m_bSelected = p_bSelected;};
	BOOL GetbSelected() { return m_bSelected;	};
	void SetComprimido(BOOL p_bComprimido) {	m_bComprimido = p_bComprimido; };
	BOOL GetbComprimido() {	return m_bComprimido;	};

	COLORREF GetColor();


	CBufOF(CRsPedidos& p_RsPedidos);
	CBufOF(CRsPedidosAyer& p_RsPedidosAyer);
//	CBufOF(CBufOF& p_BufOF);

	//virtual CBufOF& operator =(CBufOF& p_BufOF);


	CBufOF*	GetID() { return this; };		// same return type as COrdMaq::GetIDOF()

	long	m_lMargenMax;	// Margen sobre el periodo de maduración maximo
	long	m_lMargenMed;	// Margen sobre el periodo de maduración medio
	long	m_lMargenMin;	// Margen sobre el periodo de maduración mínimo
	CTime	m_FecFab;		// Fecha en la que realmente se va a fabricar. FecFin de Prog si no se llega a fabricar.
	double	m_dNoProg;		// Cantidad del Acumulado que no está fabricada
	double	m_dNoCubierta;		// Cantidad del Acumulado que no está cubierta
	double	m_dPendAcum;
	long	m_lHorasAcum;	// Horas necesarias para completar la columna Acumulado de NoCubiertas
	double	m_dCantAlmac;
	double	m_dCantCurso;
	double	m_dCantOrig;
	double	m_dCantPauta;
	double	m_dCantPend;
	double	m_dCantObsoletos;
	double	m_dCantStockSeguridad;
	CTime	m_FecNec;
	CString	m_sIDArticulo;
	long	m_lIDCliente;
	CString	m_sNecesidad;
	CString	m_sNomcli;
	long	m_lSeccion;
	double	m_dContratas;
	CBufArticulo* m_pBufArt;                                
	
// Implementation
protected:
	BOOL m_bSelected;
	BOOL m_bComprimido;
	DECLARE_DYNAMIC(CBufOF)   
	CBufOF();

	void AssignData(CRsPedidos& p_RsPedidos);

	
//	virtual void Serialize(CArchive& ar);
	
};


/////////////////////////////////////////////////////////////////////////////
// CRsListadoValorado recordset

class CRsListadoValorado : public CRecordset
{
public:
	CRsListadoValorado(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsListadoValorado)

// Field/Param Data
	//{{AFX_FIELD(CRsListadoValorado, CRecordset)
	CString	m_Articulo;
	CString	m_Cliente;
	double	m_P_H_real;
	double	m__Rechazos;
	long	m_Cantidad_Fabricada;
	long	m_Cantidad_Rechazada;
	long	m_Total_Horas;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsListadoValorado)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
/////////////////////////////////////////////////////////////////////////////
// CRsPedidosAyer recordset

class CRsPedidosAyer : public CRecordset
{
public:
	CRsPedidosAyer(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsPedidosAyer)

// Field/Param Data
	//{{AFX_FIELD(CRsPedidosAyer, CRecordset)
	CString	m_IDArticulo;
	long	m_IDCliente;
	CString	m_Nomcli;
	CString	m_Necesidad;
	double	m_CantOrig;
	double	m_CantPend;
	CTime	m_FecNec;
	double	m_CantAlmac;
	double	m_CantCurso;
	double	m_CantPauta;
	long	m_Seccion;
	double	m_CantObsoletos;
	double	m_CantStockSeguridad;
	double	m_Contratas;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsPedidosAyer)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
/////////////////////////////////////////////////////////////////////////////
// CRsCambiosMoldes recordset

class CRsCambiosMoldes : public CRecordset
{
public:
	CRsCambiosMoldes(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsCambiosMoldes)

// Field/Param Data
	//{{AFX_FIELD(CRsCambiosMoldes, CRecordset)
	long	m_Contador;
	CString	m_IDMaquina;
	CString	m_IDMoldeEnt;
	CString	m_IDMoldeSal;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsCambiosMoldes)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

class CBufCambiosMoldes : public CObject
{
public:
	CBufCambiosMoldes(CRsCambiosMoldes& p_RsCambiosMoldes);


	CString	m_sIDMaquina;
	CTime	m_Fecha;
	CString	m_sIDMoldeSal;
	CString	m_sIDMoldeEnt;

// Implementation
protected:
	DECLARE_DYNAMIC(CBufCambiosMoldes)   
	CBufCambiosMoldes();
	
};

/////////////////////////////////////////////////////////////////////////////
// CRsMezclasPlanta recordset

class CRsMezclasPlanta : public CRecordset
{
public:
	CRsMezclasPlanta(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsMezclasPlanta)

// Field/Param Data
	//{{AFX_FIELD(CRsMezclasPlanta, CRecordset)
	CString	m_ID;
	long	m_Cantidad;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsMezclasPlanta)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
/////////////////////////////////////////////////////////////////////////////
// CRsLisMezclas recordset

class CRsLisMezclas : public CRecordset
{
public:
	CRsLisMezclas(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsLisMezclas)

// Field/Param Data
	//{{AFX_FIELD(CRsLisMezclas, CRecordset)
	long	m_Cant;
	CTime	m_FecIni;
	CString	m_IDArticulo;
	CString	m_IDCliente;
	CString	m_IDLocal;
	long	m_Mezcla;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsLisMezclas)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

class CBufLisMezclas : public CObject
{
public:
	CBufLisMezclas(CRsLisMezclas& p_RsLisMezclas);


	CString		m_sIDLocal;
	CString		m_sIDCliente;
	CString		m_sIDArticulo;
	CTime		m_FecIni;
	long		m_lMezcla;
	long		m_lCant;
	

// Implementation
protected:
	DECLARE_DYNAMIC(CBufLisMezclas)   
	CBufLisMezclas();
	
};
/////////////////////////////////////////////////////////////////////////////
// CRsCalendario recordset

class CRsCalendario : public CRecordset
{
public:
	CRsCalendario(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsCalendario)

// Field/Param Data
	//{{AFX_FIELD(CRsCalendario, CRecordset)
	CString	m_ID;
	CTime	m_Fecha;
	BOOL	m_Turno1;
	BOOL	m_Turno2;
	BOOL	m_Turno3;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsCalendario)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
/////////////////////////////////////////////////////////////////////////////
// CRsLisGeneral recordset

class CRsLisGeneral : public CRecordset
{
public:
	CRsLisGeneral(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsLisGeneral)
#define m_ciNumVal 20
	CString* m_sArrVal[m_ciNumVal];
// Field/Param Data
	//{{AFX_FIELD(CRsLisGeneral, CRecordset)
	CString	m_Texto1;
	CString	m_Texto10;
	CString	m_Texto11;
	CString	m_Texto12;
	CString	m_Texto13;
	CString	m_Texto14;
	CString	m_Texto15;
	CString	m_Texto16;
	CString	m_Texto17;
	CString	m_Texto18;
	CString	m_Texto19;
	CString	m_Texto2;
	CString	m_Texto20;
	CString	m_Texto3;
	CString	m_Texto4;
	CString	m_Texto5;
	CString	m_Texto6;
	CString	m_Texto7;
	CString	m_Texto8;
	CString	m_Texto9;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsLisGeneral)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
/////////////////////////////////////////////////////////////////////////////
// CRsAcabado recordset

class CRsAcabado : public CRecordset
{
public:
	CRsAcabado(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsAcabado)

// Field/Param Data
	//{{AFX_FIELD(CRsAcabado, CRecordset)
	CString	m_articulo;
	long	m_operacion;
	CString	m_NombreOperacion;
	long	m_centro;
	CString	m_nombrecentro;
	double	m_cadencia;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsAcabado)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

class CBufAcabado : public CObject
{
public:
	CBufAcabado(CRsAcabado& p_RsAcabado);

	CString	m_sArticulo;
	long	m_lOperacion;
	CString	m_sNombreOperacion;
	long	m_lCentro;
	CString	m_sNombreCentro;
	double	m_dCadencia;

	CBufArticulo* m_pBufArticulo;
	// Implementation
protected:
	DECLARE_DYNAMIC(CBufAcabado)

	CString GetsInfo(long p_lCant);
	
};

/////////////////////////////////////////////////////////////////////////////
// CRsHomologadas recordset

class CRsHomologadas : public CRecordset
{
public:
	CRsHomologadas(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsHomologadas)

// Field/Param Data
	//{{AFX_FIELD(CRsHomologadas, CRecordset)
	long	m_Empresa;
	CString	m_Articulo;
	CString	m_Tipo;
	CString	m_Homologacion;
	long	m_lRespFab;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsHomologadas)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

class CBufHomologada : public CObject
{
public:
	CBufHomologada(CRsHomologadas& p_RsHomologada);

	long	m_lEmpresa;
	CString	m_sArticulo;
	CString	m_sTipo;
	CString	m_sHomologacion;
	long	m_lRespFab;
	
	CBufArticulo* m_pBufArticulo;
	// Implementation
protected:
	DECLARE_DYNAMIC(CBufHomologada)

	
};

/////////////////////////////////////////////////////////////////////////////
// CRsComponentes recordset

class CRsComponentes : public CRecordset
{
public:
	CRsComponentes(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsComponentes)

	// Field/Param Data
		//{{AFX_FIELD(CRsComponentes, CRecordset)
	CString	m_articulo;
	CString m_componente;
	double	m_cantidad;
	double  m_stock;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsComponentes)
public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

class CBufComponentes : public CObject
{
public:
	CBufComponentes(CRsComponentes& p_RsComponentes);

	CString	m_sArticulo;
	CString	m_sComponente;
	double	m_dCantidad;
	double  m_dStock;

	CBufArticulo* m_pBufArticulo;
	// Implementation
protected:
	DECLARE_DYNAMIC(CBufComponentes)

};





/////////////////////////////////////////////////////////////////////////////
// CRsMoldes_Inyectora recordset

class CRsMoldes_Inyectora : public CRecordset
{
public:
	CRsMoldes_Inyectora(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsMoldes_Inyectora)

	// Field/Param Data
		//{{AFX_FIELD(CRsMoldes_Inyectora, CRecordset)
	CString	m_idMolde;
	long	m_id;
	CString m_nombre;
	double	m_cantidad;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsMoldes_Inyectora)
public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CRsMoldes_Soplado recordset

class CRsMoldes_Soplado : public CRecordset
{
public:
	CRsMoldes_Soplado(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRsMoldes_Soplado)

	// Field/Param Data
		//{{AFX_FIELD(CRsMoldes_Soplado, CRecordset)
	CString	m_idMolde;
	long	m_id;
	CString m_nombre;
	double	m_cantidad;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsMoldes_Soplado)
public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};





class CBufMoldesElem : public CObject
{
public:
	CBufMoldesElem() {};
	CBufMoldesElem(CRsMoldes_Inyectora& p_RsMoldes_Inyectora);
	CBufMoldesElem(CBufMoldesElem& p_BufMoldes_Inyectora);
	CBufMoldesElem(CRsMoldes_Soplado& p_RsMoldes_Soplado);
	~CBufMoldesElem() {};

	CString	m_sTipo; // "Iny" o "Sop" dependiendo de si es inyectora o soplado
	CString	m_sIDMolde;
	long	m_lId;
	CString	m_sNombre;
	double	m_dCantidadMax; 
	int		m_iAux; // Entero auxiliar para contadores en proceso
	// Implementation
protected:
	DECLARE_DYNAMIC(CBufMoldesElem)

};

