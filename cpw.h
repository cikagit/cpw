// cpw.h : main header file for the CPW application
//

#if !defined(AFX_CPW_H__2F09B385_A6B1_11D1_BF6A_0080AD1B0F43__INCLUDED_)
#define AFX_CPW_H__2F09B385_A6B1_11D1_BF6A_0080AD1B0F43__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CProgMaq;
class COrdMaq;
extern COrdMaq* g_pOM;
extern COrdMaq* g_pIniMovOM;
extern CProgMaq* g_pPM;

extern int g_iContErr;		// Contador de mensajes de error de estructura

/////////////////////////////////////////////////////////////////////////////
// CCpwApp:
// See cpw.cpp for the implementation of this class
//

class CCache;    

class CCpwApp : public CWinApp
{
public:
	CCpwApp();

    CDatabase m_MyDb;
	CCache* pCache;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCpwApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCpwApp)
	afx_msg void OnAppAbout();
	afx_msg void OnTest();
	afx_msg void OnReloadDb();
	afx_msg void OnExecDb();
	afx_msg void OnExecMezclas();
	afx_msg void OnExecSegundo();
	afx_msg void OnExecMoldes();
	afx_msg void OnOpciones();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void InitGlobalValues();
	void IniTurnos(CString sDir);
	
	BOOL ExecComandos(const CString& p_sRegCom, const CString& p_sParam = "");
	bool CheckCmd(const CString& p_sComando);
	void UpdateCmd(const CString& p_sComando);
	CString GetEmpresa();


public:
	void CreateDbPedidos(const CString& p_sPath);
	void SetBrushes();

	//UINT GetProfileInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault );
	//BOOL WriteProfileInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue );
	//BOOL WriteProfileString( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue );




};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPW_H__2F09B385_A6B1_11D1_BF6A_0080AD1B0F43__INCLUDED_)
