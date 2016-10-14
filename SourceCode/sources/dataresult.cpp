/**
 * @file   dataresult.cpp
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de la clase DataResult.
 *
 */
#include "dataresult.h"

DataResult::DataResult(){
}

void DataResult::SetLabelZ(QString string){
    labelZ=string;
}

QString DataResult::GetLabelZ(){
    return labelZ;
}

float DataResult::GetMinZValue(){
    return minZValue;
}

float DataResult::GetMaxZValue(){
    return maxZValue;
}

