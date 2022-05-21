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

#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
class QWidget;
class QLabel;
QT_END_NAMESPACE

namespace Ui
{
    class ResponseSummary : public QWidget
    {
        Q_OBJECT

    public:
        explicit ResponseSummary(QWidget *parent = nullptr);
        ~ResponseSummary();

        void setData(int status, double totalTime, double dnsLookupTime, double tcpHandshakeTime, double sslHandshakeTime, double transferStartTime, double downloadTime);

    private:
        QWidget *mainWidget;
        QLabel *statusLabel;
        QLabel *totalTimeLabel;
        QLabel *dnsLookupTimeLabel;
        QLabel *tcpHandshakeTimeLabel;
        QLabel *sslHandshakeTimeLabel;
        QLabel *transferStartTimeLabel;
        QLabel *downloadTimeLabel;
    };
} // namespace Ui
