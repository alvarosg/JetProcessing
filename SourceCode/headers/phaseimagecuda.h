/**
 * @file   phaseimagecuda.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de PhaseImage para cálculo con la CUDA.
 *
 */
#ifndef PHASEIMAGECUDA_H
#define PHASEIMAGECUDA_H

#include "phaseimage.h"
#include "phaselinecuda.h"
#include "cudaphaseinterface.h"

/*! \class PhaseImageCUDA phaseimagecuda.h "phaseimagecuda.h"
 * \brief Implementación de PhaseImage utilizando CUDA.
 *
 *  La imagen estará contenida en la memoria de la GPU, y las operaciones se realizarán con el multiprocesador de la GPU.
 *
 *  Para realizar estas operaciones utilizará la interfaz definida en cudaphaseinterface.h
 *
 */


class PhaseImageCUDA : public PhaseImage
{


public:
    PhaseImageCUDA(float * deviceBuffer, int sizeX_in, int sizeY_in);
    /**< Constructor.
      @param deviceBuffer Puntero a la zona de memoria de la GPU elegida con la imagen a cargar. El puntero y la zona de memoria pasará a ser responsabilidad del objeto creado.
      @param sizeX_in Ancho en píxeles de la imagen.
      @param sizeY_in Alto en píxeles de la imagen.*/
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
