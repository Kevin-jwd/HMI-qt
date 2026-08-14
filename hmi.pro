QT       += core gui widgets
CONFIG   += c++17

TARGET    = hmi
TEMPLATE  = app

# ---------------- OpenCV ----------------
# MSYS2 UCRT64 에 설치한 경우 기준.
#   pacman -S mingw-w64-ucrt-x86_64-opencv
# 다른 위치에 설치했다면 OPENCV_ROOT 만 바꾸면 된다.
win32 {
    OPENCV_ROOT = C:/msys64/ucrt64

    INCLUDEPATH += $$OPENCV_ROOT/include/opencv4
    LIBS += -L$$OPENCV_ROOT/lib \
            -lopencv_core \
            -lopencv_imgproc \
            -lopencv_videoio
}
unix {
    CONFIG    += link_pkgconfig
    PKGCONFIG += opencv4
}
# ----------------------------------------

SOURCES  += main.cpp \
            mainwindow.cpp \
            capturethread.cpp \
            displaythread.cpp

HEADERS  += mainwindow.h \
            capturethread.h \
            displaythread.h \
            framesource.h

FORMS    += mainwindow.ui