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

#include <QLabel>
#include <QWidget>

namespace Ui
{
    class NavButton : public QLabel
    {
        Q_OBJECT
    public:
        explicit NavButton(QWidget *parent = nullptr);
        void setImage(const QPixmap &image, const QPixmap &activeImage);
        void setStatus(bool status);

    signals:
        void clicked();

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void enterEvent(QEnterEvent *event) override;
        void leaveEvent(QEvent *event) override;

    private:
        QPixmap image;
        QPixmap activeImage;
        QSize padding;
        bool status;
    };
} // namespace Ui
