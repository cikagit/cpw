#include "stdafx.h"

//////////////////
// Generic dialog-that-uses-accelerators.
//
class CDlgWithAccelerators : public CDialog {
public:
	CDlgWithAccelerators(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	CDlgWithAccelerators(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	~CDlgWithAccelerators();

protected:
	HACCEL m_hAccel; // accelerator table

	// MFC overrides
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
};
