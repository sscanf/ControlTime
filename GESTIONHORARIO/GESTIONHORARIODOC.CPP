// GestionHorarioDoc.cpp: implementación de la clase CGestionHorarioDoc
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


// Construcción o destrucción de CGestionHorarioDoc

CGestionHorarioDoc::CGestionHorarioDoc()
{
	// TODO: agregar aquí el código de construcción único

}

CGestionHorarioDoc::~CGestionHorarioDoc()
{
}

BOOL CGestionHorarioDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: agregar aquí código de reinicio
	// (los documentos SDI volverán a utilizar este documento)

	return TRUE;
}




// Serialización de CGestionHorarioDoc

void CGestionHorarioDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: agregar aquí el código de almacenamiento
	}
	else
	{
		// TODO: agregar aquí el código de carga
	}
}


// Diagnósticos de CGestionHorarioDoc

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
