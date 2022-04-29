/*
 * This file is part of the Kitty (https://github.com/TaipaXu/kitty).
 * Copyright (c) 2022 Taipa Xu.
 *
 * Kitty is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Kitty is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Kitty. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <QString>

namespace Model
{
    class FormData
    {
    public:
        enum class Type
        {
            Text,
            File
        };

        FormData();
        FormData(Type type, const QString &key, const QString &value, const QString &description, bool enabled);
        ~FormData() = default;

        Type getType() const;
        void setType(Type type);
        QString getKey() const;
        void setKey(const QString &key);
        QString getValue() const;
        void setValue(const QString &value);
        QString getDescription() const;
        void setDescription(const QString &description);
        bool getEnabled() const;
        void setEnabled(bool enabled);

    private:
        Type type;
        QString key;
        QString value;
        QString description;
        bool enabled;
    };

    FormData::Type getFormDataType(const QString &type);

    QString getFormDataTypeName(FormData::Type type);
} // namespace Model
