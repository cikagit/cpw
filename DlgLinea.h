#if !defined(AFX_DLGLINEA_H__824C3EE3_31BA_11D4_899A_F0938714E675__INCLUDED_)
#define AFX_DLGLINEA_H__824C3EE3_31BA_11D4_899A_F0938714E675__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLinea.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLinea dialog
class CInsView;

class CDlgLinea : public CDialog
{
// Construction
public:
	BOOL Validate();
	CDlgLinea(CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLinea)
	enum { IDD = IDD_LINEA };
	CComboBox	m_ComboLinMolde;
	CEdit	m_EditLinMezcla;
	CEdit	m_EditLinMaq;
	CEdit	m_EditLinHoras;
	CEdit	m_EditLinCant;
	CEdit	m_EditLinCad;
	CEdit	m_EditLinArt;
	CString	m_sLinArt;
	CString	m_sLinCad;
	CString	m_sLinCant;
	CString	m_sLinHoras;
	CString	m_sLinMaq;
	CString	m_sLinMezcla;
	CString	m_sLinMolde;
	//}}AFX_DATA

	CPoint m_Point;
	BOOL m_bClose;

protected:
	HACCEL m_hAccel; // accelerator table

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLinea)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CInsView* m_pParent;
	// Generated message map functions
	//{{AFX_MSG(CDlgLinea)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeLinArt();
	afx_msg void OnChangeLinCant();
	afx_msg void OnChangeLinCad();
	//}}AFX_MSG

	void NextInTabOrder();

	afx_msg void OnMyEnter();

	void CalcTime(); 
	LRESULT CDlgLinea::OnGetDefID(WPARAM wp, LPARAM lp) ;

	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLINEA_H__824C3EE3_31BA_11D4_899A_F0938714E675__INCLUDED_)
