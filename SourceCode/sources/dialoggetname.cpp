/**
 * @file   dialoggetname.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase DialogGetName.
 *
 */
#include "dialoggetname.h"
#include "ui_dialoggetname.h"

DialogGetName::DialogGetName(QWidget *parent,QString * name_in) :
    QDialog(parent,Qt::CustomizeWindowHint | Qt::WindowTitleHint ),
    ui(new Ui::DialogGetName){
    ui->setupUi(this);
    this->setModal(true);
    this->setWindowTitle(GN_CAPTION);
    name=name_in;


    ui->labelSeries->setText(GN_LABEL);
    ui->editSeries->setText(*name);


    connect(ui->buttonBox, SIGNAL(accepted()),this,SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()),this,SLOT(reject()));
}

DialogGetName::~DialogGetName(){
    *name=ui->editSeries->text();
    delete ui;
}
