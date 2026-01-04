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

#include "grblengine.h"

#include "grblgcodestate.h"
#include "grblserial.h"
#include "grblstatus.h"

#include <QFile>
#include <QTimer>
#include <QUrl>

#include <QDebug>

namespace {
    // 5Hz according to recomendation:
    // https://github.com/gnea/grbl/wiki/Grbl-v1.1-Interface#status-reporting
    constexpr int StatusUpdateInterval = 200;
}

using namespace QtGrbl;

GrblEngine::GrblEngine(QObject *parent) : QObject(parent),
    m_gcodeState(std::make_unique<GrblGCodeState>()),
    m_grblStatus(std::make_unique<GrblStatus>()),
    m_settings(std::make_unique<GrblSettingsModel>()),
    m_settingsProxy(std::make_unique<GrblSettingsSortingModel>())
{
    m_settingsProxy->setSourceModel(m_settings.get());
    m_settingsProxy->sort(0, Qt::AscendingOrder);
    initStatusUpdates();
}

GrblEngine::~GrblEngine()
{
}

void GrblEngine::initStatusUpdates()
{
    m_statusTimer.setInterval(StatusUpdateInterval);
    m_statusTimer.setSingleShot(false);
    connect(&m_statusTimer, &QTimer::timeout, this, [this] {
        emit sendCommand(QByteArray("?"), CommandPriority::Realtime);
    });
}

void GrblEngine::setFilePath(const QString &fileUrl)
{
    QString filePath = QUrl(fileUrl).toLocalFile();
    m_file.close();
    m_file.setFileName("");
    if (!QFile::exists(filePath)) {
        qCritical() << "File doesn't exists" << filePath;
        emit filePathChanged();
        return;
    }

    m_file.setFileName(filePath);
    if (!m_file.open(QFile::ReadOnly)) {
        qCritical() << "Unable to open file" << filePath;
        m_file.setFileName("");
        emit filePathChanged();
        return;
    }
}

void GrblEngine::resetState()
{
    m_grblStatus->clear();
    m_gcodeState->clear();
}

void GrblEngine::applySettings()
{
    const auto settings = m_settings->serialize(QtGrbl::GrblSettingsModel::SerializeMode::Changed);
    for (auto it = settings.cbegin(); it != settings.cend(); ++it)
        emit sendCommand(*it, CommandPriority::Back);
    emit sendCommand("$$", CommandPriority::Back);
    qDebug() << "Apply settings: " << settings;
}

void GrblEngine::saveSettings(const QUrl &fileUrl) const
{
    qDebug() << "Save settings to" << fileUrl;
    if (!fileUrl.isLocalFile()) {
        // TODO: show error message
        qDebug() << "Only local files are supported.";
        return;
    }

    QFile settingFile(fileUrl.path());
    if (!settingFile.open(QFile::WriteOnly)) {
        // TODO: show error message
        qDebug() << "Unable to open file to write settings" << settingFile.fileName();
        return;
    }
    const auto settings = m_settings->serialize();
    settingFile.write(settings.join("\r\n"));
}

void GrblEngine::start()
{
    if (!m_file.isOpen()) {
        qCritical() << "File is not opened";
        return;
    }

    while (!m_file.atEnd()) {
        emit sendCommand(m_file.readLine(), CommandPriority::Back);
    }
}

void GrblEngine::home()
{
    if(m_serialEngine.isNull()) {
        qCritical() << "Unable to reset the current position to zero, serial engine is null";
        return;
    }
    emit sendCommand(QByteArray("$H"), CommandPriority::Back);
}

void GrblEngine::resetToZero()
{
    if(m_serialEngine.isNull()) {
        qCritical() << "Unable to reset the current position to zero, serial engine is null";
        return;
    }

    emit sendCommand(QByteArray("G92 X0 Y0 Z0"), CommandPriority::Back);
}

void GrblEngine::attach(GrblSerial *serialEngine)
{
    if (!m_serialEngine.isNull()) {
        qCritical() << "Already attached to serial engine";
        return;
    }

    m_serialEngine = serialEngine;
    connect(this, qOverload<const QByteArray &, QtGrbl::CommandPriority>(&GrblEngine::sendCommand),
            m_serialEngine, qOverload<const QByteArray &, QtGrbl::CommandPriority>(&GrblSerial::sendCommand));
    connect(this, qOverload<const QByteArrayList &, QtGrbl::CommandPriority>(&GrblEngine::sendCommand),
            m_serialEngine, qOverload<QByteArrayList, QtGrbl::CommandPriority>(&GrblSerial::sendCommand));
    connect(m_serialEngine, &GrblSerial::responseReceived, this, [this](const QByteArray &response) {
        if (response.startsWith(GCodeStatePrefix)) {
            m_gcodeState->parseRawData(response);
        } else if (response.startsWith(GrblStatusPrefix)) {
            m_grblStatus->parseRawData(response);
        } else if (response.startsWith('$')) {
            if (!m_settings->parseItemData(response))
                qDebug() << "Unknown setting received" << response;
        }
    });
}

void GrblEngine::hold()
{
    if (m_serialEngine.isNull()) {
        qCritical() << "Unable to hold, serial engine is null";
        return;
    }
    //Stop Spindle first
    emit sendCommand(QByteArrayList() << "!", CommandPriority::Realtime);
}

void GrblEngine::resume()
{
    if (m_serialEngine.isNull()) {
        qCritical() << "Unable to resume, serial engine is null";
        return;
    }
    emit sendCommand(QByteArrayList() << "~", CommandPriority::Realtime);
    subscribeStatusUpdate();
}

void GrblEngine::stop()
{
    if (m_serialEngine.isNull()) {
        qCritical() << "Unable to stop, serial engine is null";
        return;
    }
    emit sendCommand(QByteArrayList() << "M5" << "M0", CommandPriority::Front);
    m_serialEngine->clearCommandQueue();
}

void GrblEngine::returnToZero()
{
    if (m_serialEngine.isNull()) {
        qCritical() << "Unable to resume, serial engine is null";
        return;
    }
    emit sendCommand(QByteArrayList() << "G90" << "G0 Z5" << "G0 Y0 X0" << "G0 Z0",
                     CommandPriority::Front);
}

void GrblEngine::reset()
{
    emit sendCommand(QByteArray("\x18"), CommandPriority::Realtime);
}

void GrblEngine::resetAlarm()
{
    emit sendCommand(QByteArray("$X"), CommandPriority::Front);
}

void GrblEngine::updateGCodeState()
{
    m_gcodeState->clear();
    emit sendCommand(QByteArray("$G"), CommandPriority::Front);
}

void GrblEngine::subscribeStatusUpdate()
{
    m_statusTimer.start();
}

void GrblEngine::unsubscribeStatusUpdate()
{
    m_statusTimer.stop();
}

void GrblEngine::requestSettings()
{
    emit sendCommand(QByteArray("$$"), CommandPriority::Front);
}

void GrblEngine::clearSettings()
{
    m_settings->clear();
}
