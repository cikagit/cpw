// cpw.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxdtctl.h"
#include "afxext.h"

#include "cpw.h"
#include "cpwDefs.h"

#include "MainFrm.h"
#include "ObArrOrd.h"
#include "AbSpan.h"
#include "cpwExt.h"
#include "prog.h"
#include "cpwDb.h"
#include "cache.h"            
#include "ProgMaq.h"
#include "ChildFrm.h"
#include "cpwDoc.h"
#include "cpwView.h"
#include "TextView.h"
#include "DlgOpciones.h" 
#include "Dialogs.h" 
#include "Turnos.h" 

#include "SADirRead.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCache* g_pCache = NULL;
CProgMaq* g_pPM = NULL;
COrdMaq* g_pOM = NULL;
COrdMaq* g_pIniMovOM = NULL;
BOOL	g_bException = FALSE; // Solo hacer TRUE antes de sacar un mesaje y salir de la app
BOOL	g_bHorizontal = TRUE;
BOOL	g_BMoviendo = FALSE;
enum EObjSel g_eObjSel;	
int g_iContErr = 0;		// Contador de mensajes de error de estructura

struct SMargenColor g_MC;

struct SVerOF g_VerOF;

CString g_sEmpresa = "";

CString g_sError = "\\%%**ERROR";

CBrush	g_BrushParada;
CBrush	g_BrushArr[CPW_N_BRUSHES*2];
CBrush	g_BrushArrImp[CPW_N_BRUSHES*2];

//CLogFile logFile("logFile.txt",false, 100000000);


/////////////////////////////////////////////////////////////////////////////
// CCpwApp

BEGIN_MESSAGE_MAP(CCpwApp, CWinApp)
	//{{AFX_MSG_MAP(CCpwApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_TEST, OnTest)
	ON_COMMAND(ID_RELOAD_DB, OnReloadDb)
	ON_COMMAND(ID_EXEC_DB, OnExecDb)
	ON_COMMAND(ID_EXEC_MEZCLAS, OnExecMezclas)
	ON_COMMAND(ID_EXEC_SEGUNDO, OnExecSegundo)
	ON_COMMAND(ID_EXEC_MOLDES, OnExecMoldes)
	ON_COMMAND(ID_OPCIONES, OnOpciones)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCpwApp construction

CCpwApp::CCpwApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCpwApp object

CCpwApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCpwApp initialization

