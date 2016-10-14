/**
 * @file   cudageneralpurpose.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Cabecera con funciones de uso general relacionadas con el uso de CUDA.
 *
 */

#ifndef CUDAGENERALPURPOSE_H
#define CUDAGENERALPURPOSE_H

#include <cuda.h>
#include "cudasafe.h"

/** Funci�n que indica si CUDA est� disponible en el sistema.
    @return Verdadero si est� disponible.*/
bool IsCudaAvailable();

#endif // CUDAGENERALPURPOSE_H
