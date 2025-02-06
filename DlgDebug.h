#if !defined(AFX_DLGDEBUG_H__167D1081_1640_11D4_899A_928157A73906__INCLUDED_)
#define AFX_DLGDEBUG_H__167D1081_1640_11D4_899A_928157A73906__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDebug.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDebug dialog

class CDlgDebug : public CDialog
{
// Construction
public:
	CString m_sID;
	CStringArray m_ArrStr;
	CDlgDebug(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDebug)
	enum { IDD = IDD_DEBUG };
	CListBox	m_ListDebug;
	CString	m_sDebugList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDebug)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDebug)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEBUG_H__167D1081_1640_11D4_899A_928157A73906__INCLUDED_)
