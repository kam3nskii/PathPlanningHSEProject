TARGET = Denis_Kamenskii_ASearch
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++17 -O2 -Wall -Wextra

win32 {
QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++
}

SOURCES += \
    cell.cpp \
    config.cpp \
    main.cpp \
    map.cpp \
    mission.cpp \
    node.cpp \
    Astar.cpp \
    tinyxml2.cpp \
    xmllogger.cpp
     
HEADERS += \
    cell.h \
    config.h \
    environmentoptions.h \
    gl_const.h \
    ilogger.h \
    map.h \
    mission.h \
    node.h \
    Astar.h \
    searchresult.h \
    tinyxml2.h \
    xmllogger.h