/**
 * @file   phaseimagecpu.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de PhaseImage para c�lculo con la CPU.
 *
 */
#ifndef PHASEIMAGECPU_H
#define PHASEIMAGECPU_H

#include "phaseimage.h"
#include "complex.h"
#include "phaselinecpu.h"
#include "cpuphaseinterface.h"

/*! \class PhaseImageCPU phaseimagecpu.h "phaseimagecpu.h"
 * \brief Implementaci�n de PhaseImage utilizando la CPU.
 *
 *  La imagen estar� contenida en la memoria de la CPU, y las operaciones se realizar�n con el procesador del ordenador.
 *
 *  Para realizar estas operaciones utilizar� la interfaz definida en cpuphaseinterface.h
 *
 */

class PhaseImageCPU : public PhaseImage
{
private:
    float ** dbptBuffer;   /**< Array de punteros a puntero, para permitir el acceso a a los p�xeles de la imagen, primero usando el �ndice de las filas y despu�s el de las columnas.*/

public:
    PhaseImageCPU(float * buffer, int sizeX_in, int sizeY_in);
    /**< Constructor.
      @param buffer Puntero a la zona de memoria de la CPU con la imagen a cargar. El puntero y la zona de memoria pasar� a ser responsabilidad del objeto creado.
      @param sizeX_in Ancho en p�xeles de la imagen.
      @param sizeY_in Alto en p�xeles de la imagen.*/
    PhaseImageCPU( const PhaseImageCPU & input);
    /**< Constructor de copia del objeto. Crea una copia del objeto pasado como argumento.
            @param input Objeto a copiar.*/
    ~PhaseImageCPU();
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

#endif // PHASEIMAGECPU_H
