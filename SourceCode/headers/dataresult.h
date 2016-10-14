/**
 * @file   dataresult.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
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
 * Guardar� datos de una variable dependiente en funci�n de un cierto n�mero de variables independientes.
 *
 */

class DataResult{


protected:
    QString labelZ;         /**< Etiqueta para la variable dependiente.*/
    float maxZValue;        /**< M�ximo valor tomado por la variable dependiente.*/
    float minZValue;        /**< M�nimo valor tomado por la variable dependiente.*/

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
    /**< Guarda la lista los datos en un fichero de texto, incluyendo en cada l�nea valores sepadarados por tabuladores de las variables independientes con su correspondiente valor de variable dependiente.
      @param filePath ruta y nombre del fichero donde se deben guardar.
      @return Verdadero si se guard� corectamente.*/

protected:
    virtual void CalculateMaxMinZ()=0;
    /**< Recorre los datos para calcular el valor m�ximo y m�nimo de la variable dependiente.*/
};

#endif // DATARESULT_H
