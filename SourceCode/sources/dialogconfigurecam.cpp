/**
 * @file   dialogconfigurecam.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase DialogConfigureCam.
 *
 */
#include "dialogconfigurecam.h"
#include "ui_dialogconfigurecam.h"

DialogConfigureCam::DialogConfigureCam(QWidget *parent,CameraController * cameraController_in) :
    QDialog(parent,Qt::CustomizeWindowHint | Qt::WindowTitleHint),ui(new Ui::DialogConfigureCam){
    ui->setupUi(this);

    cameraController=cameraController_in;
    this->setModal(true);
    this->setWindowTitle(CC_CAPTION);

    //Inicio los controles
    this->InitControls();

    //Cojo los valores iniciales para el caso en el que se quiera cancelar
    initValues=cameraController->GetParameters();

    //Coloco los controles
    this->UpdateControls();

    //Tomo una vista previa
    this->UpdatePreview();

    //Conecto todas las señales con los slots
    connect(ui->buttonLoadConfig, SIGNAL(clicked()),this, SLOT(LoadConfig()));
    connect(ui->buttonSaveConfig, SIGNAL(clicked()),this, SLOT(SaveConfig()));
    connect(ui->buttonResetDefaults, SIGNAL(clicked()),this, SLOT(ResetDefaults()));

    connect(ui->sliderPixelClock, SIGNAL(valueChanged(int)),this, SLOT(PixelClockChanged(int)));
    connect(ui->sliderFrameRate, SIGNAL(valueChanged(int)),this, SLOT(FrameRateChanged(int)));
    connect(ui->sliderExposure, SIGNAL(valueChanged(int)),this, SLOT(ExposureChanged(int)));
    connect(ui->sliderMasterGain, SIGNAL(valueChanged(int)),this, SLOT(MasterGainChanged(int)));
    connect(ui->sliderRedGain, SIGNAL(valueChanged(int)),this, SLOT(RedGainChanged(int)));
    connect(ui->sliderGreenGain, SIGNAL(valueChanged(int)),this, SLOT(GreenGainChanged(int)));
    connect(ui->sliderBlueGain, SIGNAL(valueChanged(int)),this, SLOT(BlueGainChanged(int)));
    connect(ui->editSizeX, SIGNAL(editingFinished()),this, SLOT(SizeXChanged()));
    connect(ui->editSizeY, SIGNAL(editingFinished()),this, SLOT(SizeYChanged()));

    connect(ui->buttonRefresh, SIGNAL(clicked()),this, SLOT(UpdatePreview()));
    connect(ui->checkAutoRefresh, SIGNAL(clicked()),this, SLOT(EnableAutoRefresh()));
    connect(timerAutoRefresh, SIGNAL(timeout()), this, SLOT(UpdatePreview()));

    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(this, SIGNAL(rejected()), this, SLOT(Cancel()));
    connect(this, SIGNAL(accepted()), this, SLOT(Accept()));

}

DialogConfigureCam::~DialogConfigureCam(){
    delete ui;
}

