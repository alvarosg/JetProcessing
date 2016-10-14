/**
 * @file   imagelistwidgetitem.cpp
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de la clase ImageListWidgetItem.
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
