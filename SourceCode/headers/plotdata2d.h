/**
 * @file   plotdata2d.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase abstracta para la representación de datos bidimensionales.
 *
 */
#ifndef PLOTDATA2D_H
#define PLOTDATA2D_H

#include <QtGui/QApplication>
#include "data2d.h"
#include "abstractplot.h"

/*! \class PlotData2D plotdata2d.h "plotdata2d.h"
 * \brief Clase abstracta para la representación de una variable dependiente bidimensional usando un objeto del tipo Data2D.
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
    /**< Función que configura el gráfico con los datos bidimensionales pasados como argumento.
        @param data2d Puntero a datos bidimensionales. PlotData2D hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.*/

    virtual void SetData2D(Data2D * data2d)=0;
    /**< Función que representa en el gráfico los datos bidimensionales. Si init estaba a falso, también configura el gráfico con ellos.
        @param data2d Puntero a datos bidimensionales. PlotData2D hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.*/

protected:
    QString labelX;     /**< Etiqueta para la variable independiente X.*/
    QString labelY;     /**< Etiqueta para la variable independiente Y.*/

    float minXValue;    /**< Valor mínimo de la variable independiente X.*/
    float maxXValue;    /**< Valor máximo de la variable independiente X.*/
    float minYValue;    /**< Valor mínimo de la variable independiente Y.*/
    float maxYValue;    /**< Valor máximo de la variable independiente Y.*/
};

#endif // PLOTDATA2D_H