void DialogConfigureCam::InitControls(){
    QString string;

    //Vista previa
    ui->preview->setScaledContents(true);
    ui->preview->setFixedSize(480,320);

    //Botones
    ui->buttonLoadConfig->setText(CC_LOAD_LABEL);
    ui->buttonSaveConfig->setText(CC_SAVE_LABEL);
    ui->buttonResetDefaults->setText(CC_RESET_LABEL);
    ui->buttonRefresh->setText(CC_REFRESH_LABEL);


    //Checkboxes
    ui->checkAutoRefresh->setText(CC_AUTOREFRESH_LABEL);
    timerAutoRefresh = new QTimer(this);
    ui->checkAutoRefresh->setChecked(false);

    //Etiquetas con el nombre
    ui->labelPixelClock->setText(CC_PIXELCLOCK_LABEL);
    ui->labelFrameRate->setText(CC_FRAMERATE_LABEL);
    ui->labelExposure->setText(CC_EXPOSURE_LABEL);

    ui->labelMasterGain->setText(CC_MASTERGAIN_LABEL);
    ui->labelRedGain->setText(CC_REDGAIN_LABEL);
    ui->labelGreenGain->setText(CC_GREENGAIN_LABEL);
    ui->labelBlueGain->setText(CC_BLUEGAIN_LABEL);


    ui->labelWidth->setText(CC_WIDTH_LABEL);
    ui->labelHeight->setText(CC_HEIGHT_LABEL);

    string.setNum(cameraController->GetSize(SIZE_X));
    ui->editSizeX->setText(string);

    string.setNum(cameraController->GetSize(SIZE_Y));
    ui->editSizeY->setText(string);


}
void DialogConfigureCam::UpdateControls(){
    QString string;
    int aux;
    double auxDouble;

    aux=cameraController->GetPixelClock(PIXELCLOCK_MIN);
    ui->sliderPixelClock->setMinimum(aux);
    string.setNum(aux);
    string.append(CC_PIXELCLOCK_UNITS);
    ui->minPixelClock->setText(string);

    aux=cameraController->GetPixelClock(PIXELCLOCK_MAX);
    ui->sliderPixelClock->setMaximum(aux);
    string.setNum(aux);
    string.append(CC_PIXELCLOCK_UNITS);
    ui->maxPixelClock->setText(string);

    aux=cameraController->GetPixelClock(PIXELCLOCK);
    ui->sliderPixelClock->setValue(aux);
    string.setNum(aux);
    string.append(CC_PIXELCLOCK_UNITS);
    ui->curPixelClock->setText(string);

    auxDouble=cameraController->GetFrameRate(FRAMERATE_MIN);
    ui->sliderFrameRate->setMinimum(auxDouble*100);
    string.setNum(auxDouble,'g',3);
    string.append(CC_FRAMERATE_UNITS);
    ui->minFrameRate->setText(string);

    auxDouble=cameraController->GetFrameRate(FRAMERATE_MAX);
    ui->sliderFrameRate->setMaximum(auxDouble*100);
    string.setNum(auxDouble,'g',3);
    string.append(CC_FRAMERATE_UNITS);
    ui->maxFrameRate->setText(string);

    auxDouble=cameraController->GetFrameRate(FRAMERATE);
    ui->sliderFrameRate->setValue(auxDouble*100);
    string.setNum(auxDouble,'g',3);
    string.append(CC_FRAMERATE_UNITS);
    ui->curFrameRate->setText(string);

    auxDouble=cameraController->GetExposure(EXPOSURE_MIN);
    ui->sliderExposure->setMinimum(auxDouble*100);
    string.setNum(auxDouble,'g',3);
    string.append(CC_EXPOSURE_UNITS);
    ui->minExposure->setText(string);

    auxDouble=cameraController->GetExposure(EXPOSURE_MAX);
    ui->sliderExposure->setMaximum(auxDouble*100);
    string.setNum(auxDouble,'g',3);
    string.append(CC_EXPOSURE_UNITS);
    ui->maxExposure->setText(string);

    auxDouble=cameraController->GetExposure(EXPOSURE);
    ui->sliderExposure->setValue(auxDouble*100);
    string.setNum(auxDouble,'g',3);
    string.append(CC_EXPOSURE_UNITS);
    ui->curExposure->setText(string);

    aux=cameraController->GetMinGain(GAIN_MASTER);
    ui->sliderMasterGain->setMinimum(aux);
    ui->minMasterGain->setNum(aux);

    aux=cameraController->GetMaxGain(GAIN_MASTER);
    ui->sliderMasterGain->setMaximum(aux);
    ui->maxMasterGain->setNum(aux);

    aux=cameraController->GetCurrentGain(GAIN_MASTER);
    ui->sliderMasterGain->setValue(aux);
    ui->curMasterGain->setNum(aux);

    aux=cameraController->GetMinGain(GAIN_RED);
    ui->sliderRedGain->setMinimum(aux);
    ui->minRedGain->setNum(aux);

    aux=cameraController->GetMaxGain(GAIN_RED);
    ui->sliderRedGain->setMaximum(aux);
    ui->maxRedGain->setNum(aux);

    aux=cameraController->GetCurrentGain(GAIN_RED);
    ui->sliderRedGain->setValue(aux);
    ui->curRedGain->setNum(aux);

    aux=cameraController->GetMinGain(GAIN_GREEN);
    ui->sliderGreenGain->setMinimum(aux);
    ui->minGreenGain->setNum(aux);

    aux=cameraController->GetMaxGain(GAIN_GREEN);
    ui->sliderGreenGain->setMaximum(aux);
    ui->maxGreenGain->setNum(aux);

    aux=cameraController->GetCurrentGain(GAIN_GREEN);
    ui->sliderGreenGain->setValue(aux);
    ui->curGreenGain->setNum(aux);

    aux=cameraController->GetMinGain(GAIN_BLUE);
    ui->sliderBlueGain->setMinimum(aux);
    ui->minBlueGain->setNum(aux);

    aux=cameraController->GetMaxGain(GAIN_BLUE);
    ui->sliderBlueGain->setMaximum(aux);
    ui->maxBlueGain->setNum(aux);

    aux=cameraController->GetCurrentGain(GAIN_BLUE);
    ui->sliderBlueGain->setValue(aux);
    ui->curBlueGain->setNum(aux);

}


