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
#include "grblsettingsmodel.h"

#include <QStringLiteral>
#include <QRegularExpression>

#include <array>

namespace {
std::array IntSettings {
    QtGrbl::IntSetting{0, "Step pulse time", 10, "µs"},
    QtGrbl::IntSetting{1, "Step idle delay", 255, "ms"},
    QtGrbl::IntSetting{26, "Homing debounce", 10, "ms"},
};

std::array FloatSettings {
    QtGrbl::FloatSetting {11, "Junction deviation", 0.010, "mm"},
    QtGrbl::FloatSetting {12, "Arc tolerance", 0.002, "mm"},
    QtGrbl::FloatSetting {24, "Homing feed", 25.0, "mm/min"},
    QtGrbl::FloatSetting {25, "Homing seek", 500.0, "mm/min"},
    QtGrbl::FloatSetting {27, "Homing pulloff", 1.0, "mm"},
    QtGrbl::FloatSetting {30, "Max spindle speed", 1000.0, "RPM"},
    QtGrbl::FloatSetting {31, "Min spindle speed", 0.0, "RPM"},
    QtGrbl::FloatSetting {100, "Step per millimeter X", 250.0, "steps/mm"},
    QtGrbl::FloatSetting {101, "Step per millimeter Y", 250.0, "steps/mm"},
    QtGrbl::FloatSetting {102, "Step per millimeter Z", 250.0, "steps/mm"},
    QtGrbl::FloatSetting {110, "Max rate X", 500.0, "mm/min"},
    QtGrbl::FloatSetting {111, "Max rate Y", 500.0, "mm/min"},
    QtGrbl::FloatSetting {112, "Max rate Z", 500.0, "mm/min"},
    QtGrbl::FloatSetting {120, "Acceleration X", 10.0, "mm/s²"},
    QtGrbl::FloatSetting {121, "Acceleration Y", 10.0, "mm/s²"},
    QtGrbl::FloatSetting {122, "Acceleration Z", 10.0, "mm/s²"},
    QtGrbl::FloatSetting {130, "Max travel X", 200.0, "mm"},
    QtGrbl::FloatSetting {131, "Max travel Y", 200.0, "mm"},
    QtGrbl::FloatSetting {132, "Max travel Z", 200.0, "mm"},
};

std::array BoolSettings {
    QtGrbl::BoolSetting {4, "Step enable invert", false},
    QtGrbl::BoolSetting {5, "Limit pins invert", false},
    QtGrbl::BoolSetting {6, "Probe pin invert", false},
    QtGrbl::BoolSetting {13, "Report inches", false},
    QtGrbl::BoolSetting {20, "Soft limits", false},
    QtGrbl::BoolSetting {21, "Hard limits", true},
    QtGrbl::BoolSetting {22, "Homing cycle", true},
    QtGrbl::BoolSetting {32, "Laser mode", false},
};

std::array MaskSettings {
    QtGrbl::MaskSetting {2, "Step port invert", 0},
    QtGrbl::MaskSetting {3, "Direction port invert", 0},
    QtGrbl::MaskSetting {10, "Status report", 0},
    QtGrbl::MaskSetting {23, "Homing dir invert", 0},
};

constexpr size_t SettingsSize = IntSettings.size() + FloatSettings.size() + BoolSettings.size() + MaskSettings.size();
constexpr size_t IntSettintgsStartIndex = 0;
constexpr size_t FloatSettingsStartIndex = IntSettintgsStartIndex + IntSettings.size();
constexpr size_t BoolSettingsStartIndex = FloatSettingsStartIndex + FloatSettings.size();
constexpr size_t MaskSettingsStartIndex = BoolSettingsStartIndex + BoolSettings.size();

enum SettingModelRoles
{
    ControlType = Qt::UserRole + 1,
    Data,
    Valid,
    Changed,
    UserValue,
};

std::optional<int> codeByIndex(int row)
{
    std::optional<int> result = std::nullopt;
    if (row >= IntSettintgsStartIndex && row < FloatSettingsStartIndex) {
        result.emplace(IntSettings.at(row - IntSettintgsStartIndex).code());
    } else if (row >= FloatSettingsStartIndex && row < BoolSettingsStartIndex) {
        result.emplace(FloatSettings.at(row - FloatSettingsStartIndex).code());
    } else if (row >= BoolSettingsStartIndex && row < MaskSettingsStartIndex) {
        result.emplace(BoolSettings.at(row - BoolSettingsStartIndex).code());
    } else if (row >= MaskSettingsStartIndex && row < SettingsSize) {
        result.emplace(MaskSettings.at(row - MaskSettingsStartIndex).code());
    }
    return result;
}

QVariant valueByIndex(int row)
{
    if (row >= IntSettintgsStartIndex && row < FloatSettingsStartIndex) {
        return QVariant::fromValue(IntSettings.at(row - IntSettintgsStartIndex).value);
    } else if (row >= FloatSettingsStartIndex && row < BoolSettingsStartIndex) {
        return QVariant::fromValue(FloatSettings.at(row - FloatSettingsStartIndex).value);
    } else if (row >= BoolSettingsStartIndex && row < MaskSettingsStartIndex) {
        return QVariant::fromValue(BoolSettings.at(row - BoolSettingsStartIndex).value);
    } else if (row >= MaskSettingsStartIndex && row < SettingsSize) {
        return QVariant::fromValue(MaskSettings.at(row - MaskSettingsStartIndex).value);
    }
    return {};
}

QVariant dataByIndex(int row)
{
    if (row >= IntSettintgsStartIndex && row < FloatSettingsStartIndex) {
        return QVariant::fromValue(IntSettings.at(row - IntSettintgsStartIndex));
    } else if (row >= FloatSettingsStartIndex && row < BoolSettingsStartIndex) {
        return QVariant::fromValue(FloatSettings.at(row - FloatSettingsStartIndex));
    } else if (row >= BoolSettingsStartIndex && row < MaskSettingsStartIndex) {
        return QVariant::fromValue(BoolSettings.at(row - BoolSettingsStartIndex));
    } else if (row >= MaskSettingsStartIndex && row < SettingsSize) {
        return QVariant::fromValue(MaskSettings.at(row - MaskSettingsStartIndex));
    }
    return {};
}

inline bool isInRange(int row)
{
    return row >= 0 && row < SettingsSize;
}

}