BOOL CCpwApp::InitInstance()
{

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	//Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	g_sEmpresa = GetEmpresa();		//Selecciona la empresa con la que queremos trabajar
	CString sKey = "Complex";
	if (g_sEmpresa.GetLength() > 0) sKey += "\\" + g_sEmpresa;
//	logFile.Write("%s - %s", "Entrando.", sKey);
	SetRegistryKey(_T(sKey));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)



	g_bHorizontal = GetProfileInt("Valores Iniciales", "Vistas Horizontales", TRUE);
	WriteProfileInt("Valores Iniciales", "Vistas Horizontales", g_bHorizontal);

	g_VerOF.m_bAcumOFs = FALSE;
	g_VerOF.m_bSelecFec = FALSE;
	g_VerOF.m_bSoloNoProg = FALSE;
	g_VerOF.m_bSoloNoFab = FALSE;
	g_VerOF.m_FecSelec = CTime::GetCurrentTime();
	g_VerOF.m_OrdenOF = Articulo;

	
	WriteProfileInt("Dialogo de Impresion", "TipoListado", 0);
	WriteProfileString("Dialogo de Impresion", "DesdeGFH", "1");
	WriteProfileString("Dialogo de Impresion", "HastaGFH", "1");
	WriteProfileString("Dialogo de Impresion", "DesdeMaq", "1");
	WriteProfileString("Dialogo de Impresion", "HastaMaq", "1");
	
	CTime FecTemp = CTime::GetCurrentTime();
	WriteProfileInt("Dialogo de Impresion", "DesdeFecYY", FecTemp.GetYear());
	WriteProfileInt("Dialogo de Impresion", "DesdeFecMM", FecTemp.GetMonth());
	WriteProfileInt("Dialogo de Impresion", "DesdeFecDD", FecTemp.GetDay());

	WriteProfileInt("Dialogo de Impresion", "HastaFecYY", FecTemp.GetYear());
	WriteProfileInt("Dialogo de Impresion", "HastaFecMM", FecTemp.GetMonth());
	WriteProfileInt("Dialogo de Impresion", "HastaFecDD", FecTemp.GetDay());

	// Obtenemos el directorio y el nombre de la base de datos
	CString sDbDir, sDbName, sDb;
	sDbDir = GetProfileString("Base de Datos Cikautxo", "Directorio", "J:\\cpw\\" + g_sEmpresa + "\\datos");
	sDbName = GetProfileString("Base de Datos Cikautxo", "Nombre", "cikadat.accdb");
	WriteProfileString("Base de Datos Cikautxo", "Directorio", sDbDir);
	WriteProfileString("Base de Datos Cikautxo", "Nombre", sDbName);
   	//Miramos a ver si hay un fichero de turnos en el directorio de datos, y si lo hay lo cargamos.
	IniTurnos(sDbDir);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_CPWTYPE,
		RUNTIME_CLASS(CCpwDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CCpwView));
	AddDocTemplate(pDocTemplate);

	CMultiDocTemplate* pDocTemplateTxt;
	pDocTemplateTxt = new CMultiDocTemplate(
		IDR_TEXTTYPE,
		RUNTIME_CLASS(CCpwDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTextView));
	AddDocTemplate(pDocTemplateTxt);


	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	
	// Abrimos la base de datos y inicializamos el objeto "Cache" para usarlo como
	// interface con la base de datos.
	sDb = "ODBC;DSN=MS Access Database;DBQ=" + sDbDir + "\\" +
             sDbName + ";DefaultDir=" + sDbDir + 
			 ";";
    m_MyDb.Open(sDb);  
	
	
	if (!m_MyDb.IsOpen())
		return FALSE;
	
	// Creamos el cache

    g_pCache = new CCache(&m_MyDb);
    if (!g_pCache)
	{
		CString sMsg = STRID( IDS_ERR_NODB );
		sMsg = sMsg + sDbDir + "\\" + sDbName ;
		pMainFrame->MessageBox(sMsg);
		return FALSE;
	}

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Inicializamos los valores globales
	InitGlobalValues();
		
	// The main window has been initialized, so show and update it.
	//pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	

	AfxInitRichEdit( );

	return TRUE;
}

void
CCpwApp::IniTurnos(CString sDir)
{
	if (FileExists(sDir + "\\Turnos.ini"))
	{
		CTurnos::LoadTurnos();
	} else {
		// El fichero Turnos.ini no existe, así que no hacemos nada, se tomarán valores por defecto
	}
}

