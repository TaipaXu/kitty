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

#include "./project.hpp"
#include "group.hpp"

namespace Model
{
    Project::Project(const QString &id, const QString &name, const QString &description)
        : id{id}, name{name}, description{description}
    {
    }

    Project::~Project()
    {
    }

    QString Project::getId() const
    {
        return id;
    }

    QString Project::getName() const
    {
        return name;
    }

    void Project::setName(const QString &name)
    {
        this->name = name;
    }

    QString Project::getDescription() const
    {
        return description;
    }

    void Project::setDescription(const QString &description)
    {
        this->description = description;
    }

    void Project::edit(const QString &name, const QString &description)
    {
        this->name = name;
        this->description = description;
    }

    Groups Project::getGroups() const
    {
        return groups;
    }

    void Project::setGroups(Groups groups)
    {
        for (auto &&group : groups)
        {
            group->setProject(this);
        }

        this->groups = groups;
    }

    void Project::addGroup(Group *group)
    {
        group->setProject(this);
        groups.push_back(group);
    }

    void Project::removeGroup(Group *group)
    {
        delete group;
        groups.remove(group);
    }
} // namespace Model
