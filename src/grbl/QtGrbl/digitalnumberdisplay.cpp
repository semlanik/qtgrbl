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

#include "digitalnumberdisplay.h"

#include <cmath>
#include <algorithm>
DigitalNumberDisplayImpl::DigitalNumberDisplayImpl()
{
}

void DigitalNumberDisplayImpl::setValue(qreal newValue)
{
    if (m_value && qFuzzyCompare(*m_value, newValue))
        return;
    m_value = newValue;
    emit valueChanged();

    recalculate();
}

void DigitalNumberDisplayImpl::setBefore(int newBefore)
{
    if (m_before == newBefore)
        return;
    m_before = newBefore;
    emit beforeChanged();

    recalculate();
}

void DigitalNumberDisplayImpl::setAfter(int newAfter)
{
    if (m_after == newAfter)
        return;
    m_after = newAfter;
    emit afterChanged();

    recalculate();
}

void DigitalNumberDisplayImpl::setDecimalValue(int newDecimalValue)
{
    if (m_decimalValue == newDecimalValue)
        return;
    m_decimalValue = newDecimalValue;
    emit decimalValueChanged();
}

void DigitalNumberDisplayImpl::setIntegerValue(int newIntegerValue)
{
    if (m_integerValue == newIntegerValue)
        return;
    m_integerValue = newIntegerValue;
    emit integerValueChanged();
}

void DigitalNumberDisplayImpl::setCalculatedBefore(int newCalculatedBefore)
{
    if (m_calculatedBefore == newCalculatedBefore)
        return;
    m_calculatedBefore = newCalculatedBefore;
    emit calculatedBeforeChanged();
}

void DigitalNumberDisplayImpl::setCalculatedAfter(int newCalculatedAfter)
{
    if (m_calculatedAfter == newCalculatedAfter)
        return;
    m_calculatedAfter = newCalculatedAfter;
    emit calculatedAfterChanged();
}

void DigitalNumberDisplayImpl::setFits(bool newFits)
{
    if (m_fits == newFits)
        return;
    m_fits = newFits;
    emit fitsChanged();
}

void DigitalNumberDisplayImpl::recalculate()
{
    if (!m_value) {
        setCalculatedBefore(-1);
        setCalculatedAfter(-1);
        setFits(false);
        return;
    }

    const auto absValue = std::abs(*m_value);
    setIntegerValue(std::abs(int(*m_value)));
    setDecimalValue(std::round((absValue - qreal(m_integerValue)) * std::pow(10, m_after)));
    setCalculatedBefore(std::max(0, int(std::log10(m_integerValue))) + 1);
    setCalculatedAfter(std::max(0, int(std::log10(m_decimalValue))) + 1);
    setFits(m_calculatedBefore <= m_before && m_calculatedAfter <= m_after);

    qDebug() << *this;
}

QDebug operator<<(QDebug debug, const DigitalNumberDisplayImpl &data)
{
    const QDebugStateSaver save(debug);
    debug.nospace().noquote();
    debug << "DigitalNumberDisplayImpl(value: " << data.m_value << ", before: " << data.m_before
          << ", after: " << data.m_after << ", decimalValue: " << data.m_decimalValue
          << ", integerValue: " << data.m_integerValue << ", calculatedBefore: "
          << data.m_calculatedBefore << ", calculatedAfter: " << data.m_calculatedAfter
          << ", fits: " << data.m_fits << ")";
    return debug;
}
