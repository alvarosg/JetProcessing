/**
 * @file   sourcepicture.cpp
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de la clase SourcePicture.
 *
 */
#include "sourcepicture.h"

SourcePicture::SourcePicture(uchar * data, int width, int height):
    QImage(data,width,height,QImage::Format_RGB32){

    buffer=data;
}

SourcePicture::~SourcePicture(){
    free(buffer);
}
