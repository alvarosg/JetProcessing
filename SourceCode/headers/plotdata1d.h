/**
 * @file   plotdata1d.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase abstracta para la representaci�n de datos unidimensionales.
 *
 */
#ifndef PLOTDATA1D_H
#define PLOTDATA1D_H

#include "data1d.h"
#include "abstractplot.h"



/*! \class PlotData1D plotdata1d.h "plotdata1d.h"
 * \brief Clase abstracta para la representaci�n de una variable dependiente unidimensional usando un objeto del tipo Data1D.
 *
 *
 */


class PlotData1D : public AbstractPlot
{

protected:
    QString labelX;         /**< Etiqueta para la variable independiente.*/
    float minXValue;        /**< Valor m�nimo de la variable independiente.*/
    float maxXValue;        /**< Valor m�ximo de la variable independiente.*/

public:
    PlotData1D();
    /**< Constructor. */
    ~PlotData1D();
    /**< Destructor. */
    virtual void ConfigurePlot(Data1D * data1d)=0;
    /**< Funci�n que configura el gr�fico con los datos pasados unidimensionales como argumento.
        @param data1d Puntero a datos unidimensionales. PlotData1D hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.*/
    virtual void SetData1D(Data1D * data1d)=0;
    /**< Funci�n que representa en el gr�fico los datos unidimensionales. Si init estaba a falso, tambi�n configura el gr�fico con ellos.
        @param data1d Puntero a datos unidimensionales. PlotData1D hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.*/


};

#endif // PLOTDATA1D_H
