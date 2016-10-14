/**
 * @file   programsettings.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase ProgramSettings.
 *
 */
#include "programsettings.h"

ProgramSettings::ProgramSettings(){
    mode=MODE_CUDA;
    useDefaultMode=false;
    outputFolder=QDir::homePath();
    saveCameraPictures=true;
    applyUntilCurrentAlgorithm=true;
    saveGraphicsViewSize=true;
    customSize.setHeight(800);
    customSize.setWidth(800);
    defaultImageFormat="png";
    firstExecution=true;
}
void ProgramSettings::SetFirstExecution(bool value){
    firstExecution=value;
}

bool ProgramSettings::GetFirstExecution(){
    return firstExecution;
}

void ProgramSettings::SetUseDefaultMode(bool value){
    useDefaultMode=value;

}

bool ProgramSettings::GetUseDefaultMode(){
    return useDefaultMode;
}

void ProgramSettings::SetMode(CalcMode value){
    mode=value;
}

CalcMode ProgramSettings::GetMode(){
    return mode;
}

void ProgramSettings::SetOutputFolder(QString path){
    outputFolder=path;
}

QString ProgramSettings::GetOutputFolder(){
    return outputFolder;
}

void ProgramSettings::SetSaveCameraPictures(bool value){
    saveCameraPictures=value;
}

bool ProgramSettings::GetSaveCameraPictures(){
    return saveCameraPictures;
}

void ProgramSettings::SetApplyUntilCurrentAlgorithm(bool value){
    applyUntilCurrentAlgorithm=value;
}

bool ProgramSettings::GetApplyUntilCurrentAlgorithm(){
    return applyUntilCurrentAlgorithm;
}

void ProgramSettings::SetSaveGraphicsViewSize(bool value){
    saveGraphicsViewSize=value;
}

bool ProgramSettings::GetSaveGraphicsViewSize(){
    return saveGraphicsViewSize;
}

void ProgramSettings::SetCustomSize(QSize size){
    customSize=size;
}

QSize ProgramSettings::GetCustomSize(){
    return customSize;
}

void ProgramSettings::SetDefaultImageFormat(QString format){
    defaultImageFormat=format;
}

QString ProgramSettings::GetDefaultImageFormat(){
    return defaultImageFormat;
}

bool ProgramSettings::LoadFromFile(QString path){
    QSettings * settings;
    if(path.isEmpty()==true) settings=new QSettings(QSettings::IniFormat, QSettings::UserScope, "ASG", "JetProcessing");
    else settings=new QSettings(path,QSettings::IniFormat);
    firstExecution=settings->value("FirstExecution",true).toBool();
    switch(settings->value("Mode",MODE_CUDA).toInt()){
        case 0:    mode=MODE_CUDA; break;
        case 1:   mode=MODE_CPU;  break;
    }
    useDefaultMode=settings->value("UseDefaultMode",false).toBool();
    outputFolder=settings->value("OutputFolder",QDir::homePath()).toString();
    saveCameraPictures=settings->value("SaveCameraPictures",false).toBool();
    applyUntilCurrentAlgorithm=settings->value("ApplyUntilCurrentAlgorithm",true).toBool();
    saveGraphicsViewSize=settings->value("SaveGraphicsViewSize",true).toBool();
    customSize=settings->value("CustomSize",QSize::QSize(800,800)).toSize();
    defaultImageFormat=settings->value("DefaultImageFormat",QString::QString("png")).toString();

    if(settings->status()!=QSettings::NoError){
        delete settings;
        return false;
    }
    delete settings;
    return true;
}
bool ProgramSettings::SaveToFile(QString path){
    QSettings * settings;
    if(path.isEmpty()==true) settings=new QSettings(QSettings::IniFormat, QSettings::UserScope, "ASG", "JetProcessing");
    else settings=new QSettings(path,QSettings::IniFormat);
    settings->setValue("FirstExecution",false);
    settings->setValue("Mode", mode);
    settings->setValue("UseDefaultMode", useDefaultMode);
    settings->setValue("OutputFolder", outputFolder);
    settings->setValue("SaveCameraPictures",saveCameraPictures);
    settings->setValue("ApplyUntilCurrentAlgorithm",applyUntilCurrentAlgorithm);
    settings->setValue("SaveGraphicsViewSize",saveGraphicsViewSize);
    settings->setValue("CustomSize",customSize);
    settings->setValue("DefaultImageFormat",defaultImageFormat);

    if(settings->status()!=QSettings::NoError){
        delete settings;
        return false;
    }
    delete settings;
    return true;
}


