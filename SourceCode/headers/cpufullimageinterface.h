/**
 * @file   cpufullimageinterface.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Cabecera para la interfaz de algoritmos aplicables a un objeto de la clase FullImage implementada con la CPU.
 *
 */
#ifndef CPUFULLIMAGEINTERFACE_H
#define CPUFULLIMAGEINTERFACE_H

#include "complex.h"
#include "stdlib.h"
#include "math.h"

/** Para la transformada de fourier directa.*/
#define FORWARD 1
/** Para la transformada de fourier inverse.*/
#define INVERSE 2

/** Funci�n que aplica una m�scara (Coloca todo a cero excepto los puntos dentro de la m�scara).
    @param linearBuffer Buffer con la estructura compleja alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.
    @param x1 Posici�n horizontal de la esquina superior izquierda de la m�scara.
    @param x2 Posici�n horizontal de la esquina inferior derecha de la m�scara.
    @param y1 Posici�n vertical de la esquina superior izquierda de la m�scara.
    @param y2 Posici�n vertical de la esquina inferior derecha de la m�scara.*/
void ApplyMaskCPU(complex *linearBuffer,int sizeX,int sizeY,int x1,int y1,int x2, int y2);

/** Calcula la transformada r�pida de fourier para la imagen compleja.
    @param linearBuffer Buffer con la estructura compleja alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.
    @param mode FORWARD o INVERSE.*/
void FFTCPU(complex **linearBuffer,int sizeX,int sizeY,int mode);

/** Recoloca la transformada de fourier para su correcto visualizado, con las frecuencia 0 en el centro de la imagen, ya que el algoritmo FFT deja los valores colocados de una forma poco convencional.
    @param linearBuffer Buffer con la estructura compleja alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.
    @return Buffer del mismo tama�o, con los p�xeles recolocada, responsabilidad del cliente.*/
complex *ReArrangeFTCPU(complex *linearBuffer,int sizeX,int sizeY);

/** Calcula el m�dulo del n�mero complejo situado en cada punto.
    @param linearBuffer Buffer con la estructura compleja alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.
    @return Buffer del mismo tama�o, con los valores absolutos de cada pixel, responsabilidad del cliente.*/
float * GetAbsoluteValueCPU(complex *linearBuffer,int sizeX,int sizeY);

/** Obtiene la fase de una zona de la imagen delimitada por una m�scara
    @param linearBuffer Buffer con la estructura compleja alineada fila tras fila.
    @param sizeX Ancho en p�xeles de la imagen.
    @param sizeY Alto en p�xeles de la imagen.
    @param x1 Posici�n horizontal de la esquina superior izquierda de la m�scara.
    @param x2 Posici�n horizontal de la esquina inferior derecha de la m�scara.
    @param y1 Posici�n vertical de la esquina superior izquierda de la m�scara.
    @param y2 Posici�n vertical de la esquina inferior derecha de la m�scara.
    @return Buffer con la fase de la zona seleccionada, de 0 a 2Pi y de tama�o {(x2-x1)+1}x{(y2-y1)+1} responsabilidad del cliente.*/
float * CropPhaseCPU(complex *linearBuffer,int sizeX,int sizeY,int x1,int y1,int x2, int y2);

/** Funci�n auxiliar para calcular la transformada r�pida de fourier para la imagen compleja.
    @param comp Array de punteros a puntero accediendo primero por filas y luego por columnas.
    @param width Ancho en p�xeles de la imagen.
    @param height Alto en p�xeles de la imagen.
    @param dir FORWARD o INVERSE.*/
void FFT2D(complex **comp,int width,int height,int dir);

/** Funci�n auxiliar para calcular la transformada r�pida de fourier unidimensional.
    @param real Array con la parte real del vector cuya transformada se quiere calcular.
    @param imag Array con la parte imaginaria del vector cuya transformada se quiere calcular.
    @param size N�mero de elementos en los vectores.
    @param dir FORWARD o INVERSE.*/
void FFT(float *real, float *imag, int size, int dir);

#endif // CPUFULLIMAGEINTERFACE_H

