#include "stdafx.h"
#include "cpw.h"  
#include "cpwDefs.h"
#include "AbSpan.h"
#include "cpwExt.h"
#include "ObArrOrd.h"

#include "prog.h"
#include "DlgDebug.h"




IMPLEMENT_DYNAMIC( CObArrayOrd , CObArray )                 

void	
CObArrayOrd::Add( CObject* p_pObject )
{           
	int iLim = GetSize();                             
	int i;
	for(i=0; i<iLim ; i++)
	{
		if (!m_pfBiggerFunc(p_pObject, (GetAt(i)))) break;
	}                                               
	InsertAt(i, p_pObject);
}

int
CObArrayOrd::Find( CObject* p_pObject)
{
	int iLim = GetSize();
	if (iLim < 10)
	{
		// Hacemos búsqueda secuencial
		for(int i = 0; i < iLim; i++)
		{
			if (m_pfEqualFunc(p_pObject, (GetAt(i)))) return i;
		}
	}
		else
	{
		return FindOrd(this, p_pObject, m_pfEqualFunc, m_pfBiggerFunc);
	}	
	return -1;
	
}

int
FindOrd(CObArray* p_pArr, CObject* p_pO,
		BOOL (*p_pfEqualFunc) (CObject* p_pO1, CObject* p_pO2),
		BOOL (*p_pfBiggerFunc) (CObject* p_pO1, CObject* p_pO2))
{
/***
*FindOrd - do a binary search on an array of Objects
*
*Purpose:
*       Does a binary search of a sorted array for a key.
*
*Entry:
*       p_pArr    - puntero a un array de objetos
*       p_pO      - objeto a buscar
*       BOOL (*p_pfEqualFunc) () - Función que compara dos elementos del array, y
*									devuelve TRUE si son iguales y FALSE si no
*       BOOL (*p_pfBiggerFunc) () - Función que compara dos elementos del array, y
*									devuelve TRUE si p_pO1 > p_pO2  y FALSE si no
*               
*               
*
*Exit:
*       if key is found:
*               returns index to one occurrence of key in array
*       if key is not found:
*               returns negative index to first biggest element (not implemented)
*
	*******************************************************************************/
	// Hacemos búsqueda binaria
	int iLim = p_pArr->GetSize();
	int iIni = 0;
	int iFin = iLim - 1;
	int iHalf;
	int i;
	while(iIni <= iFin)
	{
		if (iHalf = iLim / 2) 
		{
			i = iIni + ( iLim & 1 ? iHalf : (iHalf - 1) );
			if (p_pfEqualFunc(p_pO, (p_pArr->GetAt(i)))) return i; // Si son iguales
			else if (p_pfBiggerFunc(p_pArr->GetAt(i), p_pO))		// Si es menor
			{
				iFin = i - 1;
				iLim = (iLim & 1) ? iHalf : iHalf - 1;
			}
			else
			{
				iIni = i + 1;
				iLim = iHalf;
			}
		}
		else if (iLim)
			return (p_pfEqualFunc(p_pO, (p_pArr->GetAt(iIni))) ? iIni : -1);
		else
			break;
	}
	return -1;
}



