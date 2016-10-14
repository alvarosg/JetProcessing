/**
 * @file   plot3ddata2d.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase Plot3DData2D.
 *
 */
#include "plot3ddata2d.h"

Plot3DData2D::Plot3DData2D(QWidget * parent): SurfacePlot(parent){

    for (unsigned i=0; i!=coordinates()->axes.size(); ++i)
    {
        coordinates()->axes[i].setMajors(10);
        coordinates()->axes[i].setMinors(1);
    }
}
Plot3DData2D::~Plot3DData2D(){
}



QPixmap * Plot3DData2D::GetAsQPixmap(){
    QPixmap * output;
    QPixmap qPix = this->renderPixmap();
    output=new QPixmap(qPix);
    return output;
}
