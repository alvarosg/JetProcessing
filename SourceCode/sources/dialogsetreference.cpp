/**
 * @file   dialogsetreference.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase DialogSetReference.
 *
 */
#include "dialogsetreference.h"
#include "ui_dialogsetreference.h"

DialogSetReference::DialogSetReference(QWidget *parent, MainController * mainController_in) :
    QDialog(parent,Qt::CustomizeWindowHint | Qt::WindowTitleHint|Qt::WindowMaximizeButtonHint |Qt::WindowCloseButtonHint ),
    ui(new Ui::DialogSetReference){
    ui->setupUi(this);

    this->setModal(true);
    this->setWindowTitle(SR_CAPTION);

    //Inicio variables
    mainController=mainController_in;

    //Inicio los controles
    this->InitControls();
    this->CheckCamera();

    //Comienzo en el paso 0
    this->SetControlsStep(0);

    //Un temporizador para actualizar los conroles cada 5 segundos y poder detectar si se ha conectado la cámara
    QTimer * updateTimer;
    updateTimer= new QTimer(this);

    //La escena que necesitaré para el recuadro de dibujo de la máscara, con sus items
    maskScene = new MaskSelectionScene(ui->fullImageMaskPositionPreview);
    ui->fullImageMaskPositionPreview->setScene(maskScene);
    ui->fullImageMaskPositionPreview->scale(0.25, 0.25);
    maskPixmap =new QGraphicsPixmapItem(0,maskScene);
    maskRect = new QGraphicsRectItem(0,maskScene);
    maskRect->setPen(QPen::QPen(Qt::green));
    lineOrigin1 = new QGraphicsLineItem(0,maskScene);
    lineOrigin2 = new QGraphicsLineItem(0,maskScene);
    lineOrigin1->setPen(QPen::QPen(Qt::red));
    lineOrigin2->setPen(QPen::QPen(Qt::red));

    //Desactivo el keyboard tracking de las SpinBox
    ui->spinGainFourierPreview->setKeyboardTracking(false);
    ui->spinOriginX->setKeyboardTracking(false);
    ui->spinOriginY->setKeyboardTracking(false);
    ui->spinXMaxFourierMask->setKeyboardTracking(false);
    ui->spinXMaxMask->setKeyboardTracking(false);
    ui->spinXMinFourierMask->setKeyboardTracking(false);
    ui->spinXMinMask->setKeyboardTracking(false);
    ui->spinYMaxFourierMask->setKeyboardTracking(false);
    ui->spinYMaxMask->setKeyboardTracking(false);
    ui->spinYMinFourierMask->setKeyboardTracking(false);
    ui->spinYMinMask->setKeyboardTracking(false);

    //Lo mismo para la máscara de Fourier
    maskFourierScene = new MaskSelectionScene(ui->fullImageMaskPositionPreview);
    ui->fullImageFourierMaskPositionPreview->setScene(maskFourierScene);
    ui->fullImageFourierMaskPositionPreview->scale(0.25, 0.25);
    maskFourierPixmap =new QGraphicsPixmapItem(NULL,maskFourierScene);
    maskFourierRect = new QGraphicsRectItem(0,maskFourierScene);
    maskFourierRect->setPen(QPen::QPen(Qt::green));


    //Conecto todas las señales con los slots
    connect(ui->buttonNext, SIGNAL(clicked()),this, SLOT(NextStep()));
    connect(ui->buttonPrevious, SIGNAL(clicked()),this, SLOT(PreviousStep()));
    connect(ui->buttonFromFile, SIGNAL(clicked()),this, SLOT(GetImageFromFile()));
    connect(ui->buttonFromCamera, SIGNAL(clicked()),this, SLOT(GetImageFromCamera()));
    connect(ui->buttonCameraSettings, SIGNAL(clicked()),this, SLOT(RunCameraSettings()));
    connect(ui->buttonSavePicture, SIGNAL(clicked()),this, SLOT(SavePictureToFile()));
    connect(ui->editRealSizeX, SIGNAL(editingFinished()),this, SLOT(RealSizeXChanged()));
    connect(ui->editRealSizeY, SIGNAL(editingFinished()),this, SLOT(RealSizeYChanged()));
    connect(ui->radio0, SIGNAL(clicked()),this, SLOT(RadioOrCheckChanged()));
    connect(ui->radio90, SIGNAL(clicked()),this, SLOT(RadioOrCheckChanged()));
    connect(ui->radio180, SIGNAL(clicked()),this, SLOT(RadioOrCheckChanged()));
    connect(ui->radio270, SIGNAL(clicked()),this, SLOT(RadioOrCheckChanged()));
    connect(ui->checkMirrorX, SIGNAL(clicked()),this, SLOT(RadioOrCheckChanged()));
    connect(ui->checkMirrorY, SIGNAL(clicked()),this, SLOT(RadioOrCheckChanged()));
    connect(ui->editSizeX, SIGNAL(editingFinished()),this, SLOT(SizeXChanged()));
    connect(ui->editSizeY, SIGNAL(editingFinished()),this, SLOT(SizeYChanged()));
    connect(ui->spinXMinMask, SIGNAL(valueChanged(int)),this, SLOT(MaskSpinBoxChanged()));
    connect(ui->spinXMaxMask, SIGNAL(valueChanged(int)),this, SLOT(MaskSpinBoxChanged()));
    connect(ui->spinYMinMask, SIGNAL(valueChanged(int)),this, SLOT(MaskSpinBoxChanged()));
    connect(ui->spinYMaxMask, SIGNAL(valueChanged(int)),this, SLOT(MaskSpinBoxChanged()));
    connect(ui->spinOriginX, SIGNAL(valueChanged(int)),this, SLOT(MaskSpinBoxChanged()));
    connect(ui->spinOriginY, SIGNAL(valueChanged(int)),this, SLOT(MaskSpinBoxChanged()));
    connect(ui->spinXMinFourierMask, SIGNAL(valueChanged(int)),this, SLOT(MaskSpinBoxChanged()));
    connect(ui->spinXMaxFourierMask, SIGNAL(valueChanged(int)),this, SLOT(MaskSpinBoxChanged()));
    connect(ui->spinYMinFourierMask, SIGNAL(valueChanged(int)),this, SLOT(MaskSpinBoxChanged()));
    connect(ui->spinYMaxFourierMask, SIGNAL(valueChanged(int)),this, SLOT(MaskSpinBoxChanged()));

    connect(ui->buttonLoadMask, SIGNAL(clicked()),this, SLOT(LoadMaskFromFile()));
    connect(ui->buttonSaveMask, SIGNAL(clicked()),this, SLOT(SaveMaskToFile()));
    connect(maskScene, SIGNAL(mouseRightButtonPressed(int,int)),this, SLOT(MouseRightButtonPressed(int,int)));
    connect(maskScene, SIGNAL(mouseLeftButtonPressed(int,int)),this, SLOT(MouseLeftButtonPressed(int,int)));
    connect(maskScene, SIGNAL(mouseRightButtonMoved(int,int)),this, SLOT(MouseRightButtonMoved(int,int)));
    connect(maskScene, SIGNAL(mouseReleased(int,int)),this, SLOT(MouseReleased(int,int)));
    connect(maskScene, SIGNAL(mouseLeftButtonDoubleClick(int,int)),this, SLOT(MouseLeftButtonDoubleClick(int,int)));
    connect(maskScene, SIGNAL(mouseWheel(int,int,int)),this, SLOT(MouseWheel(int,int,int)));

    connect(ui->buttonLoadMaskFourier, SIGNAL(clicked()),this, SLOT(LoadMaskFromFile()));
    connect(ui->buttonSaveMaskFourier, SIGNAL(clicked()),this, SLOT(SaveMaskToFile()));
    connect(maskFourierScene, SIGNAL(mouseRightButtonPressed(int,int)),this, SLOT(MouseRightButtonPressed(int,int)));
    connect(maskFourierScene, SIGNAL(mouseLeftButtonPressed(int,int)),this, SLOT(MouseLeftButtonPressed(int,int)));
    connect(maskFourierScene, SIGNAL(mouseRightButtonMoved(int,int)),this, SLOT(MouseRightButtonMoved(int,int)));
    connect(maskFourierScene, SIGNAL(mouseReleased(int,int)),this, SLOT(MouseReleased(int,int)));
    connect(maskFourierScene, SIGNAL(mouseWheel(int,int,int)),this, SLOT(MouseWheel(int,int,int)));

    connect(ui->spinGainFourierPreview, SIGNAL(valueChanged(int)),this, SLOT(PreviewGainChanged()));
    connect(ui->buttonFinish, SIGNAL(clicked()),this, SLOT(accept()));
    connect(updateTimer, SIGNAL(timeout()),this, SLOT(CheckCamera()));
    updateTimer->start(5000);

}

