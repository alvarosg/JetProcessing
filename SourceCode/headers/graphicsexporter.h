/**
 * @file   graphicsexporter.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase para la exportación de las imágenes de los gráficos.
 *
 */
#ifndef GRAPHICSEXPORTER_H
#define GRAPHICSEXPORTER_H


#include <QDir>
#include <QProcess>
#include "maincontroller.h"


/** Tipos de Gráficos. */
enum GraphType {
    GRAPH_SPECTROGRAM=0,  /**< Espectrograma. */
    GRAPH_3DSURFACE=1,    /**< Gráfico 3D. */
    GRAPH_PARAMETRIC=2,   /**< Grafico 3D Paramétrico. */
    GRAPH_LINE=3          /**< Línea*/
};


/*! \class GraphicsExporter graphicsexporter.h "graphicsexporter.h"
 * \brief Clase para la exportación de las imágenes de los gráficos.
 *
 */


class GraphicsExporter
{

private:
    MainController * mainController;    /**< Puntero al controlador principal.*/
    QString temporaryFolder;            /**< Ruta de la carpeta temporal creada que se está utilizando.*/
    QString seriesLabel;                /**< Etiqueta para la serie actual.*/

public:
    GraphicsExporter(MainController * mainController_in);
    /**< Constructor.
      @param mainController_in Puntero al controlador principal.*/
    ~GraphicsExporter();
        /**< Destructor.*/
    QString GetNameSingleGraph(GraphType type,int imgNum, float paramThreshold=0);
    /**< Obtiene el nombre por defecto para un gráfico con las características pasadas como parámetros.
      @param type Tipo de gráfico.
      @param imgNum Número de imagen de la lista de la que se ha realizado el gráfico.
      @param paramThreshold Parámetro "threshold" del gráfico paramétrico.
      @return Nombre por defecto para el gráfico.*/
    bool SaveSingleGraphic(QString filePath,QPixmap * pixmap);
    /**< Guarda una imagen tipo QPixmap en la ruta indicada.
      @param filePath Ruta y nombre para el archivo de salida.
      @param pixmap Objeto de la imagen para guardar.
      @return Verdadero en caso de guardado con éxito.*/
    void SetSeriesLabel(QString label);
    /**< Establece la etiqueta para la serie de imágenes creando una carpeta con el nombre.
      @param label Etiqueta a establecer.*/
    bool SaveGraphicSeries(QPixmap * pixmap,GraphType type,int imgNum,float paramThreshold=0);
    /**< Guarda un gráfico perteneciente a una serie.
      @param pixmap Objeto de la imagen para guardar.
      @param type Tipo de gráfico.
      @param imgNum Número de imagen de la lista de la que se ha realizado el gráfico.
      @param paramThreshold Parámetro "threshold" del gráfico paramétrico.
      @return Verdadero en caso de guardado con éxito.*/
    bool AddToTempFolder(QPixmap * pixmap, int imgNum);
    /**< Añade a la carpeta temporal una imagen, para su posterior exportado como vídeo o animación.
      @param pixmap Objeto de la imagen para guardar.
      @param imgNum Número de imagen de la lista de la que se ha realizado el gráfico.
      @return Verdadero en caso de guardado con éxito.*/
    bool SaveTempContentAsVideo(QString filePath,int fps,QSize size);
    /**< Guarda el contenido de la carpeta temporal en un archivo de vídeo (AVI).
      @param filePath Ruta y nombre para el archivo de salida.
      @param fps Imágenes por segundo deseadas para el vídeo.
      @param size Tamaño del vídeo.
      @return Verdadero en caso de guardado con éxito.*/
    bool SaveTempContentAsAnimation(QString filePath,float delay);
    /**< Guarda el contenido de la carpeta temporal en una animación (GIF).
      @param filePath Ruta y nombre para el archivo de salida.
      @param delay Tiempo en milisegundos que debe pasar entre una imagen y la siguiente.
      @return Verdadero en caso de guardado con éxito.*/
};

#endif // GRAPHICSEXPORTER_H
