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

#include <memory>

#include <QFile>
#include <QFileInfo>
#include <QPointer>
#include <QTimer>

#include "qmlsingletonebase.h"
#include "qtgrblcommon.h"
#include "grblgcodestate.h"
#include "grblstatus.h"
#include "grblsettingsmodel.h"

namespace QtGrbl {

class GrblSerial;

class GrblEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(QtGrbl::GrblGCodeState *gcodeState READ gcodeState CONSTANT)
    Q_PROPERTY(QtGrbl::GrblStatus *grblStatus READ grblStatus CONSTANT)
    Q_PROPERTY(QtGrbl::GrblSettingsModel *settings READ settings CONSTANT)
    Q_PROPERTY(QtGrbl::GrblSettingsSortingModel *settingsProxy READ settingsProxy CONSTANT)
public:
    explicit GrblEngine(QObject *parent = nullptr);
    virtual ~GrblEngine();

    static GrblEngine* instance()
    {
        static GrblEngine instance;
        return &instance;
    }

    void attach(GrblSerial *serialEngine);
    Q_INVOKABLE void start();
    Q_INVOKABLE void home();
    Q_INVOKABLE void returnToZero();
    Q_INVOKABLE void resetToZero();
    Q_INVOKABLE void hold();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void resetAlarm();
    Q_INVOKABLE void updateGCodeState();
    Q_INVOKABLE void subscribeStatusUpdate();
    Q_INVOKABLE void unsubscribeStatusUpdate();
    Q_INVOKABLE void requestSettings();
    Q_INVOKABLE void clearSettings();


    QString filePath() const
    {
        return QFileInfo(m_file).absoluteFilePath();
    }

    GrblGCodeState *gcodeState() const
    {
        return m_gcodeState.get();
    }

    void setFilePath(const QString &fileUrl);

    QtGrbl::GrblStatus *grblStatus() const
    {
        return m_grblStatus.get();
    }

    QtGrbl::GrblSettingsModel *settings() const
    {
        return m_settings.get();
    }

    GrblSettingsSortingModel *settingsProxy() const
    {
        return m_settingsProxy.get();
    }

    Q_INVOKABLE void resetState();

    Q_INVOKABLE void applySettings();
    Q_INVOKABLE void saveSettings(const QUrl &fileUrl) const;
signals:
    void consoleOutputChanged();
    void filePathChanged();

    void sendCommand(const QByteArray &command, QtGrbl::CommandPriority prio);
    void sendCommand(const QByteArrayList &command, QtGrbl::CommandPriority prio);
    void gcodeStateChanged();
    void grblStateChanged();

private:
    void initStatusUpdates();

    QFile m_file;
    QPointer<GrblSerial> m_serialEngine;
    std::unique_ptr<GrblGCodeState> m_gcodeState;
    std::unique_ptr<GrblStatus> m_grblStatus;
    std::unique_ptr<GrblSettingsModel> m_settings;
    std::unique_ptr<GrblSettingsSortingModel> m_settingsProxy;
    QTimer m_statusTimer;
    inline static QJSEngine *s_engine = nullptr;
};

class QmlGrblEngine : public QmlSingletoneBase<GrblEngine>
{
    Q_GADGET
    QML_FOREIGN(GrblEngine)
    QML_SINGLETON
    QML_NAMED_ELEMENT(GrblEngine)
};

}

//Q_DECLARE_METATYPE(QtGrbl::GrblEngine)
