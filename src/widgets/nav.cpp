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

#include "./nav.hpp"
#include <QVBoxLayout>
#include "navbutton.hpp"

namespace Ui
{
    Nav::Nav(QWidget *parent)
        : QWidget(parent),
          enableStatuses{{Status::Manager, true}, {Status::Projects, true}, {Status::Histories, true}, {Status::Settings, true}},
          status{Status::Manager}
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setAlignment(Qt::AlignCenter);

        managerNav = new Ui::NavButton(this);
        projectsNav = new Ui::NavButton(this);
        historiesNav = new Ui::NavButton(this);
        settingsNav = new Ui::NavButton(this);
        managerNav->setMinimumSize(40, 40);
        projectsNav->setMinimumSize(40, 40);
        historiesNav->setMinimumSize(40, 40);
        settingsNav->setMinimumSize(40, 40);
        mainLayout->addWidget(managerNav);
        mainLayout->addWidget(projectsNav);
        mainLayout->addWidget(historiesNav);
        mainLayout->addStretch(1);
        mainLayout->addWidget(settingsNav);

        managerNav->setImage(QPixmap(":/images/manager"), QPixmap(":/images/manager-active"));
        managerNav->setStatus(true);
        projectsNav->setImage(QPixmap(":/images/projects"), QPixmap(":/images/projects-active"));
        projectsNav->setStatus(false);
        historiesNav->setImage(QPixmap(":/images/history"), QPixmap(":/images/history-active"));
        historiesNav->setStatus(false);
        settingsNav->setImage(QPixmap(":/images/settings"), QPixmap(":/images/settings-active"));
        settingsNav->setStatus(false);

        connect(managerNav, &Ui::NavButton::clicked, this, &Nav::handleManagerNavClicked);
        connect(projectsNav, &Ui::NavButton::clicked, this, &Nav::handleProjectsNavClicked);
        connect(historiesNav, &Ui::NavButton::clicked, this, &Nav::handleHistoriesNavClicked);
        connect(settingsNav, &Ui::NavButton::clicked, this, &Nav::handleSettingsNavClicked);

        setLayout(mainLayout);
    }

    bool Nav::setActive(Status status)
    {
        if (!enableStatuses[status])
        {
            return false;
        }

        switch (status)
        {
        case Status::Manager:
        {
            this->status = Status::Manager;
            managerNav->setStatus(true);
            projectsNav->setStatus(false);
            historiesNav->setStatus(false);
            settingsNav->setStatus(false);
            break;
        }

        case Status::Projects:
        {
            this->status = Status::Projects;
            managerNav->setStatus(false);
            projectsNav->setStatus(true);
            historiesNav->setStatus(false);
            settingsNav->setStatus(false);
            break;
        }

        case Status::Histories:
        {
            this->status = Status::Histories;
            managerNav->setStatus(false);
            projectsNav->setStatus(false);
            historiesNav->setStatus(true);
            settingsNav->setStatus(false);
            break;
        }

        case Status::Settings:
        {
            this->status = Status::Settings;
            managerNav->setStatus(false);
            projectsNav->setStatus(false);
            historiesNav->setStatus(false);
            settingsNav->setStatus(true);
            break;
        }

        default:
            break;
        }
        return true;
    }

    void Nav::enable(Status status)
    {
        enableStatuses[status] = true;
    }

    void Nav::disable(Status status)
    {
        enableStatuses[status] = false;
    }

    void Nav::handleManagerNavClicked()
    {
        if (enableStatuses[Status::Manager] && status != Status::Manager)
        {
            status = Status::Manager;
            managerNav->setStatus(true);
            projectsNav->setStatus(false);
            historiesNav->setStatus(false);

            emit managerActived();
        }
    }

    void Nav::handleProjectsNavClicked()
    {
        if (enableStatuses[Status::Projects] && status != Status::Projects)
        {
            status = Status::Projects;
            managerNav->setStatus(false);
            projectsNav->setStatus(true);
            historiesNav->setStatus(false);

            emit projectsActived();
        }
    }

    void Nav::handleHistoriesNavClicked()
    {
        if (enableStatuses[Status::Histories] && status != Status::Histories)
        {
            status = Status::Histories;
            managerNav->setStatus(false);
            projectsNav->setStatus(false);
            historiesNav->setStatus(true);

            emit historiesActived();
        }
    }

    void Nav::handleSettingsNavClicked()
    {
        emit settingsActived();
    }
} // namespace Ui
