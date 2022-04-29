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
#include <QVBoxLayout>
#include <QTabWidget>
#include "widgets/responseHeaders.hpp"
#include "widgets/responseBody.hpp"
#include "models/response.hpp"

namespace Ui
{
    Response::Response(QWidget *parent)
        : QWidget(parent), headers{nullptr}, body{nullptr}
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setContentsMargins(0, 0, 0, 0);

        QTabWidget *tabWidget = new QTabWidget(this);
        mainLayout->addWidget(tabWidget);

        headers = new ResponseHeaders(tabWidget);
        tabWidget->addTab(headers, tr("Headers"));

        body = new ResponseBody(tabWidget);
        tabWidget->addTab(body, tr("Body"));

        setLayout(mainLayout);
    }

    void Response::setData(const Model::Response &response)
    {
        headers->setData(response.getHeaders());
        body->setData(response.getBody());
    }
} // namespace Ui
