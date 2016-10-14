/**
 * @file   algorithmsequencecontroller.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase AlgorithmSequenceController.
 *
 */
#include "algorithmsequencecontroller.h"


AlgorithmSequenceController::AlgorithmSequenceController(){
}

void AlgorithmSequenceController::CorrectAlgorithms(){
    int i=0;
    bool flagGetLine=false;
    Algorithm alg;

    while(i<this->size()){
        alg=this->at(i);
        //Si ya he seleccionado una línea, ciertos algoritmos no se pueden aplicar -> los elimino
        if(flagGetLine==true && (alg.GetAlgorithm()==ALG_GETLINE ||
                                 alg.GetAlgorithm()==ALG_RMVTILT ||
                                 alg.GetAlgorithm()==ALG_RECTANGLE )){
            this->removeAt(i);
        }
        else
            i++;
        //Si es una selección de línea, activo el flag
        if(alg.GetAlgorithm()==ALG_GETLINE)
            flagGetLine=true;
    }
}

bool AlgorithmSequenceController::IsLineSelected(int nAlg){
    int i=0;
    Algorithm alg;

    if(nAlg==-1 || nAlg > this->size()){
        nAlg=this->size();
    }

    for(i=0;i<nAlg;i++){
        alg=this->at(i);
        //Compruebo si alguno de los algoritmos es de coger línea
        if(alg.GetAlgorithm()==ALG_GETLINE){
            return true;
        }
    }
    return false;
}

bool AlgorithmSequenceController::SaveSequenceToFile(QString filePath){

    //Abro el archivo XML
    QFile fileXML(filePath);
    if (!fileXML.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)){
        return false;
    }

    //Establezco la categoría principal para el archivo
    QDomDocument doc("AlgorithmList");
    QDomElement root = doc.createElement("AlgorithmList");
    doc.appendChild(root);

    QDomElement element;
    QDomText node;

    //Añado la cuenta del número de algoritmos
    element = doc.createElement("Count");
    node = doc.createTextNode(QString::number(this->count()));
    element.appendChild(node);
    root.appendChild(element);

    QDomElement auxAlgorithm;
    QDomElement auxAlgorithmType;
    QDomElement auxAuxInt;
    QDomElement auxAuxFloat;
    QDomElement auxAuxRectX1;
    QDomElement auxAuxRectX2;
    QDomElement auxAuxRectY1;
    QDomElement auxAuxRectY2;
    QDomText auxNode;

    //Añado la lista de algoritmos, uno por uno
    int i;
    Algorithm alg;
    for(i=0;i<this->count();i++){
        alg=this->at(i);
        auxAlgorithm = doc.createElement("Algorithm");
        root.appendChild(auxAlgorithm);

        //Creo y enlazo los distintos elementos
        auxAlgorithmType = doc.createElement("AlgorithmType");
        auxNode = doc.createTextNode(QString::number(alg.GetAlgorithm()));
        auxAlgorithmType.appendChild(auxNode);
        auxAlgorithm.appendChild(auxAlgorithmType);

        auxAuxInt = doc.createElement("AuxInt");
        auxNode = doc.createTextNode(QString::number(alg.GetAuxInt()));
        auxAuxInt.appendChild(auxNode);
        auxAlgorithm.appendChild(auxAuxInt);

        auxAuxFloat = doc.createElement("AuxFloat");
        auxNode = doc.createTextNode(QString::number(alg.GetAuxFloat()));
        auxAuxFloat.appendChild(auxNode);
        auxAlgorithm.appendChild(auxAuxFloat);

        auxAuxRectX1 = doc.createElement("AuxRectX1");
        auxNode = doc.createTextNode(QString::number(alg.GetAuxRect().left()));
        auxAuxRectX1.appendChild(auxNode);
        auxAlgorithm.appendChild(auxAuxRectX1);

        auxAuxRectX2 = doc.createElement("AuxRectX2");
        auxNode = doc.createTextNode(QString::number(alg.GetAuxRect().right()));
        auxAuxRectX2.appendChild(auxNode);
        auxAlgorithm.appendChild(auxAuxRectX2);

        auxAuxRectY1 = doc.createElement("AuxRectY1");
        auxNode = doc.createTextNode(QString::number(alg.GetAuxRect().top()));
        auxAuxRectY1.appendChild(auxNode);
        auxAlgorithm.appendChild(auxAuxRectY1);

        auxAuxRectY2 = doc.createElement("AuxRectY2");
        auxNode = doc.createTextNode(QString::number(alg.GetAuxRect().bottom()));
        auxAuxRectY2.appendChild(auxNode);
        auxAlgorithm.appendChild(auxAuxRectY2);
    }


    //Guardo el archivo xml
    QIODevice * device;
    device = &fileXML;
    QTextStream out(device);
    doc.save(out,4);

    return true;

}

