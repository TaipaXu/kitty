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
#include <map>

namespace Ui
{
    class NavButton;

    class Nav : public QWidget
    {
        Q_OBJECT

    public:
        enum class Status
        {
            Manager,
            Projects,
            Histories,
            Settings
        };

    public:
        explicit Nav(QWidget *parent = nullptr);
        ~Nav() = default;

        bool setActive(Status status);
        void enable(Status status);
        void disable(Status status);

    signals:
        void managerActived();
        void projectsActived();
        void historiesActived();
        void settingsActived();

    private slots:

        void handleManagerNavClicked();
        void handleProjectsNavClicked();
        void handleHistoriesNavClicked();
        void handleSettingsNavClicked();

    private:
        std::map<Status, bool> enableStatuses;
        Status status;
        NavButton *managerNav;
        NavButton *projectsNav;
        NavButton *historiesNav;
        NavButton *settingsNav;
    };
} // namespace Ui
