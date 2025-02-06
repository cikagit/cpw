#if !defined(AFX_DLGOPCIONES_H__C8416428_BD48_11D3_8997_EA7DAEEB0A33__INCLUDED_)
#define AFX_DLGOPCIONES_H__C8416428_BD48_11D3_8997_EA7DAEEB0A33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOpciones.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOpciones dialog

class CDlgOpciones : public CDialog
{
// Construction
public:
	CDlgOpciones(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOpciones)
	enum { IDD = IDD_OPCIONES };
	CRichEditCtrl	m_DispColor6;
	CRichEditCtrl	m_DispColor5;
	CRichEditCtrl	m_DispColor4;
	CRichEditCtrl	m_DispColor3;
	CRichEditCtrl	m_DispColor2;
	CRichEditCtrl	m_DispColor1;
	int		m_Max3;
	int		m_Max4;
	int		m_Max_5;
	int		m_Min_2;
	int		m_Min_3;
	int		m_Min_4;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOpciones)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	COLORREF OnColor(int p_iNum);
	int SetValueMargen(const CString& p_sMargenID, int p_sValue);

	// Generated message map functions
	//{{AFX_MSG(CDlgOpciones)
	afx_msg void OnColor0();
	afx_msg void OnColor1();
	afx_msg void OnColor2();
	afx_msg void OnColor3();
	afx_msg void OnColor4();
	afx_msg void OnColor5();
	afx_msg void OnChangeMargenMin2();
	afx_msg void OnChangeMargenMin4();
	afx_msg void OnChangeMargenMin3();
	afx_msg void OnChangeMargenMax5();
	afx_msg void OnChangeMargenMax4();
	afx_msg void OnChangeMargenMax3();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPCIONES_H__C8416428_BD48_11D3_8997_EA7DAEEB0A33__INCLUDED_)
