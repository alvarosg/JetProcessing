/**
 * @file   dialogsetrectangle.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase para el cuadro de diálogo que seleccionar un rectángulo o una línea sobre la imagen de la fase.
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
 * \brief Clase para el control del cuadro de diálogo que permite seleccionar un rectángulo o una recta, para los correspondientes algoritmos.
 *
 * Su principal cometido es devolver un rectángulo.
 *
 * Esta clase actúa de nexo entre el controlador principal y la clase que contiene los widgets: Ui::DialogSetRectangle.
 *
 */

namespace Ui {
class DialogSetRectangle;
}

class DialogSetRectangle : public QDialog
{
    Q_OBJECT

private:
    Ui::DialogSetRectangle *ui;                 /**< Puntero a la clase que contiene la interfaz gráfica. Esta clase es generada automáticamente por Qt Designer.*/
    MainController * mainController;            /**< Puntero al controlador principal.*/
    QRect * rect;                               /**< Puntero al rectángulo.*/
    MaskSelectionScene * rectScene;             /**< Escena donde se selecciona el rectángulo o la recta.*/
    QGraphicsPixmapItem * rectPixmap;           /**< Pixmap para dibujar el fondo en la escena.*/
    QGraphicsRectItem * rectRect;               /**< Rectángulo para dibujar en la escena en caso de selección de un rectangulo.*/
    QGraphicsLineItem * rectLine;               /**< Recta para dibujar en la escena en caso de selección de un recta.*/
    QGraphicsPolygonItem * firstPointLine;      /**< Extremo inicial de la recta en caso de selección de una recta.*/
    QGraphicsPolygonItem * secondPointLine;     /**< Extremo final de la recta en caso de selección de una recta.*/
    QGraphicsPolygonItem * centerPointLine;     /**< Punto central de la recta en caso de selección de una recta.*/

    float dX;                                   /**< Distancia en mm entre un pixel y el siguiente en el eje X.*/
    float dY;                                   /**< Distancia en mm entre un pixel y el siguiente en el eje Y.*/
    float minXValue;                            /**< Valor mínimo en mm del rectángulo para el eje X de la imagen actual.*/
    float maxYValue;                            /**< Valor máximo en mm del rectángulo para el eje Y de la imagen actual.*/
    Data2D * data2d;                            /**< Datos 2D que se estan representando, sobre los que se está aplicando la recta o el rectángulo.*/

    int pointBeingMoved;                        /**< Punto que se está arrastrando con el ratón. 0 para ninguno, 1 para extremo inicial, 2 para extremo final y 3 para punto central.*/

    QPolygonF GetPolygonCentered(float x, float y);
    /**< Función que devuelve un polígono centrado en un punto determinado.
        @param x Posición horizontal del centro.
        @param y Posición vertical del centro.
        @return Polígono centrado.*/
    int GetPointBeingMoved(float x, float y);
    /**< Función que indica sobre qué parte de la recta se está pinchado si se ha pinchado en ciertas coordenadas.
        @param x Posición horizontal del click.
        @param y Posición vertical del click.
        @return Punto d el arecta pulsado. 0 para ninguno, 1 para extremo inicial, 2 para extremo final y 3 para punto central.*/
    
public:
    explicit DialogSetRectangle(QWidget *parent,MainController * mainController_in,QRect * rect_in,int nAlg=-1,int nImg=0,bool line=false);
    /**< Constructor.
      @param parent Widget padre.
      @param mainController_in Referencia el controlador principal.
      @param rect_in Referencia al rectángulo.
      @param nAlg Número de algoritmo hasta el que se está aplicando en el momento de tomar el rectángulo (-1 para todos).
      @param nImg Número de imagen selccionada en la lista, para la vista previa.
      @param line Verdadero para selcción de recta, false para selección de rectángulo.*/
    ~DialogSetRectangle();
    /**< Destructor.*/
    void UpdateControls();
    /**< Función que actualiza los controles en función del rectángulo/recta selecionada.*/

public slots:
    void SpinBoxMinXChanged();
    /**< Slot llamado cuando el valor del "SpinBox" de la posición mínima en X del rectángulo ha variado.*/
    void SpinBoxMaxXChanged();
    /**< Slot llamado cuando el valor del "SpinBox" de la posición máxima en X del rectángulo ha variado.*/
    void SpinBoxMinYChanged();
    /**< Slot llamado cuando el valor del "SpinBox" de la posición mínima en Y del rectángulo ha variado.*/
    void SpinBoxMaxYChanged();
    /**< Slot llamado cuando el valor del "SpinBox" de la posición máxima en Y del rectángulo ha variado.*/
    void MouseLeftButtonPressed(int x,int y);
    /**< Slot llamado cuando se ha pulsado con el botón izquierdo del ratón en un punto de la imagen.
        @param x Coordenada X.
        @param y Coordenada Y.*/
    void MouseLeftButtonMoved(int x,int y);
    /**< Slot llamado cuando se ha movido el raton con el botón izquierdo pulsado.
        @param x Coordenada X.
        @param y Coordenada Y.*/
    void MouseReleased(int x,int y);
    /**< Slot llamado cuando se ha soltado un botón del ratón.
        @param x Coordenada X.
        @param y Coordenada Y.*/
    void MouseMoved(int x,int y);
    /**< Slot llamado cuando se ha movido el ratón.
        @param x Coordenada X.
        @param y Coordenada Y.*/
    void MouseWheel(int delta, int x, int y);
    /**< Slot llamado cuando se ha movido la rueda del ratón.
        @param delta Cantidad y sentido del movimiento.
        @param x Coordenada X.
        @param y Coordenada Y.*/
};

#endif // DIALOGSETRECTANGLE_H