void DialogSetReference::InitControls(){

    ui->tabWidget->setTabText(0,SR_STEP0_LABEL);
    ui->tabWidget->setTabText(1,SR_STEP1_LABEL);
    ui->tabWidget->setTabText(2,SR_STEP2_LABEL);
    ui->tabWidget->setTabText(3,SR_STEP3_LABEL);
    ui->tabWidget->setTabText(4,SR_STEP4_LABEL);

    ui->buttonNext->setText(SR_NEXT_LABEL);
    ui->buttonPrevious->setText(SR_PREVIOUS_LABEL);

    ui->buttonFromFile->setText(SR_FROMFILE_LABEL);
    ui->buttonFromCamera->setText(SR_FROMCAM_LABEL);
    ui->buttonCameraSettings->setText(SR_CAMSETTINGS_LABEL);
    ui->buttonSavePicture->setText(SR_SAVEPIC_LABEL);
    ui->labelRealSizeX->setText(SR_REALSIZEX_LABEL);
    ui->labelRealSizeY->setText(SR_REALSIZEY_LABEL);

    ui->checkMirrorX->setText(SR_HMIRROR_LABEL);
    ui->checkMirrorY->setText(SR_VMIRROR_LABEL);
    ui->radio0->setText(SR_ROTATE_0_LABEL);
    ui->radio90->setText(SR_ROTATE_90_LABEL);
    ui->radio180->setText(SR_ROTATE_180_LABEL);
    ui->radio270->setText(SR_ROTATE_270_LABEL);

    ui->labelSizeX->setText(SR_SIZE_X_LABEL);
    ui->labelSizeY->setText(SR_SIZE_Y_LABEL);

    ui->labelOrigin->setText(SR_ORIGIN);
    ui->labelHelpOrigin->setText(SR_MASK_HELPORIGIN);

    ui->labelMaskPosition->setText(SR_MASK);
    ui->labelHelpMask->setText(SR_MASK_HELPMASK);
    ui->labelXMinMask->setText(SR_MASK_XMIN);
    ui->labelXMaxMask->setText(SR_MASK_XMAX);
    ui->labelYMinMask->setText(SR_MASK_YMIN);
    ui->labelYMaxMask->setText(SR_MASK_YMAX);
    ui->buttonSaveMask->setText(SR_SAVE_VALUES);
    ui->buttonLoadMask->setText(SR_LOAD_VALUES);

    ui->labelMaskFourierPosition->setText(SR_MASK);
    ui->labelHelpMaskFourier->setText(SR_MASK_HELPMASK);
    ui->labelXMinFourierMask->setText(SR_MASK_XMIN);
    ui->labelXMaxFourierMask->setText(SR_MASK_XMAX);
    ui->labelYMinFourierMask->setText(SR_MASK_YMIN);
    ui->labelYMaxFourierMask->setText(SR_MASK_YMAX);
    ui->buttonSaveMaskFourier->setText(SR_SAVE_VALUES);
    ui->buttonLoadMaskFourier->setText(SR_LOAD_VALUES);

    ui->labelGainFourierPreview->setText(SR_GAIN_FOURIER_PREVIEW);

    ui->spectrogramFinalRef->setTitle(SR_SPECTROGRAM_TITLE);
    ui->spectrogramFinalRef->SetMinZValue(0);
    ui->spectrogramFinalRef->SetMaxZValue((float)6.28);
    ui->buttonFinish->setText(SR_FINISH_BUTTON);

    this->UpdateControls();

}

