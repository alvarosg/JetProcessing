/**
 * @file   mainwindowlive.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase MainWindowLive.
 *
 */
#include "mainwindowlive.h"
#include "ui_mainwindowlive.h"

MainWindowLive::MainWindowLive(QWidget *parent, MainController *mainController_in) :
    QMainWindow(parent),
    ui(new Ui::MainWindowLive){
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);

    //Temporizador para la captura
    refreshTimer=new QTimer(this);
    //Semáforo para no estar produciendo más de una imagen a la vez
    semReadyToProduce= new QSemaphore(1);
    //Inicio variables
    mainController=mainController_in;
    currentGraph=0;
    playing=false;
    recording=false;
    refreshTimeMs=1000/mainController->GetLiveFPS();
    paramThreshold=mainController->GetThresholdLive();
    applyAlgorithms=mainController->GetApplyAlgorithmsLive();
    autoUpdateAxis=mainController->GetAutoScaleAxesLive();

    refreshTimer->setInterval(refreshTimeMs);
    lastData1D=NULL;
    lastData2D=NULL;

    //Y para la barra de estado
    statusLabel=new QLabel();
    ui->statusBar->addWidget(statusLabel);

    //Etiquetas
    this->setWindowTitle(MWL_CAPTION);
    ui->checkApplyAlgorithms->setText(MWL_APPLYALG);
    ui->checkUpdateZ->setText(MWL_AUTOUPDATEZ);
    ui->buttonUpdateZ->setText(MWL_UPDATEAXIS);
    ui->buttonRenewReference->setText(MWL_RENEWREFERENCE);
    ui->labelFPS->setText(MWL_FPSLABEL);
    ui->labelThreshold->setText(MWL_PARAM_THRES);

    //Inicio los valores de los controles
    ui->editThreshold->setText(QString::QString().setNum(paramThreshold,'g',3));
    ui->checkApplyAlgorithms->setChecked(applyAlgorithms);
    ui->spinRefreshFPS->setValue(1000/refreshTimeMs);
    ui->checkUpdateZ->setChecked(autoUpdateAxis);

    //Pongo de otro color el stacked Widget que contiene los gráficos
    ui->stackedWidget->setAutoFillBackground(true);
    QPalette pal(ui->stackedWidget->palette());
    pal.setBrush(QPalette::Window,QBrush::QBrush(Qt::white));
    ui->stackedWidget->setPalette(pal);

    connect(ui->buttonRenewReference,SIGNAL(clicked()),this,SLOT(RenewReference()));
    connect(ui->spinRefreshFPS,SIGNAL(valueChanged(double)),this,SLOT(SpinFPSChanged(double)));
    connect(refreshTimer,SIGNAL(timeout()),this,SLOT(UpdateGraph()));
    connect(ui->actionPlay,SIGNAL(triggered(bool )),this,SLOT(Play()));
    connect(ui->actionStop,SIGNAL(triggered(bool )),this,SLOT(Stop()));
    connect(ui->actionRecord,SIGNAL(triggered(bool )),this,SLOT(Record()));
    connect(ui->actionSpectrogram,SIGNAL(triggered(bool )),this,SLOT(SpectrogramClicked()));
    connect(ui->action3dSurface,SIGNAL(triggered(bool )),this,SLOT(Surface3DClicked()));
    connect(ui->actionParametric,SIGNAL(triggered(bool )),this,SLOT(ParametricClicked()));
    connect(ui->actionLine,SIGNAL(triggered(bool )),this,SLOT(LineClicked()));
    connect(ui->checkUpdateZ,SIGNAL(clicked()),this,SLOT(AutoUpdateAxisChanged()));
    connect(ui->checkApplyAlgorithms,SIGNAL(clicked()),this,SLOT(ApplyAlgorithmsChanged()));
    connect(ui->buttonUpdateZ,SIGNAL(clicked()),this,SLOT(RestartZAxis()));
    connect(ui->editThreshold, SIGNAL(editingFinished()),this, SLOT(ThresholdChanged()));


    this->UpdateControls();
    this->GraphChanged();
}

MainWindowLive::~MainWindowLive(){
    refreshTimer->stop();
    if(lastData2D!=NULL)
        delete lastData2D;
    if(lastData1D!=NULL)
        delete lastData1D;

    delete semReadyToProduce;
    delete ui;
}

void MainWindowLive::RenewReference(){
    mainController->LoadRefImageFromCamera();
}

void MainWindowLive::SpinFPSChanged(double value){
    if(refreshTimeMs!=value){
        refreshTimeMs=1000/value;
        refreshTimer->setInterval(refreshTimeMs);
        mainController->SetLiveFPS(value);
    }
}

