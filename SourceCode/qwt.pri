#/**
# * @file   qwt.pri
# * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
# * @date   Mon Jul  23 2012
# *
# * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
# *
# * @brief  Archivo .pri que contiene la parte relacionada con la biblioteca qwt para la generaci�n del makefile.
# */

DEFINES+=QWT_DLL

QwtVersion = 6.0.1

QwtBase = C:/Qwt-$${QwtVersion}


debug {
    LIBS += $${QwtBase}/lib/qwtd.lib
    LIBS += $${QwtBase}/lib/qwtmathmld.lib
}
release {
    LIBS += $${QwtBase}/lib/qwt.lib
    LIBS += $${QwtBase}/lib/qwtmathml.lib
}


INCLUDEPATH += $${QwtBase}/include


