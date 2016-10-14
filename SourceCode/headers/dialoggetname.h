/**
 * @file   dialoggetname.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase para el cuadro de diálogo que permite obtener una etiqueta.
 *
 */
#ifndef DIALOGGETNAME_H
#define DIALOGGETNAME_H

#include <QDialog>

#define GN_CAPTION              "Series\' Name"                                             /**< Etiqueta.*/
#define GN_LABEL                "Write a name for the series (A folder will be created)"    /**< Etiqueta.*/

/*! \class DialogGetName dialoggetname.h "dialoggetname.h"
 * \brief Clase para el control del cuadro de diálogo que permite obtener una cadena de texto.
 *
 * Su único cometido es conseguir una cadena de texto.
 *
 * Esta clase actúa de nexo entre la interfaz principal y la clase que contiene los widgets: Ui::DialogGetName
 *
 */


namespace Ui {
class DialogGetName;
}

class DialogGetName : public QDialog
{
    Q_OBJECT
private:
    QString * name;             /**< Puntero a la cadena de texto.*/
    
public:
    explicit DialogGetName(QWidget *parent,QString * name_in);
    /**< Constructor.
      @param parent Widget padre.
      @param name_in Referencia a la cadena de texto.*/

    ~DialogGetName();
    /**< Destructor.*/
    
private:
    Ui::DialogGetName *ui;      /**< Puntero a la clase que contiene la interfaz gráfica. Esta clase es generada automáticamente por Qt Designer.*/
};

#endif // DIALOGGETNAME_H
