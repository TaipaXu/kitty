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

#include "./manager.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QScrollArea>
#include <QPushButton>
#include "widgets/managerProject.hpp"
#include "models/project.hpp"
#include "persistence/project.hpp"
#include "events/bus.hpp"

namespace Ui
{
    Manager::Manager(QWidget *parent)
        : QWidget(parent),
          projectPersistence{Persistence::Project::getInstance()}
    {
        bus = Event::Bus::getInstance();
        connect(bus, &Event::Bus::projectEdited, this, &Manager::handleProjectEdited);

        QVBoxLayout *mainLayout = new QVBoxLayout();

        QHBoxLayout *controlsLayout = new QHBoxLayout();

        QPushButton *createButton = new QPushButton(tr("Create Project"), this);
        connect(createButton, &QPushButton::clicked, bus, &Event::Bus::requestCreateProject);
        controlsLayout->addWidget(createButton);

        QPushButton *importButton = new QPushButton(tr("Import Project"), this);
        connect(importButton, &QPushButton::clicked, bus, &Event::Bus::requestImportProject);
        controlsLayout->addWidget(importButton);

        QSpacerItem *horizontalSpacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
        controlsLayout->addItem(horizontalSpacer);

        mainLayout->addLayout(controlsLayout);

        QScrollArea *scrollArea = new QScrollArea(this);
        QWidget *scrollWidget = new QWidget(scrollArea);
        scrollArea->setWidget(scrollWidget);

        projectsLayout = new QVBoxLayout();
        projectsLayout->setAlignment(Qt::AlignTop);
        scrollArea->setWidgetResizable(true);
        scrollWidget->setLayout(projectsLayout);

        mainLayout->addWidget(scrollArea);

        setLayout(mainLayout);

        loadProjects();
    }

    Manager::~Manager()
    {
        for (const auto &[model, widget] : projects)
        {
            delete model;
        }
    }

    void Manager::addProject(Model::Project *project)
    {
        ManagerProject *managerProject = new ManagerProject(project, this);
        connect(managerProject, &ManagerProject::requestEdit, bus, &Event::Bus::requestEditProject);
        connect(managerProject, &ManagerProject::requestDelete, bus, &Event::Bus::requestDeleteProject);
        connect(managerProject, &ManagerProject::requestLoad, bus, &Event::Bus::requestLoadProject);
        connect(managerProject, &ManagerProject::requestExport, bus, &Event::Bus::requestExportProject);
        projects.push_back(std::make_pair(project, managerProject));

        resetProjects();
    }

    void Manager::removeProject(Model::Project *project)
    {
        auto it = std::find_if(projects.begin(), projects.end(), [project](std::pair<Model::Project *, ManagerProject *> item) {
            return project == item.first;
        });
        if (it != projects.end())
        {
            it->second->deleteLater();
            projects.remove(*it);
        }

        resetProjects();
    }

    void Manager::handleProjectEdited(Model::Project *project) const
    {
        auto it = std::find_if(projects.begin(), projects.end(), [project](std::pair<Model::Project *, ManagerProject *> item) {
            return project == item.first;
        });
        if (it != projects.end())
        {
            it->second->edit(project->getName(), project->getDescription());
        }
    }

    void Manager::loadProjects()
    {
        std::vector<Model::Project *> projects = projectPersistence->getProjects();

        for (auto &&project : projects)
        {
            addProject(project);
        }
    }

    void Manager::resetProjects() const
    {
        QLayoutItem *item;
        while ((item = projectsLayout->takeAt(0)))
        {
            if (item->widget())
            {
                projectsLayout->removeWidget(item->widget());
            }
        }

        for (auto &&i : projects)
        {
            projectsLayout->addWidget(i.second);
        }

        projectsLayout->update();
    }
} // namespace Ui
