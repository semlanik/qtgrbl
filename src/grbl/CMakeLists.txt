cmake_minimum_required(VERSION 3.21)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Quick LinguistTools SerialPort Scxml REQUIRED)

qt_standard_project_setup(REQUIRES 6.10)

set(ts_files qtgrbl_en_US.ts)

qt_add_executable(qtgrbl
    main.cpp
    grblremotemessagebuffer.cpp grblremotemessagebuffer.h
    qmlsingletonebase.h
)

qt_add_resources(qtgrbl fonts
    PREFIX "/qtgrbl"
    FILES
        fonts/DSEG14.ttf
)

qt_add_library(QtGrblCommon SHARED
    universallistmodel.cpp universallistmodel.h
    universallistmodelbase.cpp
    grblabstractdatamodel.cpp grblabstractdatamodel.h
    qtgrblcommon.h
)

target_link_libraries(QtGrblCommon PUBLIC Qt::Core)
target_include_directories(QtGrblCommon INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

add_subdirectory(QtGrbl)

target_link_libraries(qtgrbl PRIVATE Qt::Core Qt::Quick Qt::SerialPort PolicyStateMachine QtGrbl QtGrblCommon)

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${ts_files})

