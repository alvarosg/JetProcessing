/**
 * @file   cameraueye.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase CameraUEye.
 *
 */
#include"cameraueye.h"



CameraUEye::CameraUEye()
{
    //Inicio la cámara a 0
    hCam = 0;
    //Al crear el objeto, abro la cámara, con la configuración por defecto
    OpenCamera(RESET_CONFIG);

    return;

}

bool CameraUEye::IsOpen(){

    //Si tengo handler, compruebo que la cámara sigue estando ahí
    if (hCam!=0){
        if(is_CameraStatus(hCam,0,0)!=IS_SUCCESS){
            this->CloseCamera();
            return false;
        }
        else
            return true;
    }
    else
        return false;
}


CameraUEye::~CameraUEye(){
    this->CloseCamera();
}


void CameraUEye::SetDefaultConfigValues(){

    //Estos serán los editables por el usuario, a través de métodos
    parameters.SetPixelClock(GetDefaultPixelClock());
    parameters.SetFrameRate(GetDefaultFrameRate());
    parameters.SetExposure(GetDefaultExposure());
    parameters.SetGainMaster(GetDefaultGain(GAIN_MASTER));
    parameters.SetGainRed(GetDefaultGain(GAIN_RED));
    parameters.SetGainGreen(GetDefaultGain(GAIN_GREEN));
    parameters.SetGainBlue(GetDefaultGain(GAIN_BLUE));
    parameters.SetSizeX(GetMaxSize(SIZE_X));
    parameters.SetSizeY(GetMaxSize(SIZE_Y));

    //Estos no serán editables, serán siempre los mismos
    nColorCorrection=IS_CCOR_DISABLE;
    nBayerMode=IS_SET_BAYER_CV_NORMAL;
    renderMode=IS_RENDER_NORMAL;
    colorMode = IS_CM_BGRA8_PACKED;

}


void CameraUEye::CloseCamera(){
    //Si ya tenía la cámara abierta, la cierro, liberando la memoria
    if ( hCam != 0 ){
        is_FreeImageMem( hCam, imageMemory, memoryId );
        is_ExitCamera( hCam );
    }
    hCam = (HIDS) 0;
}

bool CameraUEye::OpenCamera(ConfigChanges config){
    int retValue;

    this->CloseCamera();

    //Inicio la Camara
    retValue = is_InitCamera( &hCam, NULL );

    if( retValue == IS_SUCCESS )
    {
        //Si tengo que reiniciar los parámetros a los parámetros por defecto, llamo a la función
        if(config==RESET_CONFIG)
            SetDefaultConfigValues();

        //Cargo todos los parámetros en la cámara
        LoadParameters();
    }
    else
        return false;
    return true;
}


QImage * CameraUEye::AcquireSourcePicture()
{
    int i;
    if ( hCam == 0 )
        OpenCamera(KEEP_CONFIG);

    char * imageBuffer;
    SourcePicture * picture;

    int maxSizeX=GetMaxSize(SIZE_X);

    if ( hCam != 0 ){
        if( is_FreezeVideo( hCam, IS_WAIT ) == IS_SUCCESS ){
            imageBuffer=(char *)malloc(parameters.GetSizeX()*parameters.GetSizeY()*4);

            for(i=0;i<parameters.GetSizeY();i++)
                memcpy(imageBuffer+i*4*parameters.GetSizeX(),imageMemory+i*4*maxSizeX,parameters.GetSizeX()*4);

            picture=new SourcePicture((uchar *)imageBuffer,parameters.GetSizeX(),parameters.GetSizeY());
            return picture;
        }
    }

    return new QImage();

}


void CameraUEye::LoadParameters()
{

    if(hCam == 0)
        return;

    //Configuro el tamaño
    SetSize(SIZE_X,parameters.GetSizeX());
    SetSize(SIZE_Y,parameters.GetSizeY());

    //Configuro los modos de color y de presentación
    is_SetColorMode(hCam,colorMode);
    is_GetColorDepth(hCam, &bitsPerPixel, &colorMode);
    is_SetDisplayMode( hCam, IS_SET_DM_DIB);

    //Reservo Memoria en el dispositivo
    is_AllocImageMem(hCam,GetMaxSize(SIZE_X),GetMaxSize(SIZE_Y),bitsPerPixel,&imageMemory,&memoryId);
    is_SetImageMem( hCam, imageMemory, memoryId );

    //Configuro los parámetros (Frames por segundo, reloj del pixel, exposición, ganancia)
    SetFrameRate(parameters.GetFrameRate());
    SetPixelClock(parameters.GetPixelClock());
    SetExposure(parameters.GetExposure());
    SetGain(GAIN_MASTER,parameters.GetGainMaster());
    SetGain(GAIN_RED,parameters.GetGainRed());
    SetGain(GAIN_GREEN,parameters.GetGainGreen());
    SetGain(GAIN_BLUE,parameters.GetGainBlue());

    //Deshabilito la corrección de color
    is_SetColorCorrection(hCam, nColorCorrection, NULL);
    is_SetBayerConversion(hCam, nBayerMode);
}



