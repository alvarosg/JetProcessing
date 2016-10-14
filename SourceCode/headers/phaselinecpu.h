/**
 * @file   phaselinecpu.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de PhaseLine para cálculo con la CPU.
 *
 */
#ifndef PHASELINECPU_H
#define PHASELINECPU_H

#include "phaseline.h"
#include "cpuphaseinterface.h"

/*! \class PhaseLineCPU phaselinecpu.h "phaselinecpu.h"
 * \brief Implementación de PhaseLine utilizando la CPU.
 *
 *  La recta estará contenida en la memoria de la CPU, y las operaciones se realizarán con el procesador del ordenador.
 *
 *  Para realizar estas operaciones utilizará la interfaz definida en cpuphaseinterface.h
 *
 */


class PhaseLineCPU : public PhaseLine
{
public:
    PhaseLineCPU(float * buffer, int size_in);
    /**< Constructor.
      @param buffer Puntero a la zona de memoria de la CPU con los datos a cargar. El puntero y la zona de memoria pasará a ser responsabilidad del objeto creado.
      @param size_in Número de muestras en el buffer.*/
    PhaseLineCPU( const PhaseLineCPU & input);
    /**< Constructor de copia del objeto. Crea una copia del objeto pasado como argumento.
            @param input Objeto a copiar.*/
    ~PhaseLineCPU();
    /**< Destructor.*/
    Data1D * GetData1D(Data1D * data1dIn=NULL);
    void AdditiveInverse();
    void RemoveLinearTilt();
    void SetGround();
    void OffsetAndMultiply(float offset,float factor);
    void MakeSymmetric();
    void AbelInversion(float dx,int neighbours);
};

#endif // PHASELINECPU_H
