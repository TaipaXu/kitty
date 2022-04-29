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

QT_BEGIN_NAMESPACE
class QStackedWidget;
QT_END_NAMESPACE

namespace Model
{
    class Project;
    class Api;
} // namespace Model

namespace Persistence
{
    class Project;
} // namespace Persistence

namespace Ui
{
    class Explorer;
    class Tabbar;

    class Projects : public QWidget
    {
        Q_OBJECT

    public:
        explicit Projects(QWidget *parent = nullptr);
        ~Projects();

        void loadProject(Model::Project *project) const;
        void saveCurrentApi() const;
        void saveApi(Model::Api *api) const;
        void tryToChangeTab(int index);
        void tryToChangeNextTab();
        void tryToCloseCurrentTab();

    private slots:
        void handleExplorerRequestOpen(Model::Api *api);
        void handleTabActivated(Model::Api *api);
        void tryToCloseApi(Model::Api *api);
        void handleApiModified(Model::Api *api, bool modified) const;

    private:
        void closeApi(Model::Api *api);

    private:
        Explorer *explorer;
        Tabbar *tabbar;
        QStackedWidget *workviews;
        std::list<Model::Api *> openedApis;
        Model::Api *currentApi;
        Persistence::Project *projectPersistence;
    };
} // namespace Ui