void DialogConfigureCam::PixelClockChanged(const int value){
    static int prev_value=-1;
    if(prev_value!=value){
        prev_value=value;
        cameraController->SetPixelClock(value);
        this->UpdateControls();
    }
}

void DialogConfigureCam::FrameRateChanged(const int value){
    static int prev_value=-1;
    if(prev_value!=value){
        prev_value=value;
        cameraController->SetFrameRate(((double)value)/100);
        this->UpdateControls();
    }
}

void DialogConfigureCam::ExposureChanged(const int value){
    static int prev_value=-1;
    if(prev_value!=value){
        prev_value=value;
        cameraController->SetExposure(((double)value)/100);
        this->UpdateControls();
    }
}

void DialogConfigureCam::MasterGainChanged(const int value){
    static int prev_value=-1;
    if(prev_value!=value){
        prev_value=value;
        cameraController->SetGain(GAIN_MASTER,value);
        this->UpdateControls();
    }
}

void DialogConfigureCam::RedGainChanged(int value){
    static int prev_value=-1;
    if(prev_value!=value){
        prev_value=value;
        cameraController->SetGain(GAIN_RED,value);
        this->UpdateControls();
    }
}

void DialogConfigureCam::GreenGainChanged(int value){
    static int prev_value=-1;
    if(prev_value!=value){
        prev_value=value;
        cameraController->SetGain(GAIN_GREEN,value);
        this->UpdateControls();
    }
}

void DialogConfigureCam::BlueGainChanged(int value){
    static int prev_value=-1;
    if(prev_value!=value){
        prev_value=value;
        cameraController->SetGain(GAIN_BLUE,value);
        this->UpdateControls();
    }
}

void DialogConfigureCam::SizeXChanged(){
    QString string;
    static int prev_value=-1;
    int value;
    value=(ui->editSizeX->text()).toInt();
    if(prev_value!=value){
        prev_value=value;
        cameraController->SetSize(SIZE_X,value);
        string.setNum(cameraController->GetSize(SIZE_X));
        ui->editSizeX->setText(string);
        this->UpdateControls();
    }
}

void DialogConfigureCam::SizeYChanged(){
    QString string;
    static int prev_value=-1;
    int value;
    value=(ui->editSizeY->text()).toInt();
    if(prev_value!=value){
        prev_value=value;
        cameraController->SetSize(SIZE_Y,value);
        string.setNum(cameraController->GetSize(SIZE_Y));
        ui->editSizeY->setText(string);
        this->UpdateControls();
    }
}


