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
import QtQuick.Controls

import QtGrbl

import PolicyStateMachine

Column {
    id: root
    Row {
        Button {
            text: qsTr("Clear")
            onClicked: {
                GrblConsole.clear()
            }
        }
        Button {
            text: qsTr("Save to file...")
            onClicked: {
                // TODO: add log saving
            }
        }
        Switch {
            id: followLog
            text: qsTr("Follow logs")
            anchors.verticalCenter: parent.verticalCenter
            onCheckedChanged: {
                viewport.positionViewAtEnd()
            }
        }
    }
    ListView {
        id: viewport
        model: GrblConsole
        height: 500
        width: parent.width
        clip: true
        ScrollBar.vertical: ScrollBar { }
        delegate: Row {
            height: childrenRect.height
            spacing: 5
            Image {
                source: model.type === GrblConsoleRecord.Command ? "qrc:/qtgrbl/res/arrow-left-48.png" : "qrc:/qtgrbl/res/arrow-right-48.png"
                width: 10
                height: 10
            }
            Text {
                text: Qt.formatDateTime(model.timestamp,"yyyy-MM-dd hh:mm:ss.zzz")
                font.pointSize: 8
                width: implicitWidth
                height: implicitHeight
            }
            TextEdit {
                font.pointSize: 9
                text: model.log.trim()
                width: implicitWidth
                height: implicitHeight
                selectByMouse: true
                readOnly: true
            }
        }

        onCountChanged: {
            if (followLog.checked)
                positionViewAtEnd()
        }
    }
}
