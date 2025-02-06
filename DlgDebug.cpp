// DlgDebug.cpp : implementation file
//

#include "stdafx.h"
#include "cpw.h"
#include "DlgDebug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDebug dialog


CDlgDebug::CDlgDebug(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDebug::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDebug)
	m_sDebugList = _T("");
	//}}AFX_DATA_INIT
}


void CDlgDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDebug)
	DDX_Control(pDX, IDC_DEBUGLIST, m_ListDebug);
	DDX_LBString(pDX, IDC_DEBUGLIST, m_sDebugList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDebug, CDialog)
	//{{AFX_MSG_MAP(CDlgDebug)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDebug message handlers

BOOL CDlgDebug::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int iLim = m_ArrStr.GetSize();
	for (int i=0; i < iLim; i++)
	{
		m_ListDebug.AddString(m_ArrStr.GetAt(i));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
