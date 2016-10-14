/**
 * @file   phaselinecuda.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de PhaseLine para cálculo con la CUDA.
 *
 */
#ifndef PHASELINECUDA_H
#define PHASELINECUDA_H

#include "phaseline.h"
#include "cudaphaseinterface.h"

/*! \class PhaseLineCUDA phaselinecuda.h "phaselinecuda.h"
 * \brief Implementación de PhaseLine utilizando la CPU.
 *
 *  La recta estará contenida en la memoria de la GPU, y las operaciones se realizarán con el multiprocesador de la GPU.
 *
 *  Para realizar estas operaciones utilizará la interfaz definida en cudaphaseinterface.h
 *
 */

class PhaseLineCUDA : public PhaseLine{
public:
    PhaseLineCUDA(float * deviceBuffer, int size_in);
    /**< Constructor.
      @param deviceBuffer Puntero a la zona de memoria de la GPU con los datos a cargar. El puntero y la zona de memoria pasará a ser responsabilidad del objeto creado.
      @param size_in Número de muestras en el buffer.*/
    PhaseLineCUDA( const PhaseLineCUDA & input);
    /**< Constructor de copia del objeto. Crea una copia del objeto pasado como argumento.
            @param input Objeto a copiar.*/
    ~PhaseLineCUDA();
    /**< Destructor.*/
    Data1D * GetData1D(Data1D * data1dIn=NULL);
    void AdditiveInverse();
    void RemoveLinearTilt();
    void SetGround();
    void OffsetAndMultiply(float offset,float factor);
    void MakeSymmetric();
    void AbelInversion(float dx,int neighbours);
};

#endif // PHASELINECUDA_H
