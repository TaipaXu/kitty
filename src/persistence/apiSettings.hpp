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

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace Model
{
    enum class Language;
} // namespace Model

namespace Persistence
{
    class ApiSettings : public QObject
    {
        Q_OBJECT

    public:
        enum class ProxyType
        {
            NoProxy,
            Proxy
        };

    public:
        static ApiSettings *getInstance();
        ApiSettings(const ApiSettings &) = delete;
        ApiSettings &operator=(const ApiSettings &) = delete;
        ~ApiSettings() = default;

    public:
        bool getAutoFollowRedirects() const;
        void setAutoFollowRedirects(bool autoFollowRedirects);
        bool getEnableSslVerification() const;
        void setEnableSslVerification(bool enableSslVerification);
        int getTimeout() const;
        void setTimeout(int timeout);
        ProxyType getProxyType() const;
        QString getProxyStr() const;
        void setNoProxy();
        void setProxy(const QString &proxyStr);

    private:
        ApiSettings();
        void readSettings();

    private:
        QSettings *settings;
        bool autoFollowRedirects;
        bool enableSslVerification;
        int timeout;
        ProxyType proxyType;
        QString proxyStr;
    };
} // namespace Persistence
