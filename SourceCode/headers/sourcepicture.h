/**
 * @file   sourcepicture.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
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
 *  Es uan clase derivada de QImage, con ún único constructor, que fuerza a que el objeto lleve el formato QImage::Format_RGB32.
 *
 */

class SourcePicture : public QImage
{
public:
    SourcePicture(uchar * data, int width, int height);
    /**< Constructor.
        @param data Puntero a un buffer con la imagen, en formato RGB32, con los píxeles alineados fila tras fila. Este puntero pasa a ser propiedad del objeto creado.
        @param width Anchura en píxeles de la imagen.
        @param height Altura en píxeles de la imagen.*/
    ~SourcePicture();
    /**< Destructor.*/
    
private:
    uchar * buffer;     /**< Puntero al buffer que se introdujo en la creación.*/
};

#endif // SOURCEPICTURE_H
