/**
 * @file   camerafactoryforsimulator.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Factor�a para el simulador de c�mara.
 *
 */
#ifndef CAMERAFACTORYFORSIMULATOR_H
#define CAMERAFACTORYFORSIMULATOR_H


#include "abstractcamerafactory.h"
#include "camerasimulator.h"

/*! \class CameraFactoryForSimulator camerafactoryforsimulator.h "camerafactoryforsimulator.h"
 * \brief Factor�a para el simulador de c�mara.
 *
 */

class CameraFactoryForSimulator : public AbstractCameraFactory
{
public:
    CameraFactoryForSimulator();
    /**< Constructor.*/
    AbstractCamera * GetCamera();
};

#endif // CAMERAFACTORYFORSIMULATOR_H
