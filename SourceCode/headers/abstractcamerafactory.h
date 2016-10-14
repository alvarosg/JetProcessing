/**
 * @file   abstractcamerafactory.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Factoria abstracta para la creaci�n de c�maras.
 *
 */
#ifndef ABSTRACTCAMERAFACTORY_H
#define ABSTRACTCAMERAFACTORY_H

#include "abstractcamera.h"

/*! \class AbstractCameraFactory abstractcamerafactory.h "abstractcamerafactory.h"
 * \brief Factoria abstracta para la creaci�n de c�maras.
 *
 */

class AbstractCameraFactory
{
public:
    AbstractCameraFactory();
    /**< Constructor. */
    virtual AbstractCamera * GetCamera()=0;
    /**< Crea una c�mara y la devuelve.
        @return Una c�mara del tipo que corresponda a la factor�a concreta que se este utilizando.*/
};

#endif // ABSTRACTCAMERAFACTORY_H
