#include "stdafx.h"
#include "cpw.h"
#include "cpwDefs.h"

#include "AbSpan.h"
#include "cpwExt.h"
#include "prog.h"
#include "cpwDoc.h"
#include "cpwDefs.h"
#include "cpwView.h"
#include "cpwDb.h"
#include "cache.h"            
#include "progMaq.h"
#include "OrdMaq.h"
#include "Turnos.h"
#include <fstream>


// Devuelve la misma fecha:Hora con 0 en los segundos
CTime
ZeroSec(CTime p_Fec)
{
	return CTime ( p_Fec.GetYear(),	p_Fec.GetMonth(), p_Fec.GetDay(),
					p_Fec.GetHour(), p_Fec.GetMinute(), 0 );

}

COLORREF
SelecColorRef(COLORREF p_crRaw)
{
	// Dado un COLORREF, nos devuelve uno similar pero más oscuro. Si Es muy oscuro,
	// lo pasa a claro.
	const int iShade = 50;
	int iBlue = p_crRaw & 0XFF;
	int iGreen = (p_crRaw >> 8) & 0xFF;
	int iRed = (p_crRaw >> 16) & 0xFF;

	if (iBlue < iShade) iBlue /= (iShade/10);
	else iBlue -= iShade;
	if (iGreen < iShade) iGreen /= (iShade/10);
	else iGreen -= iShade;
	if (iRed < iShade) iRed /= (iShade/10);
	else iRed -= iShade;
	

	return 256*256*iRed+256*iGreen+iBlue;
}

CTimeSpan
StringToTs(const CString& p_sTimeSpan)
{
	CString sHoras = "";
	CString sMin = "";
	int iHoras = 0;
	int iMin = 0;
	BOOL bMin = FALSE;
	// Convierte una cadena con formato "12:20" a un TimeSpan representando la misma cantidad
	int iLim = p_sTimeSpan.GetLength();
	for (int i = 0; i < iLim; i++)
	{
		char c = p_sTimeSpan[i];
		if (c == ':')
		{
			bMin = TRUE;
			continue;
		}
		if (!bMin)
		{
			sHoras += c;
		} else
		{
			sMin += c;
		}
	}
	iHoras = atoi(sHoras);
	iMin = atoi(sMin);
	CTimeSpan tsRes =  CTimeSpan(0, iHoras, iMin, 0);
	return tsRes;
}
// Toma una CString y nos devuelve una linea de la misma. bNext indica si hay que empezar
// desde el principio (FALSE).
CString
GetsLinea(const CString& sFile, BOOL bNext, const char cSeparator)
{
	static int i;
	if (!bNext) i = 0;
	CString sRetVal;
	sRetVal = "";
	// Buscamos el LF, no CR-LF
	int iLim = sFile.GetLength();
	// Partimos del valor anterior de i;
	for ( ;  i < iLim; i++)
	{
		sRetVal += sFile[i];
		if (sFile[i] == cSeparator)
		{
			i++;
			return sRetVal;
		}
	}
	return sRetVal;
}

CString
GetsFec(const CTime& p_FecIni, const CTime& p_FecAnt, BOOL p_BPrimera)
{
// Convierte una fecha (p_FecIni) a una cadena, comparándola con una fecha anterior
// para no sacar los datos que coincidan, y quede más bonito en el listado.
// La fecha sale en formato AA/MM/DD HH:MM
	CString sFec;
	CString sTemp;
	if (p_FecIni.GetYear() == p_FecAnt.GetYear() && !p_BPrimera)
		sFec.Format("   ");
	else
	{
		int iYear = p_FecIni.GetYear();
		int iSiglo = (iYear / 100) * 100;
		sTemp.Format("%2.2d/", iYear % iSiglo );
		sFec += sTemp;
	}
	if (p_FecIni.GetMonth() == p_FecAnt.GetMonth() && !p_BPrimera)
		sFec += "   ";
	else
	{
		sTemp.Format("%2.2d/",p_FecIni.GetMonth());
		sFec += sTemp;
	}
	if (p_FecIni.GetDay() == p_FecAnt.GetDay() && !p_BPrimera)
		sFec += "  ";
	else
	{
		sTemp.Format("%2.2d",p_FecIni.GetDay());
		sFec += sTemp;
	}
	sTemp.Format(" %2.2d:%2.2d",  p_FecIni.GetHour(),	p_FecIni.GetMinute() );
	sFec += sTemp;
	return sFec;
}                                           

