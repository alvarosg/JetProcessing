/**
 * @file   cpuphaseinterface.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la cabecera cpuphaseinterface.h.
 *
 */

#include "cpuphaseinterface.h"

void ScanPhaseCPU(float * linearBuffer, int sizeX,int sizeY){
    int * accumulator;
    int x,y;

    accumulator=(int*) malloc(sizeof(int)*sizeX*sizeY);

    //Al punto central le asigno un offset de 0
    y=sizeY/2;
    accumulator[sizeX/2+sizeX*y]=0;

    //Voy calculando secuencialmente por la fila central si hay grandes saltos en la fase
    //y en base a ello aumento o decremento el acumulador para cada punto
    //Hacia la izquierda
    for(x=sizeX/2-1;x>=0;x--){
        accumulator[x+sizeX*y]=accumulator[x+1+sizeX*y];

        if      ((linearBuffer[x+sizeX*y]-linearBuffer[x+1+sizeX*y])>3*PI_NUM/2)
            accumulator[x+sizeX*y]--;

        else if ((linearBuffer[x+1+sizeX*y]-linearBuffer[x+sizeX*y])>3*PI_NUM/2)
            accumulator[x+sizeX*y]++;
    }
    //Hacia la derecha
    for(x=sizeX/2+1;x<sizeX;x++){
        accumulator[x+sizeX*y]=accumulator[x-1+sizeX*y];

        if      ((linearBuffer[x+sizeX*y]-linearBuffer[x-1+sizeX*y])>3*PI_NUM/2)
            accumulator[x+sizeX*y]--;

        else if ((linearBuffer[x-1+sizeX*y]-linearBuffer[x+sizeX*y])>3*PI_NUM/2)
            accumulator[x+sizeX*y]++;
    }

    //A partir de los datos de la línea central voy calculando para
    //cada linea vertical, los saltos de fase

    //Hacia arriba
    for(x=0;x<sizeX;x++){
        for(y=sizeY/2-1;y>=0;y--){
            accumulator[x+y*sizeX]=accumulator[x+(y+1)*sizeX];

            if      ((linearBuffer[x+y*sizeX]-linearBuffer[x+(y+1)*sizeX])>3*PI_NUM/2)
                accumulator[x+y*sizeX]--;

            else if ((linearBuffer[x+(y+1)*sizeX]-linearBuffer[x+y*sizeX])>3*PI_NUM/2)
                accumulator[x+y*sizeX]++;
        }
    }

    //Hacia abajo
    for(x=0;x<sizeX;x++){
        for(y=sizeY/2+1;y<sizeY;y++){
            accumulator[x+y*sizeX]=accumulator[x+(y-1)*sizeX];

            if      ((linearBuffer[x+y*sizeX]-linearBuffer[x+(y-1)*sizeX])>3*PI_NUM/2)
                accumulator[x+y*sizeX]--;

            else if ((linearBuffer[x+(y-1)*sizeX]-linearBuffer[x+y*sizeX])>3*PI_NUM/2)
                accumulator[x+y*sizeX]++;
        }
    }

    //Sumo múltiplos de 2PI en función del acumulador
    for(x=0;x<sizeX;x++){
        for(y=0;y<sizeY;y++){
            linearBuffer[x+y*sizeX]=linearBuffer[x+y*sizeX]+((float)(accumulator[x+y*sizeX]))*2*PI_NUM;
        }
    }

    free(accumulator);

}

void SubtractCPU(float * linearBuffer,float * offsetLinearBuffer, int sizeX,int sizeY){
    int x,y;

    for (y=0;y<sizeY;y++){
        for (x=0;x<sizeX;x++){
            linearBuffer[x+y*sizeX]=linearBuffer[x+y*sizeX]-offsetLinearBuffer[x+y*sizeX];
        }
    }

}

void AdditiveInverseCPU(float * linearBuffer, int sizeX,int sizeY){
    int x,y;
    for (y=0;y<sizeY;y++){
        for (x=0;x<sizeX;x++){
            linearBuffer[x+y*sizeX]=-linearBuffer[x+y*sizeX];
        }
    }
}

void RemoveHorizontalLinearTiltCPU(float * linearBuffer, int sizeX,int sizeY){
    int x,y;
    float avgSlope=0;
    float * slopesArray=(float *)malloc(sizeof(float)*sizeY);

    //Calculo la pendiente de cada recta vertical con una regresión
    float sumX;
    float sumX2;
    float sumZ;
    float sumXZ;
    for(y=0;y<sizeY;y++){
        sumX=0;
        sumX2=0;
        sumZ=0;
        sumXZ=0;
        for (x=0;x<sizeX;x++){
            sumX+=x;
            sumX2+=x*x;
            sumZ+=linearBuffer[x+sizeX*y];
            sumXZ+=linearBuffer[x+sizeX*y]*x;
        }
        slopesArray[y]=(sumX*sumZ-sizeX*sumXZ)/(sumX*sumX-sizeX*sumX2);
    }

    //Hago una media de las pendientes
    avgSlope=0;
    for (y=0;y<sizeY;y++){
        avgSlope+=slopesArray[y];
    }
    avgSlope=avgSlope/sizeY;

    //Aplico a cada punto la corrección en función de la inclinación
    for(x=0;x<sizeX;x++){
        for (y=0;y<sizeY;y++){
            linearBuffer[x+y*sizeX]=linearBuffer[x+y*sizeX]-avgSlope*(x-sizeX/2);
        }
    }

    free(slopesArray);

}

