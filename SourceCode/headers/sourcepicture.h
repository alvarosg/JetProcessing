/**
 * @file   sourcepicture.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase que representa la imagen de origen, antes de todo el procesado.
 *
 */
#ifndef SOURCEPICTURE_H
#define SOURCEPICTURE_H

#include <QImage>

/*! \class SourcePicture  sourcepicture.h "sourcepicture.h"
 * \brief Clase que representa la imagen de origen, antes de todo el procesado.
 *
 *
 *  Es uan clase derivada de QImage, con �n �nico constructor, que fuerza a que el objeto lleve el formato QImage::Format_RGB32.
 *
 */

class SourcePicture : public QImage
{
public:
    SourcePicture(uchar * data, int width, int height);
    /**< Constructor.
        @param data Puntero a un buffer con la imagen, en formato RGB32, con los p�xeles alineados fila tras fila. Este puntero pasa a ser propiedad del objeto creado.
        @param width Anchura en p�xeles de la imagen.
        @param height Altura en p�xeles de la imagen.*/
    ~SourcePicture();
    /**< Destructor.*/
    
private:
    uchar * buffer;     /**< Puntero al buffer que se introdujo en la creaci�n.*/
};

#endif // SOURCEPICTURE_H
