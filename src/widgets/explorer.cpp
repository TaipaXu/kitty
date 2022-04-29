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

#include "./explorer.hpp"
#include <QVBoxLayout>
#include <QTreeView>
#include <QStackedWidget>
#include <QComboBox>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>
#include "widgets/groupDialog.hpp"
#include "models/project.hpp"
#include "models/group.hpp"
#include "models/api.hpp"
#include "qmodels/api.hpp"
#include "persistence/project.hpp"
#include "events/bus.hpp"
#include "utils/utils.hpp"

namespace Ui
{
    Explorer::Explorer(QWidget *parent)
        : QWidget(parent)
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setAlignment(Qt::AlignTop);

        projectSelector = new QComboBox(this);
        connect(projectSelector, &QComboBox::currentIndexChanged, this, &Explorer::handleComboBoxCurrentIndexChanged);
        mainLayout->addWidget(projectSelector);

        projectStacks = new QStackedWidget(this);
        mainLayout->addWidget(projectStacks);

        setLayout(mainLayout);

        projectMenu = new QMenu(this);
        newGroupAction = new QAction(tr("New Group"), this);
        connect(newGroupAction, &QAction::triggered, this, &Explorer::handleNewGroupTriggered);
        projectMenu->addAction(newGroupAction);

        groupMenu = new QMenu(this);
        newApiAction = new QAction(tr("New API"), this);
        connect(newApiAction, &QAction::triggered, this, &Explorer::handleNewApiTriggered);
        groupMenu->addAction(newApiAction);
        renameGroupAction = new QAction(tr("Rename Group"), this);
        connect(renameGroupAction, &QAction::triggered, this, &Explorer::handleRenameGroupTriggered);
        groupMenu->addAction(renameGroupAction);
        deleteGroupAction = new QAction(tr("Delete Group"), this);
        connect(deleteGroupAction, &QAction::triggered, this, &Explorer::handleDeleteGroupTriggered);
        groupMenu->addAction(deleteGroupAction);

        apiMenu = new QMenu(this);
        renameApiAction = new QAction(tr("Rename API"), this);
        connect(renameApiAction, &QAction::triggered, this, &Explorer::handleRenameApiTriggered);
        apiMenu->addAction(renameApiAction);
        deleteApiAction = new QAction(tr("Delete API"), this);
        connect(deleteApiAction, &QAction::triggered, this, &Explorer::handleDeleteApiTriggered);
        apiMenu->addAction(deleteApiAction);

        bus = Event::Bus::getInstance();

