/**
 * @file   spectrogramplot.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase SpectrogramPlot.
 *
 */
#include "spectrogramplot.h"


/*! \class SpectrogramZoomer spectrogramplot.cpp "spectrogramplot.cpp"
 * \brief Clase necesaria para realizar zoom sobre el objeto SpectrogramPlot.
 *
 */

class SpectrogramZoomer: public QwtPlotZoomer
{
public:

    SpectrogramZoomer(QwtPlotCanvas *canvas):
        QwtPlotZoomer(canvas)
    {
        setTrackerMode(AlwaysOn);
    }
    /**< Constructor.
      @param canvas Parámetro necesario para el constructor de QwtPlotZoomer.*/


    virtual QwtText trackerTextF(const QPointF &pos) const
    {
        QColor bg(Qt::white);
        bg.setAlpha(200);

        QwtText text = QwtPlotZoomer::trackerTextF(pos);
        text.setBackgroundBrush( QBrush( bg ));
        return text;
    }
    /**< Función reimplementada para realizar el tracking del ratón al hacer zoom.
        @param pos Posición del ratón.*/
};

/*! \class SpectrogramData spectrogramplot.cpp "spectrogramplot.cpp"
 * \brief Clase que contiene los datos que se van a representar en el espectrograma.
 *
 * El método reimplementado "value" se encarga de responder a peticiones acerca de qué valor se debe representar en uan posición dada.
 *
 */

class SpectrogramData: public QwtRasterData
{

public:
    Data2D * sourceData;        /**< Datos 2d que se están representando.*/
    float ** buffer;            /**< Puntero a puntero que apunta al array en la zona de memoria donde están los datos.*/
    int sizeX;                  /**< Ancho de los datos.*/
    int sizeY;                  /**< Alto de los datos.*/
    float ratioX;               /**< Número de píxeles por unidad para la variable independiente X.*/
    float ratioY;               /**< Número de píxeles por unidad para la variable independiente Y.*/
    float minX;                 /**< Mínimo valor tomado por la variable independiente X. Correspondiente al índice 0.*/
    float minY;                 /**< Mínimo valor tomado por la variable independiente Y. Correspondiente al índice sizeY-1.*/

public:
    SpectrogramData(const Data2D * data,float minZ, float maxZ){
        sourceData=new Data2D(*data);
        setInterval( Qt::XAxis, QwtInterval( sourceData->GetMinXValue(), sourceData->GetMaxXValue()) );
        setInterval( Qt::YAxis, QwtInterval( sourceData->GetMinYValue(), sourceData->GetMaxYValue()) );
        setInterval( Qt::ZAxis, QwtInterval( minZ, maxZ) );
        buffer=sourceData->GetBuffer();
        sizeX=sourceData->GetSizeX();
        sizeY=sourceData->GetSizeY();
        ratioX=sourceData->GetRatioPixelValueX();
        ratioY=sourceData->GetRatioPixelValueY();
        minX=sourceData->GetMinXValue();
        minY=sourceData->GetMinYValue();
    }
    /**< Constructor.
        @param data Puntero a datos bidimensionales. SpectrogramData hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.
        @param minZ Mínimo valor para la variable dependiente.
        @param maxZ Máximo valor para la variable dependiente.*/

    ~SpectrogramData(){
        delete sourceData;
    }
    /**< Destructor.*/

    virtual double value(double x, double y) const{
        int pixX=(x-minX)/ratioX;
        int pixY=(sizeY-1-(y-minY)/ratioY);
        if (buffer==NULL || pixX>=sizeX || pixY >=sizeY ||
                            pixX<0 || pixY<0){
            return 0;
        }
        else{
            return buffer[pixY][pixX];

        }
    }
    /**< Función reimplementada para responder a qué valor debe asignarse a cierta posición del gráfico.
        @param x Coordenada x para la que se solicita el valor.
        @param y Coordenada y para la que se solicita el valor.
        @return Valor que se debe representar.*/
};

/*! \class ColorMap spectrogramplot.cpp "spectrogramplot.cpp"
 * \brief Clase que determina la escala de colores usada para el espectrograma.
 *
 */


class ColorMap: public QwtLinearColorMap
{
public:

    ColorMap():
        QwtLinearColorMap(Qt::darkBlue, Qt::red){
        addColorStop(0.3, Qt::cyan);
        addColorStop(0.5, Qt::green);
        addColorStop(0.7, Qt::yellow);
    }
    /**< Constructor.*/
};

