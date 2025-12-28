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

#include "grblabstractdatamodel.h"

#include <QObject>
#include <QByteArray>
#include <QtQmlIntegration>
#include <grblconsolerecord.h>

namespace QtGrbl {
class GrblGCodeState : public GrblAbstractDataModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int toolNumber READ toolNumber NOTIFY toolNumberChanged)
    Q_PROPERTY(qreal spindleSpeed READ spindleSpeed NOTIFY spindleSpeedChanged)
    Q_PROPERTY(qreal feedRate READ feedRate NOTIFY feedRateChanged)
    Q_PROPERTY(QtGrbl::GrblGCodeState::MotionMode motionMode READ motionMode WRITE setMotionMode NOTIFY motionModeChanged);
    Q_PROPERTY(QtGrbl::GrblGCodeState::PlaneSelect planeSelect READ planeSelect WRITE setPlaneSelect NOTIFY planeSelectChanged)
    Q_PROPERTY(QtGrbl::GrblGCodeState::DistanceMode distanceMode READ distanceMode WRITE setDistanceMode NOTIFY distanceModeChanged)
    Q_PROPERTY(QtGrbl::GrblGCodeState::FeedRateMode feedRateMode READ feedRateMode WRITE setFeedRate NOTIFY feedRateModeChanged)
    Q_PROPERTY(QtGrbl::GrblGCodeState::UnitsMode unitsMode READ unitsMode WRITE setUnitsMode NOTIFY unitsModeChanged)
    Q_PROPERTY(QtGrbl::GrblGCodeState::ToolLengthOffset toolLengthOffset READ toolLengthOffset WRITE setToolLengthOffset NOTIFY toolLengthOffsetChanged)
    Q_PROPERTY(QtGrbl::GrblGCodeState::ProgramMode programMode READ programMode WRITE setProgramMode NOTIFY programModeChanged)
    Q_PROPERTY(QtGrbl::GrblGCodeState::SpindleState spindleState READ spindleState WRITE setSpindleState NOTIFY spindleStateChanged)
    Q_PROPERTY(QtGrbl::GrblGCodeState::CoolantState coolantState READ coolantState WRITE setCoolantState NOTIFY coolantStateChanged)
    Q_PROPERTY(bool cutterRadiusCompensation READ cutterRadiusCompensation WRITE setCutterRadiusCompensation NOTIFY cutterRadiusCompensationChanged)
    Q_PROPERTY(bool arcIJKDistanceMode READ arcIJKDistanceMode WRITE setArcIJKDistanceMode NOTIFY arcIJKDistanceModeChanged)

    Q_PROPERTY(QString motionModeString READ motionModeString NOTIFY motionModeChanged)
    Q_PROPERTY(QString planeSelectString READ planeSelectString NOTIFY planeSelectChanged)
    Q_PROPERTY(QString distanceModeString READ distanceModeString NOTIFY distanceModeChanged)
    Q_PROPERTY(QString feedRateModeString READ feedRateModeString NOTIFY feedRateModeChanged)
    Q_PROPERTY(QString unitsModeString READ unitsModeString NOTIFY unitsModeChanged)
    Q_PROPERTY(QString toolLengthOffsetString READ toolLengthOffsetString NOTIFY toolLengthOffsetChanged)
    Q_PROPERTY(QString programModeString READ programModeString NOTIFY programModeChanged)
    Q_PROPERTY(QString spindleStateString READ spindleStateString NOTIFY spindleStateChanged)
    Q_PROPERTY(QString coolantStateString READ coolantStateString NOTIFY coolantStateChanged)

