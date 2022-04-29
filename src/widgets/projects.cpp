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

#include "./projects.hpp"
#include <QVBoxLayout>
#include <QSplitter>
#include <QStackedWidget>
#include <QMessageBox>
#include "widgets/explorer.hpp"
#include "widgets/tabbar.hpp"
#include "widgets/api.hpp"
#include "models/project.hpp"
#include "models/api.hpp"
#include "persistence/project.hpp"

namespace Ui
{
    Projects::Projects(QWidget *parent)
        : QWidget(parent), currentApi(nullptr)
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setContentsMargins(0, 0, 0, 0);

        QSplitter *splitter = new QSplitter(this);

        explorer = new Explorer(this);
        connect(explorer, &Explorer::requestOpen, this, &Projects::handleExplorerRequestOpen);
        splitter->addWidget(explorer);

        QWidget *workviewsWrapper = new QWidget(this);

        QVBoxLayout *workviewsLayout = new QVBoxLayout();
        workviewsLayout->setContentsMargins(0, 0, 0, 0);
        workviewsLayout->setAlignment(Qt::AlignTop);

        tabbar = new Tabbar(workviewsWrapper);
        connect(tabbar, &Tabbar::tabActivated, this, &Projects::handleTabActivated);
        connect(tabbar, &Tabbar::requestCloseApi, this, &Projects::tryToCloseApi);
        workviewsLayout->addWidget(tabbar);

        workviews = new QStackedWidget(workviewsWrapper);
        workviews->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        workviewsLayout->addWidget(workviews);

        workviewsWrapper->setLayout(workviewsLayout);

        splitter->addWidget(workviewsWrapper);
        splitter->setSizes({200, 600});

        mainLayout->addWidget(splitter);

        setLayout(mainLayout);

        projectPersistence = Persistence::Project::getInstance();
    }

    Projects::~Projects()
    {
    }

    void Projects::loadProject(Model::Project *project) const
    {
        explorer->addProject(project);
    }

    void Projects::saveCurrentApi() const
    {
        if (currentApi)
        {
            // TODO
            // Check

            tabbar->setApiModified(currentApi, false);

            projectPersistence->save(currentApi);
            // apis[currentApi].first->reloadOldApi();
        }
    }

    void Projects::saveApi(Model::Api *api) const
    {
        tabbar->setApiModified(currentApi, false);

        projectPersistence->save(currentApi);
    }

    void Projects::tryToChangeTab(int index)
    {
        if (index < openedApis.size())
        {
            auto it = openedApis.begin();
            std::advance(it, index);
            tabbar->setCurrentApi(*it);
            handleTabActivated(*it);
        }
    }

    void Projects::tryToChangeNextTab()
    {
        if (currentApi)
        {
            auto it = std::find(openedApis.begin(), openedApis.end(), currentApi);
            int index = std::distance(openedApis.begin(), it);

            if (index < openedApis.size() - 1)
            {
                ++it;
                handleTabActivated(*it);
            }
            else
            {
                handleTabActivated(openedApis.front());
            }
        }
    }

    void Projects::tryToCloseCurrentTab()
    {
    }

    void Projects::handleExplorerRequestOpen(Model::Api *api)
    {
        currentApi = api;

        auto it = std::find(openedApis.begin(), openedApis.end(), api);
        if (it == openedApis.end())
        {
            // Add
            tabbar->addApi(api);
            projectPersistence->load(api);
            // Model::Api::Data *data = projectPersistence->getData(api);
            Api *apiWidget = new Api(api, workviews);
            connect(apiWidget, &Api::modified, this, &Projects::handleApiModified);
            workviews->addWidget(apiWidget);
            workviews->setCurrentWidget(apiWidget);

            openedApis.push_back(api);
        }
        else
        {
            // Active
            workviews->setCurrentWidget(workviews->widget(std::distance(openedApis.begin(), it)));
        }
        tabbar->setCurrentApi(api);
    }

    void Projects::handleTabActivated(Model::Api *api)
    {
        currentApi = api;
        explorer->setCurrentApi(api);
        auto it = std::find(openedApis.begin(), openedApis.end(), api);
        int distance = std::distance(openedApis.begin(), it);
        workviews->setCurrentIndex(distance);
    }

    void Projects::tryToCloseApi(Model::Api *api)
    {
        if (tabbar->isModified(api))
        {
            QMessageBox::StandardButton result = QMessageBox::critical(this, tr("Do you want ot save the changes you've made"), tr("Your changes will be lost if you dont't save them"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
            if (result == QMessageBox::Cancel)
            {
                return;
            }
            if (result == QMessageBox::Save)
            {
                saveApi(api);
            }

            closeApi(api);
        }
        else
        {
            closeApi(api);
        }
    }

    void Projects::handleApiModified(Model::Api *api, bool modified) const
    {
        tabbar->setApiModified(api, modified);
    }

    void Projects::closeApi(Model::Api *api)
    {
        tabbar->removeApi(api);

        auto it = std::find(openedApis.begin(), openedApis.end(), api);
        int distance = std::distance(openedApis.begin(), it);
        workviews->removeWidget(workviews->widget(distance));

        (*it)->unloadData();

        openedApis.erase(it);

        if (currentApi == api)
        {
            if (openedApis.size() > 0)
            {
                currentApi = openedApis.back();
                tabbar->setCurrentApi(currentApi);

                auto it = std::find(openedApis.begin(), openedApis.end(), currentApi);
                int distance = std::distance(openedApis.begin(), it);
                workviews->setCurrentIndex(distance);
            }
            else
            {
                currentApi = nullptr;
            }

            explorer->setCurrentApi(currentApi);
        }
    }
} // namespace Ui