void
CCpwApp::InitGlobalValues()
{
	// Esta función es llamada al inicial el programa y al cambiar los colores
	COLORREF colTmp;
	colTmp = GetProfileInt("Valores Iniciales", "Color Bloque 0", TK_COLOR_0);
	g_BrushArr[0].CreateSolidBrush(colTmp);		 // Ordenes sin peligro
	g_BrushArr[1].CreateSolidBrush(SelecColorRef(colTmp));			// Sin peligro seleccionadas

	colTmp = GetProfileInt("Valores Iniciales", "Color Bloque 1", TK_COLOR_1);
	g_BrushArr[2].CreateSolidBrush(colTmp);		 // Ordenes peligro 1
	g_BrushArr[3].CreateSolidBrush(SelecColorRef(colTmp));			// peligro 1 seleccionadas
	
	colTmp = GetProfileInt("Valores Iniciales", "Color Bloque 2", TK_COLOR_2);
	g_BrushArr[4].CreateSolidBrush(colTmp);
	g_BrushArr[5].CreateSolidBrush(SelecColorRef(colTmp));

	g_BrushParada.CreateSolidBrush(RGB(255, 255, 255));
	
	g_BrushArrImp[0].CreateSolidBrush(RGB(255, 255, 255));
	g_BrushArrImp[1].CreateSolidBrush(RGB(150, 150, 150));
	g_BrushArrImp[2].CreateSolidBrush(RGB(255, 200, 152));
	g_BrushArrImp[3].CreateSolidBrush(RGB(220, 172, 52));
	g_BrushArrImp[4].CreateSolidBrush(RGB(240, 10, 10));
	g_BrushArrImp[5].CreateSolidBrush(RGB(176, 0, 0));



	g_MC.m_Color1 = GetProfileInt("Valores Iniciales", "Color Peligro 0", 0);
	g_MC.m_Color2 = GetProfileInt("Valores Iniciales", "Color Peligro 1", 0);
	g_MC.m_Color3 = GetProfileInt("Valores Iniciales", "Color Peligro 2", 0);
	g_MC.m_Color4 = GetProfileInt("Valores Iniciales", "Color Peligro 3", 0);
	g_MC.m_Color5 = GetProfileInt("Valores Iniciales", "Color Peligro 4", 0);
	g_MC.m_Color6 = GetProfileInt("Valores Iniciales", "Color Peligro 5", 0);
	
	g_MC.m_iMax3 = GetProfileInt("Valores Iniciales", "Margen Max_3", 0);
	g_MC.m_iMax4 = GetProfileInt("Valores Iniciales", "Margen Max_4", 0);
	g_MC.m_iMax5 = GetProfileInt("Valores Iniciales", "Margen Max_5", 0);
	g_MC.m_iMin2 = GetProfileInt("Valores Iniciales", "Margen Min_2", 0);
	g_MC.m_iMin3 = GetProfileInt("Valores Iniciales", "Margen Min_3", 0);
	g_MC.m_iMin4 = GetProfileInt("Valores Iniciales", "Margen Min_4", 0);



	
}

