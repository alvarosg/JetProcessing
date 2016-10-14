/**
 * @file   cpuphaseinterface.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Cabecera para la interfaz de algoritmos aplicables a un objeto de la clase PhaseImage implementada con la CPU.
 *
 */
#ifndef CPUPHASEINTERFACE_H
#define CPUPHASEINTERFACE_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "complex.h"

/** Función que escanea la fase, para eliminar los saltos de 2Pi a 0 y hacerla contínua.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.*/
void ScanPhaseCPU(float * linearBuffer, int sizeX,int sizeY);

/** Función que resta a la imagen de la fase, otra imagen de la fase.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param offsetLinearBuffer Buffer con con la imagen de la fase que se va a restar.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.*/
void SubtractCPU(float * linearBuffer,float * offsetLinearBuffer, int sizeX,int sizeY);

/** Función que asigna a cada valor de la imagen su opuesto.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.*/
void AdditiveInverseCPU(float * linearBuffer, int sizeX,int sizeY);

/** Función que elimina la inclinación horizontal de la imagen.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.*/
void RemoveHorizontalLinearTiltCPU(float * linearBuffer, int sizeX,int sizeY);

/** Función que elimina la inclinación vertical de la imagen.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.*/
void RemoveVerticalLinearTiltCPU(float * linearBuffer, int sizeX,int sizeY);

/** Función que establece la base de la imagen, eliminando ruido.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.*/
void SetGroundCPU(float * linearBuffer, int sizeX,int sizeY);

/** Función que multiplica por un factor y suma un offset a la imagen.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param offset Valor para sumar.
    @param factor Valor para multiplicar.*/
void OffsetAndMultiplyCPU(float * linearBuffer, int sizeX,int sizeY,float offset,float factor);

/** Función que hace la imagen simétrica horizontalmente, respecto al valor máximo de cada fila, centrando dicho valor en la imagen.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.*/
void MakeSymmetricCPU(float * linearBuffer, int sizeX,int sizeY);

/** Función que calcula la Inversión de Abel horizontalmente de la imagen.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param dx Distancia entre dos pixeles.
    @param neighbours Número de vecinos para calcular la derivada.*/
void AbelInversionCPU(float * linearBuffer, int sizeX,int sizeY, float dx,int neighbours);

/** Función que extrae un rectángulo de la imagen de la fase.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param x1 Posición horizontal de la esquina superior izquierda de la máscara.
    @param x2 Posición horizontal de la esquina inferior derecha de la máscara.
    @param y1 Posición vertical de la esquina superior izquierda de la máscara.
    @param y2 Posición vertical de la esquina inferior derecha de la máscara.
    @return Buffer con la zona seleccionada de tamaño {(x2-x1)+1}x{(y2-y1)+1} responsabilidad del cliente.*/
float * CropRectangleCPU(float *linearBuffer,int sizeX,int sizeY,int x1,int y1,int x2, int y2);

/** Función que extrae una recta de la imagen de la fase.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param x1 Posición horizontal de la esquina superior izquierda de la máscara.
    @param x2 Posición horizontal de la esquina inferior derecha de la máscara.
    @param y1 Posición vertical de la esquina superior izquierda de la máscara.
    @param y2 Posición vertical de la esquina inferior derecha de la máscara.
    @param outputSize Número de muestras para la recta.
    @return Buffer con la zona seleccionada de tamaño outputSize responsabilidad del cliente.*/
float * GetPhaseLineCPU(float * linearBuffer,int sizeX,int sizeY,int x1,int y1,int x2, int y2,int outputSize);

#endif // CPUPHASEINTERFACE_H
