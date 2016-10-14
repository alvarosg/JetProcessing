/**
 * @file   imagesequencecontroller.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase ImageSequenceController.
 *
 */
#include "imagesequencecontroller.h"

ImageSequenceController::ImageSequenceController(){

}

void ImageSequenceController::AppendImageElement(PhaseImage * source_in,QString label,float pixelsMmX_in,float pixelsMmY_in,float pixelsFromOriginX_in,float pixelsFromOriginY_in){
    ImageElement img(source_in,label,pixelsMmX_in,pixelsMmY_in,pixelsFromOriginX_in,pixelsFromOriginY_in);
    this->append(img);
}

QString ImageSequenceController::GetLabelIndex(int index){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
    }
    return img.GetLabel();
}
PhaseImage * ImageSequenceController::GetPhaseImageSourceIndex(int index){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
    }
    return img.GetPhaseImageSource();
}
PhaseImage * ImageSequenceController::GetPhaseImageIndex(int index){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
    }
    return img.GetPhaseImage();
}
PhaseLine * ImageSequenceController::GetPhaseLineIndex(int index){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
    }
    return img.GetPhaseLine();
}
Data2D * ImageSequenceController::GetData2DIndex(int index){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
    }
    return img.GetData2D();
}
Data1D * ImageSequenceController::GetData1DIndex(int index){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
    }
    return img.GetData1D();
}
QImage ImageSequenceController::GetPreviewIndex(int index){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
    }
    return img.GetPreview();
}
bool ImageSequenceController::GetUpdatedIndex(int index){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
    }
    return img.GetUpdated();
}

float ImageSequenceController::GetPixelsMmXIndex(int index){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
    }
    return img.GetPixelsMmX();
}
float ImageSequenceController::GetPixelsMmYIndex(int index){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
    }
    return img.GetPixelsMmY();
}
int ImageSequenceController::GetPixelsFromOriginXIndex(int index){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
    }
    return img.GetPixelsFromOriginX();
}
int ImageSequenceController::GetPixelsFromOriginYIndex(int index){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
    }
    return img.GetPixelsFromOriginY();
}

void ImageSequenceController::SetLabelIndex(int index,QString value){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
        img.SetLabel(value);
        this->replace(index,img);
    }
}
void ImageSequenceController::SetPhaseImageIndex(int index,PhaseImage * value){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
        img.SetPhaseImage(value);
        this->replace(index,img);
    }
}
void ImageSequenceController::SetPhaseLineIndex(int index,PhaseLine * value){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
        img.SetPhaseLine(value);
        this->replace(index,img);
    }
}
void ImageSequenceController::SetPreviewIndex(int index,QImage value){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
        img.SetPreview(value);
        this->replace(index,img);
    }
}
void ImageSequenceController::SetUpdatedIndex(int index,bool value){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
        img.SetUpdated(value);
        this->replace(index,img);
    }
}

void ImageSequenceController::FreePointersIndex(int index){
    ImageElement img;
    if(index < this->count() && index >=0){
        img=this->at(index);
        img.FreePointers();
        this->replace(index,img);
    }
}


void ImageSequenceController::CheckLabels(){
    //Compruebo que no hay ninguna etiqueta repetida,si encuentro una, la modifico ligeramente

    int i=0,j;
    QString auxStr;
    ImageElement img,img2;

    while(i<this->size()){
        img=this->at(i);
        auxStr=img.GetLabel();
        for(j=0;j<i;j++){
            img2=this->at(j);
            //Si encuentro que es igual a alguna de las anteriores
            if(img2.GetLabel() == auxStr){
                //La modifico
                img.SetLabel(auxStr+"_C");
                //Y salgo del bucle decrementando, para volver a comprobar con el nuevo nombre
                this->replace(i,img);
                i--;
                break;
            }
        }
        i++;
    }

}

bool ImageSequenceController::LoadImageList(QString filePath, AbstractImageFactory * imageFactory){
    //Abro el archivo XML
    QFile fileXML(filePath);
    if(fileXML.open(QFile::ReadOnly | QFile::Text)==false)
        return false;

    QIODevice * device;
    device = &fileXML;
    QDomDocument doc;

    doc.setContent(device,true,NULL,NULL,NULL);

    QDomElement root = doc.documentElement();
    if(root.tagName()!="ImageList"){
        return false;
    }

    //Obtengo la cuenta
    int count;
    count=root.firstChildElement("Count").text().toInt();

    //Obtengo el nombre del archivo de datos binarios
    QString dataFileName=root.firstChildElement("DataFile").text();


    //Abro el archivo para los datos binarios en el mismo directorio que el pasado como argumento
    QString dataFileFullPath=QFileInfo(filePath).absoluteDir().absolutePath()+"/"+dataFileName;
    QFile fileData(dataFileFullPath);

    if (!fileData.open(QIODevice::ReadOnly))
         return false;
    QDataStream inData(&fileData);


    //Elimino la lista anterior
    int i;
    for(i=0;i<this->count();i++){
        this->FreePointersIndex(i);
    }
    this->clear();

    //Variables donde almaceno temporalmente los datos
    QString label;
    int pixelsFromOriginX;
    int pixelsFromOriginY;
    float pixelsMmX;
    float pixelsMmY;
    PhaseImage * phaseImage;
    int sizeX;
    int sizeY;
    float * buffer;

    //Extraigo imagen a imagen
    QDomElement image;
    image = root.firstChildElement("Image");
    for(i=0;i<count;i++){
        //Cargo los atributos
        label=image.firstChildElement("Label").text();
        pixelsFromOriginX=image.firstChildElement("PixelsFromOriginX").text().toInt();
        pixelsFromOriginY=image.firstChildElement("PixelsFromOriginY").text().toInt();
        pixelsMmX=image.firstChildElement("PixelsMmX").text().toFloat();
        pixelsMmY=image.firstChildElement("PixelsMmY").text().toFloat();
        sizeX=image.firstChildElement("SizeX").text().toInt();
        sizeY=image.firstChildElement("SizeY").text().toInt();

        //Cargo el buffer
        buffer=(float *) malloc (sizeof(float)*sizeX*sizeY);
        inData.readRawData((char *)buffer,sizeof(float)*sizeX*sizeY);
        phaseImage=imageFactory->CreatePhaseImage(NULL,0,0);
        phaseImage->LoadFromMemoryData(buffer,sizeX,sizeY);

        realloc(buffer,0); //No puedo usar free para liberar, pues QList tiene un método privado con el mismo nombre
        this->AppendImageElement(phaseImage,label,pixelsMmX,pixelsMmY,pixelsFromOriginX,pixelsFromOriginY);
        image = image.nextSiblingElement("Image");
    }
    return true;
}


