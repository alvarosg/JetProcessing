/**
 * @file   imagefactoryforcuda.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase ImageFactoryForCUDA.
 *
 */
#include "imagefactoryforcuda.h"

ImageFactoryForCUDA::ImageFactoryForCUDA(){
}

FullImage * ImageFactoryForCUDA::CreateFullImage(QImage *source,int power2SizeX, int power2SizeY){
    return new FullImageCUDA(source,power2SizeX,power2SizeY);
}

FullImage * ImageFactoryForCUDA::CopyFullImage(FullImage * input){
    return new FullImageCUDA(*((FullImageCUDA*)input));
}

PhaseImage * ImageFactoryForCUDA::CreatePhaseImage(float * buffer, int sizeX_in, int sizeY_in){
    return new PhaseImageCUDA(buffer,sizeX_in,sizeY_in);
}

PhaseImage * ImageFactoryForCUDA::CopyPhaseImage(PhaseImage * input){
    return new PhaseImageCUDA(*((PhaseImageCUDA*)input));
}

PhaseLine * ImageFactoryForCUDA::CreatePhaseLine(float * buffer, int size_in){
    return new PhaseLineCUDA(buffer,size_in);
}

PhaseLine * ImageFactoryForCUDA::CopyPhaseLine(PhaseLine * input){
    return new PhaseLineCUDA(*((PhaseLineCUDA*)input));
}
