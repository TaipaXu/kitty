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

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QStackedWidget;
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
    class Nav;
    class Manager;
    class Projects;
    class Histories;
    class Settings;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void handleManagerNavActived();
        void handleProjectsNavActived();
        void handleHistoryNavActived();
        void handleSettingsNavActived();
        void handleRequestCreateProject();
        void handleRequestImportProject();
        void handleNewProjectShortcutActivated();
        void handleRequestEditProject(Model::Project *project);
        void handleRequestDeleteProject(Model::Project *project);
        void handleRequestLoadProject(Model::Project *project);
        void handleRequestExportProject(Model::Project *project);
        void handleCloseProjectShortcutActivated();
        void saveCurrentApi();
        void changeNav(int index);
        void changeTab(int index);
        void changeTab();
        void toggleFullscreen();
        void aboutToClose();
        void handleAboutActionTriggered();
        void handleReportBugActionTriggered();

    private:
        void createActions();
        void createShortcuts();

    private:
        Nav *nav;
        QStackedWidget *navStack;
        Manager *manager;
        Projects *projects;
        Histories *histories;
        Settings *settings;
        Event::Bus *bus;
        Persistence::Project *projectPersistence;
    };
} // namespace Ui
