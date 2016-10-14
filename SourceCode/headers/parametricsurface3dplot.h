/**
 * @file   parametricsurface3dplot.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase para la representación de la superficie bidimensional en tres dimensiones de forma paramética con simetría cilíndrica.
 *
 */
#ifndef PARAMETRICSURFACE3DPLOT_H
#define PARAMETRICSURFACE3DPLOT_H

#include <qwt3d_parametricsurface.h>
#include "plot3ddata2d.h"

using namespace Qwt3D;

/*! \class ParametricSurfaceFunction parametricsurface3dplot.h "parametricsurface3dplot.h"
 * \brief Clase que contiene los datos a partir de los que se genera el gráfico.
 *
 * El operador reimplementado se encarga de responder qué raido debe dibujarse a una altura y ángulo dado.
 *
 */

class ParametricSurfaceFunction : public ParametricSurface{

private:
    Data2D * sourceData;    /**< Datos 2d que se están representando.*/
    float ** buffer;        /**< Puntero a puntero que apunta al array en la zona de memoria donde están los datos.*/
    int sizeX;              /**< Ancho de los datos.*/
    int sizeY;              /**< Alto de los datos.*/
    float ratioX;           /**< Número de píxeles por unidad para la variable independiente X.*/
    float ratioY;           /**< Número de píxeles por unidad para la variable independiente Y.*/
    float minX;             /**< Mínimo valor tomado por la variable independiente X. Correspondiente al índice 0.*/
    float minY;             /**< Mínimo valor tomado por la variable independiente Y. Correspondiente al índice sizeY-1.*/
    float threshold;        /**< Valor de la variable dependiente alcanzado en la superficie equipotencial.*/

public:
    ParametricSurfaceFunction(SurfacePlot& pw,const Data2D * data,float inThreshold);
    /**< Constructor.
        @param pw Parámetro necesario para el constructor de Function.
        @param data Puntero a datos bidimensionales. ParametricSurfaceFunction hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.
        @param inThreshold Valor inicial para el atributo threshold.*/
    ~ParametricSurfaceFunction();
    /**< Destructor.*/
    void LoadData(const Data2D * data);
    /**< Carga los datos pasados como argumeno en la superficie.
        @param data Puntero a datos bidimensionales. ParametricSurfaceFunction hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.*/
    void SetThreshold(float value);
    /**< Establece el valor del atributo threshold.
        @param value Valor para el atributo.*/
    float GetThreshold();
    /**< Recupera el atributo threshold.
        @return Valor del atributo.*/

    Triple operator()(double phi, double z);
    /**< Operador reimplementado para responder a qué punto se debe pintar para una altura y ángulo dado.
            @param phi Ángulo para el que se solicida el valor.
            @param z Altura para la que se solicita el valor.
            @return Punto que se debe dibujar.*/
};

/*! \class ParametricSurface3DPlot parametricsurface3dplot.h "parametricsurface3dplot.h"
 * \brief Clase que permite representar una los datos data 2d de forma parámetrica.
 *
 * Presupone que la los datos contenidos en data2d son una imagen de la seccion central de algo con simetría cilíndrica, siendo el eje radial el eje x.
 * Asumiendo esto, representa una superficie tridimensional mostrando en qué punto los datos llegan a sobrepasar cierto valor. De esta forma si la variable dependiente
 * representara un potencial, el gráfico representaría una superficie equipotencial, de valor, el pasado como argumento: "threshold".
 *
 */



class ParametricSurface3DPlot : public Plot3DData2D
{
public:
    ParametricSurface3DPlot(QWidget * parent);
    /**< Constructor.
      @param parent Widget padre.*/
    ~ParametricSurface3DPlot();
    /**< Destructor. */
    void ConfigurePlot(Data2D * data);
    void SetData2D(Data2D * data2d);

    void SetThreshold(float value);
    /**< Establece el valor del atributo threshold.
        @param value Valor para el atributo.*/
    float GetThreshold();
    /**< Recupera el atributo threshold.
        @return Valor del atributo.*/


private:
    void Update();
    /**< Redibuja con todos los posibles cambios de parámetros que haya habido en el gráfico.*/
    void calculateHull();
    /**< Función que reimplementada que calcula el tamaño de la caja que se debe dibujar para representar los datos.*/

    float threshold;                            /**< Valor de la variable dependiente alcanzado en la superficie equipotencial.*/
    ParametricSurfaceFunction * surface;        /**< Puntero al objeto superficie necesario para la representación.*/
};

#endif // PARAMETRICSURFACE3DPLOT_H
