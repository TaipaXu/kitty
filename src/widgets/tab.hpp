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
class QLabel;
class QPushButton;
class QMenu;
QT_END_NAMESPACE

namespace Model
{
    class Api;
}

namespace Ui
{
    class Tab : public QWidget
    {
        Q_OBJECT

    public:
        explicit Tab(Model::Api *api, QWidget *parent = nullptr);
        ~Tab() = default;

        bool contains(const Model::Api *api) const;
        void setActive(bool status);
        bool isModified() const;
        void setModified(bool modified);

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void enterEvent(QEnterEvent *event) override;
        void leaveEvent(QEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;
        void dragEnterEvent(QDragEnterEvent *event) override;
        void dragLeaveEvent(QDragLeaveEvent *event) override;
        void dropEvent(QDropEvent *event) override;
        void contextMenuEvent(QContextMenuEvent *event) override;
        void paintEvent(QPaintEvent *event) override;

    signals:
        void clicked(Model::Api *api);
        void requestClose(Model::Api *api);
        void requestSwap(Model::Api *, Model::Api *toApi);

    private slots:
        void handleCloseButtonClicked();

    public:
        Model::Api *api;
        QLabel *label;
        QPushButton *closeButton;
        QMenu *contextMenu;
        bool isHovered;
        bool isActive;
        bool pressed;
        bool modified;
        QPoint startPos;
        static const QString format;
    };
} // namespace Ui
