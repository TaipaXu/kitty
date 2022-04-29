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

#include "./api.hpp"
#include "models/project.hpp"
#include "models/group.hpp"
#include "models/api.hpp"

namespace QModel
{
    Api::Api(QObject *parent)
        : QAbstractItemModel(parent),
          item{new QModel::ApiItem("root", "root")},
          project{nullptr}
    {
    }

    Api::~Api()
    {
        delete item;
        item = nullptr;
    }

    int Api::rowCount(const QModelIndex &parent) const
    {
        if (parent.column() > 0)
        {
            return 0;
        }

        QModel::ApiItem *parentItem;
        if (!parent.isValid())
        {
            parentItem = item;
        }
        else
        {
            parentItem = static_cast<QModel::ApiItem *>(parent.internalPointer());
        }

        return parentItem->children.size();
    }

    int Api::columnCount(const QModelIndex &parent) const
    {
        return 1;
    }

    QModelIndex Api::index(int row, int column, const QModelIndex &parent) const
    {
        if (!hasIndex(row, column, parent))
        {
            return QModelIndex();
        }

        QModel::ApiItem *parentItem = nullptr;
        if (!parent.isValid())
        {
            parentItem = item;
        }
        else
        {
            parentItem = static_cast<QModel::ApiItem *>(parent.internalPointer());
        }

        QModel::ApiItem *childItem = parentItem->children.at(row);
        if (childItem)
        {
            return createIndex(row, column, childItem);
        }
        else
        {
            return QModelIndex();
        }
    }

    QModelIndex Api::parent(const QModelIndex &index) const
    {
        if (!index.isValid())
        {
            return QModelIndex();
        }

        QModel::ApiItem *childItem = static_cast<QModel::ApiItem *>(index.internalPointer());
        QModel::ApiItem *parentItem = childItem->parent;
        if (parentItem == item)
        {
            return QModelIndex();
        }

        int row = 0;
        if (parentItem->parent)
        {
            auto it = std::find(parentItem->parent->children.begin(), parentItem->parent->children.end(), parentItem);
            row = std::distance(parentItem->parent->children.begin(), it);
        }

        return createIndex(row, 0, parentItem);
    }

    QVariant Api::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
        {
            return QVariant();
        }

        if (role != Qt::DisplayRole)
        {
            return QVariant();
        }

        QModel::ApiItem *item = static_cast<QModel::ApiItem *>(index.internalPointer());
        return item->name;
    }

    QModel::ApiItem *Api::getItem(const QModelIndex &index) const
    {
        return static_cast<QModel::ApiItem *>(index.internalPointer());
    }

    Model::Project *Api::getProject() const
    {
        return project;
    }

    void Api::setProject(Model::Project *project)
    {
        this->project = project;
        beginResetModel();
        item = new QModel::ApiItem(QString("root"), QString("root"));
        for (auto &&group : project->getGroups())
        {
            QModel::ApiItem *groupItem = new QModel::ApiItem(group->getName(), group->getId(), item);
            for (auto &&api : group->getApis())
            {
                QModel::ApiItem *apiItem = new QModel::ApiItem(api->getName(), api->getId(), groupItem);
            }
        }
        endResetModel();
    }

    Model::Group *Api::getGroup(const QModelIndex &index) const
    {
        QModel::ApiItem *item = static_cast<QModel::ApiItem *>(index.internalPointer());
        for (auto &&group : project->getGroups())
        {
            if (item->id == group->getId())
            {
                return group;
            }
        }
    }

    Model::Api *Api::getApi(const QModelIndex &index) const
    {
        QModel::ApiItem *item = static_cast<QModel::ApiItem *>(index.internalPointer());
        for (auto &&group : project->getGroups())
        {
            for (auto &&api : group->getApis())
            {
                if (item->id == api->getId())
                {
                    return api;
                }
            }
        }
    }

    QModelIndex Api::getIndex(Model::Group *group)
    {
        int groupIndex = 0;
        for (auto &&group2 : project->getGroups())
        {
            if (group = group2)
            {
                return index(groupIndex, 0);
            }
            groupIndex++;
        }

        return QModelIndex();
    }

    QModelIndex Api::getIndex(Model::Api *api)
    {
        int groupIndex = 0;
        for (auto &&group : project->getGroups())
        {
            int apiIndex = 0;
            for (auto &&apiItem : group->getApis())
            {
                if (api == apiItem)
                {
                    return index(apiIndex, 0, index(groupIndex, 0));
                }

                apiIndex++;
            }
            groupIndex++;
        }

        return QModelIndex();
    }

    void Api::create(Model::Group *group)
    {
        QModelIndex index = getIndex(group);
        beginInsertRows(QModelIndex(), index.row(), index.row());
        item->create(group);
        endInsertRows();
    }

    void Api::add(Model::Group *group)
    {
        QModelIndex index = getIndex(group);
        beginInsertRows(QModelIndex(), index.row(), index.row());
        item->create(group);
        endInsertRows();
    }

    void Api::remove(Model::Group *group)
    {
        QModelIndex index = getIndex(group);
        beginRemoveRows(QModelIndex(), index.row(), index.row());
        item->remove(group);
        endRemoveRows();
    }

    void Api::rename(Model::Group *group, const QString &name)
    {
        item->rename(group, name);
    }

    void Api::create(Model::Api *api)
    {
        QModelIndex index = getIndex(api->getGroup());
        beginInsertRows(QModelIndex(), index.row(), index.row());
        item->create(api);
        endInsertRows();
    }

    void Api::remove(Model::Api *api)
    {
        QModelIndex index = getIndex(api);
        beginRemoveRows(QModelIndex(), index.row(), index.row());
        item->removeApi(api->getGroup()->getId(), api->getId());
        endRemoveRows();
    }

    void Api::rename(Model::Api *api, const QString &name)
    {
        item->rename(api, name);
    }
} // namespace QModel
