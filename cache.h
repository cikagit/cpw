// cache.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCache class

class CProgMaq;
class CProg;
class CBufArticulo;
class CObArrayOrd;
class CBufMaquina;
class CBufOF;
class CBufCambiosMoldes;

class CComp : public CObject
{
protected:
	DECLARE_SERIAL(CComp)

public:

	CComp(CComp* pComp = NULL);
	CComp(CBufOF* pBufOF);
	BOOL operator ==(const CComp& p_Comp) const;
	BOOL operator <(const CComp& p_Comp) const;


	CTime	m_FecNec;
	CString	m_sIDArticulo;
	CString m_sIDCliente;
	double	m_dCantAyer;
	double	m_dCantHoy;
	long	m_lSeccion;

};

class CCache : public CObject
{
// Field/Param Data

	protected:
	CRsArticulo m_RsArticulo;
	CRsAcabado m_RsAcabado;
	CRsComponentes m_RsComponentes;
	CRsMaquinas m_RsMaquinas;
	CRsCliente m_RsCliente;
	CRsEntregas m_RsEntregas;
	CRsArtMol m_RsArtMol;
	CRsHistorico m_RsHistorico;
	CRsCalendario m_RsCalendario;
	CRsLocales m_RsLocales;
	CRsMezclas m_RsMezclas;
	CRsMoldes m_RsMoldes;
	CRsMolMaq m_RsMolMaq;
	CRsListadoValorado m_RsListadoValorado;
	CRsCambiosMoldes m_RsCambiosMoldes;
	CRsMezclasPlanta m_RsMezclasPlanta;
	CRsLisMezclas m_RsLisMezclas;
	CRsLisGeneral m_RsLisGeneral;

	CRsMoldes_Inyectora m_RsMoldes_Inyectora;
	CRsMoldes_Soplado m_RsMoldes_Soplado;

	CRsPedidos* m_pRsPedidos;
	CRsPedidosAyer* m_pRsPedidosAyer;
	


	CObArray*	m_pArrLocales;
	CObArray*	m_pArrArticulos;
	CObArray*	m_pArrMoldes;
	CObArray*	m_pArrMolMaq;
	CObArray*	m_pArrOFs;
	CObArray*	m_pArrMaquinas;
	CObArray*	m_pArrMezclas;
	CObArrayOrd* m_pArrOFComb;			// Array de OFs combinadas
	CObArrayOrd* m_pArrOFSeccion;		// Array de OFs ordenadas por seccion
	CObArray*	m_pArrOFSel;			// Array de OFs seleccionadas
	CObArray*	m_pArrOFAcum;			// Array de OFs Acumuladas (1 por fecha)
	CObArray*	m_pArrOFAyer;			// Array de OFs del d�a anterior
	CObArray*	m_pArrCambMold;			// Array de cambios de moldes
	CObArray*	m_pArrHomologadas;
		
	CDatabase* m_pDB;

	bool		m_bConnPedidosIsDefault;

// Implementation
public:         
	BOOL EsMoldeComun(const CString& p_sIDA, const CString& p_sIDB);
	BOOL LimitMolMaq(const CString& p_sIDMolde, const CString& p_sIDMaquina);
	void AddHistorico(COrdMaq* p_pOM);
	void CargaLisMezclas(CObArray* p_ArrBufLisMezclas);
	void CargaLisGeneral(const CListCtrl* p_pListCtrl);
	
	CBufLocal* FindpLocal(const CString &p_sLocal);
	int	GetiDiasMezcla(const CString &p_sLocal);
	int	GetiDiasRetorno(const CString &p_sLocal);
	int GetiFrecuenciaMezcla(const CString &p_sLocal);


	CBufCambiosMoldes* GetNextCambioMolde(CTime p_Fec);
	CBufCambiosMoldes* GetFirstCambioMolde(CTime p_Fec);
	void ActLimitMaq(const CString& p_sMolde, const CString& p_sMaq);
	void UpdateMezclasPlanta();
	void SetOrdenOF();
	void RecalcOFs(CProg* p_pProg, const char* p_sIDArt = NULL);
	long GetlGFH(const CString& p_sMaquina);

	CBufMaquina* FindpMaquina(const CString &p_sMaquina);
	CBufMolde* FindpMolde( const CString& p_sMolde );
	CBufMezcla* FindpMezcla(const CString& p_sMezcla );
	void LinkPointer(BOOL m_BRecarga = FALSE);

	CBufArticulo* FindpArticulo(const CString& p_sArticulo);
	int GetArrMaq( const CString& p_sArticulo, CObArray& p_ArrMaq, CObArray& p_ArrMoldes);
	int GetArrMolMaq(const CString &p_sMaquina, CObArray &p_ArrMolMaq, const CString* p_psIDArt = NULL);
	int GetArrMolArt(const CString &p_sMaquina, CObArray &p_ArrMolArt );
	
	int	GetArrMoldeDetalle(const CString& p_sIDArt, CStringArray& p_pArrMoldeDetalle);

	int	GetArrComponentes(const CString& p_sIDArt, CObArray& p_pArrComponentes);
	int	GetArrAcabado(const CString& p_sIDArt, CObArray& p_pArrAcabado);
	double GetdCadenciaAcabado(const CString &p_sArticulo, long p_lCentro = -1);

	CCache(CDatabase* miDb);
	~CCache();   
	
	CBufArticulo* AddArticulo(const char* IDArticulo);
	CBufMaquina* AddMaquina(const char* IDMaquina);
	CBufOF* FindOFID( long p_lOFID );
	CBufOF* FindOFNum( long p_lOFNum );
	CBufOF* FindOFArt( const CString& p_sArticulo , int& m_iIndFirtsOM);