DialogSetReference::~DialogSetReference(){
    delete maskScene;
    delete maskFourierScene;
    delete ui;
}

void DialogSetReference::GetImageFromFile(){

    QFileDialog fileDialog(this,tr("Load Image"));
    fileDialog.setNameFilter(tr("Image Files (*.jpg *.png *.bmp);;All Files (*.*)"));
    //fileDialog.setDirectory(mainController->GetOutputFolder());
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setLabelText(QFileDialog::Accept,tr("Load"));
    if(fileDialog.exec()){
        if(mainController->LoadRefImageFromFile(fileDialog.selectedFiles().at(0))==false){
            QMessageBox::critical(this,SR_ERROR_LABEL,SR_FAILLOADIMAGE_LABEL+fileDialog.selectedFiles().at(0),QMessageBox::Ok);
            return;
        }


        this->UpdateControls();
    }

}


void DialogSetReference::GetImageFromCamera(){
    if(mainController->LoadRefImageFromCamera()!=true){
        QMessageBox::critical(this,SR_ERROR_LABEL,SR_FAILCAMIMAGE_LABEL,QMessageBox::Ok);
        return;
    }


    this->UpdateControls();

}

void DialogSetReference::RunCameraSettings(){

    DialogConfigureCam * dialogConfigureCam  = new DialogConfigureCam (this,mainController->GetCameraController());
    dialogConfigureCam->exec();
    delete dialogConfigureCam;

}

