/**
 * @file   fullimagecuda.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de FullImage para cálculo con la CUDA.
 *
 */
#ifndef FULLIMAGECUDA_H
#define FULLIMAGECUDA_H

#include "fullimage.h"
#include "phaseimagecuda.h"
#include "cudafullimageinterface.h"

/*! \class FullImageCUDA fullimagecuda.h "fullimagecuda.h"
 * \brief Implementación de FullImage utilizando CUDA.
 *
 *  La imagen estará contenida en la memoria de la GPU, y las operaciones se realizarán con el multiprocesador de la GPU.
 *
 *  Para realizar estas operaciones utilizará la interfaz definida en cudafullimageinterface.h
 *
 */


class FullImageCUDA : public FullImage
{

public:
    FullImageCUDA(QImage *source,int power2SizeX, int power2SizeY);
    /**< Constructor.
        @param source Imagen de origen que se cargará en la parte real d ela imagen compleja. Dicha imagen sigue siendo responsabilidad del cliente.
        @param power2SizeX Ancho deseado para la FullImage creada. Debe ser una potencia de 2. Si no lo es, se redondeará a la siguiente potencia de 2.
        @param power2SizeY Alto deseado para la FullImage creada. Debe ser una potencia de 2. Si no lo es, se redondeará a la siguiente potencia de 2.*/

    FullImageCUDA( const FullImageCUDA & input);
    /**< Constructor de copia del objeto. Crea una copia del objeto pasado como argumento.
            @param input Objeto a copiar.*/
    ~FullImageCUDA();
    /**< Destructor.*/
    void ApplyMask(QRect mask);
    void ComplexFFT(DirFFT dir);
    void ReArrangeFT();
    PhaseImage * CropPhase(QRect mask);
    QImage * GetAbsoluteValueAsQImage(int gain);
};

#endif // FULLIMAGECUDA_H
