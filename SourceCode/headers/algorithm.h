/**
 * @file  algorithm.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
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
    ALG_RMHTILT,    /**< Elimina la inclinación vertical de una imagen usando mínimos cuadrados. Sólo aplicable a imágenes bidimensionales.*/
    ALG_RMVTILT,    /**< Elimina la inclinación horizontal de una imagen usando mínimos cuadrados.*/
    ALG_SETGROUND,  /**< Establece un valor mínimo a partir de la media de los valores de las líneas verticales de los bordes y asigna el cero a ese valor, asignando también el 0 a todo loq eu este por debajo.*/
    ALG_OFFSET,     /**< Añade un valor a todos los puntos de la imagen.*/
    ALG_MULTIPLY,   /**< Multiplica por un valor todos los puntos de la imagen.*/
    ALG_SYMMETRIC,  /**< Simetriza horizontalmente la imagen centrando el máximo en el centro de la imagen.*/
    ALG_ABELINV,    /**< Realiza una Inversión de Abel horizontalmente sobre la imagen. La imagen debe ser horizontalmente simétrica*/
    ALG_RECTANGLE,  /**< Extrae una parte rectangular del interior de una imágen*/
    ALG_GETLINE     /**< Extrae una línea de una imagen, dados los puntos inicial y final.*/
};

/*! \class Algorithm algorithm.h "algorithm.h"
 * \brief Clase que contiene la información acerca de un algoritmo. Es una clase entidad.
 *
 */


class Algorithm {
private:
    AlgorithmType algorithm; /**< Tipo de algoritmo.*/
    int auxInt;              /**< Entero auxiliar. Utilizado por la Inversión de Abel para indicar el número de vecinos sobre los que realizar la derivada.*/
    float auxFloat;          /**< Decimal auxiliar. Utilizado por el algoritmo de Offset, y por el de Multiplicación.*/
    QRect auxRect;           /**< Rectángulo auxiliar. Utilizado por el algoritmo de extraer rectángulo y de extraer recta.*/

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
    /**< Establece el rectángulo auxiliar.
      @param value Rectángulo a establecer.*/
    QRect GetAuxRect();
    /**< Devuelve el rectángulo auxiliar.
      @return Rectángulo auxiliar.*/
    bool HasAuxParameters();
    /**< Indica si el tipo de algoritmo asociado utiliza parámetros auxiliares.
      @return Verdadero si utiliza parámetros auxiliares y falso si no los utiliza.*/
};

#endif // ALGORITHM_H
