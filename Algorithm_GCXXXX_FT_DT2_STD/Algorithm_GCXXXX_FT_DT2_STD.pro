#-------------------------------------------------
#
# Project created by QtCreator 2025-11-26T18:41:30
#
#-------------------------------------------------


DEFINES += APP_VERSION=\\\"1.4.1.24\\\"

# ==========================
# Git 版本信息
# 勿用 system(git)/where git：无 Git 的 Windows 会命中商店占位程序并卡住。
# 仅 exists() 探测常见安装路径；没有 Git 则不注册 PRE_TARGETDEPS，
# 直接使用仓库里已有的 git_version_data.h。
# ==========================
HAS_REAL_GIT = 0
win32 {
    exists("C:/Program Files/Git/cmd/git.exe"): HAS_REAL_GIT = 1
    exists("C:/Program Files/Git/bin/git.exe"): HAS_REAL_GIT = 1
    exists("C:/Program Files (x86)/Git/cmd/git.exe"): HAS_REAL_GIT = 1
    exists("$$(LOCALAPPDATA)/Programs/Git/cmd/git.exe"): HAS_REAL_GIT = 1
    exists("$$(USERPROFILE)/scoop/apps/git/current/cmd/git.exe"): HAS_REAL_GIT = 1
    export(HAS_REAL_GIT)
} else {
    exists(/usr/bin/git): HAS_REAL_GIT = 1
    exists(/usr/local/bin/git): HAS_REAL_GIT = 1
    exists(/opt/homebrew/bin/git): HAS_REAL_GIT = 1
    export(HAS_REAL_GIT)
}

equals(HAS_REAL_GIT, 1) {
    win32 {
        UPDATE_GIT_VERSION = $$PWD/update_git_version.bat
        git_version_hdr.commands = cmd /c \"$$UPDATE_GIT_VERSION\"
    } else {
        UPDATE_GIT_VERSION = $$PWD/update_git_version.sh
        git_version_hdr.commands = sh $$UPDATE_GIT_VERSION
    }
    git_version_hdr.target = git_version_update
    QMAKE_EXTRA_TARGETS += git_version_hdr
    PRE_TARGETDEPS += git_version_update
    export(QMAKE_EXTRA_TARGETS)
    export(PRE_TARGETDEPS)
    !build_pass: message("Git version: git_version_data.h (updated before link)")
} else {
    !build_pass: message("Git not detected, skip version refresh, use existing git_version_data.h")
}



QT       -= gui

QT     += core xml

TARGET = FTAlgorithm
TEMPLATE = lib

CONFIG += plugin
CONFIG += shared
CONFIG += c++11

QMAKE_CXXFLAGS+=-std=c++0x

DEFINES += FTALGORITHM_LIBRARY

QMAKE_LFLAGS += -static-libgcc -static-libstdc++


INCLUDEPATH += D:/Qt2/software/boost_1_70_0 \
               D:/Qt2/opencv-2.4.13.6-vc14/opencv\build/include \
               D:/Qt2/opencv-2.4.13.6-vc14/opencv\build/include\opencv




LIBS += -lpthread
LIBS += D:/Qt2/software/boost/lib/libboost_date_time-mgw48-mt-x64-1_70.a
LIBS += D:/Qt2/opencv-2.4.13.6-vc14/opencv/build/x64/mingw/lib/libopencv_core2413.dll.a
LIBS += D:/Qt2/opencv-2.4.13.6-vc14/opencv/build/x64/mingw/lib/libopencv_highgui2413.dll.a
LIBS += D:/Qt2/opencv-2.4.13.6-vc14/opencv/build/x64/mingw/lib/libopencv_imgproc2413.dll.a



SOURCES += FTAlgorithm.cpp \
    Params/parammanage.cpp \
    Params/gbuf_manage.cpp \
    Params/cpalgorithmraw.cpp \
    Params/frequencyfilter.cpp \
    Params/testitems.cpp \
    Params/multithread.cpp \
    Params/algorithmitems.cpp \
    TestItems/BrightTest.cpp \
    TestItems/ColorbarTest.cpp \
    TestItems/DarkCurrent.cpp \
    TestItems/Darktest.cpp \
    TestItems/FindClusterType.cpp \
    TestItems/LargeGainTest.cpp \
    TestItems/LagTest.cpp \
    TestItems/NoiseTest.cpp \
    TestItems/MapBinarization.cpp \
    TestItems/SearchSensorDD.cpp \
    TestItems/RangeTest.cpp

HEADERS += FTAlgorithm.h \
    git_version_data.h \
    Params/parammanage.h \
    Params/gbuf_manage.h \
    Params/cpalgorithmraw.h \
    Params/frequencyfilter.h \
    Params/testitems.h \
    Params/multithread.h \
    Params/algorithmitems.h \
    TestItems/BrightTest.h \
    CPBaseTestItem.h \
    TestItems/ColorbarTest.h \
    TestItems/DarkCurrent.h \
    TestItems/Darktest.h \
    TestItems/FindClusterType.h \
    TestItems/LargeGainTest.h \
    TestItems/LagTest.h \
    TestItems/NoiseTest.h \
    TestItems/MapBinarization.h \
    TestItems/SearchSensorDD.h \
    TestItems/RangeTest.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

DISTFILES += \
    update.txt


