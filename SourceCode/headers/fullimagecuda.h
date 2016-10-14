/**
 * @file   fullimagecuda.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de FullImage para c�lculo con la CUDA.
 *
 */
#ifndef FULLIMAGECUDA_H
#define FULLIMAGECUDA_H

#include "fullimage.h"
#include "phaseimagecuda.h"
#include "cudafullimageinterface.h"

/*! \class FullImageCUDA fullimagecuda.h "fullimagecuda.h"
 * \brief Implementaci�n de FullImage utilizando CUDA.
 *
 *  La imagen estar� contenida en la memoria de la GPU, y las operaciones se realizar�n con el multiprocesador de la GPU.
 *
 *  Para realizar estas operaciones utilizar� la interfaz definida en cudafullimageinterface.h
 *
 */


class FullImageCUDA : public FullImage
{

public:
    FullImageCUDA(QImage *source,int power2SizeX, int power2SizeY);
    /**< Constructor.
        @param source Imagen de origen que se cargar� en la parte real d ela imagen compleja. Dicha imagen sigue siendo responsabilidad del cliente.
        @param power2SizeX Ancho deseado para la FullImage creada. Debe ser una potencia de 2. Si no lo es, se redondear� a la siguiente potencia de 2.
        @param power2SizeY Alto deseado para la FullImage creada. Debe ser una potencia de 2. Si no lo es, se redondear� a la siguiente potencia de 2.*/

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
