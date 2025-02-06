#if !defined(AFX_DIALOGS_H__6E39F1C3_AF4A_11D1_BF7E_0080AD1B0F43__INCLUDED_)
#define AFX_DIALOGS_H__6E39F1C3_AF4A_11D1_BF7E_0080AD1B0F43__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Dialogs.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// DlgEscala dialog
class DlgEscala : public CDialog
{
// Construction
public:
	DlgEscala(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgEscala)
	enum { IDD = IDD_ESCALA };
	CEdit	m_EditEscala;
	CSpinButtonCtrl	m_SpinEscala;
	int		m_Escala;
	//}}AFX_DATA
	UDACCEL m_AccelArr;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgEscala)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	BOOL OnInitDialog();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgEscala)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// DlgCargaInicial dialog

class DlgCargaInicial : public CDialog
{
// Construction
public:
	DlgCargaInicial(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgCargaInicial)
	enum { IDD = IDD_CARGAINICIAL };
	CEdit	m_EditHastaOrden;
	CEdit	m_EditDesdeOrden;
	CComboBox	m_ComboBloque;
	int		m_Bloque;
	int		m_DesdeOrden;
	int		m_HastaOrden;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgCargaInicial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgCargaInicial)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeBloque();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDlgValidarCambio dialog

class CDlgValidarCambio : public CDialog
{
// Construction
public:
	CDlgValidarCambio(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgValidarCambio)
	enum { IDD = IDD_VALIDAR_CAMBIO };
	CString	m_sCampo;
	CString	m_sValAnt;
	CString	m_sValNue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgValidarCambio)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgValidarCambio)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CIntrLong dialog

class CIntrLong : public CDialog
{
// Construction
public:
	CIntrLong(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIntrLong)
	enum { IDD = IDD_INTR_LONG };
	CEdit	m_EditLong;
	CEdit	m_EditLongValue;
	long	m_lValue;
	CString	m_sText;
	//}}AFX_DATA

	CString m_sCaption;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIntrLong)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIntrLong)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CIntrDbl dialog

class CIntrDbl : public CDialog
{
// Construction
public:
	CIntrDbl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIntrDbl)
	enum { IDD = IDD_INTR_DBL };
	CEdit	m_EditDblValue;
	CString	m_sText;
	double	m_dValue;
	//}}AFX_DATA

	CString m_sCaption;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIntrDbl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIntrDbl)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDlgPrint dialog
class CProg;
class CDlgPrint : public CDialog
{
// Construction
public:
	CDlgPrint(CProg* p_pProg, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPrint)
	enum { IDD = IDD_PRINT };
	CDateTimeCtrl	m_DTctrlHastaFecha;
	CDateTimeCtrl	m_DTctrlDesdeFecha;
	CComboBox	m_ComboTipoListado;
	CComboBox	m_ComboHastaMaq;
	CComboBox	m_ComboHastaGFH;
	CComboBox	m_ComboDesdeMaq;
	CComboBox	m_ComboDesdeGFH;
	CTime	m_FecDesde;
	CString	m_sDesdeGFH;
	CString	m_sDesdeMaq;
	CTime	m_FecHasta;
	CString	m_sHastaGFH;
	CString	m_sHastaMaq;
	CString	m_sTipoListado;
	//}}AFX_DATA
	
	int	m_iTipoListado;
	CProg* m_pProg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPrint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPrint)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeTipoListado();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDlgBuscarOrden dialog

class CDlgBuscarOrden : public CDialog
{
// Construction
public:
	CDlgBuscarOrden(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBuscarOrden)
	enum { IDD = IDD_BUSCAR_ORDEN };
	CComboBox	m_ComboOrden;
	CString	m_sOrden;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBuscarOrden)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBuscarOrden)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDlgIntrDate dialog

class CDlgIntrDate : public CDialog
{
// Construction
public:
	CDlgIntrDate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgIntrDate)
	enum { IDD = IDD_INTR_DATE };
	CDateTimeCtrl	m_DTctrlFecha;
	CString	m_sCaption;
	CTime	m_Fec;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgIntrDate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgIntrDate)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDlgInsMaq dialog

class CDlgInsMaq : public CDialog
{
// Construction
public:
	CDlgInsMaq(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInsMaq)
	enum { IDD = IDD_INSERT_MAQ };
	CButton	m_ButtonAntes;
	CComboBox	m_ComboInsMaq;
	CString	m_sInsMaq;
	int		m_iAntes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInsMaq)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInsMaq)
	afx_msg void OnEditchangeInsmaqMaq();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDlgMaqOrd dialog
enum eTipoConsulta { ConsMaqOrd, ConsCambioMolde , ConsLotes, ConsOF, ConsMezExt };
class CCpwDoc;

