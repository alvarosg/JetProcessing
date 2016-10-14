/**
 * @file   imagelistwidgetitem.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase para cada elemento de la lista del imágenes en la interfaz gráfica.
 *
 */
#ifndef IMAGELISTWIDGETITEM_H
#define IMAGELISTWIDGETITEM_H

#include <QListWidgetItem>
#include <QString>

/*! \class ImageListWidgetItem imagelistwidgetitem.h "imagelistwidgetitem.h"
 * \brief Clase abstracta para todo tipo de representaciones gráficas.
 *
 *  Clase para cada elemento de la lista del imágenes en la interfaz gráfica.
 *
 *  Contendrá esencialmente una etiqueta con el nombre de la imagen y una vista previa.
 *
 */

class ImageListWidgetItem : public QListWidgetItem{

private:
    QString label; /**< Etiqueta correspondiente a la imagen.*/
    QImage preview;/**< Vista previa correspondiente a la imagen.*/

public:
    ImageListWidgetItem(QImage preview_in,QString label_in);
    /**< Constructor del ítem a partir de los datos de la imagen pasados como parámetros.
        @param preview_in Vista previa de la imagen.
        @param label_in Etiqueta de la imagen.*/
    void GenerateTextAndIcon();
    /**< Método que genera el texto y el icono a partir de la vista previa y la etiqeuta de texto.*/
};

#endif // IMAGELISTWIDGETITEM_H
