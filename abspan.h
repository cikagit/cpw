             
class CTime;
class ostream;
class istream;             
          
class CAbSpan : public CObject
{                  
protected:
	DECLARE_SERIAL(CAbSpan)
	CTime m_FecIni;
	CTime m_FecFin;
public:                    
// constructores
	CAbSpan() ;
	CAbSpan( CTime p_FecIni, CTime p_FecFin ) ;
	CAbSpan(const CAbSpan& p_Abs );
// Gets
	CTime GetFecIni() const { return m_FecIni; };
	CTime GetFecFin() const { return m_FecFin; };
	void SetFecIni(CTime p_Fec);
	void SetFecFin(CTime p_Fec);
	long GetSecs() const;
// operadores
	inline operator CTimeSpan () const;
	CAbSpan& operator =(const CAbSpan& p_Abs);
	BOOL operator ==(const CAbSpan& p_Abs) const;
	BOOL operator !=(const CAbSpan& p_Abs) const;
	BOOL Overlaps(const CAbSpan& p_Abs) const;
	bool Contains(const CTime p_Fec) const;
	CTimeSpan operator -(const CAbSpan& p_Abs) const;
	BOOL MismoMinuto(const CAbSpan& p_Abs) const;

	void Serialize( CArchive& ar);
	
};

class CFecTur	: public CObject
{
// Miembros
protected:
	DECLARE_DYNAMIC(CFecTur);
	CTime		m_Fec;
	int			m_iTurno;
public:
//Constructores
	CFecTur();
	CFecTur(const CFecTur& p_FecTur);
	CFecTur(CTime p_Fec);
// Gets
	CTime GetFec() const { return m_Fec; }
	int GetTurno() const { return m_iTurno; }
// Operadores
	CFecTur& operator =(const CFecTur& p_FecT);
	BOOL operator == (const CFecTur& p_FecT) const ;
	BOOL operator != (const CFecTur& p_FecT) const ;
	
};

class CSet	: public CObject
{
// Miembros
protected:
	DECLARE_DYNAMIC(CSet);
	CString		m_sSet;
public:
//Constructores
	CSet(const CString& p_sIni = "") { m_sSet = p_sIni; }

// Gets
	BOOL In(int p_iVal) const;
	int GetSize() { CUIntArray UArr; return Extract(UArr); };
	CString GetAsString() { return m_sSet; };
// Operadores
	CSet& operator =(const CSet& p_Set) { m_sSet = p_Set.m_sSet; return *this; };

// Funciones
	void Add(int p_iVal);
	void Remove(int p_iVal);
	void RemoveAll() { m_sSet = ""; };

protected:
	int Extract(CUIntArray& p_UArr) const;
	int Set(CUIntArray& p_UArr);
};

