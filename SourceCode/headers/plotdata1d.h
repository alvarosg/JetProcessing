/**
 * @file   plotdata1d.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase abstracta para la representación de datos unidimensionales.
 *
 */
#ifndef PLOTDATA1D_H
#define PLOTDATA1D_H

#include "data1d.h"
#include "abstractplot.h"



/*! \class PlotData1D plotdata1d.h "plotdata1d.h"
 * \brief Clase abstracta para la representación de una variable dependiente unidimensional usando un objeto del tipo Data1D.
 *
 *
 */


class PlotData1D : public AbstractPlot
{

protected:
    QString labelX;         /**< Etiqueta para la variable independiente.*/
    float minXValue;        /**< Valor mínimo de la variable independiente.*/
    float maxXValue;        /**< Valor máximo de la variable independiente.*/

public:
    PlotData1D();
    /**< Constructor. */
    ~PlotData1D();
    /**< Destructor. */
    virtual void ConfigurePlot(Data1D * data1d)=0;
    /**< Función que configura el gráfico con los datos pasados unidimensionales como argumento.
        @param data1d Puntero a datos unidimensionales. PlotData1D hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.*/
    virtual void SetData1D(Data1D * data1d)=0;
    /**< Función que representa en el gráfico los datos unidimensionales. Si init estaba a falso, también configura el gráfico con ellos.
        @param data1d Puntero a datos unidimensionales. PlotData1D hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.*/


};

#endif // PLOTDATA1D_H
