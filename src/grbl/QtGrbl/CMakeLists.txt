cmake_minimum_required(VERSION 3.21)

qt_add_qml_module(QtGrbl
    URI QtGrbl
    VERSION 1.0
    QML_FILES
        GrblStateMachine/GrblStateMachine.qml
        GCodeStateView.qml
        ConsoleOutput.qml
        JogControl/PlainMaterial.qml
        JogControl/GizmoJogPlane.qml
        JogControl/GizmoControl.qml
        JogControl/GizmoArrow.qml
        JogControl/Jog3dControl.qml
        JogControl/PlaneControl.qml
        Main.qml
        GrblStatusView.qml
        SideToolbar.qml
        DigitalNumberDisplay.qml
        GCodeStateItem.qml
        GrblStatusIndicator.qml
        GrblErrorPopup.qml
    SOURCES
        grblserial.cpp grblserial.h
        grblgcodestate.cpp grblgcodestate.h
        grblconsolerecord.cpp grblconsolerecord.h
        grblstatus.cpp grblstatus.h
        grblengine.cpp grblengine.h
        grblconsole.cpp grblconsole.h
        SOURCES grblsettings.h grblsettings.cpp
)

target_link_libraries(QtGrbl PRIVATE QtGrblCommon Qt::SerialPort)
target_include_directories(QtGrblCommon INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
