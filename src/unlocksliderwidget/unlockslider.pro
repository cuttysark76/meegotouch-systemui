TEMPLATE = app
TARGET = unlockslidertest
CONFIG += meegotouch silent

SOURCES += main.cpp \
    unlockslider.cpp \
    unlockslidermodel.cpp \
    unlocksliderview.cpp
HEADERS += unlockslider.h \
    unlocksliderstyle.h \
    unlockslidermodel.h \
    unlocksliderview.h

MODEL_HEADERS += unlockslidermodel.h
STYLE_HEADERS += unlocksliderstyle.h
