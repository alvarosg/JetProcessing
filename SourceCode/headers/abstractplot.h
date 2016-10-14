/**
 * @file   abstractplot.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase abstracta para todo tipo de representaciones gráficas.
 *
 */

#ifndef ABSTRACTPLOT_H
#define ABSTRACTPLOT_H

#include <QtGui/QApplication>

/*! \class AbstractPlot abstractplot.h "abstractplot.h"
 * \brief Clase abstracta para todo tipo de representaciones gráficas.
 *
 *  Todas las representaciones que tengan una variable dependiente deben ser derivadas de esta clase.
 *
 */

class AbstractPlot
{
public:
    AbstractPlot();
    /**< Contstructor.*/
    virtual ~AbstractPlot();
    /**< Destructor.*/
    virtual void SetMinZValue(float value);
    /**< Fija manualmente el valor mínimo para la variable dependiente en el gráfico.
        @param value Valor mínimo para fijar. Si se configura un valor mínimo mayor o igual al valor máximo se realizará escalado automático.*/
    virtual float GetMinZValue();
    /**< Obtiene el valor mínimo fijado para la variable dependiente.
        @return Valor mínimo.*/
    virtual void SetMaxZValue(float value);
    /**< Fija manualmente el valor mínimo para la variable dependiente en el gráfico
        @param value Valor mínimo para fijar. Si se configura un valor mínimo mayor o igual al valor máximo se realizará escalado automático.*/
    virtual float GetMaxZValue();
    /**< Obtiene el valor máximo fijado para la variable dependiente.
        @return Valor máximo.*/
    virtual void SetMinMaxZValue(float minValue,float maxValue);
    /**< Fija manualmente los valores mínimo y máximo para la variable dependiente en el gráfico.
        @param minValue Valor mínimo para fijar. Si se configura un valor mínimo mayor o igual al valor máximo se realizará escalado automático.
        @param maxValue Valor mánimo para fijar. Si se configura un valor máximo menor o igual al valor mínimo se realizará escalado automático.*/
    void SetInit(bool value);
    /**< Configura el estado de inizialización o no, del gráfico.
        @param value Si se establece a falso, los ejes y otros elementos del gráfico se configurarán en función de los siguientes datos representados.*/
    bool GetInit();
    /**< Obtiene el estado de inicialización del gráfico.
        @return Estado de inicialización.*/

    virtual QPixmap * GetAsQPixmap()=0;
    /**< Obtiene la imagen actual del gráfico como un objeto QPixmap.
        @return El objeto QPixmap con la imagen del gráfico. Este objeto será responsbilidad del cliente.*/

protected:
    bool init; /**< Estado de inicialización del gráfico. Si está a falso, se configurará con los siguientes datos representados.*/
    QString labelZ;     /**< Etiqueta para el eje de la variable dependiente.*/
    float * minZValue; /**< Puntero que apunta al valor mínimo para el eje Z en la representación. Apuntará o bien a minZValueCustom o bien a minZValueData.*/
    float * maxZValue; /**< Puntero que apunta al valor máximo para el eje Z en la representación. Apuntará o bien a maxZValueCustom o bien a maxZValueData.*/
    float minZValueCustom;  /**< Valor mínimo para la variable dependiente introducido por el usuario, se usará si y sólo si es menor que el máximo. Por defecto será mayor que el máximo.*/
    float maxZValueCustom;  /**< Valor máximo para la variable dependiente introducido por el usuario, se usará si y sólo si es mayor que el mínimo. Por defecto será menor que el mínimo.*/
    float minZValueData;  /**< Valor mínimo para la variable dependiente en el caso de escalado automático. Se obtiendrá de los datos introducidos.*/
    float maxZValueData;  /**< Valor máximo para la variable dependiente en el caso de escalado automático. Se obtiendrá de los datos introducidos.*/
    virtual void Update()=0;
    /**< Este método debe implementarse para que tras su llamada se redibuje el gráfico con todos los posibles cambios de parámetros que haya habido en el gráfico.*/
};

#endif // ABSTRACTPLOT_H
