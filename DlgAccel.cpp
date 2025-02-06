////////////////////////////////////////////////////////////////
// MSDN -- May 2000
//
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual C++ 6.0, runs on Windows 98 and probably NT too.
//
// CDlgWithAccelerators is a general-purpose class that adds
// accelerators to CDialog.
//
#include "stdafx.h"
#include "dlgaccel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CDlgWithAccelerators,CDialog)
END_MESSAGE_MAP()

////////////////
// do-nothing ctors and dtor
//
CDlgWithAccelerators::CDlgWithAccelerators(LPCTSTR lpszTemplateName,
	CWnd* pParentWnd) : CDialog(lpszTemplateName, pParentWnd)
{
}

CDlgWithAccelerators::CDlgWithAccelerators(UINT nIDTemplate,
	CWnd* pParentWnd) : CDialog(nIDTemplate, pParentWnd)
{
}

CDlgWithAccelerators::~CDlgWithAccelerators()
{
}

//////////////////
// Pre-translate message: translate keystrokes using acclerator table. 
//
BOOL CDlgWithAccelerators::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST) {
		HACCEL hAccel = m_hAccel;
		if (hAccel && ::TranslateAccelerator(m_hWnd, hAccel, pMsg))
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////
// Initialize dialog: load accelerators
//
BOOL CDlgWithAccelerators::OnInitDialog()
{
	BOOL bRet = CDialog::OnInitDialog();

	// Load dialog's accelerators
	m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(),
		m_lpszTemplateName); // use same resource name as dialog

	return bRet;
}
