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

#include "./body.hpp"
#include "models/formData.hpp"
#include "models/xWwwFormUrlEncoded.hpp"
#include "models/raw.hpp"
#include "models/binary.hpp"

namespace Model
{
    Body::Body(Type type)
        : type{type}, raw{nullptr}, binary{nullptr}
    {
    }

    Body::~Body()
    {
        switch (type)
        {
        case Type::None:
        {
            break;
        }

        case Type::FormData:
        {
            for (auto &&i : formDatas)
            {
                delete i;
            }
            break;
        }

        case Type::XWwwFormUrlEncoded:
        {
            for (auto &&i : xWwwFormUrlEncodeds)
            {
                delete i;
            }
            break;
        }

        case Type::Raw:
        {
            delete raw;
            break;
        }

        case Type::Binary:
        {
            delete binary;
            break;
        }

        default:
        {
            break;
        }
        }
    }

    Body::Type Body::getType()
    {
        return type;
    }

    void Body::setData(std::list<FormData *> formDatas)
    {
        type = Type::FormData;
        this->formDatas = formDatas;
        xWwwFormUrlEncodeds = {};
        raw = nullptr;
        binary = nullptr;
    }

    void Body::setData(std::list<XWwwFormUrlEncoded *> xWwwFormUrlEncodeds)
    {
        type = Type::XWwwFormUrlEncoded;
        formDatas = {};
        this->xWwwFormUrlEncodeds = xWwwFormUrlEncodeds;
        raw = nullptr;
        binary = nullptr;
    }

    void Body::setData(Raw *raw)
    {
        type = Type::Raw;
        formDatas = {};
        xWwwFormUrlEncodeds = {};
        this->raw = raw;
        binary = nullptr;
    }

    void Body::setData(Binary *binary)
    {
        type = Type::Binary;
        formDatas = {};
        xWwwFormUrlEncodeds = {};
        raw = nullptr;
        this->binary = binary;
    }

    void Body::removeData()
    {
        type = Type::None;
        formDatas = {};
        xWwwFormUrlEncodeds = {};
        raw = nullptr;
        binary = nullptr;
    }

    std::list<FormData *> Body::getFormDatas() const
    {
        return formDatas;
    }

    std::list<XWwwFormUrlEncoded *> Body::getXWwwFormUrlEncodeds() const
    {
        return xWwwFormUrlEncodeds;
    }

    Raw *Body::getRaw() const
    {
        return raw;
    }

    Binary *Body::getBinary() const
    {
        return binary;
    }

    Body::Type getBodyType(const QString type)
    {
        if (type == "none")
        {
            return Body::Type::None;
        }
        else if (type == "form_data")
        {
            return Body::Type::FormData;
        }
        else if (type == "x_www_form_urlencoded")
        {
            return Body::Type::XWwwFormUrlEncoded;
        }
        else if (type == "raw")
        {
            return Body::Type::Raw;
        }
        else if (type == "binary")
        {
            return Body::Type::Binary;
        }
    }

    QString getBodyTypeName(Body::Type type)
    {
        switch (type)
        {
        case Body::Type::None:
        {
            return QStringLiteral("none");
        }

        case Body::Type::FormData:
        {
            return QStringLiteral("form_data");
        }

        case Body::Type::XWwwFormUrlEncoded:
        {
            return QStringLiteral("x_www_form_urlencoded");
        }

        case Body::Type::Raw:
        {
            return QStringLiteral("raw");
        }

        case Body::Type::Binary:
        {
            return QStringLiteral("binary");
        }

        default:
        {
        }
        }
    }
} // namespace Model
