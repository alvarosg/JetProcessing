/**
 * @file   maincontroller.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase MainController.
 *
 */
#include "maincontroller.h"

MainController::MainController(){

    //Inicio los parámetros de la referencia
    refSourceQImage=NULL;
    sourceHMirror=false;
    sourceVMirror=false;
    sourceRotation=0;
    fullImageSizeX=1;
    fullImageSizeY=1;
    pixelsMmX=5;
    pixelsMmY=5;

    //Inicio los parámetros de visualización
    zLabel=MC_DFLTLABELZ;
    autoScaleAxes=true;
    minZValue=0;
    maxZValue=2;
    sequenceFPS=5;
    viewLevelCurves=false;
    threshold=(float)0.7;
    applyAlgorithmsLive=true;
    autoScaleAxesLive=false;
    thresholdLive=(float)0.7;
    liveFPS=5;

    currentProjectSaved=false;

    originPoint.setX(0);
    originPoint.setY(0);
    fullImageMask.setRect(0,0,1,1);
    fullImageFourierMask.setRect(0,0,1,1);

    refFullImage=NULL;
    refFullImageUpdated=false;

    refFullImageFourier=NULL;
    refFullImageFourierUpdated=false;

    refFullImageFourierAbsolute=NULL;
    refFullImageFourierAbsoluteUpdated=false;

    refFullImageInverse=NULL;
    refFullImageInverseUpdated=false;

    refPhaseImage=NULL;
    refPhaseImageScanned=NULL;
    refPhaseImageUpdated=false;

    currentProject="";

    //En función de los argumentos, lanzo el controlador para un tipo de cámara u otro
    QStringList arglist=QCoreApplication::arguments();
    if(arglist.contains("-camerasimulator",Qt::CaseInsensitive)==true ||
            arglist.contains("-cs",Qt::CaseInsensitive)==true)
        cameraController=new CameraController(CAMERA_SIMULATOR);
    else
        cameraController=new CameraController(CAMERA_UEYE);

    this->LoadPersistentSettings();

    cameraController->Initialize();

}

MainController::~MainController(){
    int i;

    this->SavePersistentSettings();

    //Libero los punteros de las imágenes guardadas
    for(i=0;i<imgController.count();i++){
        imgController.FreePointersIndex(i);
    }

    if(refSourceQImage!=NULL)
        delete refSourceQImage;

    if(refFullImage!=NULL)
        delete refFullImage;
    if(refPhaseImage!=NULL)
        delete refPhaseImage;

    delete cameraController;
    delete imageFactory;
}

void MainController::LoadPersistentSettings(){

    //Ruta de la carpeta de datos del programa
    QString path=QDesktopServices::storageLocation(QDesktopServices::DataLocation)+"/JetProcessing";

    //Cargo la configuración de la cámara
    CameraParameters camParameters;
    if(camParameters.LoadFromFile(path+"/camerasettings.ini")!=false){
        cameraController->SetParameters(camParameters);
    }

    //Cargo la configuración del programa
    programSettings.LoadFromFile(path+"/settings.ini");

}

void MainController::SavePersistentSettings(){

    //Ruta de la carpeta de datos del programa
    QString path=QDesktopServices::storageLocation(QDesktopServices::DataLocation)+"/JetProcessing";

    //Si no existe la carpeta del programa, la creo en la zona de datos de los programas
    if(QDir(path).exists()==false){
        QDir().mkdir(path);
    }

    //Guardo la configuración de la cámara
    cameraController->GetParameters().SaveToFile(path+"/camerasettings.ini");

    //Y del programa en general
    programSettings.SaveToFile(path+"/settings.ini");

}


QString MainController::GetOutputFolder(){
    return programSettings.GetOutputFolder();
}

QString MainController::GetCurrentProject(){
    return currentProject;
}

bool MainController::GetCurrentProjectSaved(){
    return currentProjectSaved;
}

QString MainController::GetZLabel(){
    return zLabel;
}

bool MainController::GetAutoScaleAxes(){
    return autoScaleAxes;
}

float MainController::GetMinZValue(){
    return minZValue;
}

float MainController::GetMaxZValue(){
    return maxZValue;
}

float MainController::GetSequenceFPS(){
    return sequenceFPS;
}

bool MainController::GetViewLevelCurves(){
    return viewLevelCurves;
}

float MainController::GetThreshold(){
    return threshold;
}

bool MainController::GetApplyAlgorithmsLive(){
    return applyAlgorithmsLive;
}

bool MainController::GetAutoScaleAxesLive(){
    return autoScaleAxesLive;
}

float MainController::GetThresholdLive(){
    return thresholdLive;
}

float MainController::GetLiveFPS(){
    return liveFPS;
}

bool MainController::GetSourceHMirror(){
    return sourceHMirror;
}

bool MainController::GetSourceVMirror(){
    return sourceVMirror;
}

int MainController::GetSourceRotation(){
    return sourceRotation;
}

int MainController::GetFullImageSizeX(){
    return fullImageSizeX;
}

int MainController::GetFullImageSizeY(){
    return fullImageSizeY;
}

float MainController::GetPixelsMmX(){
    return pixelsMmX;
}

float MainController::GetPixelsMmY(){
    return pixelsMmY;
}

QPoint MainController::GetOriginPoint(){
    return originPoint;
}

QRect MainController::GetFullImageMask(){
    return fullImageMask;
}

QRect MainController::GetFullImageFourierMask(){
    return fullImageFourierMask;
}

QImage * MainController::GetRefFullImageFourier(int gain){
    this->CalculateRefFullImageFourierAbsolute(gain);
    return refFullImageFourierAbsolute;

}

Data2D * MainController::GetRefPhaseData2D(){
    Data2D * data2d;

    data2d = new Data2D();
    this->CalculateRefPhaseImage();

    data2d->SetLabelZ(zLabel);
    data2d->SetLabelX("Distance X (mm)");
    data2d->SetLabelY("Distance Y (mm)");
    data2d->SetMinXValue((fullImageMask.left()-originPoint.x())/pixelsMmX);
    data2d->SetMaxXValue((fullImageMask.right()-originPoint.x())/pixelsMmX);
    data2d->SetMinYValue(-(fullImageMask.bottom()-originPoint.y())/pixelsMmY);
    data2d->SetMaxYValue(-(fullImageMask.top()-originPoint.y())/pixelsMmY);

    if(refPhaseImage!=NULL)
        data2d=refPhaseImage->GetData2D(data2d);

    return data2d;
}

AlgorithmSequenceController MainController::GetAlgorithmSequenceController(){
    return algController;
}

