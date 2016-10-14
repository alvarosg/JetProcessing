/**
 * @file   dialogpreferences.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase para el cuadro de di�logo que permite cambiar las preferencias del programa.
 *
 */
#ifndef DIALOGPREFERENCES_H
#define DIALOGPREFERENCES_H

#include <QDialog>
#include <programsettings.h>

#define PF_CAPTION             "Preferences"                                                            /**< Etiqueta.*/

#define PF_SAVECAMPICTURES    "Save camera pictures to folder \"CameraPics\" in the output directory"   /**< Etiqueta.*/
#define PF_UNTILSELECTEDALG   "Apply algorithms only until selected algorithm"                          /**< Etiqueta.*/
#define PF_MODEDIALOG         "Show mode selection dialog when the program starts"                      /**< Etiqueta.*/
#define PF_VIEWSIZE           "Save images using the size of the view"                                  /**< Etiqueta.*/
#define PF_WIDTH              "Width: "                                                                 /**< Etiqueta.*/
#define PF_HEIGHT             "Height: "                                                                /**< Etiqueta.*/
#define PF_FORMAT             "Default format for the output graphics image files"                      /**< Etiqueta.*/
#define PF_FORMATPNG          "Portable Network Graphics Image (.png)"                                  /**< Etiqueta.*/
#define PF_FORMATJPG          "JPEG Image (.jpg)"                                                       /**< Etiqueta.*/
#define PF_FORMATBMP          "Bitmap Image (.bmp)"                                                     /**< Etiqueta.*/


/*! \class DialogPreferences dialogpreferences.h "dialogpreferences.h"
 * \brief Clase para el control del cuadro de di�logo que permite modificar las preferencias del programa.
 *
 * Su �nico cometido es modificar un objeto del tipo ProgramSettings.
 *
 * Esta clase act�a de nexo entre la interfaz principal y la clase que contiene los widgets: Ui::DialogPreferences
 *
 */


namespace Ui {
class DialogPreferences;
}

class DialogPreferences : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogPreferences(QWidget *parent,ProgramSettings * programSettings_in);
    /**< Constructor.
      @param parent Widget padre.
      @param programSettings_in Referencia a las preferencias del programa.*/
    ~DialogPreferences();
    /**< Destructor.*/
    
private:
    Ui::DialogPreferences *ui;              /**< Puntero a la clase que contiene la interfaz gr�fica. Esta clase es generada autom�ticamente por Qt Designer.*/
    ProgramSettings * programSettings;      /**< Puntero a las preferencias dle programa.*/

public slots:
    void Finished();
    /**< Slot al que se llama cuando se sale del cuadro de di�logo.*/
    void UpdateSizesEnabled();
    /**< Slot al que se llama cuando se activa o desactiva el "CheckBox" de tama�o autom�tico para los gr�ficos guardados.*/
};

#endif // DIALOGPREFERENCES_H
