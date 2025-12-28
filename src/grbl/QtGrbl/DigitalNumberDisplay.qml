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

Item {
    id: root
    property alias pointSize: text.font.pointSize
    property alias color: text.color
    property real value: 0
    property int digitsBefore: 3
    property int digitsAfter: 3
    property alias labelText: label.text
    property alias displayLabel: label.visible
    property bool isValid: false

    width: parent.width
    height: text.height + root.pointSize / 2

    QtObject {
        id: privateRoot
        readonly property int spacingSize : 4
    }

    Text {
        id: label
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font.pointSize: text.font.pointSize
        height: implicitHeight
        visible: false
    }

    Text {
        id: shadow
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        color {
            a: text.color.a/10
            r: text.color.r
            g: text.color.g
            b: text.color.b
        }
        font.family: "DSEG14 Classic"
        font.pointSize: text.font.pointSize
        height: implicitHeight
        text: " ".repeat(privateRoot.spacingSize) +
              "~".repeat(root.digitsBefore) +
              "." +
              "~".repeat(root.digitsAfter)
    }
    Text {
        id: text
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        font.family: "DSEG14 Classic"
        height: implicitHeight
        text: {
            var absValue = Math.abs(root.value)
            var minusIndent = absValue === root.value ? 1 : 0
            var digitNumber =
                        Math.max(Math.floor(Math.log10(Math.abs(Math.floor(absValue)))), 0) + 1

            if (digitNumber > root.digitsBefore || !root.isValid)
                return "-".repeat(root.digitsBefore) + "." + "-".repeat(root.digitsAfter)

            var spacing =
                    " ".repeat((minusIndent + root.digitsAfter - digitNumber) * privateRoot.spacingSize)
            return spacing + root.value.toFixed(root.digitsAfter)
        }
    }
}
