/*
 * MIT License
 *
 * Copyright (c) 2022 Alexey Edelev <semlanik@gmail.com>
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

import QtGrbl

Item {
    id: root
    property alias pointSize: valueText.font.pointSize
    property alias decimalPointSize: decimalText.font.pointSize
    property alias color: valueText.color
    property alias value: privateRoot.value
    property alias digitsBefore: privateRoot.before
    property alias digitsAfter: privateRoot.after
    property alias labelText: label.text
    property alias displayLabel: label.visible
    property bool isValid: false

    width: labelText ? parent.width : childrenRect.width
    height: valueText.height + root.pointSize / 2

    DigitalNumberDisplayImpl {
        id: privateRoot
        readonly property int spacingSize : 4
    }

    Text {
        id: label
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font.pointSize: valueText.font.pointSize
        height: implicitHeight
        visible: false
    }

    Text {
        id: valueShadow
        anchors {
            verticalCenter: parent.verticalCenter
            right: valueText.right
        }
        color {
            a: valueText.color.a/10
            r: valueText.color.r
            g: valueText.color.g
            b: valueText.color.b
        }
        font.family: "DSEG14 Classic"
        font.pointSize: valueText.font.pointSize
        height: implicitHeight
        text: " ".repeat(privateRoot.spacingSize) +
              "~".repeat(root.digitsBefore) +
              "."
    }
    Text {
        id: decimalShadow
        anchors {
            verticalCenter: parent.verticalCenter
            right: decimalText.right
        }
        color {
            a: valueText.color.a/10
            r: valueText.color.r
            g: valueText.color.g
            b: valueText.color.b
        }
        font.family: "DSEG14 Classic"
        font.pointSize: decimalText.font.pointSize
        height: implicitHeight
        text: "~".repeat(root.digitsAfter)
    }
    Text {
        id: valueText
        anchors {
            verticalCenter: parent.verticalCenter
            right: decimalText.left
        }
        font.family: "DSEG14 Classic"
        height: implicitHeight
        width: implicitWidth
        text: {
            if (!privateRoot.fits || !root.isValid)
                return "-".repeat(root.digitsBefore) + "."

            var minusIndent = Math.abs(root.value) === root.value ? 1 : 0
            var spacing =
                    " ".repeat((minusIndent + root.digitsBefore - privateRoot.calculatedBefore) * privateRoot.spacingSize)
            return spacing + (minusIndent ? "" : "-") + privateRoot.integerValue + "."
        }
    }
    Text {
        id: decimalText
        anchors {
            bottom: valueText.bottom
            right: parent.right
        }
        font.family: "DSEG14 Classic"
        font.pointSize: valueText.font.pointSize - 2
        height: implicitHeight
        width: implicitWidth
        text: {
            if (!privateRoot.fits || !root.isValid)
                return "-".repeat(root.digitsAfter)

            var spacing =
                    "0".repeat(root.digitsAfter - privateRoot.calculatedAfter)
            return spacing + privateRoot.decimalValue
        }
    }
}
