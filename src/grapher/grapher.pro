######################################################################
# Automatically generated by qmake (3.1) Sat Mar 16 12:57:38 2019
######################################################################

TEMPLATE = lib
TARGET = ../../build/lib/grapher
INCLUDEPATH += .

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS GRAPHER_LIB

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += grapher.h \
            grapher_types.h \
            grapher_global.h \
            grapher_types/cpu_grapher.h \
            grapher_types/ram_grapher.h \
            grapher_types/hdd_grapher.h

SOURCES += grapher.cpp \
            grapher_types/cpu_grapher.cpp \
            grapher_types/ram_grapher.cpp \
            grapher_types/hdd_grapher.cpp

QT += core gui widgets