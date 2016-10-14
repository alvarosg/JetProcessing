/**
 * @file   fullimage.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase FullImage.
 *
 */
#include "fullimage.h"

FullImage::FullImage(QImage * source,int power2SizeX,int power2SizeY){
    int x,y;
    //Cojo el tamaño de la original
    sizeX=NextPowerOf2(power2SizeX);
    sizeY=NextPowerOf2(power2SizeY);


    if(source!=NULL){
        //Reservo memoria para el buffer lineal
        linearBuffer=(complex *) malloc(sizeof(complex)*sizeX*sizeY);


        //Inicio la imagen en el buffer lineal
        for(y=0;y<sizeY;y++){
            for(x=0;x<sizeX;x++){
                if(x < source->width() && y< source->height())
                    //Inicio la parte real con el valor en gris
                    linearBuffer[x+y*sizeX].x=qGray(source->pixel(x,y));
                else
                    linearBuffer[x+y*sizeX].x=0;
                //Y la parte imaginaria con 0
                linearBuffer[x+y*sizeX].y=0;
            }
        }
    }
}


FullImage::~FullImage(){
}

int FullImage::GetSizeX(){
    return sizeX;
}

int FullImage::GetSizeY(){
    return sizeY;
}

int FullImage::NextPowerOf2(int value){
    int power=1;
    while(power<value){
        power=power*2;
    }
    return power;
}
