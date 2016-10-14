/**
 * @file   abstractcamera.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de una clase abstracta para la representación de una cámara.
 *
 */
#include"abstractcamera.h"


double AbstractCamera::GetExposure(ExposureParameter command){

    switch(command){
        case EXPOSURE:
            return parameters.GetExposure();
            break;
        default:
            return -1;
    }
    return -1;

}

double AbstractCamera::GetDefaultExposure(){
    return 90;
}

int AbstractCamera::GetPixelClock(PixelClockParameter command){

    switch(command){
        case PIXELCLOCK:
            return parameters.GetPixelClock();
        default:
            return -1;
    }

    return -1;
}

double AbstractCamera::GetFrameRate(FramerateParameter command){
    switch(command){
        case FRAMERATE:
            return parameters.GetFrameRate();
        default:
            return -1;
    }

    return -1;
}


int AbstractCamera::GetCurrentGain(GainType command){

    switch(command){
        case GAIN_MASTER:        
            return parameters.GetGainMaster();
            break;
        case GAIN_RED:
            return parameters.GetGainRed();
            break;
        case GAIN_GREEN:
            return parameters.GetGainGreen();
            break;
        case GAIN_BLUE:
            return parameters.GetGainBlue();
            break;
    }

    return -1;
}

int AbstractCamera::GetMinGain(GainType command){
    return 0;
}

int AbstractCamera::GetMaxGain(GainType command){
    return 100;
}

int AbstractCamera::GetSize(SizeAxis  command){

    switch (command){
        case SIZE_X:
            return parameters.GetSizeX();
            break;
        case SIZE_Y:
            return parameters.GetSizeY();
            break;
        default:
            return -1;
    }
    return -1;
}


CameraParameters AbstractCamera::GetParameters(){
    return parameters;
}

CameraParameters AbstractCamera::SetParameters(CameraParameters parameters_in){
    parameters=parameters_in;
    this->SetSize(SIZE_X,parameters.GetSizeX());
    this->SetSize(SIZE_Y,parameters.GetSizeY());
    this->SetPixelClock(parameters.GetPixelClock());
    this->SetFrameRate(parameters.GetFrameRate());
    this->SetExposure(parameters.GetExposure());
    this->SetGain(GAIN_MASTER,parameters.GetGainMaster());
    this->SetGain(GAIN_RED,parameters.GetGainRed());
    this->SetGain(GAIN_GREEN,parameters.GetGainGreen());
    this->SetGain(GAIN_BLUE,parameters.GetGainBlue());
    return parameters;
}




