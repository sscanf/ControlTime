// GestionHorarioDoc.cpp: implementaci�n de la clase CGestionHorarioDoc
//

#include "stdafx.h"
#include "GestionHorario.h"

#include "GestionHorarioDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGestionHorarioDoc

IMPLEMENT_DYNCREATE(CGestionHorarioDoc, CDocument)

BEGIN_MESSAGE_MAP(CGestionHorarioDoc, CDocument)
END_MESSAGE_MAP()


// Construcci�n o destrucci�n de CGestionHorarioDoc

CGestionHorarioDoc::CGestionHorarioDoc()
{
	// TODO: agregar aqu� el c�digo de construcci�n �nico

}

CGestionHorarioDoc::~CGestionHorarioDoc()
{
}

BOOL CGestionHorarioDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: agregar aqu� c�digo de reinicio
	// (los documentos SDI volver�n a utilizar este documento)

	return TRUE;
}




// Serializaci�n de CGestionHorarioDoc

void CGestionHorarioDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: agregar aqu� el c�digo de almacenamiento
	}
	else
	{
		// TODO: agregar aqu� el c�digo de carga
	}
}


// Diagn�sticos de CGestionHorarioDoc

#ifdef _DEBUG
void CGestionHorarioDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGestionHorarioDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Comandos de CGestionHorarioDoc
