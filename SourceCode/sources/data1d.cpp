/**
 * @file   data1d.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase Data1D.
 *
 */
#include "data1d.h"

Data1D::Data1D(float *buffer, int sizeXin){

    //Inicio el resto de atributos a los valores por defecto
    this->SetDefaults();

    //Inicio a NULL el puntero
    linearBuffer=NULL;

    //Establezco los datos
    this->SetData(buffer,sizeXin);

}

Data1D::Data1D(const Data1D &input){
    float * buffer =NULL;
    //Creo una copia de los datos
    linearBuffer=NULL;
    if(input.linearBuffer!=NULL){
        buffer = (float *) malloc(sizeof(float)*(input.sizeX));
        memcpy(buffer,input.linearBuffer,sizeof(float)*(input.sizeX));
    }
    this->SetData(buffer,(input.sizeX));

    //Asigno el resto de parámetros
    this->SetLabelZ(input.labelZ);
    this->SetLabelX(input.labelX);
    this->SetMinXValue(input.minXValue);
    this->SetMaxXValue(input.maxXValue);
    this->SetInitXPoint(input.initXPoint);
    this->SetInitYPoint(input.initYPoint);
    this->SetEndXPoint(input.endXPoint);
    this->SetEndYPoint(input.endYPoint);
}

Data1D::~Data1D(){
    free(linearBuffer);
}


void Data1D::SetData(float * buffer, int sizeXin){

    //Si tenía algo antes, lo libero
    if(linearBuffer!=NULL)
        free(linearBuffer);

    //Si no han metido nada, lo dejamos vacío
    if(buffer==NULL){
        sizeX=0;
        linearBuffer=NULL;
        return;
    }

    //Cojo el tamaño de la original
    sizeX=sizeXin;

    //Asigno el buffer lineal
    linearBuffer=buffer;

    //Busco el máximo y el mínimo
    this->CalculateMaxMinZ();
    this->CalculateRatios();

}

float * Data1D::GetBuffer(){
    return linearBuffer;
}

int Data1D::GetSizeX(){
    return sizeX;
}

void Data1D::SetLabelX(QString string){
    labelX=string;
}

QString Data1D::GetLabelX(){
    return labelX;
}

void Data1D::SetMaxXValue(float value){
    maxXValue=value;
    this->CalculateRatios();
}

float Data1D::GetMaxXValue(){
    return maxXValue;
}

void Data1D::SetMinXValue(float value){
    minXValue=value;
    this->CalculateRatios();
}

float Data1D::GetMinXValue(){
    return minXValue;
}

float Data1D::GetRatioPixelValueX(){
    return ratioPixelValueX;
}

void Data1D::SetDefaults(){
    labelZ="UnknownZ";

    labelX="UnknownX";
    maxXValue=10;
    minXValue=-10;

    initXPoint=0;
    initYPoint=0;
    endXPoint=0;
    endYPoint=0;

    this->CalculateRatios();
}

float Data1D::GetInitXPoint(){
    return initXPoint;
}

void Data1D::SetInitXPoint(float value){
    initXPoint=value;
}

float Data1D::GetInitYPoint(){
    return initYPoint;
}

void Data1D::SetInitYPoint(float value){
    initYPoint=value;
}

float Data1D::GetEndXPoint(){
    return endXPoint;
}

void Data1D::SetEndXPoint(float value){
    endXPoint=value;
}

float Data1D::GetEndYPoint(){
    return endYPoint;
}

void Data1D::SetEndYPoint(float value){
    endYPoint=value;
}

void Data1D::CalculateRatios(){
    sizeX!=0 ? ratioPixelValueX=(maxXValue-minXValue)/(sizeX-1) : (maxXValue-minXValue);
}

void Data1D::CalculateMaxMinZ(){
    int x;
    float aux;
    maxZValue=-6000000;
    minZValue=6000000;
    for(x=0;x<sizeX;x++){
        aux=linearBuffer[x];
        if(maxZValue<aux)
            maxZValue=aux;
        if(minZValue>aux)
            minZValue=aux;
    }

}

bool Data1D::SaveToFile(QString filePath){
    int x;
    QFile file(filePath);
    if(linearBuffer==NULL)
        return false;

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
         return false;

    QTextStream out(&file);

    out << scientific;
    for(x=0;x<sizeX;x++){
        out << minXValue+x*(maxXValue-minXValue)/(sizeX-1) << "\t"<< linearBuffer[x] << endl;
    }
    return true;
}
