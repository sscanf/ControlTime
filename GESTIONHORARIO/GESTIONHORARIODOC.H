// GestionHorarioDoc.h: interfaz de la clase CGestionHorarioDoc
//


#pragma once

class CGestionHorarioDoc : public CDocument
{
protected: // Crear sólo a partir de serialización
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

// Implementación
public:
	virtual ~CGestionHorarioDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Funciones de asignación de mensajes generadas
protected:
	DECLARE_MESSAGE_MAP()
};


