             
class CTime;
class ostream;
class istream;             
          
class CObArrayOrd : public CObArray
{                  
protected:
	BOOL (*m_pfBiggerFunc) (CObject* p_Arr1, CObject* p_Arr2);
					// puntero a una función 
					// que devuelve BOOL y tiene dos parámetros que son otros CObject
					// Esta función nos servirá para ordenar. Debe devolver TRUE 
					// cuando p_Arr1 sea mayor que p_Arr2
	BOOL (*m_pfEqualFunc) (CObject* p_Arr1, CObject* p_Arr2);
	DECLARE_DYNAMIC(CObArrayOrd)
public:                    
// constructores
	CObArrayOrd(BOOL (*p_pfBiggerFunc) (CObject* p_Arr1, CObject* p_Arr2),
				BOOL (*p_pfEqualFunc) (CObject* p_Arr1, CObject* p_Arr2)) 
		{ m_pfBiggerFunc = p_pfBiggerFunc; m_pfEqualFunc = p_pfEqualFunc; };
// implementacion
	void	Add( CObject* p_pNewObject );
	int		Find( CObject * p_pObject );	
};

int FindOrd(CObArray* p_pArr, CObject* p_pO,
		BOOL (*p_pfEqualFunc) (CObject* p_pO1, CObject* p_pO2),
		BOOL (*p_pfBiggerFunc) (CObject* p_pO1, CObject* p_pO2));
