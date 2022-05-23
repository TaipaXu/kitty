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

#include "./apiSettings.hpp"
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QTabWidget>
#include <QLabel>
#include <QCheckBox>
#include "persistence/apiSettings.hpp"

namespace Ui
{
    ApiSettings::ApiSettings(Persistence::ApiSettings *settings, QWidget *parent)
        : QWidget(parent), settings{settings}
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setAlignment(Qt::AlignTop);

        QHBoxLayout *hLayout = new QHBoxLayout();

        QLabel *label = new QLabel(tr("Auto Follow Redirects"), this);
        QCheckBox *autoFollowRedirectsCheckBox = new QCheckBox(this);
        QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        autoFollowRedirectsCheckBox->setChecked(settings->getAutoFollowRedirects());
        connect(autoFollowRedirectsCheckBox, &QCheckBox::stateChanged, this, &ApiSettings::handleFollowAutoRedirectsChanged);
        hLayout->addWidget(label);
        hLayout->addItem(spacer);
        hLayout->addWidget(autoFollowRedirectsCheckBox);

        mainLayout->addLayout(hLayout);

        setLayout(mainLayout);
    }

    ApiSettings::~ApiSettings()
    {
    }

    void ApiSettings::handleFollowAutoRedirectsChanged(bool checked)
    {
        settings->setAutoFollowRedirects(checked);
    }
} // namespace Ui
