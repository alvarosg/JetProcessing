/**
 * @file   surface3dplot.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase Surface3DPlot.
 *
 */
#include "surface3dplot.h"

using namespace Qwt3D;


SurfaceFunction::SurfaceFunction(SurfacePlot& pw,const Data2D * data):Function(pw){
    sourceData=new Data2D(*data);
    buffer=sourceData->GetBuffer();
    sizeX=sourceData->GetSizeX();
    sizeY=sourceData->GetSizeY();
    ratioX=sourceData->GetRatioPixelValueX();
    ratioY=sourceData->GetRatioPixelValueY();
    minX=sourceData->GetMinXValue();
    minY=sourceData->GetMinYValue();

}

void SurfaceFunction::LoadData(const Data2D * data){
    Data2D * aux1, *aux2;
    //Con dos punteros auxiliares, consigo que el valor esté a NULL mientras hago el trasbase de los datos viejos a los nuevos
    aux1=sourceData;
    sourceData=NULL;

    aux2=new Data2D(*data);
    buffer=aux2->GetBuffer();
    sizeX=aux2->GetSizeX();
    sizeY=aux2->GetSizeY();
    ratioX=aux2->GetRatioPixelValueX();
    ratioY=aux2->GetRatioPixelValueY();
    minX=aux2->GetMinXValue();
    minY=aux2->GetMinYValue();

    sourceData=aux2;
    delete aux1;
}

double SurfaceFunction::operator()(double x, double y){
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

SurfaceFunction::~SurfaceFunction(){
    delete sourceData;
}

Surface3DPlot::Surface3DPlot(QWidget * parent):Plot3DData2D(parent){
    surface=NULL;
    setShift(0.15,0,0);
    setZoom(0.8);
    setRotation(30,0,15);
}

Surface3DPlot::~Surface3DPlot(){
    if(surface!=NULL)
        delete surface;
}

void Surface3DPlot::ConfigurePlot(Data2D * data){
    init=true;

    labelX=data->GetLabelX();
    labelY=data->GetLabelY();
    labelZ=data->GetLabelZ();

    minXValue=data->GetMinXValue();
    maxXValue=data->GetMaxXValue();
    minYValue=data->GetMinYValue();
    maxYValue=data->GetMaxYValue();

    minZValueData=data->GetMinZValue();
    maxZValueData=data->GetMaxZValue();

    if(surface!=NULL) delete surface;
    surface = new SurfaceFunction(*this,data);

    surface->setMesh(40,40);
    surface->setDomain(minXValue,maxXValue,minYValue,maxYValue);
    surface->setMinZ(*minZValue);
    surface->setMaxZ(*maxZValue);

    //Ajusto la escala para que el eje z se vea bien de tamaño
    if(*maxZValue-*minZValue!=0)
        setScale(1,1,(qMin(maxXValue-minXValue,maxYValue-minYValue))/(*maxZValue-*minZValue));
    else
        setScale(1,1,1);

}


void Surface3DPlot::SetData2D(Data2D * data){

    if(init==false)
        this->ConfigurePlot(data);
    else
        surface->LoadData(data);

    this->Update();
}

void Surface3DPlot::Update(){

    if(surface==NULL)
        return;

    surface->setMinZ(*minZValue);
    surface->setMaxZ(*maxZValue);

    surface->create();

    this->setTitleFont("Arial",15,QFont::Bold);

    this->coordinates()->axes[X1].setLabelString(labelX);
    this->coordinates()->axes[X1].setLabelFont("Arial",10);
    this->coordinates()->axes[X1].setNumberFont("Arial",6);
    this->coordinates()->axes[X2].setLabelString(labelX);
    this->coordinates()->axes[X2].setLabelFont("Arial",10);
    this->coordinates()->axes[X2].setNumberFont("Arial",6);
    this->coordinates()->axes[X3].setLabelString(labelX);
    this->coordinates()->axes[X3].setLabelFont("Arial",10);
    this->coordinates()->axes[X3].setNumberFont("Arial",6);
    this->coordinates()->axes[X4].setLabelString(labelX);
    this->coordinates()->axes[X4].setLabelFont("Arial",10);
    this->coordinates()->axes[X4].setNumberFont("Arial",6);

    this->coordinates()->axes[Y1].setLabelString(labelY);
    this->coordinates()->axes[Y1].setLabelFont("Arial",10);
    this->coordinates()->axes[Y1].setNumberFont("Arial",6);
    this->coordinates()->axes[Y2].setLabelString(labelY);
    this->coordinates()->axes[Y2].setLabelFont("Arial",10);
    this->coordinates()->axes[Y2].setNumberFont("Arial",6);
    this->coordinates()->axes[Y3].setLabelString(labelY);
    this->coordinates()->axes[Y3].setLabelFont("Arial",10);
    this->coordinates()->axes[Y3].setNumberFont("Arial",6);
    this->coordinates()->axes[Y4].setLabelString(labelY);
    this->coordinates()->axes[Y4].setLabelFont("Arial",10);
    this->coordinates()->axes[Y4].setNumberFont("Arial",6);

    this->coordinates()->axes[Z1].setLabelString(labelZ);
    this->coordinates()->axes[Z1].setLabelFont("Arial",10);
    this->coordinates()->axes[Z1].setNumberFont("Arial",6);
    this->coordinates()->axes[Z2].setLabelString(labelZ);
    this->coordinates()->axes[Z2].setLabelFont("Arial",10);
    this->coordinates()->axes[Z2].setNumberFont("Arial",6);
    this->coordinates()->axes[Z3].setLabelString(labelZ);
    this->coordinates()->axes[Z3].setLabelFont("Arial",10);
    this->coordinates()->axes[Z3].setNumberFont("Arial",6);
    this->coordinates()->axes[Z4].setLabelString(labelZ);
    this->coordinates()->axes[Z4].setLabelFont("Arial",10);
    this->coordinates()->axes[Z4].setNumberFont("Arial",6);

    setCoordinateStyle(BOX);

    //Ajusto la escala para que el eje z se vea bien de tamaño
    if(*maxZValue-*minZValue!=0)
        setScale(1,1,(qMin(maxXValue-minXValue,maxYValue-minYValue))/(*maxZValue-*minZValue));
    else
        setScale(1,1,1);

    updateData();
    updateGL();

}

void Surface3DPlot::calculateHull(){
    if (actualData_p->empty())
        return;
    this->setHull(Qwt3D::ParallelEpiped(Triple(minXValue,minYValue,*minZValue),Triple(maxXValue,maxYValue,*maxZValue)));
}
