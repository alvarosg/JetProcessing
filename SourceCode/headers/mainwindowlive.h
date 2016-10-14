/**
 * @file   mainwindowlive.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
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
 * Esta clase actúa de nexo entre el controlador principal y la clase que contiene los widgets: Ui::MainWindowLive.
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
    Ui::MainWindowLive *ui;                 /**< Puntero a la clase que contiene la interfaz gráfica. Esta clase es generada automáticamente por Qt Designer.*/
    MainController * mainController;        /**< Puntero al controlador principal.*/
    int currentGraph;                       /**< Indíca cuál de los cuatro gráficos (0-3) está visible actualmente.*/

    QTimer * refreshTimer;                  /**< Temporizador para el refresco de la imagen.*/
    float refreshTimeMs;                    /**< Tiempo de refresco entre toma y y toma de imagen.*/

    bool playing;                           /**< Indica si el modo live está funcionando.*/
    bool recording;                         /**< Indica si está grabando los datos que muestra.*/
    bool applyAlgorithms;                   /**< Si es verdadero, aplica los algoritmos de la lista a la imagen obtenida.*/
    QSemaphore * semReadyToProduce;         /**< Semáforo para que nunca se esté calculando más de un dato a la vez.*/
    QLabel * statusLabel;                   /**< Etiqueta para la barra de estado.*/
    float paramThreshold;                   /**< Parámetro para el gráfico paramétrico.*/
    Data1D * lastData1D;                    /**< Último dato unidimensional representado. Si se quiere mostrar y está a NULL, se mostrará una pantalla en blanco.*/
    Data2D * lastData2D;                    /**< Último dato bidimensional representado. Si se quiere mostrar y está a NULL, se mostrará una pantalla en blanco.*/
    bool autoUpdateAxis;                    /**< Indica si los ejes se deben actualizar automáticamente.*/
    float minZValue;                        /**< Valor mínimo de la variable independiente para los gráficos.*/
    float maxZValue;                        /**< Valor máximo de la variable independiente para los gráficos.*/
    void UpdateStatusBar(float fps=0);
    /**< Función que actualiza la barra de estado.
      @param fps Valor para las imágenes por segundo que debe mostrar.*/
    void showEvent(QShowEvent * event);
    /**< Evento que se reimplementa para poder actualizar el estado de los controles al mostrar la ventana.
      @param event Datos del evento.*/
    void closeEvent(QCloseEvent *event);
    /**< Evento que se reimplementa para saber cuando la ventana se ha cerrado y poder enviar señales y parar la toma de imágenes.
      @param event Datos del evento.*/
    void ShowSelectedGraph();
    /**< Función que se encarga de que el gráfico visible sea el que está seleccionado.*/


