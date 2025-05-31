QT += testlib
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$PWD/../include

# Include source files directly (except main.cpp)
SOURCES += \
    repository_test.cpp \
    controller_test.cpp \
    test_main.cpp \
    $$PWD/../src/models/artwork.cpp \
    $$PWD/../src/repository/repository.cpp \
    $$PWD/../src/repository/csvrepository.cpp \
    $$PWD/../src/repository/jsonrepository.cpp \
    $$PWD/../src/controller/artcontroller.cpp \
    $$PWD/../src/controller/command/addcommand.cpp \
    $$PWD/../src/controller/command/removecommand.cpp \
    $$PWD/../src/controller/command/updatecommand.cpp \
    $$PWD/../src/controller/filters/filter.cpp \
    $$PWD/../src/controller/filters/titlefilter.cpp \
    $$PWD/../src/controller/filters/artistfilter.cpp \
    $$PWD/../src/controller/filters/compositefilter.cpp

# Add only test headers and controller headers with Q_OBJECT macro
HEADERS += \
    repository_test.h \
    controller_test.h \
    $$PWD/../include/controller/artcontroller.h

# Ensure we run the MOC on all headers
CONFIG += moc
