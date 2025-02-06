// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "cpw.h"
#include "cpwDefs.h"
#include "AbSpan.h"

#include "cpwDb.h"
#include "cpwExt.h"
#include "cache.h"            
#include "prog.h"

#include "cpwDoc.h"

#include "ChildFrm.h"
#include "cpwView.h"
#include "TextView.h"
#include "afxcview.h"
#include "ListViewEx.h"
#include "InsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	m_sSearch = "";
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
	
	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
			// create a splitter with 1 row, 3 columns OR 3 rows 1 column
	if (!m_wndSplitter.CreateStatic(this, (g_bHorizontal ? 3:1), 
											(g_bHorizontal ? 1:3)))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// add the first splitter pane 
	if (!m_wndSplitter.CreateView((g_bHorizontal ? 2:0),
									(g_bHorizontal  ? 0:2),
		pContext->m_pNewViewClass, CSize(0, 0), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	
	// add the second splitter pane 
	if (!m_wndSplitter.CreateView((g_bHorizontal ? 1:0),
									(g_bHorizontal  ? 0:1),
		RUNTIME_CLASS(CInsView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	
	
	// add the third splitter pane 
	if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(CTextView), CSize(130, 130), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}

	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,0));
	
	return TRUE;
}

void 
CChildFrame::OnCharRec(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// El filtro se hace para evitar parpadeos de pantalla al pulsar shift y cosas asi
	if (nChar == VK_SHIFT || nChar == VK_CONTROL )
	{
		CMDIChildWnd::OnChar(nChar, nRepCnt, nFlags);

		return;
	}

	if (nChar >= VK_NUMPAD0 && nChar <= VK_NUMPAD9)
		nChar = nChar - VK_NUMPAD0 + '0';

	CTextView* pTextView = (CTextView*) m_wndSplitter.GetPane(0,0);
	ASSERT (pTextView->IsKindOf( RUNTIME_CLASS( CTextView ) ));
	SCROLLINFO ScInfo;
	pTextView->DoScroll(nChar, 1, 0);
// Aqui hay código redundante e inútil, pero bueno.
	pTextView->GetScrollInfo(SB_VERT, &ScInfo);
	int iLen = m_sSearch.GetLength();
	if (nChar == VK_NEXT ) 
	{
		pTextView->SetScrollPos(SB_VERT, pTextView->GetScrollPos(SB_VERT) + ScInfo.nPage );
		pTextView->Invalidate();
	}
	else if (nChar == VK_PRIOR ) 
	{
		pTextView->SetScrollPos(SB_VERT, pTextView->GetScrollPos(SB_VERT) - ScInfo.nPage );
		pTextView->Invalidate();

	}
	else if (nChar == VK_END ) 
	{
		pTextView->SetScrollPos(SB_VERT, ScInfo.nMax );
		pTextView->Invalidate();

	}
	else if (nChar == VK_HOME ) 
	{
		pTextView->SetScrollPos(SB_VERT, ScInfo.nMin );
		pTextView->Invalidate();

	}
	else if (nChar == VK_UP ) 
	{
		pTextView->SetScrollPos(SB_VERT, pTextView->GetScrollPos(SB_VERT) - pTextView->GetFontHeight() );
		pTextView->Invalidate();

	}
	else if (nChar == VK_DOWN ) 
	{
		pTextView->SetScrollPos(SB_VERT, pTextView->GetScrollPos(SB_VERT) + pTextView->GetFontHeight() );
		pTextView->Invalidate();

	}
	else if (nChar == VK_ESCAPE || nChar == VK_RETURN ) 
	{
		m_sSearch = "";
	} else	if (nChar == VK_BACK && iLen > 0) 
	{
		m_sSearch = m_sSearch.Left(iLen - 1);
	} else
	{
		m_sSearch += (char) nChar;
		if (!pTextView->LocateOF(m_sSearch))
			m_sSearch = m_sSearch.Left(iLen);
	}

	if (m_sSearch != "") SetMessageText("Busqueda : " + m_sSearch);
	else SetMessageText(AFX_IDS_IDLEMESSAGE);
	
	CMDIChildWnd::OnChar(nChar, nRepCnt, nFlags);
}

void 
CChildFrame::SearchOF(const CString &p_sIDArticulo)
{
	CTextView* pTextView = (CTextView*) m_wndSplitter.GetPane(0,0);
	ASSERT (pTextView->IsKindOf( RUNTIME_CLASS( CTextView ) ));
	if (!pTextView->LocateOF(p_sIDArticulo))
	{
		MessageBox("No hay OFs para éste bloque", "Error");
	}

}
