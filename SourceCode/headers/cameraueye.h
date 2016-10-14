/**
 * @file   cameraueye.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase que implementa una cámara usando la api proporcioanda por ids-imaging para cámaras uEye.
 *
 */
#ifndef CAMERAUEYE_H
#define CAMERAUEYE_H

#include<stdio.h>
#include<ctime>
#include<uEye.h>
#include"abstractcamera.h"

/*! \class CameraUEye cameraueye.h "cameraueye.h"
 * \brief Implementación de AbstractCamera para cámaras uEye.
 *
 * Utiliza la API proporcionada por ids-imaging.
 *
 */


class CameraUEye : public AbstractCamera{

private:
    /** Mantiene o no la configuración al abrir una cámara.*/
    enum ConfigChanges {
        KEEP_CONFIG , /**< La mantiene.*/
        RESET_CONFIG  /**< La reinicia.*/
    };


    HIDS hCam; /**< Handle a la cámara actualmente abierta. 0 si ninguna.*/

    char * imageMemory; /**< Puntero a la zona de memoria donde la cámara escribe la imagen.*/
    int memoryId;       /**< Identificador de la zona de memoria de la cámara.*/

    int colorMode;      /**< Modo de color.*/
    int	bitsPerPixel;   /**< Bits de información por cada pixel.*/
    int nColorCorrection; /**< Tipo de corrección de color.*/
    int nBayerMode;     /**< Modo bayer.*/
    int renderMode;     /**< Modo de renderización.*/

    bool OpenCamera(ConfigChanges config);
    /**< Abre la cámara.
        * @param config Indica si debe mantener o reiniciar la configuración.
        * @return Verdadero si se abrió correctamente.*/
    void SetDefaultConfigValues();
    /**< Establece las variables a los valores por defecto.*/
    void LoadParameters();
    /**< Configura la cámara con los valores encontrados en las variables.*/
    void CloseCamera();
    /**< Cierra la cámara.*/


public:
    CameraUEye();
    /**< Constructor. Crea y abre una cámara.*/
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
