/**
 * @file   camerafactoryforsimulator.cpp
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de la clase CameraFactoryForSimulator.
 *
 */
#include "camerafactoryforsimulator.h"

CameraFactoryForSimulator::CameraFactoryForSimulator(){
}

AbstractCamera * CameraFactoryForSimulator::GetCamera(){
    return new CameraSimulator();
}
