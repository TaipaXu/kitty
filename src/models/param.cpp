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

#include "./param.hpp"

namespace Model
{
    Param::Param()
        : enabled{true}
    {
    }

    Param::Param(const QString &key, const QString &value, const QString &description, bool enabled)
        : key{key},
          value{value},
          description{description},
          enabled{enabled}
    {
    }

    QString Param::getKey() const
    {
        return key;
    }

    void Param::setKey(const QString &key)
    {
        this->key = key;
    }

    QString Param::getValue() const
    {
        return value;
    }

    void Param::setValue(const QString &value)
    {
        this->value = value;
    }

    QString Param::getDescription() const
    {
        return description;
    }

    void Param::setDescription(const QString &description)
    {
        this->description = description;
    }

    bool Param::getEnabled() const
    {
        return enabled;
    }

    void Param::setEnabled(bool enabled)
    {
        this->enabled = enabled;
    }
} // namespace Model
