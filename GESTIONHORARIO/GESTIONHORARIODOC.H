// GestionHorarioDoc.h: interfaz de la clase CGestionHorarioDoc
//


#pragma once

class CGestionHorarioDoc : public CDocument
{
protected: // Crear s�lo a partir de serializaci�n
	CGestionHorarioDoc();
	DECLARE_DYNCREATE(CGestionHorarioDoc)

// Atributos
public:

// Operaciones
public:

// Reemplazos
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementaci�n
public:
	virtual ~CGestionHorarioDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Funciones de asignaci�n de mensajes generadas
protected:
	DECLARE_MESSAGE_MAP()
};

