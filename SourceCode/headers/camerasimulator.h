/**
 * @file   camerasimulator.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase que simula el comportamiento de una cámara.
 *
 */
#ifndef CAMERASIMULATOR_H
#define CAMERASIMULATOR_H

#include "abstractcamera.h"
#include <QThread>
#include <QDir>
#include <QCoreApplication>

/*! \class CameraSimulator camerasimulator.h "camerasimulator.h"
 * \brief Implementación de AbstractCamera con un simulador de cámara.
 *
 * Simula el funcionamiento d una cámara CCD, provocando retados y cambiando los valores máximos y mínimos para unos parámetros en función de otros.
 *
 * Lee imágenes secuencialmente de un directorio llamado CameraSimulator en el directorio del ejecutable.
 *
 */

class CameraSimulator : public AbstractCamera
{

private:
    void SetDefaultConfigValues();
    /**< Establece las variables a los valores por defecto.*/
    QString imageFolder;
    /**< Directorio de dónde lee las imágenes.*/
    QStringList imageList;
    /**< Lista con los nombres de las imágenes que ha encontrado en el directorio de imágenes.*/


public:
    CameraSimulator();
    /**< Constructor. Crea y abre una cámara.*/

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

/*! \class SleeperClass camerasimulator.h "camerasimulator.h"
 * \brief Clase para detener a un hilo durante un tiempo determinado.
 *
 */

class SleeperClass : public QThread
{
public:

    static void sleep(unsigned long secs) {
        QThread::sleep(secs);
    }
    /**< Interrumpe la ejecución durante cierto tiempo.
        @param secs segundos durante los que se interrumpe la ejecución.*/

    static void msleep(unsigned long msecs) {
        QThread::msleep(msecs);
    }
    /**< Interrumpe la ejecución durante cierto tiempo.
        @param msecs milisegundos durante los que se interrumpe la ejecución.*/

    static void usleep(unsigned long usecs) {
        QThread::usleep(usecs);
    }
    /**< Interrumpe la ejecución durante cierto tiempo.
        @param usecs microsegundos durante los que se interrumpe la ejecución.*/
};

#endif // CAMERASIMULATOR_H
