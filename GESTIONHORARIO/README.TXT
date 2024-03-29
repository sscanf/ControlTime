================================================================================
    BIBLIOTECA DE MICROSOFT FOUNDATION CLASS: GestionHorario Informaci�n general del proyecto
===============================================================================

El Asistente para aplicaciones ha creado esta GestionHorario aplicaci�n para usted. Esta 
aplicaci�n no s�lo muestra los conceptos b�sicos para utilizar Microsoft Foundation Classes,
sino que tambi�n es un punto de partida para escribir su aplicaci�n.

Este archivo contiene un resumen de lo que encontrar� en cada uno de los archivos que
componen su GestionHorario aplicaci�n.

GestionHorario.vcproj
    Este es el archivo de proyecto principal para proyectos de VC++ generados con un Asistente 
    para aplicaciones. Contiene informaci�n sobre la versi�n de Visual C++ que gener� el archivo e 
    informaci�n acerca de las plataformas, las configuraciones y las caracter�sticas del proyecto
    seleccionadas con el Asistente para aplicaciones.

GestionHorario.h
    Este es el archivo de encabezado principal para la aplicaci�n. Incluye otros
    encabezados espec�ficos del proyecto (incluyendo Resource.h) y declara la
    clase de la aplicaci�n CGestionHorarioApp.

GestionHorario.cpp
    Este es el archivo de c�digo fuente principal de la aplicaci�n que contiene la
    clase de la aplicaci�n CGestionHorarioApp.

GestionHorario.rc
    Esta es una lista de todos los recursos de Microsoft Windows que utiliza
    el programa. Incluye los iconos, mapas de bits y cursores almacenados en el
    subdirectorio RES. Este archivo se puede editar directamente en Microsoft
    Visual C++. Los recursos del proyecto est�n en 3082.

res\GestionHorario.ico
    Este es un archivo de icono, que se utiliza como icono de la aplicaci�n. El
    icono est� incluido en el archivo de recursos principal GestionHorario.rc.

res\GestionHorario.rc2
    Este archivo contiene recursos no editados por Microsoft 
    Visual C++. Debe ubicar todos los recursos no editables por
    el editor de recursos en este archivo.
/////////////////////////////////////////////////////////////////////////////

Para la ventana de marco principal:
    El proyecto incluye una interfaz de MFC est�ndar.
MainFrm.h, MainFrm.cpp
    Estos archivos contienen la clase de marco CMainFrame, que deriva de
    CMDIFrameWnd y controla todas las funciones de marco MDI.
res\Toolbar.bmp
    Este archivo de mapa de bits se utiliza para crear im�genes en mosaico para la barra de herramientas.
    La barra de herramientas y la barra de estado iniciales se construyen en la clase CMainFrame.
    Edite el mapa de bits de esta barra de herramientas con el editor de recursos y
    actualice la matriz IDR_MAINFRAME TOOLBAR en GestionHorario.rc para agregar
    botones de la barra de herramientas.
/////////////////////////////////////////////////////////////////////////////

Para la ventana de marco secundaria:

ChildFrm.h, ChildFrm.cpp
    Estos archivos definen e implementan la clase CChildFrame, que
    admite ventanas secundarias en una aplicaci�n MDI.

/////////////////////////////////////////////////////////////////////////////

El Asistente para aplicaciones crea un tipo de documento y una vista:

GestionHorarioDoc.h, GestionHorarioDoc.cpp - documento
    Estos archivos contienen la clase CGestionHorarioDoc. Edite estos archivos para
    agregar sus datos de documentos especiales e implementar la carga y guardado de archivos
    (via CGestionHorarioDoc::Serialize).
GestionHorarioView.h, GestionHorarioView.cpp - vista del documento
    Estos archivos contienen la clase CGestionHorarioView.
    Los objetos CGestionHorarioView se utilizan para ver objetos CGestionHorarioDoc.
res\GestionHorarioDoc.ico
    Este es un archivo de icono, que se utiliza como icono para las ventanas secundarias MDI
    en la clase CGestionHorarioDoc. Este icono lo incluye el archivo de 
    recursos principal GestionHorario.rc.
/////////////////////////////////////////////////////////////////////////////

Otras caracter�sticas:

Controles ActiveX
    La aplicaci�n admite el uso de los controles ActiveX.

Compatibilidad con bases de datos
    El Asistente para aplicaciones agreg� el nivel b�sico de compatibilidad con bases de datos para el programa. 
    S�lo se han incluido los archivos necesarios.
/////////////////////////////////////////////////////////////////////////////
Otros archivos est�ndar:

StdAfx.h, StdAfx.cpp
    Estos archivos se utilizan para compilar un archivo de encabezado precompilado (PCH)
    denominado GestionHorario.pch y un archivo de tipos precompilados denominado StdAfx.obj.

Resource.h
    Este es el archivo de encabezado est�ndar, que define nuevos identificadores de recursos.
    Microsoft Visual C++ lee y actualiza este archivo.

/////////////////////////////////////////////////////////////////////////////

Otras notas:

El Asistente para aplicaciones utiliza "ToDo:" para indicar las partes del c�digo fuente que
debe agregar o personalizar.

Si su aplicaci�n utiliza MFC en un archivo DLL compartido y la aplicaci�n est� en un
idioma distinto al idioma actual del sistema operativo, ser� necesario copiar 
los recursos MFC70XXX.DLL localizados correspondientes del CD-ROM de Microsoft
Visual C++ bajo el directorio Win\System en el directorio del sistema o 
system32 de su equipo y cambiar el nombre a MFCLOC.DLL. ("XXX" corresponde a la 
abreviatura del idioma. Por ejemplo, MFC70DEU.DLL contiene recursos traducidos
al alem�n.) Si no realiza esta operaci�n, algunos de los elementos de la interfaz de usuario de
su aplicaci�n permanecer�n en el idioma del sistema operativo.

/////////////////////////////////////////////////////////////////////////////
