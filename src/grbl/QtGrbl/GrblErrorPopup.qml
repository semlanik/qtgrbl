/*
 * MIT License
 *
 * Copyright (c) 2025 Alexey Edelev <semlanik@gmail.com>
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

Popup {
    id: root
    anchors.centerIn: parent
    contentWidth: 300
    contentHeight: 200
    modal: true
    focus: true
    onClosed: root.clear()
    contentItem: Item {
        Text {
            id: errorText
            anchors {
                left: parent.left
                right: parent.right
            }
            height: implicitHeight
            wrapMode: Text.WordWrap
            font.bold: true
        }
        Text {
            id: errorDetails
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: parent.right
            }
            height: implicitHeight
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }

        Button {
            anchors{
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
            text: qsTr("Clear and reset")
            onClicked: root.close()
        }
    }
    Connections {
        target: GrblSerial
        function onErrorCodeChanged() {
            if (GrblSerial.errorCode !== 0) {
                errorText.text = "Error(" + GrblSerial.errorCode + "): " + GrblErrorCodeMapper.getString(GrblSerial.errorCode)
                errorDetails.text = GrblErrorCodeMapper.getDetails(GrblSerial.errorCode)
                root.open()
            } else {
                root.close()
            }
        }
    }

    function clear() {
        errorDetails.text = ""
        errorText.text = ""
        GrblSerial.clearError()
    }
}
