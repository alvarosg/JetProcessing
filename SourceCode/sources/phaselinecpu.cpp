/**
 * @file   phaselinecpu.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase PhaseLineCPU.
 *
 */
#include "phaselinecpu.h"

PhaseLineCPU::PhaseLineCPU(float * buffer, int size_in):PhaseLine(buffer,size_in){

}

PhaseLineCPU::PhaseLineCPU( const PhaseLineCPU & input):PhaseLine(NULL,0){
    //Copio los datos y hago una copia del buffer
    this->size=input.size;
    this->linearBuffer=(float *)malloc(sizeof(float)*size);
    memcpy(this->linearBuffer,input.linearBuffer,size);
}

PhaseLineCPU::~PhaseLineCPU(){
    free(linearBuffer);
}

Data1D * PhaseLineCPU::GetData1D(Data1D * data1dIn){
    float * outputBuffer;
    Data1D * data1d;

    //Reservo memoria para el buffer que voy a dejar para el nuevo objeto
    outputBuffer = (float *) malloc (sizeof(float)*size);

    memcpy(outputBuffer, linearBuffer,sizeof(float)*size);

    //Si no me han pasado ningún objeto, creo el objeto, y le asigno la zona de memoria
    if(data1dIn==NULL){
        data1d=new Data1D(outputBuffer,size);
        return data1d;
    }
    //Si sí me han pasado un objeto, lo modifico
    else{
        data1dIn->SetData(outputBuffer,size);
        return data1dIn;
    }
}

void PhaseLineCPU::AdditiveInverse(){
    AdditiveInverseCPU(linearBuffer,size,1);
}

void PhaseLineCPU::RemoveLinearTilt(){
    RemoveHorizontalLinearTiltCPU(linearBuffer,size,1);
}

void PhaseLineCPU::SetGround(){
    SetGroundCPU(linearBuffer,size,1);
}

void PhaseLineCPU::OffsetAndMultiply(float offset,float factor){
    OffsetAndMultiplyCPU(linearBuffer,size,1,offset,factor);
}


void PhaseLineCPU::MakeSymmetric(){
    MakeSymmetricCPU(linearBuffer,size,1);
}

void PhaseLineCPU::AbelInversion(float dx,int neighbours){
    if(neighbours<1)
        neighbours=1;
    if(dx==0)
        dx=1;
    AbelInversionCPU(linearBuffer,size,1,dx,neighbours);
}
