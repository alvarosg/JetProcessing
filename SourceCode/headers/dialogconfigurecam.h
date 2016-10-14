/**
 * @file   dialogconfigurecam.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase para el cuadro de diálogo que permite configurar la cámara.
 *
 */
#ifndef DIALOGCONFIGURECAM_H
#define DIALOGCONFIGURECAM_H

#define CC_CAPTION             "Camera Configuration"           /**< Etiqueta.*/

#define CC_PIXELCLOCK_LABEL    "Pixel Clock:"                   /**< Etiqueta.*/
#define CC_FRAMERATE_LABEL     "Frame Rate:"                    /**< Etiqueta.*/
#define CC_EXPOSURE_LABEL      "Exposure:"                      /**< Etiqueta.*/

#define CC_PIXELCLOCK_UNITS    " MHz"                           /**< Etiqueta.*/
#define CC_FRAMERATE_UNITS     " fps"                           /**< Etiqueta.*/
#define CC_EXPOSURE_UNITS      " ms"                            /**< Etiqueta.*/

#define CC_MASTERGAIN_LABEL    "Master Gain: "                  /**< Etiqueta.*/
#define CC_REDGAIN_LABEL       "Red Gain: "                     /**< Etiqueta.*/
#define CC_GREENGAIN_LABEL     "Green Gain: "                   /**< Etiqueta.*/
#define CC_BLUEGAIN_LABEL      "Blue Gain: "                    /**< Etiqueta.*/

#define CC_REFRESH_LABEL       "Refresh Preview"                /**< Etiqueta.*/
#define CC_AUTOREFRESH_LABEL   "Auto Refresh"                   /**< Etiqueta.*/

#define CC_WIDTH_LABEL         "Width:"                         /**< Etiqueta.*/
#define CC_HEIGHT_LABEL        "Height:"                        /**< Etiqueta.*/

#define CC_LOAD_LABEL          "Load from File"                 /**< Etiqueta.*/
#define CC_SAVE_LABEL          "Save to File"                   /**< Etiqueta.*/
#define CC_RESET_LABEL         "Reset Defaults"                 /**< Etiqueta.*/

#define CC_FAILSAVE_LABEL      "Failed to save the file: "      /**< Etiqueta.*/
#define CC_FAILLOAD_LABEL      "Corrupt or incomplete file: "   /**< Etiqueta.*/

#define CC_ERROR_LABEL         "Error"                          /**< Etiqueta.*/


#include <QDialog>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include "cameracontroller.h"

/*! \class DialogConfigureCam dialogconfigurecam.h "dialogconfigurecam.h"
 * \brief Clase para el control del cuadro de diálogo que permite configurar los parámetros de la cámara.
 *
 * Esta clase actúa de nexo entre el controlador de la cámara y la clase que contiene los widgets: Ui::DialogConfigureCam.
 *
 */

namespace Ui {
class DialogConfigureCam;
}

class DialogConfigureCam : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogConfigureCam(QWidget *parent,CameraController * cameraController_in);
    /**< Constructor.
      @param parent Widget padre.
      @param cameraController_in Objeto controlador de la cámara.*/
    ~DialogConfigureCam();
    /**< Destructor.*/
    
private:
    CameraController * cameraController;    /**< Controlador de la cámara pasado como argumento.*/
    CameraParameters initValues;            /**< Para guardar los parámetros iniciales y restaurarlos en caso de cancelar.*/
    QTimer * timerAutoRefresh;              /**< Temporizador para el autorefresco de la vista previa.*/

private:
    void InitControls();
    /**< Inicia los controles durante la creación.*/
    void UpdateControls();
    /**< Función que actualiza los controles en función de los parámetros reales.*/

public slots:
    void PixelClockChanged(int value);
    /**< Slot llamado cuando el valor de la barra del reloj de pixel ha variado.
        @param value Nuevo valor en la barra.*/
    void FrameRateChanged(int value);
    /**< Slot llamado cuando el valor de la barra de la tasa de toma d eimágenes ha variado.
        @param value Nuevo valor en la barra.*/
    void ExposureChanged(int value);
    /**< Slot llamado cuando el valor de la barra de la exposición ha variado.
        @param value Nuevo valor en la barra.*/
    void MasterGainChanged(int value);
    /**< Slot llamado cuando el valor de la barra de la ganancia máster ha variado.
        @param value Nuevo valor en la barra.*/
    void RedGainChanged(int value);
    /**< Slot llamado cuando el valor de la barra de la ganancia de rojo ha variado.
        @param value Nuevo valor en la barra.*/
    void GreenGainChanged(int value);
    /**< Slot llamado cuando el valor de la barra de la ganancia de verde ha variado.
        @param value Nuevo valor en la barra.*/
    void BlueGainChanged(int value);
    /**< Slot llamado cuando el valor de la barra de la ganancia de azul ha variado.
        @param value Nuevo valor en la barra.*/
    void SizeXChanged();
    /**< Slot llamado cuando el valor en la caja de texto para el ancho en píxeles de la imagen ha variado.*/
    void SizeYChanged();
    /**< Slot llamado cuando el valor en la caja de texto para el alto en píxeles de la imagen ha variado.*/
    void UpdatePreview();
    /**< Slot llamado para actualizar la vista previa.*/
    void EnableAutoRefresh();
    /**< Slot llamado cuando se cambia el valor de la "CheckBox" que permite actualizar automáticamente o no la vista previa.*/
    void ResetDefaults();
    /**< Slot llamado para resetear todos los parámetros a los valores por defecto.*/
    void LoadConfig();
    /**< Slot llamado para cargar los parámetros de un fichero.*/
    void SaveConfig();
    /**< Slot llamado para guardar los parámetros a un fichero.*/
    void Accept();
    /**< Slot llamado cuando el cuadro de diálogo ha sido aceptado.*/
    void Cancel();
    /**< Slot llamado cuando el cuadro de diálogo ha sido rechazado.*/

private:
    Ui::DialogConfigureCam *ui;             /**< Puntero a la clase que contiene la interfaz gráfica. Esta clase es generada automáticamente por Qt Designer.*/

};

#endif // DIALOGCONFIGURECAM_H
