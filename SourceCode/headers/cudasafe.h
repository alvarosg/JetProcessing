/**
 * @file   cudasafe.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Cabecera para la gesti�n de errores en CUDA.
 *
 */

#ifndef CUDASAFE_H
#define CUDASAFE_H

#include <cuda.h>
#include <stdio.h>
#include "cufft.h"

/** Funci�n que comprueba si hubo errores al ejecutar una funci�n de CUDA, sacando por stderr el error.
    @param error Error obtenido de la llamda a la funci�n de CUDA.
    @param message Cadena terminada con el caracter fin de cadena que se a�adir� al error.
    @return Verdadero si no hubo errores.*/
bool CudaSafe(cudaError_t error, char* message);
/** Funci�n que comprueba si hubo errores al ejecutar un kernel de CUDA, sacando por stderr el error.
    @param message Cadena terminada con el caracter fin de cadena que se a�adir� al error.
    @return Verdadero si no hubo errores.*/
bool CudaSafeKernel(char* message);
/** Funci�n que comprueba si hubo errores al ejecutar una funci�n de la biblioteca cuFFT, sacando por stderr el error.
    @param error Error obtenido de la llamda a la funci�n de cuFFT.
    @param message Cadena terminada con el caracter fin de cadena que se a�adir� al error.
    @return Verdadero si no hubo errores.*/
bool CudaSafeFFT(cufftResult error, char* message);
/** Funci�n que devuelve la cadena de error asociada a un error e cuFFT.
    @param error Error obtenido de la llamda a la funci�n de cuFFT.
    @return Cadena terminada en caracter fin de cadena con la descripci�n del error.*/
char * cuFFTGetErrorString(cufftResult error);


#endif // CUDASAFE_H
