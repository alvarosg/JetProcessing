/**
 * @file   mainwindow.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase MainWindow.
 *
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent,MainController * mainController_in) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){

    this->setWindowOpacity(0);
    this->show();


    ui->setupUi(this);
    this->setWindowTitle(MW_CAPTION);

    //Inicio las variables
    mainController = mainController_in;
    label="SampleSeries";

    //Opciones de visualización
    autoUpdateZ=mainController->GetAutoScaleAxes();
    minZValue=mainController->GetMinZValue();
    maxZValue=mainController->GetMaxZValue();
    paramThreshold=mainController->GetThreshold();
    sequenceMs=1000/mainController->GetSequenceFPS();
    viewLevelCurves=mainController->GetViewLevelCurves();


    //Creo el temporizador de la secuencia
    sequenceTimer = new QTimer(this);
    sequenceTimer->setInterval(sequenceMs);


    //Si procede, muestro la ventana de elegir modo
    if(mainController->GetDefaultMode()==false){
        DialogModeSelection * dialogModeSelection = new DialogModeSelection(this,mainController);
        dialogModeSelection->exec();
        delete dialogModeSelection;
    }
    else{
        //Es necesario hacer esta llamada para que se inicie la facotría del controlador
        mainController->SetMode(mainController->GetMode());
    }

    this->InitControls();

    //Si es la primera vez, pido un directorio de trabajo
    if(mainController->GetFirstExecution()==true)
        this->SetOutputFolder();

    //Pongo un temporizador para comprobar si se ha enchufado la cámara
    QTimer * cameraTimer;
    cameraTimer= new QTimer(this);
    connect(cameraTimer, SIGNAL(timeout()),this, SLOT(CheckCamera()));
    cameraTimer->start(5000);

    this->setWindowOpacity(1);


    //Si me lo han pasado como argumento, abro el proyecto que me han pasado
    QStringList arglist=QCoreApplication::arguments();
    arglist=arglist.filter(".jpp");
    if(arglist.count()>=1){
        if(mainController->LoadProject(arglist.at(0))==false)
            QMessageBox::critical(this,MW_ERROR_LABEL,MW_FAILLOADPROJ_LABEL1+arglist.at(0)+MW_FAILLOADPROJ_LABEL2+QDir::tempPath(),QMessageBox::Ok);

        //Cargo las nuevas opciones de visualización
        autoUpdateZ=mainController->GetAutoScaleAxes();
        minZValue=mainController->GetMinZValue();
        maxZValue=mainController->GetMaxZValue();
        paramThreshold=mainController->GetThreshold();
        sequenceMs=1000/mainController->GetSequenceFPS();
        viewLevelCurves=mainController->GetViewLevelCurves();

        ui->editMinZ->setText(QString::QString().sprintf("%3.3g",minZValue));
        ui->editMaxZ->setText(QString::QString().sprintf("%3.3g",maxZValue));
        ui->editThreshold->setText(QString::QString().setNum(paramThreshold,'g',3));
        ui->spinFPS->setValue(1000/sequenceMs);
        sequenceTimer->setInterval(sequenceMs);
        ui->checkLevelCurves->setChecked(viewLevelCurves);
        ui->graphSpectrogram->ShowContour(viewLevelCurves);
        ui->editLabelZ->setText(mainController->GetZLabel());
        ui->checkUpdateZ->setChecked(autoUpdateZ);


        this->AutoScaleChanged();
        this->UpdateMenusAndButtonsState();
        this->UpdatePhaseImagesList(false,0);
        this->UpdateAlgorithmsList(false,-2);

        //Aseguro que los gráficos comenzarán de cero
        ui->graphSpectrogram->SetInit(false);
        ui->graph3DSurface->SetInit(false);
        ui->graph3DParametric->SetInit(false);
        ui->graphSection->SetInit(false);

        //Actualizo los gráficos
        this->UpdateGraph();
        }

}

void MainWindow::InitControls(){
    QString string;

    //La barra de estado
    cameraStateLabel=new QLabel();
    modeStateLabel=new QLabel();
    currentPathStateLabel=new QLabel();
    ui->statusbar->addWidget(modeStateLabel);
    ui->statusbar->addWidget(cameraStateLabel);
    ui->statusbar->addWidget(currentPathStateLabel);

    //Etiquetas para menús y botones

    ui->menuFile->setTitle(MW_FILE_MENU);
    ui->actionLoadProject->setText(MW_FILE_LOADP);
    ui->actionSaveProject->setText(MW_FILE_SAVEP);
    ui->actionSaveProjectAs->setText(MW_FILE_SAVEPAS);
    ui->actionSetOutputFolder->setText(MW_FILE_OUTPUTF);
    ui->actionExit->setText(MW_FILE_EXIT);


    ui->menuReference->setTitle(MW_REF_MENU);
    ui->actionSetReference->setText(MW_REF_SET);

    ui->menuImage->setTitle(MW_IMG_MENU);
    ui->actionMoveImageUp->setText(MW_IMG_MOVEUP);
    ui->actionMoveImageDown->setText(MW_IMG_MOVEDOWN);
    ui->actionDeleteImage->setText(MW_IMG_DELETE);
    ui->actionDeleteAllImages->setText(MW_IMG_DELETEALL);
    ui->actionLoadImagesFromFile->setText(MW_IMG_LOAD);
    ui->actionLoadImageFromCamera->setText(MW_IMG_CAM);

    ui->menuAlgorithm->setTitle(MW_ALG_MENU);
    ui->actionMoveAlgUp->setText(MW_ALG_MOVEUP);
    ui->actionMoveAlgDown->setText(MW_ALG_MOVEDOWN);
    ui->actionEditAlg->setText(MW_ALG_EDIT);
    ui->actionDeleteAlgorithm->setText(MW_ALG_DELETE);
    ui->actionSaveAlgList->setText(MW_ALG_SAVE);
    ui->actionLoadAlgList->setText(MW_ALG_LOAD);

    ui->menuLiveMode->setTitle(MW_LM_MENU);
    ui->actionStart->setText(MW_LM_START);

    ui->menuExport->setTitle(MW_EXP_MENU);
    ui->actionSaveGraph->setText(MW_EXP_SAVE);
    ui->actionSaveSeriesGraph->setText(MW_EXP_SAVES);
    ui->actionSaveData->setText(MW_EXP_SAVED);
    ui->actionSaveSeriesData->setText(MW_EXP_SAVEDS);
    ui->actionSaveSeriesVideo->setText(MW_EXP_SAVEVIDEO);
    ui->actionSaveSeriesAnimation->setText(MW_EXP_SAVEANIM);

    ui->menuConfiguration->setTitle(MW_CONF_MENU);
    ui->actionCameraSettings->setText(MW_CONF_CAM);
    ui->actionPreferences->setText(MW_CONF_PREF);

    ui->menuHelp->setTitle(MW_HELP_MENU);
    ui->actionHelp->setText(MW_HELP_HELP);
    ui->actionAbout->setText(MW_HELP_ABOUT);

    ui->buttonAddAlgorithmBefore->setText(MW_ALG_BEFORE);
    ui->buttonAddAlgorithmAfter->setText(MW_ALG_AFTER);
    ui->buttonMoveAlgUp->setText(MW_ALG_MOVEUP);
    ui->buttonEditAlg->setText(MW_ALG_EDIT);
    ui->buttonMoveAlgDown->setText(MW_ALG_MOVEDOWN);
    ui->buttonDeleteAlgorithm->setText(MW_ALG_DELETE);

    ui->comboAlgorithms->addItem(MW_ALG_INVERSE);
    ui->comboAlgorithms->addItem(MW_ALG_RMHTILT);
    ui->comboAlgorithms->addItem(MW_ALG_RMVTILT);
    ui->comboAlgorithms->addItem(MW_ALG_SETGROUND);
    ui->comboAlgorithms->addItem(MW_ALG_OFFSET);
    ui->comboAlgorithms->addItem(MW_ALG_MULTIPLY);
    ui->comboAlgorithms->addItem(MW_ALG_SYMMETRIC);
    ui->comboAlgorithms->addItem(MW_ALG_ABELINV);
    ui->comboAlgorithms->addItem(MW_ALG_RECTANGLE);
    ui->comboAlgorithms->addItem(MW_ALG_GETLINE);

    ui->listAlgorithms->setSpacing(2);
    ui->listAlgorithms->setMinimumHeight(0);

    this->UpdateAlgorithmsList(false);

    ui->tabGraphs->setTabText(0,MW_TAB_SPECT);
    ui->tabGraphs->setTabText(1,MW_TAB_2DSURF);
    ui->tabGraphs->setTabText(2,MW_TAB_PARAM);
    ui->tabGraphs->setTabText(3,MW_TAB_1DDATA);
    ui->tabGraphs->setCurrentIndex(0);

    ui->labelLabelZ->setText(MW_LABELZ);
    ui->editLabelZ->setText(mainController->GetZLabel());
    ui->checkUpdateZ->setText(MW_AUTOSCALEZ);
    ui->checkUpdateZ->setChecked(autoUpdateZ);
    this->AutoScaleChanged();
    ui->labelMinZ->setText(MW_MINZLABEL);
    ui->labelMaxZ->setText(MW_MAXZLABEL);
    string.sprintf("%3.3g",minZValue);
    ui->editMinZ->setText(string);
    string.sprintf("%3.3g",maxZValue);
    ui->editMaxZ->setText(string);
    ui->checkSequence->setText(MW_SEQUENCE);
    ui->labelFPS->setText(MW_FPSLABEL);
    sequenceTimer->setInterval(sequenceMs);
    ui->spinFPS->setValue(1000/sequenceMs);


    //Pongo una paleta blanca para que cuando guarde los gráficos quede fondo blanco, no obstante todavia no estará activada
    QPalette palette;
    palette= ui->graphSpectrogram->palette();
    palette.setBrush(QPalette::Window,QBrush::QBrush(Qt::white));
    ui->graphSpectrogram->setPalette(palette);
    palette= ui->graph3DSurface->palette();
    palette.setBrush(QPalette::Window,QBrush::QBrush(Qt::white));
    ui->graph3DSurface->setPalette(palette);
    palette= ui->graph3DParametric->palette();
    palette.setBrush(QPalette::Window,QBrush::QBrush(Qt::white));
    ui->graph3DParametric->setPalette(palette);
    palette= ui->graphSection->palette();
    palette.setBrush(QPalette::Window,QBrush::QBrush(Qt::white));
    ui->graphSection->setPalette(palette);


    ui->checkLevelCurves->setText(MW_LEVELCURV);
    ui->checkLevelCurves->setChecked(viewLevelCurves);
    ui->graphSpectrogram->ShowContour(viewLevelCurves);
    ui->labelThreshold->setText(MW_PARAM_THRES);
    string.sprintf("%3.3g",paramThreshold);
    ui->editThreshold->setText(string);

    this->UpdateGraph();

    ui->listPhaseImages->setIconSize(QSize(64,64));
    ui->listPhaseImages->setSpacing(5);
    ui->listPhaseImages->setEditTriggers(QAbstractItemView::DoubleClicked |QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);
    ui->buttonMoveImageUp->setText(MW_IMG_MOVEUP);
    ui->buttonMoveImageDown->setText(MW_IMG_MOVEDOWN);
    ui->buttonDeleteImage->setText(MW_IMG_DELETE);

    this->UpdatePhaseImagesList(false);

    //La ventana para modo a tiempo real
    mainWindowLive = new MainWindowLive(this,mainController);

    connect(ui->actionSetOutputFolder,SIGNAL(triggered()),this,SLOT(SetOutputFolder()));
    connect(ui->actionLoadProject,SIGNAL(triggered()),this,SLOT(LoadProject()));
    connect(ui->actionSaveProject,SIGNAL(triggered()),this,SLOT(SaveProject()));
    connect(ui->actionSaveProjectAs,SIGNAL(triggered()),this,SLOT(SaveProjectAs()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));

    connect(ui->comboAlgorithms,SIGNAL(currentIndexChanged(int)),this,SLOT(UpdateMenusAndButtonsState()));
    connect(ui->buttonAddAlgorithmBefore, SIGNAL(clicked()),this, SLOT(AddAlgorithmBefore()));
    connect(ui->buttonAddAlgorithmAfter, SIGNAL(clicked()),this, SLOT(AddAlgorithmAfter()));
    connect(ui->buttonMoveAlgUp, SIGNAL(clicked()),this, SLOT(MoveAlgorithmUp()));
    connect(ui->actionMoveAlgUp, SIGNAL(triggered()),this, SLOT(MoveAlgorithmUp()));
    connect(ui->buttonMoveAlgDown, SIGNAL(clicked()),this, SLOT(MoveAlgorithmDown()));
    connect(ui->actionMoveAlgDown, SIGNAL(triggered()),this, SLOT(MoveAlgorithmDown()));
    connect(ui->buttonEditAlg, SIGNAL(clicked()),this, SLOT(EditAlgorithm()));
    connect(ui->actionEditAlg, SIGNAL(triggered()),this, SLOT(EditAlgorithm()));
    connect(ui->buttonDeleteAlgorithm, SIGNAL(clicked()),this, SLOT(DeleteAlgorithm()));
    connect(ui->actionDeleteAlgorithm, SIGNAL(triggered()),this, SLOT(DeleteAlgorithm()));
    connect(ui->actionSaveAlgList,SIGNAL(triggered()),this,SLOT(SaveAlgorithmList()));
    connect(ui->actionLoadAlgList,SIGNAL(triggered()),this,SLOT(LoadAlgorithmList()));
    connect(ui->listAlgorithms,SIGNAL(currentRowChanged(int)),this,SLOT(UpdateGraph()));
    connect(ui->listAlgorithms,SIGNAL(currentRowChanged(int)),this,SLOT(UpdateMenusAndButtonsState()));

    connect(ui->buttonMoveImageUp, SIGNAL(clicked()),this, SLOT(MoveImageUp()));
    connect(ui->actionMoveImageUp, SIGNAL(triggered()),this, SLOT(MoveImageUp()));
    connect(ui->buttonMoveImageDown, SIGNAL(clicked()),this, SLOT(MoveImageDown()));
    connect(ui->actionMoveImageDown, SIGNAL(triggered()),this, SLOT(MoveImageDown()));
    connect(ui->buttonDeleteImage, SIGNAL(clicked()),this, SLOT(DeleteImage()));
    connect(ui->actionDeleteImage, SIGNAL(triggered()),this, SLOT(DeleteImage()));
    connect(ui->actionDeleteAllImages, SIGNAL(triggered()),this, SLOT(DeleteAllImage()));
    connect(ui->actionLoadImagesFromFile,SIGNAL(triggered()),this,SLOT(LoadImagesFromFile()));
    connect(ui->actionLoadImageFromCamera,SIGNAL(triggered()),this,SLOT(LoadImageFromCamera()));
    connect(ui->listPhaseImages,SIGNAL(currentRowChanged(int)),this,SLOT(UpdateGraph()));
    connect(ui->listPhaseImages,SIGNAL(currentRowChanged(int)),this,SLOT(UpdateMenusAndButtonsState()));
    connect(ui->listPhaseImages,SIGNAL(itemChanged(QListWidgetItem *)),this,SLOT(ImageLabelEdited()));
    connect(ui->tabGraphs,SIGNAL(currentChanged(int)),this,SLOT(UpdateGraph()));

    connect(ui->actionSaveGraph,SIGNAL(triggered()),this,SLOT(SaveImage()));
    connect(ui->actionSaveSeriesGraph,SIGNAL(triggered()),this,SLOT(SaveSeriesImage()));
    connect(ui->actionSaveData,SIGNAL(triggered()),this,SLOT(SaveData()));
    connect(ui->actionSaveSeriesData,SIGNAL(triggered()),this,SLOT(SaveSeriesData()));
    connect(ui->actionSaveSeriesVideo,SIGNAL(triggered()),this,SLOT(SaveSeriesVideo()));
    connect(ui->actionSaveSeriesAnimation,SIGNAL(triggered()),this,SLOT(SaveSeriesAnimation()));

    connect(ui->actionStart,SIGNAL(triggered()),this,SLOT(LiveMode()));

    connect(ui->actionSetReference,SIGNAL(triggered()),this,SLOT(SetReference()));
    connect(ui->actionCameraSettings,SIGNAL(triggered()),this,SLOT(CameraSettings()));
    connect(ui->actionPreferences,SIGNAL(triggered()),this,SLOT(ProgramPreferences()));
    connect(ui->actionHelp,SIGNAL(triggered()),this,SLOT(OpenHelp()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(OpenAbout()));

    connect(ui->checkLevelCurves,SIGNAL(clicked()),this,SLOT(EnableLevelCurves()));
    connect(ui->editThreshold, SIGNAL(editingFinished()),this, SLOT(ThresholdChanged()));
    connect(ui->editLabelZ,SIGNAL(editingFinished()),this,SLOT(ZLabelChanged()));
    connect(ui->checkUpdateZ,SIGNAL(clicked()),this,SLOT(AutoScaleChanged()));
    connect(ui->editMinZ,SIGNAL(editingFinished()),this,SLOT(MinZValueChanged()));
    connect(ui->editMaxZ,SIGNAL(editingFinished()),this,SLOT(MaxZValueChanged()));
    connect(ui->checkSequence,SIGNAL(clicked()),this,SLOT(EnableSequenceChanged()));
    connect(ui->spinFPS,SIGNAL(valueChanged(double)),this,SLOT(FPSSequenceChanged()));
    connect(sequenceTimer,SIGNAL(timeout()),this,SLOT(SequenceTimeout()));
    connect(mainWindowLive,SIGNAL(Closed()),this,SLOT(UpdatePhaseImagesList()));

    //Accesos de teclado para subir, bajar, eliminar elementos
    new QShortcut(QKeySequence(Qt::Key_Delete), this, SLOT(DeleteAlgorithm()));
    new QShortcut(QKeySequence(Qt::Key_Delete+Qt::CTRL), this, SLOT(DeleteImage()));

    this->CheckCamera();
    this->UpdateStatusBar();
}

MainWindow::~MainWindow(){
    delete ui;
    delete mainWindowLive;
}

void MainWindow::UpdateAlgorithmsList(bool updateGraph,int selectRow){
    int i;
    AlgorithmSequenceController algController;
    algController=mainController->GetAlgorithmSequenceController();
    AlgorithmListWidgetItem * item;

    int currentRow;

    if(selectRow==-1) currentRow=ui->listAlgorithms->currentRow();
    else if (selectRow==-2) currentRow=algController.count()-1;
    else currentRow=selectRow;

    //Antes de hacer todo desconecto la señal de cambio de posición con el slot de actualizar el gráfico, para no generar un exceso de actualizaciones
    disconnect(ui->listAlgorithms,SIGNAL(currentRowChanged(int)),this,SLOT(UpdateGraph()));
    ui->listAlgorithms->clear();
    for(i=0;i<algController.count();i++){
        item = new AlgorithmListWidgetItem(algController.GetAlgorithmIndex(i),algController.GetAuxIntIndex(i),
                                           algController.GetAuxFloatIndex(i),algController.GetAuxRectIndex(i));
        ui->listAlgorithms->addItem(item);
    }
    //Vuelvo a conectar la señal
    connect(ui->listAlgorithms,SIGNAL(currentRowChanged(int)),this,SLOT(UpdateGraph()));


    //Recoloco la selección. En el caso de que ya no existe el índice, la coloco en la última posición

    if(currentRow < ui->listAlgorithms->count())
        ui->listAlgorithms->setCurrentRow(currentRow);
    else
        ui->listAlgorithms->setCurrentRow(ui->listAlgorithms->count()-1);

    if(updateGraph==true)
        this->UpdateGraph();

    UpdateMenusAndButtonsState();
}

void MainWindow::AddAlgorithmBefore(){
    int currentRow=ui->listAlgorithms->currentRow();

    if(currentRow!=-1)
        AddAlgorithmPosition(ui->listAlgorithms->currentRow());
    else
        AddAlgorithmPosition(0);
}

void MainWindow::AddAlgorithmAfter(){
    int currentRow=ui->listAlgorithms->currentRow();

    if(currentRow!=-1)
        AddAlgorithmPosition(ui->listAlgorithms->currentRow()+1);
    else
        AddAlgorithmPosition(ui->listAlgorithms->count());

}

void MainWindow::MoveAlgorithmUp(){
    AlgorithmSequenceController algController;
    int currentRow=ui->listAlgorithms->currentRow();

    if(currentRow!=-1 && currentRow !=0 ){
        algController=mainController->GetAlgorithmSequenceController();
        algController.move(currentRow,currentRow-1);
        mainController->SetAlgorithmSequenceController(algController);
        ui->listAlgorithms->setCurrentRow(currentRow-1);
        this->UpdateAlgorithmsList(true);
    }
}

void MainWindow::MoveAlgorithmDown(){
    AlgorithmSequenceController algController;
    int currentRow=ui->listAlgorithms->currentRow();

    if(currentRow!=-1 && currentRow != ui->listAlgorithms->count()-1){
        algController=mainController->GetAlgorithmSequenceController();
        algController.move(currentRow,currentRow+1);
        mainController->SetAlgorithmSequenceController(algController);
        ui->listAlgorithms->setCurrentRow(currentRow+1);
        this->UpdateAlgorithmsList(true);

    }
}

void MainWindow::EditAlgorithm(){
    AlgorithmSequenceController algController;
    DialogSetRectangle * dialogSetRectangle;
    DialogSetRectangle * dialogSetLine;
    DialogGetFloat * dialogGetOffset;
    DialogGetFloat * dialogGetFactor;
    DialogSetAbel * dialogSetAbel;
    int position=ui->listAlgorithms->currentRow();
    AlgorithmType algType;
    QRect auxRect;
    int auxInt;
    float auxFloat;

    if(position==-1)
        return;

    int currentRowImage=ui->listPhaseImages->currentRow();
    if(currentRowImage==-1) currentRowImage=0;

    algController=mainController->GetAlgorithmSequenceController();

    algType=algController.GetAlgorithmIndex(position);
    auxInt=algController.GetAuxIntIndex(position);
    auxFloat=algController.GetAuxFloatIndex(position);
    auxRect=algController.GetAuxRectIndex(position);

    switch(algType){
        case ALG_OFFSET:
            //Creo el cuadro de diálogo para seleccionar el offset
            dialogGetOffset  = new DialogGetFloat (this,&auxFloat,false);

            //Lanzo el cuadro de diálogo para seleccionar los parámetros
            if(dialogGetOffset->exec()==QDialog::Rejected){
                delete dialogGetOffset;
                return;
            }
            delete dialogGetOffset;
            break;
        case ALG_MULTIPLY:
            //Creo el cuadro de diálogo para seleccionar el offset
            dialogGetFactor  = new DialogGetFloat (this,&auxFloat,true);

            //Lanzo el cuadro de diálogo para seleccionar los parámetros
            if(dialogGetFactor->exec()==QDialog::Rejected){
                delete dialogGetFactor;
                return;
            }
            delete dialogGetFactor;
            break;
        case ALG_ABELINV:
            //Creo el cuadro de diálogo para configurar la inversión de Abel
            dialogSetAbel  = new DialogSetAbel (this,&auxInt);
            //Lanzo el cuadro de diálogo para seleccionar los parámetros
            if(dialogSetAbel->exec()==QDialog::Rejected){
                delete dialogSetAbel;
                return;
            }
            delete dialogSetAbel;
            break;
        case ALG_GETLINE:
            //Lanzo el cuadro de diálogo para seleccionar la línea, con la imagen correspondiente
            dialogSetLine  = new DialogSetRectangle (this,mainController,&auxRect,position,currentRowImage,true);

            if(dialogSetLine->exec()==QDialog::Rejected){
                delete dialogSetLine;
                return;
            }
            delete dialogSetLine;
            break;
        case ALG_RECTANGLE:
            //Lanzo el cuadro de diálogo para seleccionar el rectángulo, con la imagen correspondiente
            dialogSetRectangle  = new DialogSetRectangle (this,mainController,&auxRect,position,currentRowImage,false);

            if(dialogSetRectangle->exec()==QDialog::Rejected){
                delete dialogSetRectangle;
                return;
            }
            delete dialogSetRectangle;
            break;
        default:
            return;
    }
    algController.ReplaceAlgorithmIndex(position,algType,auxInt,auxFloat,auxRect);
    mainController->SetAlgorithmSequenceController(algController);

    this->UpdateAlgorithmsList(true,position);

}

void MainWindow::DeleteAlgorithm(){
    AlgorithmSequenceController algController;
    int currentRow=ui->listAlgorithms->currentRow();
    if(currentRow!=-1){
        algController=mainController->GetAlgorithmSequenceController();
        algController.removeAt(currentRow);
        mainController->SetAlgorithmSequenceController(algController);
        this->UpdateAlgorithmsList(true);
    }
}

void MainWindow::AddAlgorithmPosition(int position){
    AlgorithmSequenceController algController;
    DialogSetRectangle * dialogSetRectangle;
    DialogSetRectangle * dialogSetLine;
    DialogGetFloat * dialogGetOffset;
    DialogGetFloat * dialogGetFactor;
    DialogSetAbel * dialogSetAbel;
    AlgorithmType algType;
    QRect auxRect;
    int auxInt;
    float auxFloat;
    int currentRowImage=ui->listPhaseImages->currentRow();
    if(currentRowImage==-1) currentRowImage=0;

    algController=mainController->GetAlgorithmSequenceController();

    switch(ui->comboAlgorithms->currentIndex()){
        case ALG_INVERSE:algType=ALG_INVERSE;     break;
        case ALG_RMHTILT:algType=ALG_RMHTILT;     break;
        case ALG_RMVTILT:algType=ALG_RMVTILT;     break;
        case ALG_SETGROUND:algType=ALG_SETGROUND; break;
        case ALG_SYMMETRIC:algType=ALG_SYMMETRIC; break;
        case ALG_OFFSET:algType=ALG_OFFSET;       break;
        case ALG_MULTIPLY:algType=ALG_MULTIPLY;   break;
        case ALG_ABELINV:algType=ALG_ABELINV;     break;
        case ALG_RECTANGLE:algType=ALG_RECTANGLE; break;
        case ALG_GETLINE:algType=ALG_GETLINE;     break;
    }

    switch(algType){
        case ALG_OFFSET:
            auxFloat=0;
            //Creo el cuadro de diálogo para seleccionar el offset
            dialogGetOffset  = new DialogGetFloat (this,&auxFloat,false);

            //Lanzo el cuadro de diálogo para seleccionar los parámetros
            if(dialogGetOffset->exec()==QDialog::Rejected){
                delete dialogGetOffset;
                return;
            }
            delete dialogGetOffset;
            break;
        case ALG_MULTIPLY:
            auxFloat=1;
            //Creo el cuadro de diálogo para seleccionar el offset
            dialogGetFactor  = new DialogGetFloat (this,&auxFloat,true);

            //Lanzo el cuadro de diálogo para seleccionar los parámetros
            if(dialogGetFactor->exec()==QDialog::Rejected){
                delete dialogGetFactor;
                return;
            }
            delete dialogGetFactor;
            break;
        case ALG_ABELINV:
            auxInt=1;
            //Creo el cuadro de diálogo para configurar la inversión de Abel
            dialogSetAbel  = new DialogSetAbel (this,&auxInt);

            //Lanzo el cuadro de diálogo para seleccionar los parámetros
            if(dialogSetAbel->exec()==QDialog::Rejected){
                delete dialogSetAbel;
                return;
            }
            delete dialogSetAbel;
            break;
        case ALG_GETLINE:
            auxRect=QRect(0,0,2,2);
            //Si ya tengo seleccionada una línea, salgo
            if(algController.IsLineSelected()){
                return;
            }
            //Lanzo el cuadro de diálogo para seleccionar la línea, con la imagen correspondiente
            dialogSetLine  = new DialogSetRectangle (this,mainController,&auxRect,position,currentRowImage,true);

            if(dialogSetLine->exec()==QDialog::Rejected){
                delete dialogSetLine;
                return;
            }
            delete dialogSetLine;
            break;
        case ALG_RECTANGLE:
            auxRect=QRect(0,0,2,2);
            //Lanzo el cuadro de diálogo para seleccionar el rectángulo, con la imagen correspondiente
            dialogSetRectangle  = new DialogSetRectangle (this,mainController,&auxRect,position,currentRowImage,false);

            if(dialogSetRectangle->exec()==QDialog::Rejected){
                delete dialogSetRectangle;
                return;
            }
            delete dialogSetRectangle;
            break;
    }
    algController.AddAlgorithmIndex(position,algType,auxInt,auxFloat,auxRect);
    mainController->SetAlgorithmSequenceController(algController);

    //Actualizo la lista de algoritmos
    this->UpdateAlgorithmsList(true,position);

    //Pongo automáticamente la vista de la línea en el caso de que haya seleccionado una línea (No se puede hacer dentro del switch)
    if(algType==ALG_GETLINE)
        ui->tabGraphs->setCurrentIndex(3);

}

void MainWindow::LoadAlgorithmList(){
    QFileDialog fileDialog(this,tr("Load"));
    fileDialog.setNameFilter(tr("Algorithm Sequence Files (*.alg)"));
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setLabelText(QFileDialog::Accept,tr("Load"));
    AlgorithmSequenceController algController;
    if(fileDialog.exec()){
        if(algController.LoadSequenceFromFile(fileDialog.selectedFiles().at(0))==false){
            QMessageBox::critical(this,MW_ERROR_LOAD,fileDialog.selectedFiles().at(0),QMessageBox::Ok);
            return;
        }
        mainController->SetAlgorithmSequenceController(algController);
        this->UpdateAlgorithmsList(true,-2);
    }


}

void MainWindow::SaveAlgorithmList(){    

    QString path=mainController->GetOutputFolder();
    QString format=".alg";
    QString name;
    AlgorithmSequenceController algController;

    name="*"+format;

    QFileDialog fileDialog(this,tr("Save"));
    fileDialog.setDirectory(path);
    fileDialog.selectFile(name);
    fileDialog.setNameFilter(tr("Algorithm Sequence Files (*.alg)"));
    fileDialog.setDefaultSuffix("alg");
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setLabelText(QFileDialog::Accept,tr("Save"));

    //Lanzo el diálogo
    if(fileDialog.exec()){
        algController=mainController->GetAlgorithmSequenceController();

        if(algController.SaveSequenceToFile(fileDialog.selectedFiles().at(0))==false){
            QMessageBox::critical(this,MW_ERROR_SAVE,fileDialog.selectedFiles().at(0),QMessageBox::Ok);
        }

    }
}

void MainWindow::EnableLevelCurves(){
    viewLevelCurves=ui->checkLevelCurves->isChecked();
    ui->graphSpectrogram->ShowContour(viewLevelCurves);
    mainController->SetViewLevelCurves(viewLevelCurves);

    this->UpdateMenusAndButtonsState();
}

void MainWindow::ThresholdChanged(){
    QString string;
    float value;
    value=(ui->editThreshold->text()).toFloat();
    if(paramThreshold!=value){
        paramThreshold=value;
        string.setNum(value,'g',3);
        ui->editThreshold->setText(string);
        mainController->SetThreshold(paramThreshold);
        this->UpdateGraph();

    }
}

void MainWindow::ZLabelChanged(){
    mainController->SetZLabel(ui->editLabelZ->text());
    this->UpdateGraph();
    this->UpdateMenusAndButtonsState();
}

void MainWindow::AutoScaleChanged(){
    if(ui->checkUpdateZ->isChecked()==true){
        ui->editMinZ->hide();
        ui->editMaxZ->hide();
        ui->labelMinZ->hide();
        ui->labelMaxZ->hide();
        autoUpdateZ=true;
    }
    else{
        ui->editMinZ->show();
        ui->editMaxZ->show();
        ui->labelMinZ->show();
        ui->labelMaxZ->show();
        autoUpdateZ=false;
    }
    mainController->SetAutoScaleAxes(autoUpdateZ);
    this->SetMaxMinZ();
    this->UpdateGraph();

}

void MainWindow::MinZValueChanged(){
    QString string;
    minZValue=ui->editMinZ->text().toFloat();
    if (minZValue>=maxZValue)
        minZValue=maxZValue-0.1;
    string.sprintf("%3.3g",minZValue);
    ui->editMinZ->setText(string);
    mainController->SetMinZValue(minZValue);
    this->SetMaxMinZ();
    this->UpdateGraph();
}

void MainWindow::MaxZValueChanged(){
    QString string;
    maxZValue=ui->editMaxZ->text().toFloat();
    if (minZValue>=maxZValue)
        maxZValue=minZValue+0.1;
    string.sprintf("%3.3g",maxZValue);
    ui->editMaxZ->setText(string);
    mainController->SetMaxZValue(maxZValue);
    this->SetMaxMinZ();
    this->UpdateGraph();
}

void MainWindow::SetMaxMinZ(){
    if(autoUpdateZ==false){
        ui->graphSpectrogram->SetMinMaxZValue(minZValue,maxZValue);
        ui->graph3DSurface->SetMinMaxZValue(minZValue,maxZValue);
        ui->graph3DParametric->SetMinMaxZValue(minZValue,maxZValue);
        ui->graphSection->SetMinMaxZValue(minZValue,maxZValue);
    }
    else{
        ui->graphSpectrogram->SetMinMaxZValue(0,0);
        ui->graph3DSurface->SetMinMaxZValue(0,0);
        ui->graph3DParametric->SetMinMaxZValue(0,0);
        ui->graphSection->SetMinMaxZValue(0,0);
    }
}

void MainWindow::EnableSequenceChanged(){
    if(ui->checkSequence->isChecked()==true){
        sequenceTimer->start();
    }
    else{
        sequenceTimer->stop();
    }
}

void MainWindow::FPSSequenceChanged(){
    sequenceMs=1000/ui->spinFPS->value();
    sequenceTimer->setInterval(sequenceMs);
    mainController->SetSequenceFPS(ui->spinFPS->value());
}

void MainWindow::SequenceTimeout(){
    int i=ui->listPhaseImages->currentRow();
    if(i==ui->listPhaseImages->count()-1)
        i=0;
    else
        i++;

    ui->listPhaseImages->setCurrentRow(i);
}

void MainWindow::UpdatePhaseImagesList(bool updateGraph,int selectRow){
    int i;
    ImageSequenceController imgController;
    //Obtengo la secuencia de imágenes
    imgController=mainController->GetImageSequenceController();
    ImageListWidgetItem * item;
    int currentRow;

    if(selectRow==-1) currentRow=ui->listPhaseImages->currentRow();
    else if (selectRow==-2) currentRow=imgController.count()-1;
    else currentRow=selectRow;

    //Antes de hacer todo desconecto la señal de cambio de posición con el slot de actualizar el gráfico, para no generar un exceso de actualizaciones
    disconnect(ui->listPhaseImages,SIGNAL(currentRowChanged(int)),this,SLOT(UpdateGraph()));

    //Vacío y actualizo las imágenes
    ui->listPhaseImages->clear();
    for(i=0;i<imgController.size();i++){
        item = new ImageListWidgetItem(imgController.GetPreviewIndex(i),imgController.GetLabelIndex(i));
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->listPhaseImages->addItem(item);
    }
    //Vuelvo a conectar la señal
    connect(ui->listPhaseImages,SIGNAL(currentRowChanged(int)),this,SLOT(UpdateGraph()));

    //Vuelvo a cargar la secuencia, para no perder las vistas previas que se han generado al pedirlas
    mainController->SetImageSequenceController(imgController,false);

    //Recoloco la selección. En el caso de que ya no exista el índice, la coloco en la última posición
    if(currentRow < ui->listPhaseImages->count() && currentRow!=-1)
        ui->listPhaseImages->setCurrentRow(currentRow);
    else
        ui->listPhaseImages->setCurrentRow(ui->listPhaseImages->count()-1);

    if(updateGraph==true)
        this->UpdateGraph();

    UpdateMenusAndButtonsState();
}

void MainWindow::UpdateStatusBar(){
    QString aux;

    aux=MW_STATUS_CAMERA;
    if(mainController->CameraIsReady()==false)
        aux=aux+MW_STATUS_CAMOFF;
    else
        aux=aux+MW_STATUS_CAMON;
    cameraStateLabel->setText(aux);

    aux=MW_STATUS_CALC;
    if(mainController->GetMode()==MODE_CUDA)
        aux=aux+MW_STATUS_CALCCUDA;
    else
        aux=aux+MW_STATUS_CALCCPU;
    modeStateLabel->setText(aux);

    aux=MW_STATUS_OUTF+mainController->GetOutputFolder();
    currentPathStateLabel->setText(aux);

}

void MainWindow::closeEvent(QCloseEvent * event ){
    //Si no habia cogido ni siquiera referencia, o ya estaba guardado, salgo directamente
    if(mainController->GetRefSourceQImage()==NULL || mainController->GetCurrentProjectSaved()==true){
        event->accept();
        return;
    }
    switch(QMessageBox::question(this,MW_QUESTION,MW_CLOSEPROJECT,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,QMessageBox::Cancel)){
        case QMessageBox::Yes:
            this->SaveProject();
            event->accept();
            break;
        case QMessageBox::No:
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
    }
}

void MainWindow::LoadImagesFromFile(){

    int i;
    //Cojo el número de imágenes actual, pues sera donde tengo que después la selección
    int position=ui->listPhaseImages->count();
    QFileDialog fileDialog(this,tr("Load Images"));
    fileDialog.setNameFilter(tr("Image Files (*.jpg *.png *.bmp);;All Files (*.*)"));
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setLabelText(QFileDialog::Accept,tr("Load"));
    if(fileDialog.exec()){
        for(i=0;i<fileDialog.selectedFiles().count();i++){
            if(mainController->AddImageFromFile(fileDialog.selectedFiles().at(i))==false){
                QMessageBox::critical(this,MW_ERROR_LOAD,MW_FAILLOADIMAGE_LABEL+fileDialog.selectedFiles().at(i),QMessageBox::Ok);
                continue;
            }
        }

        //Coloco el índice en la primera imagen de las cargadas
        this->UpdatePhaseImagesList(true,position);

    }
}

void MainWindow::LoadImageFromCamera(){
    if(mainController->AddImageFromCamera()==false){
        QMessageBox::critical(this,MW_ERROR_LABEL,MW_FAILCAMIMAGE_LABEL,QMessageBox::Ok);
        return;
    }

    this->UpdatePhaseImagesList(true,ui->listPhaseImages->count());
}

void MainWindow::LiveMode(){
    sequenceTimer->stop();
    ui->checkSequence->setChecked(false);
    mainWindowLive->show();
}

void MainWindow::OpenHelp(){
    DialogHelp * dialogHelp;
    QString path="/help";
    QString homePage="help.html";
    dialogHelp  = new DialogHelp  (this,QCoreApplication::applicationDirPath()+path,homePage);
    dialogHelp ->exec();
    delete dialogHelp;
}

void MainWindow::OpenAbout(){
    QMessageBox::information(this,MW_ABOUT_TITLE,MW_ABOUT_BODY);
}

void MainWindow::UpdateGraph(){
    Data2D * data2d;
    Data1D * data1d;
    int currentImageRow=ui->listPhaseImages->currentRow();
    if(currentImageRow==-1) currentImageRow=0;

    //Gestiono las variables que indican si debo aplicar los algoritmos
    bool showUntilSelectedAlg=mainController->GetApplyUntilCurrentAlgorithm();
    int currentAlgRow;
    if(showUntilSelectedAlg==true)
        currentAlgRow=ui->listAlgorithms->currentRow()+1;
    else
        currentAlgRow=-1;


    //Si no hay una algoritmo de extraer una línea
    //O bien hasta el algoritmo que estamos mostrando en el caso de
    //tener activada la opción de mostrar sólo hasta el seleccionado
    //desactivo la pestaña, y si estaba en ella voy a la pestaña 0
    if(mainController->GetAlgorithmSequenceController().IsLineSelected()==false ||
            (showUntilSelectedAlg==true && mainController->GetAlgorithmSequenceController().IsLineSelected(currentAlgRow)==false)){
        ui->tabGraphs->setTabEnabled(3,false);
        if(ui->tabGraphs->currentIndex()==3)
            ui->tabGraphs->setCurrentIndex(0);
    }
    else{
        ui->tabGraphs->setTabEnabled(3,true);
    }

    switch(ui->tabGraphs->currentIndex()){
        case 0:
            data2d = mainController->GetPhaseData2D(currentImageRow,currentAlgRow);
            if(autoUpdateZ==true) ui->graphSpectrogram->SetInit(false);
            ui->graphSpectrogram->SetData2D(data2d);
            delete data2d;
            break;
        case 1:
            data2d = mainController->GetPhaseData2D(currentImageRow,currentAlgRow);
            if(autoUpdateZ==true) ui->graph3DSurface->SetInit(false);
            ui->graph3DSurface->SetData2D(data2d);
            delete data2d;
            break;
        case 2:
            data2d = mainController->GetPhaseData2D(currentImageRow,currentAlgRow);
            if(autoUpdateZ==true) ui->graph3DParametric->SetInit(false);
            ui->graph3DParametric->SetThreshold(paramThreshold);
            ui->graph3DParametric->SetData2D(data2d);
            delete data2d;
            break;
        case 3:
            data1d = mainController->GetPhaseData1D(currentImageRow,currentAlgRow);
            if(autoUpdateZ==true) ui->graphSection->SetInit(false);
            ui->graphSection->SetData1D(data1d);
            delete data1d;
            break;
    }
}

void MainWindow::MoveImageUp(){
    ImageSequenceController imgController;
    int currentRow=ui->listPhaseImages->currentRow();

    if(currentRow!=-1 && currentRow !=0 ){
        imgController=mainController->GetImageSequenceController();
        imgController.move(currentRow,currentRow-1);
        mainController->SetImageSequenceController(imgController);
        ui->listPhaseImages->setCurrentRow(currentRow-1);
        this->UpdatePhaseImagesList(true);
    }

}

void MainWindow::MoveImageDown(){
    ImageSequenceController imgController;
    int currentRow=ui->listPhaseImages->currentRow();

    if(currentRow!=-1 && currentRow != ui->listAlgorithms->count()-1){
        imgController=mainController->GetImageSequenceController();
        imgController.move(currentRow,currentRow+1);
        mainController->SetImageSequenceController(imgController);
        ui->listPhaseImages->setCurrentRow(currentRow+1);
        this->UpdatePhaseImagesList(true);

    }
}

void MainWindow::DeleteImage(){

    int currentRow=ui->listPhaseImages->currentRow();
    if(currentRow!=-1){

        mainController->DeleteImage(currentRow);

        this->UpdatePhaseImagesList(true);
    }

}

void MainWindow::DeleteAllImage(){
    int i;
    //Pido confirmación antes de eliminar todo
    if(QMessageBox::question(this,MW_CONFIRM,MW_REMOVEIMAGES,QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel)!=QMessageBox::Ok ){
        return;
    }

    for(i=ui->listPhaseImages->count()-1;i>=0;i--){
        mainController->DeleteImage(i);
    }

    this->UpdatePhaseImagesList(true);
}

void MainWindow::CameraSettings(){
    //Entro al cuadro modal
    DialogConfigureCam * dialogConfigureCam  = new DialogConfigureCam  (this,mainController->GetCameraController());
    //Si salí aceptando supongo que hubo algún cambio y que se debe guardar
    if (dialogConfigureCam->exec()==1){
        mainController->MarkProjectUnsaved();
        this->UpdateMenusAndButtonsState();
    }
    delete dialogConfigureCam;
}

void MainWindow::ProgramPreferences(){
    ProgramSettings settings;
    settings=mainController->GetProgramSettings();
    //Entro al cuadro modal
    DialogPreferences * dialogPreferences  = new DialogPreferences  (this,&settings);
    if(dialogPreferences ->exec()==QDialog::Rejected){
        delete dialogPreferences;
        return;
    }
    mainController->SetProgramSettings(settings);
    this->UpdateGraph();
    delete dialogPreferences;
}

void MainWindow::SetReference(){
    ui->checkSequence->setChecked(false);
    sequenceTimer->stop();
    //Entro al cuadro modal
    DialogSetReference * dialogSetReference  = new DialogSetReference (this,mainController);
    dialogSetReference->exec();
    delete dialogSetReference;

    this->UpdateMenusAndButtonsState();
}

void MainWindow::CheckCamera(){
    //Activo o desactivo los botones de la cámara
    if(mainController->CameraIsReady()==true){
        ui->actionCameraSettings->setEnabled(true);
        if(mainController->GetRefSourceQImage()==NULL){
             ui->actionLoadImageFromCamera->setEnabled(false);
             ui->actionStart->setEnabled(false);
        }
        else{
             ui->actionLoadImageFromCamera->setEnabled(true);
             ui->actionStart->setEnabled(true);
        }
    }
    else{
        ui->actionCameraSettings->setEnabled(false);
        ui->actionLoadImageFromCamera->setEnabled(false);
        ui->actionStart->setEnabled(false);
    }

    this->UpdateStatusBar();
}

void MainWindow::ImageLabelEdited(){
    int currentImageRow=ui->listPhaseImages->currentRow();

    ImageSequenceController imgController;
    QListWidgetItem * item;

    item=ui->listPhaseImages->item(currentImageRow);
    imgController=mainController->GetImageSequenceController();
    imgController.SetLabelIndex(currentImageRow,item->text());
    mainController->SetImageSequenceController(imgController);

    this->UpdatePhaseImagesList(false);
}

void MainWindow::LoadProject(){

    //Antes de nada, si no estaba guardado, pregunto si se quiere guardar lo anterior
    //Si no habia cogido ni siquiera referencia, o ya estaba guardado, salgo directamente
    if(mainController->GetRefSourceQImage()!=NULL && mainController->GetCurrentProjectSaved()==false){
        switch(QMessageBox::question(this,MW_QUESTION,MW_CLOSEPROJECT,QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes)){
            case QMessageBox::Yes:
                this->SaveProject();
                break;
        }
    }

    QFileDialog fileDialog(this,tr("Load Project"));
    fileDialog.setNameFilter(tr("JetProcessing Project Files (*.jpp)"));
    fileDialog.setDefaultSuffix("jpp");
    //fileDialog.setDirectory(mainController->GetOutputFolder());
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setLabelText(QFileDialog::Accept,tr("Load"));
    if(fileDialog.exec()){
        if(mainController->LoadProject(fileDialog.selectedFiles().at(0))==false){
            QMessageBox::critical(this,MW_ERROR_LABEL,MW_FAILLOADPROJ_LABEL1+fileDialog.selectedFiles().at(0)+MW_FAILLOADPROJ_LABEL2+QDir::tempPath(),QMessageBox::Ok);
        }
    }
    //Cargo las nuevas opciones de visualización
    autoUpdateZ=mainController->GetAutoScaleAxes();
    minZValue=mainController->GetMinZValue();
    maxZValue=mainController->GetMaxZValue();
    paramThreshold=mainController->GetThreshold();
    sequenceMs=1000/mainController->GetSequenceFPS();
    viewLevelCurves=mainController->GetViewLevelCurves();

    ui->editMinZ->setText(QString::QString().sprintf("%3.3g",minZValue));
    ui->editMaxZ->setText(QString::QString().sprintf("%3.3g",maxZValue));
    ui->editThreshold->setText(QString::QString().setNum(paramThreshold,'g',3));
    ui->spinFPS->setValue(1000/sequenceMs);
    sequenceTimer->setInterval(sequenceMs);
    ui->checkLevelCurves->setChecked(viewLevelCurves);
    ui->graphSpectrogram->ShowContour(viewLevelCurves);
    ui->editLabelZ->setText(mainController->GetZLabel());
    ui->checkUpdateZ->setChecked(autoUpdateZ);


    this->AutoScaleChanged();
    this->UpdateMenusAndButtonsState();
    this->UpdatePhaseImagesList(false,0);
    this->UpdateAlgorithmsList(false,-2);

    //Aseguro que los gráficos comenzarán de cero
    ui->graphSpectrogram->SetInit(false);
    ui->graph3DSurface->SetInit(false);
    ui->graph3DParametric->SetInit(false);
    ui->graphSection->SetInit(false);

    //Actualizo los gráficos
    this->UpdateGraph();


}

void MainWindow::SaveProject(){
    //Si no tenía projecto y hay algo que guardar, hago un guardar como
    if(mainController->GetCurrentProject().isEmpty()==true){
        if(mainController->GetCurrentProjectSaved()!=true)
            this->SaveProjectAs();
    }
    //Si sí tenía, simplemente lo guardo
    else{
        if(mainController->SaveProject(mainController->GetCurrentProject())!=true){
            QMessageBox::critical(this,MW_ERROR_LABEL,MW_FAILSAVEPROJ_LABEL1+mainController->GetCurrentProject()
                                  +MW_FAILSAVEPROJ_LABEL2+QDir::tempPath(),QMessageBox::Ok);
            return;
        }
        this->UpdateMenusAndButtonsState();
    }
}

void MainWindow::SaveProjectAs(){
    QImage * sourceImage;
    sourceImage=mainController->GetRefSourceQImage();
    QFileDialog fileDialog(this,tr("Save Project"));
    fileDialog.setNameFilter(tr("JetProcessing Project Files (*.jpp)"));
    fileDialog.setDefaultSuffix("jpp");
    //fileDialog.setDirectory(mainController->GetOutputFolder());
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setLabelText(QFileDialog::Accept,tr("Save"));
    if(fileDialog.exec()){
        if(mainController->SaveProject(fileDialog.selectedFiles().at(0))!=true){
            QMessageBox::critical(this,MW_ERROR_LABEL,MW_FAILSAVEPROJ_LABEL1+fileDialog.selectedFiles().at(0)
                                  +MW_FAILSAVEPROJ_LABEL2+QDir::tempPath(),QMessageBox::Ok);
            return;
        }
        this->UpdateMenusAndButtonsState();
    }
}

void MainWindow::SetOutputFolder(){
    QFileDialog folderDialog(this,MW_OFDIALOGTITLE);
    folderDialog.setFileMode(QFileDialog::DirectoryOnly);
    folderDialog.setOptions(QFileDialog::ShowDirsOnly);
    folderDialog.setDirectory(mainController->GetOutputFolder());

    if(folderDialog.exec()){
        mainController->SetOutputFolder(folderDialog.selectedFiles().at(0));
        this->UpdateStatusBar();
    }
}

void MainWindow::SaveImage(){
    //Si no hay imágenes, salgo
    if(ui->listPhaseImages->count()==0)
        return;

    //Paro la secuencia
    sequenceTimer->stop();
    ui->checkSequence->setChecked(false);

    //Creo el objeto que exporta
    GraphicsExporter graphsExporter(mainController);

    //Obtengo la imagen a exportar
    int currentImageRow=ui->listPhaseImages->currentRow();
    if(currentImageRow==-1)
        return;

    //Obtengo el tamaño personalizado de guardado
    QSize savingSize=mainController->GetSaveGraphicsCustomSize();

    //Gestiono las variables que indican si debo aplicar los algoritmos
    bool showUntilSelectedAlg=mainController->GetApplyUntilCurrentAlgorithm();
    int currentAlgRow;
    if(showUntilSelectedAlg==true)
        currentAlgRow=ui->listAlgorithms->currentRow()+1;
    else
        currentAlgRow=-1;

    //Consigo el tipo de gráfico
    GraphType type;
    switch (ui->tabGraphs->currentIndex()){
        case 0: type=GRAPH_SPECTROGRAM; break;
        case 1: type=GRAPH_3DSURFACE; break;
        case 2: type=GRAPH_PARAMETRIC; break;
        case 3: type=GRAPH_LINE; break;
    }


    //Obtengo el nombre por defecto
    QString name;
    name=graphsExporter.GetNameSingleGraph(type,currentImageRow,paramThreshold);


    QFileDialog fileDialog(this,tr("Save"));
    fileDialog.setDirectory(mainController->GetOutputFolder());
    fileDialog.selectFile(name);
    fileDialog.setNameFilter(tr("Image Files (*.jpg *.png *.bmp);;All Files (*.*)"));
    fileDialog.setDefaultSuffix(mainController->GetSaveGraphicsDefaultFormat());
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setLabelText(QFileDialog::Accept,tr("Save"));

    //Lanzo el diálogo
    if(fileDialog.exec()){
        //Ajusto los gráficos para guardar
        ui->graphSpectrogram->setAutoFillBackground(true);
        ui->graph3DSurface->setAutoFillBackground(true);
        ui->graph3DParametric->setAutoFillBackground(true);
        ui->graphSection->setAutoFillBackground(true);
        if(mainController->GetSaveGraphicsViewSize()==false){
            ui->graphSpectrogram->setFixedSize(savingSize);
            ui->graph3DSurface->setFixedSize(savingSize);
            ui->graph3DParametric->setFixedSize(savingSize);
            ui->graphSection->setFixedSize(savingSize);
        }


        //Dependiendo qué gráfico tenga activado salvo una cosa u otra
        QPixmap * pixmap;
        Data1D * data1d;
        Data2D * data2d;
        switch(type){
            case GRAPH_SPECTROGRAM:
                data2d = mainController->GetPhaseData2D(currentImageRow,currentAlgRow);
                if(autoUpdateZ==true) ui->graphSpectrogram->SetInit(false);
                ui->graphSpectrogram->SetData2D(data2d);
                pixmap= ui->graphSpectrogram->GetAsQPixmap();
                delete data2d;
                break;
            case GRAPH_3DSURFACE:
                data2d = mainController->GetPhaseData2D(currentImageRow,currentAlgRow);
                if(autoUpdateZ==true) ui->graph3DSurface->SetInit(false);
                ui->graph3DSurface->SetData2D(data2d);
                pixmap= ui->graph3DSurface->GetAsQPixmap();
                delete data2d;
                break;
            case GRAPH_PARAMETRIC:
                data2d = mainController->GetPhaseData2D(currentImageRow,currentAlgRow);
                if(autoUpdateZ==true) ui->graph3DParametric->SetInit(false);
                ui->graph3DParametric->SetData2D(data2d);
                ui->graph3DParametric->SetThreshold(paramThreshold);
                pixmap= ui->graph3DParametric->GetAsQPixmap();
                delete data2d;
                break;
            case GRAPH_LINE:
                data1d = mainController->GetPhaseData1D(currentImageRow,currentAlgRow);
                if(autoUpdateZ==true) ui->graphSection->SetInit(false);
                ui->graphSection->SetData1D(data1d);
                pixmap= ui->graphSection->GetAsQPixmap();
                delete data1d;
                break;
        }
        //Lo guardo
        if(pixmap!=NULL){
            if(graphsExporter.SaveSingleGraphic(fileDialog.selectedFiles().at(0),pixmap)==false){
                QMessageBox::critical(this,MW_ERROR_SAVE,fileDialog.selectedFiles().at(0),QMessageBox::Ok);
            }
            delete pixmap;
        }

        //Dejo los gráficos como estaban
        ui->graphSpectrogram->setAutoFillBackground(false);
        ui->graph3DSurface->setAutoFillBackground(false);
        ui->graph3DParametric->setAutoFillBackground(false);
        ui->graphSection->setAutoFillBackground(false);
        ui->graphSpectrogram->setMinimumSize(0,0);
        ui->graph3DSurface->setMinimumSize(0,0);
        ui->graph3DParametric->setMinimumSize(0,0);
        ui->graphSection->setMinimumSize(0,0);
        ui->graphSpectrogram->setMaximumSize(30000,30000);
        ui->graph3DSurface->setMaximumSize(30000,30000);
        ui->graph3DParametric->setMaximumSize(30000,30000);
        ui->graphSection->setMaximumSize(30000,30000);

        this->UpdateGraph();
    }



}

void MainWindow::SaveSeriesImage(){
    //Si no hay imágenes, salgo
    if(ui->listPhaseImages->count()==0)
        return;

    //Paro la secuencia
    sequenceTimer->stop();
    ui->checkSequence->setChecked(false);

    //Creo el objeto que exporta
    GraphicsExporter graphsExporter(mainController);

    //Obtengo el tamaño personalizado de guardado
    QSize savingSize=mainController->GetSaveGraphicsCustomSize();

    //Gestiono las variables que indican si debo aplicar los algoritmos
    bool showUntilSelectedAlg=mainController->GetApplyUntilCurrentAlgorithm();
    int currentAlgRow;
    if(showUntilSelectedAlg==true)
        currentAlgRow=ui->listAlgorithms->currentRow()+1;
    else
        currentAlgRow=-1;

    //Lanzo el cuadro de diálogo para seleccionar el nombre de la serie y se lo asigno al exportador
    DialogGetName * dialogGetName;
    dialogGetName  = new DialogGetName (this,&label);
    if(dialogGetName->exec()==QDialog::Rejected){
        delete dialogGetName;
        return;
    }
    delete dialogGetName;
    graphsExporter.SetSeriesLabel(label);

    //Consigo el tipo de gráfico
    GraphType type;
    switch (ui->tabGraphs->currentIndex()){
        case 0: type=GRAPH_SPECTROGRAM; break;
        case 1: type=GRAPH_3DSURFACE; break;
        case 2: type=GRAPH_PARAMETRIC; break;
        case 3: type=GRAPH_LINE; break;
    }


    //Ajusto los gráficos para guardar
    ui->graphSpectrogram->setAutoFillBackground(true);
    ui->graph3DSurface->setAutoFillBackground(true);
    ui->graph3DParametric->setAutoFillBackground(true);
    ui->graphSection->setAutoFillBackground(true);
    if(mainController->GetSaveGraphicsViewSize()==false){
        ui->graphSpectrogram->setFixedSize(savingSize);
        ui->graph3DSurface->setFixedSize(savingSize);
        ui->graph3DParametric->setFixedSize(savingSize);
        ui->graphSection->setFixedSize(savingSize);
    }

    //Dependiendo qué gráfico tenga activado salvo una cosa u otra
    QPixmap * pixmap=NULL;
    Data1D * data1d;
    Data2D * data2d;
    int i;
    for(i=0;i<ui->listPhaseImages->count();i++){
        switch(type){
            case GRAPH_SPECTROGRAM:
                data2d = mainController->GetPhaseData2D(i,currentAlgRow);
                if(autoUpdateZ==true) ui->graphSpectrogram->SetInit(false);
                ui->graphSpectrogram->SetData2D(data2d);
                pixmap= ui->graphSpectrogram->GetAsQPixmap();
                delete data2d;
                break;
            case GRAPH_3DSURFACE:
                data2d = mainController->GetPhaseData2D(i,currentAlgRow);
                if(autoUpdateZ==true) ui->graph3DSurface->SetInit(false);
                ui->graph3DSurface->SetData2D(data2d);
                pixmap= ui->graph3DSurface->GetAsQPixmap();
                delete data2d;
                break;
            case GRAPH_PARAMETRIC:
                data2d = mainController->GetPhaseData2D(i,currentAlgRow);
                if(autoUpdateZ==true) ui->graph3DParametric->SetInit(false);
                ui->graph3DParametric->SetData2D(data2d);
                ui->graph3DParametric->SetThreshold(paramThreshold);
                pixmap= ui->graph3DParametric->GetAsQPixmap();
                delete data2d;
                break;
            case GRAPH_LINE:
                data1d = mainController->GetPhaseData1D(i,currentAlgRow);
                if(autoUpdateZ==true) ui->graphSection->SetInit(false);
                ui->graphSection->SetData1D(data1d);
                pixmap= ui->graphSection->GetAsQPixmap();
                delete data1d;
                break;
        }

        //Lo guardo con el exportador
        if(pixmap!=NULL){
            if(graphsExporter.SaveGraphicSeries(pixmap,type,i,paramThreshold)==false){
                QMessageBox::critical(this,MW_ERROR_SAVE,MW_ERROR_SERIES+mainController->GetOutputFolder()+"/"+label,QMessageBox::Ok);
            }
            delete pixmap;
        }

    }

    //Dejo los gráficos como estaban
    ui->graphSpectrogram->setAutoFillBackground(false);
    ui->graph3DSurface->setAutoFillBackground(false);
    ui->graph3DParametric->setAutoFillBackground(false);
    ui->graphSection->setAutoFillBackground(false);
    ui->graphSpectrogram->setMinimumSize(0,0);
    ui->graph3DSurface->setMinimumSize(0,0);
    ui->graph3DParametric->setMinimumSize(0,0);
    ui->graphSection->setMinimumSize(0,0);
    ui->graphSpectrogram->setMaximumSize(30000,30000);
    ui->graph3DSurface->setMaximumSize(30000,30000);
    ui->graph3DParametric->setMaximumSize(30000,30000);
    ui->graphSection->setMaximumSize(30000,30000);


    this->UpdateGraph();
}

void MainWindow::SaveData(){
    //Si no hay imágenes, salgo
    if(ui->listPhaseImages->count()==0)
        return;

    //Paro la secuencia
    sequenceTimer->stop();
    ui->checkSequence->setChecked(false);

    //Gestiono las variables que indican si debo aplicar los algoritmos
    bool showUntilSelectedAlg=mainController->GetApplyUntilCurrentAlgorithm();
    int currentAlgRow;
    if(showUntilSelectedAlg==true)
        currentAlgRow=ui->listAlgorithms->currentRow()+1;
    else
        currentAlgRow=-1;

    //Obtengo la imagen seleccionada
    int currentImageRow=ui->listPhaseImages->currentRow();

    //Obtengo el nombre por defecto
    QString name;
    switch(ui->tabGraphs->currentIndex()){
        case 0:
        case 1:
        case 2: name=mainController->GetDefaultNameForExport2D(currentImageRow); break;
        case 3: name=mainController->GetDefaultNameForExport1D(currentImageRow); break;
    }

    //Configuro el cuadro de diálogo para obtener el nombre
    QFileDialog fileDialog(this,tr("Save"));
    fileDialog.setDirectory(mainController->GetOutputFolder());
    fileDialog.selectFile(name);
    fileDialog.setNameFilter(tr("Text Files (*.txt)"));
    fileDialog.setDefaultSuffix("txt");
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setLabelText(QFileDialog::Accept,tr("Save"));

    //Lanzo el diálogo
    if(fileDialog.exec()){
        //Salvo el gráfico dependiendo de la pestaña
        switch(ui->tabGraphs->currentIndex()){
            case 0:
            case 1:
            case 2:
                //Guardo datos 2d
                if(mainController->ExportSingleData2D(fileDialog.selectedFiles().at(0),currentImageRow,currentAlgRow)==false){
                    QMessageBox::critical(this,MW_ERROR_SAVE,fileDialog.selectedFiles().at(0),QMessageBox::Ok);
                }
                break;
            case 3:
                //Guardo datos 1d
                if(mainController->ExportSingleData1D(fileDialog.selectedFiles().at(0),currentImageRow,currentAlgRow)==false){
                    QMessageBox::critical(this,MW_ERROR_SAVE,fileDialog.selectedFiles().at(0),QMessageBox::Ok);
                }
                break;
        }
    }
}

void MainWindow::SaveSeriesData(){
    //Si no hay imágenes, salgo
    if(ui->listPhaseImages->count()==0)
        return;

    //Paro la secuencia
    sequenceTimer->stop();
    ui->checkSequence->setChecked(false);

    //Gestiono las variables que indican si debo aplicar los algoritmos
    bool showUntilSelectedAlg=mainController->GetApplyUntilCurrentAlgorithm();
    int currentAlgRow;
    if(showUntilSelectedAlg==true)
        currentAlgRow=ui->listAlgorithms->currentRow()+1;
    else
        currentAlgRow=-1;


    //Lanzo el cuadro de diálogo para seleccionar el nombre de la serie
    DialogGetName * dialogGetName;
    dialogGetName  = new DialogGetName (this,&label);

    if(dialogGetName->exec()==QDialog::Rejected){
        delete dialogGetName;
        return;
    }
    delete dialogGetName;

    //Dependiendo qué gráfico tenga activado salvo una cosa u otra
    switch(ui->tabGraphs->currentIndex()){
        case 0:
        case 1:
        case 2:
            //Guardo datos 2d para la serie
            if(mainController->ExportSeriesData2D(label,currentAlgRow)==false){
                QMessageBox::critical(this,MW_ERROR_SAVE,MW_ERROR_SERIES+mainController->GetOutputFolder()+"/"+label,QMessageBox::Ok);
            }
            break;
        case 3:
            //Guardo datos 1d para la serie
            if(mainController->ExportSeriesData1D(label,currentAlgRow)==false){
                QMessageBox::critical(this,MW_ERROR_SAVE,MW_ERROR_SERIES+mainController->GetOutputFolder()+"/"+label,QMessageBox::Ok);
            }
            break;
    }
}

void MainWindow::SaveSeriesVideo(){
    //Si no hay imágenes, salgo
    if(ui->listPhaseImages->count()==0)
        return;

    //Paro la secuencia
    sequenceTimer->stop();
    ui->checkSequence->setChecked(false);

    //Creo el objeto que exporta
    GraphicsExporter graphsExporter(mainController);

    //Pido un nombre de archivo con el cuadro de diálogo
    QFileDialog fileDialog(this,tr("Save Video"));
    fileDialog.setDirectory(mainController->GetOutputFolder());
    fileDialog.selectFile("*.avi");
    fileDialog.setNameFilter(tr("AVI Video File (*.avi)"));
    fileDialog.setDefaultSuffix("avi");
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setLabelText(QFileDialog::Accept,tr("Save"));

    //Lanzo el diálogo
    if(fileDialog.exec()==false)
        return;

    //Añado las imágenes al exportador, obteniendo el tamaño con que se han añadido
    QSize size=this->AddTempImagesExporter(&graphsExporter);

    //Exporto las imágenes en el directorio temporal como vídeo
    if(graphsExporter.SaveTempContentAsVideo(fileDialog.selectedFiles().at(0),ceil(1000/sequenceMs),size)==false){
        QMessageBox::critical(this,MW_ERROR_SAVE,fileDialog.selectedFiles().at(0),QMessageBox::Ok);
    }

}

void MainWindow::SaveSeriesAnimation(){
    //Si no hay imágenes, salgo
    if(ui->listPhaseImages->count()==0)
        return;

    //Paro la secuencia
    sequenceTimer->stop();
    ui->checkSequence->setChecked(false);

    //Creo el objeto que exporta
    GraphicsExporter graphsExporter(mainController);

    //Pido un nombre de archivo con el cuadro de diálogo
    QFileDialog fileDialog(this,tr("Save Animation"));
    fileDialog.setDirectory(mainController->GetOutputFolder());
    fileDialog.selectFile("*.gif");
    fileDialog.setNameFilter(tr("Animated GIF file (*.gif)"));
    fileDialog.setDefaultSuffix("gif");
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setLabelText(QFileDialog::Accept,tr("Save"));

    //Lanzo el diálogo
    if(fileDialog.exec()==false)
        return;

    //Añado las imágenes al exportador, obteniendo el tamaño con que se han añadido
    this->AddTempImagesExporter(&graphsExporter);

    //Exporto las imágenes en el directorio temporal como animación
    if(graphsExporter.SaveTempContentAsAnimation(fileDialog.selectedFiles().at(0),sequenceMs)==false){
        QMessageBox::critical(this,MW_ERROR_SAVE,fileDialog.selectedFiles().at(0),QMessageBox::Ok);
    }
}

QSize MainWindow::AddTempImagesExporter(GraphicsExporter * graphsExporter){

    //Obtengo el tamaño personalizado de guardado
    QSize savingSize=mainController->GetSaveGraphicsCustomSize();

    //Gestiono las variables que indican si debo aplicar los algoritmos
    bool showUntilSelectedAlg=mainController->GetApplyUntilCurrentAlgorithm();
    int currentAlgRow;
    if(showUntilSelectedAlg==true)
        currentAlgRow=ui->listAlgorithms->currentRow()+1;
    else
        currentAlgRow=-1;


    //Ajusto los gráficos para guardar
    ui->graphSpectrogram->setAutoFillBackground(true);
    ui->graph3DSurface->setAutoFillBackground(true);
    ui->graph3DParametric->setAutoFillBackground(true);
    ui->graphSection->setAutoFillBackground(true);
    if(mainController->GetSaveGraphicsViewSize()==false){
        ui->graphSpectrogram->setFixedSize(savingSize);
        ui->graph3DSurface->setFixedSize(savingSize);
        ui->graph3DParametric->setFixedSize(savingSize);
        ui->graphSection->setFixedSize(savingSize);
    }

    //Cojo el tamaño, para poder devolverlo luego
    QSize size=ui->graphSpectrogram->size();

    //Dependiendo qué gráfico tenga activado salvo una cosa u otra
    Data1D * data1d;
    Data2D * data2d;
    QListWidgetItem * img;
    QPixmap * pixmap;
    int i;
    for(i=0;i<ui->listPhaseImages->count();i++){
        img=ui->listPhaseImages->item(i);
        switch(ui->tabGraphs->currentIndex()){
            case 0:
                data2d = mainController->GetPhaseData2D(i,currentAlgRow);
                if(autoUpdateZ==true) ui->graphSpectrogram->SetInit(false);
                ui->graphSpectrogram->setTitle(img->text());
                ui->graphSpectrogram->SetData2D(data2d);
                pixmap= ui->graphSpectrogram->GetAsQPixmap();
                delete data2d;
                break;
            case 1:
                data2d = mainController->GetPhaseData2D(i,currentAlgRow);
                if(autoUpdateZ==true) ui->graph3DSurface->SetInit(false);
                ui->graph3DSurface->setTitle(img->text());
                ui->graph3DSurface->SetData2D(data2d);
                pixmap= ui->graph3DSurface->GetAsQPixmap();
                delete data2d;
                break;
            case 2:
                data2d = mainController->GetPhaseData2D(i,currentAlgRow);
                if(autoUpdateZ==true) ui->graph3DParametric->SetInit(false);
                ui->graph3DParametric->setTitle(img->text());
                ui->graph3DParametric->SetData2D(data2d);
                ui->graph3DParametric->SetThreshold(paramThreshold);
                pixmap= ui->graph3DParametric->GetAsQPixmap();
                delete data2d;
                break;
            case 3:
                data1d = mainController->GetPhaseData1D(i,currentAlgRow);
                if(autoUpdateZ==true) ui->graphSection->SetInit(false);
                ui->graphSection->setTitle(img->text());
                ui->graphSection->SetData1D(data1d);
                pixmap= ui->graphSection->GetAsQPixmap();
                delete data1d;
                break;
        }

        //Lo voy añadiendo al exportador
        if(pixmap!=NULL){
            if(graphsExporter->AddToTempFolder(pixmap,i)==false){
                QMessageBox::critical(this,MW_ERROR_SAVE,MW_ERROR_SAVE,QMessageBox::Ok);
            }
            delete pixmap;
        }
    }


    //Dejo los gráficos como estaban
    ui->graphSpectrogram->setAutoFillBackground(false);
    ui->graph3DSurface->setAutoFillBackground(false);
    ui->graph3DParametric->setAutoFillBackground(false);
    ui->graphSection->setAutoFillBackground(false);
    ui->graphSpectrogram->setMinimumSize(0,0);
    ui->graph3DSurface->setMinimumSize(0,0);
    ui->graph3DParametric->setMinimumSize(0,0);
    ui->graphSection->setMinimumSize(0,0);
    ui->graphSpectrogram->setMaximumSize(30000,30000);
    ui->graph3DSurface->setMaximumSize(30000,30000);
    ui->graph3DParametric->setMaximumSize(30000,30000);
    ui->graphSection->setMaximumSize(30000,30000);
    ui->graphSpectrogram->setTitle(QString::QString());
    ui->graph3DSurface->setTitle(QString::QString());
    ui->graph3DParametric->setTitle(QString::QString());
    ui->graphSection->setTitle(QString::QString());


    this->UpdateGraph();
    return size;
}

void MainWindow::UpdateMenusAndButtonsState(){

    //Detecto si hay una referencia tomada
    bool reference;
    if(mainController->GetRefSourceQImage()==NULL)
        reference=false;
    else
        reference=true;

    //Relacionados con los algoritmos
    //Añadir antes y después, salvo si estamos añadiendo un recta, y ya hay una,
    //o si estamos añadiendo un algoritmo después de la recta no permitido

    int algInt=ui->comboAlgorithms->currentIndex();
    switch(algInt){
        case ALG_GETLINE:
            if(mainController->GetAlgorithmSequenceController().IsLineSelected()){
                ui->buttonAddAlgorithmAfter->setEnabled(false);
                ui->buttonAddAlgorithmBefore->setEnabled(false);
            }
            else{
                ui->buttonAddAlgorithmAfter->setEnabled(true);
                ui->buttonAddAlgorithmBefore->setEnabled(true);
            }
            break;
        case ALG_RECTANGLE:
        case ALG_RMVTILT:
            if(mainController->GetAlgorithmSequenceController().IsLineSelected(ui->listAlgorithms->currentRow()))
                ui->buttonAddAlgorithmBefore->setEnabled(false);
            else
                ui->buttonAddAlgorithmBefore->setEnabled(true);
            if(mainController->GetAlgorithmSequenceController().IsLineSelected(ui->listAlgorithms->currentRow()+1))
                ui->buttonAddAlgorithmAfter->setEnabled(false);
            else
                ui->buttonAddAlgorithmAfter->setEnabled(true);

            break;
        default:
            if(reference==true){
                ui->buttonAddAlgorithmAfter->setEnabled(true);
                ui->buttonAddAlgorithmBefore->setEnabled(true);
            }
            else{
                ui->buttonAddAlgorithmAfter->setEnabled(false);
                ui->buttonAddAlgorithmBefore->setEnabled(false);
            }

    }


    //Guardar lista, salvo si está vacia
    if (ui->listAlgorithms->count()==0)
        ui->actionSaveAlgList->setEnabled(false);
    else
        ui->actionSaveAlgList->setEnabled(true);

    //Eliminar o editar, salvo si no hay selección
    if(ui->listAlgorithms->currentRow()==-1){
        ui->buttonDeleteAlgorithm->setEnabled(false);
        ui->actionDeleteAlgorithm->setEnabled(false);
        ui->buttonEditAlg->setEnabled(false);
        ui->actionEditAlg->setEnabled(false);
    }
    else{
        ui->buttonDeleteAlgorithm->setEnabled(true);
        ui->actionDeleteAlgorithm->setEnabled(true);
        //Sólo habilito editar si el algoritmo es de los que se pueden editar
        AlgorithmSequenceController algController=mainController->GetAlgorithmSequenceController();
        if(algController.HasAuxParametersIndex(ui->listAlgorithms->currentRow())){
            ui->buttonEditAlg->setEnabled(true);
            ui->actionEditAlg->setEnabled(true);
        }
        else{
            ui->buttonEditAlg->setEnabled(false);
            ui->actionEditAlg->setEnabled(false);
        }
    }
    //Subir, salvo si no hay selección o es el de arriba
    if(ui->listAlgorithms->currentRow()==-1 || ui->listAlgorithms->currentRow()==0){
        ui->buttonMoveAlgUp->setEnabled(false);
        ui->actionMoveAlgUp->setEnabled(false);
    }
    else{
        ui->buttonMoveAlgUp->setEnabled(true);
        ui->actionMoveAlgUp->setEnabled(true);
    }
    //Bajar, salvo si no hay selección o es el de abajo
    if(ui->listAlgorithms->currentRow()==-1 || ui->listAlgorithms->currentRow() == ui->listAlgorithms->count()-1){
        ui->buttonMoveAlgDown->setEnabled(false);
        ui->actionMoveAlgDown->setEnabled(false);
    }
    else{
        ui->buttonMoveAlgDown->setEnabled(true);
        ui->actionMoveAlgDown->setEnabled(true);
    }

    //Relacionados con las imágenes
    //Guardar todas las imágenes o borrar todas, salvo si está vacia o no hay referencia
    if (ui->listPhaseImages->count()==0 || reference==false){
        ui->actionSaveSeriesData->setEnabled(false);
        ui->actionSaveSeriesGraph->setEnabled(false);
        ui->actionSaveSeriesVideo->setEnabled(false);
        ui->actionDeleteAllImages->setEnabled(false);
    }
    else{
        ui->actionSaveSeriesData->setEnabled(true);
        ui->actionSaveSeriesGraph->setEnabled(true);
        ui->actionSaveSeriesVideo->setEnabled(true);
        ui->actionDeleteAllImages->setEnabled(true);
    }
    //Guardar como gif, solo si hay entre 1 y 50
    if(ui->listPhaseImages->count()==0 || ui->listPhaseImages->count()>50 || reference==false){
        ui->actionSaveSeriesAnimation->setEnabled(false);
    }
    else{
        ui->actionSaveSeriesAnimation->setEnabled(true);
    }

    //Eliminar, guardar imagen suelta y mostrar gráficos, salvo si no hay selección o referencia
    if(ui->listPhaseImages->currentRow()==-1 || reference==false){
        ui->buttonDeleteImage->setEnabled(false);
        ui->actionDeleteImage->setEnabled(false);
        ui->actionSaveData->setEnabled(false);
        ui->actionSaveGraph->setEnabled(false);
        ui->stackedCenter->setCurrentIndex(1);
    }
    else{
        ui->buttonDeleteImage->setEnabled(true);
        ui->actionDeleteImage->setEnabled(true);
        ui->actionSaveData->setEnabled(true);
        ui->actionSaveGraph->setEnabled(true);
        ui->stackedCenter->setCurrentIndex(0);
    }
    //Subir, salvo si no hay selección o es el de arriba
    if(ui->listPhaseImages->currentRow()==-1 || ui->listPhaseImages->currentRow()==0 || reference==false){
        ui->buttonMoveImageUp->setEnabled(false);
        ui->actionMoveImageUp->setEnabled(false);
    }
    else{
        ui->buttonMoveImageUp->setEnabled(true);
        ui->actionMoveImageUp->setEnabled(true);
    }
    //Bajar, salvo si no hay selección o es el de abajo
    if(ui->listPhaseImages->currentRow()==-1 || ui->listPhaseImages->currentRow() == ui->listPhaseImages->count()-1 || reference==false){
        ui->buttonMoveImageDown->setEnabled(false);
        ui->actionMoveImageDown->setEnabled(false);
    }
    else{
        ui->buttonMoveImageDown->setEnabled(true);
        ui->actionMoveImageDown->setEnabled(true);
    }



    //Cargar imagen, guardar projecto o cargar algoritmos, salvo si no hay referencia

    if(reference==false){
        ui->actionLoadImageFromCamera->setEnabled(false);
        ui->actionLoadImagesFromFile->setEnabled(false);
        ui->actionSaveProject->setEnabled(false);
        ui->actionSaveProjectAs->setEnabled(false);
        ui->actionLoadAlgList->setEnabled(false);
    }
    else{
        ui->actionLoadImageFromCamera->setEnabled(true);
        ui->actionLoadImagesFromFile->setEnabled(true);
        ui->actionSaveProject->setEnabled(true);
        ui->actionSaveProjectAs->setEnabled(true);
        ui->actionLoadAlgList->setEnabled(true);
    }

    //Pongo el nombre del proyecto abierto en el título
    QString title;
    title=MW_CAPTION;
    if(mainController->GetCurrentProject().isEmpty()){
        if(reference==true)
            this->setWindowTitle(title+" - * Untitled");
        else
            this->setWindowTitle(title);
    }
    else
        if(mainController->GetCurrentProjectSaved()==false)
            this->setWindowTitle(title+" - * "+mainController->GetCurrentProject());
        else
            this->setWindowTitle(title+" - "+mainController->GetCurrentProject());

    this->CheckCamera();

}