BOOL
IsSameDay( CTime p_FecA, CTime p_FecB)
{
	return ( p_FecA.GetDay() == p_FecB.GetDay() &&
			 p_FecA.GetMonth() == p_FecB.GetMonth() &&
			 p_FecA.GetYear() == p_FecB.GetYear() );
}

BOOL
IsLaterDay( CTime p_FecA, CTime p_FecB)
{
	if (p_FecA.GetYear() > p_FecB.GetYear() ) return TRUE;
	if (p_FecA.GetYear() < p_FecB.GetYear() ) return FALSE;
	if (p_FecA.GetMonth() > p_FecB.GetMonth() ) return TRUE;
	if (p_FecA.GetMonth() < p_FecB.GetMonth() ) return FALSE;
	return ( p_FecA.GetDay() > p_FecB.GetDay() );
}

CTime
AddDays( CTime p_rFecA , int p_iNumDays)
{
	// Hacemos un ciclo para evitar overflows
	
	BOOL BNeg = (p_iNumDays < 0) ? TRUE : FALSE;
	int iNumDays = (BNeg) ? ( -p_iNumDays ) : p_iNumDays;
	
	for (int i = 0; i < iNumDays; i++)
	{
		if (BNeg)
			p_rFecA -= (CTimeSpan) TK_SEGS_DIA ;
		else
			p_rFecA += (CTimeSpan) TK_SEGS_DIA;
	}

	return p_rFecA;
}


CString
STRID( UINT nid )
{
	CString s = _T("");
	if (! s.LoadString( nid ))
	{
		CString sMsg;
		sMsg.Format("Error loading string. StringID = %d", nid );
		g_bException = TRUE;
		AfxMessageBox(sMsg);
		g_bException = FALSE;
		return "";
	} else
	{
		return s;
	}
}

void
ErrStruct(UINT nid, const CString& sErr1, const CString sErr2, BOOL p_BRecarga)
{
	if (p_BRecarga) return;
	if (g_iContErr > TK_LIM_ERRSTRUCT) return;
	g_iContErr = TK_LIM_ERRSTRUCT + 1;

	CString sLisCont = AfxGetApp()->GetProfileString("Listados", "Control", "P");
	AfxGetApp()->WriteProfileString("Listados", "Control", sLisCont);

	if (!sLisCont.CompareNoCase("S") || 
		( !sLisCont.CompareNoCase("P") && (AfxMessageBox("Sacar Listados de Control", MB_YESNO)  == IDYES) ))
	{
		LisAccess("Control Inicial", "LisFallosBD");
	}

/*
	if (sErr1 == "" && sErr2 == "") return;
	g_iContErr++;
	if (g_iContErr > TK_LIM_ERRSTRUCT) return;
	CString sMsg;
	sMsg.Format(STRID(nid), sErr1, sErr2);
	g_bException = TRUE;
	AfxMessageBox(sMsg);
	if (g_iContErr == TK_LIM_ERRSTRUCT) AfxMessageBox("No se sacarán más mensajes de error.");
	g_bException = FALSE;
*/
}

