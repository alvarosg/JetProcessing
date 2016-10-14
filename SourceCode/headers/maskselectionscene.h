/**
 * @file   maskselectionscene.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase que reimplementa "QGraphicsScene" para añadirle gestión de eventos a un cuadro de dibujo.
 *
 */
#ifndef MASKSELECTIONSCENE_H
#define MASKSELECTIONSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsSceneWheelEvent>

/*! \class MaskSelectionScene maskselectionscene.h "maskselectionscene.h"
 * \brief Escena sobre la que se dibujan las máscaras y las líneas.
 *
 *
 * Esta clase es derivada de QGraphicsScene y reimplementa eventos de click del ratón y de teclado para enviar señales y personalizar las acciones.
 */


class MaskSelectionScene : public QGraphicsScene{

Q_OBJECT

private:
    bool ctrlPressed;       /**< Flag que indica si la tecla "Control" está pulsada.*/

public:
    MaskSelectionScene(QObject * parent);
    /**< Constructor.
      @param parent Widget padre.*/
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    /**< Evento de pulsación de raton.
        @param event Objeto con los datos del evento.*/
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
    /**< Evento de pulsación "doble click" del raton.
        @param event Objeto con los datos del evento.*/
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    /**< Evento de movimiento del raton.
        @param event Objeto con los datos del evento.*/
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    /**< Evento generado al soltar los botones del ratón.
        @param event Objeto con los datos del evento.*/
    void wheelEvent (QGraphicsSceneWheelEvent * event );
    /**< Evento de movimiento de la rueda del ratón.
        @param event Objeto con los datos del evento.*/
    void keyPressEvent(QKeyEvent  * event);
    /**< Evento de pulsación de una tecla.
        @param event Objeto con los datos del evento.*/
    void keyReleaseEvent(QKeyEvent  * event);
    /**< Evento de soltado de una tecla.
        @param event Objeto con los datos del evento.*/


signals:
    void mouseRightButtonPressed(int x, int y);
    /**< Señal enviada cuando se hace click con el ratón con el botón derecho.
        @param x Posición del ratón en el eje x.
        @param y Posición del ratón en el eje y.*/
    void mouseRightButtonMoved(int x, int y);
    /**< Señal enviada cuando se mueve el ratón con el botón derecho pulsado.
        @param x Posición del ratón en el eje x.
        @param y Posición del ratón en el eje y.*/
    void mouseLeftButtonDoubleClick(int x, int y);
    /**< Señal enviada cuando se hace "doble-click" con el ratón con el botón derecho.
        @param x Posición del ratón en el eje x.
        @param y Posición del ratón en el eje y.*/
    void mouseLeftButtonPressed(int x, int y);
    /**< Señal enviada cuando se hace click con el ratón con el botón izquierdo.
        @param x Posición del ratón en el eje x.
        @param y Posición del ratón en el eje y.*/
    void mouseLeftButtonMoved(int x, int y);
    /**< Señal enviada cuando se mueve el ratón con el botón izquierdo pulsado.
        @param x Posición del ratón en el eje x.
        @param y Posición del ratón en el eje y.*/
    void mouseMoved(int x, int y);
    /**< Señal enviada cuando se mueve el ratón.
        @param x Posición del ratón en el eje x.
        @param y Posición del ratón en el eje y.*/
    void mouseReleased(int x, int y);
    /**< Señal enviada cuando se sueltan los botones del ratón.
        @param x Posición del ratón en el eje x.
        @param y Posición del ratón en el eje y.*/
    void mouseCtrlReleased(int x, int y);
    /**< Señal enviada cuando se sueltan los botones del ratón con la tecla "control" pulsada.
        @param x Posición del ratón en el eje x.
        @param y Posición del ratón en el eje y.*/
    void mouseWheel(int delta,int x,int y);
    /**< Señal enviada cuando se mueve la rueda del ratón con el botón izquierdo pulsado.
        @param delta Cantidad y dirección del movimiento
        @param x Posición del ratón en el eje x.
        @param y Posición del ratón en el eje y.*/

};

#endif // MASKSELECTIONSCENE_H

