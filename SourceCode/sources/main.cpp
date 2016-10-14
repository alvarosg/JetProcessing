/**
 * @file   main.cpp
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Punto de entrada al programa.
 *
 */
#include <QtGui/QApplication>
#include <qdebug>
#include "mainwindow.h"
#include "maincontroller.h"


int main(int argc, char *argv[]){
    //Creo la aplicaci�n
    QApplication jetProcessing(argc, argv);
    //Creo el controlador principal
    MainController mainController;
    //Creo la ventana principal
    MainWindow w(NULL,&mainController);
    //Entro al bucle de eventos
    return jetProcessing.exec();
}
/**< Punto de entrada al programa.
    @param argc N�mero de argumentos recibidos.
    @param argv Array de punteros a char con los argumentos recibidos.
    @return Valor de retorno al sistema operativo.*/
