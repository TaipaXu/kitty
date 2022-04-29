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

#include "./persistence.hpp"
#include <QStandardPaths>
#include "config.hpp"

namespace Persistence
{
    Persistence::Persistence()
    {
        dataDirPath = getDataDirPath();
    }

    QString Persistence::getSettingsDirPath() const
    {
        const QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
        return path;
    }

    QString Persistence::getSettingsFilePath() const
    {
        const QString dirPath = getSettingsDirPath();
        const QString fileName{"settings.ini"};
        QString filePath;
        filePath = QString("%1/%2/%3").arg(dirPath).arg(PROJECT_NAME).arg(fileName);

        return filePath;
    }

    QString Persistence::getDataDirPath() const
    {
        const QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        return path;
    }
} // namespace Persistence
