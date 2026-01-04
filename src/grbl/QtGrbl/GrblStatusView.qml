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

import QtQuick 2.0

//ToDo: SidebarItem should be here
Rectangle {
    id: root
    color: "#dddddd"
    border.width: 1
    width: parent ? parent.width : 0
    height: childrenRect.height

    Column {
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        Text {
            width: root.width
            height: implicitHeight
            font.pointSize: 13
            text: "Status: " + GrblEngine.grblStatus.grblStateString
        }
        Column {
            width: parent.width
            Text {
                width: implicitWidth
                anchors.right: parent.right
                font.pointSize: 13
                height: implicitHeight
                text: "WPos"
            }
            Text {
                width: implicitWidth
                anchors.right: parent.right
                font.pointSize: 8
                height: implicitHeight
                text: "MPos"
            }
            DigitalNumberDisplay {
                pointSize: 14
                value: GrblEngine.grblStatus.wPos.x
                labelText: "X"
                displayLabel: true
                isValid: GrblEngine.grblStatus.isValid
            }
            DigitalNumberDisplay {
                pointSize: 8
                value: GrblEngine.grblStatus.mPos.x
                isValid: GrblEngine.grblStatus.isValid
            }
            DigitalNumberDisplay {
                pointSize: 14
                value: GrblEngine.grblStatus.wPos.y
                labelText: "Y"
                displayLabel: true
                isValid: GrblEngine.grblStatus.isValid
            }
            DigitalNumberDisplay {
                pointSize: 8
                value: GrblEngine.grblStatus.mPos.y
                isValid: GrblEngine.grblStatus.isValid
            }
            DigitalNumberDisplay {
                pointSize: 14
                value: GrblEngine.grblStatus.wPos.z
                labelText: "Z"
                displayLabel: true
                isValid: GrblEngine.grblStatus.isValid
            }
            DigitalNumberDisplay {
                pointSize: 8
                value: GrblEngine.grblStatus.mPos.z
                isValid: GrblEngine.grblStatus.isValid
            }
        }
    }
}
