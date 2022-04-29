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

#include "./navbutton.hpp"

namespace Ui
{
    NavButton::NavButton(QWidget *parent)
        : QLabel(parent), padding{QSize(15, 15)}, status{false}
    {
        setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        setCursor(Qt::PointingHandCursor);
    }

    void NavButton::setImage(const QPixmap &image, const QPixmap &activeImage)
    {
        this->image = image;
        this->activeImage = activeImage;
    }

    void NavButton::setStatus(bool status)
    {
        this->status = status;
        if (status)
        {
            setPixmap(activeImage.scaled(size() - padding, Qt::KeepAspectRatio));
        }
        else
        {
            setPixmap(image.scaled(size() - padding, Qt::KeepAspectRatio));
        }
    }

    void NavButton::mousePressEvent(QMouseEvent *)
    {
        emit clicked();
    }

    void NavButton::enterEvent(QEnterEvent *event)
    {
        if (!this->status)
        {
            setPixmap(activeImage.scaled(size() - padding, Qt::KeepAspectRatio));
        }
    }

    void NavButton::leaveEvent(QEvent *event)
    {
        if (!this->status)
        {
            setPixmap(image.scaled(size() - padding, Qt::KeepAspectRatio));
        }
    }
} // namespace Ui
