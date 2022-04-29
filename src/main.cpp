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

#include <QApplication>
#include <QProcess>
#include <QDir>
#include <QStyleFactory>
#include <csignal>
#include "core.hpp"
#include "config.hpp"

void signalHandler(int sig)
{
    qApp->quit();
}

int main(int argc, char *argv[])
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    QApplication::setStyle("Fusion");
    QApplication app(argc, argv);
    app.setOrganizationDomain(ORGANIZATION_DOMAIN);
    app.setApplicationName(PROJECT_NAME);
    app.setApplicationVersion(PROJECT_VERSION);
    app.setApplicationDisplayName(DISPLAY_NAME);

    Core core;

    const int result = app.exec();
    if (result == REBOOT_CODE)
    {
        const QString program = QApplication::applicationFilePath();
        const QStringList arguments = QApplication::arguments();
        const QString appDirectory = QDir::currentPath();
        QProcess::startDetached(program, arguments, appDirectory);
        return EXIT_SUCCESS;
    }

    return result;
}
