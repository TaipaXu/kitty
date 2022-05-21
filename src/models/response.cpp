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

#include "./response.hpp"

namespace Model
{
    int Response::getStatus() const
    {
        return status;
    }

    void Response::setStatus(int status)
    {
        this->status = status;
    }

    double Response::getTotalTime() const
    {
        return totalTime;
    }

    void Response::setTotalTime(double time)
    {
        this->totalTime = time;
    }

    double Response::getDnsLookupTime() const
    {
        return dnsLookupTime;
    }

    void Response::setDnsLookupTime(double time)
    {
        this->dnsLookupTime = time;
    }

    double Response::getTcpHandshakeTime() const
    {
        return tcpHandshakeTime;
    }

    void Response::setTcpHandshakeTime(double time)
    {
        this->tcpHandshakeTime = time;
    }

    double Response::getSslHandshakeTime() const
    {
        return sslHandshakeTime;
    }

    void Response::setSslHandshakeTime(double time)
    {
        this->sslHandshakeTime = time;
    }

    double Response::getTransferStartTime() const
    {
        return transferStartTime;
    }

    void Response::setTransferStartTime(double time)
    {
        this->transferStartTime = time;
    }

    double Response::getDownloadTime() const
    {
        return downloadTime;
    }

    void Response::setDownloadTime(double time)
    {
        this->downloadTime = time;
    }

    const std::list<Response::Header> Response::getHeaders() const
    {
        return headers;
    }

    void Response::addHeader(Header header)
    {
        headers.push_back(header);
    }

    QString Response::getBody() const
    {
        return body;
    }

    void Response::setBody(const QString &body)
    {
        this->body = body;
    }
} // namespace Model
