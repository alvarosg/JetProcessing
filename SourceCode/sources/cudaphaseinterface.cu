/**
 * @file   cudaphaseinterface.cu
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la cabecera cudaphaseinterface.h. Este fichero se compila con el compilador de CUDA.
 *
 */
#include "cudaphaseinterface.h"


//Kernels

extern "C"

/** Kernel que se ejecuta en la tarjeta gráfica como primer paso para escanear la fase.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param accumulator Zona de memoria que contiene el número de saltos de fase encontrados hasta llegar a cada punto.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.*/
__global__ void ScanPhaseStep1Kernel(float * source,int * accumulator, int sizeX,int sizeY){
    int i = threadIdx.x;
    int x;
    int y=(sizeY/2);
    //El hilo número 0 asigna al punto central un offset de 0
    if(i==0)
        accumulator[sizeX/2+sizeX*y]=0;

    //Sincronizo los hilos
    __syncthreads();

    //Voy calculando secuencialmente por fila intermedia si hay grandes saltos en la fase
    //y en base a ello aumento o decremento el acumulador para cada punto
    //El hilo 0 va hacia la izquierda y el hilo 1 hacia la derecha
    if(i==0){
        for(x=sizeX/2-1;x>=0;x--){
            accumulator[x+sizeX*y]=accumulator[x+1+sizeX*y];

            if      ((source[x+sizeX*y]-source[x+1+sizeX*y])>PI_NUM)
                accumulator[x+sizeX*y]--;

            else if ((source[x+1+sizeX*y]-source[x+sizeX*y])>PI_NUM)
                accumulator[x+sizeX*y]++;
        }
    }
    else if(i==1){
        for(x=sizeX/2+1;x<sizeX;x++){
            accumulator[x+sizeX*y]=accumulator[x-1+sizeX*y];

            if      ((source[x+sizeX*y]-source[x-1+sizeX*y])>PI_NUM)
                accumulator[x+sizeX*y]--;

            else if ((source[x-1+sizeX*y]-source[x+sizeX*y])>PI_NUM)
                accumulator[x+sizeX*y]++;
        }
    }
}

/** Kernel que se ejecuta en la tarjeta gráfica como segundo paso para escanear la fase.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param accumulator Zona de memoria que contiene el número de saltos de fase encontrados hasta llegar a cada punto.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.*/
__global__ void ScanPhaseStep2Kernel(float * source,int * accumulator, int sizeX,int sizeY){

    int x = threadIdx.x + blockIdx.x*blockDim.x;
    int y;

    //A partir de los datos de la línea central, voy calculando hacia arriba(bloques de índice menos que el ancho)
    //y hacia abajo (bloques de índice mayor que el ancho), en paralelo para
    //cada linea vertical y dirección, los saltos de fase

    //Detecto la dirección y asigno la columna para los bloques cuyo índice esté en la segunda mitad
    //Hacia arriba
    if(x < (2*sizeX)){
        int dir=x/sizeX;
        x=x%sizeX;
        if(dir==0){
            for(y=sizeY/2-1;y>=0;y--){
                accumulator[x+y*sizeX]=accumulator[x+(y+1)*sizeX];

                if      ((source[x+y*sizeX]-source[x+(y+1)*sizeX])>PI_NUM)
                    accumulator[x+y*sizeX]--;

                else if ((source[x+(y+1)*sizeX]-source[x+y*sizeX])>PI_NUM)
                    accumulator[x+y*sizeX]++;
            }
        }
        //Hacia abajo
        else if(dir==1){
            for(y=sizeY/2+1;y<sizeY;y++){
                accumulator[x+y*sizeX]=accumulator[x+(y-1)*sizeX];

                if      ((source[x+y*sizeX]-source[x+(y-1)*sizeX])>PI_NUM)
                    accumulator[x+y*sizeX]--;

                else if ((source[x+(y-1)*sizeX]-source[x+y*sizeX])>PI_NUM)
                    accumulator[x+y*sizeX]++;
            }
        }
    }
}

