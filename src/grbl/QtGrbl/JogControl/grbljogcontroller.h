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

#include <cstdint>

namespace QtGrbl {

class GrblSettingsModel;

class GrblJogController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Belongs to the active GrblEngine")

    Q_PROPERTY(qreal xUpperLimit READ xUpperLimit NOTIFY xUpperLimitChanged)
    Q_PROPERTY(qreal xLowerLimit READ xLowerLimit NOTIFY xLowerLimitChanged)

    Q_PROPERTY(qreal yUpperLimit READ yUpperLimit NOTIFY yUpperLimitChanged)
    Q_PROPERTY(qreal yLowerLimit READ yLowerLimit NOTIFY yLowerLimitChanged)

    Q_PROPERTY(qreal zUpperLimit READ zUpperLimit NOTIFY zUpperLimitChanged)
    Q_PROPERTY(qreal zLowerLimit READ zLowerLimit NOTIFY zLowerLimitChanged)

    Q_PROPERTY(qreal feedRate READ feedRate WRITE setFeedRate NOTIFY feedRateChanged FINAL)

public:
    enum class JogDirection : uint8_t
    {
        None = 0,
        Left = 1u << 1,
        Right = 1u << 2,
        Front = 1u << 3,
        Back = 1u << 4,
        Up = 1u << 5,
        Down = 1u << 6,
    };
    Q_ENUM(JogDirection)

    Q_DECLARE_FLAGS(JogDirections, JogDirection)
    Q_FLAG(JogDirections)

    GrblJogController(GrblSettingsModel *settings, QObject *parent = nullptr);

    Q_INVOKABLE bool jog(JogDirections directions);
    Q_INVOKABLE bool jog(const QString x, const QString y, const QString z);
    Q_INVOKABLE bool cancel();

    qreal xUpperLimit() const;
    qreal xLowerLimit() const;

    qreal yUpperLimit() const;
    qreal yLowerLimit() const;

    qreal zUpperLimit() const;
    qreal zLowerLimit() const;

    void setXLimits(qreal upper, qreal lower);
    void setYLimits(qreal upper, qreal lower);
    void setZLimits(qreal upper, qreal lower);

    qreal feedRate() const;
    void setFeedRate(qreal newFeedRate);

signals:
    void xUpperLimitChanged();
    void xLowerLimitChanged();

    void yUpperLimitChanged();
    void yLowerLimitChanged();

    void zUpperLimitChanged();
    void zLowerLimitChanged();

    void feedRateChanged();

private:
    bool jogImpl(qreal x, qreal y, qreal z);
    bool jogImpl(const QByteArray &command);

    qreal m_xUpperLimit = 0.0;
    qreal m_xLowerLimit = 0.0;
    qreal m_yUpperLimit = 0.0;
    qreal m_yLowerLimit = 0.0;
    qreal m_zUpperLimit = 0.0;
    qreal m_zLowerLimit = 0.0;
    qreal m_feedRate = 250.0;
};

}
