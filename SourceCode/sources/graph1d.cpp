/**
 * @file   graph1d.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase Graph1D.
 *
 */
#include "graph1d.h"


Zoomer::Zoomer(int xAxis, int yAxis, QwtPlotCanvas *canvas): QwtPlotZoomer(xAxis, yAxis, canvas){
    setTrackerMode(QwtPicker::AlwaysOff);
    setRubberBand(QwtPicker::NoRubberBand);

    // Botón derecho: Hacer zoom hacia atrás una vez
    // Ctrl+Botón derecho: zoom hacia fuera del todo

    setMousePattern(QwtEventPattern::MouseSelect2,
        Qt::RightButton, Qt::ControlModifier);
    setMousePattern(QwtEventPattern::MouseSelect3,
        Qt::RightButton);
}

Graph1D::Graph1D(QWidget * parent):QwtPlot(parent){

    //Inicializo variables
    init=false;
    xValues=NULL;
    zValues=NULL;
    zoomer[0]=NULL;
    zoomer[1]=NULL;
    panner=NULL;
    picker=NULL;
    minXValue=-1;
    maxXValue=1;

    setAutoReplot(false);

    this->setCanvasLineWidth(0);
    plotLayout()->setAlignCanvasToScales(true);
    setCanvasBackground(QColor(Qt::blue));

    // Establezco los ejes
    setAxisMaxMajor(QwtPlot::xBottom, 6);
    setAxisMaxMinor(QwtPlot::xBottom, 5);
    setAxisMaxMajor(QwtPlot::yLeft, 6);
    setAxisMaxMinor(QwtPlot::yLeft, 5);

    // Establezco la cuadrícula
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->setMajPen(QPen(Qt::white, 0, Qt::DotLine));
    grid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
    grid->attach(this);

    // curves
    curve = new QwtPlotCurve();
    curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    curve->setYAxis(QwtPlot::yLeft);
    curve->setPen(QPen(QBrush(Qt::yellow),3));
    curve->attach(this);

    setAutoReplot(true);
}

void Graph1D::ConfigurePlot(Data1D * data1d){
    init=true;
    labelX=data1d->GetLabelX();
    labelZ=data1d->GetLabelZ();

    minXValue=data1d->GetMinXValue();
    maxXValue=data1d->GetMaxXValue();
    //Cojo los valores máximos y mínimos para el caso de que sean necesarios
    minZValueData=data1d->GetMinZValue();
    maxZValueData=data1d->GetMaxZValue();

    QString string;
    string.sprintf("\n(%3g,%3g) (%3g,%3g)",data1d->GetInitXPoint(),data1d->GetInitYPoint(),data1d->GetEndXPoint(),data1d->GetEndYPoint());
    setAxisTitle(QwtPlot::xBottom, labelX+string);
    setAxisTitle(QwtPlot::yLeft, labelZ);


    this->LoadData(data1d);
    this->Update();
}

void Graph1D::SetData1D(Data1D * data1d){

    const bool doReplot = autoReplot();
    setAutoReplot(false);

    if(init==false)
        this->ConfigurePlot(data1d);
    else
        this->LoadData(data1d);

    replot();
    setAutoReplot(doReplot);

}

void Graph1D::LoadData(Data1D * data1d){
    int i;

    sizeX=data1d->GetSizeX();

    if(xValues!=NULL){
        free(xValues);
        free(zValues);
    }
    zValues=(double *) malloc(sizeof(double)*sizeX);
    xValues=(double *) malloc(sizeof(double)*sizeX);

    float * buffer = data1d->GetBuffer();

    float minX=data1d->GetMinXValue();
    float maxX=data1d->GetMaxXValue();

    if(buffer!=NULL){
        for (i=0;i<sizeX;i++){
            zValues[i]=buffer[i];
            xValues[i]=minX+i*(maxX-minX)/(sizeX-1);
        }

        curve->setSamples(xValues, zValues, sizeX);
    }

}

void Graph1D::Update(){

    this->setAxisScale(QwtPlot::xBottom,minXValue,maxXValue);


    //Si los valores de Z no son coherentes, usamos un autoescalado
    this->setAxisScale(QwtPlot::yLeft,*minZValue,*maxZValue);

    //Para hacer zoom y otras operaciones con el ratón
    if(zoomer[0]!=NULL) delete zoomer[0],zoomer[1];
    zoomer[0] = new Zoomer( QwtPlot::xBottom, QwtPlot::yLeft, this->canvas());
    zoomer[0]->setRubberBand(QwtPicker::RectRubberBand);
    zoomer[0]->setRubberBandPen(QColor(Qt::green));
    zoomer[0]->setTrackerMode(QwtPicker::ActiveOnly);
    zoomer[0]->setTrackerPen(QColor(Qt::white));

    zoomer[1] = new Zoomer(QwtPlot::xTop, QwtPlot::yRight,this->canvas());

    if(panner!=NULL) delete panner;
    panner = new QwtPlotPanner(this->canvas());
    panner->setMouseButton(Qt::MidButton);

    if(picker!=NULL) delete picker;
    picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn,
                                this->canvas());
    picker->setStateMachine(new QwtPickerDragPointMachine());
    picker->setRubberBandPen(QColor(Qt::green));
    picker->setRubberBand(QwtPicker::CrossRubberBand);
    picker->setTrackerPen(QColor(Qt::white));
    replot();

}
Graph1D::~Graph1D(){
    if(xValues!=NULL){
        free(xValues);
        free(zValues);
    }
    if(zoomer[0]!=NULL) delete zoomer[0],zoomer[1];
    if(picker!=NULL) delete picker;
    if(panner!=NULL) delete panner;
}


QPixmap * Graph1D::GetAsQPixmap(){

    QPixmap * output;
    QPixmap qPix = QPixmap::grabWidget(this);
    output=new QPixmap(qPix);
    return output;

}

