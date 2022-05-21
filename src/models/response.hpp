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

#include <QString>
#include <list>

namespace Model
{
    class Response
    {
    public:
        struct Header
        {
            QString key;
            QString value;
        };

    public:
        Response() = default;
        ~Response() = default;

        int getStatus() const;
        void setStatus(int status);
        double getTotalTime() const;
        void setTotalTime(double time);
        double getDnsLookupTime() const;
        void setDnsLookupTime(double time);
        double getTcpHandshakeTime() const;
        void setTcpHandshakeTime(double time);
        double getSslHandshakeTime() const;
        void setSslHandshakeTime(double time);
        double getTransferStartTime() const;
        void setTransferStartTime(double time);
        double getDownloadTime() const;
        void setDownloadTime(double time);
        const std::list<Header> getHeaders() const;
        void addHeader(Header header);
        QString getBody() const;
        void setBody(const QString &body);

    private:
        int status;
        double totalTime;
        double dnsLookupTime;
        double tcpHandshakeTime;
        double sslHandshakeTime;
        double transferStartTime;
        double downloadTime;
        std::list<Header> headers;
        QString body;
    };
} // namespace Model
