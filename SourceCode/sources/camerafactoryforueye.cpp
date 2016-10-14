/**
 * @file   camerafactoryforueye.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase CameraFactoryForUEye.
 *
 */
#include "camerafactoryforueye.h"

CameraFactoryForUEye::CameraFactoryForUEye(){
}

AbstractCamera * CameraFactoryForUEye::GetCamera(){
    return new CameraUEye();
}
