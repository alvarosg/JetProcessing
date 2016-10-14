/**
 * @file   phaseimage.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase abstracta que contiene la imagen bidimensional de la fase durante las etapas intermedias del procesado.
 *
 */
#ifndef PHASEIMAGE_H
#define PHASEIMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <QImage>
#include "data2d.h"
#include "math.h"
#include "phaseline.h"

/*! \class PhaseImage phaseimage.h "phaseimage.h"
 * \brief Clase abstracta que almacena la imagen de la fase.
 *
 * Sobre objetos de esta clase se aplican los algoritmos elegidos por el usuario.
 *
 * Define una interfaz que debe ser implementada por cualquier sistema de cálculo (En este caso CUDA o CPU), que debe ser programada derivando una clase de ésta.
 *
 *
 */

class PhaseImage
{

protected:
    int sizeX;              /**< Ancho en píxeles de la imagen.*/
    int sizeY;              /**< Alto en píxeles de la imagen.*/
    float *  linearBuffer;  /**< Puntero al buffer donde se encuentra la información de los píxeles de imagen, organizado fila tras fila. Este puntero puede apuntar tanto a memoria de la CPU como a memoria de la GPU.*/

public:
    PhaseImage(float * buffer, int sizeX_in, int sizeY_in);
    /**< Constructor.
      @param buffer Puntero a la zona de memoria de la implementación elegida con la imagen a cargar. El puntero y la zona de memoria pasará a ser responsabilidad del objeto creado.
      @param sizeX_in Ancho en píxeles de la imagen.
      @param sizeY_in Alto en píxeles de la imagen.*/
    virtual ~PhaseImage();
    /**< Destructor.*/
    int GetSizeX();
    /**< Devuelve el valor del atributo sizeX.
        @return El valor del atributo.*/
    int GetSizeY();
    /**< Devuelve el valor del atributo sizeY.
        @return El valor del atributo.*/
    virtual void LoadFromMemoryData(float * buffer, int sizeX_in, int sizeY_in)=0;
    /**< Carga los datos de la imagen desde datos en memoria principal.
      @param buffer Puntero a la zona de memoria principal con la imagen a cargar. El objeto creará una copia, por lo que la zona de memoria sigue siendo resposabilidad del padre.
      @param sizeX_in Ancho en píxeles de la imagen.
      @param sizeY_in Alto en píxeles de la imagen.*/
    virtual float * GetMemoryBuffer()=0;
    /**< Crea una copia en memoria principal del buffer y lo devuelve como argumento.
      @return Puntero a la zona de memoria con el buffer, de tamaño sizeX*sizeY*sizeof(float).*/
    virtual void ScanPhase()=0;
    /**< Función que escanea al fase para hacerla contínua. Esto es, dada una imagen con una fase variando entre 0 y 2Pi, se pasa a tener una imagen contínua de fase, es decir, a cada salto encontrado en la fase, se le suma/resta múltiplos de 2Pi hasta que no hay saltos.*/
    virtual QImage * GetAsQImage(int gain)=0;
    /**< Devuelve una imagen de la fase. Reescala los valores del modulo entre 0 y 255, y después les aplica la ganancia deseada.
        @param gain Ganancia a aplicar después del reescalado entre 0 y 255.
        @return Puntero a un objeto QImage. Responsabilidad del cliente.*/
    virtual Data2D * GetData2D(Data2D * data2dIn=NULL)=0;
    /**< Obtiene un objeto tipo Data2D con los datos de la fase contenidos en el objeto PhaseImage.
        @param data2dIn Si este parámetro es nulo, se crea un objeto Data2D nuevo. Si no lo es, se rellenan los datos en el objeto pasado como parámetro y se devuelve.*/
    virtual void SubtractPhase(PhaseImage * offset_in)=0;
    /**< Resta de la imagen contenida en el objeto PhaseImage, otra imagen de un objeto tipo PhaseImage pasado como parámetro.
        @param offset_in Objeto que se va a restar. Este objeto debe estar implementado de la misma forma que el objeto a cuyo método se llama.*/

    virtual void AdditiveInverse()=0;
    /**< Función que asigna a cada valor de la imagen de la fase su opuesto.*/
    virtual void RemoveHorizontalLinearTilt()=0;
    /**< Función que elimina la inclinación horizontal de la imagen mediante un ajuste de mínimos cuadrados.*/
    virtual void RemoveVerticalLinearTilt()=0;
    /**< Función que elimina la inclinación vertical de la imagen mediante un ajuste de mínimos cuadrados.*/
    virtual void SetGround()=0;
    /**< Función que establece la base de la imagen, eliminando ruido, utilizando el promedio de los valores laterales.*/
    virtual void OffsetAndMultiply(float offset,float factor)=0;
    /**< Función que multiplica por un factor y suma un offset a la imagen.*/
    virtual void MakeSymmetric()=0;
    /**< Función que hace la imagen simétrica horizontalmente, respecto al valor máximo de cada fila, centrando dicho valor en la imagen.*/
    virtual void AbelInversion(float dx,int neighbours)=0;
    /**< Función que calcula la Inversión de Abel horizontalmente de la imagen.
        @param dx Distancia entre dos pixeles.
        @param neighbours Número de vecinos para calcular la derivada.*/
    virtual void CropRectangle(QRect rect)=0;
    /**< Función que recorta parte de la imagen, dejando sólo la parte que se encuentre dentro de una máscara pasada como argumento.
        @param rect Máscara de recorte.*/
    virtual PhaseLine * GetPhaseLine(QRect points)=0;
    /**< Devuelve un objeto tipo PhaseLine responsabilidad del cliente, implementado de la misma forma que el objeto llamado con los datos desde los puntos indicados. El número de muestras de la recta se establece en proporción a la longitud del segmento.
        @param points Rectángulo que indica los puntos inicial y final de la recta, desde la esquina superior izquierda del rectángulo hasta la inferior derecha.
        @return Datos unidimensionales a lo largo de la recta indicada.*/


};

#endif // PHASEIMAGE_H