double CameraUEye::SetExposure(double value){
    if ( hCam == 0 ){
        return -1;
    }
    double exposure=value;
    is_Exposure(hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&exposure, sizeof(exposure));
    is_Exposure(hCam,IS_EXPOSURE_CMD_GET_EXPOSURE,(void*)&exposure, sizeof(exposure));
    return exposure;

}

double CameraUEye::GetExposure(ExposureParameter command){
    if ( hCam == 0 ){
        return -1;
    }
    double exposure;
    switch(command){
        case EXPOSURE:
            is_Exposure(hCam,IS_EXPOSURE_CMD_GET_EXPOSURE,(void*)&exposure,sizeof(exposure));
            parameters.SetExposure(exposure);
            break;
        case EXPOSURE_MIN:
            is_Exposure(hCam,IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE_MIN,(void*)&exposure,sizeof(exposure));
            break;
        case EXPOSURE_MAX:
            is_Exposure(hCam,IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE_MAX,(void*)&exposure,sizeof(exposure));
            break;
        default:
            return -1;
    }
    return exposure;

}

int CameraUEye::SetGain(GainType command, int value){

    if ( hCam == 0 ){
        return -1;
    }
    int retValue;
    switch(command){
        case GAIN_MASTER:
            is_SetHardwareGain(hCam, value, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            retValue  = is_SetHardwareGain(hCam,IS_GET_MASTER_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            parameters.SetGainMaster(retValue);
            return retValue;
            break;
        case GAIN_RED:
            is_SetHardwareGain(hCam, IS_IGNORE_PARAMETER, value, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            retValue = is_SetHardwareGain(hCam,IS_GET_RED_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            parameters.SetGainRed(retValue);
            return retValue;
            break;
        case GAIN_GREEN:
            is_SetHardwareGain(hCam, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER,value,  IS_IGNORE_PARAMETER);
            retValue  = is_SetHardwareGain(hCam,IS_GET_GREEN_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            parameters.SetGainGreen(retValue);
            return retValue;
            break;
        case GAIN_BLUE:
            is_SetHardwareGain(hCam, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER,value );
            retValue  = is_SetHardwareGain(hCam,IS_GET_BLUE_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            parameters.SetGainBlue(retValue);
            return retValue;
            break;
        default:
            return -1;
    }


    return -1;
}

int CameraUEye::GetCurrentGain(GainType command){
    if ( hCam == 0 ){
        return -1;
    }
    int retValue;
    switch(command){
        case GAIN_MASTER:
            retValue  = is_SetHardwareGain(hCam,IS_GET_MASTER_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            parameters.SetGainMaster(retValue);
            return retValue;
            break;
        case GAIN_RED:
            retValue  = is_SetHardwareGain(hCam,IS_GET_RED_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            parameters.SetGainRed(retValue);
            return retValue;
            break;
        case GAIN_GREEN:
            retValue  = is_SetHardwareGain(hCam,IS_GET_GREEN_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            parameters.SetGainGreen(retValue);
            return retValue;
            break;
        case GAIN_BLUE:
            retValue  = is_SetHardwareGain(hCam,IS_GET_BLUE_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            parameters.SetGainBlue(retValue);
            return retValue;
            break;
    }

    return -1;
}

int CameraUEye::GetDefaultGain(GainType command){
    if ( hCam == 0 ){
        return -1;
    }
    int retValue;
    switch(command){
        case GAIN_MASTER:
            retValue  = is_SetHardwareGain(hCam,IS_GET_DEFAULT_MASTER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            return retValue;
            break;
        case GAIN_RED:
            retValue  = is_SetHardwareGain(hCam,IS_GET_DEFAULT_RED, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            return retValue;
            break;
        case GAIN_GREEN:
            retValue  = is_SetHardwareGain(hCam,IS_GET_DEFAULT_GREEN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            return retValue;
            break;
        case GAIN_BLUE:
            retValue  = is_SetHardwareGain(hCam,IS_GET_DEFAULT_BLUE, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            return retValue;
            break;
    }

    return -1;
}


int CameraUEye::GetMaxSize(SizeAxis command){

    INT AOISupported=0;
    SENSORINFO sensorInfo;
    IS_SIZE_2D imageSize;
    int size;

    if ( hCam == 0 ){
        return -1;
    }

    // Compruebo si AOI está soportado
    if (is_ImageFormat(hCam,IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED,
                  (void*)&AOISupported, sizeof(AOISupported)) != IS_SUCCESS){
        AOISupported = 1;
    }

    is_AOI(hCam,IS_AOI_IMAGE_GET_SIZE_INC, (void*)&imageSize, sizeof(imageSize));

    if (AOISupported == 1){
        // Si AOI está soportado por la cámara, puedo obtener realmente los valores máximos

        is_GetSensorInfo (hCam, &sensorInfo);
        if (command==SIZE_X) size = sensorInfo.nMaxWidth;
        if (command==SIZE_Y) size = sensorInfo.nMaxHeight;

    }
    else{
        // Si AOI no está soportado por la cámara, cojo los valores actuales

        is_AOI(hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

        if (command==SIZE_X) size = imageSize.s32Width;
        if (command==SIZE_Y) size = imageSize.s32Height;

    }
    return size;

}

int CameraUEye::GetMinSize(SizeAxis command){
    switch (command){
        case SIZE_X:
            return 128;
            break;
        case SIZE_Y:
            return 128;
            break;
        default:
            return -1;
    }
    return -1;
}

int CameraUEye::GetSize(SizeAxis command){

    IS_SIZE_2D imageSize;
    int size;

    if ( hCam == 0 ){
        return -1;
    }

    // Si AOI no está soportado por la cámara, cojo los valores actuales

    is_AOI(hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

    switch (command){
        case SIZE_X:
            size=imageSize.s32Width;
            parameters.SetSizeX(size);
            return size;
            break;
        case SIZE_Y:
            size=imageSize.s32Height;
            parameters.SetSizeY(size);
            return size;
            break;
        default:
            return -1;
    }
    return -1;
}

int CameraUEye::SetSize(SizeAxis command, int value){
    IS_SIZE_2D imageSize;

    if ( hCam == 0 ){
        return -1;
    }
    if(command==SIZE_X) parameters.SetSizeX(value);
    else if(command==SIZE_Y) parameters.SetSizeY(value);
    else return -1;

    imageSize.s32Width = parameters.GetSizeX();
    imageSize.s32Height = parameters.GetSizeY();

    is_AOI(hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

    parameters.SetSizeX(GetSize(SIZE_X));
    parameters.SetSizeY(GetSize(SIZE_Y));

    if(command==SIZE_X) return parameters.GetSizeX();
    else if(command==SIZE_Y) return parameters.GetSizeY();

    return -1;

}

int CameraUEye::GetPixelClock(PixelClockParameter command){
    if ( hCam == 0 ){
        return -1;
    }
    int range[3];
    int pixelClock;
    switch(command){
        case PIXELCLOCK:
            is_PixelClock(hCam, IS_PIXELCLOCK_CMD_GET, (void*)&pixelClock, sizeof(pixelClock));
            parameters.SetPixelClock(pixelClock);
            return pixelClock;
        case PIXELCLOCK_MIN:
             is_PixelClock(hCam, IS_PIXELCLOCK_CMD_GET_RANGE, (void*)range, sizeof(range));
            return range[0];
        case PIXELCLOCK_MAX:
             is_PixelClock(hCam, IS_PIXELCLOCK_CMD_GET_RANGE, (void*)range, sizeof(range));
            return range[1];
    }

    return -1;
}

int CameraUEye::GetDefaultPixelClock(){
    if ( hCam == 0 ){
        return -1;
    }
    int pixelClockDflt;
    is_PixelClock(hCam, IS_PIXELCLOCK_CMD_GET_DEFAULT, (void*)&pixelClockDflt, sizeof(pixelClockDflt));
    return pixelClockDflt;

}

int CameraUEye::SetPixelClock(int value){
    if ( hCam == 0 ){
        return -1;
    }
    int pixelClock;
    is_PixelClock(hCam, IS_PIXELCLOCK_CMD_SET, (void*)&value, sizeof(value));
    pixelClock=GetPixelClock(PIXELCLOCK);
    parameters.SetPixelClock(pixelClock);
    return pixelClock;

}

double CameraUEye::GetFrameRate(FramerateParameter command){
    if ( hCam == 0 ){
        return -1;
    }

    double value;
    switch(command){
        case FRAMERATE:
            is_SetFrameRate (hCam,IS_GET_FRAMERATE, &value);
            parameters.SetFrameRate(value);
            break;
        case FRAMERATE_MIN:
            is_GetFrameTimeRange (hCam, NULL,&value,NULL);
            value=1/value;
            break;
        case FRAMERATE_MAX:
            is_GetFrameTimeRange (hCam,&value, NULL,NULL);
            value=1/value;
            break;
        default:
            return -1;
    }
    return value;
}

double CameraUEye::GetDefaultFrameRate(){
    if ( hCam == 0 ){
        return -1;
    }
    double frameRateDflt;
    is_SetFrameRate (hCam,IS_GET_DEFAULT_FRAMERATE, &frameRateDflt);
    return frameRateDflt;

}

double CameraUEye::SetFrameRate(double value){
    if ( hCam == 0 ){
        return -1;
    }
    double frameRate;
    is_SetFrameRate (hCam,value, &frameRate);
    parameters.SetFrameRate(frameRate);
    return frameRate;

}

