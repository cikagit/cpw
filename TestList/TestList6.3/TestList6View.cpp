// TestList6View.cpp : implementation of the CTestList6View class
//

#include "stdafx.h"
#include "TestList6.h"

#include "TestList6Doc.h"
#include "TestList6View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestList6View

IMPLEMENT_DYNCREATE(CTestList6View, CListViewExt)

BEGIN_MESSAGE_MAP(CTestList6View, CListViewExt)
	//{{AFX_MSG_MAP(CTestList6View)
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_VIEW_LINEUP, OnViewLineup)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnListStyle)
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateListStyles)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestList6View construction/destruction

CTestList6View::CTestList6View()
{
	// TODO: add construction code here

}

CTestList6View::~CTestList6View()
{
}

BOOL CTestList6View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style |= LVS_REPORT;

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestList6View drawing

void CTestList6View::OnDraw(CDC* pDC)
{
	CTestList6Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CTestList6View::OnInitialUpdate()
{
	CListViewExt::OnInitialUpdate();

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().

	if(! GetListCtrl().GetItemCount())
	{
		m_pFont = GetListCtrl().GetFont();
		CRect Rect(CPoint(0,0),CSize(100,500));
		m_DT.Create(WS_CHILD | WS_TABSTOP, Rect, this, IDC_DATE);
		m_Combo.Create(WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SORT | CBS_AUTOHSCROLL,Rect,this,IDC_COMBO);
		m_Edit.Create(WS_CHILD | WS_TABSTOP | WS_BORDER,Rect,this,IDC_EDIT);

		m_Combo.AddString("Test 1");
		m_Combo.AddString("Test 2");
		m_Combo.AddString("Test 3");
		m_Combo.AddString("Test 4");
		m_Combo.AddString("Test 5");
		m_Combo.AddString("Test 6");
		m_Combo.AddString("Test 7");
		m_Combo.AddString("Test 8");
		m_Combo.AddString("Test 9");

		m_Edit.SetFont(m_pFont);
		m_Combo.SetFont(m_pFont);
	}

	GetListCtrlExt().SetGridBehaviour();
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_INFOTIP);

	GetListCtrlExt().DeleteAllColumns();
	GetListCtrl().InsertColumn(0, "Integer",	LVCFMT_LEFT, 100);
	GetListCtrl().InsertColumn(1, "String", LVCFMT_LEFT, 100);
	GetListCtrl().InsertColumn(2, "List", LVCFMT_LEFT, 100);
	GetListCtrl().InsertColumn(3, "DateTime", LVCFMT_LEFT, 100);
	GetListCtrl().InsertColumn(4, "Random", LVCFMT_LEFT, 100);

	GetListCtrlExt().SetColumnEditor(2, &CTestList6View::InitEditor, &CTestList6View::EndEditor, &m_Combo);
	GetListCtrlExt().SetColumnEditor(3, &CTestList6View::InitEditor, &CTestList6View::EndEditor, &m_DT);

	GetListCtrlExt().SetColumnSorting(0, CListViewExt::Auto, CListViewExt::Int);
	GetListCtrlExt().SetColumnSorting(1, CListViewExt::Auto, CListViewExt::StringNoCase);
	GetListCtrlExt().SetColumnSorting(2, CListViewExt::Auto, CListViewExt::StringNoCase);
	GetListCtrlExt().SetColumnSorting(3, CListViewExt::Auto, CListViewExt::Date);
	GetListCtrlExt().SetColumnSorting(4, CListViewExt::Auto, CListViewExt::StringNoCase);

	PopulateList();
	GetListCtrlExt().GetHeaderCtrl()->SetRemovable(0,FALSE);
	GetListCtrlExt().RestoreState(_T("TestList"));
}

BOOL CTestList6View::InitEditor(CWnd** pWnd, int nRow, int nColumn, CString &strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
	ASSERT(*pWnd);
	switch(nColumn)
	{
	case 2:
		{
			CComboBox* pCmb = reinterpret_cast<CComboBox*>(*pWnd);
			pCmb->SelectString(0, strSubItemText);
		}
		break;
	case 3:
		{
			CDateTimeCtrl* pDTC = reinterpret_cast<CDateTimeCtrl*>(*pWnd);
			COleDateTime dt;
			if(dt.ParseDateTime(strSubItemText))pDTC->SetTime(dt);
		}
		break;
	}

	return TRUE;
}