/** Kernel que se ejecuta en la tarjeta gráfica como tercer paso para escanear la fase.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param accumulator Zona de memoria que contiene el número de saltos de fase encontrados hasta llegar a cada punto.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.*/
__global__ void ScanPhaseStep3Kernel(float * source,int * accumulator, int sizeX,int sizeY){
    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;

    //Sumo múltiplos de 2Pi en función de la fase acumulada

    if(x<sizeX && y<sizeY){

        source[x+y*sizeX]=source[x+y*sizeX]+((float)(accumulator[x+y*sizeX]))*2*PI_NUM;
    }
}

/** Kernel que se ejecuta en la tarjeta gráfica para restar la fase de la referencia.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param offset Buffer con la imagen de la fase de la referencia alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.*/
__global__ void SubtractKernel(float * source,float * offset, int sizeX,int sizeY){
    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;

    //Sumo múltiplos de 2Pi en función de la fase acumulada

    if(x<sizeX && y<sizeY){

        source[x+y*sizeX]=source[x+y*sizeX]-offset[x+y*sizeX];
    }
}
/** Kernel que se ejecuta en la tarjeta gráfica para calcular el opuesto de la fase.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.*/
__global__ void AdditiveInverseKernel(float * source,int sizeX,int sizeY){
    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;

    //Sumo múltiplos de 2Pi en función de la fase acumulada

    if(x<sizeX && y<sizeY){

        source[x+y*sizeX]=-source[x+y*sizeX];
    }
}

/** Kernel que se ejecuta en la tarjeta gráfica como primer paso para eliminar la inclinación lineal horizontal usando los bordes.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param avgSlope Puntero a la zona de memoria donde se escribirá la inclinación media.*/
__global__ void RemoveHorizontalLinearTiltBordersStep1Kernel(float * source,int sizeX,int sizeY,float * avgSlope){
    int y;

    //Recorro cada línea horizontal calculando la pendiente entre el primer valor y el último
    (*avgSlope)=0;
    for (y=0;y<sizeY;y++){
        (*avgSlope)+=(source[(sizeX-1)+y*sizeX]-source[y*sizeX])/(sizeX-1);
    }
    (*avgSlope)=(*avgSlope)/sizeY;
}
/** Kernel que se ejecuta en la tarjeta gráfica como segundo paso para eliminar la inclinación lineal horizontal usando los bordes.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param avgSlope Puntero a la zona de memoria con la inclinación media.*/
__global__ void RemoveHorizontalLinearTiltBordersStep2Kernel(float * source,int sizeX,int sizeY,float * avgSlope){
    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;

    //Aplico a cada punto la corrección en función de la inclinación
    if(x<sizeX && y<sizeY){
        source[x+y*sizeX]=source[x+y*sizeX]-(*avgSlope)*(x-sizeX/2);
    }
}

/** Kernel que se ejecuta en la tarjeta gráfica como primer paso para eliminar la inclinación lineal horizontal.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param slopesArray Puntero a la zona de memoria donde se escribirá la inclinación de cada una de las rectas.*/
__global__ void RemoveHorizontalLinearTiltStep1Kernel(float * source,int sizeX,int sizeY, float * slopesArray){
    int x;
    int y = threadIdx.x+blockIdx.x*blockDim.x;
    //Calculo la pendiente de cada recta horizontal con una regresión
    float sumX=0;
    float sumX2=0;
    float sumZ=0;
    float sumXZ=0;
    if(y<sizeY){
        for (x=0;x<sizeX;x++){
            sumX+=x;
            sumX2+=x*x;
            sumZ+=source[x+sizeX*y];
            sumXZ+=source[x+sizeX*y]*x;
        }
        slopesArray[y]=(sumX*sumZ-sizeX*sumXZ)/(sumX*sumX-sizeX*sumX2);
    }
}

/** Kernel que se ejecuta en la tarjeta gráfica como segundo paso para eliminar la inclinación lineal horizontal.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param avgSlope Puntero a la zona de memoria donde se escribirá la inclinación media.
    @param slopesArray Puntero a la zona de memoria con la inclinación de cada una de las rectas.*/
