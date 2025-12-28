cmake_minimum_required(VERSION 3.21)

project(PolicyStateMachine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Quick REQUIRED)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_qml_module(PolicyStateMachine
    URI PolicyStateMachine
    VERSION 1.0
    SOURCES
        statepolicy.cpp statepolicy.h
    QML_FILES
        PolicyState.qml
        PolicyStateMachine.qml
    OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/src/grbl/PolicyStateMachine
)

target_link_libraries(PolicyStateMachine PRIVATE Qt::Quick)
