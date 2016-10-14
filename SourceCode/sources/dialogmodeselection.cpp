/**
 * @file   dialogmodeselection.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase DialogModeSelection.
 *
 */
#include "dialogmodeselection.h"
#include "ui_dialogmodeselection.h"

DialogModeSelection::DialogModeSelection(QWidget *parent, MainController * mainController_in) :
    QDialog(parent,Qt::CustomizeWindowHint | Qt::WindowTitleHint ),
    ui(new Ui::DialogModeSelection){
    ui->setupUi(this);
    this->setModal(true);
    this->setWindowTitle(MS_CAPTION);

    //Inicio variables
    mainController=mainController_in;


    //Configuro los widgets
    ui->labelSubtitle->setText(MS_SUBTITLE_LABEL);

    QListWidgetItem * qlistwidgetitem = ui->listOptions->item(0);
    qlistwidgetitem->setText(QApplication::translate("DialogModeSelection", MS_ITEM1_LABEL, 0, QApplication::UnicodeUTF8));

    qlistwidgetitem = ui->listOptions->item(1);
    qlistwidgetitem->setText(QApplication::translate("DialogModeSelection", MS_ITEM2_LABEL, 0, QApplication::UnicodeUTF8));

    if(mainController->GetMode()==MODE_CUDA){
        ui->listOptions->setCurrentRow(0);
    }
    else{
        ui->listOptions->setCurrentRow(1);
    }

    ui->checkDefaultSelection->setText(MS_DEFAULT_LABEL);
    ui->checkDefaultSelection->setChecked(mainController->GetDefaultMode());
    ui->buttonContinue->setText(MS_CONTINUE_LABEL);

    connect(this, SIGNAL(accepted()), this, SLOT(Finished()));
    connect(ui->buttonContinue, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->listOptions,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
}

DialogModeSelection::~DialogModeSelection(){
    delete ui;
}

void DialogModeSelection::Finished(){
    mainController->SetDefaultMode(ui->checkDefaultSelection->isChecked());

    if(ui->listOptions->currentRow()==0)
        mainController->SetMode(MODE_CUDA);
    else
        mainController->SetMode(MODE_CPU);
}

