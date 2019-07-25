// GestionHorario.h: archivo de encabezado principal para la aplicación GestionHorario
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // Símbolos principales


// CGestionHorarioApp:
// Consulte la sección GestionHorario.cpp para obtener información sobre la implementación de esta clase
//

class CGestionHorarioApp : public CWinApp
{
public:
	CGestionHorarioApp();


// Reemplazos
public:
	virtual BOOL InitInstance();

// Implementación
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGestionHorarioApp theApp;
