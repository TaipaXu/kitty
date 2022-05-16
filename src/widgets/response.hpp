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

QT_BEGIN_NAMESPACE
class QLabel;
class QMovie;
class QResizeEvent;
QT_END_NAMESPACE

namespace Model
{
    class Response;
} // namespace Model

namespace Ui
{
    class ResponseHeaders;
    class ResponseBody;

    class Response : public QWidget
    {
        Q_OBJECT

    public:
        explicit Response(QWidget *parent = nullptr);
        ~Response() = default;

        void setData(const Model::Response &response);
        void showLoading();
        void hideLoading();

    protected:
        void resizeEvent(QResizeEvent *event) override;

    private:
        ResponseHeaders *headers;
        ResponseBody *body;
        QWidget *loadingWidget;
        QLabel *loadingLabel;
        QMovie *loadingMovie;
        bool isLoading;
    };
} // namespace Ui
