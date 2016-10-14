/**
 * @file   abstractimagefactory.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Factor�a abstracta para la creaci�n y duplicado de objetos FullImage, PhaseImage o PhaseLine.
 *
 */

#ifndef ABSTRACTIMAGEFACTORY_H
#define ABSTRACTIMAGEFACTORY_H

#include "fullimage.h"
#include "phaseimage.h"
#include "phaseline.h"

/*! \class AbstractImageFactory abstractimagefactory.h "abstractimagefactory.h"
 * \brief Factor�a abstracta para la creaci�n y duplicado de objetos FullImage, PhaseImage o PhaseLine.
 *
 */


class AbstractImageFactory{
public:
    AbstractImageFactory();
    /**< Constructor.*/
    virtual FullImage * CreateFullImage(QImage *source,int power2SizeX, int power2SizeY)=0;
    /**< Crea un objeto del tipo FullImage implementado seg�n la factor�a concreta a la que se este llamando.
        @param source Imagen de origen. Dicha imagen sigue siendo responsabilidad del cliente.
        @param power2SizeX Ancho deseado para la FullImage creada. Debe ser una potencia de 2. Si no lo es, se redondear� a la siguiente potencia de 2.
        @param power2SizeY Alto deseado para la FullImage creada. Debe ser una potencia de 2. Si no lo es, se redondear� a la siguiente potencia de 2.
        @return Objeto FullImage creado.*/
    virtual FullImage * CopyFullImage(FullImage * input)=0;
    /**< Duplica un objeto de la clase FullImage.
        @param input Objeto origen.
        @return Copia del objeto pasado como argumento.*/
    virtual PhaseImage * CreatePhaseImage(float * buffer, int sizeX_in, int sizeY_in)=0;
    /**< Crea un objeto del tipo PhaseImage implementado seg�n la factor�a concreta a la que se este llamando.
        @param buffer Buffer con los datos de la imagen, alineados fila tras fila. El objeto creado toma posesi�n del buffer, por lo que no debe ser modificado.
        @param sizeX_in Ancho de la imagen cuya informaci�n va en el buffer.
        @param sizeY_in Alto de la imagen cuya informaci�n va en el buffer.
        @return Objeto PhaseImage creado.*/
    virtual PhaseImage * CopyPhaseImage(PhaseImage * input)=0;
    /**< Duplica un objeto de la clase PhaseImage.
        @param input Objeto origen.
        @return Copia del objeto pasado como argumento.*/
    virtual PhaseLine * CreatePhaseLine(float * buffer, int size_in)=0;
    /**< Crea un objeto del tipo PhaseLine implementado seg�n la factor�a concreta a la que se este llamando.
        @param buffer Buffer con los datos de la imagen. El objeto creado toma posesi�n del buffer, por lo que no debe ser modificado.
        @param size_in N�mero de elementos en el buffer.
        @return Objeto PhaseLine creado.*/
    virtual PhaseLine * CopyPhaseLine(PhaseLine * input)=0;
    /**< Duplica un objeto de la clase PhaseLine.
        @param input Objeto origen.
        @return Copia del objeto pasado como argumento.*/
};

#endif // ABSTRACTIMAGEFACTORY_H
