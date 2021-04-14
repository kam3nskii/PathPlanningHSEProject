
set(HEADERS     cell.h
                config.h
                environmentoptions.h
                gl_const.h
                map.h
                mission.h
                node.h
                Astar.h
                ilogger.h
                searchresult.h
                tinyxml2.h
                LPAstar.h
                xmllogger.h)

set(SOURCES     cell.cpp
                config.cpp
                map.cpp
                mission.cpp
                node.cpp
                Astar.cpp
                tinyxml2.cpp
                LPAstar.cpp
                xmllogger.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} main.cpp)

add_library(PPLIB ${HEADERS} ${SOURCES})

install(TARGETS ${PROJECT_NAME} 
        CONFIGURATIONS Debug
        DESTINATION Bin/Debug)

install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        DESTINATION Bin/Release)
