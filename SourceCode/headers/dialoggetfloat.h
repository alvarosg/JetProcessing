/**
 * @file   dialoggetfloat.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase para el cuadro de di�logo que permite obtener un n�mero decimal.
 *
 */
#ifndef DIALOGGETFLOAT_H
#define DIALOGGETFLOAT_H

#define GF_CAPTION_OFFSET              "Set Offset"         /**< Etiqueta.*/
#define GF_CAPTION_MULTIPLY            "Set Factor"         /**< Etiqueta.*/
#define GF_OFFSET                      "Offset Value: "     /**< Etiqueta.*/
#define GF_MULTIPLY                    "Factor Value: "     /**< Etiqueta.*/

/*! \class DialogGetFloat dialoggetfloat.h "dialoggetfloat.h"
 * \brief Clase para el control del cuadro de di�logo que permite obtener un n�mero decimal para ciertos algoritmos.
 *
 * Su �nico cometido es conseguir un n�mero decimal.
 *
 * Esta clase act�a de nexo entre la interfaz principal y la clase que contiene los widgets: Ui::DialogGetFloat.
 *
 */

#include <QDialog>

namespace Ui {
class DialogGetFloat;
}

class DialogGetFloat : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogGetFloat(QWidget *parent, float * number_in, bool factor=false);
    /**< Constructor.
      @param parent Widget padre.
      @param number_in Referencia al n�mero que se mostrar� inicialmente, y en el que se guardar� el valor final.
      @param factor Si es verdadero se mostrar� el texto suponiendo que el d�cimal es para un factor de proporci�n, si es falso para un offset.*/
    ~DialogGetFloat();
    /**< Destructor.*/
    
private:
    Ui::DialogGetFloat *ui;   /**< Puntero a la clase que contiene la interfaz gr�fica. Esta clase es generada autom�ticamente por Qt Designer.*/
    float * number;           /**< Puntero al n�mero decimal.*/
};

#endif // DIALOGGETFLOAT_H
