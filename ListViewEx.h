#if !defined(AFX_LISTVIEWEX_H__DCEF9161_30C7_11D4_899A_95F9041F0A74__INCLUDED_)
#define AFX_LISTVIEWEX_H__DCEF9161_30C7_11D4_899A_95F9041F0A74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CListViewEx : public CListView
{
	DECLARE_DYNCREATE(CListViewEx)

// Construction
public:
	CListViewEx();

// Attributes
protected:
	BOOL m_bFullRowSel;

public:
	BOOL SetFullRowSel(BOOL bFillRowSel);
	BOOL GetFullRowSel();

	BOOL m_bClientWidthSel;

// Overrides
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListViewEx)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListViewEx();
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	static LPCTSTR MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset);
	void RepaintSelectedItems();

// Implementation - client area width
	int m_cxClient;

// Implementation - state icon width
	int m_cxStateImageOffset;
	afx_msg LRESULT OnSetImageList(WPARAM wParam, LPARAM lParam);

// Implementation - list view colors
	COLORREF m_clrText;
	COLORREF m_clrTextBk;
	COLORREF m_clrBkgnd;
	afx_msg LRESULT OnSetTextColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetTextBkColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetBkColor(WPARAM wParam, LPARAM lParam);

// Generated message map functions
protected:
	//{{AFX_MSG(CListViewEx)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_LISTVIEWEX_H__DCEF9161_30C7_11D4_899A_95F9041F0A74__INCLUDED_)
