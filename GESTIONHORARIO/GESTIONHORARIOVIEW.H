// GestionHorarioView.h: interfaz de la clase CGestionHorarioView
//


#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"


class CGestionHorarioView : public CFormView
{
protected: // Crear s�lo a partir de serializaci�n
	CGestionHorarioView();
	DECLARE_DYNCREATE(CGestionHorarioView)

public:
	enum{ IDD = IDD_GESTIONHORARIO_FORM };

// Atributos
public:
	CGestionHorarioDoc* GetDocument() const;

// Operaciones
public:

// Reemplazos
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
	virtual void OnInitialUpdate(); // Se llama la primera vez despu�s de la construcci�n

// Implementaci�n
public:
	virtual ~CGestionHorarioView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Funciones de asignaci�n de mensajes generadas
protected:
	DECLARE_MESSAGE_MAP()
private:
	COleDateTime m_fecha;
public:
	CListCtrl m_resultado;
	CStatic m_TotalHoras;
	afx_msg void OnBnClickedConsultar();
	CString m_operarioID;
};

#ifndef _DEBUG  // Versi�n de depuraci�n en GestionHorarioView.cpp
inline CGestionHorarioDoc* CGestionHorarioView::GetDocument() const
   { return reinterpret_cast<CGestionHorarioDoc*>(m_pDocument); }
#endif