bool AlgorithmSequenceController::LoadSequenceFromFile(QString filePath){

    //Abro el archivo para lectura
    QFile file(filePath);
    if(file.open(QFile::ReadOnly | QFile::Text)==false)
        return false;

    QIODevice * device;
    device = &file;
    QDomDocument doc;

    doc.setContent(device,true,NULL,NULL,NULL);

    QDomElement root = doc.documentElement();
    if(root.tagName()!="AlgorithmList"){
        return false;
    }

    //Vacío la lista
    this->empty();

    //Obtengo la cuenta
    int count;
    count=root.firstChildElement("Count").text().toInt();

    //Extraigo algoritmo a algoritmo
    int i;
    Algorithm alg;
    QDomElement algorithm;
    QRect rect;
    algorithm = root.firstChildElement("Algorithm");
    for(i=0;i<count;i++){
        switch(algorithm.firstChildElement("AlgorithmType").text().toInt()){
            case ALG_INVERSE:       alg.SetAlgorithm(ALG_INVERSE);      break;
            case ALG_RMHTILT:       alg.SetAlgorithm(ALG_RMHTILT);      break;
            case ALG_RMVTILT:       alg.SetAlgorithm(ALG_RMVTILT);      break;
            case ALG_SETGROUND:     alg.SetAlgorithm(ALG_SETGROUND);    break;
            case ALG_SYMMETRIC:     alg.SetAlgorithm(ALG_SYMMETRIC);    break;
            case ALG_OFFSET:        alg.SetAlgorithm(ALG_OFFSET);       break;
            case ALG_MULTIPLY:      alg.SetAlgorithm(ALG_MULTIPLY);     break;
            case ALG_ABELINV:       alg.SetAlgorithm(ALG_ABELINV);      break;
            case ALG_RECTANGLE:     alg.SetAlgorithm(ALG_RECTANGLE);    break;
            case ALG_GETLINE:       alg.SetAlgorithm(ALG_GETLINE);      break;
        };
        alg.SetAuxInt(algorithm.firstChildElement("AuxInt").text().toInt());
        alg.SetAuxFloat(algorithm.firstChildElement("AuxFloat").text().toFloat());
        rect.setLeft(algorithm.firstChildElement("AuxRectX1").text().toInt());
        rect.setRight(algorithm.firstChildElement("AuxRectX2").text().toInt());
        rect.setTop(algorithm.firstChildElement("AuxRectY1").text().toInt());
        rect.setBottom(algorithm.firstChildElement("AuxRectY2").text().toInt());
        alg.SetAuxRect(rect);
        this->append(alg);

        algorithm = algorithm.nextSiblingElement("Algorithm");
    }

    return true;
}

