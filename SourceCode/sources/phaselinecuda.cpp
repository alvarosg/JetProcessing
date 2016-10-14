/**
 * @file   phaselinecuda.cpp
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de la clase PhaseLineCUDA.
 *
 */
#include "phaselinecuda.h"

PhaseLineCUDA::PhaseLineCUDA(float * deviceBuffer, int size_in):PhaseLine(deviceBuffer,size_in){

}

PhaseLineCUDA::PhaseLineCUDA( const PhaseLineCUDA & input):PhaseLine(NULL,0){
    //Copio los datos y hago una copia del buffer
    this->size=input.size;
    this->linearBuffer=MemCpyCUDA(input.linearBuffer,size,1);
}

PhaseLineCUDA::~PhaseLineCUDA(){
    FreeCUDA(linearBuffer);

}

Data1D * PhaseLineCUDA::GetData1D(Data1D * data1dIn){

    float * phaseHost;
    Data1D * data1d;

    //Reservo memoria para el buffer que voy a dejar para el nuevo objeto
    phaseHost = (float *) malloc (sizeof(float)*size);

    GetBufferBackCUDA(phaseHost, linearBuffer,size,1);

    //Si no me han pasado ning�n objeto, creo el objeto, y le asigno la zona de memoria
    if(data1dIn==NULL){
        data1d=new Data1D(phaseHost,size);
        return data1d;
    }
    //Si s� me han pasado un objeto, lo modifico
    else{
        data1dIn->SetData(phaseHost,size);
        return data1dIn;
    }

}

void PhaseLineCUDA::AdditiveInverse(){
    AdditiveInverseCUDA(linearBuffer,size,1);
}

void PhaseLineCUDA::RemoveLinearTilt(){
    RemoveHorizontalLinearTiltCUDA(linearBuffer,size,1);
}

void PhaseLineCUDA::SetGround(){
    SetGroundCUDA(linearBuffer,size,1);
}

void PhaseLineCUDA::OffsetAndMultiply(float offset,float factor){
    OffsetAndMultiplyCUDA(linearBuffer,size,1,offset,factor);
}

void PhaseLineCUDA::MakeSymmetric(){
    MakeSymmetricCUDA(linearBuffer,size,1);
}

void PhaseLineCUDA::AbelInversion(float dx,int neighbours){
    if(neighbours<1)
        neighbours=1;
    if(dx==0)
        dx=1;
    AbelInversionCUDA(linearBuffer,size,1,dx,neighbours);
}
