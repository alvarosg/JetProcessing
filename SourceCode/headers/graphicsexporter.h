/**
 * @file   graphicsexporter.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase para la exportaci�n de las im�genes de los gr�ficos.
 *
 */
#ifndef GRAPHICSEXPORTER_H
#define GRAPHICSEXPORTER_H


#include <QDir>
#include <QProcess>
#include "maincontroller.h"


/** Tipos de Gr�ficos. */
enum GraphType {
    GRAPH_SPECTROGRAM=0,  /**< Espectrograma. */
    GRAPH_3DSURFACE=1,    /**< Gr�fico 3D. */
    GRAPH_PARAMETRIC=2,   /**< Grafico 3D Param�trico. */
    GRAPH_LINE=3          /**< L�nea*/
};


/*! \class GraphicsExporter graphicsexporter.h "graphicsexporter.h"
 * \brief Clase para la exportaci�n de las im�genes de los gr�ficos.
 *
 */


class GraphicsExporter
{

private:
    MainController * mainController;    /**< Puntero al controlador principal.*/
    QString temporaryFolder;            /**< Ruta de la carpeta temporal creada que se est� utilizando.*/
    QString seriesLabel;                /**< Etiqueta para la serie actual.*/

public:
    GraphicsExporter(MainController * mainController_in);
    /**< Constructor.
      @param mainController_in Puntero al controlador principal.*/
    ~GraphicsExporter();
        /**< Destructor.*/
    QString GetNameSingleGraph(GraphType type,int imgNum, float paramThreshold=0);
    /**< Obtiene el nombre por defecto para un gr�fico con las caracter�sticas pasadas como par�metros.
      @param type Tipo de gr�fico.
      @param imgNum N�mero de imagen de la lista de la que se ha realizado el gr�fico.
      @param paramThreshold Par�metro "threshold" del gr�fico param�trico.
      @return Nombre por defecto para el gr�fico.*/
    bool SaveSingleGraphic(QString filePath,QPixmap * pixmap);
    /**< Guarda una imagen tipo QPixmap en la ruta indicada.
      @param filePath Ruta y nombre para el archivo de salida.
      @param pixmap Objeto de la imagen para guardar.
      @return Verdadero en caso de guardado con �xito.*/
    void SetSeriesLabel(QString label);
    /**< Establece la etiqueta para la serie de im�genes creando una carpeta con el nombre.
      @param label Etiqueta a establecer.*/
    bool SaveGraphicSeries(QPixmap * pixmap,GraphType type,int imgNum,float paramThreshold=0);
    /**< Guarda un gr�fico perteneciente a una serie.
      @param pixmap Objeto de la imagen para guardar.
      @param type Tipo de gr�fico.
      @param imgNum N�mero de imagen de la lista de la que se ha realizado el gr�fico.
      @param paramThreshold Par�metro "threshold" del gr�fico param�trico.
      @return Verdadero en caso de guardado con �xito.*/
    bool AddToTempFolder(QPixmap * pixmap, int imgNum);
    /**< A�ade a la carpeta temporal una imagen, para su posterior exportado como v�deo o animaci�n.
      @param pixmap Objeto de la imagen para guardar.
      @param imgNum N�mero de imagen de la lista de la que se ha realizado el gr�fico.
      @return Verdadero en caso de guardado con �xito.*/
    bool SaveTempContentAsVideo(QString filePath,int fps,QSize size);
    /**< Guarda el contenido de la carpeta temporal en un archivo de v�deo (AVI).
      @param filePath Ruta y nombre para el archivo de salida.
      @param fps Im�genes por segundo deseadas para el v�deo.
      @param size Tama�o del v�deo.
      @return Verdadero en caso de guardado con �xito.*/
    bool SaveTempContentAsAnimation(QString filePath,float delay);
    /**< Guarda el contenido de la carpeta temporal en una animaci�n (GIF).
      @param filePath Ruta y nombre para el archivo de salida.
      @param delay Tiempo en milisegundos que debe pasar entre una imagen y la siguiente.
      @return Verdadero en caso de guardado con �xito.*/
};

#endif // GRAPHICSEXPORTER_H
