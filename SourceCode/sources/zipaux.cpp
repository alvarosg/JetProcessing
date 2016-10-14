/**
 * @file   zipaux.cpp
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de la clase ZipAux.
 *
 */
#include "zipaux.h"

bool ZipAux::compressFolder(QString destinationZip, QString sourceFolder){
    QuaZip zip(destinationZip);
    if(!zip.open(QuaZip::mdCreate)) {
      return false;
    }
    QDir dir(sourceFolder);
    if(!dir.exists())
    {
        return false;
    }
    dir.makeAbsolute();
    QFileInfoList files=dir.entryInfoList();
    QFile inFile;
    QuaZipFile outFile(&zip);
    char c;
    foreach(QFileInfo file, files) {
      if(!file.isFile()) continue;
      inFile.setFileName(dir.absolutePath()+QString::fromLatin1("/")+file.fileName());
      if(!inFile.open(QIODevice::ReadOnly)) {
        return false;
      }
      if(!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(file.fileName(), inFile.fileName()))) {
        return false;
      }

      while(inFile.getChar(&c)&&outFile.putChar(c));
      if(outFile.getZipError()!=UNZ_OK) {
        return false;
      }
      outFile.close();
      if(outFile.getZipError()!=UNZ_OK) {
        return false;
      }
      inFile.close();
    }
    zip.close();
    if(zip.getZipError()!=0) {
      return false;
    }
    return true;
}



bool ZipAux::uncompressFolder(QString sourceZip, QString destinationFolder){
    QuaZip zip(sourceZip);
    if(!zip.open(QuaZip::mdUnzip)) {
      return false;
    }
    zip.setFileNameCodec("IBM866");
    QuaZipFileInfo info;

    QuaZipFile file(&zip);
    QFile out;
    QString name;
    char c;
    for(bool more=zip.goToFirstFile(); more; more=zip.goToNextFile()) {
      if(!zip.getCurrentFileInfo(&info)) {
        return false;
      }

      if(!file.open(QIODevice::ReadOnly)) {
        return false;
      }
      name=file.getActualFileName();
      if(file.getZipError()!=UNZ_OK) {
        return false;
      }
      QDir aux(destinationFolder);
      if(!aux.exists())
      {
          return false;
      }
      aux.makeAbsolute();
      QString dirn = aux.absolutePath() + QString::fromLatin1("/") + name;
      if (name.contains('/')) {
        dirn.chop(dirn.length() - dirn.lastIndexOf("/"));
        QDir().mkpath(dirn);
      }
      out.setFileName(aux.absolutePath() + QString::fromLatin1("/") + name);
      out.open(QIODevice::WriteOnly);
      char buf[4096];
      int len = 0;
      while (file.getChar(&c)) {
        buf[len++] = c;
        if (len >= 4096) {
          out.write(buf, len);
          len = 0;
        }
      }
      if (len > 0) {
        out.write(buf, len);
      }
      out.close();
      if(file.getZipError()!=UNZ_OK) {
        return false;
      }
      if(!file.atEnd()) {
        return false;
      }
      file.close();
      if(file.getZipError()!=UNZ_OK) {
        return false;
      }
    }
    zip.close();
    if(zip.getZipError()!=UNZ_OK) {
      return false;
    }
    return true;
}
