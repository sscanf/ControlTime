// GestionHorario.cpp : define los comportamientos de las clases para la aplicaci�n.
//

#include "stdafx.h"
#include "GestionHorario.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "GestionHorarioDoc.h"
#include "GestionHorarioView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGestionHorarioApp

BEGIN_MESSAGE_MAP(CGestionHorarioApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Comandos de documento est�ndar basados en archivo
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// Construcci�n de CGestionHorarioApp

CGestionHorarioApp::CGestionHorarioApp()
{
	// TODO: agregar aqu� el c�digo de construcci�n,
	// Colocar toda la inicializaci�n importante en InitInstance
}


// El �nico objeto CGestionHorarioApp

CGestionHorarioApp theApp;

// Inicializaci�n de CGestionHorarioApp

BOOL CGestionHorarioApp::InitInstance()
{
	// Windows XP requiere InitCommonControls() si un manifiesto de 
	// aplicaci�n especifica el uso de ComCtl32.dll versi�n 6 o posterior para habilitar
	// estilos visuales. De lo contrario, se generar� un error al crear ventanas.
	InitCommonControls();

	CWinApp::InitInstance();

	// Inicializar bibliotecas OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Inicializaci�n est�ndar
	// Si no utiliza estas caracter�sticas y desea reducir el tama�o
	// del archivo ejecutable final, debe quitar
	// las rutinas de inicializaci�n espec�ficas que no necesite
	// Cambie la clave del Registro en la que se almacena la configuraci�n
	// TODO: debe modificar esta cadena para que contenga informaci�n correcta
	// como el nombre de su compa��a u organizaci�n
	SetRegistryKey(_T("Aplicaciones generadas con el Asistente para aplicaciones local"));
	LoadStdProfileSettings(4);  // Cargar opciones de archivo INI est�ndar (incluidas las de la lista MRU)
	// Registrar las plantillas de documento de la aplicaci�n. Las plantillas de documento
	//  sirven como conexi�n entre documentos, ventanas de marco y vistas
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_GestionHorarioTYPE,
		RUNTIME_CLASS(CGestionHorarioDoc),
		RUNTIME_CLASS(CChildFrame), // Marco MDI secundario personalizado
		RUNTIME_CLASS(CGestionHorarioView));
	AddDocTemplate(pDocTemplate);
	// Crear ventana de marco principal MDI
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	// Llamar a DragAcceptFiles s�lo si existe un sufijo
	//  En una aplicaci�n MDI, esto debe ocurrir inmediatamente despu�s de establecer m_pMainWnd
	// Analizar l�nea de comandos para comandos Shell est�ndar, DDE, Archivo Abrir
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Enviar comandos especificados en la l�nea de comandos. Devolver� FALSE si
	// la aplicaci�n se inici� con los modificadores /RegServer, /Register, /Unregserver o /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// La ventana principal se ha inicializado; mostrarla y actualizarla
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();
	return TRUE;
}



// Cuadro de di�logo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Datos del cuadro de di�logo
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementaci�n
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// Comando de la aplicaci�n para ejecutar el cuadro de di�logo
void CGestionHorarioApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// Controladores de mensaje de CGestionHorarioApp

