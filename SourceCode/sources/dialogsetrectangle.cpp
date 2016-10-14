/**
 * @file   dialogsetrectangle.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase DialogSetRectangle.
 *
 */
#include "dialogsetrectangle.h"
#include "ui_dialogsetrectangle.h"

DialogSetRectangle::DialogSetRectangle(QWidget *parent,MainController * mainController_in,QRect * rect_in,int nAlg,int nImg,bool line) :
    QDialog(parent,Qt::CustomizeWindowHint | Qt::WindowTitleHint ),
    ui(new Ui::DialogSetRectangle){
    ui->setupUi(this);
    this->setModal(true);
    if(line==false)
        this->setWindowTitle(CR_CAPTION_RECT);
    else
        this->setWindowTitle(CR_CAPTION_LINE);

    //Inicio parámetros
    rect=rect_in;
    pointBeingMoved=0;
    mainController=mainController_in;

    //Desactivo el keyboard tracking de las SpinBox
    ui->spinXMax->setKeyboardTracking(false);
    ui->spinXMin->setKeyboardTracking(false);
    ui->spinYMax->setKeyboardTracking(false);
    ui->spinYMin->setKeyboardTracking(false);

    //Pido la imagen
    data2d=mainController->GetPhaseData2D(nImg,nAlg);
    QPixmap * phasePixmap;

    //Sólo si había datos
    if(data2d->GetSizeX()!=0 || data2d->GetSizeY()!=0){
        //Obtengo los parámetros para averiguar las coordenadas de los puntos
        dX=data2d->GetRatioPixelValueX();
        dY=data2d->GetRatioPixelValueY();
        minXValue=data2d->GetMinXValue();
        maxYValue=data2d->GetMaxYValue();

        //Creo la vista previa
        SpectrogramPlot * spectrogram;
        spectrogram = new SpectrogramPlot(NULL);
        spectrogram->SetData2D(data2d);
        spectrogram->enableAxis(0,false);
        spectrogram->enableAxis(1,false);
        spectrogram->enableAxis(2,false);
        spectrogram->setFixedSize(data2d->GetSizeX(),data2d->GetSizeY());
        phasePixmap=spectrogram->GetAsQPixmap();
        delete spectrogram;
        //Establezo valores máximos
        ui->spinXMin->setMaximum(data2d->GetSizeX()-1);
        ui->spinXMax->setMaximum(data2d->GetSizeX()-1);
        ui->spinYMin->setMaximum(data2d->GetSizeY()-1);
        ui->spinYMax->setMaximum(data2d->GetSizeY()-1);
    }
    //Si no habia datos, pongo el tamaño de la máscara
    else{
        delete data2d;
        data2d=NULL;
        QRect mask=mainController->GetAlgorithmSequenceController().GetMinRectangleAbsolute(nAlg);
        if (mask==QRect(0,0,0,0))
            mask=mainController->GetFullImageMask();
        phasePixmap=new QPixmap(mask.width(),mask.height());
        phasePixmap->fill(Qt::cyan);
        //Establezo valores máximos
        ui->spinXMin->setMaximum(mask.width()-1);
        ui->spinXMax->setMaximum(mask.width()-1);
        ui->spinYMin->setMaximum(mask.height()-1);
        ui->spinYMax->setMaximum(mask.height()-1);
    }

    //La escena que necesitaré para el recuadro de dibujo del rectángulo, con sus ítems
    rectScene = new MaskSelectionScene(ui->previewScene);
    ui->previewScene->setScene(rectScene);
    ui->previewScene->setMouseTracking(true);
    rectPixmap =new QGraphicsPixmapItem(*phasePixmap,0,rectScene);
    delete phasePixmap;

    if(line==false){
        rectRect = new QGraphicsRectItem(0,rectScene);
        rectRect->setPen(QPen::QPen(Qt::gray));
        rectLine=NULL;
        ui->graphLine->hide();
    }
    else{
        rectLine = new QGraphicsLineItem(0,rectScene);
        rectLine->setPen(QPen::QPen(Qt::gray));


        firstPointLine = new QGraphicsPolygonItem(0,rectScene) ;
        firstPointLine->setBrush(QBrush::QBrush(Qt::gray));
        secondPointLine = new QGraphicsPolygonItem(0,rectScene) ;
        secondPointLine->setBrush(QBrush::QBrush(Qt::gray));
        centerPointLine = new QGraphicsPolygonItem(0,rectScene) ;
        centerPointLine->setBrush(QBrush::QBrush(Qt::gray));
        rectRect=NULL;
        if(data2d!=NULL)
            ui->graphLine->show();
        else
            ui->graphLine->hide();
    }

    //Establezco etiquetas  de texto
    ui->labelRectPosition->setText(CR_RECT);
    ui->labelHelpMask->setText(CR_HELPMASK);
    ui->labelXMin->setText(CR_RECT_XMIN);
    ui->labelXMax->setText(CR_RECT_XMAX);
    ui->labelYMin->setText(CR_RECT_YMIN);
    ui->labelYMax->setText(CR_RECT_YMAX);

    connect(ui->spinXMin, SIGNAL(valueChanged(int)),this, SLOT(SpinBoxMinXChanged()));
    connect(ui->spinXMax, SIGNAL(valueChanged(int)),this, SLOT(SpinBoxMaxXChanged()));
    connect(ui->spinYMin, SIGNAL(valueChanged(int)),this, SLOT(SpinBoxMinYChanged()));
    connect(ui->spinYMax, SIGNAL(valueChanged(int)),this, SLOT(SpinBoxMaxYChanged()));
    connect(rectScene, SIGNAL(mouseLeftButtonPressed(int,int)),this, SLOT(MouseLeftButtonPressed(int,int)));
    connect(rectScene, SIGNAL(mouseLeftButtonMoved(int,int)),this, SLOT(MouseLeftButtonMoved(int,int)));
    connect(rectScene, SIGNAL(mouseMoved(int,int)),this,SLOT(MouseMoved(int,int)));
    connect(rectScene, SIGNAL(mouseReleased(int,int)),this, SLOT(MouseReleased(int,int)));
    connect(rectScene, SIGNAL(mouseWheel(int,int,int)),this, SLOT(MouseWheel(int,int,int)));
    connect(ui->buttonBox, SIGNAL(accepted()),this,SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()),this,SLOT(reject()));

    this->UpdateControls();


}