ImageSequenceController MainController::GetImageSequenceController(){
    return imgController;
}

Data2D * MainController::GetPhaseData2D(int index,int nAlg){

    Data2D * data2d;
    QRect rect;
    if(index >= imgController.count() || index < 0){
        data2d = new Data2D();
    }
    else{

        CalculatePhaseImage(index,nAlg);
        //Si no es nulo, hago una copia
        if(imgController.GetData2DIndex(index)!=NULL){
            data2d= new Data2D(*(imgController.GetData2DIndex(index)));
        }
        else{
            data2d = new Data2D();
        }
    }
    data2d->SetLabelZ(zLabel);
    data2d->SetLabelX("Distance X (mm)");
    data2d->SetLabelY("Distance Y (mm)");

    rect=algController.GetMinRectangleAbsolute(nAlg);
    data2d->SetMinXValue((imgController.GetPixelsFromOriginXIndex(index)+rect.left())/imgController.GetPixelsMmXIndex(index));
    data2d->SetMaxXValue((imgController.GetPixelsFromOriginXIndex(index)+rect.left()+data2d->GetSizeX())/imgController.GetPixelsMmXIndex(index));
    data2d->SetMinYValue(-(imgController.GetPixelsFromOriginYIndex(index)+rect.top()+data2d->GetSizeY())/imgController.GetPixelsMmYIndex(index));
    data2d->SetMaxYValue(-(imgController.GetPixelsFromOriginYIndex(index)+rect.top())/imgController.GetPixelsMmYIndex(index));

    return data2d;
}

Data1D * MainController::GetPhaseData1D(int index,int nAlg){
    Data1D * data1d;
    QRect line;

    if(index >= imgController.count() || index < 0){
        data1d = new Data1D();
    }
    else{
        CalculatePhaseImage(index,nAlg);
        //Si no es nulo, hago una copia
        if(imgController.GetData1DIndex(index)!=NULL)
            data1d= new Data1D(*(imgController.GetData1DIndex(index)));
        else
            data1d = new Data1D();
    }


    data1d->SetLabelZ(zLabel);
    data1d->SetLabelX("Distance X (mm)");

    //En función de la línea, calculo el número adecuado de pixels por mm
    line=algController.GetLineAbsolute();
    float pixelsMm=(line.width()*imgController.GetPixelsMmXIndex(index) +line.height()*imgController.GetPixelsMmYIndex(index));
    pixelsMm=pixelsMm/sqrt((float)(line.width()*line.width()+line.height()*line.height()));

    //Escribo las coordenadas de inicio y de fin
    data1d->SetInitXPoint((imgController.GetPixelsFromOriginXIndex(index)+line.left())/imgController.GetPixelsMmXIndex(index));
    data1d->SetEndXPoint((imgController.GetPixelsFromOriginXIndex(index)+line.right())/imgController.GetPixelsMmXIndex(index));
    data1d->SetInitYPoint(-(imgController.GetPixelsFromOriginYIndex(index)+line.top())/imgController.GetPixelsMmYIndex(index));
    data1d->SetEndYPoint(-(imgController.GetPixelsFromOriginYIndex(index)+line.bottom())/imgController.GetPixelsMmYIndex(index));


    data1d->SetMinXValue((float)(-data1d->GetSizeX())/pixelsMm/2.0);
    data1d->SetMaxXValue((float)(data1d->GetSizeX())/pixelsMm/2.0);
    return data1d;
}

QImage MainController::GetPhaseQImage(int index,int nAlg){
    QImage image;

    if(index>= imgController.count())
        return image;

    this->CalculatePhaseImage(index,nAlg);


    Data2D * data2d=imgController.GetData2DIndex(index);

    SpectrogramPlot * spectrogram;
    spectrogram = new SpectrogramPlot(NULL);
    spectrogram->SetData2D(data2d);
    spectrogram->enableAxis(0,false);
    spectrogram->enableAxis(1,false);
    spectrogram->enableAxis(2,false);
    spectrogram->setFixedSize(data2d->GetSizeX(),data2d->GetSizeY());
    image=spectrogram->GetAsQPixmap()->toImage();
    image=image.scaled(data2d->GetSizeX(),data2d->GetSizeY());

    delete spectrogram;
    return image;
}

void MainController::SetOutputFolder(QString value){
    programSettings.SetOutputFolder(value);
}

bool MainController::SaveFullImageMaskParameters(QString filePath){

    QSettings * settings;
    settings=new QSettings(filePath,QSettings::IniFormat);

    settings->setValue("OriginX",originPoint.x());
    settings->setValue("OriginY",originPoint.y());
    settings->setValue("x1", fullImageMask.left());
    settings->setValue("x2", fullImageMask.right());
    settings->setValue("y1",fullImageMask.top());
    settings->setValue("y2",fullImageMask.bottom());

    if(settings->status()!=QSettings::NoError){
        delete settings;
        return false;
    }
    delete settings;
    return true;
}

bool MainController::SaveFullImageMaskFourierParameters(QString filePath){

    QSettings * settings;
    settings=new QSettings(filePath,QSettings::IniFormat);

    settings->setValue("x1", fullImageFourierMask.left());
    settings->setValue("x2", fullImageFourierMask.right());
    settings->setValue("y1",fullImageFourierMask.top());
    settings->setValue("y2",fullImageFourierMask.bottom());

    if(settings->status()!=QSettings::NoError){
        delete settings;
        return false;
    }
    delete settings;
    return true;
}

bool MainController::SaveFullImageMaskOperationsAndSize(QString filePath){

    QSettings * settings;

    settings=new QSettings(filePath,QSettings::IniFormat);
    settings->setValue("SourceHMirror",sourceHMirror);
    settings->setValue("SourceVMirror",sourceVMirror);
    settings->setValue("SourceRotation",sourceRotation);
    settings->setValue("FullImageSizeX",fullImageSizeX);
    settings->setValue("FullImageSizeY",fullImageSizeY);
    settings->setValue("PixelsMmX",pixelsMmX);
    settings->setValue("PixelsMmY",pixelsMmY);

    if(settings->status()!=QSettings::NoError){
        delete settings;
        return false;
    }
    delete settings;
    return true;
}


