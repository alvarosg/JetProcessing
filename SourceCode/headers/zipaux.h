/**
 * @file   zipaux.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase que facilita la compresión de un grupo de ficheros en un fichero .zip y viceversa, utilizando la biblioteca quazip.
 *
 */
#ifndef ZIPAUX_H
#define ZIPAUX_H

#include <QDir>
#include <QFile>

#include <quazip.h>
#include <quazipfile.h>



/*! \class ZipAux zipaux.h "zipaux.h"
 * \brief Clase que facilita la compresión de descompresión de una carpeta en un fichero .zip.
 *
 *  Utiliza la biblioteca multiplataforma para Qt Quazip.
 *
 */
class ZipAux
{
public:
    static bool compressFolder(QString destinationZip, QString sourceFolder);
    /**< Método que crea un fichero comprimido .zip a partir del contenido de una carpeta pasada como argumento.
            @param destinationZip Ruta y nombre del fichero comprimido a generar.
            @param sourceFolder Ruta de la carpeta cuyo contenido se quiere comprimir.
            @return Verdadero en caso de éxito.*/

    static bool uncompressFolder(QString sourceZip, QString destinationFolder);
    /**< Método que descomprime el contenido de un fichero .zip en un directorio pasado como argumento.
            @param sourceZip Ruta y nombre del fichero comprimido a descomprimir.
            @param destinationFolder Ruta de la carpeta donde se debe extraer.
            @return Verdadero en caso de éxito.*/
};

#endif // ZIPAUX_H
