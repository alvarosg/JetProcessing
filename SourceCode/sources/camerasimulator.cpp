/**
 * @file   camerasimulator.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase CameraSimulator.
 *
 */
#include "camerasimulator.h"

CameraSimulator::CameraSimulator(){
    this->SetDefaultConfigValues();
    imageFolder=QCoreApplication::applicationDirPath()+"/CameraSimulator";


    //Cargo la carpeta de imágenes para el simulador
    QDir simulatorPicturesDir(imageFolder);

    //Obtengo los archivos de imagen disponibles y los dejo en la variable
    QStringList formats;
    formats.append("*.bmp");
    formats.append("*.png");
    formats.append("*.jpg");
    imageList=simulatorPicturesDir.entryList(formats,QDir::NoDotAndDotDot | QDir::Files);

}

bool CameraSimulator::IsOpen(){
    return true;
}


void CameraSimulator::SetDefaultConfigValues(){

    //Estos serán los editables por el usuario, a través de métodos
    parameters.SetPixelClock(this->GetDefaultPixelClock());
    parameters.SetFrameRate(this->GetDefaultFrameRate());
    parameters.SetExposure(this->GetDefaultExposure());
    parameters.SetGainMaster(this->GetDefaultGain(GAIN_MASTER));
    parameters.SetGainRed(this->GetDefaultGain(GAIN_RED));
    parameters.SetGainGreen(this->GetDefaultGain(GAIN_GREEN));
    parameters.SetGainBlue(this->GetDefaultGain(GAIN_BLUE));
    parameters.SetSizeX(this->GetMaxSize(SIZE_X));
    parameters.SetSizeY(this->GetMaxSize(SIZE_Y));

}


QImage * CameraSimulator::AcquireSourcePicture(){
    static int num=0;
    QImage * out;
    QImage aux;
    QString string;

    //Si el índice actual es mayor que los archivos disponibles, lo reinicio
    if(num >= imageList.count())
        num=0;
    //Si habia al menos un archivo, lo obtengo
    if(imageList.count()>0)
        string=imageFolder+"/"+imageList.at(num);
    //Si no, intento cargar cualquier cosa, que por supuesto no va a existir
    else
        string=imageFolder+"/randomName.png";

    //Si he conseguido cargar la imagen, la copio con las dimensiones adecuadas y la devuelvo
    if(aux.load(string)==true){
        out=new QImage(aux.copy(0,0,parameters.GetSizeX(),parameters.GetSizeY()));
    }
    //Si no, croe una en negro de las dimensiones adecuadas
    else{
        out=new QImage(parameters.GetSizeX(),parameters.GetSizeY(),QImage::Format_RGB32);
    }

    //Aumento el índice para la siguiente vuelta
    num++;
    //Simulo la latencia la tomar la imagen
    SleeperClass::msleep(parameters.GetExposure()); 
    return out;
}


double CameraSimulator::SetExposure(double value){
    parameters.SetExposure(value);
    return value;
}

double CameraSimulator::GetExposure(ExposureParameter command){
    double exposure;
    switch(command){
        case EXPOSURE:
            exposure=parameters.GetExposure();
            break;
        case EXPOSURE_MIN:
            exposure=0;
            break;
        case EXPOSURE_MAX:
            exposure=1000/parameters.GetFrameRate();
            break;
        default:
            return -1;
    }
    return exposure;
}

int CameraSimulator::SetGain(GainType command, int value){
    switch(command){
        case GAIN_MASTER:
            parameters.SetGainMaster(value);
            return value;
            break;
        case GAIN_RED:
            parameters.SetGainRed(value);
            break;
        case GAIN_GREEN:
            parameters.SetGainGreen(value);
            break;
        case GAIN_BLUE:
            parameters.SetGainBlue(value);
            break;
        default:
            return -1;
    }
    return value;
}

int CameraSimulator::GetCurrentGain(GainType command){
    int value;
    switch(command){
        case GAIN_MASTER:
            value=parameters.GetGainMaster();
            break;
        case GAIN_RED:
            value=parameters.GetGainRed();
            break;
        case GAIN_GREEN:
            value=parameters.GetGainGreen();
            break;
        case GAIN_BLUE:
            value=parameters.GetGainBlue();
            break;
    }

    return value;
}

int CameraSimulator::GetDefaultGain(GainType command){
    int retValue;
    switch(command){
        case GAIN_MASTER:
            retValue  = 0;
            break;
        case GAIN_RED:
            retValue  = 10;
            break;
        case GAIN_GREEN:
            retValue  = 0;
            break;
        case GAIN_BLUE:
            retValue  = 15;
            break;
    }

    return retValue;
}


int CameraSimulator::GetMaxSize(SizeAxis command){
    int size;

    if (command==SIZE_X) size = 1280;
    if (command==SIZE_Y) size = 1024;

    return size;

}

int CameraSimulator::GetMinSize(SizeAxis command){
    switch (command){
        case SIZE_X:
            return 10;
            break;
        case SIZE_Y:
            return 10;
            break;
        default:
            return -1;
    }
    return -1;
}

int CameraSimulator::GetSize(SizeAxis command){
    int size;
    switch (command){
        case SIZE_X:
            size=parameters.GetSizeX();
            return size;
            break;
        case SIZE_Y:
            size=parameters.GetSizeY();
            return size;
            break;
        default:
            return -1;
    }
    return -1;
}

int CameraSimulator::SetSize(SizeAxis command, int value){

    if(command==SIZE_X) parameters.SetSizeX(value);
    else if(command==SIZE_Y) parameters.SetSizeY(value);

    return value;

}

int CameraSimulator::GetPixelClock(PixelClockParameter command){

    int pixelClock;
    switch(command){
        case PIXELCLOCK:
            pixelClock=parameters.GetPixelClock();
            return pixelClock;
        case PIXELCLOCK_MIN:
            return 1;
        case PIXELCLOCK_MAX:
            return 48;
    }

    return -1;
}

int CameraSimulator::GetDefaultPixelClock(){
    return 24;

}

int CameraSimulator::SetPixelClock(int value){
    parameters.SetPixelClock(value);
    return value;

}

double CameraSimulator::GetFrameRate(FramerateParameter command){
    double value;
    switch(command){
        case FRAMERATE:
            value=parameters.GetFrameRate();
            break;
        case FRAMERATE_MIN:
            value=0.01;
            break;
        case FRAMERATE_MAX:
            value=parameters.GetPixelClock();
            break;
        default:
            return -1;
    }
    return value;
}

double CameraSimulator::GetDefaultFrameRate(){
    return 9;
}

double CameraSimulator::SetFrameRate(double value){
    parameters.SetFrameRate(value);
    return value;
}
