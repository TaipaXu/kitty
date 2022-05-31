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
#include <QMimeData>
#include "models/project.hpp"
#include "models/group.hpp"
#include "models/api.hpp"

namespace QModel
{
    const QString Api::groupMimeType("application/kitty-group");
    const QString Api::apiMimeType("application/kitty-api");

    Api::Api(Model::Project *project, QObject *parent)
        : QAbstractItemModel(parent),
          project{project}
    {
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
            parentItem = getItem(parent);
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
            parentItem = getItem(parent);
        }

        auto childItem = parentItem->children.begin();
        std::advance(childItem, row);
        return createIndex(row, column, *childItem);
    }

    QModelIndex Api::parent(const QModelIndex &index) const
    {
        if (!index.isValid())
        {
            return QModelIndex();
        }

        QModel::ApiItem *childItem = getItem(index);
        QModel::ApiItem *parentItem = childItem->parent;
        if (parentItem == nullptr)
        {
            return QModelIndex();
        }
        else
        {
            auto it = std::find(parentItem->children.begin(), parentItem->children.end(), childItem);
            const int row = std::distance(parentItem->children.begin(), it);
            return createIndex(row, 0, parentItem);
        }
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

        QModel::ApiItem *item = getItem(index);
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

    Model::Group *Api::getGroup(const QModelIndex &index) const
    {
        QModel::ApiItem *item = getItem(index);
        std::list<Model::Group *> groups = project->getGroups();
        auto it = std::find_if(groups.begin(), groups.end(), [item](Model::Group *group) {
            return group->getId() == item->id;
        });
        return it == groups.end() ? nullptr : *it;
    }

    Model::Api *Api::getApi(const QModelIndex &index) const
    {
        QModel::ApiItem *item = getItem(index);
        std::list<Model::Group *> groups = project->getGroups();
        for (auto &&group : groups)
        {
            std::list<Model::Api *> apis = group->getApis();
            auto it = std::find_if(apis.begin(), apis.end(), [item](Model::Api *api) {
                return api->getId() == item->id;
            });
            if (it != apis.end())
            {
                return *it;
            }
        }
        return nullptr;

        // QModel::ApiItem *item = getItem(index);
        // for (auto &&group : project->getGroups())
        // {
        //     for (auto &&api : group->getApis())
        //     {
        //         if (item->id == api->getId())
        //         {
        //             return api;
        //         }
        //     }
        // }
    }

    QModelIndex Api::getIndex(Model::Group *group)
    {
        auto it = std::find_if(item->children.begin(), item->children.end(), [group](QModel::ApiItem *item) {
            return item->id == group->getId();
        });
        return it == item->children.end() ? QModelIndex() : index(std::distance(item->children.begin(), it), 0);
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

    Qt::ItemFlags Api::flags(const QModelIndex &index) const
    {
        if (!index.isValid())
        {
            return Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
        }

        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
    }

    QStringList Api::mimeTypes() const
    {
        return {groupMimeType, apiMimeType};
    }

    QMimeData *Api::mimeData(const QModelIndexList &indexes) const
    {
        if (indexes.size() < 1)
        {
            return nullptr;
        }

        QMimeData *mimeData = new QMimeData();
        const QModelIndex index = indexes.at(0);
        const QModel::ApiItem *item = getItem(index);
        if (item->type == QString("group"))
        {
            Model::Group *group = getGroup(index);
            mimeData->setData(groupMimeType, group->getId().toUtf8());
        }
        else if (item->type == QString("api"))
        {
            Model::Api *api = getApi(index);
            mimeData->setData(apiMimeType, api->getId().toUtf8());
        }

        return mimeData;
    }

    Qt::DropActions Api::supportedDragActions() const
    {
        return Qt::MoveAction;
    }

    Qt::DropActions Api::supportedDropActions() const
    {
        return Qt::MoveAction;
    }

    bool Api::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
    {
        if (!data)
        {
            return false;
        }

        if (data->hasFormat(groupMimeType))
        {
            if (row == parent.row() && !parent.parent().isValid())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (data->hasFormat(apiMimeType))
        {
            if (!index(row, column, parent).isValid() && parent.isValid() || row == parent.row() && parent.isValid())
            {
                QModel::ApiItem *item = getItem(parent);
                if (item->type == "group")
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    bool Api::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
    {
        if (!data)
        {
            return false;
        }

        if (data->hasFormat(groupMimeType))
        {
            if (row == parent.row())
            {
                const QString fromGroupId = data->data(groupMimeType);
                int fromIndex = 0;
                std::list<ApiItem *> &groups = item->children;
                auto fromIt = std::find_if(groups.begin(), groups.end(), [fromGroupId](ApiItem *item) {
                    return item->id == fromGroupId;
                });
                fromIndex = std::distance(groups.begin(), fromIt);
                if (fromIndex == row - 1)
                {
                    return false;
                }
                auto toIt = groups.begin();
                std::advance(toIt, row);
                beginMoveRows(QModelIndex(), fromIndex, fromIndex, QModelIndex(), row);
                groups.splice(toIt, groups, fromIt);
                endMoveRows();

                return true;
            }
            else
            {
                return false;
            }
        }
        else if (data->hasFormat(apiMimeType))
        {
            if (parent.isValid() && (!index(row, column, parent).isValid() || row == parent.row()))
            {
                QModel::ApiItem *item = getItem(parent);
                if (item->type == "group")
                {
                    const QString fromApiId = data->data(apiMimeType);
                    int fromGroupIndex = 0;
                    int fromApiIndex = 0;
                    for (auto &&group : this->item->children)
                    {
                        bool found = false;
                        fromApiIndex = 0;
                        for (auto &&api : group->children)
                        {
                            if (api->id == fromApiId)
                            {
                                found = true;
                                break;
                            }
                            fromApiIndex++;
                        }
                        if (found)
                        {
                            break;
                        }
                        fromGroupIndex++;
                    }
                    auto itGroupFrom = this->item->children.begin();
                    std::advance(itGroupFrom, fromGroupIndex);
                    auto itApiFrom = (*itGroupFrom)->children.begin();
                    std::advance(itApiFrom, fromApiIndex);
                    const int toGroupIndex = parent.parent().row();
                    const int toApiIndex = row;
                    auto itGroupTo = this->item->children.begin();
                    std::advance(itGroupTo, toGroupIndex);
                    auto itApiTo = (*itGroupTo)->children.begin();
                    std::advance(itApiTo, toApiIndex);

                    beginMoveRows(index(fromGroupIndex, 0, QModelIndex()), fromApiIndex, fromApiIndex, index(parent.parent().row(), 0, QModelIndex()), toApiIndex);
                    (*itGroupTo)->children.splice((*itGroupTo)->children.end(), (*itGroupFrom)->children, itApiFrom);
                    (*itApiFrom)->parent = *itGroupTo;
                    endMoveRows();
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
} // namespace QModel
