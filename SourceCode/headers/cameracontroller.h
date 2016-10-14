/**
 * @file   cameracontroller.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase controlador para la cámara.
 *
 */

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

/** Tipos de cámaras admitidas por el programa.*/
enum CameraType {
    CAMERA_SIMULATOR,    /**< Simulador de cámara.*/
    CAMERA_UEYE          /**< Cámara uEye.*/
};

#include<QThread>
#include<QMutex>
#include<QSemaphore>
#include"abstractcamerafactory.h"
#include"camerafactoryforueye.h"
#include"camerafactoryforsimulator.h"
#include"abstractcamera.h"
#include"sourcepicture.h"

/*! \class CameraController cameracontroller.h "cameracontroller.h"
 * \brief Clase que se encarga de manejar y gestionar la cámara, del tipo que sea.
 *
 * Posee un hilo independiente que se encarga de tomar y almacenar una imagen en un buffer para tenerla siempre disponible y no hacer esperar al cliente.
 *
 *
 */

class CameraController : QThread
{

private:
    QImage * buffer;                       /**< Buffer con la última imagen tomada, se renueva, cada vez que la obtiene el cliente o se renueva explícitamente.*/
    QSemaphore * semaphoreProducer;        /**< Semáforo de espera para el productor, deberá esperar a que el cliente haya obtenido la imagen anterior.*/
    QSemaphore * semaphoreConsumer;        /**< Semáforo de espera para el consumidor, debera esperar a que haya una imagen disponible.*/
    QSemaphore * semaphoreEnding;          /**< Semáforo para acabar la ejecución del hilo. Las solicitudes a este semáforo se harán no bloqueantes.*/
    QMutex * mutexPicture;                 /**< Mutex para asegurar atomicidad cuando se trabaja sobre el buffer*/
    AbstractCamera * camera;               /**< Puntero a la cámara que se está usando*/
    AbstractCameraFactory * cameraFactory; /**< Factoría para crear las cámaras siempre del mismo tipo.*/
    CameraParameters cameraParameters;     /**< Parámetros de configuración de la cámara.*/
    bool init;                             /**< Variable que indica si la cámara ha sido correctamente inicializada.*/
    void run();                            /**< Método reimplementado de QThread que ejecuta el hilo independiente.*/

public:
    CameraController(CameraType cameraType);
    /**< Constructor.
        @param cameraType Tipo de cámara para la que se está creando el controlador.*/
    ~CameraController();
    /**< Destructor*/
    bool Initialize();
    /**< Función que intenta inicializar la cámara. Creando en caso de éxito el hilo independiente y que comienza a tomar la primera imagen.
        @return Verdadero si se inicializó correctamente.*/
    QImage * GetSourcePicture();
    /**< Devuelve una imagen de la cámara. El controlador pasará a tomar otra.
        @return Última imagen tomada. Siempre distinto de NULL.*/
    void RenewPicture();
    /**< Renueva la imagen del buffer. Es decir desecha la actual y toma otra.*/

    //Métodos de acceso a parámatros de la cámara
    int GetSize(SizeAxis command);
    /**< Obtiene el tamaño actual de la imagen a tomar.
        * @param command El alto o el ancho.
        * @return Valor en píxeles de la dimensión solicitada.*/
    int GetMaxSize(SizeAxis command);
    /**< Obtiene el tamaño máximo de la imagen a tomar.
        * @param command El alto o el ancho.
        * @return Valor en píxeles máximo de la dimensión solicitada.*/
    int GetMinSize(SizeAxis command);
    /**< Obtiene el tamaño mínimo de la imagen a tomar.
        * @param command El alto o el ancho.
        * @return Valor en píxeles míximo de la dimensión solicitada.*/
    int SetSize(SizeAxis command,int value);
    /**< Configura el tamaño de la imagen a tomar.
        * @param command El alto o el ancho.
        * @param value Valor en píxeles a configurar.
        * @return Valor en píxeles realmente configurado para la dimensión indicada.*/

    int GetCurrentGain(GainType command);
    /**< Obtiene el valor actual de la ganancia.
        * @param command Para indicar cuál de todas las ganancias.
        * @return Valor de la ganancia.*/
    int GetDefaultGain(GainType command);
    /**< Obtiene el valor mínimo de la ganancia.
        * @param command Para indicar cuál de todas las ganancias.
        * @return Valor de la ganancia mínima.*/
    int GetMinGain(GainType command);
    /**< Obtiene el valor máximo de la ganancia.
        * @param command Para indicar cuál de todas las ganancias.
        * @return Valor de la ganancia máxima.*/
    int GetMaxGain(GainType command);
    /**< Obtiene el valor por defecto de la ganancia.
        * @param command Para indicar cuál de todas las ganancias.
        * @return Valor de la ganancia por defecto.*/
    int SetGain(GainType command, int value);
    /**< Configura el valor de la ganancia.
        * @param command Para indicar cuál de todas las ganancias.
        * @param value Valor para configurar.
        * @return Valor realmente configurado para la ganancia indicada.*/

    double GetExposure(ExposureParameter command);
    /**< Obtiene un valor para la exposición.
        * @param command Para indicar si el valor máximo, mínimo o actual.
        * @return Valor de la exposición en milisegundos.*/
    double GetDefaultExposure();
    /**< Obtiene el valor por defecto para la exposición.
        * @return Valor de la exposición en milisegundos.*/
    double SetExposure(double value);
    /**< Configura el valor para la exposición.
        * @param value Valor en milisegundos a configurar.
        * @return Valor de la exposición en milisegundos realmente configurado.*/

    int GetPixelClock(PixelClockParameter command);
    /**< Obtiene un valor para el reloj de pixel.
        * @param command Para indicar si el valor máximo, mínimo o actual.
        * @return Valor del reloj de pixel en megahercios.*/
    int GetDefaultPixelClock();
    /**< Obtiene el valor por defecto para  el reloj de pixel.
        * @return Valor del reloj de pixel en megahercios.*/
    int SetPixelClock(int value);
    /**< Configura el valor para  el reloj de pixel.
        * @param value Valor en milisegundos a configurar.
        * @return Valor del reloj de pixel en megahercios realmente configurado.*/

    double GetFrameRate(FramerateParameter command);
    /**< Obtiene un valor para la tasa de toma de imágenes.
        * @param command Para indicar si el valor máximo, mínimo o actual.
        * @return Valor de la de la tasa de toma de imágenes en imágenes por segundo.*/
    double GetDefaultFrameRate();
    /**< Obtiene el valor por defecto para la tasa de toma de imágenes.
        * @return Valor de la de la tasa de toma de imágenes en imágenes por segundo.*/
    double SetFrameRate(double value);
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

    bool CameraIsReady();
    /**< Función para averiguar el estado actual de la cámara. Si no estaba inicializada intenta inicializarla antes de reponder.
        @return Verdadero si la cámara está funcionando correctamente o Falso si la cámara no funciona o no se detecta.*/

signals:
    void finished();
    /**< Señal que se emite cuando el hilo de tomar imágenes finaliza su ejecución.*/
};

#endif // CAMERACONTROLLER_H
