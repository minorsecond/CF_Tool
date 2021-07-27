QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets core

CONFIG += c++17
CONFIG += static
QMAKE_CXXFLAGS += -static
QMAKE_LFLAGS_WINDOWS += -static
LIBS += -LC:\msys64\mingw64\lib -llibgdal
INCLUDEPATH += C:\msys64\mingw64\include
DEPENDPATH += C:\msys64\mingw64\include
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    include/elzip.cpp \
    include/miniz.c \
    include/minizip/src/adler32.c \
    include/minizip/src/compress.c \
    include/minizip/src/crc32.c \
    include/minizip/src/deflate.c \
    include/minizip/src/gzclose.c \
    include/minizip/src/gzlib.c \
    include/minizip/src/gzread.c \
    include/minizip/src/gzwrite.c \
    include/minizip/src/infback.c \
    include/minizip/src/inffast.c \
    include/minizip/src/inflate.c \
    include/minizip/src/inftrees.c \
    include/minizip/src/ioapi.c \
    include/minizip/src/iowin32.c \
    include/minizip/src/miniunz.c \
    include/minizip/src/minizip.c \
    include/minizip/src/mztools.c \
    include/minizip/src/trees.c \
    include/minizip/src/uncompr.c \
    include/minizip/src/unzip.c \
    include/minizip/src/zip.c \
    include/minizip/src/zutil.c \
    include/unzipper.cpp \
    include/zipper.cpp \
    src/job.cpp \
    src/main.cpp \
    src/shapeeditor.cpp \
    ui/confirmdialog.cpp \
    ui/errorwindow.cpp \
    ui/mainwindow.cpp \
    src/utility_functions.cpp \

HEADERS += \
    include/elzip.hpp \
    include/fswrapper.hpp \
    include/miniz.h \
    include/minizip/include/minizip/crc32.h \
    include/minizip/include/minizip/crypt.h \
    include/minizip/include/minizip/deflate.h \
    include/minizip/include/minizip/gzguts.h \
    include/minizip/include/minizip/inffast.h \
    include/minizip/include/minizip/inffixed.h \
    include/minizip/include/minizip/inflate.h \
    include/minizip/include/minizip/inftrees.h \
    include/minizip/include/minizip/ioapi.h \
    include/minizip/include/minizip/iowin32.h \
    include/minizip/include/minizip/mztools.h \
    include/minizip/include/minizip/trees.h \
    include/minizip/include/minizip/unzip.h \
    include/minizip/include/minizip/zconf.h \
    include/minizip/include/minizip/zip.h \
    include/minizip/include/minizip/zlib.h \
    include/minizip/include/minizip/zutil.h \
    include/tinydir.h \
    include/unzipper.hpp \
    include/zipper.hpp \
    src/job.h \
    src/shapeeditor.h \
    src/utility_functions.h \
    ui/confirmdialog.h \
    ui/errorwindow.h \
    ui/mainwindow.h \

FORMS += \
    ui/confirmdialog.ui \
    ui/errorwindow.ui \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