void
CCpwApp::SetBrushes()
{
	COLORREF colTmp;
	colTmp = GetProfileInt("Valores Iniciales", "Color Bloque 0", TK_COLOR_0);

	g_BrushArr[0].DeleteObject();
	g_BrushArr[1].DeleteObject();
	g_BrushArr[2].DeleteObject();
	g_BrushArr[3].DeleteObject();
	g_BrushArr[4].DeleteObject();
	g_BrushArr[5].DeleteObject();
	
	g_BrushArr[0].CreateSolidBrush(colTmp);		 // Ordenes sin peligro
	g_BrushArr[1].CreateSolidBrush(SelecColorRef(colTmp));			// Sin peligro seleccionadas

	colTmp = GetProfileInt("Valores Iniciales", "Color Bloque 1", TK_COLOR_1);
	g_BrushArr[2].CreateSolidBrush(colTmp);		 // Ordenes peligro 1
	g_BrushArr[3].CreateSolidBrush(SelecColorRef(colTmp));			// peligro 1 seleccionadas
	
	colTmp = GetProfileInt("Valores Iniciales", "Color Bloque 2", TK_COLOR_2);
	g_BrushArr[4].CreateSolidBrush(colTmp);
	g_BrushArr[5].CreateSolidBrush(SelecColorRef(colTmp));
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CCpwApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CCpwApp commands

int CCpwApp::ExitInstance() 
{
	delete g_pCache;
	
	return CWinApp::ExitInstance();
}

void CCpwApp::OnTest() 
{
}

void CCpwApp::OnReloadDb() 
{
	CWaitCursor WaitCursor;

	g_pCache->EmptyArrays();
	g_pCache->IniFillCache(TRUE);
	//Ahora tenemos que volver a poner los punteros de las OM's y PM's de
	// todos los documentos abiertos (de tipo Cpw)
    POSITION posTemp = GetFirstDocTemplatePosition(); 
	ASSERT (posTemp);
	CDocTemplate* pDocTemplate = GetNextDocTemplate(posTemp);
	if (pDocTemplate) // Cogemos cualquier Template, todas van a los mismos documentos
	{
		POSITION pos = pDocTemplate->GetFirstDocPosition(); 
		if (pos == NULL) return;		
		CCpwDoc* pDoc = NULL;
		CProg* pProg = NULL;
		do
		{
			pDoc = (CCpwDoc*)pDocTemplate->GetNextDoc(pos);
	
			if (pDoc->IsKindOf( RUNTIME_CLASS(CCpwDoc) ) )
			{
				pProg = pDoc->GetpProg();

				pProg->RelinkAll();
				pDoc->UpdateAllViews(NULL);
				pDoc->UpdateTextView();
			}
		} while (pos != NULL);
	}
}

BOOL 
CCpwApp::ExecComandos(const CString& p_sRegCom, const CString& p_sParam)
{
	STARTUPINFO startup;
	PROCESS_INFORMATION process;
	
	memset( &startup, 0, sizeof( startup ) );
	startup.cb = sizeof( startup );
	
	memset( &process, 0, sizeof( process ) );
	
	CString sComm = GetProfileString("Comandos", p_sRegCom, "");
	WriteProfileString("Comandos", p_sRegCom, sComm);

	if (sComm == "") return FALSE;

	sComm += p_sParam;

	char* pszCommandLine = sComm.GetBuffer( 0 );
	BOOL tSuccess = CreateProcess( NULL, pszCommandLine, NULL, NULL, FALSE, 0, NULL,   NULL, &startup, &process );
	sComm.ReleaseBuffer();
	if( tSuccess )
	{
		WaitForInputIdle(process.hProcess, INFINITE);
		HANDLE aHandles[1];
		aHandles[0] = process.hProcess;
		DWORD dwWaitStatus;
		do 
		{
			dwWaitStatus = MsgWaitForMultipleObjects(1, aHandles, FALSE, INFINITE, QS_ALLEVENTS);
			if (dwWaitStatus != WAIT_OBJECT_0)
			{
				MSG msg;
				PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE);
			}
		} while (dwWaitStatus != WAIT_OBJECT_0);
	}	
	return tSuccess;
	
}

void 
CCpwApp::CreateDbPedidos(const CString& p_sPath) 
{
	CString sParam = (CString) " /cmd " + p_sPath;
	ExecComandos(TK_CMD_PEDIDOS, sParam);
}

void 
CCpwApp::OnExecDb() 
{
	
	if (!CheckCmd(TK_CMD_DIARIO)) return;

	if (ExecComandos(TK_CMD_DIARIO))
	{
		UpdateCmd(TK_CMD_DIARIO);
		AfxMessageBox( "Ahora se recargará la base de datos completa" );
		CWaitCursor WaitCursor;
		OnReloadDb();
	}
	else
	{
			AfxMessageBox( "Error al ejecutar proceso, no se recargará la base de datos." );
	}
}

void CCpwApp::OnExecSegundo() 
{
	if (!CheckCmd(TK_CMD_OFART)) return;
	if (ExecComandos(TK_CMD_OFART))
	{
		UpdateCmd(TK_CMD_OFART);
		AfxMessageBox( "Ahora se recargará la base de datos completa" );
		CWaitCursor WaitCursor;
		OnReloadDb();
	}
	else
	{
			AfxMessageBox( "Error al ejecutar proceso, no se recargará la base de datos." );
	}
		
}