void DialogSetReference::UpdateControls(){
    QString string;
    QTransform transform;
    QImage transformed;
    int widthScene;
    int heightScene;
    QRect rect;
    QPoint point;
    QImage * sourceImage;
    QImage * auxImagePt;
    Data2D * phaseRef2D;
    sourceImage=mainController->GetRefSourceQImage();
    switch(step){
        case 0:

            //Quito las imagenes de las otras pestañas para que no estorben al tamaño mínimo
            ui->fullImagePreview->setPixmap(NULL);
            ui->fullImageMaskPreview->setPixmap(NULL);
            ui->fullImageFourierMaskPreview->setPixmap(NULL);

            //Actualizo la imagen y el tamaño real
            if (sourceImage!=NULL){
                ui->sourceImagePreview->setPixmap(QPixmap::fromImage(sourceImage->scaledToWidth(450)));
                //Si hay un número par de rotaciones de 90 grados
                if(mainController->GetSourceRotation()%2==0){
                    string.setNum(sourceImage->width()/mainController->GetPixelsMmX(),'g',3);
                    ui->editRealSizeX->setText(string);
                    string.setNum(sourceImage->height()/mainController->GetPixelsMmY(),'g',3);
                    ui->editRealSizeY->setText(string);
                }
                //Si hay un número impar de rotaciones de 90 grados
                else{
                    string.setNum(sourceImage->width()/mainController->GetPixelsMmY(),'g',3);
                    ui->editRealSizeX->setText(string);
                    string.setNum(sourceImage->height()/mainController->GetPixelsMmX(),'g',3);
                    ui->editRealSizeY->setText(string);
                }

                ui->buttonNext->setEnabled(true);
            }
            else{
                string.setNum((float)0,'g',3);
                ui->editRealSizeX->setText(string);
                string.setNum((float)0,'g',3);
                ui->editRealSizeY->setText(string);
                //Si no hay imagen, no dejo dar a siguiente
                ui->buttonNext->setEnabled(false);
            }

            break;
        case 1:
            //Quito las imagenes de las otras pestañas para que no estorben al tamaño mínimo
            ui->sourceImagePreview->setPixmap(NULL);
            ui->fullImageMaskPreview->setPixmap(NULL);
            ui->fullImageFourierMaskPreview->setPixmap(NULL);

            //Actualizo los checkboxes
            ui->checkMirrorX->setChecked(mainController->GetSourceHMirror());
            ui->checkMirrorY->setChecked(mainController->GetSourceVMirror());

            //Actualizo los radiobutton
            switch(mainController->GetSourceRotation()){
                case 0: ui->radio0->setChecked(true); break;
                case 1: ui->radio90->setChecked(true); break;
                case 2: ui->radio180->setChecked(true); break;
                case 3: ui->radio270->setChecked(true); break;
            }

            //Actualizo el tamaño de fullimage
            string.setNum(mainController->GetFullImageSizeX());
            ui->editSizeX->setText(string);
            string.setNum(mainController->GetFullImageSizeY());
            ui->editSizeY->setText(string);

            //Actualizo la vista previa del paso 2 y la escena
            if(sourceImage!=NULL){
                transformed = sourceImage->mirrored(mainController->GetSourceHMirror(),mainController->GetSourceVMirror());
                transform.rotate(90*mainController->GetSourceRotation());
                transformed = transformed.transformed(transform);
                fullImageTransformed=transformed;
                transformed = transformed.copy(0,0,mainController->GetFullImageSizeX(),mainController->GetFullImageSizeY());
                ui->fullImagePreview->setPixmap(QPixmap::fromImage(transformed.scaledToWidth(300)));
            }
            break;
        case 2:
            //Quito las imagenes de las otras pestañas para que no estorben al tamaño mínimo
            ui->sourceImagePreview->setPixmap(NULL);
            ui->fullImagePreview->setPixmap(NULL);
            ui->fullImageFourierMaskPreview->setPixmap(NULL);

            if(sourceImage!=NULL){
                //Para no mostrar zonas negras, escojo el valor más pequeño
                //entre el tamaño de la imagen original y el tamaño de la imagen "recortada"
                widthScene=qMin(mainController->GetFullImageSizeX(),mainController->GetRefSourceQImage()->width());
                heightScene=qMin(mainController->GetFullImageSizeY(),mainController->GetRefSourceQImage()->height());

                //Actualizo los spinbox
                rect=mainController->GetFullImageMask();
                point=mainController->GetOriginPoint();

                ui->spinOriginX->setMaximum(widthScene-1);
                ui->spinOriginY->setMaximum(heightScene-1);
                ui->spinXMinMask->setMaximum(widthScene-1);
                ui->spinXMaxMask->setMaximum(widthScene-1);
                ui->spinYMinMask->setMaximum(heightScene-1);
                ui->spinYMaxMask->setMaximum(heightScene-1);

                ui->spinOriginX->setValue(point.x());
                ui->spinOriginY->setValue(point.y());
                ui->spinXMinMask->setValue(rect.left());
                ui->spinXMaxMask->setValue(rect.right());
                ui->spinYMinMask->setValue(rect.top());
                ui->spinYMaxMask->setValue(rect.bottom());

                //Actualizo las vistas previas
                //Antes de dibujar el rectángulo, compruebo que efectivamente esta en los límites al menos de anchura
                rect.setWidth(qMin(widthScene-1,rect.width()));
                rect.setHeight(qMin(heightScene-1,rect.height()));

                //Pinto la vista previa d ela máscara ampliada
                transformed = fullImageTransformed.copy(rect);
                ui->fullImageMaskPreview->setPixmap(QPixmap::fromImage(transformed.scaled(
                        ui->fullImageMaskPreview->baseSize().width(),ui->fullImageMaskPreview->baseSize().height())));

                //Y la vista previa de la imagen para seleccionar la máscara
                ui->fullImageMaskPositionPreview->setSceneRect(0,0,widthScene,heightScene);
                transformed = fullImageTransformed.copy(0,0,widthScene,heightScene);
                maskPixmap->setPixmap(QPixmap::fromImage(transformed));
                maskRect->setRect(rect);

                //Pinto el origen de coordenadas
                lineOrigin1->setLine(point.x()-20,point.y()-20,point.x()+20,point.y()+20);
                lineOrigin2->setLine(point.x()-20,point.y()+20,point.x()+20,point.y()-20);

            }
            break;
        case 3:
            //Quito las imagenes de las otras pestañas para que no estorben al tamaño mínimo
            ui->sourceImagePreview->setPixmap(NULL);
            ui->fullImagePreview->setPixmap(NULL);
            ui->fullImageMaskPreview->setPixmap(NULL);

            if(sourceImage!=NULL){
                //El ancho total será el de la transformada de fourier, que coincidirá con el tamaño de fullImage
                widthScene=mainController->GetFullImageSizeX();
                heightScene=mainController->GetFullImageSizeY();

                //Actualizo los spinbox
                rect=mainController->GetFullImageFourierMask();
                ui->spinXMinFourierMask->setMaximum(widthScene-1);
                ui->spinXMaxFourierMask->setMaximum(widthScene-1);
                ui->spinYMinFourierMask->setMaximum(heightScene-1);
                ui->spinYMaxFourierMask->setMaximum(heightScene-1);

                ui->spinXMinFourierMask->setValue(rect.left());
                ui->spinXMaxFourierMask->setValue(rect.right());
                ui->spinYMinFourierMask->setValue(rect.top());
                ui->spinYMaxFourierMask->setValue(rect.bottom());

                //Pinto la vista previa de la máxcara ampliada
                auxImagePt=mainController->GetRefFullImageFourier(ui->spinGainFourierPreview->value());
                if(auxImagePt!=NULL){
                    transformed = auxImagePt->copy(rect);
                    ui->fullImageFourierMaskPreview->setPixmap(QPixmap::fromImage(transformed.scaled(
                            ui->fullImageFourierMaskPreview->baseSize().width(),ui->fullImageFourierMaskPreview->baseSize().height())));

                    //Y la vista previa de la imagen para seleccionar la máscara
                    ui->fullImageFourierMaskPositionPreview->setSceneRect(0,0,widthScene,heightScene);
                    transformed = auxImagePt->copy(0,0,widthScene,heightScene);
                    maskFourierPixmap->setPixmap(QPixmap::fromImage(transformed));
                }
                maskFourierRect->setRect(rect);

            }
            break;

        case 4:
            //Quito las imagenes de las otras pestañas para que no estorben al tamaño mínimo
            ui->sourceImagePreview->setPixmap(NULL);
            ui->fullImagePreview->setPixmap(NULL);
            ui->fullImageMaskPreview->setPixmap(NULL);
            ui->fullImageFourierMaskPreview->setPixmap(NULL);


            //Obtengo los datos
            phaseRef2D=mainController->GetRefPhaseData2D();


            ui->spectrogramFinalRef->SetInit(false);
            ui->spectrogramFinalRef->SetData2D(phaseRef2D);
            delete phaseRef2D;

    }
}

