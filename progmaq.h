
class CProg;
class CObArray;     
class COrdMaq;    

enum enumSelecOrd { SinCambios, SelecTrue, SelecFalse };

class CProgMaq : public CObject
{
// Serialización
	DECLARE_SERIAL(CProgMaq)
	virtual void Serialize(CArchive& ar);
// Miembros
	protected:
		
		CBufMaquina*	m_pBufMaquina;
		CPoint			m_PntPosition;
		CProg*			m_pProg;
		BOOL			m_BSelected;
        BOOL			m_BEditCal;
		CString			m_sID;
        CObArray*		m_pArrOMs;
		CObArray*		m_pArrInacts;

		int				m_iDiasRetorno;			//Dias de retorno de las piezas, a restar al margen


	public:
		long GetlGFH();
		int GetDiasRetorno();
		CTime AdjustInact(CTime p_ts, BOOL BIni = FALSE);
		int GetiDiasMezcla();
		double GetdPersonas();

		int GetiDiasRetorno();
		int GetiFrecuenciaMezcla();

		int GetArrMoldesEnMaq(const CString& p_sIDArt, CObArray& p_ArrMoldes);
		int GetArrMoldesYaUsados(const CString& p_sIDArt, CObArray& p_ArrMoldes);
		COrdMaq* FindOMbyMolde(const CString& p_sIDMolde, 
								CObArray* p_pArrBorrar = NULL, 
								CObArray* p_pArrNoFab = NULL );
		void Relink();
		CString GetsIDLocal();
		double GetdInactividad();
		CTime			m_FecOrd;		// Fecha para ordenaciones

		CProgMaq(const char* p_sID = NULL);
		friend void CCache::AssignInacts ( CProgMaq& p_PM );
		CProgMaq& operator =(const CProgMaq& p_ProgMaq);

		~CProgMaq();
		
 		
		CTime GetFecIni() const;		
		CTime GetFecFin() const;
		BOOL GetBSelected() const { return m_BSelected; };
		BOOL GetBEditCal() const { return m_BEditCal; };
		CString	GetsID() const		
		{ 
			ASSERT (m_pBufMaquina);
			return m_pBufMaquina->m_sID; 
		};
		
		CString	GetsNombre() const	
		{ 
			ASSERT (m_pBufMaquina);
			return m_pBufMaquina->m_sNombre; 
		};
		CString	GetsDescripcion() const	
		{ 
			ASSERT (m_pBufMaquina);
			//return m_pBufMaquina->m_sDescripcion; 
			return "";
		};
		CString	GetsGFHDesc() const	
		{ 
			ASSERT (m_pBufMaquina);
			return m_pBufMaquina->m_sNombreGFH; 
		};
		CString	GetsGFH() const	
		{ 
			ASSERT (m_pBufMaquina);
			CString sGFH;
			sGFH.Format("%ld", m_pBufMaquina->m_lGFH); 
			return sGFH;
		};
		long	GetlOperacion() const	
		{ 
			ASSERT (m_pBufMaquina);
			return m_pBufMaquina->m_lOperacion; 
		};		
		int GetNumOM() const;
		COrdMaq* GetpOM(int p_NumOrd) const;
		int GetNumInacts() const;
		CAbSpan* GetpInact(int p_NumInact) const;
		CPoint GetPosition() { return m_PntPosition; };
		CProg* GetpProg() { return m_pProg; };

		void SetProg( CProg* p_pProg );
		void SetPosition(CPoint& p_Point) { m_PntPosition = p_Point; };
		void SetBEditCal(BOOL p_BAct) { m_BEditCal = p_BAct; };
		void Selec( BOOL p_BSelecMaq, enum enumSelecOrd p_eSelecOrd = SinCambios,
					int iDesdeOrden = 0, int iHastaOrden = INT_MAX );
		void SetpBufMaq(CBufMaquina* p_pBufMaq);

		int		Add(COrdMaq* p_pOM);
		void	RemoveAt(int i) { m_pArrOMs->RemoveAt(i); };
		int		Find(const COrdMaq* p_pOM) const ;
		void	InsertOM(COrdMaq* p_pOM, int pIndice = 0);
		void	RecalcTL();
		void	Recalc(int p_IndFrom = 0);

		CTimeSpan GettsCambio() { return (CTimeSpan) 0L; };

		BOOL	HayInactEn(CAbSpan p_AbSpan);
// Funciones de calendario.

		CTime		AddCal(const CTime p_FecDT, CTimeSpan p_lSecs) const;
		CTime		SubsCal(const CTime p_FecDT, CTimeSpan p_lSecs) const;
		CTimeSpan	DiffCalF(const CTime pB, const CTime pA, CTimeSpan p_tsLim) const;
		CTimeSpan	DiffCal(const CTime pB, const CTime pA, CTimeSpan p_tsLim = 0L) const;
		
		int			GetTurnos(const CTime FecD) const;
		void		SetTurnos(const CTime FecD) const;

};


