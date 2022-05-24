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
class QAbstractButton;
class QRadioButton;
class QLineEdit;
QT_END_NAMESPACE

namespace Persistence
{
    class ApiSettings;
} // namespace Persistence

namespace Ui
{
    class ApiSettings : public QWidget
    {
        Q_OBJECT

    public:
        explicit ApiSettings(Persistence::ApiSettings *settings, QWidget *parent = nullptr);
        ~ApiSettings();

    private slots:
        void handleAutoFollowRedirectsChanged(bool checked);
        void handleEnableSslVerificationChanged(bool checked);
        void handleTimeoutChanged(const QString &text);
        void handlePorxyButtonToggled(QAbstractButton *button, bool checked) const;
        void handleProxyStrChanged(const QString &text);

    private:
        Persistence::ApiSettings *settings;
        QRadioButton *noProxyRadio;
        QRadioButton *proxyRadio;
        QLineEdit *proxyInput;
    };
} // namespace Ui
