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
#include "grblerrorcodemapper.h"

namespace {
struct ErrorCodeMapping {
    std::string_view errorString;
    std::string_view details;
};

// TODO: TRNOOP them
std::array ErrorCodeMappings = {
    ErrorCodeMapping{"Expected command letter", "G-code words consist of a letter and a value. Letter was not found."},
    ErrorCodeMapping{"Bad number format","Missing the expected G-code word value or numeric value format is not valid."},
    ErrorCodeMapping{"Invalid statement","Grbl '$' system command was not recognized or supported."},
    ErrorCodeMapping{"Value < 0","Negative value received for an expected positive value."},
    ErrorCodeMapping{"Setting disabled","Homing cycle failure. Homing is not enabled via settings."},
    ErrorCodeMapping{"Value < 3 usec","Minimum step pulse time must be greater than 3usec."},
    ErrorCodeMapping{"EEPROM read fail. Using defaults","An EEPROM read failed. Auto-restoring affected EEPROM to default values."},
    ErrorCodeMapping{"Not idle","Grbl '$' command cannot be used unless Grbl is IDLE. Ensures smooth operation during a job."},
    ErrorCodeMapping{"G-code lock","G-code commands are locked out during alarm or jog state."},
    ErrorCodeMapping{"Homing not enabled","Soft limits cannot be enabled without homing also enabled."},
    ErrorCodeMapping{"Line overflow","Max characters per line exceeded. Received command line was not executed."},
    ErrorCodeMapping{"Step rate > 30kHz","Grbl '$' setting value cause the step rate to exceed the maximum supported."},
    ErrorCodeMapping{"Check Door","Safety door detected as opened and door state initiated."},
    ErrorCodeMapping{"Line length exceeded","Build info or startup line exceeded EEPROM line length limit. Line not stored."},
    ErrorCodeMapping{"Travel exceeded","Jog target exceeds machine travel. Jog command has been ignored."},
    ErrorCodeMapping{"Invalid jog command","Jog command has no '=' or contains prohibited g-code."},
    ErrorCodeMapping{"Setting disabled","Laser mode requires PWM output."},
    ErrorCodeMapping{"Unsupported command","Unsupported or invalid g-code command found in block."},
    ErrorCodeMapping{"Modal group violation","More than one g-code command from same modal group found in block."},
    ErrorCodeMapping{"Undefined feed rate","Feed rate has not yet been set or is undefined."},
    ErrorCodeMapping{"Invalid gcode ID:23","G-code command in block requires an integer value."},
    ErrorCodeMapping{"Invalid gcode ID:24","More than one g-code command that requires axis words found in block."},
    ErrorCodeMapping{"Invalid gcode ID:25","Repeated g-code word found in block."},
    ErrorCodeMapping{"Invalid gcode ID:26","No axis words found in block for g-code command or current modal state which requires them."},
    ErrorCodeMapping{"Invalid gcode ID:27","Line number value is invalid."},
    ErrorCodeMapping{"Invalid gcode ID:28","G-code command is missing a required value word."},
    ErrorCodeMapping{"Invalid gcode ID:29","G59.x work coordinate systems are not supported."},
    ErrorCodeMapping{"Invalid gcode ID:30","G53 only allowed with G0 and G1 motion modes."},
    ErrorCodeMapping{"Invalid gcode ID:31","Axis words found in block when no command or current modal state uses them."},
    ErrorCodeMapping{"Invalid gcode ID:32","G2 and G3 arcs require at least one in-plane axis word."},
    ErrorCodeMapping{"Invalid gcode ID:33","Motion command target is invalid."},
    ErrorCodeMapping{"Invalid gcode ID:34","Arc radius value is invalid."},
    ErrorCodeMapping{"Invalid gcode ID:35","G2 and G3 arcs require at least one in-plane offset word."},
    ErrorCodeMapping{"Invalid gcode ID:36","Unused value words found in block."},
    ErrorCodeMapping{"Invalid gcode ID:37","G43.1 dynamic tool length offset is not assigned to configured tool length axis."},
    ErrorCodeMapping{"Invalid gcode ID:38","Tool number greater than max supported value."},
};

constexpr size_t ErrorCodeMax = ErrorCodeMappings.size();
}
GrblErrorCodeMapper::GrblErrorCodeMapper(QObject *parent) : QObject(parent) {}

QString GrblErrorCodeMapper::getString(int errorCode) const
{
    --errorCode;
    Q_ASSERT_X(errorCode >= 0 && errorCode < ErrorCodeMax, "GrblErrorCodeMapper::getString", "Error code is out of bounds");
    return QString::fromUtf8(ErrorCodeMappings[errorCode].errorString);
}

QString GrblErrorCodeMapper::getDetails(int errorCode) const
{
    --errorCode;
    Q_ASSERT_X(errorCode >= 0 && errorCode < ErrorCodeMax, "GrblErrorCodeMapper::getString", "Error code is out of bounds");
    return QString::fromUtf8(ErrorCodeMappings[errorCode].details);
}
