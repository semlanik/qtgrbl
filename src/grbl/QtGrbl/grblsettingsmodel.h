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

#include <QAbstractListModel>
#include <QMetaObject>
#include <QSortFilterProxyModel>
#include <QString>
#include <QtTypes>
#include <QtQmlIntegration>

#include <cstdint>
#include <string_view>

namespace QtGrbl
{

struct SettingBase
{
    Q_GADGET
    Q_PROPERTY(int code MEMBER m_code CONSTANT FINAL)
    Q_PROPERTY(QString name READ name CONSTANT FINAL)
public:
    SettingBase(int code, std::string_view name) : m_code(code), m_name(name){}
    QString name() const { return QString::fromUtf8(m_name); };
    uint8_t code() const { return m_code; }

private:
    uint8_t m_code;
    std::string_view m_name;
};

template <typename T>
struct Setting : public SettingBase
{
    Setting(int code_, std::string_view name_, T default_) : SettingBase(code_, name_), value(default_) {}

    T value;
};

template <typename T>
struct UnitSetting : public Setting<T>
{
    UnitSetting(int code_, std::string_view name_, T default_, std::string_view unit_) : Setting<T>(code_, name_, default_), m_unit(unit_) {}
    std::string_view m_unit;
};

struct BoolSetting : public Setting<bool>
{
    Q_GADGET
    QML_VALUE_TYPE(boolSetting)

    Q_PROPERTY(bool value MEMBER value FINAL)
public:
    BoolSetting() : Setting(-1, "", false) {}
    BoolSetting(int code_, std::string_view name_, bool default_) : Setting(code_, name_, default_){}
};

struct IntSetting : public UnitSetting<int>
{
    Q_GADGET
    QML_VALUE_TYPE(intSetting)

    Q_PROPERTY(QString unit READ unit CONSTANT FINAL)
    Q_PROPERTY(int value MEMBER value FINAL)
public:
    IntSetting() : UnitSetting(-1, "", 0, "") {}
    IntSetting(int code_, std::string_view name_, int default_, std::string_view unit_) : UnitSetting(code_, name_, default_, unit_){}

    QString unit() const { return QString::fromUtf8(m_unit); }
};

struct FloatSetting : public UnitSetting<qreal>
{
    Q_GADGET
    QML_VALUE_TYPE(floatSetting)

    Q_PROPERTY(QString unit READ unit CONSTANT FINAL)
    Q_PROPERTY(qreal value MEMBER value FINAL)
public:
    FloatSetting() : UnitSetting(-1, "", 0, "") {}
    FloatSetting(int code_, std::string_view name_, qreal default_, std::string_view unit_) : UnitSetting(code_, name_, default_, unit_){}

    QString unit() const { return QString::fromUtf8(m_unit); }
};

struct MaskSetting : public Setting<int>
{
    Q_GADGET
    QML_VALUE_TYPE(maskSetting)

    Q_PROPERTY(int value MEMBER value FINAL)
public:
    MaskSetting() : Setting(-1, "", 0) {}
    MaskSetting(int code_, std::string_view name_, int default_) : Setting(code_, name_, default_){}
};

struct SettingsRawData;
class GrblSettingsModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Can be only created in C++")

    Q_PROPERTY(bool hasChanges READ hasChanges NOTIFY hasChangesChanged FINAL)
public:
    enum class SettingType : uint8_t
    {
        Bool,
        Int,
        Float,
        Mask
    };
    Q_ENUM(SettingType)

    enum class SerializeMode : uint8_t
    {
        All, // If the value is changed serializes it, otherwise serializes saved value
        Saved, // Ignores changes and serializes only saved values
        Changed // Serializes only changed values, the unchanged values are ignored
    };

    GrblSettingsModel();
    ~GrblSettingsModel() override;

    Q_DISABLE_COPY_MOVE(GrblSettingsModel)

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QList<QByteArray> serialize(SerializeMode mode = SerializeMode::All) const;

    bool parseItemData(const QByteArray &data);
    void clear();

    Q_INVOKABLE void resetChanges();
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool hasChanges() const;

signals:
    void hasChangesChanged();

private:
    SettingsRawData *m_data;
};

class GrblSettingsSortingModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Can be only created in C++")

public:
    GrblSettingsSortingModel(QObject* parent = nullptr);

protected:
    bool lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const override;
};
}