void DialogSetReference::SavePictureToFile(){
    QImage * sourceImage;
    sourceImage=mainController->GetRefSourceQImage();
    QFileDialog fileDialog(this,tr("Save Picture"));
    fileDialog.setNameFilter(tr("Image Files (*.jpg *.png *.bmp);;All Files (*.*)"));
    fileDialog.setDefaultSuffix("bmp");
    fileDialog.setDirectory(mainController->GetOutputFolder());
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setLabelText(QFileDialog::Accept,tr("Save"));
    if(sourceImage!=NULL && fileDialog.exec()){
        if(sourceImage->save(fileDialog.selectedFiles().at(0),0,100)!=true){
            QMessageBox::critical(this,SR_ERROR_LABEL,SR_FAILSAVEPIC_LABEL+fileDialog.selectedFiles().at(0),QMessageBox::Ok);
            return;
        }
    }
}

void DialogSetReference::SaveMaskToFile(){
    bool returnValue;
    QFileDialog fileDialog(this,tr("Save"));
    if(step==2) { fileDialog.setNameFilter(tr("(*.msk1)")); fileDialog.setDefaultSuffix("msk1");}
    else { fileDialog.setNameFilter(tr("(*.msk2)")); fileDialog.setDefaultSuffix("msk2");}

    fileDialog.setDirectory(mainController->GetOutputFolder());
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setLabelText(QFileDialog::Accept,tr("Save"));
    if(fileDialog.exec()){
        if(step==2) { returnValue=mainController->SaveFullImageMaskParameters(fileDialog.selectedFiles().at(0));}
        else { returnValue=mainController->SaveFullImageMaskFourierParameters(fileDialog.selectedFiles().at(0));}
        if(returnValue!=true){
            QMessageBox::critical(this,SR_ERROR_SAVE,fileDialog.selectedFiles().at(0),QMessageBox::Ok);
            return;
        }
    }
}

