/**
 * @file   cudageneralpurpose.cu
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la cabecera cudageneralpurpose.h. Este fichero se compila con el compilador de CUDA.
 *
 */
#include "cudageneralpurpose.h"


bool IsCudaAvailable(){
    int number;
    if(CudaSafe(cudaGetDeviceCount(&number),"cudaGetDeviceCount") == false || number<1){
        return false;
    }
    return true;
}
