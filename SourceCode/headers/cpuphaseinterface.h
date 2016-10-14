/**
 * @file   cpuphaseinterface.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
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

/** Funci�n que escanea la fase, para eliminar los saltos de 2Pi a 0 y hacerla cont�nua.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.*/
void ScanPhaseCPU(float * linearBuffer, int sizeX,int sizeY);

/** Funci�n que resta a la imagen de la fase, otra imagen de la fase.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param offsetLinearBuffer Buffer con con la imagen de la fase que se va a restar.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.*/
void SubtractCPU(float * linearBuffer,float * offsetLinearBuffer, int sizeX,int sizeY);

/** Funci�n que asigna a cada valor de la imagen su opuesto.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.*/
void AdditiveInverseCPU(float * linearBuffer, int sizeX,int sizeY);

/** Funci�n que elimina la inclinaci�n horizontal de la imagen.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.*/
void RemoveHorizontalLinearTiltCPU(float * linearBuffer, int sizeX,int sizeY);

/** Funci�n que elimina la inclinaci�n vertical de la imagen.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.*/
void RemoveVerticalLinearTiltCPU(float * linearBuffer, int sizeX,int sizeY);

/** Funci�n que establece la base de la imagen, eliminando ruido.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.*/
void SetGroundCPU(float * linearBuffer, int sizeX,int sizeY);

/** Funci�n que multiplica por un factor y suma un offset a la imagen.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.
    @param offset Valor para sumar.
    @param factor Valor para multiplicar.*/
void OffsetAndMultiplyCPU(float * linearBuffer, int sizeX,int sizeY,float offset,float factor);

/** Funci�n que hace la imagen sim�trica horizontalmente, respecto al valor m�ximo de cada fila, centrando dicho valor en la imagen.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.*/
void MakeSymmetricCPU(float * linearBuffer, int sizeX,int sizeY);

/** Funci�n que calcula la Inversi�n de Abel horizontalmente de la imagen.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.
    @param dx Distancia entre dos pixeles.
    @param neighbours N�mero de vecinos para calcular la derivada.*/
void AbelInversionCPU(float * linearBuffer, int sizeX,int sizeY, float dx,int neighbours);

/** Funci�n que extrae un rect�ngulo de la imagen de la fase.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.
    @param x1 Posici�n horizontal de la esquina superior izquierda de la m�scara.
    @param x2 Posici�n horizontal de la esquina inferior derecha de la m�scara.
    @param y1 Posici�n vertical de la esquina superior izquierda de la m�scara.
    @param y2 Posici�n vertical de la esquina inferior derecha de la m�scara.
    @return Buffer con la zona seleccionada de tama�o {(x2-x1)+1}x{(y2-y1)+1} responsabilidad del cliente.*/
float * CropRectangleCPU(float *linearBuffer,int sizeX,int sizeY,int x1,int y1,int x2, int y2);

/** Funci�n que extrae una recta de la imagen de la fase.
    @param linearBuffer Buffer con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.
    @param x1 Posici�n horizontal de la esquina superior izquierda de la m�scara.
    @param x2 Posici�n horizontal de la esquina inferior derecha de la m�scara.
    @param y1 Posici�n vertical de la esquina superior izquierda de la m�scara.
    @param y2 Posici�n vertical de la esquina inferior derecha de la m�scara.
    @param outputSize N�mero de muestras para la recta.
    @return Buffer con la zona seleccionada de tama�o outputSize responsabilidad del cliente.*/
float * GetPhaseLineCPU(float * linearBuffer,int sizeX,int sizeY,int x1,int y1,int x2, int y2,int outputSize);

#endif // CPUPHASEINTERFACE_H
