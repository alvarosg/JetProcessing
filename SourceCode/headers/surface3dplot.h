/**
 * @file   surface3dplot.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase para la representación de la superficie bidimensional en tres dimensiones de forma cartesiana.
 *
 */
#ifndef SURFACE3DPLOT_H
#define SURFACE3DPLOT_H

#include <qwt3d_function.h>
#include "plot3ddata2d.h"

using namespace Qwt3D;

/*! \class SurfaceFunction surface3dplot.h "surface3dplot.h"
 * \brief Clase que contiene los datos de la superficie que se va a representar.
 *
 * El operador reimplementado se encarga de responder a peticiones acerca de qué valor se debe representar en uan posición dada.
 *
 */

class SurfaceFunction : public Function{

private:
    Data2D * sourceData;  /**< Datos 2d que se están representando.*/
    float ** buffer;      /**< Puntero a puntero que apunta al array en la zona de memoria donde están los datos.*/
    int sizeX;            /**< Ancho de los datos.*/
    int sizeY;            /**< Alto de los datos.*/
    float ratioX;         /**< Número de píxeles por unidad para la variable independiente X.*/
    float ratioY;         /**< Número de píxeles por unidad para la variable independiente Y.*/
    float minX;           /**< Mínimo valor tomado por la variable independiente X. Correspondiente al índice 0.*/
    float minY;           /**< Mínimo valor tomado por la variable independiente Y. Correspondiente al índice sizeY-1.*/

public:
    SurfaceFunction(SurfacePlot& pw,const Data2D * data);
    /**< Constructor.
        @param pw Parámetro necesario para el constructor de Function.
        @param data Puntero a datos bidimensionales. SurfaceFunction hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.*/
    ~SurfaceFunction();
    /**< Destructor.*/
    void LoadData(const Data2D * data);
    /**< Carga los datos pasados como argumeno en la superficie.
        @param data Puntero a datos bidimensionales. SurfaceFunction hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.*/
    double operator()(double x, double y);
    /**< Operador reimplementado para responder a qué valor debe asignarse a cierta posición del gráfico.
            @param x Coordenada x para la que se solicita el valor.
            @param y Coordenada y para la que se solicita el valor.
            @return Valor que se debe representar.*/
};

/*! \class Surface3DPlot surface3dplot.cpp "surface3dplot.h"
 * \brief Clase que permite representar una los datos data 2d en una superficie tridimensional.
 *
 * Permite girar, y hacer zoom sobre el gráfico.
 *
 */


class Surface3DPlot : public Plot3DData2D
{
public:
    Surface3DPlot(QWidget * parent);
    /**< Constructor.
      @param parent Widget padre.*/
    ~Surface3DPlot();
    /**< Destructor. */
    void ConfigurePlot(Data2D * data);
    void SetData2D(Data2D * data2d);

private:
    void Update();
    /**< Redibuja con todos los posibles cambios de parámetros que haya habido en el gráfico.*/
    void calculateHull();
    /**< Función que reimplementada que calcula el tamaño de la caja que se debe dibujar para representar los datos.*/

    SurfaceFunction * surface;  /**< Puntero al objeto superficie necesario para la representación.*/
};

#endif // SURFACE3DPLOT_H
