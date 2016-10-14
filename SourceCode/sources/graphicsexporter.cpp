/**
 * @file   graphicsexporter.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase GraphicsExporter.
 *
 */
#include "graphicsexporter.h"

GraphicsExporter::GraphicsExporter(MainController * mainController_in){

    //Inicio variables
    mainController=mainController_in;
    seriesLabel="Default";


    //Inicio la carpeta temporal
    QString temporaryPath=QDir::tempPath();
    temporaryFolder=temporaryPath+"/"+"JetProcessingTempSeries";

    //Creo una carpeta temporal para el guardado en el directorio temporal
    //en el caso de que no exista ya
    if(QDir(temporaryFolder).exists()==false){
        QDir().mkdir(temporaryFolder);
    }

    //Vacío la carpeta temporal
    QDir tempDir(temporaryFolder);
    Q_FOREACH(QFileInfo info, tempDir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
        if (!info.isDir()) {
            QFile::remove(info.absoluteFilePath());
        }
    }
}

GraphicsExporter::~GraphicsExporter(){
    //Vacío la carpeta temporal
    QDir tempDir(temporaryFolder);
    Q_FOREACH(QFileInfo info, tempDir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
        if (!info.isDir()) {
            QFile::remove(info.absoluteFilePath());
        }
    }

    //Elimino la carpeta temporal
    QDir aux;
    aux.rmdir(temporaryFolder);
}


QString GraphicsExporter::GetNameSingleGraph(GraphType type,int imgNum,float paramThreshold){


    ImageSequenceController imgController=mainController->GetImageSequenceController();
    QString format;
    QString name;
    QString typeStr;
    format=mainController->GetSaveGraphicsDefaultFormat();

    switch(type){
        case GRAPH_SPECTROGRAM:     typeStr="spectrogram"; break;
        case GRAPH_3DSURFACE:       typeStr="3dSurface"; break;
        case GRAPH_PARAMETRIC:      typeStr=typeStr.setNum(paramThreshold,'g',3); typeStr="parametric"+typeStr; break;
        case GRAPH_LINE:            typeStr="line"; break;
    }

    name=imgController.GetLabelIndex(imgNum)+"_"+typeStr+"."+format;

    return name;
}

bool GraphicsExporter::SaveSingleGraphic(QString filePath,QPixmap * pixmap){
    if (pixmap==NULL){
        return false;
    }
    return pixmap->save(filePath,0,100);

}

void GraphicsExporter::SetSeriesLabel(QString label){
    seriesLabel=label;

    //Compruebo si la carpeta existe, y si no, la creo
    QString path=mainController->GetOutputFolder();

    if(QDir(path+'/'+seriesLabel).exists()==false){
        QDir().mkdir(path+'/'+seriesLabel);
    }

}

bool GraphicsExporter::SaveGraphicSeries(QPixmap * pixmap,GraphType type,int imgNum,float paramThreshold){
    if (pixmap==NULL){
        return false;
    }

    QString path=mainController->GetOutputFolder();
    path=path+'/'+seriesLabel+'/';
    QString name;
    name.sprintf("%05i",imgNum);
    name=name+"_"+this->GetNameSingleGraph(type,imgNum,paramThreshold);

    return pixmap->save(path+name,0,100);

}

bool GraphicsExporter::AddToTempFolder(QPixmap * pixmap,int imgNum){
    if (pixmap==NULL){
        return false;
    }

    //Calculo el nombre
    QString name;
    QString formatImage="bmp";

    name.sprintf("%05i",imgNum);
    name=name+"."+formatImage;

    return pixmap->save(temporaryFolder+"/"+name);

}

bool GraphicsExporter::SaveTempContentAsVideo(QString filePath,int fps,QSize size){

    //Las proceso con el programa
    QProcess * mencodeProcess = new QProcess();
    mencodeProcess->setWorkingDirectory(temporaryFolder);
    QStringList arg;
    QString program = QCoreApplication::applicationDirPath()+"/mencoder/mencoder.exe";
    QString config,codec;

    config.sprintf("w=%i:h=%i:fps=%i:type=bmp",size.width(),size.height(),fps);

    codec.sprintf("vcodec=mpeg4:vbitrate=%i",600 * fps * size.width() * size.height()/ 256);
    arg << "mf://*.bmp" << "-mf" << config << "-ovc"
    << "lavc" << "-lavcopts" << codec << "-oac" << "copy"
    << "-o" << filePath;


    //Ejecuto el proceso
    mencodeProcess->start(program, arg);
    bool returnValue=mencodeProcess->waitForFinished(60000);

    delete mencodeProcess;

    return returnValue;


}

bool GraphicsExporter::SaveTempContentAsAnimation(QString filePath,float delay){
    //Las proceso con el programa
    QProcess * convertProcess = new QProcess();
    convertProcess->setWorkingDirectory(temporaryFolder);
    QStringList arg;
    QString ticks;
    ticks.sprintf("%i",(int) delay/10);
    QString program = QCoreApplication::applicationDirPath()+"/gifconverter/convert.exe";

    arg << "-delay" << ticks << "-loop" << "0" << "*.bmp" << filePath;

    //Ejecuto el proceso
    convertProcess->start(program, arg);
    bool returnValue=convertProcess->waitForFinished(60000);

    delete convertProcess;

    return returnValue;
}
