/**
 * @file   data2d.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase Data2D.
 *
 */
#include "data2d.h"

Data2D::Data2D(float *buffer, int sizeXin, int sizeYin){

    //Inicio el resto de atributos a los valores por defecto
    this->SetDefaults();

    //Inicio a NULL los punteros de memoria
    linearBuffer=NULL;
    dbptBuffer=NULL;

    //Establezco los datos
    this->SetData(buffer,sizeXin,sizeYin);
}

Data2D::Data2D(const Data2D &input){

    float * buffer=NULL;

    //Creo una copia de los datos
    linearBuffer=NULL;
    dbptBuffer=NULL;
    if(input.linearBuffer!=NULL){
        buffer = (float *) malloc(sizeof(float)*(input.sizeX)*(input.sizeY));
        memcpy(buffer,input.linearBuffer,sizeof(float)*(input.sizeX)*(input.sizeY));
    }
    this->SetData(buffer,(input.sizeX),(input.sizeY));

    //Asigno el resto de parámetros
    this->SetLabelZ(input.labelZ);
    this->SetLabelX(input.labelX);
    this->SetMinXValue(input.minXValue);
    this->SetMaxXValue(input.maxXValue);

    this->SetLabelY(input.labelY);
    this->SetMinYValue(input.minYValue);
    this->SetMaxYValue(input.maxYValue);
}

Data2D::~Data2D(){
    free(linearBuffer);
    free(dbptBuffer);
}


void Data2D::SetData(float * buffer, int sizeXin, int sizeYin){
    int i;



    //Si tenía algo antes, lo libero
    if(linearBuffer!=NULL)
        free(linearBuffer);
    if(dbptBuffer!=NULL)
        free(dbptBuffer);

    //Si no han metido nada, lo dejamos vacío
    if(buffer==NULL){
        sizeX=0;
        sizeY=0;
        dbptBuffer=NULL;
        linearBuffer=NULL;
        return;
    }

    //Cojo el tamaño de la original
    sizeX=sizeXin;
    sizeY=sizeYin;

    //Asigno el buffer lineal
    linearBuffer=buffer;

    //Reservo memoria para la matriz de puntero a punteros, para poder trabajar con índices,
    //manteniendo la ventaja de tener la imagen en solamente un área de memoria

    dbptBuffer=(float **) malloc(sizeof( float *) * sizeY);
    for(i=0;i<sizeY;i++){
        dbptBuffer[i]=&(linearBuffer[i*sizeX]);
    }


    this->CalculateMaxMinZ();
    this->CalculateRatios();
}

float ** Data2D::GetBuffer(){
    return dbptBuffer;
}

int Data2D::GetSizeX(){
    return sizeX;
}

int Data2D::GetSizeY(){
    return sizeY;
}

void Data2D::SetLabelX(QString string){
    labelX=string;
}

QString Data2D::GetLabelX(){
    return labelX;
}

void Data2D::SetMaxXValue(float value){
    maxXValue=value;
    this->CalculateRatios();
}

float Data2D::GetMaxXValue(){
    return maxXValue;
}

void Data2D::SetMinXValue(float value){
    minXValue=value;
    this->CalculateRatios();
}

float Data2D::GetMinXValue(){
    return minXValue;
}

float Data2D::GetRatioPixelValueX(){
    return ratioPixelValueX;
}

void Data2D::SetLabelY(QString string){
    labelY=string;
}

QString Data2D::GetLabelY(){
    return labelY;
}

void Data2D::SetMaxYValue(float value){
    maxYValue=value;
    this->CalculateRatios();
}

float Data2D::GetMaxYValue(){
    return maxYValue;
}

void Data2D::SetMinYValue(float value){
    minYValue=value;
    this->CalculateRatios();
}

float Data2D::GetMinYValue(){
    return minYValue;
}

float Data2D::GetRatioPixelValueY(){
    return ratioPixelValueY;
}


void Data2D::SetDefaults(){
    labelZ="UnknownZ";

    labelX="UnknownX";
    maxXValue=10;
    minXValue=-10;


    labelY="UnknownY";
    maxYValue=10;
    minYValue=-10;

    this->CalculateRatios();


}

void Data2D::CalculateRatios(){
    sizeX!=0 ? ratioPixelValueX=(maxXValue-minXValue)/(sizeX-1) : (maxXValue-minXValue);
    sizeY!=0 ? ratioPixelValueY=(maxYValue-minYValue)/(sizeY-1) : (maxYValue-minYValue);
}

void Data2D::CalculateMaxMinZ(){
    int x,y;
    float aux;
    maxZValue=-6000000;
    minZValue=6000000;
    for(y=0;y<sizeY;y++){
        for(x=0;x<sizeX;x++){
            aux=dbptBuffer[y][x];
            if(maxZValue<aux)
                maxZValue=aux;
            if(minZValue>aux)
                minZValue=aux;
        }
    }
}

bool Data2D::SaveToFile(QString filePath){
    int x,y;
    if(dbptBuffer==NULL)
        return false;
    QFile file(filePath);

     if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
         return false;

    QTextStream out(&file);

    out << scientific;
    for(y=0;y<sizeY;y++){
        for(x=0;x<sizeX;x++){
            out << minXValue+x*(maxXValue-minXValue)/(sizeX-1) << "\t"<< minYValue+y*(maxYValue-minYValue)/(sizeY-1) << "\t"<< dbptBuffer[y][x] << endl;
        }
    }
    return true;
}
#include <qdebug>
Data1D * Data2D::GetData1D(QRect points){
    int x1=points.left();
    int x2=points.right();
    int y1=points.top();
    int y2=points.bottom();

    if(x1<0) x1=0;
    if(y1<0) y1=0;
    if(x2<0) x2=0;
    if(y2<0) y2=0;
    if(x1>=sizeX) x1=sizeX-1;
    if(y1>=sizeY) y1=sizeY-1;
    if(x2>=sizeX) x2=sizeX-1;
    if(y2>=sizeY) y2=sizeY-1;

    //Obtengo y asigno los datos
    int outputSize=sqrt((float)((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)))+1;

    float * outputBuffer;
    if(outputSize>1)
        outputBuffer=GetPhaseLineCPU(linearBuffer,sizeX,sizeY,x1,y1,x2,y2,outputSize);
    else{
        outputBuffer=NULL;
        outputSize=0;
    }
    Data1D * data1d = new Data1D(outputBuffer,outputSize);


    //Calculo el resto de atributos
    float initXValue=minXValue+x1*(maxXValue-minXValue)/sizeX;
    float endXValue=minXValue+x2*(maxXValue-minXValue)/sizeX;
    float initYValue=minYValue+(sizeY-y1)*(maxYValue-minYValue)/sizeY;
    float endYValue=minYValue+(sizeY-y2)*(maxYValue-minYValue)/sizeY;
    data1d->SetInitXPoint(initXValue);
    data1d->SetEndXPoint(endXValue);
    data1d->SetInitYPoint(initYValue);
    data1d->SetEndYPoint(endYValue);

    float length=sqrt((endXValue-initXValue)*(endXValue-initXValue)+(endYValue-initYValue)*(endYValue-initYValue));

    data1d->SetLabelX(labelX);
    data1d->SetLabelZ(labelZ);
    data1d->SetMinXValue(-length/2);
    data1d->SetMaxXValue(length/2);

    return data1d;
}