SpectrogramPlot::SpectrogramPlot(QWidget * parent):QwtPlot(parent){
    spectrogram = new QwtPlotSpectrogram();
    spectrogram->setRenderThreadCount(0);
    spectrogram->setColorMap(new ColorMap());
    spectrogram->attach(this);
    this->setCanvasLineWidth(0);

    //Una barra a la derecha para mostrar la escala de colores
    rightAxis = axisWidget(QwtPlot::yRight);
    zoomer=NULL;
    panner=NULL;

}

SpectrogramPlot::~SpectrogramPlot(){
    delete spectrogram;
}


void SpectrogramPlot::ConfigurePlot(Data2D * data){
    int axis;

    labelX=data->GetLabelX();
    labelY=data->GetLabelY();
    labelZ=data->GetLabelZ();

    minXValue=data->GetMinXValue();
    maxXValue=data->GetMaxXValue();
    minYValue=data->GetMinYValue();
    maxYValue=data->GetMaxYValue();

    //Cojo los valores máximos y mínimos para el caso de que sean necesarios
    minZValueData=data->GetMinZValue();
    maxZValueData=data->GetMaxZValue();

    plotLayout()->setAlignCanvasToScales(true);

    //Configurar la barra de la derecha con nuevos datos
    rightAxis->setTitle(labelZ);
    rightAxis->setColorBarEnabled(true);

    enableAxis(QwtPlot::yRight);

    axis=spectrogram->xAxis();
    QwtPlot::setAxisScale(axis,minXValue,maxXValue);
    QwtPlot::setAxisTitle(axis,labelX);

    axis=spectrogram->yAxis();
    QwtPlot::setAxisScale(axis,minYValue,maxYValue);
    QwtPlot::setAxisTitle(axis,labelY);

    replot();


    // Botón izquierdo para el zoom
    // Botón central para desplazamiento
    // Botón derecho para zoom out
    // Ctrl+botón Derecho: para quitar el zoom

    if(zoomer!=NULL) delete zoomer;
    zoomer = new SpectrogramZoomer(canvas());
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
        Qt::RightButton, Qt::ControlModifier);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,
        Qt::RightButton);

    if(panner!=NULL) delete panner;
    panner = new QwtPlotPanner(canvas());
    panner->setAxisEnabled(QwtPlot::yRight, false);
    panner->setMouseButton(Qt::MidButton);

    // Evita saltosc cuando las etiquetas con más/menos dígitos
    // aparecen/desaparecen al hacer scroll vertical

    const QFontMetrics fm(axisWidget(QwtPlot::yLeft)->font());
    QwtScaleDraw *sd = axisScaleDraw(QwtPlot::yLeft);
    sd->setMinimumExtent( fm.width("100.00") );

    const QColor c(Qt::darkBlue);
    zoomer->setRubberBandPen(c);
    zoomer->setTrackerPen(c);

    init=true;
    Update();
}


void SpectrogramPlot::SetData2D(Data2D * data){

    if(init==false)
        this->ConfigurePlot(data);

    QwtRasterData * specData;

    specData=new SpectrogramData(data,*minZValue,*maxZValue);

    //Nótese que la función setData() elimina automáticamente el objeto anterior
    spectrogram->setData(specData);
    replot();
}

void SpectrogramPlot::ShowContour(bool show){
    spectrogram->setDisplayMode(QwtPlotSpectrogram::ContourMode, show);
    replot();
}

void SpectrogramPlot::ShowSpectrogram(bool show){
    spectrogram->setDisplayMode(QwtPlotSpectrogram::ImageMode, show);
    spectrogram->setDefaultContourPen(show ? QPen() : QPen(Qt::NoPen));
    replot();
}

void SpectrogramPlot::Update(){

    //Reestablezco la separación entre líneas de nivel para los nuevos máximos
    QList<double> contourLevels;
    for ( double level = *minZValue ; level < *maxZValue; level += (*maxZValue-*minZValue)/10 )
        contourLevels += level;
    spectrogram->setContourLevels(contourLevels);

    //Actualizo la barra lateral
    QwtInterval zInterval;
    zInterval.setInterval(*minZValue,*maxZValue);
    setAxisScale(QwtPlot::yRight, zInterval.minValue(), zInterval.maxValue() );

    rightAxis->setColorMap( zInterval, new ColorMap());

    //Si ya tenía datos, los repinto de acuerdo a los nuevos máximos
    if(spectrogram->data()!=NULL)
        spectrogram->data()->setInterval( Qt::ZAxis, QwtInterval( *minZValue, *maxZValue) );

    replot();
}




QPixmap * SpectrogramPlot::GetAsQPixmap(){
    QPixmap * output;
    QPixmap qPix = QPixmap::grabWidget(this);

    output=new QPixmap(qPix);
    return output;

}