class CDlgMaqOrd : public CDialog
{
// Construction
public:
	CCpwDoc* m_pDoc;
	CObArray m_ArrMezExt;
	CObArray m_ArrOM;
	CObArray m_ArrOF;
	
	CDlgMaqOrd(CCpwDoc* p_pDoc, CWnd* pParent = NULL);   // standard constructor

	void UpdateList();
	
	void IniLotes();
	void IniCambioMolde();
	void AddItemMoldes(COrdMaq* pOM, COrdMaq* pOMNext, int& iLin, char* sMaq);
	bool EsPrimeraConHueco(COrdMaq* pOM, COrdMaq* pOMPrev);

	void IniMaqOrd();
	void IniOF();
	void IniMezExt();
	bool ChequeaListGeneral();

// Dialog Data
	//{{AFX_DATA(CDlgMaqOrd)
	enum { IDD = IDD_LST_MAQ };
	CComboBox	m_ComboHastaMaq;
	CComboBox	m_ComboDesdeMaq;
	CDateTimeCtrl	m_DTctrlFecHasta;
	CDateTimeCtrl	m_DTctrlDesde;
	CListCtrl	m_ListCtrl;
	CTime	m_FecDesde;
	CTime	m_FecHasta;
	CString	m_sDesdeMaq;
	CString	m_sHastaMaq;
	//}}AFX_DATA
	enum eTipoConsulta m_eTipoCons;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMaqOrd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMaqOrd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDatetimechangeFecDesde(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeFecHasta(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPrintMaqord();
	afx_msg void OnEditchangeDesdeMaq();
	afx_msg void OnEditchangeHastaMaq();
	afx_msg void OnSelchangeHastaMaq();
	afx_msg void OnSelchangeDesdeMaq();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDlgIntrStr dialog

class CDlgIntrStr : public CDialog
{
// Construction
public:
	CDlgIntrStr(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgIntrStr)
	enum { IDD = IDD_INTR_STR };
	CEdit	m_EditStr;
	CString	m_sCaption;
	CString	m_sValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgIntrStr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgIntrStr)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CMyCtrlList window
class CChildFrame;
class CProg;
class CCpwDoc;
class CMyCtrlList : public CListCtrl
{
// Construction
public:
	CMyCtrlList();

// Attributes
public:
	CObArray*	 m_pArrSort;
	CChildFrame* m_pFrame;
	CProg*		 m_pProg;
	CCpwDoc*	 m_pDoc;
	CString		 m_sID;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCtrlList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyCtrlList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyCtrlList)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnCargaInicial();
	

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CDlgSelecEmpresa dialog

class CDlgSelecEmpresa : public CDialog
{
// Construction
public:
	CDlgSelecEmpresa(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelecEmpresa)
	enum { IDD = IDD_SELEC_EMPRESA };
	CComboBox	m_ComboEmpresa;
	CString	m_sEmpresa;
	//}}AFX_DATA
	CStringArray* m_psArrEmpresas;

	void LoadEmpresas(CStringArray* sArrEmpresas) {m_psArrEmpresas = sArrEmpresas;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelecEmpresa)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelecEmpresa)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDlgExcel dialog

class CDlgExcel : public CDialog
{
// Construction
public:
	CDlgExcel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgExcel)
	enum { IDD = IDD_EXCEL };
	CComboBox	m_ComboHastaMaq;
	CComboBox	m_ComboDesdeMaq;
	CDateTimeCtrl	m_DTctrlHastaFecha;
	CDateTimeCtrl	m_DTctrlDesdeFecha;
	CTime	m_FecDesde;
	CString	m_sDesdeMaq;
	CTime	m_FecHasta;
	CString	m_sHastaMaq;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExcel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExcel)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

/////////////////////////////////////////////////////////////////////////////
// CDlgLista dialog
class CDlgLista : public CDialog
{
// Construction
public:
	CDlgLista(CObArray* p_pArrInfoMolArt, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLista)
	enum { IDD = IDD_LISTA };
	CMyCtrlList	m_ListCtrl;
	BOOL	m_BNoProg;
	//}}AFX_DATA
protected:
	CObArray* m_pArrInfoMolArt;
	CObArray m_ArrSort;
public:
	CChildFrame*	m_pFrame;
	CProg*			m_pProg;
	CCpwDoc*		m_pDoc;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLista)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void UpdateList();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLista)
	virtual BOOL OnInitDialog();
	afx_msg void OnButNoprog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_DIALOGS_H__6E39F1C3_AF4A_11D1_BF7E_0080AD1B0F43__INCLUDED_)
