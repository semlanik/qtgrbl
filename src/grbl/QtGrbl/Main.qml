/*
 * MIT License
 *
 * Copyright (c) 2020 Alexey Edelev <semlanik@gmail.com>
 *
 * This file is part of QtGrbl project https://github.com/semlanik/qtgrbl
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and
 * to permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml.Models

import QtGrbl
import "GrblStateMachine"
import "JogControl"
import "Settings"

import PolicyStateMachine

Window {
    id: root
    width: 640
    height: 480
    title: qsTr("QtGrbl")

    SideToolbar {
        id: leftToolbar
        maxWidth: root.width - rightToolbar.width - toolBar.minWidth
        position: "left"
        content: ObjectModel {
            GCodeStateView {}
            GrblStatusView {}
            Item {
                width: parent ? parent.width : 0
                height: reset0Button.height
                Button {
                    id: reset0Button
                    anchors.left: parent.left
                    width: parent.width / 2
                    objectName: "reset0Button"
                    text: "Reset to 0"
                    StatePolicy.allowed: "idle"
                    onClicked: {
                        GrblEngine.resetToZero();
                    }
                }
                Button {
                    width: parent.width / 2
                    anchors.right: parent.right
                    objectName: "return0Button"
                    text: "Return to 0"
                    StatePolicy.allowed: "idle"
                    onClicked: {
                        GrblEngine.returnToZero();
                    }
                }
            }
        }
    }

    SideToolbar {
        id: rightToolbar
        maxWidth: root.width - leftToolbar.width - toolBar.width
        position: "right"
        content: ObjectModel {
            ConsoleOutput {
                width: parent ? parent.width : 0
            }
            Item {
                height: 500
                width: parent ? parent.width : 0
                Switch { //TODO: replace with multistate button(like radio button)
                    id: controlType
                    text: "Control type"
                    checked: true
                }
//                Jog3dControl {
//                    id: jog
//                    type: controlType.checked
//                    anchors.top: controlType.top
//                    anchors.bottom: parent.bottom
//                    anchors.left: parent.left
//                    anchors.right: parent.right
//                }
            }
        }
    }

    Item {
        id: preview
        property int minWidth: 200
        anchors {
            left: leftToolbar.right
            right: rightToolbar.left
            top: toolBar.bottom
            bottom: consoleInput.top
        }
    }

    FileDialog {
        id: fileSelection
        fileMode: FileDialog.OpenFile
        onAccepted: {
            GrblEngine.filePath = fileSelection.fileUrl;//TODO: not a SerialEngine functionality
        }
    }

    GrblSettingsView {
        id: settings
        anchors.centerIn: parent
        contentHeight: parent.height/2
        contentWidth: 600
    }

    TextField {
        id: consoleInput
        focus: true
        objectName: "consoleInput"
        StatePolicy.allowed: "idle|alarm"
        onActiveFocusChanged: {
            forceActiveFocus()
        }

        width: parent.width/4
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
        }

        Keys.onUpPressed: {
            consoleInput.text = GrblConsole.moveHistoryUp()
        }

        Keys.onDownPressed: {
            consoleInput.text = GrblConsole.moveHistoryDown()
        }

        onTextEdited: {
            GrblConsole.recentInput = text;
        }

        onAccepted: {
            GrblConsole.sendCommand(consoleInput.text)
            consoleInput.text = "";
        }
    }

    ToolBar {
        id: toolBar
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        width: toolBarContent.width + 10
        Row {
            id: toolBarContent
            GrblStatusIndicator {
                anchors.verticalCenter: parent.verticalCenter
                id: grblTransferStatus
            }
            ComboBox {
                id: portSelector
                objectName: "portSelector"
                model: GrblSerial.portList
                StatePolicy.allowed: "disconnected"
                onCurrentIndexChanged: {
                    GrblSerial.selectedPort = portSelector.currentIndex
                }
            }
            ToolButton {
                objectName: "updateButton"
                text: "Update"
                StatePolicy.allowed: "disconnected"
                onClicked: {
                    GrblSerial.updatePortList()
                }
            }
            ToolButton {
                objectName: "connectButton"
                text: stateMachine.currentState === "disconnected" ? "Connect" : "Disconnect"
                onClicked: {
                    stateMachine.toggleConnect()
                }
            }
            ToolButton {
                objectName: "selectFileButton"
                text: "Select file"
                StatePolicy.allowed: "idle"
                onClicked: {
                    fileSelection.open()
                }
            }
            ToolButton {
                objectName: "homeButton"
                text: "Find home"
                StatePolicy.allowed: "idle|alarm"
                onClicked: {
                    GrblEngine.home();
                }
            }
            ToolButton {
                objectName: "startButton"
                text: "Start"
                visible: enabled
                StatePolicy.allowed: "idle"
                onClicked: {
                    GrblEngine.start();
                }
            }
            ToolButton {
                objectName: "stopButton"
                text: "Stop"
                visible: enabled
                StatePolicy.allowed: "running"
                onClicked: {
                    GrblEngine.stop();
                }
            }
            ToolButton {
                objectName: "holdButton"
                text: "Reset Alarm"
                StatePolicy.allowed: "alarm"
                onClicked: {
                    GrblEngine.resetAlarm();
                }
            }
            ToolButton {
                objectName: "holdButton"
                text: "Hold"
                StatePolicy.forbidden: "hold|alarm|disconnected"
                onClicked: {
                    GrblEngine.hold();
                }
            }
            ToolButton {
                objectName: "resumeButton"
                text: "Resume"
                StatePolicy.allowed: "hold"
                onClicked: {
                    GrblEngine.resume();
                }
            }
            ToolButton {
                objectName: "settingsButton"
                icon.source: "qrc:/qtgrbl/res/services-48.png"
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Grbl Settings")
                onClicked: {
                    settings.open()
                }
            }
        }
    }

    GrblErrorPopup {
        id: errorPopup
    }

    GrblStateMachine {
        id: stateMachine
    }

    Component.onCompleted: {
        showMaximized()
    }
}
