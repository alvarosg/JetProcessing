/**
 * @file   data2d.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase que que contiene una estructura de datos bidimensional.
 *
 */


#ifndef DATA2D_H
#define DATA2D_H

#include <stdlib.h>
#include <QFile>
#include <QRect>
#include <QTextStream>
#include <data1d.h>
#include <dataresult.h>
#include "cpuphaseinterface.h"


/*! \class Data2D data2d.h "data2d.h"
 * \brief Clase que que contiene una información de fase bidimensional.
 *
 */

class Data2D: public DataResult{

private:
    int sizeX;              /**< Ancho en pixeles de la imagen de la fase.*/
    int sizeY;              /**< Alto en pixeles de la imagen de la fase.*/
    float * linearBuffer;   /**< Puntero que apunta a la zona de memoria donde se encuentran los datos fila tras fila.*/
    float ** dbptBuffer;    /**< Array de punteros a puntero para acceso mediante doble referencia.*/

    QString labelX;         /**< Etiqueta para la variable independiente X*/
    float maxXValue;        /**< Mínimo valor tomado por la variable independiente X. Correspondiente al índice 0.*/
    float minXValue;        /**< Máximo valor tomado por la variable independiente X. Correspondiente al índice sizeX-1.*/
    float ratioPixelValueX; /**< Devuelve la distancia en mm que hay entre pixel y pixel en el eje X.*/

    QString labelY;         /**< Etiqueta para la variable independiente Y*/
    float maxYValue;        /**< Máximo valor tomado por la variable independiente Y. Correspondiente al índice sizeY-1.*/
    float minYValue;        /**< Mínimo valor tomado por la variable independiente Y. Correspondiente al índice 0.*/
    float ratioPixelValueY; /**< Devuelve la distancia en mm que hay entre pixel y pixel en el eje Y.*/

public:
    Data2D(float * buffer=NULL, int sizeXin=0, int sizeYin=0);
    /**< Crea un objeto del tipo Data2D.
        @param buffer Puntero a área de memoria con la información. Esta memoria pasa a ser propiedad del objeto creado.
        @param sizeXin Ancho de los datos que se están intruduciendo en el área de memoria.
        @param sizeYin Alto de los datos que se están intruduciendo en el área de memoria.*/
    Data2D(const Data2D& input);
    /**< Constructor de copia del objeto. Crea una copia del objeto pasado como argumento.
        @param input Objeto a copiar.*/
    ~Data2D();
    /**< Destructor.*/

    void SetData(float * buffer, int sizeXin, int sizeYin);
    /**< Establece los datos del objeto.
        @param buffer Puntero a área de memoria con la información. Esta memoria pasa a ser propiedad del objeto creado.
        @param sizeXin Ancho de los datos que se están intruduciendo en el área de memoria.
        @param sizeYin Alto de los datos que se están intruduciendo en el área de memoria.*/
    float ** GetBuffer();
    /**< Recupera un puntero a los datos.
        @return Array de punteros a puntero. Debe usarse sólo para lectura.*/
    int GetSizeX();
    /**< Devuelve el valor del atributo sizeX.
        @return Valor del atributo.*/
    int GetSizeY();
    /**< Devuelve el valor del atributo sizeY.
        @return Valor del atributo.*/

    void SetLabelX(QString string);
    /**< Establece el valor del atributo labelX.
        @param string Valor del atributo.*/
    QString GetLabelX();
    /**< Devuelve el valor del atributo labelZ.
        @return Valor del atributo.*/
    void SetMaxXValue(float value);
    /**< Establece el valor del atributo maxXValue.
        @param value Valor del atributo.*/
    float GetMaxXValue();
    /**< Devuelve el valor del atributo maxXValue.
        @return Valor del atributo.*/
    void SetMinXValue(float value);
    /**< Establece el valor del atributo minXValue.
        @param value Valor del atributo.*/
    float GetMinXValue();
    /**< Devuelve el valor del atributo minXValue.
        @return Valor del atributo.*/
    float GetRatioPixelValueX();
    /**< Devuelve el valor del atributo ratioPixelValueX.
        @return Valor del atributo.*/

    void SetLabelY(QString string);
    /**< Establece el valor del atributo labelY.
        @param string Valor del atributo.*/
    QString GetLabelY();
    /**< Devuelve el valor del atributo labelZ.
        @return Valor del atributo.*/
    void SetMaxYValue(float value);
    /**< Establece el valor del atributo maxYValue.
        @param value Valor del atributo.*/
    float GetMaxYValue();
    /**< Devuelve el valor del atributo maxYValue.
        @return Valor del atributo.*/
    void SetMinYValue(float value);
    /**< Establece el valor del atributo minYValue.
        @param value Valor del atributo.*/
    float GetMinYValue();
    /**< Devuelve el valor del atributo minYValue.
        @return Valor del atributo.*/
    float GetRatioPixelValueY();
    /**< Devuelve el valor del atributo ratioPixelValueY.
        @return Valor del atributo.*/


    bool SaveToFile(QString filePath);
    Data1D * GetData1D(QRect points);
    /**< Devuelve una recta con los datos desde los puntos indicados. El número de muestras de la recta se establece en proporción a la longitud del segmento.
        @param points Rectángulo que indica los puntos inicial y final de la recta, desde la esquina superior izquierda del rectángulo hasta la inferior derecha.
        @return Datos unidimensionales a lo largo de la recta indicada.*/

private:
    void SetDefaults();
    /**< Establece los valores por defecto para los atributos.*/
    void CalculateRatios();
    /**< Calcula los atributos ratioPixelValueX y ratioPixelValueY en función de los valores mínimos y máximos.*/
    void CalculateMaxMinZ();
    /**< Recorre los datos para calcular el valor máximo y mínimo de la variable dependiente.*/

};

#endif // DATA2D_H
