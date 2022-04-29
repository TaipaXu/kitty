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

#include "./request.hpp"
#include <QVBoxLayout>
#include <QTabWidget>
#include "widgets/requestParams.hpp"
#include "widgets/requestHeaders.hpp"
#include "widgets/requestBody.hpp"
#include "widgets/requestSettings.hpp"
#include "models/api.hpp"
#include "models/param.hpp"
#include "models/header.hpp"
#include "models/body.hpp"
#include "models/formData.hpp"
#include "models/xWwwFormUrlEncoded.hpp"

namespace Ui
{
    Request::Request(Model::Api *api, QWidget *parent)
        : QWidget(parent), api{api}
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setContentsMargins(0, 0, 0, 0);

        QTabWidget *tabWidget = new QTabWidget(this);

        RequestParams *params = new RequestParams(api->getParams(), tabWidget);
        connect(params, &RequestParams::changed, this, &Request::handleParamsChanged);
        tabWidget->addTab(params, tr("Params"));

        RequestHeaders *headers = new RequestHeaders(api->getHeaders(), tabWidget);
        connect(headers, &RequestHeaders::changed, this, &Request::handleHeadersChanged);
        tabWidget->addTab(headers, tr("Headers"));

        RequestBody *body = new RequestBody(api->getBody(), tabWidget);
        connect(body, &RequestBody::changed, this, &Request::changed);
        tabWidget->addTab(body, tr("Body"));

        RequestSettings *settings = new RequestSettings(tabWidget);
        tabWidget->addTab(settings, tr("Settings"));

        mainLayout->addWidget(tabWidget);

        setLayout(mainLayout);
    }

    void Request::handleParamsChanged(std::list<Model::Param *> &params)
    {
        api->setParams(params);
        emit changed();
    }

    void Request::handleHeadersChanged(std::list<Model::Header *> &headers)
    {
        api->setHeaders(headers);
        emit changed();
    }
} // namespace Ui
