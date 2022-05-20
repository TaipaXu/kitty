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

#include "core.hpp"
#include <QApplication>
#include <QTranslator>
extern "C"
{
#include <curl/curl.h>
}
#include "config.hpp"
#include "widgets/mainWindow.hpp"
#include "models/language.hpp"
#include "persistence/settings.hpp"

Core::Core(QObject *parent)
    : QObject(parent)
{
    settings = Persistence::Settings::getInstance();
    connect(settings, &Persistence::Settings::settingsChanged, this, &Core::handleSettingsChanged);

    installTranslator();

    mainWindow = new Ui::MainWindow();
    mainWindow->show();

    curl_global_init(CURL_GLOBAL_ALL);
}

Core::~Core()
{
    mainWindow->deleteLater();
    mainWindow = nullptr;

    curl_global_cleanup();
}

void Core::handleSettingsChanged() const
{
    const Model::Language language = settings->getLanguage();
    if (currentLanguage != language)
    {
        reboot();
    }
}

void Core::installTranslator()
{
    currentLanguage = settings->getLanguage();

    if (currentLanguage == Model::Language::Zh)
    {
        // QTranslator *translator = new QTranslator(this);
        // translator->load(":/i18n/zh_CN");
        // qApp->installTranslator(translator);
    }
}

void Core::reboot() const
{
    qApp->exit(REBOOT_CODE);
}
