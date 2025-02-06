
enum eTipoDiv { Interna, Media, Externa };
enum eTipoPrint { Pantalla, Impresora };


class COrdMaq : public CObject
{
	DECLARE_SERIAL(COrdMaq)
	protected:
		CAbSpan				m_Abs;    
		CTimeSpan			m_ts;
		CTimeSpan			m_tsMargen;
		long				m_lRestoMezcla;	// Cantidad de piezas que se pueden hacer con la mezcla que le corresponde
		CTimeSpan			m_tsMezcla;		// Tiempo que le queda con esa mezcla
		CTime				m_FecMezcla;	// Fecha hasta la que puede seguir con la mezcla
		COLORREF			m_Color;
		CProgMaq*			m_pPM;
		CBufArticulo*		m_pBufArt;
		CBufMolde*			m_pBufMolde;
		CSet				m_SetErr;		// Errores que tiene
		long				m_lCant;
		double				m_dCadencia;
		BOOL				m_iLink;		// Indica que el siguiente va ligado a esta

		long				m_lCantFab;		// Cantidad ya fabricada
		CTime				m_FecFab;		// Fecha en la que se ha fabricado
		
		BOOL				m_BSelected;
		BOOL				m_BFija;
		BOOL				m_BTiempoFijo;
		BOOL				m_bConfMolde;
		BOOL				m_bConfMoldeElem;
		COrdMaq*			m_pOMConf;		// OM con la que tiene conflicto de molde
		CObArray			m_arrOMConfMoldeElem;
		CString				m_sIDArticulo;
		CString				m_sIDMolde;
		double				m_dChatarras;	// Chatarras específicas para este bloque
		double				m_dPersonas;	// Personas que trabajan en esa máquina para esta orden
	public:     
		void Delay();
		int GetDiasRetorno();
		BOOL TieneInacts() const;
		int GetArrInacts(CObArray& p_ArrInact) const;
		long				m_lAux;			// Valor 
		double				m_dAux;			// Valor doble auxiliar en cálculos
		double				m_dAuxCant;		// Valor doble auxiliar en cálculos

		CString				m_sAux;			// Valor String auxiliar en cálculos
		CTime				m_FecIniMargen;	// Fecha de inicio desplazada para el cálculo de márgenes
		CTime				m_FecFinMargen;	// Fecha de fin desplazada para el cálculo de márgenes

		void Relink();
		long		GetlCantMezcla() const;
		
		long		GetlNecMezcla(BOOL p_bRoundUp = TRUE, BOOL p_bKilos = TRUE);
		
		void RecalcTL();

		void RecalcFin();
		BOOL Overlaps(COrdMaq* p_pOM);
		int ModifMolde(const char* p_sIDMolde);
		double GetdPeso() const;
		CString GetsMolde() const;
		CString GetsMoldeDesc() const;
		CString GetsCliente() const;
		CString GetsMoldeInySop() const;


		CString GetsMaqNom() const;
		CString GetsMezcla() const;
		CString GetsPlanificador() const;

		CString GetsMezclaKilos() const;
		BOOL GetbConfMolde() const { if (GetlGFH() == TK_GFH_INYECCION) return m_bConfMolde; else return FALSE; };
		COrdMaq* GetpOMConfMolde() const { return m_pOMConf; };
		double GetdCadAbsPs() const;
		long CalcCantLote();
		double GetdChatarras() const;
		long GetlLote();
		void ModifCant(long p_lCant);
		void ModifCantChatarras(long p_lCant);
		void ModifCantTiradas(long p_lCant);
		double GetdPersonas() const { return m_dPersonas;};
		void SetdPersonas(double p_dPersonas) { m_dPersonas = p_dPersonas;};

		CTime				m_FecOrd;		// Fecha generica para ordenar

		COrdMaq&	operator =(const COrdMaq& p_OM);
		BOOL operator ==(const COrdMaq& p_OM) const;
		
		COrdMaq(const CString&);
		COrdMaq(CBufOF* pBufOF = NULL, CBufMolde* p_pMolde = NULL);
		~COrdMaq() {};
 		virtual void Serialize(CArchive& ar);
		int			Recalc();
		void		SetValues(const COrdMaq* p_pOM);

//	Get´s inmediatos
		CTime		GetFecIni() const { return m_Abs.GetFecIni(); }; 		
		CTime		GetFecFin() const { return m_Abs.GetFecFin(); }; 
		CTime		GetFecMezcla() const { return m_FecMezcla; }; 
		CTime		GetFecLim() const;
		CProgMaq* 	GetpPM() const { return m_pPM; };
		CTimeSpan	GetTimeSpan() const { return m_ts; };
		CTimeSpan	GetTsMargen() const { return m_tsMargen; };
		CTimeSpan	GetTsMezcla() const { return m_tsMezcla; };
		long		GetlRestoCantConMezcla() const;
		BOOL		GetBSelected() const { return m_BSelected; };
		BOOL		GetBNextLink() const { return (m_iLink == 1); };
		BOOL		GetBPrevLink() const { return (m_iLink == -1); };
		BOOL		GetBFija() const { return m_BFija; };
		BOOL		GetBTiempoFijo() const { return m_BTiempoFijo; };
		CBufOF*		GetpOF()	const	// same return type as CBufOF::GetID()
					{ if (m_pBufArt) return m_pBufArt->GetpBufOF(); else return NULL; };
		CBufArticulo*		GetpBufArticulo()	const	{ return m_pBufArt; };
		CBufMolde*		GetpBufMolde()	const	{ return m_pBufMolde; };
		CBufMezcla*		GetpBufMezcla()	const	{ if (m_pBufArt) return m_pBufArt->m_pBufMezcla; else return NULL; };
		CBufArtMol*		GetpBufArtMol() const;
		CTime		GetFecFab() const { return m_FecFab; };
		long		GetlCantFab() const { return m_lCantFab; };
        
// Get´s indirectos
		long		GetlCantTot() const;
		long		GetlCantSinChatarras(long p_lCantChatarras) const;
		long		GetlCantTiradas(long p_lCantTiradas) const;

