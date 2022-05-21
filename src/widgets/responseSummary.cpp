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

#include "./responseSummary.hpp"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>

namespace Ui
{
    ResponseSummary::ResponseSummary(QWidget *parent)
        : QWidget(parent)
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainWidget = new QWidget(this);

        QFormLayout *formLayout = new QFormLayout();
        formLayout->setContentsMargins(0, 0, 0, 0);
        statusLabel = new QLabel("", this);
        formLayout->addRow(tr("Status"), statusLabel);
        totalTimeLabel = new QLabel("", this);
        formLayout->addRow(tr("Total Time"), totalTimeLabel);
        dnsLookupTimeLabel = new QLabel("", this);
        formLayout->addRow(tr("DNS Lookup"), dnsLookupTimeLabel);
        tcpHandshakeTimeLabel = new QLabel("", this);
        formLayout->addRow(tr("TCP Handshake"), tcpHandshakeTimeLabel);
        sslHandshakeTimeLabel = new QLabel("", this);
        formLayout->addRow(tr("SSL Handshake"), sslHandshakeTimeLabel);
        transferStartTimeLabel = new QLabel("", this);
        formLayout->addRow(tr("Transfer Start"), transferStartTimeLabel);
        downloadTimeLabel = new QLabel("", this);
        formLayout->addRow(tr("Download"), downloadTimeLabel);
        mainWidget->setLayout(formLayout);

        mainLayout->addWidget(mainWidget);

        setLayout(mainLayout);

        mainWidget->hide();
    }

    ResponseSummary::~ResponseSummary()
    {
    }

    void ResponseSummary::setData(int status, double totalTime, double dnsLookupTime, double tcpHandshakeTime, double sslHandshakeTime, double transferStartTime, double downloadTime)
    {
        statusLabel->setText(QString::number(status));
        totalTimeLabel->setText(QStringLiteral("%1 ms").arg(totalTime));
        dnsLookupTimeLabel->setText(QStringLiteral("%1 ms").arg(dnsLookupTime));
        tcpHandshakeTimeLabel->setText(QStringLiteral("%1 ms").arg(tcpHandshakeTime));
        sslHandshakeTimeLabel->setText(QStringLiteral("%1 ms").arg(sslHandshakeTime));
        transferStartTimeLabel->setText(QStringLiteral("%1 ms").arg(transferStartTime));
        downloadTimeLabel->setText(QStringLiteral("%1 ms").arg(downloadTime));

        if (status >= 200 && status < 300)
        {
            statusLabel->setStyleSheet("color: green");
        }
        else if (status >= 300 && status < 400)
        {
            statusLabel->setStyleSheet("color: blue");
        }
        else if (status >= 400 && status < 500)
        {
            statusLabel->setStyleSheet("color: orange");
        }
        else if (status >= 500)
        {
            statusLabel->setStyleSheet("color: red");
        }

        if (mainWidget->isHidden())
        {
            mainWidget->show();
        }
    }
} // namespace Ui
