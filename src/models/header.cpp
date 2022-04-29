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

#include "./header.hpp"

namespace Model
{
    Header::Header()
        : enabled{true}
    {
    }

    Header::Header(const QString &key, const QString &value, const QString &description, bool enabled)
        : key{key},
          value{value},
          description{description},
          enabled{enabled}
    {
    }

    QString Header::getKey() const
    {
        return key;
    }

    void Header::setKey(const QString &key)
    {
        this->key = key;
    }

    QString Header::getValue() const
    {
        return value;
    }

    void Header::setValue(const QString &value)
    {
        this->value = value;
    }

    QString Header::getDescription() const
    {
        return description;
    }

    void Header::setDescription(const QString &description)
    {
        this->description = description;
    }

    bool Header::getEnabled() const
    {
        return enabled;
    }

    void Header::setEnabled(bool enabled)
    {
        this->enabled = enabled;
    }
} // namespace Model
