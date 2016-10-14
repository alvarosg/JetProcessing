/**
 * @file   cameracontroller.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase controlador para la c�mara.
 *
 */

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

/** Tipos de c�maras admitidas por el programa.*/
enum CameraType {
    CAMERA_SIMULATOR,    /**< Simulador de c�mara.*/
    CAMERA_UEYE          /**< C�mara uEye.*/
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
 * \brief Clase que se encarga de manejar y gestionar la c�mara, del tipo que sea.
 *
 * Posee un hilo independiente que se encarga de tomar y almacenar una imagen en un buffer para tenerla siempre disponible y no hacer esperar al cliente.
 *
 *
 */

class CameraController : QThread
{

private:
    QImage * buffer;                       /**< Buffer con la �ltima imagen tomada, se renueva, cada vez que la obtiene el cliente o se renueva expl�citamente.*/
    QSemaphore * semaphoreProducer;        /**< Sem�foro de espera para el productor, deber� esperar a que el cliente haya obtenido la imagen anterior.*/
    QSemaphore * semaphoreConsumer;        /**< Sem�foro de espera para el consumidor, debera esperar a que haya una imagen disponible.*/
    QSemaphore * semaphoreEnding;          /**< Sem�foro para acabar la ejecuci�n del hilo. Las solicitudes a este sem�foro se har�n no bloqueantes.*/
    QMutex * mutexPicture;                 /**< Mutex para asegurar atomicidad cuando se trabaja sobre el buffer*/
    AbstractCamera * camera;               /**< Puntero a la c�mara que se est� usando*/
    AbstractCameraFactory * cameraFactory; /**< Factor�a para crear las c�maras siempre del mismo tipo.*/
    CameraParameters cameraParameters;     /**< Par�metros de configuraci�n de la c�mara.*/
    bool init;                             /**< Variable que indica si la c�mara ha sido correctamente inicializada.*/
    void run();                            /**< M�todo reimplementado de QThread que ejecuta el hilo independiente.*/

public:
    CameraController(CameraType cameraType);
    /**< Constructor.
        @param cameraType Tipo de c�mara para la que se est� creando el controlador.*/
    ~CameraController();
    /**< Destructor*/
    bool Initialize();
    /**< Funci�n que intenta inicializar la c�mara. Creando en caso de �xito el hilo independiente y que comienza a tomar la primera imagen.
        @return Verdadero si se inicializ� correctamente.*/
    QImage * GetSourcePicture();
    /**< Devuelve una imagen de la c�mara. El controlador pasar� a tomar otra.
        @return �ltima imagen tomada. Siempre distinto de NULL.*/
    void RenewPicture();
    /**< Renueva la imagen del buffer. Es decir desecha la actual y toma otra.*/

    //M�todos de acceso a par�matros de la c�mara
    int GetSize(SizeAxis command);
    /**< Obtiene el tama�o actual de la imagen a tomar.
        * @param command El alto o el ancho.
        * @return Valor en p�xeles de la dimensi�n solicitada.*/
    int GetMaxSize(SizeAxis command);
    /**< Obtiene el tama�o m�ximo de la imagen a tomar.
        * @param command El alto o el ancho.
        * @return Valor en p�xeles m�ximo de la dimensi�n solicitada.*/
    int GetMinSize(SizeAxis command);
    /**< Obtiene el tama�o m�nimo de la imagen a tomar.
        * @param command El alto o el ancho.
        * @return Valor en p�xeles m�ximo de la dimensi�n solicitada.*/
    int SetSize(SizeAxis command,int value);
    /**< Configura el tama�o de la imagen a tomar.
        * @param command El alto o el ancho.
        * @param value Valor en p�xeles a configurar.
        * @return Valor en p�xeles realmente configurado para la dimensi�n indicada.*/

    int GetCurrentGain(GainType command);
    /**< Obtiene el valor actual de la ganancia.
        * @param command Para indicar cu�l de todas las ganancias.
        * @return Valor de la ganancia.*/
    int GetDefaultGain(GainType command);
    /**< Obtiene el valor m�nimo de la ganancia.
        * @param command Para indicar cu�l de todas las ganancias.
        * @return Valor de la ganancia m�nima.*/
    int GetMinGain(GainType command);
    /**< Obtiene el valor m�ximo de la ganancia.
        * @param command Para indicar cu�l de todas las ganancias.
        * @return Valor de la ganancia m�xima.*/
    int GetMaxGain(GainType command);
    /**< Obtiene el valor por defecto de la ganancia.
        * @param command Para indicar cu�l de todas las ganancias.
        * @return Valor de la ganancia por defecto.*/
    int SetGain(GainType command, int value);
    /**< Configura el valor de la ganancia.
        * @param command Para indicar cu�l de todas las ganancias.
        * @param value Valor para configurar.
        * @return Valor realmente configurado para la ganancia indicada.*/

    double GetExposure(ExposureParameter command);
    /**< Obtiene un valor para la exposici�n.
        * @param command Para indicar si el valor m�ximo, m�nimo o actual.
        * @return Valor de la exposici�n en milisegundos.*/
    double GetDefaultExposure();
    /**< Obtiene el valor por defecto para la exposici�n.
        * @return Valor de la exposici�n en milisegundos.*/
    double SetExposure(double value);
    /**< Configura el valor para la exposici�n.
        * @param value Valor en milisegundos a configurar.
        * @return Valor de la exposici�n en milisegundos realmente configurado.*/

    int GetPixelClock(PixelClockParameter command);
    /**< Obtiene un valor para el reloj de pixel.
        * @param command Para indicar si el valor m�ximo, m�nimo o actual.
        * @return Valor del reloj de pixel en megahercios.*/
    int GetDefaultPixelClock();
    /**< Obtiene el valor por defecto para  el reloj de pixel.
        * @return Valor del reloj de pixel en megahercios.*/
    int SetPixelClock(int value);
    /**< Configura el valor para  el reloj de pixel.
        * @param value Valor en milisegundos a configurar.
        * @return Valor del reloj de pixel en megahercios realmente configurado.*/

    double GetFrameRate(FramerateParameter command);
    /**< Obtiene un valor para la tasa de toma de im�genes.
        * @param command Para indicar si el valor m�ximo, m�nimo o actual.
        * @return Valor de la de la tasa de toma de im�genes en im�genes por segundo.*/
    double GetDefaultFrameRate();
    /**< Obtiene el valor por defecto para la tasa de toma de im�genes.
        * @return Valor de la de la tasa de toma de im�genes en im�genes por segundo.*/
    double SetFrameRate(double value);
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

    bool CameraIsReady();
    /**< Funci�n para averiguar el estado actual de la c�mara. Si no estaba inicializada intenta inicializarla antes de reponder.
        @return Verdadero si la c�mara est� funcionando correctamente o Falso si la c�mara no funciona o no se detecta.*/

signals:
    void finished();
    /**< Se�al que se emite cuando el hilo de tomar im�genes finaliza su ejecuci�n.*/
};

#endif // CAMERACONTROLLER_H