bool MainController::SaveViewOptions(QString filePath){
    QSettings * settings;

    settings=new QSettings(filePath,QSettings::IniFormat);

    settings->setValue("ZLabel",zLabel);
    settings->setValue("AutoScaleAxes",autoScaleAxes);
    settings->setValue("MinZValue",minZValue);
    settings->setValue("MaxZValue",maxZValue);
    settings->setValue("SequenceFPS",sequenceFPS);
    settings->setValue("ViewLevelCurves",viewLevelCurves);
    settings->setValue("Threshold",threshold);
    settings->setValue("ApplyAlgorithmsLive",applyAlgorithmsLive);
    settings->setValue("AutoScaleAxesLive",autoScaleAxesLive);
    settings->setValue("ThresholdLive",thresholdLive);
    settings->setValue("LiveFPS",liveFPS);

    if(settings->status()!=QSettings::NoError){
        delete settings;
        return false;
    }
    delete settings;
    return true;
}

bool MainController::LoadViewOptions(QString filePath){
    QSettings * settings;
    settings=new QSettings(filePath,QSettings::IniFormat);

    zLabel=settings->value("ZLabel",MC_DFLTLABELZ).toString();
    autoScaleAxes=settings->value("AutoScaleAxes",true).toBool();
    minZValue=settings->value("MinZValue",0).toFloat();
    maxZValue=settings->value("MaxZValue",2).toFloat();
    sequenceFPS=settings->value("SequenceFPS",5).toFloat();
    viewLevelCurves=settings->value("ViewLevelCurves",false).toBool();
    threshold=settings->value("Threshold",0.7).toFloat();
    applyAlgorithmsLive=settings->value("ApplyAlgorithmsLive",true).toBool();
    autoScaleAxesLive=settings->value("AutoScaleAxesLive",false).toBool();
    thresholdLive=settings->value("ThresholdLive",0.7).toFloat();
    liveFPS=settings->value("LiveFPS",5).toFloat();

    if(settings->status()!=QSettings::NoError){
        delete settings;
        return false;
    }
    delete settings;
    currentProjectSaved=false;
    return true;
}

bool MainController::LoadFullImageMaskOperationsAndSize(QString filePath){

    QSettings * settings;
    settings=new QSettings(filePath,QSettings::IniFormat);

    sourceHMirror=settings->value("SourceHMirror",false).toBool();
    sourceVMirror=settings->value("SourceVMirror",false).toBool();
    sourceRotation=settings->value("SourceRotation",true).toInt();
    fullImageSizeX=settings->value("FullImageSizeX",1024).toInt();
    fullImageSizeY=settings->value("FullImageSizeY",1024).toInt();
    pixelsMmX=settings->value("PixelsMmX",1).toFloat();
    pixelsMmY=settings->value("PixelsMmY",1).toFloat();

    refFullImageUpdated=false;
    fullImageSizeX=FullImage::NextPowerOf2(fullImageSizeX);
    fullImageSizeY=FullImage::NextPowerOf2(fullImageSizeY);

    if(settings->status()!=QSettings::NoError){
        delete settings;
        return false;
    }
    delete settings;
    currentProjectSaved=false;
    return true;

}

bool MainController::LoadFullImageMaskParameters(QString filePath){
    QRect rect;
    QPoint point;

    QSettings * settings;
    settings=new QSettings(filePath,QSettings::IniFormat);

    rect.setLeft(settings->value("x1",0).toInt());
    rect.setRight(settings->value("x2",200).toInt());
    rect.setTop(settings->value("y1",0).toInt());
    rect.setBottom(settings->value("y2",200).toInt());
    point.setX(settings->value("OriginX",0).toInt());
    point.setY(settings->value("OriginY",0).toInt());

    if(settings->status()!=QSettings::NoError){
        delete settings;
        return false;
    }
    delete settings;

    fullImageMask=rect;
    originPoint=point;
    currentProjectSaved=false;
    return true;
}

bool MainController::LoadFullImageMaskFourierParameters(QString filePath){
    QRect rect;

    QSettings * settings;
    settings=new QSettings(filePath,QSettings::IniFormat);

    rect.setLeft(settings->value("x1",0).toInt());
    rect.setRight(settings->value("x2",200).toInt());
    rect.setTop(settings->value("y1",0).toInt());
    rect.setBottom(settings->value("y2",200).toInt());

    if(settings->status()!=QSettings::NoError){
        delete settings;
        return false;
    }
    delete settings;

    fullImageFourierMask=rect;
    currentProjectSaved=false;
    return true;
}

void MainController::SetZLabel(QString value){
    if(zLabel!=value){
        zLabel=value;
        currentProjectSaved=false;
    }
}
void MainController::SetAutoScaleAxes(bool value){
    if(autoScaleAxes!=value){
        autoScaleAxes=value;
        currentProjectSaved=false;
    }
}
void MainController::SetMinZValue(float value){
    if(minZValue!=value){
        minZValue=value;
        currentProjectSaved=false;
    }
}
void MainController::SetMaxZValue(float value){
    if(maxZValue!=value){
        maxZValue=value;
        currentProjectSaved=false;
    }
}
void MainController::SetSequenceFPS(float value){
    if(sequenceFPS!=value){
        sequenceFPS=value;
        currentProjectSaved=false;
    }
}
void MainController::SetViewLevelCurves(bool value){
    if(viewLevelCurves!=value){
        viewLevelCurves=value;
        currentProjectSaved=false;
    }
}
void MainController::SetThreshold(float value){
    if(threshold!=value){
        threshold=value;
        currentProjectSaved=false;
    }
}
void MainController::SetApplyAlgorithmsLive(bool value){
    if(applyAlgorithmsLive!=value){
        applyAlgorithmsLive=value;
        currentProjectSaved=false;
    }
}
void MainController::SetAutoScaleAxesLive(bool value){
    if(autoScaleAxesLive!=value){
        autoScaleAxesLive=value;
        currentProjectSaved=false;
    }
}
void MainController::SetThresholdLive(float value){
    if(thresholdLive!=value){
        thresholdLive=value;
        currentProjectSaved=false;
    }
}
void MainController::SetLiveFPS(float value){
    if(liveFPS!=value){
        liveFPS=value;
        currentProjectSaved=false;
    }
}

void MainController::SetSourceHMirror(bool value){
    if(sourceHMirror!=value){
        refFullImageUpdated=false;
        sourceHMirror=value;
        currentProjectSaved=false;
    }
}

void MainController::SetSourceVMirror(bool value){
    if(sourceVMirror!=value){
        refFullImageUpdated=false;
        sourceVMirror=value;
        currentProjectSaved=false;
    }
}

int MainController::SetSourceRotation(int value){
    if(sourceRotation!=value){
        refFullImageUpdated=false;
        sourceRotation=value;
        currentProjectSaved=false;
    }
    return sourceRotation;

}

