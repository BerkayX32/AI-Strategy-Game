cmake_minimum_required (VERSION 3.15)

project ("MinimaxGame" VERSION 0.1 LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(FORMS_DIR "${CMAKE_SOURCE_DIR}/forms")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

include_directories(${FORMS_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})


set(PROJECT_SOURCES
        "${CMAKE_SOURCE_DIR}/main.cpp" 
        "${CMAKE_SOURCE_DIR}/forms/mainwindow.ui" 
        "${CMAKE_SOURCE_DIR}/forms/board.ui" 
        "${CMAKE_SOURCE_DIR}/include/mainwindow.h" 
        "${CMAKE_SOURCE_DIR}/include/graphicsscene.h" 
        "${CMAKE_SOURCE_DIR}/include/board.h" 
        "${CMAKE_SOURCE_DIR}/include/tilewidget.h" 
        "${CMAKE_SOURCE_DIR}/src/mainwindow.cpp" 
        "${CMAKE_SOURCE_DIR}/src/graphicsscene.cpp"
        "${CMAKE_SOURCE_DIR}/src/board.cpp" 
        "${CMAKE_SOURCE_DIR}/src/tilewidget.cpp"  
        "${CMAKE_SOURCE_DIR}/include/minimaxgame.h" 
        "${CMAKE_SOURCE_DIR}/include/virtualboard.h" 
        "${CMAKE_SOURCE_DIR}/src/virtualboard.cpp" 
        "${CMAKE_SOURCE_DIR}/include/virtualtile.h" 
        "${CMAKE_SOURCE_DIR}/src/virtualtile.cpp" "include/ai.h" "src/ai.cpp")


set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} "${CMAKE_SOURCE_DIR}/forms")



qt_add_executable(MinimaxGame
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)


target_include_directories(MinimaxGame PRIVATE ${FORMS_DIR})
target_include_directories(MinimaxGame PRIVATE ${INCLUDE_DIR})
target_include_directories(MinimaxGame PRIVATE ${SOURCE_DIR})

target_link_libraries(MinimaxGame PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)



if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MinimaxGame)
endif()

