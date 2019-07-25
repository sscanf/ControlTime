// GestionHorario.cpp : define los comportamientos de las clases para la aplicación.
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
	// Comandos de documento estándar basados en archivo
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// Construcción de CGestionHorarioApp

CGestionHorarioApp::CGestionHorarioApp()
{
	// TODO: agregar aquí el código de construcción,
	// Colocar toda la inicialización importante en InitInstance
}


// El único objeto CGestionHorarioApp

CGestionHorarioApp theApp;

// Inicialización de CGestionHorarioApp

BOOL CGestionHorarioApp::InitInstance()
{
	// Windows XP requiere InitCommonControls() si un manifiesto de 
	// aplicación especifica el uso de ComCtl32.dll versión 6 o posterior para habilitar
	// estilos visuales. De lo contrario, se generará un error al crear ventanas.
	InitCommonControls();

	CWinApp::InitInstance();

	// Inicializar bibliotecas OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Inicialización estándar
	// Si no utiliza estas características y desea reducir el tamaño
	// del archivo ejecutable final, debe quitar
	// las rutinas de inicialización específicas que no necesite
	// Cambie la clave del Registro en la que se almacena la configuración
	// TODO: debe modificar esta cadena para que contenga información correcta
	// como el nombre de su compañía u organización
	SetRegistryKey(_T("Aplicaciones generadas con el Asistente para aplicaciones local"));
	LoadStdProfileSettings(4);  // Cargar opciones de archivo INI estándar (incluidas las de la lista MRU)
	// Registrar las plantillas de documento de la aplicación. Las plantillas de documento
	//  sirven como conexión entre documentos, ventanas de marco y vistas
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
	// Llamar a DragAcceptFiles sólo si existe un sufijo
	//  En una aplicación MDI, esto debe ocurrir inmediatamente después de establecer m_pMainWnd
	// Analizar línea de comandos para comandos Shell estándar, DDE, Archivo Abrir
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Enviar comandos especificados en la línea de comandos. Devolverá FALSE si
	// la aplicación se inició con los modificadores /RegServer, /Register, /Unregserver o /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// La ventana principal se ha inicializado; mostrarla y actualizarla
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();
	return TRUE;
}



// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
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

// Comando de la aplicación para ejecutar el cuadro de diálogo
void CGestionHorarioApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// Controladores de mensaje de CGestionHorarioApp

