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

#include "./raw.hpp"

namespace Model
{
    Raw::Raw()
        : type{Type::Text}
    {
    }

    Raw::Raw(Type type, const QString &data)
        : type{type}, data{data}
    {
    }

    Raw::Type Raw::getType() const
    {
        return type;
    }

    void Raw::setType(Type type)
    {
        this->type = type;
    }

    QString Raw::getData() const
    {
        return data;
    }

    void Raw::setData(const QString &data)
    {
        this->data = data;
    }

    Raw::Type getRawType(const QString &type)
    {
        if (type == "text")
        {
            return Raw::Type::Text;
        }
        else if (type == "javascript")
        {
            return Raw::Type::JavaScript;
        }
        else if (type == "json")
        {
            return Raw::Type::Json;
        }
        else if (type == "html")
        {
            return Raw::Type::Html;
        }
        else if (type == "xml")
        {
            return Raw::Type::Xml;
        }
    }

    QString getRawTypeName(Raw::Type type, bool upper)
    {
        switch (type)
        {
        case Raw::Type::Text:
        {
            return upper ? QStringLiteral("Text") : QStringLiteral("text");
        }

        case Raw::Type::JavaScript:
        {
            return upper ? QStringLiteral("JavaScript") : QStringLiteral("javascript");
        }

        case Raw::Type::Json:
        {
            return upper ? QStringLiteral("Json") : QStringLiteral("json");
        }

        case Raw::Type::Html:
        {
            return upper ? QStringLiteral("Html") : QStringLiteral("html");
        }

        case Raw::Type::Xml:
        {
            return upper ? QStringLiteral("Xml") : QStringLiteral("xml");
        }

        default:
            break;
        }
    }

    QStringList getRawTypes()
    {
        return {"Text", "JavaScript", "Json", "Html", "Xml"};
    }
} // namespace Model
