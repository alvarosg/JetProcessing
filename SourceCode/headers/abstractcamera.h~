/**
 * @file   abstractcamera.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase abstracta para representar una cámara.
 *
 */
#ifndef ABSTRACTCAMERA_H
#define ABSTRACTCAMERA_H

#include<stdio.h>
#include"sourcepicture.h"
#include"cameraparameters.h"

/** Tipos de ganancia. */
enum GainType {
    GAIN_MASTER , /**< Ganancia máster. */
    GAIN_RED,     /**< Ganancia de rojo. */
    GAIN_GREEN,   /**< Ganancia de verde. */
    GAIN_BLUE     /**< Ganancia de azul. */
};
/** Valores de exposición. */
enum ExposureParameter {
    EXPOSURE_MAX, /**< Máximo. */
    EXPOSURE_MIN, /**< Míximo. */
    EXPOSURE      /**< Actual. */
};
/** Valores de tasa de toma de imágenes. */
enum FramerateParameter {
    FRAMERATE_MAX,/**< Máximo. */
    FRAMERATE_MIN,/**< Mínimo. */
    FRAMERATE     /**< Actual. */
};
/** Valores del reloj de pixel. */
enum PixelClockParameter {
    PIXELCLOCK_MAX,/**< Máximo. */
    PIXELCLOCK_MIN,/**< Mínimo. */
    PIXELCLOCK     /**< Actual. */
};
/** Valores de dimensiones en ejes.*/
enum SizeAxis {
    SIZE_X ,       /**< Ancho. */
    SIZE_Y         /**< Alto. */
};

/*! \class AbstractCamera abstractcamera.h "abstractcamera.h"
 * \brief Clase abstracta que representa una cámara.
 *
 * Define una interfaz que debe ser implementada por cualquier cámara, que debe ser programada derivando una clase de ésta.
 *
 *
 */


class AbstractCamera{

protected:

    CameraParameters parameters; /**< Parámetros actuales de configuración de la cámara.*/

public:

    virtual QImage * AcquireSourcePicture()=0;
    /**< Toma una imagen con la cámara. El cliente es responsable de la eliminación de dicha imagen.
        * @return Imagen tomada*/
    virtual int GetSize(SizeAxis command);
    /**< Obtiene el tamaño actual de la imagen a tomar.
        * @param command El alto o el ancho.
        * @return Valor en píxeles de la dimensión solicitada.*/
    virtual int GetMaxSize(SizeAxis command)=0;
    /**< Obtiene el tamaño máximo de la imagen a tomar.
        * @param command El alto o el ancho.
        * @return Valor en píxeles máximo de la dimensión solicitada.*/
    virtual int GetMinSize(SizeAxis command)=0;
    /**< Obtiene el tamaño mínimo de la imagen a tomar.
        * @param command El alto o el ancho.
        * @return Valor en píxeles míximo de la dimensión solicitada.*/
    virtual int SetSize(SizeAxis command,int value)=0;
    /**< Configura el tamaño de la imagen a tomar.
        * @param command El alto o el ancho.
        * @param value Valor en píxeles a configurar.
        * @return Valor en píxeles realmente configurado para la dimensión indicada.*/

    virtual int GetCurrentGain(GainType command);
    /**< Obtiene el valor actual de la ganancia.
        * @param command Para indicar cuál de todas las ganancias.
        * @return Valor de la ganancia.*/
    virtual int GetMinGain(GainType command);
    /**< Obtiene el valor mínimo de la ganancia.
        * @param command Para indicar cuál de todas las ganancias.
        * @return Valor de la ganancia mínima.*/
    virtual int GetMaxGain(GainType command);
    /**< Obtiene el valor máximo de la ganancia.
        * @param command Para indicar cuál de todas las ganancias.
        * @return Valor de la ganancia máxima.*/
    virtual int GetDefaultGain(GainType command)=0;
    /**< Obtiene el valor por defecto de la ganancia.
        * @param command Para indicar cuál de todas las ganancias.
        * @return Valor de la ganancia por defecto.*/
    virtual int SetGain(GainType command, int value)=0;
    /**< Configura el valor de la ganancia.
        * @param command Para indicar cuál de todas las ganancias.
        * @param value Valor para configurar.
        * @return Valor realmente configurado para la ganancia indicada.*/

    virtual double GetExposure(ExposureParameter command);
    /**< Obtiene un valor para la exposición.
        * @param command Para indicar si el valor máximo, mínimo o actual.
        * @return Valor de la exposición en milisegundos.*/
    virtual double GetDefaultExposure();
    /**< Obtiene el valor por defecto para la exposición.
        * @return Valor de la exposición en milisegundos.*/
    virtual double SetExposure(double value)=0;
    /**< Configura el valor para la exposición.
        * @param value Valor en milisegundos a configurar.
        * @return Valor de la exposición en milisegundos realmente configurado.*/

    virtual int GetPixelClock(PixelClockParameter command);
    /**< Obtiene un valor para el reloj de pixel.
        * @param command Para indicar si el valor máximo, mínimo o actual.
        * @return Valor del reloj de pixel en megahercios.*/
    virtual int GetDefaultPixelClock()=0;
    /**< Obtiene el valor por defecto para  el reloj de pixel.
        * @return Valor del reloj de pixel en megahercios.*/
    virtual int SetPixelClock(int value)=0;
    /**< Configura el valor para  el reloj de pixel.
        * @param value Valor en milisegundos a configurar.
        * @return Valor del reloj de pixel en megahercios realmente configurado.*/

    virtual double GetFrameRate(FramerateParameter command);
    /**< Obtiene un valor para la tasa de toma de imágenes.
        * @param command Para indicar si el valor máximo, mínimo o actual.
        * @return Valor de la de la tasa de toma de imágenes en imágenes por segundo.*/
    virtual double GetDefaultFrameRate()=0;
    /**< Obtiene el valor por defecto para la tasa de toma de imágenes.
        * @return Valor de la de la tasa de toma de imágenes en imágenes por segundo.*/
    virtual double SetFrameRate(double value)=0;
    /**< Configura el valor para la tasa de toma de imágenes.
        * @param value Valor en milisegundos a configurar.
        * @return Valor de la de la tasa de toma de imágenes en imágenes por segundo realmente configurado.*/

    CameraParameters SetParameters(CameraParameters parameters_in);
    /**< Configura un set completo de parámetros.
        * @param parameters_in Set de parámetros.
        * @return Set de parámetros realmente configurado.*/
    CameraParameters GetParameters();
    /**< Devuelve el set de parámetros.
        * @return Set de parámetros.*/

    virtual bool IsOpen()=0;
    /**< Indica si la cámara está correctamente abierta y en funcionamiento.
        * @return Verdadero, sí la cámara esta detectada y funcionando correctamente.*/
};

#endif // ABSTRACTCAMERA_H
