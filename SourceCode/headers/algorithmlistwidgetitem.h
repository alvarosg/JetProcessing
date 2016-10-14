/**
 * @file   algorithmlistwidgetitem.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase para cada elemento de la lista del algoritmos en la interfaz gráfica.
 *
 */

#ifndef ALGORITHMLISTWIDGETITEM_H
#define ALGORITHMLISTWIDGETITEM_H

#define ALG_INVERSE_LABEL         "Invert"                  /**< Etiqueta.*/
#define ALG_RMVTILT_LABEL         "Remove Vertical Tilt"    /**< Etiqueta.*/
#define ALG_RMHTILT_LABEL         "Remove Horizontal Tilt"  /**< Etiqueta.*/
#define ALG_SETGROUND_LABEL       "Set Ground"              /**< Etiqueta.*/
#define ALG_OFFSET_LABEL          "Add Offset"              /**< Etiqueta.*/
#define ALG_MULTIPLY_LABEL        "Multiply by"             /**< Etiqueta.*/
#define ALG_SYMMETRIC_LABEL       "Make Symmetric"          /**< Etiqueta.*/
#define ALG_ABELINV_LABEL         "Abel Inversion"          /**< Etiqueta.*/
#define ALG_GETLINE_LABEL         "Extract Line"            /**< Etiqueta.*/
#define ALG_RECTANGLE_LABEL       "Extract Rectangle"       /**< Etiqueta.*/

#include <QListWidgetItem>
#include <algorithmsequencecontroller.h>

/*! \class AlgorithmListWidgetItem algorithmlistwidgetitem.h "algorithmlistwidgetitem.h"
 * \brief Clase abstracta para todo tipo de representaciones gráficas.
 *
 *  Clase para cada elemento de la lista del algoritmos en la interfaz gráfica.
 *
 */


class AlgorithmListWidgetItem : public QListWidgetItem
{
private:
    AlgorithmType algType;  /**< Tipo de algoritmo representado por el item.*/
    int auxInt;             /**< Entero auxiliar del algoritmo.*/
    float auxFloat;         /**< Decimal auxiliar del algoritmo.*/
    QRect auxRect;          /**< Rectángulo auxiliar del algoritmo.*/
public:
    AlgorithmListWidgetItem(AlgorithmType algType_in,int auxInt_in,float auxFloat_in,QRect auxRect_in);
    /**< Constructor del ítem a partir de los datos de un algoritmo que se le pasa como parámetro.
        @param algType_in Tipo de algoritmo que será representado por el ítem.
        @param auxInt_in Entero auxiliar del algoritmo.
        @param auxFloat_in Decimal auxiliar del algoritmo.
        @param auxRect_in Rectángulo auxiliar del algoritmo.*/
    void SetAlgorithm(AlgorithmType algType_in,int auxInt_in,float auxFloat_in,QRect auxRect_in);
    /**< Función que cambia el algoritmo representado por un ítem.
        @param algType_in Tipo de algoritmo que será representado por el ítem.
        @param auxInt_in Entero auxiliar del algoritmo.
        @param auxFloat_in Decimal auxiliar del algoritmo.
        @param auxRect_in Rectángulo auxiliar del algoritmo.*/
    void GenerateText();
    /**< Función que asigna al ítem una etiqueta generada a partir del nombre del algoritmo y los parámetros que correspondan a cada tipo de algoritmo.*/
};

#endif // ALGORITHMLISTWIDGETITEM_H
