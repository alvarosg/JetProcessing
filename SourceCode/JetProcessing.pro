#/**
# * @file   JetProcessing.pro
# * @author Álvaro Sánchez González <alvarosg@usal.es>
# * @date   Mon Jul  23 2012
# *
# * Copyright (c) 2012 Álvaro Sánchez González
# *
# * @brief  Archivo .pro para generar el "makefile" del proyecto.
# */
QT       += core gui \
            webkit \
            xml

TARGET = JetProcessing
TEMPLATE = app

INCLUDEPATH += headers

SOURCES += \
    sources/spectrogramplot.cpp \
    sources/sourcepicture.cpp \
    sources/plotdata2d.cpp \
    sources/plotdata1d.cpp \
    sources/plot3ddata2d.cpp \
    sources/phaselinecuda.cpp \
    sources/phaselinecpu.cpp \
    sources/phaseline.cpp \
    sources/phaseimagecuda.cpp \
    sources/phaseimagecpu.cpp \
    sources/phaseimage.cpp \
    sources/parametricsurface3dplot.cpp \
    sources/maskselectionscene.cpp \
    sources/mainwindowlive.cpp \
    sources/mainwindow.cpp \
    sources/maincontroller.cpp \
    sources/main.cpp \
    sources/imagesequencecontroller.cpp \
    sources/imagelistwidgetitem.cpp \
    sources/imagefactoryforcuda.cpp \
    sources/imagefactoryforcpu.cpp \
    sources/fullimagecuda.cpp \
    sources/fullimagecpu.cpp \
    sources/fullimage.cpp \
    sources/dialogsetreference.cpp \
    sources/dialogsetrectangle.cpp \
    sources/dialogsetabel.cpp \
    sources/dialogmodeselection.cpp \
    sources/dialoggetname.cpp \
    sources/dialoggetfloat.cpp \
    sources/dialogconfigurecam.cpp \
    sources/data2d.cpp \
    sources/data1d.cpp \
    sources/cudasafe.cu \
    sources/cudaphaseinterface.cu \
    sources/cudageneralpurpose.cu \
    sources/cudafullimageinterface.cu \
    sources/cameraueye.cpp \
    sources/camerasimulator.cpp \
    sources/cameraparameters.cpp \
    sources/camerafactoryforueye.cpp \
    sources/camerafactoryforsimulator.cpp \
    sources/cameracontroller.cpp \
    sources/algorithmsequencecontroller.cpp \
    sources/algorithmlistwidgetitem.cpp \
    sources/abstractplot.cpp \
    sources/abstractimagefactory.cpp \
    sources/abstractcamerafactory.cpp \
    sources/abstractcamera.cpp \
    sources/surface3dplot.cpp \
    sources/cpufullimageinterface.cpp \
    sources/programsettings.cpp \
    sources/dialogpreferences.cpp \
    sources/dataresult.cpp \
    sources/dialoghelp.cpp \
    sources/algorithm.cpp \
    sources/imageelement.cpp \
    sources/cpuphaseinterface.cpp \
    sources/graph1d.cpp \
    sources/zipaux.cpp \
    sources/graphicsexporter.cpp

HEADERS += \
    headers/spectrogramplot.h \
    headers/sourcepicture.h \
    headers/plotdata2d.h \
    headers/plotdata1d.h \
    headers/plot3ddata2d.h \
    headers/phaselinecuda.h \
    headers/phaselinecpu.h \
    headers/phaseline.h \
    headers/phaseimagecuda.h \
    headers/phaseimagecpu.h \
    headers/phaseimage.h \
    headers/parametricsurface3dplot.h \
    headers/maskselectionscene.h \
    headers/mainwindowlive.h \
    headers/mainwindow.h \
    headers/maincontroller.h \
    headers/imagesequencecontroller.h \
    headers/imagelistwidgetitem.h \
    headers/imagefactoryforcuda.h \
    headers/imagefactoryforcpu.h \
    headers/fullimagecuda.h \
    headers/fullimagecpu.h \
    headers/fullimage.h \
    headers/dialogsetreference.h \
    headers/dialogsetrectangle.h \
    headers/dialogsetabel.h \
    headers/dialogmodeselection.h \
    headers/dialoggetname.h \
    headers/dialoggetfloat.h \
    headers/dialogconfigurecam.h \
    headers/data2d.h \
    headers/data1d.h \
    headers/cudasafe.h \
    headers/cudaphaseinterface.h \
    headers/cudageneralpurpose.h \
    headers/cudafullimageinterface.h \
    headers/complex.h \
    headers/cameraueye.h \
    headers/camerasimulator.h \
    headers/cameraparameters.h \
    headers/camerafactoryforueye.h \
    headers/camerafactoryforsimulator.h \
    headers/cameracontroller.h \
    headers/algorithmsequencecontroller.h \
    headers/algorithmlistwidgetitem.h \
    headers/abstractplot.h \
    headers/abstractimagefactory.h \
    headers/abstractcamerafactory.h \
    headers/abstractcamera.h \
    headers/surface3dplot.h \
    headers/cpufullimageinterface.h \
    headers/programsettings.h \
    headers/dialogpreferences.h \
    headers/dataresult.h \
    headers/dialoghelp.h \
    headers/algorithm.h \
    headers/imageelement.h \
    headers/cpuphaseinterface.h \
    headers/graph1d.h \
    headers/zipaux.h \
    headers/graphicsexporter.h

FORMS += \
    forms/mainwindowlive.ui \
    forms/mainwindow.ui \
    forms/dialogsetreference.ui \
    forms/dialogsetrectangle.ui \
    forms/dialogsetabel.ui \
    forms/dialogmodeselection.ui \
    forms/dialoggetname.ui \
    forms/dialoggetfloat.ui \
    forms/dialogconfigurecam.ui \
    forms/dialogpreferences.ui \
    forms/dialoghelp.ui

debug_and_release {
    CONFIG -= debug_and_release
    CONFIG += debug_and_release
}

CONFIG(debug, debug|release) {
    CONFIG -= debug release
    CONFIG += debug
}

CONFIG(release, debug|release) {
    CONFIG -= debug release
    CONFIG += release
}

#Para el fijar el icono del ejecutable
RC_FILE = resources/resources.rc

#Añado QWT
include(qwt.pri)
#Añado QWT3D
include(qwt3d.pri)
#Añado CUDA
include(cuda.pri)
#Añado las bibliotecas de la cámara
include(cameraueye.pri)
#Añado Quazip
include(quazip.pri)

RESOURCES += \
    resources/resources.qrc






