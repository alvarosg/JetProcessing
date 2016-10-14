/**
 * @file   mainwindow.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase que gestiona la ventana principal del programa.
 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <qDebug>

#define MW_CAPTION          "JetProcessing"             /**< Etiqueta.*/

#define MW_FILE_MENU        "File"                      /**< Etiqueta.*/
#define MW_FILE_LOADP       "Load Project"              /**< Etiqueta.*/
#define MW_FILE_SAVEP       "Save Project"              /**< Etiqueta.*/
#define MW_FILE_SAVEPAS     "Save Project As"           /**< Etiqueta.*/
#define MW_FILE_OUTPUTF     "Select Output Folder"      /**< Etiqueta.*/
#define MW_FILE_EXIT        "Exit"                      /**< Etiqueta.*/

#define MW_REF_MENU         "Reference"                 /**< Etiqueta.*/
#define MW_REF_SET          "Change Reference"          /**< Etiqueta.*/

#define MW_IMG_MENU         "Image"                     /**< Etiqueta.*/

#define MW_IMG_MOVEUP       "Move Up"                   /**< Etiqueta.*/
#define MW_IMG_MOVEDOWN     "Move Down"                 /**< Etiqueta.*/
#define MW_IMG_DELETE       "Delete"                    /**< Etiqueta.*/
#define MW_IMG_DELETEALL    "Delete All"                /**< Etiqueta.*/
#define MW_IMG_LOAD         "Process Images from File"  /**< Etiqueta.*/
#define MW_IMG_CAM          "Process Image from Camera" /**< Etiqueta.*/

#define MW_ALG_MENU         "Algorithm"                 /**< Etiqueta.*/

#define MW_ALG_BEFORE       "Add Before"                /**< Etiqueta.*/
#define MW_ALG_AFTER        "Add After"                 /**< Etiqueta.*/
#define MW_ALG_MOVEUP       "Move Up"                   /**< Etiqueta.*/
#define MW_ALG_MOVEDOWN     "Move Down"                 /**< Etiqueta.*/
#define MW_ALG_EDIT         "Edit"                      /**< Etiqueta.*/
#define MW_ALG_DELETE       "Delete"                    /**< Etiqueta.*/

#define MW_ALG_INVERSE      "Invert"                    /**< Etiqueta.*/
#define MW_ALG_RMHTILT      "Remove Horizontal Tilt"    /**< Etiqueta.*/
#define MW_ALG_RMVTILT      "Remove Vertical Tilt"      /**< Etiqueta.*/
#define MW_ALG_SETGROUND    "Set Ground"                /**< Etiqueta.*/
#define MW_ALG_OFFSET       "Add Offset"                /**< Etiqueta.*/
#define MW_ALG_MULTIPLY     "Multiply by..."            /**< Etiqueta.*/
#define MW_ALG_SYMMETRIC    "Make Symmetric"            /**< Etiqueta.*/
#define MW_ALG_ABELINV      "Abel Inversion"            /**< Etiqueta.*/
#define MW_ALG_RECTANGLE    "Extract Rectangle"         /**< Etiqueta.*/
#define MW_ALG_GETLINE      "Extract Line"              /**< Etiqueta.*/

#define MW_ALG_SAVE       "Save Algorithm List to File" /**< Etiqueta.*/
#define MW_ALG_LOAD       "Load Algorithm List from File"/**< Etiqueta.*/

#define MW_LM_MENU          "Live Mode"                 /**< Etiqueta.*/
#define MW_LM_START         "Start Live Mode"           /**< Etiqueta.*/

#define MW_EXP_MENU         "Export Results"            /**< Etiqueta.*/
#define MW_EXP_SAVE         "Save Image"                /**< Etiqueta.*/
#define MW_EXP_SAVES        "Save Image for the Series" /**< Etiqueta.*/
#define MW_EXP_SAVED        "Save Data"                 /**< Etiqueta.*/
#define MW_EXP_SAVEDS       "Save Data for the Series"  /**< Etiqueta.*/
#define MW_EXP_SAVEVIDEO    "Save Video for the Series" /**< Etiqueta.*/
#define MW_EXP_SAVEANIM     "Save Animation for the Series"/**< Etiqueta.*/

#define MW_CONF_MENU        "Configuration"             /**< Etiqueta.*/
#define MW_CONF_CAM         "Camera Settings"           /**< Etiqueta.*/
#define MW_CONF_PREF        "Preferences"               /**< Etiqueta.*/

#define MW_TAB_SPECT        "Spectrogam"                /**< Etiqueta.*/
#define MW_TAB_2DSURF       "3d Surface"                /**< Etiqueta.*/
#define MW_TAB_PARAM        "Parametric"                /**< Etiqueta.*/
#define MW_TAB_1DDATA       "Line"                      /**< Etiqueta.*/

