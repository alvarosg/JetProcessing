/**
 * @file  algorithmsequencecontroller.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
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
 *  Se encargar� de comprobar la integridad de los algoritmos, asi como de recuperar el algoritmo que se le solicite, o datos acerca de ellos.
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
    /**< Indica si hasta cierta posici�n de la lista de algoritmos, hay alguno de selecci�n de l�nea.
      @param nAlg N�mero de algoritmos que se van a comprobar. Para comprobar toda la lista introducir -1.
      @return Verdadero, si alg�n algoritmo dentro de los comprobados es del tipo selecci�n de l�nea.*/
    bool SaveSequenceToFile(QString filePath);
    /**< Guarda la lista de algoritmos en un fichero.
      @param filePath ruta y nombre del fichero donde se deben guardar.
      @return Verdadero si se guard� corectamente.*/
    bool LoadSequenceFromFile(QString filePath);
    /**< Carga la lista de algoritmos de un fichero.
      @param filePath ruta y nombre del fichero de donde se deben leer.
      @return Verdadero si se carg� correctamente.*/
    QRect GetMinRectangleAbsolute(int until=-1);
    /**< Devuelve la posici�n del �ltimo rect�ngulo extraido hasta el algoritmo indicado de forma absoluta.
      @param until N�mero de algoritmos que se van a comprobar. Para comprobar toda la lista introducir -1.
      @return Rect�ngulo m�s peque�o en coordenadas absolutas respecto del la imagen completa, o bien QRect(0,0,0,0) si ning�n rect�ngulo fue seleccionado.*/
    QRect GetLineAbsolute();
    /**< Devuelve la posici�n de la l�nea seleccionada entre los algoritmos.
      @return Posici�n absoluta de la l�nea respecto a la imagen completa, o bien QRect(0,0,0,0) si ning�na l�nea fue seleccionado.*/
    void ReplaceAlgorithmIndex(int index,AlgorithmType algType,int auxInt, float auxFloat,QRect auxRect);
    /**< Sustituye el algortimo en una posici�n.
      @param index Posici�n del algoritmo a reemplazar.
      @param algType Algoritmo a establecer.
      @param auxInt Entero auxiliar a establecer.
      @param auxFloat Decimal auxiliar a establecer.
      @param auxRect Rect�ngulo auxiliar a establecer.*/
    void AddAlgorithmIndex(int index,AlgorithmType algType,int auxInt, float auxFloat,QRect auxRect);
    /**< A�ade un algoritmo en una posici�n.
      @param index Posici�n en la que se debe a�adir el algoritmo.
      @param algType Algoritmo a establecer.
      @param auxInt Entero auxiliar a establecer.
      @param auxFloat Decimal auxiliar a establecer.
      @param auxRect Rect�ngulo auxiliar a establecer.*/
    AlgorithmType GetAlgorithmIndex(int index);
    /**< Devuelve el tipo de algoritmo en la posici�n indicada.
      @return Tipo de algoritmo.*/
    int GetAuxIntIndex(int index);
    /**< Devuelve el entero auxiliar del algoritmo en la posici�n indicada.
      @param index Posici�n del algoritmo.
      @return Entero auxiliar.*/
    float GetAuxFloatIndex(int index);
    /**< Devuelve el decimal auxiliar del algoritmo en la posici�n indicada.
      @param index Posici�n del algoritmo.
      @return Decimal auxiliar.*/
    QRect GetAuxRectIndex(int index);
    /**< Devuelve el rect�ngulo auxiliar del algoritmo en la posici�n indicada.
      @param index Posici�n del algoritmo.
      @return Rect�ngulo auxiliar.*/
    bool HasAuxParametersIndex(int index);
    /**< Indica si el tipo de algoritmo en la posici�n indicada utiliza par�metros auxiliares.
      @param index Posici�n del algoritmo.
      @return Verdadero si utiliza par�metros auxiliares y falso si no los utiliza.*/
};

#endif // ALGORITHMSEQUENCECONTROLLER_H