		long		GetlTiradas() const;

		CTimeSpan   GetTotalTs() const;
		long		GetlCantidad() const { return m_lCant; };
		long		GetlOF() const { return 0;};
		CString		GetsID() const;
		CString		GetsNombre() const;
		CString		GetsDescripcion() const;
		double		GetdCadencia(long p_lCentro = -1) const;
		double		GetdCadenciaGFH() const;
		double		GetdCadenciaTL(enum  eTimeLine p_eTL) const;

		long		GetlCavidades() const;
		long		GetlCantReal() const;
		long		CalclCantReal(CTimeSpan p_ts) const;
		double		GetdInactividad() const;
		BOOL		GetBNuevoLlen() const { return FALSE; };
		BOOL		GetBNuevoCel() const { return FALSE; };
		            
		long		GetlGFH() const;
		CString		GetsMaquina() const;

					
		CTime		GetFecEnt() const; 		
		CProg*		GetpProg() const 
					{
						if (m_pPM)
							return m_pPM->GetpProg();
						else
							return NULL;
					};
 // Set´s inmediatos
		void		SetFecIni(CTime p_Fec) { m_Abs.SetFecIni(ZeroSec(p_Fec)); }; 		
		void		SetFecFin(CTime p_Fec) ;
		void		SetBSelected(BOOL p_BSelected) { m_BSelected = p_BSelected; }
		void		SetBNextLink(BOOL p_BNextLink) { m_iLink = (p_BNextLink ? 1 : 0); }
		void		SetBPrevLink(BOOL p_BPrevLink) { m_iLink = (p_BPrevLink ? -1 : 0); }
		void		SetBFija(BOOL p_BFija) { m_BFija = p_BFija; }
		void		SetbConfMolde(BOOL p_bConfMolde, COrdMaq* p_pOM = NULL);
		void		SetbConfMoldeElem(BOOL p_bConfMoldeElem, CObArray& arrOMConfElem);
		void		SetBTiempoFijo(BOOL p_BTiempoFijo) { m_BTiempoFijo = p_BTiempoFijo; }
		void		SetTs(CTimeSpan p_ts) { m_ts = p_ts; };
		void		SetTsMargen(CTimeSpan p_ts) { m_tsMargen = p_ts; };
		void		SetRestoMezcla(long p_lRestoMezcla);
		void		SetdCadencia(double p_dCadencia) { m_dCadencia = p_dCadencia; }  ;
		void		SetdChatarras(double p_dChatarras) { m_dChatarras = p_dChatarras; }  ;
		void		SetpBufArt(CBufArticulo* p_pBufArt) 
			{
				m_pBufArt = p_pBufArt; 
				if (p_pBufArt) 
				{
					m_sIDArticulo = p_pBufArt->m_sID;
				}
				else 
				{
					m_sIDArticulo = "";
				}
			};
		void		SetpBufMolde(CBufMolde* p_pBufMolde) 
			{ 
				m_pBufMolde = p_pBufMolde;
				if (p_pBufMolde) m_sIDMolde = p_pBufMolde->m_sID;
				else m_sIDMolde = "";
			};
		void		SetFab(long p_lCantFab, CTime p_FecFab) 
					{ m_lCantFab = p_lCantFab;
						m_FecFab = p_FecFab;};


// Get´s calculados
		CBrush*		GetpBrush(enum eTipoPrint p_eTipoPrint = Pantalla) const;

		void		SetpPM(CProgMaq* p_pPM, BOOL p_bCheck = FALSE);
		void		SelecTo();
		void		CalcTimeSpan(); 
		void		CalcFecLim();

		void		CalcFecFin();      
		void		CalcTsMargen();
		CTimeSpan	CalcTsMezcla(long lCant);
		long		CalclTiradas(long p_lCantTiradas) const;
		long		CalclCantDeTiradas(long p_lCant) const;


        
		CString		FormatLimit( long p_lGFH ) const; 
		CString		GetsInfo(bool p_bFull = false) const;  
		int			PMIndex() const;
		BOOL		LimitMaquina( const char* p_sMaquina ) const;
		BOOL		LimitGFH( long p_lGFH ) const;
		long		CalclCantTot(long p_lCant) const;
		CObArray*	GetpArrMolElem();
		
//	Funciones de modificación de datos
		void		ModifCant( const char* p_sCant );
		void		ModifCantChatarras( const char* p_sCant );
		void		ModifCantTiradas( const char* p_sTiradas );
		void		ModifPersonas( const char* sPersonas );
		void		ModifFecEnt( CTime p_FecEnt );
		void		ModifTimeSpan( const char* sTimeSpan );
		void		ModifTimeSpanInact( const char* sTimeSpan );
		void		ModifTimeSpanInact( long p_lSecs );
		void		UpdateTs(CTimeSpan p_ts);
		void		SetlCantidad(long p_lCant);
		void		SyncCant();		

		void		UnselectAll();
		void		CalcFecIni();

// Otras funciones
		
	BOOL		First() const;
	BOOL		Last() const;
	COrdMaq*	Prev() ;
	COrdMaq*	Next() ;
};

