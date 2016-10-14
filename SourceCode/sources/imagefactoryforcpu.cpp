/**
 * @file   imagefactoryforcpu.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase ImageFactoryForCPU.
 *
 */
#include "imagefactoryforcpu.h"

ImageFactoryForCPU::ImageFactoryForCPU(){
}

FullImage * ImageFactoryForCPU::CreateFullImage(QImage *source,int power2SizeX, int power2SizeY){
    return new FullImageCPU(source,power2SizeX,power2SizeY);
}

FullImage * ImageFactoryForCPU::CopyFullImage(FullImage * input){
    return new FullImageCPU(*((FullImageCPU*)input));
}

PhaseImage * ImageFactoryForCPU::CreatePhaseImage(float * buffer, int sizeX_in, int sizeY_in){
    return new PhaseImageCPU(buffer,sizeX_in,sizeY_in);
}

PhaseImage * ImageFactoryForCPU::CopyPhaseImage(PhaseImage * input){
    return new PhaseImageCPU(*((PhaseImageCPU*)input));
}

PhaseLine * ImageFactoryForCPU::CreatePhaseLine(float * buffer, int size_in){
    return new PhaseLineCPU(buffer,size_in);
}

PhaseLine * ImageFactoryForCPU::CopyPhaseLine(PhaseLine * input){
    return new PhaseLineCPU(*((PhaseLineCPU*)input));
}
