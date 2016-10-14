# JetProcessing

JetProcessing is a tool designed to process spectral interferometry images as they are being recorded. The main application is to observe density distributions of transparent gas flows, such as supersonic gas jets, in real time.

It is developed in C++ using the Qt SDK, and it implements several CUDA kernels to increase the processing capabilities. It uses an abstract factory approach to deal with CPU vs GPU execution.


Requirements (Multiplatform):
Qt SDK 4.8.1
Microsoft Visual C++ 2008
CUDA SDK 4.2
QwtPlot 6.0.1
QwtPlot3D 0.2.7
API uEye 4.1 
QuaZIP 0.4.4
MEncoder - MxPlayer (for video encoding)
ImageMagick 6.7.8-2

Copyright © 2012 Alvaro Sanchez Gonzalez
