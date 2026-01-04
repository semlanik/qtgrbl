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
        Settings/GrblSettingsView.qml
    SOURCES
        grblserial.cpp grblserial.h
        grblgcodestate.cpp grblgcodestate.h
        grblconsolerecord.cpp grblconsolerecord.h
        grblstatus.cpp grblstatus.h
        grblengine.cpp grblengine.h
        grblconsole.cpp grblconsole.h
        grblsettingsmodel.h grblsettingsmodel.cpp
        QML_FILES Settings/SettingIntView.qml
        QML_FILES Settings/SettingUnitView.qml
        QML_FILES Settings/SettingFloatView.qml
        QML_FILES Settings/SettingBoolView.qml
        SOURCES
        QML_FILES Settings/SettingMaskView.qml
        SOURCES grblerrorcodemapper.h grblerrorcodemapper.cpp
)

target_link_libraries(QtGrbl PRIVATE QtGrblCommon Qt::SerialPort)
target_include_directories(QtGrblCommon INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
