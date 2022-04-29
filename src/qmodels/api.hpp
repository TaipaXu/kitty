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

#include <QAbstractItemModel>
#include "item.hpp"

QT_BEGIN_NAMESPACE
class QDropEvent;
QT_END_NAMESPACE

namespace Model
{
    class Project;
    class Group;
    class Api;
} // namespace Model

namespace QModel
{
    class Api : public QAbstractItemModel
    {
        Q_OBJECT

    public:
        explicit Api(QObject *parent = nullptr);
        ~Api();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        QModelIndex parent(const QModelIndex &index) const override;
        QVariant data(const QModelIndex &index, int role) const override;

        QModel::ApiItem *getItem(const QModelIndex &index) const;
        Model::Project *getProject() const;
        void setProject(Model::Project *project);
        Model::Group *getGroup(const QModelIndex &index) const;
        Model::Api *getApi(const QModelIndex &index) const;
        QModelIndex getIndex(Model::Group *group);
        QModelIndex getIndex(Model::Api *api);
        void create(Model::Group *group);
        void add(Model::Group *group);
        void remove(Model::Group *group);
        void rename(Model::Group *group, const QString &name);
        void create(Model::Api *api);
        void rename(Model::Api *api, const QString &name);
        void remove(Model::Api *api);

    private:
        QModel::ApiItem *item;
        Model::Project *project;
    };
} // namespace QModel