CString 
FormatFec( CTime p_Fec, enum eFormatFec p_FF)
{                
	int iYear = p_Fec.GetYear();
	int iSiglo = (iYear / 100) * 100;
	CString sTemp, sAdd;
	if (p_FF == FechaDB)
	{
		sTemp.Format("%d/%d/%d", p_Fec.GetMonth(), p_Fec.GetDay(), iYear % iSiglo );
	}
	if (p_FF == Fecha || p_FF == FecHora || p_FF == FecHoraSec)
	{
		sTemp.Format("%2.2d/%2.2d/%2.2d", p_Fec.GetDay(), p_Fec.GetMonth(), iYear % iSiglo );
	}
	if ( p_FF == FecHoraInvert)
	{
		sTemp.Format("%2.2d/%2.2d/%2.2d", iYear % iSiglo , p_Fec.GetMonth(), p_Fec.GetDay());
	}
	if ( p_FF == FecHora || p_FF == FecHoraSec || p_FF == FecHoraInvert)
        sTemp += " ";
	if (p_FF == Hora || p_FF == FecHora || p_FF == FecHoraSec || p_FF == FecHoraInvert)
	{
		sAdd.Format("%d:%2.2d", p_Fec.GetHour(), p_Fec.GetMinute());
		sTemp += sAdd;
	}
	if ( p_FF == HoraSec || p_FF == FecHoraSec)
	{
		sAdd.Format(":%2.2d", p_Fec.GetSecond());
		sTemp += sAdd;
	}            
	return sTemp;
} 

CString
FormatFec( CTimeSpan p_ts, enum eFormatFec p_FF)
{                         
	CString sTemp, sAdd;
	sTemp.Format("%d:%d", p_ts.GetTotalHours(), p_ts.GetMinutes());
	if ( p_FF == HoraSec || p_FF == FecHoraSec)
	{
		sAdd.Format(":%d", p_ts.GetSeconds());
		sTemp += sAdd;
	}
	return sTemp;
}

CString
FormatLong(long p_lCant, int p_iMaxLen, enum eFLTipo p_eFLTipo, BOOL p_BZeroBlank)
{
	CString sTemp;
	FormatLong(p_lCant, sTemp, p_iMaxLen, p_eFLTipo, p_BZeroBlank);
	return sTemp;
}

void
FormatLong(long p_lCant, CString& p_sCant, int p_iMaxLen, enum eFLTipo p_eFLTipo, BOOL p_BZeroBlank)
{
	if (p_lCant == 0 )
	{
		if (p_BZeroBlank)
			p_sCant = "";
		else
			p_sCant = "0";
	
		return;
	}
	// Esta funcion convierte un entero largo en una cadena con puntos de separacion
	// de millares. Tiene un máximo de longitud.
	BOOL bNeg = (p_lCant < 0);
	if (bNeg) p_lCant = labs(p_lCant);
	const int iSep = 4;
	// Primero nos hacemos con el buffer de la String, porque vamos a modificarlo a pelo
	
	char*	pChar = p_sCant.GetBuffer(p_iMaxLen);
	int iFact = 10;
	int iDigit = 0;
	int iPosicion = 0;
	pChar[p_iMaxLen] = '\0';
	// Si es Real, hacemos los decimales primero.
	int iCompPos = 0;
	if (p_eFLTipo != Long && p_eFLTipo != NoPoint)
	{
		if (p_eFLTipo == Real2Dec)
		{
			iDigit = (int) (p_lCant % iFact);
			pChar[p_iMaxLen - 1 - iPosicion] = '0' + iDigit;
			iPosicion++;
			iCompPos++;
			p_lCant /= iFact;
		}
		if (iPosicion <= p_iMaxLen)
		{
			iDigit = (int) (p_lCant % iFact);
			pChar[p_iMaxLen - 1 - iPosicion] = '0' + iDigit;
			iPosicion++;
			iCompPos++;
			p_lCant /= iFact;
		}
		if (iPosicion <= p_iMaxLen)
		{
			pChar[p_iMaxLen - 1 - iPosicion] = ',';
			iPosicion++;			
			iCompPos++;
		
		}
	}
	
	if (iPosicion <= p_iMaxLen)
	{
		do
		{
			// Empezamos de atras hacia delante calculando digitos
			iDigit = (int) (p_lCant % iFact);
			pChar[p_iMaxLen - 1 - iPosicion] = '0' + iDigit;
			iPosicion++;
			if (iPosicion > p_iMaxLen) break;
			// Determinamos si es una posicion para poner un punto
			if (((iPosicion+1-iCompPos) % iSep) == 0 && p_eFLTipo != NoPoint)
			{
				pChar[p_iMaxLen - 1 - iPosicion] = '.';
				iPosicion++;			
				if (iPosicion > p_iMaxLen) break;
			}
			p_lCant /= iFact;
		}  while ( p_lCant);
	}
	// Comprobamos no vaya a ser que nos haya quedado un punto al final 
	if (((iPosicion+1-iCompPos) % iSep) == 1 && iPosicion > 1 && p_eFLTipo != NoPoint) 
	{
		pChar[p_iMaxLen - iPosicion] = ' ';
		iPosicion--;
	}

	// Eliminamos los 0´s del final y la coma si sólo quedan 0´s
	if (p_eFLTipo != Long && p_eFLTipo != NoPoint)
	{
		BOOL bSignif = FALSE;
		int i;
		for (i = 0; i< (p_eFLTipo == Real2Dec ? 2 : 1); i++)
		{
			int j = p_iMaxLen -1 - i;
			if (!bSignif && pChar[j] == '0') pChar[j] = ' ';
			else bSignif = TRUE;
		}
		if (!bSignif) pChar[p_iMaxLen -1 - i] = ' ';
	}

	// Si es negativo metemos el menos
	if (bNeg)
	{
		pChar[p_iMaxLen - 1 - iPosicion] = '-';
		iPosicion++;
	}
	// Y rellenamos con blancos lo que queda
	for ( ; iPosicion < p_iMaxLen; iPosicion++)
		pChar[p_iMaxLen - 1 - iPosicion] = ' ';
	// Soltamos el buffer de la CString
	p_sCant.ReleaseBuffer(p_iMaxLen);
	
}



