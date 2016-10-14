/**
 * @file   cudasafe.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Cabecera para la gestión de errores en CUDA.
 *
 */

#ifndef CUDASAFE_H
#define CUDASAFE_H

#include <cuda.h>
#include <stdio.h>
#include "cufft.h"

/** Función que comprueba si hubo errores al ejecutar una función de CUDA, sacando por stderr el error.
    @param error Error obtenido de la llamda a la función de CUDA.
    @param message Cadena terminada con el caracter fin de cadena que se añadirá al error.
    @return Verdadero si no hubo errores.*/
bool CudaSafe(cudaError_t error, char* message);
/** Función que comprueba si hubo errores al ejecutar un kernel de CUDA, sacando por stderr el error.
    @param message Cadena terminada con el caracter fin de cadena que se añadirá al error.
    @return Verdadero si no hubo errores.*/
bool CudaSafeKernel(char* message);
/** Función que comprueba si hubo errores al ejecutar una función de la biblioteca cuFFT, sacando por stderr el error.
    @param error Error obtenido de la llamda a la función de cuFFT.
    @param message Cadena terminada con el caracter fin de cadena que se añadirá al error.
    @return Verdadero si no hubo errores.*/
bool CudaSafeFFT(cufftResult error, char* message);
/** Función que devuelve la cadena de error asociada a un error e cuFFT.
    @param error Error obtenido de la llamda a la función de cuFFT.
    @return Cadena terminada en caracter fin de cadena con la descripción del error.*/
char * cuFFTGetErrorString(cufftResult error);


#endif // CUDASAFE_H
