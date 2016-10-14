/**
 * @file   cameracontroller.cpp
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Implementaci�n de la clase CameraController.
 *
 */

#include "cameracontroller.h"


CameraController::CameraController(CameraType cameraType){
    buffer=NULL;

    //Sem�foros para controlar el flujo productor/consumidor
    semaphoreProducer = new QSemaphore(0);
    semaphoreConsumer = new QSemaphore(0);

    //Sem�foro para una finalizaci�n ordenada
    semaphoreEnding = new QSemaphore(0);

    //Mutex para evitar modificar o leer par�metros de la c�mara mientras se est� tomando una imagen
    mutexPicture = new QMutex();

    //Desbloqueo el sem�foro del productor
    semaphoreProducer->release();

    init =false;
    //Creo la factor�a
    switch(cameraType){
        case CAMERA_UEYE:
            cameraFactory= new CameraFactoryForUEye();
            break;
        default:
            cameraFactory= new CameraFactoryForSimulator();
    }



    //Abro la c�mara
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

    //Reestablezco los valores de los sem�foros
    delete semaphoreProducer;
    delete semaphoreConsumer;
    delete semaphoreEnding;
    delete mutexPicture;
    semaphoreProducer = new QSemaphore(0);
    semaphoreConsumer = new QSemaphore(0);
    semaphoreEnding = new QSemaphore(0);
    mutexPicture = new QMutex();
    semaphoreProducer->release();

    //Cargo los par�metros
    cameraParameters=camera->SetParameters(cameraParameters);

    init=true;
    this->start();
    return true;

}

bool CameraController::CameraIsReady(){
    //Incluso si ya est� iniciado puede que no este ah�, por lo que lo compruebo
    if(init==true){
        //Si no esta abierta, quito el hilo que recoge im�genes y elimino la c�mara
        if(camera->IsOpen()==false){
            //Libero el sem�foro de finalizar para que el hilo acabe
            semaphoreEnding->release();
            //Libero el sem�foro del productor, para asegurar que no se queda atascado en ese paso
            semaphoreProducer->release();
            //Espero a que el hilo haya acabado
            this->wait();
            init=false;
            //Y elimino la c�mara creando otra nueva
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
        //Libero el sem�foro de finalizar para que el hilo acabe
        semaphoreEnding->release();
        //Libero el sem�foro del productor, para asegurar que no se queda atascado en ese paso
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
//(La foto que se esta sacando ya estar� actualizada)
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
        //Si me han dicho que tengo que finalizar (Se est� destruyendo el controlador), salgo del bucle
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
