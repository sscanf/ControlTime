// ChildFrm.cpp: implementaci�n de la clase CChildFrame
//
#include "stdafx.h"
#include "GestionHorario.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()


// Construcci�n o destrucci�n de CChildFrame

CChildFrame::CChildFrame()
{
	// TODO: agregar aqu� el c�digo de inicializaci�n adicional de miembros
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: modificar aqu� la clase Window o los estilos cambiando CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// Diagn�sticos de CChildFrame

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// Controladores de mensaje de CChildFrame