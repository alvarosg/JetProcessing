/**
 * @file   phaseimage.cpp
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de la clase PhaseImage.
 *
 */
#include "phaseimage.h"

PhaseImage::PhaseImage(float * buffer, int sizeX_in,int sizeY_in){
    sizeX=sizeX_in;
    sizeY=sizeY_in;
    linearBuffer=buffer;
}

PhaseImage::~PhaseImage(){
}

int PhaseImage::GetSizeX(){
    return sizeX;
}

int PhaseImage::GetSizeY(){
    return sizeY;
}
