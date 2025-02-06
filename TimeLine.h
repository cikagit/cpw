



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLinSeg		:		Production Segment in Line -> A Time Segment with a constant production rate or comsuption rate
												// All the machines are considered, and the final rate saved


class CLinSeg : public CObject
{
public:
	CAbSpan		m_AbSpan;		// Espacio de tiempo cubierto por la cadencia homogénea
	double		m_dCadencia;		// Cadencia acumulada de todas las órdenes que se simultanean
	
	CLinSeg();
	CLinSeg( CLinSeg& p_LS);
	CLinSeg& operator =(const CLinSeg& p_LS);
	long GetlCantFab();
	double GetdCantFab();
	
protected:
	DECLARE_DYNAMIC(CLinSeg)   
	
};






////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CProdSeg		:		Production Segment -> A Production Time in a Machine, that can overlap with others


class CProdSeg : public CObject
{
public:
	CAbSpan		m_AbSpan;		// Espacio de tiempo de producción, no puede contener inactividades
	COrdMaq*	m_pOM;			// Puntero a una OM

	CProdSeg(COrdMaq* p_OM = NULL);
	CProdSeg( CProdSeg& p_PS);
	CProdSeg& operator =(const CProdSeg& p_PS);
	
protected:
	DECLARE_DYNAMIC(CProdSeg)   
	
};



/////////////////////////////////////////////////////////////////////////////
// TimeLine

class CTimeLine : public CObject
{
protected:
	DECLARE_SERIAL(CTimeLine)


// Attributes
public:
	CString			m_sID;			// Identificador del recurso
	CObArray*		m_pArrOM;		// Array de las OMs afectadas
	CObArrayOrd*	m_pArrPS;		// Array de ProdSeg's ordenadas por FecIni
	CObArray*		m_pArrLS;		// Array de LinSeg's sin solapes,
	long			m_lResto;		// Resto que queda tras repartir una cantidad
								//  combinadas en base a las de ArrPS en un momento determinado.

// Operations
public:
	CTimeLine();
	BOOL HaySolapes();
	void GetArrPSsAfec( CLinSeg* p_pLinSeg, CObArray* p_pArrOMs);
	void GetArrOMs( CObArray* p_pArrOMs);
	long GetlResto() { return m_lResto; };
	long ReparteCant(long p_lCant, enum  eTimeLine p_eTL);

	void AplicarDistancias();
	void AplicarCalendarioArrPSs();
	void Recalc(enum  eTimeLine p_eTL);


	int Desolapar(enum  eTimeLine p_eTL);


	virtual void Serialize(CArchive& ar) {};   
	
// Implementation
public:
	void Empty(BOOL p_BOrdMaqs = TRUE);
	virtual ~CTimeLine();

};

inline BOOL TL_Bigger( CObject* p_pA, CObject* p_pB ) { return ((CTimeLine*)p_pA)->m_sID > ((CTimeLine*)p_pB)->m_sID; }
inline BOOL TL_Equal( CObject* p_pA, CObject* p_pB ) { return ((CTimeLine*)p_pA)->m_sID == ((CTimeLine*)p_pB)->m_sID; }

inline BOOL PS_Bigger( CObject* p_pA, CObject* p_pB ) { return ((CProdSeg*)p_pA)->m_AbSpan.GetFecIni() > ((CProdSeg*)p_pB)->m_AbSpan.GetFecIni(); }
inline BOOL PS_Equal( CObject* p_pA, CObject* p_pB ) { return ((CProdSeg*)p_pA)->m_AbSpan.GetFecIni() == ((CProdSeg*)p_pB)->m_AbSpan.GetFecIni(); }
