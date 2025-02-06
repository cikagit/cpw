// cpwView.h : interface of the CCpwView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPWVIEW_H__2F09B38F_A6B1_11D1_BF6A_0080AD1B0F43__INCLUDED_)
#define AFX_CPWVIEW_H__2F09B38F_A6B1_11D1_BF6A_0080AD1B0F43__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class COrdMaq;
class CProgMaq;

struct SPrintInfo
{
	int m_iTipoListado;			// Tipo de listado solicitado
	CString m_sDesdeGFH;		// GFH Desde el que se lista
	CString m_sHastaGFH;		// GFH Hasta el que se lista
	CString m_sDesdeMaq;		// Maquina desde la que se lista
	CString m_sHastaMaq;		// Maquina hasta la que se lista
	CAbSpan m_Abs;				// Fechas del intervalo que se lista
	
	// Informaciones calculadas
	int m_iDaysPerPage;			// Número de días que caben por página
	int m_iLinsPerPage;			// Lines per page
	int m_iNumCols;				// Number of columns to print
	int m_iHres;				// Number of pixels x-axis
	int m_iVres;				// Number of raster lines y-axis
	int m_iPixCol;				// Number of pixels by column
	CFont* m_pFontNormal;		// Proportional Font
	CFont* m_pFontNormalBold;	// Proportional Font Bold
	CFont* m_pFontMaq;			// Font para sacar los códigos de las máquinas
	CFont* m_pFont90;			// Font with fixed number of characters
	CFont* m_pFont90b;			// Font bold with fixed number of characters
	int m_iWidthNormal;			// Width of the FontNormal
	int m_iWidthMaq;			// Width of the FontMaq
	int m_iWidth90;				// Width of the Font90
	int m_iLinHeight;			// Height of the line
	int m_iFontHeight;			// Height of the proportional font
	long m_lConvFactor;			// Relation between screen and printing resolution

	int m_iCabHeight;			// Height of the Header
	int m_iTitleHeight;			// Height of the Title of the Report
	int m_iFootHeight;			// Heigth of the Footer

	CTime m_FecPrev;			// Fecha previa para sacar diferencias
	int m_iNumOM;				// Número de OM que estamos listando
	int m_iIniOM;				// Número de OM inicial en esta página
	int m_iFinOM;				// Número de OM final en esta página
	int m_iNumPM;				// Número de PM que estamos listando
	CProgMaq*	m_pPM;			// Maquina que estamos listando
	CObArray	m_ArrOMs;		// Array de OMs a listar


	SPrintInfo() 
	{
		m_pFontNormal = m_pFontNormalBold = m_pFont90 = m_pFont90b = m_pFontMaq = (CFont*) NULL;	
	};
	~SPrintInfo() 
	{ 
		if (m_pFontNormal) delete m_pFontNormal;
		if (m_pFontMaq) delete m_pFontMaq;
		if (m_pFontNormalBold) delete m_pFontNormalBold;
		if (m_pFont90) delete m_pFont90;
		if (m_pFont90b) delete m_pFont90b;
	};

};

class CCpwView : public CScrollView
{
protected: // create from serialization only

	enum eCambio { Primero, Cambiado, Igual };
	enum eUpdate { Otro, Scroll, Doc };

	CCpwView();
	DECLARE_DYNCREATE(CCpwView)
	CSize	m_Size;
	int		m_FontHeight;  
	int		m_FontWidth;   
	enum	eCambio	m_FontChanged;
	enum	eUpdate m_Update;
	int		m_ColWidth; 
	int		m_SepWidth; 
	int		m_HeadHeight;
	
	
	CFont*	m_pFont;
	CFont*	m_pOldFont;
	CBrush	m_Brush;

	SPrintInfo	m_PI;

// Attributes
public:
	COrdMaq*	m_pSelOM;
	CProgMaq*	m_pSelPM;
// Operations
public:
	CCpwDoc* GetDocument();

	CSize	GetSize() { return m_Size; };
	CRect 	GetRect(COrdMaq* p_pOM);
	CRect 	GetRectPrint(COrdMaq* p_pOM, int p_iCol, CTime p_FecIni);
	CObject* ObjetoApuntado( CPoint p_Pnt );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCpwView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

public:
	// Printing functions
	virtual void BeginPrintingGrafico(CDC* pDC, CPrintInfo* pInfo);
	virtual void PrintGrafico(CDC* pDC, CPrintInfo* pInfo);
	
	virtual void EndPrintingGrafico(CDC* pDC, CPrintInfo* pInfo);
	
	virtual void BeginPrintingProgMaq(CDC* pDC, CPrintInfo* pInfo);
	virtual void EndPrintingProgMaq(CDC* pDC, CPrintInfo* pInfo);
	virtual void PrintProgMaq(CDC* pDC, CPrintInfo* pInfo);	

	virtual BOOL GetLinProgMaq(CString& p_sLin, BOOL p_BPrimera, BOOL& pr_BNuevo);
	
	virtual void BeginPrintingMoldes(CDC* pDC, CPrintInfo* pInfo);
	virtual void EndPrintingMoldes(CDC* pDC, CPrintInfo* pInfo);
	virtual void PrintMoldes(CDC* pDC, CPrintInfo* pInfo);
	void SetSelec(CObject* p_pObjSel, enum EObjSel p_eObjSel);
	virtual BOOL GetLinMoldes(CString& p_sLin, BOOL p_BPrimera, BOOL& pr_BNuevo);
	bool EsSelectedMaq(CProgMaq* p_pPM);
	int GetNumPMsInGFHs() ;
		
	
protected:
	virtual void OnPrepareDC( CDC* pDC, CPrintInfo* pInfo = NULL );
	virtual void OnEditarCalend();
	virtual void OnConsArtMaq();
	virtual void OnRecalcMaq();
	virtual void OnBorrarMaq();
	virtual void OnInsertMaq();
	virtual void OnLeftMaq();
	virtual void OnRightMaq();
	virtual void OnSelectEnd();
	virtual void OnListarGraf();
	virtual void OnCargaGFH();
	virtual void OnReprogGFH();


// Implementation
public:
	void OnNuevaOrden();
	void OnCambiarPersonas();

	virtual ~CCpwView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	int m_nAccumulatedDelta;

	void OnCalendGFH();
	void OnDividirOrden();
	void OnUnirOrden();


	//{{AFX_MSG(CCpwView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditarOrden();
	afx_msg void OnBuscarOrden();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMoverOrden();
	afx_msg void OnTipoLetra();
	afx_msg void OnTest();
	afx_msg void OnCargaAntiguo();
	afx_msg void OnListarMaq();
	afx_msg void OnVerifStruct();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditPaste();
	afx_msg void OnCargainicial();
	afx_msg void OnOrdenRecalcular();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // debug version in cpwView.cpp
inline CCpwDoc* CCpwView::GetDocument()
   { return (CCpwDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPWVIEW_H__2F09B38F_A6B1_11D1_BF6A_0080AD1B0F43__INCLUDED_)
