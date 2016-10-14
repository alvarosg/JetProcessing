/**
 * @file   cudaphaseinterface.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Cabecera para la interfaz de algoritmos aplicables a un objeto de la clase tipo PhaseImage implementada con CUDA.
 *
 */

#ifndef CUDAPHASEINTERFACE_H
#define CUDAPHASEINTERFACE_H


#include <cuda.h>
#include "complex.h"
#include "cudasafe.h"

/** Función que carga una imagen de la fase en la memoria de la GPU.
    @param hostPt Buffer en la memoria de la CPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.
    @return Puntero a la copia en la GPU.*/
float * LoadInGPUCUDA(float * hostPt,int width,int height);

/** Función que crea una copia de la imagen de la fase en la GPU a partir de otra también en la GPU.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase a alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.
    @return Puntero a la copia en la GPU.*/
float * MemCpyCUDA(float * devicePt,int width,int height);

/** Función que libera una zona de memoria en la GPU.
    @param devicePt Puntero a la zona de memoria d ela GPU a liberar.
    */
void FreeCUDA(float * devicePt);

/** Función que crea una copia de una imagen de la fase en la CPU a partir de otra en la GPU.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param hostPt Buffer en la memoria de la CPU donde será copiada la imagen. Debe apuntar a una zona de memoria suficientemente grande.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen. */
void GetBufferBackCUDA(float * hostPt, float * devicePt, int width,int height);

/** Función que escanea la fase, para eliminar los saltos de 2Pi a 0 y hacerla contínua.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.*/
void ScanPhaseCUDA(float * devicePt, int width,int height);

/** Función que resta a la imagen de la fase, otra imagen de la fase.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param offsetDevicePt Buffer en la memoria de la GPU con con la imagen de la fase que se va a restar.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.*/
void SubtractCUDA(float * devicePt,float * offsetDevicePt, int width,int height);

/** Función que asigna a cada valor de la imagen su opuesto.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.*/
void AdditiveInverseCUDA(float * devicePt, int width,int height);

/** Función que elimina la inclinación horizontal de la imagen basándose en los valores de los bordes.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.*/
void RemoveHorizontalLinearTiltBordersCUDA(float * devicePt, int width,int height);

/** Función que elimina la inclinación horizontal de la imagen.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.*/
void RemoveHorizontalLinearTiltCUDA(float * devicePt, int width,int height);

/** Función que elimina la inclinación vertical de la imagen.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.*/
void RemoveVerticalLinearTiltCUDA(float * devicePt, int width,int height);

/** Función que establece la base de la imagen, eliminando ruido.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.*/
void SetGroundCUDA(float * devicePt, int width,int height);

/** Función que multiplica por un factor y suma un offset a la imagen.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.
    @param offset Valor para sumar.
    @param factor Valor para multiplicar.*/
void OffsetAndMultiplyCUDA(float * devicePt, int width,int height,float offset,float factor);

/** Función que hace la imagen simétrica horizontalmente, respecto al valor máximo de cada fila, centrando dicho valor en la imagen.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.*/
void MakeSymmetricCUDA(float * devicePt, int width,int height);

/** Función que calcula la Inversión de Abel horizontalmente de la imagen.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.
    @param dx Distancia entre dos pixeles.
    @param neighbours Número de vecinos para calcular la derivada.*/
void AbelInversionCUDA(float * devicePt, int width,int height, float dx,int neighbours);

/** Función que extrae un rectángulo de la imagen de la fase.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.
    @param x1 Posición horizontal de la esquina superior izquierda de la máscara.
    @param x2 Posición horizontal de la esquina inferior derecha de la máscara.
    @param y1 Posición vertical de la esquina superior izquierda de la máscara.
    @param y2 Posición vertical de la esquina inferior derecha de la máscara.
    @return Buffer en la memoria de la GPU con la zona seleccionada de tamaño {(x2-x1)+1}x{(y2-y1)+1} responsabilidad del cliente.*/
float * CropRectangleCUDA(float *devicePt,int width,int height,int x1,int y1,int x2, int y2);

/** Función que extrae una recta de la imagen de la fase.
    @param devicePt Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.
    @param x1 Posición horizontal de la esquina superior izquierda de la máscara.
    @param x2 Posición horizontal de la esquina inferior derecha de la máscara.
    @param y1 Posición vertical de la esquina superior izquierda de la máscara.
    @param y2 Posición vertical de la esquina inferior derecha de la máscara.
    @param outputSize Número de muestras para la recta.
    @return Buffer en la memoria de la GPU con la zona seleccionada de tamaño outputSize responsabilidad del cliente.*/
float * GetPhaseLineCUDA(float * devicePt,int width,int height,int x1,int y1,int x2, int y2,int outputSize);


#endif // CUDAPHASEINTERFACE_H
