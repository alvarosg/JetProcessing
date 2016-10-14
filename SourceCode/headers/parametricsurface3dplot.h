/**
 * @file   parametricsurface3dplot.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase para la representaci�n de la superficie bidimensional en tres dimensiones de forma param�tica con simetr�a cil�ndrica.
 *
 */
#ifndef PARAMETRICSURFACE3DPLOT_H
#define PARAMETRICSURFACE3DPLOT_H

#include <qwt3d_parametricsurface.h>
#include "plot3ddata2d.h"

using namespace Qwt3D;

/*! \class ParametricSurfaceFunction parametricsurface3dplot.h "parametricsurface3dplot.h"
 * \brief Clase que contiene los datos a partir de los que se genera el gr�fico.
 *
 * El operador reimplementado se encarga de responder qu� raido debe dibujarse a una altura y �ngulo dado.
 *
 */

class ParametricSurfaceFunction : public ParametricSurface{

private:
    Data2D * sourceData;    /**< Datos 2d que se est�n representando.*/
    float ** buffer;        /**< Puntero a puntero que apunta al array en la zona de memoria donde est�n los datos.*/
    int sizeX;              /**< Ancho de los datos.*/
    int sizeY;              /**< Alto de los datos.*/
    float ratioX;           /**< N�mero de p�xeles por unidad para la variable independiente X.*/
    float ratioY;           /**< N�mero de p�xeles por unidad para la variable independiente Y.*/
    float minX;             /**< M�nimo valor tomado por la variable independiente X. Correspondiente al �ndice 0.*/
    float minY;             /**< M�nimo valor tomado por la variable independiente Y. Correspondiente al �ndice sizeY-1.*/
    float threshold;        /**< Valor de la variable dependiente alcanzado en la superficie equipotencial.*/

public:
    ParametricSurfaceFunction(SurfacePlot& pw,const Data2D * data,float inThreshold);
    /**< Constructor.
        @param pw Par�metro necesario para el constructor de Function.
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
    /**< Operador reimplementado para responder a qu� punto se debe pintar para una altura y �ngulo dado.
            @param phi �ngulo para el que se solicida el valor.
            @param z Altura para la que se solicita el valor.
            @return Punto que se debe dibujar.*/
};

/*! \class ParametricSurface3DPlot parametricsurface3dplot.h "parametricsurface3dplot.h"
 * \brief Clase que permite representar una los datos data 2d de forma par�metrica.
 *
 * Presupone que la los datos contenidos en data2d son una imagen de la seccion central de algo con simetr�a cil�ndrica, siendo el eje radial el eje x.
 * Asumiendo esto, representa una superficie tridimensional mostrando en qu� punto los datos llegan a sobrepasar cierto valor. De esta forma si la variable dependiente
 * representara un potencial, el gr�fico representar�a una superficie equipotencial, de valor, el pasado como argumento: "threshold".
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
    /**< Redibuja con todos los posibles cambios de par�metros que haya habido en el gr�fico.*/
    void calculateHull();
    /**< Funci�n que reimplementada que calcula el tama�o de la caja que se debe dibujar para representar los datos.*/

    float threshold;                            /**< Valor de la variable dependiente alcanzado en la superficie equipotencial.*/
    ParametricSurfaceFunction * surface;        /**< Puntero al objeto superficie necesario para la representaci�n.*/
};

#endif // PARAMETRICSURFACE3DPLOT_H
