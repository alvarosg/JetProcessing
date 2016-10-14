/**
 * @file   cameraparameters.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase que contiene los par�metros de configuraci�n de la c�mara.
 *
 */
#ifndef CAMERAPARAMETERS_H
#define CAMERAPARAMETERS_H

#include <QString>
#include <QSettings>

/*! \class CameraParameters cameraparameters.h "cameraparameters.h"
 * \brief Clase que contiene un set de par�metros para configuraci�n de la c�mara.
 *
 */


class CameraParameters{

private:
    int sizeX;                          /**< Ancho en p�xeles de la imagen a tomar.*/
    int sizeY;                          /**< Alto en p�xeles de la imagen a tomar.*/
    int pixelClock;                     /**< Reloj de pixel, en megahercios.*/
    double frameRate;                   /**< Tasa de toma de im�genes, en im�genes por segundo.*/
    double exposureTime;                /**< Tiempo de exposici�n en milisegundos.*/
    int gainM;                          /**< Ganancia m�ster.*/
    int gainR;                          /**< Ganancia de rojo.*/
    int gainG;                          /**< Ganancia de verde.*/
    int gainB;                          /**< Ganancia de azul.*/

public:
    CameraParameters();
    /**< Constructor.*/
    void SetSizeX(int value);
    /**< Establece el valor del atributo sizeX.
        @param value Valor para el atributo.*/
    void SetSizeY(int value);
    /**< Establece el valor del atributo sizeY.
        @param value Valor para el atributo.*/
    void SetPixelClock(int value);
    /**< Establece el valor del atributo pixelClock.
        @param value Valor para el atributo.*/
    void SetFrameRate(double value);
    /**< Establece el valor del atributo frameRate.
        @param value Valor para el atributo.*/
    void SetExposure(double value);
    /**< Establece el valor del atributo exposureTime.
        @param value Valor para el atributo.*/
    void SetGainMaster(int value);
    /**< Establece el valor del atributo gainM.
        @param value Valor para el atributo.*/
    void SetGainRed(int value);
    /**< Establece el valor del atributo gainR.
        @param value Valor para el atributo.*/
    void SetGainGreen(int value);
    /**< Establece el valor del atributo gainG.
        @param value Valor para el atributo.*/
    void SetGainBlue(int value);
    /**< Establece el valor del atributo gainB.
        @param value Valor para el atributo.*/

    int GetSizeX();
    /**< Recupera el atributo sizeX.
        @return Valor del atributo.*/
    int GetSizeY();
    /**< Recupera el atributo sizeY.
        @return Valor del atributo.*/
    int GetPixelClock();
    /**< Recupera el atributo pixelClock.
        @return Valor del atributo.*/
    double GetFrameRate();
    /**< Recupera el atributo framRate.
        @return Valor del atributo.*/
    double GetExposure();
    /**< Recupera el atributo exposureTime.
        @return Valor del atributo.*/
    int GetGainMaster();
    /**< Recupera el atributo gainM.
        @return Valor del atributo.*/
    int GetGainRed();
    /**< Recupera el atributo gainR.
        @return Valor del atributo.*/
    int GetGainGreen();
    /**< Recupera el atributo gainG.
        @return Valor del atributo.*/
    int GetGainBlue();
    /**< Recupera el atributo gainB.
        @return Valor del atributo.*/

    bool LoadFromFile(QString filePath);
    /**< Carga los par�metros en un fichero.
      @param filePath Ruta y nombre del fichero de donde se deben leer.
      @return Verdadero si se cargaron correctamente todos los par�metros.*/
    bool SaveToFile(QString filePath);
    /**< Guarda los par�metros en un fichero.
      @param filePath Ruta y nombre del fichero donde se deben guardar.
      @return Verdadero si se guard� corectamente.*/

};

#endif // CAMERAPARAMETERS_H
