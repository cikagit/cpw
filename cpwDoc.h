// cpwDoc.h : interface of the CCpwDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPWDOC_H__2F09B38D_A6B1_11D1_BF6A_0080AD1B0F43__INCLUDED_)
#define AFX_CPWDOC_H__2F09B38D_A6B1_11D1_BF6A_0080AD1B0F43__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTextView;

class CCpwDoc : public CDocument
{
protected: // create from serialization only
	CCpwDoc();
	DECLARE_DYNCREATE(CCpwDoc)

	enum eUpdate { Otro, Recalcular };

	CProg	m_Prog;
	BOOL	m_BReadOnly;
	enum	eUpdate m_Update;
//	BOOL		m_BMoviendo;		// lo eliminamos para que el mover sea global
	COrdMaq*	m_pSelOM;
	CProgMaq*	m_pSelPM;
	COrdMaq*	m_pIniMovOM;
	enum EObjSel m_eObjSel;	

// Attributes
public:

// Operations
protected:


	void DescargarPrograma();

public:
	BOOL	GetBMoviendo() { return g_BMoviendo; };
	void	SetBMoviendo(BOOL p_BMoviendo) {g_BMoviendo = p_BMoviendo;};
	COrdMaq*	GetpSelOM() { /*return m_pSelOM;*/ return g_pOM; };
	void	SelectOM(COrdMaq* p_pSelOM) {m_pSelOM = p_pSelOM; g_pOM = p_pSelOM;};
	CProgMaq*	GetpSelPM() { /*return m_pSelPM;*/ return g_pPM; };
	void	SelectPM(CProgMaq* p_pSelPM) {m_pSelPM = p_pSelPM; g_pPM = p_pSelPM;};
	void	UnselectOM() {m_pSelOM = NULL; g_pOM = NULL;};
	void	UnselectPM() {m_pSelPM = NULL; g_pPM = NULL;};
	COrdMaq*	GetpIniMovOM() { /*return m_pIniMovOM;*/ return g_pIniMovOM; };
	void	SetIniMovOM(COrdMaq* p_pIniMovOM) {m_pIniMovOM = p_pIniMovOM; g_pIniMovOM = p_pIniMovOM; };
	enum EObjSel GeteObjSel() { /*return m_eObjSel;*/ return g_eObjSel; };
	void SeteObjSel(enum EObjSel p_eObjSel) { /*m_eObjSel = p_eObjSel;*/ g_eObjSel = p_eObjSel; };
	
	CProg*	GetpProg() { return &m_Prog; };
	BOOL	GetBReadOnly() { return m_BReadOnly; };
	void CargarPrograma(const char* p_sPathName);
	virtual	void CargaCelulas(long p_lBloque, long p_lSerieIni, long p_lSerieFin);
	CTextView* GetpTextView();
	void	UpdateTextView();
	void SetSelec(CObject* p_pObjSel, enum EObjSel p_eObjSel);
	



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCpwDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCpwDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
public:
	void MoverOrden();
	//{{AFX_MSG(CCpwDoc)
	afx_msg void OnEscala();
	afx_msg void OnRecalcular();
	afx_msg void OnCambiarFechaInicio();
	afx_msg void OnConsMaqord();
	afx_msg void OnConsCammol();
	afx_msg void OnConsLotes();
	afx_msg void OnOfOrden();
	afx_msg void OnSelecFecha();
	afx_msg void OnNextArt();
	afx_msg void OnNextPage();
	afx_msg void OnPrevArt();
	afx_msg void OnPrevPage();
	afx_msg void OnPrevSeccion();
	afx_msg void OnNextSeccion();
	afx_msg void OnSolonoprog();
	afx_msg void OnOfHastafecha();
	afx_msg void OnCambiarFechaFin();
	afx_msg void OnOrdenarMaquinas();
	afx_msg void OnConsOfs();
	afx_msg void OnDescontarCika();
	afx_msg void OnDescontarOtros();
	afx_msg void OnVerAcumfecha();
	afx_msg void OnEscalaAmplia();
	afx_msg void OnEscalaEstrecha();
	afx_msg void OnConsMezExt();
	afx_msg void OnCliOrden();
	afx_msg void OnSolonofab();
	afx_msg void OnSacarAscii();
	afx_msg void OnArchivoGuardarconpedidos();
	afx_msg void OnArchivoAbrirconpedidos();
	afx_msg void OnExcel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPWDOC_H__2F09B38D_A6B1_11D1_BF6A_0080AD1B0F43__INCLUDED_)
