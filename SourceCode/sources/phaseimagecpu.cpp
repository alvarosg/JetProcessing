/**
 * @file   phaseimagecpu.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase PhaseImageCPU.
 *
 */
#include "phaseimagecpu.h"


PhaseImageCPU::PhaseImageCPU(float * buffer, int sizeX_in, int sizeY_in):PhaseImage(buffer,sizeX_in,sizeY_in){

    int y;
    //Reservo memoria para la matriz de puntero a punteros, para poder trabajar con Indices,
    //manteniendo la ventaja de tener la imagen en solamente un área de memoria
    dbptBuffer=(float **) malloc(sizeof( float * )*sizeY);
    for(y=0;y<sizeY;y++){
        dbptBuffer[y]=&(linearBuffer[y*sizeX]);
    }
}

PhaseImageCPU::PhaseImageCPU( const PhaseImageCPU & input):PhaseImage(NULL,0,0){
    int y;
    //Copio los datos y hago una copia del buffer
    this->sizeX=input.sizeX;
    this->sizeY=input.sizeY;

    this->linearBuffer=(float *) malloc (sizeof(float)*sizeX*sizeY);
    memcpy(this->linearBuffer,input.linearBuffer,sizeof(float)*sizeX*sizeY);


    dbptBuffer=(float **) malloc(sizeof( float * )*sizeY);

    for(y=0;y<sizeY;y++){
        dbptBuffer[y]=&(linearBuffer[y*sizeX]);
    }

}

PhaseImageCPU::~PhaseImageCPU(){
    if(dbptBuffer!=NULL)
        free(dbptBuffer);
    if(linearBuffer!=NULL)
        free(linearBuffer);
}

void PhaseImageCPU::LoadFromMemoryData(float * buffer, int sizeX_in, int sizeY_in){
    int y;
    if(dbptBuffer!=NULL)
        free(dbptBuffer);
    if(linearBuffer!=NULL)
        free(linearBuffer);

    sizeX=sizeX_in;
    sizeY=sizeY_in;
    linearBuffer=(float *) malloc(sizeof(float)*sizeX*sizeY);
    memcpy(linearBuffer,buffer,sizeof(float)*sizeX*sizeY);
    dbptBuffer=(float **) malloc(sizeof( float * )*sizeY);
    for(y=0;y<sizeY;y++){
        dbptBuffer[y]=&(linearBuffer[y*sizeX]);
    }
}

float * PhaseImageCPU::GetMemoryBuffer(){
    if(linearBuffer==NULL){
        return NULL;
    }
    float * phaseBuffer=(float *) malloc(sizeof(float)*sizeX*sizeY);

    memcpy(phaseBuffer,linearBuffer,sizeof(float)*sizeX*sizeY);

    return phaseBuffer;

}

void PhaseImageCPU::ScanPhase(){
    ScanPhaseCPU(linearBuffer,sizeX,sizeY);
}

