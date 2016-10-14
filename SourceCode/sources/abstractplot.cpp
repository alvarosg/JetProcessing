/**
 * @file   abstractplot.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de una clase abstracta para las representaciones gráficas.
 *
 */
#include "abstractplot.h"

AbstractPlot::AbstractPlot(){
    init=false;
    minZValueCustom=1;
    maxZValueCustom=-1;
    minZValueData=-1;
    maxZValueData=1;

    //Inicialmente apuntan a los valores según los datos
    minZValue=&minZValueData;
    maxZValue=&maxZValueData;
}

AbstractPlot::~AbstractPlot(){
}



float AbstractPlot::GetMinZValue(){
    return *minZValue;
}


float AbstractPlot::GetMaxZValue(){
    return *maxZValue;
}

void AbstractPlot::SetMinZValue(float value){
    bool update=false;
    if(value!=*minZValue)
        update=true;
    minZValueCustom=value;

    if(minZValueCustom>maxZValueCustom){
        minZValue=&minZValueData;
        maxZValue=&maxZValueData;
    }
    else{
        minZValue=&minZValueCustom;
        maxZValue=&maxZValueCustom;
    }
    if(update==true)
        this->Update();
}

void AbstractPlot::SetMaxZValue(float value){
    bool update=false;
    if(value!=*maxZValue)
        update=true;
    maxZValueCustom=value;

    if(minZValueCustom>=maxZValueCustom){
        minZValue=&minZValueData;
        maxZValue=&maxZValueData;
    }
    else{
        minZValue=&minZValueCustom;
        maxZValue=&maxZValueCustom;
    }
    if(update==true)
        this->Update();
}

void AbstractPlot::SetMinMaxZValue(float minValue,float maxValue){
    bool update=false;
    if(minValue!=*minZValue || maxValue!=*maxZValue)
        update=true;

    maxZValueCustom=maxValue;
    minZValueCustom=minValue;
    if(minZValueCustom>=maxZValueCustom){
        minZValue=&minZValueData;
        maxZValue=&maxZValueData;
    }
    else{
        minZValue=&minZValueCustom;
        maxZValue=&maxZValueCustom;
    }
    if(update==true)
        this->Update();
}

void AbstractPlot::SetInit(bool value){
    init=value;
}

bool AbstractPlot::GetInit(){
    return init;
}
