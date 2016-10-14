/**
 * @file   cudasafe.cu
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la cabecera cudasafe.h. Este fichero se compila con el compilador de CUDA.
 *
 */
#include "cudasafe.h"


bool CudaSafe(cudaError_t error, char* message){

    if(error!=cudaSuccess) {
        fprintf(stderr,"ERROR: %s : %s\n",message,cudaGetErrorString( error ));
        fflush(stderr);
        return false;
    }
    return true;
}

bool CudaSafeKernel(char* message){
    cudaError error = cudaGetLastError();
    if ( cudaSuccess != error ){
        fprintf(stderr, "ERROR: %s : %s\n", message , cudaGetErrorString( error ) );
        fflush(stderr);
        return false;
    }
    return true;
}

bool CudaSafeFFT(cufftResult error, char* message){
    if(error!=CUFFT_SUCCESS) {
        fprintf(stderr,"ERROR: %s : %s\n",message,cuFFTGetErrorString( error ));
        fflush(stderr);
        return false;
    }
    return true;

}

char * cuFFTGetErrorString(cufftResult error){
    char * errorS;
    errorS=(char *)malloc(30 *sizeof(char));
    switch(error){
        case CUFFT_SUCCESS: strcpy(errorS,"CUFFT_SUCCESS"); break;
        case CUFFT_INVALID_PLAN: strcpy(errorS,"CUFFT_INVALID_PLAN"); break;
        case CUFFT_ALLOC_FAILED: strcpy(errorS,"CUFFT_ALLOC_FAILED"); break;
        case CUFFT_INVALID_TYPE: strcpy(errorS,"CUFFT_INVALID_TYPE"); break;
        case CUFFT_INVALID_VALUE: strcpy(errorS,"CUFFT_INVALID_VALUE"); break;
        case CUFFT_INTERNAL_ERROR: strcpy(errorS,"CUFFT_INTERNAL_ERROR"); break;
        case CUFFT_EXEC_FAILED: strcpy(errorS,"CUFFT_EXEC_FAILED"); break;
        case CUFFT_SETUP_FAILED: strcpy(errorS,"CUFFT_SETUP_FAILED"); break;
        case CUFFT_INVALID_SIZE: strcpy(errorS,"CUFFT_INVALID_SIZE"); break;
        default: strcpy(errorS,"UNKNOWN");
    }

    return errorS;

}
