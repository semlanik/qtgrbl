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
#include "grblalarmcodemapper.h"

namespace {
struct AlarmCodeMapping {
    std::string_view errorString;
    std::string_view details;
};

// TODO: TRNOOP them
std::array AlarmCodeMappings = {
    AlarmCodeMapping{"Hard limit triggered", "Machine position is likely lost due to sudden and immediate halt. Re-homing is highly recommended."},
    AlarmCodeMapping{"G-code motion target exceeds machine travel","Machine position safely retained. Alarm may be unlocked."},
    AlarmCodeMapping{"Reset while in motion","Grbl cannot guarantee position. Lost steps are likely. Re-homing is highly recommended."},
    AlarmCodeMapping{"Probe fail","The probe is not in the expected initial state before starting probe cycle, where G38.2 and G38.3 is not triggered and G38.4 and G38.5 is triggered."},
    AlarmCodeMapping{"Probe fail","Probe did not contact the workpiece within the programmed travel for G38.2 and G38.4."},
    AlarmCodeMapping{"Homing fail","Reset during active homing cycle."},
    AlarmCodeMapping{"Homing fail","Safety door was opened during active homing cycle."},
    AlarmCodeMapping{"Homing fail","Cycle failed to clear limit switch when pulling off. Try increasing pull-off setting or check wiring."},
    AlarmCodeMapping{"Homing fail","Could not find limit switch within search distance. Defined as `1.5 * max_travel` on search and `5 * pulloff` on locate phases."}
};

constexpr size_t ErrorCodeMax = AlarmCodeMappings.size();
}
GrblAlarmCodeMapper::GrblAlarmCodeMapper(QObject *parent) : QObject(parent) {}

QString GrblAlarmCodeMapper::getString(int errorCode) const
{
    --errorCode;
    Q_ASSERT_X(errorCode >= 0 && errorCode < ErrorCodeMax, "GrblAlarmCodeMapper::getString", "Error code is out of bounds");
    return QString::fromUtf8(AlarmCodeMappings[errorCode].errorString);
}

QString GrblAlarmCodeMapper::getDetails(int errorCode) const
{
    --errorCode;
    Q_ASSERT_X(errorCode >= 0 && errorCode < ErrorCodeMax, "GrblAlarmCodeMapper::getDetails", "Error code is out of bounds");
    return QString::fromUtf8(AlarmCodeMappings[errorCode].details);
}
