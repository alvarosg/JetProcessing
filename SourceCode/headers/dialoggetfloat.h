/**
 * @file   dialoggetfloat.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase para el cuadro de diálogo que permite obtener un número decimal.
 *
 */
#ifndef DIALOGGETFLOAT_H
#define DIALOGGETFLOAT_H

#define GF_CAPTION_OFFSET              "Set Offset"         /**< Etiqueta.*/
#define GF_CAPTION_MULTIPLY            "Set Factor"         /**< Etiqueta.*/
#define GF_OFFSET                      "Offset Value: "     /**< Etiqueta.*/
#define GF_MULTIPLY                    "Factor Value: "     /**< Etiqueta.*/

/*! \class DialogGetFloat dialoggetfloat.h "dialoggetfloat.h"
 * \brief Clase para el control del cuadro de diálogo que permite obtener un número decimal para ciertos algoritmos.
 *
 * Su único cometido es conseguir un número decimal.
 *
 * Esta clase actúa de nexo entre la interfaz principal y la clase que contiene los widgets: Ui::DialogGetFloat.
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
      @param number_in Referencia al número que se mostrará inicialmente, y en el que se guardará el valor final.
      @param factor Si es verdadero se mostrará el texto suponiendo que el décimal es para un factor de proporción, si es falso para un offset.*/
    ~DialogGetFloat();
    /**< Destructor.*/
    
private:
    Ui::DialogGetFloat *ui;   /**< Puntero a la clase que contiene la interfaz gráfica. Esta clase es generada automáticamente por Qt Designer.*/
    float * number;           /**< Puntero al número decimal.*/
};

#endif // DIALOGGETFLOAT_H