QImage * PhaseImageCPU::GetAsQImage(int gain){
    if(gain < 1) gain=1;

    float * phaseBuffer=(float *) malloc(sizeof(float)*sizeX*sizeY);

    memcpy(phaseBuffer,linearBuffer,sizeof(float)*sizeX*sizeY);

    int x,y;
    float max=-60000,min=60000,aux;
    for (y=0;y<sizeY;y++){
        for (x=0;x<sizeX;x++){
            aux=phaseBuffer[x+y*sizeX];
            if(aux>max){
                max=aux;
            }
            if(aux<min){
                min=aux;
            }
        }
    }

    for (y=0;y<sizeY;y++){
        for (x=0;x<sizeX;x++){
            phaseBuffer[x+y*sizeX]=((phaseBuffer[x+y*sizeX]-min)*255*gain)/(max-min);
            if(phaseBuffer[x+y*sizeX]>255){
                phaseBuffer[x+y*sizeX]=255;
            }
        }
    }

    QImage * imageGray=new QImage(sizeX, sizeY, QImage::Format_Indexed8);
    //Inicio la tabla de colores en escala de grises

    QVector<QRgb> colors;
    for(x=0;x<256;x++){
        colors << qRgb(  x,   x,   x);
    }
    imageGray->setColorTable(colors);

    int gray;
    for(y=0;y<sizeY;y++){
        for(x=0;x<sizeX;x++){
            gray=floor(phaseBuffer[x+y*sizeX]);
            if(gray<=255 && gray >=0)
                imageGray->setPixel(x,y,gray);
        }
    }

    free(phaseBuffer);

    return imageGray;

}
Data2D * PhaseImageCPU::GetData2D(Data2D * data2dIn){
    float * returnBuffer;
    Data2D * data2d;

    //Reservo memoria para el buffer que voy a dejar para el nuevo objeto
    returnBuffer = (float *) malloc (sizeof(float)*sizeX*sizeY);

    memcpy(returnBuffer,linearBuffer,sizeof(float)*sizeX*sizeY);

    //Si no me han pasado ningún objeto, creo el objeto, y le asigno la zona de memoria
    if(data2dIn==NULL){
        data2d=new Data2D(returnBuffer,sizeX,sizeY);
        return data2d;
    }
    //Si sí me han pasado un objeto, lo modifico
    else{
        data2dIn->SetData(returnBuffer,sizeX,sizeY);
        return data2dIn;
    }



}
void PhaseImageCPU::SubtractPhase(PhaseImage * offset_in){

    PhaseImageCPU * offset=(PhaseImageCPU* )offset_in;
    //Si la fase de entrada es más pequeña, no hacemos nada, pues si no violaríamos memoria
    if(offset->sizeX < this->sizeX || offset->sizeY < this->sizeY)
        return;

    float * offsetPt=offset->linearBuffer;
    SubtractCPU(linearBuffer,offsetPt,sizeX,sizeY);


}
void PhaseImageCPU::AdditiveInverse(){
    AdditiveInverseCPU(linearBuffer,sizeX,sizeY);

}
void PhaseImageCPU::RemoveHorizontalLinearTilt(){
    RemoveHorizontalLinearTiltCPU(linearBuffer,sizeX,sizeY);
}
void PhaseImageCPU::RemoveVerticalLinearTilt(){
    RemoveVerticalLinearTiltCPU(linearBuffer,sizeX,sizeY);
}

void PhaseImageCPU::SetGround(){
    SetGroundCPU(linearBuffer,sizeX,sizeY);
}

void PhaseImageCPU::OffsetAndMultiply(float offset,float factor){
    OffsetAndMultiplyCPU(linearBuffer,sizeX,sizeY,offset,factor);
}

void PhaseImageCPU::MakeSymmetric(){
    MakeSymmetricCPU(linearBuffer,sizeX,sizeY);

}

void PhaseImageCPU::AbelInversion(float dx,int neighbours){
    if(neighbours<1)
        neighbours=1;
    if(dx==0)
        dx=1;

    AbelInversionCPU(linearBuffer,sizeX,sizeY,dx,neighbours);
}

void PhaseImageCPU::CropRectangle(QRect rect){
    int x1=rect.left();
    int x2=rect.right();
    int y1=rect.top();
    int y2=rect.bottom();

    int y;

    if(x1<0) x1=0;
    if(y1<0) y1=0;
    if(x2>=sizeX) x2=sizeX-1;
    if(y2>=sizeY) y2=sizeY-1;
    if(x1>=x2 || y1>=y2)
        return;    

    float * buffer = CropRectangleCPU(linearBuffer,sizeX,sizeY,x1,y1,x2,y2);

    free(linearBuffer);
    linearBuffer=buffer;
    sizeX=(x2-x1+1);
    sizeY=(y2-y1+1);
    free(dbptBuffer);
    dbptBuffer=(float **) malloc(sizeof( float * )*sizeY);
    for(y=0;y<sizeY;y++){
        dbptBuffer[y]=&(linearBuffer[y*sizeX]);
    }

}

PhaseLine * PhaseImageCPU::GetPhaseLine(QRect points){
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

    int outputSize=sqrt((float)((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)))+1;

    float * outputBuffer=GetPhaseLineCPU(linearBuffer,sizeX,sizeY,x1,y1,x2,y2,outputSize);

    PhaseLine* phase;
    phase = new PhaseLineCPU(outputBuffer,outputSize);
    return phase;

}
