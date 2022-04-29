#include "./api.hpp"
#include "models/project.hpp"
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

#include "models/group.hpp"

namespace Model
{
    Api::Data::Data(const QString &url, Method method, const std::list<Param *> &params, const std::list<Header *> &headers, Body *body)
        : method{method}, url{url}, params{params}, headers{headers}, body{body}
    {
    }

    Api::Data::~Data()
    {
        for (auto &&param : params)
        {
            delete param;
        }

        for (auto &&header : headers)
        {
            delete header;
        }

        delete body;
    }

    Api::Api(const QString &id, const QString &name)
        : id{id}, name{name}, data{nullptr}
    {
    }

    Api::Api(const QString &id, const QString &name, const QString &url, const Method method, const std::list<Param *> &params, const std::list<Header *> &headers, Body *body)
        : id{id}, name{name}, data{nullptr}
    {
        setData(url, method, params, headers, body);
    }

    Api::~Api()
    {
        delete data;
        data = nullptr;
    }

    QString Api::getId() const
    {
        return id;
    }

    QString Api::getName() const
    {
        return name;
    }

    void Api::setName(const QString &name)
    {
        this->name = name;
    }

    bool Api::isLoaded() const
    {
        return data != nullptr;
    }

    void Api::setData(const QString &url, const Method method, const std::list<Param *> &params, const std::list<Header *> &headers, Body *body)
    {
        delete data;
        this->data = new Data(url, method, params, headers, body);
    }

    void Api::unloadData()
    {
        delete data;
        data = nullptr;
    }

    QString Api::getUrl() const
    {
        return data->url;
    }

    void Api::setUrl(const QString &url)
    {
        data->url = url;
    }

    Method Api::getMethod() const
    {
        return data->method;
    }

    void Api::setMethod(const Method &method)
    {
        data->method = method;
    }

    std::list<Param *> Api::getParams() const
    {
        return data->params;
    }

    void Api::setParams(std::list<Param *> params)
    {
        data->params = params;
    }

    std::list<Header *> Api::getHeaders() const
    {
        return data->headers;
    }

    void Api::setHeaders(std::list<Header *> headers)
    {
        data->headers = headers;
    }

    Body *Api::getBody() const
    {
        return data->body;
    }

    Group *Api::getGroup() const
    {
        return group;
    }

    void Api::setGroup(Group *group)
    {
        this->group = group;
    }

    Project *Api::getProject() const
    {
        return group->getProject();
    }
} // namespace Model