void DialogSetReference::LoadMaskFromFile(){


    bool returnValue;
    QFileDialog fileDialog(this,tr("Load"));
    if(step==2) { fileDialog.setNameFilter(tr("(*.msk1)")); fileDialog.setDefaultSuffix("msk1");}
    else { fileDialog.setNameFilter(tr("(*.msk2)")); fileDialog.setDefaultSuffix("msk2");}

    //fileDialog.setDirectory(mainController->GetOutputFolder());
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setLabelText(QFileDialog::Accept,tr("Load"));
    if(fileDialog.exec()){
        if(step==2) { returnValue=mainController->LoadFullImageMaskParameters(fileDialog.selectedFiles().at(0));}
        else { returnValue=mainController->LoadFullImageMaskFourierParameters(fileDialog.selectedFiles().at(0));}
        if(returnValue!=true){
            QMessageBox::critical(this,SR_ERROR_LOAD,fileDialog.selectedFiles().at(0),QMessageBox::Ok);
            return;
        }

        this->UpdateControls();
    }
}

void DialogSetReference::SetControlsStep(int newStep){
    int i;
    int nSteps=ui->tabWidget->count();

    if(newStep<0 || newStep >= nSteps)
        return;

    step=newStep;

    for(i=0;i<ui->tabWidget->count();i++)
        ui->tabWidget->setTabEnabled(i,false);

    ui->tabWidget->setTabEnabled(step,true);
    ui->tabWidget->setCurrentIndex(step);

    if(step==0){
        ui->buttonPrevious->setEnabled(false);
        ui->buttonNext->setEnabled(true);
    }
    else if (step==nSteps-1){
        ui->buttonPrevious->setEnabled(true);
        ui->buttonNext->setEnabled(false);
    }
    else{
        ui->buttonPrevious->setEnabled(true);
        ui->buttonNext->setEnabled(true);
    }
    this->UpdateControls();

}