__global__ void RemoveHorizontalLinearTiltStep2Kernel(float * source,int sizeX,int sizeY,float * avgSlope, float * slopesArray){
    int y;

    //Hago una media de las pendientes
    (*avgSlope)=0;
    for (y=0;y<sizeY;y++){
        (*avgSlope)+=slopesArray[y];
    }
    (*avgSlope)=(*avgSlope)/sizeY;
}

/** Kernel que se ejecuta en la tarjeta gráfica como tercer paso para eliminar la inclinación lineal horizontal.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param avgSlope Puntero a la zona de memoria con la inclinación media.*/
__global__ void RemoveHorizontalLinearTiltStep3Kernel(float * source,int sizeX,int sizeY,float * avgSlope){
    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;

    //Aplico a cada punto la corrección en función de la inclinación
    if(x<sizeX && y<sizeY){
        source[x+y*sizeX]=source[x+y*sizeX]-(*avgSlope)*(x-sizeX/2);
    }
}

/** Kernel que se ejecuta en la tarjeta gráfica como primer paso para eliminar la inclinación lineal vertical.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param slopesArray Puntero a la zona de memoria donde se escribirá la inclinación de cada una de las rectas.*/
__global__ void RemoveVerticalLinearTiltStep1Kernel(float * source,int sizeX,int sizeY, float * slopesArray){
    int y;
    int x = threadIdx.x+blockIdx.x*blockDim.x;
    //Calculo la pendiente de cada recta vertical con una regresión
    float sumY=0;
    float sumY2=0;
    float sumZ=0;
    float sumYZ=0;
    if(x<sizeX){
        for (y=0;y<sizeY;y++){
            sumY+=y;
            sumY2+=y*y;
            sumZ+=source[x+sizeX*y];
            sumYZ+=source[x+sizeX*y]*y;
        }
        slopesArray[x]=(sumY*sumZ-sizeY*sumYZ)/(sumY*sumY-sizeY*sumY2);
    }
}

/** Kernel que se ejecuta en la tarjeta gráfica como segundo paso para eliminar la inclinación lineal vertical.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param avgSlope Puntero a la zona de memoria donde se escribirá la inclinación media.
    @param slopesArray Puntero a la zona de memoria con la inclinación de cada una de las rectas.*/
__global__ void RemoveVerticalLinearTiltStep2Kernel(float * source,int sizeX,int sizeY,float * avgSlope, float * slopesArray){
    int x;

    //Hago una media de las pendientes
    (*avgSlope)=0;
    for (x=0;x<sizeX;x++){
        (*avgSlope)+=slopesArray[x];
    }
    (*avgSlope)=(*avgSlope)/sizeX;
}

/** Kernel que se ejecuta en la tarjeta gráfica como tercer paso para eliminar la inclinación lineal vertical.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param avgSlope Puntero a la zona de memoria con la inclinación media.*/
__global__ void RemoveVerticalLinearTiltStep3Kernel(float * source,int sizeX,int sizeY,float * avgSlope){
    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;

    //Aplico a cada punto la corrección en función de la inclinación
    if(x<sizeX && y<sizeY){
        source[x+y*sizeX]=source[x+y*sizeX]-(*avgSlope)*(y-sizeY/2);
    }
}
/** Kernel que se ejecuta en la tarjeta gráfica como primer paso para fijar el suelo de la imagen y quitar ruido.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param avgValue Puntero a la zona de memoria conde se escribirá el valor del suelo calculado.*/
__global__ void SetGroundStep1Kernel(float * source,int sizeX,int sizeY,float * avgValue){
    int y;
    //Calculo la media de todos los valores en las rectas verticales laterales de los extremos
    (*avgValue)=0;
    for (y=0;y<sizeY;y++){
        (*avgValue)+=source[y*sizeX]+source[(sizeX-1)+y*sizeX];
    }
    (*avgValue)=(*avgValue)/(2*sizeY);
}

