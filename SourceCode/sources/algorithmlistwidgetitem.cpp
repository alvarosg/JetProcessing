/**
 * @file   algorithmlistwidgetitem.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase AlgorithmListWidgetItem.
 *
 */
#include "algorithmlistwidgetitem.h"

AlgorithmListWidgetItem::AlgorithmListWidgetItem(AlgorithmType algType_in, int auxInt_in, float auxFloat_in, QRect auxRect_in):QListWidgetItem(0){
    algType=algType_in;
    auxInt=auxInt_in;
    auxFloat=auxFloat_in;
    auxRect=auxRect_in;
    this->GenerateText();
}

void AlgorithmListWidgetItem::SetAlgorithm(AlgorithmType algType_in, int auxInt_in, float auxFloat_in, QRect auxRect_in){
    algType=algType_in;
    auxInt=auxInt_in;
    auxFloat=auxFloat_in;
    auxRect=auxRect_in;
    this->GenerateText();

}

void AlgorithmListWidgetItem::GenerateText(){
    QString text;
    switch(algType){
        case ALG_INVERSE:
            text=ALG_INVERSE_LABEL;
            break;
        case ALG_RMHTILT:
            text=ALG_RMHTILT_LABEL;
            break;
        case ALG_RMVTILT:
            text=ALG_RMVTILT_LABEL;
            break;
        case ALG_SETGROUND:
            text=ALG_SETGROUND_LABEL;
            break;
        case ALG_SYMMETRIC:
            text=ALG_SYMMETRIC_LABEL;
            break;
        case ALG_OFFSET:
            text.sprintf(": %g",auxFloat);
            text.prepend(ALG_OFFSET_LABEL);
            break;
        case ALG_MULTIPLY:
            text.sprintf(" %g",auxFloat);
            text.prepend(ALG_MULTIPLY_LABEL);
            break;
        case ALG_ABELINV:
            if(auxInt!=1)
                text.sprintf(" (%i neighbours)",auxInt);
            else
                text.sprintf(" (%i neighbour)",auxInt);
            text.prepend(ALG_ABELINV_LABEL);
            break;
        case ALG_RECTANGLE:
            text.sprintf(" from %i,%i to %i,%i",auxRect.left(),auxRect.top(),auxRect.right(),auxRect.bottom());
            text.prepend(ALG_RECTANGLE_LABEL);
            break;
        case ALG_GETLINE:
            text.sprintf(" from %i,%i to %i,%i",auxRect.left(),auxRect.top(),auxRect.right(),auxRect.bottom());
            text.prepend(ALG_GETLINE_LABEL);
            break;

    }

    this->setText(text);
}
