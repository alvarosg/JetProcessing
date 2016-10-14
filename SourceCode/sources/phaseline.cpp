/**
 * @file   phaseline.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase PhaseLine.
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
