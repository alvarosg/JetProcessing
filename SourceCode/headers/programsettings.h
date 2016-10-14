/**
 * @file   programsettings.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase que contiene las preferencias del programa.
 *
 */
#ifndef PROGRAMSETTINGS_H
#define PROGRAMSETTINGS_H

#include <QString>
#include <QSettings>
#include <QDir>
#include <QSize>

/** Modos de funcionamiento del programa. */
enum CalcMode {
    MODE_CUDA=0 ,  /**< C�lculo con la GPU. */
    MODE_CPU=1     /**< C�lculo co la CPU. */
};

/*! \class ProgramSettings programsettings.h "programsettings.h"
 * \brief Clase que contiene las preferencias persistentes del programa.
 *
 */

class ProgramSettings
{

private:
    bool firstExecution;                /**< Indica si es la primera ejecuci�n del programa en el ordenador.*/
    bool useDefaultMode;                /**< Indica si se debe usar el modo de c�lculo por defecto o se debe preguntar al usuario.*/
    CalcMode mode;                      /**< Indica el modo de c�lculo que se est� utilizando.*/
    QString outputFolder;               /**< Indica un directorio elegido por el usuario para la salida de los archivos.*/
    bool saveCameraPictures;            /**< Indica si se debenn guardar en un directorio las im�genes tomadas con la c�mara.*/
    bool applyUntilCurrentAlgorithm;    /**< Indica si se debe aplicar s�lo hasta el algoritmo seleccionado, o por el contrario toda la lista.*/
    bool saveGraphicsViewSize;          /**< Indica si las im�genes de los gr�ficos se deben guardar con la resoluci�n mostrada en pantalla o con una elegida por el usuario.*/
    QSize customSize;                   /**< Tama�o con el que se guardan los gr�ficos, en caso de seleccionar guardado personalizado.*/
    QString defaultImageFormat;         /**< Indica la extensi�n por defecto con la que se guardan las im�genes de los gr�ficos. Consiste en una cadena con las letras de la extensi�n sin el punto.*/



public:
    ProgramSettings();
    /**< Constructor.*/
    void SetFirstExecution(bool value);
    /**< Establece el valor del atributo firstExecution.
        @param value Valor para el atributo.*/
    bool GetFirstExecution();
    /**< Recupera el atributo firstExecution.
        @return Valor del atributo.*/
    void SetUseDefaultMode(bool value);
    /**< Establece el valor del atributo useDefaultMode.
        @param value Valor para el atributo.*/
    bool GetUseDefaultMode();
    /**< Recupera el atributo useDefaultMode.
        @return Valor del atributo.*/
    void SetMode(CalcMode value);
    /**< Establece el valor del atributo mode.
        @param value Valor para el atributo.*/
    CalcMode GetMode();
    /**< Recupera el atributo mode.
        @return Valor del atributo.*/
    void SetOutputFolder(QString path);
    /**< Establece el valor del atributo outputFolder.
        @param path Valor para el atributo.*/
    QString GetOutputFolder();
    /**< Recupera el atributo outputFolder.
        @return Valor del atributo.*/
    void SetSaveCameraPictures(bool value);
    /**< Establece el valor del atributo saveCameraPictures.
        @param value Valor para el atributo.*/
    bool GetSaveCameraPictures();
    /**< Recupera el atributo saveCameraPictures.
        @return Valor del atributo.*/
    void SetApplyUntilCurrentAlgorithm(bool value);
    /**< Establece el valor del atributo applyUntilCurrentAlgorithm.
        @param value Valor para el atributo.*/
    bool GetApplyUntilCurrentAlgorithm();
    /**< Recupera el atributo applyUntilCurrentAlgorithm.
        @return Valor del atributo.*/
    void SetSaveGraphicsViewSize(bool value);
    /**< Establece el valor del atributo saveGraphicsViewSize.
        @param value Valor para el atributo.*/
    bool GetSaveGraphicsViewSize();
    /**< Recupera el atributo saveGraphicsViewSize.
        @return Valor del atributo.*/
    void SetCustomSize(QSize size);
    /**< Establece el valor del atributo customSize.
        @param size Valor para el atributo.*/
    QSize GetCustomSize();
    /**< Recupera el atributo customSize.
        @return Valor del atributo.*/
    void SetDefaultImageFormat(QString format);
    /**< Establece el valor del atributo defaultImageFormat.
        @param format Valor para el atributo.*/
    QString GetDefaultImageFormat();
    /**< Recupera el atributo defaultImageFormat.
        @return Valor del atributo.*/
    bool LoadFromFile(QString path="");
    /**< Carga la configuraci�n en un fichero.
      @param path Ruta y nombre del fichero de donde se deben leer. Si esta vac�o se guardar�n en la ubicaci�n autom�tica proporcionada por Qt.
      @return Verdadero si se cargaron correctamente todos los par�metros.*/

    bool SaveToFile(QString path="");
    /**< Guarda la configuraci� en un fichero.
      @param path Ruta y nombre del fichero donde se deben guardar. Si est� vac�o se guardar�n en la ubicaci�n autom�tica proporcionada por Qt.
      @return Verdadero si se guard� corectamente.*/


};

#endif // PROGRAMSETTINGS_H
