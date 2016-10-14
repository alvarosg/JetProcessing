/**
 * @file   surface3dplot.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase para la representaci�n de la superficie bidimensional en tres dimensiones de forma cartesiana.
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
 * El operador reimplementado se encarga de responder a peticiones acerca de qu� valor se debe representar en uan posici�n dada.
 *
 */

class SurfaceFunction : public Function{

private:
    Data2D * sourceData;  /**< Datos 2d que se est�n representando.*/
    float ** buffer;      /**< Puntero a puntero que apunta al array en la zona de memoria donde est�n los datos.*/
    int sizeX;            /**< Ancho de los datos.*/
    int sizeY;            /**< Alto de los datos.*/
    float ratioX;         /**< N�mero de p�xeles por unidad para la variable independiente X.*/
    float ratioY;         /**< N�mero de p�xeles por unidad para la variable independiente Y.*/
    float minX;           /**< M�nimo valor tomado por la variable independiente X. Correspondiente al �ndice 0.*/
    float minY;           /**< M�nimo valor tomado por la variable independiente Y. Correspondiente al �ndice sizeY-1.*/

public:
    SurfaceFunction(SurfacePlot& pw,const Data2D * data);
    /**< Constructor.
        @param pw Par�metro necesario para el constructor de Function.
        @param data Puntero a datos bidimensionales. SurfaceFunction hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.*/
    ~SurfaceFunction();
    /**< Destructor.*/
    void LoadData(const Data2D * data);
    /**< Carga los datos pasados como argumeno en la superficie.
        @param data Puntero a datos bidimensionales. SurfaceFunction hace una copia de los datos, por lo que el padre sigue siendo repsonsable de el objeto pasado como argumento.*/
    double operator()(double x, double y);
    /**< Operador reimplementado para responder a qu� valor debe asignarse a cierta posici�n del gr�fico.
            @param x Coordenada x para la que se solicita el valor.
            @param y Coordenada y para la que se solicita el valor.
            @return Valor que se debe representar.*/
};

/*! \class Surface3DPlot surface3dplot.cpp "surface3dplot.h"
 * \brief Clase que permite representar una los datos data 2d en una superficie tridimensional.
 *
 * Permite girar, y hacer zoom sobre el gr�fico.
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
    /**< Redibuja con todos los posibles cambios de par�metros que haya habido en el gr�fico.*/
    void calculateHull();
    /**< Funci�n que reimplementada que calcula el tama�o de la caja que se debe dibujar para representar los datos.*/

    SurfaceFunction * surface;  /**< Puntero al objeto superficie necesario para la representaci�n.*/
};

#endif // SURFACE3DPLOT_H