int MainController::SetFullImageSizeX(int value){
    if(fullImageSizeX!=value){
        refFullImageUpdated=false;
        fullImageSizeX=FullImage::NextPowerOf2(value);
        currentProjectSaved=false;
    }
    return fullImageSizeX;

}

int MainController::SetFullImageSizeY(int value){
    if(fullImageSizeY!=value){
        refFullImageUpdated=false;
        fullImageSizeY=FullImage::NextPowerOf2(value);
        currentProjectSaved=false;
    }
    return fullImageSizeY;
}

float MainController::SetPixelsMmX(float value){
    if(pixelsMmX!=value){
        pixelsMmX=value;
        currentProjectSaved=false;
    }
    return pixelsMmX;
}

float MainController::SetPixelsMmY(float value){
    if(pixelsMmY!=value){
        pixelsMmY=value;
        currentProjectSaved=false;
    }
    return pixelsMmY;
}

void MainController::SetOriginPoint(QPoint value) {
    if(originPoint!=value){
        originPoint=value;
        currentProjectSaved=false;
    }
}

void MainController::SetFullImageMask(QRect value){
    if(fullImageMask!=value){
        refFullImageFourierUpdated=false;
        fullImageMask=value;
        currentProjectSaved=false;
    }
}

void MainController::SetFullImageFourierMask(QRect value){
    if(fullImageFourierMask!=value){
        refFullImageInverseUpdated=false;
        fullImageFourierMask=value;
        currentProjectSaved=false;
    }
}

void MainController::SetAlgorithmSequenceController(AlgorithmSequenceController value, bool unSave){
    algController=value;
    algController.CorrectAlgorithms();
    //Como hemos cambiado los algoritmos, desactualizamos la secuencia de imágenes
    int i;
    for(i=0;i<imgController.count();i++){
        imgController.SetUpdatedIndex(i,false);
    }
    if(unSave==true)
        currentProjectSaved=false;
}

void MainController::SetImageSequenceController(ImageSequenceController value, bool unSave){
    imgController=value;
    imgController.CheckLabels();
    if(unSave==true)
        currentProjectSaved=false;
}



bool MainController::LoadRefImageFromFile(QString filePath){
    QImage * auxImage;

    auxImage=new QImage(filePath);
    if(auxImage->isNull()==true){
        delete auxImage;
        return false;
    }
    if(refSourceQImage!=NULL)
        delete refSourceQImage;
    refSourceQImage=auxImage;
    SetFullImageSizeX(refSourceQImage->width());
    SetFullImageSizeY(refSourceQImage->height());
    refFullImageUpdated=false;
    currentProjectSaved=false;
    return true;
}

bool MainController::LoadRefImageFromCamera(){
    QImage * auxImage;
    cameraController->RenewPicture();
    auxImage=cameraController->GetSourcePicture();
    if(auxImage==NULL){
        return false;
    }
    this->SavePictureToFileDated(auxImage,"_ref");
    if(refSourceQImage!=NULL)
        delete refSourceQImage;
    refSourceQImage=auxImage;
    SetFullImageSizeX(refSourceQImage->width());
    SetFullImageSizeY(refSourceQImage->height());
    refFullImageUpdated=false;
    currentProjectSaved=false;
    return true;
}

QImage * MainController::GetRefSourceQImage(){
    return refSourceQImage;
}

void MainController::CalculateRefFullImage(){
    QTransform transform;
    QImage transformed;
    if(refFullImageUpdated==false){
        if(refSourceQImage!=NULL){
            if(refFullImage!=NULL)
                delete refFullImage;
            transformed = refSourceQImage->mirrored(sourceHMirror,sourceVMirror);
            transform.rotate(90*sourceRotation);
            transformed = transformed.transformed(transform);
            refFullImage = imageFactory->CreateFullImage(&transformed,fullImageSizeX,fullImageSizeY);
            refFullImageUpdated=true;
        }

        refFullImageFourierUpdated=false;

    }

}

void MainController::CalculateRefFullImageFourier(){
    this->CalculateRefFullImage();
    if(refFullImageFourierUpdated==false){        
        if(refFullImage!=NULL){
            if(refFullImageFourier!=NULL)
                delete refFullImageFourier;
            refFullImageFourier = imageFactory->CopyFullImage(refFullImage);
            refFullImageFourier->ApplyMask(fullImageMask);
            refFullImageFourier->ComplexFFT(FFT_FORWARD);
            refFullImageFourier->ReArrangeFT();
            refFullImageFourierUpdated=true;
        }

        refFullImageFourierAbsoluteUpdated=false;
        refFullImageInverseUpdated=false;

    }
}

void MainController::CalculateRefFullImageFourierAbsolute(int gainIn){
    static int gain=1;
    this->CalculateRefFullImageFourier();
    if(refFullImageFourierAbsoluteUpdated==false || gain!=gainIn){
        if(refFullImageFourier!=NULL){
            gain=gainIn;
            if(refFullImageFourierAbsolute!=NULL)
                delete refFullImageFourierAbsolute;
            refFullImageFourierAbsolute=refFullImageFourier->GetAbsoluteValueAsQImage(gain);

            refFullImageFourierAbsoluteUpdated=true;
        }
    }
}

void MainController::CalculateRefFullImageInverse(){
    this->CalculateRefFullImageFourier();
    if(refFullImageInverseUpdated==false){
        if(refFullImageFourier!=NULL){
            if(refFullImageInverse!=NULL)
                delete refFullImageInverse;
            refFullImageInverse = imageFactory->CopyFullImage(refFullImageFourier);
            refFullImageInverse->ApplyMask(fullImageFourierMask);
            refFullImageInverse->ReArrangeFT();
            refFullImageInverse->ComplexFFT(FFT_INVERSE);
            refFullImageInverseUpdated=true;
        }

        refPhaseImageUpdated=false;

    }
}

void MainController::CalculateRefPhaseImage(){
    this->CalculateRefFullImageInverse();
    if(refPhaseImageUpdated==false){
        if(refFullImageInverse!=NULL){
            if(refPhaseImage!=NULL)
                delete refPhaseImage;
            refPhaseImage=refFullImageInverse->CropPhase(fullImageMask);
            if(refPhaseImageScanned!=NULL)
                delete refPhaseImageScanned;
            refPhaseImageScanned=imageFactory->CopyPhaseImage(refPhaseImage);
            refPhaseImageScanned->ScanPhase();
            refPhaseImageUpdated=true;
        }
    }
}


ProgramSettings MainController::GetProgramSettings(){
    return programSettings;
}

void MainController::SetProgramSettings(ProgramSettings settings){
    programSettings=settings;
}