#define MW_LABELZ           "Label Z"                   /**< Etiqueta.*/
#define MW_DFLTLABELZ       "Phase"                     /**< Etiqueta.*/
#define MW_AUTOSCALEZ       "Auto Scale Z Axis"         /**< Etiqueta.*/
#define MW_MINZLABEL        "Min Z: "                   /**< Etiqueta.*/
#define MW_MAXZLABEL        "Max Z: "                   /**< Etiqueta.*/

#define MW_SEQUENCE         "Enable Sequence"           /**< Etiqueta.*/
#define MW_FPSLABEL         "FPS: "                     /**< Etiqueta.*/

#define MW_PARAM_THRES      "Threshold Value: "         /**< Etiqueta.*/
#define MW_LEVELCURV        "View Level Curves: "       /**< Etiqueta.*/

#define MW_HELP_MENU        "Help"                      /**< Etiqueta.*/
#define MW_HELP_HELP        "Help"                      /**< Etiqueta.*/
#define MW_HELP_ABOUT       "About..."                  /**< Etiqueta.*/

#define MW_STATUS_CAMERA    "Camera: "                  /**< Etiqueta.*/
#define MW_STATUS_CAMON     "Detected "                 /**< Etiqueta.*/
#define MW_STATUS_CAMOFF    "Not Detected "             /**< Etiqueta.*/

#define MW_STATUS_CALC      "Computation Mode: "        /**< Etiqueta.*/
#define MW_STATUS_CALCCUDA  "CUDA "                     /**< Etiqueta.*/
#define MW_STATUS_CALCCPU   "CPU "                      /**< Etiqueta.*/

#define MW_STATUS_OUTF      "Output Folder: "           /**< Etiqueta.*/

#define MW_ERROR_SAVE       "Error Saving"              /**< Etiqueta.*/
#define MW_ERROR_LOAD       "Error Loading"             /**< Etiqueta.*/
#define MW_ERROR_LABEL      "Error"                     /**< Etiqueta.*/
#define MW_FAILLOADIMAGE_LABEL  "Cannot load the image file: "/**< Etiqueta.*/
#define MW_FAILCAMIMAGE_LABEL   "Cannot take the picture"/**< Etiqueta.*/
#define MW_FAILSAVEPROJ_LABEL1   "Cannot save the project file "/**< Etiqueta.*/
#define MW_FAILSAVEPROJ_LABEL2   ". \nMake sure the current user has write permissions for that directory and the temporary folder "/**< Etiqueta.*/
#define MW_FAILLOADPROJ_LABEL1   "Cannot load the project file "/**< Etiqueta.*/
#define MW_FAILLOADPROJ_LABEL2   " completely. \nMake sure the file is not corrupt and the current user has write permissions for the temporary folder: "/**< Etiqueta.*/
#define MW_ERROR_SERIES   "There was a problem saving in the folder "/**< Etiqueta.*/

#define MW_CONFIRM          "Confirmation"              /**< Etiqueta.*/
#define MW_REMOVEIMAGES     "Click on \"Ok\" to permanently remove every image in the list"/**< Etiqueta.*/
#define MW_QUESTION         "Question"              /**< Etiqueta.*/
#define MW_CLOSEPROJECT     "Do you wanna save the current project?"/**< Etiqueta.*/
#define MW_OFDIALOGTITLE    "Select a working directory for the output files"/**< Etiqueta.*/

#define MW_ABOUT_TITLE      "About Jet Processing"/**< Etiqueta.*/
#define MW_ABOUT_BODY       "Jet Processing was developed by �lvaro S�nchez Gonz�lez \
                            \nduring spring and summer of 2012 as a final project for \
                            \nTechnical Engineering in Computer Systems\
                            \nAll Rights Reserved"      /**< Etiqueta.*/



#include "maincontroller.h"
#include "mainwindowlive.h"
#include "dialogmodeselection.h"
#include "dialogsetreference.h"
#include "dialogconfigurecam.h"
#include "dialogsetrectangle.h"
#include "dialogsetabel.h"
#include "dialoggetfloat.h"
#include "dialoggetname.h"
#include "dialogpreferences.h"
#include "dialoghelp.h"
#include "algorithmlistwidgetitem.h"
#include "imagelistwidgetitem.h"
#include "spectrogramplot.h"
#include "surface3dplot.h"
#include "parametricsurface3dplot.h"
#include "plotdata1d.h"
#include "graphicsexporter.h"

