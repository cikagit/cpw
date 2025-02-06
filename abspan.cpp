#include "stdafx.h"
#include "cpw.h"  
#include "cpwDefs.h"
#include "AbSpan.h"
#include "cpwExt.h"
#include "Turnos.h"
#include <ole2.h> // OLE2 Definitions

                 
IMPLEMENT_SERIAL(CAbSpan , CObject , 1)                 

void CAbSpan::Serialize(CArchive& ar)
{                            
	CObject::Serialize(ar);
	if ( ar.IsStoring() )
	{
		ar << m_FecIni;
		ar << m_FecFin;
	}
	else
	{
		ar >> m_FecIni;
		ar >> m_FecFin;
	}
}


CAbSpan::CAbSpan()
{
	m_FecIni = m_FecFin =  CTime::GetCurrentTime();
}
      
              
CAbSpan::CAbSpan(const CAbSpan& p_Abs )
{
	*this = p_Abs;
}                                


CAbSpan::CAbSpan( CTime p_FecA, CTime p_FecB )
{
	if (p_FecA < p_FecB)
	{
		m_FecIni = p_FecA;                      
		m_FecFin = p_FecB;
	}
	else
	{
		m_FecIni = p_FecB;
		m_FecFin = p_FecA;
	}
}                        

void
CAbSpan::SetFecIni(CTime p_Fec)
{
	m_FecIni = p_Fec;
	if (m_FecIni > m_FecFin) m_FecFin = m_FecIni;
}

void
CAbSpan::SetFecFin(CTime p_Fec)
{
	m_FecFin = p_Fec;
	if (m_FecIni > m_FecFin) m_FecIni = m_FecFin;
}

CAbSpan::operator CTimeSpan () const
{
	return (m_FecFin - m_FecIni);
}
 
long 
CAbSpan::GetSecs() const
{ 
	return (long) (m_FecFin - m_FecIni).GetTotalSeconds(); 
}

CAbSpan&
CAbSpan::operator =(const CAbSpan& p_Abs)
{                                           
	if (this == &p_Abs) return *this;
	
	m_FecIni = p_Abs.m_FecIni;
	m_FecFin = p_Abs.m_FecFin;

	return *this;
}       
      
BOOL
CAbSpan::operator ==(const CAbSpan& p_Abs) const
{
	return ((m_FecIni == p_Abs.m_FecIni) && (m_FecFin == p_Abs.m_FecFin));
}                                                                         

BOOL
CAbSpan::MismoMinuto(const CAbSpan& p_Abs) const
{
	return (
		(m_FecIni.GetYear() == p_Abs.m_FecIni.GetYear()) &&
		(m_FecIni.GetMonth() == p_Abs.m_FecIni.GetMonth()) &&
		(m_FecIni.GetDay() == p_Abs.m_FecIni.GetDay()) &&
		(m_FecIni.GetHour() == p_Abs.m_FecIni.GetHour()) &&
		(m_FecIni.GetMinute() == p_Abs.m_FecIni.GetMinute()) &&
		(m_FecFin.GetYear() == p_Abs.m_FecFin.GetYear()) &&
		(m_FecFin.GetMonth() == p_Abs.m_FecFin.GetMonth()) &&
		(m_FecFin.GetDay() == p_Abs.m_FecFin.GetDay()) &&
		(m_FecFin.GetHour() == p_Abs.m_FecFin.GetHour()) &&
		(m_FecFin.GetMinute() == p_Abs.m_FecFin.GetMinute()) 
		);
}                                                                         

BOOL
CAbSpan::operator !=(const CAbSpan& p_Abs) const
{
	return !operator==(p_Abs);
}                     
            
BOOL
CAbSpan::Overlaps(const CAbSpan& p_Abs) const
{
	if (p_Abs.m_FecFin <= m_FecIni || p_Abs.m_FecIni >= m_FecFin)
		return FALSE;
	else
		return TRUE;
}
    
bool
CAbSpan::Contains(const CTime p_Fec) const
{
	if (m_FecFin > p_Fec && m_FecIni <= p_Fec)
		return true;
	else
		return false;
}
          
// Operador - : Resta dos AbSpan. Devuelve el número de segundos resultante, 
//				que será el del primer AbSpan si son disjuntos, el del segundo
//				menos el del primero si el segundo está contenido en el primero
//				o el del segundo menos la coincidencia del primero. Si el primero
//				está contenido en el segundo, devolverá un número negativo.
CTimeSpan
CAbSpan:: operator - (const CAbSpan& p_Abs) const
{  
	CTimeSpan Tot(0);
	CTimeSpan Parc;
	
	if (!Overlaps(p_Abs)) return (CTimeSpan(0));
	Parc = p_Abs.m_FecIni - m_FecIni;
	if (Parc.GetTotalSeconds() > 0) Tot += Parc;
	Parc = m_FecFin - p_Abs.m_FecFin;
	if (Parc.GetTotalSeconds() > 0) Tot += Parc;
	return Tot;		

}





