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
class QVBoxLayout;
QT_END_NAMESPACE

namespace Model
{
    class Project;
} // namespace Model

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
    class ManagerProject;

    class Manager : public QWidget
    {
        Q_OBJECT

    public:
        explicit Manager(QWidget *parent = nullptr);
        ~Manager();

        void addProject(Model::Project *project);
        void removeProject(Model::Project *project);

    signals:
        // void requestEditProject(Model::Project *project);

    private slots:
        void handleProjectEdited(Model::Project *project) const;

    private:
        void loadProjects();
        void resetProjects() const;

    private:
        QVBoxLayout *projectsLayout;
        Persistence::Project *projectPersistence;
        std::list<std::pair<Model::Project *, ManagerProject *>> projects;
        Event::Bus *bus;
    };
} // namespace Ui
