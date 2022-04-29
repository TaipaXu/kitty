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
    class Group;

    using Groups = std::list<Group *>;

    class Project
    {
    public:
        Project(const QString &id, const QString &name, const QString &description);
        ~Project();

        QString getId() const;
        QString getName() const;
        void setName(const QString &name);
        QString getDescription() const;
        void setDescription(const QString &description);
        void edit(const QString &name, const QString &description);
        Groups getGroups() const;
        void setGroups(Groups groups);
        void addGroup(Group *group);
        void removeGroup(Group *group);

    private:
        QString id;
        QString name;
        QString description;
        Groups groups;
    };
} // namespace Model
