/**
 * @file   phaseimagecuda.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase PhaseImageCUDA.
 *
 */
#include "phaseimagecuda.h"

PhaseImageCUDA::PhaseImageCUDA(float *deviceBuffer, int sizeX_in, int sizeY_in):PhaseImage(deviceBuffer,sizeX_in,sizeY_in){

}

PhaseImageCUDA::PhaseImageCUDA(const PhaseImageCUDA & input):PhaseImage(NULL,0,0){
    //Copio los datos y hago una copia del buffer
    this->sizeX=input.sizeX;
    this->sizeY=input.sizeY;
    this->linearBuffer=MemCpyCUDA(input.linearBuffer,sizeX,sizeY);
}

PhaseImageCUDA::~PhaseImageCUDA(){
    if(linearBuffer!=NULL)
        FreeCUDA(linearBuffer);
}

void PhaseImageCUDA::LoadFromMemoryData(float * buffer, int sizeX_in, int sizeY_in){
    if(linearBuffer!=NULL)
        FreeCUDA(linearBuffer);

    sizeX=sizeX_in;
    sizeY=sizeY_in;

    linearBuffer=LoadInGPUCUDA(buffer,sizeX,sizeY);
}

float * PhaseImageCUDA::GetMemoryBuffer(){
    if(linearBuffer==NULL){
        return NULL;
    }
    float * phaseHost=(float *) malloc(sizeof(float)*sizeX*sizeY);

    //Recupero los datos de la GPU
    GetBufferBackCUDA(phaseHost, linearBuffer,sizeX,sizeY);

    return phaseHost;

}


void PhaseImageCUDA::ScanPhase(){

    ScanPhaseCUDA(linearBuffer,sizeX,sizeY);
}


QImage * PhaseImageCUDA::GetAsQImage(int gain){

    float * phaseHost;

    //Reservo memoria para un buffer intermedio, donde hacer cuentas
    phaseHost = (float *) malloc (sizeof(float)*sizeX*sizeY);

    //Recupero los datos de la GPU
    GetBufferBackCUDA(phaseHost, linearBuffer,sizeX,sizeY);

    //Redimensiono los datos para que los valores vayan entre 0 y 1
    int i,j;
    float max=-60000,min=60000;

    for (i=0;i<sizeY;i++){
        for (j=0;j<sizeX;j++){
            if(phaseHost[j+i*sizeX]>max){
                max=phaseHost[j+i*sizeX];
            }
            if(phaseHost[j+i*sizeX]<min){
                min=phaseHost[j+i*sizeX];
            }

        }
    }

    for (i=0;i<sizeY;i++){
        for (j=0;j<sizeX;j++){
            phaseHost[j+i*sizeX]=((phaseHost[j+i*sizeX]-min)*255*gain)/(max-min);
            if(phaseHost[j+i*sizeX]>255){
                phaseHost[j+i*sizeX]=255;
            }
        }
    }

    QImage * imageGray=new QImage(sizeX, sizeY, QImage::Format_Indexed8);
    //Inicio la tabla de colores en escala de grises

    QVector<QRgb> colors;
    for(i=0;i<256;i++){
        colors << qRgb(  i,   i,   i);
    }
    imageGray->setColorTable(colors);

    int gray;
    for(i=0;i<sizeY;i++){
        for(j=0;j<sizeX;j++){
            gray=floor(phaseHost[j+i*sizeX]);
            if (gray>255) gray=255;
            if (gray<0) gray=0;
            if(gray<=255 && gray >=0)
                imageGray->setPixel(j,i,gray);
        }
    }

    free(phaseHost);

    return imageGray;

}


Data2D * PhaseImageCUDA::GetData2D(Data2D * data2dIn){

    float * phaseHost;
    Data2D * data2d;

    //Reservo memoria para el buffer que voy a dejar para el nuevo objeto
    phaseHost = (float *) malloc (sizeof(float)*sizeX*sizeY);

    GetBufferBackCUDA(phaseHost, linearBuffer,sizeX,sizeY);

    //Si no me han pasado ningún objeto, creo el objeto, y le asigno la zona de memoria
    if(data2dIn==NULL){
        data2d=new Data2D(phaseHost,sizeX,sizeY);
        return data2d;
    }
    //Si sí me han pasado un objeto, lo modifico
    else{
        data2dIn->SetData(phaseHost,sizeX,sizeY);
        return data2dIn;
    }
}


void PhaseImageCUDA::SubtractPhase(PhaseImage *offset_in){

    PhaseImageCUDA * offset=(PhaseImageCUDA* )offset_in;
    //Si la fase de entrada es más pequeña, no hacemos nada, pues si no violaríamos memoria
    if(offset->sizeX < this->sizeX || offset->sizeY < this->sizeY)
        return;

    float * offsetPt=offset->linearBuffer;
    SubtractCUDA(linearBuffer,offsetPt,sizeX,sizeY);
}

void PhaseImageCUDA::AdditiveInverse(){
    AdditiveInverseCUDA(linearBuffer,sizeX,sizeY);
}

void PhaseImageCUDA::RemoveHorizontalLinearTilt(){
    RemoveHorizontalLinearTiltCUDA(linearBuffer,sizeX,sizeY);
}

void PhaseImageCUDA::RemoveVerticalLinearTilt(){
    RemoveVerticalLinearTiltCUDA(linearBuffer,sizeX,sizeY);
}

void PhaseImageCUDA::SetGround(){
    SetGroundCUDA(linearBuffer,sizeX,sizeY);
}

void PhaseImageCUDA::OffsetAndMultiply(float offset,float factor){
    OffsetAndMultiplyCUDA(linearBuffer,sizeX,sizeY,offset,factor);
}

void PhaseImageCUDA::MakeSymmetric(){
    MakeSymmetricCUDA(linearBuffer,sizeX,sizeY);
}

void PhaseImageCUDA::AbelInversion(float dx,int neighbours){
    if(neighbours<1)
        neighbours=1;
    if(dx==0)
        dx=1;
    AbelInversionCUDA(linearBuffer,sizeX,sizeY,dx,neighbours);
}

void PhaseImageCUDA::CropRectangle(QRect rect){
    int x1=rect.left();
    int x2=rect.right();
    int y1=rect.top();
    int y2=rect.bottom();

    if(x1<0) x1=0;
    if(y1<0) y1=0;
    if(x2>=sizeX) x2=sizeX-1;
    if(y2>=sizeY) y2=sizeY-1;
    if(x1>=x2 || y1>=y2)
        return;

    float * deviceBuffer;

    deviceBuffer = CropRectangleCUDA(linearBuffer,sizeX,sizeY,x1,y1,x2,y2);

    FreeCUDA(linearBuffer);
    linearBuffer=deviceBuffer;
    sizeX=(x2-x1+1);
    sizeY=(y2-y1+1);
}

PhaseLine * PhaseImageCUDA::GetPhaseLine(QRect points){
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

    PhaseLine* phase;
    //La dimensión en pixeles de la salida, mantendrá la proporción
    int outputSize=sqrt((float)((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)))+1;

    float * deviceBuffer;

    deviceBuffer = GetPhaseLineCUDA(linearBuffer,sizeX,sizeY,x1,y1,x2,y2,outputSize);

    phase = new PhaseLineCUDA(deviceBuffer,outputSize);
    return phase;

}
