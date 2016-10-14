/**
 * @file   dialoghelp.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase DialogHelp.
 *
 */
#include "dialoghelp.h"
#include "ui_dialoghelp.h"

DialogHelp::DialogHelp(QWidget *parent,QString path,QString homePage) :
    QDialog(parent,Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint),
    ui(new Ui::DialogHelp){
    ui->setupUi(this);
    this->setModal(true);
    this->setWindowTitle(HLP_CAPTION);
    this->setMinimumSize(700,500);

    homePagePath=path+"/"+homePage;

    connect(ui->buttonBack,SIGNAL(clicked()),ui->webView,SLOT(back()));
    connect(ui->buttonForward,SIGNAL(clicked()),ui->webView,SLOT(forward()));
    connect(ui->buttonHome,SIGNAL(clicked()),this,SLOT(GoHome()));


    connect(ui->webView,SIGNAL(loadFinished(bool)),this,SLOT(SetButtonsState()));

    this->GoHome();
    this->SetButtonsState();
}

DialogHelp::~DialogHelp(){
    delete ui;
}


void DialogHelp::GoHome(){
    ui->webView->load(QUrl::fromLocalFile(homePagePath));
}

void DialogHelp::SetButtonsState(){
    //Si el primer item del historial es el vacío, lo elimino
    if(ui->webView->history()->itemAt(0).url()==QUrl( "about:blank" ))
        ui->webView->history()->clear();

    //ui->buttonBack->setEnabled(ui->webView->history()->canGoBack());
    //ui->buttonForward->setEnabled(ui->webView->history()->canGoForward());
}