/** Kernel que se ejecuta en la tarjeta gráfica como segundo paso para fijar el suelo de la imagen y quitar ruido.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param avgValue Puntero a la zona de memoria con el valor del suelo.*/
__global__ void SetGroundStep2Kernel(float * source,int sizeX,int sizeY,float * avgValue){
    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;

    //Si el valor en el punto es menor que el calculado, asigno el calculado

    if(x<sizeX && y<sizeY){
        if(source[x+y*sizeX] < (*avgValue)){
            source[x+y*sizeX]=(*avgValue);
        }
        source[x+y*sizeX]=source[x+y*sizeX]-(*avgValue);
    }
}

/** Kernel que se ejecuta en la tarjeta gráfica para añadir un offset y aplicar una ganancia a la imagen.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param offset Valor del offset a aplicar.
    @param factor Valor de la ganancia a aplicar.*/
__global__ void OffsetAndMultiplyKernel(float * source,int sizeX,int sizeY,float offset,float factor){
    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;

    //Recalculo el valor en función del offset y el factor de multiplicación

    if(x<sizeX && y<sizeY){
        source[x+y*sizeX]=source[x+y*sizeX]*factor+offset;
    }
}

/** Kernel que se ejecuta en la tarjeta gráfica como primer paso (Buscar máximos) para simetrizar la imagen.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param maxPos Puntero a la zona de memoria donde se excribirán las posiciones de los máximos.*/
__global__ void MakeSymmetricStep1Kernel(float * source,int sizeX,int sizeY,int * maxPos){
    int x;
    int y=threadIdx.x+blockIdx.x*blockDim.x;
    float maxValue;
    //Busco la posición del máximo en cada recta horizontal
    if(y<sizeY){
        maxPos[y]=0;
        maxValue=source[y*sizeX];

        for (x=1;x<sizeX;x++){
            if (source[x+y*sizeX]> maxValue){
                maxValue=source[x+y*sizeX];
                maxPos[y]=x;
            }
        }
    }
}

/** Kernel que se ejecuta en la tarjeta gráfica como segundo paso para simetrizar la imagen.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param dest Puntero a la zona donde se debe escribir la salida.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param maxPos Puntero a la zona de memoria donde se encuentran las posiciones de los máximos.*/
__global__ void MakeSymmetricStep2Kernel(float * source,float * dest,int sizeX,int sizeY,int * maxPos){
    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;
    float meanValue;
    int dist;

    //Centro el máximo y promedio lateralmente respecto del máximo
    if(x<sizeX && y<sizeY){
        dist=abs(x-maxPos[y]);
        if((maxPos[y]+dist) < sizeX && (maxPos[y]-dist) >= 0)
            meanValue=(source[(maxPos[y]+dist)+y*sizeX]+source[(maxPos[y]-dist)+y*sizeX])/2;
        else if ((maxPos[y]+dist) < sizeX)
            meanValue=(source[(maxPos[y]+dist)+y*sizeX]+source[0+y*sizeX])/2;
        else if ((maxPos[y]-dist) >=0)
            meanValue=(source[sizeX-1+y*sizeX]+source[(maxPos[y]-dist)+y*sizeX])/2;
        else
            meanValue=0;

        if(sizeX/2+dist < sizeX)
            dest[(sizeX/2+dist)+y*sizeX]=meanValue;
        if(sizeX/2-dist >= 0)
            dest[(sizeX/2-dist)+y*sizeX]=meanValue;



    }
}

/** Kernel que se ejecuta en la tarjeta gráfica para el cálculo de la derivada de la imagen.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param dest Puntero a la zona donde se debe escribir la derivada.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param dx Distancia en entre dos puntos.
    @param points Número de vecinos utilizados para el cálculo de la derivada.*/
__global__ void DerivateKernel(float * source,float * dest,int sizeX,int sizeY,float dx,int points){
    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;
    float value=0;
    int count=0;
    int i;

    //Calculo la derivada en cada punto, promediando la pendiente con 2 puntos anteriores y posteriores

    if(x<sizeX && y<sizeY){
        for(i=1;i<=points;i++){
            if(x-i>=0){
                value+=(source[x+y*sizeX]-source[(x-i)+y*sizeX])/i;
                count++;
            }
            if(x+i<sizeX){
                value+=(source[x+i+y*sizeX]-source[x+y*sizeX])/i;
                count++;
            }
        }
        dest[x+y*sizeX]=value/(dx*count);
    }
}
/** Kernel que se ejecuta en la tarjeta gráfica para el cálculo de la inversión de Abel.
    @param derivative Puntero a la matriz con la derivada en cada punto.
    @param dest Puntero a la zona donde se debe escribir la salida.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.*/