bool MainController::GetFirstExecution(){
    return programSettings.GetFirstExecution();
}

void MainController::SetMode(CalcMode mode){
    programSettings.SetMode(mode);
    if(IsCudaAvailable()==false){
        imageFactory = new ImageFactoryForCPU();
        programSettings.SetMode(MODE_CPU);
        return;
    }
    switch(programSettings.GetMode()){
        case MODE_CUDA: imageFactory = new ImageFactoryForCUDA(); break;
        default:        imageFactory = new ImageFactoryForCPU();
    }
}

CalcMode MainController::GetMode(){
    if(IsCudaAvailable()==true)
        return programSettings.GetMode();
    else
        return MODE_CPU;
}

void MainController::SetDefaultMode(bool value){
    programSettings.SetUseDefaultMode(value);
}

bool MainController::GetDefaultMode(){
    if(IsCudaAvailable()==true)
        return programSettings.GetUseDefaultMode();
    else
        //Si cuda no está disponible, devuelve verdadero, pues sí hay modo por defecto (CPU)
        return true;
}

bool MainController::GetApplyUntilCurrentAlgorithm(){
    return programSettings.GetApplyUntilCurrentAlgorithm();
}

bool MainController::GetSaveGraphicsViewSize(){
    return programSettings.GetSaveGraphicsViewSize();
}

QSize MainController::GetSaveGraphicsCustomSize(){
    return programSettings.GetCustomSize();
}

QString MainController::GetSaveGraphicsDefaultFormat(){
    return programSettings.GetDefaultImageFormat();
}

bool MainController::AddImageFromFile(QString filePath){
    PhaseImage * phaseImage;
    QImage auxImage(filePath);

    if(auxImage.isNull()==true){
        return false;
    }
    //Proceso la imagen
    phaseImage=this->ProcessSourcePictureImage(auxImage);

    //Y la añado con el nombre del archivo
    QFileInfo pathInfo(filePath);
    this->AddPhaseImage(phaseImage,pathInfo.baseName());
    currentProjectSaved=false;
    return true;
}

bool MainController::AddImageFromCamera(){
    PhaseImage * phaseImage;
    cameraController->RenewPicture();
    QImage * auxImage=cameraController->GetSourcePicture();;

    if(auxImage==NULL || auxImage->isNull()==true){
        delete auxImage;
        return false;
    }
    //Proceso la imagen
    phaseImage=this->ProcessSourcePictureImage(*auxImage);

    //Y la añado con la etiqueta de la la fecha
    QDateTime dateTime;
    dateTime=QDateTime::currentDateTime();

    this->AddPhaseImage(phaseImage,dateTime.toString("yyyy-MM-dd_hh-mm-ss-zzz"));

    this->SavePictureToFileDated(auxImage);


    delete auxImage;
    currentProjectSaved=false;
    return true;
}

Data2D * MainController::ProcessImageFromCamera(bool save,bool applyAlg){

    PhaseImage * phaseImage;
    //Tomo la imagen
    QImage * auxImage=cameraController->GetSourcePicture();

    if(auxImage==NULL || auxImage->isNull()==true){
        delete auxImage;
        return new Data2D();
    }

    //Proceso la imagen
    phaseImage=this->ProcessSourcePictureImage(*auxImage);

    //Si hay que guardar la fase escribo la etiqueta y la guardo, además guardo la foto original
    if(save==true){
        //Y la añado con la etiqueta de la la fecha
        QDateTime dateTime;
        dateTime=QDateTime::currentDateTime();
        this->AddPhaseImage(phaseImage,dateTime.toString("yyyy-MM-dd_hh-mm-ss-zzz"));

        //Y me quedo con una copia para aplicar los algoritmos
        phaseImage=imageFactory->CopyPhaseImage(phaseImage);

        //Guardo la imagen de la cámara
        this->SavePictureToFileDated(auxImage);

    }
    delete auxImage;

    //Si hay que aplicar los algoritmos hasta encontrar la línea, se los aplico
    int i;
    if(applyAlg==true){
        for(i=0;i<algController.count();i++){
            switch(algController.GetAlgorithmIndex(i)){
                case ALG_INVERSE: phaseImage->AdditiveInverse();                                                                    break;
                case ALG_RMHTILT: phaseImage->RemoveHorizontalLinearTilt();                                                         break;
                case ALG_RMVTILT: phaseImage->RemoveVerticalLinearTilt();                                                           break;
                case ALG_SETGROUND: phaseImage->SetGround();                                                                        break;
                case ALG_OFFSET: phaseImage->OffsetAndMultiply(algController.GetAuxFloatIndex(i),1);                                  break;
                case ALG_MULTIPLY: phaseImage->OffsetAndMultiply(0,algController.GetAuxFloatIndex(i));                                break;
                case ALG_SYMMETRIC: phaseImage->MakeSymmetric();                                                                    break;
                case ALG_ABELINV:phaseImage->AbelInversion(1/pixelsMmX/1000,algController.GetAuxIntIndex(i));   break;
                case ALG_RECTANGLE: phaseImage->CropRectangle(algController.GetAuxRectIndex(i));                                      break;
                //Cuando encuentro la línea fuerzo una salida del bucle
                case ALG_GETLINE: i=algController.count(); break;
            }
        }
    }


    //Preparo el archivo de datos en dos dimensiones
    Data2D * data2d;
    QRect rect;

    //Obtengo los datos
    data2d=phaseImage->GetData2D();

    //Inicio el rectángulo, o bien al tamaño d el amáscara, si no estoy aplicando los algoritmos,
    //o al tamaño del rectángulo más pequeño (Devuelto con GetRectangle)
    if(applyAlg==true)
        rect=algController.GetMinRectangleAbsolute();
    else
        rect.setCoords(0,0,data2d->GetSizeX(),data2d->GetSizeY());

    data2d->SetLabelZ(zLabel);
    data2d->SetLabelX("Distance X (mm)");
    data2d->SetLabelY("Distance Y (mm)");

    data2d->SetMinXValue((fullImageMask.left()-originPoint.x()+rect.left())/pixelsMmY);
    data2d->SetMaxXValue((fullImageMask.left()-originPoint.x()+rect.left()+data2d->GetSizeX())/pixelsMmY);
    data2d->SetMinYValue(-(fullImageMask.top()-originPoint.y()+rect.top()+data2d->GetSizeY())/pixelsMmY);
    data2d->SetMaxYValue(-(fullImageMask.top()-originPoint.y()+rect.top())/pixelsMmY);


    //Elimino la imagen de la phase
    delete phaseImage;

    return data2d;
}

