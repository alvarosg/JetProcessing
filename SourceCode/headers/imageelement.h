/**
 * @file   imageelement.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Fichero que define la clase ImageElement. Clase entidad que guarda los datos de una imagen de la fase.
 *
 */
#ifndef IMAGEEELEMENT_H
#define IMAGEEELEMENT_H

#include <QList>
#include <QString>
#include <phaseimage.h>
#include <phaseline.h>
#include <spectrogramplot.h>
#include <data2d.h>
#include <data1d.h>

/*! \class ImageElement imageelement.h "imageelement.h"
 * \brief Clase que contiene la información acerca de una imagen de la fase, de las guardadas en el programa.
 *
 * Contiene datos de la fase antes y después de aplicar los algoritmos. Es una clase entidad.
 *
 *
 */

class ImageElement{

private:
    QString label;                  /**< Etiqueta asociada a la imagen.*/
    PhaseImage * phaseImageSource;  /**< Imagen de la fase original, recién calculada y restada la referencia, sin aplicar ningun algoritmo.*/
    PhaseImage * phaseImage;        /**< Última imagen de la fase que haya sido procesada a partir de la original con los algoritmos.*/
    PhaseLine * phaseLine;          /**< Última línea extraída de la imagen de la fase que haya sido procesada a partir de la original con los algoritmos.*/
    Data1D * data1d;                /**< Datos 1D correspondientes al atributo phaseLine.*/
    Data2D * data2d;                /**< Datos 2D correspondientes al atributo phaseImaege.*/
    QImage * preview;               /**< Vista pevia de phaseImageSource.*/
    bool updated;                   /**< Atributo que indica si los valores en phaseImage y phaseLine estan actualizados, es decir, prodecen de aplicar la lista completa del algoritmos actual a la imagen de origen.*/
    float pixelsMmX;                /**< Relación de pixeles por milímetro en el eje horizontal para la imagen.*/
    float pixelsMmY;                /**< Relación de pixeles por milímetro en el eje vertical para la imagen.*/
    float pixelsFromOriginX;        /**< Distancia en horizontal desde la esquina superior izquierda de la imagen hasta el origen de coordenadas fijado por el usuario.*/
    float pixelsFromOriginY;        /**< Distancia en vertical desde la esquina superior izquierda de la imagen hasta el origen de coordenadas fijado por el usuario.*/

public:
    ImageElement(PhaseImage * source_in=NULL,QString label = "",float pixelsMmX_in=1,float pixelsMmY_in=1,float pixelsFromOriginX_in=0,float pixelsFromOriginY_in=0);
    /**< Constructor de la imagen.
        @param source_in Imagen de la fase antes de aplicar algoritmos. El objeto creado toma la responsabilidad de este puntero.
        @param label Etiqueta para la imagen.
        @param pixelsMmX_in Valor para iniciar el atributo pixelsMmX.
        @param pixelsMmY_in Valor para iniciar el atributo pixelsMmY.
        @param pixelsFromOriginX_in Valor para iniciar el atributo pixelsFromOriginX.
        @param pixelsFromOriginY_in Valor para iniciar el atributo pixelsFromOriginY.*/
    ~ImageElement();
    /**< Destructor. Nótese que no libera los punteros, porque a veces interesará tener copias de este tipo de objetos apuntando a las mismas estructuras. Cuando se deseen liberar deberá llamarse explícitamente al método FreePointers().*/
    QString GetLabel();
    /**< Devuelve la etiqueta de la imagen.
      @return Etiqueta de la imagen.*/
    PhaseImage * GetPhaseImageSource();
    /**< Devuelve el puntero a la imagen de origen. Debe usarse sólo para lectura.
      @return Puntero a la imagen de origen. Puede devolver NULL.*/
    PhaseImage * GetPhaseImage();
    /**< Devuelve el puntero a la imagen procesada. Debe usarse sólo para lectura.
      @return Puntero a la imagen procesada. Puede devolver NULL.*/
    PhaseLine * GetPhaseLine();
    /**< Devuelve el puntero a la línea procesada. Debe usarse sólo para lectura.
      @return Puntero a la línea procesada. Puede devolver NULL.*/
    Data2D * GetData2D();
    /**< Devuelve el puntero a los datos 2d de la imagen procesada. Debe usarse sólo para lectura.
      @return Puntero a los datos 2d de la imagen procesada. Puede devolver NULL.*/
    Data1D * GetData1D();
    /**< Devuelve el puntero a los datos 1d de la línea procesada. Debe usarse sólo para lectura.
      @return Puntero a los datos 1d línea procesada. Puede devolver NULL.*/
    QImage GetPreview();
    /**< Devuelve, generandola si es necesario, la vista previa.
      @return Vista previa.*/
    bool GetUpdated();
    /**< Devuelve el valor del atributo updated.
      @return Atributo updated.*/

    float GetPixelsMmX();
    /**< Devuelve el valor del atributo pixelsMmX.
      @return Atributo pixelsMmX.*/
    float GetPixelsMmY();
    /**< Devuelve el valor del atributo pixelsMmY.
      @return Atributo pixelsMmY.*/
    int GetPixelsFromOriginX();
    /**< Devuelve el valor del atributo pixelsFromOriginX.
      @return Atributo pixelsFromOriginX.*/
    int GetPixelsFromOriginY();
    /**< Devuelve el valor del atributo pixelsFromOriginY.
      @return Atributo pixelsFromOriginY.*/

    void SetLabel(QString value);
    /**< Establece la etiqueta de la imagen.
        @param value Etiqueta a establecer.*/
    void SetPhaseImage(PhaseImage * value);
    /**< Establece la imagen de la fase procesada. La propiedad del puntero pasa a ser del objeto tipo ImageElement. Automáticamente actualiza data2d.
        @param value Imagen a establecer.*/
    void SetPhaseLine(PhaseLine * value);
    /**< Establece la imagen de la línea procesada. La propiedad del puntero pasa a ser del objeto tipo ImageElement. Automáticamente actualiza data1d.
        @param value Línea a establecer.*/
    void SetPreview(QImage value);
    /**< Establece una vista previa personalizada por el usuario.
        @param value Imagen de la vista previa.*/
    void SetUpdated(bool value);
    /**< Establece el estado de actualización.
        @param value Nuevo estado de actualización.*/

    void FreePointers();
    /**< Libera los diferentes objetos a los que apuntan los atributos.*/
};

#endif // IMAGEEELEMENT_H
