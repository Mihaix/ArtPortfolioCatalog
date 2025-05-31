QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Include directories
INCLUDEPATH += $$PWD/include

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/main.cpp \
    src/models/artwork.cpp \
    src/repository/repository.cpp \
    src/repository/csvrepository.cpp \
    src/repository/jsonrepository.cpp \
    src/controller/artcontroller.cpp \
    src/controller/command/addcommand.cpp \
    src/controller/command/removecommand.cpp \
    src/controller/command/updatecommand.cpp \
    src/controller/filters/titlefilter.cpp \
    src/controller/filters/artistfilter.cpp \
    src/controller/filters/compositefilter.cpp \
    src/ui/mainwindow.cpp \
    src/ui/artworkform.cpp \
    src/ui/artworklist.cpp \


HEADERS += \
    include/models/artwork.h \
    include/repository/repository.h \
    include/repository/csvrepository.h \
    include/repository/jsonrepository.h \
    include/controller/artcontroller.h \
    include/controller/command/command.h \
    include/controller/command/addcommand.h \
    include/controller/command/removecommand.h \
    include/controller/command/updatecommand.h \
    include/controller/filters/filter.h \
    include/controller/filters/titlefilter.h \
    include/controller/filters/artistfilter.h \
    include/controller/filters/compositefilter.h \
    include/ui/mainwindow.h \
    include/ui/artworkform.h \
    include/ui/artworklist.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# Add JSON support
QT += core















