/**
 * @file   maskselectionscene.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase que reimplementa "QGraphicsScene" para a�adirle gesti�n de eventos a un cuadro de dibujo.
 *
 */
#ifndef MASKSELECTIONSCENE_H
#define MASKSELECTIONSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsSceneWheelEvent>

/*! \class MaskSelectionScene maskselectionscene.h "maskselectionscene.h"
 * \brief Escena sobre la que se dibujan las m�scaras y las l�neas.
 *
 *
 * Esta clase es derivada de QGraphicsScene y reimplementa eventos de click del rat�n y de teclado para enviar se�ales y personalizar las acciones.
 */


class MaskSelectionScene : public QGraphicsScene{

Q_OBJECT

private:
    bool ctrlPressed;       /**< Flag que indica si la tecla "Control" est� pulsada.*/

public:
    MaskSelectionScene(QObject * parent);
    /**< Constructor.
      @param parent Widget padre.*/
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    /**< Evento de pulsaci�n de raton.
        @param event Objeto con los datos del evento.*/
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
    /**< Evento de pulsaci�n "doble click" del raton.
        @param event Objeto con los datos del evento.*/
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    /**< Evento de movimiento del raton.
        @param event Objeto con los datos del evento.*/
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    /**< Evento generado al soltar los botones del rat�n.
        @param event Objeto con los datos del evento.*/
    void wheelEvent (QGraphicsSceneWheelEvent * event );
    /**< Evento de movimiento de la rueda del rat�n.
        @param event Objeto con los datos del evento.*/
    void keyPressEvent(QKeyEvent  * event);
    /**< Evento de pulsaci�n de una tecla.
        @param event Objeto con los datos del evento.*/
    void keyReleaseEvent(QKeyEvent  * event);
    /**< Evento de soltado de una tecla.
        @param event Objeto con los datos del evento.*/


signals:
    void mouseRightButtonPressed(int x, int y);
    /**< Se�al enviada cuando se hace click con el rat�n con el bot�n derecho.
        @param x Posici�n del rat�n en el eje x.
        @param y Posici�n del rat�n en el eje y.*/
    void mouseRightButtonMoved(int x, int y);
    /**< Se�al enviada cuando se mueve el rat�n con el bot�n derecho pulsado.
        @param x Posici�n del rat�n en el eje x.
        @param y Posici�n del rat�n en el eje y.*/
    void mouseLeftButtonDoubleClick(int x, int y);
    /**< Se�al enviada cuando se hace "doble-click" con el rat�n con el bot�n derecho.
        @param x Posici�n del rat�n en el eje x.
        @param y Posici�n del rat�n en el eje y.*/
    void mouseLeftButtonPressed(int x, int y);
    /**< Se�al enviada cuando se hace click con el rat�n con el bot�n izquierdo.
        @param x Posici�n del rat�n en el eje x.
        @param y Posici�n del rat�n en el eje y.*/
    void mouseLeftButtonMoved(int x, int y);
    /**< Se�al enviada cuando se mueve el rat�n con el bot�n izquierdo pulsado.
        @param x Posici�n del rat�n en el eje x.
        @param y Posici�n del rat�n en el eje y.*/
    void mouseMoved(int x, int y);
    /**< Se�al enviada cuando se mueve el rat�n.
        @param x Posici�n del rat�n en el eje x.
        @param y Posici�n del rat�n en el eje y.*/
    void mouseReleased(int x, int y);
    /**< Se�al enviada cuando se sueltan los botones del rat�n.
        @param x Posici�n del rat�n en el eje x.
        @param y Posici�n del rat�n en el eje y.*/
    void mouseCtrlReleased(int x, int y);
    /**< Se�al enviada cuando se sueltan los botones del rat�n con la tecla "control" pulsada.
        @param x Posici�n del rat�n en el eje x.
        @param y Posici�n del rat�n en el eje y.*/
    void mouseWheel(int delta,int x,int y);
    /**< Se�al enviada cuando se mueve la rueda del rat�n con el bot�n izquierdo pulsado.
        @param delta Cantidad y direcci�n del movimiento
        @param x Posici�n del rat�n en el eje x.
        @param y Posici�n del rat�n en el eje y.*/

};

#endif // MASKSELECTIONSCENE_H