        projectPersistence = Persistence::Project::getInstance();
    }

    void Explorer::addProject(Model::Project *project)
    {
        if (projects.contains(project))
        {
            auto it = projects.find(project);
            int index = std::distance(projects.begin(), it);
            projectSelector->setCurrentIndex(index);
        }
        else
        {
            projectPersistence->load(project);

            QTreeView *tree = new QTreeView(this);
            tree->setHeaderHidden(true);
            tree->setContextMenuPolicy(Qt::CustomContextMenu);
            tree->setSelectionBehavior(QAbstractItemView::SelectRows);
            tree->setSelectionMode(QAbstractItemView::SingleSelection);
            connect(tree, &QTreeView::customContextMenuRequested, [tree, this](const QPoint &pos) {
                handleTreeContextMenu(tree, pos);
            });

            QModel::Api *qmodel = new QModel::Api(this);
            qmodel->setProject(project);
            tree->setModel(qmodel);
            connect(tree->selectionModel(), &QItemSelectionModel::currentChanged, this, &Explorer::handleTreeCurrentChanged);

            projects[project] = tree;
            projectSelector->addItem(project->getName());
            projectSelector->setCurrentIndex(projectSelector->count() - 1);
            projectStacks->addWidget(tree);
        }

        projectStacks->setCurrentWidget(projects[project]);
    }

    void Explorer::setCurrentApi(Model::Api *api) const
    {
        for (auto &&[key, value] : projects)
        {
            value->clearSelection();
        }
        if (api == nullptr)
        {
            return;
        }
        else
        {
            Model::Project *project = api->getProject();
            for (auto &&[key, value] : projects)
            {
                if (key == project)
                {
                    projectSelector->setCurrentIndex(std::distance(projects.begin(), projects.find(api->getProject())));
                    projectStacks->setCurrentWidget(value);
                    QModel::Api *qmodel = static_cast<QModel::Api *>(value->model());
                    auto index = qmodel->getIndex(api);
                    value->setCurrentIndex(index);
                    break;
                }
            }
        }
    }

    void Explorer::handleComboBoxCurrentIndexChanged(int index) const
    {
        projectStacks->setCurrentIndex(index);
    }

    void Explorer::handleTreeCurrentChanged(const QModelIndex &current, const QModelIndex &previous) const
    {
        QModel::Api *qmodel = static_cast<QModel::Api *>(static_cast<QTreeView *>(projectStacks->currentWidget())->model());
        QModel::ApiItem *item = qmodel->getItem(current);
        QString type = item->type;
        QString name = item->name;
        QString id = item->id;
        // TODO enum
        if (item->type == "api")
        {
            Model::Api *api = qmodel->getApi(current);
            emit requestOpen(api);
        }
    }

    void Explorer::handleNewGroupTriggered()
    {
        bool ok;
        QString name = QInputDialog::getText(this, tr("Create Group"), tr("Group Name"), QLineEdit::Normal, "Group Name", &ok);
        if (ok && !name.isEmpty())
        {
            QAction *action = qobject_cast<QAction *>(sender());
            QTreeView *tree = static_cast<QTreeView *>(projectStacks->currentWidget());
            // QTreeView *tree = Utils::stringToPointer<QTreeView>(action->data().toString());
            QModel::Api *qmodel = static_cast<QModel::Api *>(tree->model());
            Model::Project *project = qmodel->getProject();
            Model::Group *group = projectPersistence->createGroup(project, name);
            project->addGroup(group);
            qmodel->add(group);
        }
    }

    void Explorer::handleNewApiTriggered()
    {
        auto [qmodel, group] = getApiAndGroup(sender());
        Model::Project *project = qmodel->getProject();

        GroupDialog *groupDialog = new GroupDialog(project->getGroups(), group, this);
        const int result = groupDialog->exec();
        if (result == QDialog::Accepted)
        {
            Model::Group *group = groupDialog->getCurrentGroup();
            const QString name = groupDialog->getName();
            if (!name.isEmpty())
            {
                Model::Api *api = projectPersistence->createApi(group, name);
                // api->setGroup(group);
                group->add(api);
                qmodel->create(api);
            }
        }
        groupDialog->deleteLater();
    }

    void Explorer::handleRenameGroupTriggered()
    {
        auto [qmodel, group] = getApiAndGroup(sender());

        bool ok;
        QString name = QInputDialog::getText(this, tr("Create Group"), tr("Create Group"), QLineEdit::Normal, group->getName(), &ok);
        if (ok && !name.isEmpty())
        {
            projectPersistence->rename(group, name);
            group->setName(name);
            qmodel->rename(group, name);
        }
    }

    void Explorer::handleDeleteGroupTriggered()
    {
        auto [qmodel, group] = getApiAndGroup(sender());

        if (QMessageBox::question(this, tr("Question"), tr("Delete this group?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
        {
            projectPersistence->remove(group);
            qmodel->remove(group);
            Model::Project *project = qmodel->getProject();
            project->removeGroup(group);

            // TODO
            // Close opened apis
        }
    }

    void Explorer::handleRenameApiTriggered()
    {
        auto [qmodel, api] = getApiAndApi(sender());

        bool ok;
        const QString name = QInputDialog::getText(this, tr("Rename api"), tr("Rename api"), QLineEdit::Normal, api->getName(), &ok);
        if (ok && !name.isEmpty())
        {
            projectPersistence->rename(api, name);
            qmodel->rename(api, name);
            api->setName(name);

            // TODO
            // Rename tabs
        }
    }

    void Explorer::handleDeleteApiTriggered()
    {
        auto [qmodel, api] = getApiAndApi(sender());

        if (QMessageBox::question(this, tr("Question"), tr("Delete this api?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
        {
            projectPersistence->remove(api);
            qmodel->remove(api);
            Model::Group *group = api->getGroup();
            group->remove(api);
        }
    }

    void Explorer::handleTreeContextMenu(QTreeView *tree, const QPoint &pos) const
    {
        const QModelIndex index = tree->indexAt(pos);
        if (index.isValid())
        {
            const QModel::Api *qmodel = static_cast<QModel::Api *>(tree->model());
            const QModel::ApiItem *apiItem = qmodel->getItem(index);
            const QString type = apiItem->type;
            if (type == "group")
            {
                newApiAction->setData(pos);
                renameGroupAction->setData(pos);
                deleteGroupAction->setData(pos);
                groupMenu->popup(tree->viewport()->mapToGlobal(pos));
            }
            else if (type == "api")
            {
                renameApiAction->setData(pos);
                deleteApiAction->setData(pos);
                apiMenu->popup(tree->viewport()->mapToGlobal(pos));
            }
        }
        else
        {
            // newGroupAction->setData(Utils::pointerToString(tree));
            projectMenu->popup(tree->viewport()->mapToGlobal(pos));
        }
    }

    std::tuple<QModel::Api *, Model::Group *> Explorer::getApiAndGroup(QObject *object) const
    {
        const QAction *action = qobject_cast<QAction *>(object);
        const QPoint point = action->data().toPoint();
        QTreeView *tree = static_cast<QTreeView *>(projectStacks->currentWidget());
        const QModelIndex index = tree->indexAt(point);
        QModel::Api *qmodel = static_cast<QModel::Api *>(tree->model());
        Model::Group *group = qmodel->getGroup(index);

        return std::make_tuple(qmodel, group);
    }

    std::tuple<QModel::Api *, Model::Api *> Explorer::getApiAndApi(QObject *object) const
    {
        const QAction *action = qobject_cast<QAction *>(object);
        const QPoint point = action->data().toPoint();
        QTreeView *tree = static_cast<QTreeView *>(projectStacks->currentWidget());
        const QModelIndex index = tree->indexAt(point);
        QModel::Api *qmodel = static_cast<QModel::Api *>(tree->model());
        Model::Api *api = qmodel->getApi(index);

        return std::make_tuple(qmodel, api);
    }
} // namespace Ui
