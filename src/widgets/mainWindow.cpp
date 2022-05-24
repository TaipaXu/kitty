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

#include "./mainWindow.hpp"
#include <QApplication>
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenuBar>
#include <QShortcut>
#include "widgets/nav.hpp"
#include "widgets/manager.hpp"
#include "widgets/projects.hpp"
#include "widgets/histories.hpp"
#include "widgets/projectDialog.hpp"
#include "widgets/settings.hpp"
#include "models/project.hpp"
#include "persistence/project.hpp"
#include "events/bus.hpp"
#include "config.hpp"

namespace Ui
{
    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow{parent}
    {
        setWindowIcon(QIcon(":/images/icon"));

        QWidget *centralwidget = new QWidget(this);
        QHBoxLayout *mainLayout = new QHBoxLayout();
        mainLayout->setContentsMargins(0, 0, 0, 0);

        nav = new Nav(this);
        connect(nav, &Nav::managerActived, this, &MainWindow::handleManagerNavActived);
        connect(nav, &Nav::projectsActived, this, &MainWindow::handleProjectsNavActived);
        connect(nav, &Nav::historiesActived, this, &MainWindow::handleHistoryNavActived);
        connect(nav, &Nav::settingsActived, this, &MainWindow::handleSettingsNavActived);
        nav->disable(Nav::Status::Projects);
        mainLayout->addWidget(nav);

        navStack = new QStackedWidget(this);

        manager = new Manager(navStack);
        navStack->addWidget(manager);
        projects = new Projects(navStack);
        navStack->addWidget(projects);
        histories = new Histories(navStack);
        navStack->addWidget(histories);
        settings = new Settings(navStack);
        navStack->addWidget(settings);

        mainLayout->addWidget(navStack);

        centralwidget->setLayout(mainLayout);
        setCentralWidget(centralwidget);

        resize(1200, 800);

        createActions();
        createShortcuts();

        bus = Event::Bus::getInstance();
        connect(bus, &Event::Bus::requestCreateProject, this, &MainWindow::handleRequestCreateProject);
        connect(bus, &Event::Bus::requestImportProject, this, &MainWindow::handleRequestImportProject);
        connect(bus, &Event::Bus::requestEditProject, this, &MainWindow::handleRequestEditProject);
        connect(bus, &Event::Bus::requestDeleteProject, this, &MainWindow::handleRequestDeleteProject);
        connect(bus, &Event::Bus::requestLoadProject, this, &MainWindow::handleRequestLoadProject);
        connect(bus, &Event::Bus::requestExportProject, this, &MainWindow::handleRequestExportProject);

        projectPersistence = Persistence::Project::getInstance();
    }

    MainWindow::~MainWindow()
    {
    }

    void MainWindow::handleManagerNavActived()
    {
        navStack->setCurrentWidget(manager);
    }

    void MainWindow::handleProjectsNavActived()
    {
        navStack->setCurrentWidget(projects);
    }

    void MainWindow::handleHistoryNavActived()
    {
        navStack->setCurrentWidget(histories);
    }

    void MainWindow::handleSettingsNavActived()
    {
        navStack->setCurrentWidget(settings);
    }

    void MainWindow::handleRequestCreateProject()
    {
        ProjectDialog *dialog = new ProjectDialog(this);
        connect(dialog, &ProjectDialog::accepted, [this](const QString &name, const QString &description) {
            Model::Project *project = projectPersistence->createProject(name, description);
            manager->addProject(project);

            // emit bus->projectCreated(project);
        });
        dialog->exec();
        dialog->deleteLater();
    }

    void MainWindow::handleRequestImportProject()
    {
        // TODO
        // Import
        QString filePath = QFileDialog::getOpenFileUrl(this, tr("File"), QUrl(), "kitty(*.kitty)").toLocalFile();
        // projectPersistence->import(filePath);
    }

    void MainWindow::handleNewProjectShortcutActivated()
    {
        if (navStack->currentWidget() == manager)
        {
            handleRequestCreateProject();
        }
    }

    void MainWindow::handleRequestEditProject(Model::Project *project)
    {
        ProjectDialog *dialog = new ProjectDialog(project);
        connect(dialog, &ProjectDialog::accepted, this, [this, project](const QString &name, const QString &description) {
            project->edit(name, description);
            projectPersistence->edit(project, name, description);

            // manager->edit(project);
            emit bus->projectEdited(project);

            // TODO
            // projects change
        });
        dialog->exec();
        dialog->deleteLater();
    }

