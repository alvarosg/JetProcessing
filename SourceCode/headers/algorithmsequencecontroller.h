/**
 * @file  algorithmsequencecontroller.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Fichero que define la clase que controla y gestiona la lista de algoritmos que se deben aplicar al procesar la imagen.
 *
 */

#ifndef ALGORITHMSEQUENCECONTROLLER_H
#define ALGORITHMSEQUENCECONTROLLER_H

#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include "algorithm.h"


/*! \class AlgorithmSequenceController algorithmsequencecontroller.h "algorithmsequencecontroller.h"
 * \brief Clase que controla y conoce una lista de algoritmos.
 *
 *  Se encargará de comprobar la integridad de los algoritmos, asi como de recuperar el algoritmo que se le solicite, o datos acerca de ellos.
 *
 */

class AlgorithmSequenceController : public QList <Algorithm>
{
public:
    AlgorithmSequenceController();
    /**< Constructor.*/
    void CorrectAlgorithms();
    /**< Comprueba si el orden de los algoritmos es coherente y elimina los algoritmos mal posicionados*/
    bool IsLineSelected(int nAlg=-1);
    /**< Indica si hasta cierta posición de la lista de algoritmos, hay alguno de selección de línea.
      @param nAlg Número de algoritmos que se van a comprobar. Para comprobar toda la lista introducir -1.
      @return Verdadero, si algún algoritmo dentro de los comprobados es del tipo selección de línea.*/
    bool SaveSequenceToFile(QString filePath);
    /**< Guarda la lista de algoritmos en un fichero.
      @param filePath ruta y nombre del fichero donde se deben guardar.
      @return Verdadero si se guardó corectamente.*/
    bool LoadSequenceFromFile(QString filePath);
    /**< Carga la lista de algoritmos de un fichero.
      @param filePath ruta y nombre del fichero de donde se deben leer.
      @return Verdadero si se cargó correctamente.*/
    QRect GetMinRectangleAbsolute(int until=-1);
    /**< Devuelve la posición del último rectángulo extraido hasta el algoritmo indicado de forma absoluta.
      @param until Número de algoritmos que se van a comprobar. Para comprobar toda la lista introducir -1.
      @return Rectángulo más pequeño en coordenadas absolutas respecto del la imagen completa, o bien QRect(0,0,0,0) si ningún rectángulo fue seleccionado.*/
    QRect GetLineAbsolute();
    /**< Devuelve la posición de la línea seleccionada entre los algoritmos.
      @return Posición absoluta de la línea respecto a la imagen completa, o bien QRect(0,0,0,0) si ningúna línea fue seleccionado.*/
    void ReplaceAlgorithmIndex(int index,AlgorithmType algType,int auxInt, float auxFloat,QRect auxRect);
    /**< Sustituye el algortimo en una posición.
      @param index Posición del algoritmo a reemplazar.
      @param algType Algoritmo a establecer.
      @param auxInt Entero auxiliar a establecer.
      @param auxFloat Decimal auxiliar a establecer.
      @param auxRect Rectángulo auxiliar a establecer.*/
    void AddAlgorithmIndex(int index,AlgorithmType algType,int auxInt, float auxFloat,QRect auxRect);
    /**< Añade un algoritmo en una posición.
      @param index Posición en la que se debe añadir el algoritmo.
      @param algType Algoritmo a establecer.
      @param auxInt Entero auxiliar a establecer.
      @param auxFloat Decimal auxiliar a establecer.
      @param auxRect Rectángulo auxiliar a establecer.*/
    AlgorithmType GetAlgorithmIndex(int index);
    /**< Devuelve el tipo de algoritmo en la posición indicada.
      @return Tipo de algoritmo.*/
    int GetAuxIntIndex(int index);
    /**< Devuelve el entero auxiliar del algoritmo en la posición indicada.
      @param index Posición del algoritmo.
      @return Entero auxiliar.*/
    float GetAuxFloatIndex(int index);
    /**< Devuelve el decimal auxiliar del algoritmo en la posición indicada.
      @param index Posición del algoritmo.
      @return Decimal auxiliar.*/
    QRect GetAuxRectIndex(int index);
    /**< Devuelve el rectángulo auxiliar del algoritmo en la posición indicada.
      @param index Posición del algoritmo.
      @return Rectángulo auxiliar.*/
    bool HasAuxParametersIndex(int index);
    /**< Indica si el tipo de algoritmo en la posición indicada utiliza parámetros auxiliares.
      @param index Posición del algoritmo.
      @return Verdadero si utiliza parámetros auxiliares y falso si no los utiliza.*/
};

#endif // ALGORITHMSEQUENCECONTROLLER_H