void RemoveVerticalLinearTiltCPU(float * linearBuffer, int sizeX,int sizeY){
    int x,y;
    float avgSlope=0;

    float * slopesArray=(float *)malloc(sizeof(float)*sizeX);

    //Calculo la pendiente de cada recta vertical con una regresión
    float sumY;
    float sumY2;
    float sumZ;
    float sumYZ;
    for(x=0;x<sizeX;x++){
        sumY=0;
        sumY2=0;
        sumZ=0;
        sumYZ=0;
        for (y=0;y<sizeY;y++){
            sumY+=y;
            sumY2+=y*y;
            sumZ+=linearBuffer[x+sizeX*y];
            sumYZ+=linearBuffer[x+sizeX*y]*y;
        }
        slopesArray[x]=(sumY*sumZ-sizeY*sumYZ)/(sumY*sumY-sizeY*sumY2);
    }

    //Hago una media de las pendientes
    avgSlope=0;
    for (x=0;x<sizeX;x++){
        avgSlope+=slopesArray[x];
    }
    avgSlope=avgSlope/sizeX;

    //Aplico a cada punto la corrección en función de la inclinación
    for(x=0;x<sizeX;x++){
        for (y=0;y<sizeY;y++){
            linearBuffer[x+y*sizeX]=linearBuffer[x+y*sizeX]-avgSlope*(y-sizeY/2);
        }
    }

    free(slopesArray);

}

void SetGroundCPU(float * linearBuffer, int sizeX,int sizeY){
    int x,y;

    float avgValue=0;

    //Calculo la media de todos los valores en las rectas verticales laterales de los extremos

    for (y=0;y<sizeY;y++){
        avgValue+=linearBuffer[y*sizeX]+linearBuffer[(sizeX-1)+y*sizeX];
    }
    avgValue=avgValue/(2*sizeY);


    //Si el valor en el punto es menor que el calculado, asigno el calculado
    for (y=0;y<sizeY;y++){
        for (x=0;x<sizeX;x++){
            if(linearBuffer[x+y*sizeX] < avgValue){
                linearBuffer[x+y*sizeX]=avgValue;
            }
            linearBuffer[x+y*sizeX]=linearBuffer[x+y*sizeX]-avgValue;
        }
    }
}

void OffsetAndMultiplyCPU(float * linearBuffer, int sizeX,int sizeY,float offset,float factor){
    int x,y;
    for (y=0;y<sizeY;y++){
        for (x=0;x<sizeX;x++){
            linearBuffer[x+y*sizeX]=linearBuffer[x+y*sizeX]*factor+offset;
        }
    }
}

void MakeSymmetricCPU(float * linearBuffer, int sizeX,int sizeY){
    int x,y;
    float maxValue;
    float meanValue;
    int dist;


    int * maxPos;
    maxPos=(int*) malloc(sizeof(int)*sizeY);
    float * auxMatrix;
    auxMatrix= (float *) malloc(sizeof(float)*sizeY*sizeX);
    //Hago una copia de los datos
    memcpy(auxMatrix,linearBuffer,sizeof(float)*sizeY*sizeX);

    for (y=0;y<sizeY;y++){
        //Busco la posición del máximo en cada recta horizontal
        maxPos[y]=0;
        maxValue=linearBuffer[y*sizeX];
        for (x=0;x<sizeX;x++){
            if (linearBuffer[x+y*sizeX]> maxValue){
                maxValue=linearBuffer[x+y*sizeX];
                maxPos[y]=x;
            }
        }
    }

    //Centro el máximo y promedio lateralmente respecto del máximo
    for (y=0;y<sizeY;y++){
        for (x=0;x<sizeX;x++){
            dist=abs(x-maxPos[y]);
            if((maxPos[y]+dist) < sizeX && (maxPos[y]-dist) >= 0)
                meanValue=(auxMatrix[(maxPos[y]+dist)+y*sizeX]+auxMatrix[(maxPos[y]-dist)+y*sizeX])/2;
            else if ((maxPos[y]+dist) < sizeX)
                meanValue=(auxMatrix[(maxPos[y]+dist)+y*sizeX]+auxMatrix[0+y*sizeX])/2;
            else if ((maxPos[y]-dist) >=0)
                meanValue=(auxMatrix[sizeX-1+y*sizeX]+auxMatrix[(maxPos[y]-dist)+y*sizeX])/2;
            else
                meanValue=0;

            if(sizeX/2+dist < sizeX)
                linearBuffer[(sizeX/2+dist)+y*sizeX]=meanValue;
            if(sizeX/2-dist >= 0)
                linearBuffer[(sizeX/2-dist)+y*sizeX]=meanValue;

        }
    }

    free(auxMatrix);
    free(maxPos);
}