public slots:
    void Play();
    /**< Slot llamado en respuesta a la solicitud de comienzo de la muestra de imágenes.*/
    void Stop();
    /**< Slot llamado en respuesta a la solicitud de finalización de la muestra de imágenes.*/
    void Record();
    /**< Slot llamado en respuesta a la solicitud de comienzo o finalización del grabado de los datos.*/
    void UpdateGraph();
    /**< Slot llamado para actualizar la imagen. Es el que va conectado al temporizador. Se encargará de lanza un hilo independiente que tome y calcule los datos sin mantener la interfaz bloqueada.*/
    void PrintGraph(Data2D * data2d, bool releaseSem=true);
    /**< Slot llamado por el hilo de cálculo cuando los datos 2d se han acabado de calcular y por tanto se pueden representar. O bien en general para representar unos datos 2d en la gráfica seleccionada.
        @param data2d Datos para representar.
        @param releaseSem Si es verdadero, libera el semáforo de producción.*/
    void PrintGraph(Data1D * data1d, bool releaseSem=true);
    /**< Slot llamado por el hilo de cálculo cuando los datos 1d se han acabado de calcular y por tanto se pueden representar. O bien en general para representar unos datos 1d en la gráfica seleccionada.
        @param data1d Datos para representar.
        @param releaseSem Si es verdadero, libera el semáforo de producción.*/
    void RenewReference();
    /**< Slot llamado cuando se solicita una renovación instantánea de la referencia con la siguiente imagen tomada.*/
    void SpinFPSChanged(double value);
    /**< Slot llamado cuando el valor de las imágenes por segundo deseadas ha cambiado.
        @param value Nuevo valor para las imágenes por segundo.*/
    void UpdateControls();
    /**< Slot llamado para actualizar el estado de los botones de reproducir, parar y grabar.*/
    void UpdateAxis();
    /**< Slot llamado cada vez que se desea que se actualicen los ejes del gráfico con los datos de la siguiente imagen tomada.*/
    void RestartZAxis();
    /**< Slot llamado cada vez que se quiera reiniciar la escala del eje Z.*/
    void SpectrogramClicked();
    /**< Slot llamado cada vez que se selecciona la vista del espectrograma.*/
    void Surface3DClicked();
    /**< Slot llamado cada vez que se selecciona la vista de la superficie 3D.*/
    void ParametricClicked();
    /**< Slot llamado cada vez que se selecciona la vista paramétrica.*/
    void LineClicked();
    /**< Slot llamado cada vez que se selecciona la vist de la línea.*/
    void GraphChanged();
    /**< Slot llamado cada vez que ha cambiado el gráfico seleccionado, para actualizar los botones.*/
    void ThresholdChanged();
    /**< Slot llamado cada vez que cambia el parámetro de la representación paramétrica.*/
    void ApplyAlgorithmsChanged();
    /**< Slot llamado cada vez que cambia la opción de aplicar los algoritmos a la imagen procesada.*/
    void AutoUpdateAxisChanged();
    /**< Slot llamado cuando se modifica la casilla de autoescalado de ejes.*/

signals:
    void Closed();
    /**< Señal enviada cuando se cierra la ventana, para que la madre sepa que se ha cerrado.*/
};


/*! \class DataProducer mainwindowlive.h "mainwindowlive.h"
 * \brief Clase que realiza en segundo plano los cálculos necesarios para el modo en vivo, sin bloquear la interfaz.
 *
 */


class DataProducer: public QObject {


    Q_OBJECT
public:
    DataProducer(MainController * mainController_in=NULL,bool recording_in=false,bool applyAlgorithms_in=false);
    /**< Constructor.
      @param mainController_in Referencia al controlador principal.
      @param recording_in Si es verdadero, además de realizar los cálculos, guardará los datos.
      @param applyAlgorithms_in Si es verdadero aplicará los algoritmos despues de procesar inicialmente la imagen.*/
    ~DataProducer();
    /**< Destructor.*/

public slots:
    void ProduceData1D();
    /**< Slot que se debe llamar cuando se desee que comencemos a producir los datos 1d.*/
    void ProduceData2D();
    /**< Slot que se debe llamar cuando se desee que comencemos a producir los datos 2d.*/

private:
    MainController * mainController;    /**< Puntero al controlador principal.*/
    bool recording;                     /**< Indica si está grabando los datos que muestra.*/
    bool applyAlgorithms;               /**< Si es verdadero, aplica los algoritmos de la lista a la imagen.*/

signals:
    void Calculated2D(Data2D * data2d);
    /**< Señal para avisar al padre de que ya tenemos los datos 2d calculados.
        @param data2d Los datos calculados. Serán responsabilidad de quién recibe la señal.*/
    void Calculated1D(Data1D * data1d);
    /**< Señal para avisar al padre de que ya tenemos los datos 1d calculados.
        @param data1d Los datos calculados. Serán responsabilidad de quién recibe la señal.*/
    void Finished();
    /**< Señal para avisar al padre de hemos acabado nuestra vida útil.*/

};


#endif // MAINWINDOWLIVE_H
