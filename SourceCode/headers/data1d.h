/**
 * @file   data1d.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase que que contiene una informaci�n de fase unidimensional.
 *
 */

#ifndef DATA1D_H
#define DATA1D_H

#include <stdlib.h>
#include <QFile>
#include <QTextStream>
#include <dataresult.h>

/*! \class Data1D data1d.h "data1d.h"
 * \brief Clase que que contiene una informaci�n de fase unidimensional.
 *
 */

class Data1D: public DataResult{
private:
    int sizeX;              /**< Longitud en muestras de la recta.*/
    float * linearBuffer;   /**< Puntero que apunta a la zona de memoria donde se encuentran los datos*/

    QString labelX;         /**< Etiqueta para la variable independiente*/
    float maxXValue;        /**< M�nimo valor tomado por la variable independiente. Correspondiente al �ndice sizeX-1.*/
    float minXValue;        /**< M�ximo valor tomado por la variable independiente. Correspondiente al �ndice 0.*/
    float ratioPixelValueX; /**< Devuelve la distancia en mm que hay entre pixel y pixel en el eje X.*/

    float initXPoint;       /**< Punto inicial horizontal en unidades de eje X donde comenzaba la recta extra�da para los datos*/
    float initYPoint;       /**< Punto inicial vertical en unidades de eje Y donde comenzaba la recta extra�da para los datos*/
    float endXPoint;        /**< Punto final horizontal en unidades de eje X donde acababa la recta extra�da para los datos*/
    float endYPoint;        /**< Punto final vertical en unidades de eje Y donde acababa la recta extra�da para los datos*/

public:
    Data1D(float * buffer=NULL, int sizeXin=0);
    /**< Crea un objeto del tipo Data1D.
        @param buffer Puntero a �rea de memoria con la informaci�n. Esta memoria pasa a ser propiedad del objeto creado.
        @param sizeXin N�mero de muestras de la informaci�n.*/
    Data1D(const Data1D& input);
    /**< Constructor de copia del objeto. Crea una copia del objeto pasado como argumento.
            @param input Objeto a copiar.*/
    ~Data1D();
    /**< Destructor.*/

    void SetData(float * buffer, int sizeXin);
    /**< Establece los datos del objeto.
        @param buffer Puntero a �rea de memoria con la informaci�n. Esta memoria pasa a ser propiedad del objeto creado.
        @param sizeXin N�mero de muestras de la informaci�n.*/
    float * GetBuffer();
    /**< Recupera un puntero a los datos.
        @return Puntero a la zona de memoria con los datos. Debe usarse s�lo para lectura.*/
    int GetSizeX();
    /**< Devuelve el valor del atributo sizeX.
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

    float GetInitXPoint();
    /**< Devuelve el valor del atributo initXPoint.
        @return Valor del atributo.*/
    void SetInitXPoint(float value);
    /**< Establece el valor del atributo initXPoint.
        @param value Valor del atributo.*/
    float GetInitYPoint();
    /**< Devuelve el valor del atributo initYPoint.
        @return Valor del atributo.*/
    void SetInitYPoint(float value);
    /**< Establece el valor del atributo initYPoint.
        @param value Valor del atributo.*/
    float GetEndXPoint();
    /**< Devuelve el valor del atributo endXPoint.
        @return Valor del atributo.*/
    void SetEndXPoint(float value);
    /**< Establece el valor del atributo endXPoint.
        @param value Valor del atributo.*/
    float GetEndYPoint();
    /**< Devuelve el valor del atributo endYPoint.
        @return Valor del atributo.*/
    void SetEndYPoint(float value);
    /**< Establece el valor del atributo endYPoint.
        @param value Valor del atributo.*/

    bool SaveToFile(QString filePath);

private:
    void SetDefaults();
    /**< Establece los valores por defecto para los atributos.*/
    void CalculateRatios();
    /**< Calcula el atributo ratioPixelValueX en funci�n de los valores m�nimos y m�ximos.*/
    void CalculateMaxMinZ();
    /**< Recorre los datos para calcular el valor m�ximo y m�nimo de la variable dependiente.*/
};

#endif // DATA1D_H
