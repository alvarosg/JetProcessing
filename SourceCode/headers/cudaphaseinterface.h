/**
 * @file   cudaphaseinterface.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Cabecera para la interfaz de algoritmos aplicables a un objeto de la clase tipo PhaseImage implementada con CUDA.
 *
 */

#ifndef CUDAPHASEINTERFACE_H
#define CUDAPHASEINTERFACE_H


#include <cuda.h>
#include "complex.h"
#include "cudasafe.h"

/** Funci�n que carga una imagen de la fase en la memoria de la GPU.
    @param hostPt Buffer en la memoria de la CPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.
    @return Puntero a la copia en la GPU.*/
float * LoadInGPUCUDA(float * hostPt,int width,int height);

/** Funci�n que crea una copia de la imagen de la fase en la GPU a partir de otra tambi�n en la GPU.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase a alineada fila tras fila.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.
    @return Puntero a la copia en la GPU.*/
float * MemCpyCUDA(float * devicePt,int width,int height);

/** Funci�n que libera una zona de memoria en la GPU.
    @param devicePt Puntero a la zona de memoria d ela GPU a liberar.
    */
void FreeCUDA(float * devicePt);

/** Funci�n que crea una copia de una imagen de la fase en la CPU a partir de otra en la GPU.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param hostPt Buffer en la memoria de la CPU donde ser� copiada la imagen. Debe apuntar a una zona de memoria suficientemente grande.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen. */
void GetBufferBackCUDA(float * hostPt, float * devicePt, int width,int height);

/** Funci�n que escanea la fase, para eliminar los saltos de 2Pi a 0 y hacerla cont�nua.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.*/
void ScanPhaseCUDA(float * devicePt, int width,int height);

/** Funci�n que resta a la imagen de la fase, otra imagen de la fase.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param offsetDevicePt Buffer en la memoria de la GPU con con la imagen de la fase que se va a restar.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.*/
void SubtractCUDA(float * devicePt,float * offsetDevicePt, int width,int height);

/** Funci�n que asigna a cada valor de la imagen su opuesto.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.*/
void AdditiveInverseCUDA(float * devicePt, int width,int height);

/** Funci�n que elimina la inclinaci�n horizontal de la imagen bas�ndose en los valores de los bordes.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.*/
void RemoveHorizontalLinearTiltBordersCUDA(float * devicePt, int width,int height);

/** Funci�n que elimina la inclinaci�n horizontal de la imagen.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.*/
void RemoveHorizontalLinearTiltCUDA(float * devicePt, int width,int height);

/** Funci�n que elimina la inclinaci�n vertical de la imagen.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.*/
void RemoveVerticalLinearTiltCUDA(float * devicePt, int width,int height);

/** Funci�n que establece la base de la imagen, eliminando ruido.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.*/
void SetGroundCUDA(float * devicePt, int width,int height);

/** Funci�n que multiplica por un factor y suma un offset a la imagen.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.
    @param offset Valor para sumar.
    @param factor Valor para multiplicar.*/
void OffsetAndMultiplyCUDA(float * devicePt, int width,int height,float offset,float factor);

/** Funci�n que hace la imagen sim�trica horizontalmente, respecto al valor m�ximo de cada fila, centrando dicho valor en la imagen.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.*/
void MakeSymmetricCUDA(float * devicePt, int width,int height);

/** Funci�n que calcula la Inversi�n de Abel horizontalmente de la imagen.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.
    @param dx Distancia entre dos pixeles.
    @param neighbours N�mero de vecinos para calcular la derivada.*/
void AbelInversionCUDA(float * devicePt, int width,int height, float dx,int neighbours);

/** Funci�n que extrae un rect�ngulo de la imagen de la fase.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.
    @param x1 Posici�n horizontal de la esquina superior izquierda de la m�scara.
    @param x2 Posici�n horizontal de la esquina inferior derecha de la m�scara.
    @param y1 Posici�n vertical de la esquina superior izquierda de la m�scara.
    @param y2 Posici�n vertical de la esquina inferior derecha de la m�scara.
    @return Buffer en la memoria de la GPU con la zona seleccionada de tama�o {(x2-x1)+1}x{(y2-y1)+1} responsabilidad del cliente.*/
float * CropRectangleCUDA(float *devicePt,int width,int height,int x1,int y1,int x2, int y2);

/** Funci�n que extrae una recta de la imagen de la fase.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.
    @param x1 Posici�n horizontal de la esquina superior izquierda de la m�scara.
    @param x2 Posici�n horizontal de la esquina inferior derecha de la m�scara.
    @param y1 Posici�n vertical de la esquina superior izquierda de la m�scara.
    @param y2 Posici�n vertical de la esquina inferior derecha de la m�scara.
    @param outputSize N�mero de muestras para la recta.
    @return Buffer en la memoria de la GPU con la zona seleccionada de tama�o outputSize responsabilidad del cliente.*/
float * GetPhaseLineCUDA(float * devicePt,int width,int height,int x1,int y1,int x2, int y2,int outputSize);


#endif // CUDAPHASEINTERFACE_H