DialogSetRectangle::~DialogSetRectangle(){
    if(data2d!=NULL) delete data2d;
    delete ui;
}

void DialogSetRectangle::SpinBoxMinXChanged(){
    rect->setLeft(ui->spinXMin->value());

    //Si es un rectángulo, hay que hacerlo la dirección adecuada (Si es una recta, no)
    if(rectRect!=NULL){
        if(rect->right()<rect->left())
            rect->setRight(rect->left());
    }

    this->UpdateControls();
}

void DialogSetRectangle::SpinBoxMaxXChanged(){
    rect->setRight(ui->spinXMax->value());

    //Si es un rectángulo, hay que hacerlo la dirección adecuada (Si es una recta, no)
    if(rectRect!=NULL){
        if(rect->right()<rect->left())
            rect->setRight(rect->left());
    }

    this->UpdateControls();
}

void DialogSetRectangle::SpinBoxMinYChanged(){
    rect->setTop(ui->spinYMin->value());

    //Si es un rectángulo, hay que hacerlo la dirección adecuada (Si es una recta, no)
    if(rectRect!=NULL){
        if(rect->bottom()<rect->top())
            rect->setBottom(rect->top());
    }

    this->UpdateControls();
}

void DialogSetRectangle::SpinBoxMaxYChanged(){
    rect->setBottom(ui->spinYMax->value());

    //Si es un rectángulo, hay que hacerlo la dirección adecuada (Si es una recta, no)
    if(rectRect!=NULL){
        if(rect->bottom()<rect->top())
            rect->setBottom(rect->top());
    }

    this->UpdateControls();
}

void DialogSetRectangle::UpdateControls(){
    QString string;
    float valueFloat;

    //Actualizo los spinbox
    ui->spinXMin->setValue(rect->left());
    ui->spinXMax->setValue(rect->right());
    ui->spinYMin->setValue(rect->top());
    ui->spinYMax->setValue(rect->bottom());

    //Actualizo las etiquetas, poniendo cosas sólo si tengo una imagen
    if(data2d != NULL){
        valueFloat=minXValue+((float)rect->left())*dX;
        string.setNum(valueFloat,'g',4);
        string=string+CR_UNITS;
        ui->labelXMinMm->setText(string);

        valueFloat=minXValue+((float)rect->right())*dX;
        string.setNum(valueFloat,'g',4);
        string=string+CR_UNITS;
        ui->labelXMaxMm->setText(string);

        valueFloat=maxYValue-((float)rect->bottom())*dY;
        string.setNum(valueFloat,'g',4);
        string=string+CR_UNITS;
        ui->labelYMinMm->setText(string);

        valueFloat=maxYValue-((float)rect->top())*dY;
        string.setNum(valueFloat,'g',4);
        string=string+CR_UNITS;
        ui->labelYMaxMm->setText(string);
    }
    else{
        ui->labelXMinMm->setText("");
        ui->labelXMaxMm->setText("");
        ui->labelYMinMm->setText("");
        ui->labelYMaxMm->setText("");
    }

    //Pinto el rectángulo o la recta (y sus extremos)
    if(rectRect!=NULL)
        rectRect->setRect(*rect);
    else{
        rectLine->setLine(rect->left(),rect->top(),rect->right(),rect->bottom());
        QPolygonF polygon;
        polygon=GetPolygonCentered(rect->left(),rect->top());
        firstPointLine->setPolygon(polygon);
        polygon=GetPolygonCentered(rect->right(),rect->bottom());
        secondPointLine->setPolygon(polygon);
        polygon=GetPolygonCentered((rect->left()+rect->right())/2,(rect->top()+rect->bottom())/2);
        centerPointLine->setPolygon(polygon);
        //Pinto el gráfico de la recta
        if(data2d!=NULL){
            Data1D * data1d=data2d->GetData1D(*rect);
            ui->graphLine->SetInit(false);
            ui->graphLine->SetData1D(data1d);
            ui->graphLine->SetMinMaxZValue(data2d->GetMinZValue(),data2d->GetMaxZValue());
            delete data1d;
        }
    }


}