public:
    enum MotionMode {
        RapidPositioning,
        LinearInterpolation,
        CircularInterpolationCW,
        CircularInterpolationCCW,
        CancelCannedCycle
    };
    Q_ENUM(MotionMode)

    enum PlaneSelect {
        XYPlane,
        ZXPlane,
        YZPlane
    };
    Q_ENUM(PlaneSelect)

    enum DistanceMode {
        AbsoluteProgramming,
        IncrementalProgramming
    };
    Q_ENUM(DistanceMode)

    enum FeedRateMode {
        InverseTime,
        PerMinute
    };
    Q_ENUM(FeedRateMode)

    enum UnitsMode {
        Inches,
        Millimeters
    };
    Q_ENUM(UnitsMode)

    enum ToolLengthOffset {
        CompensationNegative,
        CompensationCancel
    };
    Q_ENUM(ToolLengthOffset)

    enum ProgramMode {
        CompulsoryStop,
        OptionalStop,
        EndOfProgram,
        EndOfProgramWithReturn
    };
    Q_ENUM(ProgramMode)

    enum SpindleState {
        SpindleOnCW,
        SpindleOnCCW,
        SpindleStop
    };
    Q_ENUM(SpindleState)

    enum CoolantState {
        CoolantMist,
        CoolantFlood,
        CoolantOff
    };
    Q_ENUM(CoolantState)

    explicit GrblGCodeState(const QByteArray &stateData = {}, QObject *parent = nullptr);
    virtual ~GrblGCodeState() = default;

    int toolNumber() const
    {
        return m_toolNumber;
    }

    qreal spindleSpeed() const
    {
        return m_spindleSpeed;
    }

    qreal feedRate() const
    {
        return m_feedRate;
    }

    QtGrbl::GrblGCodeState::MotionMode motionMode() const
    {
        return m_motionMode;
    }

    QtGrbl::GrblGCodeState::PlaneSelect planeSelect() const
    {
        return m_planeSelect;
    }

    QtGrbl::GrblGCodeState::DistanceMode distanceMode() const
    {
        return m_distanceMode;
    }

    QtGrbl::GrblGCodeState::FeedRateMode feedRateMode() const
    {
        return m_feedRateMode;
    }

    QtGrbl::GrblGCodeState::UnitsMode unitsMode() const
    {
        return m_unitsMode;
    }

    QtGrbl::GrblGCodeState::ToolLengthOffset toolLengthOffset() const
    {
        return m_toolLengthOffset;
    }

    QtGrbl::GrblGCodeState::ProgramMode programMode() const
    {
        return m_programMode;
    }

    QtGrbl::GrblGCodeState::SpindleState spindleState() const
    {
        return m_spindleState;
    }

    QtGrbl::GrblGCodeState::CoolantState coolantState() const
    {
        return m_coolantState;
    }

    bool cutterRadiusCompensation() const
    {
        return m_cutterRadiusCompensation;
    }

    bool arcIJKDistanceMode() const
    {
        return m_arcIJKDistanceMode;
    }

    QString motionModeString() const;
    QString planeSelectString() const;
    QString distanceModeString() const;
    QString feedRateModeString() const;
    QString unitsModeString() const;
    QString toolLengthOffsetString() const;
    QString programModeString() const;
    QString spindleStateString() const;
    QString coolantStateString() const;

    void setToolNumber(int toolNumber);
    void setSpindleSpeed(qreal spindleSpeed);
    void setFeedRate(qreal feedRate);
    void setMotionMode(QtGrbl::GrblGCodeState::MotionMode motionMode);
    void setPlaneSelect(QtGrbl::GrblGCodeState::PlaneSelect planeSelect);
    void setDistanceMode(QtGrbl::GrblGCodeState::DistanceMode distanceMode);
    void setFeedRateMode(QtGrbl::GrblGCodeState::FeedRateMode feedRateMode);
    void setUnitsMode(QtGrbl::GrblGCodeState::UnitsMode unitsMode);
    void setToolLengthOffset(QtGrbl::GrblGCodeState::ToolLengthOffset toolLengthOffset);
    void setProgramMode(QtGrbl::GrblGCodeState::ProgramMode programMode);
    void setSpindleState(QtGrbl::GrblGCodeState::SpindleState spindleState);
    void setCoolantState(QtGrbl::GrblGCodeState::CoolantState coolantState);
    void setCutterRadiusCompensation(bool cutterRadiusCompensation);
    void setArcIJKDistanceMode(bool arcIJKDistanceMode);

signals:
    void toolNumberChanged();
    void spindleSpeedChanged();
    void feedRateChanged();
    void motionModeChanged();
    void planeSelectChanged();
    void distanceModeChanged();
    void feedRateModeChanged();
    void unitsModeChanged();
    void toolLengthOffsetChanged();
    void programModeChanged();
    void spindleStateChanged();
    void coolantStateChanged();
    void cutterRadiusCompensationChanged();
    void arcIJKDistanceModeChanged();

protected:
    bool parseData() override;

private:
    int m_toolNumber;
    qreal m_spindleSpeed;
    qreal m_feedRate;
    MotionMode m_motionMode;
    PlaneSelect m_planeSelect;
    DistanceMode m_distanceMode;
    FeedRateMode m_feedRateMode;
    UnitsMode m_unitsMode;
    ProgramMode m_programMode;
    SpindleState m_spindleState;
    CoolantState m_coolantState;
    ToolLengthOffset m_toolLengthOffset;
    bool m_cutterRadiusCompensation;
    bool m_arcIJKDistanceMode;
};
}

//Q_DECLARE_METATYPE(QtGrbl::GrblGCodeState)
