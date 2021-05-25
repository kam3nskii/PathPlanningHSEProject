TARGET = search
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++17 -O2 -Wall -Wextra

win32 {
QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++
}

SOURCES += \
    Astar.cpp \
    cell.cpp \
    commonfuncs.cpp \
    config.cpp \
    DstarLite.cpp \
    LPAstar.cpp \
    main.cpp \
    map.cpp \
    mission.cpp \
    node.cpp \
    tinyxml2.cpp \
    xmllogger.cpp

HEADERS += \
    Astar.h \
    cell.h \
    commonfuncs.h \
    config.h \
    DstarLite.h \
    environmentoptions.h \
    gl_const.h \
    ilogger.h \
    LPAstar.h \
    map.h \
    mission.h \
    node.h \
    searchresult.h \
    tinyxml2.h \
    xmllogger.h