int
GetDefaultTurnos(CTime p_Fec, BOOL p_pBTurnos[]) 
{
	// Obtenemos los turnos por defecto para una fecha
	// Por ahora se trabaja a todas horas

	
	int iNumTur = CTurnos::GetiNumTurnos();	//Sacamos los turnos de esta planta
	if (!p_pBTurnos) return iNumTur;

	if (iNumTur == 2 && p_Fec.GetDayOfWeek() == 1 ) { // Si es Domingo en la de 2 turnos, no se trabaja.
		for (int ii = 0; ii < iNumTur; ii++) {
			p_pBTurnos[ii] = FALSE;
		}
		iNumTur = 0;
	} else { // Si estamos en 3 turnos o en 2 y no es domingo, se trabajan todos los turnos
		for (int ii = 0; ii < iNumTur; ii++) {
			p_pBTurnos[ii] = TRUE;
		}
	}

	return iNumTur;
}

CTime
FecBCWConvert(const char* sDate, const char* sTime)
{
	CTime FecResult;
	// La constante es consecuencia del modo de almacenamiento de BCW
	DATE Date = atof(sDate) - (float) TK_CONV_DATE;
	COleDateTime ODTDate(Date);
	FecResult = ConvDateToCTime(ODTDate);
	if (sTime) FecResult += (CTimeSpan) atol(sTime);
	
	return FecResult;
}

CString
FecToBCWConvert(CTime p_Fec)
{
	CString sResult;
	// La constante es consecuencia del modo de almacenamiento de BCW

	COleDateTime ODTDate(	p_Fec.GetYear(), 
							p_Fec.GetMonth(),
							p_Fec.GetDay(),
							0,0,0 );
	DATE Date = DATE(ODTDate);
	long lDate = (long) Date + TK_CONV_DATE;
	CTimeSpan tsTemp(0, p_Fec.GetHour(), p_Fec.GetMinute(), p_Fec.GetSecond());
	long lNumSecs = (long) tsTemp.GetTotalSeconds();
	sResult.Format("%ld %ld ", lDate, lNumSecs);
	
	return sResult;
}


CPoint
StringToPoint(const char* p_sPoint)
{
	CString sPoint = p_sPoint;
	CString sX;
	CString sY;
	int i;
	for ( i = 1; i < sPoint.GetLength() && sPoint[i] != ','; i++)
	{
		sX += sPoint[i];
	}
	if (i < sPoint.GetLength())
	{
		i++;

		for ( ; i < sPoint.GetLength() && sPoint[i] != ')'; i++)
		{
			sY += sPoint[i];
		}
	}
	return CPoint( atoi(sX), atoi(sY) );
}

