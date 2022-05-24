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
#include <QButtonGroup>
#include <QRadioButton>
#include <QLineEdit>
#include <QIntValidator>
#include "persistence/apiSettings.hpp"

namespace Ui
{
    ApiSettings::ApiSettings(Persistence::ApiSettings *settings, QWidget *parent)
        : QWidget(parent), settings{settings}
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setAlignment(Qt::AlignTop);

        QHBoxLayout *hLayout = new QHBoxLayout();
        QLabel *autoFollowRedirectsLabel = new QLabel(tr("Auto Follow Redirects"), this);
        QCheckBox *autoFollowRedirectsCheckBox = new QCheckBox(this);
        autoFollowRedirectsCheckBox->setChecked(settings->getAutoFollowRedirects());
        connect(autoFollowRedirectsCheckBox, &QCheckBox::stateChanged, this, &ApiSettings::handleAutoFollowRedirectsChanged);
        QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        hLayout->addWidget(autoFollowRedirectsLabel);
        hLayout->addItem(spacer);
        hLayout->addWidget(autoFollowRedirectsCheckBox);
        mainLayout->addLayout(hLayout);

        hLayout = new QHBoxLayout();
        QLabel *enableSslVerificationLabel = new QLabel(tr("Enable SSL verfication"), this);
        QCheckBox *enableSslVerificationCheckBox = new QCheckBox(this);
        enableSslVerificationCheckBox->setChecked(settings->getEnableSslVerification());
        connect(enableSslVerificationCheckBox, &QCheckBox::stateChanged, this, &ApiSettings::handleEnableSslVerificationChanged);
        spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        hLayout->addWidget(enableSslVerificationLabel);
        hLayout->addItem(spacer);
        hLayout->addWidget(enableSslVerificationCheckBox);
        mainLayout->addLayout(hLayout);

        hLayout = new QHBoxLayout();
        QLabel *timeoutLabel = new QLabel(tr("Timeout / s"), this);
        QLineEdit *timeoutInput = new QLineEdit(this);
        timeoutInput->setFixedWidth(50);
        timeoutInput->setValidator(new QIntValidator(0, 300, this));
        timeoutInput->setText(QString::number(settings->getTimeout()));
        connect(timeoutInput, &QLineEdit::textChanged, this, &ApiSettings::handleTimeoutChanged);
        spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        hLayout->addWidget(timeoutLabel);
        hLayout->addItem(spacer);
        hLayout->addWidget(timeoutInput);
        mainLayout->addLayout(hLayout);

        hLayout = new QHBoxLayout();
        QButtonGroup *proxyButtons = new QButtonGroup(this);
        noProxyRadio = new QRadioButton(tr("No Proxy"), this);
        proxyRadio = new QRadioButton(tr("Proxy"), this);
        // proxyRadio->setChecked(settings->getProxyType() == Persistence::ApiSettings::ProxyType::PROXY);
        // noProxyRadio->setChecked(settings->getProxyType() == Persistence::ApiSettings::ProxyType::NO_PROXY);
        proxyButtons->addButton(noProxyRadio, 0);
        proxyButtons->addButton(proxyRadio, 1);
        hLayout->addWidget(noProxyRadio);
        hLayout->addWidget(proxyRadio);
        spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        hLayout->addItem(spacer);
        proxyInput = new QLineEdit(this);
        mainLayout->addLayout(hLayout);
        Persistence::ApiSettings::ProxyType proxyType = settings->getProxyType();
        if (proxyType == Persistence::ApiSettings::ProxyType::Proxy)
        {
            proxyRadio->setChecked(true);
            proxyInput->setText(settings->getProxyStr());
        }
        else
        {
            noProxyRadio->setChecked(true);
            proxyInput->hide();
        }
        connect(proxyButtons, &QButtonGroup::buttonToggled, this, &ApiSettings::handlePorxyButtonToggled);
        connect(proxyInput, &QLineEdit::textChanged, this, &ApiSettings::handleProxyStrChanged);
        mainLayout->addWidget(proxyInput);

        setLayout(mainLayout);
    }

    ApiSettings::~ApiSettings()
    {
    }

    void ApiSettings::handleAutoFollowRedirectsChanged(bool checked)
    {
        settings->setAutoFollowRedirects(checked);
    }

    void ApiSettings::handleEnableSslVerificationChanged(bool checked)
    {
        settings->setEnableSslVerification(checked);
    }

    void ApiSettings::handleTimeoutChanged(const QString &text)
    {
        settings->setTimeout(text.toInt());
    }

    void ApiSettings::handlePorxyButtonToggled(QAbstractButton *button, bool checked) const
    {
        if (button == noProxyRadio)
        {
            settings->setNoProxy();
            proxyInput->hide();
        }
        else if (button == proxyRadio)
        {
            settings->setProxy(proxyInput->text());
            proxyInput->show();
        }
    }

    void ApiSettings::handleProxyStrChanged(const QString &text)
    {
        settings->setProxy(text);
    }
} // namespace Ui
