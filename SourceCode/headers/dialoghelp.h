/**
 * @file   dialoghelp.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase para el cuadro de di�logo de la ayuda.
 *
 */
#ifndef DIALOGHELP_H
#define DIALOGHELP_H

#define HLP_CAPTION "Jet Processing Help"           /**< Etiqueta.*/

#include <QDialog>
#include <QWebHistory>


/*! \class DialogHelp dialoghelp.h "dialoghelp.h"
 * \brief Clase para el control del cuadro de di�logo que muestra la ayuda.
 *
 * Muestra una web con la ayuda.
 *
 * Esta clase act�a de nexo entre la interfaz principal y la clase que contiene los widgets: Ui::DialogGetFloat
 *
 */


namespace Ui {
class DialogHelp;
}

class DialogHelp : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogHelp(QWidget *parent, QString path, QString homePage);
    /**< Constructor.
      @param parent Widget padre.
      @param path Directorio donde se encuentra la web.
      @param homePage Archivo de entrada a la web. */
    ~DialogHelp();
    /**< Destructor.*/
public slots:
    void GoHome();
    /**< Vuelve a mostrar la p�gina inicial.*/
    void SetButtonsState();
    /**< Establece el estado de los botones en funci�n de se si puede ir adelante o atr�s.*/

    
private:
    Ui::DialogHelp *ui;         /**< Puntero a la clase que contiene la interfaz gr�fica. Esta clase es generada autom�ticamente por Qt Designer.*/
    QString homePagePath;           /**< P�gina inicial de la ayuda.*/
};

#endif // DIALOGHELP_H