void AbelInversionCPU(float * linearBuffer, int sizeX,int sizeY, float dx,int neighbours){
    int x ,y ,i;
    float * derivative=(float *)malloc(sizeof(float)*sizeX*sizeY);
    float value;
    int count;

    //Calculo la derivada en cada punto, promediando la pendiente con 2 puntos anteriores y posteriores

    for (y=0;y<sizeY;y++){
        for (x=0;x<sizeX;x++){
            value=0;
            count=0;
            for(i=1;i<=neighbours;i++){
                if(x-i>=0){
                    value+=(linearBuffer[x+y*sizeX]-linearBuffer[(x-i)+y*sizeX])/i;
                    count++;
                }
                if(x+i<sizeX){
                    value+=(linearBuffer[x+i+y*sizeX]-linearBuffer[x+y*sizeX])/i;
                    count++;
                }
            }
            derivative[x+y*sizeX]=value/(dx*count);
        }
    }

    float iFloat;
    float xFloat;

    //Calculo la integral

    for (y=0;y<sizeY;y++){
        for (x=0;x<sizeX;x++){
            xFloat=(float)(x-sizeX/2);
            value=0;
            if(xFloat>=0){
                //Calculo el integrando para cada posición correspondiente y lo voy sumando al acumulador
                //haciendo aproximación trapezoidal (Sumo la mitad del primero y del último)
                for(i=x;i<sizeX;i++){
                    //Evito la singularidad en i=x, asignandole el siguiente valor
                    if(i==x){
                        if(i+1<sizeX){
                            iFloat=(float)(i+1-(int)sizeX/2);
                            value+=-derivative[i+1+y*sizeX]/(PI_NUM*sqrt(iFloat*iFloat-xFloat*xFloat))/2;
                        }
                    }
                    else if(i==sizeX-1){
                        iFloat=(float)(i-(int)sizeX/2);
                        value+=-derivative[i+y*sizeX]/(PI_NUM*sqrt(iFloat*iFloat-xFloat*xFloat))/2;
                    }
                    else{
                        iFloat=(float)(i-(int)sizeX/2);
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
                            iFloat=(float)(i-1-(int)sizeX/2);
                            value+=derivative[i-1+y*sizeX]/(PI_NUM*sqrt(iFloat*iFloat-xFloat*xFloat))/2;
                        }
                    }
                    else if(i==0){
                        iFloat=(float)(i-(int)sizeX/2);
                        value+=derivative[i+y*sizeX]/(PI_NUM*sqrt(iFloat*iFloat-xFloat*xFloat))/2;
                    }
                    else{
                        iFloat=(float)(i-(int)sizeX/2);
                        value+=derivative[i+y*sizeX]/(PI_NUM*sqrt(iFloat*iFloat-xFloat*xFloat));

                    }
                }
            }
            linearBuffer[x+y*sizeX]=value;
        }
    }
    free(derivative);

}

float * CropRectangleCPU(float *linearBuffer,int sizeX,int sizeY,int x1,int y1,int x2, int y2){
    int x,y;
    float * buffer = (float*) malloc(sizeof(float)*(x2-x1+1)*(y2-y1+1));
    for (y=0;y<sizeY;y++){
        for (x=0;x<sizeX;x++){
            if (x>=x1 && y>=y1 && x<=x2 && y<=y2){
                buffer[(x-x1)+(y-y1)*(x2-x1+1)]=linearBuffer[x+y*sizeX];
            }
        }
    }
    return buffer;

}

float * GetPhaseLineCPU(float * linearBuffer,int sizeX,int sizeY,int x1,int y1,int x2, int y2,int outputSize){
    int j,x,y;

    float * outputBuffer=(float *)malloc(sizeof(float)*outputSize);

    //La dimensión en pixeles de la salida, mantendrá la proporción
    //Calculo las coordenadas del pixel en la imagen original que tengo que colocar en la posición j d ela línea
    for(j=0;j<outputSize;j++){
        x=x1+((x2-x1)*j)/(outputSize-1);
        y=y1+((y2-y1)*j)/(outputSize-1);
        if(x>=0 && y>=0 && x<sizeX && y < sizeY)
            outputBuffer[j]=linearBuffer[x+y*sizeX];
        else{
            outputBuffer[j]=0;
        }
    }
    return outputBuffer;
}
