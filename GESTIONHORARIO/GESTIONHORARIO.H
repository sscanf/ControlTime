// GestionHorario.h: archivo de encabezado principal para la aplicaci�n GestionHorario
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // S�mbolos principales


// CGestionHorarioApp:
// Consulte la secci�n GestionHorario.cpp para obtener informaci�n sobre la implementaci�n de esta clase
//

class CGestionHorarioApp : public CWinApp
{
public:
	CGestionHorarioApp();


// Reemplazos
public:
	virtual BOOL InitInstance();

// Implementaci�n
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGestionHorarioApp theApp;