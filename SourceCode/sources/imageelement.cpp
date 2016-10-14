/**
 * @file   imageelement.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase ImageSequenceController.
 *
 */
#include "imageelement.h"

ImageElement::ImageElement(PhaseImage * source_in,QString label_in, float pixelsMmX_in, float pixelsMmY_in,float pixelsFromOriginX_in,float pixelsFromOriginY_in){
    phaseImageSource=source_in;
    label=label_in;
    phaseImage=NULL;
    phaseLine=NULL;
    data1d=NULL;
    data2d=NULL;
    preview=NULL;
    updated=false;
    pixelsMmX=pixelsMmX_in;
    pixelsMmY=pixelsMmY_in;
    pixelsFromOriginX=pixelsFromOriginX_in;
    pixelsFromOriginY=pixelsFromOriginY_in;
}

ImageElement::~ImageElement(){
}

QString ImageElement::GetLabel(){
    return label;
}

PhaseImage * ImageElement::GetPhaseImageSource(){
    return phaseImageSource;
}

PhaseImage * ImageElement::GetPhaseImage(){
    return phaseImage;
}

PhaseLine * ImageElement::GetPhaseLine(){
    return phaseLine;
}

Data2D * ImageElement::GetData2D(){
    return data2d;
}

Data1D * ImageElement::GetData1D(){
    return data1d;
}
int ImageElement::GetPixelsFromOriginX(){
    return pixelsFromOriginX;
}
int ImageElement::GetPixelsFromOriginY(){
    return pixelsFromOriginY;
}

QImage ImageElement::GetPreview(){
    QImage previewOut;
    QPixmap * auxPixmap;
    //Si ya tengo vista previa, la devuelvo
    if (preview !=NULL){
        previewOut=QImage::QImage(*preview);
    }

    //Si es la primera vez y puedo, la calculo
    else{
        if(phaseImageSource!=NULL){
            //Calculo la vista previa y se la asigno
            Data2D * data2d = phaseImageSource->GetData2D();
            SpectrogramPlot * spectrogram;
            spectrogram = new SpectrogramPlot(NULL);
            spectrogram->SetData2D(data2d);
            spectrogram->enableAxis(0,false);
            spectrogram->enableAxis(1,false);
            spectrogram->enableAxis(2,false);
            auxPixmap=spectrogram->GetAsQPixmap();
            this->SetPreview(auxPixmap->toImage());
            delete auxPixmap;
            delete spectrogram;
            delete data2d;


            if(preview->isNull()==false){
                previewOut=QImage::QImage(*preview);
            }
            else{
                delete preview;
                preview=NULL;
                previewOut=QImage::QImage(40,40,QImage::Format_RGB32);
            }

        }
        else{
            previewOut=QImage::QImage(40,40,QImage::Format_RGB32);
            previewOut.fill(0);
        }
    }
    return previewOut;
}

bool ImageElement::GetUpdated(){
    return updated;
}

float ImageElement::GetPixelsMmX(){
    return pixelsMmX;
}

float ImageElement::GetPixelsMmY(){
    return pixelsMmY;
}

void ImageElement::SetLabel(QString value){
    label=value;
}

void ImageElement::SetPhaseImage(PhaseImage * value){

    if (phaseImage!=NULL ) delete phaseImage;
    phaseImage=value;
    if (data2d!=NULL ) delete data2d;
    if (phaseImage!=NULL)
        data2d=phaseImage->GetData2D();
    else
        data2d=NULL;
}

void ImageElement::SetPhaseLine(PhaseLine * value){
    if (phaseLine!=NULL ) delete phaseLine;
    phaseLine=value;
    if (data1d!=NULL ) delete data1d;
    if (phaseLine!=NULL)
        data1d=phaseLine->GetData1D();
    else
        data1d=NULL;

}

void ImageElement::SetPreview(QImage value){
    if (preview!=NULL) delete preview;
    preview = new QImage (value);
}

void ImageElement::SetUpdated(bool value){
    updated=value;
}

void ImageElement::FreePointers(){
    if (phaseImage!=NULL ) delete phaseImage;
    if (phaseLine!=NULL ) delete phaseLine;
    if (data1d!=NULL ) delete data1d;
    if (data2d!=NULL ) delete data2d;
    if (preview!=NULL ) delete preview;
    if (phaseImageSource!=NULL ) delete phaseImageSource;
    phaseImage=NULL;
    phaseLine=NULL;
    data1d=NULL;
    data2d=NULL;
    preview=NULL;
    phaseImageSource=NULL;
}
