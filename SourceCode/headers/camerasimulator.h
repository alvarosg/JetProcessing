/**
 * @file   camerasimulator.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase que simula el comportamiento de una c�mara.
 *
 */
#ifndef CAMERASIMULATOR_H
#define CAMERASIMULATOR_H

#include "abstractcamera.h"
#include <QThread>
#include <QDir>
#include <QCoreApplication>

/*! \class CameraSimulator camerasimulator.h "camerasimulator.h"
 * \brief Implementaci�n de AbstractCamera con un simulador de c�mara.
 *
 * Simula el funcionamiento d una c�mara CCD, provocando retados y cambiando los valores m�ximos y m�nimos para unos par�metros en funci�n de otros.
 *
 * Lee im�genes secuencialmente de un directorio llamado CameraSimulator en el directorio del ejecutable.
 *
 */

class CameraSimulator : public AbstractCamera
{

private:
    void SetDefaultConfigValues();
    /**< Establece las variables a los valores por defecto.*/
    QString imageFolder;
    /**< Directorio de d�nde lee las im�genes.*/
    QStringList imageList;
    /**< Lista con los nombres de las im�genes que ha encontrado en el directorio de im�genes.*/


public:
    CameraSimulator();
    /**< Constructor. Crea y abre una c�mara.*/

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
    /**< Interrumpe la ejecuci�n durante cierto tiempo.
        @param secs segundos durante los que se interrumpe la ejecuci�n.*/

    static void msleep(unsigned long msecs) {
        QThread::msleep(msecs);
    }
    /**< Interrumpe la ejecuci�n durante cierto tiempo.
        @param msecs milisegundos durante los que se interrumpe la ejecuci�n.*/

    static void usleep(unsigned long usecs) {
        QThread::usleep(usecs);
    }
    /**< Interrumpe la ejecuci�n durante cierto tiempo.
        @param usecs microsegundos durante los que se interrumpe la ejecuci�n.*/
};

#endif // CAMERASIMULATOR_H
