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

Column {
    id: root
    width: 80
    property real upperLimit
    property real lowerLimit
    property alias value: value.text
    property bool isValid: value.acceptableInput
    DigitalNumberDisplay {
        pointSize: 8
        value: root.upperLimit > root.lowerLimit ? root.upperLimit : root.lowerLimit
        isValid: root.upperLimit !== root.lowerLimit
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
    }
    TextField {
        id: value
        width: 80
        validator: DoubleValidator {
            top: root.upperLimit > root.lowerLimit ? root.upperLimit : root.lowerLimit
            bottom: root.upperLimit < root.lowerLimit ? root.upperLimit : root.lowerLimit
            notation: DoubleValidator.StandardNotation
            decimals: 3
            locale: "C"
        }
        color: value.acceptableInput || value.text === "" ? "black" : "red"
        ToolTip.visible: !value.acceptableInput && hovered && value.text !== ""
        ToolTip.text: qsTr("Value out of bounds")
    }
    DigitalNumberDisplay {
        pointSize: 8
        value: root.upperLimit < root.lowerLimit ? root.upperLimit : root.lowerLimit
        isValid: root.lowerLimit !== root.upperLimit
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
    }
}
