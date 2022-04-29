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
#include "models/response.hpp"

namespace QModel
{
    class ResponseHeader;
} // namespace QModel

namespace Ui
{
    class ResponseHeaders : public QWidget
    {
        Q_OBJECT

    public:
        explicit ResponseHeaders(QWidget *parent = nullptr);
        ~ResponseHeaders() = default;

        void setData(const std::list<Model::Response::Header> &headers);

    private:
        QModel::ResponseHeader *model;
    };
} // namespace Ui
