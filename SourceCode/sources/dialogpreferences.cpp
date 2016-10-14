/**
 * @file   dialogpreferences.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase DialogPreferences.
 *
 */
#include "dialogpreferences.h"
#include "ui_dialogpreferences.h"

DialogPreferences::DialogPreferences(QWidget *parent,ProgramSettings * programSettings_in) :
    QDialog(parent,Qt::CustomizeWindowHint | Qt::WindowTitleHint ),
    ui(new Ui::DialogPreferences){
    ui->setupUi(this);

    this->setModal(true);
    this->setWindowTitle(PF_CAPTION);
    programSettings=programSettings_in;

    ui->checkSaveCameraPictures->setChecked(programSettings->GetSaveCameraPictures());
    ui->checkUntilSelectedAlg->setChecked(programSettings->GetApplyUntilCurrentAlgorithm());
    ui->checkViewSize->setChecked(programSettings->GetSaveGraphicsViewSize());
    ui->checkModeDialog->setChecked(!programSettings->GetUseDefaultMode());

    ui->spinWidth->setValue(programSettings->GetCustomSize().width());
    ui->spinHeight->setValue(programSettings->GetCustomSize().height());

    ui->comboFormat->addItem(PF_FORMATPNG);
    ui->comboFormat->addItem(PF_FORMATJPG);
    ui->comboFormat->addItem(PF_FORMATBMP);

    QString format=programSettings->GetDefaultImageFormat();
    if(format=="png")
        ui->comboFormat->setCurrentIndex(0);
    else if(format=="jpg")
        ui->comboFormat->setCurrentIndex(1);
    else if(format=="bmp")
        ui->comboFormat->setCurrentIndex(2);


    connect(this, SIGNAL(accepted()), this, SLOT(Finished()));
    connect(ui->buttonBox, SIGNAL(accepted()),this,SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()),this,SLOT(reject()));
    connect(ui->checkViewSize,SIGNAL(clicked()),this,SLOT(UpdateSizesEnabled()));


    ui->checkSaveCameraPictures->setText(PF_SAVECAMPICTURES);
    ui->checkUntilSelectedAlg->setText(PF_UNTILSELECTEDALG);
    ui->checkViewSize->setText(PF_VIEWSIZE);
    ui->checkModeDialog->setText(PF_MODEDIALOG);
    ui->labelWidth->setText(PF_WIDTH);
    ui->labelHeight->setText(PF_HEIGHT);
    ui->labelFormat->setText(PF_FORMAT);

    this->UpdateSizesEnabled();

}

void DialogPreferences::UpdateSizesEnabled(){
    if(ui->checkViewSize->isChecked()){
        ui->labelWidth->setVisible(false);
        ui->labelHeight->setVisible(false);
        ui->spinWidth->setVisible(false);
        ui->spinHeight->setVisible(false);
    }
    else{
        ui->labelWidth->setVisible(true);
        ui->labelHeight->setVisible(true);
        ui->spinWidth->setVisible(true);
        ui->spinHeight->setVisible(true);
    }
}

DialogPreferences::~DialogPreferences(){
    delete ui;
}

void DialogPreferences::Finished(){
    programSettings->SetSaveCameraPictures(ui->checkSaveCameraPictures->isChecked());
    programSettings->SetApplyUntilCurrentAlgorithm(ui->checkUntilSelectedAlg->isChecked());
    programSettings->SetSaveGraphicsViewSize(ui->checkViewSize->isChecked());
    programSettings->SetUseDefaultMode(!(ui->checkModeDialog->isChecked()));
    if(programSettings->GetSaveGraphicsViewSize()==false){
        QSize size;
        size.setWidth(ui->spinWidth->value());
        size.setHeight(ui->spinHeight->value());
        programSettings->SetCustomSize(size);
    }

    switch(ui->comboFormat->currentIndex()){
        case 0: programSettings->SetDefaultImageFormat("png"); break;
        case 1: programSettings->SetDefaultImageFormat("jpg"); break;
        case 2: programSettings->SetDefaultImageFormat("bmp"); break;
    }
}
