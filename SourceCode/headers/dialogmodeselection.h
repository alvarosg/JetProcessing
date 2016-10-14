/**
 * @file   dialogmodeselection.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase para el cuadro de di�logo que permite elegir entre los modos de funcionamiento al comenzar el programa.
 *
 */
#ifndef DIALOGMODESELECTION_H
#define DIALOGMODESELECTION_H


#define MS_CAPTION              "Mode Selection"                                    /**< Etiqueta.*/

#define MS_SUBTITLE_LABEL       "Select the desired mode:"                          /**< Etiqueta.*/
#define MS_ITEM1_LABEL          "CUDA Computation (NVIDIA GPU Parallel Computing)"  /**< Etiqueta.*/
#define MS_ITEM2_LABEL          "CPU Computation"                                   /**< Etiqueta.*/
#define MS_DEFAULT_LABEL        "Remember this selection"                           /**< Etiqueta.*/
#define MS_CONTINUE_LABEL       "Continue"                                          /**< Etiqueta.*/


#include <QDialog>
#include "maincontroller.h"

/*! \class DialogModeSelection dialogmodeselection.h "dialogmodeselection.h"
 * \brief Clase para el control del cuadro de di�logo que permite elegir entre los dos modos de c�lculo.
 *
 *
 * Esta clase act�a de nexo entre el controlador principal y la clase que contiene los widgets: Ui::DialogModeSelection.
 *
 */

namespace Ui {
class DialogModeSelection;
}

class DialogModeSelection : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogModeSelection(QWidget *parent, MainController * mainController_in);
    /**< Constructor.
      @param parent Widget padre.
      @param mainController_in Puntero al controlador principal.*/
    ~DialogModeSelection();
    /**< Destructor.*/
    
private:
    Ui::DialogModeSelection *ui;            /**< Puntero a la clase que contiene la interfaz gr�fica. Esta clase es generada autom�ticamente por Qt Designer.*/
    MainController * mainController;        /**< Puntero al controlador principal.*/

public slots:
    void Finished();
    /**< Slot al que se llama cuando se sale del cuadro de di�logo.*/
};

#endif // DIALOGMODESELECTION_H
