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
class QMenu;
class QGraphicsOpacityEffect;
class QPropertyAnimation;
QT_END_NAMESPACE

namespace Model
{
    class Project;
} // namespace Model

namespace Ui
{
    class ManagerProject : public QWidget
    {
        Q_OBJECT

    public:
        explicit ManagerProject(Model::Project *project, QWidget *parent = nullptr);
        ~ManagerProject() = default;

        void setActive(bool status);
        void edit(const QString &name, const QString &description) const;

    signals:
        void requestLoad(Model::Project *project) const;
        void requestEdit(Model::Project *project) const;
        void requestExport(Model::Project *project) const;
        void requestDelete(Model::Project *project) const;

    protected:
        void enterEvent(QEnterEvent *event) override;
        void leaveEvent(QEvent *event) override;
        void focusInEvent(QFocusEvent *event) override;
        void focusOutEvent(QFocusEvent *event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;
        void contextMenuEvent(QContextMenuEvent *event) override;

    private slots:
        void handleLoadActionTriggered() const;
        void handleEditActionTriggered() const;
        void handleExportActionTriggered() const;
        void handleDeleteActionTriggered() const;

    private:
        Model::Project *project;
        QLabel *nameLabel;
        QWidget *hover;
        QLabel *descriptionLabel;
        QMenu *contextMenu;
        bool isActive;
        QGraphicsOpacityEffect *hoverOpacity;
        QPropertyAnimation *hoverAnimation;
    };
} // namespace Ui
