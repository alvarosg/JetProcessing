/**
 * @file   cudafullimageinterface.cu
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la cabecera cudafullimageinterface.h. Este fichero se compila con el compilador de CUDA.
 *
 */
#include "cudafullimageinterface.h"


//Kernels

extern "C"
/** Kernel que se ejecuta en la tarjeta gráfica para obtener el valor absoluto de la imagen compleja.
    @param source Buffer en la memoria de la GPU con la imagen compleja alineada fila tras fila.
    @param result Zona de memoria para el resultado.
    @param sizeX Ancho en píxeles de la imagen compleja.
    @param sizeY Alto en píxeles de la imagen compleja.*/
__global__ void AbsoluteValueKernel(complex * source, float * result,int sizeX,int sizeY){
    float real,imag;
    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;

    if (x<sizeX && y<sizeY){
        real=source[x+sizeX*y].x;
        imag=source[x+sizeX*y].y;
        result[x+sizeX*y] = sqrtf(real*real+imag*imag);
    }
}

/** Kernel que se ejecuta en la tarjeta gráfica para aplicar una máscara a la imagen compleja.
    @param source Buffer en la memoria de la GPU con la imagen compleja alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen compleja.
    @param sizeY Alto en píxeles de la imagen compleja.
    @param x1 Posición horizontal de la esquina superior izquierda de la máscara.
    @param x2 Posición horizontal de la esquina inferior derecha de la máscara.
    @param y1 Posición vertical de la esquina superior izquierda de la máscara.
    @param y2 Posición vertical de la esquina inferior derecha de la máscara.*/
__global__ void ApplyMaskKernel(complex * source,int sizeX,int sizeY,int x1,int y1,int x2, int y2){
    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;

    if (x<sizeX && y<sizeY && (x<x1 || x>x2 || y<y1 || y>y2)){
        source[x+sizeX*y].x=0;
        source[x+sizeX*y].y=0;
    }
}

/** Kernel que recoloca la imagen compleja tras haber realizado una transformada de fourier.
    @param source Buffer en la memoria de la GPU con la imagen compleja alineada fila tras fila.
    @param result Zona de memoria para el resultado.
    @param sizeX Ancho en píxeles de la imagen compleja.
    @param sizeY Alto en píxeles de la imagen compleja.*/
__global__ void ReArrangeFTKernel(complex * source, complex * result,int sizeX,int sizeY){

    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;
    int xAux,yAux;

    if(x<sizeX/2)
        xAux=x+sizeX/2;
    else
        xAux=x-sizeX/2;
    if(y<sizeY/2)
        yAux=y+sizeY/2;
    else
        yAux=y-sizeY/2;


    if (x<sizeX && y<sizeY && xAux<sizeX && yAux<sizeY ){
        result[xAux+sizeX*yAux]=source[x+sizeX*y];
    }
}

/** Kernel que extrae la fase de una parte de la imagen compleja delimitada por uan máscara.
    @param source Buffer en la memoria de la GPU con la imagen compleja alineada fila tras fila.
    @param result Zona de memoria para el resultado.
    @param sizeX Ancho en píxeles de la imagen compleja.
    @param sizeY Alto en píxeles de la imagen compleja.
    @param x1 Posición horizontal de la esquina superior izquierda de la máscara.
    @param x2 Posición horizontal de la esquina inferior derecha de la máscara.
    @param y1 Posición vertical de la esquina superior izquierda de la máscara.
    @param y2 Posición vertical de la esquina inferior derecha de la máscara.*/
__global__ void CropPhaseKernel(complex * source,float * result,int sizeX,int sizeY,int x1,int y1,int x2, int y2){

    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;

    int xAux,yAux;
    float real,imag,phase;

    //Sólo calculo y guardo la fase de los puntos dentro de la máscara

    if (x>=x1 && y>=y1 && x<=x2 && y<=y2 && x<sizeX && y<sizeY){
        xAux=x-x1;
        yAux=y-y1;
        real=source[x+y*sizeX].x;
        imag=source[x+y*sizeX].y;
        //Paso a modulo 2Pi
        phase=atan2f(imag,real);

        //Poner en el intervalo (0,2PI)
        if (phase<0)
            phase+=2*PI_NUM;

        result[xAux+yAux*(x2-x1+1)]=phase;

    }

}


