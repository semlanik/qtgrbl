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

namespace QtGrbl {

GrblCoordinates &GrblCoordinates::operator+=(const GrblCoordinates &other)
{
    m_x += other.m_x;
    m_y += other.m_y;
    m_z += other.m_z;
    return *this;
}

GrblCoordinates &GrblCoordinates::operator-=(const GrblCoordinates &other)
{
    m_x -= other.m_x;
    m_y -= other.m_y;
    m_z -= other.m_z;
    return *this;
}

qreal GrblCoordinates::x() const
{
    return m_x;
}

qreal GrblCoordinates::y() const
{
    return m_y;
}

qreal GrblCoordinates::z() const
{
    return m_z;
}

bool GrblCoordinates::update(QByteArray value)
{
    auto coords = QString::fromLatin1(value).split(',');
    bool changed = false;
    const auto updateCoordinate = [&](qreal &member, size_t pos) {
        if (coords.size() <= pos)
            return;

        if (const auto &item = coords.at(pos); !item.isEmpty()) {
            if (const qreal coord = item.toDouble(); coord != member) {
                changed = true;
                member = coord;
            }
        }
    };

    updateCoordinate(m_x, 0);
    updateCoordinate(m_y, 1);
    updateCoordinate(m_z, 2);
    return changed;
}

bool GrblStatus::parseData()
{
    QByteArray statusData = m_raw.mid(GrblStatusPrefix.size(), m_raw.size() - (GrblStatusPrefix.size() + 1));
    QByteArrayList statusDataList = statusData.split('|');
    parseState(statusDataList.takeFirst());
    for (const auto &data : statusDataList)
        parseStatusField(data);
    return true;
}

void GrblStatus::parseState(QByteArray stateData)
{
    qDebug() << "GrblStatus::parseState(" << stateData << ")";
    stateData = stateData.split(':').first();
    int index = metaObject()->indexOfEnumerator("GrblState");
    Q_ASSERT_X(index >= 0 && index < metaObject()->enumeratorCount(),
               "GrblStatus", "GrblState is not found in meta object");

    bool ok = false;
    QMetaEnum enumerator = metaObject()->enumerator(index);
    int value = enumerator.keyToValue(stateData.data(), &ok);

    if (!ok) {
        qWarning() << "Invalid grbl state: " << stateData;
        setGrblState(Invalid);
        return;
    }
    setGrblState(static_cast<GrblStatus::GrblState>(value));
}

void GrblStatus::parseStatusField(const QByteArray &data)
{
    auto dataList = data.split(':');
    if (dataList.size() != 2) {
        qCritical() << "Invalid grbl status data: " << dataList;
        return;
    }

    auto key = dataList.at(0);
    auto value = dataList.at(1);

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
}

GrblCoordinates GrblStatus::mPos()
{
    return m_mPos;
}

GrblCoordinates GrblStatus::wPos()
{
    return m_wPos;
}

QString GrblStatus::grblStateString() const
{
    return enumToString(m_grblState, isValid());
}

qreal GrblStatus::feedSpeed() const
{
    return m_feedSpeed;
}

void GrblStatus::setFeedSpeed(qreal newFeedSpeed)
{
    if (qFuzzyCompare(m_feedSpeed, newFeedSpeed))
        return;
    m_feedSpeed = newFeedSpeed;
    emit feedSpeedChanged();
}

}
