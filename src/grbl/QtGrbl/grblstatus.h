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

public:
    GrblCoordinates &operator+=(const GrblCoordinates &other);
    GrblCoordinates &operator-=(const GrblCoordinates &other);
    qreal x() const;
    qreal y() const;
    qreal z() const;

    bool update(QByteArray data);

    GrblCoordinates &operator+(const GrblCoordinates& rhs)
    {
        m_x += rhs.m_x;
        m_y += rhs.m_y;
        m_z += rhs.m_z;
        return *this;
    }
    GrblCoordinates &operator-(const GrblCoordinates& rhs)
    {
        m_x -= rhs.m_x;
        m_y -= rhs.m_y;
        m_z -= rhs.m_z;
        return *this;
    }

private:
    qreal m_x = 0.0;
    qreal m_y = 0.0;
    qreal m_z = 0.0;
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
    enum GrblState {
        Invalid = -1,
        Idle,
        Run,
        Hold,
        Jog,
        Alarm,
        Door,
        Check,
        Home,
        Sleep
    };
    Q_ENUM(GrblState)

    GrblStatus() = default;
    ~GrblStatus() = default;

    GrblState grblState() const
    {
        return m_grblState;
    }

    GrblCoordinates mPos();
    GrblCoordinates wPos();
    QString grblStateString() const;

    qreal feedSpeed() const;
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

    void parseState(QByteArray state);
    void parseStatusField(const QByteArray &data);

    GrblState m_grblState;
    GrblCoordinates m_mPos;
    GrblCoordinates m_wPos;
    GrblCoordinates m_wCO;
    qreal m_feedSpeed;
    bool m_lastUpdateMPos = false;
};

}

Q_DECLARE_METATYPE(QtGrbl::GrblStatus::GrblState)
Q_DECLARE_METATYPE(QtGrbl::GrblStatus)
Q_DECLARE_METATYPE(QtGrbl::GrblCoordinates)