void DialogConfigureCam::UpdatePreview(){
    QImage * previewImage;

    cameraController->RenewPicture();
    previewImage = cameraController->GetSourcePicture();
    if (previewImage!=NULL){
        ui->preview->setPixmap(QPixmap::fromImage(previewImage->scaledToWidth(ui->preview->width(),
                           Qt::FastTransformation), Qt::AutoColor));
        delete previewImage;
    }
}

void DialogConfigureCam::EnableAutoRefresh(){

    if (ui->checkAutoRefresh->isChecked()== true)
        timerAutoRefresh->start(200);
    else
        timerAutoRefresh->stop();

}

void DialogConfigureCam::ResetDefaults(){
    QString string;

    //Es necesario actualizar después de cada cambio, pues si no, como a veces se cambian
    //los valores máximos se puede perder información
    cameraController->SetSize(SIZE_X,cameraController->GetMaxSize(SIZE_X));
    cameraController->SetSize(SIZE_Y,cameraController->GetMaxSize(SIZE_Y));
    string.setNum(cameraController->GetSize(SIZE_X));
    ui->editSizeX->setText(string);
    string.setNum(cameraController->GetSize(SIZE_Y));
    ui->editSizeY->setText(string);

    cameraController->SetPixelClock(cameraController->GetDefaultPixelClock());
    this->UpdateControls();
    cameraController->SetFrameRate(cameraController->GetDefaultFrameRate());
    this->UpdateControls();
    cameraController->SetExposure(cameraController->GetDefaultExposure());
    this->UpdateControls();
    cameraController->SetGain(GAIN_MASTER,cameraController->GetDefaultGain(GAIN_MASTER));
    this->UpdateControls();
    cameraController->SetGain(GAIN_RED,cameraController->GetDefaultGain(GAIN_RED));
    this->UpdateControls();
    cameraController->SetGain(GAIN_GREEN,cameraController->GetDefaultGain(GAIN_GREEN));
    this->UpdateControls();
    cameraController->SetGain(GAIN_BLUE,cameraController->GetDefaultGain(GAIN_BLUE));
    this->UpdateControls();

}

void DialogConfigureCam::LoadConfig(){
    QString string;
    CameraParameters config;

    QFileDialog fileDialog(this,tr("Load Config"));
    fileDialog.setNameFilter(tr("Ini Files (*.ini)"));
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setLabelText(QFileDialog::Accept,tr("Load"));

    if(fileDialog.exec()){
        if(config.LoadFromFile( fileDialog.selectedFiles().at(0).toLocal8Bit().constData() )!=true){
            QMessageBox::critical(this,CC_ERROR_LABEL,CC_FAILLOAD_LABEL+fileDialog.selectedFiles().at(0),QMessageBox::Ok);
            return;
        }
        cameraController->SetParameters(config);
        this->UpdateControls();
        string.setNum(cameraController->GetSize(SIZE_X));
        ui->editSizeX->setText(string);
        string.setNum(cameraController->GetSize(SIZE_Y));
        ui->editSizeY->setText(string);

    }
}

void DialogConfigureCam::SaveConfig(){
    QFileDialog fileDialog(this,tr("Save Config"));
    fileDialog.setNameFilter(tr("Ini Files (*.ini)"));
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setDefaultSuffix("ini");
    fileDialog.setLabelText(QFileDialog::Accept,tr("Save"));
    if(fileDialog.exec()){
        if((cameraController->GetParameters()).SaveToFile( fileDialog.selectedFiles().at(0).toLocal8Bit().constData() )!=true){
            QMessageBox::critical(this,CC_ERROR_LABEL,CC_FAILSAVE_LABEL+fileDialog.selectedFiles().at(0),QMessageBox::Ok);
            return;
        }
    }


}

void DialogConfigureCam::Accept(){

}

void DialogConfigureCam::Cancel(){

    cameraController->SetParameters(initValues);

}
