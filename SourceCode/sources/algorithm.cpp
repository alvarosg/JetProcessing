/**
 * @file   algorithm.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase Algorithm.
 *
 */
#include "algorithm.h"


Algorithm::Algorithm(){
    algorithm=ALG_INVERSE;
    auxInt=0;
    auxFloat=0;
    auxRect=QRect::QRect(0,0,0,0);
}

void Algorithm::SetAlgorithm(AlgorithmType value){
    algorithm=value;
}

AlgorithmType Algorithm::GetAlgorithm(){
    return algorithm;
}

void Algorithm::SetAuxInt(int value){
    auxInt=value;
}

int Algorithm::GetAuxInt(){
    return auxInt;
}

void Algorithm::SetAuxFloat(float value){
    auxFloat=value;
}

float Algorithm::GetAuxFloat(){
    return auxFloat;
}

void Algorithm::SetAuxRect(QRect value){
    auxRect=value;
}

QRect Algorithm::GetAuxRect(){
    return auxRect;
}

bool Algorithm::HasAuxParameters(){
    if(this->algorithm==ALG_OFFSET || this->algorithm==ALG_MULTIPLY || this->algorithm==ALG_ABELINV  ||
            this->algorithm==ALG_RECTANGLE || this->algorithm==ALG_GETLINE )
        return true;

    else
        return false;
}
