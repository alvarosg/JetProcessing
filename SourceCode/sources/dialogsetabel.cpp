/**
 * @file   dialogsetabel.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase DialogSetAbel.
 *
 */
#include "dialogsetabel.h"
#include "ui_dialogsetabel.h"

DialogSetAbel::DialogSetAbel(QWidget *parent,int * neighbours_in) :
    QDialog(parent,Qt::CustomizeWindowHint | Qt::WindowTitleHint ),
    ui(new Ui::DialogSetAbel){
    ui->setupUi(this);

    this->setModal(true);
    this->setWindowTitle(SA_CAPTION);


    ui->labelAbel->setText(SA_NEIGHBOURS);
    neighbours=neighbours_in;
    ui->spinNeighbours->setValue(*neighbours);

    connect(ui->buttonBox, SIGNAL(accepted()),this,SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()),this,SLOT(reject()));
}

DialogSetAbel::~DialogSetAbel(){
    *neighbours=ui->spinNeighbours->value();
    delete ui;
}
