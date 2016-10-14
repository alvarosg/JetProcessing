/**
 * @file   phaseline.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase abstracta que contiene la imagen unidimiensional de la fase durante las etapas intermedias del procesado.
 *
 */
#ifndef PHASELINE_H
#define PHASELINE_H

#include <stdio.h>
#include <stdlib.h>
#include "data1d.h"
#include "math.h"
#include "complex.h"


/*! \class PhaseLine phaseline.h "phaseline.h"
 * \brief Clase abstracta que almacena informaci�n unidimensional de la fase.
 *
 * Sobre objetos de esta clase se aplican los algoritmos elegidos por el usuario.
 *
 * Define una interfaz que debe ser implementada por cualquier sistema de c�lculo (En este caso CUDA o CPU), que debe ser programada derivando una clase de �sta.
 *
 *
 */

class PhaseLine{

protected:

    float * linearBuffer;   /**< Puntero al buffer donde se encuentra la informaci�n de la fase. Este puntero puede apuntar tanto a memoria de la CPU como a memoria de la GPU.*/
    int size;               /**< N�mero de muestras en el buffer.*/

public:

    PhaseLine(float * buffer, int size_in);
    /**< Constructor.
      @param buffer Puntero a la zona de memoria de la implementaci�n elegida con los datos a cargar. El puntero y la zona de memoria pasar� a ser responsabilidad del objeto creado.
      @param size_in N�mero de muestras en el buffer.*/
    virtual ~PhaseLine();
    /**< Destructor.*/
    int GetSize();
    /**< Devuelve el valor del atributo size.
        @return El valor del atributo.*/
    virtual Data1D * GetData1D(Data1D * data1dIn=NULL)=0;
    /**< Obtiene un objeto tipo Data1D con los datos de la fase contenidos en el objeto PhaseLine.
        @param data1dIn Si este par�metro es nulo, se crea un objeto Data1D nuevo. Si no lo es, se rellenan los datos en el objeto pasado como par�metro y se devuelve.*/
    virtual void AdditiveInverse()=0;
    /**< Funci�n que asigna a cada valor de la imagen de la fase su opuesto.*/
    virtual void RemoveLinearTilt()=0;
    /**< Funci�n que elimina la inclinaci�n lineal de los datos, usando un ajuste de m�nimos cuadrados.*/
    virtual void SetGround()=0;
    /**< Funci�n que establece la base de los datos, eliminando ruido, utilizando el promedio de los valores laterales.*/
    virtual void OffsetAndMultiply(float offset,float factor)=0;
    /**< Funci�n que multiplica por un factor y suma un offset a los datos.*/
    virtual void MakeSymmetric()=0;
    /**< Funci�n que simetriza los datos respecto al valor m�ximo centrando dicho valor.*/
    virtual void AbelInversion(float dx,int neighbours)=0;
    /**< Funci�n que calcula la Inversi�n de Abel horizontalmente de los datos.
        @param dx Distancia entre dos pixeles.
        @param neighbours N�mero de vecinos para calcular la derivada.*/
};
#endif // PHASELINE_H
