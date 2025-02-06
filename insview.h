#if !defined(AFX_INSVIEW_H__DCEF9162_30C7_11D4_899A_95F9041F0A74__INCLUDED_)
#define AFX_INSVIEW_H__DCEF9162_30C7_11D4_899A_95F9041F0A74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InsView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInsView view

class CInsView : public CListViewEx
{
protected:
	CInsView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CInsView)

// Attributes
protected:
	CObArray m_ArrOM;				// OMs que se van a sacar

	CUIntArray m_ArrUWidths;		// Anchuras de los campos opcionales
	CUIntArray m_ArrUFmts;			// Formato de los campos opcionales
	CUIntArray m_ArrUKeys;			// Claves de los campos opcionales
	CStringArray m_ArrsNomCol;		// Nombres de columna de los campos opcionales
	CStringArray m_ArrsDescCol;		// Descripciones de columna de los campos opcionales

public:
	COrdMaq*	m_pSelOM;
	CProgMaq*	m_pSelPM;
	enum EObjSel m_eObjSel;
// Operations
public:
	CString GetsColValue(int p_iCol, COrdMaq* p_pOM);
	void RecalcMaq();
	int LinEdit(int p_iInd);
	CCpwView* GetpCpwView();
	void Recalc();
	CCpwDoc* GetDocument();
	CObject* ObjetoApuntado( CPoint p_Pnt );
	void SetSelec(CObject* p_pObjSel, enum EObjSel p_eObjSel);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CInsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CInsView)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditarOrden();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMoverOrden();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	
    afx_msg void OnAddColumn( UINT nID );
    afx_msg void OnDelColumn( UINT nID );
    
	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in insView.cpp
inline CCpwDoc* CInsView::GetDocument()
   { return (CCpwDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_INSVIEW_H__DCEF9162_30C7_11D4_899A_95F9041F0A74__INCLUDED_)
