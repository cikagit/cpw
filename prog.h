class CCpwDoc;  
class CProgMaq;   
class CBufOF;
class CObArrayOrd;
class COMComb;
class CTimeLine;
class CBufArticulo;
class CBufMezcla;
	
enum EDirMove { Left, Right };


class CProg : public CObject
{
	DECLARE_SERIAL(CProg)
	protected:
		CAbSpan			m_Abs;
		CCpwDoc*		m_pDoc;
		long			m_lTimeScale;
		CObArray*		m_pArrPMs;
		long			m_lNumDias;
		CObArrayOrd*	m_pArrTLPiezas;
		CObArrayOrd*	m_pArrTLMargen;
		CObArrayOrd*	m_pArrTLMezcla;


public:
		int GetiNumOM();

		
		void AplicarCalendarioArrPSs(CObArrayOrd* p_pArrOM);
		void AplicarDistancias(CObArrayOrd* p_pArrPS);

		void DescuentoConSolapes(long& p_rlCant, CTimeLine* p_pTL, CObArray* p_pArrNoCambio, CTime p_Fec);
		void InformarErrores(CObArray* p_pArrNoCambio, CObArray* p_pArrNoFab, CStringArray* p_pArrNotFound, CObArray* p_pArrBorrar);
		void RelinkAll();
		//long CalcCantAFecha(const CString& p_sIDArticulo, CTime p_Fec);
		CTime	CalcFecParaCantTL(CTimeLine* p_pTL, long p_lCant);


		void DescuentaVal(CTime p_Fec, int p_iLocal);
		void AplicarCambiosMoldes(CTime p_Fec,  CObArray* p_pArrNoCambios, CObArray* p_pArrBorrar, CObArray* p_pArrNoCambio, CStringArray* p_pArrNotFound );
		int GetNumOM();
		void GetArrPM(CObArray& p_PMArr, long p_lGFH = -1);
		int GetArrOMC(CObArrayOrd& p_pArr);
		int GetArrMaqOM(CObArray& p_pArrOM);
		int GetArrMaqOMLis(CObArray& p_ArrOM);
		int GetArrMezExt(CObArray& p_ArrMezExt, const CString& p_sDesdeMaq, const CString& p_sHastaMaq);

		int DesolaparArrOMC(CObArray* p_pArrOM, CObArray* p_pArrOMComb);
		void AplicarCalendarioOMC(CObArray* p_pArrOM, CObArray* p_pArrOMComb);


		long GetlCantArt(const CString& m_pIDArt);
		COrdMaq* FindOMArt(const CString& p_sArticulo);

		
		CProg();
		~CProg();
		CProg& operator =(const CProg& p_Prog);
		void AddPM( CProgMaq* p_pPM, int p_iInd = -1);
		void DeleteOM (COrdMaq* p_pOM);
		void DeletePM( int p_iPM );
		BOOL MovePM( CProgMaq* p_pPM, EDirMove p_eDirMove);
		void SwitchPM( int p_iInd, int p_iInd2 );

		void ClearPMs();
 		void ClearOMs();
		void CalcConflictosMoldes();
		void CalcConflictosMoldesElem();
		void CheckConfOM(COrdMaq* p_pOM, int p_iPM = 0);
		void CheckConfOMElem(COrdMaq* p_pOM, int p_iPM = 0);
		bool CheckConfInArraysElem(CObArray& arrOrig, CObArray& arrCheck, int indMaq);
		bool CheckArrElem(CObArray& arrOrig);
		
		void UpdateArrTL();

		int CalcArrTL(enum  eTimeLine p_eTL = Piezas);

		void	CalcMezclasTL(const char* p_sIDMezcla = NULL);


 		virtual void Serialize(CArchive& ar);
 		
		CTime		GetFecIni() const { return m_Abs.GetFecIni(); }; 		
		CTime		GetFecFin() const { return m_Abs.GetFecFin(); }; 		
		CCpwDoc*	GetpDoc() const { return m_pDoc; };
		long	GetTimeScale() const { return m_lTimeScale; };        

		void	SetTimeScale(int p_lTimeScale)	
		{ if (p_lTimeScale > 0) m_lTimeScale = p_lTimeScale; };        
		void	SetpDoc(CCpwDoc* p_pDoc) { m_pDoc = p_pDoc; }; 
		void	RecalcInacts();
		void	Recalc();
		void	SetFecIni(CTime p_Fec) ;
		void	SetFecFin(CTime p_Fec) { if (p_Fec > GetFecIni()) m_Abs.SetFecFin(p_Fec); RecalcInacts();};
		int GetNumPM() const ;
		CProgMaq* GetpPM(int p_NumMaq) const;

		int Find(const CProgMaq* p_pPM) const ;
		CProgMaq* FindMaq(const char* p_ID) const;
		CProgMaq* FindMaqByDesc(const char* p_sDesc) const;
		
		int	GetArrOM( CObArray* p_pArrOM, CBufOF* p_pOF );
		int	GetArrOMArt( CObArray* p_pArrOM, const CString p_sIDArt );
		
		void MoveOrdMaq(CProgMaq* p_pPM, COrdMaq* p_pOM, int pInd);

		COrdMaq*	GetPrevOM(const COrdMaq* p_pOM, long p_lGFH = -1) const;
		
		void UnselectAll();

		void CargaGFH(long p_lGFH);
		void ReprogramaGFH(long p_lGFH);
		
		void BuscaOptimizar( CObArray& p_ArrOMs, int p_iInd );
		void InsertaEnGFH(CObArray& p_ArrOMs, long p_lGFH);
		int BuscarHuecosGFH(long p_lGFH, COrdMaq* p_pOM, CObArray& p_ArrPMs) const;
		BOOL VerifStruct();
		BOOL InsertNewOM(COrdMaq* p_pOM, BOOL& p_rbModif );
		CTimeLine* GetpTL(const CString& p_sID, enum  eTimeLine p_eTL = Piezas);
		CTimeLine* GetpTL(CBufArticulo* p_pBufArt, enum  eTimeLine p_eTL);
//		CTimeLine* GetpTL(CBufMezcla* p_pBufMez);
		
		int GetArrMaquinas(CStringArray& p_ArrMaquinas);
		int GetArrGFHs(CStringArray& p_ArrGFHs);
		




};

