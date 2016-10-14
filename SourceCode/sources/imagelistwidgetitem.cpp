/**
 * @file   imagelistwidgetitem.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase ImageListWidgetItem.
 *
 */
#include "imagelistwidgetitem.h"

ImageListWidgetItem::ImageListWidgetItem(QImage preview_in,QString label_in) :
    QListWidgetItem(0){

    preview=preview_in;
    label=label_in;
    this->GenerateTextAndIcon();
}

void ImageListWidgetItem::GenerateTextAndIcon(){
    QIcon icon(QPixmap::fromImage(preview));

    this->setText(label);
    this->setIcon(icon);
}
