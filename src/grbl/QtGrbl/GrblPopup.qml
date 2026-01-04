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

Popup {
    id: root
    property alias headerText: header.text
    property alias detailsText: details.text
    property alias buttonText: confirmButton.text

    signal clicked()

    anchors.centerIn: parent
    contentWidth: 300
    contentHeight: 200
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    onClosed: root.clear()
    contentItem: Item {
        Text {
            id: header
            anchors {
                left: parent.left
                right: parent.right
            }
            height: implicitHeight
            wrapMode: Text.WordWrap
            font.bold: true
        }
        Text {
            id: details
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
            id: confirmButton
            anchors{
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
            onClicked: root.clicked()
        }
    }

    function clear()
    {
        details.text = ""
        header.text = ""
    }
}
