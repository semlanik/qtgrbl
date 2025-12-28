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
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>

#include "grblconsole.h"
#include "grblengine.h"
#include "grblserial.h"

#include <QSurfaceFormat>
//#include <QQuick3D>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QFontDatabase::addApplicationFont(":/qtgrbl/fonts/DSEG14.ttf");
//    QSurfaceFormat::setDefaultFormat(QQuick3D::idealSurfaceFormat());
    //Make connections between components
    QtGrbl::GrblEngine::instance()->attach(QtGrbl::GrblSerial::instance());
    QObject::connect(QtGrbl::GrblConsole::instance(), qOverload<const QString &, QtGrbl::CommandPriority>(&QtGrbl::GrblConsole::sendCommand),
                     QtGrbl::GrblSerial::instance(), qOverload<const QString &, QtGrbl::CommandPriority>(&QtGrbl::GrblSerial::sendCommand));
    QObject::connect(QtGrbl::GrblSerial::instance(), &QtGrbl::GrblSerial::commandSent,
                     QtGrbl::GrblConsole::instance(), &QtGrbl::GrblConsole::writeCommand);
    QObject::connect(QtGrbl::GrblSerial::instance(), &QtGrbl::GrblSerial::responseReceived,
                     QtGrbl::GrblConsole::instance(), &QtGrbl::GrblConsole::writeResponse);
    app.setFont(QFont("Hack", 10));

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("QtGrbl", "Main");
    return app.exec();
}
