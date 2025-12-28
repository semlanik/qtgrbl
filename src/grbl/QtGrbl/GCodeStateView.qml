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

//ToDo: SidebarItem should be here
Rectangle {
    color: "#dddddd"
    border.width: 1
    width: parent ? parent.width : 0
    height: childrenRect.height

    Column {
        anchors.left: parent.left
        anchors.right: parent.right

        GCodeStateItem {
            text: "Motion Mode: " + GrblEngine.gcodeState.motionModeString
        }
        GCodeStateItem {
            text: "Plane Select: " + GrblEngine.gcodeState.planeSelectString
        }
        GCodeStateItem {
            text: "Distance Mode: " + GrblEngine.gcodeState.distanceModeString
        }
        GCodeStateItem {
            text: "Feed Rate Mode: " + GrblEngine.gcodeState.feedRateModeString
        }
        GCodeStateItem {
            text: "Units Mode: " + GrblEngine.gcodeState.unitsModeString
        }
        GCodeStateItem {
            text: "Tool Length Offset: " + GrblEngine.gcodeState.toolLengthOffsetString
        }
        GCodeStateItem {
            text: "Program Mode: " + GrblEngine.gcodeState.programModeString
        }
        GCodeStateItem {
            text: "Spindle State: " + GrblEngine.gcodeState.spindleStateString
        }
        GCodeStateItem {
            text: "Coolant State: " + GrblEngine.gcodeState.coolantStateString
        }
    }
}
