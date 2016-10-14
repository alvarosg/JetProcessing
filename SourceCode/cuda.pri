#/**
# * @file   cuda.pri
# * @author Álvaro Sánchez González <alvarosg@usal.es>
# * @date   Mon Jul  23 2012
# *
# * Copyright (c) 2012 Álvaro Sánchez González
# *
# * @brief  Archivo .pri que contiene la parte relacionada con CUDA para la generación del makefile.
# */

#Añando bibliotecas y etapas de compilación de CUDA

SOURCES-=sources/cudafullimageinterface.cu \
              sources/cudaphaseinterface.cu \
              sources/cudageneralpurpose.cu \
              sources/cudasafe.cu
CUDA_SOURCES+=sources/cudafullimageinterface.cu \
              sources/cudaphaseinterface.cu \
              sources/cudageneralpurpose.cu \
              sources/cudasafe.cu

CUDA_DIR = $$quote(C:/Program Files (x86)/NVIDIA GPU Computing Toolkit/CUDA/v4.2)
CUDA_BIN_DIR=$$CUDA_DIR/bin
CUDA_LIB_DIR=$$CUDA_DIR/lib/Win32
QMAKE_LIBDIR = $$CUDA_LIB_DIR

OBJECTS_DIR=Obj

INCLUDEPATH += $$CUDA_DIR/include
INCLUDEPATH += ..\\JetProcessing\\headers

LIBS += -lcudart -lcuFFT -lcuda

cuda.input = CUDA_SOURCES

QMAKE_CXXFLAGS -=-Zc:wchar_t-
QMAKE_CXXFLAGS +=-Zc:wchar_t
cuda.output = $$OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.obj

QMAKE_CUFLAGS += $$QMAKE_CXXFLAGS
DebugBuild:QMAKE_CUFLAGS += $$QMAKE_CXXFLAGS_DEBUG
ReleaseBuild:QMAKE_CUFLAGS += $$QMAKE_CXXFLAGS_RELEASE
QMAKE_CUFLAGS += $$QMAKE_CXXFLAGS_RTTI_ON $$QMAKE_CXXFLAGS_WARN_ON $$QMAKE_CXXFLAGS_STL_ON

cuda.name = Cuda ${QMAKE_FILE_IN}
cuda.commands = \"$$CUDA_BIN_DIR/nvcc.exe\" -c -Xcompiler $$join(QMAKE_CUFLAGS,",") $$join(INCLUDEPATH,'" -I "','-I "','"') ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}

cuda.dependcy_type = TYPE_C
cuda.depend_command = nvcc -M -Xcompiler $$join(QMAKE_CXXFLAGS,",") $$join(INCLUDEPATH,'" -I "','-I "','"') ${QMAKE_FILE_NAME} | sed "s,^.*: ,," | sed "s,^ *,," | tr -d '\\n'

QMAKE_EXTRA_COMPILERS += cuda