Data1D * MainController::ProcessImageFromCamera1D(bool save){

    PhaseImage * phaseImage;
    PhaseLine * phaseLine=NULL;
    //Tomo la imagen
    QImage * auxImage=cameraController->GetSourcePicture();

    if(auxImage==NULL || auxImage->isNull()==true){
        delete auxImage;
        return new Data1D();
    }
    //Proceso la imagen
    phaseImage=this->ProcessSourcePictureImage(*auxImage);

    //Si hay que guardar la fase escribo la etiqueta y la guardo, además guardo la foto original
    if(save==true){
        //Y la añado con la etiqueta de la la fecha
        QDateTime dateTime;
        dateTime=QDateTime::currentDateTime();
        this->AddPhaseImage(phaseImage,dateTime.toString("yyyy-MM-dd_hh-mm-ss-zzz"));

        //Y me quedo con una copia para aplicar los algoritmos
        phaseImage=imageFactory->CopyPhaseImage(phaseImage);

        //Guardo la imagen de la cámara
        this->SavePictureToFileDated(auxImage);

    }
    delete auxImage;

    //Aplico los algoritmos
    bool lineFlag=false;
    int i;
    for(i=0;i<algController.count();i++){
        switch(algController.GetAlgorithmIndex(i)){
            case ALG_INVERSE:
                if(lineFlag==false) phaseImage->AdditiveInverse();
                else                phaseLine->AdditiveInverse();
                break;
            case ALG_RMHTILT:
                if(lineFlag==false) phaseImage->RemoveHorizontalLinearTilt();
                else                phaseLine->RemoveLinearTilt();
                break;
            case ALG_RMVTILT:
                if(lineFlag==false) phaseImage->RemoveVerticalLinearTilt();
                break;
            case ALG_SETGROUND:
                if(lineFlag==false) phaseImage->SetGround();
                else                phaseLine->SetGround();
                break;
            case ALG_OFFSET:
                if(lineFlag==false) phaseImage->OffsetAndMultiply(algController.GetAuxFloatIndex(i),1);
                else                phaseLine->OffsetAndMultiply(algController.GetAuxFloatIndex(i),1);
                break;
            case ALG_MULTIPLY:
                if(lineFlag==false) phaseImage->OffsetAndMultiply(0,algController.GetAuxFloatIndex(i));
                else                phaseLine->OffsetAndMultiply(0,algController.GetAuxFloatIndex(i));
                break;
            case ALG_SYMMETRIC:
                if(lineFlag==false) phaseImage->MakeSymmetric();
                else                phaseLine->MakeSymmetric();
                break;
            case ALG_ABELINV:
                if(lineFlag==false) phaseImage->AbelInversion(1/pixelsMmX/1000,algController.GetAuxIntIndex(i));
                else                phaseLine->AbelInversion(1/pixelsMmX/1000,algController.GetAuxIntIndex(i));
                break;
            case ALG_RECTANGLE:
                if(lineFlag==false) phaseImage->CropRectangle(algController.GetAuxRectIndex(i));
                break;
            case ALG_GETLINE:
                if(lineFlag==false) phaseLine=phaseImage->GetPhaseLine(algController.GetAuxRectIndex(i));
                lineFlag=true;
                break;
        }
    }


    //Preparo el archivo de datos en dos dimensiones
    Data1D * data1d;
    QRect line;

    //Obtengo los datos
    if(phaseLine!=NULL){
        data1d=phaseLine->GetData1D();
        //Obtengo la línea que he aplicado
        line=algController.GetLineAbsolute();
        delete phaseLine;
    }
    else{
        data1d=new Data1D();
        line.setCoords(0,0,10,10);
    }

    //Pongo las etiquetas
    data1d->SetLabelZ(zLabel);
    data1d->SetLabelX("Distance X (mm)");

    float pixelsMm=(line.width()*pixelsMmX +line.height()*pixelsMmY);
    pixelsMm=pixelsMm/sqrt((float)(line.width()*line.width()+line.height()*line.height()));

    //Escribo las coordenadas de inicio y de fin
    data1d->SetInitXPoint((fullImageMask.left()-originPoint.x()+line.left())/pixelsMmX);
    data1d->SetEndXPoint((fullImageMask.left()-originPoint.x()+line.right())/pixelsMmX);
    data1d->SetInitYPoint(-(fullImageMask.top()-originPoint.y()+line.top())/pixelsMmY);
    data1d->SetEndYPoint(-(fullImageMask.top()-originPoint.y()+line.bottom())/pixelsMmY);


    //Y valores máximos y mínimos
    data1d->SetMinXValue((float)(-data1d->GetSizeX())/pixelsMm/2.0);
    data1d->SetMaxXValue((float)(data1d->GetSizeX())/pixelsMm/2.0);

    //Elimino la imagen de la fase
    delete phaseImage;

    return data1d;
}

void MainController::SavePictureToFileDated(QImage * image,QString extraLabel){

    QDateTime dateTime;
    if(programSettings.GetSaveCameraPictures()==true){
        dateTime=QDateTime::currentDateTime();

        //Guardo la imagen
        //Compruebo si existe la carpeta, y si no, la creo
        if(QDir(programSettings.GetOutputFolder()+"/CameraPics").exists()==false){
            QDir().mkdir(programSettings.GetOutputFolder()+"/CameraPics");
        }
        QString outputName=programSettings.GetOutputFolder()+"/CameraPics";
        outputName=outputName+"/"+dateTime.toString("yyyy-MM-dd_hh-mm-ss-zzz")+extraLabel;
        outputName=outputName+".bmp";
        image->save(outputName);
    }

}

PhaseImage * MainController::ProcessSourcePictureImage(QImage auxImage){
    FullImage * fullImage;
    PhaseImage * phaseImage;
    //Aseguro que está actualizada la imagen de la phase
    //(Normalmente esta llamada no llevará nada de tiempo)
    this->CalculateRefPhaseImage();

    //Hago todos los cálculos, hasta la resta
    QTransform transform;

    if(sourceHMirror==true || sourceVMirror==true)
        auxImage = auxImage.mirrored(sourceHMirror,sourceVMirror);
    if(sourceRotation!=0){
        transform.rotate(90*sourceRotation);
        auxImage = auxImage.transformed(transform);
    }
    fullImage = imageFactory->CreateFullImage(&auxImage,fullImageSizeX,fullImageSizeY);
    fullImage->ApplyMask(fullImageMask);
    fullImage->ComplexFFT(FFT_FORWARD);
    fullImage->ReArrangeFT();
    fullImage->ApplyMask(fullImageFourierMask);
    fullImage->ReArrangeFT();
    fullImage->ComplexFFT(FFT_INVERSE);
    phaseImage=fullImage->CropPhase(fullImageMask);
    delete fullImage;
    phaseImage->ScanPhase();
    phaseImage->SubtractPhase(refPhaseImageScanned);


    return phaseImage;
}