    void MainWindow::handleRequestDeleteProject(Model::Project *project)
    {
        if (QMessageBox::question(this, tr("Question"), tr("Delete this project?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
        {
            projectPersistence->remove(project);
            manager->removeProject(project);
        }
    }

    void MainWindow::handleRequestLoadProject(Model::Project *project)
    {
        nav->enable(Nav::Status::Projects);
        // TODO none projects

        nav->setActive(Nav::Status::Projects);
        navStack->setCurrentWidget(projects);
        // projectPersistence->load(project);
        projects->loadProject(project);
    }

    void MainWindow::handleRequestExportProject(Model::Project *project)
    {
    }

    void MainWindow::handleCloseProjectShortcutActivated()
    {
        // TODO
        if (navStack->currentWidget() == projects)
        {
            projects->tryToCloseCurrentTab();
        }
    }

    void MainWindow::saveCurrentApi()
    {
        if (navStack->currentWidget() == projects)
        {
            projects->saveCurrentApi();
        }
    }

    void MainWindow::changeNav(int index)
    {
        switch (index)
        {
        case 1:
        {
            nav->setActive(Nav::Status::Manager);
            handleManagerNavActived();
            break;
        }
        case 2:
        {
            if (nav->setActive(Nav::Status::Projects))
            {
                handleProjectsNavActived();
            }
            break;
        }
        case 3:
        {
            nav->setActive(Nav::Status::Histories);
            handleHistoryNavActived();
            break;
        }
        case 9:
        {
            nav->setActive(Nav::Status::Settings);
            handleSettingsNavActived();
            break;
        }

        default:
            break;
        }
    }

    void MainWindow::changeTab(int index)
    {
        if (navStack->currentWidget() == projects)
        {
            projects->tryToChangeTab(--index);
        }
    }

    void MainWindow::changeTab()
    {
        if (navStack->currentWidget() == projects)
        {
            projects->tryToChangeNextTab();
        }
    }

    void MainWindow::toggleFullscreen()
    {
        if (!isFullScreen())
        {
            showFullScreen();
        }
        else
        {
            showNormal();
        }
    }

    void MainWindow::aboutToClose()
    {
    }

    void MainWindow::handleAboutActionTriggered()
    {
        QDesktopServices::openUrl(QUrl(PROJECT_DOMAIN));
    }

    void MainWindow::handleReportBugActionTriggered()
    {
        QDesktopServices::openUrl(QUrl(PROJECT_ISSUES_DOMAIN));
    }

    void MainWindow::createActions()
    {
        QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

        QAction *createProjectAction = new QAction(tr("Create Project"), this);
        connect(createProjectAction, &QAction::triggered, this, &MainWindow::handleRequestCreateProject);
        fileMenu->addAction(createProjectAction);

        QAction *importProjectAction = new QAction(tr("Import Project"), this);
        connect(importProjectAction, &QAction::triggered, this, &MainWindow::handleRequestImportProject);
        fileMenu->addAction(importProjectAction);

        fileMenu->addSeparator();

        const QIcon exitIcon = QIcon::fromTheme("application-exit");
        QAction *exitAction = fileMenu->addAction(exitIcon, tr("E&xit"), this, &MainWindow::aboutToClose);
        exitAction->setShortcuts(QKeySequence::Quit);
        exitAction->setStatusTip(tr("Exit the application"));

        QMenu *aboutMenu = menuBar()->addMenu(tr("&About"));

        QAction *aboutAction = new QAction(tr("About"), aboutMenu);
        connect(aboutAction, &QAction::triggered, this, &MainWindow::handleAboutActionTriggered);
        aboutAction->setStatusTip(tr("Open the page of this project."));
        aboutMenu->addAction(aboutAction);

        QAction *aboutQtAction = new QAction(tr("About Qt"), aboutMenu);
        connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
        aboutMenu->addAction(aboutQtAction);

        aboutMenu->addSeparator();

        QAction *reportBugAction = new QAction(tr("Report Bug"), aboutMenu);
        connect(reportBugAction, &QAction::triggered, this, &MainWindow::handleReportBugActionTriggered);
        aboutMenu->addAction(reportBugAction);
    }

#define createAltShortcut(i)                                                  \
    {                                                                         \
        QShortcut *shortcut = new QShortcut(this);                            \
        shortcut->setKey(Qt::ALT + Qt::Key_##i);                              \
        shortcut->setAutoRepeat(false);                                       \
        connect(shortcut, &QShortcut::activated, [this]() { changeTab(i); }); \
    }

#define createCtrlShortcut(i)                                                 \
    {                                                                         \
        QShortcut *shortcut = new QShortcut(this);                            \
        shortcut->setKey(Qt::CTRL + Qt::Key_##i);                             \
        shortcut->setAutoRepeat(false);                                       \
        connect(shortcut, &QShortcut::activated, [this]() { changeNav(i); }); \
    }

    void MainWindow::createShortcuts()
    {
        createAltShortcut(1);
        createAltShortcut(2);
        createAltShortcut(3);
        createAltShortcut(4);
        createAltShortcut(5);
        createAltShortcut(6);
        createAltShortcut(7);
        createAltShortcut(8);
        createAltShortcut(9);

        createCtrlShortcut(1);
        createCtrlShortcut(2);
        createCtrlShortcut(3);
        createCtrlShortcut(9);

        QShortcut *shortcut = nullptr;

        shortcut = new QShortcut(this);
        shortcut->setKey(Qt::CTRL + Qt::Key_Tab);
        shortcut->setAutoRepeat(false);
        connect(shortcut, &QShortcut::activated, this, QOverload<>::of(&MainWindow::changeTab));

        shortcut = new QShortcut(this);
        shortcut->setKey(Qt::CTRL + Qt::Key_N);
        shortcut->setAutoRepeat(false);
        connect(shortcut, &QShortcut::activated, this, &MainWindow::handleNewProjectShortcutActivated);

        shortcut = new QShortcut(this);
        shortcut->setKey(Qt::CTRL + Qt::Key_W);
        shortcut->setAutoRepeat(false);
        connect(shortcut, &QShortcut::activated, this, &MainWindow::handleCloseProjectShortcutActivated);

        shortcut = new QShortcut(this);
        shortcut->setKey(Qt::CTRL + Qt::Key_S);
        shortcut->setAutoRepeat(false);
        connect(shortcut, &QShortcut::activated, this, &MainWindow::saveCurrentApi);

        shortcut = new QShortcut(this);
        shortcut->setKey(QKeySequence::FullScreen);
        shortcut->setAutoRepeat(false);
        connect(shortcut, &QShortcut::activated, this, &MainWindow::toggleFullscreen);
    }
} // namespace Ui
