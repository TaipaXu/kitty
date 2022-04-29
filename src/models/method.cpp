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

#include "./method.hpp"

namespace Model
{
    Method getMethod(const QString &method)
    {
        if (method == "GET")
        {
            return Method::GET;
        }
        else if (method == "POST")
        {
            return Method::POST;
        }
        else if (method == "HEAD")
        {
            return Method::HEAD;
        }
        else if (method == "PUT")
        {
            return Method::PUT;
        }
        else if (method == "DELETE")
        {
            return Method::DELETE;
        }
        else if (method == "CONNECT")
        {
            return Method::CONNECT;
        }
        else if (method == "OPTIONS")
        {
            return Method::OPTIONS;
        }
        else if (method == "TRACE")
        {
            return Method::TRACE;
        }
        else if (method == "PATCH")
        {
            return Method::PATCH;
        }
    }

    QString getMethodName(Method method)
    {
        switch (method)
        {
        case Method::GET:
        {
            return QStringLiteral("GET");
        }

        case Method::POST:
        {
            return QStringLiteral("POST");
        }

        case Method::HEAD:
        {
            return QStringLiteral("HEAD");
        }

        case Method::PUT:
        {
            return QStringLiteral("PUT");
        }

        case Method::DELETE:
        {
            return QStringLiteral("DELETE");
        }

        case Method::CONNECT:
        {
            return QStringLiteral("CONNECT");
        }

        case Method::OPTIONS:
        {
            return QStringLiteral("OPTIONS");
        }

        case Method::TRACE:
        {
            return QStringLiteral("TRACE");
        }

        case Method::PATCH:
        {
            return QStringLiteral("PATCH");
        }

        default:
        {
            break;
        }
        }
    }

    QStringList getMethodNames()
    {
        return {"GET", "POST", "HEAD", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"};
    }
} // namespace Model
