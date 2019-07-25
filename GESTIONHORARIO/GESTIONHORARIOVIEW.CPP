// GestionHorarioView.cpp: implementación de la clase CGestionHorarioView
//

#include "stdafx.h"
#include "GestionHorario.h"

#include "GestionHorarioDoc.h"
#include "GestionHorarioView.h"
#include "SQLDirect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define NUM_COLUMNS	5

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] = 
{
	_T("Hora Entrada"),			//1
	_T("Hora Salida"),			//2
	_T("Tipo Horario"),			//3
	_T ("Hora Apertura"),		//4
	_T ("Total")				//5
};

static int _gnColumnFmt[NUM_COLUMNS] = 
{
	LVCFMT_LEFT,		//Hora Entrada
	LVCFMT_LEFT,		//Hora Salida
	LVCFMT_LEFT,		//Tipo Horario
	LVCFMT_LEFT,		//Hora Apertura		
	LVCFMT_LEFT			//Total
};

static int _gnColumnWitdh[NUM_COLUMNS] =
{
	75,	//Hora Entrada
	75,	//Hora Salida
	75,	//Tipo Horario
	75,	//Hora Apertura
	75		//Hora Apertura
};


// CGestionHorarioView

IMPLEMENT_DYNCREATE(CGestionHorarioView, CFormView)

BEGIN_MESSAGE_MAP(CGestionHorarioView, CFormView)
	ON_BN_CLICKED(IDC_CONSULTAR, OnBnClickedConsultar)
END_MESSAGE_MAP()

// Construcción o destrucción de CGestionHorarioView

CGestionHorarioView::CGestionHorarioView()
	: CFormView(CGestionHorarioView::IDD)
	,m_fecha(COleDateTime::GetCurrentTime())
{
}

CGestionHorarioView::~CGestionHorarioView()
{
}

void CGestionHorarioView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OPERARIO, m_operarioID);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_fecha);
	DDX_Control(pDX, IDC_LIST2, m_resultado);
	DDX_Control(pDX, IDC_TOTALHORAS, m_TotalHoras);
}

BOOL CGestionHorarioView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: modificar aquí la clase Window o los estilos cambiando
	//  CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CGestionHorarioView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (int n=0;n<NUM_COLUMNS;n++)
	{
		lvc.iSubItem=n;
		lvc.pszText = _gszColumnLabel[n];
		lvc.cx = _gnColumnWitdh[n];
		lvc.fmt= _gnColumnFmt[n];
		m_resultado.InsertColumn(n,&lvc);
	}
}


// Diagnósticos de CGestionHorarioView

#ifdef _DEBUG
void CGestionHorarioView::AssertValid() const
{
	CFormView::AssertValid();
}

void CGestionHorarioView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CGestionHorarioDoc* CGestionHorarioView::GetDocument() const // La versión de no depuración es en línea
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGestionHorarioDoc)));
	return (CGestionHorarioDoc*)m_pDocument;
}
#endif //_DEBUG


// Controladores de mensaje de CGestionHorarioView

void CGestionHorarioView::OnBnClickedConsultar()
{
	CString strComandoSQL;
	LV_ITEM		lvi;

	int nItem=0;
	CString str,strFecha;

	m_resultado.DeleteAllItems();
	UpdateData (TRUE);

	strFecha = m_fecha.Format ("%Y-%m-%d");

	CSQLDirect SQLDirect("ebsl_dbo","oscar","optr2c");
	strComandoSQL.Format ("SELECT time(HoraEntrada) as HoraEntrada,time(HoraSalida) as HoraSalida,TipoHorario as TipoHorario,time(HoraApertura) as HoraApertura, timediff(HoraSalida,HoraEntrada) as total  FROM ControlHorario WHERE date(Fecha)='%s' and operarioid='%s'",strFecha,m_operarioID);

		if (SQLDirect.ExecuteSQL (strComandoSQL)==SQL_SUCCESS)
		{
			while (SQLDirect.Fetch()==SQL_SUCCESS)
			{
				/*************** Hora Entrada*****************************/
		
				int nSubItem=1;
				str=SQLDirect.GetCol("HoraEntrada");
				m_resultado.InsertItem (nItem,str);

				/*************** Hora Salida*****************************/
		
				str=SQLDirect.GetCol("HoraSalida");
				m_resultado.SetItemText (nItem,nSubItem++,str);

				/*************** Tipo Horario*****************************/
				str=SQLDirect.GetCol("TipoHorario");
				m_resultado.SetItemText (nItem,nSubItem++,str);

				/*************** Hora Apertura*****************************/
		
				str=SQLDirect.GetCol("HoraApertura");
				m_resultado.SetItemText(nItem,nSubItem++,str);

				/*************** Total *****************************/
		
				str=SQLDirect.GetCol("total");
				m_resultado.SetItemText(nItem,nSubItem++,str);

				nItem++;
			}
		}

		strComandoSQL.Format ("SELECT time(HoraEntrada) as HoraEntrada,time(HoraSalida) as HoraSalida,TipoHorario as TipoHorario,time(HoraApertura) as HoraApertura, timediff(HoraSalida,HoraEntrada) as total  FROM ControlHorario WHERE date(Fecha)='%s' and operarioid='%s'",strFecha,m_operarioID);

		if (SQLDirect.ExecuteSQL (strComandoSQL)==SQL_SUCCESS)
		{
			while (SQLDirect.Fetch()==SQL_SUCCESS)
			{
			}
		}
}