__global__ void AbelInversionKernel(float * derivative,float * dest,int sizeX,int sizeY){

    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;
    int i;
    float iFloat;
    float xFloat=x-sizeX/2;
    float value=0;

    //Cada hilo calcula la integral en un punto a una distancia determinada

    if(x<sizeX && y<sizeY){
        xFloat=(x-sizeX/2);
        if(xFloat>=0){
            //Calculo el integrando para cada posición correspondiente y lo voy sumando al acumulador
            //haciendo aproximación trapezoidal (Sumo la mitad del primero y del último)
            for(i=x;i<sizeX;i++){
                //Evito la singularidad en i=x, asignandole el siguiente valor
                if(i==x){
                    if(i+1<sizeX){
                        iFloat=(i+1-(int)sizeX/2);
                        value+=-derivative[i+1+y*sizeX]/(PI_NUM*sqrt(iFloat*iFloat-xFloat*xFloat))/2;
                    }
                }
                else if(i==sizeX-1){
                    iFloat=(i-(int)sizeX/2);
                    value+=-derivative[i+y*sizeX]/(PI_NUM*sqrt(iFloat*iFloat-xFloat*xFloat))/2;
                }
                else{
                    iFloat=(i-(int)sizeX/2);
                    value+=-derivative[i+y*sizeX]/(PI_NUM*sqrt(iFloat*iFloat-xFloat*xFloat));
                }
            }
        }
        else{
            //Calculo el integrando para cada posición correspondiente y lo voy sumando al acumulador
            //haciendo aproximación trapezoidal (Sumo la mitad del primero y del último)
            for(i=0;i<=x;i++){
                //Evito la singularidad en i=x, asignandole el siguiente valor
                if(i==x){
                    if(i-1>=0){
                        iFloat=(i-1-(int)sizeX/2);
                        value+=derivative[i-1+y*sizeX]/(PI_NUM*sqrt(iFloat*iFloat-xFloat*xFloat))/2;
                    }
                }
                else if(i==0){
                    iFloat=(i-(int)sizeX/2);
                    value+=derivative[i+y*sizeX]/(PI_NUM*sqrt(iFloat*iFloat-xFloat*xFloat))/2;
                }
                else{
                    iFloat=(i-(int)sizeX/2);
                    value+=derivative[i+y*sizeX]/(PI_NUM*sqrt(iFloat*iFloat-xFloat*xFloat));

                }
            }
        }
        dest[x+y*sizeX]=value;
    }
}

/** Kernel que se ejecuta en la tarjeta gráfica para la obtención de un rectángulo dentro de la imagen de la fase.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param result Puntero a la zona donde se debe escribir la salida.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param x1 Posición horizontal de la esquina superior izquierda de la máscara.
    @param x2 Posición horizontal de la esquina inferior derecha de la máscara.
    @param y1 Posición vertical de la esquina superior izquierda de la máscara.
    @param y2 Posición vertical de la esquina inferior derecha de la máscara.*/
__global__ void CropRectangleKernel(float * source,float * result,int sizeX,int sizeY,int x1,int y1,int x2, int y2){

    int x = threadIdx.x+blockIdx.x*blockDim.x;
    int y = threadIdx.y+blockIdx.y*blockDim.y;

    int xAux,yAux;

    //Asigno los puntos adecuadamente

    if (x>=x1 && y>=y1 && x<=x2 && y<=y2 && x<sizeX && y<sizeY){
        xAux=x-x1;
        yAux=y-y1;

        result[xAux+yAux*(x2-x1+1)]=source[x+y*sizeX];
    }
}


