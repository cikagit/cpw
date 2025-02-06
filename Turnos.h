
// Contiene el número de turnos que se utilizan en una 

class CTurnos : public CObject
{                  
protected:
	DECLARE_SERIAL(CTurnos)

	CObArray*	m_pArrTurnos;
	
public:                    

	static CTurnos& GetInstance() {
		static CTurnos m_pInstance;

		return m_pInstance;
	}

// constructores
	CTurnos() { };
	//CTurnos(CTurnos const&);
	void operator=(CTurnos const&);

	~CTurnos() { if (m_pArrTurnos) ClearArray(*m_pArrTurnos, RUNTIME_CLASS( CAbSpan )); delete m_pArrTurnos;};
// Gets
	static CAbSpan GetTurno(CTime p_Fec, int p_iNumTurno);
	static CTime GetFecIni(CTime p_Fec, int p_iNumTurno);
	static CTime GetFecFin(CTime p_Fec, int p_iNumTurno);
	static long	GetSecsTurno(int iNumTurno);
	static int GetiNumTurnos();
	static void LoadTurnos();

protected:
	static int GetDefaultNumTurnos();
	static CTime GetDefaultTimeIni(int iNumTurno);
	static CTime GetDefaultTimeFin(int iNumTurno);

	void Serialize( CArchive& ar);
	
};

