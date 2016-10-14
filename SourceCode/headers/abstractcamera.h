/**
 * @file   abstractcamera.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase abstracta para representar una c�mara.
 *
 */
#ifndef ABSTRACTCAMERA_H
#define ABSTRACTCAMERA_H

#include<stdio.h>
#include"sourcepicture.h"
#include"cameraparameters.h"

/** Tipos de ganancia. */
enum GainType {
    GAIN_MASTER , /**< Ganancia m�ster. */
    GAIN_RED,     /**< Ganancia de rojo. */
    GAIN_GREEN,   /**< Ganancia de verde. */
    GAIN_BLUE     /**< Ganancia de azul. */
};
/** Valores de exposici�n. */
enum ExposureParameter {
    EXPOSURE_MAX, /**< M�ximo. */
    EXPOSURE_MIN, /**< M�ximo. */
    EXPOSURE      /**< Actual. */
};
/** Valores de tasa de toma de im�genes. */
enum FramerateParameter {
    FRAMERATE_MAX,/**< M�ximo. */
    FRAMERATE_MIN,/**< M�nimo. */
    FRAMERATE     /**< Actual. */
};
/** Valores del reloj de pixel. */
enum PixelClockParameter {
    PIXELCLOCK_MAX,/**< M�ximo. */
    PIXELCLOCK_MIN,/**< M�nimo. */
    PIXELCLOCK     /**< Actual. */
};
/** Valores de dimensiones en ejes.*/
enum SizeAxis {
    SIZE_X ,       /**< Ancho. */
    SIZE_Y         /**< Alto. */
};

/*! \class AbstractCamera abstractcamera.h "abstractcamera.h"
 * \brief Clase abstracta que representa una c�mara.
 *
 * Define una interfaz que debe ser implementada por cualquier c�mara, que debe ser programada derivando una clase de �sta.
 *
 *
 */


class AbstractCamera{

protected:

    CameraParameters parameters; /**< Par�metros actuales de configuraci�n de la c�mara.*/

public:

    virtual QImage * AcquireSourcePicture()=0;
    /**< Toma una imagen con la c�mara. El cliente es responsable de la eliminaci�n de dicha imagen.
        * @return Imagen tomada*/
    virtual int GetSize(SizeAxis command);
    /**< Obtiene el tama�o actual de la imagen a tomar.
        * @param command El alto o el ancho.
        * @return Valor en p�xeles de la dimensi�n solicitada.*/
    virtual int GetMaxSize(SizeAxis command)=0;
    /**< Obtiene el tama�o m�ximo de la imagen a tomar.
        * @param command El alto o el ancho.
        * @return Valor en p�xeles m�ximo de la dimensi�n solicitada.*/
    virtual int GetMinSize(SizeAxis command)=0;
    /**< Obtiene el tama�o m�nimo de la imagen a tomar.
        * @param command El alto o el ancho.
        * @return Valor en p�xeles m�ximo de la dimensi�n solicitada.*/
    virtual int SetSize(SizeAxis command,int value)=0;
    /**< Configura el tama�o de la imagen a tomar.
        * @param command El alto o el ancho.
        * @param value Valor en p�xeles a configurar.
        * @return Valor en p�xeles realmente configurado para la dimensi�n indicada.*/

    virtual int GetCurrentGain(GainType command);
    /**< Obtiene el valor actual de la ganancia.
        * @param command Para indicar cu�l de todas las ganancias.
        * @return Valor de la ganancia.*/
    virtual int GetMinGain(GainType command);
    /**< Obtiene el valor m�nimo de la ganancia.
        * @param command Para indicar cu�l de todas las ganancias.
        * @return Valor de la ganancia m�nima.*/
    virtual int GetMaxGain(GainType command);
    /**< Obtiene el valor m�ximo de la ganancia.
        * @param command Para indicar cu�l de todas las ganancias.
        * @return Valor de la ganancia m�xima.*/
    virtual int GetDefaultGain(GainType command)=0;
    /**< Obtiene el valor por defecto de la ganancia.
        * @param command Para indicar cu�l de todas las ganancias.
        * @return Valor de la ganancia por defecto.*/
    virtual int SetGain(GainType command, int value)=0;
    /**< Configura el valor de la ganancia.
        * @param command Para indicar cu�l de todas las ganancias.
        * @param value Valor para configurar.
        * @return Valor realmente configurado para la ganancia indicada.*/

    virtual double GetExposure(ExposureParameter command);
    /**< Obtiene un valor para la exposici�n.
        * @param command Para indicar si el valor m�ximo, m�nimo o actual.
        * @return Valor de la exposici�n en milisegundos.*/
    virtual double GetDefaultExposure();
    /**< Obtiene el valor por defecto para la exposici�n.
        * @return Valor de la exposici�n en milisegundos.*/
    virtual double SetExposure(double value)=0;
    /**< Configura el valor para la exposici�n.
        * @param value Valor en milisegundos a configurar.
        * @return Valor de la exposici�n en milisegundos realmente configurado.*/

    virtual int GetPixelClock(PixelClockParameter command);
    /**< Obtiene un valor para el reloj de pixel.
        * @param command Para indicar si el valor m�ximo, m�nimo o actual.
        * @return Valor del reloj de pixel en megahercios.*/
    virtual int GetDefaultPixelClock()=0;
    /**< Obtiene el valor por defecto para  el reloj de pixel.
        * @return Valor del reloj de pixel en megahercios.*/
    virtual int SetPixelClock(int value)=0;
    /**< Configura el valor para  el reloj de pixel.
        * @param value Valor en milisegundos a configurar.
        * @return Valor del reloj de pixel en megahercios realmente configurado.*/

    virtual double GetFrameRate(FramerateParameter command);
    /**< Obtiene un valor para la tasa de toma de im�genes.
        * @param command Para indicar si el valor m�ximo, m�nimo o actual.
        * @return Valor de la de la tasa de toma de im�genes en im�genes por segundo.*/
    virtual double GetDefaultFrameRate()=0;
    /**< Obtiene el valor por defecto para la tasa de toma de im�genes.
        * @return Valor de la de la tasa de toma de im�genes en im�genes por segundo.*/
    virtual double SetFrameRate(double value)=0;
    /**< Configura el valor para la tasa de toma de im�genes.
        * @param value Valor en milisegundos a configurar.
        * @return Valor de la de la tasa de toma de im�genes en im�genes por segundo realmente configurado.*/

    CameraParameters SetParameters(CameraParameters parameters_in);
    /**< Configura un set completo de par�metros.
        * @param parameters_in Set de par�metros.
        * @return Set de par�metros realmente configurado.*/
    CameraParameters GetParameters();
    /**< Devuelve el set de par�metros.
        * @return Set de par�metros.*/

    virtual bool IsOpen()=0;
    /**< Indica si la c�mara est� correctamente abierta y en funcionamiento.
        * @return Verdadero, s� la c�mara esta detectada y funcionando correctamente.*/
};

#endif // ABSTRACTCAMERA_H