/** Kernel que se ejecuta en la tarjeta gráfica para la obtención de una línea de la fase.
    @param source Buffer en la memoria de la GPU con la imagen de la fase alineada fila tras fila.
    @param sizeX Ancho en píxeles de la imagen.
    @param sizeY Alto en píxeles de la imagen.
    @param x1 Posición horizontal de la esquina superior izquierda de la máscara.
    @param x2 Posición horizontal de la esquina inferior derecha de la máscara.
    @param y1 Posición vertical de la esquina superior izquierda de la máscara.
    @param y2 Posición vertical de la esquina inferior derecha de la máscara.
    @param output Puntero a la zona donde se debe escribir la salida.
    @param outputSize Número de muestras para la recta.*/
__global__ void GetPhaseLineKernel(float * source,int sizeX,int sizeY,int x1,int y1,int x2, int y2,float * output,int outputSize){
    int j=threadIdx.x+blockIdx.x*blockDim.x;

    //Calculo las coordenadas del pixel en la imagen original que tengo que colocar en la posición j d ela línea
    int x=x1+((x2-x1)*j)/(outputSize-1);
    int y=y1+((y2-y1)*j)/(outputSize-1);

    if(j<outputSize){
        if(x>=0 && y>=0 && x<sizeX &&y < sizeY)
            output[j]=source[x+y*sizeX];
        else
            output[j]=0;
    }

}


float * LoadInGPUCUDA(float * hostPt,int width,int height){

    float * devicePt;
    CudaSafe( cudaMalloc((void **)&devicePt,sizeof(float)*height*width),"cudaMalloc");
    CudaSafe( cudaMemcpy(devicePt, hostPt, sizeof(float)*height*width,cudaMemcpyHostToDevice),"cudaMemcpy");

    return devicePt;
}

float * MemCpyCUDA(float * devicePt,int width,int height){
    float * returnDevicePt;
    CudaSafe( cudaMalloc((void **)&returnDevicePt,sizeof(float)*height*width),"cudaMalloc") ;
    CudaSafe( cudaMemcpy(returnDevicePt, devicePt, sizeof(float)*height*width,cudaMemcpyDeviceToDevice),"cudaMemcpy");
    return returnDevicePt;
}



void FreeCUDA(float * devicePt){
    CudaSafe( cudaFree(devicePt),"cudaFree");
}


void GetBufferBackCUDA(float * hostPt, float * devicePt, int width,int height){

    CudaSafe( cudaMemcpy(hostPt, devicePt, sizeof(float)*height*width, cudaMemcpyDeviceToHost),"cudaMemcpy");
}


void ScanPhaseCUDA(float * devicePt, int width,int height){


    int * phaseAccumulator;

    CudaSafe( cudaMalloc((void **)&phaseAccumulator,sizeof(int)*height*width),"cudaMalloc");

    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;
    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);

    //Escaneo la línea horizontal central, hacia derecha e izquierda (con dos hilos), acumulando saltos de fase
    ScanPhaseStep1Kernel <<< 1, 2>>> (devicePt,phaseAccumulator,width,height);
    CudaSafeKernel("ScanPhaseStep1");
    //A partir de la línea central escaneo en cascada cada línea vertical, hacia arriba y hacia abajo, acumulando saltos de fase
    ScanPhaseStep2Kernel <<< (int)(2*width+31)/32, 32 >>> (devicePt,phaseAccumulator,width,height);
    CudaSafeKernel("ScanPhaseStep2");
    //Sumo múltiplos de 2Pi por cada salto de fase
    ScanPhaseStep3Kernel <<< blocks, threads >>> (devicePt,phaseAccumulator,width,height);
    CudaSafeKernel("ScanPhaseStep3");

    CudaSafe( cudaFree(phaseAccumulator),"cudaFree");


}


void SubtractCUDA(float * devicePt,float * offsetDevicePt, int width,int height){
    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;
    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);

    SubtractKernel <<< blocks, threads >>> (devicePt,offsetDevicePt,width,height);
    CudaSafeKernel("Subtract");

}

void AdditiveInverseCUDA(float * devicePt, int width,int height){

    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;
    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);

    AdditiveInverseKernel <<< blocks, threads >>> (devicePt,width,height);
    CudaSafeKernel("AdditiveInverse");

}

