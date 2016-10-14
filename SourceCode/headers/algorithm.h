/**
 * @file  algorithm.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Fichero que define la clase Algorithm. Clase entidad que guarda un algoritmo.
 *
 */

#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QList>
#include <QRect>
#include <QFile>
#include <QTextStream>

/** Tipos de algoritmos admitidos por el programa.*/
enum AlgorithmType {
    ALG_INVERSE,    /**< Asigna a acada valor su valor opuesto.*/
    ALG_RMHTILT,    /**< Elimina la inclinaci�n vertical de una imagen usando m�nimos cuadrados. S�lo aplicable a im�genes bidimensionales.*/
    ALG_RMVTILT,    /**< Elimina la inclinaci�n horizontal de una imagen usando m�nimos cuadrados.*/
    ALG_SETGROUND,  /**< Establece un valor m�nimo a partir de la media de los valores de las l�neas verticales de los bordes y asigna el cero a ese valor, asignando tambi�n el 0 a todo loq eu este por debajo.*/
    ALG_OFFSET,     /**< A�ade un valor a todos los puntos de la imagen.*/
    ALG_MULTIPLY,   /**< Multiplica por un valor todos los puntos de la imagen.*/
    ALG_SYMMETRIC,  /**< Simetriza horizontalmente la imagen centrando el m�ximo en el centro de la imagen.*/
    ALG_ABELINV,    /**< Realiza una Inversi�n de Abel horizontalmente sobre la imagen. La imagen debe ser horizontalmente sim�trica*/
    ALG_RECTANGLE,  /**< Extrae una parte rectangular del interior de una im�gen*/
    ALG_GETLINE     /**< Extrae una l�nea de una imagen, dados los puntos inicial y final.*/
};

/*! \class Algorithm algorithm.h "algorithm.h"
 * \brief Clase que contiene la informaci�n acerca de un algoritmo. Es una clase entidad.
 *
 */


class Algorithm {
private:
    AlgorithmType algorithm; /**< Tipo de algoritmo.*/
    int auxInt;              /**< Entero auxiliar. Utilizado por la Inversi�n de Abel para indicar el n�mero de vecinos sobre los que realizar la derivada.*/
    float auxFloat;          /**< Decimal auxiliar. Utilizado por el algoritmo de Offset, y por el de Multiplicaci�n.*/
    QRect auxRect;           /**< Rect�ngulo auxiliar. Utilizado por el algoritmo de extraer rect�ngulo y de extraer recta.*/

public:
    Algorithm();
    /**< Constructor.*/
    void SetAlgorithm(AlgorithmType value);
    /**< Establece el algoritmo.
      @param value Algoritmo a establecer.*/
    AlgorithmType GetAlgorithm();
    /**< Devuelve el algoritmo.
      @return Tipo de algoritmo.*/
    void SetAuxInt(int value);
    /**< Establece el entero auxiliar.
      @param value Entero a establecer.*/
    int GetAuxInt();
    /**< Devuelve el entero auxiliar.
      @return Entero auxiliar.*/
    void SetAuxFloat(float value);
    /**< Establece el decimal auxiliar.
      @param value Decimal a establecer.*/
    float GetAuxFloat();
    /**< Devuelve el decimal auxiliar.
      @return Decimal auxiliar.*/
    void SetAuxRect(QRect value);
    /**< Establece el rect�ngulo auxiliar.
      @param value Rect�ngulo a establecer.*/
    QRect GetAuxRect();
    /**< Devuelve el rect�ngulo auxiliar.
      @return Rect�ngulo auxiliar.*/
    bool HasAuxParameters();
    /**< Indica si el tipo de algoritmo asociado utiliza par�metros auxiliares.
      @return Verdadero si utiliza par�metros auxiliares y falso si no los utiliza.*/
};

#endif // ALGORITHM_H
