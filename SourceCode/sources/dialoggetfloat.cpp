/**
 * @file   dialoggetfloat.cpp
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de la clase DialogGetFloat.
 *
 */
#include "dialoggetfloat.h"
#include "ui_dialoggetfloat.h"

DialogGetFloat::DialogGetFloat(QWidget *parent, float * number_in, bool multiply) :
    QDialog(parent,Qt::CustomizeWindowHint | Qt::WindowTitleHint ),
    ui(new Ui::DialogGetFloat){
    ui->setupUi(this);

    this->setModal(true);
    if(multiply==false){
        this->setWindowTitle(GF_CAPTION_OFFSET);
        ui->labelFloat->setText(GF_OFFSET);
    }
    else{
        this->setWindowTitle(GF_CAPTION_MULTIPLY);
        ui->labelFloat->setText(GF_MULTIPLY);
    }

    number=number_in;
    QString auxStr;
    auxStr.setNum(*number,'g',3);
    ui->editFloat->setText(auxStr);

    connect(ui->buttonBox, SIGNAL(accepted()),this,SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()),this,SLOT(reject()));
}

DialogGetFloat::~DialogGetFloat(){
    *number=ui->editFloat->text().toFloat();
    delete ui;
}
