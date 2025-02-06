#if !defined(AFX_ORDENDLG_H__3566F713_C969_11D1_BF99_0080AD1B0F43__INCLUDED_)
#define AFX_ORDENDLG_H__3566F713_C969_11D1_BF99_0080AD1B0F43__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OrdenDlg.h : header file
//

class COrdMaq;
class CView;
class CTextView;
class CCpwView;

/////////////////////////////////////////////////////////////////////////////
// COrdenDlg dialog


class COrdenDlg : public CDialog
{
// Construction
public:
	void ActOM();
	BOOL m_bEntrada;
	BOOL m_bInsertada;
	BOOL m_bCancelar;
	COrdenDlg( COrdMaq* p_pOM, CProg* p_pProg, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COrdenDlg)
	enum { IDD = IDD_ORDEN };
	CEdit	m_EditPersonas;
	CListBox	m_LstAcabado;
	CEdit	m_EditTiradas;
	CDateTimeCtrl	m_DTctrlFecFin;
	CEdit	m_EditTiempoFijo;
	CEdit	m_EditCantTot;
	CComboBox	m_ComboInsMaq;
	CEdit	m_EditCadencia;
	CButton	m_ButtonFinalizar;
	CButton	m_ButtonBorrar;
	CButton	m_ButtonAjustar;
	CComboBox	m_ComboMolde;
	CListBox	m_LstMaqMol;
	CEdit	m_EditTiempo;
	CButton	m_ButtonSelected;
	CEdit	m_EditCantidad;
	CButton	m_ButtonTiempoFijo;
	CDateTimeCtrl	m_DTctrlFecFab;
	CDateTimeCtrl	m_DTctrlFecIni;
	CButton	m_ButtonFecFija;
	CDateTimeCtrl	m_DTctrlFecEnt;
	CListBox	m_LstMaquinas;
	CString	m_sCantidad;
	CString	m_sMaquinas;
	CString	m_sTiempo;
	CTime	m_FecEnt;
	BOOL	m_BFecFija;
	CTime	m_FecIni;
	CString	m_sCadencia;
	CString	m_sFabricadas;
	CTime	m_FecFab;
	CString	m_sInactividad;
	CString	m_sRestantes;
	BOOL	m_BTiempoFijo;
	BOOL	m_BSelected;
	CString	m_sMaqMol;
	long	m_lLote;
	CString	m_sChatarras;
	CString	m_sMolde;
	CString	m_sInsMaq;
	CString	m_sCavidades;
	CString	m_sConsumo;
	CString	m_sMezcla;
	CString	m_sMezclaKilos;
	CString	m_sPesoGramos;
	CString	m_sRestCant;
	CString	m_sRestHoras;
	CString	m_sCantTot;
	CString	m_sTiempoTot;
	CTime	m_FecFin;
	CString	m_sTiradas;
	CString	m_sAcabado;
	CString	m_sCadenciaAcabado;
	CString	m_sPersonas;
	//}}AFX_DATA

	CString	m_sCaption;
	BOOL	m_BReadOnly;
	CObArray m_ArrOM;
	CObArray m_ArrListaMaq;
	CObArray m_ArrTodasMaq;
	CObArray m_ArrListaMoldes;
	CObArray m_ArrAcabado;
	CObArray m_ArrComponentes;
	CStringArray m_ArrMoldeDetalle;

	COrdMaq* m_pOM;
	

protected:
	CView* m_pView;
	CCpwView* m_pCpwView;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrdenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CProg* m_pProg;

	// Generated message map functions
	//{{AFX_MSG(COrdenDlg)
	virtual BOOL OnInitDialog();
	void RefreshMoldeDetalle(CString p_sMoldeAlt = "");
	afx_msg void OnBorrar();
	virtual void OnOK();
	afx_msg void OnDblclkMaquinas();
	afx_msg void OnSelchangeMaquinas();
	afx_msg void OnDblclkMaqmol();
	afx_msg void OnFinalizar();
	afx_msg void OnAjustar();
	afx_msg void OnSelchangeInsmaq();
	afx_msg void OnFabricada();
	afx_msg void OnAjustarAMezcla();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CListBox m_LstComponentes;
	CListBox m_LstMoldeDetalle;
	afx_msg void OnCbnSelchangeMolde();
};

/////////////////////////////////////////////////////////////////////////////
// CDlgAjustar dialog

class CDlgAjustar : public CDialog
{
// Construction
public:
	COrdMaq m_OM;
	COrdenDlg* m_pOrdenDlg;
	CDlgAjustar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAjustar)
	enum { IDD = IDD_AJUSTAR };
	CEdit	m_EditLote;
	CSpinButtonCtrl	m_SpinRatio;
	CSpinButtonCtrl	m_SpinHoras;
	CSpinButtonCtrl	m_SpinCad;
	CEdit	m_EditRatio;
	CEdit	m_EditMaterial;
	CEdit	m_EditHoras;
	CEdit	m_EditExistencias;
	CEdit	m_EditCantNue;
	CEdit	m_EditCantAct;
	CEdit	m_EditCadNue;
	CEdit	m_EditCadAnt;
	CString	m_sCadAnt;
	CString	m_sCantAct;
	CString	m_sCantNue;
	CString	m_sExistencias;
	CString	m_sMaterial;
	CString	m_sLote;
	long	m_lCadNue;
	long	m_lHoras;
	long	m_lRatio;
	//}}AFX_DATA
	UDACCEL m_UdAcRatio;
	UDACCEL m_UdAcHoras;
	UDACCEL m_UdAcCad;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAjustar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAjustar)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeRatio();
	afx_msg void OnChangeHoras();
	afx_msg void OnChangeCadNue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDENDLG_H__3566F713_C969_11D1_BF99_0080AD1B0F43__INCLUDED_)
