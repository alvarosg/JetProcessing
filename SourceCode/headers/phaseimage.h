/**
 * @file   phaseimage.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
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
 * Define una interfaz que debe ser implementada por cualquier sistema de c�lculo (En este caso CUDA o CPU), que debe ser programada derivando una clase de �sta.
 *
 *
 */

class PhaseImage
{

protected:
    int sizeX;              /**< Ancho en p�xeles de la imagen.*/
    int sizeY;              /**< Alto en p�xeles de la imagen.*/
    float *  linearBuffer;  /**< Puntero al buffer donde se encuentra la informaci�n de los p�xeles de imagen, organizado fila tras fila. Este puntero puede apuntar tanto a memoria de la CPU como a memoria de la GPU.*/

public:
    PhaseImage(float * buffer, int sizeX_in, int sizeY_in);
    /**< Constructor.
      @param buffer Puntero a la zona de memoria de la implementaci�n elegida con la imagen a cargar. El puntero y la zona de memoria pasar� a ser responsabilidad del objeto creado.
      @param sizeX_in Ancho en p�xeles de la imagen.
      @param sizeY_in Alto en p�xeles de la imagen.*/
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
      @param buffer Puntero a la zona de memoria principal con la imagen a cargar. El objeto crear� una copia, por lo que la zona de memoria sigue siendo resposabilidad del padre.
      @param sizeX_in Ancho en p�xeles de la imagen.
      @param sizeY_in Alto en p�xeles de la imagen.*/
    virtual float * GetMemoryBuffer()=0;
    /**< Crea una copia en memoria principal del buffer y lo devuelve como argumento.
      @return Puntero a la zona de memoria con el buffer, de tama�o sizeX*sizeY*sizeof(float).*/
    virtual void ScanPhase()=0;
    /**< Funci�n que escanea al fase para hacerla cont�nua. Esto es, dada una imagen con una fase variando entre 0 y 2Pi, se pasa a tener una imagen cont�nua de fase, es decir, a cada salto encontrado en la fase, se le suma/resta m�ltiplos de 2Pi hasta que no hay saltos.*/
    virtual QImage * GetAsQImage(int gain)=0;
    /**< Devuelve una imagen de la fase. Reescala los valores del modulo entre 0 y 255, y despu�s les aplica la ganancia deseada.
        @param gain Ganancia a aplicar despu�s del reescalado entre 0 y 255.
        @return Puntero a un objeto QImage. Responsabilidad del cliente.*/
    virtual Data2D * GetData2D(Data2D * data2dIn=NULL)=0;
    /**< Obtiene un objeto tipo Data2D con los datos de la fase contenidos en el objeto PhaseImage.
        @param data2dIn Si este par�metro es nulo, se crea un objeto Data2D nuevo. Si no lo es, se rellenan los datos en el objeto pasado como par�metro y se devuelve.*/
    virtual void SubtractPhase(PhaseImage * offset_in)=0;
    /**< Resta de la imagen contenida en el objeto PhaseImage, otra imagen de un objeto tipo PhaseImage pasado como par�metro.
        @param offset_in Objeto que se va a restar. Este objeto debe estar implementado de la misma forma que el objeto a cuyo m�todo se llama.*/

    virtual void AdditiveInverse()=0;
    /**< Funci�n que asigna a cada valor de la imagen de la fase su opuesto.*/
    virtual void RemoveHorizontalLinearTilt()=0;
    /**< Funci�n que elimina la inclinaci�n horizontal de la imagen mediante un ajuste de m�nimos cuadrados.*/
    virtual void RemoveVerticalLinearTilt()=0;
    /**< Funci�n que elimina la inclinaci�n vertical de la imagen mediante un ajuste de m�nimos cuadrados.*/
    virtual void SetGround()=0;
    /**< Funci�n que establece la base de la imagen, eliminando ruido, utilizando el promedio de los valores laterales.*/
    virtual void OffsetAndMultiply(float offset,float factor)=0;
    /**< Funci�n que multiplica por un factor y suma un offset a la imagen.*/
    virtual void MakeSymmetric()=0;
    /**< Funci�n que hace la imagen sim�trica horizontalmente, respecto al valor m�ximo de cada fila, centrando dicho valor en la imagen.*/
    virtual void AbelInversion(float dx,int neighbours)=0;
    /**< Funci�n que calcula la Inversi�n de Abel horizontalmente de la imagen.
        @param dx Distancia entre dos pixeles.
        @param neighbours N�mero de vecinos para calcular la derivada.*/
    virtual void CropRectangle(QRect rect)=0;
    /**< Funci�n que recorta parte de la imagen, dejando s�lo la parte que se encuentre dentro de una m�scara pasada como argumento.
        @param rect M�scara de recorte.*/
    virtual PhaseLine * GetPhaseLine(QRect points)=0;
    /**< Devuelve un objeto tipo PhaseLine responsabilidad del cliente, implementado de la misma forma que el objeto llamado con los datos desde los puntos indicados. El n�mero de muestras de la recta se establece en proporci�n a la longitud del segmento.
        @param points Rect�ngulo que indica los puntos inicial y final de la recta, desde la esquina superior izquierda del rect�ngulo hasta la inferior derecha.
        @return Datos unidimensionales a lo largo de la recta indicada.*/


};

#endif // PHASEIMAGE_H