void DialogSetReference::NextStep(){
    this->SetControlsStep(step+1);
}

void DialogSetReference::PreviousStep(){
    this->SetControlsStep(step-1);
}

void DialogSetReference::RealSizeXChanged(){
    QString string;
    float value;
    static float prev_value=-1;
    QImage * sourceImage;
    sourceImage=mainController->GetRefSourceQImage();
    value=(ui->editRealSizeX->text()).toFloat();
    if(prev_value!=value){
        prev_value=value;
        if(sourceImage!=NULL){
            //Si hay un número par de rotaciones de 90 grados
            if(mainController->GetSourceRotation()%2==0)
                mainController->SetPixelsMmX(sourceImage->width()/value);
            //Si hay un número impar de rotaciones de 90 grados
            else
                mainController->SetPixelsMmY(sourceImage->width()/value);
        }
        this->UpdateControls();
    }
}

void DialogSetReference::RealSizeYChanged(){
    QString string;
    float value;
    static float prev_value=-1;
    QImage * sourceImage;
    sourceImage=mainController->GetRefSourceQImage();
    value=(ui->editRealSizeY->text()).toFloat();
    if(prev_value!=value){
        prev_value=value;
        if(sourceImage!=NULL){
            //Si hay un número par de rotaciones de 90 grados
            if(mainController->GetSourceRotation()%2==0)
                mainController->SetPixelsMmY(sourceImage->height()/value);
            //Si hay un número impar de rotaciones de 90 grados
            else
                mainController->SetPixelsMmX(sourceImage->height()/value);
        }
        this->UpdateControls();
    }
}

void DialogSetReference::RadioOrCheckChanged(){
    int i;
    mainController->SetSourceHMirror(ui->checkMirrorX->isChecked());
    mainController->SetSourceVMirror(ui->checkMirrorY->isChecked());

    if(ui->radio90->isChecked())        i=1;
    else if(ui->radio180->isChecked())  i=2;
    else if(ui->radio270->isChecked())  i=3;
    else                                i=0;
    int auxInt=mainController->GetSourceRotation();
    mainController->SetSourceRotation(i);

    //Si se ha rotado en un número impar de ángulos de 90 grados, hay que
    //intercambiar los píxeles en x y en y
    float auxFloat;
    if((auxInt-i)%2!=0){
        auxFloat=mainController->GetPixelsMmX();
        mainController->SetPixelsMmX(mainController->GetPixelsMmY());
        mainController->SetPixelsMmY(auxFloat);
    }


    this->UpdateControls();
}

void DialogSetReference::SizeXChanged(){
    QString string;
    int value;
    static int prev_value=-1;
    value=(ui->editSizeX->text()).toInt();
    if(prev_value!=value){
        prev_value=value;
        mainController->SetFullImageSizeX(value);

    }
    this->UpdateControls();
}

void DialogSetReference::SizeYChanged(){
    QString string;
    int value;
    static int prev_value=-1;
    value=(ui->editSizeY->text()).toInt();
    if(prev_value!=value){
        prev_value=value;
        mainController->SetFullImageSizeY(value);

    }
    this->UpdateControls();
}

void DialogSetReference::MaskSpinBoxChanged(){
    QRect rect;
    QPoint point;

    //Si es la máscara normal
    if(step==2){
        rect=mainController->GetFullImageMask();
        rect.setRight(ui->spinXMaxMask->value());
        rect.setLeft(ui->spinXMinMask->value());
        rect.setTop(ui->spinYMinMask->value());
        rect.setBottom(ui->spinYMaxMask->value());

        if(rect.right()<rect.left())
            rect.setRight(rect.left());
        if(rect.bottom()<rect.top())
            rect.setBottom(rect.top());

        mainController->SetFullImageMask(rect);

        point.setX(ui->spinOriginX->value());
        point.setY(ui->spinOriginY->value());
        mainController->SetOriginPoint(point);
    }

    //Si es la máscara de fourier
    if(step==3){
            rect=mainController->GetFullImageFourierMask();
            rect.setRight(ui->spinXMaxFourierMask->value());
            rect.setLeft(ui->spinXMinFourierMask->value());
            rect.setTop(ui->spinYMinFourierMask->value());
            rect.setBottom(ui->spinYMaxFourierMask->value());

            if(rect.right()<rect.left())
                rect.setRight(rect.left());
            if(rect.bottom()<rect.top())
                rect.setBottom(rect.top());

            mainController->SetFullImageFourierMask(rect);
        }

    this->UpdateControls();
}

