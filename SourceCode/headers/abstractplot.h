/**
 * @file   abstractplot.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase abstracta para todo tipo de representaciones gr�ficas.
 *
 */

#ifndef ABSTRACTPLOT_H
#define ABSTRACTPLOT_H

#include <QtGui/QApplication>

/*! \class AbstractPlot abstractplot.h "abstractplot.h"
 * \brief Clase abstracta para todo tipo de representaciones gr�ficas.
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
    /**< Fija manualmente el valor m�nimo para la variable dependiente en el gr�fico.
        @param value Valor m�nimo para fijar. Si se configura un valor m�nimo mayor o igual al valor m�ximo se realizar� escalado autom�tico.*/
    virtual float GetMinZValue();
    /**< Obtiene el valor m�nimo fijado para la variable dependiente.
        @return Valor m�nimo.*/
    virtual void SetMaxZValue(float value);
    /**< Fija manualmente el valor m�nimo para la variable dependiente en el gr�fico
        @param value Valor m�nimo para fijar. Si se configura un valor m�nimo mayor o igual al valor m�ximo se realizar� escalado autom�tico.*/
    virtual float GetMaxZValue();
    /**< Obtiene el valor m�ximo fijado para la variable dependiente.
        @return Valor m�ximo.*/
    virtual void SetMinMaxZValue(float minValue,float maxValue);
    /**< Fija manualmente los valores m�nimo y m�ximo para la variable dependiente en el gr�fico.
        @param minValue Valor m�nimo para fijar. Si se configura un valor m�nimo mayor o igual al valor m�ximo se realizar� escalado autom�tico.
        @param maxValue Valor m�nimo para fijar. Si se configura un valor m�ximo menor o igual al valor m�nimo se realizar� escalado autom�tico.*/
    void SetInit(bool value);
    /**< Configura el estado de inizializaci�n o no, del gr�fico.
        @param value Si se establece a falso, los ejes y otros elementos del gr�fico se configurar�n en funci�n de los siguientes datos representados.*/
    bool GetInit();
    /**< Obtiene el estado de inicializaci�n del gr�fico.
        @return Estado de inicializaci�n.*/

    virtual QPixmap * GetAsQPixmap()=0;
    /**< Obtiene la imagen actual del gr�fico como un objeto QPixmap.
        @return El objeto QPixmap con la imagen del gr�fico. Este objeto ser� responsbilidad del cliente.*/

protected:
    bool init; /**< Estado de inicializaci�n del gr�fico. Si est� a falso, se configurar� con los siguientes datos representados.*/
    QString labelZ;     /**< Etiqueta para el eje de la variable dependiente.*/
    float * minZValue; /**< Puntero que apunta al valor m�nimo para el eje Z en la representaci�n. Apuntar� o bien a minZValueCustom o bien a minZValueData.*/
    float * maxZValue; /**< Puntero que apunta al valor m�ximo para el eje Z en la representaci�n. Apuntar� o bien a maxZValueCustom o bien a maxZValueData.*/
    float minZValueCustom;  /**< Valor m�nimo para la variable dependiente introducido por el usuario, se usar� si y s�lo si es menor que el m�ximo. Por defecto ser� mayor que el m�ximo.*/
    float maxZValueCustom;  /**< Valor m�ximo para la variable dependiente introducido por el usuario, se usar� si y s�lo si es mayor que el m�nimo. Por defecto ser� menor que el m�nimo.*/
    float minZValueData;  /**< Valor m�nimo para la variable dependiente en el caso de escalado autom�tico. Se obtiendr� de los datos introducidos.*/
    float maxZValueData;  /**< Valor m�ximo para la variable dependiente en el caso de escalado autom�tico. Se obtiendr� de los datos introducidos.*/
    virtual void Update()=0;
    /**< Este m�todo debe implementarse para que tras su llamada se redibuje el gr�fico con todos los posibles cambios de par�metros que haya habido en el gr�fico.*/
};

#endif // ABSTRACTPLOT_H
