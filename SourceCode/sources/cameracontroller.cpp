/**
 * @file   cameracontroller.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase CameraController.
 *
 */

#include "cameracontroller.h"


CameraController::CameraController(CameraType cameraType){
    buffer=NULL;

    //Semáforos para controlar el flujo productor/consumidor
    semaphoreProducer = new QSemaphore(0);
    semaphoreConsumer = new QSemaphore(0);

    //Semáforo para una finalización ordenada
    semaphoreEnding = new QSemaphore(0);

    //Mutex para evitar modificar o leer parámetros de la cámara mientras se está tomando una imagen
    mutexPicture = new QMutex();

    //Desbloqueo el semáforo del productor
    semaphoreProducer->release();

    init =false;
    //Creo la factoría
    switch(cameraType){
        case CAMERA_UEYE:
            cameraFactory= new CameraFactoryForUEye();
            break;
        default:
            cameraFactory= new CameraFactoryForSimulator();
    }



    //Abro la cámara
    camera=cameraFactory->GetCamera();
    cameraParameters=camera->GetParameters();

}

bool CameraController::Initialize(){
    //Si no esta abierta, lo intento de nuevo
    if(camera->IsOpen()==false){
        delete camera;
        camera=cameraFactory->GetCamera();
    }
    if(camera->IsOpen()==false)
        return false;

    //Reestablezco los valores de los semáforos
    delete semaphoreProducer;
    delete semaphoreConsumer;
    delete semaphoreEnding;
    delete mutexPicture;
    semaphoreProducer = new QSemaphore(0);
    semaphoreConsumer = new QSemaphore(0);
    semaphoreEnding = new QSemaphore(0);
    mutexPicture = new QMutex();
    semaphoreProducer->release();

    //Cargo los parámetros
    cameraParameters=camera->SetParameters(cameraParameters);

    init=true;
    this->start();
    return true;

}

bool CameraController::CameraIsReady(){
    //Incluso si ya está iniciado puede que no este ahí, por lo que lo compruebo
    if(init==true){
        //Si no esta abierta, quito el hilo que recoge imágenes y elimino la cámara
        if(camera->IsOpen()==false){
            //Libero el semáforo de finalizar para que el hilo acabe
            semaphoreEnding->release();
            //Libero el semáforo del productor, para asegurar que no se queda atascado en ese paso
            semaphoreProducer->release();
            //Espero a que el hilo haya acabado
            this->wait();
            init=false;
            //Y elimino la cámara creando otra nueva
            delete camera;
            camera=cameraFactory->GetCamera();
            return false;
        }
        else
            return true;
    }
    else
        return false;

}

CameraController::~CameraController(){
    if(init==true){
        //Libero el semáforo de finalizar para que el hilo acabe
        semaphoreEnding->release();
        //Libero el semáforo del productor, para asegurar que no se queda atascado en ese paso
        semaphoreProducer->release();
        //Espero a que el hilo haya acabado
        this->wait();
    }

    if(buffer!=NULL)
        delete buffer;

    delete semaphoreProducer;
    delete semaphoreConsumer;
    delete semaphoreEnding;
    delete mutexPicture;
    delete camera;
}

QImage * CameraController::GetSourcePicture(){
    QImage * aux;
    if(init==false)
        return new QImage();

    semaphoreConsumer->acquire();
    aux=buffer;
    buffer=NULL;
    semaphoreProducer->release();
    return aux;
}

//Es como otro cliente, que simplemente desecha la foto si habia una, y si no sale
//(La foto que se esta sacando ya estará actualizada)
void CameraController::RenewPicture(){
    if(init==false)
        return;

    if(semaphoreConsumer->tryAcquire()==true){
        delete buffer;
        buffer=NULL;
        semaphoreProducer->release();
    }
}


void CameraController::run(){
    for(;;){
        semaphoreProducer->acquire();
        //Si me han dicho que tengo que finalizar (Se está destruyendo el controlador), salgo del bucle
        if(semaphoreEnding->tryAcquire()==true) break;
        mutexPicture->lock();
        buffer=camera->AcquireSourcePicture();
        mutexPicture->unlock();
        semaphoreConsumer->release();
    }
}


int CameraController::GetSize(SizeAxis command){
    int returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->GetSize(command);
    mutexPicture->unlock();

    return returnValue;

}

int CameraController::GetMaxSize(SizeAxis command){
    int returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->GetMaxSize(command);
    mutexPicture->unlock();

    return returnValue;
}
int CameraController::GetMinSize(SizeAxis command){
    int returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->GetMinSize(command);
    mutexPicture->unlock();

    return returnValue;
}
int CameraController::SetSize(SizeAxis  command,int value){
    int returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->SetSize(command,value);
    mutexPicture->unlock();

    return returnValue;
}

int CameraController::GetCurrentGain(GainType command){
    int returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->GetCurrentGain(command);
    mutexPicture->unlock();

    return returnValue;
}
int CameraController::GetDefaultGain(GainType command){
    int returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->GetDefaultGain(command);
    mutexPicture->unlock();

    return returnValue;
}

int CameraController::GetMinGain(GainType command){
    int returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->GetMinGain(command);
    mutexPicture->unlock();

    return returnValue;
}

int CameraController::GetMaxGain(GainType command){
    int returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->GetMaxGain(command);
    mutexPicture->unlock();

    return returnValue;
}

int CameraController::SetGain(GainType command, int value){
    int returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->SetGain(command,value);
    mutexPicture->unlock();

    return returnValue;
}

double CameraController::GetExposure(ExposureParameter command){
    double returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->GetExposure(command);
    mutexPicture->unlock();

    return returnValue;
}

double CameraController::GetDefaultExposure(){
    double returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->GetDefaultExposure();
    mutexPicture->unlock();

    return returnValue;

}

double CameraController::SetExposure(double value){
    double returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->SetExposure(value);
    mutexPicture->unlock();

    return returnValue;
}

int CameraController::GetPixelClock(PixelClockParameter command){
    int returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->GetPixelClock(command);
    mutexPicture->unlock();

    return returnValue;
}
int CameraController::GetDefaultPixelClock(){
    int returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->GetDefaultPixelClock();
    mutexPicture->unlock();

    return returnValue;
}
int CameraController::SetPixelClock(int value){
    int returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->SetPixelClock(value);
    mutexPicture->unlock();

    return returnValue;
}

double CameraController::GetFrameRate(FramerateParameter command){
    double returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->GetFrameRate(command);
    mutexPicture->unlock();

    return returnValue;
}
double CameraController::GetDefaultFrameRate(){
    double returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->GetDefaultFrameRate();
    mutexPicture->unlock();

    return returnValue;
}
double CameraController::SetFrameRate(double value){
    double returnValue;
    if (init==false)
        return -1;

    mutexPicture->lock();
    returnValue=camera->SetFrameRate(value);
    mutexPicture->unlock();

    return returnValue;
}

CameraParameters CameraController::SetParameters(CameraParameters value){
    cameraParameters=value;
    if(camera->IsOpen()){
        mutexPicture->lock();
        cameraParameters=camera->SetParameters(cameraParameters);
        mutexPicture->unlock();
    }
    return cameraParameters;
}

CameraParameters CameraController::GetParameters(){

    if(camera->IsOpen()){
        mutexPicture->lock();
        cameraParameters=camera->GetParameters();
        mutexPicture->unlock();
    }

    return cameraParameters;

}
