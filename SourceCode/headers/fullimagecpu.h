/**
 * @file   fullimagecpu.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de FullImage para c�lculo con la CPU.
 *
 */
#ifndef FULLIMAGECPU_H
#define FULLIMAGECPU_H

#include "fullimage.h"
#include "phaseimagecpu.h"
#include "cpufullimageinterface.h"


/*! \class FullImageCPU fullimagecpu.h "fullimagecpu.h"
 * \brief Implementaci�n de FullImage utilizando la CPU.
 *
 *  La imagen estar� contenida en la memoria de la CPU, y las operaciones se realizar�n con el procesador del ordenador.
 *
 *  Para realizar estas operaciones utilizar� la interfaz definida en cpufullimageinterface.h
 *
 */


class FullImageCPU : public FullImage
{
private:
    complex ** dbptBuffer;      /**< Array de punteros a puntero, para permitir el acceso a a los p�xeles de la imagen, primero usando el �ndice de las filas y despu�s el de las columnas.*/
public:
    FullImageCPU(QImage *source,int power2SizeX, int power2SizeY);
    /**< Constructor.
        @param source Imagen de origen que se cargar� en la parte real d ela imagen compleja. Dicha imagen sigue siendo responsabilidad del cliente.
        @param power2SizeX Ancho deseado para la FullImage creada. Debe ser una potencia de 2. Si no lo es, se redondear� a la siguiente potencia de 2.
        @param power2SizeY Alto deseado para la FullImage creada. Debe ser una potencia de 2. Si no lo es, se redondear� a la siguiente potencia de 2.*/

    FullImageCPU( const FullImageCPU & input);
    /**< Constructor de copia del objeto. Crea una copia del objeto pasado como argumento.
            @param input Objeto a copiar.*/
    ~FullImageCPU();
    /**< Destructor. */
    void ApplyMask(QRect mask);
    void ComplexFFT(DirFFT dir);
    void ReArrangeFT();
    PhaseImage * CropPhase(QRect mask);
    QImage * GetAbsoluteValueAsQImage(int gain);

};

#endif // FULLIMAGECPU_H
