/**
 * @file   algorithmlistwidgetitem.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Clase para cada elemento de la lista del algoritmos en la interfaz gr�fica.
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
 * \brief Clase abstracta para todo tipo de representaciones gr�ficas.
 *
 *  Clase para cada elemento de la lista del algoritmos en la interfaz gr�fica.
 *
 */


class AlgorithmListWidgetItem : public QListWidgetItem
{
private:
    AlgorithmType algType;  /**< Tipo de algoritmo representado por el item.*/
    int auxInt;             /**< Entero auxiliar del algoritmo.*/
    float auxFloat;         /**< Decimal auxiliar del algoritmo.*/
    QRect auxRect;          /**< Rect�ngulo auxiliar del algoritmo.*/
public:
    AlgorithmListWidgetItem(AlgorithmType algType_in,int auxInt_in,float auxFloat_in,QRect auxRect_in);
    /**< Constructor del �tem a partir de los datos de un algoritmo que se le pasa como par�metro.
        @param algType_in Tipo de algoritmo que ser� representado por el �tem.
        @param auxInt_in Entero auxiliar del algoritmo.
        @param auxFloat_in Decimal auxiliar del algoritmo.
        @param auxRect_in Rect�ngulo auxiliar del algoritmo.*/
    void SetAlgorithm(AlgorithmType algType_in,int auxInt_in,float auxFloat_in,QRect auxRect_in);
    /**< Funci�n que cambia el algoritmo representado por un �tem.
        @param algType_in Tipo de algoritmo que ser� representado por el �tem.
        @param auxInt_in Entero auxiliar del algoritmo.
        @param auxFloat_in Decimal auxiliar del algoritmo.
        @param auxRect_in Rect�ngulo auxiliar del algoritmo.*/
    void GenerateText();
    /**< Funci�n que asigna al �tem una etiqueta generada a partir del nombre del algoritmo y los par�metros que correspondan a cada tipo de algoritmo.*/
};

#endif // ALGORITHMLISTWIDGETITEM_H
