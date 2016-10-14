/**
 * @file   fullimage.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase abstracta que representa la imagen en las primeras etapas del procesado, antes de obtener la fase.
 *
 */
#ifndef FULLIMAGE_H
#define FULLIMAGE_H

#include "complex.h"

#include <QImage>
#include "phaseimage.h"

/** Direcci�n de la transformada de fourier*/
enum DirFFT {
    FFT_FORWARD=1,   /**< Directa*/
    FFT_INVERSE=2    /**< Inversa*/
};

/*! \class FullImage fullimage.h "fullimage.h"
 * \brief Clase abstracta que almacena la imagen tomada tras aplicarle las transformaciones iniciales.
 *
 * Es la estrutura de datos que se usa durante las primeras etapas del procesado de la imagen. Para cada pixel guarda dos valores: parte real y parte imaginaria, por lo que se puede considerar que contiene una imagen compleja.
 *
 * Define una interfaz que debe ser implementada por cualquier sistema de c�lculo (En este caso CUDA o CPU), que debe ser programada derivando una clase de �sta.
 *
 *
 */



class FullImage
{

protected:
    int sizeX;              /**< Ancho en p�xeles de la imagen. Siempre ser� una potencia de 2.*/
    int sizeY;              /**< Alto en p�xeles de la imagen. Siempre ser� una potencia de 2.*/
    complex * linearBuffer; /**< Puntero al buffer donde se encuentra la informaci�n de los p�xeles de imagen compleja, organizado fila tras fila. Este puntero puede apuntar tanto a memoria de la CPU como a memoria de la GPU.*/

public:
    FullImage(QImage *source,int power2SizeX, int power2SizeY);
    /**< Constructor.
        @param source Imagen de origen que se cargar� en la parte real d ela imagen compleja. Dicha imagen sigue siendo responsabilidad del cliente.
        @param power2SizeX Ancho deseado para la FullImage creada. Debe ser una potencia de 2. Si no lo es, se redondear� a la siguiente potencia de 2.
        @param power2SizeY Alto deseado para la FullImage creada. Debe ser una potencia de 2. Si no lo es, se redondear� a la siguiente potencia de 2.*/
    virtual ~FullImage();
    /**< Destructor.*/
    int GetSizeX();
    /**< Devuelve el valor del atributo sizeX.
        @return El valor del atributo.*/
    int GetSizeY();
    /**< Devuelve el valor del atributo sizeY.
        @return El valor del atributo.*/
    virtual void ApplyMask(QRect mask)=0;
    /**< Funci�n que aplica una m�scara (Coloca todo a cero excepto los puntos dentro de la m�scara).
        @param mask Rect�ngulo que representa la m�scara.*/
    virtual void ComplexFFT(DirFFT dir)=0;
    /**< Funci�n que realiza la transformada de Fourier de la imagen compleja.
        @param dir Direcci�n de la transformada de fourier.*/
    virtual void ReArrangeFT()=0;
    /**< Funci�n que recoloca la imagen tras realizar una transformada de fourier directa, para ver la frecuencia 0 en el centro de la imagen.*/
    virtual PhaseImage * CropPhase(QRect mask)=0;
    /**< Devuelve un puntero a un objeto tipo PhaseImage, con la fase de la parte dentro de la m�scara pasada como par�matero de la imagen compleja contenida en FullImage, implementado con el mismo m�todo de c�lculo que el objeto al que se solicita, y responsabilidad del cliente.
        @param mask Rect�ngulo que delimitar� la zona de la que se obtiene la fase.
        @return Puntero a un objeto PhaseImage con la fase. Responsabilidad del cliente.*/
    virtual QImage * GetAbsoluteValueAsQImage(int gain)=0;
    /**< Devuelve una imagen con el m�dulo en escala de grises de la imagen compleja. Reescala los valores del modulo entre 0 y 255, y despu�s les aplica la ganancia deseada.
        @param gain Ganancia a aplicar despu�s del reescalado entre 0 y 255.
        @return Puntero a un objeto QImage con el valor absoluto. Responsabilidad del cliente.*/

    static int NextPowerOf2(int value);
    /**< Dado un n�mero, devuelve la siguiente potencia de 2 a ese n�mero.
        @param value Numero introducido.
        @return Siguiente potencia de 2 a partir del n�mero introducido.*/

};

#endif // FULLIMAGE_H
