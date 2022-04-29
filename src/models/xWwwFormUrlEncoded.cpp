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

#include "./xWwwFormUrlEncoded.hpp"

namespace Model
{
    XWwwFormUrlEncoded::XWwwFormUrlEncoded()
        : enabled{true}
    {
    }

    XWwwFormUrlEncoded::XWwwFormUrlEncoded(const QString &key, const QString &value, const QString &description, bool enabled)
        : key{key},
          value{value},
          description{description},
          enabled{enabled}
    {
    }

    QString XWwwFormUrlEncoded::getKey() const
    {
        return key;
    }

    void XWwwFormUrlEncoded::setKey(const QString &key)
    {
        this->key = key;
    }

    QString XWwwFormUrlEncoded::getValue() const
    {
        return value;
    }

    void XWwwFormUrlEncoded::setValue(const QString &value)
    {
        this->value = value;
    }

    QString XWwwFormUrlEncoded::getDescription() const
    {
        return description;
    }

    void XWwwFormUrlEncoded::setDescription(const QString &description)
    {
        this->description = description;
    }

    bool XWwwFormUrlEncoded::getEnabled() const
    {
        return enabled;
    }

    void XWwwFormUrlEncoded::setEnabled(bool enabled)
    {
        this->enabled = enabled;
    }
} // namespace Model
