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

import PolicyStateMachine

Column {
    id: root
    StatePolicy.allowed: "idle"
    spacing: 10
    Row {
        height: childrenRect.height
        spacing: 5
        Button {
            anchors.verticalCenter: parent.verticalCenter
            text: qsTr("Jog to")
            onClicked: {
                GrblEngine.jog.jog(jogX.value, jogY.value, jogZ.value)
            }
            enabled: jogX.isValid && jogY.isValid && jogZ.isValid
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: "x"
        }
        JogTextInput {
            id: jogX
            upperLimit: GrblEngine.jog.xUpperLimit
            lowerLimit: GrblEngine.jog.xLowerLimit
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: "y"
        }
        JogTextInput {
            id: jogY
            upperLimit: GrblEngine.jog.yUpperLimit
            lowerLimit: GrblEngine.jog.yLowerLimit
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: "z"
        }
        JogTextInput {
            id: jogZ
            upperLimit: GrblEngine.jog.zUpperLimit
            lowerLimit: GrblEngine.jog.zLowerLimit
        }
    }
    Item {
        id: buttonJog
        anchors {
            left: parent.left
            right: parent.right
        }
        height: childrenRect.height
        property int defaultMargin: 10
        property int wideMargin: 30
        Button {
            id: jogBack
            icon.source: "qrc:/qtgrbl/res/jog-up-48.png"
            anchors {
                top: parent.top
                topMargin: buttonJog.defaultMargin
                left: jogLeft.right
                leftMargin: buttonJog.defaultMargin
            }
            onPressedChanged: {
                if (pressed)
                    GrblEngine.jog.jog(GrblJogController.Back)
                else
                    GrblEngine.jog.cancel()
            }
        }
        Button {
            id: jogLeft
            icon.source: "qrc:/qtgrbl/res/jog-left-48.png"
            anchors {
                top: jogBack.bottom
                topMargin: buttonJog.defaultMargin
                left: parent.left
                leftMargin: buttonJog.defaultMargin
            }
            onPressedChanged: {
                if (pressed)
                    GrblEngine.jog.jog(GrblJogController.Left)
                else
                    GrblEngine.jog.cancel()
            }
        }
        Button {
            id: jogRight
            icon.source: "qrc:/qtgrbl/res/jog-right-48.png"
            anchors {
                verticalCenter: jogLeft.verticalCenter
                left: jogBack.right
                leftMargin: buttonJog.defaultMargin
            }
            onPressedChanged: {
                if (pressed)
                    GrblEngine.jog.jog(GrblJogController.Right)
                else
                    GrblEngine.jog.cancel()
            }
        }
        Button {
            id: jogFront
            icon.source: "qrc:/qtgrbl/res/jog-down-48.png"
            anchors {
                top: jogLeft.bottom
                topMargin: buttonJog.defaultMargin
                horizontalCenter: jogBack.horizontalCenter
            }
            onPressedChanged: {
                if (pressed)
                    GrblEngine.jog.jog(GrblJogController.Front)
                else
                    GrblEngine.jog.cancel()
            }
        }

        Button {
            id: jogUp
            icon.source: "qrc:/qtgrbl/res/jog-up-48.png"
            anchors {
                verticalCenter: jogBack.verticalCenter
                left: jogRight.right
                leftMargin: buttonJog.wideMargin
            }
            onPressedChanged: {
                if (pressed)
                    GrblEngine.jog.jog(GrblJogController.Up)
                else
                    GrblEngine.jog.cancel()
            }
        }
        Button {
            id: jogDown
            icon.source: "qrc:/qtgrbl/res/jog-down-48.png"
            anchors {
                verticalCenter: jogFront.verticalCenter
                left: jogRight.right
                leftMargin: buttonJog.wideMargin
            }
            onPressedChanged: {
                if (pressed)
                    GrblEngine.jog.jog(GrblJogController.Down)
                else
                    GrblEngine.jog.cancel()
            }
        }

        Image {
            source:  "qrc:/qtgrbl/res/x-y-axis-48.png"
            height: 40
            fillMode: Image.PreserveAspectFit
            anchors {
                horizontalCenter: jogBack.horizontalCenter
                verticalCenter: jogLeft.verticalCenter
            }
        }

        Image {
            source:  "qrc:/qtgrbl/res/z-axis-48.png"
            height: 40
            fillMode: Image.PreserveAspectFit
            anchors {
                horizontalCenter: jogUp.horizontalCenter
                verticalCenter: jogLeft.verticalCenter
            }
        }
    }
}
