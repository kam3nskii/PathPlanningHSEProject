
set(HEADERS     Astar.h
                cell.h
                commonfuncs.h
                config.h
                DstarLite.h
                environmentoptions.h
                gl_const.h
                ilogger.h
                LPAstar.h
                map.h
                mission.h
                node.h
                searchresult.h
                tinyxml2.h
                xmllogger.h)

set(SOURCES     Astar.cpp
                cell.cpp
                commonfuncs.cpp
                config.cpp
                DstarLite.cpp
                LPAstar.cpp
                map.cpp
                mission.cpp
                node.cpp
                tinyxml2.cpp
                xmllogger.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} main.cpp)

add_library(PPLIB ${HEADERS} ${SOURCES})

install(TARGETS ${PROJECT_NAME} 
        CONFIGURATIONS Debug
        DESTINATION Bin/Debug)

install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        DESTINATION Bin/Release)
