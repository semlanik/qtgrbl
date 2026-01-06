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

#pragma once

#include <QObject>
#include <QtQmlIntegration>

class DigitalNumberDisplayImpl : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged FINAL)
    Q_PROPERTY(int before READ before WRITE setBefore NOTIFY beforeChanged FINAL)
    Q_PROPERTY(int after READ after WRITE setAfter NOTIFY afterChanged FINAL)

    Q_PROPERTY(int decimalValue READ decimalValue NOTIFY decimalValueChanged FINAL)
    Q_PROPERTY(int integerValue READ integerValue NOTIFY integerValueChanged FINAL)

    Q_PROPERTY(int calculatedBefore READ calculatedBefore NOTIFY calculatedBeforeChanged FINAL)
    Q_PROPERTY(int calculatedAfter READ calculatedAfter NOTIFY calculatedAfterChanged FINAL)

    Q_PROPERTY(bool fits READ fits NOTIFY fitsChanged FINAL)
public:
    DigitalNumberDisplayImpl();
    qreal value() const { return m_value.value_or(0); }
    int before() const { return m_before; }
    int after() const { return m_after; }
    int decimalValue() const { return m_decimalValue; }
    int integerValue() const { return m_integerValue; }
    int calculatedBefore() const { return m_calculatedBefore; }
    int calculatedAfter() const { return m_calculatedAfter; }
    bool fits() const { return m_fits; }

    void setValue(qreal newValue);
    void setBefore(int newBefore);
    void setAfter(int newAfter);
    void setDecimalValue(int newDecimalValue);
    void setIntegerValue(int newIntegerValue);
    void setCalculatedBefore(int newCalculatedBefore);
    void setCalculatedAfter(int newCalculatedAfter);
    void setFits(bool newFits);

    void recalculate();

    friend QDebug operator<<(QDebug debug, const DigitalNumberDisplayImpl &data);
signals:
    void valueChanged();
    void beforeChanged();
    void afterChanged();
    void decimalValueChanged();
    void integerValueChanged();
    void calculatedBeforeChanged();
    void calculatedAfterChanged();
    void fitsChanged();

private:
    std::optional<qreal> m_value;
    int m_before = 3;
    int m_after = 3;
    int m_decimalValue = 0;
    int m_integerValue = 0;
    int m_calculatedBefore = -1;
    int m_calculatedAfter = -1;
    bool m_fits = false;
};
