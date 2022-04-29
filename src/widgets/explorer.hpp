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

#include <QWidget>
#include <list>
#include <tuple>

QT_BEGIN_NAMESPACE
class QMenu;
class QAction;
class QTreeView;
class QStackedWidget;
class QComboBox;
QT_END_NAMESPACE

namespace Model
{
    class Project;
    class Group;
    class Api;
} // namespace Model

namespace QModel
{
    class Api;
} // namespace QModel

namespace Persistence
{
    class Project;
} // namespace Persistence

namespace Event
{
    class Bus;
} // namespace Event

namespace Ui
{
    class Explorer : public QWidget
    {
        Q_OBJECT

    public:
        explicit Explorer(QWidget *parent = nullptr);
        ~Explorer() = default;

        void addProject(Model::Project *project);
        void setCurrentApi(Model::Api *api) const;

    signals:
        void requestOpen(Model::Api *api) const;

    private slots:
        void handleComboBoxCurrentIndexChanged(int index) const;
        void handleTreeCurrentChanged(const QModelIndex &current, const QModelIndex &previous) const;
        void handleNewGroupTriggered();
        void handleNewApiTriggered();
        void handleRenameGroupTriggered();
        void handleDeleteGroupTriggered();
        void handleRenameApiTriggered();
        void handleDeleteApiTriggered();

    private:
        void handleTreeContextMenu(QTreeView *tree, const QPoint &pos) const;
        std::tuple<QModel::Api *, Model::Group *> getApiAndGroup(QObject *object) const;
        std::tuple<QModel::Api *, Model::Api *> getApiAndApi(QObject *object) const;

    private:
        QComboBox *projectSelector;
        std::map<Model::Project *, QTreeView *> projects;
        QStackedWidget *projectStacks;
        QMenu *projectMenu;
        QAction *newGroupAction;
        QMenu *groupMenu;
        QAction *newApiAction;
        QAction *renameGroupAction;
        QAction *deleteGroupAction;
        QMenu *apiMenu;
        QAction *renameApiAction;
        QAction *deleteApiAction;
        Event::Bus *bus;
        Persistence::Project *projectPersistence;
    };
} // namespace Ui