void RemoveHorizontalLinearTiltBordersCUDA(float * devicePt, int width,int height){

    float * avgSlope;
    CudaSafe( cudaMalloc((void **)&avgSlope,sizeof(float)),"cudaMalloc");

    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;
    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);

    //Calculo la media de las pendientes entre el primer y el último punto de cada línea vertical
    RemoveHorizontalLinearTiltBordersStep1Kernel <<< 1, 1 >>> (devicePt,width,height,avgSlope);
    CudaSafeKernel("RemoveHorizontalLinearTiltBordersStep1");

    //Aplico la correción a cada punto en función de esa pendiente media
    RemoveHorizontalLinearTiltBordersStep2Kernel <<< blocks, threads >>> (devicePt,width,height,avgSlope);
    CudaSafeKernel("RemoveHorizontalLinearTiltBordersStep2");

    CudaSafe( cudaFree(avgSlope),"cudaFree");

}


void RemoveHorizontalLinearTiltCUDA(float * devicePt, int width,int height){
    float * avgSlope;
    float * slopesArray;
    CudaSafe( cudaMalloc((void **)&avgSlope,sizeof(float)),"cudaMalloc");
    CudaSafe( cudaMalloc((void **)&slopesArray,height*sizeof(float)),"cudaMalloc");

    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;
    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);

    //Calculo la pendiente para cada una de las rectas verticales, por un ajuste de mínimos cuadrados
    RemoveHorizontalLinearTiltStep1Kernel <<< (int)(height+31)/32, 32 >>> (devicePt,width,height,slopesArray);
    CudaSafeKernel("RemoveHorizontalLinearTiltStep1");

    //Calculo la media de todas las pendientes
    RemoveHorizontalLinearTiltStep2Kernel <<< 1, 1 >>> (devicePt,width,height,avgSlope,slopesArray);
    CudaSafeKernel("RemoveHorizontalLinearTiltStep2");

    //Aplico la correción a cada punto en función de esa pendiente media
    RemoveHorizontalLinearTiltStep3Kernel <<< blocks, threads >>> (devicePt,width,height,avgSlope);
    CudaSafeKernel("RemoveHorizontalLinearTiltStep3");

    CudaSafe( cudaFree(avgSlope),"cudaFree");
    CudaSafe( cudaFree(slopesArray),"cudaFree");

}


void RemoveVerticalLinearTiltCUDA(float * devicePt, int width,int height){
    float * avgSlope;
    float * slopesArray;
    CudaSafe( cudaMalloc((void **)&avgSlope,sizeof(float)),"cudaMalloc");
    CudaSafe( cudaMalloc((void **)&slopesArray,width*sizeof(float)),"cudaMalloc");

    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;
    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);

    //Calculo la pendiente para cada una de las rectas verticales, por un ajuste de mínimos cuadrados
    RemoveVerticalLinearTiltStep1Kernel <<< (int)(width+31)/32, 32 >>> (devicePt,width,height,slopesArray);
    CudaSafeKernel("RemoveVerticalLinearTiltStep1");

    //Calculo la media de todas las pendientes
    RemoveVerticalLinearTiltStep2Kernel <<< 1, 1 >>> (devicePt,width,height,avgSlope,slopesArray);
    CudaSafeKernel("RemoveVerticalLinearTiltStep2");

    //Aplico la correción a cada punto en función de esa pendiente media
    RemoveVerticalLinearTiltStep3Kernel <<< blocks, threads >>> (devicePt,width,height,avgSlope);
    CudaSafeKernel("RemoveVerticalLinearTiltStep3");

    CudaSafe( cudaFree(avgSlope),"cudaFree");
    CudaSafe( cudaFree(slopesArray),"cudaFree");

}

