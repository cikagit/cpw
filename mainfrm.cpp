// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "afxext.h"

#include "cpw.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_TIPO_LETRA_ST, OnTipoLetraSt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT BASED_CODE verBar[] =
{
	ID_SEPARATOR           // for combo box (placeholder)
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
	delete m_pFontStatus;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Creamos los toolbars
	if (!m_TBarDesplOf.Create(this) ||
		!m_TBarDesplOf.LoadToolBar(IDR_DESPL_OF))
	{
		TRACE0("Failed to create toolbar IDR_DESPL_OF\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CWinApp* wApp = AfxGetApp();
	ASSERT_VALID(wApp);
	int iHeight = wApp->GetProfileInt("FontStatus", "Height", -10);
	int iWidth = wApp->GetProfileInt("FontStatus", "Width", 0);
	int iEscapement = wApp->GetProfileInt("FontStatus", "Escapement", 0);
	int iOrientation = wApp->GetProfileInt("FontStatus", "Orientation", 0);
	int iWeight = wApp->GetProfileInt("FontStatus", "Weight", FW_NORMAL);
	int iItalic = wApp->GetProfileInt("FontStatus", "Italic", 0);
	int iUnderline = wApp->GetProfileInt("FontStatus", "Underline", 0);
	int iStrikeOut = wApp->GetProfileInt("FontStatus", "StrikeOut", 0);
	int iCharSet = wApp->GetProfileInt("FontStatus", "CharSet", ANSI_CHARSET);
	int iOutPrecision = wApp->GetProfileInt("FontStatus", "OutPrecision", OUT_CHARACTER_PRECIS);
	int iClipPrecision = wApp->GetProfileInt("FontStatus", "ClipPrecision", CLIP_CHARACTER_PRECIS);
	int iQuality = wApp->GetProfileInt("FontStatus", "Quality", DEFAULT_QUALITY);
	int iPitchAndFamily = wApp->GetProfileInt("FontStatus", "PitchAndFamily", FIXED_PITCH);
	CString sFaceName = wApp->GetProfileString("FontStatus", "FaceName", "FIXEDSYS");
	
	m_pFontStatus = new CFont();
	m_pFontStatus->CreateFont(iHeight, iWidth, iEscapement, iOrientation, iWeight, iItalic, 
						iUnderline, iStrikeOut, iCharSet, iOutPrecision, iClipPrecision,
						iQuality, iPitchAndFamily, sFaceName );

	m_wndStatusBar.SetFont(m_pFontStatus);

/*
	if (!m_VerBar.Create(this, WS_CHILD|WS_VISIBLE|CBRS_TOP|
			CBRS_TOOLTIPS|CBRS_FLYBY, IDW_VERBAR) ||
		!m_VerBar.SetButtons(verBar, sizeof(verBar)/sizeof(UINT)))
	{
		TRACE0("Failed to create VerBar\n");
		return -1;       // fail to create
	}
	m_VerBar.SetButtonInfo(0, IDC_ESCALA, TBBS_SEPARATOR, 100);

	CRect rect;
	m_VerBar.GetItemRect(0, &rect);
	rect.top = 3;
	rect.bottom = rect.top + 100;
	if (!m_VerBar.m_ComboBox.Create(
			CBS_DROPDOWNLIST|WS_VISIBLE|WS_TABSTOP,
			rect, &m_VerBar, IDC_ESCALA))
	{
		TRACE0("Failed to create combo-box\n");
		return FALSE;
	}




	CComboBox* pCBox = &(m_VerBar.m_ComboBox);
	CString sComm = AfxGetApp()->GetProfileString("Escalas", "Uno", "300");
	WriteProfileString("Escalas", "Uno", sComm);
	pCBox->AddString(sComm);
	sComm = AfxGetApp()->GetProfileString("Escalas", "Dos", "600");
	WriteProfileString("Escalas", "Dos", sComm);
	pCBox->AddString(sComm);
	sComm = AfxGetApp()->GetProfileString("Escalas", "Tres", "1200");
	WriteProfileString("Escalas", "Tres", sComm);
	pCBox->AddString(sComm);
	sComm = AfxGetApp()->GetProfileString("Escalas", "Cuatro", "2400");
	WriteProfileString("Escalas", "Cuatro", sComm);
	pCBox->AddString(sComm);
	
	
//  Create a font for the combobox
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(logFont));

	if (!::GetSystemMetrics(SM_DBCSENABLED))
	{
		// Since design guide says toolbars are fixed height so is the font.
		logFont.lfHeight = -12;
		logFont.lfWeight = FW_BOLD;
		logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
		CString strDefaultFont;
		strDefaultFont = "Arial";
		lstrcpy(logFont.lfFaceName, strDefaultFont);
		if (!m_VerBar.m_Font.CreateFontIndirect(&logFont))
			TRACE0("Could Not create font for combo\n");
		else
			m_VerBar.m_ComboBox.SetFont(&m_VerBar.m_Font);
	}
	else
	{
		m_VerBar.m_Font.Attach(::GetStockObject(SYSTEM_FONT));
		m_VerBar.m_ComboBox.SetFont(&m_VerBar.m_Font);
	}
*/

	// Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_TBarDesplOf.SetBarStyle(m_TBarDesplOf.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
//	m_VerBar.SetBarStyle(m_VerBar.GetBarStyle() |
//		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	

	// Delete these three lines if you don't want the toolbars to be dockable
	m_wndToolBar.SetWindowText(_T("Principal"));
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_TBarDesplOf.SetWindowText(_T("Desplazar"));
	m_TBarDesplOf.EnableDocking(CBRS_ALIGN_ANY);
//	m_VerBar.SetWindowText(_T("Ver"));
//	m_VerBar.EnableDocking(CBRS_ALIGN_ANY);
	
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP);
	DockControlBarLeftOf(&m_TBarDesplOf,&m_wndToolBar);
//	DockControlBarLeftOf(&m_VerBar,&m_wndToolBar);
		
	//LoadBarState(_T("General"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::DockControlBarLeftOf(CControlBar* Bar,CControlBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

void CMainFrame::OnClose() 
{
	// SaveBarState(_T("General"));

	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnTipoLetraSt() 
{
	LOGFONT LogFontAct;
	CWinApp* wApp = AfxGetApp();
	ASSERT_VALID(wApp);
	
	LogFontAct.lfHeight = wApp->GetProfileInt("FontStatus", "Height", -10);
	LogFontAct.lfHeight = wApp->GetProfileInt("FontStatus", "Width", 0);
	LogFontAct.lfEscapement = wApp->GetProfileInt("FontStatus", "Escapement", 0);
	LogFontAct.lfOrientation = wApp->GetProfileInt("FontStatus", "Orientation", 0);
	LogFontAct.lfWeight = wApp->GetProfileInt("FontStatus", "Weight", FW_NORMAL);
	LogFontAct.lfItalic = wApp->GetProfileInt("FontStatus", "Italic", 0);
	LogFontAct.lfUnderline = wApp->GetProfileInt("FontStatus", "Underline", 0);
	LogFontAct.lfStrikeOut = wApp->GetProfileInt("FontStatus", "StrikeOut", 0);
	LogFontAct.lfCharSet = wApp->GetProfileInt("FontStatus", "CharSet", ANSI_CHARSET);
	LogFontAct.lfOutPrecision = wApp->GetProfileInt("FontStatus", "OutPrecision", OUT_CHARACTER_PRECIS);
	LogFontAct.lfClipPrecision = wApp->GetProfileInt("FontStatus", "ClipPrecision", CLIP_CHARACTER_PRECIS);
	LogFontAct.lfQuality = wApp->GetProfileInt("FontStatus", "Quality", DEFAULT_QUALITY);
	LogFontAct.lfPitchAndFamily = wApp->GetProfileInt("FontStatus", "PitchAndFamily", FIXED_PITCH);
	strncpy(LogFontAct.lfFaceName, wApp->GetProfileString("FontStatus", "FaceName", "FIXEDSYS"), sizeof(LogFontAct.lfFaceName));
	
	
	CFontDialog FontDialog;
	FontDialog.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;
	FontDialog.m_cf.lpLogFont = &LogFontAct;
	
	if (FontDialog.DoModal() != IDOK) return;

	LOGFONT LogFont;
	FontDialog.GetCurrentFont(&LogFont);
	
	CFont* FontDel = m_pFontStatus;
	m_pFontStatus = new CFont();
	m_pFontStatus->CreateFontIndirect(&LogFont);

	//Actualizamos el Registry
	wApp->WriteProfileInt("FontStatus", "Height", LogFont.lfHeight);
	wApp->WriteProfileInt("FontStatus", "Width", LogFont.lfWidth);
	wApp->WriteProfileInt("FontStatus", "Escapement", LogFont.lfEscapement);
	wApp->WriteProfileInt("FontStatus", "Orientation", LogFont.lfOrientation);
	wApp->WriteProfileInt("FontStatus", "Weight", LogFont.lfWeight);
	wApp->WriteProfileInt("FontStatus", "Italic", LogFont.lfItalic);
	wApp->WriteProfileInt("FontStatus", "Underline", LogFont.lfUnderline);
	wApp->WriteProfileInt("FontStatus", "StrikeOut", LogFont.lfStrikeOut);
	wApp->WriteProfileInt("FontStatus", "CharSet", LogFont.lfCharSet);
	wApp->WriteProfileInt("FontStatus", "OutPrecision", LogFont.lfOutPrecision);
	wApp->WriteProfileInt("FontStatus", "ClipPrecision", LogFont.lfClipPrecision);
	wApp->WriteProfileInt("FontStatus", "Quality", LogFont.lfQuality);
	wApp->WriteProfileInt("FontStatus", "PitchAndFamily", LogFont.lfPitchAndFamily);
	wApp->WriteProfileString("FontStatus", "FaceName", LogFont.lfFaceName);
	
	m_wndStatusBar.SetFont(m_pFontStatus);
	delete FontDel;

	
}