void 
AddOMOrd( CObArray& p_OMArr, COrdMaq* p_pOM, enum eTipoOrden p_eTipoOrden)
{
	for (int i = 0; i < p_OMArr.GetSize(); i++)
	{
		COrdMaq* pOM = (COrdMaq*) p_OMArr.GetAt(i);
		ASSERT( pOM->IsKindOf( RUNTIME_CLASS( COrdMaq ) ) );
		if (p_eTipoOrden == Creciente)
		{
			if (p_pOM->m_FecOrd  < pOM->m_FecOrd )
			{
				p_OMArr.InsertAt(i, p_pOM);
				return;
			}
		} else
		{
			if (p_pOM->m_FecOrd  > pOM->m_FecOrd )
			{
				p_OMArr.InsertAt(i, p_pOM);
				return;
			}
		}
		
	}
	// Si no, se inserta al final
	p_OMArr.Add(p_pOM);
}

void 
AddPMOrd( CObArray& p_PMArr, CProgMaq* p_pPM, enum eTipoOrden p_eTipoOrden)
{
	for (int i = 0; i < p_PMArr.GetSize(); i++)
	{
		CProgMaq* pPM = (CProgMaq*) p_PMArr.GetAt(i);
		ASSERT( pPM->IsKindOf( RUNTIME_CLASS( CProgMaq ) ) );
		if (p_eTipoOrden == Creciente)
		{
			if (p_pPM->m_FecOrd  < pPM->m_FecOrd )
			{
				p_PMArr.InsertAt(i, p_pPM);
				return;
			}
		} else
		{
			if (p_pPM->m_FecOrd  > pPM->m_FecOrd )
			{
				p_PMArr.InsertAt(i, p_pPM);
				return;
			}
		}
		
	}
	// Si no, se inserta al final
	p_PMArr.Add(p_pPM);
}

int
FindInObArray(CObArray& p_PMArr, CObject* p_pObj)
{
	for (int ii = 0; ii < p_PMArr.GetSize(); ii++)
	{
		CObject* pObj = p_PMArr.GetAt(ii);
		if (p_pObj == pObj) return ii;
	}
	return -1;
}

void
Triangle( CDC* pDC, CPoint p_Point1, CPoint p_Point2, CPoint p_Point3)
{
	CPoint pArr[3];
	pArr[0] = p_Point1;
	pArr[1] = p_Point2;
	pArr[2] = p_Point3;
	CRgn RgnTriangle;
	RgnTriangle.CreatePolygonRgn(pArr, 3, WINDING);
	CBrush Brush;
	Brush.CreateSolidBrush(RGB(110,110,110));
	pDC->FillRgn(&RgnTriangle, &Brush);
}

CString
GetsDlgText(UINT p_uID, CDialog* p_pDial)
{
	const int BufLen = 1024;

	char Buffer[BufLen];
	p_pDial->GetDlgItemText(p_uID, Buffer, BufLen);
	return (CString) Buffer;
}

// Ejecuta la BD de listados Access con una macro, cuyo nombre se le da
void
LisAccess(const CString& p_sRegVal, const CString& p_sMacVal)
{
	STARTUPINFO startup;
	PROCESS_INFORMATION process;
	
	memset( &startup, 0, sizeof( startup ) );
	startup.cb = sizeof( startup );
	
	memset( &process, 0, sizeof( process ) );
	
	CString sAcc = AfxGetApp()->GetProfileString("Listados", "Access", "");
	//AfxGetApp()->WriteProfileString("Listados", "Access", sAcc);
	CString sMacro = AfxGetApp()->GetProfileString("Listados", p_sRegVal, p_sMacVal);
	if (sMacro == "") sMacro = p_sMacVal;
	//AfxGetApp()->WriteProfileString("Listados", p_sRegVal, sMacro);

	CString sComm = sAcc + " " + sMacro;
	if (sComm == "") return;

	char* pszCommandLine = sComm.GetBuffer( 0 );
	BOOL tSuccess = CreateProcess( NULL, pszCommandLine, NULL, NULL, FALSE, 0, NULL,   NULL, &startup, &process );
	if( tSuccess )
	{
		// Close the handles that CreateProcess returned so we don't leak
		// kernel resources.
		ASSERT( process.hProcess != NULL );
		CloseHandle( process.hProcess );
		ASSERT( process.hThread != NULL );
		CloseHandle( process.hThread );
	}
	else
	{
		AfxMessageBox( "Error al ejecutar proceso" );
	}
}

