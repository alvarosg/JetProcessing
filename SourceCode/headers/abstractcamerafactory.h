/**
 * @file   abstractcamerafactory.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Factoria abstracta para la creación de cámaras.
 *
 */
#ifndef ABSTRACTCAMERAFACTORY_H
#define ABSTRACTCAMERAFACTORY_H

#include "abstractcamera.h"

/*! \class AbstractCameraFactory abstractcamerafactory.h "abstractcamerafactory.h"
 * \brief Factoria abstracta para la creación de cámaras.
 *
 */

class AbstractCameraFactory
{
public:
    AbstractCameraFactory();
    /**< Constructor. */
    virtual AbstractCamera * GetCamera()=0;
    /**< Crea una cámara y la devuelve.
        @return Una cámara del tipo que corresponda a la factoría concreta que se este utilizando.*/
};

#endif // ABSTRACTCAMERAFACTORY_H
