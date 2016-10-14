/**
 * @file   cameraparameters.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase CameraParameters.
 *
 */
#include "cameraparameters.h"

CameraParameters::CameraParameters(){
    sizeX=0;
    sizeY=0;
    pixelClock=0;
    frameRate=0;
    exposureTime=0;
    gainM=0;
    gainR=0;
    gainG=0;
    gainB=0;

}
void CameraParameters::SetSizeX(int value){
    sizeX=value;
}

void CameraParameters::SetSizeY(int value){
    sizeY=value;
}

void CameraParameters::SetPixelClock(int value){
    pixelClock=value;
}

void CameraParameters::SetFrameRate(double value){
    frameRate=value;
}

void CameraParameters::SetExposure(double value){
    exposureTime=value;
}

void CameraParameters::SetGainMaster(int value){
    gainM=value;
}

void CameraParameters::SetGainRed(int value){
    gainR=value;
}

void CameraParameters::SetGainGreen(int value){
    gainG=value;
}

void CameraParameters::SetGainBlue(int value){
    gainB=value;
}

int CameraParameters::GetSizeX(){
    return sizeX;
}

int CameraParameters::GetSizeY(){
    return sizeY;
}

int CameraParameters::GetPixelClock(){
    return pixelClock;
}

double CameraParameters::GetFrameRate(){
    return frameRate;
}

double CameraParameters::GetExposure(){
    return exposureTime;
}

int CameraParameters::GetGainMaster(){
    return gainM;
}

int CameraParameters::GetGainRed(){
    return gainR;
}
int CameraParameters::GetGainGreen(){
    return gainG;
}

int CameraParameters::GetGainBlue(){
    return gainB;
}

bool CameraParameters::LoadFromFile(QString filePath){
    QSettings settings(filePath,QSettings::IniFormat);

    if(settings.status()!=QSettings::NoError)
        return false;

    sizeX=settings.value("SizeX",-1).toInt();
    sizeY=settings.value("SizeY",-1).toInt();
    pixelClock=settings.value("PixelClock",-1).toInt();
    frameRate=settings.value("FrameRate",-1).toDouble();
    exposureTime=settings.value("Exposure",-1).toDouble();
    gainM=settings.value("MasterGain",-1).toInt();
    gainR=settings.value("RedGain",-1).toInt();
    gainG=settings.value("GreenGain",-1).toInt();
    gainB=settings.value("BlueGain",-1).toInt();


    if(sizeX == -1 ||sizeY == -1 || pixelClock==-1 || frameRate==-1 || exposureTime==-1
            ||gainM==-1 ||gainR==-1 ||gainG==-1 ||gainB==-1)
        return false;
    return true;
}

bool CameraParameters::SaveToFile(QString filePath){
    QSettings settings(filePath,QSettings::IniFormat);

    if(settings.status()!=QSettings::NoError)
        return false;

    settings.setValue("SizeX", sizeX);
    settings.setValue("SizeY", sizeY);
    settings.setValue("PixelClock", pixelClock);
    settings.setValue("FrameRate", frameRate);
    settings.setValue("Exposure", exposureTime);
    settings.setValue("MasterGain", gainM);
    settings.setValue("RedGain", gainR);
    settings.setValue("GreenGain", gainG);
    settings.setValue("BlueGain", gainB);

    if(settings.status()!=QSettings::NoError)
        return false;

    return true;
}