void MainWindowLive::UpdateGraph(){

    //Si no estamos reproduciendo, volvemos
    if(playing!=true)
        return;

    //Sólo si ha acabado el hilo anterior damos paso al siguiente, en el caso de que no,
    //lanzamos otro temporizador de disparo único, para comprobar más regularmente,
    //cancelando el anterior
    if(semReadyToProduce->tryAcquire()==false){
        refreshTimer->stop();
        QTimer::singleShot(10,this,SLOT(UpdateGraph()));
        return;

    }

    //Si vengo de estar postponiendo el temporizador, reanudo el régimen normal
    if(refreshTimer->isActive()==false){
        refreshTimer->start();
    }

    //Creo un productor y un hilo
    DataProducer * dataProducer = new DataProducer(mainController,recording,applyAlgorithms);
    QThread * thread = new QThread();

    //Muevo el objeto al nuevo hilo
    dataProducer->moveToThread(thread);

    if(currentGraph!=3){
        //Coneco la señal para que sepa donde empezar
        connect(thread, SIGNAL(started()), dataProducer, SLOT(ProduceData2D()));
        //Conecto una señal para que me avisen de cuando está
        connect(dataProducer, SIGNAL(Calculated2D(Data2D *)), this, SLOT(PrintGraph(Data2D*)));
    }
    else{
        //Coneco la señal para que sepa donde empezar
        connect(thread, SIGNAL(started()), dataProducer, SLOT(ProduceData1D()));
        //Conecto una señal para que me avisen de cuando está
        connect(dataProducer, SIGNAL(Calculated1D(Data1D *)), this, SLOT(PrintGraph(Data1D*)));
    }
    //Conecto una señal para que el hilo se acabo al acabar la ejecución
    connect(dataProducer, SIGNAL(Finished()), thread, SLOT(quit()));
    //Para que al acabar el hilo se eliminen los objetos
    connect(dataProducer, SIGNAL(Finished()), dataProducer, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    //Lanzo el hilo
    thread->start();

}

void MainWindowLive::PrintGraph(Data2D *data2d, bool releaseSem){
    //Variables para guardar los tiempos anteriores y guardar los FPS efectivos
    float fps;
    static clock_t ini=-1;
    static clock_t aux=-1;
    if(ini==-1){
        ini=clock();
        aux=ini;
    }

    //Actualizo los valores máximos y mínimos
    //Si no tenía valores buenos, tomo los dos
    if(minZValue>maxZValue){
        minZValue=data2d->GetMinZValue();
        maxZValue=data2d->GetMaxZValue();
    }
    //Si ya los tenía, sólo actualizo si alguno sobrepasa los anteriores
    else{
        if(data2d->GetMinZValue()<minZValue){
            minZValue=data2d->GetMinZValue();
        }
        if(data2d->GetMaxZValue()>maxZValue){
            maxZValue=data2d->GetMaxZValue();
        }
    }

    this->UpdateAxis();

    //En función del gráfico que esté seleccionado
    switch(currentGraph){
        case 0:
            //Pinto los datos
            ui->graphSpectrogram->SetData2D(data2d);
            break;
        case 1:
            //Pinto los datos
            ui->graph3DSurface->SetData2D(data2d);
            break;
        case 2:
            //Fijo el threshold
            ui->graph3DParametric->SetThreshold(paramThreshold);
            //Pinto los datos
            ui->graph3DParametric->SetData2D(data2d);
            break;
    }

    //Actualizo los últimos datos de 2D
    Data2D * auxData2D;
    auxData2D=lastData2D;
    lastData2D=data2d;
    if(auxData2D!=NULL && auxData2D!=lastData2D)
        delete auxData2D;

    if(releaseSem==true)
        semReadyToProduce->release();

    //Actualizo los fps, calculando el tiempo que ha pasado entre el procesado de una imagen
    //y dos después que ella, y dividiéndolo entre 2, para promediar
    clock_t end=clock();

    if(end-ini!=0)
        fps=(2*(float)CLOCKS_PER_SEC)/((float)(end-ini));
    else
        fps=0;

    ini=aux;
    aux=end;

    this->ShowSelectedGraph();
    this->UpdateStatusBar(fps);
}

void MainWindowLive::PrintGraph(Data1D *data1d,bool releaseSem){
    //Variables para guardar los tiempos anteriores y guardar los FPS efectivos
    float fps;
    static clock_t ini=-1;
    static clock_t aux=-1;
    if(ini==-1){
        ini=clock();
        aux=ini;
    }

    //Actualizo los valores máximos y mínimos
    //Si no tenía valores buenos, tomo los dos
    if(minZValue>maxZValue){
        minZValue=data1d->GetMinZValue();
        maxZValue=data1d->GetMaxZValue();
    }
    //Si ya los tenía, sólo actualizo si alguno sobrepasa los anteriores
    else{
        if(data1d->GetMinZValue()<minZValue){
            minZValue=data1d->GetMinZValue();
        }
        if(data1d->GetMaxZValue()>maxZValue){
            maxZValue=data1d->GetMaxZValue();

        }
    }

    this->UpdateAxis();
    //En función del gráfico que esté seleccionado
    switch(currentGraph){
        case 3:
            //Pinto los datos
            ui->graphSection->SetData1D(data1d);
            break;
    }


    //Actualizo los últimos datos de 1D
    Data1D * auxData1D;
    auxData1D=lastData1D;
    lastData1D=data1d;
    if(auxData1D!=NULL && auxData1D!=lastData1D)
        delete auxData1D;

    if(releaseSem==true)
        semReadyToProduce->release();

    //Actualizo los fps, calculando el tiempo que ha pasado entre el procesado de una imagen
    //y dos después que ella, y dividiéndolo entre 2, para promediar
    clock_t end=clock();

    if(end-ini!=0)
        fps=(2*(float)CLOCKS_PER_SEC)/((float)(end-ini));
    else
        fps=0;

    ini=aux;
    aux=end;

    this->ShowSelectedGraph();
    this->UpdateStatusBar(fps);
}



void MainWindowLive::Play(){

    if(playing==false){
        playing=true;
        refreshTimer->start();
    }
    this->UpdateControls();

}

void MainWindowLive::Stop(){

    if(playing==true){
        playing=false;
        recording=false;
        refreshTimer->stop();
    }
    this->UpdateControls();
}

void MainWindowLive::Record(){

    if(playing==false){
        playing=true;
        recording=true;
        refreshTimer->start();
    }
    else{
        if(recording==false)
            recording=true;
        else
            recording=false;
    }

    this->UpdateControls();
}

void MainWindowLive::UpdateControls(){
    if(playing==true){
        ui->actionPlay->setChecked(true);
        ui->actionStop->setChecked(false);
        ui->buttonRenewReference->setEnabled(false);
    }
    else{
        ui->actionPlay->setChecked(false);
        ui->actionStop->setChecked(true);
        ui->buttonRenewReference->setEnabled(true);
    }
    if(recording==true){
        ui->actionRecord->setChecked(true);
    }
    else{
        ui->actionRecord->setChecked(false);
    }

    this->UpdateStatusBar();
}



void MainWindowLive::SpectrogramClicked(){
    currentGraph=0;
    this->GraphChanged();
}

void MainWindowLive::Surface3DClicked(){
    currentGraph=1;
    this->GraphChanged();
}

void MainWindowLive::ParametricClicked(){
    currentGraph=2;
    this->GraphChanged();
}

void MainWindowLive::LineClicked(){
    currentGraph=3;
    this->GraphChanged();
}

void MainWindowLive::GraphChanged(){
    //Si no hay línea, o no estamos aplicando algoritmos, desactivo la opción de mostrar la línea
    if(applyAlgorithms==false || mainController->GetAlgorithmSequenceController().IsLineSelected()==false){
        ui->actionLine->setEnabled(false);
        if(currentGraph==3){
            currentGraph=0;
        }
    }
    else
        ui->actionLine->setEnabled(true);

    ui->actionSpectrogram->setChecked(false);
    ui->action3dSurface->setChecked(false);
    ui->actionParametric->setChecked(false);
    ui->actionLine->setChecked(false);

    switch (currentGraph){
        case 0: ui->actionSpectrogram->setChecked(true); break;
        case 1: ui->action3dSurface->setChecked(true); break;
        case 2: ui->actionParametric->setChecked(true); break;
        case 3: ui->actionLine->setChecked(true); break;
    }

    //Si no estoy reproduciendo, le ordeno que repinte con los datos que tenga
    if(playing==false){
        if(lastData2D!=NULL)
            this->PrintGraph(lastData2D,false);
        if(lastData1D!=NULL)
            this->PrintGraph(lastData1D,false);
    }

    this->ShowSelectedGraph();

}

void MainWindowLive::ShowSelectedGraph(){
    //Si hay datos como para pintar un gráfico, mostramos ése índice
    if((currentGraph==3 && lastData1D!=NULL) || (currentGraph!=3 && lastData2D!=NULL))
        ui->stackedWidget->setCurrentIndex(currentGraph);
    //Si no, mostramos la ventan vacia
    else
        ui->stackedWidget->setCurrentIndex(4);
}

void MainWindowLive::ThresholdChanged(){
    QString string;
    float value;
    value=(ui->editThreshold->text()).toFloat();
    if(paramThreshold!=value){
        paramThreshold=value;
        string.setNum(value,'g',3);
        ui->editThreshold->setText(string);
        mainController->SetThresholdLive(paramThreshold);
        if(playing==false)
            this->PrintGraph(lastData2D,false);       
    }
}

void MainWindowLive::ApplyAlgorithmsChanged(){
    applyAlgorithms=ui->checkApplyAlgorithms->isChecked();
    mainController->SetApplyAlgorithmsLive(applyAlgorithms);
    this->GraphChanged();
}

void MainWindowLive::UpdateStatusBar(float fps){
    QString string;
    if(playing==false){
        string=MWL_STOPPED_LABEL;
    }
    else{
        if(recording==true){
            string.sprintf("%s %3.3g fps",MWL_RECORDING_LABEL,fps);
        }
        else{
            string.sprintf("%s %3.3g fps",MWL_PLAYING_LABEL,fps);
        }
    }
    statusLabel->setText(string);
}

void MainWindowLive::UpdateAxis(){
    if(autoUpdateAxis==true){
        ui->graphSpectrogram->SetMinMaxZValue(0,0);
        ui->graph3DSurface->SetMinMaxZValue(0,0);
        ui->graph3DParametric->SetMinMaxZValue(0,0);
        ui->graphSection->SetMinMaxZValue(0,0);
        ui->graphSpectrogram->SetInit(false);
        ui->graph3DSurface->SetInit(false);
        ui->graph3DParametric->SetInit(false);
        ui->graphSection->SetInit(false);
    }
    else{
        ui->graphSpectrogram->SetMinMaxZValue(minZValue,maxZValue);
        ui->graph3DSurface->SetMinMaxZValue(minZValue,maxZValue);
        ui->graph3DParametric->SetMinMaxZValue(minZValue,maxZValue);
        ui->graphSection->SetMinMaxZValue(minZValue,maxZValue);
    }
}

void MainWindowLive::AutoUpdateAxisChanged(){
    autoUpdateAxis = ui->checkUpdateZ->isChecked();
    mainController->SetAutoScaleAxesLive(autoUpdateAxis);
}

void MainWindowLive::RestartZAxis(){
    minZValue=1;
    maxZValue=-1;
    ui->graphSpectrogram->SetInit(false);
    ui->graph3DSurface->SetInit(false);
    ui->graph3DParametric->SetInit(false);
    ui->graphSection->SetInit(false);
}

void MainWindowLive::showEvent(QShowEvent * event){
    QMainWindow::showEvent(event);
    minZValue=1;
    maxZValue=-1;
    //Para que se active o no el gráfico de la línea
    this->GraphChanged();
    //Fijo el valor máximo de frames por segundo en función de la exposición
    float exposureMs=mainController->GetCameraController()->GetExposure(EXPOSURE);
    ui->spinRefreshFPS->setMaximum(1000/exposureMs);

    //Cargo las opciones de visualización del controlador
    refreshTimeMs=1000/mainController->GetLiveFPS();
    paramThreshold=mainController->GetThresholdLive();
    applyAlgorithms=mainController->GetApplyAlgorithmsLive();
    autoUpdateAxis=mainController->GetAutoScaleAxesLive();
    ui->editThreshold->setText(QString::QString().setNum(paramThreshold,'g',3));
    ui->checkApplyAlgorithms->setChecked(applyAlgorithms);
    ui->spinRefreshFPS->setValue(1000/refreshTimeMs);
    ui->checkUpdateZ->setChecked(autoUpdateAxis);
}

void MainWindowLive::closeEvent(QCloseEvent *event){
    //Si estoy reproduciendo, paro
    if(playing==true){
        playing=false;
        recording=false;
        refreshTimer->stop();
        this->UpdateControls();
    }
    emit Closed();
    QMainWindow::closeEvent(event);
}

DataProducer::DataProducer(MainController * mainController_in,bool recording_in,bool applyAlgorithms_in){
    mainController=mainController_in;
    recording=recording_in;
    applyAlgorithms=applyAlgorithms_in;
}

void DataProducer::ProduceData2D(){
    Data2D *data2d;
    data2d=mainController->ProcessImageFromCamera(recording,applyAlgorithms);
    emit Calculated2D(data2d);
    emit Finished();
}
void DataProducer::ProduceData1D(){
    Data1D *data1d;
    data1d=mainController->ProcessImageFromCamera1D(recording);
    emit Calculated1D(data1d);
    emit Finished();
}

DataProducer::~DataProducer(){

}