namespace QtGrbl
{

struct SettingsRawData
{
    QByteArray grblValues[SettingsSize];
    QVariant userValues[SettingsSize];

    QByteArray serializeUserValue(int row);
};

QByteArray SettingsRawData::serializeUserValue(int row)
{
    if (!isInRange(row) || userValues[row].isNull())
        return {};

    const auto code = QString::number(*codeByIndex(row));
    const QString baseString("$%1=%2");
    if (row >= IntSettintgsStartIndex && row < FloatSettingsStartIndex) {
        return baseString.arg(code, QString::number(userValues[row].toInt())).toLatin1();
    } else if (row >= FloatSettingsStartIndex && row < BoolSettingsStartIndex) {
        return baseString.arg(code, QString::number(userValues[row].toDouble(), 'f', 3)).toLatin1();
    } else if (row >= BoolSettingsStartIndex && row < MaskSettingsStartIndex) {
        return baseString.arg(code, QString::number(userValues[row].toInt())).toLatin1();
    } else if (row >= MaskSettingsStartIndex && row < SettingsSize) {
        return baseString.arg(code, QString::number(userValues[row].toInt())).toLatin1();
    }
    return {};
}

GrblSettingsModel::GrblSettingsModel()
{
    m_data = new SettingsRawData;
}

GrblSettingsModel::~GrblSettingsModel()
{
    delete m_data;
}

QHash<int, QByteArray> GrblSettingsModel::roleNames() const
{
    static const QHash<int, QByteArray> roles = {
        {SettingModelRoles::ControlType, "type"},
        {SettingModelRoles::Data, "data"},
        {SettingModelRoles::Valid, "isValid"},
        {SettingModelRoles::Changed, "isChanged"},
        {SettingModelRoles::UserValue, "userValue"}
    };
    return roles;
}

int GrblSettingsModel::rowCount(const QModelIndex &parent) const
{
    return SettingsSize;
}

QVariant GrblSettingsModel::data(const QModelIndex &index, int role) const
{
    if (const auto row = index.row(); isInRange(row)) {
        switch (role) {
        case SettingModelRoles::Data:
            if (!m_data->userValues[row].isNull())
                return m_data->userValues[row];
            else
                return dataByIndex(row);
            break;
        case SettingModelRoles::ControlType:
            if (const auto row = index.row(); row >= IntSettintgsStartIndex && row < FloatSettingsStartIndex) {
                return QVariant::fromValue(GrblSettingsModel::SettingType::Int);
            } else if (row >= FloatSettingsStartIndex && row < BoolSettingsStartIndex) {
                return QVariant::fromValue(GrblSettingsModel::SettingType::Float);
            } else if (row >= BoolSettingsStartIndex && row < MaskSettingsStartIndex) {
                return QVariant::fromValue(GrblSettingsModel::SettingType::Bool);
            } else if (row >= MaskSettingsStartIndex && row < SettingsSize) {
                return QVariant::fromValue(GrblSettingsModel::SettingType::Mask);
            }
            break;
        case SettingModelRoles::Valid:
            return QVariant::fromValue(!m_data->grblValues[row].isEmpty());
        case SettingModelRoles::UserValue:
            Q_ASSERT_X(false, "GrblSettingsModel::data", "Write only role userValue is attempted to read");
            return {};
        case SettingModelRoles::Changed:
            return QVariant::fromValue(!m_data->userValues[row].isNull() && m_data->userValues[row] != valueByIndex(row));
        }
    }
    return {};
}

const SettingBase *GrblSettingsModel::setting(int code) const
{
    SettingBase *result = nullptr;
    int row = -1;
    const auto compare = [code](const QtGrbl::SettingBase &v) -> bool { return v.code() == code; };
    if (auto it = std::find_if(IntSettings.begin(), IntSettings.end(), compare); it != IntSettings.end()) {
        row = IntSettintgsStartIndex + std::distance(IntSettings.begin(), it);
        if (!m_data->grblValues[row].isEmpty())
            result = it;
    } else if (auto it = std::find_if(FloatSettings.begin(), FloatSettings.end(), compare); it != FloatSettings.end()) {
        row = FloatSettingsStartIndex + std::distance(FloatSettings.begin(), it);
        if (!m_data->grblValues[row].isEmpty())
            result = it;
    } else if (auto it = std::find_if(BoolSettings.begin(), BoolSettings.end(), compare); it != BoolSettings.cend()) {
        row = BoolSettingsStartIndex + std::distance(BoolSettings.begin(), it);
        if (!m_data->grblValues[row].isEmpty())
            result = it;
    } else if (auto it = std::find_if(MaskSettings.begin(), MaskSettings.end(), compare); it != MaskSettings.end()) {
        row = MaskSettingsStartIndex + std::distance(MaskSettings.begin(), it);
        if (!m_data->grblValues[row].isEmpty())
            result = it;
    }

    return result;
}

QList<QByteArray> GrblSettingsModel::serialize(SerializeMode mode) const
{
    QList<QByteArray> result;
    result.reserve(SettingsSize);
    for (int i = 0; i < SettingsSize; ++i) {
        switch (mode) {
        case SerializeMode::All:
            if (!m_data->userValues[i].isNull())
                result.append(m_data->serializeUserValue(i));
            else if (!m_data->grblValues[i].isEmpty())
                result.append(m_data->grblValues[i]);
            break;
        case SerializeMode::Saved:
            if (!m_data->grblValues[i].isNull())
                result.append(m_data->grblValues[i]);
        case SerializeMode::Changed:
            if (!m_data->userValues[i].isNull())
                result.append(m_data->serializeUserValue(i));
        }
    }
    return result;
}


bool GrblSettingsModel::parseItemData(const QByteArray &data)
{
    qDebug() << "parseItemData" << data;
    static const QRegularExpression settingRegex("\\$(\\d{1,3})=([^\\s]+)\\s*");
    bool result = false;
    int row = -1;
    uint8_t code  = 0;
    bool clearUserValue = false;
    if (const auto match = settingRegex.match(QString::fromUtf8(data)); match.hasMatch()) {
        qDebug() << "Has match";
        code = uint8_t(match.captured(1).toInt(&result));
        if (!result)
            return result;

        const auto compare = [code](const QtGrbl::SettingBase &v) -> bool { return v.code() == code; };
        if (auto it = std::find_if(IntSettings.begin(), IntSettings.end(), compare); it != IntSettings.end()) {
            row = IntSettintgsStartIndex + std::distance(IntSettings.begin(), it);
            it->value = match.captured(2).toInt(&result);
            clearUserValue = m_data->userValues[row] == it->value;
        } else if (auto it = std::find_if(FloatSettings.begin(), FloatSettings.end(), compare); it != FloatSettings.end()) {
            row = FloatSettingsStartIndex + std::distance(FloatSettings.begin(), it);
            it->value = match.captured(2).toDouble(&result);
            clearUserValue = m_data->userValues[row] == it->value;
        } else if (auto it = std::find_if(BoolSettings.begin(), BoolSettings.end(), compare); it != BoolSettings.cend()) {
            row = BoolSettingsStartIndex + std::distance(BoolSettings.begin(), it);
            it->value = bool(match.captured(2).toInt(&result));
            clearUserValue = m_data->userValues[row] == it->value;
        } else if (auto it = std::find_if(MaskSettings.begin(), MaskSettings.end(), compare); it != MaskSettings.end()) {
            row = MaskSettingsStartIndex + std::distance(MaskSettings.begin(), it);
            it->value = match.captured(2).toInt(&result);
            clearUserValue = m_data->userValues[row] == it->value;
        }

        if (clearUserValue)
            m_data->userValues[row].clear();
        qDebug() << "Row: " << row;
    }
    if (isInRange(row)) {
        m_data->grblValues[row] = data;
        emit dataChanged(index(row), index(row), { SettingModelRoles::Data, SettingModelRoles::Valid, SettingModelRoles::Changed });
        emit settingUpdated(code);
    }

    return result;
}

void GrblSettingsModel::clear()
{
    for (auto &setting : m_data->grblValues)
        setting.clear();
    emit dataChanged(index(0), index(SettingsSize - 1), { SettingModelRoles::Valid });
}

bool GrblSettingsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != SettingModelRoles::UserValue)
        return false;

    if (const auto row = index.row(); isInRange(row)) {
        qDebug() << "set value" << row << value;
        if (m_data->userValues[row] != value) {
            m_data->userValues[row] = value;
            auto roles = QList<int> { SettingModelRoles::Changed };
            if (m_data->userValues[row].isNull())
                roles << SettingModelRoles::Data;

            emit dataChanged(index, index, roles);
            emit hasChangesChanged();
            return true;
        } else if (value.isNull()) {

            emit dataChanged(index, index, { SettingModelRoles::Changed });
            emit hasChangesChanged();
            return true;
        }
    }
    return false;
}

void GrblSettingsModel::resetChanges()
{
    for (auto &setting : m_data->userValues)
        setting.clear();
    emit dataChanged(index(0), index(SettingsSize - 1), { SettingModelRoles::Data, SettingModelRoles::Changed });
    emit hasChangesChanged();
}

GrblSettingsSortingModel::GrblSettingsSortingModel(QObject* parent) : QSortFilterProxyModel(parent)
{
}

bool GrblSettingsSortingModel::lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const
{
    const auto codeLeft = codeByIndex(sourceLeft.row());
    const auto codeRight = codeByIndex(sourceRight.row());
    return *codeLeft < *codeRight;
}

bool GrblSettingsModel::hasChanges() const
{
    for (const auto& userValue : m_data->userValues) {
        if (!userValue.isNull())
            return true;
    }

    return false;
}

}