void DialogSetReference::PreviewGainChanged(){
    this->UpdateControls();
}

void DialogSetReference::MouseRightButtonPressed(int x,int y){
    //Téngase en cuenta que no hace falta redibujar nada, ya que  al cambiar los valores, automáticamente se actualiza la ventana
    //Máscara normal
    if(step==2){
        ui->spinXMinMask->setValue(x);
        ui->spinXMaxMask->setValue(x);
        ui->spinYMinMask->setValue(y);
        ui->spinYMaxMask->setValue(y);
    }
    //Máscara de Fourier
    if (step==3){
        ui->spinXMinFourierMask->setValue(x);
        ui->spinXMaxFourierMask->setValue(x);
        ui->spinYMinFourierMask->setValue(y);
        ui->spinYMaxFourierMask->setValue(y);
    }
}

void DialogSetReference::MouseLeftButtonPressed(int x,int y){
    //Activo el modo de arrastrar
    if(step==2){
        ui->fullImageMaskPositionPreview->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    if (step==3){
        ui->fullImageFourierMaskPositionPreview->setDragMode(QGraphicsView::ScrollHandDrag);
    }
}

void DialogSetReference::MouseRightButtonMoved(int x,int y){
    //Téngase en cuenta que no hace falta redibujar nada, ya que  al cambiar los valores, automáticamente se actualiza la ventana
    //Máscara normal
    if(step==2){
        if (x<ui->spinXMinMask->value())
            ui->spinXMinMask->setValue(x);
        if (y<ui->spinYMinMask->value())
            ui->spinYMinMask->setValue(y);
        ui->spinXMaxMask->setValue(x);
        ui->spinYMaxMask->setValue(y);
    }
    //Máscara de Fourier
    if (step==3){
        if (x<ui->spinXMinFourierMask->value())
            ui->spinXMinFourierMask->setValue(x);
        if (y<ui->spinYMinFourierMask->value())
            ui->spinYMinFourierMask->setValue(y);
        ui->spinXMaxFourierMask->setValue(x);
        ui->spinYMaxFourierMask->setValue(y);
    }
}

void DialogSetReference::MouseReleased(int x,int y){
    //Desactivo el modo de arrastrar
    if(step==2){
        ui->fullImageMaskPositionPreview->setDragMode(QGraphicsView::NoDrag);
    }
    if (step==3){
        ui->fullImageFourierMaskPositionPreview->setDragMode(QGraphicsView::NoDrag);
    }
    this->UpdateControls();
}

void DialogSetReference::MouseLeftButtonDoubleClick(int x,int y){
    ui->spinOriginX->setValue(x);
    ui->spinOriginY->setValue(y);
}


void DialogSetReference::MouseWheel(int delta, int, int){

    //Un parámetro para el zoom
    double scaleFactor = 1.05;
    //Máscara normal
    if(step==2){
        if(delta > 0){
            //Zoom in
            ui->fullImageMaskPositionPreview->scale(scaleFactor, scaleFactor);
        } else {
            //Zooming out
            ui->fullImageMaskPositionPreview->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
    }

    //Máscara de Fourier
    if (step==3){
        if(delta > 0){
            //Zoom in
            ui->fullImageFourierMaskPositionPreview->scale(scaleFactor, scaleFactor);
        } else {
            //Zooming out
            ui->fullImageFourierMaskPositionPreview->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
    }
    this->UpdateControls();
}

void DialogSetReference::CheckCamera(){
    //Activo o desactivo los botones de la cámara
    if(mainController->CameraIsReady()==true){
        ui->buttonCameraSettings->setEnabled(true);
        ui->buttonFromCamera->setEnabled(true);
    }
    else{
        ui->buttonCameraSettings->setEnabled(false);
        ui->buttonFromCamera->setEnabled(false);
    }
}