IMPLEMENT_DYNAMIC(CFecTur , CObject )


CFecTur::CFecTur()
{
	m_Fec = CTime::GetCurrentTime();
	m_iTurno = 1;
}

CFecTur::CFecTur(const CFecTur& p_FecTur)
{
	*this = p_FecTur;
}

CFecTur::CFecTur(CTime p_Fec)
{
	// Si la hora es antes del inicio del primer turno, realmente es el turno último del día anterior
	m_Fec = CTime(p_Fec.GetYear(), p_Fec.GetMonth(), p_Fec.GetDay(), 0,0,0, 0);
	//CString sDeb = FormatFec(m_Fec, FecHora);
	//CString sDeb2 = FormatFec(CTurnos::GetFecIni(m_Fec, 1), FecHora);
	
	if (p_Fec.GetHour() < CTurnos::GetFecIni(p_Fec, 1).GetHour()) m_Fec = AddDays(m_Fec , -1);
	
//	int iHour = p_Fec.GetHour();

	for (int ii = 1; ii <= CTurnos::GetiNumTurnos(); ii++) {
		CAbSpan span = CTurnos::GetTurno(m_Fec, ii);
		CString sDeb = FormatFec(span.GetFecIni(), FecHora) + " - " + FormatFec(span.GetFecFin(), FecHora);
		CString sDeb2 = FormatFec(p_Fec, FecHora);
		if (span.Contains(p_Fec)) {
			m_iTurno = ii;
		}
	}
	/*
	if (iHour >= TK_TURNO3_FIN && iHour < TK_TURNO1_FIN)
		m_iTurno = TK_TURNO1;
	else if (iHour >= TK_TURNO1_FIN && iHour < TK_TURNO2_FIN)
		m_iTurno = TK_TURNO2;
	else if (iHour >= TK_TURNO2_FIN)
		m_iTurno = TK_TURNO3;
	else
	{
		// Si estamos antes de las 6 de la mañana, 
		// es el turno 3 del día anterior
		m_iTurno = TK_TURNO3;
		m_Fec = AddDays(m_Fec, -1);
	}
	*/
}

CFecTur& 
CFecTur::operator =(const CFecTur& p_FecT)
{
	m_Fec = p_FecT.m_Fec;
	m_iTurno = p_FecT.m_iTurno;

	return *this;
}

BOOL 
CFecTur::operator == (const CFecTur& p_FecT) const
{
	return (IsSameDay(m_Fec, p_FecT.GetFec()) && m_iTurno == p_FecT.GetTurno());
}
	
BOOL 
CFecTur::operator != (const CFecTur& p_FecT) const
{
	return !(*this == p_FecT);
}
	
// class CSet	: public CObject

	IMPLEMENT_DYNAMIC(CSet, CObject);

BOOL 
CSet::In(int p_iVal) const
{
	CUIntArray UArr;
	int iLim = Extract(UArr);
	for (int i = 0; i < iLim; i++)
	{
		if ((int) UArr.GetAt(i) == p_iVal) return TRUE;
	}
	return FALSE;
}
void
CSet::Add(int p_iVal)
{
	CUIntArray UArr;
	int iLim = Extract(UArr);
	if (!In(p_iVal))
	{
		UArr.Add(p_iVal);
		Set(UArr);
	}
	
}
void
CSet::Remove(int p_iVal)
{
	CUIntArray UArr;
	int iLim = Extract(UArr);
	for (int i=0; i < iLim; i++)
	{
		int iNum = UArr.GetAt(i);
		if (iNum == p_iVal)
		{
			UArr.RemoveAt(i);
			i--;
			iLim--;
		}

	}
}
int 
CSet::Extract(CUIntArray& p_UArr) const
{
	int iLim = m_sSet.GetLength();
	p_UArr.RemoveAll();
	CString sNum = "";
	for(int i=0; i < iLim; i++)
	{
		char c = m_sSet[i];
		if (isdigit(c)) sNum += c;
		else 
		{
			int iNum = atoi(sNum);
			if (iNum) p_UArr.Add(iNum);
			sNum = "";
		}	
	}
	return p_UArr.GetSize();
}

int 
CSet::Set(CUIntArray& p_UArr)
{
	int iLim = p_UArr.GetSize();
	m_sSet = "";
	for (int i=0; i < iLim; i++)
	{
		CString sNum = "";
		int iNum = p_UArr.GetAt(i);
		sNum.Format("%d",iNum);
		m_sSet += sNum + ',';
	}
	return iLim;
}

