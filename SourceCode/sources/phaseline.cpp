/**
 * @file   phaseline.cpp
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de la clase PhaseLine.
 *
 */
#include"phaseline.h"


PhaseLine::PhaseLine(float * buffer, int size_in){
    size=size_in;
    linearBuffer=buffer;
}

PhaseLine::~PhaseLine(){
}

int PhaseLine::GetSize(){
    return size;
}
