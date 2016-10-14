/**
 * @file   phaseimagecuda.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de PhaseImage para c�lculo con la CUDA.
 *
 */
#ifndef PHASEIMAGECUDA_H
#define PHASEIMAGECUDA_H

#include "phaseimage.h"
#include "phaselinecuda.h"
#include "cudaphaseinterface.h"

/*! \class PhaseImageCUDA phaseimagecuda.h "phaseimagecuda.h"
 * \brief Implementaci�n de PhaseImage utilizando CUDA.
 *
 *  La imagen estar� contenida en la memoria de la GPU, y las operaciones se realizar�n con el multiprocesador de la GPU.
 *
 *  Para realizar estas operaciones utilizar� la interfaz definida en cudaphaseinterface.h
 *
 */


class PhaseImageCUDA : public PhaseImage
{


public:
    PhaseImageCUDA(float * deviceBuffer, int sizeX_in, int sizeY_in);
    /**< Constructor.
      @param deviceBuffer Puntero a la zona de memoria de la GPU elegida con la imagen a cargar. El puntero y la zona de memoria pasar� a ser responsabilidad del objeto creado.
      @param sizeX_in Ancho en p�xeles de la imagen.
      @param sizeY_in Alto en p�xeles de la imagen.*/
    PhaseImageCUDA( const PhaseImageCUDA & input);
    /**< Constructor de copia del objeto. Crea una copia del objeto pasado como argumento.
            @param input Objeto a copiar.*/
    ~PhaseImageCUDA();
    /**< Destructor.*/
    void LoadFromMemoryData(float * buffer, int sizeX_in, int sizeY_in);
    float * GetMemoryBuffer();
    void ScanPhase();
    QImage * GetAsQImage(int gain);
    Data2D * GetData2D(Data2D * data2dIn=NULL);
    void SubtractPhase(PhaseImage * offset_in);
    void AdditiveInverse();
    void RemoveHorizontalLinearTilt();
    void RemoveVerticalLinearTilt();
    void SetGround();
    void OffsetAndMultiply(float offset,float factor);
    void MakeSymmetric();
    void AbelInversion(float dx,int neighbours);
    void CropRectangle(QRect rect);
    PhaseLine * GetPhaseLine(QRect points);

};

#endif // PHASEIMAGECUDA_H