CString
FormatDbl(double p_d, int p_ant, int p_des)
{
	CString s;
	CString sFormato;
	sFormato.Format("%%%d.%df", p_ant+p_des+1, p_des);
	s.Format(sFormato, p_d);
	s.Replace('.','=');
	s.Replace(',','.');
	s.Replace('=',',');
	return s;
}

static CString s_sConnPedidosDef = "";
static CString s_sConnPedidos = "";
void 
SetsConnPedidos(const CString& p_sConnPedidos, bool p_bActual)
{
	s_sConnPedidos = p_sConnPedidos;
	if (p_bActual) s_sConnPedidosDef = p_sConnPedidos;
}

CString GetsConnPedidos()
{
	return s_sConnPedidos;
}

bool GetBIsDefaultConnPedidos()
{
	return (s_sConnPedidosDef == s_sConnPedidos);
}

bool 
IsDefaultConnPedidos(const CString& p_sConnPedidos )
{
	return (s_sConnPedidosDef == p_sConnPedidos);
}

// Devuelve el número de la empresa de una cadena con el formato ">XXXX_99_XXXXXXXX"
// donde 99 será el número de la empresa, y XXXXXXXXXXXX el nombre, como en 01_Berriatua
// Devuelve 0 si la estructura de la cadena es incorrecta
int
GetNumEmpresa(const CString& p_sEmpresa)
{
	int iSubs = p_sEmpresa.Find('_');
	if (iSubs < 0) return 0;
	CString sResto =  p_sEmpresa.Mid(iSubs+1);
	iSubs = sResto.Find('_');
	if (iSubs < 0) return 0;
	CString sNum = sResto.Left(iSubs);
	return atoi(sNum);
}

// Devuelve el número de la empresa de una cadena con el formato "99_XXXXXXXX"
// donde 99 será el número de la empresa, y XXXXXXXXXXXX el nombre, como en 01_Berriatua
// Devuelve 0 si la estructura de la cadena es incorrecta
CString
GetStrNumEmpresa(const CString& p_sEmpresa)
{
	int iSubs = p_sEmpresa.Find('_');
	if (iSubs < 0) return "";
	CString sNum = p_sEmpresa.Left(iSubs);
	return sNum;
}

void
ClearArray(CObArray& p_Arr, CRuntimeClass* p_pRunClass)
{
	for (int i = 0; i < p_Arr.GetSize(); i++)
	{
		CObject* p_Ob = p_Arr.GetAt(i);
		if (p_Ob->IsKindOf(p_pRunClass)) {
			delete p_Ob;
		}
	}
	p_Arr.RemoveAll();
}

bool FileExists(const CString& p_sNomFichero)
{
	int ii = GetFileAttributes(p_sNomFichero);
	return ( ii >= 0 );
}

void
StringArrayPutElement(CObArray& p_Arr, int p_row, int p_col, CString& p_sVal)
{
	//Creamos las líneas si no existen
	if (p_row >= p_Arr.GetSize()) {
		int iOldSize = p_Arr.GetSize();
		p_Arr.SetSize(p_row+1);
		for (int ii = iOldSize; ii < p_Arr.GetSize(); ii++) {
			CStringArray* pNewStringArr = new CStringArray();
			pNewStringArr->SetSize(9, 1);
			p_Arr.SetAt(ii, pNewStringArr);
		}
	}
	//Añadimos en valor en la fila y columna indicadas
	CStringArray* pStrArr = (CStringArray*) p_Arr.GetAt(p_row);
	pStrArr->SetAt(p_col, p_sVal);
}