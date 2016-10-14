#/**
# * @file   quazip.pri
# * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
# * @date   Mon Jul  23 2012
# *
# * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
# *
# * @brief  Archivo .pri que contiene la informaci�n relacionada con la biblioteca quazip.
# */

QuazipVersion = 0.4.4

QuazipBase = C:/quazip-$${QuazipVersion}


debug {
    LIBS += $${QuazipBase}/lib/quazipd.lib
}
release {
    LIBS += $${QuazipBase}/lib/quazip.lib
}


INCLUDEPATH += $${QuazipBase}/include


