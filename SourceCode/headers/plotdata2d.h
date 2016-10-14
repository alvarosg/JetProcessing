/**
 * @file   plotdata2d.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase abstracta para la representaci�n de datos bidimensionales.
 *
 */
#ifndef PLOTDATA2D_H
#define PLOTDATA2D_H

#include <QtGui/QApplication>
#include "data2d.h"
#include "abstractplot.h"

/*! \class PlotData2D plotdata2d.h "plotdata2d.h"
 * \brief Clase abstracta para la representaci�n de una variable dependiente bidimensional usando un objeto del tipo Data2D.
 *
 *
 */

class PlotData2D : public AbstractPlot
{
public:
    PlotData2D();
    /**< Constructor. */
    virtual ~PlotData2D();
    /**< Destructor. */
    virtual void ConfigurePlot(Data2D * data2d)=0;
    /**< Funci�n que configura el gr�fico con los datos bidimensionales pasados como argumento.
        @param data2d Puntero a datos bidimensionales. PlotData2D hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.*/

    virtual void SetData2D(Data2D * data2d)=0;
    /**< Funci�n que representa en el gr�fico los datos bidimensionales. Si init estaba a falso, tambi�n configura el gr�fico con ellos.
        @param data2d Puntero a datos bidimensionales. PlotData2D hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.*/

protected:
    QString labelX;     /**< Etiqueta para la variable independiente X.*/
    QString labelY;     /**< Etiqueta para la variable independiente Y.*/

    float minXValue;    /**< Valor m�nimo de la variable independiente X.*/
    float maxXValue;    /**< Valor m�ximo de la variable independiente X.*/
    float minYValue;    /**< Valor m�nimo de la variable independiente Y.*/
    float maxYValue;    /**< Valor m�ximo de la variable independiente Y.*/
};

#endif // PLOTDATA2D_H
