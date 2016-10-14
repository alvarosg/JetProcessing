/**
 * @file   dialogsetreference.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase para el cuadro de di�logo que permite elegir y configurar la imagen de la fase de referencia.
 *
 */
#ifndef DIALOGSETREFERENCE_H
#define DIALOGSETREFERENCE_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include "maskselectionscene.h"
#include "dialogconfigurecam.h"
#include "sourcepicture.h"
#include "maincontroller.h"
#include "spectrogramplot.h"



#define SR_CAPTION              "Set Reference"             /**< Etiqueta.*/

#define SR_STEP0_LABEL          "Getting Image"             /**< Etiqueta.*/
#define SR_STEP1_LABEL          "Setting Size"              /**< Etiqueta.*/
#define SR_STEP2_LABEL          "Setting Mask"              /**< Etiqueta.*/
#define SR_STEP3_LABEL          "Setting Fourier Mask"      /**< Etiqueta.*/
#define SR_STEP4_LABEL          "Result"                    /**< Etiqueta.*/

#define SR_NEXT_LABEL           "Next"                      /**< Etiqueta.*/
#define SR_PREVIOUS_LABEL       "Previous"                  /**< Etiqueta.*/

#define SR_FROMFILE_LABEL       "Get Image From File"       /**< Etiqueta.*/
#define SR_FROMCAM_LABEL        "Get Image From Camera"     /**< Etiqueta.*/
#define SR_CAMSETTINGS_LABEL    "Camera Settings"           /**< Etiqueta.*/
#define SR_SAVEPIC_LABEL        "Save Picture To File"      /**< Etiqueta.*/
#define SR_FAILSAVEPIC_LABEL    "Cannot Save File: "        /**< Etiqueta.*/
#define SR_REALSIZEX_LABEL      "Width (mm): "              /**< Etiqueta.*/
#define SR_REALSIZEY_LABEL      "Height (mm): "             /**< Etiqueta.*/
#define SR_FAILLOADIMAGE_LABEL  "Cannot load the image file: "/**< Etiqueta.*/
#define SR_FAILCAMIMAGE_LABEL   "Cannot take the picture"   /**< Etiqueta.*/

#define SR_SIZE_X_LABEL         "Width (pixels): "          /**< Etiqueta.*/
#define SR_SIZE_Y_LABEL         "Height (pixels): "         /**< Etiqueta.*/

#define SR_HMIRROR_LABEL        "Horizontal Mirror"         /**< Etiqueta.*/
#define SR_VMIRROR_LABEL        "Vertical Mirror"           /**< Etiqueta.*/

#define SR_ROTATE_0_LABEL       "No Rotation"               /**< Etiqueta.*/
#define SR_ROTATE_90_LABEL      "Rotate 90�"                /**< Etiqueta.*/
#define SR_ROTATE_180_LABEL     "Rotate 180�"               /**< Etiqueta.*/
#define SR_ROTATE_270_LABEL     "Rotate 270�"               /**< Etiqueta.*/

#define SR_ORIGIN               "Origin Coordinates (x,y):" /**< Etiqueta.*/

#define SR_MASK                 "Mask Coordinates:"         /**< Etiqueta.*/
#define SR_MASK_XMIN     "X min: "                          /**< Etiqueta.*/
#define SR_MASK_XMAX     "X max: "                          /**< Etiqueta.*/
#define SR_MASK_YMIN     "Y min: "                          /**< Etiqueta.*/
#define SR_MASK_YMAX     "Y max: "                          /**< Etiqueta.*/
#define SR_MASK_HELPORIGIN     "(Double-Click on the left image)"                               /**< Etiqueta.*/
#define SR_MASK_HELPMASK       "(Click-&-Drag with the right mouse button on the left image)"   /**< Etiqueta.*/

#define SR_SAVE_VALUES          "Save Values to File"       /**< Etiqueta.*/
#define SR_LOAD_VALUES          "Load Values from File"     /**< Etiqueta.*/

#define SR_GAIN_FOURIER_PREVIEW "Preview Gain: "            /**< Etiqueta.*/

#define SR_SPECTROGRAM_TITLE    "Reference Phase"           /**< Etiqueta.*/
#define SR_FINISH_BUTTON        "Finish"                    /**< Etiqueta.*/

#define SR_ERROR_LABEL          "Error"                     /**< Etiqueta.*/

#define SR_ERROR_SAVE       "Error Saving"                  /**< Etiqueta.*/
#define SR_ERROR_LOAD       "Error Loading"                 /**< Etiqueta.*/

/*! \class DialogSetReference dialogsetreference.h "dialogsetreference.h"
 * \brief Clase para el control del cuadro de di�logo que permite selecionar y procesar la imagen de referencia.
 *
 * Su principal cometido es finalizar con una referencia configurada.
 *
 * Esta clase act�a de nexo entre el controlador principal y la clase que contiene los widgets: Ui::DialogSetReference.
 *
 */

namespace Ui {
class DialogSetReference;
}

class DialogSetReference : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogSetReference(QWidget *parent, MainController * mainController_in);
    /**< Constructor.
      @param parent Widget padre.
      @param mainController_in Referencia el controlador principal.*/
    ~DialogSetReference();
    /**< Destructor.*/
    
