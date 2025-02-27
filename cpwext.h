#include <strstream>

extern CCache* g_pCache; 
extern CBrush	g_BrushParada;
extern CBrush	g_BrushArr[];
extern CBrush	g_BrushArrImp[];
extern CString	g_sError;
extern CString	g_sEmpresa;
extern BOOL		g_bException;
extern BOOL		g_bHorizontal;
extern BOOL		g_BMoviendo;
extern enum EObjSel g_eObjSel;	
//extern CLogFile logFile;



// Tipos de selección de órdenes que se va a mostrar
enum eOrdenOF { Articulo, Seccion , Cliente };

struct SMargenColor
{
	COLORREF	m_Color1;
	int			m_iMin2;
	COLORREF	m_Color2;
	int			m_iMin3;
	int			m_iMax3;
	COLORREF	m_Color3;
	int			m_iMin4;
	int			m_iMax4;
	COLORREF	m_Color4;
	int			m_iMax5;
	COLORREF	m_Color5;
	COLORREF	m_Color6;

};
extern struct SMargenColor g_MC;
struct SVerOF
{

	BOOL		m_bAcumOFs;
	BOOL		m_bSelecFec;
	CTime		m_FecSelec;
	BOOL		m_bSoloNoProg;
	BOOL		m_bSoloNoFab;
	enum eOrdenOF	m_OrdenOF;

	BOOL		Selec() { return m_bAcumOFs || m_bSelecFec || m_bSoloNoProg || m_bSoloNoFab; };
	
};

extern struct SVerOF g_VerOF;

// Tipos de formatos de fecha, para la función FormatFec
enum eFormatFec { FechaDB, Fecha , Hora , FecHora , HoraSec , FecHoraSec , FecHoraInvert};
// Tipos de formatos de long, para la función FormatLong
enum eFLTipo { Long, Real1Dec , Real2Dec, NoPoint};
// Tipos de posibles objetos a ser seleccionados en una vista
enum EObjSel { Nada, Orden, Maquina, EditCal };
// Tipos de posibles TimeLine
enum eTimeLine { Piezas, Mezcla , Margen };


COLORREF SelecColorRef(COLORREF p_crRaw);

CTime ZeroSec(CTime p_Fec);
CString GetsLinea(const CString& sFile, BOOL bNext = TRUE, const char cSeparator = '\n');
CString STRID( UINT nid );
void ErrStruct(UINT nid, const CString& sErr1, const CString sErr2, BOOL m_BRecarga );


CString FormatFec( CTime p_Fec , enum eFormatFec p_eFF = FecHora);
CString FormatFec( CTimeSpan p_Fec , enum eFormatFec p_eFF = FecHora);

void FormatLong(long p_lCant, CString& p_sCant, int p_iMaxLen , enum eFLTipo p_eFLTipo= Long, BOOL p_BZeroBlank = FALSE);
CString FormatLong(long p_lCant, int p_iMaxLen, enum eFLTipo p_eFLTipo = Long, BOOL p_BZeroBlank = FALSE);
CString FormatDbl(double p_d, int p_ant, int p_des);

BOOL IsSameDay( CTime p_A, CTime p_B);
BOOL IsLaterDay( CTime p_FecA, CTime p_FecB);
CTime AddDays( CTime p_rFecA , int p_iNumDays = 1);
CTimeSpan StringToTs(const CString& p_sTimeSpan);

CString GetsFec(const CTime& p_FecIni, const CTime& p_FecAnt, BOOL p_BPrimera);
CString FecToBCWConvert(CTime p_Fec);

inline CTime ConvDateToCTime(COleDateTime DT)
{
	return CTime(DT.GetYear(), DT.GetMonth(), DT.GetDay(), 
				  DT.GetHour(), DT.GetMinute(), DT.GetSecond() );

}
CTime FecBCWConvert(const char* sItem,  const char* sTime = NULL);

CString GetNomLlen(const COrdMaq* p_pOM);

int GetDefaultTurnos(CTime p_Fec, BOOL p_pBTurnos[] = NULL);

CPoint StringToPoint(const char* p_sPoint);
void Triangle( CDC* pDC, CPoint p_Point1, CPoint p_Point2, CPoint p_Point3);
CString GetsDlgText(UINT p_uID, CDialog* p_pDial);
void LisAccess(const CString& p_sRegVal, const CString& p_sMacVal);



enum eTipoOrden { Creciente, Decreciente };
void AddOMOrd( CObArray& p_OMArr, COrdMaq* p_pOM, enum eTipoOrden p_eTipoOrden = Creciente );
void AddPMOrd( CObArray& p_OMArr, CProgMaq* p_pPM, enum eTipoOrden p_eTipoOrden = Creciente );


void SetsConnPedidos(const CString& p_sConnPedidos = "", bool p_bActual = false);
CString GetsConnPedidos();
bool GetBPedidosActuales();
bool IsDefaultConnPedidos(const CString& p_sConnPedidos );

int GetNumEmpresa(const CString& p_sEmpresa);
CString GetStrNumEmpresa(const CString& p_sEmpresa);

void ClearArray(CObArray& p_Arr, CRuntimeClass* p_pRunClass);
bool FileExists(const CString& p_sNomFichero);

void StringArrayPutElement(CObArray& p_Arr, int p_row, int p_col, CString& p_sVal);

int FindInObArray(CObArray& p_PMArr, CObject* p_pObj);

