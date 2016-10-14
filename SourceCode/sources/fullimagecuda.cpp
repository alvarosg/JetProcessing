/**
 * @file   fullimagecuda.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase FullImageCUDA.
 *
 */
#include "fullimagecuda.h"

FullImageCUDA::FullImageCUDA(QImage *source,int power2SizeX, int power2SizeY):
    FullImage(source,power2SizeX,power2SizeY){

    //Trasbaso la información que hay actualmente en linberBuffer a la memoria de la GPU
    //y vuelvo a poner el puntero en linearBuffer
    complex * auxDevice;
    auxDevice=LoadInGPUCUDA(linearBuffer,sizeX,sizeY);
    free(linearBuffer);
    linearBuffer=auxDevice;
}

FullImageCUDA::FullImageCUDA( const FullImageCUDA & input):FullImage(NULL,0,0){
    //Copio los datos y hago una copia del buffer
    this->sizeX=input.sizeX;
    this->sizeY=input.sizeY;
    this->linearBuffer=MemCpyCUDA(input.linearBuffer,sizeX,sizeY);
}


FullImageCUDA::~FullImageCUDA(){
    FreeCUDA(linearBuffer);
}

void FullImageCUDA::ApplyMask(QRect mask){
    int x1=mask.left();
    int x2=mask.right();
    int y1=mask.top();
    int y2=mask.bottom();

    ApplyMaskCUDA(linearBuffer,sizeX,sizeY,x1,y1,x2,y2);
}

void FullImageCUDA::ComplexFFT(DirFFT dir){

    switch(dir){
        case FFT_FORWARD: FFTCUDA(linearBuffer,sizeX,sizeY,FORWARD); break;
        case FFT_INVERSE: FFTCUDA(linearBuffer,sizeX,sizeY,INVERSE); break;
    }

}

void FullImageCUDA::ReArrangeFT(){
    complex * aux;
    aux=ReArrangeFTCUDA(linearBuffer,sizeX,sizeY);
    FreeCUDA(linearBuffer);
    linearBuffer=aux;
}


PhaseImage * FullImageCUDA::CropPhase(QRect mask){
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

    PhaseImage * phase;
    float * deviceBuffer;

    deviceBuffer = CropPhaseCUDA(linearBuffer,sizeX,sizeY,x1,y1,x2,y2);

    phase = new PhaseImageCUDA(deviceBuffer,(x2-x1+1),(y2-y1+1));



    return phase;

}

QImage * FullImageCUDA::GetAbsoluteValueAsQImage(int gain){

    if(gain < 1) gain=1;

    float * absolute;

    absolute=GetAbsoluteValueCUDA(linearBuffer,sizeX,sizeY);


    int x,y;
    float max=-60000,min=60000;
    for (y=0;y<sizeY;y++){
        for (x=0;x<sizeX;x++){
            if(absolute[x+y*sizeX]>max){
                max=absolute[x+y*sizeX];
            }
            if(absolute[x+y*sizeX]<min){
                min=absolute[x+y*sizeX];
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