void MainController::AddPhaseImage(PhaseImage * phaseImage, QString label){

    //Añado la imagen de la phase a la lista
    imgController.AppendImageElement(phaseImage,label,pixelsMmX,pixelsMmY
                                     ,fullImageMask.left()-originPoint.x(),fullImageMask.top()-originPoint.y());
    imgController.CheckLabels();

    currentProjectSaved=false;
}

void MainController::DeleteImage(int index){
    if(index >= imgController.count() || index < 0){
        return;
    }
    imgController.FreePointersIndex(index);
    imgController.removeAt(index);
    currentProjectSaved=false;
}


bool MainController::CameraIsReady(){

    //Si no estaba preparada, volvemos a intentar cargar y devolvemos lo que obtengamos al cargar
    if(cameraController->CameraIsReady()==false){
        return cameraController->Initialize();
    }


    return true;
}

CameraController * MainController::GetCameraController(){
    return cameraController;
}

void MainController::CalculatePhaseImage(int index,int nAlg){
    int i;
    bool update=false;
    bool lineFlag=false;

    PhaseImage * source=imgController.GetPhaseImageSourceIndex(index);
    PhaseImage * phaseImage = imageFactory->CopyPhaseImage(source);
    PhaseLine * phaseLine=NULL;
    //Si está actualizada y me están pidiendo calcular todo, salgo sin calcular nada, pues ya lo está
    if (imgController.GetUpdatedIndex(index)==true && (nAlg==-1 || nAlg==algController.count()))
        return;
    //Recorro la fase calculando el número de algoritmos que s eme indique como parámetro
    //Siempre comieno aplicándolo a la imagen 2D. En el caso en el que se extraiga la línea, activo el flag

    if(nAlg<0 || nAlg>=algController.count()){
        nAlg=algController.count();
        //Sólo lo marco como actualizado si estoy calculando todos los algoritmos
        update=true;
    }
    for(i=0;i<nAlg;i++){
        switch(algController.GetAlgorithmIndex(i)){
            case ALG_INVERSE:
                if(lineFlag==false) phaseImage->AdditiveInverse();
                else                phaseLine->AdditiveInverse();
                break;
            case ALG_RMHTILT:
                if(lineFlag==false) phaseImage->RemoveHorizontalLinearTilt();
                else                phaseLine->RemoveLinearTilt();
                break;
            case ALG_RMVTILT:
                if(lineFlag==false) phaseImage->RemoveVerticalLinearTilt();
                break;
            case ALG_SETGROUND:
                if(lineFlag==false) phaseImage->SetGround();
                else                phaseLine->SetGround();
                break;
            case ALG_OFFSET:
                if(lineFlag==false) phaseImage->OffsetAndMultiply(algController.GetAuxFloatIndex(i),1);
                else                phaseLine->OffsetAndMultiply(algController.GetAuxFloatIndex(i),1);
                break;
            case ALG_MULTIPLY:
                if(lineFlag==false) phaseImage->OffsetAndMultiply(0,algController.GetAuxFloatIndex(i));
                else                phaseLine->OffsetAndMultiply(0,algController.GetAuxFloatIndex(i));
                break;
            case ALG_SYMMETRIC:
                if(lineFlag==false) phaseImage->MakeSymmetric();
                else                phaseLine->MakeSymmetric();
                break;
            case ALG_ABELINV:
                if(lineFlag==false) phaseImage->AbelInversion(1/imgController.GetPixelsMmXIndex(index)/1000,algController.GetAuxIntIndex(i));
                else                phaseLine->AbelInversion(1/imgController.GetPixelsMmXIndex(index)/1000,algController.GetAuxIntIndex(i));
                break;
            case ALG_RECTANGLE:
                if(lineFlag==false) phaseImage->CropRectangle(algController.GetAuxRectIndex(i));
                break;
            case ALG_GETLINE:
                if(lineFlag==false) phaseLine=phaseImage->GetPhaseLine(algController.GetAuxRectIndex(i));
                lineFlag=true;
                break;
        }
    }


    imgController.SetPhaseImageIndex(index,phaseImage);
    imgController.SetPhaseLineIndex(index,phaseLine);
    imgController.SetUpdatedIndex(index,update);

}


bool MainController::LoadProject(QString filePath){


    //Creo un fichero temporal
    QString temporaryPath=QDir::tempPath();
    QString temporaryFolder="JetProcessingFolderLoading";
    temporaryFolder=temporaryPath+"/"+temporaryFolder;
    if(QDir(temporaryFolder).exists()==false){
        QDir().mkdir(temporaryFolder);
    }

    //Vacío la carpeta temporal todos los archivos por si habia algo
    QDir tempDir(temporaryFolder);
    Q_FOREACH(QFileInfo info, tempDir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
        if (!info.isDir()) {
            QFile::remove(info.absoluteFilePath());
        }
    }

    //Extraigo el archivo de proyecto en un fichero temporal
    if(ZipAux::uncompressFolder(filePath,temporaryFolder)==false){
        //Borro los archivos temporales
        Q_FOREACH(QFileInfo info, tempDir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (!info.isDir()) {
                QFile::remove(info.absoluteFilePath());
            }
        }
        //Elimino la carpeta temporal
        QDir aux;
        aux.rmdir(temporaryFolder);
        return false;
    }


    //Cargo la imagen de la referencia
    if(refSourceQImage!=NULL){
        delete refSourceQImage;
    }
    refSourceQImage=new QImage(temporaryFolder+"/ReferenceImage.bmp");
    SetFullImageSizeX(refSourceQImage->width());
    SetFullImageSizeY(refSourceQImage->height());
    refFullImageUpdated=false;

    //Variables para el valor de retorno
    bool returnValue=true;

    //Cargo las operaciones de la imagen de referencia
    if(this->LoadFullImageMaskOperationsAndSize(temporaryFolder+"/RefOperations.ops")==false)
        returnValue=false;

    //Cargo la máscara normal
    if(this->LoadFullImageMaskParameters(temporaryFolder+"/DirectMask.msk1")==false)
        returnValue=false;

    //Cargo la máscara de fourier
    if(this->LoadFullImageMaskFourierParameters(temporaryFolder+"/FourierMask.msk2")==false)
        returnValue=false;

    //Cargo la lista de algoritmos, quitando lo que hubiera
    AlgorithmSequenceController algControllerAux;
    if(algControllerAux.LoadSequenceFromFile(temporaryFolder+"/AlgorithmList.alg")==false)
        returnValue=false;
    else
        algController=algControllerAux;

    //Cargo la lista de imágenes
    if(imgController.LoadImageList(temporaryFolder+"/ImageList.iml",imageFactory)==false)
        returnValue=false;

    //Cargo la configuración de la cámara
    CameraParameters camParameters;
    if(camParameters.LoadFromFile(temporaryFolder+"/CameraSettings.ini")==false){
        returnValue=false;
    }
    else{
        cameraController->SetParameters(camParameters);
    }

    //Cargo las opciones de visualización
    if(this->LoadViewOptions(temporaryFolder+"/ViewOptions.viw")==false)
        returnValue=false;

    //Borro los archivos temporales
    Q_FOREACH(QFileInfo info, tempDir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
        if (!info.isDir()) {
            QFile::remove(info.absoluteFilePath());
        }
    }
    //Elimino la carpeta temporal
    QDir aux;
    aux.rmdir(temporaryFolder);

    currentProject=filePath;
    currentProjectSaved=true;
    return returnValue;
}

