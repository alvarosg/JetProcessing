/**
 * @file   mainwindowlive.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase que gestiona la interfaz del modo "en vivo".
 *
 */
#ifndef MAINWINDOWLIVE_H
#define MAINWINDOWLIVE_H

#include <QMainWindow>
#include <QSemaphore>
#include "maincontroller.h"
#include "spectrogramplot.h"
#include "surface3dplot.h"
#include "parametricsurface3dplot.h"
#include "plotdata1d.h"

#define MWL_CAPTION       "Live Capture"                /**< Etiqueta.*/

#define MWL_STOPPED_LABEL "Stopped"                     /**< Etiqueta.*/
#define MWL_PLAYING_LABEL "Playing at "                 /**< Etiqueta.*/
#define MWL_RECORDING_LABEL "Recording at "             /**< Etiqueta.*/

#define MWL_SPECTROGRAM   "Spectrogam"                  /**< Etiqueta.*/
#define MWL_3DSURFACE     "3d Surface"                  /**< Etiqueta.*/
#define MWL_PARAMETRIC    "Parametric"                  /**< Etiqueta.*/
#define MWL_LINE          "Line"                        /**< Etiqueta.*/

#define MWL_APPLYALG      "Apply Algorithms"            /**< Etiqueta.*/
#define MWL_AUTOUPDATEZ   "Auto Update Axes"            /**< Etiqueta.*/
#define MWL_UPDATEAXIS    "Restart Axes Range Values"   /**< Etiqueta.*/
#define MWL_RENEWREFERENCE "Renew Reference"            /**< Etiqueta.*/
#define MWL_FPSLABEL      "FPS: "                       /**< Etiqueta.*/
#define MWL_PARAM_THRES   "Threshold Value: "           /**< Etiqueta.*/

/*! \class MainWindowLive mainwindowlive.h "mainwindowlive.h"
 * \brief Clase para el control de la ventana del modo "en vivo" del programa.
 *
 *
 * Esta clase act�a de nexo entre el controlador principal y la clase que contiene los widgets: Ui::MainWindowLive.
 */

namespace Ui {
class MainWindowLive;
}


class MainWindowLive : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindowLive(QWidget *parent,MainController * mainController_in);
    /**< Constructor.
      @param parent Widget padre.
      @param mainController_in Referencia al controlador principal.*/
    /**< Destructor.*/
    ~MainWindowLive();
    
private:
    Ui::MainWindowLive *ui;                 /**< Puntero a la clase que contiene la interfaz gr�fica. Esta clase es generada autom�ticamente por Qt Designer.*/
    MainController * mainController;        /**< Puntero al controlador principal.*/
    int currentGraph;                       /**< Ind�ca cu�l de los cuatro gr�ficos (0-3) est� visible actualmente.*/

    QTimer * refreshTimer;                  /**< Temporizador para el refresco de la imagen.*/
    float refreshTimeMs;                    /**< Tiempo de refresco entre toma y y toma de imagen.*/

    bool playing;                           /**< Indica si el modo live est� funcionando.*/
    bool recording;                         /**< Indica si est� grabando los datos que muestra.*/
    bool applyAlgorithms;                   /**< Si es verdadero, aplica los algoritmos de la lista a la imagen obtenida.*/
    QSemaphore * semReadyToProduce;         /**< Sem�foro para que nunca se est� calculando m�s de un dato a la vez.*/
    QLabel * statusLabel;                   /**< Etiqueta para la barra de estado.*/
    float paramThreshold;                   /**< Par�metro para el gr�fico param�trico.*/
    Data1D * lastData1D;                    /**< �ltimo dato unidimensional representado. Si se quiere mostrar y est� a NULL, se mostrar� una pantalla en blanco.*/
    Data2D * lastData2D;                    /**< �ltimo dato bidimensional representado. Si se quiere mostrar y est� a NULL, se mostrar� una pantalla en blanco.*/
    bool autoUpdateAxis;                    /**< Indica si los ejes se deben actualizar autom�ticamente.*/
    float minZValue;                        /**< Valor m�nimo de la variable independiente para los gr�ficos.*/
    float maxZValue;                        /**< Valor m�ximo de la variable independiente para los gr�ficos.*/
    void UpdateStatusBar(float fps=0);
    /**< Funci�n que actualiza la barra de estado.
      @param fps Valor para las im�genes por segundo que debe mostrar.*/
    void showEvent(QShowEvent * event);
    /**< Evento que se reimplementa para poder actualizar el estado de los controles al mostrar la ventana.
      @param event Datos del evento.*/
    void closeEvent(QCloseEvent *event);
    /**< Evento que se reimplementa para saber cuando la ventana se ha cerrado y poder enviar se�ales y parar la toma de im�genes.
      @param event Datos del evento.*/
    void ShowSelectedGraph();
    /**< Funci�n que se encarga de que el gr�fico visible sea el que est� seleccionado.*/


