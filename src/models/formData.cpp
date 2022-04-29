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

#include "./formData.hpp"

namespace Model
{
    FormData::FormData()
        : enabled{true}
    {
        type = Type::Text;
    }

    FormData::FormData(Type type, const QString &key, const QString &value, const QString &description, bool enabled)
        : type{type}, key{key}, value{value}, description{description}, enabled{enabled}
    {
    }

    FormData::Type FormData::getType() const
    {
        return type;
    }

    void FormData::setType(FormData::Type type)
    {
        this->type = type;
    }

    QString FormData::getKey() const
    {
        return key;
    }

    void FormData::setKey(const QString &key)
    {
        this->key = key;
    }

    QString FormData::getValue() const
    {
        return value;
    }

    void FormData::setValue(const QString &value)
    {
        this->value = value;
    }

    QString FormData::getDescription() const
    {
        return description;
    }

    void FormData::setDescription(const QString &description)
    {
        this->description = description;
    }

    bool FormData::getEnabled() const
    {
        return enabled;
    }

    void FormData::setEnabled(bool enabled)
    {
        this->enabled = enabled;
    }

    FormData::Type getFormDataType(const QString &type)
    {
        if (type == "text")
        {
            return FormData::Type::Text;
        }
        else if (type == "file")
        {
            return FormData::Type::File;
        }
    }

    QString getFormDataTypeName(FormData::Type type)
    {
        switch (type)
        {
        case FormData::Type::Text:
        {
            return QStringLiteral("text");
        }

        case FormData::Type::File:
        {
            return QStringLiteral("file");
        }

        default:
            break;
        }
    }
} // namespace Model
