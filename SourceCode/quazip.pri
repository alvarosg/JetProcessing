#/**
# * @file   quazip.pri
# * @author Álvaro Sánchez González <alvarosg@usal.es>
# * @date   Mon Jul  23 2012
# *
# * Copyright (c) 2012 Álvaro Sánchez González
# *
# * @brief  Archivo .pri que contiene la información relacionada con la biblioteca quazip.
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


