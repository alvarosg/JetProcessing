/**
 * @file   cameraueye.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase que implementa una c�mara usando la api proporcioanda por ids-imaging para c�maras uEye.
 *
 */
#ifndef CAMERAUEYE_H
#define CAMERAUEYE_H

#include<stdio.h>
#include<ctime>
#include<uEye.h>
#include"abstractcamera.h"

/*! \class CameraUEye cameraueye.h "cameraueye.h"
 * \brief Implementaci�n de AbstractCamera para c�maras uEye.
 *
 * Utiliza la API proporcionada por ids-imaging.
 *
 */


class CameraUEye : public AbstractCamera{

private:
    /** Mantiene o no la configuraci�n al abrir una c�mara.*/
    enum ConfigChanges {
        KEEP_CONFIG , /**< La mantiene.*/
        RESET_CONFIG  /**< La reinicia.*/
    };


    HIDS hCam; /**< Handle a la c�mara actualmente abierta. 0 si ninguna.*/

    char * imageMemory; /**< Puntero a la zona de memoria donde la c�mara escribe la imagen.*/
    int memoryId;       /**< Identificador de la zona de memoria de la c�mara.*/

    int colorMode;      /**< Modo de color.*/
    int	bitsPerPixel;   /**< Bits de informaci�n por cada pixel.*/
    int nColorCorrection; /**< Tipo de correcci�n de color.*/
    int nBayerMode;     /**< Modo bayer.*/
    int renderMode;     /**< Modo de renderizaci�n.*/

    bool OpenCamera(ConfigChanges config);
    /**< Abre la c�mara.
        * @param config Indica si debe mantener o reiniciar la configuraci�n.
        * @return Verdadero si se abri� correctamente.*/
    void SetDefaultConfigValues();
    /**< Establece las variables a los valores por defecto.*/
    void LoadParameters();
    /**< Configura la c�mara con los valores encontrados en las variables.*/
    void CloseCamera();
    /**< Cierra la c�mara.*/


public:
    CameraUEye();
    /**< Constructor. Crea y abre una c�mara.*/
    ~CameraUEye();
    /**< Destructor.*/

    QImage * AcquireSourcePicture();

    int GetSize(SizeAxis command);
    int GetMaxSize(SizeAxis command);
    int GetMinSize(SizeAxis command);
    int SetSize(SizeAxis command,int value);

    int GetCurrentGain(GainType command);
    int GetDefaultGain(GainType command);
    int SetGain(GainType command, int value);

    double GetExposure(ExposureParameter command);
    double SetExposure(double value);

    int GetPixelClock(PixelClockParameter command);
    int GetDefaultPixelClock();
    int SetPixelClock(int value);

    double GetFrameRate(FramerateParameter command);
    double GetDefaultFrameRate();
    double SetFrameRate(double value);

    bool IsOpen();

};

#endif // CAMERAUEYE_H
