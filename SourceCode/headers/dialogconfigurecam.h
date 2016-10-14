/**
 * @file   dialogconfigurecam.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase para el cuadro de di�logo que permite configurar la c�mara.
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
 * \brief Clase para el control del cuadro de di�logo que permite configurar los par�metros de la c�mara.
 *
 * Esta clase act�a de nexo entre el controlador de la c�mara y la clase que contiene los widgets: Ui::DialogConfigureCam.
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
      @param cameraController_in Objeto controlador de la c�mara.*/
    ~DialogConfigureCam();
    /**< Destructor.*/
    
private:
    CameraController * cameraController;    /**< Controlador de la c�mara pasado como argumento.*/
    CameraParameters initValues;            /**< Para guardar los par�metros iniciales y restaurarlos en caso de cancelar.*/
    QTimer * timerAutoRefresh;              /**< Temporizador para el autorefresco de la vista previa.*/

private:
    void InitControls();
    /**< Inicia los controles durante la creaci�n.*/
    void UpdateControls();
    /**< Funci�n que actualiza los controles en funci�n de los par�metros reales.*/

public slots:
    void PixelClockChanged(int value);
    /**< Slot llamado cuando el valor de la barra del reloj de pixel ha variado.
        @param value Nuevo valor en la barra.*/
    void FrameRateChanged(int value);
    /**< Slot llamado cuando el valor de la barra de la tasa de toma d eim�genes ha variado.
        @param value Nuevo valor en la barra.*/
    void ExposureChanged(int value);
    /**< Slot llamado cuando el valor de la barra de la exposici�n ha variado.
        @param value Nuevo valor en la barra.*/
    void MasterGainChanged(int value);
    /**< Slot llamado cuando el valor de la barra de la ganancia m�ster ha variado.
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
    /**< Slot llamado cuando el valor en la caja de texto para el ancho en p�xeles de la imagen ha variado.*/
    void SizeYChanged();
    /**< Slot llamado cuando el valor en la caja de texto para el alto en p�xeles de la imagen ha variado.*/
    void UpdatePreview();
    /**< Slot llamado para actualizar la vista previa.*/
    void EnableAutoRefresh();
    /**< Slot llamado cuando se cambia el valor de la "CheckBox" que permite actualizar autom�ticamente o no la vista previa.*/
    void ResetDefaults();
    /**< Slot llamado para resetear todos los par�metros a los valores por defecto.*/
    void LoadConfig();
    /**< Slot llamado para cargar los par�metros de un fichero.*/
    void SaveConfig();
    /**< Slot llamado para guardar los par�metros a un fichero.*/
    void Accept();
    /**< Slot llamado cuando el cuadro de di�logo ha sido aceptado.*/
    void Cancel();
    /**< Slot llamado cuando el cuadro de di�logo ha sido rechazado.*/

private:
    Ui::DialogConfigureCam *ui;             /**< Puntero a la clase que contiene la interfaz gr�fica. Esta clase es generada autom�ticamente por Qt Designer.*/

};

#endif // DIALOGCONFIGURECAM_H
