#	C:\Tony\DXF\dxf.pro
QT -= gui
TARGET = main
TEMPLATE = app
CONFIG += c++20 console

SOURCES += main.cpp \
    Dxf.cpp \
    Parse.cpp \
    Util.cpp

HEADERS += \
    Dxf.hpp \
    Parse.hpp \
    Util.hpp