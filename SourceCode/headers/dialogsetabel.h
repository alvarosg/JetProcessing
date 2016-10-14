/**
 * @file   dialogsetabel.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase para el cuadro de diálogo que permite obtener los parámetros para realizar la Inversión de Abel.
 *
 */
#ifndef DIALOGSETABEL_H
#define DIALOGSETABEL_H

#include <QDialog>

#define SA_CAPTION              "Configure Abel Inversion"                                                              /**< Etiqueta.*/
#define SA_NEIGHBOURS           "Select the number of neighbours to calculate the derivative for the Abel Inversion"    /**< Etiqueta.*/

/*! \class DialogSetAbel dialogsetabel.h "dialogsetabel.h"
 * \brief Clase para el control del cuadro de diálogo que permite configurar el algoritmo de la Inversión de Abel.
 *
 * Su cometido es recuperar el número de vecinos a los que se debe de calcular la derivada.
 *
 * Esta clase actúa de nexo entre la interfaz principal y la clase que contiene los widgets: Ui::DialogSetAbel
 *
 */


namespace Ui {
class DialogSetAbel;
}

class DialogSetAbel : public QDialog
{
    Q_OBJECT
private:
    int * neighbours;                   /**< Puntero al número de vecinos*/

public:
    explicit DialogSetAbel(QWidget *parent,int * neighbours_in);
    /**< Constructor.
      @param parent Widget padre.
      @param neighbours_in Referencia al número de vecinos.*/
    ~DialogSetAbel();
    /**< Destructor.*/
    
private:
    Ui::DialogSetAbel *ui;              /**< Puntero a la clase que contiene la interfaz gráfica. Esta clase es generada automáticamente por Qt Designer.*/
};

#endif // DIALOGSETABEL_H