QPolygonF DialogSetRectangle::GetPolygonCentered(float x, float y){
    QPolygonF polygon;
    float width=3;
    polygon << QPointF(-width+x, 0+y) << QPointF(0+x, width+y) << QPointF(width+x, 0+y) << QPointF(0+x, -width+y);
    return polygon;
}

int DialogSetRectangle::GetPointBeingMoved(float x, float y){
    float width=3;

    if( x > (rect->right()+rect->left())/2-width && x < (rect->right()+rect->left())/2+width &&
             y > (rect->bottom()+rect->top())/2-width && y < (rect->bottom()+rect->top())/2+width )
        return 3;
    else if( x > rect->left()-width && x < rect->left()+width && y > rect->top()-width && y < rect->top()+width )
        return 1;
    else if( x > rect->right()-width && x < rect->right()+width && y > rect->bottom()-width && y < rect->bottom()+width )
        return 2;
    else
        return 0;
}


void DialogSetRectangle::MouseLeftButtonPressed(int x,int y){
    //Téngase en cuenta que no hace falta redibujar nada, ya que  al cambiar los valores, automáticamente se actualiza la ventana
    //Si es un rectángulo
    if(rectRect!=NULL){
        ui->spinXMin->setValue(x);
        ui->spinXMax->setValue(x);
        ui->spinYMin->setValue(y);
        ui->spinYMax->setValue(y);
    }
    //Si es una recta
    else{
        pointBeingMoved=GetPointBeingMoved(x,y);
        //si he pinchado en el vacío
        if(pointBeingMoved==0){
            ui->spinXMin->setValue(x);
            ui->spinXMax->setValue(x);
            ui->spinYMin->setValue(y);
            ui->spinYMax->setValue(y);
        }
    }
}

void DialogSetRectangle::MouseLeftButtonMoved(int x,int y){
    //Téngase en cuenta que no hace falta redibujar nada, ya que  al cambiar los valores, automáticamente se actualiza la ventana

    //Si es un rectángulo
    if(rectRect!=NULL){
        if (x<ui->spinXMin->value())
            ui->spinXMin->setValue(x);
        if (y<ui->spinYMin->value())
            ui->spinYMin->setValue(y);
        ui->spinXMax->setValue(x);
        ui->spinYMax->setValue(y);
    }
    //Si es una recta
    else{
        switch(pointBeingMoved){
            //Si he pinchado donde no hay nada
            case 0:
                ui->spinXMax->setValue(x);
                ui->spinYMax->setValue(y);
                break;
            //Si he pinchado en el primer punto
            case 1:
                ui->spinXMin->setValue(x);
                ui->spinYMin->setValue(y);
                break;
            //Si he pinchado en el segundo punto
            case 2:
                ui->spinXMax->setValue(x);
                ui->spinYMax->setValue(y);
                break;
            //Si he pinchado en el centro
            case 3:
                QRect rectAux=*rect;
                rectAux.setLeft(x-(rect->right()-rect->left())/2);
                rectAux.setRight(x+(rect->right()-rect->left())/2);
                rectAux.setTop(y-(rect->bottom()-rect->top())/2);
                rectAux.setBottom(y+(rect->bottom()-rect->top())/2);
                ui->spinXMin->setValue(rectAux.left());
                ui->spinXMax->setValue(rectAux.right());
                ui->spinYMin->setValue(rectAux.top());
                ui->spinYMax->setValue(rectAux.bottom());
                break;

        }
    }
}

void DialogSetRectangle::MouseReleased(int,int){
    this->UpdateControls();
    pointBeingMoved=0;
}

void DialogSetRectangle::MouseMoved(int x,int y){
    QString string;
    float mmX=minXValue + ((float)x)*dX;
    float mmY=maxYValue - ((float)y)*dY;
    //Sólo incluyo los milímetros si estoy sobre una imagen
    if(data2d!=NULL)
        string.sprintf("%5i (%4g%s), %5i (%4g%s)",x,mmX,CR_UNITS,y,mmY,CR_UNITS);
    else
        string.sprintf("%5i , %5i",x,y);
    ui->labelCurrentCoords->setText(string);

}

void DialogSetRectangle::MouseWheel(int delta, int, int){

    //Un parámetro para el zoom
    double scaleFactor = 1.05;

    if(delta > 0){
        //Zoom in
        ui->previewScene->scale(scaleFactor, scaleFactor);
    } else {
        //Zooming out
        ui->previewScene->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

    this->UpdateControls();
}
