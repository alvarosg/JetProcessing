/**
 * @file   maskselectionscene.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase MaskSelectionScene.
 *
 */
#include "maskselectionscene.h"

MaskSelectionScene::MaskSelectionScene(QObject * parent ) :QGraphicsScene(parent){
    ctrlPressed=false;
}

void MaskSelectionScene::keyPressEvent(QKeyEvent  * event){
    ctrlPressed=true;
}

void MaskSelectionScene::keyReleaseEvent(QKeyEvent  *event){
    ctrlPressed=false;
}

void MaskSelectionScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    int x = event->scenePos().x();
    int y = event->scenePos().y();
    if(ctrlPressed==false && Qt::RightButton & event->buttons())
        emit mouseRightButtonPressed(x, y);
    if(ctrlPressed==false && Qt::LeftButton & event->buttons())
        emit mouseLeftButtonPressed(x, y);
}

void MaskSelectionScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    int x = event->scenePos().x();
    int y = event->scenePos().y();
    if(ctrlPressed==false && Qt::LeftButton & event->buttons())
        emit mouseLeftButtonDoubleClick(x, y);
}

void MaskSelectionScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    int x = event->scenePos().x();
    int y = event->scenePos().y();

    if(ctrlPressed==false && Qt::RightButton & event->buttons())
        emit mouseRightButtonMoved(x, y);
    if(ctrlPressed==false && Qt::LeftButton & event->buttons())
        emit mouseLeftButtonMoved(x, y);
    emit mouseMoved(x,y);

}

void MaskSelectionScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    int x = event->scenePos().x();
    int y = event->scenePos().y();
    if(ctrlPressed==false){
        emit mouseReleased(x, y);
    }
    if(ctrlPressed==true){
        emit mouseCtrlReleased(x, y);
    }

}

void MaskSelectionScene::wheelEvent ( QGraphicsSceneWheelEvent * event ){
    int x = event->scenePos().x();
    int y = event->scenePos().y();
    emit mouseWheel(event->delta(),x,y);
}