/*! \class MainWindow mainwindow.h "mainwindow.h"
 * \brief Clase para el control de la ventana principal del programa, que ser� madre del resto de ventanas y di�logos.
 *
 * Es el punto de encuentro de todas las se�ales procedentes de botones o men�s y permite acceder al resto de la funcionalidas del programa casi siempre a trav�s del controlador principal.
 *
 * Esta clase act�a de nexo entre el controlador principal y la clase que contiene los widgets: Ui::MainWindow.
 */

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent=NULL, MainController *mainController_in=NULL);
    /**< Constructor.
      @param parent Widget padre.
      @param mainController_in Referencia al controlador principal.*/
    ~MainWindow();
    /**< Destructor.*/
    
private:
    Ui::MainWindow *ui;                 /**< Puntero a la clase que contiene la interfaz gr�fica. Esta clase es generada autom�ticamente por Qt Designer.*/
    MainController * mainController;    /**< Puntero al controlador principal.*/
    QString label;                      /**< Etiqueta auxiliar.*/
    float paramThreshold;               /**< Valor para el dibujo de la grafica pram�trica.*/
    QLabel * cameraStateLabel;          /**< Etiqueta para mostrar el estado de la c�mara en la barra de estado.*/
    QLabel * modeStateLabel;            /**< Etiqueta para mostrar el modo de funcionamiento en la barra de estado.*/
    QLabel * currentPathStateLabel;     /**< Etiqueta para mostrar la ruta de salida seleccionada en la barra de estado.*/
    MainWindowLive * mainWindowLive;    /**< Puntero a la ventana del modo en vivo.*/
    bool autoUpdateZ;                   /**< Variable que indica si se deben actualizar autom�ticamente los gr�ficos al pintar nuevos datos sobre ellos.*/
    double maxZValue;                   /**< Valor m�ximo del eje Z, para el caso de que autoUpdateZ sea falso.*/
    double minZValue;                   /**< Valor m�nimo del eje Z, para el caso de que autoUpdateZ sea falso.*/
    bool viewLevelCurves;               /**< Indica si est� activada la opci�n d ever las curvas de nivel en el espectrograma.*/
    QTimer * sequenceTimer;             /**< Temporizador para el modo de secuencia de los gr�ficos.*/
    double sequenceMs;                  /**< Tiempo de duranci�n de cada gr�fico para el modo de secuencia de los gr�ficos.*/

private:
    void InitControls();
    /**< Inicia los controles durante la creaci�n.*/
    void AddAlgorithmPosition(int position);
    /**< Funci�n que se llama cuando se debe a�adir el algoritmo seleccionado en la interfaz en una posici�n.*/
    QSize AddTempImagesExporter(GraphicsExporter * graphsExporter);
    /**< Funci�n que va a�adiendo las im�genes temporales al objeto exportador.
        @param graphsExporter Puntero al objeto exportador.
        @return Tama�o con el que se han guardado las im�genes.*/
    void SetMaxMinZ();
    /**< Funci�n que establece los valores m�ximos y m�nimos para el eje Z de las gr�ficas en funci�n del modo.*/
    void closeEvent(QCloseEvent * event);
    /**< Evento reimplementado para mostrar un mensaje de confirmaci�n antes de salir del programa.
        @param event Objeto del evento.*/


