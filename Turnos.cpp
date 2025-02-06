#include "stdafx.h"

#include "cpw.h"
#include "cpwDefs.h"
#include "AbSpan.h"
#include "cpwdb.h"
#include "cpwExt.h"

#include "Turnos.h"

//CTurnos, clase singleton para contener el número de turnos y las horas
                 
IMPLEMENT_SERIAL(CTurnos , CObject , 1)                 


void CTurnos::Serialize(CArchive& ar)
{                            
	CObject::Serialize(ar);
	if ( ar.IsStoring() )
	{
		ar << m_pArrTurnos->GetSize();

	}
	else
	{
		int iNumTurnos;
		ar >> iNumTurnos;
	}
	m_pArrTurnos->Serialize(ar);   
}

void 
CTurnos::LoadTurnos()
{
	CTurnos& Turnos = GetInstance();

	if (!Turnos.m_pArrTurnos) {
		Turnos.m_pArrTurnos = new CObArray();
	}

	ClearArray(*(Turnos.m_pArrTurnos), RUNTIME_CLASS( CAbSpan ));
	CTime fecTurno(0);
	CTime fecTurnoDiaSig = AddDays(fecTurno, 1);
	Turnos.m_pArrTurnos->Add( new CAbSpan(CTime(fecTurno.GetYear(),fecTurno.GetMonth(), fecTurno.GetDay(), 6,0,0), CTime(fecTurno.GetYear(),fecTurno.GetMonth(), fecTurno.GetDay(), 18,0,0) ));
	Turnos.m_pArrTurnos->Add( new CAbSpan(CTime(fecTurno.GetYear(),fecTurno.GetMonth(), fecTurno.GetDay(), 18,0,0),CTime(fecTurnoDiaSig.GetYear(),fecTurnoDiaSig.GetMonth(), fecTurnoDiaSig.GetDay(), 6,0,0) ));
	
}

CAbSpan 
CTurnos::GetTurno(CTime p_Fec, int p_iNumTurno)
{
	
	return CAbSpan(GetFecIni(p_Fec, p_iNumTurno), GetFecFin(p_Fec, p_iNumTurno));
}


int 
CTurnos::GetiNumTurnos()
{
	CTurnos& Turnos = GetInstance();

	if (Turnos.m_pArrTurnos) {
		return Turnos.m_pArrTurnos->GetSize();
	} else {
		return GetDefaultNumTurnos();
	}

}

//Obtenemos la fecha de inicio del turno. Si no hay turnos creados, usamos valores por defecto
CTime 
CTurnos::GetFecIni(CTime p_Fec, int p_iNumTurno)
{
	CTime timeTurno = CTime(0);
	CTurnos& Turnos = GetInstance();
	
	if (Turnos.m_pArrTurnos) {
		CAbSpan* span = (CAbSpan*) Turnos.m_pArrTurnos->GetAt(p_iNumTurno-1);
		timeTurno = span->GetFecIni();
	} else {
		timeTurno = GetDefaultTimeIni(p_iNumTurno);
	}
	CString sDeb = FormatFec(timeTurno, FecHora);
	return CTime(p_Fec.GetYear(), p_Fec.GetMonth(), p_Fec.GetDay(), timeTurno.GetHour(), timeTurno.GetMinute(), timeTurno.GetSecond());
}

//Obtenemos la fecha de fin del turno. Si no hay turnos creados, usamos valores por defecto.
CTime 
CTurnos::GetFecFin(CTime p_Fec, int p_iNumTurno)
{
	CTime timeTurno = CTime(0);
	CTurnos& Turnos = GetInstance();
	
	if (Turnos.m_pArrTurnos) {
		CAbSpan* span = (CAbSpan*) Turnos.m_pArrTurnos->GetAt(p_iNumTurno-1);
		timeTurno = span->GetFecFin();
	} else {
		timeTurno = GetDefaultTimeFin(p_iNumTurno);
	}
	int iMaxTurno = GetiNumTurnos();
	if (p_iNumTurno == iMaxTurno) p_Fec = AddDays(p_Fec, 1); // Si es el último turno, la fecha de fin es realmente en el día siguiente
	return CTime(p_Fec.GetYear(), p_Fec.GetMonth(), p_Fec.GetDay(), timeTurno.GetHour(), timeTurno.GetMinute(), timeTurno.GetSecond());
}

long
CTurnos::GetSecsTurno(int iNumTurno)
{

	CTimeSpan tsTurno = GetFecFin(CTime(0), iNumTurno) - GetFecIni(CTime(0), iNumTurno);
	//CString sDeb = FormatFec(GetFecFin(CTime(0), iNumTurno), FecHora);
	//CString sDeb2 = FormatFec(GetFecIni(CTime(0), iNumTurno), FecHora);

	return (long) tsTurno.GetTotalSeconds();
}

// Valores por defecto, tres turnos de 8 horas, de 6 a 14 a 22
int
CTurnos::GetDefaultNumTurnos()
{
	return 3;
}

CTime 
CTurnos::GetDefaultTimeIni(int iNumTurno)
{
	CTime fecIni(0);
	if (iNumTurno == 1) {
		fecIni = CTime(fecIni.GetYear(), fecIni.GetMonth(), fecIni.GetDay(), 6,0,0);
	} else if (iNumTurno == 2) {
		fecIni = CTime(fecIni.GetYear(), fecIni.GetMonth(), fecIni.GetDay(), 14,0,0);
	} else if (iNumTurno == 3) {
		fecIni = CTime(fecIni.GetYear(), fecIni.GetMonth(), fecIni.GetDay(), 22,0,0);
	}
	return fecIni;
}

CTime 
CTurnos::GetDefaultTimeFin(int iNumTurno)
{
	CTime fecFin(0);
	if (iNumTurno == 1) {
		fecFin = CTime(fecFin.GetYear(), fecFin.GetMonth(), fecFin.GetDay(), 14,0,0);
	} else if (iNumTurno == 2) {
		fecFin = CTime(fecFin.GetYear(), fecFin.GetMonth(), fecFin.GetDay(), 22,0,0);
	} else if (iNumTurno == 3) {
		fecFin = AddDays(fecFin, 1);
		fecFin = CTime(fecFin.GetYear(), fecFin.GetMonth(), fecFin.GetDay(), 6,0,0);
	} 
	return fecFin;
}