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

#include "./group.hpp"
#include "models/project.hpp"
#include "models/api.hpp"

namespace Model
{
    Group::Group(const QString &id, const QString &name, const Apis &apis)
        : id{id}, name{name}, apis{apis}
    {
        for (auto &&api : apis)
        {
            api->setGroup(this);
        }
    }

    Group::~Group()
    {
    }

    QString Group::getId() const
    {
        return id;
    }

    QString Group::getName() const
    {
        return name;
    }

    void Group::setName(const QString &name)
    {
        this->name = name;
    }

    Project *Group::getProject() const
    {
        return project;
    }

    void Group::setProject(Project *project)
    {
        this->project = project;
    }

    Apis Group::getApis() const
    {
        return apis;
    }

    void Group::add(Api *api)
    {
        apis.push_back(api);
        api->setGroup(this);
    }

    void Group::remove(Api *api)
    {
        apis.remove(api);
        delete api;
    }
} // namespace Model
