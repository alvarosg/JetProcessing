/**
 * @file   cudageneralpurpose.cu
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de la cabecera cudageneralpurpose.h. Este fichero se compila con el compilador de CUDA.
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