	void UpdateOF( const CString& p_sIDArt , int p_iCant );
	bool GetBConnPedidosIsDefault() { return m_bConnPedidosIsDefault;};
	void SetBConnPedidosIsDefault(bool p_bDefault) { m_bConnPedidosIsDefault = p_bDefault;};
	
	int GetNumOF();
	CBufOF* GetpOFAt( int p_iInd );

	void AssignInacts( CProgMaq& p_PM );

	int GetArrMaquinas( CStringArray& p_ArrMaquinas, long p_lGFH = -1);     
	void GetArrAcum(CObArray *p_pArrOF, CObArray *p_pArrAcum);
	int GetArrOFs( CObArray& p_ArrOFs );
	int GetArrConsOF( CObArray& p_ArrOFs );
	int GetArrGFHs( CStringArray& p_ArrMaquinas );   
	
	CObArray* GetArrBufArt();
	CObArray* GetArrBufMezcla();

	int GetArrBloques (CUIntArray& p_ArrBloques);  
	void GetMaxMinBlq( int p_iBloque, int& p_iMinBlq, int& p_iMaxBlq);
	CString GetsGFH( const char* IDMaquina );                 
	int GetiNumTurnos(CTime p_FecTur, const CProgMaq& p_PM, BOOL BTurnos[] = NULL);
	void SetTurnos(const CFecTur p_FecTur, const CProgMaq* p_pPM);
	void SerializeOFs(CArchive& ar);
	CTime GetFecNoProg( const CBufArticulo*  p_pBufArticulo );
	CTime GetFecNoStock( const CBufArticulo*  p_pBufArticulo );
	long GetlPeorMargen( const CBufArticulo*  p_pBufArticulo );
	long GetlHorasFab( const CBufArticulo*  p_pBufArticulo , CTime p_Fec);
	
	
	CBufOF* AddOF( long p_ID , BOOL p_bUpdateFec = FALSE );
	void AddOF( CBufOF* p_pOF );
	
	void DeleteBufOF( CBufOF* p_pOF);

	
	void IniFillCache(BOOL m_BRecarga = FALSE);
	void IniFillLocales();
	void IniFillArticulos();
	void IniFillMaquinas();
	void IniFillMolMaq();
	void IniFillOFs();
	void IniFillOFsAyer();
	void IniFillMezclas();
	void IniFillHomologadas();

	void EmptyArrays();
	
	void Prueba();

	void EscribirFechaParam(const CString& p_sClave);
	CString LeerFechaParam(const CString& p_sClave);
	CString GetStringValue(CString& selQuery);
	bool GetNoHomologada(CString& p_sArticulo);
	CString GetsHomologacion(CString& p_sArticulo, bool trace = false);
	CString GetsPropietario(CString& p_sArticulo);



};


/////////////////////////////////////////////////////////////////////////////
// OFComb


// Estructura auxiliar

class COFComb : public CObject
{
protected:
	DECLARE_SERIAL(COFComb)


// Attributes
public:
	CString	m_sID;
	int		m_iIni;
	int		m_iFin;

// Operations
public:
	COFComb();
	
	virtual void Serialize(CArchive& ar);   
	
// Implementation
public:
	virtual ~COFComb();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


class CInfoMolArt : public CObject
{
public:
	long		m_lPrioridad;				// Prioridad que indica la base de datos
	CString		m_sIDArticulo;				// C�digo del art�culo
	long		m_lPorcHist;				// Porcentaje de veces que ese molde ha usado esa m�quina en el pasado
	long		m_lHorasRetraso;			// Horas de retraso que tiene esa orden
	CTime		m_FecProg;					// Fecha hasta la que est� cubierta con el stock y lo programado
	CTime		m_FecStock;					// Fecha hasta la que est� cubierta s�lo con el stock
	BOOL		m_BProg;					// Indica si este art�culo est� programado o no

	long		m_lHoras0;					// Horas de programaci�n a 0 d�as
	long		m_lHoras7;					// Horas de programaci�n a 7 d�as
	long		m_lHoras15;					// Horas de programaci�n a 15 d�as
	long		m_lHoras21;					// Horas de programaci�n a 21 d�as
	long		m_lHoras30;					// Horas de programaci�n a 30 d�as
	long		m_lHoras37;					// Horas de programaci�n a 37 d�as
	long		m_lHoras45;					// Horas de programaci�n a 45 d�as
	long		m_lHoras52;					// Horas de programaci�n a 52 d�as
	long		m_lHoras60;					// Horas de programaci�n a 60 d�as
	
	CInfoMolArt();
	
protected:
	DECLARE_DYNAMIC(CInfoMolArt)   
	
};


inline BOOL OFComb_Bigger( CObject* p_pA, CObject* p_pB ) { return ((COFComb*)p_pA)->m_sID > ((COFComb*)p_pB)->m_sID; }
inline BOOL OFComb_Equal( CObject* p_pA, CObject* p_pB ) { return ((COFComb*)p_pA)->m_sID == ((COFComb*)p_pB)->m_sID; }

inline BOOL OFSeccion_Bigger( CObject* p_pA, CObject* p_pB );
inline BOOL OFSeccion_Equal( CObject* p_pA, CObject* p_pB ) ;

inline BOOL BufArt_Bigger( CObject* p_pA, CObject* p_pB );
inline BOOL BufArt_Equal( CObject* p_pA, CObject* p_pB );
