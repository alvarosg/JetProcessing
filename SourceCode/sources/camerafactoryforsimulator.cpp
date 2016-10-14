/**
 * @file   camerafactoryforsimulator.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase CameraFactoryForSimulator.
 *
 */
#include "camerafactoryforsimulator.h"

CameraFactoryForSimulator::CameraFactoryForSimulator(){
}

AbstractCamera * CameraFactoryForSimulator::GetCamera(){
    return new CameraSimulator();
}
