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

#include "./requestHeaders.hpp"
#include <QVBoxLayout>
#include "widgets/headerTable.hpp"
#include "models/header.hpp"
#include "qmodels/header.hpp"

namespace Ui
{
    RequestHeaders::RequestHeaders(const std::list<Model::Header *> &headers, QWidget *parent)
        : QWidget(parent)
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setContentsMargins(0, 0, 0, 0);

        HeaderTable *table = new HeaderTable(this);
        model = new QModel::Header(headers, this);
        connect(model, &QModel::Header::dataChanged, this, &RequestHeaders::handleHeadersChanged);
        connect(model, &QModel::Header::rowsInserted, this, &RequestHeaders::handleHeadersChanged);
        connect(model, &QModel::Header::rowsMoved, this, &RequestHeaders::handleHeadersChanged);
        connect(model, &QModel::Header::rowsRemoved, this, &RequestHeaders::handleHeadersChanged);
        table->setModel(model);
        mainLayout->addWidget(table);

        setLayout(mainLayout);
    }

    void RequestHeaders::handleHeadersChanged()
    {
        std::list<Model::Header *> items = model->getItems();
        emit changed(items);
    }
} // namespace Ui
