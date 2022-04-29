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

#include <QWidget>

namespace Model
{
    class Api;
    class Response;
} // namespace Model

class Network;

namespace Ui
{
    class Request;
    class Response;

    class Api : public QWidget
    {
        Q_OBJECT

    public:
        explicit Api(Model::Api *api, QWidget *parent = nullptr);
        ~Api() = default;

    signals:
        void modified(Model::Api *api, bool modified) const;

    private slots:
        void handleMethodChanged(const QString &text);
        void handleUrlChanged(const QString &text);
        void handleRequestChanged();
        void handleFoundResponse(const Model::Response &response);

    private:
        void sendRequest();
        bool isChanged() const;

    private:
        Model::Api *api;
        Request *request;
        Response *response;
        bool isModified;
        Network *network;
        bool isRequestting;
    };
} // namespace Ui