void SetGroundCUDA(float * devicePt, int width,int height){

    float * avgValue;
    CudaSafe( cudaMalloc((void **)&avgValue,sizeof(float)),"cudaMalloc");

    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;
    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);

    //Calculo la media de los valores en las líneas verticales laterales
    SetGroundStep1Kernel <<< 1, 1 >>> (devicePt,width,height,avgValue);
    CudaSafeKernel("SetGroundStep1");

    //Si hay algún punto con un valor menor a ese, le asigno ese valor
    SetGroundStep2Kernel <<< blocks, threads >>> (devicePt,width,height,avgValue);
    CudaSafeKernel("SetGroundStep2");

    CudaSafe( cudaFree(avgValue),"cudaFree");

}

void OffsetAndMultiplyCUDA(float * devicePt, int width,int height,float offset,float factor){

    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;
    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);

    OffsetAndMultiplyKernel <<< blocks, threads >>> (devicePt,width,height,offset,factor);
    CudaSafeKernel("OffsetAndMultiply");

}

void MakeSymmetricCUDA(float * devicePt, int width,int height){

    int * maxPos;
    CudaSafe( cudaMalloc((void **)&maxPos,sizeof(int)*height),"cudaMalloc");
    float * auxMatrixDevice;
    CudaSafe( cudaMalloc((void **)&auxMatrixDevice,sizeof(float)*height*width),"cudaMalloc");
    //Hago una copia de los datos
    CudaSafe( cudaMemcpy(auxMatrixDevice, devicePt, sizeof(float)*height*width,cudaMemcpyDeviceToDevice),"cudaMemcpy");

    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;
    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);

    //Calculo las posiciones de los máximos para cada recta horizontal
    MakeSymmetricStep1Kernel <<< (int)(height+31)/32, 32 >>> (devicePt,width,height,maxPos);
    CudaSafeKernel("MakeSymmetricStep1");

    //Recoloco la imagen desde la copia, promediando lado izquierdo y derecho respecto al máximo y centrándolo
    MakeSymmetricStep2Kernel <<< blocks, threads >>> (auxMatrixDevice,devicePt,width,height,maxPos);
    CudaSafeKernel("MakeSymmetricStep2");

    CudaSafe( cudaFree(maxPos),"cudaFree");
    CudaSafe( cudaFree(auxMatrixDevice),"cudaFree");

}

void AbelInversionCUDA(float * devicePt, int width,int height, float dx,int neighbours){
    float * derivative;
    CudaSafe( cudaMalloc((void **)&derivative,sizeof(float)*height*width),"cudaMalloc");

    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;
    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);

    //Calculo la derivada en cada línea horizontal
    DerivateKernel<<< blocks, threads >>> (devicePt,derivative,width,height,dx,neighbours);
    CudaSafeKernel("DerivativeKernel");

    //Hago la inversion de abel en cada recta horizontal secuencialmente (Si no tendría que reservar muchísima memoria)
    AbelInversionKernel<<< blocks, threads >>> (derivative,devicePt,width,height);
    CudaSafeKernel("AbelInversionKernel");

    CudaSafe( cudaFree(derivative),"cudaFree");
}

float * CropRectangleCUDA(float *devicePt,int width,int height,int x1,int y1,int x2, int y2){
    float * resultDevice;

    CudaSafe( cudaMalloc((void **)&resultDevice,sizeof(float)*(x2-x1+1)*(y2-y1+1)),"cudaMalloc");

    int blocksX=(15+width)/16;
    int blocksY=(15+height)/16;

    dim3 blocks(blocksX,blocksY);
    dim3 threads(16,16);
    CropRectangleKernel <<< blocks, threads >>> (devicePt, resultDevice,width,height,x1,y1,x2,y2);
    CudaSafeKernel("CropRectangle");

    return resultDevice;

}

float * GetPhaseLineCUDA(float * devicePt,int width,int height,int x1,int y1,int x2, int y2,int outputSize){

    //Reservo memoria para la salida
    float * output;
    CudaSafe( cudaMalloc((void **)&output,sizeof(float)*outputSize),"cudaMalloc");

    //Calculo el valor que hay que poner en cada punto de la lína en función de los valores
    GetPhaseLineKernel <<< (int)(outputSize+31)/32, 32 >>> (devicePt,width,height,x1,y1,x2,y2,output,outputSize);
    CudaSafeKernel("GetPhaseLine");

    //Devuelvo la línea
    return output;

}
