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
#include <list>

namespace Model
{
    class FormData;
    class XWwwFormUrlEncoded;
    class Raw;
    class Binary;

    class Body
    {
    public:
        enum class Type
        {
            None,
            FormData,
            XWwwFormUrlEncoded,
            Raw,
            Binary
        };

    public:
        Body(Type type);
        ~Body();

        Type getType();
        void setData(std::list<FormData *> formDatas);
        void setData(std::list<XWwwFormUrlEncoded *> xWwwFormUrlEncodeds);
        void setData(Raw *raw);
        void setData(Binary *binary);
        void removeData();
        std::list<FormData *> getFormDatas() const;
        std::list<XWwwFormUrlEncoded *> getXWwwFormUrlEncodeds() const;
        Raw *getRaw() const;
        Binary *getBinary() const;

    private:
        Type type;
        std::list<FormData *> formDatas;
        std::list<XWwwFormUrlEncoded *> xWwwFormUrlEncodeds;
        Raw *raw;
        Binary *binary;
    };

    Body::Type getBodyType(const QString type);

    QString getBodyTypeName(Body::Type type);
} // namespace Model
