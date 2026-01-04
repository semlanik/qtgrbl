/*
 * MIT License
 *
 * Copyright (c) 2021 Alexey Edelev <semlanik@gmail.com>
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

#include "grblstatus.h"

#include "qtgrblcommon.h"

#include <QDebug>
#include <QByteArrayList>
#include <QMetaEnum>

Q_STATIC_LOGGING_CATEGORY(StatusParsing, "qtgrbl.status.parsing")
#define dumpParsing qCDebug(StatusParsing)

namespace QtGrbl {

qreal GrblCoordinates::x() const
{
    return m_coord[GrblCoordinates::X];
}

qreal GrblCoordinates::y() const
{
    return m_coord[GrblCoordinates::Y];
}

qreal GrblCoordinates::z() const
{
    return m_coord[GrblCoordinates::Z];
}

bool GrblCoordinates::update(QByteArrayView data)
{
    dumpParsing << "[" << this << "] GrblCoordinates::update" << data;
    bool changed = false;
    size_t coordIdx = 0;
    for (qsizetype end = data.indexOf(','), start = 0; start < data.size(); end = data.indexOf(',', start)) {
        if (end < 0)
            end = data.size();
        const QByteArrayView dataPart(data.cbegin() + start, data.cbegin() + end);
        bool ok = false;
        const auto newValue = dataPart.toDouble(&ok);
        if (!ok)
            qCritical() << "Unable to convert " << dataPart << " to coordinate";
        if (m_coord[coordIdx] != newValue) {
            changed = true;
            m_coord[coordIdx] = newValue;
        }
        coordIdx++;
        if (coordIdx >= sizeof(m_coord)) {
            qCritical() << "Malformed coordinates " << dataPart;
            break;
        }
        start = end + 1;
    }

    qCDebug(StatusParsing) << "[" << this << "] GrblCoordinates::update updated coordinates " << *this;
    return changed;
}

QDebug operator<<(QDebug debug, const GrblCoordinates &coord)
{
    const QDebugStateSaver save(debug);
    debug.nospace().noquote();
    debug << "GrblCoordinates(x: " << coord.m_coord[GrblCoordinates::X] << ", y: " << coord.m_coord[GrblCoordinates::Y] << ", z: " << coord.m_coord[GrblCoordinates::Z] << ')';
    return debug;
}


bool GrblStatus::parseData()
{
    QByteArrayView data(m_raw.cbegin() + GrblStatusPrefix.size(), m_raw.cend() - GrblStatusPostfix.size());
    dumpParsing << "GrblStatus::parseData" << data;

    bool ok = true;
    for (qsizetype end = data.indexOf('|'), start = 0; start < data.size(); end = data.indexOf('|', start)) {
        if (end < 0)
            end = data.size();
        const QByteArrayView dataPart(data.cbegin() + start, data.cbegin() + end);
        if (start == 0)
            ok &= parseState(dataPart);
        else
            ok &= parseStatusField(dataPart);
        start = end + 1;
    }
    return ok;
}

bool GrblStatus::parseState(QByteArrayView data)
{
    dumpParsing << "GrblStatus::parseState" << data;
    if (const auto colonIndex = data.indexOf(':'); colonIndex >= 0)
        data = QByteArrayView(data.cbegin(), data.cbegin() + colonIndex);

    const auto stateEnum = QMetaEnum::fromType<GrblState>();
    const auto enumKey = data.toByteArray();
    bool ok = false;
    int value = stateEnum.keyToValue(enumKey.data(), &ok);
    if (!ok) {
        qWarning() << "Invalid grbl state: " << data;
        setGrblState(Invalid);
        return false;
    }
    setGrblState(static_cast<GrblStatus::GrblState>(value));

    return true;
}

bool GrblStatus::parseStatusField(QByteArrayView data)
{
    dumpParsing << "GrblStatus::parseState" << data;

    auto colonIndex = data.indexOf(':');
    if (colonIndex < 1) {
        qCritical() << "Invalid grbl status data: " << data;
        return false;
    }

    const QByteArrayView key(data.cbegin(), data.cbegin() + colonIndex);
    const QByteArrayView value(data.cbegin() + colonIndex + 1, data.cend());
    dumpParsing << "GrblStatus::parseState key:" << key << "value:" << value;

    if (key == "MPos") {
        m_lastUpdateMPos = true;
        if (m_mPos.update(value)) {
            m_wPos = m_mPos - m_wCO;
            emit posChanged();
        }
    } else if (key == "WPos") {
        m_lastUpdateMPos = false;
        if (m_wPos.update(value)) {
            m_mPos = m_wPos + m_wCO;
            emit posChanged();
        }
    } else if (key == "WCO") {
        if (m_wCO.update(value)) {
            if (m_lastUpdateMPos)
                m_wPos = m_mPos - m_wCO;
            else
                m_mPos = m_wPos + m_wCO;
            emit posChanged();
        }
    } else if (key == "FS") {
        setFeedSpeed(QString::fromLatin1(value).toDouble());
    } else {
        qWarning() << "Unhandled status field: " << key;
    }

    return true;
}

QString GrblStatus::grblStateString() const
{
    return enumToString(m_grblState, isValid());
}

void GrblStatus::setFeedSpeed(qreal newFeedSpeed)
{
    if (qFuzzyCompare(m_feedSpeed, newFeedSpeed))
        return;
    m_feedSpeed = newFeedSpeed;
    emit feedSpeedChanged();
}

}
