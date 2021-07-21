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
    elzip.cpp \
    errorwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    miniz.c \
    minizip/src/adler32.c \
    minizip/src/compress.c \
    minizip/src/crc32.c \
    minizip/src/deflate.c \
    minizip/src/gzclose.c \
    minizip/src/gzlib.c \
    minizip/src/gzread.c \
    minizip/src/gzwrite.c \
    minizip/src/infback.c \
    minizip/src/inffast.c \
    minizip/src/inflate.c \
    minizip/src/inftrees.c \
    minizip/src/ioapi.c \
    minizip/src/iowin32.c \
    minizip/src/miniunz.c \
    minizip/src/minizip.c \
    minizip/src/mztools.c \
    minizip/src/trees.c \
    minizip/src/uncompr.c \
    minizip/src/unzip.c \
    minizip/src/zip.c \
    minizip/src/zutil.c \
    shapeeditor.cpp \
    unzipper.cpp \
    utility_functions.cpp \
    zipper.cpp \
    #zlib/adler32.c \
    #zlib/compress.c \
    #zlib/crc32.c \
    #zlib/deflate.c \
    #zlib/gzclose.c \
    #zlib/gzlib.c \
    #zlib/gzread.c \
    #zlib/gzwrite.c \
    #zlib/infback.c \
    #zlib/inffast.c \
    #zlib/inflate.c \
    #zlib/inftrees.c \
    #zlib/trees.c \
    #zlib/uncompr.c \
    #zlib/zutil.c

HEADERS += \
    elzip.hpp \
    errorwindow.h \
    fswrapper.hpp \
    mainwindow.h \
    miniz.h \
    minizip/include/minizip/crc32.h \
    minizip/include/minizip/crypt.h \
    minizip/include/minizip/deflate.h \
    minizip/include/minizip/gzguts.h \
    minizip/include/minizip/inffast.h \
    minizip/include/minizip/inffixed.h \
    minizip/include/minizip/inflate.h \
    minizip/include/minizip/inftrees.h \
    minizip/include/minizip/ioapi.h \
    minizip/include/minizip/iowin32.h \
    minizip/include/minizip/mztools.h \
    minizip/include/minizip/trees.h \
    minizip/include/minizip/unzip.h \
    minizip/include/minizip/zconf.h \
    minizip/include/minizip/zip.h \
    minizip/include/minizip/zlib.h \
    minizip/include/minizip/zutil.h \
    shapeeditor.h \
    tinydir.h \
    unzipper.hpp \
    utility_functions.h \
    zipper.hpp \
    #zlib/crc32.h \
    #zlib/deflate.h \
    #zlib/gzguts.h \
    #zlib/inffast.h \
    #zlib/inffixed.h \
    #zlib/inflate.h \
    #zlib/inftrees.h \
    #zlib/trees.h \
    #zlib/win32/libz.rc \
    #zlib/zconf.h \
    #zlib/zlib.h \
    #zlib/zutil.h

FORMS += \
    errorwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
