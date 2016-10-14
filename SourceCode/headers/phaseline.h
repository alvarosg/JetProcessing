/**
 * @file   phaseline.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
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
 * \brief Clase abstracta que almacena información unidimensional de la fase.
 *
 * Sobre objetos de esta clase se aplican los algoritmos elegidos por el usuario.
 *
 * Define una interfaz que debe ser implementada por cualquier sistema de cálculo (En este caso CUDA o CPU), que debe ser programada derivando una clase de ésta.
 *
 *
 */

class PhaseLine{

protected:

    float * linearBuffer;   /**< Puntero al buffer donde se encuentra la información de la fase. Este puntero puede apuntar tanto a memoria de la CPU como a memoria de la GPU.*/
    int size;               /**< Número de muestras en el buffer.*/

public:

    PhaseLine(float * buffer, int size_in);
    /**< Constructor.
      @param buffer Puntero a la zona de memoria de la implementación elegida con los datos a cargar. El puntero y la zona de memoria pasará a ser responsabilidad del objeto creado.
      @param size_in Número de muestras en el buffer.*/
    virtual ~PhaseLine();
    /**< Destructor.*/
    int GetSize();
    /**< Devuelve el valor del atributo size.
        @return El valor del atributo.*/
    virtual Data1D * GetData1D(Data1D * data1dIn=NULL)=0;
    /**< Obtiene un objeto tipo Data1D con los datos de la fase contenidos en el objeto PhaseLine.
        @param data1dIn Si este parámetro es nulo, se crea un objeto Data1D nuevo. Si no lo es, se rellenan los datos en el objeto pasado como parámetro y se devuelve.*/
    virtual void AdditiveInverse()=0;
    /**< Función que asigna a cada valor de la imagen de la fase su opuesto.*/
    virtual void RemoveLinearTilt()=0;
    /**< Función que elimina la inclinación lineal de los datos, usando un ajuste de mínimos cuadrados.*/
    virtual void SetGround()=0;
    /**< Función que establece la base de los datos, eliminando ruido, utilizando el promedio de los valores laterales.*/
    virtual void OffsetAndMultiply(float offset,float factor)=0;
    /**< Función que multiplica por un factor y suma un offset a los datos.*/
    virtual void MakeSymmetric()=0;
    /**< Función que simetriza los datos respecto al valor máximo centrando dicho valor.*/
    virtual void AbelInversion(float dx,int neighbours)=0;
    /**< Función que calcula la Inversión de Abel horizontalmente de los datos.
        @param dx Distancia entre dos pixeles.
        @param neighbours Número de vecinos para calcular la derivada.*/
};
#endif // PHASELINE_H
