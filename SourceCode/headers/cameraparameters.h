/**
 * @file   cameraparameters.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase que contiene los parámetros de configuración de la cámara.
 *
 */
#ifndef CAMERAPARAMETERS_H
#define CAMERAPARAMETERS_H

#include <QString>
#include <QSettings>

/*! \class CameraParameters cameraparameters.h "cameraparameters.h"
 * \brief Clase que contiene un set de parámetros para configuración de la cámara.
 *
 */


class CameraParameters{

private:
    int sizeX;                          /**< Ancho en píxeles de la imagen a tomar.*/
    int sizeY;                          /**< Alto en píxeles de la imagen a tomar.*/
    int pixelClock;                     /**< Reloj de pixel, en megahercios.*/
    double frameRate;                   /**< Tasa de toma de imágenes, en imágenes por segundo.*/
    double exposureTime;                /**< Tiempo de exposición en milisegundos.*/
    int gainM;                          /**< Ganancia máster.*/
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
    /**< Carga los parámetros en un fichero.
      @param filePath Ruta y nombre del fichero de donde se deben leer.
      @return Verdadero si se cargaron correctamente todos los parámetros.*/
    bool SaveToFile(QString filePath);
    /**< Guarda los parámetros en un fichero.
      @param filePath Ruta y nombre del fichero donde se deben guardar.
      @return Verdadero si se guardó corectamente.*/

};

#endif // CAMERAPARAMETERS_H