public slots:
    void Play();
    /**< Slot llamado en respuesta a la solicitud de comienzo de la muestra de im�genes.*/
    void Stop();
    /**< Slot llamado en respuesta a la solicitud de finalizaci�n de la muestra de im�genes.*/
    void Record();
    /**< Slot llamado en respuesta a la solicitud de comienzo o finalizaci�n del grabado de los datos.*/
    void UpdateGraph();
    /**< Slot llamado para actualizar la imagen. Es el que va conectado al temporizador. Se encargar� de lanza un hilo independiente que tome y calcule los datos sin mantener la interfaz bloqueada.*/
    void PrintGraph(Data2D * data2d, bool releaseSem=true);
    /**< Slot llamado por el hilo de c�lculo cuando los datos 2d se han acabado de calcular y por tanto se pueden representar. O bien en general para representar unos datos 2d en la gr�fica seleccionada.
        @param data2d Datos para representar.
        @param releaseSem Si es verdadero, libera el sem�foro de producci�n.*/
    void PrintGraph(Data1D * data1d, bool releaseSem=true);
    /**< Slot llamado por el hilo de c�lculo cuando los datos 1d se han acabado de calcular y por tanto se pueden representar. O bien en general para representar unos datos 1d en la gr�fica seleccionada.
        @param data1d Datos para representar.
        @param releaseSem Si es verdadero, libera el sem�foro de producci�n.*/
    void RenewReference();
    /**< Slot llamado cuando se solicita una renovaci�n instant�nea de la referencia con la siguiente imagen tomada.*/
    void SpinFPSChanged(double value);
    /**< Slot llamado cuando el valor de las im�genes por segundo deseadas ha cambiado.
        @param value Nuevo valor para las im�genes por segundo.*/
    void UpdateControls();
    /**< Slot llamado para actualizar el estado de los botones de reproducir, parar y grabar.*/
    void UpdateAxis();
    /**< Slot llamado cada vez que se desea que se actualicen los ejes del gr�fico con los datos de la siguiente imagen tomada.*/
    void RestartZAxis();
    /**< Slot llamado cada vez que se quiera reiniciar la escala del eje Z.*/
    void SpectrogramClicked();
    /**< Slot llamado cada vez que se selecciona la vista del espectrograma.*/
    void Surface3DClicked();
    /**< Slot llamado cada vez que se selecciona la vista de la superficie 3D.*/
    void ParametricClicked();
    /**< Slot llamado cada vez que se selecciona la vista param�trica.*/
    void LineClicked();
    /**< Slot llamado cada vez que se selecciona la vist de la l�nea.*/
    void GraphChanged();
    /**< Slot llamado cada vez que ha cambiado el gr�fico seleccionado, para actualizar los botones.*/
    void ThresholdChanged();
    /**< Slot llamado cada vez que cambia el par�metro de la representaci�n param�trica.*/
    void ApplyAlgorithmsChanged();
    /**< Slot llamado cada vez que cambia la opci�n de aplicar los algoritmos a la imagen procesada.*/
    void AutoUpdateAxisChanged();
    /**< Slot llamado cuando se modifica la casilla de autoescalado de ejes.*/

signals:
    void Closed();
    /**< Se�al enviada cuando se cierra la ventana, para que la madre sepa que se ha cerrado.*/
};


/*! \class DataProducer mainwindowlive.h "mainwindowlive.h"
 * \brief Clase que realiza en segundo plano los c�lculos necesarios para el modo en vivo, sin bloquear la interfaz.
 *
 */


class DataProducer: public QObject {


    Q_OBJECT
public:
    DataProducer(MainController * mainController_in=NULL,bool recording_in=false,bool applyAlgorithms_in=false);
    /**< Constructor.
      @param mainController_in Referencia al controlador principal.
      @param recording_in Si es verdadero, adem�s de realizar los c�lculos, guardar� los datos.
      @param applyAlgorithms_in Si es verdadero aplicar� los algoritmos despues de procesar inicialmente la imagen.*/
    ~DataProducer();
    /**< Destructor.*/

public slots:
    void ProduceData1D();
    /**< Slot que se debe llamar cuando se desee que comencemos a producir los datos 1d.*/
    void ProduceData2D();
    /**< Slot que se debe llamar cuando se desee que comencemos a producir los datos 2d.*/

private:
    MainController * mainController;    /**< Puntero al controlador principal.*/
    bool recording;                     /**< Indica si est� grabando los datos que muestra.*/
    bool applyAlgorithms;               /**< Si es verdadero, aplica los algoritmos de la lista a la imagen.*/

signals:
    void Calculated2D(Data2D * data2d);
    /**< Se�al para avisar al padre de que ya tenemos los datos 2d calculados.
        @param data2d Los datos calculados. Ser�n responsabilidad de qui�n recibe la se�al.*/
    void Calculated1D(Data1D * data1d);
    /**< Se�al para avisar al padre de que ya tenemos los datos 1d calculados.
        @param data1d Los datos calculados. Ser�n responsabilidad de qui�n recibe la se�al.*/
    void Finished();
    /**< Se�al para avisar al padre de hemos acabado nuestra vida �til.*/

};


#endif // MAINWINDOWLIVE_H
