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
    class Param;
    class Header;
    class Body;
    class FormData;
    class XWwwFormUrlEncoded;
} // namespace Model

namespace Ui
{
    class Request : public QWidget
    {
        Q_OBJECT

    public:
        explicit Request(Model::Api *api, QWidget *parent = nullptr);
        ~Request() = default;

    signals:
        void changed();

    private slots:
        void handleParamsChanged(std::list<Model::Param *> &params);
        void handleHeadersChanged(std::list<Model::Header *> &params);

    private:
        Model::Api *api;
    };
} // namespace Ui
