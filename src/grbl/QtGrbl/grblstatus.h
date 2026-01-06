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

#pragma once

#include "grblabstractdatamodel.h"

#include <QObject>
#include <grblconsolerecord.h>
#include <QtQmlIntegration>

namespace QtGrbl {

class GrblCoordinates
{
    Q_GADGET
    QML_VALUE_TYPE(grblCoordinates)
    Q_PROPERTY(qreal x READ x)
    Q_PROPERTY(qreal y READ y)
    Q_PROPERTY(qreal z READ z)

    constexpr static size_t X = 0;
    constexpr static size_t Y = 1;
    constexpr static size_t Z = 2;
public:
    qreal x() const;
    qreal y() const;
    qreal z() const;

    bool update(QByteArrayView data);

    GrblCoordinates &operator+=(const GrblCoordinates &other)
    {
        m_coord[X] += other.m_coord[X];
        m_coord[Y] += other.m_coord[Y];
        m_coord[Z] += other.m_coord[Z];
        return *this;
    }

    GrblCoordinates &operator-=(const GrblCoordinates &other)
    {
        m_coord[X] -= other.m_coord[X];
        m_coord[Y] -= other.m_coord[Y];
        m_coord[Z] -= other.m_coord[Z];
        return *this;
    }

    friend GrblCoordinates operator+(GrblCoordinates lhs, const GrblCoordinates& rhs)
    {
        lhs.m_coord[X] += rhs.m_coord[X];
        lhs.m_coord[Y] += rhs.m_coord[Y];
        lhs.m_coord[Z] += rhs.m_coord[Z];
        return lhs;
    }

    friend GrblCoordinates operator-(GrblCoordinates lhs, const GrblCoordinates& rhs)
    {
        lhs.m_coord[X] -= rhs.m_coord[X];
        lhs.m_coord[Y] -= rhs.m_coord[Y];
        lhs.m_coord[Z] -= rhs.m_coord[Z];
        return lhs;
    }

    friend QDebug operator<<(QDebug debug, const GrblCoordinates &coord);
private:
    qreal m_coord[3]{ 0.0, 0.0, 0.0 };
};

class GrblStatus : public GrblAbstractDataModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(GrblState grblState READ grblState NOTIFY grblStateChanged)
    Q_PROPERTY(QString grblStateString READ grblStateString NOTIFY grblStateChanged)
    Q_PROPERTY(QtGrbl::GrblCoordinates mPos READ mPos NOTIFY posChanged)
    Q_PROPERTY(QtGrbl::GrblCoordinates wPos READ wPos NOTIFY posChanged)
    Q_PROPERTY(qreal feedSpeed READ feedSpeed WRITE setFeedSpeed NOTIFY feedSpeedChanged)
public:
    enum class GrblState : uint8_t {
        Idle,
        Run,
        Hold,
        Jog,
        Alarm,
        Door,
        Check,
        Home,
        Sleep,
        Invalid = 255
    };
    Q_ENUM(GrblState)

    GrblStatus() = default;
    ~GrblStatus() = default;

    GrblState grblState() const
    {
        return m_grblState;
    }

    GrblCoordinates mPos() const { return m_mPos; }
    GrblCoordinates wPos() const { return m_wPos; }
    qreal feedSpeed() const { return m_feedSpeed; }

    QString grblStateString() const;
    void setFeedSpeed(qreal newFeedSpeed);

signals:
    void grblStateChanged();

    void feedSpeedChanged();
    void posChanged();

protected:
    bool parseData() override;

private:
    Q_DISABLE_COPY_MOVE(GrblStatus)
    void setGrblState(GrblState state) {
        if (state != m_grblState) {
            m_grblState = state;
            emit grblStateChanged();
        }
    }

    bool parseState(QByteArrayView state);
    bool parseStatusField(QByteArrayView data);

    GrblState m_grblState = GrblState::Invalid;
    GrblCoordinates m_mPos;
    GrblCoordinates m_wPos;
    GrblCoordinates m_wCO;
    qreal m_feedSpeed = 0.0;
    bool m_lastUpdateMPos = false;
};

}

Q_DECLARE_METATYPE(QtGrbl::GrblStatus::GrblState)
Q_DECLARE_METATYPE(QtGrbl::GrblStatus)
Q_DECLARE_METATYPE(QtGrbl::GrblCoordinates)

