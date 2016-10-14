#/**
# * @file   qwt3d.pri
# * @author Álvaro Sánchez González <alvarosg@usal.es>
# * @date   Mon Jul  23 2012
# *
# * Copyright (c) 2012 Álvaro Sánchez González
# *
# * @brief  Archivo .pri que contiene la parte relacionada con la biblioteca qwt3d para la generación del makefile.
# */
QT += opengl

Qwt3dBase = C:/Qwtplot3d

DEFINES  += QT_DLL QWT3D_DLL

debug{
    LIBS += $${Qwt3dBase}/lib/qwtplot3dd.lib
}
release{
    LIBS += $${Qwt3dBase}/lib/qwtplot3d.lib
}


INCLUDEPATH += $${Qwt3dBase}/include
TEMPLATE = app