QRect AlgorithmSequenceController::GetMinRectangleAbsolute(int until){
    //Tengo que crear el rectángulo absoluto juntando todos los posibles rectángulos, hasta le paso que me soliciten
    int i=0;
    Algorithm alg;
    QRect rect,auxRect;
    bool firstRectFound=false;

    //Si quieren todos los pasos, o el número es demasiado alto
    if(until==-1 || until > this->size()){
        until=this->size();
    }

    for(i=0;i<until;i++){
        alg=this->at(i);
        if(alg.GetAlgorithm()==ALG_RECTANGLE){
            //Cuando encuentro el primero, lo asigno
            if(firstRectFound==false){
                rect=alg.GetAuxRect();
                firstRectFound=true;
            }
            //Cuando voy encontrando los siguientes, voy corrigiendo los valores
            else{
                auxRect=alg.GetAuxRect();
                rect.setLeft(rect.left()+auxRect.left());
                rect.setTop(rect.top()+auxRect.top());
                rect.setRight(rect.left()+auxRect.width()-1);
                rect.setBottom(rect.top()+auxRect.height()-1);
            }
        }
    }
    if(firstRectFound==false)
        return QRect::QRect(0,0,0,0);
    else
        return rect;
}

QRect AlgorithmSequenceController::GetLineAbsolute(){

    //Tengo que crear el rectángulo absoluto juntando todos los posibles rectángulos incluyendo el de la recta
    int i=0;
    Algorithm alg;
    QRect rect,auxRect;
    bool firstRectFound=false;

    for(i=0;i<this->size();i++){
        alg=this->at(i);
        if(alg.GetAlgorithm()==ALG_RECTANGLE || alg.GetAlgorithm()==ALG_GETLINE){
            //Cuando encuentro el primero, lo asigno
            if(firstRectFound==false){
                rect=alg.GetAuxRect();
                firstRectFound=true;
            }
            //Cuando voy encontrando los siguientes, voy corrigiendo los valores
            else{
                auxRect=alg.GetAuxRect();
                rect.setLeft(rect.left()+auxRect.left());
                rect.setTop(rect.top()+auxRect.top());
                rect.setRight(rect.left()+auxRect.width()-1);
                rect.setBottom(rect.top()+auxRect.height()-1);
            }
        }
    }
    if(firstRectFound==false)
        return QRect::QRect(0,0,0,0);
    else
        return rect;
}

void AlgorithmSequenceController::ReplaceAlgorithmIndex(int index,AlgorithmType algType,int auxInt, float auxFloat,QRect auxRect){
    Algorithm alg;
    if(index < this->count() && index >=0){
        alg.SetAlgorithm(algType);
        alg.SetAuxInt(auxInt);
        alg.SetAuxFloat(auxFloat);
        alg.SetAuxRect(auxRect);
        this->replace(index,alg);
    }
}

void AlgorithmSequenceController::AddAlgorithmIndex(int index,AlgorithmType algType,int auxInt, float auxFloat,QRect auxRect){
    Algorithm alg;
    if(index <= this->count() && index >=0){
        alg.SetAlgorithm(algType);
        alg.SetAuxInt(auxInt);
        alg.SetAuxFloat(auxFloat);
        alg.SetAuxRect(auxRect);
        this->insert(index,alg);
    }
}
AlgorithmType AlgorithmSequenceController::GetAlgorithmIndex(int index){
    Algorithm alg;
    if(index < this->count() && index >=0){
        alg=this->at(index);
    }
    return alg.GetAlgorithm();
}
int AlgorithmSequenceController::GetAuxIntIndex(int index){
    Algorithm alg;
    if(index < this->count() && index >=0){
        alg=this->at(index);
    }
    return alg.GetAuxInt();
}
float AlgorithmSequenceController::GetAuxFloatIndex(int index){
    Algorithm alg;
    if(index < this->count() && index >=0){
        alg=this->at(index);
    }
    return alg.GetAuxFloat();
}
QRect AlgorithmSequenceController::GetAuxRectIndex(int index){
    Algorithm alg;
    if(index < this->count() && index >=0){
        alg=this->at(index);
    }
    return alg.GetAuxRect();
}
bool AlgorithmSequenceController::HasAuxParametersIndex(int index){
    Algorithm alg;
    if(index < this->count() && index >=0){
        alg=this->at(index);
    }
    return alg.HasAuxParameters();
}
