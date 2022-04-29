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

#include "./item.hpp"
#include "models/project.hpp"
#include "models/group.hpp"
#include "models/api.hpp"

namespace QModel
{
    ApiItem::ApiItem(const QString &name, const QString &id, ApiItem *parent)
        : name(name),
          id(id),
          parent{parent}
    {
        if (parent)
        {
            parent->children.push_back(this);
            if (parent->name == QStringLiteral("root"))
            {
                type = QStringLiteral("group");
            }
            else
            {
                type = QStringLiteral("api");
            }
        }
    }

    ApiItem::~ApiItem()
    {
        for (auto &&i : children)
        {
            delete i;
            i = nullptr;
        }
    }

    void ApiItem::removeItem(const QString &id)
    {
        for (auto it = children.begin(); it != children.end(); ++it)
        {
            if ((*it)->id == id)
            {
                delete *it;
                children.erase(it);
                return;
            }
        }
    }

    void ApiItem::create(Model::Group *group)
    {
        if (id == "root")
        {
            new ApiItem(group->getName(), group->getId(), this);
        }
    }

    void ApiItem::rename(Model::Group *group, const QString &name)
    {
        if (id == "root")
        {
            for (auto it = children.begin(); it != children.end(); ++it)
            {
                if ((*it)->id == group->getId())
                {
                    (*it)->name = name;
                    return;
                }
            }
        }
    }

    void ApiItem::remove(Model::Group *group)
    {
        if (id == "root")
        {
            for (auto it = children.begin(); it != children.end(); ++it)
            {
                if ((*it)->id == group->getId())
                {
                    children.erase(it);
                    return;
                }
            }
        }
    }

    void ApiItem::create(Model::Api *api)
    {
        const QString groupId = api->getGroup()->getId();
        const QString name = api->getName();
        if (id == "root")
        {
            for (auto it = children.begin(); it != children.end(); ++it)
            {
                if (groupId == (*it)->id)
                {
                    ApiItem *apiItem = new ApiItem(name, api->getId(), *it);
                }
            }
        }
    }

    void ApiItem::rename(Model::Api *api, const QString &name)
    {
        if (id == "root")
        {
            for (auto it = children.begin(); it != children.end(); ++it)
            {
                for (auto it2 = (*it)->children.begin(); it2 != (*it)->children.end(); ++it2)
                {
                    if ((*it)->id == api->getGroup()->getId() && (*it2)->id == api->getId())
                    {
                        (*it2)->name = name;
                        return;
                    }
                }
            }
        }
    }

    void ApiItem::removeApi(const QString &groupId, const QString &apiId)
    {
        if (id == "root")
        {
            for (auto it = children.begin(); it != children.end(); ++it)
            {
                for (auto it2 = (*it)->children.begin(); it2 != (*it)->children.end(); ++it2)
                {
                    if ((*it)->id == groupId && (*it2)->id == apiId)
                    {
                        (*it)->children.erase(it2);
                        return;
                    }
                }
            }
        }
    }
} // namespace QModel
