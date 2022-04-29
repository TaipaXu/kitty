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
#include "models/method.hpp"

namespace Model
{
    class Project;
    class Param;
    class Header;
    class Body;
    class Group;

    class Api
    {
    private:
        class Data
        {
        public:
            Data(const QString &url, const Method method, const std::list<Param *> &params, const std::list<Header *> &headers, Body *body);
            ~Data();

            bool operator==(const Data &other);

        public:
            QString url;
            Method method;
            std::list<Param *> params;
            std::list<Header *> headers;
            Body *body;
        };

    public:
        Api(const QString &id, const QString &name);
        Api(const QString &id, const QString &name, const QString &url, const Method method, const std::list<Param *> &params, const std::list<Header *> &headers, Body *body);
        ~Api();

        QString getId() const;
        QString getName() const;
        void setName(const QString &name);
        bool isLoaded() const;
        void setData(const QString &url, const Method method, const std::list<Param *> &params, const std::list<Header *> &headers, Body *body);
        void unloadData();
        QString getUrl() const;
        void setUrl(const QString &url);
        Method getMethod() const;
        void setMethod(const Method &method);
        std::list<Param *> getParams() const;
        void setParams(std::list<Param *> params);
        std::list<Header *> getHeaders() const;
        void setHeaders(std::list<Header *> headers);
        Body *getBody() const;
        Group *getGroup() const;
        void setGroup(Group *group);
        Project *getProject() const;

    private:
        QString id;
        QString name;
        Data *data;
        Group *group;
    };
} // namespace Model
