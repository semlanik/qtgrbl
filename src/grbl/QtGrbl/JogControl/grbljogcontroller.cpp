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

#include "grbljogcontroller.h"

#include "grblengine.h"
#include "grblsettingsmodel.h"

Q_STATIC_LOGGING_CATEGORY(JogTraces, "qtgbl.jog.traces")

namespace {
inline QByteArray valueToGCode(QLatin1StringView gCodeTemplate, double value)
{
    return QString(gCodeTemplate).arg(value, 0, 'f', 3).toLatin1();
}

constexpr QByteArrayView jogGCode("$J=G53");
constexpr QLatin1StringView xGCode(" X%1");
constexpr QLatin1StringView yGCode(" Y%1");
constexpr QLatin1StringView zGCode(" Z%1");
constexpr QLatin1StringView feedGCode(" F%1");
}

namespace QtGrbl {

GrblJogController::GrblJogController(GrblSettingsModel *settings, QObject *parent) : QObject(parent)
{
    Q_ASSERT(settings != nullptr);
    connect(settings, &GrblSettingsModel::settingUpdated, this, [settings, this] (int code) {
        decltype(&GrblJogController::setXLimits) setter = nullptr;
        switch (code) {
        case 130:
            setter = &GrblJogController::setXLimits;
        break;
        case 131:
            setter = &GrblJogController::setYLimits;
        break;
        case 132:
            setter = &GrblJogController::setZLimits;
        break;
        case 25:
            if (const auto *setting = static_cast<const FloatSetting *>(settings->setting(code))) {
                setFeedRate(setting->value);
            }
        default:
            return;
        }

        if (setter) {
            qDebug() << "settingUpdated" << code;
            const auto *setting = static_cast<const FloatSetting *>(settings->setting(code));
            if (!setting)
                (this->*setter)(0.0, 0.0);
            else
                (this->*setter)(setting->value * -1.0, -1.0);

        }
    });
}

bool GrblJogController::jog(QString x, QString y, QString z)
{
    return jogImpl(x.toDouble(), y.toDouble(), z.toDouble());
}

bool GrblJogController::jog(JogDirections directions)
{
    qDebug() << "Jog" << directions;
    if (directions.testFlags({JogDirection::Front, JogDirection::Back})
        || directions.testFlags({JogDirection::Left, JogDirection::Right})
        || directions.testFlags({JogDirection::Up, JogDirection::Down})) {
        qCritical() << "Invalid jog motion in oposite directions" << directions;
        return false;
    }

    // TODO: consider inversion mask
    QByteArray gcode(jogGCode);
    if (directions.testFlag(JogDirection::Left))
        gcode.append(valueToGCode(xGCode, m_xUpperLimit));
    else if (directions.testFlag(JogDirection::Right))
        gcode.append(valueToGCode(xGCode, m_xLowerLimit));


    if (directions.testFlag(JogDirection::Back))
        gcode.append(valueToGCode(yGCode, m_yUpperLimit));
    else if (directions.testFlag(JogDirection::Front))
        gcode.append(valueToGCode(yGCode, m_yLowerLimit));

    if (directions.testFlag(JogDirection::Down))
        gcode.append(valueToGCode(zGCode, m_zUpperLimit));
    else if (directions.testFlag(JogDirection::Up))
        gcode.append(valueToGCode(zGCode, m_zLowerLimit));

    // Use hald of Homing seek rate by now. If setting is not received from machine fall back to 250 mm/min.
    gcode.append(valueToGCode(feedGCode, m_feedRate));

    return jogImpl(gcode);
}

bool GrblJogController::jogImpl(qreal x, qreal y, qreal z)
{
    QByteArray gcode(jogGCode);
    gcode.append(valueToGCode(xGCode, x));
    gcode.append(valueToGCode(yGCode, y));
    gcode.append(valueToGCode(zGCode, z));
    gcode.append(valueToGCode(feedGCode, m_feedRate));
    return jogImpl(gcode);
}

bool GrblJogController::jogImpl(const QByteArray &command)
{
    auto *engine = GrblEngine::instance();
    switch (engine->grblStatus()->grblState())
    {
    case GrblStatus::GrblState::Jog:
        qCDebug(JogTraces, "Another jog is active cancel it");
        cancel();
        break;
    case GrblStatus::GrblState::Idle:
        break;
    default:
        qCDebug(JogTraces) << "Jog is disabled in " << engine->grblStatus()->grblState() << " state";
        return false;
    }

    qCDebug(JogTraces) << "Start jogging in " << engine->grblStatus()->grblState() << " state";
    qCDebug(JogTraces, "Make jog with following GCode %s", qPrintable(command));
    engine->sendCommand(command, QtGrbl::CommandPriority::Back);
    return true;
}


bool GrblJogController::cancel()
{
    // 0x85
    auto *engine = GrblEngine::instance();
    qCDebug(JogTraces, "Jog cancel");
    engine->sendCommand("\x85", QtGrbl::CommandPriority::Back);
    return true;
}

qreal GrblJogController::xUpperLimit() const
{
    return m_xUpperLimit;
}

qreal GrblJogController::xLowerLimit() const
{
    return m_xLowerLimit;
}

qreal GrblJogController::yUpperLimit() const
{
    return m_yUpperLimit;
}

qreal GrblJogController::yLowerLimit() const
{
    return m_yLowerLimit;
}

qreal GrblJogController::zUpperLimit() const
{
    return m_zUpperLimit;
}

qreal GrblJogController::zLowerLimit() const
{
    return m_zLowerLimit;
}

void GrblJogController::setXLimits(qreal upper, qreal lower)
{
    qCDebug(JogTraces) << "setXLimits" << upper << "," << lower;

    if (m_xUpperLimit != upper) {
        m_xUpperLimit = upper;
        emit xUpperLimitChanged();
    }

    if (m_xLowerLimit != lower) {
        m_xLowerLimit = lower;
        emit xLowerLimitChanged();
    }
}

void GrblJogController::setYLimits(qreal upper, qreal lower)
{
    qCDebug(JogTraces) << "setYLimits" << upper << "," << lower;
    if (m_yUpperLimit != upper) {
        m_yUpperLimit = upper;
        emit yUpperLimitChanged();
    }

    if (m_yLowerLimit != lower) {
        m_yLowerLimit = lower;
        emit yLowerLimitChanged();
    }
}

void GrblJogController::setZLimits(qreal upper, qreal lower)
{
    qCDebug(JogTraces) << "setZLimits" << upper << "," << lower;
    if (m_zUpperLimit != upper) {
        m_zUpperLimit = upper;
        emit zUpperLimitChanged();
    }

    if (m_zLowerLimit != lower) {
        m_zLowerLimit = lower;
        emit zLowerLimitChanged();
    }
}

qreal GrblJogController::feedRate() const
{
    return m_feedRate;
}

void GrblJogController::setFeedRate(qreal newFeedRate)
{
    if (qFuzzyCompare(m_feedRate, newFeedRate))
        return;
    m_feedRate = newFeedRate;
    emit feedRateChanged();
}

}
