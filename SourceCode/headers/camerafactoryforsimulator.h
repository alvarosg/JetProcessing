/**
 * @file   camerafactoryforsimulator.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Factoría para el simulador de cámara.
 *
 */
#ifndef CAMERAFACTORYFORSIMULATOR_H
#define CAMERAFACTORYFORSIMULATOR_H


#include "abstractcamerafactory.h"
#include "camerasimulator.h"

/*! \class CameraFactoryForSimulator camerafactoryforsimulator.h "camerafactoryforsimulator.h"
 * \brief Factoría para el simulador de cámara.
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
