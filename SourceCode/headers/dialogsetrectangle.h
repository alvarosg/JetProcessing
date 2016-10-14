/**
 * @file   dialogsetrectangle.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase para el cuadro de di�logo que seleccionar un rect�ngulo o una l�nea sobre la imagen de la fase.
 *
 */
#ifndef DIALOGSETRECTANGLE_H
#define DIALOGSETRECTANGLE_H

#include <QDialog>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include "maskselectionscene.h"
#include "maincontroller.h"
#include "plotdata1d.h"

#define CR_CAPTION_RECT              "Extract Rectangle"    /**< Etiqueta.*/
#define CR_CAPTION_LINE              "Extract Line"         /**< Etiqueta.*/

#define CR_RECT                 "Rectangle Coordinates:"    /**< Etiqueta.*/
#define CR_RECT_XMIN     "X min: "                          /**< Etiqueta.*/
#define CR_RECT_XMAX     "X max: "                          /**< Etiqueta.*/
#define CR_RECT_YMIN     "Y min: "                          /**< Etiqueta.*/
#define CR_RECT_YMAX     "Y max: "                          /**< Etiqueta.*/

#define CR_UNITS         " mm"                              /**< Etiqueta.*/

#define CR_HELPMASK       "(Click-&-Drag with the left mouse button on the image)"    /**< Etiqueta.*/

/*! \class DialogSetRectangle dialogsetrectangle.h "dialogsetrectangle.h"
 * \brief Clase para el control del cuadro de di�logo que permite seleccionar un rect�ngulo o una recta, para los correspondientes algoritmos.
 *
 * Su principal cometido es devolver un rect�ngulo.
 *
 * Esta clase act�a de nexo entre el controlador principal y la clase que contiene los widgets: Ui::DialogSetRectangle.
 *
 */

namespace Ui {
class DialogSetRectangle;
}

class DialogSetRectangle : public QDialog
{
    Q_OBJECT

private:
    Ui::DialogSetRectangle *ui;                 /**< Puntero a la clase que contiene la interfaz gr�fica. Esta clase es generada autom�ticamente por Qt Designer.*/
    MainController * mainController;            /**< Puntero al controlador principal.*/
    QRect * rect;                               /**< Puntero al rect�ngulo.*/
    MaskSelectionScene * rectScene;             /**< Escena donde se selecciona el rect�ngulo o la recta.*/
    QGraphicsPixmapItem * rectPixmap;           /**< Pixmap para dibujar el fondo en la escena.*/
    QGraphicsRectItem * rectRect;               /**< Rect�ngulo para dibujar en la escena en caso de selecci�n de un rectangulo.*/
    QGraphicsLineItem * rectLine;               /**< Recta para dibujar en la escena en caso de selecci�n de un recta.*/
    QGraphicsPolygonItem * firstPointLine;      /**< Extremo inicial de la recta en caso de selecci�n de una recta.*/
    QGraphicsPolygonItem * secondPointLine;     /**< Extremo final de la recta en caso de selecci�n de una recta.*/
    QGraphicsPolygonItem * centerPointLine;     /**< Punto central de la recta en caso de selecci�n de una recta.*/

    float dX;                                   /**< Distancia en mm entre un pixel y el siguiente en el eje X.*/
    float dY;                                   /**< Distancia en mm entre un pixel y el siguiente en el eje Y.*/
    float minXValue;                            /**< Valor m�nimo en mm del rect�ngulo para el eje X de la imagen actual.*/
    float maxYValue;                            /**< Valor m�ximo en mm del rect�ngulo para el eje Y de la imagen actual.*/
    Data2D * data2d;                            /**< Datos 2D que se estan representando, sobre los que se est� aplicando la recta o el rect�ngulo.*/

    int pointBeingMoved;                        /**< Punto que se est� arrastrando con el rat�n. 0 para ninguno, 1 para extremo inicial, 2 para extremo final y 3 para punto central.*/

    QPolygonF GetPolygonCentered(float x, float y);
    /**< Funci�n que devuelve un pol�gono centrado en un punto determinado.
        @param x Posici�n horizontal del centro.
        @param y Posici�n vertical del centro.
        @return Pol�gono centrado.*/
    int GetPointBeingMoved(float x, float y);
    /**< Funci�n que indica sobre qu� parte de la recta se est� pinchado si se ha pinchado en ciertas coordenadas.
        @param x Posici�n horizontal del click.
        @param y Posici�n vertical del click.
        @return Punto d el arecta pulsado. 0 para ninguno, 1 para extremo inicial, 2 para extremo final y 3 para punto central.*/
    
public:
    explicit DialogSetRectangle(QWidget *parent,MainController * mainController_in,QRect * rect_in,int nAlg=-1,int nImg=0,bool line=false);
    /**< Constructor.
      @param parent Widget padre.
      @param mainController_in Referencia el controlador principal.
      @param rect_in Referencia al rect�ngulo.
      @param nAlg N�mero de algoritmo hasta el que se est� aplicando en el momento de tomar el rect�ngulo (-1 para todos).
      @param nImg N�mero de imagen selccionada en la lista, para la vista previa.
      @param line Verdadero para selcci�n de recta, false para selecci�n de rect�ngulo.*/
    ~DialogSetRectangle();
    /**< Destructor.*/
    void UpdateControls();
    /**< Funci�n que actualiza los controles en funci�n del rect�ngulo/recta selecionada.*/

public slots:
    void SpinBoxMinXChanged();
    /**< Slot llamado cuando el valor del "SpinBox" de la posici�n m�nima en X del rect�ngulo ha variado.*/
    void SpinBoxMaxXChanged();
    /**< Slot llamado cuando el valor del "SpinBox" de la posici�n m�xima en X del rect�ngulo ha variado.*/
    void SpinBoxMinYChanged();
    /**< Slot llamado cuando el valor del "SpinBox" de la posici�n m�nima en Y del rect�ngulo ha variado.*/
    void SpinBoxMaxYChanged();
    /**< Slot llamado cuando el valor del "SpinBox" de la posici�n m�xima en Y del rect�ngulo ha variado.*/
    void MouseLeftButtonPressed(int x,int y);
    /**< Slot llamado cuando se ha pulsado con el bot�n izquierdo del rat�n en un punto de la imagen.
        @param x Coordenada X.
        @param y Coordenada Y.*/
    void MouseLeftButtonMoved(int x,int y);
    /**< Slot llamado cuando se ha movido el raton con el bot�n izquierdo pulsado.
        @param x Coordenada X.
        @param y Coordenada Y.*/
    void MouseReleased(int x,int y);
    /**< Slot llamado cuando se ha soltado un bot�n del rat�n.
        @param x Coordenada X.
        @param y Coordenada Y.*/
    void MouseMoved(int x,int y);
    /**< Slot llamado cuando se ha movido el rat�n.
        @param x Coordenada X.
        @param y Coordenada Y.*/
    void MouseWheel(int delta, int x, int y);
    /**< Slot llamado cuando se ha movido la rueda del rat�n.
        @param delta Cantidad y sentido del movimiento.
        @param x Coordenada X.
        @param y Coordenada Y.*/
};

#endif // DIALOGSETRECTANGLE_H
