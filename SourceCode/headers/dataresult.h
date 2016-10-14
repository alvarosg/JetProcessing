/**
 * @file   dataresult.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase abstracta que para almacenar informacion de la fase una vez que se han aplicado todos los algoritmos deseados.
 *
 */
#ifndef DATARESULT_H
#define DATARESULT_H

#include <QString>


/*! \class DataResult dataresult.h "dataresult.h"
 * \brief Clase abstracta que sirve para almacenar informacion de la fase una vez que se han aplicado todos los algoritmos deseados.
 *
 * Guardará datos de una variable dependiente en función de un cierto número de variables independientes.
 *
 */

class DataResult{


protected:
    QString labelZ;         /**< Etiqueta para la variable dependiente.*/
    float maxZValue;        /**< Máximo valor tomado por la variable dependiente.*/
    float minZValue;        /**< Mínimo valor tomado por la variable dependiente.*/

public:
    DataResult();
    /**< Constructor*/

    void SetLabelZ(QString string);
    /**< Establece el valor del atributo labelZ.
        @param string Valor del atributo.*/
    QString GetLabelZ();
    /**< Devuelve el valor del atributo labelZ.
        @return Valor del atributo.*/
    float GetMinZValue();
    /**< Devuelve el valor del atributo minZValue.
        @return Valor del atributo.*/
    float GetMaxZValue();
    /**< Devuelve el valor del atributo maxZValue.
        @return Valor del atributo.*/

    virtual bool SaveToFile(QString filePath)=0;
    /**< Guarda la lista los datos en un fichero de texto, incluyendo en cada línea valores sepadarados por tabuladores de las variables independientes con su correspondiente valor de variable dependiente.
      @param filePath ruta y nombre del fichero donde se deben guardar.
      @return Verdadero si se guardó corectamente.*/

protected:
    virtual void CalculateMaxMinZ()=0;
    /**< Recorre los datos para calcular el valor máximo y mínimo de la variable dependiente.*/
};

#endif // DATARESULT_H