bool ImageSequenceController::SaveImageList(QString filePath){

    //Nombre para el fichero de datos que crearé en la misma carpeta
    QString dataFileName="ImageData.dat";

    //Creo un archivo para los datos binarios en el mismo directorio que el pasado como argumento
    QString dataFileFullPath=QFileInfo(filePath).absoluteDir().absolutePath()+"/"+dataFileName;
    QFile fileData(dataFileFullPath);
    if (!fileData.open(QIODevice::WriteOnly | QIODevice::Truncate))
         return false;
    QDataStream outData(&fileData);

    //Creo el archivo XML
    QFile fileXML(filePath);
    if (!fileXML.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)){
        return false;
    }

    //Establezco la categoría principal para el archivo
    QDomDocument doc("ImageList");
    QDomElement root = doc.createElement("ImageList");
    doc.appendChild(root);

    QDomElement element;
    QDomText node;

    //Añado la cuenta del número de imágenes
    element = doc.createElement("Count");
    node = doc.createTextNode(QString::number(this->count()));
    element.appendChild(node);
    root.appendChild(element);

    //Añado el nombre del fichero con los datos binarios
    element = doc.createElement("DataFile");
    node = doc.createTextNode(dataFileName);
    element.appendChild(node);
    root.appendChild(element);

    //Objetos auxiliares para guardar los datos
    QDomElement auxImage;
    QDomElement auxLabel;
    QDomElement auxPixelsFromOriginX;
    QDomElement auxPixelsFromOriginY;
    QDomElement auxPixelsMmX;
    QDomElement auxPixelsMmY;
    QDomElement auxSizeX;
    QDomElement auxSizeY;
    QDomText auxNode;

    //Guardo la lista de algoritmos, uno por uno
    int i;
    PhaseImage * phaseImage;
    float * buffer;
    for(i=0;i<this->count();i++){
        auxImage = doc.createElement("Image");
        root.appendChild(auxImage);

        //Creo y enlazo los distintos elementos
        auxLabel = doc.createElement("Label");
        auxNode = doc.createTextNode(this->GetLabelIndex(i));
        auxLabel.appendChild(auxNode);
        auxImage.appendChild(auxLabel);

        auxPixelsFromOriginX = doc.createElement("PixelsFromOriginX");
        auxNode = doc.createTextNode(QString::number(this->GetPixelsFromOriginXIndex(i)));
        auxPixelsFromOriginX.appendChild(auxNode);
        auxImage.appendChild(auxPixelsFromOriginX);

        auxPixelsFromOriginY = doc.createElement("PixelsFromOriginY");
        auxNode = doc.createTextNode(QString::number(this->GetPixelsFromOriginYIndex(i)));
        auxPixelsFromOriginY.appendChild(auxNode);
        auxImage.appendChild(auxPixelsFromOriginY);

        auxPixelsMmX = doc.createElement("PixelsMmX");
        auxNode = doc.createTextNode(QString::number(this->GetPixelsMmXIndex(i)));
        auxPixelsMmX.appendChild(auxNode);
        auxImage.appendChild(auxPixelsMmX);

        auxPixelsMmY = doc.createElement("PixelsMmY");
        auxNode = doc.createTextNode(QString::number(this->GetPixelsMmYIndex(i)));
        auxPixelsMmY.appendChild(auxNode);
        auxImage.appendChild(auxPixelsMmY);

        phaseImage=this->GetPhaseImageSourceIndex(i);

        auxSizeX = doc.createElement("SizeX");
        auxNode = doc.createTextNode(QString::number(phaseImage->GetSizeX()));
        auxSizeX.appendChild(auxNode);
        auxImage.appendChild(auxSizeX);

        auxSizeY = doc.createElement("SizeY");
        auxNode = doc.createTextNode(QString::number(phaseImage->GetSizeY()));
        auxSizeY.appendChild(auxNode);
        auxImage.appendChild(auxSizeY);

        //Añado los datos binarios al archivo binario
        buffer=phaseImage->GetMemoryBuffer();
        if(buffer!=NULL){
            outData.writeRawData((char*) buffer,phaseImage->GetSizeX()*phaseImage->GetSizeY()*sizeof(float));
            realloc(buffer,0); //No puedo usar free para liberar, pues QList tiene un método privado con el mismo nombre
        }
    }


    //Guardo el archivo xml
    QIODevice * device;
    device = &fileXML;
    QTextStream out(device);
    doc.save(out,4);

    return true;
}
