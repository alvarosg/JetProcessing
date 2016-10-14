/**
 * @file   plot3ddata2d.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase abstracta para la representanción de superficies bidimensionales en tres dimensiones.
 *
 */
#ifndef PLOT3DDATA2D_H
#define PLOT3DDATA2D_H

#include <qwt3d_surfaceplot.h>
#include "plotdata2d.h"

/*! \class Plot3DData2D plot3ddata2d.h "plot3ddata2d.h"
 * \brief Clase abstracta para la representanción de superficies bidimensionales en tres dimensiones usando la biblioteca qwt3D.
 *
 *
 */

using namespace Qwt3D;

class Plot3DData2D : public SurfacePlot,  public PlotData2D{

public:
    Plot3DData2D(QWidget * parent);
    /**< Constructor.
      @param parent Widget padre.*/
    virtual ~Plot3DData2D();
    /**< Destructor.*/
    QPixmap * GetAsQPixmap();

};

#endif // PLOT3DDATA2D_H
