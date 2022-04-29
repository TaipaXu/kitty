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

#include "./requestParams.hpp"
#include <QVBoxLayout>
#include "widgets/paramTable.hpp"
#include "models/param.hpp"
#include "qmodels/param.hpp"

namespace Ui
{
    RequestParams::RequestParams(const std::list<Model::Param *> &params, QWidget *parent)
        : QWidget(parent)
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setContentsMargins(0, 0, 0, 0);

        ParamTable *table = new ParamTable(this);
        model = new QModel::Param(params, this);
        connect(model, &QModel::Param::dataChanged, this, &RequestParams::handleParamsChanged);
        connect(model, &QModel::Param::rowsInserted, this, &RequestParams::handleParamsChanged);
        connect(model, &QModel::Param::rowsMoved, this, &RequestParams::handleParamsChanged);
        connect(model, &QModel::Param::rowsRemoved, this, &RequestParams::handleParamsChanged);
        table->setModel(model);
        mainLayout->addWidget(table);

        setLayout(mainLayout);
    }

    void RequestParams::handleParamsChanged()
    {
        std::list<Model::Param *> items = model->getItems();
        emit changed(items);
    }
} // namespace Ui