public slots:
    void UpdateGraph();
    /**< Slot que actualiza el gr�fico mostrado en funci�n de la imagen y el algoritmo seleccionado.*/
    void UpdateMenusAndButtonsState();
    /**< Slot que actualiza el estado de los men�s y bot�nes en funci�n de si deben estar disponibles o no.*/
    void UpdateAlgorithmsList(bool updateGraph=false, int selectRow=-1);
    /**< Slot que actualiza la lista de algoritmos.
        @param updateGraph Verdadero para que actualice el gr�fico al finalizar.
        @param selectRow Para que seleccione la fila indicada al final de actualizar. -1 Para que conserve la misma posici�n. -2 Para que se coloque en el �ltimo elemento.*/
    void UpdatePhaseImagesList(bool updateGraph=false, int selectRow=-1);
    /**< Slot que actualiza la lista de im�genes, con sus vistas previas.
        @param updateGraph Verdadero para que actualice el gr�fico al finalizar.
        @param selectRow Para que seleccione la fila indicada al final de actualizar. -1 Para que conserve la misma posici�n. -2 Para que se coloque en el �ltimo elemento.*/
    void UpdateStatusBar();
    /**< Slot que actualiza la barra de estado.*/

    void AddAlgorithmBefore();
    /**< Slot al que se llama en respuesta a la petici�n de a�adir un algoritmo antes.*/
    void AddAlgorithmAfter();
    /**< Slot al que se llama en respuesta a la petici�n de a�adir un algoritmo despu�s.*/
    void MoveAlgorithmUp();
    /**< Slot al que se llama en respuesta a la petici�n de mover un algoritmo arriba.*/
    void MoveAlgorithmDown();
    /**< Slot al que se llama en respuesta a la petici�n de mover un algoritmo abajo.*/
    void EditAlgorithm();
    /**< Slot al que se llama en respuesta a la petici�n de editar un algoritmo.*/
    void DeleteAlgorithm();
    /**< Slot al que se llama en respuesta a la petici�n de eliminar un algoritmo.*/
    void LoadAlgorithmList();
    /**< Slot al que se llama en respuesta a la petici�n de cargar una lista de algoritmos de un fichero.*/
    void SaveAlgorithmList();
    /**< Slot al que se llama en respuesta a la petici�n de salvar una lista de algoritmos a un fichero.*/
    void EnableLevelCurves();
    /**< Slot al que se llama en al activar o desactivar la opci�n de mostrar las curvas de nivel.*/
    void ThresholdChanged();
    /**< Slot al que se llama al cambiar el par�metro del gr�fico param�trico.*/
    void ZLabelChanged();
    /**< Slot al que se llama al cambiar la etiqueta para el eje Z de la fase.*/
    void AutoScaleChanged();
    /**< Slot al que se llama al cambiar la opci�n de auto escalado.*/
    void MinZValueChanged();
    /**< Slot al que se llama al cambiar el valor m�nimo para la variable Z.*/
    void MaxZValueChanged();
    /**< Slot al que se llama al cambiar el valor m�ximo para la variable Z.*/
    void EnableSequenceChanged();
    /**< Slot al que se llama al cambiar la opci�n de vista de secuencia.*/
    void FPSSequenceChanged();
    /**< Slot al que se llama al cambiar la frecuencia con la que cambian los gr�ficos en la vista de secuencia.*/
    void SequenceTimeout();
    /**< Slot al que llama el temporizador cuando llega el momento de pasar a la siguiente imagen de la secuencia.*/
    void LoadImagesFromFile();
    /**< Slot al que se llama en respuesta a la petici�n de cargar una imagen de un fichero.*/
    void LoadImageFromCamera();
    /**< Slot al que se llama en respuesta a la petici�n de cargar una imagen de la c�mara.*/
    void MoveImageUp();
    /**< Slot al que se llama en respuesta a la petici�n de subir una imagen de la fase.*/
    void MoveImageDown();
    /**< Slot al que se llama en respuesta a la petici�n de bajar una imagen de la fase.*/
    void DeleteImage();
    /**< Slot al que se llama en respuesta a la petici�n de eliminar una imagen de la fase.*/
    void DeleteAllImage();
    /**< Slot al que se llama en respuesta a la petici�n de vaciar la lista de im�genes de la fase.*/
    void LiveMode();
    /**< Slot al que se llama en respuesta a la petici�n de comenzar el modo "En vivo".*/
    void CameraSettings();
    /**< Slot al que se llama en respuesta a la petici�n de abrir la configuraci�n de la c�mara.*/
    void ProgramPreferences();
    /**< Slot al que se llama en respuesta a la petici�n de abrir las preferencias del programa.*/
    void OpenHelp();
    /**< Slot al que se llama en respuesta a la petici�n de abrir la ayuda.*/
    void OpenAbout();
    /**< Slot al que se llama en respuesta a la petici�n de abrir el "Acerca de...".*/
    void SetReference();
    /**< Slot al que se llama en respuesta a la petici�n de cambiar la referencia.*/
    void CheckCamera();
    /**< Slot al que se llama regularmente para comprobar el estado de la c�mara.*/
    void ImageLabelEdited();
    /**< Slot al que se llama cuando se ha editado la etiqueta de una imagen en la lista de im�genes.*/
    void LoadProject();
    /**< Slot al que se llama en respuesta a la petici�n de carga de projecto completo.*/
    void SaveProject();
    /**< Slot al que se llama en respuesta a la petici�n de guardado de projecto completo.*/
    void SaveProjectAs();
    /**< Slot al que se llama en respuesta a la petici�n de guardado de projecto completo en otro fichero.*/
    void SetOutputFolder();
    /**< Slot al que se llama en respuesta a la petici�n de cambio del directorio de salida.*/
    void SaveSeriesImage();
    /**< Slot al que se llama en respuesta a la petici�n de guardado del gr�fico actual para todas las im�genes.*/
    void SaveSeriesVideo();
    /**< Slot al que se llama en respuesta a la petici�n de guardado la secuencia de im�genes en un v�deo.*/
    void SaveSeriesAnimation();
    /**< Slot al que se llama en respuesta a la petici�n de guardado la secuencia de im�genes en una animaci�n.*/
    void SaveSeriesData();
    /**< Slot al que se llama en respuesta a la petici�n de guardado de los datos para todas las im�genes.*/
    void SaveImage();
    /**< Slot al que se llama en respuesta a la petici�n de guardado del gr�fico actual para una imagen.*/
    void SaveData();
    /**< Slot al que se llama en respuesta a la petici�n de guardado de los datos para una imagen.*/


};

#endif // MAINWINDOW_H
