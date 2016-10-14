/**
 * @file   cpufullimageinterface.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la cabecera cpufullimageinterface.h.
 *
 */
#include "cpufullimageinterface.h"

void ApplyMaskCPU(complex *linearBuffer,int sizeX,int sizeY,int x1,int y1,int x2, int y2){
    int x,y;
    for(y=0;y<sizeY;y++){
        for(x=0;x<sizeX;x++){
            if (x<x1 || x>x2 || y<y1 || y>y2){
                linearBuffer[x+y*sizeX].x=0;
                linearBuffer[x+y*sizeX].y=0;
            }
        }
    }

}

void FFTCPU(complex **dbptBuffer,int sizeX,int sizeY,int mode){
    FFT2D(dbptBuffer,sizeX,sizeY,mode);
}

complex *ReArrangeFTCPU(complex *linearBuffer,int sizeX,int sizeY){
    int xAux,yAux,x,y;
    complex * linearResult=(complex *) malloc(sizeof(complex)*sizeX*sizeY);

    for(y=0;y<sizeY;y++){
        for(x=0;x<sizeX;x++){
            if(x<sizeX/2)
                xAux=x+sizeX/2;
            else
                xAux=x-sizeX/2;
            if(y<sizeY/2)
                yAux=y+sizeY/2;
            else
                yAux=y-sizeY/2;


            if (xAux<sizeX && yAux<sizeY){
                linearResult[xAux+yAux*sizeX]=linearBuffer[x+y*sizeX];
            }
        }
    }
    return linearResult;
}

float * GetAbsoluteValueCPU(complex *linearBuffer,int sizeX,int sizeY){
    float * absolute=(float *) malloc(sizeof(float)*sizeX*sizeY);
    int x,y;
    for (y=0;y<sizeY;y++){
        for (x=0;x<sizeX;x++){
            absolute[x+y*sizeX]=sqrt(linearBuffer[x+y*sizeX].x*linearBuffer[x+y*sizeX].x +
                     linearBuffer[x+y*sizeX].y*linearBuffer[x+y*sizeX].y);
        }
    }
    return absolute;
}

float * CropPhaseCPU(complex *linearBuffer,int sizeX,int sizeY,int x1,int y1,int x2, int y2){

    int rSizeX=x2-x1+1;
    int rSizeY=y2-y1+1;
    float * resultBuffer=(float *) malloc(sizeof(float)*rSizeX*rSizeY);

    int x,y,xAux,yAux;
    float real,imag,phase;

    for(y=0;y<sizeY;y++){
        for(x=0;x<sizeX;x++){
            //Sólo calculo y guardo la fase de los puntos dentro de la máscara
            if (x>=x1 && y>=y1 && x<=x2 && y<=y2){
                xAux=x-x1;
                yAux=y-y1;
                real=linearBuffer[x+sizeX*y].x;
                imag=linearBuffer[x+sizeX*y].y;
                //Paso a modulo 2Pi
                phase=atan2f(imag,real);
                //Poner en el intervalo (0,2PI)
                if (phase<0)
                    phase+=(float)(2*PI_NUM);
                resultBuffer[xAux+yAux*(x2-x1+1)]=phase;
            }
        }
    }
    return resultBuffer;
}



void FFT2D(complex **comp,int width,int height,int dir){
    float *real,*imag;

    //Rows
    real = new float[height * sizeof(float)];
    imag = new float[height * sizeof(float)];

    for (int j=0;j<width;j++) {
      for (int i=0;i<height;i++) {
         real[i] = comp[i][j].x;
         imag[i] = comp[i][j].y;
      }
      FFT(real,imag,height,dir);
      for (int i=0;i<height;i++) {
         comp[i][j].x = real[i];
         comp[i][j].y = imag[i];
      }
    }
    delete [] real;
    delete [] imag;

    //Columns
    real = new float[width * sizeof(float)];
    imag = new float[width * sizeof(float)];

    for (int i=0;i<height;i++) {
      for (int j=0;j<width;j++) {
         real[j] = comp[i][j].x;
         imag[j] = comp[i][j].y;
      }
      FFT(real,imag,width,dir);
      for (int j=0;j<width;j++) {
         comp[i][j].x = real[j];
         comp[i][j].y = imag[j];
      }
    }
    delete [] real;
    delete [] imag;
}

void FFT(float *real, float *imag, int size, int dir){
       int npoints;
       // Number of points needed
       int times=0;
       for (npoints=1;npoints<size;times++)
          npoints *= 2;

       // Reversal
       int i2 = npoints >> 1;
       int j = 0;
       int k;
       for (int i=0;i<npoints-1;i++) {
          if (i < j) {
             float x = real[i];
             float y = imag[i];
             real[i] = real[j];
             imag[i] = imag[j];
             real[j] = x;
             imag[j] = y;
          }
          k = i2;
          while (k <= j) {
             j -= k;
             k >>= 1;
          }
          j += k;
       }

       //Compute the FFT
       float c1 = -1.0;
       float c2 = 0.0;
       int l2 = 1;
       for (int l=0;l<times;l++) {
          int l1 = l2;
          l2 <<= 1;
          float u1 = 1.0;
          float u2 = 0.0;
          for (int j=0;j<l1;j++) {
             for (int i=j;i<npoints;i+=l2) {
                int i1 = i + l1;
                float t1 = u1 * real[i1] - u2 * imag[i1];
                float t2 = u1 * imag[i1] + u2 * real[i1];
                real[i1] = real[i] - t1;
                imag[i1] = imag[i] - t2;
                real[i] += t1;
                imag[i] += t2;
             }
             float z =  u1 * c1 - u2 * c2;
             u2 = u1 * c2 + u2 * c1;
             u1 = z;
          }
          c2 = (float) sqrt((1.0 - c1) / 2.0);
          if (dir == 1)
             c2 = -c2;
          c1 = (float) sqrt((1.0 + c1) / 2.0);
       }

       //Scaling on the inverse
       if (dir == INVERSE) {
          for (int i=0;i<npoints;i++) {
             real[i] = real[i]/npoints;
             imag[i] = imag[i]/npoints;
          }
       }
    }
