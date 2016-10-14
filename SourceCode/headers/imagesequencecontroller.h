/**
 * @file   imagesequencecontroller.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Fichero que define la clase que controla y gestiona la lista de imagenes de la fase guardadas en el programa.
 *
 */
#ifndef IMAGESEQUENCECONTROLLER_H
#define IMAGESEQUENCECONTROLLER_H

#include <QList>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QDomElement>
#include <stdio.h>
#include <stdlib.h>
#include "imageelement.h"
#include "abstractimagefactory.h"

/*! \class ImageSequenceController imagesequencecontroller.h "imagesequencecontroller.h"
 * \brief Clase que controla y conoce una lista de imágenes de la fase.
 *
 *  Se encargará de comprobar la integridad de los nombres de las imágenes asi como de devolver una imagen que se le solicite.
 *
 */

class ImageSequenceController : public QList <ImageElement>
{
public:
    ImageSequenceController();
    /**< Constructor.*/

    void AppendImageElement(PhaseImage * source_in=NULL,QString label = "",float pixelsMmX_in=1,float pixelsMmY_in=1,float pixelsFromOriginX_in=0,float pixelsFromOriginY_in=0);
    /**< Añade una imagen al final de la lista.
        @param source_in Imagen de la fase antes de aplicar algoritmos. El objeto creado toma la responsabilidad de este puntero.
        @param label Etiqueta para la imagen.
        @param pixelsMmX_in Valor para iniciar el atributo pixelsMmX.
        @param pixelsMmY_in Valor para iniciar el atributo pixelsMmY.
        @param pixelsFromOriginX_in Valor para iniciar el atributo pixelsFromOriginX.
        @param pixelsFromOriginY_in Valor para iniciar el atributo pixelsFromOriginY.*/
    QString GetLabelIndex(int index);
    /**< Devuelve la etiqueta de la imagen en la posición indicada.
      @param index Índice de la imagen.
      @return Etiqueta de la imagen.*/
    PhaseImage * GetPhaseImageSourceIndex(int index);
    /**< Devuelve el puntero a la imagen de origen de la imagen en la posición indicada. Debe usarse sólo para lectura.
      @param index Índice de la imagen.
      @return Puntero a la imagen de origen. Puede devolver NULL.*/
    PhaseImage * GetPhaseImageIndex(int index);
    /**< Devuelve el puntero a la imagen procesada de la imagen en la posición indicada. Debe usarse sólo para lectura.
      @param index Índice de la imagen.
      @return Puntero a la imagen procesada. Puede devolver NULL.*/
    PhaseLine * GetPhaseLineIndex(int index);
    /**< Devuelve el puntero a la línea procesada de la imagen en la posición indicada. Debe usarse sólo para lectura.
      @param index Índice de la imagen.
      @return Puntero a la línea procesada. Puede devolver NULL.*/
    Data2D * GetData2DIndex(int index);
    /**< Devuelve el puntero a los datos 2d de la imagen procesada de la imagen en la posición indicada. Debe usarse sólo para lectura.
      @param index Índice de la imagen.
      @return Puntero a los datos 2d de la imagen procesada. Puede devolver NULL.*/
    Data1D * GetData1DIndex(int index);
    /**< Devuelve el puntero a los datos 1d de la línea procesada de la imagen en la posición indicada. Debe usarse sólo para lectura.
      @param index Índice de la imagen.
      @return Puntero a los datos 1d línea procesada. Puede devolver NULL.*/
    QImage GetPreviewIndex(int index);
    /**< Devuelve, generándola si es necesario, la vista previa de la imagen en la posición indicada.
      @param index Índice de la imagen.
      @return Vista previa.*/
    bool GetUpdatedIndex(int index);
    /**< Devuelve el valor del atributo updated de la imagen en la posición indicada.
      @param index Índice de la imagen.
      @return Atributo updated.*/

    float GetPixelsMmXIndex(int index);
    /**< Devuelve el valor del atributo pixelsMmX de la imagen en la posición indicada.
      @param index Índice de la imagen.
      @return Atributo pixelsMmX.*/
    float GetPixelsMmYIndex(int index);
    /**< Devuelve el valor del atributo pixelsMmY de la imagen en la posición indicada.
      @param index Índice de la imagen.
      @return Atributo pixelsMmY.*/
    int GetPixelsFromOriginXIndex(int index);
    /**< Devuelve el valor del atributo pixelsFromOriginX de la imagen en la posición indicada.
      @param index Índice de la imagen.
      @return Atributo pixelsFromOriginX.*/
    int GetPixelsFromOriginYIndex(int index);
    /**< Devuelve el valor del atributo pixelsFromOriginY de la imagen en la posición indicada.
      @param index Índice de la imagen.
      @return Atributo pixelsFromOriginY.*/

    void SetLabelIndex(int index,QString value);
    /**< Establece la etiqueta de la imagen en la posición indicada.
      @param index Índice de la imagen.
      @param value Etiqueta a establecer.*/
    void SetPhaseImageIndex(int index,PhaseImage * value);
    /**< Establece la imagen de la fase procesada en la posición indicada. El cliente pierde la propiedad del puntero. Automáticamente actualiza data2d.
      @param index Índice de la imagen.
      @param value Imagen a establecer.*/
    void SetPhaseLineIndex(int index,PhaseLine * value);
    /**< Establece la imagen de la línea procesada en la posición indicada. El cliente pierde la propiedad del puntero. Automáticamente actualiza data1d.
      @param index Índice de la imagen.
      @param value Línea a establecer.*/
    void SetPreviewIndex(int index,QImage value);
    /**< Establece una vista previa personalizada por el usuario en la posición indicada.
      @param index Índice de la imagen.
      @param value Imagen de la vista previa.*/
    void SetUpdatedIndex(int index,bool value);
    /**< Establece el estado de actualización de la imagen en la posición indicada.
      @param index Índice de la imagen.
      @param value Nuevo estado de actualización.*/

    void FreePointersIndex(int index);
    /**< Libera los diferentes objetos a los que apuntan los atributos de la imagen en la posición indicada.
      @param index Índice de la imagen.*/

    void CheckLabels();
    /**< Comprueba las etiquetas de la lista de imágenes, modificándolas para que nunca haya dos iguales.*/

    bool LoadImageList(QString filePath, AbstractImageFactory * imageFactory);
    /**< Carga un fichero .dat con la secuencia de imágenes de la ruta indicada, eliminando la lista anterior.
      @param filePath Ruta y nombre del fichero para cargar.
      @param imageFactory Puntero a la factoría que se deba usar para cargar las imágenes.
      @return Verdadero si se cargó correctamente.*/
    bool SaveImageList(QString filePath);
    /**< Guarda un fichero .dat con la secuencia de imágenes de la ruta indicada, eliminando la lista anterior.
      @param filePath Ruta y nombre del fichero para guardar.
      @return Verdadero si se guardó correctamente.*/
};
#endif // IMAGESEQUENCECONTROLLER_H
