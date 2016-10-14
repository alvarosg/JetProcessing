/**
 * @file   cudageneralpurpose.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Cabecera con funciones de uso general relacionadas con el uso de CUDA.
 *
 */

#ifndef CUDAGENERALPURPOSE_H
#define CUDAGENERALPURPOSE_H

#include <cuda.h>
#include "cudasafe.h"

/** Función que indica si CUDA está disponible en el sistema.
    @return Verdadero si está disponible.*/
bool IsCudaAvailable();

#endif // CUDAGENERALPURPOSE_H
