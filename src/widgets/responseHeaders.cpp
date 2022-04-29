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

#include "./responseHeaders.hpp"
#include <QVBoxLayout>
#include <QHeaderView>
#include "widgets/responseTable.hpp"
#include "qmodels/responseHeader.hpp"

namespace Ui
{
    ResponseHeaders::ResponseHeaders(QWidget *parent)
        : QWidget(parent)
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();

        ResponseTable *table = new ResponseTable(this);
        model = new QModel::ResponseHeader(this);
        table->setModel(model);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        mainLayout->addWidget(table);

        setLayout(mainLayout);
    }

    void ResponseHeaders::setData(const std::list<Model::Response::Header> &headers)
    {
        model->setHeaders(headers);
    }
} // namespace Ui
