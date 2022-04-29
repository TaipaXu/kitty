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
    class Header;
} // namespace Model

namespace QModel
{
    class Header;
} // namespace QModel

namespace Ui
{
    class RequestHeaders : public QWidget
    {
        Q_OBJECT

    public:
        explicit RequestHeaders(const std::list<Model::Header *> &headers, QWidget *parent = nullptr);
        ~RequestHeaders() = default;

    signals:
        void changed(std::list<Model::Header *> &params);

    private slots:
        void handleHeadersChanged();

    private:
        QModel::Header *model;
    };
} // namespace Ui
