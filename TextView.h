#if !defined(AFX_GRAVIEW_H__4D79B33C_F216_11D3_8999_CE7C89B3320D__INCLUDED_)
#define AFX_GRAVIEW_H__4D79B33C_F216_11D3_8999_CE7C89B3320D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextView view
class CTextView : public CView
{
protected:
	CTextView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTextView)

	enum eCambio { Primero, Cambiado, Igual };
	enum eUpdate { Otro, Scroll, Doc };
	
	CSize	m_Size;
	int		m_FontHeight;  
	int		m_FontWidth;   
	enum	eCambio	m_FontChanged;
	enum	eUpdate m_Update;
	int		m_ColWidth; 
	int		m_SepWidth; 
	int		m_HeadHeight;

	int		m_iIni;						// OF primera a sacar
	int		m_iPage;					// Tamaño de página
	CPoint	m_Point;					// Selected point (for internal processing)
	
	COrdMaq*	m_pSelOM;
	CProgMaq*	m_pSelPM;
	
	CFont*	m_pFont;
	CFont*	m_pOldFont;
	CBrush	m_Brush;

	SPrintInfo	m_PI;
	BOOL	m_bCancelar;

	CObArray		m_ArrOFs;			// Array de OFs
	CStringArray*	m_pArrCompArt;		// Array de IDs de Artículos comprimidos. Debe ser persistente


// Attributes
public:
	CCpwDoc* GetDocument();

// Operations
public:
	void OnAbrirOFs();
	void OnCerrarOFs();
	void SwitchComprimido(CBufOF* p_pBufOF);
	void DoScroll(UINT p_nChar, UINT p_nRepCnt, UINT p_nFlags);
	void SeteOrden();
	void CopyOFsFromCache();
	BOOL LocateOF(const CString& p_sOF);
	CString m_sSearch;
	CUIntArray m_ArrUPos;
	int		GetNumOF();
	CBufOF* GetpBufOFAt(CPoint p_Point);
	CBufOF* GetpBufOFAt(int p_iIndex);
	void	GetArrsIDArt(CStringArray& p_ArrsIDArt);

	void SelecToFec(CTime p_Fec);
	void UnselectAllBufOF();
	void SelectToBufOF(CBufOF *p_pBufOF);
	int GetiOFSec(int p_iOFAct, BOOL p_bNext = TRUE );
	int GetiOFArt(int p_iOFAct, BOOL p_bNext = TRUE );


	int GetFontHeight() { return m_FontHeight; };
	void DoCargaInicial();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextView)
	public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTextView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCargainicial();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTipoLetraTxt();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditarOrden();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TextView.cpp
inline CCpwDoc* CTextView::GetDocument()
   { return (CCpwDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAVIEW_H__4D79B33C_F216_11D3_8999_CE7C89B3320D__INCLUDED_)
