// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__2F09B389_A6B1_11D1_BF6A_0080AD1B0F43__INCLUDED_)
#define AFX_MAINFRM_H__2F09B389_A6B1_11D1_BF6A_0080AD1B0F43__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Instead of using a class here we could have just put these controls
// directly into CMainFrame.  As it is they are sending messages to the
// main frame just like they were part of it instead of part of a control
// bar.
class CVerBar : public CToolBar
{
public:
	CComboBox   m_ComboBox;
	CFont       m_Font;
};



class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_TBarDesplOf;		// ToolBar de desplazamiento por OF´s
	CVerBar		m_VerBar;			// ToolBar de visualización
	CFont*		m_pFontStatus;		// Font de Status

	void DockControlBarLeftOf(CControlBar* Bar,CControlBar* LeftOf);

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnTipoLetraSt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

static BOOL ReadWindowPlacement(LPWINDOWPLACEMENT pwp);
static void WriteWindowPlacement(LPWINDOWPLACEMENT pwp);
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__2F09B389_A6B1_11D1_BF6A_0080AD1B0F43__INCLUDED_)
