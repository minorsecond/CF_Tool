QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
LIBS += -LC:\OSGeo4W64\apps\gdal-dev\lib -lgdal_i
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
    unzipper.cpp \
    utility_functions.cpp \
    zipper.cpp \
    zlib/adler32.c \
    zlib/compress.c \
    zlib/crc32.c \
    zlib/deflate.c \
    zlib/gzclose.c \
    zlib/gzlib.c \
    zlib/gzread.c \
    zlib/gzwrite.c \
    zlib/infback.c \
    zlib/inffast.c \
    zlib/inflate.c \
    zlib/inftrees.c \
    zlib/trees.c \
    zlib/uncompr.c \
    zlib/zutil.c

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
    tinydir.h \
    unzipper.hpp \
    utility_functions.h \
    zipper.hpp \
    zlib/crc32.h \
    zlib/deflate.h \
    zlib/gzguts.h \
    zlib/inffast.h \
    zlib/inffixed.h \
    zlib/inflate.h \
    zlib/inftrees.h \
    zlib/trees.h \
    zlib/win32/libz.rc \
    zlib/zconf.h \
    zlib/zlib.h \
    zlib/zutil.h

FORMS += \
    errorwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    zlib/ChangeLog \
    zlib/README.md \
    zlib/README.zlib \
    zlib/THIS_VERSION_IS_1.2.11 \
    zlib/build-VS2008/libz-static/libz-static.vcproj \
    zlib/build-VS2008/libz/libz.vcproj \
    zlib/build-VS2008/tests/example-static/example-static.vcproj \
    zlib/build-VS2008/tests/example/example.vcproj \
    zlib/build-VS2008/tests/infcover-static/infcover-static.vcproj \
    zlib/build-VS2008/tests/minigzip-static/minigzip-static.vcproj \
    zlib/build-VS2008/tests/minigzip/minigzip.vcproj \
    zlib/build-VS2008/zlib.sln \
    zlib/build-VS2010/libz-static/libz-static.vcxproj \
    zlib/build-VS2010/libz-static/libz-static.vcxproj.filters \
    zlib/build-VS2010/libz/libz.vcxproj \
    zlib/build-VS2010/libz/libz.vcxproj.filters \
    zlib/build-VS2010/tests/example-static/example-static.vcxproj \
    zlib/build-VS2010/tests/example-static/example-static.vcxproj.filters \
    zlib/build-VS2010/tests/example/example.vcxproj \
    zlib/build-VS2010/tests/example/example.vcxproj.filters \
    zlib/build-VS2010/tests/infcover-static/infcover-static.vcxproj \
    zlib/build-VS2010/tests/infcover-static/infcover-static.vcxproj.filters \
    zlib/build-VS2010/tests/minigzip-static/minigzip-static.vcxproj \
    zlib/build-VS2010/tests/minigzip-static/minigzip-static.vcxproj.filters \
    zlib/build-VS2010/tests/minigzip/minigzip.vcxproj \
    zlib/build-VS2010/tests/minigzip/minigzip.vcxproj.filters \
    zlib/build-VS2010/zlib.sln \
    zlib/build-VS2013/libz-static/libz-static.vcxproj \
    zlib/build-VS2013/libz-static/libz-static.vcxproj.filters \
    zlib/build-VS2013/libz/libz.vcxproj \
    zlib/build-VS2013/libz/libz.vcxproj.filters \
    zlib/build-VS2013/tests/example-static/example-static.vcxproj \
    zlib/build-VS2013/tests/example-static/example-static.vcxproj.filters \
    zlib/build-VS2013/tests/example/example.vcxproj \
    zlib/build-VS2013/tests/example/example.vcxproj.filters \
    zlib/build-VS2013/tests/infcover-static/infcover-static.vcxproj \
    zlib/build-VS2013/tests/infcover-static/infcover-static.vcxproj.filters \
    zlib/build-VS2013/tests/minigzip-static/minigzip-static.vcxproj \
    zlib/build-VS2013/tests/minigzip-static/minigzip-static.vcxproj.filters \
    zlib/build-VS2013/tests/minigzip/minigzip.vcxproj \
    zlib/build-VS2013/tests/minigzip/minigzip.vcxproj.filters \
    zlib/build-VS2013/zlib.sln \
    zlib/build-VS2015-MT/libz-static/libz-static.vcxproj \
    zlib/build-VS2015-MT/libz-static/libz-static.vcxproj.filters \
    zlib/build-VS2015-MT/libz/libz.vcxproj \
    zlib/build-VS2015-MT/libz/libz.vcxproj.filters \
    zlib/build-VS2015-MT/tests/example-static/example-static.vcxproj \
    zlib/build-VS2015-MT/tests/example-static/example-static.vcxproj.filters \
    zlib/build-VS2015-MT/tests/example/example.vcxproj \
    zlib/build-VS2015-MT/tests/example/example.vcxproj.filters \
    zlib/build-VS2015-MT/tests/infcover-static/infcover-static.vcxproj \
    zlib/build-VS2015-MT/tests/infcover-static/infcover-static.vcxproj.filters \
    zlib/build-VS2015-MT/tests/minigzip-static/minigzip-static.vcxproj \
    zlib/build-VS2015-MT/tests/minigzip-static/minigzip-static.vcxproj.filters \
    zlib/build-VS2015-MT/tests/minigzip/minigzip.vcxproj \
    zlib/build-VS2015-MT/tests/minigzip/minigzip.vcxproj.filters \
    zlib/build-VS2015-MT/zlib.sln \
    zlib/build-VS2015/libz-static/libz-static.vcxproj \
    zlib/build-VS2015/libz-static/libz-static.vcxproj.filters \
    zlib/build-VS2015/libz/libz.vcxproj \
    zlib/build-VS2015/libz/libz.vcxproj.filters \
    zlib/build-VS2015/tests/example-static/example-static.vcxproj \
    zlib/build-VS2015/tests/example-static/example-static.vcxproj.filters \
    zlib/build-VS2015/tests/example/example.vcxproj \
    zlib/build-VS2015/tests/example/example.vcxproj.filters \
    zlib/build-VS2015/tests/infcover-static/infcover-static.vcxproj \
    zlib/build-VS2015/tests/infcover-static/infcover-static.vcxproj.filters \
    zlib/build-VS2015/tests/minigzip-static/minigzip-static.vcxproj \
    zlib/build-VS2015/tests/minigzip-static/minigzip-static.vcxproj.filters \
    zlib/build-VS2015/tests/minigzip/minigzip.vcxproj \
    zlib/build-VS2015/tests/minigzip/minigzip.vcxproj.filters \
    zlib/build-VS2015/zlib.sln \
    zlib/build-VS2017-MT/libz-static/libz-static.vcxproj \
    zlib/build-VS2017-MT/libz-static/libz-static.vcxproj.filters \
    zlib/build-VS2017-MT/libz/libz.vcxproj \
    zlib/build-VS2017-MT/libz/libz.vcxproj.filters \
    zlib/build-VS2017-MT/tests/example-static/example-static.vcxproj \
    zlib/build-VS2017-MT/tests/example-static/example-static.vcxproj.filters \
    zlib/build-VS2017-MT/tests/example/example.vcxproj \
    zlib/build-VS2017-MT/tests/example/example.vcxproj.filters \
    zlib/build-VS2017-MT/tests/infcover-static/infcover-static.vcxproj \
    zlib/build-VS2017-MT/tests/infcover-static/infcover-static.vcxproj.filters \
    zlib/build-VS2017-MT/tests/minigzip-static/minigzip-static.vcxproj \
    zlib/build-VS2017-MT/tests/minigzip-static/minigzip-static.vcxproj.filters \
    zlib/build-VS2017-MT/tests/minigzip/minigzip.vcxproj \
    zlib/build-VS2017-MT/tests/minigzip/minigzip.vcxproj.filters \
    zlib/build-VS2017-MT/zlib.sln \
    zlib/build-VS2017/libz-static/libz-static.vcxproj \
    zlib/build-VS2017/libz-static/libz-static.vcxproj.filters \
    zlib/build-VS2017/libz/libz.vcxproj \
    zlib/build-VS2017/libz/libz.vcxproj.filters \
    zlib/build-VS2017/tests/example-static/example-static.vcxproj \
    zlib/build-VS2017/tests/example-static/example-static.vcxproj.filters \
    zlib/build-VS2017/tests/example/example.vcxproj \
    zlib/build-VS2017/tests/example/example.vcxproj.filters \
    zlib/build-VS2017/tests/infcover-static/infcover-static.vcxproj \
    zlib/build-VS2017/tests/infcover-static/infcover-static.vcxproj.filters \
    zlib/build-VS2017/tests/minigzip-static/minigzip-static.vcxproj \
    zlib/build-VS2017/tests/minigzip-static/minigzip-static.vcxproj.filters \
    zlib/build-VS2017/tests/minigzip/minigzip.vcxproj \
    zlib/build-VS2017/tests/minigzip/minigzip.vcxproj.filters \
    zlib/build-VS2017/zlib.sln \
    zlib/distfiles/download.url \
    zlib/distfiles/zlib-1.2.11.tar.xz
