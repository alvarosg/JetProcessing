/**
 * @file   spectrogramplot.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase para la representación de datos bidimensionales en un espectrograma.
 *
 */
#ifndef SPECTROGRAMPLOT_H
#define SPECTROGRAMPLOT_H

#include "plotdata2d.h"
#include <qwt_plot.h>
#include <qwt_plot_spectrogram.h>
#include <qprinter.h>
#include <qprintdialog.h>
#include <qwt_color_map.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_renderer.h>


/*! \class SpectrogramPlot spectrogramplot.h "spectrogramplot.h"
 * \brief Clase que implementa PlotData2D para representar un spectrograma.
 *
 *  Utiliza la biblioteca QwtPlot.
 *
 */

class SpectrogramPlot : public QwtPlot, public PlotData2D{
    Q_OBJECT

public:

    SpectrogramPlot(QWidget * parent);
    /**< Constructor.
      @param parent Widget padre.*/
    ~SpectrogramPlot();
    /**< Destructor. */
    void ConfigurePlot(Data2D * data);
    void SetData2D(Data2D * data2d);
    QPixmap * GetAsQPixmap();


public Q_SLOTS:
    void ShowContour(bool show);
    /**< Slot para activar o desactivar las curvas de nivel.
       @param show Estado de activación deseado.*/
    void ShowSpectrogram(bool show);
    /**< Slot para activar o desactivar los colores del espectrograma.
       @param show Estado de activación deseado.*/

private:
    QwtPlotSpectrogram * spectrogram;      /**< Puntero al objeto necesario para dibujar el espectrograma.*/
    QwtScaleWidget * rightAxis;            /**< Puntero al objeto necesario para mostrar la escala de color a la derecha.*/
    QwtPlotZoomer * zoomer;                /**< Puntero al objeto necesario para realizar zoom.*/
    QwtPlotPanner *panner;                 /**< Puntero al objeto necesario para mover la imagen arrastrando.*/

private:
    void Update();
    /**< Redibuja con todos los posibles cambios de parámetros que haya habido en el gráfico.*/

};

#endif // SPECTROGRAMPLOT_H
