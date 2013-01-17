QT       += core gui

TARGET = kcalc
TEMPLATE = app


SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/cCalcore.cpp

HEADERS  += src/mainwindow.h \
            src/cCalcore.h

FORMS    += src/mainwindow.ui

CONFIG(release, debug|release):DESTDIR = build/release
CONFIG(debug, debug|release):DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.rcc
UI_DIR = $$DESTDIR/.ui
