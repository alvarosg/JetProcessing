/**
 * @file   fullimagecpu.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase FullImageCPU.
 *
 */
#include "fullimagecpu.h"

FullImageCPU::FullImageCPU(QImage *source,int power2SizeX, int power2SizeY):
        FullImage(source,power2SizeX,power2SizeY){

    int y;
    //Reservo memoria para la matriz de puntero a punteros, para poder trabajar con Indices,
    //manteniendo la ventaja de tener la imagen en solamente un área de memoria
    dbptBuffer=(complex **) malloc(sizeof( complex * )*sizeY);
    for(y=0;y<sizeY;y++){
        dbptBuffer[y]=&(linearBuffer[y*sizeX]);
    }
}
FullImageCPU::FullImageCPU(const FullImageCPU &input):FullImage(NULL,0,0){
    int y;

    //Copio los datos y hago una copia del buffer
    this->sizeX=input.sizeX;
    this->sizeY=input.sizeY;
    this->linearBuffer=(complex *) malloc(sizeX*sizeY*sizeof(complex));
    memcpy(linearBuffer,input.linearBuffer,sizeX*sizeY*sizeof(complex));



    //Lleno el buffer para la matriz punteros a punteros
    dbptBuffer=(complex **) malloc(sizeof( complex * )*sizeY);
    for(y=0;y<sizeY;y++){
        dbptBuffer[y]=&(linearBuffer[y*sizeX]);
    }

}


FullImageCPU::~FullImageCPU(){
    free(dbptBuffer);
    free(linearBuffer);

}

void FullImageCPU::ApplyMask(QRect mask){
    int x1=mask.left();
    int x2=mask.right();
    int y1=mask.top();
    int y2=mask.bottom();
    ApplyMaskCPU(linearBuffer,sizeX,sizeY,x1,y1,x2,y2);
}

void FullImageCPU::ComplexFFT(DirFFT dir){
    switch(dir){
        case FFT_FORWARD: FFTCPU(dbptBuffer,sizeX,sizeY,FORWARD); break;
        case FFT_INVERSE: FFTCPU(dbptBuffer,sizeX,sizeY,INVERSE); break;
    }
}

void FullImageCPU::ReArrangeFT(){
    int y;
    complex * linearResult = ReArrangeFTCPU(linearBuffer,sizeX,sizeY);
    free(linearBuffer);
    linearBuffer=linearResult;
    for(y=0;y<sizeY;y++){
        dbptBuffer[y]=&(linearBuffer[y*sizeX]);
    }
}

PhaseImage * FullImageCPU::CropPhase(QRect mask){
    int x1=mask.left();
    int x2=mask.right();
    int y1=mask.top();
    int y2=mask.bottom();
    if(x1<0) x1=0;
    if(y1<0) y1=0;
    if(x2>=sizeX) x2=sizeX-1;
    if(y2>=sizeY) y2=sizeY-1;
    if(x1>x2)
        x1=x2;
    if(y1>y2)
        y1=y2;

    int rSizeX=x2-x1+1;
    int rSizeY=y2-y1+1;

    float * resultBuffer = CropPhaseCPU(linearBuffer,sizeX,sizeY,x1,y1,x2,y2);

    PhaseImage * phaseImage;

    phaseImage = new PhaseImageCPU(resultBuffer,rSizeX,rSizeY);
    return phaseImage;
}

QImage * FullImageCPU::GetAbsoluteValueAsQImage(int gain){
    if(gain < 1) gain=1;

    float * absolute=GetAbsoluteValueCPU(linearBuffer,sizeX,sizeY);

    int x,y;
    float max=-60000,min=60000,aux;
    for (y=0;y<sizeY;y++){
        for (x=0;x<sizeX;x++){
            aux=absolute[x+y*sizeX];
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
            absolute[x+y*sizeX]=((absolute[x+y*sizeX]-min)*255*gain)/(max-min);
            if(absolute[x+y*sizeX]>255){
                absolute[x+y*sizeX]=255;
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
            gray=floor(absolute[x+y*sizeX]);
            if(gray<=255 && gray >=0)
                imageGray->setPixel(x,y,gray);
        }
    }

    free(absolute);

    return imageGray;
}
