/**
 * @file   parametricsurface3dplot.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase ParametricSurface3DPlot.
 *
 */
#include "parametricsurface3dplot.h"

using namespace Qwt3D;


ParametricSurfaceFunction::ParametricSurfaceFunction(SurfacePlot& pw,const Data2D * data,float inThreshold):ParametricSurface(pw){
    sourceData=new Data2D(*data);
    buffer=sourceData->GetBuffer();
    sizeX=sourceData->GetSizeX();
    sizeY=sourceData->GetSizeY();
    ratioX=sourceData->GetRatioPixelValueX();
    ratioY=sourceData->GetRatioPixelValueY();
    minX=sourceData->GetMinXValue();
    minY=sourceData->GetMinYValue();
    threshold=inThreshold;
}

void ParametricSurfaceFunction::LoadData(const Data2D * data){
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

void ParametricSurfaceFunction::SetThreshold(float value){
    threshold=value;
}
float ParametricSurfaceFunction::GetThreshold(){
    return threshold;
}

Triple ParametricSurfaceFunction::operator()(double phi, double z){

    float R;
    int pixZ=(sizeY-1-(z-minY)/ratioY);
    int xAux=-1;

    if(pixZ<sizeY && pixZ>0){
        while(++xAux < sizeX && buffer[pixZ][xAux] < threshold);

        if (xAux==sizeX)
            R=0;
        else
            R=(sizeX/2-xAux)*ratioX;
        }
    else
        R=0;
    Triple triple;
    triple.x=R*cos(phi);
    triple.y=R*sin(phi);
    triple.z=z;

    return triple;
}
ParametricSurfaceFunction::~ParametricSurfaceFunction(){
  delete sourceData;
}

ParametricSurface3DPlot::ParametricSurface3DPlot(QWidget * parent):Plot3DData2D(parent){
    surface=NULL;
    threshold=0;
    setRotation(30,0,15);
    setZoom(0.8);
    setShift(0.15,0,0);
}

ParametricSurface3DPlot::~ParametricSurface3DPlot(){
    if(surface!=NULL)
        delete surface;
}

void ParametricSurface3DPlot::ConfigurePlot(Data2D * data){
    init=true;

    labelX=data->GetLabelX();
    labelY=data->GetLabelX();
    labelZ=data->GetLabelY();

    minXValue=data->GetMinXValue();
    maxXValue=data->GetMaxXValue();
    minYValue=data->GetMinYValue();
    maxYValue=data->GetMaxYValue();

    ParametricSurfaceFunction * surfaceAux;

    surfaceAux = new ParametricSurfaceFunction(*this,data,threshold);

    if(surface!=NULL)
        delete surface;

    surface=surfaceAux;

    surface->setMesh(30,40);
    surface->setDomain(0,2*PI,minYValue,maxYValue);


    //Ajusto la escala para que el eje z se vea bien de tamaño
    if(data->GetMaxZValue()-data->GetMinZValue()!=0)
        setScale(1,1,(maxXValue-minXValue)/(maxYValue-minYValue));
    else
        setScale(1,1,1);

}


void ParametricSurface3DPlot::SetData2D(Data2D * data){

    if(init==false)
        this->ConfigurePlot(data);
    else
        surface->LoadData(data);

    this->Update();
}

void ParametricSurface3DPlot::SetThreshold(float value){
    threshold=value;
    if(surface!=NULL){
        surface->SetThreshold(threshold);
        this->Update();
    }

}
float ParametricSurface3DPlot::GetThreshold(){
    return threshold;
}

void ParametricSurface3DPlot::Update(){
    if(surface==NULL)
        return;

    coordinates()->axes[X1].setMajors(6);
    coordinates()->axes[X2].setMajors(6);
    coordinates()->axes[X3].setMajors(6);
    coordinates()->axes[X4].setMajors(6);
    coordinates()->axes[Y1].setMajors(6);
    coordinates()->axes[Y2].setMajors(6);
    coordinates()->axes[Y3].setMajors(6);
    coordinates()->axes[Y4].setMajors(6);

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

    updateData();
    updateGL();

}

void ParametricSurface3DPlot::calculateHull(){
    if (actualData_p->empty())
        return;
    float width=maxXValue-minXValue;

    this->setHull(Qwt3D::ParallelEpiped(Triple(-width/2,-width/2,minYValue),Triple(width/2,width/2,maxYValue)));
}
