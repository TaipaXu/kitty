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
#include <QLabel>
#include <QMovie>
#include "widgets/responseHeaders.hpp"
#include "widgets/responseBody.hpp"
#include "models/response.hpp"

namespace Ui
{
    Response::Response(QWidget *parent)
        : QWidget(parent), headers{nullptr}, body{nullptr}, isLoading{false}
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

        QVBoxLayout *loadingLayout = new QVBoxLayout();
        loadingLayout->setAlignment(Qt::AlignCenter);
        loadingWidget = new QWidget(this);
        loadingWidget->setLayout(loadingLayout);
        loadingWidget->hide();
        loadingWidget->setFixedSize(size());
        loadingWidget->setStyleSheet("background-color: rgba(0, 0, 0, 0.6);");

        loadingMovie = new QMovie(this);
        loadingMovie->setFileName(":/images/loading");
        loadingMovie->setScaledSize({30, 30});

        loadingLabel = new QLabel(this);
        loadingLabel->setMovie(loadingMovie);
        loadingLabel->setFixedSize({30, 30});
        loadingLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        loadingLabel->setAlignment(Qt::AlignCenter);
        loadingLayout->addWidget(loadingLabel);
    }

    void Response::setData(const Model::Response &response)
    {
        headers->setData(response.getHeaders());
        body->setData(response.getBody());
    }

    void Response::showLoading()
    {
        isLoading = true;
        loadingMovie->start();
        loadingWidget->setFixedSize(size());
        loadingWidget->show();
        loadingWidget->raise();

        update();
    }

    void Response::hideLoading()
    {
        isLoading = false;
        loadingMovie->stop();
        setDisabled(false);
        loadingWidget->hide();
        loadingWidget->lower();

        update();
    }

    void Response::resizeEvent(QResizeEvent *event)
    {
        if (isLoading)
        {
            loadingWidget->setFixedSize(size());
        }
    }
} // namespace Ui
