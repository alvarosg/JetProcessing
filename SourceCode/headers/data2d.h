/**
 * @file   data2d.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
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
 * \brief Clase que que contiene una informaci�n de fase bidimensional.
 *
 */

class Data2D: public DataResult{

private:
    int sizeX;              /**< Ancho en pixeles de la imagen de la fase.*/
    int sizeY;              /**< Alto en pixeles de la imagen de la fase.*/
    float * linearBuffer;   /**< Puntero que apunta a la zona de memoria donde se encuentran los datos fila tras fila.*/
    float ** dbptBuffer;    /**< Array de punteros a puntero para acceso mediante doble referencia.*/

    QString labelX;         /**< Etiqueta para la variable independiente X*/
    float maxXValue;        /**< M�nimo valor tomado por la variable independiente X. Correspondiente al �ndice 0.*/
    float minXValue;        /**< M�ximo valor tomado por la variable independiente X. Correspondiente al �ndice sizeX-1.*/
    float ratioPixelValueX; /**< Devuelve la distancia en mm que hay entre pixel y pixel en el eje X.*/

    QString labelY;         /**< Etiqueta para la variable independiente Y*/
    float maxYValue;        /**< M�ximo valor tomado por la variable independiente Y. Correspondiente al �ndice sizeY-1.*/
    float minYValue;        /**< M�nimo valor tomado por la variable independiente Y. Correspondiente al �ndice 0.*/
    float ratioPixelValueY; /**< Devuelve la distancia en mm que hay entre pixel y pixel en el eje Y.*/

public:
    Data2D(float * buffer=NULL, int sizeXin=0, int sizeYin=0);
    /**< Crea un objeto del tipo Data2D.
        @param buffer Puntero a �rea de memoria con la informaci�n. Esta memoria pasa a ser propiedad del objeto creado.
        @param sizeXin Ancho de los datos que se est�n intruduciendo en el �rea de memoria.
        @param sizeYin Alto de los datos que se est�n intruduciendo en el �rea de memoria.*/
    Data2D(const Data2D& input);
    /**< Constructor de copia del objeto. Crea una copia del objeto pasado como argumento.
        @param input Objeto a copiar.*/
    ~Data2D();
    /**< Destructor.*/

    void SetData(float * buffer, int sizeXin, int sizeYin);
    /**< Establece los datos del objeto.
        @param buffer Puntero a �rea de memoria con la informaci�n. Esta memoria pasa a ser propiedad del objeto creado.
        @param sizeXin Ancho de los datos que se est�n intruduciendo en el �rea de memoria.
        @param sizeYin Alto de los datos que se est�n intruduciendo en el �rea de memoria.*/
    float ** GetBuffer();
    /**< Recupera un puntero a los datos.
        @return Array de punteros a puntero. Debe usarse s�lo para lectura.*/
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
    /**< Devuelve una recta con los datos desde los puntos indicados. El n�mero de muestras de la recta se establece en proporci�n a la longitud del segmento.
        @param points Rect�ngulo que indica los puntos inicial y final de la recta, desde la esquina superior izquierda del rect�ngulo hasta la inferior derecha.
        @return Datos unidimensionales a lo largo de la recta indicada.*/

private:
    void SetDefaults();
    /**< Establece los valores por defecto para los atributos.*/
    void CalculateRatios();
    /**< Calcula los atributos ratioPixelValueX y ratioPixelValueY en funci�n de los valores m�nimos y m�ximos.*/
    void CalculateMaxMinZ();
    /**< Recorre los datos para calcular el valor m�ximo y m�nimo de la variable dependiente.*/

};

#endif // DATA2D_H