complex * LoadInGPUCUDA(complex * hostPt,int width,int height){

    complex * devicePt;
    CudaSafe( cudaMalloc((void **)&devicePt,sizeof(complex)*height*width),"cudaMalloc") ;
    CudaSafe( cudaMemcpy(devicePt, hostPt, sizeof(complex)*height*width,cudaMemcpyHostToDevice),"cudaMemcpy");

    return devicePt;
}

complex * MemCpyCUDA(complex * devicePt,int width,int height){
    complex * returnDevicePt;
    CudaSafe( cudaMalloc((void **)&returnDevicePt,sizeof(complex)*height*width),"cudaMalloc") ;
    CudaSafe( cudaMemcpy(returnDevicePt, devicePt, sizeof(complex)*height*width,cudaMemcpyDeviceToDevice),"cudaMemcpy");
    return returnDevicePt;
}

void FreeCUDA(complex * devicePt){

    CudaSafe( cudaFree(devicePt),"cudaFree");
}


void GetBufferBackCUDA(complex * hostPt, complex * devicePt, int width,int height){

    CudaSafe( cudaMemcpy(hostPt, devicePt, sizeof(complex)*height*width, cudaMemcpyDeviceToHost),"cudaMemcpy");

}

void ApplyMaskCUDA(complex *devicePt,int width,int height,int x1,int y1,int x2, int y2){
    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;

    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);
    ApplyMaskKernel <<< blocks, threads >>> (devicePt,width,height,x1,y1,x2,y2);

}



void FFTCUDA(complex * devicePt,int width,int height, int mode){
    cufftHandle plan;

    CudaSafeFFT(cufftPlan2d(&plan, height, width, CUFFT_C2C),"cufftPlan2d");

    if (mode==FORWARD)
        CudaSafeFFT(cufftExecC2C(plan, (cufftComplex *)devicePt, (cufftComplex *)devicePt, CUFFT_FORWARD),"cufftExecC2C");
    else if (mode==INVERSE)
        CudaSafeFFT(cufftExecC2C(plan, (cufftComplex *)devicePt, (cufftComplex *)devicePt, CUFFT_INVERSE),"cufftExecC2C");

    CudaSafeFFT(cufftDestroy(plan),"cufftDestroy");

}

complex * ReArrangeFTCUDA(complex * devicePt,int width,int height){
    complex * resultDevice;

    CudaSafe( cudaMalloc((void **)&resultDevice,sizeof(complex)*height*width),"cudaMalloc");
    //Con ese truco consigo un redondeo hacia arriba en la división
    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;

    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);
    ReArrangeFTKernel <<< blocks, threads >>> (devicePt, resultDevice,width,height);
    CudaSafeKernel("ReArrangeFT");

    return resultDevice;

}

float * GetAbsoluteValueCUDA(complex *devicePt,int width,int height){

    float * resultHost, * resultDevice;
    resultHost=(float *)malloc(sizeof(float)*height*width);
    CudaSafe( cudaMalloc((void **)&resultDevice,sizeof(float)*height*width),"cudaMalloc");
    //Con ese truco consigo un redondeo hacia arriba en la división
    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;

    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);
    AbsoluteValueKernel <<< blocks, threads >>> (devicePt, resultDevice,width,height);
    CudaSafeKernel("AbsoluteValue");

    CudaSafe( cudaMemcpy(resultHost, resultDevice, sizeof(float)*height*width, cudaMemcpyDeviceToHost),"cudaMemcpy");
    CudaSafe( cudaFree(resultDevice),"cudaFree");

    return resultHost;
}


float * CropPhaseCUDA(complex *devicePt,int width,int height,int x1,int y1,int x2, int y2){
    float * resultDevice;

    CudaSafe( cudaMalloc((void **)&resultDevice,sizeof(float)*(x2-x1+1)*(y2-y1+1)),"cudaMalloc");

    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;

    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);
    CropPhaseKernel <<< blocks, threads >>> (devicePt, resultDevice,width,height,x1,y1,x2,y2);
    CudaSafeKernel("CropPhase");

    return resultDevice;

}