void CCpwApp::OnExecMezclas() 
{
	if (!CheckCmd(TK_CMD_MEZCLAS)) return;
	if (ExecComandos(TK_CMD_MEZCLAS))
	{
		UpdateCmd(TK_CMD_MEZCLAS);
		AfxMessageBox( "Ahora se recargará la base de datos de mezclas." );
		CWaitCursor WaitCursor;
		g_pCache->UpdateMezclasPlanta();
	}
	else
	{
			AfxMessageBox( "Error al ejecutar proceso, no se recargará la base de datos." );
	}
}

void CCpwApp::OnExecMoldes() 
{
	if (!CheckCmd(TK_CMD_MOLDES)) return;
	if (ExecComandos(TK_CMD_MOLDES))
	{
		UpdateCmd(TK_CMD_MOLDES);
		AfxMessageBox( "Ahora se recargará la base de datos completa" );
		CWaitCursor WaitCursor;
		OnReloadDb();
	}
	else
	{
			AfxMessageBox( "Error al ejecutar proceso, no se recargará la base de datos." );
	}
		
}

bool
CCpwApp::CheckCmd(const CString& p_sComando)
{
	CString sMsg;
	CString sFec = g_pCache->LeerFechaParam(p_sComando);
	sMsg.Format(
		"Va a efectuar un proceso de actualización (%s). Este proceso se realizó por última vez el %s",
		p_sComando, 
		sFec);


	if (AfxMessageBox( sMsg, MB_OKCANCEL ) == IDOK ) return true;
	else return false;
}

void
CCpwApp::UpdateCmd(const CString& p_sComando)
{
	g_pCache->EscribirFechaParam(p_sComando);

}
	
void CCpwApp::OnOpciones() 
{
	CDlgOpciones DlgOpciones;
	DlgOpciones.DoModal();

	
	POSITION posTemp = GetFirstDocTemplatePosition(); 
	ASSERT (posTemp);
	CDocTemplate* pDocTemplate = GetNextDocTemplate(posTemp);
	if (pDocTemplate) // Cogemos cualquier Template, todas van a los mismos documentos
	{
		POSITION pos = pDocTemplate->GetFirstDocPosition(); 
		if (pos == NULL) return;		
		CCpwDoc* pDoc = NULL;
		CProg* pProg = NULL;
		do
		{
			pDoc = (CCpwDoc*)pDocTemplate->GetNextDoc(pos);
	
			if (pDoc->IsKindOf( RUNTIME_CLASS(CCpwDoc) ) )
			{
				pDoc->UpdateAllViews(NULL);
				pDoc->UpdateTextView();
			}
		} while (pos != NULL);
	}
	
}

// Mira si hay más directorios al mismo nivel que empiezen por "Empresa_" y 
// en ese caso pregunta al usuario para que elija una. Devuelve la elegida o la
// actual si es que no hay más empresas
CString CCpwApp::GetEmpresa()
{
	CSADirRead dr;
	CStringArray sArrEmpresas;

	// Las empresas son subdirectorios del directorio actual, que empiezan por "Empresa_"
	dr.ClearDirs();         
	dr.GetDirs(".",true); 

	CSADirRead::SADirVector &dirs = dr.Dirs();

	for (CSADirRead::SADirVector::const_iterator dit = dirs.begin(); dit!=dirs.end(); dit++)
	{
		if ((*dit).m_sName.Left(10) == ".\\Empresa_")
			sArrEmpresas.Add((*dit).m_sName.Right((*dit).m_sName.GetLength()-2));
	}
	
	if (sArrEmpresas.GetSize() == 0) {
		return "";
	} else if (sArrEmpresas.GetSize() == 1) {
		return sArrEmpresas.GetAt(0);
	} else {
		CDlgSelecEmpresa DlgEmpresa;
		DlgEmpresa.LoadEmpresas(&sArrEmpresas);

		if (DlgEmpresa.DoModal() != IDOK)
		{
			return sArrEmpresas.GetAt(0);
		} else {                       
			return DlgEmpresa.m_sEmpresa;
		}
	}

}

