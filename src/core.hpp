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

#include <QObject>

namespace Ui
{
    class MainWindow;
} // namespace Ui

namespace Model
{
    enum class Language;
} // namespace Model

namespace Persistence
{
    class Settings;
} // namespace Persistence

class Core : public QObject
{
    Q_OBJECT

public:
    explicit Core(QObject *parent = nullptr);
    ~Core();

private slots:
    void handleSettingsChanged() const;

private:
    void installTranslator();
    void reboot() const;

private:
    Ui::MainWindow *mainWindow;
    Persistence::Settings *settings;
    Model::Language currentLanguage;
};
