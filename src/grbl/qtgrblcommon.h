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

#pragma once

#include <string_view>
#include <QString>
#include <QMetaEnum>

namespace QtGrbl {
    enum CommandPriority {
        Back,     //! Push to end of queue(default)
        Front,    //! Push to front of queue
        Realtime //! Realtime send command without avaiting of previous acknowledge
    };

    constexpr unsigned int GrblMaxCommandLineSize = 128 - 2; //Allow sending realtime commands anytime

    constexpr std::string_view GCodeStatePrefix = "[GC:";
    constexpr std::string_view GCodeStatePostfix = "]";

    constexpr std::string_view GrblStatusPrefix = "<";
    constexpr std::string_view GrblStatusPostfix = ">\r\n";

    template<typename T>
    QString enumToString(T value, bool isValid = true)
    {
        return isValid ? QMetaEnum::fromType<T>().valueToKey(quint64(value)) : "-";
    }
}
