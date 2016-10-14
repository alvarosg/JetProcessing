/**
 * @file   dialogsetabel.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase para el cuadro de di�logo que permite obtener los par�metros para realizar la Inversi�n de Abel.
 *
 */
#ifndef DIALOGSETABEL_H
#define DIALOGSETABEL_H

#include <QDialog>

#define SA_CAPTION              "Configure Abel Inversion"                                                              /**< Etiqueta.*/
#define SA_NEIGHBOURS           "Select the number of neighbours to calculate the derivative for the Abel Inversion"    /**< Etiqueta.*/

/*! \class DialogSetAbel dialogsetabel.h "dialogsetabel.h"
 * \brief Clase para el control del cuadro de di�logo que permite configurar el algoritmo de la Inversi�n de Abel.
 *
 * Su cometido es recuperar el n�mero de vecinos a los que se debe de calcular la derivada.
 *
 * Esta clase act�a de nexo entre la interfaz principal y la clase que contiene los widgets: Ui::DialogSetAbel
 *
 */


namespace Ui {
class DialogSetAbel;
}

class DialogSetAbel : public QDialog
{
    Q_OBJECT
private:
    int * neighbours;                   /**< Puntero al n�mero de vecinos*/

public:
    explicit DialogSetAbel(QWidget *parent,int * neighbours_in);
    /**< Constructor.
      @param parent Widget padre.
      @param neighbours_in Referencia al n�mero de vecinos.*/
    ~DialogSetAbel();
    /**< Destructor.*/
    
private:
    Ui::DialogSetAbel *ui;              /**< Puntero a la clase que contiene la interfaz gr�fica. Esta clase es generada autom�ticamente por Qt Designer.*/
};

#endif // DIALOGSETABEL_H
