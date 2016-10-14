/**
 * @file   graph1d.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase para la representación de datos unidimensionales con la biblioteca QwtPlot.
 *
 */
#ifndef GRAPH1D_H
#define GRAPH1D_H

#include "plotdata1d.h"

#include <qwt_plot.h>
#include <qwt_math.h>
#include <qwt_scale_engine.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_panner.h>
#include <qwt_picker_machine.h>
#include <qwt_legend.h>
#include <qwt_text.h>
#include <qmath.h>
#include <QStaticText>
#include <QPainter>

/*! \class Zoomer graph1d.h "graph1d.h"
 * \brief Clase necesaria para realizar zoom sobre el objeto Graph1D.
 *
 */

class QwtPlotCurve;

class Zoomer: public QwtPlotZoomer{
public:
    Zoomer(int xAxis, int yAxis, QwtPlotCanvas *canvas);
    /**< Constructor.
      @param xAxis Parámetro necesario para el constructor de QwtPlotZoomer.
      @param yAxis Parámetro necesario para el constructor de QwtPlotZoomer.
      @param canvas Parámetro necesario para el constructor de QwtPlotZoomer.*/
};


/*! \class Graph1D graph1d.h "graph1d.h"
 * \brief Clase que implementa PlotData1D usando la biblioteca QwtPlot.
 *
 *  Representar un clásico gráfico variable indpendiente/variable dependiente.
 *
 *  Utiliza la biblioteca QwtPlot.
 *
 */

class Graph1D : public QwtPlot, public PlotData1D
{
    Q_OBJECT

public:
    Graph1D(QWidget *parent);
    /**< Constructor.
      @param parent Widget padre.*/
    ~Graph1D();
    /**< Destructor.*/

    void ConfigurePlot(Data1D * data1d);
    void SetData1D(Data1D * data1d);
    QPixmap * GetAsQPixmap();

private:
    QwtPlotCurve * curve;   /**< Puntero a la curva que se representa.*/
    double * zValues;       /**< Puntero a un buffer con los valores de la variable dependiente.*/
    double * xValues;       /**< Puntero a un buffer con los valores de la variable independiente.*/
    int sizeX;              /**< Número de elementos en los buffer.*/

    QwtPlotZoomer *zoomer[2];   /**< Puntero a los objetos para hacer zoom.*/
    QwtPlotPicker *picker;      /**< Puntero al objeto que permite obtener datos acerca de un punto en el gráfico.*/
    QwtPlotPanner *panner;      /**< Puntero al objeto que permite mover y arrastrar el grafico.*/

private:
    void Update();
    /**< Redibuja con todos los posibles cambios de parámetros que haya habido en el gráfico.*/
    void LoadData(Data1D * data1d);
    /**< Función auxiliar que carga los datos en el gráfico.
         @param data1d Datos unidimensionales que se quieren cargar.*/
};

#endif // GRAPH1D_H