BOOL CTestList6View::EndEditor(CWnd** pWnd, int nRow, int nColumn, CString &strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
	ASSERT(pWnd);
	switch(nColumn)
	{
	case 2:
		{
			CComboBox* pCmb = reinterpret_cast<CComboBox*>(*pWnd);
			int index = pCmb->GetCurSel();
			if(index >= 0) pCmb->GetLBText(index, strSubItemText);
		}
		break;
	case 3:
		{
			CDateTimeCtrl* pDTC = reinterpret_cast<CDateTimeCtrl*>(*pWnd);
			COleDateTime dt;
			pDTC->GetTime(dt);
			strSubItemText = dt.Format();
		}
		break;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTestList6View printing

BOOL CTestList6View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestList6View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestList6View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestList6View diagnostics

#ifdef _DEBUG
void CTestList6View::AssertValid() const
{
	CListView::AssertValid();
}

void CTestList6View::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CTestList6Doc* CTestList6View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestList6Doc)));
	return (CTestList6Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestList6View message handlers

void CTestList6View::OnDestroy() 
{
	CListViewExt::OnDestroy();

	// TODO: Add your message handler code here

	GetListCtrlExt().SaveState(_T("TestList"));
}

void CTestList6View::PopulateList()
{
	int nIndex = -1;
	CListCtrl& ListCtrl = GetListCtrl();
	CString sDate = COleDateTime(CTime::GetCurrentTime().GetTime()).Format();

	ListCtrl.DeleteAllItems();

	for(int i = 0;i < 10;++i)
	{
		CString sTemp;
		sTemp.Format("%d",i * 100);
		nIndex = ListCtrl.InsertItem(i, sTemp);
		if(i % 4)ListCtrl.SetItemText(i, 1, "Test string");
		else ListCtrl.SetItemText(i, 1, "Lorepsum ipsum");
		ListCtrl.SetItemText(i, 2, "Choose value from ComboBox");
		if(i % 3)ListCtrl.SetItemText(i, 3, _T("31/12/2008"));
		else ListCtrl.SetItemText(i, 3, sDate);
		sTemp.Format(_T("%04X"),rand() % 0x10000);
		ListCtrl.SetItemText(i, 4, sTemp);
		if(i % 3)
		{
			GetListCtrlExt().SetCellEditor(i,4,&m_Edit);
			GetListCtrlExt().SetCellColors(i,4,RGB(230,255,230),-1);
		}
		GetListCtrlExt().SetItemUserData(nIndex,nIndex + 1000);
	}

	GetListCtrlExt().SetColumnColors(2,-1,RGB(0,180,0));
	GetListCtrlExt().SetColumnColors(3,RGB(200,255,200),-1);
	GetListCtrlExt().SetCellReadOnly(7,3);
	GetListCtrlExt().SetCellColors(7,3,RGB(255,255,255),-1);
	GetListCtrlExt().SetCellColors(3,0,-1,RGB(0,0,255));
}

void CTestList6View::RefreshList()
{
	SetRedraw(FALSE);
	PopulateList();
	GetListCtrlExt().SortOnColumn(GetListCtrlExt().GetColumnSort());
	SetRedraw();
	Invalidate(FALSE);
}

void CTestList6View::OnListStyle(UINT nCommandID)
{
	DWORD dwStyle = -1;

	switch (nCommandID)
	{
	// other commands change the style on the list control
	case ID_VIEW_DETAILS:
		dwStyle = LVS_REPORT;
		break;
	case ID_VIEW_SMALLICON:
		dwStyle = LVS_SMALLICON;
		break;
	case ID_VIEW_LARGEICON:
		dwStyle = LVS_ICON;
		break;
	case ID_VIEW_LIST:
		dwStyle = LVS_LIST;
		break;
	case ID_VIEW_LINEUP:
		GetListCtrl().Arrange(LVA_SNAPTOGRID);
		break;
	}

	// change the style; window will repaint automatically
	if(dwStyle != -1)ModifyStyle(LVS_TYPEMASK, dwStyle);
	if(dwStyle == LVS_REPORT)GetListCtrlExt().SetGridBehaviour();
	else GetListCtrlExt().SetGridBehaviour(FALSE);
}

void CTestList6View::OnUpdateListStyles(CCmdUI* pCmdUI)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.

	BOOL bChecked = FALSE;
	DWORD dwStyle = GetListCtrl().GetStyle() & LVS_TYPEMASK;

	if(pCmdUI->m_nID == ID_VIEW_LINEUP)
	{
		if(dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)pCmdUI->Enable();
		else pCmdUI->Enable(FALSE);
	}
	else
	{
		switch (pCmdUI->m_nID)
		{
		case ID_VIEW_DETAILS:
			bChecked = (dwStyle == LVS_REPORT);
			break;
		case ID_VIEW_SMALLICON:
			bChecked = (dwStyle == LVS_SMALLICON);
			break;
		case ID_VIEW_LARGEICON:
			bChecked = (dwStyle == LVS_ICON);
			break;
		case ID_VIEW_LIST:
			bChecked = (dwStyle == LVS_LIST);
			break;
		default:
			bChecked = FALSE;
			break;
		}
		pCmdUI->SetRadio(bChecked ? 1 : 0);
	}
}

void CTestList6View::OnViewRefresh() 
{
	// TODO: Add your command handler code here

	RefreshList();
}


void CTestList6View::OnViewLineup() 
{
	// TODO: Add your command handler code here

	GetListCtrl().Arrange(LVA_DEFAULT);
}