private:
    Ui::DialogSetReference *ui;                 /**< Puntero a la clase que contiene la interfaz gr�fica. Esta clase es generada autom�ticamente por Qt Designer.*/
    MainController * mainController;            /**< Puntero al controlador principal.*/
    MaskSelectionScene * maskScene;             /**< Escena donde se selecciona la m�scara de la imagen.*/
    MaskSelectionScene * maskFourierScene;      /**< Escena donde se selecciona la m�scara de la transformada de fourier.*/
    QGraphicsLineItem * lineOrigin1;            /**< Primera l�nea transversal para dibujar el origen de coordenadas.*/
    QGraphicsLineItem * lineOrigin2;            /**< Segunda l�nea transversal para dibujar el origen de coordenadas.*/
    QGraphicsRectItem * maskRect;               /**< Rect�ngulo para el dibujo de la m�scara de la imagen.*/
    QGraphicsRectItem * maskFourierRect;        /**< Rect�ngulo para el dibujo de la m�scara de la transformada de fourier.*/
    QGraphicsPixmapItem * maskPixmap;           /**< Pixmap para la vista previa de la imagen en la escena.*/
    QGraphicsPixmapItem * maskFourierPixmap;    /**< Pixmap para la vista previa de la transformada de fourier en la escena.*/
    QImage fullImageTransformed;                /**< Imagen almacenada de la imagen original de la referencia tras aplicarle las transformaciones.*/
    int step;                                   /**< Paso de la configuraci�n de la referencia en el que se encuentra del cuadro de di�logo. De 0 a 4.*/

private:
    void InitControls();
    /**< Inicia los controles durante la creaci�n.*/
    void SetControlsStep(int newStep);
    /**< Configura los controles cuando se cambia de un paso a otro en el proceso de fijar la referencia.
        @param newStep Paso al que se est� pasando.*/

public slots:
    void UpdateControls();
    /**< Slot que actualiza los controles en el paso en el que se encuentre el proceso.*/
    void GetImageFromCamera();
    /**< Slot llamado cuando se quiere cargar la imagen de la referencia desde la c�mara.*/
    void RunCameraSettings();
    /**< Slot llamado cuando se quiere modificar los par�meros de la c�mara.*/
    void GetImageFromFile();
    /**< Slot llamado cuando se quiere cargar la imagen de la referencia desde un archivo.*/
    void SavePictureToFile();
    /**< Slot llamado cuando se quiere guardar la imagen de la referencia actual a un archivo.*/
    void SaveMaskToFile();
    /**< Slot llamado cuando se quiere guardar una m�scara un archivo.*/
    void LoadMaskFromFile();
    /**< Slot llamado cuando se quiere cargar una m�scara de un archivo.*/
    void NextStep();
    /**< Slot llamado cuando se quiere avanzar al siguiente paso del proceso.*/
    void PreviousStep();
    /**< Slot llamado cuando se quiere retroceder al paso anterior.*/
    void RealSizeXChanged();
    /**< Slot llamado cuando cambia el valor de la caja de texto que contiene la dimensi�n real en el eje x.*/
    void RealSizeYChanged();
    /**< Slot llamado cuando cambia el valor de la caja de texto que contiene la dimensi�n real en el eje y.*/
    void RadioOrCheckChanged();
    /**< Slot llamado cuando cambia el valor de la caja de texto que contiene la dimensi�n real para el c�lculo en el eje x.*/
    void SizeXChanged();
    /**< Slot llamado cuando cambia el valor de la caja de texto que contiene la dimensi�n en p�xeles para el c�lculo en el eje x.*/
    void SizeYChanged();
    /**< Slot llamado cuando cambia el valor de la caja de texto que contiene la dimensi�n en p�xeles para el c�lculo en el eje y.*/
    void MaskSpinBoxChanged();
    /**< Slot llamado cuando cambia el valor de una "SpinBox".*/
    void MouseRightButtonPressed(int x,int y);
    /**< Slot llamado cuando se ha pulsado con el bot�n derecho del rat�n en un punto de la imagen.
        @param x Coordenada X.
        @param y Coordenada Y.*/
    void MouseLeftButtonPressed(int x,int y);
    /**< Slot llamado cuando se ha pulsado con el bot�n izquierdo del rat�n en un punto de la imagen.
        @param x Coordenada X.
        @param y Coordenada Y.*/
    void MouseRightButtonMoved(int x,int y);
    /**< Slot llamado cuando se ha movido el raton con el bot�n derecho pulsado.
        @param x Coordenada X.
        @param y Coordenada Y.*/
    void MouseReleased(int x,int y);
    /**< Slot llamado cuando se ha soltado un bot�n del rat�n.
        @param x Coordenada X.
        @param y Coordenada Y.*/
    void MouseLeftButtonDoubleClick(int x,int y);
    /**< Slot llamado cuando se ha pulsado doble click con el bot�n izquierdo del rat�n en un punto de la imagen.
        @param x Coordenada X.
        @param y Coordenada Y.*/
    void MouseWheel(int delta,int x,int y);
    /**< Slot llamado cuando se ha movido la rueda del rat�n.
        @param delta Cantidad y sentido del movimiento.
        @param x Coordenada X.
        @param y Coordenada Y.*/
    void PreviewGainChanged();
    /**< Slot llamado cuando se ha cambiado el "SpinBox" de la ganancia de la vista previa de la transformada de fourier.*/
    void CheckCamera();
    /**< Slot que se llama para conocer el estado de la c�mara y activar o desactivar controles.*/

};

#endif // DIALOGSETREFERENCE_H