bool MainController::SaveProject(QString filePath){

    //Guardo todos los archivos en una carpeta dentro de una carpeta temporal
    QString temporaryPath=QDir::tempPath();
    QString temporaryFolder="JetProcessingFolderSaving";
    temporaryFolder=temporaryPath+"/"+temporaryFolder;

    //Creo una carpeta temporal para el guardado en el directorio temporal
    //en el caso de que no exista ya
    if(QDir(temporaryFolder).exists()==false){
        QDir().mkdir(temporaryFolder);
    }

    //Vacío la carpeta borrando todos los archivos por si habia algo
    QDir tempDir(temporaryFolder);
    Q_FOREACH(QFileInfo info, tempDir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
        if (!info.isDir()) {
            QFile::remove(info.absoluteFilePath());
        }
    }

    //Variables para el valor de retorno
    bool returnValue=true;

    //Guardo los distintos elementos en la carpeta temporal
    //Guardo la imagen de la referencia
    if(refSourceQImage!=NULL)
        if(refSourceQImage->save(temporaryFolder+"/ReferenceImage.bmp")==false)
            returnValue=false;

    //Guardo las operaciones de la imagen de referencia
    if(this->SaveFullImageMaskOperationsAndSize(temporaryFolder+"/RefOperations.ops")==false)
        returnValue=false;

    //Guardo la máscara normal
    if(this->SaveFullImageMaskParameters(temporaryFolder+"/DirectMask.msk1")==false)
        returnValue=false;

    //Guardo la máscara de fourier
    if(this->SaveFullImageMaskFourierParameters(temporaryFolder+"/FourierMask.msk2")==false)
        returnValue=false;

    //Guardo la lista de algoritmos
    if(algController.SaveSequenceToFile(temporaryFolder+"/AlgorithmList.alg")==false)
        returnValue=false;

    //Guardo la lista de imágenes y el fichero de datos asociado
    if(imgController.SaveImageList(temporaryFolder+"/ImageList.iml")==false)
        returnValue=false;

    //Guardo la configuración de la cámara
    if(cameraController->GetParameters().SaveToFile(temporaryFolder+"/CameraSettings.ini")==false)
        returnValue=false;

    //Salvo las opciones de visualización
    if(this->SaveViewOptions(temporaryFolder+"/ViewOptions.viw")==false)
        returnValue=false;

    //Comprimo la carpeta temporal en el fichero solicitado
    if(ZipAux::compressFolder(filePath,temporaryFolder)==false)
        returnValue=false;

    //Borro los archivos temporales
    Q_FOREACH(QFileInfo info, tempDir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
        if (!info.isDir()) {
            QFile::remove(info.absoluteFilePath());
        }
    }

    //Elimino la carpeta temporal
    QDir aux;
    aux.rmdir(temporaryFolder);

    if(returnValue==true){
        currentProject=filePath;
        currentProjectSaved=true;
        return true;
    }
    else
        return false;
}

void MainController::MarkProjectUnsaved(){
    currentProjectSaved=false;
}

QString MainController::GetDefaultNameForExport2D(int imgNum){
    QString format="txt";

    QString name=imgController.GetLabelIndex(imgNum)+"_2dData."+format;

    return name;

}

QString MainController::GetDefaultNameForExport1D(int imgNum){
    QString format="txt";

    QString name=imgController.GetLabelIndex(imgNum)+"_1dData."+format;

    return name;

}

bool MainController::ExportSingleData2D(QString filePath,int imgNum,int algNum){

    Data2D * data2d;
    data2d = this->GetPhaseData2D(imgNum,algNum);
    bool returnValue=data2d->SaveToFile(filePath);
    delete data2d;
    return returnValue;

}
bool MainController::ExportSingleData1D(QString filePath,int imgNum,int algNum){
    Data1D * data1d;
    data1d = this->GetPhaseData1D(imgNum,algNum);
    bool returnValue=data1d->SaveToFile(filePath);
    delete data1d;
    return returnValue;

}
bool MainController::ExportSeriesData2D(QString label,int algNum){

    QString path=this->GetOutputFolder();
    QString name;

    //Compruebo si existe la carpeta, y si no, la creo
    if(QDir(path+'/'+label).exists()==false){
        QDir().mkdir(path+'/'+label);
    }

    path=path+"/"+label+"/";

    //Guardo todas las imágenes
    int i;
    for(i=0;i<imgController.count();i++){
        //Calculo el nombre
        name.sprintf("%05i",i);
        name=name+"_"+this->GetDefaultNameForExport2D(i);
        //La guardo
        if(this->ExportSingleData2D(path+name,i,algNum)==false)
            return false;


    }
    return true;


}
bool MainController::ExportSeriesData1D(QString label,int algNum){
    QString path=this->GetOutputFolder();
    QString name;

    //Compruebo si existe la carpeta, y si no, la creo
    if(QDir(path+'/'+label).exists()==false){
        QDir().mkdir(path+'/'+label);
    }

    path=path+"/"+label+"/";

    //Guardo todas las imágenes
    int i;
    for(i=0;i<imgController.count();i++){
        //Calculo el nombre
        name.sprintf("%05i",i);
        name=name+"_"+this->GetDefaultNameForExport1D(i);
        //La guardo
        if(this->ExportSingleData1D(path+name,i,algNum)==false)
            return false;


    }
    return true;

}
