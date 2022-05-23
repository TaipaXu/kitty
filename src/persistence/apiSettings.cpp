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

#include "./apiSettings.hpp"
#include <QSettings>
#include "models/language.hpp"

namespace Persistence
{
    ApiSettings::ApiSettings()
    {
        settings = new QSettings(this);
        readSettings();
    }

    ApiSettings *ApiSettings::getInstance()
    {
        static ApiSettings instance;
        return &instance;
    }

    bool ApiSettings::getAutoFollowRedirects() const
    {
        return autoFollowRedirects;
    }

    void ApiSettings::setAutoFollowRedirects(bool autoFollowRedirects)
    {
        this->autoFollowRedirects = autoFollowRedirects;
        settings->setValue("autoFollowRedirects", autoFollowRedirects);
    }

    bool ApiSettings::getEnableSslVerification() const
    {
        return enableSslVerification;
    }

    void ApiSettings::setEnableSslVerification(bool enableSslVerification)
    {
        this->enableSslVerification = enableSslVerification;
        settings->setValue("enableSslVerification", enableSslVerification);
    }

    int ApiSettings::getTimeout() const
    {
        return timeout;
    }

    void ApiSettings::setTimeout(int timeout)
    {
        this->timeout = timeout;
        settings->setValue("timeout", timeout);
    }

    void ApiSettings::readSettings()
    {
        autoFollowRedirects = settings->value("autoFollowRedirects", true).toBool();
        enableSslVerification = settings->value("enableSslVerification", true).toBool();
        timeout = settings->value("timeout", 50).toInt();
    }
} // namespace Persistence
