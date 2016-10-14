/**
 * @file   camerafactoryforueye.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Factoría para la cámara uEye.
 *
 */
#ifndef CAMERAFACTORYFORUEYE_H
#define CAMERAFACTORYFORUEYE_H

#include "abstractcamerafactory.h"
#include "cameraueye.h"

/*! \class CameraFactoryForUEye camerafactoryforueye.h "camerafactoryforueye.h"
 * \brief Factoría para la cámara uEye.
 *
 */

class CameraFactoryForUEye : public AbstractCameraFactory
{
public:
    CameraFactoryForUEye();
    /**< Constructor.*/
    AbstractCamera * GetCamera();
};

#endif // CAMERAFACTORYFORUEYE_H
