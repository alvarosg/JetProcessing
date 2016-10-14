/**
 * @file   camerafactoryforueye.cpp
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de la clase CameraFactoryForUEye.
 *
 */
#include "camerafactoryforueye.h"

CameraFactoryForUEye::CameraFactoryForUEye(){
}

AbstractCamera * CameraFactoryForUEye::GetCamera(){
    return new CameraUEye();
}
