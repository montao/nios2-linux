TARGET = pictureflow
TEMPLATE = app
linux-bfin-flat-* {
	QMAKE_LIBS_THREAD += -lz -lpng -lqjpeg -ljpeg
	QTPLUGIN += qjpeg
	CONFIG += static
}
HEADERS = pictureflow.h
SOURCES = pictureflow.cpp main.cpp
