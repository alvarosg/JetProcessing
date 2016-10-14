/**
 * @file   camerafactoryforueye.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Factor�a para la c�mara uEye.
 *
 */
#ifndef CAMERAFACTORYFORUEYE_H
#define CAMERAFACTORYFORUEYE_H

#include "abstractcamerafactory.h"
#include "cameraueye.h"

/*! \class CameraFactoryForUEye camerafactoryforueye.h "camerafactoryforueye.h"
 * \brief Factor�a para la c�mara uEye.
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
