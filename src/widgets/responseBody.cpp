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

#include "./responseBody.hpp"
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QPushButton>
#include <QPlainTextEdit>

namespace Ui
{
    ResponseBody::ResponseBody(QWidget *parent)
        : QWidget{parent}
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();

        QHBoxLayout *buttonsLayout = new QHBoxLayout();

        rawButton = new QPushButton(tr("Raw"), this);
        connect(rawButton, &QPushButton::clicked, this, &ResponseBody::handleRawButtonClicked);
        rawButton->setFlat(true);
        rawButton->setCheckable(true);
        rawButton->setChecked(true);
        buttonsLayout->addWidget(rawButton);

        QSpacerItem *horizontalSpacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
        buttonsLayout->addItem(horizontalSpacer);

        QPushButton *exportButton = new QPushButton(tr("Export"), this);
        connect(exportButton, &QPushButton::clicked, this, &ResponseBody::handleExportButtonClicked);
        buttonsLayout->addWidget(exportButton);

        mainLayout->addLayout(buttonsLayout);

        stack = new QStackedWidget(this);

        rawTextEdit = new QPlainTextEdit(this);
        rawTextEdit->setReadOnly(true);
        stack->addWidget(rawTextEdit);

        stack->setCurrentWidget(rawTextEdit);

        mainLayout->addWidget(stack);

        setLayout(mainLayout);
    }

    void ResponseBody::setData(const QString &data)
    {
        this->data = data;
        rawTextEdit->setPlainText(data);
    }

    void ResponseBody::handleRawButtonClicked() const
    {
        rawButton->setChecked(true);
        stack->setCurrentWidget(rawTextEdit);
    }

    void ResponseBody::handleExportButtonClicked() const
    {
    }
} // namespace Ui
