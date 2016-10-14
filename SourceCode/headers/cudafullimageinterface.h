/**
 * @file   cudafullimageinterface.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Cabecera para la interfaz de algoritmos aplicables a un objeto de la clase FullImage implementada con CUDA.
 *
 */
#ifndef CUDAFULLIMAGEINTERFACE_H
#define CUDAFULLIMAGEINTERFACE_H


#include <cuda.h>
#include "complex.h"
#include "cufft.h"
#include "cudasafe.h"

/** Para la transformada de fourier directa.*/
#define FORWARD 1
/** Para la transformada de fourier inversa.*/
#define INVERSE 2

/** Función que carga una matriz compleja en la memoria de la GPU.
    @param hostPt Buffer en la memoria de la CPU con la estructura compleja alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.
    @return Puntero a la copia en la GPU.*/
complex * LoadInGPUCUDA(complex * hostPt,int width,int height);

/** Función que crea una copia de una matriz compleja en la GPU a partir de otra también en la GPU.
    @param devicePt Buffer en la memoria de la GPU con la estructura compleja alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.
    @return Puntero a la copia en la GPU.*/
complex * MemCpyCUDA(complex * devicePt,int width,int height);

/** Función que libera una zona de memoria en la GPU.
    @param devicePt Puntero a la zona de memoria d ela GPU a liberar.*/
void FreeCUDA(complex * devicePt);

/** Función que crea una copia de una matriz compleja en la CPU a partir de otra en la GPU.
    @param devicePt Buffer en la memoria de la GPU con la estructura compleja alineada fila tras fila.
    @param hostPt Buffer en la memoria de la CPU donde será copiada la estructura. Debe apuntar a una zona de memoria suficientemente grande.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.*/
void GetBufferBackCUDA(complex * hostPt,complex * devicePt,int width,int height);

/** Función que aplica una máscara (Coloca todo a cero excepto los puntos dentro de la máscara).
    @param devicePt Buffer en la memoria de la GPU con la estructura compleja alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.
    @param x1 Posición horizontal de la esquina superior izquierda de la máscara.
    @param x2 Posición horizontal de la esquina inferior derecha de la máscara.
    @param y1 Posición vertical de la esquina superior izquierda de la máscara.
    @param y2 Posición vertical de la esquina inferior derecha de la máscara.*/
void ApplyMaskCUDA(complex *devicePt,int width,int height,int x1,int y1,int x2, int y2);

/** Calcula la transformada rápida de fourier para la imagen compleja.
    @param devicePt Buffer en la memoria de la GPU con la estructura compleja alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.
    @param mode FORWARD o INVERSE.*/
void FFTCUDA(complex *devicePt,int width,int height,int mode);

/** Recoloca la transformada de fourier para su correcto visualizado, con las frecuencia 0 en el centro de la imagen, ya que el algoritmo FFT deja los valores colocados de una forma poco convencional.
    @param devicePt Buffer en la memoria de la GPU con la estructura compleja alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.
    @return Buffer del mismo tamaño, con los píxeles recolocada, responsabilidad del cliente.*/
complex *ReArrangeFTCUDA(complex * devicePt,int width, int height);

/** Calcula el módulo del número complejo situado en cada punto.
    @param devicePt Buffer en la memoria de la GPU con la estructura compleja alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.
    @return Buffer en la memoria de la CPU del mismo tamaño, con los valores absolutos de cada pixel, responsabilidad del cliente.*/
float * GetAbsoluteValueCUDA(complex *devicePt,int width,int height);

/** Obtiene la fase de una zona de la imagen delimitada por una máscara
    @param devicePt Buffer en la memoria de la GPU con la estructura compleja alineada fila tras fila.
    @param width Ancho en píxeles de la imagen.
    @param height Alto en píxeles de la imagen.
    @param x1 Posición horizontal de la esquina superior izquierda de la máscara.
    @param x2 Posición horizontal de la esquina inferior derecha de la máscara.
    @param y1 Posición vertical de la esquina superior izquierda de la máscara.
    @param y2 Posición vertical de la esquina inferior derecha de la máscara.
    @return Buffer en la memoria de la GPU con la fase de la zona seleccionada, de 0 a 2Pi y de tamaño {(x2-x1)+1}x{(y2-y1)+1} responsabilidad del cliente */
float * CropPhaseCUDA(complex *devicePt,int width,int height,int x1,int y1,int x2, int y2);

#endif // CUDAFULLIMAGEINTERFACE_H
